"""Issue #234: tools/host_frozen_int.py resolves to the single owner in
src/gf180_dx7/host.py.

The detune/fine derivations are NUM-008 float-gated frozen integers
(dx7note.cc:46-53): the float math stays host-side and only the frozen
integers cross the RTL boundary. Issue #158 consolidated the byte-identical
copies in h06_compare.py / h07_compare.py into tools/host_frozen_int.py,
which left a third copy in the production host; issue #234 makes
src/gf180_dx7/host.py the single owner and turns the tool-facing names
into re-exports.

These tests keep the tool-facing names tied to the owner:

  * an equality sweep over the full admissible domain (field bounds, not
    chosen: osc_detune 0-14 and osc_freq_fine 0-99 per sysex.py, transpose
    0-48 per sysex.py, so midinote + (transpose - 24) spans [-24, 151];
    2640 detune rows + 100 fine rows), and
  * an object-identity assertion that the re-exports have not been
    replaced by a local body.

A one-sided edit of either copy, or a "helpful" restoration of a local
body in tools/host_frozen_int.py, must fail these (live negative
controls; both are demonstrated failing in the #234 PR).
"""

import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
for _sub in ("src", "tools"):
    if str(REPO / _sub) not in sys.path:
        sys.path.insert(0, str(REPO / _sub))

import host_frozen_int  # noqa: E402
from gf180_dx7 import host  # noqa: E402
from gf180_dx7.model.pitch import midinote_to_logfreq  # noqa: E402

# Declared sweep domain (issue #234, C2): derived from the field bounds
# (sysex.py osc_detune 0-14, osc_freq_fine 0-99, transpose 0-48; host.py
# shift = transpose - 24), so the logfreq argument spans [-24, 151] --
# 176 distinct logfreq values, [17303353, 261971028].
N_RANGE = range(-24, 152)
DETUNE_RANGE = range(0, 15)
FINE_RANGE = range(0, 100)
LOGFREQ_LOW, LOGFREQ_HIGH = 17303353, 261971028


class TestDetuneSweep(unittest.TestCase):
    """2640 rows: the tool-facing detune delta equals the owner's."""

    def test_detune_sweep(self):
        logfreqs = [midinote_to_logfreq(n) for n in N_RANGE]
        self.assertEqual(len(set(logfreqs)), 176,
                         "domain drift: expected 176 distinct logfreqs")
        self.assertEqual(min(logfreqs), LOGFREQ_LOW)
        self.assertEqual(max(logfreqs), LOGFREQ_HIGH)
        rows = 0
        for logfreq in logfreqs:
            for detune in DETUNE_RANGE:
                with self.subTest(logfreq=logfreq, detune=detune):
                    self.assertEqual(
                        host_frozen_int.host_detune_delta(logfreq, detune),
                        host.detune_delta(logfreq, detune))
                rows += 1
        self.assertEqual(rows, 2640)

    def test_detune_neutral_branches(self):
        """Named edges inside the swept domain: detune == 7 is a zero
        delta at the domain endpoints (fine == 0 is TestFineSweep's)."""
        for n in (-24, 151):
            logfreq = midinote_to_logfreq(n)
            self.assertEqual(
                host_frozen_int.host_detune_delta(logfreq, 7),
                host.detune_delta(logfreq, 7))
            self.assertEqual(
                host_frozen_int.host_detune_delta(logfreq, 7), 0)


class TestFineSweep(unittest.TestCase):
    """100 rows: the tool-facing fine term equals the owner's, covering
    the fine == 0 early-return branch present in both copies."""

    def test_fine_sweep(self):
        for fine in FINE_RANGE:
            with self.subTest(fine=fine):
                self.assertEqual(
                    host_frozen_int.host_fine_term(fine),
                    host.fine_term(fine))


class TestReexportIdentity(unittest.TestCase):
    """The tool-facing names must BE the owner's implementations.

    Equality alone cannot tell a re-export from a value-identical local
    copy; a local body in tools/host_frozen_int.py -- even a byte-faithful
    one -- must fail here, because nothing then keeps it from drifting
    one-sided later (the gap issue #234 closes).
    """

    def test_tool_names_resolve_to_owner(self):
        self.assertIs(host_frozen_int.host_detune_delta, host.detune_delta)
        self.assertIs(host_frozen_int.host_fine_term, host.fine_term)


if __name__ == "__main__":
    unittest.main()
