"""Issue #96: LFO-AMS coverage audit + X-propagation probe (fast lane).

The fast-lane part re-derives everything from committed artifacts (no
simulator):

* the corpus-wide `amp_mod_sensitivity` census (path (a) of issue #96),
  including the 4 stress cases whose voices are not in
  `evidence/h07-core/cases.json`;
* the compared-window coverage of the recorded iverilog canonical shadow
  (`results-iverilog-shadow.json`) -- the masking mechanism;
* the committed iverilog-vs-Verilator artifact divergence;
* the shape of the committed probe evidence, including that each negative
  control really is clean where the positive run is not;
* the committed windowed conformance compares (`tools/ams_window_compare.py`)
  -- the two nonzero-AMS cases failing bit-exactness under iverilog over a
  window that contains the live note, and the two controls (Verilator, and
  the AMS = 0 sibling under the same truncation) passing it.

The live probe runs themselves (`tools/ams_xprobe.py`, ~1-2 min each under
iverilog/Verilator) are EVIDENCE runs, not fast-lane tests: set
`GF180_ISSUE96_LIVE=1` to re-run the positive case and its AMS=0 control
here.  See docs/AMS-XPROBE-96.md.
"""

import json
import os
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
sys.path.insert(0, str(REPO / "src"))

import h07_ams_audit as A  # noqa: E402

EV = REPO / "evidence" / "issue-96-ams-xprobe"
SHADOW = REPO / "evidence" / "h07-core" / "results-iverilog-shadow.json"
LIVE = os.environ.get("GF180_ISSUE96_LIVE", "0") == "1"


class AmsCensus(unittest.TestCase):
    """Path (a): which corpus patches can reach the exp() unit at all."""

    @classmethod
    def setUpClass(cls):
        cls.census = A.ams_census()

    def test_every_corpus_case_is_audited(self):
        self.assertEqual(len(self.census), 34)
        self.assertEqual(
            sum(1 for r in self.census.values() if r["set"] == "stress"), 4)

    def test_exactly_two_cases_have_a_nonzero_ams_operator(self):
        nonzero = sorted(c for c, r in self.census.items()
                         if r["ams_nonzero"])
        self.assertEqual(nonzero, ["dev32-06", "dev32-30"])
        for cid in nonzero:
            self.assertEqual(self.census[cid]["ams_op1_to_op6"],
                             [0, 0, 0, 0, 0, 3], cid)

    def test_the_four_stress_cases_are_ams_zero(self):
        """The stress voices come from the frozen registry, not cases.json,
        so they need their own check (the gap issue #96 called out)."""
        for cid in ("stress-sum16", "stress-burst", "stress-reset",
                    "stress-patchcommit"):
            row = self.census[cid]
            self.assertEqual(row["set"], "stress")
            for voice in row["ams_op1_to_op6_per_voice"]:
                self.assertEqual(voice, [0, 0, 0, 0, 0, 0], cid)
        self.assertEqual(self.census["stress-patchcommit"]["voices"], 2)


class ShadowWindowCoverage(unittest.TestCase):
    """The masking mechanism: what the recorded shadow actually compared."""

    @classmethod
    def setUpClass(cls):
        cls.cov = A.window_coverage(SHADOW)["cases"]

    def test_shadow_windows_are_almost_all_golden_silence(self):
        vac = [c for c, r in self.cov.items()
               if r["window"] == "golden_silence"]
        self.assertEqual(len(vac), 30)
        audio = sorted(c for c, r in self.cov.items()
                       if r["window"] == "golden_has_audio")
        self.assertEqual(audio, ["stress-patchcommit", "stress-reset",
                                 "stress-sum16"])

    def test_both_nonzero_ams_cases_are_uncovered(self):
        self.assertEqual(self.cov["dev32-06"]["window"], "golden_silence")
        self.assertTrue(self.cov["dev32-06"]["pass"])
        # dev32-30 never produced a compare at all in this file
        self.assertEqual(self.cov["dev32-30"]["window"], "NOT_RUN")
        self.assertFalse(self.cov["dev32-30"]["pass"])

    def test_control_the_verilator_acceptance_windows_are_not_vacuous(self):
        """Live control for the coverage check itself: the same audit over
        the full-length Verilator acceptance run must NOT report a
        corpus-wide vacuous window, or the metric is measuring nothing."""
        cov = A.window_coverage(
            REPO / "evidence" / "h07-core" / "results-verilog-accept.json")
        vac = [c for c, r in cov["cases"].items()
               if r["window"] == "golden_silence"]
        self.assertEqual(vac, ["stress-burst"])
        for cid in ("dev32-06", "dev32-30"):
            self.assertEqual(cov["cases"][cid]["window"], "golden_has_audio")


