"""Wave-lane live Verilator reproduction test for H03 SPI admission probe (#339).

Not matched by tools/test_fast.sh `-k` filters (keeps make test-fast sub-second
per DR-0009); runs in `make test` and guarded-skips (NOT_RUN) when verilator is
not on PATH.
"""

import os
import shutil
import subprocess
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNNER = os.path.join(REPO, "evidence", "h03-event-admission", "run_probe.py")


class TestH03AdmissionProbeLive(unittest.TestCase):
    def test_probe_reproduces_committed_results(self):
        if not shutil.which("verilator"):
            self.skipTest(
                "NOT_RUN (guarded skip): verilator not installed on this runner"
            )
        proc = subprocess.run(
            ["python3", RUNNER, "--check", "-j", "2"],
            cwd=REPO,
            capture_output=True,
            text=True,
            timeout=600,
            check=False,
        )
        self.assertEqual(
            proc.returncode,
            0,
            f"probe check failed:\n{proc.stdout}\n{proc.stderr}",
        )
        self.assertIn("PASS", proc.stdout)


if __name__ == "__main__":
    unittest.main()
