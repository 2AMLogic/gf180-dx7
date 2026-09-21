"""N06 — integrated single-note fixed model (issue #22, contract
gf180-dx7-integer-v1).

Composes the merged module models into one integrated single-note voice and
mirrors the pinned render wrapper (external GPL oracle main.cpp State loop,
oracle commit 0047c3f5 pinned by reference/oracle-identity.json) block for
block:

  * N02 model/operator.py  — mki_sin operator kernel, frame gain ramp,
    output conversion (NUM-004/NUM-010);
  * N03 model/envelope.py  — operator envelopes + per-note derived params
    (NUM-005);
  * N04 model/algorithm.py — AlgorithmRouter, NoteState, feedback
    (NUM-006/NUM-009);
  * N05 model/pitch.py     — NotePitch (basepitch/pitch EG/LFO/bend),
    amp_mod_per_frame (NUM-007/NUM-008).

The wrapper facts mirrored here (read from the pinned oracle main.cpp at
oracle commit 0047c3f5 and the pinned Dexed tree b6cf2e91, R01; semantics
only, zero GPL bytes copied — NUM-001, docs/REUSE-RULINGS.md):

  * per block: apply every event with pos < block_end (NUM-011), consume the
    refreshVoice flag, sample the shared LFO once (getsample + getdelay)
    REGARDLESS of live voices, compute each live voice once, convert per
    voice (>>4 -> clip +/-2^24 with the 0x8000 quirk -> >>9 -> /0x8000) and
    sum (NUM-010 integer-mix proof; single live voice in this model);
  * event grammar (oracle main.cpp:32-35): "<pos> note_on <pitch 0-127>
    <velocity 1-127>", "<pos> note_off <pitch>", "<pos> cc
    <1|2|4|64|120|123> <value 0-127>", "<pos> pb <lsb> <msb>"; note_on with
    velocity 0 is a keyup; controller defaults per reference/manifest.json
    (pitch range up/dn 3, step 0, master tune 0, wheel 0x2000; all four
    FmMod sources default range 0 with pitch/amp/eg false, so CC 1/2/4 are
    render-inert and eg_mod stays 127; mpeEnabled true with per-note
    mpePitchBend = 8192 keeps the MPE bend term zero);
  * voice bookkeeping: 16 slots, currentNote rotation, first non-keydown
    slot allocation, lfo.keydown() per allocated note_on, keyup matches the
    first keydown slot (channel match while mpeEnabled), sustain pedal
    (cc 64) defers Dx7Note::keyup to pedal-up, cc 120 panic (all slots
    keydown/live false + oscSync), cc 123 all-notes-off (keyup all);
  * Dx7Note state lives per slot and is reused when a slot is re-allocated
    (Dx7Note::init does not reset phases/gain_out/fb_buf; only oscSync
    does, when patch byte 136 is set), and the engine buses persist across
    notes (one AlgorithmRouter per render, as in the wrapper).

SINGLE-NOTE SCOPE (polyphony is issue #23/N07): the model renders the
pinned wrapper exactly while at most one live voice is non-silent. A second
allocated voice while a predecessor still sounds raises PolyphonyExceeded —
the case runner records those cases as N07-scope (NOT_RUN), never as a
pass. A predecessor already parked in the NUM-012 exact-silence state (all
six envelopes down, level_ = targetlevel_ = 16 << 16, ix_ >= 3, gain_out >
kLevelThresh) contributes exactly zero forever (Mark I gate,
EngineMkI.cpp:314-318), so the render stays byte-exact and proceeds.

STUB SEAMS (negative controls): render(..., stubs={...}) replaces one module
seam with a constant/stale output; tools/n06_compare.py --stub uses this to
demonstrate suite failure + localization. Documented seam names:
  * "envelope": "constant" — every operator envelope level forced to
    ENV_STUB_LEVEL (state not advanced);
  * "lfo": "zero" — the shared LFO read forced to (0, 0).

Stdlib only.
"""

