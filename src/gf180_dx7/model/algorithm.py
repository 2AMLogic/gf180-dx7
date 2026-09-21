"""N04 — integer algorithm/feedback model (issue #18, contract
gf180-dx7-integer-v1).

Mirrors the pinned oracle's Mark I algorithm router one-for-one as numeric
facts with zero GPL bytes: the 32-graph connection table, the two persistent
interconnect buses, carrier-sum accumulation, the Mark I level gate, and the
feedback history semantics (including the algorithm 4/6 inline serial forms
and the algorithm 32 +2 shift rule). Sources are semantic specifications
only, never copied (NUM-001; docs/REUSE-RULINGS.md dexed-enginemki):

  * connection table: Source/msfa/fm_core.cc:29-62 (Apache-2.0), packed
    per-op descriptor: bit0-1 out bus, bit2 OUT_BUS_ADD, bit4-5 in bus,
    bit6 FB_IN + bit7 FB_OUT together mark the one feedback op;
  * routing decode + gate + has_contents: Source/EngineMkI.cpp:295-363;
  * frame forms compute/compute_pure/compute_fb: EngineMkI.cpp:152-205;
  * algorithm 6 two-op serial (compute_fb2): EngineMkI.cpp:208-244;
  * algorithm 4 three-op serial (compute_fb3): EngineMkI.cpp:247-293;
  * special-case dispatch + min(shift+2, 16): EngineMkI.cpp:301-305,327-348;
  * feedback shift mapping: Source/msfa/dx7note.cc:197 (8 - feedback, else
    16), FEEDBACK_BITDEPTH = 8 (dx7note.cc:28);
  * per-note state facts: FmOpParams (fm_op_kernel.h:20-25), fb_buf_[2]
    (dx7note.h:77), constructor zeroing (dx7note.cc:141-147), osc key sync
    reset (dx7note.cc:398-403), per-frame level_in/freq composition
    (dx7note.cc:268-296).

The single-operator kernel (mki_sin), the frame gain ramp, and the output
conversion are reused unchanged from model/operator.py (N02); envelopes come
from model/envelope.py (N03) and the pitch path from model/pitch.py (N05).
render_note() assembles them into a full 6-operator note whose PCM is
comparable to the pinned oracle dry f32le render (NUM-010/NUM-011).

Stdlib only.
"""

from __future__ import annotations

import math
import struct
from dataclasses import dataclass

from .. import sysex
from .envelope import Envelope, derive_op_envelope_params
from .operator import (ENV_MAX, LG_N, N, clip_val_domain,
                       clip_val_to_f32_bytes, mki_sin)
from .pitch import (Lfo, NotePitch, amp_mod_per_frame, i32, init as pitch_init)

_U32 = 0xFFFFFFFF

# ---------------------------------------------------------------------------
# The 32-algorithm connection table (facts; fm_core.cc:29-62)
# ---------------------------------------------------------------------------
# Each entry is 6 packed op descriptors for engine params_[0..5]. Decode
# (EngineMkI.cpp:307-313): inbus = (flags >> 4) & 3, outbus = flags & 3,
# add = flags & 0x04, feedback loop op = (flags & 0xC0) == 0xC0.
# Bus 0 is the note output; buses 1 and 2 are the two persistent int32
# interconnect buffers (fm_core.h buf_[2], EngineMkI.cpp:313).
#
# OUT_BUS_ONE  = 1 << 0   IN_BUS_ONE = 1 << 4   FB_IN  = 1 << 6
# OUT_BUS_TWO  = 1 << 1   IN_BUS_TWO = 1 << 5   FB_OUT = 1 << 7
# OUT_BUS_ADD  = 1 << 2                                   (fm_core.h:22-28)

