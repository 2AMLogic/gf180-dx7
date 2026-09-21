"""N07 — 16-voice polyphonic state/event manager (issue #23, contract
gf180-dx7-integer-v1).

One manager, sixteen independent voice slots, one active timbre (DEC-003),
built around the merged N06 single-note model (integrated.py composes N02
operator, N03 envelope, N04 algorithm, N05 pitch — all reused unchanged).
This module owns POLICY: allocation, retrigger, sustain, patch binding,
stealing, and reclamation. Engine arithmetic is not duplicated: the
per-frame Dx7Note::compute composition is IntegratedVoice.compute_frame
applied to each slot's NoteState, and the block loop mirrors the pinned
wrapper's (NUM-011 event commit, NUM-010 integer mix, unconditional LFO
step).

D00 policy mapping (docs/DECISIONS-v1.md; normative):

  * DEC-014 / NUM-011 — events commit at 64-sample frame boundaries; the
    block loop applies every event with pos < block_end before the block.
    No event is ever dropped silently.
  * DEC-015 — retriggering a reused (already-sounding) voice re-legs every
    operator envelope AND the pitch EG from their CURRENT levels (forced
    key-down edge: down_ = False then keydown(True), i.e. env.cc segment-0
    start from level_). Phases and the feedback history are NOT reset
    (DEC-020 free-running phases as the frozen model specifies: the frozen
    slot-reuse path keeps them). Per-note derived params (outlevel,
    rate_scaling, basepitch) are recomputed for the new midinote/velocity.
  * DEC-016 — a note-on for a pitch that already sounds reuses (steals)
    the OLDEST matching voice, but only when no free voice exists; with a
    free voice available a repeated same-pitch note allocates a new voice
    (clause 1's "and no free voice exists"; clause 2 names the reuse
    preference that then applies). This reading keeps every <=16-voice
    registry case wrapper-comparable (the pinned wrapper also stacks).
    The alternative reading (always reuse the matching voice) would
    diverge from the pinned wrapper on ev-repeated-note/ev-retrigger
    without any DEC mandate; it is recorded here, not improvised.
  * DEC-017 — stealing is deterministic and surfaced in alloc_log:
    candidates are live slots; preference (1) voices in release
    (key-up received, not sustain-held), then (2) the oldest sounding
    voice (earliest still-present note-on sequence). The victim is
    retriggered per DEC-015 with the new note. steal_policy="newest"
    exists ONLY as the live negative-control mutator (non-contract).
  * DEC-018 — sustain (CC 64 >63/< =63): a note-off under the pedal marks
    the voice sustain-held (keeps sounding, envelope still down); pedal
    release starts the normal release; pedal release with no held
    note-offs changes nothing. CC 120 (all-sound-off) and reset override
    sustain.
  * DEC-019 — a patch change (this model's extended "patch <n>" event;
    the pinned oracle grammar cannot express it — registry case
    ev-patch-change-held is BLOCKED there) commits atomically at the
    frame boundary. Sounding voices keep the patch they were struck with
    until they are re-struck (their release tails included); new note-ons
    bind the current patch. The chip-global LFO is reconfigured from the
    new patch's LFO bytes without a phase reset (performance machinery,
    like bend; held voices keep their own envelopes).
  * DEC-020 — LFO: free-running except the patch's own key-sync (the
    wrapper's lfo.keydown() per allocated/struck note is kept, matching
    the frozen N06 comparison model); per-note phases free-running.
  * DEC-021 / NUM-012 — no arbitrary tails: a voice whose six operator
    envelopes sit in the exact-silence state (level_ == targetlevel_ ==
    16 << 16, ix_ >= 3, gain_out > 16284) for 64 consecutive frames is
    reclaimed at the frame boundary (output-neutral: the Mark I gate
    already skips it). The only forced kill is CC 120.
  * Note-off matching is PITCH-based (first keydown slot at that pitch,
    lowest slot index): the product policy. The pinned wrapper matches
    the first keydown slot by CHANNEL while mpeEnabled and only falls
    back to pitch after a same-channel overlap flips the MPE flag — a
    wrapper bookkeeping semantic (render-inert at the pinned defaults)
    that this model deliberately does not reproduce. Wrapper-valid
    comparisons therefore use cases where the two matchings coincide.

Wrapper-validity boundary: the pinned wrapper never steals (the 17th note
is dropped — registry finding alloc-17th-drop, docs/REGISTRY-POLICY.md
section 5), so oracle comparison is valid only for <=16-voice cases where
no stealing is triggered AND note-off matching coincides. The 17th-note
case is a DOCUMENTED POLICY DIFFERENCE (D00 DEC-017 vs the wrapper drop),
measured in reference/evidence/n07-poly/evidence.json — not a defect and
not a budgeted equality.

Isolation: one AlgorithmRouter instance is shared by all sixteen slots,
exactly like the pinned wrapper's single engine. This provably leaks
nothing: the router's interconnect buses are only READ by an operator
when a same-frame earlier operator of the SAME voice rendered to that bus
(has_contents is render-local and starts false on the buses), so every
bus word a voice reads it wrote itself that frame. Voice separation is
tested empirically: A+B must equal the exact integer sum of A-alone and
B-alone clip streams (NUM-010), and A's prefix before B's onset must be
byte-identical to the A-alone render.

STUB/CONTROL SEAMS (negative controls):
  * steal_policy="newest" — steals the newest sounding voice; the D00
    battery's 17th-note assertion must fail naming the case.
  * negative_control_state_share=(i, j) — aliases slots i and j onto one
    NoteState (deliberate cross-voice state sharing); the isolation
    additivity tests must fail.

Stdlib only.
"""

