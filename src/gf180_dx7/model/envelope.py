"""N03 integer operator-envelope model (issue #17, contract gf180-dx7-integer-v1).

Mirrors the pinned Dexed Mark I operator envelope as numeric facts, with zero
GPL bytes: semantics only, cited in docs/N03-ENVELOPE-MODEL.md against the
R01-pinned tree (/Users/joseph/dev/dexed @ b6cf2e9190e216387ad96f977284ebbef765b447).

State members are named after the pinned reference members and classified as
per-note runtime state exactly as N01 section 6 (spec/numeric-profile-v1.json
storage.per_operator_runtime_state_bits): level_, targetlevel_, inc_,
staticcount_, ix_, rising_, down_, plus the per-note DERIVED effective
outlevel_ / rate_scaling_ (dx7note.cc:169-178).

Frame-geometry and gain-side constants (synth.h:29-30; EngineMkI.cpp:51-62,
155, 314-315) are included because the envelope's observable effect on PCM is
defined through them: one model step per frame, and the 14-bit per-sample
gain applied by the operator is gain2 = ENV_MAX - (level_ >> 14) with a
linear intra-frame ramp.

Stdlib-only; pure integer arithmetic (Python ints are unbounded; every
operation here stays within the reference's int32/int64 ranges, asserted by
tests).
"""

from __future__ import annotations

# --- frozen frame geometry (synth.h:29-30, NUM-002) ---

LG_N = 6
N = 1 << LG_N

# --- envelope constants (env.cc) ---

# Q24 per doubling (env.h:64-67): level is stored << 16 relative to the
# 8-bit DX7 domain, i.e. 2^24 units per amplitude doubling.
LEVEL_ONE = 1 << 24
# Rising-leg jump-start floor (env.cc:79-82): 1716 in the 8-bit domain.
JUMP_TARGET = 1716 << 16
# Rising-leg exponential base (env.cc:83).
RISE_BASE = 17 << 24
# Target clamp floor (env.cc:119,166): actuallevel < 16 -> 16.
LEVEL_FLOOR = 16
# Target offset (env.cc:118,164).
TARGET_OFFSET = 4256

# Mark I operator gain side (EngineMkI.cpp:51-62,314-315).
ENV_BITDEPTH = 14
ENV_MAX = 1 << ENV_BITDEPTH
GAIN_SHIFT = 28 - ENV_BITDEPTH


def sr_multiplier(sample_rate_hz: float) -> int:
    """Env::init_sr (env.cc:47-49): uint32 truncation of
    (44100.0 / sampleRate) * (1 << 24). At 48 kHz this is 15414067; the
    frozen value is always re-derived by test, never trusted as a literal
    (NUM-005, rev 1.1)."""
    return int((44100.0 / sample_rate_hz) * (1 << 24))


SR_MULTIPLIER_48K = sr_multiplier(48000)

# levellut (env.cc:28-30), consulted by scaleoutlevel for outlevel < 20.
_LEVEL_LUT = (
    0, 5, 9, 13, 17, 20, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
    42, 43, 45, 46,
)

# statics (env.cc:33-44): ACCURATE_ENVELOPE hold lengths in 44.1 kHz samples
# for rate+rate_scaling values 0..76 (>= 77 uses 20 * (99 - rate) instead).
_STATICS = (
    1764000, 1764000, 1411200, 1411200, 1190700, 1014300, 992250,
    882000, 705600, 705600, 584325, 507150, 502740, 441000, 418950,
    352800, 308700, 286650, 253575, 220500, 220500, 176400, 145530,
    145530, 125685, 110250, 110250, 88200, 88200, 74970, 61740,
    61740, 55125, 48510, 44100, 37485, 31311, 30870, 27562, 27562,
    22050, 18522, 17640, 15435, 14112, 13230, 11025, 9261, 9261, 7717,
    6615, 6615, 5512, 5512, 4410, 3969, 3969, 3439, 2866, 2690, 2249,
    1984, 1896, 1808, 1411, 1367, 1234, 1146, 926, 837, 837, 705,
    573, 573, 529, 441, 441,
)


def scale_out_level(outlevel: int) -> int:
    """Env::scaleoutlevel (env.cc:109-111)."""
    return 28 + outlevel if outlevel >= 20 else _LEVEL_LUT[outlevel]


# velocity_data (dx7note.cc:66-72).
_VELOCITY_DATA = (
    0, 70, 86, 97, 106, 114, 121, 126, 132, 138, 142, 148, 152, 156, 160, 163,
    166, 170, 173, 174, 178, 181, 184, 186, 189, 190, 194, 196, 198, 200, 202,
    205, 206, 209, 211, 214, 216, 218, 220, 222, 224, 225, 227, 229, 230, 232,
    233, 235, 237, 238, 240, 241, 242, 243, 244, 246, 246, 248, 249, 250, 251,
    252, 253, 254,
)


