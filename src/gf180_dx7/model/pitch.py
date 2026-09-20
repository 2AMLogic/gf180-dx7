"""N05 — integer pitch and modulation model (issue #19).

Mirrors the pinned Dexed Mark I reference's *pitch path* integer semantics
one-for-one (contract gf180-dx7-integer-v1, spec/numeric-profile-v1.json
NUM-003/NUM-007/NUM-008):

  * Q24-octave log-frequency domain, A4 base 50857777 (standard tuning);
  * Freqlut: 1025-entry int32 LUT, linear interpolation, per-sample
    phase increment = 32-bit;
  * per-operator base pitch: ratio mode (keyboard + coarse/fine/detune)
    vs fixed mode (no keyboard tracking), dx7note.cc:38-64;
  * pitch EG: per-frame additive level_ +/- inc_ stepping against
    pitchenv_tab targets (pitchenv.cc);
  * LFO: six waveforms, rate shaping, delay ramp, keydown sync
    (lfo.cc), one getsample()/getdelay() per 64-sample frame;
  * per-frame pitch-modulation composition: LFO PM depth/sensitivity,
    controller pitch_mod, pitch bend, master tune; ratio-mode
    operators take basepitch + pitch_mod, fixed-mode operators take
    basepitch + pitch_base only (dx7note.cc:278-281).

PINNED-CONFIG DIVERGENCE (explicit switch): upstream Dexed applies the
patch transpose at keydown (``pitch += tuningTranspositionShift()``,
Source/PluginProcessor.cpp:458 with data[144]-24 at :566-580), but the
pinned oracle wrapper's keydown omits that shift (oracle main.cpp:168-194;
registry finding perf-transpose-12 in spec/compatibility-registry-v1.json
and docs/REGISTRY-POLICY.md section 5). ``NotePitch(transpose_mode=...)``
selects:

  * ``"documented"``      — upstream semantics: transpose - 24 semitone
                            shift applied to the keydown note;
  * ``"pinned_wrapper"``  — the pinned oracle wrapper's observed
                            behavior: transpose ignored.

The controller pitch_mod input (CC 1 mod wheel path) is modeled with its
FmMod range factor; at the pinned defaults wheel range = 0
(Source/msfa/controllers.h:43-48, manifest
mod_wheel_default_modifiers) so CC 1 is inert (registry finding
ev-modwheel-127). Passing a nonzero ``wheel_range`` exercises the
documented response.

Licensing: numeric semantics, constants, and table contents are adopted
as *facts* from the pinned Apache-2.0 msfa files (citations inline);
this is a clean re-implementation with zero copied bytes. The GPL
EngineMkI wrapper is never copied or imported.

Stdlib-only.
"""

from __future__ import annotations

import math
import struct

# ---------------------------------------------------------------------------
# C arithmetic helpers
# ---------------------------------------------------------------------------

_U32 = 0xFFFFFFFF
_I32_MASK = 0xFFFFFFFF


def u32(x: int) -> int:
    """C uint32 truncation."""
    return x & _U32


def i32(x: int) -> int:
    """C int32 truncation (two's complement wrap)."""
    x &= _I32_MASK
    return x - (1 << 32) if x >= (1 << 31) else x


def f32(x: float) -> float:
    """C float (IEEE binary32) narrowing, as used by the declared
    float-gated spots (NUM-008: pitch-bend scaling)."""
    return struct.unpack("<f", struct.pack("<f", x))[0]


# ---------------------------------------------------------------------------
# Frame constants (synth.h:29-30)
# ---------------------------------------------------------------------------

LG_N = 6
N = 1 << LG_N

# Standard tuning (tuning.cc:14-19): base = (1 << 24) * (log(440)/log(2) - 69/12)
A4_BASE_LOGFREQ = 50857777
SEMI_LOGFREQ = (1 << 24) // 12  # integer division, as in tuning.cc:15


def midinote_to_logfreq(midinote: int) -> int:
    """Standard tuning table (tuning.cc:23-25)."""
    return i32(A4_BASE_LOGFREQ + SEMI_LOGFREQ * midinote)


# ---------------------------------------------------------------------------
# Freqlut (freqlut.cc:27-55) — Q24 logfreq -> 32-bit phase increment
# ---------------------------------------------------------------------------