from __future__ import annotations

import struct

from .. import sysex
from .algorithm import (AlgorithmRouter, NoteState, _AMP_MOD_SENS_TAB,
                        fb_shift_for, voice_patch)
from .envelope import derive_op_envelope_params
from .integrated import (Controllers, IntegratedVoice, K_CONTROLLER_PITCH,
                         K_CONTROLLER_PITCH_RANGE_DN,
                         K_CONTROLLER_PITCH_RANGE_UP,
                         K_CONTROLLER_PITCH_STEP, K_LEVEL_THRESH,
                         LEVEL_FLOOR, MAX_ACTIVE_NOTES, OutOfPinnedConfig,
                         PinnedWrapperLfo)
from .operator import N, clip_val_domain
from .pitch import Lfo, NotePitch, i32, init as pitch_init

NUM012_HOLD_FRAMES = 64     # NUM-012: 64 consecutive exact-silence frames

ALLOC_KINDS = (
    "fresh",            # never-used slot
    "reuse",            # reclaimed/freed slot (Num-012-frozen state kept)
    "retrigger-dec016",  # same-pitch reuse under exhaustion (DEC-016)
    "steal-dec017",      # general steal (DEC-017)
)


class StealPolicyError(ValueError):
    """An unknown steal_policy value."""


# ---------------------------------------------------------------------------
# One polyphonic voice slot
# ---------------------------------------------------------------------------