def scale_velocity(velocity: int, sensitivity: int) -> int:
    """ScaleVelocity (dx7note.cc:75-80): velocity delta in microsteps."""
    clamped_vel = max(0, min(127, velocity))
    vel_value = _VELOCITY_DATA[clamped_vel >> 1] - 239
    return ((sensitivity * vel_value + 7) >> 3) << 4


# exp_scale_data (dx7note.cc:96-99).
_EXP_SCALE_DATA = (
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 14, 16, 19, 23, 27, 33, 39, 47, 56, 66,
    80, 94, 110, 126, 142, 158, 174, 190, 206, 222, 238, 250,
)


def scale_curve(group: int, depth: int, curve: int) -> int:
    """ScaleCurve (dx7note.cc:101-116): curves 0/3 linear, 1/2 exponential."""
    if curve == 0 or curve == 3:
        scale = (group * depth * 329) >> 12
    else:
        raw_exp = _EXP_SCALE_DATA[min(group, len(_EXP_SCALE_DATA) - 1)]
        scale = (raw_exp * depth * 329) >> 15
    if curve < 2:
        scale = -scale
    return scale


def scale_level(midinote: int, break_pt: int, left_depth: int,
                right_depth: int, left_curve: int, right_curve: int) -> int:
    """ScaleLevel (dx7note.cc:118-126): keyboard level scaling."""
    offset = midinote - break_pt - 17
    if offset >= 0:
        return scale_curve((offset + 1) // 3, right_depth, right_curve)
    return scale_curve(-(offset - 1) // 3, left_depth, left_curve)


def scale_rate(midinote: int, sensitivity: int) -> int:
    """ScaleRate (dx7note.cc:82-94): keyboard rate scaling in qRate units."""
    x = min(31, max(0, midinote // 3 - 7))
    return (sensitivity * x) >> 3


def derive_op_envelope_params(op, midinote: int, velocity: int):
    """Per-note derived envelope params (Dx7Note::init, dx7note.cc:163-179).

    Returns (rates[4], levels[4], outlevel, rate_scaling) where outlevel is
    the effective microstep level (scaleoutlevel + key-level scaling, clamp
    127, <<5, + velocity scaling, clamp 0) and rate_scaling the keyboard rate
    delta — both per-note runtime state per N01 section 6.
    """
    rates = [op.rate_1, op.rate_2, op.rate_3, op.rate_4]
    levels = [op.level_1, op.level_2, op.level_3, op.level_4]
    outlevel = scale_out_level(op.output_level)
    outlevel += scale_level(midinote, op.break_point, op.scale_left_depth,
                            op.scale_right_depth, op.scale_left_curve,
                            op.scale_right_curve)
    outlevel = min(127, outlevel)
    outlevel = outlevel << 5
    outlevel += scale_velocity(velocity, op.key_velocity_sensitivity)
    outlevel = max(0, outlevel)
    rate_scaling = scale_rate(midinote, op.rate_scaling)
    return rates, levels, outlevel, rate_scaling


class Envelope:
    """One operator envelope; mirrors msfa Env member-for-member.

    Runtime state per N01 section 6: level_, targetlevel_, inc_,
    staticcount_ (32-bit each), ix_ (3-bit content), rising_, down_ (flags),
    plus the per-note derived outlevel_ and rate_scaling_.
    """

    def __init__(self, sr_multiplier: int = SR_MULTIPLIER_48K):
        self.sr_multiplier = sr_multiplier
        self.rates_ = [0, 0, 0, 0]
        self.levels_ = [0, 0, 0, 0]
        self.outlevel_ = 0
        self.rate_scaling_ = 0
        self.level_ = 0
        self.targetlevel_ = 0
        self.rising_ = False
        self.ix_ = 0
        self.inc_ = 0
        self.staticcount_ = 0
        self.down_ = True

    def init(self, rates, levels, outlevel: int, rate_scaling: int) -> None:
        """Env::init (env.cc:51-61)."""
        self.rates_ = list(rates)
        self.levels_ = list(levels)
        self.outlevel_ = outlevel
        self.rate_scaling_ = rate_scaling
        self.level_ = 0
        self.down_ = True
        self.advance(0)

    def update(self, rates, levels, outlevel: int, rate_scaling: int) -> None:
        """Env::update (env.cc:153-169): parameter refresh while playing;
        while the key is down the sustain target is recomputed from
        levels_[2] without the outlevel term."""
        self.rates_ = list(rates)
        self.levels_ = list(levels)
        self.outlevel_ = outlevel
        self.rate_scaling_ = rate_scaling
        if self.down_:
            newlevel = self.levels_[2]
            actuallevel = scale_out_level(newlevel) >> 1
            actuallevel = (actuallevel << 6) - TARGET_OFFSET
            actuallevel = LEVEL_FLOOR if actuallevel < LEVEL_FLOOR \
                else actuallevel
            self.targetlevel_ = actuallevel << 16
            self.advance(2)

    def keydown(self, down: bool) -> None:
        """Env::keydown (env.cc:102-107): key-down -> segment 0 from the
        current level; key-up -> segment 3 from the current level."""
        if self.down_ != down:
            self.down_ = down
            self.advance(0 if down else 3)

    def getsample(self) -> int:
        """Env::getsample (env.cc:63-100): one step per 64-sample frame."""
        if self.staticcount_:
            self.staticcount_ -= N
            if self.staticcount_ <= 0:
                self.staticcount_ = 0
                self.advance(self.ix_ + 1)
        if self.ix_ < 3 or (self.ix_ < 4 and not self.down_):
            if self.staticcount_:
                pass
            elif self.rising_:
                if self.level_ < JUMP_TARGET:
                    self.level_ = JUMP_TARGET
                self.level_ += ((RISE_BASE - self.level_) >> 24) * self.inc_
                if self.level_ >= self.targetlevel_:
                    self.level_ = self.targetlevel_
                    self.advance(self.ix_ + 1)
            else:
                self.level_ -= self.inc_
                if self.level_ <= self.targetlevel_:
                    self.level_ = self.targetlevel_
                    self.advance(self.ix_ + 1)
        return self.level_

    def advance(self, newix: int) -> None:
        """Env::advance (env.cc:113-151)."""
        self.ix_ = newix
        if self.ix_ < 4:
            newlevel = self.levels_[self.ix_]
            actuallevel = scale_out_level(newlevel) >> 1
            actuallevel = (actuallevel << 6) + self.outlevel_ - TARGET_OFFSET
            actuallevel = LEVEL_FLOOR if actuallevel < LEVEL_FLOOR \
                else actuallevel
            self.targetlevel_ = actuallevel << 16
            self.rising_ = self.targetlevel_ > self.level_
            qrate = (self.rates_[self.ix_] * 41) >> 6
            qrate += self.rate_scaling_
            qrate = min(qrate, 63)
            if self.targetlevel_ == self.level_ or \
                    (self.ix_ == 0 and newlevel == 0):
                staticrate = self.rates_[self.ix_] + self.rate_scaling_
                staticrate = min(staticrate, 99)
                if staticrate < 77:
                    self.staticcount_ = _STATICS[staticrate]
                else:
                    self.staticcount_ = 20 * (99 - staticrate)
                if staticrate < 77 and self.ix_ == 0 and newlevel == 0:
                    self.staticcount_ //= 20
                self.staticcount_ = int(
                    (self.staticcount_ * self.sr_multiplier) >> 24)
            else:
                self.staticcount_ = 0
            self.inc_ = (4 + (qrate & 3)) << (2 + LG_N + (qrate >> 2))
            self.inc_ = int((self.inc_ * self.sr_multiplier) >> 24)

    def transfer(self, src: "Envelope") -> None:
        """Env::transfer (env.cc:175-191): mono-mode state handoff."""
        self.rates_ = list(src.rates_)
        self.levels_ = list(src.levels_)
        self.outlevel_ = src.outlevel_
        self.rate_scaling_ = src.rate_scaling_
        self.level_ = src.level_
        self.targetlevel_ = src.targetlevel_
        self.rising_ = src.rising_
        self.ix_ = src.ix_
        self.down_ = src.down_
        self.staticcount_ = src.staticcount_
        self.inc_ = src.inc_

    # --- Mark I per-frame gain derivation (EngineMkI.cpp:155,314-315) ---

    def gain1(self, prev_gain_out: int) -> int:
        return ENV_MAX - 1 if prev_gain_out == 0 else prev_gain_out

    def gain2(self) -> int:
        return ENV_MAX - (self.level_ >> GAIN_SHIFT)

    @staticmethod
    def dgain(gain1: int, gain2: int) -> int:
        return (gain2 - gain1 + (N >> 1)) >> LG_N


def simulate(n_frames: int, events, rates, levels, outlevel: int,
             rate_scaling: int,
             sr_multiplier: int = SR_MULTIPLIER_48K):
    """Frame-accurate envelope run: events commit at frame boundaries
    (NUM-011, oracle event_to_block_policy), then the envelope steps once
    (dx7note.cc:283). A "keydown" event re-inits the envelope exactly like
    the reference voice allocator does on note-on (Dx7Note::init ->
    Env::init, level_ = 0); "keyup" releases from the current level.

    Returns the list of level_ after each frame's step.
    """
    by_block = {}
    for block, kind in events:
        by_block.setdefault(block, []).append(kind)
    env = Envelope(sr_multiplier)
    trace = []
    for frame in range(n_frames):
        for kind in by_block.get(frame, ()):
            if kind == "keydown":
                env.init(rates, levels, outlevel, rate_scaling)
                env.keydown(True)
            elif kind == "keyup":
                env.keydown(False)
            else:
                raise ValueError(f"unknown event kind {kind!r}")
        env.getsample()
        trace.append(env.level_)
    return trace