class Freqlut:
    """Mirror of msfa Freqlut (freqlut.cc). init() accumulates 1024
    doublings in float64 exactly like the reference (the <=1-LSB drift
    budget for regenerated LUTs is declared in NUM-003/NUM-008)."""

    LG_N_SAMPLES = 10
    N_SAMPLES = 1 << LG_N_SAMPLES
    SAMPLE_SHIFT = 24 - LG_N_SAMPLES
    MAX_LOGFREQ_INT = 20

    _lut: list[int] | None = None

    @classmethod
    def init(cls, sample_rate: float) -> None:
        y = (1 << (24 + cls.MAX_LOGFREQ_INT)) / sample_rate  # double
        inc = 2.0 ** (1.0 / cls.N_SAMPLES)
        lut = []
        for _ in range(cls.N_SAMPLES + 1):
            lut.append(int(math.floor(y + 0.5)))
            y *= inc
        cls._lut = lut

    @classmethod
    def lut(cls) -> list[int]:
        if cls._lut is None:
            raise RuntimeError("Freqlut.init(sample_rate) not called")
        return cls._lut

    @classmethod
    def lookup(cls, logfreq: int) -> int:
        lut = cls.lut()
        logfreq = i32(logfreq)
        ix = (logfreq & 0xFFFFFF) >> cls.SAMPLE_SHIFT
        y0 = lut[ix]
        y1 = lut[ix + 1]
        lowbits = logfreq & ((1 << cls.SAMPLE_SHIFT) - 1)
        y = y0 + i32((y1 - y0) * lowbits >> cls.SAMPLE_SHIFT)  # int64 math
        hibits = logfreq >> 24  # arithmetic shift (C int32)
        # C shifts by (20 - hibits); counts >= 32 are UB in C and
        # unreachable at audio rates (freqlut.cc:44-45 note).
        return y >> (cls.MAX_LOGFREQ_INT - hibits)


# ---------------------------------------------------------------------------
# msfa Sin table (sin.cc:31-56 + sin.h:29-35, SIN_DELTA form) — used only by
# the LFO sine branch (lfo.cc:67)
# ---------------------------------------------------------------------------

SIN_LG_N_SAMPLES = 10
SIN_N_SAMPLES = 1 << SIN_LG_N_SAMPLES

_sintab: list[int] | None = None


