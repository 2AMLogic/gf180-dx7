#!/usr/bin/env python3
"""Live negative control for the H10 fit-conclusion gate (issue #32, item 5).

The report generator MUST refuse to emit any "fits" conclusion when a
required stage (e.g. routing) did not run for the packaging being judged.
These tests build in-memory bundles with small hash-pinned evidence files
and assert:

  A. route stage missing  -> fit gate refuses (NO_VERDICT, named reason),
     and --require-fit exits 3.
  B. full RUN-evidenced chain (shared synth + variant place/route/sta,
     0 DRC, ss setup slack >= 0) -> the positive "fits" conclusion is
     permitted.
  C. measured mapped area exceeding the fixed core area -> "does not fit"
     by the area-alone measured path (no place/route stage needed).
  D. hash drift in pinned evidence -> bundle verification fails (exit 4).
  E. full chain but negative setup slack at the headline corner -> refused
     (timing does not close; a clean route is not a fit claim).
  F. a stage reported PASS with fabricated (absent) evidence -> refused.
  G. route with DRC errors > 0 -> refused (the pre-fix gate accepted it).
  H. route with NO measured DRC count -> refused (never read as 0).
  I. a synth stage that is not a mapped real-cell result (an estimate)
     -> refused for BOTH "fits" and "does not fit".
  J. area-alone non-fit uses the most favourable measured policy: if any
     policy's mapped area is within the core, "does not fit" is refused.
  K. the committed bundle re-verifies, the committed report is FRESH,
     and --require-fit on it exits 3 (no die has a routed chain).
  L. the committed bundle records, for each yosys run, the binary path and
     sha256 (the image ships two builds), fresh against committed evidence,
     with the logged banner matching that binary.
  M. negative control for L: a log carrying the other binary's banner is
     recorded as a mismatch, and a missing banner as unknown, never a match.
  N. the bundle's RTL provenance is what the runs recorded, never a re-hash
     of the working tree (DR-0012, issue #98).
  O. while the measured core differs from the frozen core, the report
     carries a supersession banner -- and must stop carrying one when H10
     is re-run on the current pin.

Fast lane (DR-0009): stdlib only, runs in well under a second.
"""

import hashlib
import json
import os
import subprocess
import sys
import tempfile
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))

import h10_report  # noqa: E402


def _stage(status, evidence, **extra):
    st = {"status": status}
    if evidence:
        st["evidence"] = evidence
    st.update(extra)
    return st


class _BundleHarness:
    """Builds a bundle + temp evidence tree; root points at the temp dir."""

    def __init__(self, mapped_area=1.0e6, core_area=2.0e6):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        ev = {}
        for name, content in [
            ("synth.json", "{}"),
            ("place.json", "{}"),
            ("route.json", '{"drc_errors": 0}'),
            ("sta.json", "{}"),
        ]:
            p = os.path.join(self.root, name)
            with open(p, "w") as f:
                f.write(content)
            ev[name] = h10_report.sha256_file(p)

        self.bundle = {
            "design": "test core",
            "target_clock_mhz": 24.576,
            "clock_period_ns": 40.692,
            "packaging_budgets": {
                "quarterslot": {
                    "die_area_um2": 1.7319e6,
                    "core_area_um2": core_area,
                    "die_um_w": 1314.88,
                    "die_um_h": 1317.12,
                    "source": "test",
                },
            },
            "synth": _stage("PASS", {"synth.json": ev["synth.json"]},
                            mapped_stdcell_area_um2=mapped_area,
                            instance_count=100, flop_count=10,
                            mapped=True),
            "stages": {
                "quarterslot": {
                    "place": _stage("PASS", {"place.json": ev["place.json"]},
                                    converged=True),
                    "route": _stage("PASS", {"route.json": ev["route.json"]},
                                    converged=True, drc_errors=0),
                    "sta": _stage("PASS", {"sta.json": ev["sta.json"]},
                                  corners={"ss": {"setup_wns_ns": 1.0,
                                                  "setup_tns_ns": 0.0,
                                                  "hold_wns_ns": 0.5}}),
                },
            },
            "family_context": {"notes": []},
            "acceptance_rows": [],
            "unaccounted": [],
            "unproved": [],
        }
        self.evidence_files = ev

    def bundle_path(self, tag="bundle.json"):
        p = os.path.join(self.root, tag)
        with open(p, "w") as f:
            json.dump(self.bundle, f)
        return p

    def run_tool(self, *args):
        return subprocess.run(
            [sys.executable, os.path.join(REPO_ROOT, "tools", "h10_report.py"),
             "--root", self.root, *args],
            capture_output=True, text=True, cwd=REPO_ROOT)


