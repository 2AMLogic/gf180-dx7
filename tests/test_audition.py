"""Tests for the U01 audition/recall tool (issue #34).

Fast tests (store validation, phrase schema, select against the committed
catalog, recall comparison logic against pre-recorded stores) run everywhere.
The real favorite -> recall round trip needs the pinned external oracle
binary and the pinned archive zip; it skips naming exactly what is missing
and never reports a skipped test as a pass.
"""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
TOOL = REPO / "tools" / "audition.py"
PHRASES_PATH = REPO / "spec" / "audition-phrases-v1.json"
CATALOG_PATH = REPO / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
IDENTITY_PATH = REPO / "reference" / "oracle-identity.json"

with open(IDENTITY_PATH, "r", encoding="utf-8") as fh:
    _IDENTITY = json.load(fh)
ORACLE = Path(_IDENTITY["binary"]["path"])
ORACLE_PIN = _IDENTITY["binary"]["sha256"]

STORE_SCHEMA = "gf180-dx7.favorites/1"
PHRASES_SCHEMA = "gf180-dx7.audition-phrases/1"

FAKE_HASH = hashlib.sha256(b"unit-test-identity").hexdigest()
DIGEST_A = hashlib.sha256(b"render-a").hexdigest()
DIGEST_B = hashlib.sha256(b"render-b").hexdigest()


def load_tool():
    spec = importlib.util.spec_from_file_location("audition", TOOL)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def oracle_ready() -> bool:
    return ORACLE.is_file() and sha256_file(ORACLE) == ORACLE_PIN


def make_store(fav_hash: str = FAKE_HASH, phrase_hashes=None) -> dict:
    if phrase_hashes is None:
        phrase_hashes = {"mid-60-vel100": DIGEST_A,
                         "mid-60-vel32": DIGEST_B}
    return {"schema": STORE_SCHEMA,
            "favorites": {fav_hash: {
                "note": "bright tine, thin bass",
                "added": "2026-09-20T00:00:00Z",
                "phrase_hashes": phrase_hashes,
            }}}


def fake_phrases() -> list[dict]:
    return [{"id": "mid-60-vel100"}, {"id": "mid-60-vel32"}]


# ---------------------------------------------------------------------------
# Phrase set (committed spec, no oracle)


