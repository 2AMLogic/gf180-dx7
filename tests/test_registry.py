"""Tests for the R06 directed compatibility registry (issue #13).

Covers: registry schema validity; coverage validation against the P01
canonical field list (reflection over the Voice/Operator dataclasses);
live negative controls (dropping an algorithm's or a field's cases must
fail coverage with the hole named); P01 construct+decode round trips for
every synthetic patch; oracle-format event parsing for every case; short
subset discipline; and the committed smoke evidence's oracle-identity pin.

Fast and oracle-independent except where noted: the smoke-evidence tests
read the committed evidence file and, when the pinned oracle binary is
locally present, may re-run the smoke (they never report a skipped run as
a pass).
"""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import subprocess
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import sysex  # noqa: E402
import registry_coverage  # noqa: E402

REGISTRY_PATH = REPO / "spec" / "compatibility-registry-v1.json"
SMOKE_PATH = REPO / "reference" / "evidence" / "registry-smoke.json"
IDENTITY_PATH = REPO / "reference" / "oracle-identity.json"
COVERAGE_TOOL = REPO / "tools" / "registry_coverage.py"
SMOKE_TOOL = REPO / "tools" / "registry_smoke.py"
CHECK_TOOL = REPO / "tools" / "check_registry.py"

with open(REGISTRY_PATH, "r", encoding="utf-8") as fh:
    REG = json.load(fh)
with open(IDENTITY_PATH, "r", encoding="utf-8") as fh:
    IDENTITY = json.load(fh)

CASES = {c["id"]: c for c in REG["cases"]}
SCHEMA = "gf180-dx7.compatibility-registry/1"


