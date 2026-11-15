"""Tests for tools/check_fv_pins.py — the FV pin/provenance gate (#226).

The four live negative controls from the issue (each must demonstrably
fail the check it targets):
  (a) bumped klayout-tools pin + unregenerated report -> exit 1, for the
      pin reason (the #185/#186 regression replayed);
  (b) bumped cocotb pin + unregenerated report -> exit 1;
  (c) a matching pin/report pair -> exit 0 (the failure above is not
      unconditional);
  (d) unparseable FV_PINS -> exit 2, asserted NOT to be 0.
Plus: the real-tree agreement case (what actually bites during a bump),
Makefile absent / no provenance-bearing report -> exit 2, request.json
(no provenance) skipped without a verdict, a missing or malformed
klt_version -> a stated failure (never a silent pass), reformatted /
reordered FV_PINS still parses, and an unknown pin distribution -> exit 2.
"""

import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CHECKER = os.path.join(REPO_ROOT, "tools", "check_fv_pins.py")

PINS = '"klayout-tools==0.8.0" "cocotb==2.1.0"'


def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data if isinstance(data, bytes) else data.encode("utf-8"))


def report_bytes(klt="klt 0.8.0", cocotb="2.1.0", with_klt=True):
    provenance = {"rtl_sha256": {}}
    if with_klt:
        provenance["klt_version"] = klt
    return json.dumps({
        "status": "pass",
        "provenance": provenance,
        "environment": {"cocotb_version": cocotb, "engine": "icarus"},
    }).encode("utf-8")


def request_bytes():
    # The klt request: no provenance block, no recorded toolchain.
    return json.dumps({"top": "dx7_core", "seed": 1}).encode("utf-8")


class CheckFvPinsTestCase(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(prefix="fv-pins-check-")
        self.addCleanup(shutil.rmtree, self.tmp, True)

    def run_check(self, root=None):
        cmd = [sys.executable, CHECKER]
        if root is not None:
            cmd += ["--repo-root", root]
        return subprocess.run(cmd, capture_output=True, text=True)

    def make_tree(self, makefile=PINS, reports=("report.json",),
                  request=True):
        write(os.path.join(self.tmp, "Makefile"),
              f"FV_PINS = {makefile}\n")
        for name in reports:
            write(os.path.join(self.tmp, "evidence", "t1-functional",
                               name), report_bytes())
        if request:
            write(os.path.join(self.tmp, "evidence", "t1-functional",
                               "request.json"), request_bytes())

    # -- the real-tree case ---------------------------------------------

    def test_real_tree_agrees(self):
        # The case that bites during a bump: the committed evidence and
        # the committed FV_PINS must name the same toolchain.
        proc = self.run_check()
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("PASS", proc.stdout)

    # -- negative control (a): bumped klayout-tools pin ------------------

    def test_bumped_klayout_pin_fails_for_pin_reason(self):
        self.make_tree(makefile=PINS.replace("klayout-tools==0.8.0",
                                             "klayout-tools==0.9.0"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("provenance.klt_version", proc.stderr)
        self.assertIn("klayout-tools==0.9.0", proc.stderr)
        self.assertIn("klt 0.8.0", proc.stderr)
        self.assertIn("report.json", proc.stderr)
        self.assertIn("make functional-verification", proc.stderr)

    # -- negative control (b): bumped cocotb pin -------------------------

    def test_bumped_cocotb_pin_fails(self):
        self.make_tree(makefile=PINS.replace("cocotb==2.1.0",
                                             "cocotb==2.2.0"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("environment.cocotb_version", proc.stderr)
        self.assertIn("cocotb==2.2.0", proc.stderr)
        self.assertIn("2.1.0", proc.stderr)

    # -- negative control (c): matching pair passes ----------------------

    def test_matching_pair_passes(self):
        self.make_tree()
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("PASS", proc.stdout)

    # -- negative control (d): unparseable FV_PINS ------------------------

    def test_unparseable_fv_pins_is_not_a_pass(self):
        self.make_tree(makefile='"klayout-tools==0.8.0" cocotb-garbage')
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0, proc.stdout)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("FV_PINS", proc.stderr)

    def test_makefile_absent_could_not_run(self):
        self.make_tree()
        os.remove(os.path.join(self.tmp, "Makefile"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)

    def test_unquoted_pins_are_unparseable(self):
        self.make_tree(makefile="klayout-tools==0.8.0 cocotb==2.1.0")
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)

    def test_unknown_pin_distribution_could_not_run(self):
        self.make_tree(makefile=PINS + ' "iverilog==13.0"')
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0, proc.stdout)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("iverilog", proc.stderr)

    # -- request.json handling -------------------------------------------

    def test_request_without_provenance_is_skipped(self):
        self.make_tree()
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("request.json", proc.stdout)
        self.assertIn("skipped", proc.stdout)

    def test_no_provenance_bearing_report_could_not_run(self):
        self.make_tree(reports=())
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)

    def test_missing_evidence_dir_could_not_run(self):
        write(os.path.join(self.tmp, "Makefile"), f"FV_PINS = {PINS}\n")
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)

    # -- stated verdicts, never silent passes -----------------------------

    def test_missing_klt_version_fails(self):
        self.make_tree()
        write(os.path.join(self.tmp, "evidence", "t1-functional",
                           "report.json"), report_bytes(with_klt=False))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("klt_version", proc.stderr)

    def test_malformed_klt_version_fails(self):
        self.make_tree()
        # A bare version without the "klt " prefix is malformed, not a
        # match: compare the token of "klt <version>", never the raw tail.
        write(os.path.join(self.tmp, "evidence", "t1-functional",
                           "report.json"), report_bytes(klt="0.8.0"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)

    def test_unreadable_json_report_fails(self):
        self.make_tree()
        write(os.path.join(self.tmp, "evidence", "t1-functional",
                           "report.json"), b"{not json")
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)

    def test_report_without_environment_fails_cocotb(self):
        self.make_tree()
        write(os.path.join(self.tmp, "evidence", "t1-functional",
                           "report.json"),
              json.dumps({"status": "pass",
                          "provenance": {"klt_version": "klt 0.8.0"}})
              .encode("utf-8"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("environment", proc.stderr)

    # -- parser tolerance --------------------------------------------------

    def test_reformatted_reordered_fv_pins_still_parses(self):
        # Different operator, reversed pin order, extra spacing, a
        # backslash continuation and a trailing comment all still parse.
        # (Single quotes are not Makefile quoting: a bare 'x' on the
        # assignment is outside-text and must stay unparseable.)
        self.make_tree()
        write(os.path.join(self.tmp, "Makefile"),
              "FV_PINS := \\\n  \"cocotb==2.1.0\""
              '  "klayout-tools==0.8.0"  # the FV venv pins\n')
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 0, proc.stderr)


if __name__ == "__main__":
    unittest.main()
