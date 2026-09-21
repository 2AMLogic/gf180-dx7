#!/usr/bin/env python3
"""H03 core-contract tests (issue #25).

Covers, per the issue's acceptance:
  - the per-frame budget arithmetic recomputed INDEPENDENTLY in this file
    (6,144 operator evals/frame; 16,384 / 32,768 clk frames; every row's
    unit x count; utilization at BOTH clocks; the recorded FAIL/PASS
    verdicts) -- never by importing the checker's arithmetic;
  - every interface decision cites a DEC id that exists in
    spec/contract-v1.json (D00);
  - queue/latency constants present with their derivation numbers
    (DEC-014: bounded depth, never guessed);
  - tools/h03_budget_check.py passes live;
  - negative controls: a budget row mutated (2 clk/eval) fails the checker;
    a budget with the worst-burst rows removed (the forbidden average-case
    framing) fails the checker; a drifted evidence hash fails the checker;
  - lockstep: docs/CONTRACT-CORE-v1.md carries the headline finding, the
    module boundaries, the STALE rules, and the recomputed numbers; every
    module's N08 conformance case id exists in the frozen release bundle.

Stdlib only.
"""

import copy
import json
import os
import re
import subprocess
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC_PATH = os.path.join(REPO, "spec", "core-contract-v1.json")
CONTRACT_PATH = os.path.join(REPO, "spec", "contract-v1.json")
DOC_PATH = os.path.join(REPO, "docs", "CONTRACT-CORE-v1.md")
TOOL = os.path.join(REPO, "tools", "h03_budget_check.py")
VECTORS_DIR = os.path.join(REPO, "reference", "release", "vectors")

sys_path = os.path.join(REPO, "tools")
if sys_path not in __import__("sys").path:
    __import__("sys").path.insert(0, sys_path)
import h03_budget_check  # noqa: E402


