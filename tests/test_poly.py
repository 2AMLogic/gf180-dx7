"""N07 polyphonic state/event manager tests (issue #23).

Sections:
1. D00 policy battery (pure model): 16 held + 17th note steals per
   DEC-017 (victim named, old note cut, deterministic); release-phase
   steal preference; repeated same-pitch notes allocate a new voice
   while a free voice exists and reuse the oldest matching voice under
   exhaustion (DEC-016); retrigger continues from the current envelope
   and pitch-EG level (DEC-015); sustain hold + pedal release + CC 120
   override (DEC-018); patch change while held keeps the old patch
   sounding and new notes use the new patch (DEC-019, verified by
   additive renders against per-patch solo renders); all-sound-off
   silences immediately (DEC-021); overload determinism.
2. Isolation: A+B equals the exact integer sum of A-alone and B-alone
   clip streams (NUM-010), A's prefix before B's onset is byte-identical
   to the A-alone render, and the pinned semantics that the SUM is
   stored unclamped (measured: the wrapper mix exceeds ±1.0) is
   reproduced.
3. Negative controls: deliberate cross-voice state sharing must break
   the isolation additivity; the "newest" steal-policy mutator must fail
   the DEC-017 battery naming the case.
4. Committed evidence validation: reference/evidence/n07-poly/
   evidence.json wrapper-valid verdicts + documented divergence row.
5. Live pinned-oracle checks (guarded skip -> NOT_RUN, never a silent
   pass): re-render two wrapper-valid cases and the divergence case.

No fidelity claim: oracle agreement is software-reference agreement only
(reference/oracle-protocol.md section 4). Stdlib-only.
"""

from __future__ import annotations

import json
import struct
import sys
import unittest
from dataclasses import replace
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model.poly import (  # noqa: E402
    PolyManager, StealPolicyError, parse_events, render_poly)
from gf180_dx7.model.pitch import init as pitch_init  # noqa: E402

EVIDENCE = REPO / "reference" / "evidence" / "n07-poly" / "evidence.json"


def _voice(name: bytes, **overrides) -> sysex.Voice:
    op_loud = sysex.Operator(
        rate_1=99, rate_2=99, rate_3=99, rate_4=99,
        level_1=99, level_2=99, level_3=99, level_4=0,
        break_point=0, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=0,
        amp_mod_sensitivity=0, key_velocity_sensitivity=0,
        output_level=99, osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0,
        osc_detune=7)
    op_silent = replace(op_loud, output_level=0)
    common = dict(
        pitch_eg_rate_1=99, pitch_eg_rate_2=99, pitch_eg_rate_3=99,
        pitch_eg_rate_4=99, pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=31, feedback=0, osc_key_sync=1,
        lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name_bytes=name, channel_byte=0)
    common.update(overrides)
    return sysex.Voice(operators=[op_loud] + [op_silent] * 5, **common)


def fast_voice(**overrides) -> sysex.Voice:
    """Loud sustained voice with a fast (rate-99) release."""
    return _voice(b"N07FAST ", **overrides)


def slow_voice(**overrides) -> sysex.Voice:
    """Loud voice with a slow release (rate_4 = 30, level_4 = 40) whose
    release tail is still audible many frames after note-off."""
    op = sysex.Operator(
        rate_1=99, rate_2=80, rate_3=70, rate_4=30,
        level_1=99, level_2=90, level_3=85, level_4=40,
        break_point=0, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=0,
        amp_mod_sensitivity=0, key_velocity_sensitivity=0,
        output_level=99, osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0,
        osc_detune=7)
    common = dict(
        pitch_eg_rate_1=99, pitch_eg_rate_2=99, pitch_eg_rate_3=99,
        pitch_eg_rate_4=99, pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=31, feedback=0, osc_key_sync=1,
        lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name_bytes=b"N07SLOW ", channel_byte=0)
    common.update(overrides)
    return sysex.Voice(operators=[op] + [replace(op, output_level=0)] * 5,
                       **common)


def frames(n: int) -> float:
    """Seconds for exactly n 64-sample frames at 48 kHz."""
    return n * 64 / 48000.0


def hold_16(pm: PolyManager, base: int = 60) -> None:
    """Fill all sixteen slots with held notes (DEC-017 setup)."""
    for i in range(16):
        pm.apply_event("note_on", (base + i, 100))


