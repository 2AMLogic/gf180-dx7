"""Tests for the U02 candidate pre-selection / listening kit (issue #35).

Fast, zip-free tests run everywhere: schema + coverage of the committed
candidates.json (all 8 plan-section-6 families present, no dev32 or
favorite overlap, every entry resolves in the committed catalog at its own
path/slot, kit regenerates byte-identically minus the timestamp line) plus
negative controls (a mutated manifest must fail validation).

Environment-gated tests skip WITH REASON naming exactly what is missing:
the full re-selection determinism check needs the pinned archive zip (the
only clock-free re-run of selection), and the render-kit preflight needs
the pinned oracle binary. A skipped test is never reported as a pass.
"""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import re
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
TOOL = REPO / "tools" / "u02_candidates.py"
CANDIDATES_PATH = REPO / "corpus" / "u02-candidates" / "candidates.json"
KIT_PATH = REPO / "corpus" / "u02-candidates" / "LISTENING-KIT.md"
CATALOG_PATH = REPO / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
DEV32_PATH = REPO / "corpus" / "dev32.json"
FAVORITES_PATH = REPO / "corpus" / "favorites.json"
PHRASES_PATH = REPO / "spec" / "audition-phrases-v1.json"
IDENTITY_PATH = REPO / "reference" / "oracle-identity.json"

with open(CANDIDATES_PATH, "r", encoding="utf-8") as fh:
    CAND = json.load(fh)
with open(CATALOG_PATH, "r", encoding="utf-8") as fh:
    CAT = json.load(fh)
with open(IDENTITY_PATH, "r", encoding="utf-8") as fh:
    _IDENTITY = json.load(fh)
ORACLE = Path(_IDENTITY["binary"]["path"])
ORACLE_PIN = _IDENTITY["binary"]["sha256"]
ZIP_NAME = CAT["archive"]["filename"]
ZIP_DEFAULT = Path.home() / "Downloads" / ZIP_NAME
ZIP_ENV = "DX7_ALLTHEWEB_ZIP"


def load_tool():
    spec = importlib.util.spec_from_file_location("u02_candidates", TOOL)
    mod = importlib.util.module_from_spec(spec)
    sys.modules["u02_candidates"] = mod
    spec.loader.exec_module(mod)
    return mod


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def zip_ready() -> tuple[bool, str]:
    paths = []
    import os
    if os.environ.get(ZIP_ENV):
        paths.append(Path(os.environ[ZIP_ENV]).expanduser())
    paths.append(ZIP_DEFAULT)
    for p in paths:
        if p.is_file():
            if sha256_file(p) == CAT["archive"]["sha256"]:
                return True, str(p)
            return False, (f"{p} present but its sha256 does not match the "
                           f"catalog archive pin")
    return False, (f"pinned archive zip {ZIP_NAME} not found (tried: "
                   f"{', '.join(str(p) for p in paths)}); selection must "
                   f"decode voice parameters from the archive")


def oracle_ready() -> tuple[bool, str]:
    if ORACLE.is_file() and sha256_file(ORACLE) == ORACLE_PIN:
        return True, str(ORACLE)
    return False, (f"pinned oracle binary missing or drifted at {ORACLE} "
                   f"(pin sha256 {ORACLE_PIN[:16]}…)")


def excluded_hashes() -> set[str]:
    out = set()
    with open(DEV32_PATH, "r", encoding="utf-8") as fh:
        out |= {e["canonical_hash"] for e in json.load(fh)["entries"]}
    with open(FAVORITES_PATH, "r", encoding="utf-8") as fh:
        out |= set(json.load(fh)["favorites"].keys())
    return out


def catalog_resolution(candidates_mod) -> dict:
    return candidates_mod.resolution_index(CAT)


# ---------------------------------------------------------------------------
# Committed manifest: schema + coverage (fast, zip-free)