_ALG_ROWS = (
    ("c1", "11", "11", "14", "01", "14"),  # 1
    ("01", "11", "11", "14", "c1", "14"),  # 2
    ("c1", "11", "14", "01", "11", "14"),  # 3
    ("c1", "11", "94", "01", "11", "14"),  # 4
    ("c1", "14", "01", "14", "01", "14"),  # 5
    ("c1", "94", "01", "14", "01", "14"),  # 6
    ("c1", "11", "05", "14", "01", "14"),  # 7
    ("01", "11", "c5", "14", "01", "14"),  # 8
    ("01", "11", "05", "14", "c1", "14"),  # 9
    ("01", "05", "14", "c1", "11", "14"),  # 10
    ("c1", "05", "14", "01", "11", "14"),  # 11
    ("01", "05", "05", "14", "c1", "14"),  # 12
    ("c1", "05", "05", "14", "01", "14"),  # 13
    ("c1", "05", "11", "14", "01", "14"),  # 14
    ("01", "05", "11", "14", "c1", "14"),  # 15
    ("c1", "11", "02", "25", "05", "14"),  # 16
    ("01", "11", "02", "25", "c5", "14"),  # 17
    ("01", "11", "11", "c5", "05", "14"),  # 18
    ("c1", "14", "14", "01", "11", "14"),  # 19
    ("01", "05", "14", "c1", "14", "14"),  # 20
    ("01", "14", "14", "c1", "14", "14"),  # 21
    ("c1", "14", "14", "14", "01", "14"),  # 22
    ("c1", "14", "14", "01", "14", "04"),  # 23
    ("c1", "14", "14", "14", "04", "04"),  # 24
    ("c1", "14", "14", "04", "04", "04"),  # 25
    ("c1", "05", "14", "01", "14", "04"),  # 26
    ("01", "05", "14", "c1", "14", "04"),  # 27
    ("04", "c1", "11", "14", "01", "14"),  # 28
    ("c1", "14", "01", "14", "04", "04"),  # 29
    ("04", "c1", "11", "14", "04", "04"),  # 30
    ("c1", "14", "04", "04", "04", "04"),  # 31
    ("c4", "04", "04", "04", "04", "04"),  # 32
)

ALGORITHMS = tuple(
    tuple(int(f, 16) for f in row) for row in _ALG_ROWS)


def decode_op(flags: int) -> tuple[int, int, bool, bool]:
    """(inbus, outbus, add, fb_loop) per the EngineMkI.cpp:307-313 decode."""
    return ((flags >> 4) & 3, flags & 3, bool(flags & 0x04),
            (flags & 0xC0) == 0xC0)


# ---------------------------------------------------------------------------
# Feedback mapping (facts; dx7note.cc:28,197; EngineMkI.cpp:196,327-348)
# ---------------------------------------------------------------------------

FEEDBACK_BITDEPTH = 8  # dx7note.cc:28


def fb_shift_for(feedback: int) -> int:
    """fb_shift_ = feedback != 0 ? 8 - feedback : 16 (dx7note.cc:197)."""
    return FEEDBACK_BITDEPTH - feedback if feedback != 0 else 16


# ---------------------------------------------------------------------------
# FmOpParams (fm_op_kernel.h:20-25)
# ---------------------------------------------------------------------------


@dataclass
class FmParams:
    """One operator's per-note render parameters (FmOpParams layout)."""

    phase: int = 0
    freq: int = 0
    gain_out: int = 0
    level_in: int = 0


# ---------------------------------------------------------------------------
# The router (EngineMkI.cpp:295-363, one-for-one)
# ---------------------------------------------------------------------------


