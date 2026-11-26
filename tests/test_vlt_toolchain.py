"""Regression coverage for the Verilator toolchain-vs-project diagnosis
(#343). Runs without Verilator: the build is faked through `run`."""
import os
import subprocess
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vlt_toolchain as VT  # noqa: E402


def cp(rc=0, out="", err=""):
    return subprocess.CompletedProcess([], rc, out, err)


def fake(build_rc, build_creates=True, run_rc=0, run_out="SMOKE_OK\n",
         raises=None):
    def run(cmd, **kw):
        if raises:
            raise raises
        if cmd[0] != "./smoke" and "--binary" in cmd:
            if build_creates and build_rc == 0:
                mdir = cmd[cmd.index("--Mdir") + 1]
                os.makedirs(mdir, exist_ok=True)
                open(os.path.join(mdir, "smoke"), "w").close()
            return cp(build_rc, "", "c++: error: unrecognized option")
        return cp(run_rc, run_out, "")
    return run


class TestDiagnose(unittest.TestCase):
    def test_toolchain_failure(self):
        v, why = VT.diagnose("vlt", run=fake(1))
        self.assertEqual(v, VT.TOOLCHAIN)
        self.assertIn("unrecognized option", why)
        self.assertIn("CXX=", why)

    def test_project_failure(self):
        v, _ = VT.diagnose("vlt", run=fake(0))
        self.assertEqual(v, VT.PROJECT)

    def test_smoke_run_bad_is_inconclusive(self):
        v, _ = VT.diagnose("vlt", run=fake(0, run_out="nope"))
        self.assertEqual(v, VT.INCONCLUSIVE)

    def test_probe_exception_is_inconclusive(self):
        v, _ = VT.diagnose("vlt", run=fake(0, raises=OSError("x")))
        self.assertEqual(v, VT.INCONCLUSIVE)
        v, _ = VT.diagnose("vlt", run=fake(
            0, raises=subprocess.TimeoutExpired("c", 1)))
        self.assertEqual(v, VT.INCONCLUSIVE)


if __name__ == "__main__":
    unittest.main()