class _Base(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pitch_init(48000.0)


# ---------------------------------------------------------------------------
# 1. D00 policy battery
# ---------------------------------------------------------------------------


class TestDec017Stealing(_Base):
    def events_16_plus_1(self, base=60, extra=76):
        evs = [(2400 + i * 100, "note_on", (base + i, 100))
               for i in range(16)]
        evs.append((20000, "note_on", (extra, 100)))
        return evs

    def assert_dec017_steals_oldest(self, pm: PolyManager) -> None:
        """The DEC-017 battery assertion (17th note). Raises naming the
        case when the allocation is not the oldest sounding voice."""
        entry = pm.alloc_log[-1]
        # All sixteen notes were struck keydown in slot order, so the
        # oldest sounding voice is slot 0 (pitch 60); none is in release.
        try:
            self.assertEqual(entry["kind"], "steal-dec017", entry)
            self.assertEqual(entry["slot"], 0, entry)
            self.assertEqual(entry["midi_note"], 76, entry)
            self.assertEqual(entry["victim_slot"], 0, entry)
            self.assertEqual(entry["victim_midi_note"], 60, entry)
        except AssertionError as exc:
            raise AssertionError(
                "DEC-017 violated (case ev-steal-17: 16 held notes + a "
                "17th must steal the OLDEST sounding voice, slot 0 / "
                f"pitch 60): {exc}") from exc
        # The victim was re-struck: the old note is gone (cut, not
        # released into a tail — a steal is a retrigger per DEC-015).
        slot0 = pm.slots[0]
        self.assertTrue(slot0.keydown)
        self.assertEqual(slot0.midi_note, 76)
        self.assertEqual(slot0.velocity, 100)
        for k in range(6):
            self.assertEqual(slot0.note.envs[k].ix_, 0,
                             "steal must re-leg the envelopes (DEC-015)")
        # every other voice untouched
        for s in pm.slots[1:]:
            self.assertTrue(s.keydown)
            self.assertEqual(s.midi_note, 60 + s.index)

    def test_17th_note_steals_oldest_sounding_voice(self):
        pm = PolyManager([fast_voice()])
        pm.render(self.events_16_plus_1()[:-1], 0.4)
        pm.apply_event("note_on", (76, 100))     # the 17th note, now
        self.assert_dec017_steals_oldest(pm)     # asserts ix_ == 0 fresh
        pm.render([], 0.1)                       # and it sounds
        pcm16 = PolyManager([fast_voice()]).render(
            self.events_16_plus_1()[:-1], 0.5)
        pcm17 = PolyManager([fast_voice()]).render(
            self.events_16_plus_1(), 0.5)
        self.assertNotEqual(pcm16, pcm17)

    def test_17th_note_render_differs_from_16_note_render(self):
        pcm16 = render_poly([fast_voice()], self.events_16_plus_1()[:-1],
                            seconds=0.5)
        pcm17 = render_poly([fast_voice()], self.events_16_plus_1(),
                            seconds=0.5)
        self.assertNotEqual(pcm16, pcm17)
        rep = compare.compare_streams(pcm16, pcm17)
        self.assertEqual(rep.primary["first_divergent_sample"], 19968,
                         "divergence must start at the 17th note's first "
                         "block (20000 -> block 312)")

    def test_steal_prefers_voices_already_in_release(self):
        """DEC-017 rank 1: released voices are stolen before held ones."""
        pm = PolyManager([slow_voice()])
        hold_16(pm)                      # slots 0..15 held
        pm.apply_event("note_off", (61,))   # slot 1 -> release
        pm.apply_event("note_off", (62,))   # slot 2 -> release
        pm.render([], frames(6))         # ~6 frames of release decay
        self.assertTrue(pm.slots[1].in_release())
        self.assertTrue(pm.slots[2].in_release())
        pm.apply_event("note_on", (80, 100))
        entry = pm.alloc_log[-1]
        self.assertEqual(entry["kind"], "steal-dec017")
        self.assertEqual(entry["victim_slot"], 1,
                         "the OLDEST released voice must die first")
        self.assertEqual(entry["victim_midi_note"], 61)
        self.assertEqual(pm.slots[1].midi_note, 80)
        self.assertTrue(pm.slots[0].keydown and
                        pm.slots[0].midi_note == 60,
                        "held voices must not be touched")
        self.assertEqual(pm.slots[2].midi_note, 62,
                         "the newer released voice survives")

    def test_overload_allocation_is_deterministic(self):
        a = PolyManager([fast_voice()])
        b = PolyManager([fast_voice()])
        pcm_a = a.render(self.events_16_plus_1(), 0.5)
        pcm_b = b.render(self.events_16_plus_1(), 0.5)
        self.assertEqual(pcm_a, pcm_b)
        self.assertEqual(a.alloc_log, b.alloc_log)

    def test_sustained_voices_are_steal_candidates_not_release(self):
        """A sustain-held voice is sounding (DEC-018), not in release:
        it is stolen after every released voice, before nothing."""
        pm = PolyManager([slow_voice()])
        hold_16(pm)
        pm.apply_event("cc", (64, 127))
        pm.apply_event("note_off", (61,))   # sustain-held (not release)
        self.assertTrue(pm.slots[1].sustained)
        self.assertFalse(pm.slots[1].in_release())
        pm.apply_event("note_on", (80, 100))
        entry = pm.alloc_log[-1]
        # No voice is in release -> oldest sounding held voice dies.
        self.assertEqual(entry["kind"], "steal-dec017")
        self.assertEqual(entry["victim_slot"], 0)
        self.assertEqual(entry["victim_midi_note"], 60)


class TestDec016RepeatedNotes(_Base):
    def test_repeated_same_pitch_allocates_new_voice_when_free(self):
        """DEC-016 clause 1: reuse requires full polyphony. With a free
        voice, a repeated same-pitch note stacks a second voice — the
        wrapper-coinciding reading (see docs/N07-POLY-MODEL.md)."""
        pm = PolyManager([fast_voice()])
        pm.render([(0, "note_on", (60, 100)), (6400, "note_on", (60, 100))],
                  0.2)
        self.assertTrue(pm.slots[0].keydown and pm.slots[0].midi_note == 60)
        self.assertTrue(pm.slots[1].keydown and pm.slots[1].midi_note == 60)
        kinds = [e["kind"] for e in pm.alloc_log]
        self.assertEqual(kinds, ["fresh", "fresh"])
        self.assertEqual([e["slot"] for e in pm.alloc_log], [0, 1])

    def test_same_pitch_under_exhaustion_reuses_oldest_matching(self):
        pm = PolyManager([fast_voice()])
        hold_16(pm)
        pm.apply_event("note_on", (60, 70))
        entry = pm.alloc_log[-1]
        self.assertEqual(entry["kind"], "retrigger-dec016")
        self.assertEqual(entry["slot"], 0, "oldest matching voice")
        self.assertEqual(entry["victim_midi_note"], 60)
        self.assertEqual(entry["victim_slot"], 0)
        for s in pm.slots[1:]:
            self.assertTrue(s.keydown and s.midi_note == 60 + s.index,
                            "no other voice may be touched")

    def test_distinct_pitch_under_exhaustion_skips_dec016(self):
        pm = PolyManager([fast_voice()])
        hold_16(pm)
        pm.apply_event("note_on", (90, 100))
        self.assertEqual(pm.alloc_log[-1]["kind"], "steal-dec017")


class TestDec015Retrigger(_Base):
    def test_retrigger_held_voice_continues_from_current_level(self):
        pm = PolyManager([fast_voice()])
        pm.render([(0, "note_on", (60, 100))], 0.2)
        env0 = pm.slots[0].note.envs[0]
        held_level = env0.level_
        self.assertGreater(held_level, 0)
        pitch_level = pm.slots[0].note.pitch.pitchenv_.level_
        for i in range(1, 16):
            pm.apply_event("note_on", (60 + i, 100))
        pm.apply_event("note_on", (60, 70))
        slot0 = pm.slots[0]
        self.assertEqual(pm.alloc_log[-1]["kind"], "retrigger-dec016")
        # DEC-015: the new attack starts AT the current level ...
        self.assertEqual(slot0.note.envs[0].level_, held_level)
        self.assertEqual(slot0.note.pitch.pitchenv_.level_, pitch_level)
        # ... on a fresh segment-0 leg, not a reset-to-zero
        self.assertEqual(slot0.note.envs[0].ix_, 0)
        self.assertGreater(slot0.note.envs[0].targetlevel_, 0)

    def test_retrigger_release_tail_continues_from_current_level(self):
        """Rekeying during a release tail under exhaustion: the matching
        voice is reused and the attack continues from the tail level."""
        pm = PolyManager([slow_voice()])
        pm.render([(0, "note_on", (60, 100))], 0.2)
        pm.apply_event("note_off", (60,))
        pm.render([], frames(6))               # ~6 frames into release
        env0 = pm.slots[0].note.envs[0]
        tail_level = env0.level_
        self.assertFalse(env0.down_)
        self.assertGreater(tail_level, 0)
        for i in range(1, 16):
            pm.apply_event("note_on", (60 + i, 100))
        pm.apply_event("note_on", (60, 80))
        self.assertEqual(pm.alloc_log[-1]["kind"], "retrigger-dec016")
        env0 = pm.slots[0].note.envs[0]
        self.assertEqual(env0.level_, tail_level,
                         "no stale reset: attack starts at the tail level")
        self.assertTrue(env0.down_)
        self.assertEqual(env0.ix_, 0)

    def test_retrigger_phases_and_feedback_carry(self):
        """DEC-020/DEC-015: phases and fb history are not reset by a
        retrigger (free-running per-note state). Uses osc_key_sync=0:
        with the patch's osc-sync parameter SET, the pinned oscSync
        (dx7note.cc:398-403) zeroes them at every strike — the patch's
        own in-patch restart control (DEC-020)."""
        pm = PolyManager([fast_voice(osc_key_sync=0)])
        pm.render([(0, "note_on", (60, 100))], 0.2)
        note = pm.slots[0].note
        phases = [p.phase for p in note.params]
        fb = list(note.fb_buf)
        for i in range(1, 16):
            pm.apply_event("note_on", (60 + i, 100))
        pm.apply_event("note_on", (60, 70))
        note2 = pm.slots[0].note
        self.assertEqual([p.phase for p in note2.params], phases)
        self.assertEqual(note2.fb_buf, fb)


class TestDec018Sustain(_Base):
    def test_sustain_holds_then_pedal_release_starts_release(self):
        pm = PolyManager([fast_voice()])
        pm.render("0 cc 64 127\n3200 note_on 60 100\n"
                  "16000 note_off 60\n", 0.5)
        slot = pm.slots[0]
        self.assertFalse(slot.keydown)
        self.assertTrue(slot.sustained)
        self.assertTrue(slot.note.envs[0].down_,
                        "sustain-hold keeps sounding (DEC-018)")
        pm.apply_event("cc", (64, 0))
        self.assertFalse(slot.sustained)
        self.assertFalse(slot.note.envs[0].down_, "released at pedal-up")

    def test_sustain_holds_across_new_notes(self):
        pm = PolyManager([slow_voice()])
        pm.render([(0, "cc", (64, 127)), (100, "note_on", (60, 100)),
                   (6400, "note_on", (72, 100)), (12800, "note_off", (60,))],
                  0.4)
        self.assertTrue(pm.slots[0].sustained)
        self.assertTrue(pm.slots[1].keydown)
        self.assertTrue(pm.slots[0].sounding(),
                        "held voice still sounds while the new note plays")
        pm.apply_event("cc", (64, 0))
        self.assertFalse(pm.slots[0].sustained)
        self.assertFalse(pm.slots[0].note.envs[0].down_)

    def test_pedal_up_with_no_held_note_offs_changes_nothing(self):
        pm = PolyManager([fast_voice()])
        pm.render("0 cc 64 127\n3200 note_on 60 100\n", 0.2)
        before = [(s.keydown, s.sustained,
                   None if s.note is None else s.note.envs[0].level_)
                  for s in pm.slots]
        pm.apply_event("cc", (64, 0))
        after = [(s.keydown, s.sustained,
                  None if s.note is None else s.note.envs[0].level_)
                 for s in pm.slots]
        self.assertEqual(before, after,
                         "DEC-018: pedal release with no held note-offs "
                         "changes nothing")

    def test_all_sound_off_overrides_sustain(self):
        pm = PolyManager([fast_voice()])
        pm.render("0 cc 64 127\n3200 note_on 60 100\n"
                  "16000 note_off 60\n", 0.4)
        self.assertTrue(pm.slots[0].sustained)
        pm.apply_event("cc", (120, 0))
        self.assertFalse(pm.slots[0].live)
        self.assertFalse(pm.slots[0].sustained)
        pcm = pm.render([], 0.1)
        self.assertEqual(pcm, bytes(len(pcm)))


class TestDec019PatchChangeWhileHeld(_Base):
    def setUp(self):
        super().setUp()
        self.voice_a = fast_voice()
        self.voice_b = slow_voice(feedback=7, osc_key_sync=1)
        self.patches = [self.voice_a, self.voice_b]

    def test_held_voice_keeps_old_patch_new_note_uses_new(self):
        events = [(100, "note_on", (60, 100)), (9600, "patch", (1,)),
                  (19200 + 100, "note_on", (72, 100))]
        # State assertions on one fresh manager (0.5 s covers 24000).
        pm = PolyManager(self.patches)
        pm.render(events, 0.5)
        s0, s1 = pm.slots[0], pm.slots[1]
        self.assertEqual(s0.midi_note, 60)
        self.assertEqual(s1.midi_note, 72)
        self.assertIs(s0.note.voice, self.voice_a)
        self.assertIs(s1.note.voice, self.voice_b)
        self.assertEqual(pm.alloc_log[0]["patch_index"], 0)
        self.assertEqual(pm.alloc_log[1]["patch_index"], 1)
        # The held voice keeps sounding on the OLD patch: the mixed
        # render must equal the exact sum of the two solo renders.
        clips_mixed = PolyManager(self.patches).render_clips(events, 0.5)
        solo_a = PolyManager([self.voice_a]).render_clips(
            [(100, "note_on", (60, 100))], 0.5)
        solo_b = PolyManager([self.voice_b]).render_clips(
            [(19300, "note_on", (72, 100))], 0.5)
        expected = [a + b for a, b in zip(solo_a, solo_b)]
        self.assertEqual(clips_mixed, expected,
                         "held voice must keep the old patch (DEC-019): "
                         "mixed render == patch-A solo + patch-B solo")

    def test_patch_event_is_atomic_at_frame_boundary(self):
        pm = PolyManager(self.patches)
        pm.render([(0, "patch", (1,)), (64, "note_on", (60, 100))], 0.1)
        self.assertEqual(pm.current_voice, 1)
        self.assertIs(pm.slots[0].note.voice, self.voice_b)
        with self.assertRaises(ValueError):
            pm.apply_event("patch", (5,))

    def test_patch_change_reconfigures_lfo_without_phase_reset(self):
        pm = PolyManager(self.patches)
        pm.render([(0, "note_on", (60, 100))], 0.1)
        phase = pm.lfo.phase_
        pm.apply_event("patch", (1,))
        self.assertEqual(pm.lfo.phase_, phase,
                         "LFO phase is free-running (DEC-020)")


class TestDec021AllSoundOff(_Base):
    def test_all_sound_off_kills_all_voices_immediately(self):
        pm = PolyManager([fast_voice()])
        events = [(0, "note_on", (60, 100)), (100, "note_on", (64, 100)),
                  (9600, "cc", (120, 0))]
        pcm = pm.render(events, 0.3)
        for s in pm.slots[:2]:
            self.assertFalse(s.live)
            self.assertFalse(s.keydown)
            self.assertFalse(s.sustained)
        off_at = 9600 // 64 * 64
        tail = pcm[4 * off_at:]
        self.assertEqual(tail, bytes(len(tail)),
                         "exactly silent after the all-sound-off block")
        head = pcm[4 * 64:4 * off_at]
        self.assertNotEqual(head, bytes(len(head)), "audible before")

    def test_cc123_releases_instead_of_killing(self):
        pm = PolyManager([slow_voice()])
        pm.render([(0, "note_on", (60, 100)), (9600, "cc", (123, 0))],
                  0.3)
        self.assertTrue(pm.slots[0].live)
        self.assertFalse(pm.slots[0].keydown)
        self.assertFalse(pm.slots[0].note.envs[0].down_)
        self.assertTrue(pm.slots[0].sounding(), "release tail, not a kill")


class TestOverloadDeterminismAndIsolation(_Base):
    def test_same_input_same_allocation_twice(self):
        events = [(2400 + i * 100, "note_on", (60 + i, 100))
                  for i in range(16)]
        events += [(20000, "note_on", (76, 100)),
                   (22000, "note_on", (77, 100))]
        a = PolyManager([fast_voice()])
        b = PolyManager([fast_voice()])
        pcm_a = a.render(events, 0.5)
        pcm_b = b.render(events, 0.5)
        self.assertEqual(pcm_a, pcm_b)
        self.assertEqual(a.alloc_log, b.alloc_log)
        self.assertEqual(a.alloc_log[-1]["kind"], "steal-dec017")
        self.assertEqual(a.alloc_log[-2]["kind"], "steal-dec017")

    # -- isolation -------------------------------------------------------

    def test_two_voice_sum_is_exact_integer_additivity(self):
        """Isolation (issue acceptance): B must not perturb A. At the
        NUM-010 clip domain the mixed render must equal the exact
        integer sum of the solo clip streams."""
        voice = fast_voice()
        clips_a = PolyManager([voice]).render_clips(
            [(0, "note_on", (60, 100))], 0.4)
        clips_b = PolyManager([voice]).render_clips(
            [(100, "note_on", (72, 100))], 0.4)
        clips_ab = PolyManager([voice]).render_clips(
            [(0, "note_on", (60, 100)), (100, "note_on", (72, 100))], 0.4)
        self.assertEqual(len(clips_ab), len(clips_a))
        for j, (a, b, ab) in enumerate(zip(clips_a, clips_b, clips_ab)):
            self.assertEqual(ab, a + b,
                             f"sample {j}: B perturbed A's contribution")

    def test_voice_a_prefix_is_byte_identical_alone_vs_with_b(self):
        """Bit-identical prefix discipline: before B's onset, A+B equals
        A alone byte for byte (f32le)."""
        voice = fast_voice()
        alone = render_poly([voice], [(0, "note_on", (60, 100))],
                            seconds=0.4)
        both = render_poly([voice], [(0, "note_on", (60, 100)),
                                     (12800, "note_on", (72, 100))],
                           seconds=0.4)
        prefix = 4 * 12800
        self.assertEqual(both[:prefix], alone[:prefix])

    def test_pinned_sum_is_stored_unclamped(self):
        """Measured pinned semantics (oracle 15-voice peak 1.0985): the
        per-voice clip_val clipping is the only saturation; the summed
        mix is stored raw. 16 voices of this patch exceed ±1.0 and the
        model must NOT clamp them."""
        voice = fast_voice()
        events = [(i * 64, "note_on", (60 + i, 100)) for i in range(16)]
        pcm = render_poly([voice], events, seconds=0.2)
        vals = struct.unpack("<%df" % (len(pcm) // 4), pcm)
        peak = max(abs(v) for v in vals)
        self.assertGreater(peak, 1.0,
                           "the pinned raw-mix semantics must be kept")
        # ... and the peak is exactly the integer sum scaled by 2^-15
        clips_ab = PolyManager([voice]).render_clips(events, 0.2)
        peak_idx = max(range(len(vals)), key=lambda i: abs(vals[i]))
        self.assertEqual(vals[peak_idx], clips_ab[peak_idx] / 32768.0)

    def test_sixteen_voice_sum_is_finite_and_bounded(self):
        voice = fast_voice()
        events = [(i * 64, "note_on", (60 + i, 100)) for i in range(16)]
        pcm = render_poly([voice], events, seconds=0.2)
        vals = struct.unpack("<%df" % (len(pcm) // 4), pcm)
        peak = max(abs(v) for v in vals)
        self.assertGreater(peak, 0.0)
        self.assertLessEqual(peak, 16.0, "16 per-voice clip bounds")

    def test_freed_slot_reclaims_after_num012_hold(self):
        """NUM-012: 64 consecutive exact-silence frames reclaim the
        voice; until then the slot is NOT free (steal stock only)."""
        pm = PolyManager([fast_voice()])
        pm.render([(0, "note_on", (60, 100)), (6400, "note_off", (60,))],
                  frames(112))
        slot = pm.slots[0]
        self.assertTrue(slot.live, "release tail keeps the slot")
        pm.render([], 1.5)
        self.assertFalse(slot.live, "reclaimed at the frame boundary")
        self.assertTrue(slot.reclaimed)
        # The reclaimed slot re-enters service when the rotation reaches
        # it: 15 fresh strikes fill slots 1..15, the 16th finds slot 0.
        hold_16(pm)
        self.assertEqual(pm.alloc_log[-1]["kind"], "reuse")
        self.assertEqual(pm.alloc_log[-1]["slot"], 0)

    def test_parse_events_superset_grammar(self):
        evs = parse_events("0 note_on 60 100\n100 patch 1\n")
        self.assertEqual(evs[1], (100, "patch", (1,)))
        with self.assertRaises(ValueError):
            parse_events("0 patch")
        with self.assertRaises(ValueError):
            parse_events("0 pitchmod 3 4")

    def test_steal_policy_validation(self):
        with self.assertRaises(StealPolicyError):
            PolyManager([fast_voice()], steal_policy="random")
        with self.assertRaises(ValueError):
            PolyManager([fast_voice()],
                        negative_control_state_share=(0, 0))
        with self.assertRaises(ValueError):
            PolyManager([])


# ---------------------------------------------------------------------------
# 3. Negative controls (must demonstrably fail the check they target)
# ---------------------------------------------------------------------------


class TestNegativeControls(_Base):
    CONTROL_EVENTS = [(0, "note_on", (60, 100)),
                      (100, "note_on", (72, 100))]

    def clips_solo_and_mixed(self, **kw):
        voice = fast_voice()
        a = PolyManager([voice], **kw).render_clips(
            [self.CONTROL_EVENTS[0]], 0.3)
        b = PolyManager([voice], **kw).render_clips(
            [self.CONTROL_EVENTS[1]], 0.3)
        ab = PolyManager([voice], **kw).render_clips(self.CONTROL_EVENTS,
                                                     0.3)
        return a, b, ab

    def test_state_share_seam_breaks_isolation(self):
        """Deliberate cross-voice state sharing between slots 0 and 1:
        the isolation additivity check must FAIL."""
        a, b, ab = self.clips_solo_and_mixed(
            negative_control_state_share=(0, 1))
        expected = [x + y for x, y in zip(a, b)]
        differing = sum(1 for x, y in zip(ab, expected) if x != y)
        self.assertGreater(
            differing, 0,
            "NEGATIVE CONTROL DID NOT FIRE: state sharing between two "
            "voices did not perturb the isolation additivity check")

    def test_clean_manager_passes_the_same_check(self):
        a, b, ab = self.clips_solo_and_mixed()
        self.assertEqual(ab, [x + y for x, y in zip(a, b)])

    def assert_dec017_battery(self, pm: PolyManager) -> None:
        """The exact battery assertion of TestDec017Stealing, run against
        an arbitrary manager (used by the mutator control)."""
        entry = pm.alloc_log[-1]
        self.assertEqual(entry["kind"], "steal-dec017", entry)
        self.assertEqual(entry["slot"], 0, entry)
        self.assertEqual(entry.get("victim_slot"), 0, entry)
        self.assertEqual(entry.get("victim_midi_note"), 60, entry)

    def test_steal_newest_mutator_fails_battery_naming_case(self):
        events = [(2400 + i * 100, "note_on", (60 + i, 100))
                  for i in range(16)]
        events.append((20000, "note_on", (76, 100)))
        pm = PolyManager([fast_voice()], steal_policy="newest")
        pm.render(events, 0.5)
        with self.assertRaises(AssertionError) as cm:
            self.assert_dec017_battery(pm)
        msg = str(cm.exception)
        self.assertIn("0", msg)
        self.assertNotIn("PASS", msg)
        entry = pm.alloc_log[-1]
        self.assertEqual(entry["victim_slot"], 15,
                         "the mutator must demonstrably steal the NEWEST "
                         "voice, which is not the DEC-017 policy")
        self.assertIn("steal-dec017", entry["kind"],
                      "case: ev-steal-17 / DEC-017 oldest-sounding steal")

    def test_steal_newest_mutator_named_for_what_it_breaks(self):
        # The battery helper's failure message must name the DEC and the
        # case so the control "fails naming the case" (issue #23).
        events = [(2400 + i * 100, "note_on", (60 + i, 100))
                  for i in range(16)]
        events.append((20000, "note_on", (76, 100)))
        pm = PolyManager([fast_voice()], steal_policy="newest")
        pm.render(events, 0.5)
        battery = TestDec017Stealing("test_17th_note_steals_oldest_"
                                     "sounding_voice")
        try:
            battery.assert_dec017_steals_oldest(pm)
        except AssertionError as exc:
            self.assertIn("DEC-017", str(exc))
            self.assertIn("ev-steal-17", str(exc))
        else:
            self.fail("NEGATIVE CONTROL DID NOT FIRE: the newest-steal "
                      "mutator passed the DEC-017 battery")


# ---------------------------------------------------------------------------
# 4. Committed evidence validation
# ---------------------------------------------------------------------------


@unittest.skipUnless(EVIDENCE.is_file(),
                     "committed N07 evidence not present")
class TestCommittedEvidence(_Base):
    @classmethod
    def setUpClass(cls):
        cls.ev = json.loads(EVIDENCE.read_text(encoding="utf-8"))

    def test_schema_and_policy(self):
        self.assertEqual(self.ev["schema"], "gf180-dx7.n07-poly/1")
        self.assertIn("DEC-017", self.ev["policy"])
        self.assertIn("wrapper", self.ev["wrapper_validity_boundary"])

    def test_wrapper_valid_rows_all_exact(self):
        rows = (self.ev["sections"]["wrapper_valid_registry"]
                + self.ev["sections"]
                ["wrapper_valid_dev32_velocity_contrast"])
        ids = {r["id"] for r in rows}
        self.assertTrue({"ev-repeated-note", "ev-retrigger", "ev-sum-15",
                         "ev-steal-16"} <= ids)
        self.assertTrue(any(r["id"] == "dev32-03" and r.get("primary")
                            for r in rows),
                        "one dev32 velocity_contrast entry must be run")
        for r in rows:
            if r["verdict"] == "NOT_RUN":
                continue
            self.assertEqual(r["verdict"], "EXACT", r["id"])
            self.assertTrue(r["byte_identical"], r["id"])
            self.assertEqual(r["max_abs_error"], 0.0, r["id"])

    def test_divergence_row_is_documented_not_budgeted(self):
        rows = self.ev["sections"]["policy_divergence"]
        self.assertEqual(len(rows), 1)
        r = rows[0]
        self.assertEqual(r["id"], "ev-steal-17")
        self.assertEqual(r["verdict"], "POLICY_DIFFERENCE")
        self.assertEqual(r["comparison"], "DIFFER")
        self.assertTrue(r["wrapper_alloc_17th_drop_confirmed"])
        self.assertIn("NOT a defect", r["classification"])
        self.assertIn("NOT a budgeted equality", r["classification"])
        self.assertIsNotNone(r["max_abs_error"])
        self.assertEqual(r["model_allocations"]["steal_entries"][0]
                         ["victim_slot"], 0)

    def test_oracle_identity_pinned(self):
        self.assertTrue(self.ev["oracle"]["observed_sha256_match"])


# ---------------------------------------------------------------------------
# 5. Live pinned-oracle checks (guarded; NOT_RUN when absent)
# ---------------------------------------------------------------------------


def _oracle():
    try:
        from render_reference import check_oracle_binary, load_identity
        identity = load_identity(REPO / "reference" / "oracle-identity.json")
        path = Path(identity["binary"]["path"])
        check_oracle_binary(identity, path)
        return path
    except Exception:  # noqa: BLE001 - guarded skip -> NOT_RUN
        return None


ORACLE = _oracle()
ORACLE_ABSENT = ("pinned oracle binary absent or unpinned; live checks "
                 "NOT_RUN")


@unittest.skipUnless(ORACLE is not None, ORACLE_ABSENT)
class TestLiveOracle(_Base):
    maxDiff = None

    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        from registry_coverage import load_registry
        from registry_smoke import build_voice
        cls.reg = load_registry(REPO / "spec" /
                                "compatibility-registry-v1.json")
        cls.cases = {c["id"]: c for c in cls.reg["cases"]}

    def _oracle_render(self, voice_bytes, events, seconds):
        import subprocess
        import tempfile
        with tempfile.TemporaryDirectory(prefix="n07-live-") as td:
            tmp = Path(td)
            vp, ep, op = tmp / "v.syx", tmp / "e.txt", tmp / "o.f32"
            vp.write_bytes(voice_bytes)
            ep.write_text(events)
            proc = subprocess.run(
                [str(ORACLE), "--voice", str(vp), "--events", str(ep),
                 "--out", str(op), "--rate", "48000",
                 "--seconds", str(seconds)], capture_output=True, text=True)
            self.assertEqual(proc.returncode, 0, proc.stderr)
            return op.read_bytes()

    def _case_render(self, cid):
        from registry_smoke import build_voice
        case = self.cases[cid]
        voice = build_voice(self.reg, case)
        events = "\n".join(case["events"]) + "\n"
        seconds = float(case.get("render_seconds", 1.5))
        ora = self._oracle_render(sysex.encode_voice(voice), events,
                                  seconds)
        model = PolyManager([voice]).render(events, seconds)
        return ora, model

    def test_ev_sum_15_exact(self):
        ora, mod = self._case_render("ev-sum-15")
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "PASS", rep.primary)

    def test_ev_steal_16_exact(self):
        ora, mod = self._case_render("ev-steal-16")
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "PASS", rep.primary)

    def test_ev_steal_17_diverges_as_documented(self):
        """The 17th-note case: the model (D00 DEC-017) deliberately
        diverges from the wrapper (alloc-17th-drop). This test asserts
        the DOCUMENTED difference, not agreement."""
        ora17, mod17 = self._case_render("ev-steal-17")
        rep = compare.compare_streams(ora17, mod17)
        self.assertEqual(rep.verdict, "FAIL",
                         "the documented divergence must be observable")
        self.assertGreater(rep.primary["max_abs_error"], 0.0)
        self.assertEqual(rep.primary["first_divergent_sample"], 3968)
        # wrapper finding alloc-17th-drop: oracle(17th) == oracle(16th)
        ora16, _ = self._case_render("ev-steal-16")
        self.assertEqual(ora17, ora16,
                         "the pinned wrapper must drop the 17th note")


if __name__ == "__main__":
    unittest.main()