def load_tool(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def oracle_ready() -> bool:
    binary = Path(IDENTITY["binary"]["path"])
    return binary.is_file() and \
        sha256_file(binary) == IDENTITY["binary"]["sha256"]


# ---------------------------------------------------------------------------
# Schema validity


class TestRegistrySchema(unittest.TestCase):
    def test_schema_and_top_level(self):
        self.assertEqual(REG["schema"], SCHEMA)
        self.assertEqual(REG["registry_version"], 1)
        self.assertEqual(REG["sample_rate"], 48000)
        for key in ("synthetic_base", "holdout", "fields", "algorithms",
                    "event_interactions", "boundary", "performance_controls",
                    "cases", "short_subset"):
            self.assertIn(key, REG, f"registry missing {key}")

    def test_unique_case_ids(self):
        ids = [c["id"] for c in REG["cases"]]
        self.assertEqual(len(ids), len(set(ids)), "duplicate case ids")
        self.assertEqual(len(ids), len(CASES))

    def test_every_reference_resolves(self):
        ids = set(CASES)
        for row in REG["fields"]:
            for cid in row.get("directed_cases", []) + \
                    row.get("boundary_cases", []):
                self.assertIn(cid, ids,
                              f"field {row['field']} references unknown {cid}")
        for row in REG["algorithms"]:
            for cid in row["cases"]:
                self.assertIn(cid, ids)
        for group, key in (("event_interactions", "interaction"),
                           ("boundary", "boundary")):
            for row in REG[group]:
                for cid in row["cases"]:
                    self.assertIn(cid, ids)
        for row in REG["performance_controls"]:
            for cid in row["cases"]:
                self.assertIn(cid, ids)
        for cid in REG["short_subset"]:
            self.assertIn(cid, ids)

    def test_case_structure(self):
        for c in REG["cases"]:
            cid = c["id"]
            self.assertIn(c["patch"]["selector"], ("synthetic", "corpus"),
                          f"{cid}: bad selector")
            if c["patch"]["selector"] == "corpus":
                digest = c["patch"].get("canonical_hash", "")
                self.assertEqual(len(digest), 64, f"{cid}: corpus hash length")
            self.assertIsInstance(c.get("expect"), dict, f"{cid}: expect")
            self.assertTrue(c["expect"].get("property"), f"{cid}: property")
            self.assertTrue(c["expect"].get("description"), f"{cid}: description")
            if c.get("blocked"):
                self.assertTrue(c.get("blocked_reason"),
                                f"{cid}: blocked needs a reason")
                self.assertEqual(c["events"], [], f"{cid}: blocked has events")
            else:
                self.assertTrue(c["events"], f"{cid}: no events")
                self.assertTrue(0 < c["render_seconds"] <= 30,
                                f"{cid}: render_seconds")

    def test_blocked_cases_have_no_events_and_reason(self):
        blocked = [c for c in REG["cases"] if c.get("blocked")]
        self.assertTrue(blocked, "negative control: blocked cases must exist")
        for c in blocked:
            self.assertEqual(c["events"], [])
            self.assertIn("oracle", c["blocked_reason"].lower())

    def test_holdout_unsealed_until_thresholds_frozen(self):
        holdout = REG["holdout"]
        self.assertIsInstance(holdout["sealed"], bool)
        if holdout["sealed"] is False:
            self.assertEqual(holdout["challenge_subset"], [],
                             "unsealed holdout must have an empty challenge "
                             "subset")
        self.assertTrue(holdout.get("reason"))


# ---------------------------------------------------------------------------
# Coverage against the P01 reflection


class TestCoverage(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = registry_coverage.validate_registry(copy.deepcopy(REG))

    def test_field_list_matches_p01_reflection(self):
        reflected = registry_coverage.reflected_fields()
        rows = {r["field"]: r for r in REG["fields"]}
        self.assertEqual(set(rows), {name for name, _ in reflected},
                         "registry fields must equal the P01 supported list")
        for name, kind in reflected:
            self.assertEqual(rows[name]["kind"], kind, f"{name}: kind")

    def test_every_field_has_a_case(self):
        ids = set(CASES)
        blocked = {c["id"] for c in REG["cases"] if c.get("blocked")}
        for row in REG["fields"]:
            usable = [cid for cid in
                      row.get("directed_cases", []) +
                      row.get("boundary_cases", [])
                      if cid in ids and cid not in blocked]
            self.assertTrue(usable, f"field {row['field']} has no executable case")

    def test_all_32_algorithms_with_feedback_cases(self):
        rows = {r["algorithm"]: r for r in REG["algorithms"]}
        self.assertEqual(sorted(rows), list(range(32)))
        for alg in (4, 6):
            feedback_values = set()
            for cid in rows[alg]["cases"]:
                fb = CASES[cid]["patch"].get("overrides", {}).get("feedback")
                if fb is not None:
                    feedback_values.add(fb)
            self.assertTrue({0, 3, 7} <= feedback_values,
                            f"algorithm {alg}: feedback variants {feedback_values}")

    def test_no_holes(self):
        self.assertEqual(self.report["holes"], [],
                         f"coverage holes: {self.report['holes']}")

    def test_required_interactions_present(self):
        interactions = {r["interaction"] for r in REG["event_interactions"]}
        for required in ("repeated_notes", "retrigger", "sustain", "stealing",
                         "patch_change_while_held", "all_sound_off",
                         "pitch_bend_range", "modwheel_sweep", "aftertouch"):
            self.assertIn(required, interactions)

    def test_required_boundaries_present(self):
        boundaries = {r["boundary"] for r in REG["boundary"]}
        for required in ("ratio_fixed_switch", "coarse_zero", "detune_extremes",
                         "lfo_waveforms", "slow_rates", "fast_rates",
                         "rising_envelope_segments", "delayed_attack",
                         "nonzero_final_level", "pitch_eg_up_down",
                         "max_voice_summation", "keyboard_scaling_curves",
                         "velocity_sensitivity"):
            self.assertIn(required, boundaries)

    def test_performance_controls_track_dec006(self):
        controls = {r["control"]: r["status"]
                    for r in REG["performance_controls"]}
        for control in ("note_on_off", "velocity", "sustain", "pitch_bend",
                        "modwheel", "aftertouch", "volume", "all_sound_off"):
            self.assertIn(control, controls)


# ---------------------------------------------------------------------------
# Negative controls (the check must have resolution)


class TestNegativeControls(unittest.TestCase):
    def fails_or_holes(self, mutated: dict):
        """Validate a mutated registry; return a failure description or the
        hole list."""
        try:
            return registry_coverage.validate_registry(mutated)["holes"]
        except registry_coverage.ValidationFailure as exc:
            return str(exc)

    def test_dropping_algorithm_4_cases_fails(self):
        mutated = copy.deepcopy(REG)
        row = next(r for r in mutated["algorithms"] if r["algorithm"] == 4)
        row["cases"] = []
        result = self.fails_or_holes(mutated)
        self.assertTrue(result, "dropping algorithm 4's cases must fail coverage")
        text = json.dumps(result)
        self.assertIn("4", text)

    def test_dropping_a_field_cases_fails_naming_the_field(self):
        mutated = copy.deepcopy(REG)
        row = next(r for r in mutated["fields"] if r["field"] == "osc_detune")
        row["directed_cases"] = []
        row["boundary_cases"] = []
        result = self.fails_or_holes(mutated)
        self.assertTrue(result, "dropping osc_detune's cases must fail coverage")
        text = json.dumps(result)
        self.assertIn("osc_detune", text)

    def test_unknown_field_name_fails(self):
        mutated = copy.deepcopy(REG)
        mutated["fields"].append({"field": "not_a_real_field", "kind": "op",
                                  "directed_cases": ["dir-base"],
                                  "boundary_cases": []})
        with self.assertRaises(registry_coverage.ValidationFailure):
            registry_coverage.validate_registry(mutated)

    def test_unknown_override_field_fails(self):
        mutated = copy.deepcopy(REG)
        CASES_BY_ID = {c["id"]: c for c in mutated["cases"]}
        CASES_BY_ID["bd-detune0"]["patch"]["overrides"]["nope"] = 1
        with self.assertRaises(registry_coverage.ValidationFailure):
            registry_coverage.validate_registry(mutated)

    def test_empty_short_subset_fails(self):
        mutated = copy.deepcopy(REG)
        mutated["short_subset"] = []
        with self.assertRaises(registry_coverage.ValidationFailure):
            registry_coverage.validate_registry(mutated)

    def test_selftest_negative_mode_passes(self):
        proc = subprocess.run([sys.executable, str(COVERAGE_TOOL),
                               "--selftest-negative"],
                              capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0,
                         f"selftest-negative failed: {proc.stdout}{proc.stderr}")
        payload = json.loads(proc.stdout)
        self.assertEqual(payload["selftest_negative"], "PASS")
        for mutation in payload["mutations"]:
            self.assertTrue(mutation["detected"],
                            f"mutation not detected: {mutation}")


# ---------------------------------------------------------------------------
# Short subset discipline


class TestShortSubset(unittest.TestCase):
    def test_short_subset_nonempty_and_within_cases(self):
        short = REG["short_subset"]
        self.assertTrue(short)
        ids = set(CASES)
        self.assertTrue(set(short) <= ids)

    def test_short_subset_has_no_blocked_or_corpus_cases(self):
        for cid in REG["short_subset"]:
            c = CASES[cid]
            self.assertFalse(c.get("blocked"), f"{cid} blocked but short")
            self.assertEqual(c["patch"]["selector"], "synthetic",
                             f"{cid} corpus but short")

    def test_short_subset_hash_partners_present(self):
        short = set(REG["short_subset"])
        prefixes = ("distinct_hash_vs:", "identical_hash_vs:")
        for cid in REG["short_subset"]:
            expect = CASES[cid]["expect"]
            for pred in [expect["property"]] + list(expect.get("also", [])):
                if pred.startswith(prefixes):
                    ref = pred.split(":", 1)[1]
                    self.assertIn(ref, short,
                                  f"{cid} compares against {ref} not in subset")

    def test_fingerprints_stable_and_distinct(self):
        fp = registry_coverage.input_fingerprint
        base = fp(CASES["dir-base"])
        self.assertEqual(base, fp(CASES["dir-base"]), "fingerprint unstable")
        self.assertNotEqual(base, fp(CASES["ev-repeated-note"]),
                            "different inputs must not share a fingerprint")
        mutated = copy.deepcopy(CASES["dir-base"])
        mutated["patch"]["overrides"]["feedback"] = 5
        self.assertNotEqual(base, fp(mutated),
                            "changing covered inputs must change the "
                            "fingerprint (STALE detection)")


# ---------------------------------------------------------------------------
# P01 construct + decode round trips and oracle-format events


def load_smoke_tool():
    return load_tool(SMOKE_TOOL, "registry_smoke")


class TestSyntheticPatches(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.smoke = load_smoke_tool()

    def test_every_synthetic_override_applies_cleanly(self):
        covered_fields = {}
        for c in REG["cases"]:
            if c["patch"]["selector"] != "synthetic":
                continue
            cid = c["id"]
            voice = self.smoke.build_voice(REG, c)
            encoded = sysex.encode_voice(voice)
            decoded = sysex.decode_voice(encoded)
            self.assertEqual(decoded.anomalies, [],
                             f"{cid}: anomalies {decoded.anomalies}")
            for key, val in c["patch"].get("overrides", {}).items():
                target, sep, field = key.partition(".")
                if sep and target == "op":
                    for op in decoded.operators:
                        self.assertEqual(getattr(op, field), val,
                                         f"{cid}: {key}")
                        covered_fields.setdefault(field, set()).add(cid)
                elif sep:
                    op = decoded.operators[int(target[2:]) - 1]
                    self.assertEqual(getattr(op, field), val, f"{cid}: {key}")
                    covered_fields.setdefault(field, set()).add(cid)
                else:
                    self.assertEqual(getattr(decoded, key), val,
                                     f"{cid}: {key}")
                    covered_fields.setdefault(key, set()).add(cid)

    def test_name_override_lands_in_name_bytes(self):
        for cid in ("codec-name-a", "codec-name-b"):
            voice = self.smoke.build_voice(REG, CASES[cid])
            self.assertTrue(voice.name_bytes, f"{cid}: name bytes empty")
            self.assertLessEqual(len(voice.name_bytes), 10)


class TestEventFormat(unittest.TestCase):
    def test_every_case_events_parse_in_oracle_format(self):
        for c in REG["cases"]:
            if c.get("blocked"):
                continue
            last_pos = -1
            for line in c["events"]:
                registry_coverage.validate_event_line(c["id"], line)
                pos = int(line.split()[0])
                self.assertGreaterEqual(pos, last_pos,
                                        f"{c['id']}: positions must be "
                                        "non-decreasing")
                last_pos = pos

    def test_blocked_and_corpus_cases_marked(self):
        corpus = [c for c in REG["cases"]
                  if c["patch"]["selector"] == "corpus"]
        self.assertTrue(corpus, "corpus-reuse selector must be exercised")
        for c in corpus:
            self.assertTrue(c["expect"]["property"].startswith("manual:"))


# ---------------------------------------------------------------------------
# Committed smoke evidence


class TestSmokeEvidence(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.smoke_available = SMOKE_PATH.is_file()
        if cls.smoke_available:
            with open(SMOKE_PATH, "r", encoding="utf-8") as fh:
                cls.evidence = json.load(fh)

    def test_evidence_exists(self):
        self.assertTrue(self.smoke_available,
                        "reference/evidence/registry-smoke.json must be "
                        "committed (run tools/registry_smoke.py)")

    @unittest.skipUnless(SMOKE_PATH.is_file(), "smoke evidence not committed")
    def test_evidence_matches_registry_and_pin(self):
        ev = self.evidence
        self.assertEqual(ev["schema"], "gf180-dx7.registry-smoke/1")
        self.assertEqual(ev["registry_sha256"],
                         hashlib.sha256(REGISTRY_PATH.read_bytes()).hexdigest(),
                         "smoke evidence is STALE: registry bytes changed")
        self.assertTrue(ev["oracle"]["observed_sha256_match"])
        self.assertEqual(ev["oracle"]["pinned_sha256"],
                         IDENTITY["binary"]["sha256"])
        rows = {r["id"]: r for r in ev["cases"]}
        for cid in REG["short_subset"]:
            self.assertIn(cid, rows, f"short-subset case {cid} not in evidence")

    @unittest.skipUnless(SMOKE_PATH.is_file(), "smoke evidence not committed")
    def test_no_machine_failures_and_manual_rows_explicit(self):
        summary = self.evidence["summary"]
        self.assertEqual(summary["n_fail"], 0,
                         "committed evidence must not carry machine failures; "
                         "re-run the smoke and resolve")
        manual_ids = {r["id"] for r in self.evidence["cases"]
                      if r.get("manual") and r["verdict"] != "BLOCKED"}
        for cid in manual_ids:
            row = next(r for r in self.evidence["cases"] if r["id"] == cid)
            if row["verdict"] == "NO_VERDICT":
                self.assertTrue(row["property"].startswith("manual:"),
                                f"{cid}: manual verdict without manual property")
        for r in self.evidence["cases"]:
            self.assertIn(r["verdict"],
                          ("PASS", "FAIL", "NOT_RUN", "BLOCKED", "NO_VERDICT",
                           "STALE"),
                            f"{r['id']}: bad verdict {r['verdict']}")

    def test_smoke_rerun_matches_committed_hashes_when_oracle_present(self):
        if not oracle_ready():
            self.skipTest("pinned oracle binary not present locally")
        rows_before = {r["id"]: (r.get("pcm_sha256"), r["verdict"])
                       for r in self.evidence["cases"]}
        proc = subprocess.run([sys.executable, str(SMOKE_TOOL)],
                              capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0,
                         f"smoke re-run failed: {proc.stdout}{proc.stderr}")
        self.assertEqual(SMOKE_PATH.read_bytes(), SMOKE_PATH.read_bytes())
        with open(SMOKE_PATH, "r", encoding="utf-8") as fh:
            fresh = json.load(fh)
        rows_after = {r["id"]: (r.get("pcm_sha256"), r["verdict"])
                      for r in fresh["cases"]}
        self.assertEqual(rows_before, rows_after,
                         "fresh smoke disagrees with committed evidence")
        self.assertEqual(fresh, self.evidence,
                         "smoke re-run is not byte-identical (committed "
                         "evidence must be exactly reproducible)")


class TestCheckWrapper(unittest.TestCase):
    def test_check_registry_wrapper_exit0(self):
        proc = subprocess.run([sys.executable, str(CHECK_TOOL)],
                              capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0,
                         f"check_registry failed: {proc.stdout}{proc.stderr}")


if __name__ == "__main__":
    unittest.main()