from __future__ import annotations

import math
import struct

from .. import sysex
from .algorithm import AlgorithmRouter, NoteState, voice_patch
from .envelope import ENV_MAX, derive_op_envelope_params
from .operator import N, clip_val_domain
from .pitch import (Freqlut, Lfo, amp_mod_per_frame,
                    init as pitch_init, i32, u32)

_U32 = 0xFFFFFFFF

MAX_ACTIVE_NOTES = 16          # wrapper main.cpp (PluginProcessor.h)
K_CONTROLLER_PITCH = 128       # controllers.h
K_CONTROLLER_PITCH_RANGE_UP = 129
K_CONTROLLER_PITCH_STEP = 130
K_CONTROLLER_PITCH_RANGE_DN = 131
K_LEVEL_THRESH = ENV_MAX - 100  # EngineMkI.cpp:296 (NUM-009)

LEVEL_FLOOR = 16 << 16         # env.cc:119 clamp floor, stored << 16
ENV_STUB_LEVEL = 99 << 16      # negative-control constant envelope level
LFO_SINE_PINNED = 1 << 23      # pinned wrapper's wave-4 output (finding F-2)


class PolyphonyExceeded(Exception):
    """A case left the single-note scope (issue #23/N07).

    Raised at the first 64-sample block where more than one live voice is
    not in the exact-silence state. Never converted into a pass.
    """

    def __init__(self, block: int, live_slots: list[int]) -> None:
        self.block = block
        self.live_slots = list(live_slots)
        super().__init__(
            f"block {self.block}: live voices {self.live_slots} exceed the "
            "single-note model scope (polyphony is N07)")


class OutOfPinnedConfig(Exception):
    """A wrapper state outside the pinned configuration was reached."""


class PinnedWrapperLfo(Lfo):
    """LFO with the pinned wrapper's observable sine branch (finding F-2).

    The pinned wrapper's State.init (oracle main.cpp:103-107) calls
    Freqlut::init / Lfo::init / PitchEnv::init / Env::init_sr but never
    Sin::init(); msfa's sintab is a zero-initialized global (sin.cc:14,
    static storage), so Sin::lookup returns 0 for every phase and the
    wrapper's LFO sine waveform (wave 4) outputs the constant 1 << 23.
    Oracle-trace evidence: lfo_value == 8388608 on every block of a
    wave-4 render (dev32-01). The frozen profile (NUM-007) specifies the
    regenerated msfa sine table — that is the documented behavior, kept
    available via the base Lfo class; the comparison target is the pinned
    oracle, so the integrated voice defaults to this mirror. The phase
    register still advances exactly like the reference (the output is
    invariant to it in wave 4; nothing else observes it).
    """

    def getsample(self) -> int:
        if self.waveform_ == 4:
            self.phase_ = u32(self.phase_ + self.delta_)
            return LFO_SINE_PINNED
        return super().getsample()


def f32(x: float) -> float:
    """C float (IEEE binary32) narrowing for the float-gated spots."""
    return struct.unpack("<f", struct.pack("<f", x))[0]


# ---------------------------------------------------------------------------
# Controllers (msfa/controllers.h + the wrapper constructor defaults)
# ---------------------------------------------------------------------------


