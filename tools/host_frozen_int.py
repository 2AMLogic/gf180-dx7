"""Shared host-side frozen-integer derivations (issue #158).

One owner for the dx7note.cc float-gated event-time spots (NUM-008) that
tools/h06_compare.py and tools/h07_compare.py both feed to the RTL: the
float math stays host-side and only the frozen integers cross the boundary.
Moved verbatim out of those two tools, which used to carry byte-identical
copies. Import after `src/` is on `sys.path` (both callers arrange that).
"""

from __future__ import annotations

import math

from gf180_dx7.model import pitch as P


def i32(x: int) -> int:
    return P.i32(x)


def host_fine_term(fine: int) -> int:
    """dx7note.cc:50-53: host-side float math, frozen integer result."""
    if fine == 0:
        return 0
    return int(math.floor(24204406.323123 * math.log(1 + 0.01 * fine) + 0.5))


def host_detune_delta(logfreq: int, detune: int) -> int:
    """dx7note.cc:46-47: host-side float64 math, frozen integer result.
    detuned = int(logfreq + ratio*logfreq*(detune-7)); delta = detuned -
    logfreq. The RTL adds delta to its own identically-derived logfreq."""
    ratio = 0.0209 * math.exp(-0.396 * (float(logfreq) / (1 << 24))) / 7
    detuned = i32(int(logfreq + ratio * logfreq * (detune - 7)))
    return detuned - logfreq


def host_bend_pb(bend_value: int, range_up: int = 3, range_dn: int = 3,
                 range_step: int = 0) -> int:
    """NUM-008 float-gated bend scaling (dx7note.cc:219-233), frozen
    integer result; master tune is added core-side."""
    return P.NotePitch._pitch_base(bend_value, range_up, range_dn,
                                   range_step, 0)[0]
