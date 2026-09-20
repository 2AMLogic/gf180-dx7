"""Frozen integer single-operator model (N02, issue #16).

Contract: gf180-dx7-integer-v1 (spec/numeric-profile-v1.json,
docs/DECISIONS-NUMERIC-v1.md NUM-003/NUM-004/NUM-010). This module
re-implements, integer for integer, the pinned oracle's Mark I per-sample
operator path. The GPL engine file (Source/EngineMkI.cpp) is the semantic
SPECIFICATION only — facts (operation order, shifts, widths) are mirrored
here; no bytes are copied (NUM-001; docs/REUSE-RULINGS.md dexed-enginemki).

Per-sample operator (facts, EngineMkI.cpp:124-150):
    expVal    = sinLog(phase >> 12) + env          (uint16 wrap)
    isSigned  = expVal & 0x8000
    expVal   &= 0x7FFF
    result    = 4096 + sinExpTable[(expVal & 0x3FF) ^ 0x3FF]
    result  >>= expVal >> 10
    out       = isSigned ? (-result - 1) << 13 : result << 13

with the quarter-wave fold (EngineMkI.cpp:64-78):
    index = phi & 1023, quadrant = phi & 3072:
      0    -> table[index]
      1024 -> table[index ^ 1023]
      2048 -> table[index] | 0x8000
      3072 -> table[index ^ 1023] | 0x8000

Frame (facts, EngineMkI.cpp:152-205): 64 samples per frame (N = 64, LG_N = 6,
msfa/synth.h:29-30); intra-frame gain ramp
    dgain = (gain2 - gain1 + (N >> 1)) >> LG_N
    gain starts at gain1 and advances BEFORE each sample (gain_i = gain1 +
    i*dgain, i = 1..64); phase wraps modulo 2^32; at frame end the carried
    phase skips the whole frame: phase += freq << LG_N (EngineMkI.cpp:361,
    msfa/fm_core.cc:133).

Single-operator feedback input (interface only; routing/policy is N04):
    scaled_fb = (y0 + y) >> (fb_shift + 1)   (EngineMkI.cpp:196)
with the two-element int32 history carried across frames
(EngineMkI.cpp:203-204; msfa/dx7note.h:77).

Output conversion for a single-note block (facts, PluginProcessor.cpp:258-268
and the oracle mirror main.cpp:455-465, frozen as NUM-010):
    val = block >> 4
    clip_val = val < -(1<<24) ? 0x8000 : val >= (1<<24) ? 0x7fff : val >> 9
    f = clip_val / 0x8000 (float32; the negative-overflow quirk maps to
    +32768 => +1.0)
The product transport hands off clip_val; comparison renders are f32le.

Stdlib only.
"""

from __future__ import annotations

import struct

from .tables import (FREQLUT_MAX_LOGFREQ_INT, FREQLUT_N_SAMPLES,
                     FREQLUT_SAMPLE_SHIFT, TableArtifact)

N = 64  # msfa/synth.h:30 (LG_N = 6)
LG_N = 6

NEGATIVE_BIT = 0x8000          # EngineMkI.cpp:51
ENV_BITDEPTH = 14              # EngineMkI.cpp:52
SINLOG_BITDEPTH = 10           # EngineMkI.cpp:54
SINLOG_TABLESIZE = 1 << SINLOG_BITDEPTH   # 1024
SINLOG_TABLEFILTER = SINLOG_TABLESIZE - 1  # EngineMkI.cpp:65
SINEXP_BITDEPTH = 10           # EngineMkI.cpp:58
SINEXP_TABLESIZE = 1 << SINEXP_BITDEPTH   # 1024
ENV_MAX = 1 << ENV_BITDEPTH    # 16384, EngineMkI.cpp:62

_K_LEVEL_THRESH = ENV_MAX - 100  # EngineMkI.cpp:296 (NUM-009)

_U32 = 0xFFFFFFFF


def _i32(v: int) -> int:
    """Wrap to C int32 (two's complement)."""
    v &= _U32
    return v - (1 << 32) if v >= (1 << 31) else v