class ArtifactDivergence(unittest.TestCase):
    """The committed dumps disagree; the recorded verdicts did not say so."""

    @classmethod
    def setUpClass(cls):
        cls.div = A.artifact_divergence("iverilog-shadow",
                                        "verilog-accept")["cases"]

    def test_ams_zero_case_is_bit_identical_across_simulators(self):
        row = self.div["dev32-03"]
        self.assertEqual(row["diverging_samples"], 0)
        self.assertGreater(row["overlap_samples"], 100000)

    def test_both_nonzero_ams_cases_diverge_from_the_first_audio_sample(self):
        for cid in ("dev32-06", "dev32-30"):
            row = self.div[cid]
            self.assertGreater(row["diverging_samples"], 100000, cid)
            # note-on is model block 75 -> dump index (0 + 75 + 2) * 64
            self.assertGreater(row["first_diverging_index"], 4928, cid)
            self.assertLess(row["first_diverging_index"], 5000, cid)
            self.assertEqual(row["iverilog-shadow_state_obs"], "x", cid)
            self.assertNotEqual(row["verilog-accept_state_obs"], "x", cid)

    def test_the_x_corrupted_renders_are_dumped_as_silence(self):
        """Why the divergence was invisible: iverilog writes an unknown
        byte as NUL through tb_dx7_core.v's "%c" sample writer, so an
        x-valued sample lands in actual.i32 as a plain integer 0."""
        self.assertEqual(self.div["dev32-30"]["iverilog-shadow_nonzero"], 0)
        self.assertGreater(self.div["dev32-30"]["verilog-accept_nonzero"],
                           180000)


class CommittedProbeEvidence(unittest.TestCase):
    """Path (b): the probe report and its three negative controls."""

    files = {
        "positive": "probe-asfrozen-iverilog-dev32-30.json",
        "zero_ams": "control-zeroams-iverilog-dev32-30.json",
        "widened": "control-widened-iverilog-dev32-30.json",
        "verilator": "control-verilator-asfrozen-dev32-30.json",
    }

    @classmethod
    def setUpClass(cls):
        cls.rep = {k: json.loads((EV / v).read_text(encoding="utf-8"))
                   for k, v in cls.files.items()}

    def test_every_committed_report_passed_its_own_expectation(self):
        for k, r in self.rep.items():
            self.assertEqual(r["status"], "PASS", f"{k}: {r['problems']}")

    def test_the_probed_rtl_is_the_frozen_dr0011_core(self):
        for k, r in self.rep.items():
            self.assertTrue(r["frozen_rtl_matches_dr0011_pin"], k)
            if k != "widened":
                self.assertEqual(r["rtl_sha256"], r["frozen_rtl_sha256"], k)
            else:
                self.assertNotEqual(r["rtl_sha256"], r["frozen_rtl_sha256"])

    def test_positive_run_propagates_x_into_the_audio_path(self):
        res = self.rep["positive"]["result"]
        self.assertEqual(res["hsum"], 0)          # x from the first frame
        self.assertGreaterEqual(res["pt"], 0)
        self.assertGreaterEqual(res["li"], 0)
        self.assertGreaterEqual(res["wli"], 0)
        self.assertGreaterEqual(res["mix"], 0)
        self.assertEqual(res["opli_slot0_unknown_ops"], 1)
        self.assertGreater(res["unknown_samples"], 0)
        # ordering: source -> exp_pt -> level -> mix
        self.assertLessEqual(res["hsum"], res["pt"])
        self.assertLessEqual(res["pt"], res["mix"])

    def test_each_control_removes_exactly_one_link_and_comes_back_clean(self):
        for k in ("zero_ams", "widened", "verilator"):
            res = self.rep[k]["result"]
            self.assertEqual(res["unknown_samples"], 0, k)
            self.assertEqual(res["opli_slot0_unknown_ops"], 0, k)
            for sig in ("pt", "li", "wli", "mix"):
                self.assertEqual(res[sig], -1, f"{k}/{sig}")
        # the AMS=0 control keeps the x SOURCE (exp_hsum is combinational
        # and unconditionally unknown on the as-frozen RTL) and only
        # removes the gate; widening / Verilator remove the source itself.
        self.assertEqual(self.rep["zero_ams"]["result"]["hsum"], 0)
        self.assertEqual(self.rep["widened"]["result"]["hsum"], -1)
        self.assertEqual(self.rep["verilator"]["result"]["hsum"], -1)

    def test_controls_are_not_vacuously_clean(self):
        """A control that never ran the same stimulus proves nothing: each
        control must have rendered the same number of samples as the
        positive run."""
        base = self.rep["positive"]["result"]
        for k in ("zero_ams", "widened", "verilator"):
            res = self.rep[k]["result"]
            self.assertEqual(res["samples"], base["samples"], k)
            self.assertEqual(res["live"], base["live"], k)
            self.assertEqual(res["note_frame"], base["note_frame"], k)