def load(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


SPEC = load(SPEC_PATH)
D00 = load(CONTRACT_PATH)


def recompute(spec):
    """Independent recomputation from first principles (test-local)."""
    p = spec["premises"]
    evals_frame = p["polyphony"] * p["operators_per_note"] * p["frame_samples"]
    frames = p["sample_rate_hz"] / p["frame_samples"]
    out = {
        "evals_frame": evals_frame,
        "evals_second": evals_frame * frames,
        "frames": frames,
        "frame_clks": {"12m288": 12288000 / frames,
                       "24m576": 24576000 / frames},
    }
    rows = {r["id"]: r for r in spec["budget_worst_case"]["rows"]}
    totals = {}
    for key in ("12m288", "24m576"):
        total = 0
        for r in spec["budget_worst_case"]["rows"]:
            cnt = r[f"count_{key}"] if f"count_{key}" in r else r["count"]
            clk = r.get(f"clocks_{key}", r.get("clocks"))
            total += r["unit_clks"] * cnt
            assert clk == r["unit_clks"] * cnt, f"row {r['id']} {key} arith"
        totals[key] = total
    out["totals"] = totals
    out["util"] = {k: totals[k] / out["frame_clks"][k] for k in totals}
    out["margin"] = {k: out["frame_clks"][k] - totals[k] for k in totals}
    return out


class TestBudgetArithmetic(unittest.TestCase):
    def test_frame_facts(self):
        rc = recompute(SPEC)
        self.assertEqual(rc["evals_frame"], 6144)
        self.assertEqual(rc["evals_second"], 4608000)
        self.assertEqual(rc["frames"], 750)
        self.assertEqual(rc["frame_clks"]["12m288"], 16384)
        self.assertEqual(rc["frame_clks"]["24m576"], 32768)

    def test_row_arithmetic_and_totals(self):
        rc = recompute(SPEC)
        t = SPEC["budget_worst_case"]["totals"]
        self.assertEqual(t["clocks_12m288"], rc["totals"]["12m288"])
        self.assertEqual(t["clocks_24m576"], rc["totals"]["24m576"])
        self.assertEqual(t["clocks_12m288"], 17260)
        self.assertEqual(t["clocks_24m576"], 17344)

    def test_utilization_both_clocks_and_verdicts(self):
        rc = recompute(SPEC)
        t = SPEC["budget_worst_case"]["totals"]
        self.assertAlmostEqual(t["utilization_12m288"], rc["util"]["12m288"])
        self.assertAlmostEqual(t["utilization_24m576"], rc["util"]["24m576"])
        self.assertAlmostEqual(rc["util"]["12m288"], 17260 / 16384)
        self.assertAlmostEqual(rc["util"]["24m576"], 17344 / 32768)
        self.assertGreater(rc["util"]["12m288"], 1.0)
        self.assertEqual(t["verdict_12m288"], "FAIL")
        self.assertEqual(t["verdict_24m576"], "PASS")
        self.assertEqual(t["margin_12m288"], -876)
        self.assertEqual(t["margin_24m576"], 15424)

    def test_worst_case_rows_are_worst(self):
        rows = {r["id"]: r for r in SPEC["budget_worst_case"]["rows"]}
        # worst case = every voice live, every frame
        self.assertEqual(rows["op_evals"]["count"], 6144)
        self.assertEqual(rows["mix_convert"]["count"], 1024)
        # worst admissible event burst: full polyphony chord change
        self.assertEqual(rows["note_on_application"]["count"], 16)
        self.assertEqual(rows["note_off_application"]["count"], 16)
        self.assertEqual(rows["controller_events"]["count"], 16)
        # envelope steps carry the worst-case advance (>= N01's 12 clk estimate)
        self.assertGreaterEqual(rows["env_steps"]["unit_clks"], 24)

    def test_measured_anchor_h01(self):
        rows = {r["id"]: r for r in SPEC["budget_worst_case"]["rows"]}
        self.assertEqual(rows["op_evals"]["unit_class"], "measured")
        self.assertEqual(rows["op_evals"]["unit_clks"], 1)
        report = load(os.path.join(REPO, "evidence", "h01-operator-probe",
                                   "synth_report.json"))
        self.assertEqual(
            report["cycles_per_operator"]["throughput_clks_per_eval"], 1)


class TestInterfaceCitations(unittest.TestCase):
    def test_every_cited_dec_id_exists(self):
        known = set(D00["decisions"])
        cites = []
        for row in SPEC["budget_worst_case"]["rows"]:
            cites += [(row["id"], d) for d in row.get("dec_ids", [])]
        for section in ("patch_interface", "event_interface", "status_word",
                        "reset_boot"):
            cites += [(section, d)
                      for d in SPEC[section].get("dec_ids", [])]
        for mod in SPEC["modules"]:
            cites += [(mod["name"], d) for d in mod.get("dec_ids", [])]
        self.assertTrue(cites, "contract must cite D00 decisions")
        for label, dec in cites:
            self.assertIn(dec, known, f"{label} cites unknown {dec}")

    def test_key_interface_rules_cited(self):
        ev = SPEC["event_interface"]
        self.assertIn("DEC-014", ev["dec_ids"])
        self.assertIn("DEC-019", SPEC["patch_interface"]["dec_ids"])
        self.assertIn("DEC-011", SPEC["reset_boot"]["dec_ids"])
        self.assertIn("DEC-010", ev["dec_ids"])

    def test_no_per_sample_host_dependency(self):
        # DEC-010 declaration present in the spec's transport + doc
        self.assertIn("DEC-010", SPEC["event_interface"]["dec_ids"])
        with open(DOC_PATH, "r", encoding="utf-8") as f:
            doc = f.read()
        self.assertIn("No per-sample host dependency", doc)


class TestQueueLatencyConstants(unittest.TestCase):
    def test_queue_constants_present(self):
        ev = SPEC["event_interface"]
        self.assertEqual(ev["queue_depth_skid"], 8)
        self.assertEqual(ev["queue_entry_bits"], 49)
        self.assertEqual(ev["drain_rate_writes_per_clk"], 1)
        self.assertEqual(ev["min_transaction_period_clks"], 194)
        self.assertIn("explicit", ev["overflow_policy"])
        self.assertIn("forbidden", ev["overflow_policy"])

    def test_drain_counts_derived_not_guessed(self):
        rows = {r["id"]: r for r in SPEC["budget_worst_case"]["rows"]}
        drain = rows["write_queue_drain"]
        period = SPEC["event_interface"]["min_transaction_period_clks"]
        self.assertEqual(drain["count_12m288"], 16384 // period)
        self.assertEqual(drain["count_24m576"], 32768 // period)

    def test_latency_constants_present(self):
        lat = SPEC["latency"]
        self.assertEqual(lat["commit_bound_frames"], 1)
        self.assertEqual(lat["worst_case_to_sdata_samples"], 128)
        self.assertIn("SDATA", lat["definition"])

    def test_status_word_pattern(self):
        sw = SPEC["status_word"]
        for flag in ("OVERRUN", "QUEUE", "OVERFLOW", "FRESH", "FRAME"):
            self.assertIn(flag, sw["flags"])
        self.assertIn("DR-0007", sw["pattern_source"])


class TestToolAndControls(unittest.TestCase):
    def test_budget_check_tool_passes(self):
        proc = subprocess.run(["python3", TOOL], capture_output=True,
                              text=True, cwd=REPO, timeout=300)
        self.assertEqual(proc.returncode, 0,
                         f"tool failed:\n{proc.stdout}\n{proc.stderr}")
        self.assertIn("PASS", proc.stdout)
        self.assertIn("105.35%", proc.stdout)
        self.assertIn("52.93%", proc.stdout)

    def test_negative_control_mutated_unit_fails(self):
        bad = copy.deepcopy(SPEC)
        for row in bad["budget_worst_case"]["rows"]:
            if row["id"] == "op_evals":
                row["unit_clks"] = 2          # claim the kernel costs 2 clk
                row["clocks"] = 2 * row["count"]
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails, "2 clk/eval claim must fail the check")
        self.assertTrue(any("op_evals" in f for f in fails),
                        f"failure must name the row: {fails}")

    def test_negative_control_average_case_budget_fails(self):
        """Deleting the worst-burst rows would 'close' 12.288 MHz -- the
        forbidden average-case framing must fail the check, never pass."""
        bad = copy.deepcopy(SPEC)
        burst = {"note_on_application", "note_off_application",
                 "controller_events"}
        rows = [r for r in bad["budget_worst_case"]["rows"]
                if r["id"] not in burst]
        bad["budget_worst_case"]["rows"] = rows
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails, "average-case (burst-stripped) budget must fail")
        self.assertTrue(any("required worst-case budget row missing" in f
                            for f in fails), f"failures: {fails}")

    def test_negative_control_fake_pass_verdict_fails(self):
        bad = copy.deepcopy(SPEC)
        bad["budget_worst_case"]["totals"]["verdict_12m288"] = "PASS"
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(any("verdict_12m288" in f for f in fails))

    def test_negative_control_hash_drift_fails(self):
        bad = copy.deepcopy(SPEC)
        bad["depends_on"]["h01_evidence"]["synth_report"]["sha256"] = "0" * 64
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(any("drifted" in f for f in fails))


class TestDocLockstep(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with open(DOC_PATH, "r", encoding="utf-8") as f:
            cls.doc = f.read()

    def test_headline_finding_present(self):
        self.assertIn("12.288 MHz does NOT close", self.doc)
        self.assertIn("105.35%", self.doc)
        self.assertIn("52.93%", self.doc)
        self.assertIn("876", self.doc)
        self.assertIn("15,424", self.doc)
        self.assertIn("average-case", self.doc)

    def test_budget_table_lines(self):
        rc = recompute(SPEC)
        self.assertIn(f"{rc['totals']['12m288']:,}", self.doc)
        self.assertIn(f"{rc['totals']['24m576']:,}", self.doc)
        self.assertIn("6,144", self.doc)      # op evals/frame
        self.assertIn("4,608,000", self.doc)  # evals/s

    def test_modules_present(self):
        for mod in SPEC["modules"]:
            self.assertIn(mod["name"], self.doc, f"module {mod['name']} in doc")

    def test_stale_rules_present(self):
        self.assertIn("STALE", self.doc)
        self.assertIn("decision record", self.doc)
        self.assertIn("H04", self.doc)

    def test_claim_boundary_present(self):
        self.assertIn("does NOT say", self.doc)
        self.assertIn("No synthesis, place-and-route, timing closure", self.doc)

    def test_n08_conformance_cases_exist(self):
        bundled = {name[:-4] for name in os.listdir(VECTORS_DIR)
                   if name.endswith(".f32")}
        self.assertGreaterEqual(len(bundled), 51)
        for mod in SPEC["modules"]:
            for case in mod.get("conformance_vectors", []):
                self.assertIn(case, bundled,
                              f"{mod['name']}: case {case} not in N08 bundle")

    def test_design_point_justified(self):
        self.assertIn("design point", self.doc)
        self.assertIn("H10", self.doc)  # timing closure explicitly deferred


if __name__ == "__main__":
    unittest.main()
