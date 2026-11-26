"""Build command and live reproduction tests for the H03 SPI probe (#339).

These run in `make test`. The live reproduction guarded-skips (NOT_RUN) when
Verilator is absent, and when the bench build fails the same way as the
independent timed-module build in tests/vlt_toolchain.py (#349). A broken
project bench on a working toolchain, an inconclusive diagnosis, a simulation
error and a committed-result mismatch all FAIL with the runner's own
diagnostic. The build command and verdict mapping tests need neither Verilator
nor a compiler.
"""

import contextlib
import copy
import io
import json
import os
import shlex
import shutil
import socket
import subprocess
import sys
import unittest
from importlib.util import module_from_spec, spec_from_file_location
from unittest import mock
from unittest.mock import patch

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNNER = os.path.join(REPO, "evidence", "h03-event-admission", "run_probe.py")

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vlt_toolchain as VT  # noqa: E402


def load_runner():
    spec = spec_from_file_location("h03_probe", RUNNER)
    probe = module_from_spec(spec)
    spec.loader.exec_module(probe)
    return probe


PROBE = load_runner()
LIVE_TEST = "test_probe_reproduces_committed_results"
BUILD_ERROR = (
    "verilator build failed (exit 2):\n"
    "c++: error: unrecognized command-line option '-fbracket-depth=4096'\n"
    "make: *** [verilated.mk:299: verilated.o] Error 1"
)
TOOLCHAIN_WHY = (
    "independent timed-module build also failed (C++ toolchain unusable; "
    "try CXX=<compiler> e.g. `make test CXX=clang++`): "
    "c++: error: unrecognized command-line option '-fbracket-depth=4096'"
)


def check_verdict(proc, vlt):
    """Raise the skip for a diagnosed toolchain failure, else the failure.

    Only the runner's bench-build exit code triggers the independent
    diagnosis. TOOLCHAIN is NOT_RUN; PROJECT and INCONCLUSIVE, like every
    other nonzero exit, FAIL with the runner's original output.
    """
    if proc.returncode == 0:
        return
    output = (
        f"probe check failed (exit {proc.returncode}):\n"
        f"{proc.stdout}\n{proc.stderr}"
    )
    if proc.returncode != PROBE.BuildFailure.EXIT_CODE:
        raise AssertionError(output)
    verdict, why = VT.diagnose(vlt)
    if verdict == VT.TOOLCHAIN:
        raise unittest.SkipTest(
            f"NOT_RUN (toolchain): host {socket.gethostname()}: {why}\n{output}"
        )
    raise AssertionError(f"verilator build failed [{verdict}: {why}]:\n{output}")


def run_live(returncode, stdout="", stderr="", verdicts=None,
             verilator="/bin/verilator"):
    """Drive the live test against a faked runner process and diagnosis."""
    result = unittest.TestResult()
    proc = subprocess.CompletedProcess([], returncode, stdout, stderr)
    with (
        patch.object(shutil, "which", return_value=verilator),
        patch.object(socket, "gethostname", return_value="worker-test"),
        patch.object(subprocess, "run", return_value=proc) as run,
        patch.object(VT, "diagnose", side_effect=verdicts) as diagnose,
    ):
        TestH03AdmissionProbeLive(LIVE_TEST).run(result)
    return result, run, diagnose


class TestH03AdmissionProbeBuild(unittest.TestCase):
    def test_records_executed_command_with_clang_present(self):
        probe = PROBE
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

    def test_failed_bench_build_raises_build_failure_before_any_case_runs(self):
        def fake_run(cmd, **kw):
            if "--version" in cmd:
                return subprocess.CompletedProcess(
                    cmd, 0, stdout="Verilator test version\n"
                )
            self.assertIn("--binary", cmd, f"ran after a failed build: {cmd}")
            return subprocess.CompletedProcess(
                cmd, 2, stdout="",
                stderr="c++: error: unrecognized command-line option "
                       "'-fbracket-depth=4096'\n",
            )

        with (
            patch.object(PROBE.shutil, "which", return_value="/bin/verilator"),
            patch.object(PROBE.subprocess, "run", side_effect=fake_run) as run,
        ):
            with self.assertRaises(PROBE.BuildFailure) as raised:
                PROBE.build_and_run(jobs=2)

        self.assertIsInstance(raised.exception, RuntimeError)
        self.assertEqual(run.call_count, 2)
        self.assertIn("verilator build failed (exit 2)", str(raised.exception))
        self.assertIn("-fbracket-depth=4096", str(raised.exception))

    def test_check_exits_nonzero_whenever_it_cannot_verify_evidence(self):
        committed = json.loads(PROBE.RESULTS_PATH.read_text(encoding="utf-8"))
        drifted = copy.deepcopy(committed)
        drifted["exercised_runs"][0]["max_busy_clks"] += 1
        cases = (
            ("verilator absent", None, mock.Mock(), 2,
             "NOT_RUN: verilator not found on PATH"),
            ("bench build failed", "/bin/verilator",
             mock.Mock(side_effect=PROBE.BuildFailure(BUILD_ERROR)), 3,
             BUILD_ERROR),
            ("committed results drifted", "/bin/verilator",
             mock.Mock(return_value=drifted), 1,
             "FAIL: results.json mismatch in exercised_runs"),
        )
        for name, which, build_and_run, code, diagnostic in cases:
            with self.subTest(case=name):
                err = io.StringIO()
                with (
                    patch.object(PROBE.shutil, "which", return_value=which),
                    patch.object(PROBE, "build_and_run", build_and_run),
                    contextlib.redirect_stderr(err),
                ):
                    self.assertEqual(PROBE.main(["--check"]), code)
                self.assertIn(diagnostic, err.getvalue())
                if which is None:
                    build_and_run.assert_not_called()
                else:
                    build_and_run.assert_called_once_with(jobs=2)

    def test_write_reports_a_build_failure_instead_of_raising(self):
        err = io.StringIO()
        with (
            patch.object(PROBE.shutil, "which", return_value="/bin/verilator"),
            patch.object(
                PROBE,
                "build_and_run",
                mock.Mock(side_effect=PROBE.BuildFailure(BUILD_ERROR)),
            ),
            contextlib.redirect_stderr(err),
        ):
            self.assertEqual(PROBE.main(["--write"]), 3)
        self.assertIn(BUILD_ERROR, err.getvalue())