class Controllers:
    """Pinned wrapper controller state (oracle main.cpp State.init).

    values_ follow controllers.h indices; the four FmMod sources keep their
    default-constructed shape (range 0, pitch/amp/eg false), so refresh()
    leaves amp_mod/pitch_mod at 0 and eg_mod at 127 forever — asserted, so
    any future configuration change fails loudly instead of silently
    diverging from the pinned render path (reference/manifest.json
    mod_wheel_default_modifiers).
    """

    def __init__(self) -> None:
        self.values_ = [0] * 132
        self.values_[K_CONTROLLER_PITCH] = 0x2000
        self.values_[K_CONTROLLER_PITCH_RANGE_UP] = 3
        self.values_[K_CONTROLLER_PITCH_RANGE_DN] = 3
        self.values_[K_CONTROLLER_PITCH_STEP] = 0
        self.master_tune = 0
        self.modwheel_cc = 0
        self.breath_cc = 0
        self.foot_cc = 0
        self.aftertouch_cc = 0
        self.amp_mod = 0
        self.pitch_mod = 0
        self.eg_mod = 0
        self.op_switch = "111111"       # wrapper setOpSwitch(0x3F)
        self.mpe_enabled = True         # controllers.h default member init
        self.mpe_pitch_bend_range = 24
        self.refresh()

    def refresh(self) -> None:
        """Controllers::refresh (controllers.h:74-88) reduced to its pinned
        outcome: with every FmMod range 0 and every modifier flag false,
        applyMod contributes nothing and the no-EG rule pins eg_mod = 127
        (controllers.h:66-77, 86-88)."""
        self.amp_mod = 0
        self.pitch_mod = 0
        self.eg_mod = 127
        if (self.amp_mod, self.pitch_mod, self.eg_mod) != (0, 0, 127):
            raise OutOfPinnedConfig("controller mod invariant broken")


# ---------------------------------------------------------------------------
# One note slot (wrapper ProcessorVoice + its slot-owned Dx7Note state)
# ---------------------------------------------------------------------------


class VoiceSlot:
    """Wrapper ProcessorVoice bookkeeping plus the slot-owned NoteState.

    The Dx7Note state (params_ phases/gain_out, fb_buf_) is constructed once
    per slot and REUSED when the allocator hands the slot out again: the
    pinned Dx7Note::init does not reset phases/gain_out/fb_buf (only
    oscSync does, when patch byte 136 is set), so a re-allocated slot
    carries them exactly like the reference.
    """

    def __init__(self, index: int, voice: sysex.Voice, patch,
                 router: AlgorithmRouter) -> None:
        self.index = index
        self.voice = voice
        self.patch = patch
        self.router = router
        self.note: NoteState | None = None   # lazy: constructor state on
        self.channel = 0                     # first use (dx7note.cc:141-147)
        self.midi_note = -1
        self.velocity = 0
        self.keydown = False
        self.sustained = False
        self.live = False

    def allocate(self, channel: int, midinote: int, velocity: int) -> None:
        """Dx7Note::init on this slot (dx7note.cc:150-200): fresh slots
        start from constructor state (NoteState.__init__ zeroes phases/
        gain_out/fb_buf), reused slots keep them (NoteState.keydown
        re-inits envelopes and pitch only)."""
        self.channel = channel
        self.midi_note = midinote
        self.velocity = velocity
        if self.note is None:
            self.note = NoteState(self.voice, self.patch, midinote, velocity,
                                  self.router, transpose_mode="pinned_wrapper")
        else:
            self.note.keydown(midinote, velocity)

    def osc_sync(self) -> None:
        """Dx7Note::oscSync (dx7note.cc:398-403)."""
        if self.note is not None:
            for p in self.note.params:
                p.phase = 0
                p.gain_out = 0

    def key_up(self) -> None:
        """Dx7Note::keyup (dx7note.cc:297-301)."""
        if self.note is not None:
            self.note.keyup()

    def exact_silence(self) -> bool:
        """NUM-012 reclaimable predicate, gate-confirmed (N06 amendment,
        finding F-5): the voice contributes exactly zero from now on when
        every operator envelope is parked at the release clamp floor
        (level_ == targetlevel_ == 16 << 16, ix_ >= 3 — no stepping is
        possible from there: sustained ix_ == 3 with down_ never steps,
        env.cc:74-77; a completed release parks at ix_ == 4, and ix_ == 3
        with down_ false clamps at the floor and advances to 4 in one
        frame, env.cc:90-95) with the previous frame's gain already past
        the Mark I threshold (EngineMkI.cpp:314-318). The parked state is
        output-neutral regardless of the key state, so down_ is not part
        of the predicate (N03 section 7 recorded the same ix_ == 4
        observation against the profile's sustained-only reading)."""
        note = self.note
        if note is None:
            return True
        for k in range(6):
            env = note.envs[k]
            if not (env.level_ == LEVEL_FLOOR
                    and env.targetlevel_ == LEVEL_FLOOR and env.ix_ >= 3):
                return False
            if not note.params[k].gain_out > K_LEVEL_THRESH:
                return False
        return True


