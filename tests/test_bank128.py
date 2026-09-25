"""Tests for the U03 bank manifest validator (issue #36).

Everything here is SYNTHETIC TEST FIXTURE DATA: a four-bank fake catalog and
a 128-entry fake manifest generated in code, with names like FIXTURE-000 and
hashes derived from fixed strings. None of it is curated content, none of it
was auditioned, and none of it is a claim about any real patch. The real
bank manifest is produced from U02 listening records (issue #35); these
tests only prove the validator accepts a well-formed manifest and that each
negative control fails the check it targets.

Stdlib-only (unittest); zip-free and oracle-free, so it runs in the fast lane.
"""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
TOOL = REPO / "tools" / "validate_bank128.py"
U02_TOOL = REPO / "tools" / "u02_candidates.py"
REAL_CATALOG = REPO / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"


def load_tool(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    spec.loader.exec_module(mod)  # type: ignore[union-attr]
    return mod


vb = load_tool(TOOL, "validate_bank128_test")

FIXTURE_ARCHIVE = {"filename": "FIXTURE.zip", "size_bytes": 1,
                   "sha256": "0" * 64, "entry_count": 4}
FIXTURE_CATALOG_SHA = "f" * 64


def fx_hash(k: int) -> str:
    return hashlib.sha256(f"synthetic-fixture-{k}".encode()).hexdigest()


def fixture_catalog() -> dict:
    """Synthetic catalog: 4 banks x 32 slots, one distinct hash per slot."""
    hashes = [fx_hash(k) for k in range(128)]
    names = [f"FIXTURE-{k:03d}" for k in range(128)]
    entries = []
    for b in range(4):
        w = []
        for slot in range(32):
            k = b * 32 + slot
            w += [slot, k, k, 0]
        entries.append({"p": f"FIXTURE/bank{b}.syx", "s": 4104, "c": "bank",
                        "v": "decoded", "w": w})
    return {"schema": "gf180-dx7.archive-catalog/1",
            "archive": dict(FIXTURE_ARCHIVE),
            "hashes": hashes, "names": names, "entries": entries}


def fx_performance() -> dict:
    ctl = {"range": 50, "assign": ["pitch"]}
    return {"poly_mono": "poly", "pitch_bend_range": 2, "pitch_bend_step": 0,
            "portamento_mode": "retain", "portamento_glissando": False,
            "portamento_time": 0,
            "mod_wheel": dict(ctl), "foot_control": dict(ctl),
            "breath_control": dict(ctl), "aftertouch": dict(ctl)}


def fixture_manifest() -> dict:
    entries = []
    for k in range(128):
        entries.append({
            "index": k,
            "name": f"FIXTURE-{k:03d}",
            "canonical_hash": fx_hash(k),
            "category": vb.CATEGORIES[k % len(vb.CATEGORIES)],
            "source": {"kind": "archive",
                       "authorship": "synthetic test fixture (no author)",
                       "catalog_path": f"FIXTURE/bank{k // 32}.syx",
                       "catalog_slot": k % 32},
            "audition": {"notes": "synthetic fixture: not auditioned",
                         "listening_record": "tests/test_bank128.py",
                         "verdict": "selected"},
            "performance": fx_performance(),
            "distribution": {"status": "restricted",
                             "basis": "synthetic fixture: not distributed"},
        })
    m = {"schema": vb.SCHEMA, "revision": 1,
         "catalog": {"snapshot_sha256": FIXTURE_CATALOG_SHA,
                     "archive": dict(FIXTURE_ARCHIVE)},
         "exclusions": [],
         "entries": entries}
    m["coverage"] = vb.compute_coverage(entries)
    return m


def run(manifest: dict, catalog: dict | None = None):
    return vb.validate_manifest(manifest, catalog or fixture_catalog(),
                                FIXTURE_CATALOG_SHA)


def recount(m: dict) -> dict:
    m["coverage"] = vb.compute_coverage(m["entries"])
    return m


class TestPositive(unittest.TestCase):
    def test_fixture_passes(self):
        problems, stale, info = run(fixture_manifest())
        self.assertEqual(problems, [])
        self.assertEqual(stale, [])
        self.assertEqual(vb.verdict(problems, stale), "PASS")
        self.assertEqual(sum(info["coverage"]["by_category"].values()), 128)

    def test_categories_match_u02_families(self):
        u02 = load_tool(U02_TOOL, "u02_candidates_bank128_test")
        self.assertEqual(vb.CATEGORIES,
                         tuple(f.key for f in u02.FAMILY_SPECS))

    def test_declared_alias_passes(self):
        """Control specificity: the duplicate check accepts a declared alias."""
        m = fixture_manifest()
        e = m["entries"][5]
        e["canonical_hash"] = m["entries"][4]["canonical_hash"]
        e["source"]["catalog_slot"] = 4  # resolves to the same catalog hash
        e["duplicate_of"] = 4
        e["alias_note"] = "same parameters as entry 4, kept under a second" \
                          " performance setup"
        problems, stale, info = run(m)
        self.assertEqual(problems, [])
        self.assertEqual(info["duplicate_groups"][0]["indices"], [4, 5])
        self.assertEqual(info["duplicate_groups"][0]["catalog_alias_names"],
                         ["FIXTURE-004"])


class TestNegativeControls(unittest.TestCase):
    def assertFails(self, m: dict, needle: str):
        problems, stale, _ = run(m)
        self.assertEqual(vb.verdict(problems, stale), "FAIL")
        self.assertTrue(any(needle in p for p in problems),
                        f"{needle!r} not in {problems}")

    def test_identical_hash_without_alias_note_fails(self):
        """Issue #36 named negative control."""
        m = fixture_manifest()
        e = m["entries"][5]
        e["canonical_hash"] = m["entries"][4]["canonical_hash"]
        e["source"]["catalog_slot"] = 4
        self.assertFails(m, "without a declared alias")

    def test_duplicate_of_without_note_fails(self):
        m = fixture_manifest()
        e = m["entries"][5]
        e["canonical_hash"] = m["entries"][4]["canonical_hash"]
        e["source"]["catalog_slot"] = 4
        e["duplicate_of"] = 4
        self.assertFails(m, "without a declared alias")

    def test_false_alias_declaration_fails(self):
        m = fixture_manifest()
        m["entries"][5]["duplicate_of"] = 4
        m["entries"][5]["alias_note"] = "claims alias but hashes differ"
        self.assertFails(m, "false alias declaration")

    def test_unhashable_entry_fails(self):
        m = fixture_manifest()
        m["entries"][7]["canonical_hash"] = "not-a-hash"
        self.assertFails(m, "unhashable")

    def test_unprovenanced_entry_fails(self):
        m = fixture_manifest()
        del m["entries"][7]["source"]
        self.assertFails(m, "unprovenanced")

    def test_source_not_in_catalog_fails(self):
        m = fixture_manifest()
        m["entries"][7]["source"]["catalog_path"] = "FIXTURE/nowhere.syx"
        self.assertFails(m, "does not resolve")

    def test_source_hash_mismatch_fails(self):
        m = fixture_manifest()
        m["entries"][7]["source"]["catalog_slot"] = 8
        self.assertFails(m, "catalog hash at")

    def test_missing_authorship_fails(self):
        m = fixture_manifest()
        m["entries"][7]["source"]["authorship"] = "TBD"
        self.assertFails(m, "authorship")

    def test_unclear_distribution_fails(self):
        m = recount(fixture_manifest())
        m["entries"][7]["distribution"]["status"] = "unknown"
        recount(m)
        self.assertFails(m, "is not established")

    def test_missing_distribution_fails(self):
        m = fixture_manifest()
        del m["entries"][7]["distribution"]
        recount(m)
        self.assertFails(m, "distribution status missing")

    def test_missing_audition_notes_fails(self):
        m = fixture_manifest()
        m["entries"][7]["audition"]["notes"] = ""
        self.assertFails(m, "audition.notes")

    def test_rejected_verdict_fails(self):
        m = fixture_manifest()
        m["entries"][7]["audition"]["verdict"] = "rejected"
        self.assertFails(m, "audition.verdict")

    def test_missing_performance_fails(self):
        m = fixture_manifest()
        del m["entries"][7]["performance"]
        self.assertFails(m, "performance settings missing")

    def test_performance_out_of_range_fails(self):
        m = fixture_manifest()
        m["entries"][7]["performance"]["pitch_bend_range"] = 13
        self.assertFails(m, "pitch_bend_range")

    def test_bad_category_fails(self):
        m = fixture_manifest()
        m["entries"][7]["category"] = "misc"
        recount(m)
        self.assertFails(m, "category 'misc'")

    def test_short_bank_fails(self):
        m = fixture_manifest()
        m["entries"].pop()
        recount(m)
        self.assertFails(m, "exactly 128 entries")

    def test_missing_family_fails(self):
        m = fixture_manifest()
        for e in m["entries"]:
            if e["category"] == "organ":
                e["category"] = "bass"
        recount(m)
        self.assertFails(m, "categories with no entry")

    def test_coverage_mismatch_fails(self):
        m = fixture_manifest()
        m["coverage"]["by_category"]["bass"] += 1
        self.assertFails(m, "coverage mismatch")

    def test_exclusion_without_reason_fails(self):
        m = fixture_manifest()
        m["exclusions"] = [{"name": "WANTED", "reason": ""}]
        self.assertFails(m, "exclusion 0")

    def test_catalog_drift_is_stale(self):
        m = fixture_manifest()
        m["catalog"]["snapshot_sha256"] = "e" * 64
        problems, stale, _ = run(m)
        self.assertEqual(problems, [])
        self.assertEqual(vb.verdict(problems, stale), "STALE")

    def test_fixture_cannot_pass_against_real_catalog(self):
        """Synthetic fixtures never resolve in the committed P02 catalog."""
        if not REAL_CATALOG.is_file():
            self.skipTest("committed catalog absent")
        catalog = json.loads(REAL_CATALOG.read_text(encoding="utf-8"))
        problems, stale, _ = vb.validate_manifest(
            fixture_manifest(), catalog, vb.sha256_file(REAL_CATALOG))
        self.assertEqual(vb.verdict(problems, stale), "FAIL")
        self.assertTrue(any("does not resolve" in p for p in problems))


class TestCli(unittest.TestCase):
    def _run(self, *args):
        return subprocess.run([sys.executable, str(TOOL), *args],
                              capture_output=True, text=True)

    def test_missing_manifest_is_not_run(self):
        with tempfile.TemporaryDirectory() as td:
            r = self._run("--manifest", str(Path(td) / "absent.json"))
        self.assertEqual(r.returncode, 2)
        self.assertIn("NOT_RUN", r.stderr)

    def test_cli_pass_and_fail_with_report(self):
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            cat_path = td / "catalog.json"
            cat_path.write_text(json.dumps(fixture_catalog()))
            m = fixture_manifest()
            m["catalog"]["snapshot_sha256"] = vb.sha256_file(cat_path)
            good = td / "good.json"
            good.write_text(json.dumps(m))
            report = td / "log.json"
            r = self._run("--manifest", str(good), "--catalog", str(cat_path),
                          "--report", str(report))
            self.assertEqual(r.returncode, 0, r.stderr)
            self.assertIn("PASS", r.stdout)
            self.assertEqual(json.loads(report.read_text())["status"], "PASS")

            bad_m = copy.deepcopy(m)
            bad_m["entries"][5]["canonical_hash"] = \
                bad_m["entries"][4]["canonical_hash"]
            bad_m["entries"][5]["source"]["catalog_slot"] = 4
            bad = td / "bad.json"
            bad.write_text(json.dumps(bad_m))
            r = self._run("--manifest", str(bad), "--catalog", str(cat_path),
                          "--report", str(report))
            self.assertEqual(r.returncode, 1)
            self.assertIn("without a declared alias", r.stderr)
            self.assertEqual(json.loads(report.read_text())["status"], "FAIL")


if __name__ == "__main__":
    unittest.main()