def build_sintab() -> list[int]:
    """Regenerate sintab (SIN_DELTA layout, 2048 entries) following
    sin.cc:31-56 operation-for-operation."""
    dphase = 2.0 * math.pi / SIN_N_SAMPLES
    c = int(math.floor(math.cos(dphase) * (1 << 30) + 0.5))
    s = int(math.floor(math.sin(dphase) * (1 << 30) + 0.5))
    u = 1 << 30
    v = 0
    tab = [0] * (SIN_N_SAMPLES << 1)
    r = 1 << 29
    for i in range(SIN_N_SAMPLES // 2):
        tab[(i << 1) + 1] = (v + 32) >> 6
        tab[((i + SIN_N_SAMPLES // 2) << 1) + 1] = -((v + 32) >> 6)
        t = i32((u * s + v * c + r) >> 30)
        u = i32((u * c - v * s + r) >> 30)
        v = t
    for i in range(SIN_N_SAMPLES - 1):
        tab[i << 1] = tab[(i << 1) + 3] - tab[(i << 1) + 1]
    tab[(SIN_N_SAMPLES << 1) - 2] = -tab[(SIN_N_SAMPLES << 1) - 1]
    return tab


def sin_lookup(phase: int) -> int:
    """Sin::lookup, SIN_DELTA inline path (sin.h:45-53)."""
    global _sintab
    if _sintab is None:
        _sintab = build_sintab()
    tab = _sintab
    shift = 24 - SIN_LG_N_SAMPLES
    lowbits = phase & ((1 << shift) - 1)
    phase_int = (phase >> (shift - 1)) & ((SIN_N_SAMPLES - 1) << 1)
    dy = tab[phase_int]
    y0 = tab[phase_int + 1]
    return i32(y0 + i32((dy * lowbits) >> shift))


# ---------------------------------------------------------------------------
# Operator base pitch (dx7note.cc:30-64)
# ---------------------------------------------------------------------------

# dx7note.cc:30-36, Q24 log2 multipliers for coarse 0..31
COARSEMUL = (
    -16777216, 0, 16777216, 26591258, 33554432, 38955489, 43368474, 47099600,
    50331648, 53182516, 55732705, 58039632, 60145690, 62083076, 63876816,
    65546747, 67108864, 68576247, 69959732, 71268397, 72509921, 73690858,
    74816848, 75892776, 76922906, 77910978, 78860292, 79773775, 80654032,
    81503396, 82323963, 83117622,
)

# dx7note.cc:128-130
PITCHMODSENSTAB = (0, 10, 20, 33, 55, 92, 153, 255)


def osc_freq(midinote: int, mode: int, coarse: int, fine: int, detune: int,
             note_logfreq: int | None = None) -> int:
    """Dx7Note::osc_freq (dx7note.cc:38-64).

    mode 0 (ratio): keyboard-tracking; mode != 0 (fixed): keyboard
    independent. ``note_logfreq`` defaults to the standard-tuning value
    for ``midinote``; callers pass the (transpose-shifted) note logfreq.
    """
    if note_logfreq is None:
        note_logfreq = midinote_to_logfreq(midinote)
    if mode == 0:
        logfreq = note_logfreq
        # dx7note.cc:46-47 (double math, assigned back to int32: C
        # truncation toward zero)
        detune_ratio = 0.0209 * math.exp(-0.396 * (float(logfreq) / (1 << 24))) / 7
        logfreq = i32(int(logfreq + detune_ratio * logfreq * (detune - 7)))
        logfreq = i32(logfreq + COARSEMUL[coarse & 31])
        if fine:
            # dx7note.cc:50-53
            logfreq = i32(logfreq + int(math.floor(24204406.323123 * math.log(1 + 0.01 * fine) + 0.5)))
    else:
        # dx7note.cc:59-61
        logfreq = (4458616 * ((coarse & 3) * 100 + fine)) >> 3
        logfreq = i32(logfreq + (13457 * (detune - 7) if detune > 7 else 0))
    return logfreq


# ---------------------------------------------------------------------------
# Pitch EG (pitchenv.cc)
# ---------------------------------------------------------------------------

class PitchEnv:
    """Mirror of msfa PitchEnv (pitchenv.cc:45-93); one getsample() per
    64-sample frame (NUM-002/NUM-008)."""

    # pitchenv.cc:22-24: unit_ = N * (1 << 24) / (21.3 * sample_rate) + 0.5
    unit_: int = 1050  # frozen at 48 kHz

    @classmethod
    def init(cls, sample_rate: float) -> None:
        cls.unit_ = int(N * (1 << 24) / (21.3 * sample_rate) + 0.5)

    # pitchenv.cc:26-33
    PITCHENV_RATE = (
        1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12,
        12, 13, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 30, 31, 33, 34, 36, 37, 38, 39, 41, 42, 44, 46, 47,
        49, 51, 53, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 79, 82,
        85, 88, 91, 94, 98, 102, 106, 110, 115, 120, 125, 130, 135, 141, 147,
        153, 159, 165, 171, 178, 185, 193, 202, 211, 232, 243, 254, 255,
    )

    # pitchenv.cc:35-43
    PITCHENV_TAB = (
        -128, -116, -104, -95, -85, -76, -68, -61, -56, -52, -49, -46, -43,
        -41, -39, -37, -35, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24,
        -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,
        -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
        28, 29, 30, 31, 32, 33, 34, 35, 38, 40, 43, 46, 49, 53, 58, 65, 73,
        82, 92, 103, 115, 127,
    )

    def __init__(self) -> None:
        self.rates_ = [0, 0, 0, 0]
        self.levels_ = [0, 0, 0, 0]
        self.level_ = 0
        self.targetlevel_ = 0
        self.rising_ = False
        self.ix_ = 0
        self.inc_ = 0
        self.down_ = True

    def set(self, r, l) -> None:
        """pitchenv.cc:45-53."""
        for i in range(4):
            self.rates_[i] = r[i]
            self.levels_[i] = l[i]
        self.level_ = i32(self.PITCHENV_TAB[l[3]] << 19)
        self.down_ = True
        self.advance(0)

    def getsample(self) -> int:
        """pitchenv.cc:55-72."""
        if self.ix_ < 3 or ((self.ix_ < 4) and not self.down_):
            if self.rising_:
                self.level_ = i32(self.level_ + self.inc_)
                if self.level_ >= self.targetlevel_:
                    self.level_ = self.targetlevel_
                    self.advance(self.ix_ + 1)
            else:  # !rising
                self.level_ = i32(self.level_ - self.inc_)
                if self.level_ <= self.targetlevel_:
                    self.level_ = self.targetlevel_
                    self.advance(self.ix_ + 1)
        return self.level_

    def keydown(self, d: bool) -> None:
        """pitchenv.cc:74-79."""
        if self.down_ != d:
            self.down_ = d
            self.advance(0 if d else 3)

    def advance(self, newix: int) -> None:
        """pitchenv.cc:81-89."""
        self.ix_ = newix
        if self.ix_ < 4:
            newlevel = self.levels_[self.ix_]
            self.targetlevel_ = i32(self.PITCHENV_TAB[newlevel] << 19)
            self.rising_ = self.targetlevel_ > self.level_
            self.inc_ = self.PITCHENV_RATE[self.rates_[self.ix_]] * self.unit_


# ---------------------------------------------------------------------------
# LFO (lfo.cc) — one getsample() + one getdelay() per frame (NUM-007)
# ---------------------------------------------------------------------------

LFO_WAVEFORMS = ("triangle", "saw_down", "saw_up", "square", "sine",
                 "sample_hold")


class Lfo:
    """Mirror of msfa Lfo (lfo.cc:26-97); 32-bit phase, Q24-ish outputs."""

    # lfo.cc:26-29: unit_ = (int32)(N * 25190424 / sample_rate + 0.5)
    unit_: int = 33587  # frozen at 48 kHz

    @classmethod
    def init(cls, sample_rate: float) -> None:
        cls.unit_ = int(N * 25190424 / sample_rate + 0.5)

    def __init__(self) -> None:
        # Upstream leaves phase_/randstate_/delaystate_ uninitialized
        # (R05 instance-dependence finding); the model zeroes them and
        # relies on keydown()/reset() for deterministic runs.
        self.phase_ = 0
        self.delta_ = 0
        self.waveform_ = 0
        self.randstate_ = 0
        self.sync_ = False
        self.delaystate_ = 0
        self.delayinc_ = 0
        self.delayinc2_ = 0

    def reset(self, params) -> None:
        """lfo.cc:31-49; params = [rate, delay, ?, ?, sync, wave]."""
        rate = params[0]  # 0..99
        sr = 1 if rate == 0 else (165 * rate) >> 6
        sr *= 11 if sr < 160 else (11 + ((sr - 160) >> 4))
        self.delta_ = u32(self.unit_ * sr)
        a = 99 - params[1]  # LFO delay
        if a == 99:
            self.delayinc_ = u32(~0)
            self.delayinc2_ = u32(~0)
        else:
            a = (16 + (a & 15)) << (1 + (a >> 4))
            self.delayinc_ = u32(self.unit_ * a)
            a &= 0xFF80
            a = max(0x80, a)
            self.delayinc2_ = u32(self.unit_ * a)
        self.waveform_ = params[5]
        self.sync_ = params[4] != 0

    def getsample(self) -> int:
        """lfo.cc:51-76."""
        self.phase_ = u32(self.phase_ + self.delta_)
        w = self.waveform_
        if w == 0:  # triangle
            x = self.phase_ >> 7
            x ^= -(self.phase_ >> 31)
            x &= (1 << 24) - 1
            return x
        if w == 1:  # sawtooth down
            return u32(~u32(self.phase_) ^ (1 << 31)) >> 8
        if w == 2:  # sawtooth up
            return (self.phase_ ^ (1 << 31)) >> 8
        if w == 3:  # square
            return (u32(~u32(self.phase_)) >> 7) & (1 << 24)
        if w == 4:  # sine
            return i32((1 << 23) + (sin_lookup(self.phase_ >> 8) >> 1))
        if w == 5:  # s&h
            if self.phase_ < self.delta_:
                self.randstate_ = (self.randstate_ * 179 + 17) & 0xFF
            x = self.randstate_ ^ 0x80
            return (x + 1) << 16
        return 1 << 23

    def getdelay(self) -> int:
        """lfo.cc:78-90 (uint32/uint64 arithmetic)."""
        delta = self.delayinc_ if self.delaystate_ < (1 << 31) else self.delayinc2_
        d = self.delaystate_ + delta  # uint64 sum
        if d > _U32:
            return 1 << 24
        self.delaystate_ = u32(d)
        if d < (1 << 31):
            return 0
        return (d >> 7) & ((1 << 24) - 1)

    def keydown(self) -> None:
        """lfo.cc:92-97."""
        if self.sync_:
            self.phase_ = u32((1 << 31) - 1)
        self.delaystate_ = 0


# ---------------------------------------------------------------------------
# Per-frame pitch/mod composition (dx7note.cc:208-296 excerpt)
# ---------------------------------------------------------------------------

def transpose_shift(transpose: int, transpose_mode: str) -> int:
    """Keydown pitch shift applied to the MIDI note.

    ``documented``: data[144] - 24 (PluginProcessor.cpp:458 and
    tuningTranspositionShift at :566-580, standard-tuning path).
    ``pinned_wrapper``: 0 — the pinned oracle wrapper omits the shift
    (registry finding perf-transpose-12, docs/REGISTRY-POLICY.md §5).
    """
    if transpose_mode == "documented":
        return transpose - 24
    if transpose_mode == "pinned_wrapper":
        return 0
    raise ValueError(f"unknown transpose_mode {transpose_mode!r}")


class NotePitch:
    """Per-note pitch path: basepitch per operator plus the per-frame
    pitch-mod composition, producing per-operator phase increments.

    ``patch`` is the 155-byte unpacked single-voice body (the .syx body,
    identical to Dexed's data[0..154], PluginData.cpp:199).
    """

    def __init__(self, patch, midinote: int, *,
                 transpose_mode: str = "documented") -> None:
        if len(patch) < 145:
            raise ValueError("patch must be the 155-byte unpacked voice body")
        self.transpose_mode = transpose_mode
        self.playing_midi_note = midinote
        shift = transpose_shift(patch[144], transpose_mode)
        self.note_logfreq = midinote_to_logfreq(midinote + shift)
        self.op_mode: list[int] = []
        self.basepitch: list[int] = []
        for op in range(6):
            off = op * 21
            mode = patch[off + 17]
            coarse = patch[off + 18]
            fine = patch[off + 19]
            detune = patch[off + 20]
            self.op_mode.append(mode)
            self.basepitch.append(
                osc_freq(midinote, mode, coarse, fine, detune,
                         note_logfreq=self.note_logfreq))
        rates = [patch[126 + i] for i in range(4)]
        levels = [patch[130 + i] for i in range(4)]
        self.pitchenv_ = PitchEnv()
        self.pitchenv_.set(rates, levels)
        # dx7note.cc:198-199
        self.pitchmoddepth_ = (patch[139] * 165) >> 6
        self.pitchmodsens_ = PITCHMODSENSTAB[patch[143] & 7]

    def pitch_mod_per_frame(self, lfo_val: int, lfo_delay: int, *,
                            ctrl_pitch_mod: int = 0,
                            pitchbend_value: int = 0x2000,
                            pitch_range_up: int = 3,
                            pitch_range_dn: int = 3,
                            pitch_step: int = 0,
                            master_tune: int = 0) -> int:
        """dx7note.cc:208-257 pitch path for one 64-sample frame.

        Returns pitch_mod = pitchenv + LFO/controller PM + bend + tune,
        exactly the value added to ratio-mode basepitch before Freqlut.
        Default arguments are the pinned wrapper/controller defaults
        (reference/manifest.json settings controllers_defaults and
        pitch_bend_range); ``ctrl_pitch_mod`` stays 0 at those defaults
        because the mod-wheel FmMod range is 0 (CC1 inert, registry
        finding ev-modwheel-127).
        """
        # ==== PITCH ==== (dx7note.cc:210-217)
        pmd = u32(self.pitchmoddepth_ * lfo_delay)  # uint32 (Q32)
        senslfo = i32(self.pitchmodsens_ * (lfo_val - (1 << 23)))
        pmod_1 = i32((pmd * senslfo) >> 39)
        pmod_1 = abs(pmod_1)
        pmod_2 = i32((ctrl_pitch_mod * senslfo) >> 14)
        pmod_2 = abs(pmod_2)
        pitch_mod = max(pmod_1, pmod_2)
        pitch_mod = i32(self.pitchenv_.getsample()
                        + pitch_mod * (-1 if senslfo < 0 else 1))

        # ---- PITCH BEND ---- (dx7note.cc:219-233; float-gated spot,
        # NUM-008 item 1: reproduce the reference float formulas)
        pitch_base, pb = self._pitch_base(pitchbend_value, pitch_range_up,
                                          pitch_range_dn, pitch_step,
                                          master_tune)
        return i32(pitch_mod + pitch_base)

    @staticmethod
    def _pitch_base(pitchbend_value: int, pitch_range_up: int,
                    pitch_range_dn: int, pitch_step: int,
                    master_tune: int) -> tuple[int, int]:
        """dx7note.cc:219-233 + :256; returns (pitch_base, pb)."""
        pb = i32(pitchbend_value - 0x2000)
        if pb != 0 and pitch_step == 0:
            # ((float)(pb << 11)) * ((float)range) / 12.0 -> int32 trunc
            scale = f32(pitch_range_up if pb >= 0 else pitch_range_dn)
            pb = int(f32(f32(float(pb << 11)) * scale) / 12.0)
        elif pb != 0:
            stp = 12 // pitch_step
            pb = pb * stp // 8191
            pb = i32((pb * (8191 // stp)) << 11)
        # MPE branch (dx7note.cc:235-240): per-note mpePitchBend is 8192
        # at note init (dx7note.cc:203), so d = 0 and pb is unchanged in
        # the pinned configuration. Tuning branch (:242-254) inactive
        # under standard tuning.
        return i32(pb + master_tune), pb

    def frame_frequencies(self, lfo_val: int, lfo_delay: int, *,
                          ctrl_pitch_mod: int = 0,
                          pitchbend_value: int = 0x2000,
                          pitch_range_up: int = 3,
                          pitch_range_dn: int = 3,
                          pitch_step: int = 0,
                          master_tune: int = 0) -> list[int]:
        """Per-operator 32-bit phase increments for one frame
        (dx7note.cc:271-281). Ratio-mode operators take basepitch +
        pitch_mod (pitch EG, LFO, bend, tune); fixed-mode operators take
        basepitch + pitch_base only (bend + tune) — pitch EG and LFO PM
        do NOT reach fixed-mode operators. Advances the pitch EG by one
        frame (dx7note.cc:217 samples pitchenv_.getsample() every
        compute)."""
        pitch_mod = self.pitch_mod_per_frame(
            lfo_val, lfo_delay, ctrl_pitch_mod=ctrl_pitch_mod)
        pitch_base, _ = self._pitch_base(pitchbend_value, pitch_range_up,
                                         pitch_range_dn, pitch_step,
                                         master_tune)
        freqs = []
        for op in range(6):
            if self.op_mode[op]:
                freqs.append(Freqlut.lookup(i32(self.basepitch[op] + pitch_base)))
            else:
                freqs.append(Freqlut.lookup(i32(self.basepitch[op] + pitch_mod)))
        return freqs


# ---------------------------------------------------------------------------
# AM path interface for N06 (dx7note.cc:259-268) — depth composition only
# ---------------------------------------------------------------------------

def amp_mod_per_frame(ampmoddepth: int, lfo_val: int, lfo_delay: int, *,
                      ctrl_amp_mod: int = 0, ctrl_eg_mod: int = 127) -> int:
    """amd_mod (Q24) per frame; the AM *application* inside the operator
    (exp sensitivity, dx7note.cc:284-291) belongs to N06. Default
    ctrl_eg_mod=127 mirrors Controllers::refresh() when no EG-mod source
    is enabled (controllers.h:123-124)."""
    lfo_val = i32((1 << 24) - lfo_val)
    amod_1 = u32((ampmoddepth * lfo_delay) >> 8)  # Q24
    amod_1 = u32((amod_1 * lfo_val) >> 24)
    amod_2 = u32((ctrl_amp_mod * lfo_val) >> 7)
    amd_mod = max(amod_1, amod_2)
    amod_3 = i32((ctrl_eg_mod + 1) << 17)
    return max(u32(i32((1 << 24) - amod_3)), amd_mod)


# ---------------------------------------------------------------------------
# Startup (mirrors the wrapper's init: Freqlut/Lfo/PitchEnv at 48 kHz)
# ---------------------------------------------------------------------------

def init(sample_rate: float = 48000.0) -> None:
    Freqlut.init(sample_rate)
    Lfo.init(sample_rate)
    PitchEnv.init(sample_rate)