class TestH03AdmissionProbeVerdict(unittest.TestCase):
    def test_reproduced_results_pass_without_a_diagnosis(self):
        result, _, diagnose = run_live(
            0, stdout="h03 admission probe check PASS (9 runs reproduced)\n"
        )
        self.assertEqual(
            (result.failures, result.errors, result.skipped), ([], [], [])
        )
        diagnose.assert_not_called()

    def test_toolchain_build_failure_is_a_not_run_skip_naming_host_and_fix(self):
        result, run, diagnose = run_live(
            PROBE.BuildFailure.EXIT_CODE,
            stderr=f"BUILD FAILURE: {BUILD_ERROR}",
            verdicts=[(VT.TOOLCHAIN, TOOLCHAIN_WHY)],
        )
        self.assertEqual((result.failures, result.errors), ([], []))
        self.assertEqual(len(result.skipped), 1)
        reason = result.skipped[0][1]
        self.assertTrue(reason.startswith("NOT_RUN"), reason)
        self.assertIn("host worker-test", reason)
        self.assertIn("CXX=clang++", reason)
        self.assertIn(BUILD_ERROR, reason)
        run.assert_called_once()
        self.assertEqual(
            run.call_args.args[0], ["python3", RUNNER, "--check", "-j", "2"]
        )
        self.assertEqual(run.call_args.kwargs["cwd"], REPO)
        self.assertNotIn("env", run.call_args.kwargs)
        diagnose.assert_called_once_with("/bin/verilator")

    def test_project_build_failure_fails_with_the_runner_diagnostic(self):
        result, _, diagnose = run_live(
            PROBE.BuildFailure.EXIT_CODE,
            stderr=f"BUILD FAILURE: {BUILD_ERROR}",
            verdicts=[(VT.PROJECT, "independent timed-module build and run "
                       "succeeded; the project bench failure is real")],
        )
        self.assertEqual((result.skipped, result.errors), ([], []))
        self.assertEqual(len(result.failures), 1)
        failure = result.failures[0][1]
        self.assertIn(f"[{VT.PROJECT}:", failure)
        self.assertIn("probe check failed (exit 3)", failure)
        self.assertIn(BUILD_ERROR, failure)
        diagnose.assert_called_once_with("/bin/verilator")

    def test_inconclusive_diagnosis_fails_with_the_runner_diagnostic(self):
        result, _, diagnose = run_live(
            PROBE.BuildFailure.EXIT_CODE,
            stderr=f"BUILD FAILURE: {BUILD_ERROR}",
            verdicts=[(VT.INCONCLUSIVE, "smoke probe did not complete: "
                       "TimeoutExpired()")],
        )
        self.assertEqual((result.skipped, result.errors), ([], []))
        self.assertEqual(len(result.failures), 1)
        failure = result.failures[0][1]
        self.assertIn(f"[{VT.INCONCLUSIVE}:", failure)
        self.assertIn(BUILD_ERROR, failure)
        diagnose.assert_called_once_with("/bin/verilator")

    def test_other_nonzero_exits_fail_without_a_diagnosis(self):
        for returncode, stderr in (
            (1, "FAIL: results.json mismatch in exercised_runs"),
            (1, "RuntimeError: case burst_100_sustained failed (exit 1)"),
            (2, "NOT_RUN: verilator not found on PATH"),
            (-9, ""),
        ):
            with self.subTest(returncode=returncode, stderr=stderr):
                result, _, diagnose = run_live(
                    returncode,
                    stderr=stderr,
                    verdicts=[(VT.TOOLCHAIN, TOOLCHAIN_WHY)],
                )
                self.assertEqual((result.skipped, result.errors), ([], []))
                self.assertEqual(len(result.failures), 1)
                failure = result.failures[0][1]
                self.assertIn(f"probe check failed (exit {returncode})", failure)
                self.assertIn(stderr, failure)
                diagnose.assert_not_called()

    def test_missing_verilator_skips_before_launching_the_runner(self):
        result, run, diagnose = run_live(0, verilator=None)
        self.assertEqual((result.failures, result.errors), ([], []))
        self.assertEqual(len(result.skipped), 1)
        self.assertIn("NOT_RUN (guarded skip)", result.skipped[0][1])
        run.assert_not_called()
        diagnose.assert_not_called()

    def test_zero_exit_without_a_pass_line_fails(self):
        result, _, diagnose = run_live(0, stdout="")
        self.assertEqual((result.errors, result.skipped), ([], []))
        self.assertEqual(len(result.failures), 1)
        diagnose.assert_not_called()


class TestH03AdmissionProbeLive(unittest.TestCase):
    def test_probe_reproduces_committed_results(self):
        vlt = shutil.which("verilator")
        if not vlt:
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
        check_verdict(proc, vlt)
        self.assertIn("PASS", proc.stdout)


if __name__ == "__main__":
    unittest.main()
