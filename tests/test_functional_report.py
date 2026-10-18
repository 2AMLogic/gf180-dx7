"""Checker logic for the `klt functional-verification` evidence (issue #150).

Covers tools/functional_verification.py without cocotb, klt or a simulator:
identical report -> PASS, flipped verdict -> FAIL, changed RTL hash -> STALE,
missing tool -> NOT_RUN (never PASS), and the negative-control verdict.  Also
pins the committed evidence to the tree: the report must name the current RTL
hashes and the DR-0012 pin.  It proves nothing about the RTL itself; the
re-run (`make functional-verification`) does that.

Stdlib only.
"""

import copy
import json
import os
import sys
import tempfile
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "tools"))

import functional_verification as FV  # noqa: E402

HASHES = {"rtl/dx7_core.v": "a" * 64, "rtl/env_unit.v": "b" * 64,
          "rtl/alg_router.v": "c" * 64, "rtl/tb_dx7_core.v": "d" * 64}


def report(**over):
    r = {"status": "pass", "test_count": 2, "passed_count": 2,
         "failed_count": 0, "skipped_count": 0,
         "tests": [{"name": "case_a", "status": "passed", "real_time_s": 1.0},
                   {"name": "case_b", "status": "passed", "real_time_s": 2.0}],
         "environment": {"engine": "icarus", "engine_version": "13.0",
                         "cocotb_version": "2.1.0", "random_seed": 1,
                         "results_xml": "/tmp/x.xml"},
         "provenance": {"rtl_sha256": dict(HASHES),
                        "cases": {"a": {"actual_sha256": "1" * 64}}}}
    r.update(over)
    return r


class CheckerTests(unittest.TestCase):
    def test_identical_rerun_passes_ignoring_timing_and_paths(self):
        fresh = report()
        fresh["tests"][0]["real_time_s"] = 99.0
        fresh["environment"]["results_xml"] = "/elsewhere/r.xml"
        self.assertEqual(FV.compare(report(), fresh, HASHES), ("PASS", []))

    def test_flipped_verdict_fails(self):
        fresh = report(status="fail", failed_count=1, passed_count=1)
        fresh["tests"][1]["status"] = "failed"
        status, why = FV.compare(report(), fresh, HASHES)
        self.assertEqual(status, "FAIL")
        self.assertIn("case_b", " ".join(why))

    def test_passing_rerun_that_differs_from_committed_fails(self):
        fresh = report()
        fresh["provenance"]["cases"]["a"]["actual_sha256"] = "2" * 64
        status, why = FV.compare(report(), fresh, HASHES)
        self.assertEqual(status, "FAIL")
        self.assertIn("provenance", " ".join(why))

    def test_changed_rtl_hash_is_stale_not_pass(self):
        moved = dict(HASHES, **{"rtl/dx7_core.v": "e" * 64})
        status, why = FV.compare(report(), report(), moved)
        self.assertEqual(status, "STALE")
        self.assertIn("rtl/dx7_core.v", " ".join(why))

    def test_missing_tool_is_not_run(self):
        with tempfile.TemporaryDirectory() as d:
            klt, why = FV.tooling(FV.Path(d))
        self.assertIsNone(klt)
        self.assertEqual(FV.EXIT["NOT_RUN"], 2)
        self.assertIn("missing", why)
        self.assertNotEqual(FV.EXIT["NOT_RUN"], FV.EXIT["PASS"])

    def test_control_requires_every_selected_case_to_fail(self):
        failed = report(tests=[{"name": "case_stress_sum16",
                                "status": "failed"}])
        ok, _ = FV.control_verdict(failed, ["stress-sum16"])
        self.assertTrue(ok)
        survived = copy.deepcopy(failed)
        survived["tests"][0]["status"] = "passed"
        ok, msg = FV.control_verdict(survived, ["stress-sum16"])
        self.assertFalse(ok, "a control that does not fail is a defect")
        self.assertIn("did NOT fail", msg)
        ok, _ = FV.control_verdict(report(tests=[]), ["stress-sum16"])
        self.assertFalse(ok)

    def test_dr0012_pin_parser(self):
        text = ("| `rtl/dx7_core.v` | `" + "f" * 64 + "` |\n"
                "| `rtl/env_unit.v` | `" + "1" * 64 + "` (unchanged) |\n")
        self.assertEqual(FV.dr12_pin(text),
                         {"rtl/dx7_core.v": "f" * 64,
                          "rtl/env_unit.v": "1" * 64})


class CommittedEvidenceTests(unittest.TestCase):
    def test_committed_report_names_current_rtl_and_pin(self):
        path = os.path.join(REPO, "evidence", "t1-functional", "report.json")
        if not os.path.exists(path):
            self.skipTest("NOT_RUN: no committed report yet")
        rep = json.load(open(path))
        prov = rep["provenance"]
        self.assertEqual(prov["rtl_sha256"], FV.rtl_hashes(),
                         "STALE: RTL changed since the report was generated")
        self.assertTrue(prov["dr0012_pin_matches"])
        self.assertEqual(rep["status"], "pass")
        self.assertEqual(rep["failed_count"], 0)
        self.assertEqual(rep["skipped_count"], 0)
        # every graded test carries per-case coverage facts
        for t in rep["tests"]:
            self.assertIn(t["status"], ("passed",))
        self.assertEqual(len(prov["cases"]), rep["test_count"])
        # coverage apart from agreement: stress-burst's 24-block window is
        # silent (its notes on/off inside one window), so its PASS says
        # nothing about audio; any other silent compare is a regression
        silent = sorted(k for k, c in prov["cases"].items()
                        if c["audible_samples"] == 0)
        self.assertEqual(silent, ["stress-burst"],
                         "a graded case compared only silence")
        self.assertEqual(prov["not_graded"]["status"], "NOT_RUN")


if __name__ == "__main__":
    unittest.main()