class TestPhraseSpec(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()
        cls.phrases = cls.mod.load_phrases(PHRASES_PATH)
        with open(PHRASES_PATH, "r", encoding="utf-8") as fh:
            cls.raw = json.load(fh)

    def test_schema_and_coverage(self):
        self.assertEqual(self.raw["schema"], PHRASES_SCHEMA)
        self.assertEqual(self.raw["sample_rate"], 48000)
        ids = [p["id"] for p in self.phrases]
        self.assertEqual(len(ids), len(set(ids)), "phrase ids must be unique")
        self.mod.check_phrase_coverage(self.phrases)  # must not raise

    def test_coverage_missing_phrase_fails(self):
        shaved = [p for p in self.phrases if p["id"] != "long-hold-60"]
        with self.assertRaises(self.mod.AuditionFail):
            self.mod.check_phrase_coverage(shaved)

    def test_every_phrase_has_note_on_then_off(self):
        for p in self.phrases:
            note_on_pitch = None
            saw_off = False
            for e in p["events"]:
                if e["cmd"] == "note_on" and note_on_pitch is None:
                    note_on_pitch = e["args"][0]
                elif e["cmd"] == "note_off" and \
                        e["args"][0] == note_on_pitch:
                    saw_off = True
            self.assertTrue(saw_off,
                            f"{p['id']}: note_on has no matching note_off")

    def test_protocol_values_present(self):
        pitches = set()
        vels = set()
        cmds = set()
        for p in self.phrases:
            for e in p["events"]:
                cmds.add(e["cmd"])
                if e["cmd"] == "note_on":
                    pitches.add(e["args"][0])
                    vels.add(e["args"][1])
        # plan section 6: low/mid/high, several velocities, controllers
        self.assertLessEqual({36, 60, 84}, pitches)
        self.assertLessEqual({32, 80, 127}, vels)
        self.assertLessEqual({"note_on", "note_off", "cc", "pb"}, cmds)

    def test_chord_has_three_simultaneous_notes(self):
        chord = self.mod.phrase_by_id(self.phrases, "chord-3note-60-64-67")
        first_pos = chord["events"][0]["pos"]
        ons = [e for e in chord["events"]
               if e["cmd"] == "note_on" and e["pos"] == first_pos]
        self.assertEqual(len(ons), 3)

    def test_controllers_sweep(self):
        mw = self.mod.phrase_by_id(self.phrases, "modwheel-sweep-60")
        cc_vals = [e["args"][1] for e in mw["events"] if e["cmd"] == "cc"]
        self.assertEqual(cc_vals, sorted(cc_vals))
        self.assertEqual(cc_vals[0], 0)
        self.assertEqual(cc_vals[-1], 127)
        pb = self.mod.phrase_by_id(self.phrases, "pitch-bend-60")
        pb_pairs = [tuple(e["args"]) for e in pb["events"] if e["cmd"] == "pb"]
        self.assertIn((0, 64), pb_pairs)   # centered
        self.assertIn((0, 127), pb_pairs)  # max up
        self.assertIn((0, 0), pb_pairs)    # max down

    def test_events_deterministic_text(self):
        p = self.mod.phrase_by_id(self.phrases, "mid-60-vel100")
        text = self.mod.events_file_text(p)
        again = self.mod.events_file_text(p)
        self.assertEqual(text, again)
        self.assertIn("2400 note_on 60 100", text)
        self.assertTrue(text.endswith("\n"))

    def test_validator_rejects_bad_phrase_files(self):
        good = copy.deepcopy(self.raw)
        cases = [
            ("schema", {**good, "schema": "wrong"}),
            ("sample_rate", {**good, "sample_rate": 44100}),
            ("empty", {**good, "phrases": []}),
            ("dup id", {**good, "phrases": [good["phrases"][0],
                                            good["phrases"][0]]}),
            ("bad duration", {**good, "phrases": [
                {**good["phrases"][0], "duration_s": 0}]}),
            ("descending pos", {**good, "phrases": [
                {**good["phrases"][0],
                 "events": ["48000 note_on 60 100", "2400 note_off 60"]}]}),
            ("bad velocity", {**good, "phrases": [
                {**good["phrases"][0],
                 "events": ["2400 note_on 60 0", "48000 note_off 60"]}]}),
            ("unknown cmd", {**good, "phrases": [
                {**good["phrases"][0],
                 "events": ["2400 squelch 60 100"]}]}),
            ("no note_on", {**good, "phrases": [
                {**good["phrases"][0],
                 "events": ["2400 note_off 60"]}]}),
        ]
        for label, data in cases:
            with self.subTest(case=label):
                with self.assertRaises(self.mod.AuditionFail):
                    self.mod.validate_phrases(data)


# ---------------------------------------------------------------------------
# Select against the committed catalog (P02)


class TestSelectAgainstCatalog(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()
        cls.cat = cls.mod.load_catalog(CATALOG_PATH)

    def test_select_hash_finds_standalone_voice(self):
        entry = next(e for e in self.cat["entries"]
                     if e.get("c") == "voice" and e.get("h"))
        got = self.mod.select_instances(self.cat, chash=entry["h"])
        self.assertTrue(got)
        self.assertTrue(any(r["path"] == entry["p"] and r["bank"] is None
                            and r["slot"] is None for r in got))
        for r in got:
            self.assertEqual(set(r), {"path", "bank", "slot", "message",
                                      "name", "canonical_hash"})
            self.assertEqual(r["canonical_hash"], entry["h"])

    def test_select_hash_finds_bank_slot(self):
        entry = next(e for e in self.cat["entries"]
                     if e.get("c") == "bank")
        slot, hidx, nidx, _ = entry["w"][0:4]
        chash = self.cat["hashes"][hidx]
        got = self.mod.select_instances(self.cat, chash=chash)
        self.assertTrue(any(r["bank"] == entry["p"] and r["slot"] == slot
                            and r["name"] == self.cat["names"][nidx]
                            for r in got))

    def test_select_name_is_substring_and_case_insensitive(self):
        name = next(n for n in self.cat["names"] if len(n.strip()) >= 4)
        got = self.mod.select_instances(
            self.cat, name_substr=name.strip()[:4].lower())
        self.assertTrue(got)
        for r in got:
            self.assertIn(name.strip()[:4].lower(), r["name"].lower())

    def test_select_unknown_hash_is_empty(self):
        ghost = hashlib.sha256(b"not-in-catalog").hexdigest()
        self.assertEqual(
            self.mod.select_instances(self.cat, chash=ghost), [])

    def test_pick_instance_prefers_standalone_then_bank(self):
        standalone = {"path": "s.syx", "bank": None, "slot": None,
                      "message": None, "name": "s", "canonical_hash": "h"}
        bank = {"path": "b.syx", "bank": "b.syx", "slot": 3,
                "message": None, "name": "b", "canonical_hash": "h"}
        stream = {"path": "t.syx", "bank": "t.syx", "slot": None,
                  "message": 1, "name": "t", "canonical_hash": "h"}
        pick = self.mod.pick_instance
        self.assertEqual(pick([bank, standalone], "h"), standalone)
        self.assertEqual(pick([stream, bank], "h"), bank)
        self.assertEqual(pick([stream], "h"), stream)

    def test_corrupt_catalog_index_fails_loudly(self):
        bad = copy.deepcopy(self.cat)
        bad["entries"] = [next(e for e in bad["entries"]
                               if e.get("c") == "bank")]
        bad["entries"][0]["w"][1] = 10 ** 9  # impossible hash index
        chash = self.cat["hashes"][bad["entries"][0]["w"][2]]
        with self.assertRaises(self.mod.AuditionFail):
            self.mod.select_instances(bad, chash=chash)


# ---------------------------------------------------------------------------
# Favorites store validation + corruption negative controls (no rendering)


class TestStoreValidation(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()
        cls.phrases = fake_phrases()

    def test_valid_store_passes(self):
        got = self.mod.validate_store(make_store(), self.phrases)
        self.assertIn(FAKE_HASH, got["favorites"])

    def corrupt(self, mutate):
        store = make_store()
        mutate(store)
        with self.assertRaises(self.mod.StoreCorruption):
            self.mod.validate_store(store, self.phrases)

    def test_bad_schema_is_corruption(self):
        self.corrupt(lambda s: s.update(schema="other"))

    def test_short_hash_key_is_corruption(self):
        self.corrupt(lambda s: s["favorites"].update(
            {FAKE_HASH[:16]: s["favorites"][FAKE_HASH]}))

    def test_non_hex_hash_key_is_corruption(self):
        self.corrupt(lambda s: s["favorites"].update(
            {"zz" * 32: s["favorites"][FAKE_HASH]}))

    def test_missing_field_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].pop("note"))

    def test_extra_field_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(
            {"gain": 2.0}))

    def test_empty_note_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(note="   "))

    def test_bad_timestamp_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(
            added="yesterday"))

    def test_empty_phrase_hashes_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(
            phrase_hashes={}))

    def test_unknown_phrase_id_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(
            phrase_hashes={"nonexistent-phrase": DIGEST_A}))

    def test_short_phrase_hash_is_corruption(self):
        self.corrupt(lambda s: s["favorites"][FAKE_HASH].update(
            phrase_hashes={"mid-60-vel32": DIGEST_B[:32]}))

    def test_bad_json_is_corruption(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "favorites.json"
            path.write_text("{not json", encoding="utf-8")
            with self.assertRaises(self.mod.StoreCorruption):
                self.mod.load_store(path, self.phrases)

    def test_missing_store_is_empty(self):
        with tempfile.TemporaryDirectory() as tmp:
            store = self.mod.load_store(Path(tmp) / "absent.json",
                                        self.phrases)
            self.assertEqual(store["favorites"], {})

    def test_save_then_load_round_trip(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "favorites.json"
            self.mod.save_store(path, make_store())
            got = self.mod.load_store(path, self.phrases)
            self.assertEqual(got, make_store())


# ---------------------------------------------------------------------------
# Recall verification logic against pre-recorded stores (no rendering)


class TestRecallLogic(unittest.TestCase):
    """Inject a fake renderer: recall must compare hashes and fail loudly."""

    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()
        cls.phrases = fake_phrases()

    def run_recall(self, store, rendered):
        with tempfile.TemporaryDirectory() as tmp:
            store_path = Path(tmp) / "favorites.json"
            self.mod.save_store(store_path, store)
            args = self.mod.argparse.Namespace(
                hash=FAKE_HASH, zip=None, store=store_path,
                phrases=self.mod.DEFAULT_PHRASES,
                catalog=self.mod.DEFAULT_CATALOG,
                identity=IDENTITY_PATH)
            calls = {}

            def fake_prepare(a):
                calls["hash"] = a.hash
                return ({"path": "fake", "bank": None, "slot": None,
                         "message": None, "name": "fake",
                         "canonical_hash": a.hash},
                        self.phrases, Path(tmp), b"voice", Path("oracle"),
                        {"binary": {"sha256": "0" * 64}})

            def fake_render(oracle_path, identity, voice_bytes, phrases,
                            out_dir):
                calls["phrase_ids"] = [p["id"] for p in phrases]
                return {p["id"]: {"output": {"sha256": rendered[p["id"]]},
                                  "peak_abs": 0.5}
                        for p in phrases}

            orig = (self.mod._prepare_render, self.mod.render_phrases)
            self.mod._prepare_render = fake_prepare
            self.mod.render_phrases = fake_render
            try:
                rc = self.mod.cmd_recall(args)
            finally:
                self.mod._prepare_render, self.mod.render_phrases = orig
        self.assertEqual(calls["hash"], FAKE_HASH)
        self.assertEqual(calls["phrase_ids"],
                         [p["id"] for p in self.phrases])
        return rc

    def test_matching_renders_pass(self):
        rc = self.run_recall(make_store(),
                             {"mid-60-vel100": DIGEST_A,
                              "mid-60-vel32": DIGEST_B})
        self.assertEqual(rc, 0)

    def test_any_mismatch_fails(self):
        rc = self.run_recall(make_store(),
                             {"mid-60-vel100": DIGEST_A,
                              "mid-60-vel32": "9" * 64})
        self.assertEqual(rc, 1)

    def test_all_mismatches_fail(self):
        rc = self.run_recall(make_store(),
                             {"mid-60-vel100": "0" * 64,
                              "mid-60-vel32": "1" * 64})
        self.assertEqual(rc, 1)

    def test_not_favorited_is_not_run(self):
        other = hashlib.sha256(b"other").hexdigest()
        store = make_store(fav_hash=other)
        with tempfile.TemporaryDirectory() as tmp:
            store_path = Path(tmp) / "favorites.json"
            self.mod.save_store(store_path, store)
            args = self.mod.argparse.Namespace(
                hash=FAKE_HASH, zip=None, store=store_path,
                phrases=self.mod.DEFAULT_PHRASES,
                catalog=self.mod.DEFAULT_CATALOG, identity=IDENTITY_PATH)
            with self.assertRaises(self.mod.AuditionNotRun):
                self.mod.cmd_recall(args)

    def test_corrupt_store_fails_before_any_render(self):
        store = make_store()
        store["favorites"][FAKE_HASH]["phrase_hashes"]["mid-60-vel32"] = \
            "short"
        with tempfile.TemporaryDirectory() as tmp:
            store_path = Path(tmp) / "favorites.json"
            self.mod.save_store(store_path, store)
            args = self.mod.argparse.Namespace(
                hash=FAKE_HASH, zip=None, store=store_path,
                phrases=self.mod.DEFAULT_PHRASES,
                catalog=self.mod.DEFAULT_CATALOG, identity=IDENTITY_PATH)
            with self.assertRaises(self.mod.StoreCorruption):
                self.mod.cmd_recall(args)


# ---------------------------------------------------------------------------
# Real favorite -> recall round trip (pinned oracle + pinned zip)


class TestFavoriteRecallRoundTrip(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()

    def skip_reason(self):
        if not ORACLE.is_file():
            return (f"external oracle binary absent at {ORACLE} (pinned in "
                    "reference/oracle-identity.json)")
        if sha256_file(ORACLE) != ORACLE_PIN:
            return (f"oracle binary at {ORACLE} does not match the pinned "
                    f"sha256 {ORACLE_PIN}; rebuild from the pinned source "
                    "commit before running this test")
        try:
            with open(CATALOG_PATH, "r", encoding="utf-8") as fh:
                cat = json.load(fh)
            zip_path = self.mod.resolve_zip(None, cat)
        except (self.mod.AuditionFail, self.mod.AuditionNotRun,
                OSError) as exc:
            return f"pinned archive zip unavailable: {exc}"
            # OSError: e.g. macOS TCC denies content reads of a
            # user-directory zip (EPERM/EACCES) — the negative control
            # cannot run on this box; report NOT_RUN (skip), never
            # ERROR, and never a pass (H08 fast-lane footgun).
        self.zip_path = zip_path
        return None

    def test_favorite_then_recall_then_tamper_fails(self):
        reason = self.skip_reason()
        if reason:
            self.skipTest(reason)
        cat = self.mod.load_catalog(CATALOG_PATH)
        entry = next(e for e in cat["entries"]
                     if e.get("c") == "voice" and e.get("h"))
        chash = entry["h"]
        with tempfile.TemporaryDirectory() as tmp:
            store_path = Path(tmp) / "favorites.json"
            base = ["--catalog", str(CATALOG_PATH),
                    "--phrases", str(PHRASES_PATH),
                    "--store", str(store_path),
                    "--identity", str(IDENTITY_PATH),
                    "--zip", str(self.zip_path)]

            def run(*extra):
                return self.mod.main([*base, *extra])

            with self.subTest(step="favorite"):
                self.assertEqual(
                    run("favorite", "--hash", chash,
                        "--note", "round-trip control note"), 0)
                store = self.mod.load_store(store_path, None)
                self.assertIn(chash, store["favorites"])
                phrase_hashes = store["favorites"][chash]["phrase_hashes"]
                self.assertEqual(len(phrase_hashes), 12)
                for digest in phrase_hashes.values():
                    self.assertRegex(digest, r"^[0-9a-f]{64}$")

            with self.subTest(step="recall"):
                self.assertEqual(run("recall", "--hash", chash), 0)

            with self.subTest(step="negative: altered hash fails loudly"):
                store = self.mod.load_store(store_path, None)
                pid = sorted(store["favorites"][chash]["phrase_hashes"])[0]
                old = store["favorites"][chash]["phrase_hashes"][pid]
                store["favorites"][chash]["phrase_hashes"][pid] = \
                    hashlib.sha256(b"tampered").hexdigest()
                self.assertNotEqual(old, store["favorites"][chash]
                                    ["phrase_hashes"][pid])
                self.mod.save_store(store_path, store)
                self.assertEqual(run("recall", "--hash", chash), 1)

            with self.subTest(step="negative: corrupt store fails loudly"):
                store_path.write_text('{"schema": "gf180-dx7.favorites/1", '
                                      '"favorites": {"oops": 1}}',
                                      encoding="utf-8")
                self.assertEqual(run("recall", "--hash", chash), 1)


if __name__ == "__main__":
    unittest.main()
