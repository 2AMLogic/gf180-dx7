"""Single-class-identity tests for the shared verdict exceptions (issue #176).

The tools signal exit 1 (check failure) and exit 2 (could not run) by class,
and callers match them with `except`. These cases pin that every tool shares
the one class object, and that a copy-pasted local class (the pre-#176
shape) is caught by the control below.
"""

import subprocess
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
if str(REPO / "tools") not in sys.path:
    sys.path.insert(0, str(REPO / "tools"))

CHECK_AND_RUN = (
    "tools/h01_compare.py", "tools/h01_synth.py", "tools/h04_compare.py",
    "tools/h04_synth.py", "tools/h05_compare.py", "tools/h05_synth.py",
    "tools/h06_compare.py", "tools/h06_synth.py", "tools/h07_compare.py",
    "tools/h07_synth.py", "tools/h08_synth.py",
    "tools/storage_probe_synth.py",
)
RUN_ONLY = ("tools/h10_synth.py",)

# Loads each script by path in an isolated interpreter (tools/ added the way
# the existing suites do for sibling imports) and prints
# "<script> <CheckFailure-identical> <CouldNotRun-identical>".
_PROBE = """
import importlib.util, sys
tools = sys.argv[1]
sys.path.insert(0, tools)
import verdict_errors
for i, path in enumerate(sys.argv[2:]):
    spec = importlib.util.spec_from_file_location("probe_%d" % i, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = mod
    spec.loader.exec_module(mod)
    cf = getattr(mod, "CheckFailure", None) is verdict_errors.CheckFailure
    cnr = getattr(mod, "CouldNotRun", None) is verdict_errors.CouldNotRun
    print(path, cf, cnr)
"""


class VerdictErrorsContractTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        scripts = [str(REPO / rel) for rel in CHECK_AND_RUN + RUN_ONLY]
        proc = subprocess.run(
            [sys.executable, "-I", "-c", _PROBE, str(REPO / "tools"),
             *scripts],
            capture_output=True, text=True)
        assert proc.returncode == 0, proc.stderr[-800:]
        cls.rows = {line.split()[0]: line.split()[1:]
                    for line in proc.stdout.splitlines()}

    def test_contract_every_tool_shares_the_one_class(self):
        for rel in CHECK_AND_RUN:
            with self.subTest(script=rel):
                self.assertEqual(self.rows[str(REPO / rel)],
                                 ["True", "True"])
        for rel in RUN_ONLY:
            with self.subTest(script=rel):
                self.assertEqual(self.rows[str(REPO / rel)][1], "True")

    def test_contract_failure_raised_in_one_tool_is_caught_by_another(self):
        import verdict_errors
        import h01_compare
        import h06_synth
        self.assertIs(h01_compare.CheckFailure, h06_synth.CheckFailure)
        with self.assertRaises(h06_synth.CheckFailure):
            raise h01_compare.CheckFailure("x")
        with self.assertRaises(h06_synth.CouldNotRun):
            raise verdict_errors.CouldNotRun("x")

    def test_contract_control_local_copy_is_not_caught(self):
        # Live negative control: the pre-#176 per-file shape must fail the
        # identity check this suite relies on.
        import verdict_errors

        class CheckFailure(Exception):
            pass

        self.assertIsNot(CheckFailure, verdict_errors.CheckFailure)
        with self.assertRaises(CheckFailure):
            try:
                raise CheckFailure("x")
            except verdict_errors.CheckFailure:
                self.fail("local copy must not be caught by the shared class")


if __name__ == "__main__":
    unittest.main()
