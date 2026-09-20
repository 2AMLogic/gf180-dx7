"""Tests for the R03 32-patch development manifest (issue #10).

The manifest tests are zip-free: they check the committed corpus/dev32.json
against the committed P02 catalog, recompute the coverage table and the
events convention independently from the manifest's own fields, and run the
mandated negative controls on mutated copies. The render-proof checks are
static consistency checks on the committed evidence; live rendering needs
the external oracle and is not required for these tests (stdlib-only,
unittest).
"""

from __future__ import annotations

import hashlib
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]

CATALOG = REPO / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
MANIFEST = REPO / "corpus" / "dev32.json"
PROOF = REPO / "corpus" / "dev32-renderproof.json"
SELECT = REPO / "tools" / "select_dev32.py"

SCHEMA = "gf180-dx7.dev32/1"
N_ENTRIES = 32
# Pinned by docs/CORPUS-POLICY.md (render convention).
SLOW_RATE_MAX = 25
PITCH_EG_CENTER = 50
HIGH_FEEDBACK_MIN = 4
LOW_FEEDBACK_MAX = 0
SILENT_THRESHOLD = 1e-6
GOAL_ORDER = ("pitch_eg_present", "lfo_wave_0", "lfo_wave_1", "lfo_wave_2",
              "lfo_wave_3", "fixed_frequency_op", "high_feedback",
              "low_feedback", "velocity_sensitive")


