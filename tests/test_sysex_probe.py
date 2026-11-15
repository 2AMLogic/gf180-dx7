"""Fast-lane smoke test for tools/sysex_probe.py (issue #219).

The probe is the tree's only loose-.syx decode path and nothing executed it,
so its exit-code mapping and error classification could rot silently against
src/gf180_dx7/sysex.py. This suite encodes the observed contract as a
table of (input, rc, required stdout evidence) rows and asserts both the
exit code and the classification name — four distinct fixtures exit 1, so an
rc-only assertion could not tell BadChecksum from UnsupportedFormat.

The negative control simulates classification rot by stubbing
gf180_dx7.sysex.decode_message (the module object the probe itself calls) in
an in-process run and requires the shared expectation helper to fail; a
wrong-class stub (BadLength raised where BadChecksum is expected) also
demonstrates that an rc-only weakening of the assertion would stop failing.
"""

from __future__ import annotations

import contextlib
import importlib.util
import io
import subprocess
import sys
import types
import unittest
from pathlib import Path
from unittest.mock import patch

_REPO = Path(__file__).resolve().parents[1]

_spec = importlib.util.spec_from_file_location(
    "sysex_probe", _REPO / "tools" / "sysex_probe.py")
sysex_probe = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(sysex_probe)

FIXTURES = _REPO / "tests" / "fixtures"
NO_SUCH = "__no_such_file__.syx"

# Observed on origin/main @ 2e2ed01 (2026-11-15) and re-verified on the
# issue branch before this test was written. The framing row encodes the
# authoritative contract: a whole-file BadFraming is a classified codec
# error and exits 1; exit 2 is reserved for operational failure.
TABLE = [
    ("valid_voice.syx", 0, [
        "voice 'TESTVOICE1', algorithm 9, feedback 5",
        "1 decoded, 0 classified error(s)",
    ]),
    ("valid_bank.syx", 0, [
        "bank, 32 voices, first='BANKVCE000' last='BANKVCE031'",
        "1 decoded, 0 classified error(s)",
    ]),
    ("stream_voice_bank_voice.syx", 0, [
        "3 decoded, 0 classified error(s), 3 message(s) total",
    ]),
    ("bad_checksum_voice.syx", 1, ["BadChecksum"]),
    ("truncated_voice.syx", 1, ["BadLength"]),
    ("truncated_bank_31_voices.syx", 1, ["BadLength"]),
    ("unsupported_dx7ii_performance.syx", 1, ["UnsupportedFormat"]),
    ("bad_framing_voice.syx", 1, [
        "framing: FAILED",
        "0 messages decoded, file framing invalid",
    ]),
    (NO_SUCH, 2, ["error: cannot read "]),
    ("<no arguments>", 2, ["Usage: sysex_probe.py"]),
]
BAD_CHECKSUM_ROW = TABLE[3]


class SysexProbeSmokeTest(unittest.TestCase):
    """Subprocess runs cover the real sys.exit(main(sys.argv)) path."""

    def _expect_row(self, row, rc, stdout):
        """Assert one table row; shared with the negative control so the
        control exercises these real assertions, not a parallel copy."""
        name, expected_rc, substrings = row
        self.assertEqual(rc, expected_rc, f"{name}: exit code")
        for want in substrings:
            self.assertIn(want, stdout, f"{name}: stdout evidence")

    def _run_subprocess(self, *args):
        proc = subprocess.run(
            [sys.executable, "tools/sysex_probe.py", *args],
            cwd=_REPO, capture_output=True, text=True, timeout=60)
        return proc.returncode, proc.stdout

    def _run_inprocess(self, *args):
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            try:
                rc = sysex_probe.main(["sysex_probe.py", *args])
            except SystemExit as exc:  # probe_file raises SystemExit(2)
                rc = exc.code
        return rc, buf.getvalue()

    def test_probe_input_table(self):
        for row in TABLE:
            with self.subTest(input=row[0]):
                if row[0] == "<no arguments>":
                    rc, out = self._run_subprocess()
                else:
                    rc, out = self._run_subprocess(str(FIXTURES / row[0]))
                self._expect_row(row, rc, out)

    def test_inprocess_main_raises_systemexit_for_unreadable_path(self):
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            with self.assertRaises(SystemExit) as ctx:
                sysex_probe.main(["sysex_probe.py", str(FIXTURES / NO_SUCH)])
        self.assertEqual(ctx.exception.code, 2)
        self.assertIn("error: cannot read ", buf.getvalue())

    def test_negative_control_classification_rot_fails_expectation(self):
        row = BAD_CHECKSUM_ROW
        target = str(FIXTURES / row[0])

        with self.subTest("baseline: healthy probe passes the expectation"):
            rc, out = self._run_inprocess(target)
            self._expect_row(row, rc, out)

        voice_stub = types.SimpleNamespace(
            name="STUBVCE", algorithm=9, feedback=5, anomalies=[])
        with self.subTest("decode_message never raising: expectation fails"):
            with patch.object(sysex_probe.sysex, "decode_message",
                              lambda seg: voice_stub):
                rc, out = self._run_inprocess(target)
                self.assertEqual(rc, 0)
                self.assertNotIn(row[2][0], out)
                with self.assertRaises(AssertionError):
                    self._expect_row(row, rc, out)

        def wrong_class_stub(seg):
            raise sysex_probe.BadLength("simulated taxonomy rot: wrong class")

        with self.subTest("wrong-class rot: full expectation still fails"):
            with patch.object(sysex_probe.sysex, "decode_message",
                              wrong_class_stub):
                rc, out = self._run_inprocess(target)
                self.assertEqual(rc, row[1])
                self.assertNotIn(row[2][0], out)
                with self.assertRaises(AssertionError):
                    self._expect_row(row, rc, out)

        with self.subTest("weakened to rc-only, wrong-class rot is missed"):
            with patch.object(sysex_probe.sysex, "decode_message",
                              wrong_class_stub):
                rc, out = self._run_inprocess(target)
                self.assertEqual(rc, row[1])  # rc-only passes: the gap
                with self.assertRaises(AssertionError):
                    self.assertIn(row[2][0], out)  # name check catches it


if __name__ == "__main__":
    unittest.main()