class TestFitGate(unittest.TestCase):
    def setUp(self):
        self.h = _BundleHarness()
        self.root = self.h.root

    def tearDown(self):
        self.h.tmp.cleanup()

    def test_a_missing_route_blocks_fit_conclusion(self):
        # the control that matters: NO routing stage -> no "fits", ever
        self.h.bundle["stages"]["quarterslot"]["route"] = _stage(
            "NOT_RUN", None, note="not run for this packaging")
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "NO_VERDICT")
        self.assertTrue(any("route" in r for r in reasons),
                        "refusal must name the missing route stage: "
                        f"{reasons}")
        # the full-tool path agrees: --require-fit must exit 3
        p = self.h.bundle_path()
        r = self.h.run_tool("--bundle", p, "--out", os.path.join(self.root,
                         "r.md"), "--require-fit")
        self.assertEqual(r.returncode, 3, r.stderr)
        self.assertIn("FIT REFUSED", r.stderr)
        # and the default report must not contain a positive verdict
        r2 = self.h.run_tool("--bundle", p, "--out", os.path.join(self.root,
                          "r2.md"))
        self.assertEqual(r2.returncode, 0, r2.stderr)
        text = open(os.path.join(self.root, "r2.md")).read()
        self.assertNotIn("Verdict: **fits**", text)
        self.assertIn("NO_VERDICT", text)

    def test_b_full_chain_permits_fit(self):
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "fits", reasons)
        p = self.h.bundle_path()
        r = self.h.run_tool("--bundle", p, "--out", os.path.join(self.root,
                         "r.md"), "--require-fit")
        self.assertEqual(r.returncode, 0, r.stderr)
        text = open(os.path.join(self.root, "r.md")).read()
        self.assertIn("Verdict: **fits**", text)

    def test_c_area_alone_nonfit_needs_no_place_route(self):
        h = _BundleHarness(mapped_area=3.0e6, core_area=2.0e6)
        try:
            h.bundle["stages"]["quarterslot"]["place"] = \
                _stage("NOT_RUN", None)
            h.bundle["stages"]["quarterslot"]["route"] = \
                _stage("NOT_RUN", None)
            verdict, reasons = h10_report.fit_gate(h.bundle, "quarterslot",
                                                   h.root)
            self.assertEqual(verdict, "does not fit")
            self.assertTrue(any("measured mapped std-cell area" in r
                                for r in reasons), reasons)
        finally:
            h.tmp.cleanup()

    def test_d_hash_drift_fails_verification(self):
        p = self.h.bundle_path()
        with open(os.path.join(self.root, "route.json"), "w") as f:
            f.write('{"drc_errors": 99}')  # evidence drifted after pinning
        problems = h10_report.verify_bundle(self.h.bundle, self.root)
        self.assertTrue(any("sha256 mismatch" in x for x in problems),
                        f"expected a hash-mismatch problem, got {problems}")
        r = self.h.run_tool("--bundle", p, "--out", os.path.join(self.root,
                         "r.md"))
        self.assertEqual(r.returncode, 4, r.stderr)

    def test_e_negative_slack_blocks_fit_despite_clean_route(self):
        self.h.bundle["stages"]["quarterslot"]["sta"]["corners"]["ss"][
            "setup_wns_ns"] = -3.2
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "NO_VERDICT")
        self.assertTrue(any("NEGATIVE" in r for r in reasons), reasons)

    def test_f_fabricated_evidence_refused(self):
        # PASS status but the pinned file does not exist -> not pass-evidenced
        self.h.bundle["stages"]["quarterslot"]["sta"]["evidence"] = \
            {"missing_file.json": "0" * 64}
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "NO_VERDICT")
        self.assertTrue(any("sta" in r for r in reasons), reasons)

    def test_g_drc_errors_block_fit(self):
        self.h.bundle["stages"]["quarterslot"]["route"]["drc_errors"] = 7
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "NO_VERDICT")
        self.assertTrue(any("7 detailed-route DRC" in r for r in reasons),
                        reasons)

    def test_h_missing_drc_count_blocks_fit(self):
        del self.h.bundle["stages"]["quarterslot"]["route"]["drc_errors"]
        verdict, reasons = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                               self.root)
        self.assertEqual(verdict, "NO_VERDICT")
        self.assertTrue(any("not measured" in r for r in reasons), reasons)

    def test_i_estimated_synth_refused_both_ways(self):
        for area in (1.0e6, 3.0e6):  # would-fit and would-not-fit
            self.h.bundle["synth"]["mapped_stdcell_area_um2"] = area
            self.h.bundle["synth"]["mapped"] = False
            self.h.bundle["synth"]["method"] = "projection from per-bit anchors"
            verdict, reasons = h10_report.fit_gate(self.h.bundle,
                                                   "quarterslot", self.root)
            self.assertEqual(verdict, "NO_VERDICT", (area, reasons))
            self.assertTrue(any("estimate" in r for r in reasons), reasons)
        del self.h.bundle["synth"]["mapped"]
        verdict, _ = h10_report.fit_gate(self.h.bundle, "quarterslot",
                                         self.root)
        self.assertEqual(verdict, "NO_VERDICT")

    def test_j_nonfit_uses_most_favourable_policy(self):
        h = _BundleHarness(mapped_area=3.0e6, core_area=2.0e6)
        try:
            h.bundle["stages"]["quarterslot"] = {}
            h.bundle["synth"]["policy_areas_um2"] = {
                "orfs_stock": 3.0e6, "x1_allowed": 1.9e6}
            verdict, reasons = h10_report.fit_gate(h.bundle, "quarterslot",
                                                   h.root)
            self.assertNotEqual(verdict, "does not fit", reasons)
            self.assertEqual(verdict, "NO_VERDICT")
            h.bundle["synth"]["policy_areas_um2"]["x1_allowed"] = 2.5e6
            verdict, _ = h10_report.fit_gate(h.bundle, "quarterslot", h.root)
            self.assertEqual(verdict, "does not fit")
        finally:
            h.tmp.cleanup()


