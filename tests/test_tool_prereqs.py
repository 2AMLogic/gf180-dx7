"""tests/tool_prereqs.py: the RTL suites' shared Icarus / Yosys checks.

Hermetic: binaries are stub executables on a private PATH and the PDK is a
fake tree behind the resolver's own seams (candidate_roots, LIB_SHA256), so
nothing here needs or runs a real tool. Pins the exact NOT_RUN reasons the
H01, H04, H05 and H06 suites report, that each suite's prerequisite state is
the shared check's return value, that the check is implemented once, and
that a missing prerequisite skips every live case in those suites
(issue #354).
"""

import contextlib
import hashlib
import importlib
import os
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tool_prereqs as TP  # noqa: E402

PDK = TP.pdk_resolve
GOOD = b"fake liberty bytes (the pin is patched to their digest)"
SKIP = "NOT_RUN (guarded skip): "
ICARUS_REASON = SKIP + "iverilog not on PATH; vvp not on PATH"
YOSYS_REASON = "yosys binary not on PATH"
LIBERTY_REASON = "ciel 7t liberty not found: "

ICARUS_SUITES = (
    "test_h01.TestEquivalence",
    "test_h04.TestEquivalence",
    "test_h04.TestMutantsOnlyVerdict",
    "test_h05.TestConformance",
    "test_h06.TestEquivalence",
)
YOSYS_SUITES = (
    "test_h01.TestSynthesis",
    "test_h04.TestSynthesis",
    "test_h05.TestSynthesis",
    "test_h06.TestSynthesis",
)
ICARUS_STATE = ("iverilog", "vvp", "reason", "can_run")
YOSYS_STATE = ("yosys", "lib", "reason", "can_run")


def suite_class(name):
    module, _, klass = name.rpartition(".")
    return getattr(importlib.import_module(module), klass)


@contextlib.contextmanager
def preserved(suite, attrs):
    """Restore the suite's class attributes on exit."""
    with contextlib.ExitStack() as stack:
        for attr in attrs:
            stack.enter_context(mock.patch.object(
                suite, attr, getattr(suite, attr, None), create=True))
        yield


class _Outcomes(unittest.TestResult):
    def __init__(self):
        super().__init__()
        self.seen = []

    def addSuccess(self, test):
        super().addSuccess(test)
        self.seen.append((test.id(), "ok", None))

    def addSkip(self, test, reason):
        super().addSkip(test, reason)
        self.seen.append((test.id(), "skip", reason))

    def addFailure(self, test, err):
        super().addFailure(test, err)
        self.seen.append((test.id(), "FAIL", None))

    def addError(self, test, err):
        super().addError(test, err)
        self.seen.append((test.id(), "ERROR", None))


class _Env(unittest.TestCase):
    """A private PATH and PDK candidates: nothing on the machine leaks in."""

    def setUp(self):
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.tmp = os.path.realpath(tmp.name)
        self.bin_dir = os.path.join(self.tmp, "bin")
        os.mkdir(self.bin_dir)
        self.absent = os.path.join(self.tmp, "absent")
        self.root = os.path.join(self.tmp, "pdk")
        self.liberty = os.path.join(self.root, PDK.SC_LIB_REL)
        for patcher in (
            mock.patch.dict(os.environ, {"PATH": self.bin_dir}),
            mock.patch.object(PDK, "candidate_roots",
                              return_value=[self.absent, self.root]),
            mock.patch.object(PDK, "LIB_SHA256",
                              hashlib.sha256(GOOD).hexdigest()),
        ):
            patcher.start()
            self.addCleanup(patcher.stop)
        self.addCleanup(PDK._sha256.cache_clear)
        PDK._sha256.cache_clear()

    def binary(self, name):
        """A stub executable: present on PATH, never meant to run."""
        path = os.path.join(self.bin_dir, name)
        with open(path, "w", encoding="ascii") as f:
            f.write("#!/bin/sh\nexit 0\n")
        os.chmod(path, 0o755)
        return path

    def pdk_tree(self, data=GOOD):
        os.makedirs(os.path.dirname(self.liberty))
        with open(self.liberty, "wb") as f:
            f.write(data)

    def run_suites(self, suites):
        """[(case id, outcome, reason)] for every case of the suites."""
        seen = []
        for name in suites:
            suite = suite_class(name)
            with preserved(suite, ICARUS_STATE + YOSYS_STATE):
                result = _Outcomes()
                unittest.defaultTestLoader.loadTestsFromTestCase(
                    suite).run(result)
            self.assertGreaterEqual(
                len(result.seen), 1, f"{name} contributed no test case")
            seen.extend(result.seen)
        return seen


class IcarusCheckTests(_Env):
    def test_both_binaries_resolve_and_nothing_is_missing(self):
        iverilog, vvp = self.binary("iverilog"), self.binary("vvp")
        self.assertEqual(TP.icarus(), (iverilog, vvp, ""))

    def test_missing_binaries_are_all_named(self):
        self.assertEqual(TP.icarus(), (None, None, ICARUS_REASON))

    def test_missing_vvp_is_named_alone(self):
        iverilog = self.binary("iverilog")
        self.assertEqual(TP.icarus(),
                         (iverilog, None, SKIP + "vvp not on PATH"))

    def test_missing_iverilog_is_named_alone(self):
        vvp = self.binary("vvp")
        self.assertEqual(TP.icarus(),
                         (None, vvp, SKIP + "iverilog not on PATH"))