class AlgorithmRouter:
    """Mark I algorithm router: buses, gate, carrier sums, feedback.

    One instance holds the two persistent interconnect buses (fm_core.h
    buf_[2]): they are NOT cleared between frames, so stale bus contents
    persist exactly like the reference engine instance. The pinned wrapper
    builds one engine shared by all voices (oracle main.cpp
    setEngineMarkI); a single-note render owns its instance here.

    sin_fn seam: every operator evaluation goes through
    sin_fn(op, i, phase_plus_input, gain, mod_term) -> int32, where op is
    the engine param index, i the sample within the frame, and mod_term the
    exact quantity added to the phase (bus input, feedback term, or the
    upstream serial-op output). The default mirrors mkiSin
    (EngineMkI.cpp:124-150); scripted tests substitute known sequences.
    """

    def __init__(self, log_table, exp_table) -> None:
        self.log_table = log_table
        self.exp_table = exp_table
        self.buf1: list[int] = [0] * N  # fm_core.h buf_[0] (outbus/inbus 1)
        self.buf2: list[int] = [0] * N  # fm_core.h buf_[1] (outbus/inbus 2)

    # default mkiSin binding (EngineMkI.cpp:124-150)
    def _sin_default(self, op: int, i: int, phase_plus: int, gain: int,
                     mod_term: int) -> int:
        return mki_sin(phase_plus, gain, self.log_table, self.exp_table)

    @staticmethod
    def _mix(out: list[int], i: int, y: int, add: bool) -> None:
        """output[i] = y + adder[i]; adder = add ? output : zeros
        (EngineMkI.cpp:158,163,179,199)."""
        out[i] = i32(y + out[i]) if add else y

    def render(self, output: list[int], params, algorithm: int,
               fb_buf, feedback_shift: int, sin_fn=None) -> None:
        """One 64-sample frame of the whole note (EngineMkI.cpp:295-363).

        output: 64 slots, zeroed by the caller per frame per note (the
        wrapper's audiobuf, PluginProcessor.cpp:247-249). fb_buf: the
        note's two-element int32 history (dx7note.h:77), read at frame
        start and stored back at frame end inside the fb forms.
        """
        sin_fn = sin_fn or self._sin_default
        k_level_thresh = ENV_MAX - 100          # :296 (NUM-009)
        alg = list(ALGORITHMS[algorithm])       # :297 (local flags copy)
        has_contents = [True, False, False]     # :298
        fb_on = feedback_shift < 16             # :299

        if algorithm == 3 or algorithm == 5:    # :301-305 (ALGO 4 / ALGO 6)
            if fb_on:
                alg[0] = 0xC4                   # route op0 straight to output

        op = 0
        while op < 6:                           # :307 (op mutated at :333,339)
            cur = op
            flags = alg[cur]
            add = bool(flags & 0x04)            # :309
            p = params[cur]                     # :310 (param bound per entry)
            inbus = (flags >> 4) & 3            # :311
            outbus = flags & 3                  # :312
            out = output if outbus == 0 else (
                self.buf1 if outbus == 1 else self.buf2)     # :313
            gain1 = ENV_MAX - 1 if p.gain_out == 0 else p.gain_out  # :314
            gain2 = ENV_MAX - (p.level_in >> 14)                 # :315
            p.gain_out = gain2                  # :316

            if gain1 <= k_level_thresh or gain2 <= k_level_thresh:  # :318
                if not has_contents[outbus]:    # :320-322
                    add = False
                if inbus == 0 or not has_contents[inbus]:        # :324
                    if (flags & 0xC0) == 0xC0 and fb_on:         # :326
                        if algorithm == 3:    # :327-334, three-op serial
                            self._compute_fb3(
                                out, params, gain1, gain2, fb_buf,
                                min(feedback_shift + 2, 16), sin_fn)
                            self._skip_frame(params[1])          # :331
                            self._skip_frame(params[2])          # :332
                            op += 2                              # :333
                        elif algorithm == 5:  # :335-340, two-op serial
                            self._compute_fb2(
                                out, params, gain1, gain2, fb_buf,
                                min(feedback_shift + 2, 16), sin_fn)
                            self._skip_frame(params[1])          # :338
                            op += 1                              # :339
                        elif algorithm == 31:  # :341-344, ALGO 32
                            self._compute_fb(
                                out, p.phase, p.freq, gain1, gain2, fb_buf,
                                min(feedback_shift + 2, 16), add, sin_fn,
                                cur)
                        else:                 # :345-348, plain one-op fb
                            self._compute_fb(
                                out, p.phase, p.freq, gain1, gain2, fb_buf,
                                feedback_shift, add, sin_fn, cur)
                    else:
                        self._frame(out, None, p.phase, p.freq, gain1,
                                    gain2, add, sin_fn, cur)  # :351 pure
                else:
                    self._frame(out, self.buf1 if inbus == 1 else self.buf2,
                                p.phase, p.freq, gain1, gain2, add,
                                sin_fn, cur)                     # :354
                has_contents[outbus] = True      # :357
            elif not add:                        # :358-360
                has_contents[outbus] = False
            self._skip_frame(p)                  # :361 (param of this entry)
            op += 1

    # --- frame-boundary phase skip (EngineMkI.cpp:361, fm_core.cc:133) ---

    @staticmethod
    def _skip_frame(p: FmParams) -> None:
        p.phase = (p.phase + (i32(p.freq) << LG_N)) & _U32

    # --- frame forms (EngineMkI.cpp:152-293) ------------------------------

    def _frame(self, out: list[int], inp, phase0: int, freq: int,
               gain1: int, gain2: int, add: bool, sin_fn, opn: int) -> None:
        """compute / compute_pure (EngineMkI.cpp:152-182): per sample,
        gain advances first, mkiSin(phase + input), out[i] = y + adder[i],
        phase += freq."""
        dgain = (gain2 - gain1 + (N >> 1)) >> LG_N   # :155
        gain = gain1
        phase = phase0 & _U32
        freq = i32(freq)
        for i in range(N):
            gain += dgain
            mod = 0 if inp is None else inp[i]
            y = sin_fn(opn, i, phase + mod, gain, mod)
            self._mix(out, i, y, add)
            phase = (phase + freq) & _U32

    def _compute_fb(self, out: list[int], phase0: int, freq: int,
                    gain1: int, gain2: int, fb_buf, fb_shift: int,
                    add: bool, sin_fn, opn: int) -> None:
        """compute_fb (EngineMkI.cpp:184-205): one-operator feedback;
        scaled_fb = (y0 + y) >> (fb_shift + 1) read per sample; history
        stored back at frame end."""
        dgain = (gain2 - gain1 + (N >> 1)) >> LG_N
        gain = gain1
        phase = phase0 & _U32
        freq = i32(freq)
        y0 = i32(fb_buf[0])
        y = i32(fb_buf[1])
        for i in range(N):
            gain += dgain
            scaled_fb = i32(y0 + y) >> (fb_shift + 1)   # :196
            y0 = y
            y = sin_fn(opn, i, phase + scaled_fb, gain, scaled_fb)
            self._mix(out, i, y, add)
            phase = (phase + freq) & _U32
        fb_buf[0] = i32(y0)                             # :203-204
        fb_buf[1] = i32(y)

    def _compute_fb2(self, out: list[int], parms, gain01: int, gain02: int,
                     fb_buf, fb_shift: int, sin_fn) -> None:
        """ALGO 6 inline two-op serial (EngineMkI.cpp:208-244): op0 carries
        the feedback, its output feeds op1 within the sample, op1 writes the
        note output. parms[1].gain_out is set here (:218); dgain[1] is the
        pinned full-delta form (:224) — no >> LG_N, normally zero."""
        y0 = i32(fb_buf[0])
        y = i32(fb_buf[1])
        phase0 = parms[0].phase & _U32
        phase1 = parms[1].phase & _U32
        freq0 = i32(parms[0].freq)
        freq1 = i32(parms[1].freq)
        parms[1].gain_out = ENV_MAX - (parms[1].level_in >> 14)   # :218
        gain0 = gain01                                            # :220
        gain1 = (ENV_MAX - 1 if parms[1].gain_out == 0
                 else parms[1].gain_out)                          # :221
        dgain0 = (gain02 - gain01 + (N >> 1)) >> LG_N             # :223
        dgain1 = parms[1].gain_out - gain1                        # :224
        for i in range(N):
            scaled_fb = i32(y0 + y) >> (fb_shift + 1)             # :227
            gain0 += dgain0                                       # :230
            y0 = y                                                # :231
            y = sin_fn(0, i, phase0 + scaled_fb, gain0, scaled_fb)
            phase0 = (phase0 + freq0) & _U32                      # :233
            gain1 += dgain1                                       # :236
            y = sin_fn(1, i, phase1 + y, gain1, y)                # :237
            phase1 = (phase1 + freq1) & _U32                      # :238
            out[i] = i32(y)                                       # :240
        fb_buf[0] = i32(y0)                                       # :242-243
        fb_buf[1] = i32(y)

    def _compute_fb3(self, out: list[int], parms, gain01: int, gain02: int,
                     fb_buf, fb_shift: int, sin_fn) -> None:
        """ALGO 4 inline three-op serial (EngineMkI.cpp:247-293): op0
        carries the feedback; op0 -> op1 -> op2 within the sample; op2
        writes the note output. parms[1]/[2].gain_out are set here
        (:258-259); dgain[1]/[2] are the pinned full-delta form
        (:266-267) — no >> LG_N, normally zero."""
        y0 = i32(fb_buf[0])
        y = i32(fb_buf[1])
        phase0 = parms[0].phase & _U32
        phase1 = parms[1].phase & _U32
        phase2 = parms[2].phase & _U32
        freq0 = i32(parms[0].freq)
        freq1 = i32(parms[1].freq)
        freq2 = i32(parms[2].freq)
        parms[1].gain_out = ENV_MAX - (parms[1].level_in >> 14)   # :258
        parms[2].gain_out = ENV_MAX - (parms[2].level_in >> 14)   # :259
        gain0 = gain01                                            # :261
        gain1 = (ENV_MAX - 1 if parms[1].gain_out == 0
                 else parms[1].gain_out)                          # :262
        gain2 = (ENV_MAX - 1 if parms[2].gain_out == 0
                 else parms[2].gain_out)                          # :263
        dgain0 = (gain02 - gain01 + (N >> 1)) >> LG_N             # :265
        dgain1 = parms[1].gain_out - gain1                        # :266
        dgain2 = parms[2].gain_out - gain2                        # :267
        for i in range(N):
            scaled_fb = i32(y0 + y) >> (fb_shift + 1)             # :271
            gain0 += dgain0                                       # :274
            y0 = y                                                # :275
            y = sin_fn(0, i, phase0 + scaled_fb, gain0, scaled_fb)
            phase0 = (phase0 + freq0) & _U32                      # :277
            gain1 += dgain1                                       # :280
            y = sin_fn(1, i, phase1 + y, gain1, y)                # :281
            phase1 = (phase1 + freq1) & _U32                      # :282
            gain2 += dgain2                                       # :285
            y = sin_fn(2, i, phase2 + y, gain2, y)                # :286
            phase2 = (phase2 + freq2) & _U32                      # :287
            out[i] = i32(y)                                       # :289
        fb_buf[0] = i32(y0)                                       # :291-292
        fb_buf[1] = i32(y)