class PolyVoiceSlot:
    """One of sixteen independent note slots.

    Owns its NoteState (Dx7Note facts), its bound patch (DEC-019: fixed at
    the striking note-on until the next strike), and its own bookkeeping:
    keydown/sustained/live, the note-on sequence number used for age
    ordering, and the NUM-012 exact-silence hold counter. No state is
    shared with any other slot (isolation is test-enforced).
    """

    def __init__(self, index: int, router: AlgorithmRouter) -> None:
        self.index = index
        self.router = router
        self.voice: sysex.Voice | None = None
        self.patch: list[int] | None = None
        self.note: NoteState | None = None
        self.channel = 0
        self.midi_note = -1
        self.velocity = 0
        self.keydown = False
        self.sustained = False
        self.live = False
        self.reclaimed = False
        self.seq = -1                      # note-on sequence (-1 = never)
        self.silence_hold = 0              # NUM-012 consecutive frames

    # -- state predicates ---------------------------------------------------

    def exact_silence(self) -> bool:
        """NUM-012 exact-silence predicate (gate-confirmed; N06 finding
        F-5 reading: down_ is not part of the predicate)."""
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

    def sounding(self) -> bool:
        """Live and not parked in the NUM-012 exact-silence state."""
        return self.live and not self.exact_silence()

    def in_release(self) -> bool:
        """Key-up received and not sustain-held (DEC-017 preference rank 1)."""
        return self.live and not self.keydown and not self.sustained

    # -- allocation / strikes -------------------------------------------------

    def _bind_patch(self, voice: sysex.Voice, patch: list[int]) -> None:
        """Rebind this slot's timbre (DEC-019: the striking note-on's
        patch), recomputing the patch-derived NoteState members exactly as
        Dx7Note's constructor does (dx7note.cc:188,195-197)."""
        self.voice = voice
        self.patch = patch
        note = self.note
        note.voice = voice
        note.patch = patch
        note.algorithm = patch[134]
        note.fb_shift = fb_shift_for(patch[135])
        note.ampmodsens = [
            _AMP_MOD_SENS_TAB[patch[k * 21 + 14] & 3] for k in range(6)]

    def allocate(self, channel: int, midinote: int, velocity: int,
                 seq: int, voice: sysex.Voice, patch: list[int]) -> None:
        """New note on a free slot: fresh slots start from constructor
        state; freed slots re-enter via the frozen model's reuse path
        (envelopes/pitch re-inited from constructor levels; phases/
        gain_out/fb_buf carried, DEC-020)."""
        self.channel = channel
        self.midi_note = midinote
        self.velocity = velocity
        self.seq = seq
        self.silence_hold = 0
        self.live = True
        self.reclaimed = False
        if self.note is None:
            self.voice = voice
            self.patch = patch
            self.note = NoteState(voice, patch, midinote, velocity,
                                  self.router, transpose_mode="pinned_wrapper")
        else:
            self._bind_patch(voice, patch)
            self.note.keydown(midinote, velocity)

    def retrigger(self, channel: int, midinote: int, velocity: int,
                  seq: int, voice: sysex.Voice, patch: list[int]) -> None:
        """DEC-015 retrigger of an already-sounding voice: every operator
        envelope AND the pitch EG re-leg from their current levels (forced
        key-down edge), derived params recomputed for the new note, phases
        and feedback history carried (DEC-020)."""
        self.channel = channel
        self.midi_note = midinote
        self.velocity = velocity
        self.seq = seq
        self.silence_hold = 0
        self.live = True
        self.reclaimed = False
        self._bind_patch(voice, patch)
        note = self.note
        for k in range(6):
            opv = voice.operators[5 - k]
            rates, levels, outlevel, rate_scaling = \
                derive_op_envelope_params(opv, midinote, velocity)
            env = note.envs[k]
            env.rates_ = list(rates)
            env.levels_ = list(levels)
            env.outlevel_ = outlevel
            env.rate_scaling_ = rate_scaling
            env.down_ = False          # force the key-down edge ...
            env.keydown(True)          # ... segment 0 from current level
        saved_level = note.pitch.pitchenv_.level_
        note.pitch = NotePitch(patch, midinote,
                               transpose_mode=note.transpose_mode)
        pe = note.pitch.pitchenv_
        pe.level_ = i32(saved_level)   # pitch EG also from current level
        pe.down_ = False
        pe.keydown(True)

    def key_up(self) -> None:
        """Dx7Note::keyup (dx7note.cc:297-301)."""
        if self.note is not None:
            self.note.keyup()

    def osc_sync(self) -> None:
        """Dx7Note::oscSync (dx7note.cc:398-403)."""
        if self.note is not None:
            for p in self.note.params:
                p.phase = 0
                p.gain_out = 0