class YosysCheckTests(_Env):
    def test_yosys_and_pinned_liberty_resolve(self):
        yosys = self.binary("yosys")
        self.pdk_tree()
        self.assertEqual(TP.yosys(), (yosys, self.liberty, ""))

    def test_missing_yosys_is_named_with_the_liberty_present(self):
        self.pdk_tree()
        self.assertEqual(TP.yosys(),
                         (None, self.liberty, SKIP + YOSYS_REASON))

    def test_yosys_present_without_liberty_keeps_resolver_diagnostic(self):
        yosys = self.binary("yosys")
        diagnostic = PDK.missing_reason()
        self.assertIn(self.absent, diagnostic)
        self.assertIn(self.root, diagnostic)
        self.assertIn(hashlib.sha256(GOOD).hexdigest(), diagnostic)
        self.assertEqual(
            TP.yosys(),
            (yosys, os.path.join(PDK.NO_PDK_PLACEHOLDER, PDK.SC_LIB_REL),
             SKIP + LIBERTY_REASON + diagnostic))

    def test_wrong_liberty_bytes_are_a_missing_prerequisite(self):
        self.binary("yosys")
        self.pdk_tree(b"different liberty")
        self.assertIn("liberty sha256", PDK.missing_reason())
        self.assertEqual(TP.yosys()[2],
                         SKIP + LIBERTY_REASON + PDK.missing_reason())

    def test_missing_yosys_and_liberty_name_both_in_order(self):
        self.assertEqual(
            TP.yosys(),
            (None, os.path.join(PDK.NO_PDK_PLACEHOLDER, PDK.SC_LIB_REL),
             SKIP + YOSYS_REASON + "; " + LIBERTY_REASON
             + PDK.missing_reason()))

    def test_resolver_silence_falls_back_to_the_liberty_path(self):
        self.binary("yosys")
        with mock.patch.object(PDK, "missing_reason", return_value=None):
            _, liberty, reason = TP.yosys()
        self.assertIn(PDK.NO_PDK_PLACEHOLDER, liberty)
        self.assertEqual(reason, SKIP + LIBERTY_REASON + liberty)


class SuiteWiringTests(unittest.TestCase):
    """Each suite's prerequisite state is the shared check's return value."""

    def test_suites_bind_the_shared_icarus_check(self):
        paths = ("/sentinel/iverilog", "/sentinel/vvp")
        for name in ICARUS_SUITES:
            suite = suite_class(name)
            for reason in ("", SKIP + "sentinel"):
                with self.subTest(suite=name, reason=reason), \
                        preserved(suite, ICARUS_STATE), \
                        mock.patch.object(TP, "icarus",
                                          return_value=paths + (reason,)) \
                        as check:
                    suite.setUpClass()
                    check.assert_called_once_with()
                    self.assertEqual(
                        (suite.iverilog, suite.vvp, suite.reason,
                         suite.can_run),
                        paths + (reason, not reason))

    def test_suites_bind_the_shared_yosys_check(self):
        paths = ("/sentinel/yosys", "/sentinel/liberty")
        for name in YOSYS_SUITES:
            suite = suite_class(name)
            for reason in ("", SKIP + "sentinel"):
                with self.subTest(suite=name, reason=reason), \
                        preserved(suite, YOSYS_STATE), \
                        mock.patch.object(TP, "yosys",
                                          return_value=paths + (reason,)) \
                        as check:
                    suite.setUpClass()
                    check.assert_called_once_with()
                    self.assertEqual(
                        (suite.yosys, suite.lib, suite.reason, suite.can_run),
                        paths + (reason, not reason))


class SingleImplementationTests(unittest.TestCase):
    """The prerequisite diagnostics live in the shared module only."""

    SUITES = ("test_h01.py", "test_h04.py", "test_h05.py", "test_h06.py")
    NEEDLES = ("shutil.which", "iverilog not on PATH", "vvp not on PATH",
               "yosys binary not on PATH", "ciel 7t liberty not found",
               "NOT_RUN (guarded skip)")

    @staticmethod
    def source(name):
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)), name)
        with open(path, "r", encoding="utf-8") as f:
            return f.read()

    def test_suites_carry_no_prerequisite_check_of_their_own(self):
        for name in self.SUITES:
            source = self.source(name)
            for needle in self.NEEDLES:
                self.assertNotIn(
                    needle, source, f"{name} still implements {needle!r}")

    def test_shared_module_carries_every_diagnostic(self):
        source = self.source("tool_prereqs.py")
        for needle in self.NEEDLES:
            self.assertIn(needle, source)


class SuiteSkipControlTests(_Env):
    """The suites' own live cases behind the shared checks."""

    def assert_skipped_on(self, seen, icarus_reason, yosys_reason):
        for case, outcome, reason in seen:
            expected = yosys_reason if ".TestSynthesis." in case \
                else icarus_reason
            self.assertEqual((outcome, reason), ("skip", expected), case)

    def test_missing_binaries_skip_every_live_case(self):
        seen = self.run_suites(ICARUS_SUITES + YOSYS_SUITES)
        self.assert_skipped_on(
            seen, ICARUS_REASON,
            SKIP + YOSYS_REASON + "; " + LIBERTY_REASON
            + PDK.missing_reason())

    def test_yosys_present_without_liberty_skips_synthesis_on_diagnostic(self):
        self.binary("yosys")
        seen = self.run_suites(ICARUS_SUITES + YOSYS_SUITES)
        self.assert_skipped_on(seen, ICARUS_REASON,
                               SKIP + LIBERTY_REASON + PDK.missing_reason())


if __name__ == "__main__":
    unittest.main(verbosity=2)
