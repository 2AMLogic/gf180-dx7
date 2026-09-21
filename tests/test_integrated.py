"""N06 integrated single-note model tests (issue #22).

Sections:
1. Wrapper bookkeeping (pure model): the pinned oracle wrapper's event
   grammar and voice-slot semantics — pb/cc/note_on(vel 0)/sustain/panic/
   all-notes-off, slot allocation, CC1 inertness at pinned FmMod defaults.
2. Composition semantics (pure model): bend reaches ratio-mode operators
   through pitch_base (seam finding F-2), fixed-mode operators do not see
   pitch EG/LFO, LFO stepping from block 0 during silence, the
   PinnedWrapperLfo zeroed-sintab mirror, single-note scope guard
   (PolyphonyExceeded) and the NUM-012 exact-silence continuation.
3. Negative-control seams (pure model): the documented envelope/lfo stubs
   change the render (the oracle-compared control lives in
   reference/evidence/n06-integrated/negative-control.json and is
   re-verified live below when the oracle is present).
4. Committed evidence validation: reference/evidence/n06-integrated/
   dev32-subset.json covers >= 8 dev32 entries + >= 12 registry cases with
   EXACT/N07_SCOPE/FAIL verdicts exactly as documented; negative-control
   json control_fired.
5. Live pinned-oracle checks (guarded skip -> NOT_RUN, never a silent
   pass): re-render two representative cases and one stubbed case.

No fidelity claim: oracle agreement is software-reference agreement only
(reference/oracle-protocol.md section 4). Stdlib-only.
"""

from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model import integrated as IG  # noqa: E402
from gf180_dx7.model.integrated import (  # noqa: E402
    Controllers, IntegratedVoice, K_CONTROLLER_PITCH, PinnedWrapperLfo,
    PolyphonyExceeded, parse_events, render_integrated)
from gf180_dx7.model.pitch import Freqlut, init as pitch_init, Lfo  # noqa: E402

EVIDENCE = REPO / "reference" / "evidence" / "n06-integrated"
DEV32_JSON = REPO / "corpus" / "dev32.json"


def synth_voice(**overrides) -> sysex.Voice:
    """Audible parked-envelope single-carrier voice (algorithm 32: every
    op drives the note output; one operator audible, the rest gated to
    exact silence), neutral LFO/pitch bytes."""
    op_loud = sysex.Operator(
        rate_1=99, rate_2=99, rate_3=99, rate_4=99,
        level_1=99, level_2=99, level_3=99, level_4=0,
        break_point=0, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=0,
        amp_mod_sensitivity=0, key_velocity_sensitivity=0,
        output_level=99, osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0,
        osc_detune=7)
    op_silent = sysex.Operator(
        rate_1=99, rate_2=99, rate_3=99, rate_4=99,
        level_1=99, level_2=99, level_3=99, level_4=0,
        break_point=0, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=0,
        amp_mod_sensitivity=0, key_velocity_sensitivity=0,
        output_level=0, osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0,
        osc_detune=7)
    common = dict(
        pitch_eg_rate_1=99, pitch_eg_rate_2=99, pitch_eg_rate_3=99,
        pitch_eg_rate_4=99, pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=31, feedback=0, osc_key_sync=1,
        lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name_bytes=b"N06TEST", channel_byte=0)
    common.update(overrides)
    return sysex.Voice(operators=[op_loud] + [op_silent] * 5, **common)


def lfo_active_voice(**overrides) -> sysex.Voice:
    """Voice whose LFO reaches the PCM (pitch-PM active, keysync on)."""
    common = dict(lfo_speed=40, lfo_delay=0, lfo_pitch_mod_depth=99,
                  lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
                  lfo_pitch_mod_sensitivity=3)
    common.update(overrides)
    return synth_voice(**common)


