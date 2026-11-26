"""Build command and live reproduction tests for the H03 SPI probe (#339).

These run in `make test`. The live reproduction guarded-skips (NOT_RUN)
when Verilator is absent. The build command test runs on every host.
"""

import os
import shlex
import shutil
import subprocess
import unittest
from importlib.util import module_from_spec, spec_from_file_location
from unittest.mock import patch

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNNER = os.path.join(REPO, "evidence", "h03-event-admission", "run_probe.py")


class TestH03AdmissionProbeBuild(unittest.TestCase):
    def test_records_executed_command_with_clang_present(self):
        spec = spec_from_file_location("h03_probe", RUNNER)
        probe = module_from_spec(spec)
        spec.loader.exec_module(probe)
        with (
            patch.object(
                probe.shutil, "which", side_effect=lambda name: f"/bin/{name}"
            ),
            patch.object(probe, "CASES", ()),
            patch.object(
                probe.subprocess,
                "run",
                side_effect=lambda cmd, **kw: subprocess.CompletedProcess(
                    cmd, 0, stdout="Verilator test version\n"
                ),
            ) as run,
        ):
            report = probe.build_and_run(jobs=3)

        argv = run.call_args.args[0]
        self.assertEqual(run.call_args.kwargs["cwd"], REPO)
        self.assertFalse(any(arg.startswith("CXX=") for arg in argv), argv)
        self.assertFalse(any(os.path.isabs(arg) for arg in argv), argv)
        self.assertEqual(argv[argv.index("-j") + 1], "3")
        self.assertEqual(shlex.split(report["simulator"]["build_command"]), argv)


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