# ---------------------------------------------------------------------------
# The integrated single-note voice (wrapper State, single-note scope)
# ---------------------------------------------------------------------------


class IntegratedVoice:
    """One integrated single-note render: patch + events -> PCM.

    Mirrors the pinned wrapper's block loop for the single-live-note state
    space; PolyphonyExceeded marks anything outside it (N07).
    """

    def __init__(self, voice: sysex.Voice, *, rate: int = 48000,
                 log_table=None, exp_table=None,
                 lfo_mode: str = "pinned_wrapper") -> None:
        if log_table is None or exp_table is None:
            from . import tables as _tables
            log_table = log_table or _tables.sin_log_table()
            exp_table = exp_table or _tables.sin_exp_table()
        if lfo_mode not in ("pinned_wrapper", "documented"):
            raise ValueError(f"unknown lfo_mode {lfo_mode!r}")
        self.rate = rate
        pitch_init(float(rate))        # Freqlut/Lfo/PitchEnv (wrapper init)
        self.log_table = log_table
        self.exp_table = exp_table
        self.patch = voice_patch(voice)
        self.voice = voice
        self.lfo_mode = lfo_mode
        self.lfo = PinnedWrapperLfo() if lfo_mode == "pinned_wrapper" \
            else Lfo()
        self.lfo.reset(self.patch[137:143])   # oracle: lfo.reset(data + 137)
        self.controllers = Controllers()
        self.router = AlgorithmRouter(log_table, exp_table)  # one engine
        self.slots = [VoiceSlot(i, voice, self.patch, self.router)
                      for i in range(MAX_ACTIVE_NOTES)]
        self.current_note = 0
        self.sustain = False
        self.refresh_voice = True      # set by updateProgramFromSysex
        self.normalize_dx_velocity = False   # pinned constructor default

    # -- MIDI plumbing (wrapper State.processMidi) --------------------------

    def process_midi(self, cmd: int, b: int, c: int) -> None:
        cf0 = cmd & 0xF0
        channel = (cmd & 0x0F) + 1
        if cf0 == 0x80:
            self.keyup(channel, b)
        elif cf0 == 0x90:
            self.keydown(channel, b, c)
        elif cf0 == 0xB0:
            ctrl, value = b, c
            if ctrl == 1:
                self.controllers.modwheel_cc = value
                self.controllers.refresh()
            elif ctrl == 2:
                self.controllers.breath_cc = value
                self.controllers.refresh()
            elif ctrl == 4:
                self.controllers.foot_cc = value
                self.controllers.refresh()
            elif ctrl == 64:
                self.sustain = value > 63
                if not self.sustain:
                    for slot in self.slots:
                        if slot.sustained and not slot.keydown:
                            slot.key_up()
                            slot.sustained = False
            elif ctrl == 120:
                self.panic()
            elif ctrl == 123:
                for slot in list(self.slots):
                    if slot.keydown:
                        self.keyup(channel, slot.midi_note)
            # unmapped CCs ignored (wrapper default branch)
        elif cf0 == 0xE0:
            self.controllers.values_[K_CONTROLLER_PITCH] = b | (c << 7)
        # 0xD0 aftertouch has no events.txt command (wrapper grammar)

    def keydown(self, channel: int, pitch: int, velocity: int) -> None:
        """Wrapper State.keydown (MPE detection bookkeeping mirrored; it can
        only flip mpe_enabled when two same-channel voices overlap — N07
        scope)."""
        if velocity == 0:
            self.keyup(channel, pitch)
            return
        if self.normalize_dx_velocity:   # pinned: false
            raise OutOfPinnedConfig("normalizeDxVelocity is pinned false")
        if self.controllers.mpe_enabled:
            for i in range(MAX_ACTIVE_NOTES):
                slot = self.slots[(self.current_note + i) % MAX_ACTIVE_NOTES]
                if slot.keydown and slot.channel == channel:
                    self.controllers.mpe_enabled = False
        note = self.current_note
        allocated = False
        for _ in range(MAX_ACTIVE_NOTES):
            if not self.slots[note].keydown:
                self.current_note = (note + 1) % MAX_ACTIVE_NOTES
                self.lfo.keydown()
                self.slots[note].allocate(channel, pitch, velocity)
                self.slots[note].sustained = self.sustain
                self.slots[note].keydown = True
                if self.patch[136]:
                    self.slots[note].osc_sync()
                allocated = True
                break
            note = (note + 1) % MAX_ACTIVE_NOTES
        if not allocated:
            # wrapper quirk: with all 16 slots keydown the loop exits with
            # `note` on the last examined slot and marks it live (steal
            # bookkeeping; the render side of that is N07 scope).
            self.slots[note].live = True
            return
        self.slots[note].live = True

    def keyup(self, channel: int, pitch: int) -> None:
        """Wrapper State.keyup: first keydown slot matching the channel
        (mpeEnabled) or the pitch (otherwise); sustain defers keyup."""
        target = None
        for slot in self.slots:
            matched = ((self.controllers.mpe_enabled
                        and slot.channel == channel)
                       or (not self.controllers.mpe_enabled
                           and slot.midi_note == pitch))
            if matched and slot.keydown:
                target = slot
                break
        if target is None:
            return
        target.keydown = False
        if self.sustain:
            target.sustained = True
        else:
            target.key_up()

    def panic(self) -> None:
        """Wrapper State.panic (cc 120)."""
        for slot in self.slots:
            slot.keydown = False
            slot.live = False
            slot.osc_sync()

    # -- per-frame compute (Dx7Note::compute through the module models) -----

    def compute_frame(self, note: NoteState, lfo_val: int, lfo_delay: int,
                      stubs: dict | None = None) -> list[int]:
        """One 64-sample frame of Dx7Note::compute (dx7note.cc:208-296),
        composed from the merged module models: N05 pitch/mod composition
        (pitch EG + LFO PM + bend, one pitchenv step), the N05 amp-mod
        composition, the N03 envelope step, the NUM-008 float-gated
        amp-mod application, and the N04 router render. opSwitch is pinned
        all-on (wrapper setOpSwitch(0x3F)); anything else raises."""
        ctrls = self.controllers
        if any(ch == "0" for ch in ctrls.op_switch):
            raise OutOfPinnedConfig("opSwitch pins all six operators on")
        if stubs and stubs.get("lfo") == "zero":
            lfo_val, lfo_delay = 0, 0
        # Pitch composition (dx7note.cc:210-257): pitch_mod = pitchenv +
        # max(pmod_1, pmod_2)*sign, then pitch_mod += pitch_base where
        # pitch_base = bend + master tune. Seam finding F-3 (N06 doc/DR-0006):
        # NotePitch.frame_frequencies composes the ratio branch WITHOUT the
        # bend (N05's declared scope excluded bend renders), so the
        # integrated path composes the pinned C++ order explicitly from
        # NotePitch's public pieces. pitch_mod_per_frame advances the pitch
        # EG exactly once per frame and adds pitch_base at the pinned
        # controller defaults (= 0), so adding the real pitch_base here
        # reproduces dx7note.cc exactly.
        pitch_mod_nobase = note.pitch.pitch_mod_per_frame(
            lfo_val, lfo_delay, ctrl_pitch_mod=ctrls.pitch_mod)
        pitch_base, _ = note.pitch._pitch_base(
            ctrls.values_[K_CONTROLLER_PITCH],
            ctrls.values_[K_CONTROLLER_PITCH_RANGE_UP],
            ctrls.values_[K_CONTROLLER_PITCH_RANGE_DN],
            ctrls.values_[K_CONTROLLER_PITCH_STEP],
            ctrls.master_tune)
        freqs = []
        for op in range(6):
            if note.pitch.op_mode[op]:      # fixed: base + pitch_base only
                freqs.append(Freqlut.lookup(
                    i32(note.pitch.basepitch[op] + pitch_base)))
            else:                           # ratio: base + full pitch_mod
                freqs.append(Freqlut.lookup(i32(
                    note.pitch.basepitch[op] + i32(pitch_mod_nobase
                                                   + pitch_base))))
        ampmoddepth = (note.patch[140] * 165) >> 6     # dx7note.cc:199
        amd_mod = amp_mod_per_frame(
            ampmoddepth, lfo_val, lfo_delay,
            ctrl_amp_mod=ctrls.amp_mod, ctrl_eg_mod=ctrls.eg_mod)
        buf = [0] * N
        for k in range(6):
            note.params[k].freq = freqs[k]
            if stubs and stubs.get("envelope") == "constant":
                level = ENV_STUB_LEVEL      # documented negative-control seam
            else:
                level = note.envs[k].getsample()        # dx7note.cc:283
            if note.ampmodsens[k] != 0:                 # dx7note.cc:284-291
                # float-gated spot (NUM-008 item 2): double exp on a float
                # argument, exactly as EngineMkI/Dexed compile it.
                sensamp = ((amd_mod * note.ampmodsens[k]) >> 24) & _U32
                arg = f32(f32(float(sensamp)) / 262144.0) * 0.07 + 12.2
                pt = int(math.exp(arg)) & _U32
                level -= ((level * (pt << 4)) >> 28) & _U32
            note.params[k].level_in = level
        note.router.render(buf, note.params, note.algorithm, note.fb_buf,
                           note.fb_shift)
        return buf

    # -- events --------------------------------------------------------------

    def apply_event(self, cmd: str, args: tuple[int, ...]) -> None:
        """One events.txt command (wrapper grammar, oracle main.cpp:32-35,
        388-402)."""
        if cmd == "note_on" and len(args) >= 2:
            self.process_midi(0x90, args[0], args[1])
        elif cmd == "note_off" and len(args) >= 1:
            self.process_midi(0x80, args[0], 0)
        elif cmd == "cc" and len(args) >= 2:
            self.process_midi(0xB0, args[0], args[1])
        elif cmd == "pb" and len(args) >= 2:
            self.process_midi(0xE0, args[0], args[1])
        else:
            raise ValueError(f"bad event: {cmd} {args}")

    # -- the block loop -------------------------------------------------------

    def _consume_refresh_voice(self) -> None:
        """Wrapper refreshVoice flag: consumed at the top of the first
        block, after that block's events. A voice can be live here only if
        its note_on sat inside block 0; Dx7Note::update then rewrites the
        derived envelope params with identical bytes (the patch is
        unchanged) and forces each envelope into segment 2 (env.cc:153-169
        pinned quirk, mirrored by Envelope.update). Basepitch/opMode/
        ampmodsens recompute to identical values, so the pitch path needs
        no rebuild (and PitchEnv must NOT reset — update never touches it)."""
        if not self.refresh_voice:
            return
        for slot in self.slots:
            if slot.live and slot.note is not None:
                note = slot.note
                for k in range(6):
                    opv = note.voice.operators[5 - k]
                    rates, levels, outlevel, rate_scaling = \
                        derive_op_envelope_params(opv, slot.midi_note,
                                                  slot.velocity)
                    note.envs[k].update(rates, levels, outlevel,
                                        rate_scaling)
        self.refresh_voice = False

    def render_block(self, stubs: dict | None = None) -> list[float]:
        """One 64-sample block (events were already applied by render()):
        the LFO is sampled unconditionally (oracle main.cpp:444-445 runs
        for every block, live notes or not), each live voice computes once,
        and the per-voice clip_val values mix in integers (NUM-010: the
        reference float sum is exactly the integer sum x 2^-15; this model
        renders at most one non-silent voice)."""
        self._consume_refresh_voice()
        live = [s for s in self.slots if s.live]
        sounding = [s for s in live if not s.exact_silence()]
        if len(sounding) > 1:
            raise PolyphonyExceeded(-1, [s.index for s in sounding])
        lfo_val = self.lfo.getsample()
        lfo_delay = self.lfo.getdelay()
        clip_sum = [0] * N
        for slot in live:
            buf = self.compute_frame(slot.note, lfo_val, lfo_delay, stubs)
            for j, c in enumerate(clip_val_domain(buf)):
                clip_sum[j] += c
        out = []
        for c in clip_sum:
            f = 1.0 if c == 0x8000 else c / 32768.0
            out.append(1.0 if f > 1.0 else (-1.0 if f < -1.0 else f))
        return out

    def render(self, events, seconds: float, *,
               stubs: dict | None = None) -> bytes:
        """Render dry f32le PCM for the event list (the events.txt text
        grammar or (pos, cmd, args) tuples; positions are absolute sample
        offsets). Every event with pos inside a block is applied before
        that block (NUM-011; oracle main.cpp:385-407). The LFO steps from
        block 0 even before the first note (unlike the note-gated N04
        render_note loop; see docs/N06-INTEGRATED-MODEL.md seam F-1)."""
        total = int(self.rate * seconds)
        if total % N:
            raise ValueError(
                "seconds must yield a multiple-of-64 sample count")
        evs = parse_events(events)
        frames = total // N
        out = bytearray()
        evi = 0
        for b in range(frames):
            block_end = (b + 1) * N
            while evi < len(evs) and evs[evi][0] < block_end:
                _, cmd, args = evs[evi]
                self.apply_event(cmd, args)
                evi += 1
            try:
                block = self.render_block(stubs=stubs)
            except PolyphonyExceeded as exc:
                if exc.block < 0:
                    exc.block = b
                raise
            out += struct.pack("<%df" % N, *block)
        return bytes(out)