class WindowedConformanceEvidence(unittest.TestCase):
    """Path (b) through the real harness: `tools/ams_window_compare.py`
    compares an unmodified corpus case against the untouched frozen golden
    over a window that reaches past the note-on (block 75)."""

    files = {
        "iv_dev32_30": "window-iverilog-dev32-30.json",
        "iv_dev32_06": "window-iverilog-dev32-06.json",
        "vl_dev32_30": "control-window-verilator-dev32-30.json",
        "iv_dev32_03": "control-window-iverilog-dev32-03-ams0.json",
    }

    @classmethod
    def setUpClass(cls):
        cls.rep = {k: json.loads((EV / v).read_text(encoding="utf-8"))
                   for k, v in cls.files.items()}

    def test_every_report_met_its_declared_expectation(self):
        for k, r in self.rep.items():
            self.assertEqual(r["status"], "PASS", f"{k}: {r['problems']}")
            self.assertTrue(r["rtl_matches_dr0011_pin"], k)

    def test_every_window_actually_contains_the_live_note(self):
        """A window with no audio in it could not fail, so the FAIL rows
        below would be vacuous."""
        for k, r in self.rep.items():
            self.assertEqual(r["compared_samples"], 7680, k)
            self.assertEqual(r["first_event_block"], 75, k)
            self.assertGreater(r["golden_nonzero_samples"], 2800, k)
            self.assertEqual(r["overrun"], 0, k)
            self.assertEqual(r["overflow"], 0, k)

    def test_nonzero_ams_cases_fail_bit_exactness_under_iverilog(self):
        for k in ("iv_dev32_30", "iv_dev32_06"):
            r = self.rep[k]
            self.assertTrue(r["ams_nonzero"], k)
            self.assertEqual(r["tool"], "iverilog", k)
            self.assertFalse(r["pass"], k)
            self.assertEqual(r["actual_nonzero_samples"], 0, k)
            self.assertEqual(r["state_obs"], "x", k)
            # the first mismatch is the first audible sample of the strike
            self.assertEqual(r["first_mismatch_block"], 75, k)
            self.assertGreaterEqual(r["mismatches"][0]["index"], 4928, k)
            self.assertLess(r["mismatches"][0]["index"], 4992, k)
            self.assertEqual(r["mismatches"][0]["actual"], 0, k)

    def test_the_two_controls_are_bit_exact_over_the_same_window(self):
        """Verilator (same case) and the AMS=0 sibling under iverilog (same
        truncation) must both reproduce the frozen golden exactly -- the
        second is what shows the window truncation is not the cause."""
        for k in ("vl_dev32_30", "iv_dev32_03"):
            r = self.rep[k]
            self.assertTrue(r["pass"], k)
            self.assertEqual(r["mismatches"], [], k)
            self.assertEqual(r["actual_nonzero_samples"],
                             r["golden_nonzero_samples"], k)
            self.assertNotEqual(r["state_obs"], "x", k)
        self.assertFalse(self.rep["iv_dev32_03"]["ams_nonzero"])
        self.assertEqual(self.rep["iv_dev32_03"]["tool"], "iverilog")
        self.assertTrue(self.rep["vl_dev32_30"]["ams_nonzero"])

    def test_the_dropped_events_are_all_after_the_compared_window(self):
        for k, r in self.rep.items():
            for block in r["event_blocks_dropped_after_window"]:
                self.assertGreaterEqual(block, r["window_blocks"], k)

    def test_the_masked_shadow_pass_is_the_same_case(self):
        """dev32-06 is recorded `pass: true` in the canonical shadow and
        fails here: that pair IS the masking finding."""
        shadow = json.loads(SHADOW.read_text(encoding="utf-8"))
        self.assertTrue(shadow["cases"]["dev32-06"]["pass"])
        self.assertFalse(self.rep["iv_dev32_06"]["pass"])


@unittest.skipUnless(LIVE and shutil.which("iverilog"),
                     "live probe run (set GF180_ISSUE96_LIVE=1 with "
                     "iverilog on PATH)")
class LiveProbe(unittest.TestCase):
    """Evidence re-run: the probe and its AMS=0 control, live."""

    def _run(self, *extra):
        cmd = [sys.executable, str(REPO / "tools" / "ams_xprobe.py"),
               "--case", "dev32-30", "--sim", "iverilog",
               "--runframes", "4", *extra]
        p = subprocess.run(cmd, cwd=str(REPO), capture_output=True,
                           text=True)
        return p.returncode, json.loads(p.stdout)

    def test_live_positive_and_zero_ams_control(self):
        rc, pos = self._run()
        self.assertEqual(rc, 0, pos.get("problems"))
        self.assertGreater(pos["result"]["unknown_samples"], 0)
        rc, ctl = self._run("--zero-ams")
        self.assertEqual(rc, 0, ctl.get("problems"))
        self.assertEqual(ctl["result"]["unknown_samples"], 0)


if __name__ == "__main__":
    unittest.main()