class TestCandidatesManifest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()
        cls.cands = CAND["candidates"]

    def test_schema_and_counts(self):
        self.assertEqual(CAND["schema"], "gf180-dx7.u02-candidates/1")
        self.assertEqual(CAND["counts"]["candidates"], len(self.cands))
        self.assertEqual(CAND["counts"]["candidates"], 24)
        self.assertEqual(CAND["counts"]["families"], 8)
        self.assertEqual(CAND["counts"]["per_family"], 3)

    def test_all_eight_families_present_three_each(self):
        per = {}
        for c in self.cands:
            per[c["family"]] = per.get(c["family"], 0) + 1
        self.assertEqual(sorted(per), sorted(self.mod.FAMILY_ORDER))
        for fam, n in per.items():
            self.assertEqual(n, 3, f"family {fam} has {n} candidates")
        self.assertEqual(CAND["coverage"]["families_missing"], [])

    def test_hashes_unique_hex_and_excluded_sets(self):
        hashes = [c["canonical_hash"] for c in self.cands]
        self.assertEqual(len(hashes), len(set(hashes)), "duplicate hashes")
        banned = excluded_hashes()
        for c in self.cands:
            self.assertTrue(re.fullmatch(r"[0-9a-f]{64}",
                                         c["canonical_hash"]))
            self.assertNotIn(c["canonical_hash"], banned,
                             f"{c['canonical_hash'][:12]}… must be excluded "
                             f"(dev32/favorite)")

    def test_every_entry_resolves_in_catalog_with_provenance(self):
        res = catalog_resolution(self.mod)
        for c in self.cands:
            key = (c["catalog"]["path"], c["catalog"]["slot"])
            self.assertIn(key, res,
                          f"candidate {c['index']}: {key!r} not in catalog")
            self.assertEqual(res[key][0], c["canonical_hash"],
                             f"candidate {c['index']}: hash mismatch at "
                             f"{key!r}")
            self.assertTrue(c["name"].strip(), "empty display name")
            for field in ("heuristics", "voice_summary", "render_dir"):
                self.assertIn(field, c)

    def test_why_shortlist_present_and_render_dir_shape(self):
        for c in self.cands:
            self.assertTrue(c["heuristics"]["matched"],
                            "why-shortlist reasons missing")
            self.assertIsInstance(c["heuristics"]["score"], int)
            self.assertEqual(
                c["render_dir"],
                "corpus/u02-candidates/renders/"
                f"{c['canonical_hash'][:12]}-{self.mod.slugify(c['name'])}")

    def test_coverage_summary_matches_entries(self):
        cov = CAND["coverage"]["families"]
        for fam in self.mod.FAMILY_ORDER:
            self.assertIn(fam, cov)
            self.assertEqual(cov[fam]["selected"], 3)
            self.assertFalse(cov[fam]["hole"])
        self.assertEqual(
            CAND["selection"]["catalog"]["snapshot_sha256"],
            sha256_file(CATALOG_PATH),
            "catalog snapshot pin drifted; candidates.json is STALE")

    def test_heuristic_spec_is_self_describing(self):
        heur = CAND["selection"]["heuristics"]
        for spec in self.mod.FAMILY_SPECS:
            self.assertEqual(heur[spec.key]["hard"],
                             [t for t, _ in spec.hard])
            self.assertEqual([s["desc"] for s in heur[spec.key]["scored"]],
                             [t for t, _, _ in spec.items])


# ---------------------------------------------------------------------------
# Kit regeneration (fast: pure function of the committed manifest)


class TestKitRegeneration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()

    def test_kit_matches_deterministic_regeneration(self):
        committed = KIT_PATH.read_text(encoding="utf-8")
        regenerated = self.mod.emit_kit(CAND)
        strip = self.mod._strip_timestamps
        self.assertEqual(strip(committed), strip(regenerated),
                         "committed LISTENING-KIT.md does not match a "
                         "regeneration from candidates.json (minus "
                         "timestamp lines)")

    def test_kit_has_blank_verdict_columns_and_protocol(self):
        text = KIT_PATH.read_text(encoding="utf-8")
        self.assertIn("render-kit", text)
        self.assertIn("audition.py favorite", text)
        self.assertIn("Heuristics select; humans decide", text)
        for label in ("EPs/tines", "basses", "bells/mallets", "organs",
                      "brass/reeds", "plucks", "pads/strings",
                      "percussion/textures"):
            self.assertIn(label, text)
        # every candidate row leaves the verdict columns blank
        rows = [ln for ln in text.splitlines()
                if ln.startswith("| ") and "`corpus/u02-candidates/renders/"
                in ln]
        self.assertEqual(len(rows), 24)
        for row in rows:
            self.assertTrue(row.rstrip().endswith("|  |  |  |  |  |"),
                            f"verdict columns not blank: {row[:60]}…")

    def test_kit_timestamp_line_is_the_only_strippable_line(self):
        text = KIT_PATH.read_text(encoding="utf-8").splitlines()
        stamps = [ln for ln in text
                  if ln.startswith(self.mod.KIT_TIMESTAMP_PREFIX)]
        self.assertEqual(len(stamps), 1)
        self.assertTrue(re.fullmatch(
            r"Generated: \d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z  ", stamps[0]))


