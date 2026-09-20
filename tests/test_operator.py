"""N02 probes for the frozen integer phase/operator model (issue #16).

Layers, per the issue's acceptance list:

1. Analytic probes — quadrant/symmetry properties of the Mark I log-sine
   fold, monotone amplitude under monotone envelope gain, known-value spot
   checks, and FULL-DOMAIN table checks (1024 entries each) against the
   documented formulas, recomputed INDEPENDENTLY here (not by calling the
   model's generator).
2. Negative controls — each must demonstrably fail the probe it targets:
   a mutant operator with off-by-one exp-table indexing; a mutant with the
   sign bit applied inverted; a perturbed pinned table (SHA-256 manifest
   check); and an end-to-end control where a one-entry table mutation
   breaks the byte-identical reference probe.
3. Reference probe — one synthetic single-operator voice (algorithm 32,
   operators 1-5 at total level 0 => envelope-gated silent carriers, op 6
   audible; constant envelope target => constant 14-bit gain), rendered by
   the pinned oracle (hash-verified) and by this model, compared dry with
   src/gf180_dx7/compare.py. Declared budget: EXACT (max_abs_error == 0.0).
   Justification: the regenerated tables are byte-identical to the pinned
   toolchain's runtime tables (measured; see docs/N02-OPERATOR-MODEL.md)
   and every integer operation is mirrored, so any nonzero error indicates
   a model defect, not a numeric budget.

The probe-only envelope-gain path computes the constant frame gain from the
frozen target formula (env.cc:109-121, dx7note.cc:169-179, profile NUM-005)
— N03 remains the owner of the general envelope; nothing here weakens it.

Oracle-dependent cases guarded-skip to NOT_RUN when the pinned binary is
absent; a skipped test is never a pass. Stdlib only.
"""

from __future__ import annotations

import hashlib
import json
import math
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
_SRC = REPO / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model import operator as op  # noqa: E402
from gf180_dx7.model import tables as tmod  # noqa: E402

TABLES_DIR = REPO / "reference" / "tables"
EVIDENCE_PATH = REPO / "reference" / "evidence" / "n02-operator" / \
    "reference-probe.json"
ORACLE_IDENTITY = REPO / "reference" / "oracle-identity.json"

RATE = 48000
SECONDS = 4
RENDER_FRAMES = RATE * SECONDS // op.N
MIDINOTE = 60
VELOCITY = 100

# --- independent reimplementation of the documented formulas (facts,
# EngineMkI.cpp:81-92 / freqlut.cc:35-42); deliberately NOT calling
# gf180_dx7.model.tables -------------------------------------------------


def _f32(x: float) -> float:
    return struct.unpack("<f", struct.pack("<f", x))[0]


def _cround(v: float) -> int:
    """C round(): half away from zero (domains here are nonnegative)."""
    return math.floor(v + 0.5) if v >= 0 else math.ceil(v - 0.5)


def _expected_sinlog() -> list[int]:
    return [_cround(-1024.0 * math.log2(
        _f32(math.sin(((0.5 + i) / 1024.0) * math.pi / 2.0))))
        for i in range(1024)]


def _expected_sinexp() -> list[int]:
    return [_cround(_f32((2.0 ** (i / 1024.0) - 1.0) * 4096.0))
            for i in range(1024)]


def _expected_freqlut() -> list[int]:
    y = (1 << 44) / 48000.0
    inc = 2.0 ** (1.0 / 1024.0)
    out = []
    for _ in range(1025):
        out.append(math.floor(y + 0.5))
        y *= inc
    return out


# --- probe-only minimal envelope-gain path (constant-target voice) ------

_LEVELLUT = [0, 5, 9, 13, 17, 20, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
             42, 43, 45, 46]  # env.cc:28-30