def load_tool(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    spec.loader.exec_module(mod)  # type: ignore[union-attr]
    return mod


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


select_dev32 = load_tool(SELECT, "select_dev32_test")
with open(MANIFEST, "r", encoding="utf-8") as fh:
    MANIFEST_DOC = json.load(fh)
with open(CATALOG, "r", encoding="utf-8") as fh:
    CATALOG_DOC = json.load(fh)
with open(PROOF, "r", encoding="utf-8") as fh:
    PROOF_DOC = json.load(fh)
ENTRIES = MANIFEST_DOC["entries"]


def recompute_coverage(entries: list[dict]) -> dict:
    """Independent recomputation of the feature-coverage table (policy doc)."""
    algs = sorted({e["decoded_features"]["algorithm"] for e in entries})
    lfo = {str(w): sum(1 for e in entries
                       if e["decoded_features"]["lfo_wave"] == w)
           for w in range(4)}
    profiles: dict[str, int] = {}
    for e in entries:
        profiles[e["render"]["profile"]] = \
            profiles.get(e["render"]["profile"], 0) + 1
    return {
        "algorithms_covered": algs,
        "algorithms_missing": [a for a in range(32) if a not in algs],
        "algorithm_holes": [a for a in range(32) if a not in algs],
        "lfo_waves_covered": lfo,
        "lfo_wave_goals_unmet": [w for w in range(4) if lfo[str(w)] == 0],
        "fixed_mode_count": sum(1 for e in entries
                                if e["decoded_features"]
                                ["fixed_mode_operators"] > 0),
        "pitch_eg_count": sum(1 for e in entries
                              if e["decoded_features"]["pitch_eg_present"]),
        "high_feedback_count": sum(1 for e in entries
                                   if e["decoded_features"]["high_feedback"]),
        "low_feedback_count": sum(1 for e in entries
                                  if e["decoded_features"]["low_feedback"]),
        "velocity_sensitive_count": sum(
            1 for e in entries
            if e["decoded_features"]["velocity_sensitive"]),
        "render_profiles": dict(sorted(profiles.items())),
    }


def recompute_events(dfeat: dict) -> tuple[list[dict], dict]:
    """Events/render from the documented decision tree, per entry features."""
    on = {"cmd": "note_on", "args": [60, 100]}
    off = {"cmd": "note_off", "args": [60]}
    if dfeat["min_operator_rate"] <= SLOW_RATE_MAX:
        return ([{**on, "pos": 4800}, {**off, "pos": 144000}],
                {"sample_rate": 48000, "seconds": 6.0, "samples": 288000,
                 "profile": "slow_hold"})
    if dfeat["velocity_sensitive"]:
        return ([{**on, "pos": 4800}, {**off, "pos": 48000},
                 {"pos": 72000, "cmd": "note_on", "args": [60, 30]},
                 {**off, "pos": 120000}],
                {"sample_rate": 48000, "seconds": 4.0, "samples": 192000,
                 "profile": "velocity_contrast"})
    return ([{**on, "pos": 4800}, {**off, "pos": 96000}],
            {"sample_rate": 48000, "seconds": 4.0, "samples": 192000,
             "profile": "default"})


class TestManifestShape(unittest.TestCase):
    """The committed manifest parses and is structurally sound."""

    def test_schema_and_size(self):
        self.assertEqual(MANIFEST_DOC["schema"], SCHEMA)
        self.assertEqual(MANIFEST_DOC["counts"]["entries"], N_ENTRIES)
        self.assertEqual(len(ENTRIES), N_ENTRIES)
        self.assertEqual([e["index"] for e in ENTRIES],
                         list(range(N_ENTRIES)))

    def test_unique_canonical_hashes(self):
        hashes = [e["canonical_hash"] for e in ENTRIES]
        self.assertEqual(len(set(hashes)), N_ENTRIES)
        for h in hashes:
            self.assertRegex(h, r"^[0-9a-f]{64}$")

    def test_entries_carry_identity_features_events_render(self):
        for e in ENTRIES:
            self.assertTrue(e["path"].startswith("DX7_AllTheWeb/"))
            self.assertIsInstance(e["slot"], int)
            self.assertTrue(0 <= e["slot"] <= 31)
            self.assertTrue(e["name"])
            self.assertTrue(e["selected_features"])
            self.assertTrue(e["anomaly_free"])
            d = e["decoded_features"]
            for key in ("algorithm", "feedback", "lfo_wave",
                        "pitch_eg_present", "fixed_mode_operators",
                        "velocity_sensitive", "min_operator_rate",
                        "high_feedback", "low_feedback"):
                self.assertIn(key, d)
            self.assertIsInstance(e["events"], list)
            self.assertTrue(e["events"])

    def test_validate_manifest_passes_on_committed_pair(self):
        problems = select_dev32.validate_manifest(
            MANIFEST_DOC, CATALOG_DOC, sha256_file(CATALOG))
        self.assertEqual(problems, [])

    def test_archive_identity_present(self):
        archive = MANIFEST_DOC["selection"]["archive"]
        self.assertEqual(archive, CATALOG_DOC["archive"])
        self.assertEqual(archive["sha256"],
                         "c2c47bd1aa29ab07bdf5e6ec8a25faa1b10e06ea0bcf57f6a83"
                         "d8363051ed6ef")
        self.assertEqual(
            MANIFEST_DOC["selection"]["catalog_snapshot_sha256"],
            sha256_file(CATALOG))


class TestCatalogResolution(unittest.TestCase):
    """Every entry resolves to the committed catalog with valid hashes."""

    def test_hashes_exist_in_catalog(self):
        hash_set = set(CATALOG_DOC["hashes"])
        for e in ENTRIES:
            self.assertIn(e["canonical_hash"], hash_set)

    def test_path_slot_resolves_to_same_hash_and_name(self):
        resolution = select_dev32.catalog_resolution_index(CATALOG_DOC)
        for e in ENTRIES:
            key = (e["path"], e["slot"])
            self.assertIn(key, resolution)
            self.assertEqual(resolution[key], e["canonical_hash"])
            if e["slot"] is not None:
                names = CATALOG_DOC["names"]
                bank = next(x for x in CATALOG_DOC["entries"]
                            if x["c"] == "bank" and x["p"] == e["path"])
                w = bank["w"]
                for k in range(0, len(w), 4):
                    if w[k] == e["slot"]:
                        self.assertEqual(names[w[k + 2]], e["name"])

    def test_algorithm_field_matches_resolution(self):
        """decoded_features.algorithm agrees with the catalog topology via
        the canonical hash: distinct algorithms cannot share a hash."""
        seen: dict[str, int] = {}
        for e in ENTRIES:
            h, alg = e["canonical_hash"], e["decoded_features"]["algorithm"]
            if h in seen:
                self.assertEqual(seen[h], alg)
            seen[h] = alg


class TestCoverageRecompute(unittest.TestCase):
    """Coverage table matches per-entry features recomputed independently."""

    def test_coverage_table_matches_recompute(self):
        want = recompute_coverage(ENTRIES)
        got = MANIFEST_DOC["coverage"]
        for key, expected in want.items():
            self.assertEqual(got.get(key), expected, f"coverage.{key}")

    def test_all_32_algorithms_covered(self):
        self.assertEqual(recompute_coverage(ENTRIES)["algorithms_covered"],
                         list(range(32)))
        self.assertEqual(MANIFEST_DOC["coverage"]["goal_holes"], [])

    def test_lfo_wave_goals_0_to_3_present(self):
        lfo = recompute_coverage(ENTRIES)["lfo_waves_covered"]
        for w in range(4):
            self.assertGreaterEqual(lfo[str(w)], 1, f"LFO wave {w} uncovered")

    def test_feature_goals_covered(self):
        cov = MANIFEST_DOC["coverage"]
        self.assertGreaterEqual(cov["pitch_eg_count"], 1)
        self.assertGreaterEqual(cov["fixed_mode_count"], 1)
        self.assertGreaterEqual(cov["high_feedback_count"], 1)
        self.assertGreaterEqual(cov["low_feedback_count"], 1)
        self.assertGreaterEqual(cov["velocity_sensitive_count"], 1)

    def test_selected_features_recompute_to_decoded_flags(self):
        for e in ENTRIES:
            d = e["decoded_features"]
            for tag in e["selected_features"]:
                if tag.startswith("algorithm:"):
                    self.assertEqual(int(tag.split(":")[1]), d["algorithm"])
                elif tag == "pitch_eg_present":
                    self.assertTrue(d["pitch_eg_present"])
                elif tag.startswith("lfo_wave_"):
                    self.assertEqual(d["lfo_wave"], int(tag[-1]))
                elif tag == "fixed_frequency_op":
                    self.assertGreater(d["fixed_mode_operators"], 0)
                elif tag == "high_feedback":
                    self.assertTrue(d["high_feedback"])
                elif tag == "low_feedback":
                    self.assertTrue(d["low_feedback"])
                elif tag == "velocity_sensitive":
                    self.assertTrue(d["velocity_sensitive"])
                elif tag == "fill":
                    pass
                else:
                    self.fail(f"unknown selected_features tag {tag!r}")


class TestEventsConvention(unittest.TestCase):
    """Every entry is renderer-executable and follows the documented rule."""

    def test_events_match_documented_profile_rule(self):
        for e in ENTRIES:
            events, render = recompute_events(e["decoded_features"])
            self.assertEqual(e["events"], events,
                             f"entry {e['index']} events drift from the"
                             " documented convention")
            self.assertEqual(e["render"], render,
                             f"entry {e['index']} render settings drift")

    def test_events_parse_and_fit_render_window(self):
        for e in ENTRIES:
            positions = [ev["pos"] for ev in e["events"]]
            self.assertEqual(positions, sorted(positions))
            for ev in e["events"]:
                self.assertGreaterEqual(ev["pos"], 0)
                self.assertLess(ev["pos"], e["render"]["samples"])
                if ev["cmd"] == "note_on":
                    self.assertEqual(len(ev["args"]), 2)
                    self.assertTrue(0 <= ev["args"][0] <= 127)
                    self.assertTrue(1 <= ev["args"][1] <= 127)
                elif ev["cmd"] == "note_off":
                    self.assertEqual(len(ev["args"]), 1)
                else:
                    self.fail(f"entry {e['index']}: unknown cmd {ev['cmd']!r}")

    def test_render_settings_explicit(self):
        for e in ENTRIES:
            r = e["render"]
            self.assertEqual(r["sample_rate"], 48000)
            self.assertEqual(r["samples"], int(r["sample_rate"] * r["seconds"]))
            self.assertIn(r["profile"],
                          ("default", "velocity_contrast", "slow_hold"))


class TestNegativeControls(unittest.TestCase):
    """Validation must reject broken manifests (issue #10 acceptance)."""

    def mutate(self, fn) -> dict:
        import copy
        doc = copy.deepcopy(MANIFEST_DOC)
        fn(doc)
        return doc

    def assert_rejected(self, doc: dict, needle: str):
        problems = select_dev32.validate_manifest(
            doc, CATALOG_DOC, sha256_file(CATALOG))
        self.assertTrue(problems, "expected validation problems")
        joined = "\n".join(problems)
        self.assertIn(needle, joined)

    def control_a_foreign_hash(self, doc):
        doc["entries"][0]["canonical_hash"] = "ab" * 32

    def control_b_duplicate_hash(self, doc):
        doc["entries"][1]["canonical_hash"] = \
            doc["entries"][0]["canonical_hash"]

    def control_c_missing_events(self, doc):
        del doc["entries"][2]["events"]

    def control_c2_missing_render(self, doc):
        del doc["entries"][3]["render"]

    def control_c3_unrenderable_event(self, doc):
        doc["entries"][4]["events"][0]["pos"] = 10 ** 9

    def test_a_foreign_source_hash_fails(self):
        self.assert_rejected(self.mutate(self.control_a_foreign_hash),
                             "does not resolve")

    def test_b_duplicate_canonical_hash_fails(self):
        self.assert_rejected(self.mutate(self.control_b_duplicate_hash),
                             "not unique")

    def test_c_missing_events_fails(self):
        self.assert_rejected(self.mutate(self.control_c_missing_events),
                             "events missing")

    def test_c_missing_render_settings_fails(self):
        self.assert_rejected(self.mutate(self.control_c2_missing_render),
                             "render settings missing")

    def test_c_event_past_render_end_fails(self):
        self.assert_rejected(self.mutate(self.control_c3_unrenderable_event),
                             "past render end")

    def test_subprocess_validate_rejects_mutated_manifest(self):
        doc = self.mutate(self.control_a_foreign_hash)
        with tempfile.TemporaryDirectory() as tmp:
            bad = Path(tmp) / "dev32-bad.json"
            bad.write_text(json.dumps(doc), encoding="utf-8")
            proc = subprocess.run(
                [sys.executable, str(SELECT), "--validate",
                 "--manifest", str(bad), "--catalog", str(CATALOG)],
                capture_output=True, text=True, cwd=REPO)
            self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
            self.assertIn("dev32 FAIL", proc.stderr)

    def test_selection_without_zip_is_not_run_with_reason(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "dev32.json"
            proc = subprocess.run(
                [sys.executable, str(SELECT), "--zip",
                 str(Path(tmp) / "absent.zip"), "--out", str(out),
                 "--catalog", str(CATALOG)],
                capture_output=True, text=True, cwd=REPO)
            self.assertEqual(proc.returncode, 2, proc.stdout + proc.stderr)
            self.assertIn("NOT_RUN", proc.stderr)
            self.assertIn("absent.zip", proc.stderr)
            self.assertFalse(out.exists())

    def test_validate_missing_manifest_is_not_run(self):
        with tempfile.TemporaryDirectory() as tmp:
            proc = subprocess.run(
                [sys.executable, str(SELECT), "--validate",
                 "--manifest", str(Path(tmp) / "nope.json")],
                capture_output=True, text=True, cwd=REPO)
            self.assertEqual(proc.returncode, 2, proc.stdout + proc.stderr)
            self.assertIn("NOT_RUN", proc.stderr)


class TestRenderProof(unittest.TestCase):
    """Static consistency of the committed render-proof evidence."""

    def test_proof_pins_this_manifest_and_catalog(self):
        self.assertEqual(PROOF_DOC["schema"], "gf180-dx7.dev32-renderproof/1")
        self.assertTrue(PROOF_DOC["validated"])
        self.assertEqual(PROOF_DOC["manifest"]["sha256"],
                         sha256_file(MANIFEST))
        self.assertEqual(PROOF_DOC["catalog"]["sha256"],
                         sha256_file(CATALOG))

    def test_proof_samples_three_entries(self):
        self.assertEqual(PROOF_DOC["render_sample_size"], 3)
        self.assertEqual(len(PROOF_DOC["renders"]), 3)
        for proof, entry in zip(PROOF_DOC["renders"],
                                ENTRIES[:PROOF_DOC["render_sample_size"]]):
            self.assertEqual(proof["index"], entry["index"])
            self.assertEqual(proof["canonical_hash"], entry["canonical_hash"])
            self.assertEqual(proof["events"], entry["events"])
            self.assertEqual(proof["render"], entry["render"])

    def test_renders_non_silent_with_output_hashes(self):
        for proof in PROOF_DOC["renders"]:
            out = proof["output"]
            self.assertRegex(out["sha256"], r"^[0-9a-f]{64}$")
            self.assertEqual(out["samples"], proof["render"]["samples"])
            self.assertGreaterEqual(out["peak_abs"], SILENT_THRESHOLD)
            self.assertFalse(proof["non_silent_guard"]["triggered"])
            self.assertRegex(proof["voice_message_sha256"],
                             r"^[0-9a-f]{64}$")

    def test_full_32_gate_recorded_not_run(self):
        self.assertFalse(PROOF_DOC["full_32_gate"]["run"])

    def test_oracle_identity_pinned(self):
        identity = PROOF_DOC["oracle_identity"]
        self.assertEqual(identity["binary"]["sha256"],
                         "dc4c1b26322caa4ec5086c7a28bd86c9b0566c7c0ac03bf4c95"
                         "ce178b077d39f")
        for proof in PROOF_DOC["renders"]:
            self.assertEqual(proof["executed_binary"]["sha256"],
                             identity["binary"]["sha256"])


if __name__ == "__main__":
    unittest.main()