class _Base(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        pitch_init(48000.0)


# ---------------------------------------------------------------------------
# 1. Wrapper bookkeeping


class TestWrapperBookkeeping(_Base):
    def test_pb_event_sets_controller_value(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.apply_event("pb", (127, 127))
        self.assertEqual(
            iv.controllers.values_[K_CONTROLLER_PITCH], 127 | (127 << 7))
        iv.apply_event("pb", (0, 64))
        self.assertEqual(
            iv.controllers.values_[K_CONTROLLER_PITCH], 0x2000)

    def test_parse_events_grammar_and_sort(self):
        evs = parse_events("4800 note_on 60 100\n"
                           "# comment\n"
                           "0 cc 64 127\n"
                           "240 pb 0 64\n"
                           "96000 note_off 60\n")
        self.assertEqual([e[0] for e in evs], [0, 240, 4800, 96000])
        self.assertEqual(evs[0][1:], ("cc", (64, 127)))
        self.assertEqual(evs[3][1:], ("note_off", (60,)))
        with self.assertRaises(ValueError):
            parse_events("0 pitchmod 3 4")
        with self.assertRaises(ValueError):
            parse_events("0 note_on 60")

    def test_note_on_velocity_zero_is_keyup(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.render("0 note_on 60 100\n3200 note_on 60 0\n", 0.2)
        self.assertFalse(iv.slots[0].keydown)
        self.assertTrue(iv.slots[0].live)  # stays live like the wrapper

    def test_sustain_defers_release_until_pedal_up(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.render("0 cc 64 127\n3200 note_on 60 100\n"
                  "16000 note_off 60\n", 0.5)
        slot = iv.slots[0]
        self.assertFalse(slot.keydown)
        self.assertTrue(slot.sustained)
        self.assertTrue(slot.note.envs[0].down_)  # still held
        iv.apply_event("cc", (64, 0))             # pedal up
        self.assertFalse(slot.sustained)
        self.assertFalse(slot.note.envs[0].down_)  # released now

    def test_panic_clears_live_and_resets_osc_state(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.render("0 note_on 60 100\n", 0.1)
        self.assertTrue(iv.slots[0].live)
        note = iv.slots[0].note
        note.params[0].phase = 12345
        note.params[0].gain_out = 999
        iv.apply_event("cc", (120, 0))
        for slot in iv.slots:
            self.assertFalse(slot.live)
            self.assertFalse(slot.keydown)
        self.assertEqual(note.params[0].phase, 0)
        self.assertEqual(note.params[0].gain_out, 0)

    def test_cc123_releases_keydown_notes(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.render("0 note_on 60 100\n", 0.1)
        iv.apply_event("cc", (123, 0))
        self.assertFalse(iv.slots[0].keydown)
        self.assertTrue(iv.slots[0].live)  # release tail, like the wrapper

    def test_unmapped_cc_is_ignored(self):
        a = render_integrated(synth_voice(), "0 note_on 60 100\n"
                                             "3200 cc 7 99\n", seconds=0.2)
        b = render_integrated(synth_voice(), "0 note_on 60 100\n",
                              seconds=0.2)
        self.assertEqual(a, b)

    def test_cc1_inert_at_pinned_fmmod_defaults(self):
        a = render_integrated(synth_voice(), "0 note_on 60 100\n"
                                             "3200 cc 1 127\n", seconds=0.3)
        b = render_integrated(synth_voice(), "0 note_on 60 100\n"
                                             "3200 cc 1 0\n", seconds=0.3)
        self.assertEqual(a, b)

    def test_controllers_refresh_pinned_invariant(self):
        c = Controllers()
        c.modwheel_cc = 127
        c.breath_cc = 127
        c.foot_cc = 127
        c.refresh()
        self.assertEqual((c.pitch_mod, c.amp_mod, c.eg_mod), (0, 0, 127))

    def test_sequential_notes_allocate_fresh_slots(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        pcm = iv.render("0 note_on 60 100\n32000 note_off 60\n"
                        "64000 note_on 72 100\n", 1.4)
        self.assertEqual(len(pcm), 4 * 67200)
        self.assertTrue(iv.slots[0].live and not iv.slots[0].keydown)
        self.assertTrue(iv.slots[1].keydown)
        self.assertEqual(iv.current_note, 2)
        # a fresh allocation starts from constructor state (dx7note.cc
        # constructor zeroes phases/gain_out; fb_buf is heap-fresh)
        fresh = IntegratedVoice(synth_voice(), rate=48000)
        fresh.apply_event("note_on", (72, 100))
        self.assertEqual(fresh.slots[0].note.params[0].phase, 0)
        self.assertEqual(fresh.slots[0].note.params[0].gain_out, 0)
        self.assertEqual(fresh.slots[0].note.fb_buf, [0, 0])

    def test_mpe_flag_stays_true_for_single_note(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.render("0 note_on 60 100\n32000 note_off 60\n"
                  "64000 note_on 72 100\n", 1.4)
        self.assertTrue(iv.controllers.mpe_enabled)


# ---------------------------------------------------------------------------
# 2. Composition semantics


class TestComposition(_Base):
    def test_bend_reaches_ratio_operators(self):
        """Seam finding F-2: ratio ops take base + pitchenv + pmod +
        pitch_base (dx7note.cc adds pitch_base to pitch_mod before the op
        loop); pitch.py's frame_frequencies drops the bend on the ratio
        branch, so the integrated path composes the pinned order."""
        iv = IntegratedVoice(synth_voice(), rate=48000)
        iv.apply_event("note_on", (60, 100))
        iv.apply_event("pb", (127, 127))
        note = iv.slots[0].note
        pitch_base, _ = note.pitch._pitch_base(16383, 3, 3, 0, 0)
        self.assertEqual(pitch_base, 4193792)  # (8191<<11)*3/12 exactly
        iv.compute_frame(note, 1 << 23, 0)
        for k in range(6):
            self.assertEqual(
                note.params[k].freq,
                Freqlut.lookup(note.pitch.basepitch[k] + pitch_base),
                f"op {k} must see base + pitch_base via pitch_mod")
        # and the render actually differs from no-bend
        a = render_integrated(synth_voice(),
                              "0 note_on 60 100\n3200 pb 127 127\n",
                              seconds=0.3)
        b = render_integrated(synth_voice(), "0 note_on 60 100\n",
                              seconds=0.3)
        self.assertNotEqual(a, b)

    def test_fixed_mode_operator_ignores_pitch_mod(self):
        """Engine param k is repo operators[5-k] (algorithm.py); making
        operators[0] fixed gives a fixed engine param 5. Ratio ops track
        the LFO (pitch_mod), fixed ops see pitch_base only."""
        from dataclasses import replace
        voice = lfo_active_voice()
        ops = list(voice.operators)
        ops[0] = replace(ops[0], osc_mode=1, osc_freq_coarse=4)
        voice_fixed = replace(voice, operators=ops)
        iv = IntegratedVoice(voice_fixed, rate=48000)
        iv.apply_event("note_on", (60, 100))
        note = iv.slots[0].note
        iv.compute_frame(note, 1 << 23, 1 << 23)   # LFO zero point
        fixed_a, ratio_a = note.params[5].freq, note.params[0].freq
        iv.compute_frame(note, 0, 1 << 23)         # LFO low
        fixed_b, ratio_b = note.params[5].freq, note.params[0].freq
        iv.compute_frame(note, (1 << 24) - 1, 1 << 23)  # LFO high
        fixed_c, ratio_c = note.params[5].freq, note.params[0].freq
        self.assertEqual(fixed_a, fixed_b)
        self.assertEqual(fixed_a, fixed_c)  # fixed: no LFO/pitch-EG
        self.assertNotEqual(ratio_a, ratio_b)
        self.assertNotEqual(ratio_b, ratio_c)  # ratio: tracks pitch_mod

    def test_lfo_steps_from_block_0_before_first_note(self):
        """The shared LFO steps every block from block 0, live voices or
        not (oracle main.cpp:444-445; N06 seam F-1 vs the note-gated N04
        render loop)."""
        iv = IntegratedVoice(lfo_active_voice(), rate=48000)
        iv.render("4800 note_on 60 100\n", 0.2)
        # Replaying the LFO for the same 150-block timeline (keydown at
        # block 75 from the note-on) must land on the render's state.
        replay = PinnedWrapperLfo()
        replay.reset(iv.patch[137:143])
        for _ in range(75):
            replay.getsample(), replay.getdelay()
        replay.keydown()               # the note_on's lfo.keydown()
        for _ in range(150 - 75):
            replay.getsample(), replay.getdelay()
        self.assertEqual(iv.lfo.phase_, replay.phase_)
        # a note-gated LFO would have missed the 75 pre-onset steps
        gated = PinnedWrapperLfo()
        gated.reset(iv.patch[137:143])
        gated.keydown()
        self.assertNotEqual(iv.lfo.phase_, gated.phase_)

    def test_pinned_wrapper_lfo_sine_branch_is_constant(self):
        """Finding F-2: the wrapper never calls Sin::init(); its zeroed
        sintab makes wave-4 output the constant 1<<23."""
        lfo = PinnedWrapperLfo()
        lfo.reset([36, 0, 0, 0, 0, 4])
        outs = {lfo.getsample() for _ in range(200)}
        self.assertEqual(outs, {1 << 23})
        documented = Lfo()
        documented.reset([36, 0, 0, 0, 0, 4])
        self.assertGreater(len({documented.getsample() for _ in range(200)}),
                           1)

    def test_single_note_scope_guard_fires_on_second_keydown(self):
        iv = IntegratedVoice(synth_voice(), rate=48000)
        with self.assertRaises(PolyphonyExceeded) as ctx:
            iv.render("0 note_on 60 100\n6400 note_on 64 100\n", 0.3)
        self.assertEqual(ctx.exception.block, 100)  # 6400 // 64
        self.assertEqual(ctx.exception.live_slots, [0, 1])

    def test_sequential_note_after_exact_silence_renders(self):
        """Rate-99 release parks at the NUM-012 floor quickly, so a note_on
        one second later stays single-note (mirrors the N04 retrigger
        scenario byte-exactly against the oracle)."""
        iv = IntegratedVoice(synth_voice(), rate=48000)
        pcm = iv.render("0 note_on 60 100\n32000 note_off 60\n"
                        "64000 note_on 72 100\n", 1.4)
        self.assertEqual(len(pcm), 4 * 67200)

    def test_silence_before_onset_is_exact_zeros(self):
        pcm = render_integrated(synth_voice(), "4800 note_on 60 100\n",
                                seconds=0.2)
        self.assertEqual(pcm[:4 * 4800], bytes(4 * 4800))
        self.assertNotEqual(pcm[4 * 4800:], bytes(len(pcm) - 4 * 4800))


# ---------------------------------------------------------------------------
# 3. Negative-control seams (pure-model half)


class TestStubSeams(_Base):
    def test_envelope_stub_changes_render(self):
        events = "0 note_on 60 100\n32000 note_off 60\n"
        clean = render_integrated(synth_voice(), events, seconds=1.0)
        stubbed = render_integrated(synth_voice(), events, seconds=1.0,
                                    stubs={"envelope": "constant"})
        self.assertNotEqual(clean, stubbed)
        rep = compare.compare_streams(clean, stubbed)
        self.assertEqual(rep.verdict, "FAIL")
        self.assertLess(rep.primary["first_divergent_sample"], 64)

    def test_lfo_stub_changes_active_lfo_render(self):
        events = "0 note_on 60 100\n"
        clean = render_integrated(lfo_active_voice(), events, seconds=0.5)
        stubbed = render_integrated(lfo_active_voice(), events, seconds=0.5,
                                    stubs={"lfo": "zero"})
        self.assertNotEqual(clean, stubbed)


# ---------------------------------------------------------------------------
# 4. Committed evidence validation


@unittest.skipUnless((EVIDENCE / "dev32-subset.json").is_file(),
                     "committed N06 evidence not present")
class TestCommittedEvidence(_Base):
    @classmethod
    def setUpClass(cls):
        cls.dev = json.loads(
            (EVIDENCE / "dev32-subset.json").read_text(encoding="utf-8"))

    def test_schema_and_target(self):
        self.assertEqual(self.dev["schema"], "gf180-dx7.n06-integrated/1")
        self.assertIn("EXACT", self.dev["target"])

    def test_dev32_subset_covers_required_axes(self):
        rows = {r["id"]: r for r in self.dev["sections"]["dev32"]}
        self.assertGreaterEqual(
            sum(1 for r in rows.values() if r["verdict"] != "NOT_RUN"), 8,
            "issue #22 requires >= 8 dev32 entries run end-to-end")
        manifest = json.loads(DEV32_JSON.read_text(encoding="utf-8"))
        alg = {e["index"]: e["decoded_features"]["algorithm"]
               for e in manifest["entries"]}
        ran = {int(k.split("-")[1]) for k, r in rows.items()
               if r["verdict"] != "NOT_RUN"}
        for required in (1, 4, 6, 23):
            self.assertIn(required, ran,
                          f"dev32 subset must cover algorithm {required}")
        waves = {r["reference_lfo"]["lfo_wave"] for r in rows.values()
                 if "reference_lfo" in r}
        self.assertGreaterEqual(waves, {0, 1, 2, 3, 4, 5},
                                "all six LFO waves must appear")
        # fixed-frequency operators present (entries 1 and 2)
        fixed = {e["index"] for e in manifest["entries"]
                 if e["decoded_features"]["fixed_mode_operators"] > 0}
        self.assertTrue(ran & fixed, "fixed-mode coverage missing")

    def test_registry_coverage_and_verdicts(self):
        rows = self.dev["sections"]["registry"]
        exact = [r for r in rows if r["verdict"] == "EXACT"]
        self.assertGreaterEqual(len(exact), 12,
                                "issue #22 requires >= 12 registry cases")
        n07 = {r["id"] for r in rows if r["verdict"] == "N07_SCOPE"}
        self.assertEqual(n07, {"ev-repeated-note", "ev-retrigger",
                               "ev-sum-15", "ev-steal-16", "ev-steal-17"})
        for r in rows:
            if r["verdict"] == "FAIL":
                self.assertIn("hypothesis", r,
                              "every FAIL carries a HYPOTHESIS localization")

    def test_every_exact_row_is_byte_identical(self):
        for sec in ("dev32", "registry"):
            for r in self.dev["sections"][sec]:
                if r["verdict"] == "EXACT":
                    self.assertTrue(r["byte_identical"], r["id"])
                    self.assertEqual(r["max_abs_error"], 0.0, r["id"])

    def test_fail_rows_localize(self):
        for r in self.dev["sections"]["dev32"]:
            if r["verdict"] == "FAIL":
                self.assertEqual(r["hypothesis"]["role"], "HYPOTHESIS")
                self.assertIn("reference-side LFO indeterminacy",
                              r["hypothesis"]["module"])
                self.assertEqual(
                    r["reference_lfo"]["reference_class"],
                    "reference_indeterminate_lfo_phase")
                self.assertEqual(r["first_divergent_sample"] // 64,
                                 r["first_divergent_block"])

    @unittest.skipUnless((EVIDENCE / "negative-control.json").is_file(),
                         "committed negative-control evidence not present")
    def test_negative_control_fired(self):
        nc = json.loads(
            (EVIDENCE / "negative-control.json").read_text(encoding="utf-8"))
        self.assertTrue(nc["control"]["control_fired"])
        self.assertTrue(nc["control"]["all_cases_fail"])
        self.assertTrue(nc["control"]["localization_names_stubbed_seam"])
        self.assertEqual(nc["control"]["stub"], "envelope")
        for r in nc["sections"]["stubbed_cases"]:
            self.assertEqual(r["verdict"], "FAIL", r["id"])
            self.assertIn("envelope", r["hypothesis"]["module"])


# ---------------------------------------------------------------------------
# 5. Live pinned-oracle checks (guarded; NOT_RUN when absent)


def _oracle():
    try:
        from render_reference import (check_oracle_binary, load_identity)
        identity = load_identity(REPO / "reference" / "oracle-identity.json")
        path = Path(identity["binary"]["path"])
        check_oracle_binary(identity, path)
        return path
    except Exception:  # noqa: BLE001 - guarded skip -> NOT_RUN
        return None


ORACLE = _oracle()
ORACLE_ABSENT = ("pinned oracle binary absent or unpinned; live checks "
                 "NOT_RUN")


def registry_voice(cid):
    from registry_smoke import build_voice
    from registry_coverage import load_registry
    reg = load_registry(REPO / "spec" / "compatibility-registry-v1.json")
    case = next(c for c in reg["cases"] if c["id"] == cid)
    return build_voice(reg, case), case


@unittest.skipUnless(ORACLE is not None, ORACLE_ABSENT)
class TestLiveOracle(_Base):
    def _render_both(self, cid, seconds, events=None, stubs=None):
        import subprocess
        import tempfile
        voice, case = registry_voice(cid)
        events = events or "\n".join(case["events"]) + "\n"
        with tempfile.TemporaryDirectory(prefix="n06-live-") as td:
            tmp = Path(td)
            vp, ep, op = tmp / "v.syx", tmp / "e.txt", tmp / "o.f32"
            vp.write_bytes(sysex.encode_voice(voice))
            ep.write_text(events)
            proc = subprocess.run(
                [str(ORACLE), "--voice", str(vp), "--events", str(ep),
                 "--out", str(op), "--rate", "48000",
                 "--seconds", str(seconds)], capture_output=True, text=True)
            self.assertEqual(proc.returncode, 0, proc.stderr)
            oracle_pcm = op.read_bytes()
        model_pcm = render_integrated(voice, events, seconds=seconds,
                                      stubs=stubs)
        return oracle_pcm, model_pcm

    def test_dir_base_exact(self):
        ora, mod = self._render_both("dir-base", 1.0)
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "PASS", rep.primary)

    def test_pitch_bend_up_exact(self):
        ora, mod = self._render_both("ev-pitch-bend-up", 1.0)
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "PASS", rep.primary)

    def test_all_sound_off_exact(self):
        ora, mod = self._render_both("ev-all-sound-off", 1.0)
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "PASS", rep.primary)

    def test_envelope_stub_fails_and_localizes(self):
        ora, mod = self._render_both("dir-base", 1.0,
                                     stubs={"envelope": "constant"})
        rep = compare.compare_streams(ora, mod)
        self.assertEqual(rep.verdict, "FAIL")
        self.assertLess(rep.primary["first_divergent_sample"], 4800)


if __name__ == "__main__":
    unittest.main()