# ---------------------------------------------------------------------------
# Full-note assembly (dx7note.cc compute path + the wrapper render loop)
# ---------------------------------------------------------------------------

# dx7note.cc:135-137
_AMP_MOD_SENS_TAB = (0, 4342338, 7171437, 16777216)


def _f32(x: float) -> float:
    return struct.unpack("<f", struct.pack("<f", x))[0]


def voice_patch(voice: sysex.Voice) -> list[int]:
    """The 155-byte unpacked single-voice body (Dexed data[0..154],
    PluginData.cpp updateProgramFromSysex memcpy)."""
    body = sysex.encode_voice(voice)
    return list(body[6:6 + sysex.VOICE_DATA_LEN])


class NoteState:
    """Render state of one note slot: the Dx7Note facts (params_, fb_buf_,
    env_, pitch, per-note derived env params), mirrored for single-note
    renders.

    The engine (buses) is shared and owned by the caller; the wrapper builds
    one EngineMkI for the process (oracle main.cpp setEngineMarkI) and
    constructed-once Dx7Note slots reused round-robin (PluginProcessor.cpp:
    143,482-490): a note_on claims the next slot, so sequential
    non-overlapping notes each start from constructor state — phase 0,
    gain_out 0 (dx7note.cc:141-147) and heap-fresh fb_buf_ zeros. This
    model gives every note_on a fresh per-note state, which matches exactly
    that pinned single-note scenario (documented scope: render_note covers
    one non-overlapping note slot; polyphonic slot allocation is H03's).

    keydown() mirrors Dx7Note::init (dx7note.cc:150-200) plus the wrapper's
    osc-key-sync oscSync (oracle main.cpp keydown; dx7note.cc:398-403):
    envelopes re-init, phases/gain_out reset only when patch byte 136 is
    set. keyup() mirrors Dx7Note::keyup (dx7note.cc:297-301).
    """

    def __init__(self, voice: sysex.Voice, patch, midinote: int,
                 velocity: int, router: AlgorithmRouter, *,
                 transpose_mode: str = "pinned_wrapper") -> None:
        self.voice = voice
        self.patch = patch
        self.router = router
        self.params = [FmParams() for _ in range(6)]  # dx7note.cc:141-147
        self.fb_buf = [0, 0]                          # dx7note.h:77
        self.algorithm = patch[134]                   # dx7note.cc:195
        self.fb_shift = fb_shift_for(patch[135])      # dx7note.cc:196-197
        self.ampmodsens = [                           # dx7note.cc:188
            _AMP_MOD_SENS_TAB[patch[k * 21 + 14] & 3]
            for k in range(6)]
        self.transpose_mode = transpose_mode
        self.pitch: NotePitch | None = None
        self.envs: list[Envelope] | None = None
        self.keydown(midinote, velocity)

    def keydown(self, midinote: int, velocity: int) -> None:
        """Dx7Note::init (dx7note.cc:150-200) + osc key sync (data[136])."""
        envs = []
        for k in range(6):
            opv = self.voice.operators[5 - k]  # unpacked block k = op 6-k
            rates, levels, outlevel, rate_scaling = \
                derive_op_envelope_params(opv, midinote, velocity)
            env = Envelope()
            env.init(rates, levels, outlevel, rate_scaling)
            envs.append(env)
        self.envs = envs
        self.pitch = NotePitch(self.patch, midinote,
                               transpose_mode=self.transpose_mode)
        if self.patch[136]:
            for p in self.params:  # Dx7Note::oscSync, dx7note.cc:398-403
                p.phase = 0
                p.gain_out = 0

    def keyup(self) -> None:
        """Dx7Note::keyup (dx7note.cc:297-301)."""
        for env in self.envs:
            env.keydown(False)
        self.pitch.pitchenv_.keydown(False)

    def compute_frame(self, lfo_val: int, lfo_delay: int) -> list[int]:
        """One 64-sample frame of Dx7Note::compute (dx7note.cc:208-296)
        followed by the note-local part of the output conversion."""
        buf = [0] * N  # wrapper zeroes audiobuf per note per frame
        freqs = self.pitch.frame_frequencies(lfo_val, lfo_delay)
        # ==== AMP MOD ==== composition (dx7note.cc:259-268; controllers
        # defaults give ctrls->amp_mod = 0, eg_mod = 127 — refresh(),
        # controllers.h:104-124; pitch.py amp_mod_per_frame encodes it).
        ampmoddepth = (self.patch[140] * 165) >> 6
        amd_mod = amp_mod_per_frame(ampmoddepth, lfo_val, lfo_delay)
        for k in range(6):
            self.params[k].freq = freqs[k]
            level = self.envs[k].getsample()              # dx7note.cc:283
            if self.ampmodsens[k] != 0:                   # dx7note.cc:284-291
                # float-gated spot (NUM-008 item 2): NO_VERDICT, N06 owns
                # the measurement; unused by every N04 case (ams = 0).
                sensamp = ((amd_mod * self.ampmodsens[k]) >> 24) & _U32
                arg = _f32(_f32(float(sensamp)) / 262144.0) * 0.07 + 12.2
                pt = int(math.exp(arg)) & _U32
                level -= ((level * (pt << 4)) >> 28) & _U32
            self.params[k].level_in = level
        self.router.render(buf, self.params, self.algorithm, self.fb_buf,
                           self.fb_shift)
        return buf