# ---------------------------------------------------------------------------
# The manager (product policy; the wrapper loop around it)
# ---------------------------------------------------------------------------


class PolyManager:
    """Sixteen-voice polyphonic state/event manager per D00.

    voices: one or more sysex.Voice patches; "patch <n>" events select the
    timbre used by NEW note-ons (DEC-019). steal_policy: "oldest" (D00
    DEC-017 contract) or "newest" (negative-control mutator only).
    negative_control_state_share=(i, j): alias two slots onto one
    NoteState (isolation negative control only).
    """

    def __init__(self, voices, *, rate: int = 48000,
                 lfo_mode: str = "pinned_wrapper",
                 steal_policy: str = "oldest",
                 negative_control_state_share=None) -> None:
        voices = list(voices)
        if not voices:
            raise ValueError("PolyManager needs at least one voice")
        if steal_policy not in ("oldest", "newest"):
            raise StealPolicyError(
                f"unknown steal_policy {steal_policy!r} (oldest = DEC-017 "
                "contract; newest is the negative-control mutator)")
        if lfo_mode not in ("pinned_wrapper", "documented"):
            raise ValueError(f"unknown lfo_mode {lfo_mode!r}")
        if negative_control_state_share is not None:
            i, j = negative_control_state_share
            if not (0 <= i < MAX_ACTIVE_NOTES
                    and 0 <= j < MAX_ACTIVE_NOTES and i != j):
                raise ValueError("state-share control needs two slots")
        from . import tables as _tables
        self.rate = rate
        pitch_init(float(rate))    # Freqlut/Lfo/PitchEnv (wrapper init)
        self.log_table = _tables.sin_log_table()
        self.exp_table = _tables.sin_exp_table()
        self.voices = voices
        self.patches = [voice_patch(v) for v in voices]
        self.current_voice = 0
        self.lfo_mode = lfo_mode
        self.lfo = PinnedWrapperLfo() if lfo_mode == "pinned_wrapper" \
            else Lfo()
        self.lfo.reset(self.patches[0][137:143])
        self.controllers = Controllers()
        # ONE engine instance shared by all slots, exactly like the pinned
        # wrapper — provably voice-isolated (module docstring, Isolation).
        self.router = AlgorithmRouter(self.log_table, self.exp_table)
        self.slots = [PolyVoiceSlot(i, self.router)
                      for i in range(MAX_ACTIVE_NOTES)]
        self.current_note = 0
        self.sustain = False
        self.refresh_voice = True      # set by updateProgramFromSysex
        self.normalize_dx_velocity = False   # pinned constructor default
        self.steal_policy = steal_policy
        self._state_share = negative_control_state_share
        self._seq = 0
        # DEC-017: stealing is surfaced in trace metadata, never random.
        self.alloc_log: list[dict] = []

    # -- allocation helpers ---------------------------------------------------

    def _next_seq(self) -> int:
        self._seq += 1
        return self._seq

    def _first_free_slot(self) -> int | None:
        """First free slot scanning round-robin from current_note (the
        wrapper's rotation order; documented deterministic policy)."""
        for k in range(MAX_ACTIVE_NOTES):
            idx = (self.current_note + k) % MAX_ACTIVE_NOTES
            slot = self.slots[idx]
            if not slot.live or slot.reclaimed:
                return idx
        return None

    def _oldest_matching(self, pitch: int) -> PolyVoiceSlot | None:
        """DEC-016: the oldest still-sounding voice at this pitch."""
        best = None
        for slot in self.slots:
            if slot.sounding() and slot.midi_note == pitch:
                if best is None or slot.seq < best.seq:
                    best = slot
        return best

    def _steal_victim(self) -> PolyVoiceSlot:
        """DEC-017: prefer voices in release, then the oldest sounding
        voice; deterministic, never random. steal_policy="newest" inverts
        the age order (negative-control mutator, non-contract)."""
        releasing = [s for s in self.slots if s.in_release()]
        sounding = [s for s in self.slots if s.live and not s.in_release()]
        pool = releasing or sounding
        if not pool:
            raise OutOfPinnedConfig("steal with no live slot to steal")
        key = (lambda s: -s.seq) if self.steal_policy == "newest" \
            else (lambda s: s.seq)
        return min(pool, key=lambda s: (key(s), s.index))

    def _post_strike(self, slot: PolyVoiceSlot, kind: str, *,
                     victim: PolyVoiceSlot | None = None,
                     victim_note: int | None = None,
                     victim_seq: int | None = None) -> None:
        """Shared tail of every strike: osc key sync, pedal bookkeeping,
        LFO keydown, and the DEC-017 alloc_log entry."""
        slot.sustained = self.sustain
        slot.keydown = True
        if slot.patch[136]:
            slot.osc_sync()
        self.lfo.keydown()
        entry = {"seq": slot.seq, "slot": slot.index, "kind": kind,
                 "midi_note": slot.midi_note,
                 "velocity": slot.velocity,
                 "patch_index": self.current_voice}
        if victim is not None:
            entry["victim_slot"] = victim.index
            entry["victim_midi_note"] = victim_note
            entry["victim_seq"] = victim_seq
        self.alloc_log.append(entry)

    def _maybe_share_state(self, slot: PolyVoiceSlot) -> None:
        """Negative-control seam: alias two slots onto one NoteState
        (deliberate cross-voice state sharing; isolation tests must
        fail when this fires)."""
        if self._state_share is None:
            return
        i, j = self._state_share
        if slot.index in (i, j):
            other = self.slots[j if slot.index == i else i]
            if other.note is not None:
                slot.note = other.note

    # -- MIDI plumbing (product policy) ---------------------------------------

    def process_midi(self, cmd: int, b: int, c: int) -> None:
        cf0 = cmd & 0xF0
        channel = (cmd & 0x0F) + 1
        if cf0 == 0x80:
            self.note_off(channel, b)
        elif cf0 == 0x90:
            self.note_on(channel, b, c)
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
                self.set_sustain(value > 63)
            elif ctrl == 120:
                self.all_sound_off()
            elif ctrl == 123:
                for slot in list(self.slots):
                    if slot.keydown:
                        self.note_off(channel, slot.midi_note)
            # unmapped CCs ignored
        elif cf0 == 0xE0:
            self.controllers.values_[K_CONTROLLER_PITCH] = b | (c << 7)
        # 0xD0 aftertouch has no events.txt command (wrapper grammar)

    def note_on(self, channel: int, pitch: int, velocity: int) -> None:
        """D00 allocation: free voice first; under exhaustion the oldest
        same-pitch voice (DEC-016); else the DEC-017 steal order."""
        if velocity == 0:
            self.note_off(channel, pitch)
            return
        if self.normalize_dx_velocity:   # pinned: false
            raise OutOfPinnedConfig("normalizeDxVelocity is pinned false")
        voice = self.voices[self.current_voice]
        patch = self.patches[self.current_voice]
        idx = self._first_free_slot()
        if idx is not None:
            slot = self.slots[idx]
            kind = "fresh" if slot.note is None else "reuse"
            self.current_note = (idx + 1) % MAX_ACTIVE_NOTES
            slot.allocate(channel, pitch, velocity, self._next_seq(),
                          voice, patch)
            self._maybe_share_state(slot)
            self._post_strike(slot, kind)
            return
        match = self._oldest_matching(pitch)
        if match is not None:            # DEC-016 (clause 1 + 2)
            prev_note, prev_seq = match.midi_note, match.seq
            match.retrigger(channel, pitch, velocity, self._next_seq(),
                            voice, patch)
            self._maybe_share_state(match)
            self._post_strike(match, "retrigger-dec016", victim=match,
                              victim_note=prev_note, victim_seq=prev_seq)
            return
        victim = self._steal_victim()    # DEC-017
        prev_note, prev_seq = victim.midi_note, victim.seq
        victim.retrigger(channel, pitch, velocity, self._next_seq(),
                         voice, patch)
        self._maybe_share_state(victim)
        self._post_strike(victim, "steal-dec017", victim=victim,
                          victim_note=prev_note, victim_seq=prev_seq)

    def note_off(self, channel: int, pitch: int) -> None:
        """Product policy: the first keydown slot at this PITCH (lowest
        slot index). The pinned wrapper's channel/MPE-flip matching is a
        documented wrapper semantic, not reproduced here (module
        docstring). Sustain defers the release per DEC-018."""
        target = None
        for slot in self.slots:
            if slot.keydown and slot.midi_note == pitch:
                target = slot
                break
        if target is None:
            return
        target.keydown = False
        if self.sustain:
            target.sustained = True
        else:
            target.key_up()

    def set_sustain(self, down: bool) -> None:
        """DEC-018: pedal-down holds future note-offs; pedal-up releases
        every sustain-held voice; pedal-up with nothing held changes
        nothing."""
        self.sustain = down
        if not down:
            for slot in self.slots:
                if slot.sustained and not slot.keydown:
                    slot.key_up()
                    slot.sustained = False

    def all_sound_off(self) -> None:
        """CC 120 (DEC-021): the only forced kill; overrides sustain."""
        for slot in self.slots:
            slot.keydown = False
            slot.sustained = False
            slot.live = False
            slot.silence_hold = 0
            slot.osc_sync()

    def select_patch(self, index: int) -> None:
        """DEC-019: atomic timbre selection at the (already
        frame-boundary-committed) event; sounding voices keep their bound
        patch; the chip-global LFO reconfigures to the new patch's LFO
        bytes without a phase reset."""
        if not 0 <= index < len(self.voices):
            raise ValueError(f"patch index {index} outside "
                             f"0..{len(self.voices) - 1}")
        self.current_voice = index
        self.lfo.reset(self.patches[index][137:143])

    # -- per-frame compute ------------------------------------------------------

    def compute_frame(self, note: NoteState, lfo_val: int, lfo_delay: int,
                      stubs: dict | None = None) -> list[int]:
        """One 64-sample frame of Dx7Note::compute for one voice — the
        frozen N06 composition, applied unchanged (IntegratedVoice owns
        the exact dx7note.cc:208-296 order; this manager only supplies
        its controller state)."""
        return IntegratedVoice.compute_frame(self, note, lfo_val, lfo_delay,
                                             stubs)

    def _consume_refresh_voice(self) -> None:
        """Wrapper refreshVoice flag on the first block; per-slot patch
        binding (each live voice updates against ITS OWN patch)."""
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

    def _reclaim_pass(self) -> None:
        """NUM-012 at the frame boundary: a voice whose exact-silence
        predicate held for 64 consecutive frames is reclaimed
        (output-neutral; DEC-021)."""
        for slot in self.slots:
            if not slot.live:
                slot.silence_hold = 0
                continue
            if slot.exact_silence():
                slot.silence_hold += 1
                if slot.silence_hold >= NUM012_HOLD_FRAMES:
                    slot.live = False
                    slot.reclaimed = True
                    slot.silence_hold = 0
            else:
                slot.silence_hold = 0

    def _block_clips(self, stubs: dict | None = None) -> list[int]:
        """One 64-sample block after its events were applied: the LFO is
        sampled unconditionally (F-1), every live voice computes once,
        per-voice clip_val values mix in an exact integer sum (NUM-010;
        slot-index order — integer addition is exact and order-free)."""
        self._consume_refresh_voice()
        lfo_val = self.lfo.getsample()
        lfo_delay = self.lfo.getdelay()
        clip_sum = [0] * N
        for slot in self.slots:
            if not slot.live:
                continue
            buf = self.compute_frame(slot.note, lfo_val, lfo_delay, stubs)
            for j, c in enumerate(clip_val_domain(buf)):
                clip_sum[j] += c
        self._reclaim_pass()
        return clip_sum

    def render_block(self, stubs: dict | None = None) -> list[float]:
        """One 64-sample block as f32 floats (raw mix; _sum_to_f32)."""
        return [_sum_to_f32(c) for c in self._block_clips(stubs)]

    def render_clips(self, events, seconds: float,
                     stubs: dict | None = None) -> list[int]:
        """Render to the pre-conversion integer clip stream (NUM-010
        product transport domain). Isolation tests compare this stream:
        A+B must equal clip_A + clip_B exactly."""
        total = int(self.rate * seconds)
        if total % N:
            raise ValueError(
                "seconds must yield a multiple-of-64 sample count")
        evs = parse_events(events)
        frames = total // N
        clips: list[int] = []
        evi = 0
        for b in range(frames):
            block_end = (b + 1) * N
            while evi < len(evs) and evs[evi][0] < block_end:
                _, cmd, args = evs[evi]
                self.apply_event(cmd, args)
                evi += 1
            clips.extend(self._block_clips(stubs))
        return clips

    def render(self, events, seconds: float, *,
               stubs: dict | None = None) -> bytes:
        """Dry f32le PCM for the event list (DEC-012 dry; NUM-010 mix).
        The mix is stored raw (unclamped at the sum; measured pinned
        semantics — see _sum_to_f32)."""
        clips = self.render_clips(events, seconds, stubs=stubs)
        return struct.pack("<%df" % len(clips),
                           *[_sum_to_f32(c) for c in clips])

    # -- events -------------------------------------------------------------

    def apply_event(self, cmd: str, args: tuple[int, ...]) -> None:
        """One event: the pinned wrapper grammar (note_on/note_off/cc/pb)
        plus this model's extended "patch <n>" (DEC-019; not expressible
        in the oracle grammar — registry ev-patch-change-held is BLOCKED
        there)."""
        if cmd == "note_on" and len(args) >= 2:
            self.process_midi(0x90, args[0], args[1])
        elif cmd == "note_off" and len(args) >= 1:
            self.process_midi(0x80, args[0], 0)
        elif cmd == "cc" and len(args) >= 2:
            self.process_midi(0xB0, args[0], args[1])
        elif cmd == "pb" and len(args) >= 2:
            self.process_midi(0xE0, args[0], args[1])
        elif cmd == "patch" and len(args) >= 1:
            self.select_patch(args[0])
        else:
            raise ValueError(f"bad event: {cmd} {args}")