def sin_log_lookup(phi: int, log_table) -> int:
    """Quarter-wave fold + sign bit (EngineMkI.cpp:64-78).

    phi is the uint16 truncation of (phase >> (22 - SINLOG_BITDEPTH)); the
    caller performs that conversion (see mki_sin).
    """
    index = phi & SINLOG_TABLEFILTER
    quadrant = phi & (SINLOG_TABLESIZE * 3)
    if quadrant == 0:
        return log_table[index]
    if quadrant == SINLOG_TABLESIZE:
        return log_table[index ^ SINLOG_TABLEFILTER]
    if quadrant == SINLOG_TABLESIZE * 2:
        return log_table[index] | NEGATIVE_BIT
    return log_table[index ^ SINLOG_TABLEFILTER] | NEGATIVE_BIT


def mki_sin(phase: int, env: int, log_table, exp_table) -> int:
    """One Mark I operator evaluation (EngineMkI.cpp:124-150).

    phase: any int; only bits 12..27 of the 32-bit accumulator reach the
    tables, so signed-int32 and uint32 bit patterns agree here. env: the
    14-bit frame gain (values are truncated to uint16 as in C++).
    """
    phi = (phase >> (22 - SINLOG_BITDEPTH)) & 0xFFFF
    exp_val = (sin_log_lookup(phi, log_table) + env) & 0xFFFF
    is_signed = bool(exp_val & NEGATIVE_BIT)
    exp_val &= ~NEGATIVE_BIT & 0xFFFF
    result = 4096 + exp_table[(exp_val & 0x3FF) ^ 0x3FF]
    result >>= exp_val >> 10
    if is_signed:
        return _i32((-result - 1) << 13)
    return _i32(result << 13)


class FeedbackHistory:
    """Two-element int32 feedback history (dx7note.h:77; NUM-006).

    The scaled sum and the shift policy are frozen here; algorithm routing
    and depth mapping (fb_shift) are N04's contract.
    """

    __slots__ = ("y0", "y1")

    def __init__(self, y0: int = 0, y1: int = 0) -> None:
        self.y0 = _i32(y0)
        self.y1 = _i32(y1)

    def scaled(self, fb_shift: int) -> int:
        """(y0 + y) >> (fb_shift + 1), arithmetic (EngineMkI.cpp:196)."""
        return _i32(self.y0 + self.y1) >> (fb_shift + 1)

    def store(self, y0: int, y1: int) -> None:
        """Carry back at frame end (EngineMkI.cpp:203-204)."""
        self.y0, self.y1 = _i32(y0), _i32(y1)


class MarkIOperator:
    """One operator's phase accumulator + per-frame render.

    The phase register is 32-bit wrapping, one cycle = 2^32 (NUM-003). The
    caller supplies the per-frame phase increment `freq` (int32, from the
    pitch path) and the 14-bit frame gains gain1/gain2 (the envelope and
    gating composition around them is NUM-005/NUM-009; N03 owns envelopes).
    """

    __slots__ = ("phase",)

    def __init__(self, phase: int = 0) -> None:
        self.phase = phase & _U32

    def advance_frame(self, freq: int) -> None:
        """Frame-boundary phase skip (EngineMkI.cpp:361, fm_core.cc:133)."""
        self.phase = (self.phase + (_i32(freq) << LG_N)) & _U32

    def render_frame(self, log_table, exp_table, freq: int, gain1: int,
                     gain2: int, *, modulation=None, add: bool = False,
                     bus=None, fb_history: FeedbackHistory | None = None,
                     fb_shift: int = 16) -> list[int]:
        """Render one 64-sample frame, mirroring EngineMkI.cpp:152-205
        (compute / compute_pure / compute_fb).

        The carried register is left at phase0 + 64*freq: the C++ frame
        functions advance a LOCAL phase per sample (EngineMkI.cpp:164,180,
        200) and the frame skip `phase += freq << LG_N` (EngineMkI.cpp:361)
        lands on the same value, so both views agree by construction.
        advance_frame() exists for the gated-op path, where line 361 runs
        with no render at all.

        modulation: per-sample int32 input block (the `compute` form,
        EngineMkI.cpp:162) — algorithm bus wiring (N04) supplies it.
        fb_history: when given, the single-operator feedback loop
        (compute_fb, EngineMkI.cpp:184-205) reads scaled feedback each
        sample and stores the carried history back at frame end.
        add/bus: accumulate onto `bus` instead of overwriting
        (EngineMkI.cpp:158 `adder = add ? output : zeros`).
        """
        dgain = (gain2 - gain1 + (N >> 1)) >> LG_N   # EngineMkI.cpp:155
        gain = gain1
        phase = self.phase & _U32
        out = []
        freq = _i32(freq)
        for i in range(N):
            gain += dgain
            if fb_history is not None:
                scaled_fb = fb_history.scaled(fb_shift)
                fb_history.y0 = fb_history.y1
                y = mki_sin(phase + scaled_fb, gain, log_table, exp_table)
                fb_history.y1 = y
            else:
                p_in = phase if modulation is None else phase + _i32(
                    modulation[i])
                y = mki_sin(p_in, gain, log_table, exp_table)
            if add and bus is not None:
                out.append(_i32(y + bus[i]))
            else:
                out.append(y)
            phase = (phase + freq) & _U32
        self.phase = phase
        return out