def parse_events(events) -> list[tuple[int, str, tuple]]:
    """Normalize events to sorted (pos, cmd, args) tuples.

    Accepts either the oracle events.txt grammar (one '<pos> <cmd> <args>'
    per line; note_on/note_off) or a sequence of (pos, cmd, [args...])
    tuples. Positions are absolute sample offsets; application follows the
    pinned oracle policy (every event with pos inside a block is applied
    before that block, main.cpp:385-407; NUM-011).
    """
    out = []
    if isinstance(events, str):
        for lineno, line in enumerate(events.splitlines(), 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            try:
                pos = int(parts[0])
                args = tuple(int(v) for v in parts[2:])
            except ValueError as exc:
                raise ValueError(f"events line {lineno}: {exc}") from exc
            out.append((pos, parts[1], args))
    else:
        for e in events:
            pos, cmd = e[0], e[1]
            args = tuple(e[2]) if len(e) > 2 else ()
            out.append((pos, cmd, args))
    out.sort(key=lambda e: e[0])
    for pos, cmd, _ in out:
        if cmd not in ("note_on", "note_off"):
            raise ValueError(
                f"unsupported event {cmd!r} at {pos}: N04 renders "
                "note_on/note_off only (cc/pb are N05/N06 scope)")
    return out


def render_note(voice: sysex.Voice, events, *, seconds: float = 4.0,
                rate: int = 48000, transpose_mode: str = "pinned_wrapper",
                log_table=None, exp_table=None) -> bytes:
    """Render one note slot dry and return raw f32le PCM bytes.

    Mirrors the pinned wrapper's render loop (oracle main.cpp:381-470) for
    a single live note: events commit at 64-sample frame boundaries
    (NUM-011), the LFO is sampled once per frame, the note computes one
    64-sample block per frame, and the per-note conversion is
    >>4 -> clip +/-2^24 (negative overflow maps to +0x8000) -> >>9 ->
    /0x8000 (NUM-010). silence before the first note_on is exact zeros.
    """
    if log_table is None or exp_table is None:
        from . import tables as _tables
        log_table = log_table or _tables.sin_log_table()
        exp_table = exp_table or _tables.sin_exp_table()
    total = int(rate * seconds)
    if total % N:
        raise ValueError("seconds must yield a multiple-of-64 sample count")
    evs = parse_events(events)
    pitch_init(float(rate))  # wrapper init: Freqlut/Lfo/PitchEnv (main.cpp init)
    frames = total // N
    patch = voice_patch(voice)
    lfo = Lfo()
    lfo.reset(patch[137:143])     # oracle: lfo.reset(data + 137) at voice load
    router = AlgorithmRouter(log_table, exp_table)  # one engine instance
    note: NoteState | None = None
    clip_vals: list[int] = []
    evi = 0
    for b in range(frames):
        block_end = (b + 1) * N
        while evi < len(evs) and evs[evi][0] < block_end:
            pos, cmd, args = evs[evi]
            if cmd == "note_on":
                midinote, vel = args[0], args[1]
                note = NoteState(voice, patch, midinote, vel, router,
                                 transpose_mode=transpose_mode)
                lfo.keydown()      # wrapper keydown (PluginProcessor.cpp:484)
            elif cmd == "note_off":
                if note is not None:
                    note.keyup()
            evi += 1
        if note is None:
            clip_vals.extend([0] * N)
            continue
        lfo_val = lfo.getsample()          # oracle main.cpp:444-445
        lfo_delay = lfo.getdelay()
        buf = note.compute_frame(lfo_val, lfo_delay)
        clip_vals.extend(clip_val_domain(buf))
    return clip_val_to_f32_bytes(clip_vals)