def _sum_to_f32(c: int) -> float:
    """Mix conversion (NUM-010, measured multi-voice semantics): the mix
    is the exact integer sum x 2^-15 stored raw — the pinned wrapper
    clamps NOTHING at the sum (per-voice clip_val clipping is the only
    saturation; measured oracle peak 1.0985 on a 15-voice render). The
    0x8000 quirk value is +32768 in the integer domain and maps to +1.0
    per voice exactly as c / 32768.0."""
    return c / 32768.0


def parse_events(events) -> list[tuple[int, str, tuple]]:
    """Superset of integrated.parse_events accepting the additional
    "patch" command ('<pos> patch <index>'); same sorting and validation
    discipline (stable sort by position; strict arity)."""
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
        if cmd not in ("note_on", "note_off", "cc", "pb", "patch"):
            raise ValueError(
                f"unsupported event {cmd!r}: the poly grammar is "
                "note_on/note_off/cc/pb/patch")
        want = {"note_on": 2, "note_off": 1, "cc": 2, "pb": 2,
                "patch": 1}[cmd]
        if len(args) < want:
            raise ValueError(f"event {cmd} needs {want} args, got {args}")
    return out


def render_poly(voices, events, *, seconds: float = 2.0, rate: int = 48000,
                stubs: dict | None = None,
                steal_policy: str = "oldest") -> bytes:
    """One dry f32le render of `voices` under `events` with the D00
    polyphonic manager."""
    manager = PolyManager(voices, rate=rate, steal_policy=steal_policy)
    return manager.render(events, seconds, stubs=stubs)
