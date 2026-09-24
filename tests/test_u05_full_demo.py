#!/usr/bin/env python3
"""U05 demo WAVE tests (issue #38) - WAVE GATE (DR-0009): run via
`make test` at judge approval and wave gates, NOT in tools/test_fast.sh
(deliberately: its module name does not carry the fast-lane token
u05_demo). Adds to the fast suite (test_u05_demo.py):

  - the FULL profile clean run (longer schedule, same gates);
  - the environment-gated dev32 patch variant: with the pinned AllTheWeb
    archive (DX7_ALLTHEWEB_ZIP) readable, the fixture runs through a
    REAL committed dev32 patch hash; without it the report is NOT_RUN
    (never PASS, never a silent skip, never a mislabeled REFUSE) - an
    environment gate reports itself.
"""

import json
import os
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
for p in (str(REPO / "src"), str(REPO / "tools"),
          str(REPO / "tools" / "demo")):
    if p not in sys.path:
        sys.path.insert(0, p)

import demo_fixture as B  # noqa: E402
import fixtures_dx7 as F  # noqa: E402

FIXTURE = REPO / "spec" / "demo-audition-v1.json"

# corpus/dev32.json entry 0 (SHRPSYNTH); the dev32 catalog is the pinned
# dev32 set (issue #5 corpus): 32 canonical hashes with paths, no inline
# voice bytes (the archive provides the bytes).
DEV32_TEST_HASH = "e619786fe3cc7b6be694470709f054aa129abd042d852efaba54b413aefbec6d"


class TestFullProfile(unittest.TestCase):
    def setUp(self):
        self._td = tempfile.TemporaryDirectory()
        self.tmp = Path(self._td.name)

    def tearDown(self):
        self._td.cleanup()

    def test_clean_full_passes_and_counts_match_pins(self):
        rp = self.tmp / "bench-full-clean.json"
        rc = B.run_bench(FIXTURE, "full", "none", False, report_path=rp)
        rep = json.loads(rp.read_text())
        self.assertEqual(rc, 0, rep.get("reason", ""))
        self.assertEqual(rep["status"], "PASS")
        fx, phrases = F.load_fixture(FIXTURE)
        play = F.assemble_playthrough(fx, phrases, "full")
        self.assertEqual(rep["counts_expected"], play["counts"])
        obs = rep["counts_observed"]
        self.assertEqual(obs["drain_writes"],
                         play["counts"]["total_write_windows"],
                         "the observed drain-write count drifted from the "
                         "pinned schedule")
        for gate, val in rep["gates"].items():
            self.assertTrue(val["pass"], gate)

    def test_full_profile_negative_control_drop_note_off(self):
        """The same named-gate discipline on the longer schedule: a
        dropped final note-off must trip its named gates."""
        rp = self.tmp / "bench-full-drop-note-off.json"
        rc = B.run_bench(FIXTURE, "full", "drop-note-off", True,
                         report_path=rp)
        rep = json.loads(rp.read_text())
        self.assertEqual(rc, 0, rep)
        self.assertTrue(rep["expect_fail_verification"]["passed"],
                        rep["expect_fail_verification"]["problems"])


class TestDev32Variant(unittest.TestCase):
    """Environment-gated: a real dev32 patch when the environment
    provides the pinned archive; an honest NOT_RUN otherwise. The gate
    reports itself - a NOT_RUN here is recorded evidence, and this test
    NEVER converts it to a pass."""

    def setUp(self):
        self._td = tempfile.TemporaryDirectory()
        self.tmp = Path(self._td.name)
        self._old_env = os.environ.get("DX7_ALLTHEWEB_ZIP")

    def tearDown(self):
        if self._old_env is None:
            os.environ.pop("DX7_ALLTHEWEB_ZIP", None)
        else:
            os.environ["DX7_ALLTHEWEB_ZIP"] = self._old_env

    def test_env_gate_reports_honestly(self):
        rp = self.tmp / "bench-dev32.json"
        rc = B.run_bench(FIXTURE, "full", "none", False,
                         patch_override=f"dev32:{DEV32_TEST_HASH}",
                         report_path=rp)
        rep = json.loads(rp.read_text())
        zip_env = os.environ.get("DX7_ALLTHEWEB_ZIP")
        if not zip_env or not Path(zip_env).exists():
            # gate not met: the report must say NOT_RUN, reason named
            self.assertEqual(rc, 2)
            self.assertEqual(rep["status"], "NOT_RUN")
            self.assertIn("AllTheWeb", rep.get("reason", ""))
            self.assertNotIn("PASS", [rep["status"]])
        else:
            # gate met: the run must be a real PASS on the REAL patch
            self.assertEqual(rc, 0, rep.get("reason", ""))
            self.assertEqual(rep["status"], "PASS")
            self.assertEqual(rep["patch"].get("canonical_hash"),
                             DEV32_TEST_HASH)


if __name__ == "__main__":
    unittest.main()