_VELOCITY_DATA = [
    0, 70, 86, 97, 106, 114, 121, 126, 132, 138, 142, 148, 152, 156, 160,
    163, 166, 170, 173, 174, 178, 181, 184, 186, 189, 190, 194, 196, 198,
    200, 202, 205, 206, 209, 211, 214, 216, 218, 220, 222, 224, 225, 227,
    229, 230, 232, 233, 235, 237, 238, 240, 241, 242, 243, 244, 246, 246,
    248, 249, 250, 251, 252, 253, 254]  # dx7note.cc:47-53


def _scaleoutlevel(outlevel: int) -> int:
    """env.cc:109-111."""
    return 28 + outlevel if outlevel >= 20 else _LEVELLUT[outlevel]


def probe_frame_gain(output_level: int, levels=(99, 99, 99, 99),
                     midinote: int = MIDINOTE,
                     velocity: int = VELOCITY) -> int:
    """Constant 14-bit frame gain for the probe voice.

    Frozen facts: effective outlevel (dx7note.cc:169-177) — the probe patch
    zeroes level scaling (ScaleLevel => 0, dx7note.cc:171-173) and velocity
    sensitivity (ScaleVelocity => 0, dx7note.cc:176); segment target
    (env.cc:113-121): actuallevel = (scaleoutlevel(level)>>1 << 6) +
    outlevel - 4256, clamp <16 -> 16; frame gain (EngineMkI.cpp:315):
    gain2 = ENV_MAX - (level_in >> 14). The probe voice parks the envelope
    at this target from the first frame (rates 99, equal levels; verified
    analytically in docs/N02-OPERATOR-MODEL.md section 4).
    """
    outlevel = _scaleoutlevel(output_level) + 0  # + ScaleLevel(...) == 0
    outlevel = min(127, outlevel)
    outlevel <<= 5
    outlevel += (((0 * (_VELOCITY_DATA[min(127, velocity) >> 1] - 239)) + 7)
                 >> 3) << 4  # ScaleVelocity with sensitivity 0
    outlevel = max(0, outlevel)
    actual = (_scaleoutlevel(levels[0]) >> 1) << 6
    actual = actual + outlevel - 4256
    actual = actual if actual >= 16 else 16
    level_in = actual << 16
    return op.ENV_MAX - (level_in >> 14)


def probe_basepitch() -> int:
    """noteLogFreq for the probe note (tuning.cc:13-21) with coarse=1
    (coarsemul[1] = 0, dx7note.cc:31), fine=0, detune=7 (neutral,
    dx7note.cc:46-48): basepitch == noteLogFreq."""
    base = 50857777  # (1 << 24) * (log(440)/log(2) - 69/12), tuning.cc:14
    step = (1 << 24) // 12
    return base + step * MIDINOTE


def probe_voice() -> sysex.Voice:
    """Single-operator synthetic voice via the P01 codec.

    Algorithm 32 (byte 31): all six operators are output carriers. Ops 1-5
    at total level 0 => effective outlevel 0 => frame gain 16320 > 16284
    (kLevelThresh, EngineMkI.cpp:296,314-318) => envelope-gated, contribute
    exactly zero. Op 6 (TL 99) is the only audible operator, unmodulated.
    Rates 99 + equal levels park every envelope at its target from frame 1;
    pitch EG levels 50 (pitchenv_tab[50] = 0) make the pitch path constant;
    all LFO/mod/scale bytes neutral.
    """

    def op_fields(k: int) -> sysex.Operator:
        return sysex.Operator(
            rate_1=99, rate_2=99, rate_3=99, rate_4=99,
            level_1=99, level_2=99, level_3=99, level_4=99,
            break_point=0, scale_left_depth=0, scale_right_depth=0,
            scale_left_curve=0, scale_right_curve=0,
            rate_scaling=0, amp_mod_sensitivity=0, key_velocity_sensitivity=0,
            output_level=99 if k == 6 else 0,
            osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0, osc_detune=7)

    return sysex.Voice(
        operators=[op_fields(k) for k in range(1, 7)],
        pitch_eg_rate_1=0, pitch_eg_rate_2=0,
        pitch_eg_rate_3=0, pitch_eg_rate_4=0,
        pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=31, feedback=0, osc_key_sync=0,
        lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=0, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0,
        transpose=24,
        name="N02PROBE01", name_bytes=b"N02PROBE01", channel_byte=0)