# ---------------------------------------------------------------------------
# Module-level conveniences
# ---------------------------------------------------------------------------


def parse_events(events) -> list[tuple[int, str, tuple]]:
    """Accepts the events.txt text grammar ('<pos> <cmd> <args...>', '#'
    comments) or (pos, cmd, args) tuples; stable-sorted by position."""
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
    for _, cmd, args in out:
        if cmd not in ("note_on", "note_off", "cc", "pb"):
            raise ValueError(
                f"unsupported event {cmd!r}: the pinned wrapper grammar is "
                "note_on/note_off/cc/pb (oracle main.cpp:32-35)")
        want = {"note_on": 2, "note_off": 1, "cc": 2, "pb": 2}[cmd]
        if len(args) < want:
            raise ValueError(f"event {cmd} needs {want} args, got {args}")
    return out


def render_integrated(voice: sysex.Voice, events, *, seconds: float = 4.0,
                      rate: int = 48000, stubs: dict | None = None,
                      lfo_mode: str = "pinned_wrapper") -> bytes:
    """One dry f32le render of `voice` under `events` with the integrated
    single-note model (see IntegratedVoice). Transpose follows the pinned
    wrapper (patch transpose bytes ignored; registry finding
    perf-transpose-12). lfo_mode selects the pinned wrapper's observable
    LFO behavior (default; finding F-2) or the documented msfa sine-table
    behavior."""
    integrated = IntegratedVoice(voice, rate=rate, lfo_mode=lfo_mode)
    return integrated.render(events, seconds, stubs=stubs)
