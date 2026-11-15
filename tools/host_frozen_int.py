"""Shared host-side frozen-integer derivations (issues #158, #234).

One owner for the dx7note.cc float-gated event-time spots (NUM-008) that
tools/h06_compare.py and tools/h07_compare.py both feed to the RTL: the
float math stays host-side and only the frozen integers cross the boundary.
Issue #158 moved the tool copies verbatim out of those two tools, which
used to carry byte-identical copies; issue #234 consolidated the remaining
third copy: the single owner is src/gf180_dx7/host.py (detune_delta,
fine_term) and the two tool-facing names below are re-exports of it.
Do not reintroduce a local body here — tests/test_host_frozen_int.py
asserts equality over the admissible domain AND object identity against
the owner, and a local copy fails both (the live negative control of
issue #234). Import after `src/` is on `sys.path` (all callers arrange
that).

Provenance notes carried from the pre-#234 bodies: host_detune_delta is
dx7note.cc:46-47 host-side float64 math, frozen integer result — the RTL
adds delta to its own identically-derived logfreq; host_fine_term is
dx7note.cc:50-53 host-side float math, frozen integer result.
"""

from __future__ import annotations

from gf180_dx7.host import detune_delta as host_detune_delta
from gf180_dx7.host import fine_term as host_fine_term
from gf180_dx7.model import pitch as P


def i32(x: int) -> int:
    return P.i32(x)


def host_bend_pb(bend_value: int, range_up: int = 3, range_dn: int = 3,
                 range_step: int = 0) -> int:
    """NUM-008 float-gated bend scaling (dx7note.cc:219-233), frozen
    integer result; master tune is added core-side."""
    return P.NotePitch._pitch_base(bend_value, range_up, range_dn,
                                   range_step, 0)[0]