# ---------------------------------------------------------------------------
# validate subcommand + negative controls


class TestValidationAndControls(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()

    def test_validate_subcommand_passes(self):
        import subprocess
        proc = subprocess.run(
            ["python3", str(TOOL), "validate"],
            capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0, proc.stderr)

    def test_mutated_entry_fails_validation(self):
        res = catalog_resolution(self.mod)
        snapshot_sha = sha256_file(CATALOG_PATH)
        # control 1: point an entry at a slot that resolves elsewhere
        bad = copy.deepcopy(CAND)
        some = bad["candidates"][0]
        for (path, slot), (h, _n) in res.items():
            if slot is not None and h != some["canonical_hash"]:
                some["catalog"] = {"path": path, "slot": slot}
                break
        problems = self.mod.validate_candidates(bad, CAT, snapshot_sha)
        self.assertTrue(any("does not resolve" in p or "hash mismatch"
                            in p.lower() or "!=" in p for p in problems),
                        problems)
        # control 2: a dev32 hash must be rejected
        with open(DEV32_PATH, "r", encoding="utf-8") as fh:
            dev_hash = json.load(fh)["entries"][0]["canonical_hash"]
        bad2 = copy.deepcopy(CAND)
        bad2["candidates"][0]["canonical_hash"] = dev_hash
        problems = self.mod.validate_candidates(bad2, CAT, snapshot_sha)
        self.assertTrue(any("dev32" in p for p in problems), problems)
        # control 3: dropping a family must break coverage
        bad3 = copy.deepcopy(CAND)
        bad3["candidates"] = [c for c in bad3["candidates"]
                              if c["family"] != "organ"]
        problems = self.mod.validate_candidates(bad3, CAT, snapshot_sha)
        self.assertTrue(any("organ" in p for p in problems), problems)
        # control 4: catalog drift must mark the manifest STALE
        problems = self.mod.validate_candidates(CAND, CAT, "0" * 64)
        self.assertTrue(any("STALE" in p for p in problems), problems)

    def test_kit_mismatch_is_detected(self):
        tampered = copy.deepcopy(CAND)
        tampered["candidates"][0]["name"] = "DRIFTED NAME"
        self.assertIsNotNone(
            self.mod.check_kit_matches(tampered, KIT_PATH))


# ---------------------------------------------------------------------------
# Environment-gated: full determinism re-selection (zip) and render-kit
# preflight (oracle). Skip WITH REASON; never counted as a pass.


class TestEnvironmentGated(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = load_tool()

    def test_full_reselection_is_deterministic(self):
        ok, why = zip_ready()
        if not ok:
            self.skipTest(why)
        cand = self.mod.select_candidates(CAT, Path(why))
        strip = self.mod._strip_timestamps
        a = strip(json.dumps(cand, indent=2, ensure_ascii=False) + "\n")
        b = strip(json.dumps(CAND, indent=2, ensure_ascii=False) + "\n")
        self.assertEqual(a, b, "re-selection differs from the committed "
                               "manifest (minus timestamps)")

    def test_render_kit_preflight_verifies_pinned_oracle(self):
        ok, why = oracle_ready()
        if not ok:
            self.skipTest(why)
        self.mod.preflight_oracle()  # must not raise


if __name__ == "__main__":
    unittest.main()