BUNDLE = os.path.join(REPO_ROOT, "evidence", "h10-gf180", "bundle.json")
REPORT = os.path.join(REPO_ROOT, "docs", "H10-GF180-FEASIBILITY.md")


@unittest.skipUnless(os.path.isfile(BUNDLE), "no committed H10 bundle")
class TestCommittedBundle(unittest.TestCase):
    def _run(self, *args):
        return subprocess.run(
            [sys.executable, os.path.join(REPO_ROOT, "tools", "h10_report.py"),
             "--bundle", BUNDLE, "--out", REPORT, *args],
            capture_output=True, text=True, cwd=REPO_ROOT)

    def test_k_committed_report_fresh_and_fit_refused(self):
        r = self._run("--check")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        r = self._run("--require-fit")
        self.assertEqual(r.returncode, 3, r.stdout + r.stderr)
        with open(BUNDLE) as f:
            b = json.load(f)
        for pk in b["packaging_budgets"]:
            v, reasons = h10_report.fit_gate(b, pk)
            self.assertNotEqual(v, "fits", (pk, reasons))
        self.assertTrue(b["synth"].get("mapped"))

    def test_l_yosys_identity_per_run(self):
        # PR #84 judge finding: the image ships two yosys builds, so the
        # digest alone does not identify the binary. Each run must record
        # path + sha256, and its committed banner must match that binary.
        import h10_collect
        with open(BUNDLE) as f:
            tools = json.load(f)["tools"]
        runs = tools["yosys_runs"]
        self.assertEqual(set(runs), {"orfs_flow_synth", "policy_ab"})
        ev = os.path.join(REPO_ROOT, h10_collect.EV_REL)
        self.assertEqual(runs, h10_collect.yosys_runs(ev),
                         "bundle.tools.yosys_runs is stale vs committed "
                         "evidence; rerun tools/h10_collect.py")
        for name, r in runs.items():
            self.assertTrue(r["path"].startswith("/"), name)
            self.assertRegex(r["sha256"], r"^[0-9a-f]{64}$", name)
            self.assertIs(r["banner_matches"], True, (name, r["banner"]))
        self.assertNotEqual(tools["yosys_not_used"]["sha256"],
                            runs["orfs_flow_synth"]["sha256"])

    def test_m_yosys_banner_mismatch_is_recorded_not_assumed(self):
        # negative control: a run whose log carries the PATH binary's 0.67
        # banner must not be recorded as the 0.68+post flow binary, and a
        # missing banner is None (unknown), never True.
        import h10_collect
        with tempfile.TemporaryDirectory() as ev:
            os.makedirs(os.path.join(ev, "orfs-synth"))
            with open(os.path.join(ev, "orfs-synth",
                                   "1_2_yosys.trimmed.log"), "w") as f:
                f.write(h10_collect.YOSYS_PATH["self_reported"] + "\n")
            runs = h10_collect.yosys_runs(ev)
        self.assertIs(runs["orfs_flow_synth"]["banner_matches"], False)
        self.assertIsNone(runs["policy_ab"]["banner_matches"])

    def test_n_measured_rtl_is_the_runs_not_the_worktree(self):
        """DR-0012 (#98). The bundle's `rtl` map must be what the runs
        recorded, never a re-hash of whatever is checked out when the
        bundle is collected -- otherwise editing the core silently
        re-stamps a stale bundle with the new core's provenance."""
        import h10_collect
        with open(BUNDLE) as f:
            b = json.load(f)
        ev = os.path.join(REPO_ROOT, h10_collect.EV_REL)
        self.assertEqual(b["rtl"], h10_collect.measured_rtl(ev))
        with open(os.path.join(ev, "orfs-synth", "run.json")) as f:
            self.assertEqual(b["rtl"], json.load(f)["rtl"])

    def test_o_stale_banner_tracks_the_frozen_pin(self):
        """DR-0012 (#98). While the core the H10 runs measured differs from
        the core now in the tree, the report MUST carry a supersession
        banner; once H10 is re-run on the current pin, the banner must be
        removed rather than left as decoration."""
        with open(BUNDLE) as f:
            b = json.load(f)
        measured = (b.get("rtl") or {}).get("rtl/dx7_core.v")
        with open(os.path.join(REPO_ROOT, "rtl", "dx7_core.v"), "rb") as f:
            current = hashlib.sha256(f.read()).hexdigest()
        report = open(REPORT).read()
        if measured == current:
            self.assertIsNone(b.get("superseded_by"),
                              "H10 was re-run on the current pin; drop "
                              "h10_collect.SUPERSEDED_BY and regenerate")
            self.assertNotIn("**STALE —", report)
        else:
            sup = b.get("superseded_by")
            self.assertIsNotNone(
                sup, "bundle measured a core that is no longer the frozen "
                     "core; it must carry a supersession record")
            self.assertEqual(sup["core_pin_measured"], measured)
            self.assertEqual(sup["core_pin_current"], current)
            self.assertIn("**STALE —", report)


if __name__ == "__main__":
    unittest.main(verbosity=2)