def clip_val_domain(block) -> list[int]:
    """Per-note output in the product transport domain (NUM-010).

    val = block >> 4; clip to +/-2^24 with the pinned negative-overflow
    quirk (val < -(1<<24) maps to +0x8000); else val >> 9
    (PluginProcessor.cpp:261-263; oracle main.cpp:457-459).
    """
    out = []
    for v in block:
        val = _i32(v) >> 4
        if val < -(1 << 24):
            out.append(0x8000)
        elif val >= (1 << 24):
            out.append(0x7FFF)
        else:
            out.append(val >> 9)
    return out


def clip_val_to_f32_bytes(clip_vals) -> bytes:
    """f = clip_val / 0x8000 as float32 (exact: quotient of int by 2^15),
    clamped to [-1, 1] (main.cpp:460-462). The 0x8000 quirk value maps to
    +1.0. Returns raw f32le bytes."""
    out = bytearray()
    for c in clip_vals:
        f = c / 32768.0 if c != 0x8000 else 1.0
        f = struct.unpack("<f", struct.pack("<f", f))[0]
        if f > 1.0:
            f = 1.0
        elif f < -1.0:
            f = -1.0
        out += struct.pack("<f", f)
    return bytes(out)


def block_to_f32_bytes(block) -> bytes:
    """Convenience: block -> clip_val domain -> f32le bytes."""
    return clip_val_to_f32_bytes(clip_val_domain(block))


class Freqlut:
    """Q24-octave logfreq -> 32-bit phase increment (freqlut.cc:46-55)."""

    __slots__ = ("lut",)

    def __init__(self, lut) -> None:
        if len(lut) != FREQLUT_N_SAMPLES + 1:
            raise ValueError("freqlut needs 1025 entries")
        self.lut = list(lut)

    @classmethod
    def from_artifact(cls, artifact: TableArtifact) -> "Freqlut":
        return cls(artifact.values)

    def lookup(self, logfreq: int) -> int:
        logfreq = _i32(logfreq)
        ix = (logfreq & 0xFFFFFF) >> FREQLUT_SAMPLE_SHIFT
        y0 = self.lut[ix]
        y1 = self.lut[ix + 1]
        lowbits = logfreq & ((1 << FREQLUT_SAMPLE_SHIFT) - 1)
        y = y0 + (((y1 - y0) * lowbits) >> FREQLUT_SAMPLE_SHIFT)
        hibits = logfreq >> 24
        if hibits > FREQLUT_MAX_LOGFREQ_INT:
            # freqlut.cc:44-45: above 20.0 the result is inaccurate (many
            # times Nyquist); the pinned code would shift by a negative
            # count (UB). Out of the pinned contract; refuse loudly.
            raise ValueError(f"logfreq {logfreq} exceeds MAX_LOGFREQ_INT")
        return _i32(y >> (FREQLUT_MAX_LOGFREQ_INT - hibits))