PROBE_EVENTS = "# N02 reference probe (issue #16): note-on at block 0\n" \
               "0 note_on 60 100\n"


def model_render_f32(logt, expt, lut) -> bytes:
    """Render the probe voice through the N02 operator model.

    Frame gain path: frame 1 gain1 = ENV_MAX-1 (gain_out == 0 substitution,
    EngineMkI.cpp:314); later frames gain1 = gain2 (EngineMkI.cpp:316),
    so dgain = 0. pitch_base = 0 (no bend/masterTune), pitch EG 0, no
    mod inputs => freq constant across frames (dx7note.cc:278-281).
    """
    freq = lut.lookup(probe_basepitch())
    g2 = probe_frame_gain(99)
    operator = op.MarkIOperator(0)
    clip_vals = []
    for frame in range(RENDER_FRAMES):
        gain1 = op.ENV_MAX - 1 if frame == 0 else g2
        block = operator.render_frame(logt, expt, freq, gain1, g2, add=True)
        clip_vals.extend(op.clip_val_domain(block))
    return op.clip_val_to_f32_bytes(clip_vals)


# --- oracle harness (guarded) --------------------------------------------


class _Oracle:
    def __init__(self) -> None:
        self.available = False
        self.skip_reason = ""
        self.path: Path | None = None
        try:
            identity = json.loads(ORACLE_IDENTITY.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            self.skip_reason = f"oracle identity unreadable: {exc}"
            return
        path = Path(identity["binary"]["path"])
        pinned = identity["binary"]["sha256"]
        if not path.is_file():
            self.skip_reason = (f"oracle binary absent at {path} (pinned "
                                f"{pinned[:12]}...)")
            return
        actual = hashlib.sha256(path.read_bytes()).hexdigest()
        if actual != pinned:
            self.skip_reason = (f"oracle identity mismatch: {actual} != "
                                f"pinned {pinned}")
            return
        self.available = True
        self.path = path

    def render(self, voice_syx: bytes, events_text: str, out: Path) -> None:
        proc = subprocess.run(
            [str(self.path), "--voice", str(voice_syx),
             "--events", str(events_text), "--out", str(out),
             "--rate", str(RATE), "--seconds", str(SECONDS)],
            capture_output=True, text=True)
        if proc.returncode != 0:
            raise AssertionError(
                f"oracle render failed (exit {proc.returncode}): "
                f"{proc.stderr.strip()}")


def _mutated_tables(direction: int) -> tuple[list[int], list[int]]:
    """Perturb one sinlog entry by `direction` counts (negative control)."""
    logt = list(tmod.sin_log_table())
    expt = list(tmod.sin_exp_table())
    logt[512] += direction
    return logt, expt


class TestTableDomains(unittest.TestCase):
    """Full-domain checks: every entry against the documented formula."""

    def test_sinlog_full_domain_matches_formula(self):
        generated = tmod.sin_log_table()
        expected = _expected_sinlog()
        self.assertEqual(len(generated), 1024)
        for i, (g, e) in enumerate(zip(generated, expected)):
            self.assertEqual(g, e, f"sinLogTable[{i}]: {g} != formula {e}")

    def test_sinexp_full_domain_matches_formula(self):
        generated = tmod.sin_exp_table()
        expected = _expected_sinexp()
        self.assertEqual(len(generated), 1024)
        for i, (g, e) in enumerate(zip(generated, expected)):
            self.assertEqual(g, e, f"sinExpTable[{i}]: {g} != formula {e}")

    def test_freqlut_full_domain_matches_formula(self):
        generated = tmod.freqlut_table(48000.0)
        expected = _expected_freqlut()
        self.assertEqual(len(generated), 1025)
        self.assertEqual(list(generated), expected)

    def test_pinned_tables_match_generated_and_manifest(self):
        """The committed .hex files hash to the manifest and equal the
        generator's bytes (regenerate-and-hash discipline)."""
        pinned = tmod.load_pinned(TABLES_DIR)  # raises TableDrift on drift
        self.assertEqual(pinned["sinlog_table"].values,
                         tmod.sin_log_table())
        self.assertEqual(pinned["sinexp_table"].values,
                         tmod.sin_exp_table())
        self.assertEqual(pinned["freqlut_table"].values,
                         tmod.freqlut_table(48000.0))
        # the two Mark I tables equal the profile's provisional records
        self.assertEqual(pinned["sinlog_table"].sha256,
                         "990c19e90732efe712a19ba4272f97c7c9d884ac0e4e19450a"
                         "1067165d8aa8a8")
        self.assertEqual(pinned["sinexp_table"].sha256,
                         "e515a71ae736d92dcb3fd36973dea486c96d1521f1a0bab1f9"
                         "17be2c4ec07794")


class TestSineFoldProperties(unittest.TestCase):
    """Quadrant/symmetry properties of the log-sine lookup."""

    def setUp(self):
        self.logt = tmod.sin_log_table()

    def test_first_quadrant_nonincreasing(self):
        """Sine rises over the first quadrant, so -log2(sine) falls; the
        rounded tail flattens (multiple zero entries) but never rises."""
        q0 = self.logt[:1024]
        self.assertTrue(all(a >= b for a, b in zip(q0, q0[1:])))
        self.assertEqual(q0[0], max(q0))
        self.assertEqual(q0[-1], 0)  # sine peak => log2(1) = 0
        self.assertGreater(len(set(q0)), 800)  # resolution sanity

    def test_quadrant_mirror_symmetry(self):
        """Quadrant 1 mirrors quadrant 0 (index ^ 1023); quadrants 2/3
        repeat 0/1 with the sign bit set (EngineMkI.cpp:64-78)."""
        for index in range(1024):
            q0 = op.sin_log_lookup(index, self.logt)
            q1 = op.sin_log_lookup(1024 + index, self.logt)
            q2 = op.sin_log_lookup(2048 + index, self.logt)
            q3 = op.sin_log_lookup(3072 + index, self.logt)
            self.assertEqual(q1, self.logt[index ^ 1023])
            self.assertEqual(q2, q0 | 0x8000)
            self.assertEqual(q3, q1 | 0x8000)

    def test_full_period_periodicity(self):
        """The fold depends only on phase bits 12..27: adding whole sine
        cycles (k * 2^24 phase units) changes nothing."""
        base = 12345 << 12
        for k in (1, 1023, 4095):
            shifted = ((base + (k << 24)) >> 12) & 0xFFFF
            self.assertEqual(op.sin_log_lookup(shifted, self.logt),
                             op.sin_log_lookup((base >> 12) & 0xFFFF,
                                               self.logt))

    def test_monotone_level_path(self):
        """Closed-form probe: larger 14-bit env gain => smaller (or equal)
        amplitude, per quadrant of the positive half (log-domain add)."""
        expt = tmod.sin_exp_table()
        for phi in (0, 1, 341, 682, 1023):
            phase = phi << 12
            prev = None
            for env in range(0, 16384, 7):
                y = op.mki_sin(phase, env, self.logt, expt)
                self.assertGreaterEqual(y, 0)
                if prev is not None:
                    self.assertLessEqual(y, prev)
                prev = y

    def test_sign_reconstruction_asymmetry(self):
        """Positive/negative halves carry the same magnitude with the
        pinned (-result - 1) << 13 asymmetry (EngineMkI.cpp:146-149)."""
        expt = tmod.sin_exp_table()
        for index in range(0, 1024, 61):
            phase_pos = index << 12
            phase_neg = (2048 + index) << 12
            for env in (0, 1024, 4096):
                pos = op.mki_sin(phase_pos, env, self.logt, expt)
                neg = op.mki_sin(phase_neg, env, self.logt, expt)
                self.assertGreaterEqual(pos, 0)
                self.assertLess(neg, 0)
                self.assertEqual(neg, -pos - (1 << 13),
                                 f"index {index} env {env}")

    def test_known_value_from_documented_formula(self):
        """phase=0, env=0: expVal = sinLogTable[0] = 10597;
        result = 4096 + sinExpTable[(10597 & 1023) ^ 1023]; >> (10597>>10);
        out = result << 13 — expected computed from the formula here."""
        expt = _expected_sinexp()
        log0 = _expected_sinlog()[0]
        self.assertEqual(log0, 10597)
        idx = (log0 & 1023) ^ 1023
        result = 4096 + expt[idx]
        result >>= log0 >> 10
        self.assertEqual(op.mki_sin(0, 0, self.logt, expt), result << 13)

    def test_phase_wraparound_equivalence(self):
        """uint32 wrapping: phase 2^32 - d must equal phase -d (NUM-003)."""
        expt = tmod.sin_exp_table()
        for d in (1, 4096, 1 << 20, (1 << 31) + 12345):
            wrapped = (0x100000000 - d) & 0xFFFFFFFF
            self.assertEqual(
                op.mki_sin(wrapped, 2048, self.logt, expt),
                op.mki_sin(-d, 2048, self.logt, expt))


class TestFrameSemantics(unittest.TestCase):
    """64-sample frame forms, gain ramp, phase carry (NUM-002/003/004)."""

    def setUp(self):
        self.logt = tmod.sin_log_table()
        self.expt = tmod.sin_exp_table()

    def test_dgain_rounding_is_floor_for_negatives(self):
        """(gain2 - gain1 + 32) >> 6 with C arithmetic shift semantics
        (floor), EngineMkI.cpp:155 — including negative deltas."""
        self.assertEqual((-15327) >> 6, -240)  # Python >> floors like C++
        gain2, gain1 = 1024, 16383
        self.assertEqual((gain2 - gain1 + (op.N >> 1)) >> op.LG_N, -240)

    def test_frame_gain_ramp_starts_at_gain1_plus_dgain(self):
        """gain advances BEFORE each sample (EngineMkI.cpp:160-162): the
        first sample uses gain1 + dgain, the 64th gain1 + 64*dgain."""
        gain1, gain2, freq = 16000, 1024, 91444
        dgain = (gain2 - gain1 + (op.N >> 1)) >> op.LG_N
        operator = op.MarkIOperator(0)
        block = operator.render_frame(self.logt, self.expt, freq,
                                      gain1, gain2)
        expected_first = op.mki_sin(0, gain1 + dgain, self.logt, self.expt)
        self.assertEqual(block[0], expected_first)
        expected_last = op.mki_sin(
            (63 * freq) & 0xFFFFFFFF, gain1 + 64 * dgain,
            self.logt, self.expt)
        self.assertEqual(block[63], expected_last)

    def test_frame_end_phase_equals_freq_times_n(self):
        """Carried register after one frame == phase0 + freq << LG_N
        (EngineMkI.cpp:361); loop-final phase and the skip line agree."""
        for phase0, freq in ((0, 91444), (0xDEADBEEF, -31234),
                             ((1 << 31), 1)):
            operator = op.MarkIOperator(phase0)
            operator.render_frame(self.logt, self.expt, freq, 1024, 1024)
            carried = operator.phase
            skipped = (phase0 + ((freq & 0xFFFFFFFF) << op.LG_N)) \
                & 0xFFFFFFFF
            self.assertEqual(carried, skipped)
            gated = op.MarkIOperator(phase0)  # gated-op path: skip only
            gated.advance_frame(freq)
            self.assertEqual(gated.phase, skipped)

    def test_modulation_input_form(self):
        """The compute form adds input per sample, with the phase still
        advancing by freq each sample (EngineMkI.cpp:162-164)."""
        freq, gain = 4321, 8192
        mod = [(k * 7919) & 0xFFFFFFFF for k in range(op.N)]
        operator = op.MarkIOperator(11111)
        got = operator.render_frame(self.logt, self.expt, freq, gain, gain,
                                    modulation=mod)
        phase = 11111
        for k in range(op.N):
            self.assertEqual(
                got[k],
                op.mki_sin(phase + mod[k], gain, self.logt, self.expt))
            phase = (phase + freq) & 0xFFFFFFFF

    def test_feedback_history_path(self):
        """compute_fb: scaled_fb = (y0+y) >> (shift+1) read per sample;
        history carried back (EngineMkI.cpp:184-205). Interface-level
        agreement here; routing/depth policy is N04."""
        freq, gain, shift = 99999, 4096, 8
        fb = op.FeedbackHistory(y0=-5000, y1=7000)
        operator = op.MarkIOperator(255)
        block = operator.render_frame(self.logt, self.expt, freq, gain,
                                      gain, fb_history=fb, fb_shift=shift)
        y0, y1 = -5000, 7000
        phase = 255
        for k in range(op.N):
            scaled = (y0 + y1) >> (shift + 1)
            y0 = y1
            y = op.mki_sin(phase + scaled, gain, self.logt, self.expt)
            y1 = y
            self.assertEqual(block[k], y)
            phase = (phase + freq) & 0xFFFFFFFF
        self.assertEqual((fb.y0, fb.y1), (y0, y1))

    def test_gated_silent_ops_contribute_zero(self):
        """Mark I gate (NUM-009): TL=0 probe ops park at gain 16320 >
        kLevelThresh 16284 => gated; their only effect is the phase skip
        (EngineMkI.cpp:318,361)."""
        g_silent = probe_frame_gain(0)
        self.assertGreater(g_silent, 16284)
        g_loud = probe_frame_gain(99)
        self.assertLessEqual(g_loud, 16284)

    def test_freqlut_known_values(self):
        """lookup(0) = lut[0] >> 20; lut generated by the documented
        doubling recurrence (freqlut.cc:35-55). The pinned increment is in
        2^24-per-sine-cycle units (mkiSin consumes phase bits 12..27,
        EngineMkI.cpp:125): midinote 60 -> 91444 -> 261.63 Hz at 48 kHz
        (measured against the oracle: period 183 samples)."""
        lut = op.Freqlut(_expected_freqlut())
        self.assertEqual(lut.lookup(0), 366503876 >> 20)
        base = probe_basepitch()
        # monotone in logfreq around the probe pitch
        self.assertLess(lut.lookup(base - 4096), lut.lookup(base))
        self.assertLess(lut.lookup(base), lut.lookup(base + 4096))
        freq = lut.lookup(base)
        self.assertEqual(freq, 91444)
        approx = 261.6255653 / RATE * (1 << 24)
        self.assertLess(abs(freq - approx) / approx, 0.01)


class TestNegativeControls(unittest.TestCase):
    """Each control must demonstrably fail the check it targets."""

    def setUp(self):
        self.logt = tmod.sin_log_table()
        self.expt = tmod.sin_exp_table()

    def _probe_suite(self, sin_fn) -> bool:
        """Core analytic probes as a predicate: True iff the given
        mki-sin-like function passes known values + sign + monotonicity."""
        try:
            # known value at phase 0 / env 0
            log0 = self.logt[0]
            idx = (log0 & 1023) ^ 1023
            result = 4096 + self.expt[idx]
            result >>= log0 >> 10
            if sin_fn(0, 0) != result << 13:
                return False
            # sign reconstruction asymmetry
            for index in range(0, 1024, 37):
                pos = sin_fn(index << 12, 1024)
                neg = sin_fn((2048 + index) << 12, 1024)
                if neg != -pos - (1 << 13):
                    return False
            # monotone level path (first quadrant)
            prev = None
            for env in range(0, 16384, 13):
                y = sin_fn(0, env)
                if y < 0:
                    return False
                if prev is not None and y > prev:
                    return False
                prev = y
            return True
        except Exception:
            return False

    def test_control_exp_index_off_by_one_fails(self):
        """Mutant: exp-table index off by one. The probe suite must fail."""
        def mutant(phase, env):
            phi = (phase >> 12) & 0xFFFF
            exp_val = (op.sin_log_lookup(phi, self.logt) + env) & 0xFFFF
            signed = bool(exp_val & 0x8000)
            exp_val &= 0x7FFF
            bad_index = (((exp_val & 0x3FF) + 1) & 0x3FF) ^ 0x3FF
            result = 4096 + self.expt[bad_index]
            result >>= exp_val >> 10
            return (-result - 1) << 13 if signed else result << 13

        self.assertTrue(self._probe_suite(
            lambda p, e: op.mki_sin(p, e, self.logt, self.expt)))
        self.assertFalse(self._probe_suite(mutant),
                         "off-by-one exp indexing NOT caught by probes")

    def test_control_sign_bit_inverted_fails(self):
        """Mutant: sign application inverted. The probe suite must fail."""

        def mutant(phase, env):
            phi = (phase >> 12) & 0xFFFF
            exp_val = (op.sin_log_lookup(phi, self.logt) + env) & 0xFFFF
            signed = not (exp_val & 0x8000)  # inverted
            exp_val &= 0x7FFF
            result = 4096 + self.expt[(exp_val & 0x3FF) ^ 0x3FF]
            result >>= exp_val >> 10
            return (-result - 1) << 13 if signed else result << 13

        self.assertTrue(self._probe_suite(
            lambda p, e: op.mki_sin(p, e, self.logt, self.expt)))
        self.assertFalse(self._probe_suite(mutant),
                         "inverted sign handling NOT caught by probes")

    def test_control_perturbed_table_fails_manifest_check(self):
        """One flipped hex digit in a pinned table => load_pinned raises
        TableDrift (never a silent pass)."""
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            for name in ("sinlog_table", "sinexp_table",
                         "freqlut_table"):
                shutil.copy(TABLES_DIR / f"{name}.hex", td / f"{name}.hex")
            shutil.copy(TABLES_DIR / tmod.MANIFEST_NAME,
                        td / tmod.MANIFEST_NAME)
            hexfile = td / "sinlog_table.hex"
            lines = hexfile.read_text(encoding="ascii").splitlines()
            value = int(lines[100], 16) ^ 0x1  # flip one bit
            lines[100] = "%04x" % value
            hexfile.write_text("\n".join(lines) + "\n", encoding="ascii")
            with self.assertRaises(tmod.TableDrift):
                tmod.load_pinned(td)

    def test_control_perturbed_table_breaks_reference_probe(self):
        """End-to-end control: a one-entry sinlog mutation must change the
        model render, so the byte-identical reference comparison fails —
        proving the reference probe can detect table drift."""
        oracle = _Oracle()
        if not oracle.available:
            self.skipTest(f"NOT_RUN: {oracle.skip_reason}")
        logt_bad, expt_bad = _mutated_tables(+1)
        lut = op.Freqlut(tmod.freqlut_table())
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            voice = td / "voice.syx"
            voice.write_bytes(sysex.encode_voice(probe_voice()))
            events = td / "events.txt"
            events.write_text(PROBE_EVENTS, encoding="utf-8")
            out = td / "render.f32"
            oracle.render(voice, events, out)
            oracle_bytes = out.read_bytes()
        model_clean = model_render_f32(tmod.sin_log_table(),
                                       tmod.sin_exp_table(), lut)
        model_bad = model_render_f32(logt_bad, expt_bad, lut)
        self.assertEqual(
            hashlib.sha256(model_clean).hexdigest(),
            hashlib.sha256(oracle_bytes).hexdigest(),
            "clean model render unexpectedly differs from oracle")
        self.assertNotEqual(
            hashlib.sha256(model_bad).hexdigest(),
            hashlib.sha256(oracle_bytes).hexdigest(),
            "perturbed-table render NOT caught: negative control ineffective")


class TestReferenceProbe(unittest.TestCase):
    """Pinned-oracle vs model, single-operator voice, dry f32le, exact."""

    def setUp(self):
        self.oracle = _Oracle()
        if not self.oracle.available:
            self.skipTest(f"NOT_RUN: {self.oracle.skip_reason}")

    def _oracle_render(self, td: Path) -> bytes:
        voice = td / "voice.syx"
        voice.write_bytes(sysex.encode_voice(probe_voice()))
        events = td / "events.txt"
        events.write_text(PROBE_EVENTS, encoding="utf-8")
        out = td / "render.f32"
        self.oracle.render(voice, events, out)
        data = out.read_bytes()
        self.assertEqual(len(data), 4 * RATE * SECONDS)
        samples = struct.unpack("<%df" % (len(data) // 4), data)
        peak = max((abs(s) for s in samples), default=0.0)
        self.assertGreater(peak, 1e-6,
                           "silent substitution rejected by probe guard")
        return data

    def test_reference_probe_exact(self):
        """PASS criterion (declared in the test, not inferred): EXACT —
        byte-identical PCM, max_abs_error == 0.0. Justification: complete
        integer re-derivation over byte-identical tables (see module
        docstring and docs/N02-OPERATOR-MODEL.md section 6); a nonzero
        error is a model defect, never an accepted budget."""
        lut = op.Freqlut(tmod.freqlut_table())
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            oracle_bytes = self._oracle_render(td)
        model_bytes = model_render_f32(tmod.sin_log_table(),
                                       tmod.sin_exp_table(), lut)
        report = compare.compare_streams(oracle_bytes, model_bytes)
        self.assertEqual(report.verdict, "PASS",
                         f"compare verdict {report.verdict} "
                         f"({report.verdict_reason})")
        self.assertTrue(report.byte_identical)
        self.assertEqual(report.primary["max_abs_error"], 0.0)
        self.assertEqual(report.primary["mismatch_count"], 0)

    def test_evidence_record_matches_live_run(self):
        """The committed evidence artifact stays live: its hashes/budget
        must equal a fresh run's (else STALE -> fail loudly)."""
        self.assertTrue(EVIDENCE_PATH.is_file(),
                        f"missing evidence artifact {EVIDENCE_PATH}")
        evidence = json.loads(EVIDENCE_PATH.read_text(encoding="utf-8"))
        lut = op.Freqlut(tmod.freqlut_table())
        model_bytes = model_render_f32(tmod.sin_log_table(),
                                       tmod.sin_exp_table(), lut)
        voice_bytes = sysex.encode_voice(probe_voice())
        with tempfile.TemporaryDirectory() as td:
            td = Path(td)
            oracle_bytes = self._oracle_render(td)
        sinlog_sha = hashlib.sha256(
            struct.pack("<1024H", *tmod.sin_log_table())).hexdigest()
        self.assertEqual(evidence["tables"]["sinlog_table"], sinlog_sha)
        self.assertEqual(evidence["inputs"]["voice_syx_sha256"],
                         hashlib.sha256(voice_bytes).hexdigest())
        self.assertEqual(evidence["outputs"]["oracle_render_sha256"],
                         hashlib.sha256(oracle_bytes).hexdigest())
        self.assertEqual(evidence["outputs"]["model_render_sha256"],
                         hashlib.sha256(model_bytes).hexdigest())
        self.assertEqual(evidence["outputs"]["max_abs_error"], 0.0)
        self.assertEqual(evidence["outputs"]["byte_identical"], True)
        self.assertEqual(evidence["declared_budget"], "exact (0.0)")
        self.assertEqual(evidence["status"], "PASS")


if __name__ == "__main__":
    unittest.main()
