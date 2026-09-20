"""N05 tests — integer pitch and modulation model (issue #19).

Layers:
  1. analytic: frequency-ratio arithmetic re-derived in this test from the
     pinned freqlut.cc formulas (not from the model under test);
  2. model-vs-pinned-tree: typed constants verified against the pinned
     Dexed sources when DEXED_ROOT is present (guarded NOT_RUN skip
     otherwise, never a silent pass);
  3. model-vs-oracle PCM: rendered probes measured with a documented
     zero-crossing interval estimator against the pinned external oracle
     when its binary is present and hash-pinned (guarded NOT_RUN skip
     otherwise);
  4. PINNED-CONFIG divergence: documented transpose semantics vs the
     pinned-wrapper semantics behind an explicit switch, divergence
     asserted NONZERO (registry finding perf-transpose-12), and CC1
     inertness at pinned defaults (registry finding ev-modwheel-127);
  5. live negative controls: fixed-mode-tracking-keyboard mutation, a
     detune-extreme wrap mutation, and an LFO-sync-ignored mutation must
     each fail the specific check they target, with the case named.

Tolerances are declared in docs/N05-PITCH-MODEL.md and mirrored in the
constants below. Coverage cross-reference to R06 lives in
docs/N05-PITCH-MODEL.md.

Licensing: this test transcribes numeric facts (formulas, constants) from
the pinned Apache-2.0 msfa files by citation; no GPL bytes and no msfa
bytes are copied.
"""

from __future__ import annotations

import array
import hashlib
import json
import math
import os
import re
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

_REPO = Path(__file__).resolve().parents[1]
for _p in ("src", "tools"):
    _q = _REPO / _p
    if str(_q) not in sys.path:
        sys.path.insert(0, str(_q))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.model import pitch as P  # noqa: E402

DEXED_ROOT = Path(os.environ.get("DEXED_ROOT", "/Users/joseph/dev/dexed"))
ORACLE_ROOT = Path(os.environ.get("DX7_ORACLE_ROOT",
                                  "/Users/joseph/dx7-oracle-marki"))
ORACLE_IDENTITY = json.loads(
    (_REPO / "reference" / "oracle-identity.json").read_text())
REGISTRY_EVIDENCE = _REPO / "reference" / "evidence" / "registry-smoke.json"
TMP_ROOT = "/var/folders/fb/l4j31ymn3bn0mc6v1qbvvl8c0000gn/T/opencode"

SAMPLE_RATE = 48000
F0_C4 = 261.6255653  # midinote 60, standard tuning A4=440

# Declared tolerances (docs/N05-PITCH-MODEL.md section 4).
LUT_REL_TOL = 2e-4        # ratio math through Freqlut at audio rates
TRAJ_REL_TOL = 0.02       # rendered trajectory vs model, per window
ENDPOINT_REL_TOL = 0.02   # rendered held-segment frequency vs model
EXTENT_TOL_OCT = 0.15     # vibrato extent, octaves, either side
CENTER_REL_TOL = 0.01     # vibrato center frequency vs model
VIB_CONTROL_OCT = 0.005   # depth-0 control: measured extent bound

P.init(SAMPLE_RATE)


# ---------------------------------------------------------------------------
# Patch helpers (155-byte unpacked voice body, Dexed data[0..154])
# ---------------------------------------------------------------------------

_OP_FIELD_OFFSET = {
    "rate_1": 0, "rate_2": 1, "rate_3": 2, "rate_4": 3,
    "level_1": 4, "level_2": 5, "level_3": 6, "level_4": 7,
    "break_point": 8, "scale_left_depth": 9, "scale_right_depth": 10,
    "scale_left_curve": 11, "scale_right_curve": 12, "rate_scaling": 13,
    "amp_mod_sensitivity": 14, "key_velocity_sensitivity": 15,
    "output_level": 16,
    "osc_mode": 17, "osc_freq_coarse": 18, "osc_freq_fine": 19,
    "osc_detune": 20,
}
_COMMON_FIELD_OFFSET = {
    "algorithm": 134, "feedback": 135, "osc_key_sync": 136,
    "lfo_speed": 137, "lfo_delay": 138, "lfo_pitch_mod_depth": 139,
    "lfo_amp_mod_depth": 140, "lfo_key_sync": 141, "lfo_wave": 142,
    "lfo_pitch_mod_sensitivity": 143, "transpose": 144,
}


def make_patch(**over) -> bytearray:
    """Neutral probe patch: six ratio ops (coarse 1, fine 0, detune 7,
    fast env holding loud at level_3), neutral pitch EG, inert LFO,
    transpose 24. Field layout is the P01 unpacked layout (sysex.py
    _UNPACKED_*_FIELDS). Overrides use ``opN.<field>`` (N = 1..6, patch
    chunk = N-1) or common field names; pitch EG via ``peg_rates`` /
    ``peg_levels``; per-op env defaults via ``op_rates`` / ``op_levels``.

    Env note (env.cc:74-97): while the key is down only segments 0-2
    run, so the held level is levels_[2]; levels_[3] is the release
    target reached via keyup."""
    peg_rates = over.pop("peg_rates", [99, 99, 99, 99])
    peg_levels = over.pop("peg_levels", [50, 50, 50, 50])
    op_rates = over.pop("op_rates", [99, 99, 99, 99])
    op_levels = over.pop("op_levels", [99, 99, 99, 0])
    p = bytearray(155)
    for off in range(0, 126, 21):
        for i in range(4):
            p[off + i] = op_rates[i]
            p[off + 4 + i] = op_levels[i]
        p[off + 17] = 0   # osc_mode ratio
        p[off + 18] = 1   # coarse 1 = no shift (coarsemul[1] = 0)
        p[off + 19] = 0   # fine
        p[off + 20] = 7   # detune center
    for i in range(4):
        p[126 + i] = peg_rates[i]
        p[130 + i] = peg_levels[i]
    p[134] = 31           # algorithm 32 (all ops add to output)
    p[135] = 0            # feedback
    p[136] = 0            # osc key sync
    p[137] = 35           # lfo speed
    p[138] = 0            # lfo delay
    p[139] = 0            # lfo pitch mod depth
    p[140] = 0            # lfo amp mod depth
    p[141] = 1            # lfo key sync
    p[142] = 0            # lfo wave triangle
    p[143] = 0            # lfo pitch mod sensitivity
    p[144] = 24           # transpose
    for key, val in over.items():
        target, sep, field = key.partition(".")
        if sep and target.startswith("op") and target[2:].isdigit():
            p[21 * (int(target[2:]) - 1) + _OP_FIELD_OFFSET[field]] = val
        elif not sep:
            p[_COMMON_FIELD_OFFSET[key]] = val
        else:
            raise KeyError(key)
    return p


def _wrap(body: bytes) -> bytes:
    head = bytes((0xF0, 0x43, 0x00, 0x00, 0x01, 0x1B))
    return head + body + bytes((sysex.checksum(body), 0xF7))


def patch_to_syx(p: bytearray) -> bytes:
    """155-byte body -> single-voice .syx via the P01 codec."""
    voice = sysex.decode_voice(_wrap(bytes(p)))
    if voice.anomalies:
        raise AssertionError(f"probe patch anomalies: {voice.anomalies}")
    return sysex.encode_voice(voice)


# ---------------------------------------------------------------------------
# Oracle harness (guarded NOT_RUN: absent/unpinned binary -> skipTest)
# ---------------------------------------------------------------------------

_ORACLE_BIN = ORACLE_ROOT / "dx7-oracle"
_ORACLE_OK = None


def oracle_ok() -> bool:
    global _ORACLE_OK
    if _ORACLE_OK is None:
        try:
            digest = hashlib.sha256(_ORACLE_BIN.read_bytes()).hexdigest()
            _ORACLE_OK = digest == ORACLE_IDENTITY["binary"]["sha256"]
        except OSError:
            _ORACLE_OK = False
    return _ORACLE_OK


def render_oracle(tc: unittest.TestCase, patch: bytearray, events: str,
                  seconds: float) -> array.array:
    """One dry f32le render from the pinned oracle (no fidelity claim)."""
    binary = require_oracle(tc)
    d = tempfile.mkdtemp(prefix="n05-test", dir=TMP_ROOT)
    vpath = Path(d, "v.syx")
    vpath.write_bytes(patch_to_syx(patch))
    epath = Path(d, "e.txt")
    epath.write_text(events)
    opath = Path(d, "o.f32")
    proc = subprocess.run(
        [str(binary), "--voice", str(vpath), "--events", str(epath),
         "--out", str(opath), "--rate", str(SAMPLE_RATE),
         "--seconds", str(seconds)],
        capture_output=True, text=True)
    if proc.returncode != 0:
        raise AssertionError(f"oracle render failed: {proc.stderr.strip()}")
    pcm = array.array("f", opath.read_bytes())
    if len(pcm) != int(SAMPLE_RATE * seconds):
        raise AssertionError("oracle produced wrong sample count")
    return pcm


def require_oracle(tc: unittest.TestCase) -> Path:
    if not oracle_ok():
        tc.skipTest(
            "NOT_RUN: pinned oracle binary unavailable or hash-drifted at "
            f"{_ORACLE_BIN}")
    return _ORACLE_BIN


# ---------------------------------------------------------------------------
# Zero-crossing estimators (documented in docs/N05-PITCH-MODEL.md section 3)
# ---------------------------------------------------------------------------

def rising_crossings(pcm: array.array, t0: float, t1: float) -> list[float]:
    a, b = int(SAMPLE_RATE * t0), int(SAMPLE_RATE * t1)
    out = []
    for i in range(a + 1, b):
        if pcm[i - 1] < 0.0 <= pcm[i]:
            denom = pcm[i] - pcm[i - 1]
            frac = -pcm[i - 1] / denom if denom else 0.0
            out.append(i - 1 + frac)
    return out


def interval_freqs(pcm: array.array, t0: float, t1: float):
    """Per-period frequency estimates: SAMPLE_RATE / (interval between
    consecutive rising zero crossings), sub-sample interpolated,
    timestamped at the interval center. Returns [(t_seconds, f_hz,
    dt_seconds)] with the interval duration for time weighting."""
    cross = rising_crossings(pcm, t0, t1)
    return [((x + y) / 2 / SAMPLE_RATE, SAMPLE_RATE / (y - x),
             (y - x) / SAMPLE_RATE)
            for x, y in zip(cross, cross[1:])]


def lsq_freq(pcm: array.array, t0: float, t1: float) -> float | None:
    """Windowed least-squares crossing-phase frequency (coarse windows)."""
    cross = rising_crossings(pcm, t0, t1)
    n = len(cross)
    if n < 4:
        return None
    ks = list(range(n))
    tc = sum(cross) / n
    kc = sum(ks) / n
    slope = (sum((t - tc) * (k - kc) for t, k in zip(cross, ks))
             / sum((t - tc) ** 2 for t in cross))
    return slope * SAMPLE_RATE


def peak_abs(pcm: array.array, t0: float, t1: float) -> float:
    a, b = int(SAMPLE_RATE * t0), int(SAMPLE_RATE * t1)
    return max((abs(s) for s in pcm[a:b]), default=0.0)


# ---------------------------------------------------------------------------
# Model-side reference trajectories
# ---------------------------------------------------------------------------

ONSET_FRAME = 37  # note_on at sample 2400 -> applied before block 37


def model_frame_devs(note: "P.NotePitch", lfo: "P.Lfo",
                     n_frames: int) -> list[float]:
    """Model per-frame log2(f/f0) of the loud probe operator, LFO
    keydown-synced at ONSET_FRAME (wrapper order: lfo.keydown() inside
    note-on, before the block renders; oracle main.cpp:187-189)."""
    for _ in range(ONSET_FRAME):
        lfo.getsample()
        lfo.getdelay()
    lfo.keydown()
    devs = []
    for _ in range(n_frames):
        lv = lfo.getsample()
        ld = lfo.getdelay()
        inc = note.frame_frequencies(lv, ld)[2]
        devs.append(math.log2(inc * SAMPLE_RATE / (1 << 24) / F0_C4))
    return devs


def frame_index(t: float) -> int:
    return int(t * SAMPLE_RATE) // 64


# ---------------------------------------------------------------------------
# Checks shared by the positive tests and the negative controls
# ---------------------------------------------------------------------------

def check_fixed_mode_ignores_keyboard(osc_fn):
    """Named acceptance case: fixed mode must not track the keyboard."""
    for coarse, fine, detune in ((1, 0, 7), (3, 0, 7), (1, 50, 14)):
        lo = osc_fn(48, 1, coarse, fine, detune)
        hi = osc_fn(72, 1, coarse, fine, detune)
        if lo != hi:
            raise AssertionError(
                "fixed-mode tracks keyboard "
                f"(case: coarse={coarse} fine={fine} detune={detune})")


def check_detune_extreme_identity(osc_fn):
    """l(14) + l(0) == 2*l(7) within 2 LSB: the reference computes
    logfreq += ratio*logfreq*(detune-7) in float64, assigned back to
    int32 (truncation toward zero), so the detune term is linear up to
    truncation (dx7note.cc:46-47). A wrong detune wrap breaks it."""
    for midi in (36, 60, 84):
        l0 = osc_fn(midi, 0, 1, 0, 0)
        l7 = osc_fn(midi, 0, 1, 0, 7)
        l14 = osc_fn(midi, 0, 1, 0, 14)
        if abs(l14 + l0 - 2 * l7) > 2:
            raise AssertionError(
                f"detune-extreme wrap identity broken (midinote {midi})")


def check_lfo_keysync_restarts_phase(lfo_cls):
    """With sync set, keydown() must restart the phase at (1U<<31)-1
    (lfo.cc:92-96), so the next saw-up sample is hand-derivable."""
    lfo = lfo_cls()
    lfo.reset([99, 0, 0, 0, 1, 2])   # saw up, sync on
    for _ in range(10):
        lfo.getsample()
    lfo.keydown()
    first = lfo.getsample()
    expected = (P.u32((1 << 31) - 1 + lfo.delta_) ^ (1 << 31)) >> 8
    if first != expected:
        raise AssertionError(
            "LFO sync ignored: keydown did not restart the phase "
            "(case: lfo keysync trajectory)")


def assert_check_fails(tc: unittest.TestCase, mutant_name: str, fn):
    """Run fn() expecting AssertionError; the message must name the case."""
    try:
        fn()
    except AssertionError as exc:
        tc.assertIn(mutant_name, str(exc),
                    f"mutation {mutant_name!r} failed without naming the "
                    "case")
    else:
        tc.fail(f"negative control did not fire: {mutant_name}")


# ---------------------------------------------------------------------------
# Mutants (negative controls; never part of the model)
# ---------------------------------------------------------------------------

def _osc_freq_fixed_tracks_keyboard(midinote, mode, coarse, fine, detune,
                                    note_logfreq=None):
    if note_logfreq is None:
        note_logfreq = P.midinote_to_logfreq(midinote)
    if mode == 0:
        return P.osc_freq(midinote, 0, coarse, fine, detune, note_logfreq)
    # MUTATION: fixed mode wrongly adds the keyboard logfreq.
    return P.i32(P.osc_freq(midinote, 1, coarse, fine, detune, 0)
                 + note_logfreq)


def _osc_freq_detune_wrapbug(midinote, mode, coarse, fine, detune,
                             note_logfreq=None):
    # MUTATION: detune misread as a signed 4-bit field (0..14 wraps to
    # 0..7, -8..-1) instead of the unsigned 0..14 field centered at 7.
    signed = detune - 16 if detune >= 8 else detune
    return P.osc_freq(midinote, mode, coarse, fine, signed, note_logfreq)


class _LfoSyncIgnored(P.Lfo):
    """MUTATION: keydown() ignores the sync flag (never resets phase_)."""

    def keydown(self) -> None:
        self.delaystate_ = 0


# ---------------------------------------------------------------------------
# 1. Freqlut analytic (freqlut.cc re-derived here, independently)
# ---------------------------------------------------------------------------

def _rederive_freqlut_lut(sample_rate: float) -> list[int]:
    """Independent transcription of freqlut.cc:35-42 for the tests."""
    y = (1 << (24 + 20)) / sample_rate
    inc = 2.0 ** (1.0 / 1024)
    lut = []
    for _ in range(1025):
        lut.append(int(math.floor(y + 0.5)))
        y *= inc
    return lut


def _rederive_lookup(logfreq: int, sample_rate: float) -> int:
    """Independent transcription of freqlut.cc:46-55."""
    lut = _rederive_freqlut_lut(sample_rate)
    ix = (logfreq & 0xFFFFFF) >> 14
    y0, y1 = lut[ix], lut[ix + 1]
    lowbits = logfreq & ((1 << 14) - 1)
    y = y0 + (((y1 - y0) * lowbits) >> 14)
    hibits = logfreq >> 24
    return y >> (20 - hibits)


def _logfreq_to_hz_rederived(logfreq: int) -> float:
    """Increment->Hz reading re-derived in this test: the Mark I sine
    period is 2^24 phase LSBs (mkiSin quarter-wave fold indexes
    (phase >> 12) & 1023 with quadrant bits 22-23, EngineMkI.cpp:64-78
    and :125; msfa sin.h lookup SHIFT=14 over a 1024-entry full-period
    table), empirically anchored against the pinned smoke render (784.87
    Hz crossing rate / 3 = 261.62 Hz = C4 at midinote 60)."""
    inc = _rederive_lookup(logfreq, SAMPLE_RATE)
    return inc * SAMPLE_RATE / (1 << 24)


class TestFreqlutAnalytic(unittest.TestCase):
    A4 = P.midinote_to_logfreq(69)

    def test_lut_entry0_matches_formula(self):
        self.assertEqual(P.Freqlut.lut()[0],
                         int(math.floor((1 << 44) / SAMPLE_RATE + 0.5)))

    def test_lookup_matches_independently_rederived_formula(self):
        for midi in (36, 48, 60, 69, 81, 93):
            lf = P.midinote_to_logfreq(midi)
            self.assertEqual(P.Freqlut.lookup(lf),
                             _rederive_lookup(lf, SAMPLE_RATE))

    def test_logfreq_to_hz_matches_rederived_mapping(self):
        for midi in (36, 48, 60, 69, 81, 93):
            lf = P.midinote_to_logfreq(midi)
            self.assertAlmostEqual(
                P.Freqlut.lookup(lf) * SAMPLE_RATE / (1 << 24),
                _logfreq_to_hz_rederived(lf), places=9)

    def test_a4_is_440_through_rederived_mapping(self):
        self.assertAlmostEqual(_logfreq_to_hz_rederived(self.A4), 440.0,
                               delta=0.05)

    def test_octave_doubles_increment(self):
        base = P.Freqlut.lookup(self.A4)
        up = P.Freqlut.lookup(P.i32(self.A4 + (1 << 24)))
        self.assertAlmostEqual(up / base, 2.0, delta=LUT_REL_TOL)

    def test_semitone_ratios_rederived(self):
        for midi in (60, 61, 63, 72):
            f1 = _logfreq_to_hz_rederived(P.midinote_to_logfreq(midi))
            f2 = _logfreq_to_hz_rederived(P.midinote_to_logfreq(midi + 1))
            self.assertAlmostEqual(f2 / f1, 2 ** (1 / 12), delta=LUT_REL_TOL)

    def test_lookup_monotone_around_a4(self):
        prev = P.Freqlut.lookup(P.i32(self.A4 - 4 * P.SEMI_LOGFREQ))
        for k in range(-3, 4):
            cur = P.Freqlut.lookup(P.i32(self.A4 + k * P.SEMI_LOGFREQ))
            self.assertGreater(cur, prev)
            prev = cur


# ---------------------------------------------------------------------------
# 2. Ratio vs fixed mode (dx7note.cc:38-64 semantics)
# ---------------------------------------------------------------------------

class TestRatioVsFixed(unittest.TestCase):
    def test_ratio_mode_tracks_keyboard(self):
        """Acceptance: ratio mode tracks the keyboard (issue #19)."""
        for coarse, fine, detune in ((1, 0, 7), (2, 0, 7), (1, 35, 3)):
            b60 = P.osc_freq(60, 0, coarse, fine, detune)
            b61 = P.osc_freq(61, 0, coarse, fine, detune)
            b72 = P.osc_freq(72, 0, coarse, fine, detune)
            if detune == 7:
                # detune 7 is the neutral center: the detune term vanishes
                # and the logfreq stays exactly linear in the note
                self.assertEqual(P.i32(b61 - b60), P.SEMI_LOGFREQ)
                self.assertEqual(P.i32(b72 - b60), 12 * P.SEMI_LOGFREQ)
            i60 = P.Freqlut.lookup(b60)
            tol = LUT_REL_TOL if detune == 7 else 3e-3
            self.assertAlmostEqual(P.Freqlut.lookup(b61) / i60,
                                   2 ** (1 / 12), delta=tol)
            self.assertAlmostEqual(P.Freqlut.lookup(b72) / i60, 2.0,
                                   delta=tol)

    def test_fixed_mode_ignores_keyboard(self):
        check_fixed_mode_ignores_keyboard(P.osc_freq)

    def test_fixed_mode_decade_scale(self):
        """Fixed frequency = 10^((100*(coarse&3)+fine)/100) Hz
        (dx7note.cc:59-61; 4458616 = (1<<24)*log2(10)*0.01*8)."""
        for coarse, fine, expect in ((1, 0, 10.0), (1, 99, 10 ** 1.99),
                                     (3, 0, 10 ** 3.0), (0, 0, 1.0)):
            b = P.osc_freq(60, 1, coarse, fine, 7)
            hz = P.Freqlut.lookup(b) * SAMPLE_RATE / (1 << 24)
            self.assertAlmostEqual(hz, expect, delta=expect * 2e-3,
                                   msg=f"fixed coarse={coarse} fine={fine}")

    def test_fixed_mode_detune_only_above_seven(self):
        b7 = P.osc_freq(60, 1, 1, 0, 7)
        b6 = P.osc_freq(60, 1, 1, 0, 6)
        b14 = P.osc_freq(60, 1, 1, 0, 14)
        self.assertEqual(b6, b7)     # dx7note.cc:61: only detune > 7 shifts
        self.assertEqual(P.i32(b14 - b7), 7 * 13457)

    def test_negative_fixed_mode_tracking_keyboard_fails(self):
        assert_check_fails(
            self, "fixed-mode tracks keyboard",
            lambda: check_fixed_mode_ignores_keyboard(
                _osc_freq_fixed_tracks_keyboard))


# ---------------------------------------------------------------------------
# 3. Coarse and detune arithmetic (dx7note.cc:30-36, 46-53)
# ---------------------------------------------------------------------------

class TestCoarseAndDetune(unittest.TestCase):
    def test_coarse_zero_is_minus_one_octave(self):
        """Pinned semantics: coarsemul[0] = -(1<<24): coarse 0 halves the
        ratio-mode frequency (dx7note.cc:30-31, 49)."""
        b1 = P.osc_freq(60, 0, 1, 0, 7)
        b0 = P.osc_freq(60, 0, 0, 0, 7)
        self.assertAlmostEqual(P.Freqlut.lookup(b0) / P.Freqlut.lookup(b1),
                               0.5, delta=LUT_REL_TOL)

    def test_coarse_two_doubles(self):
        b1 = P.osc_freq(60, 0, 1, 0, 7)
        b2 = P.osc_freq(60, 0, 2, 0, 7)
        self.assertAlmostEqual(P.Freqlut.lookup(b2) / P.Freqlut.lookup(b1),
                               2.0, delta=LUT_REL_TOL)

    def test_coarse_three_triples(self):
        """coarsemul[3] = 26591258 = round(2^24 * log2(3))."""
        self.assertEqual(P.COARSEMUL[3], 26591258)
        b1 = P.osc_freq(60, 0, 1, 0, 7)
        b3 = P.osc_freq(60, 0, 3, 0, 7)
        self.assertAlmostEqual(P.Freqlut.lookup(b3) / P.Freqlut.lookup(b1),
                               3.0, delta=5e-3)

    def test_coarse_mask_31_wrap(self):
        self.assertEqual(P.osc_freq(60, 0, 33, 0, 7),
                         P.osc_freq(60, 0, 1, 0, 7))
        self.assertEqual(P.osc_freq(60, 0, 32, 0, 7),
                         P.osc_freq(60, 0, 0, 0, 7))

    def test_coarsemul_entries_are_log2_arithmetic(self):
        for c in (2, 3, 4, 5, 16):
            self.assertAlmostEqual(P.COARSEMUL[c] / (1 << 24),
                                   math.log2(c), delta=2e-6)

    def test_detune_extremes_distinct_and_monotone(self):
        """Named acceptance case: detune extremes (+/-7) produce distinct
        increments (issue #19)."""
        incs = [P.osc_freq(60, 0, 1, 0, d) for d in (0, 7, 14)]
        self.assertEqual(len(set(incs)), 3)
        self.assertLess(incs[0], incs[1])
        self.assertLess(incs[1], incs[2])

    def test_detune_identity_linear_in_detune(self):
        check_detune_extreme_identity(P.osc_freq)

    def test_detune_matches_published_ratio(self):
        l7 = P.osc_freq(60, 0, 1, 0, 7)
        l14 = P.osc_freq(60, 0, 1, 0, 14)
        k = 0.0209 * math.exp(-0.396 * (l7 / (1 << 24))) / 7
        self.assertAlmostEqual(l14 / l7, 1 + 7 * k, delta=1e-3)

    def test_fine_99_is_near_an_octave(self):
        b0 = P.osc_freq(60, 0, 1, 0, 7)
        b99 = P.osc_freq(60, 0, 1, 99, 7)
        self.assertAlmostEqual(P.Freqlut.lookup(b99) / P.Freqlut.lookup(b0),
                               1.99, delta=2e-3)

    def test_negative_detune_extreme_wrap_fails(self):
        """Negative control: a wrong detune wrap (signed-nibble misread
        of the 0..14 field) must fail the identity check with the case
        named (issue #19 negative control)."""
        assert_check_fails(
            self, "detune-extreme wrap",
            lambda: check_detune_extreme_identity(_osc_freq_detune_wrapbug))


# ---------------------------------------------------------------------------
# 4. Transpose: documented vs pinned-wrapper divergence
# ---------------------------------------------------------------------------

class TestTransposeDivergence(unittest.TestCase):
    def test_documented_mode_shifts_ratio_pitch(self):
        """Upstream semantics: keydown applies data[144]-24
        (PluginProcessor.cpp:458, :566-580): transpose 36 sounds like
        midinote 72."""
        p36 = P.NotePitch(make_patch(transpose=36), 60,
                          transpose_mode="documented")
        p24_at72 = P.NotePitch(make_patch(transpose=24), 72,
                               transpose_mode="documented")
        self.assertEqual(p36.basepitch, p24_at72.basepitch)

    def test_documented_mode_leaves_fixed_ops(self):
        """The transpose enters osc_freq through the keyboard note only;
        fixed-mode operators are unaffected (dx7note.cc:58-61 ignores
        the note)."""
        fixed36 = make_patch(transpose=36)
        fixed36[21 * 0 + 17] = 1  # engine op 0 (patch chunk 0) fixed
        fixed24 = make_patch(transpose=24)
        fixed24[21 * 0 + 17] = 1
        a = P.NotePitch(fixed36, 60, transpose_mode="documented")
        b = P.NotePitch(fixed24, 60, transpose_mode="documented")
        self.assertEqual(a.basepitch[0], b.basepitch[0])      # fixed op
        self.assertNotEqual(a.basepitch[1], b.basepitch[1])   # ratio op

    def test_pinned_wrapper_mode_ignores_transpose(self):
        """Pinned oracle wrapper's keydown omits the shift (oracle
        main.cpp:168-194; registry finding perf-transpose-12)."""
        a = P.NotePitch(make_patch(transpose=36), 60,
                        transpose_mode="pinned_wrapper")
        b = P.NotePitch(make_patch(transpose=24), 60,
                        transpose_mode="pinned_wrapper")
        self.assertEqual(a.basepitch, b.basepitch)

    def test_divergence_is_real_and_nonzero(self):
        """Both behaviors computed; difference asserted NONZERO and equal
        to exactly 12 semitones in logfreq for the neutral operator."""
        a = P.NotePitch(make_patch(transpose=36), 60,
                        transpose_mode="documented")
        b = P.NotePitch(make_patch(transpose=36), 60,
                        transpose_mode="pinned_wrapper")
        diff = P.i32(a.basepitch[1] - b.basepitch[1])
        self.assertNotEqual(diff, 0, "documented-vs-wrapper divergence "
                                     "must be nonzero for transpose=36")
        self.assertEqual(diff, 12 * P.SEMI_LOGFREQ)
        ia = P.Freqlut.lookup(a.basepitch[1])
        ib = P.Freqlut.lookup(b.basepitch[1])
        self.assertAlmostEqual(ia / ib, 2.0, delta=LUT_REL_TOL)

    def test_registry_evidence_records_wrapper_omission(self):
        """Cross-reference: the committed registry carries the
        perf-transpose-12 finding (docs/REGISTRY-POLICY.md section 5);
        the executed smoke evidence covers the CC1 twin. The fresh
        transpose renders above are this issue's own measurement."""
        registry = json.loads(
            (_REPO / "spec" / "compatibility-registry-v1.json").read_text())
        case = next(c for c in registry["cases"]
                    if c["id"] == "perf-transpose-12")
        self.assertIn("identical_hash_vs:dir-base", case["expect"]["property"])
        self.assertIn("data[144]", case["expect"]["description"])
        if not REGISTRY_EVIDENCE.is_file():
            self.skipTest("NOT_RUN: registry smoke evidence not committed")
        rows = json.loads(REGISTRY_EVIDENCE.read_text())["cases"]
        by_id = {r["id"]: r for r in rows}
        self.assertEqual(by_id["dir-base"]["verdict"], "PASS")
        if "perf-transpose-12" in by_id:
            self.assertEqual(by_id["perf-transpose-12"]["verdict"], "PASS")
            self.assertEqual(by_id["perf-transpose-12"]["pcm_sha256"],
                             by_id["dir-base"]["pcm_sha256"])

    def test_transpose_shift_helper(self):
        self.assertEqual(P.transpose_shift(36, "documented"), 12)
        self.assertEqual(P.transpose_shift(0, "documented"), -24)
        self.assertEqual(P.transpose_shift(36, "pinned_wrapper"), 0)
        with self.assertRaises(ValueError):
            P.transpose_shift(24, "bogus")


# ---------------------------------------------------------------------------
# 5. CC1 / controller pitch_mod (controllers.h FmMod; ev-modwheel-127)
# ---------------------------------------------------------------------------

def _rederive_ctrl_pitch_mod(modwheel_cc: int, wheel_range: int) -> int:
    """Controllers::applyMod + refresh re-derived for the wheel only
    (controllers.h:66-77, 113-126): total = cc * (0.01*range)."""
    rng = 0.01 * wheel_range
    return int(modwheel_cc * rng) if rng else 0


class TestControllerCC1(unittest.TestCase):
    def test_cc1_inert_at_pinned_defaults(self):
        """Pinned FmMod wheel default range = 0 (controllers.h:43-48;
        manifest mod_wheel_default_modifiers) -> CC 1 cannot move pitch:
        increments with CC1=127 equal increments with CC1=0. Registry
        finding ev-modwheel-127."""
        patch = make_patch(lfo_pitch_mod_depth=99,
                           lfo_pitch_mod_sensitivity=7)
        note = P.NotePitch(patch, 60)
        lfo = P.Lfo()
        lfo.reset([35, 0, 0, 0, 1, 0])
        self.assertEqual(_rederive_ctrl_pitch_mod(127, 0), 0)
        for _ in range(64):
            lv, ld = lfo.getsample(), lfo.getdelay()
            self.assertEqual(
                note.frame_frequencies(lv, ld, ctrl_pitch_mod=0),
                note.frame_frequencies(
                    lv, ld,
                    ctrl_pitch_mod=_rederive_ctrl_pitch_mod(127, 0)))

    def test_cc1_live_with_documented_nonzero_range(self):
        """Documented response exists: with wheel range 99, CC1=127
        yields ctrl pitch_mod 125 and frame increments move (LFO patch
        depth 0 so the controller path is the only PM term)."""
        patch = make_patch(lfo_pitch_mod_depth=0,
                           lfo_pitch_mod_sensitivity=7)
        note = P.NotePitch(patch, 60)
        lfo = P.Lfo()
        lfo.reset([35, 0, 0, 0, 1, 0])
        lfo.keydown()
        lv, ld = lfo.getsample(), lfo.getdelay()
        cc127 = _rederive_ctrl_pitch_mod(127, 99)
        self.assertEqual(cc127, 125)
        f0 = note.frame_frequencies(lv, ld, ctrl_pitch_mod=0)
        f1 = note.frame_frequencies(lv, ld, ctrl_pitch_mod=cc127)
        self.assertNotEqual(f0, f1)

    def test_registry_evidence_records_cc1_inertia(self):
        if not REGISTRY_EVIDENCE.is_file():
            self.skipTest("NOT_RUN: registry smoke evidence not committed")
        rows = json.loads(REGISTRY_EVIDENCE.read_text())["cases"]
        by_id = {r["id"]: r for r in rows}
        self.assertEqual(by_id["ev-modwheel-127"]["verdict"], "PASS")
        self.assertEqual(by_id["ev-modwheel-127"]["pcm_sha256"],
                         by_id["ev-modwheel-0"]["pcm_sha256"])


# ---------------------------------------------------------------------------
# 6. Pitch EG analytic (pitchenv.cc)
# ---------------------------------------------------------------------------

class TestPitchEnvAnalytic(unittest.TestCase):
    def test_unit_48k(self):
        self.assertEqual(P.PitchEnv.unit_, 1050)
        self.assertEqual(P.PitchEnv.unit_,
                         int(64 * (1 << 24) / (21.3 * SAMPLE_RATE) + 0.5))

    def test_neutral_env_stays_zero(self):
        pe = P.PitchEnv()
        pe.set([99, 99, 99, 99], [50, 50, 50, 50])
        self.assertEqual(pe.level_, 0)
        for _ in range(200):
            self.assertEqual(pe.getsample(), 0)

    def test_rising_steps_and_target_catch(self):
        pe = P.PitchEnv()
        pe.set([20, 99, 99, 99], [60, 50, 50, 50])
        self.assertEqual(pe.level_, 0)
        self.assertEqual(pe.targetlevel_, 10 << 19)   # tab[60] = 10
        self.assertTrue(pe.rising_)
        inc = P.PitchEnv.PITCHENV_RATE[20] * P.PitchEnv.unit_
        self.assertEqual(inc, 12 * 1050)
        self.assertEqual(pe.getsample(), inc)
        self.assertEqual(pe.getsample(), 2 * inc)
        while pe.ix_ == 0:
            level = pe.getsample()
        self.assertEqual(pe.level_, 10 << 19)
        self.assertGreaterEqual(level, 10 << 19)

    def test_falling_steps_and_target_catch(self):
        pe = P.PitchEnv()
        pe.set([20, 99, 99, 99], [40, 50, 50, 50])
        self.assertEqual(pe.targetlevel_, P.PitchEnv.PITCHENV_TAB[40] << 19)
        self.assertFalse(pe.rising_)
        prev = pe.level_
        self.assertLess(pe.getsample(), prev)

    def test_keyup_release_and_retrigger(self):
        pe = P.PitchEnv()
        pe.set([99, 99, 99, 30], [70, 50, 50, 50])
        for _ in range(10):
            pe.getsample()
        pe.keydown(False)                      # keyup -> advance(3)
        self.assertEqual(pe.ix_, 3)
        target = P.PitchEnv.PITCHENV_TAB[50] << 19
        while pe.level_ != target:
            pe.getsample()
        self.assertEqual(pe.level_, target)
        pe.keydown(True)                       # retrigger -> advance(0)
        self.assertEqual(pe.ix_, 0)

    def test_tab_entries(self):
        tab = P.PitchEnv.PITCHENV_TAB
        self.assertEqual(len(tab), 100)
        self.assertEqual(tab[0], -128)
        self.assertEqual(tab[50], 0)
        self.assertEqual(tab[99], 127)
        rate = P.PitchEnv.PITCHENV_RATE
        self.assertEqual(len(rate), 100)
        self.assertEqual(rate[0], 1)
        self.assertEqual(rate[99], 255)


# ---------------------------------------------------------------------------
# 7. Pinned-tree verification of typed constants (guarded NOT_RUN)
# ---------------------------------------------------------------------------

def _c_array(text: str, name: str) -> list[int]:
    m = re.search(rf"{name}\[\s*\]?\s*=\s*\{{(.*?)\}};", text, re.S)
    if not m:
        raise ValueError(f"array {name} not found")
    return [int(x) for x in re.findall(r"-?\d+", m.group(1))]


class TestPinnedTreeConstants(unittest.TestCase):
    def setUp(self):
        if not DEXED_ROOT.is_dir():
            self.skipTest(
                f"NOT_RUN: pinned dexed tree unavailable at {DEXED_ROOT}")

    def _read(self, rel: str) -> str:
        return (DEXED_ROOT / rel).read_text()

    def test_pitchenv_tables_match_pinned_source(self):
        text = self._read("Source/msfa/pitchenv.cc")
        self.assertEqual(P.PitchEnv.PITCHENV_RATE,
                         tuple(_c_array(text, "pitchenv_rate")))
        self.assertEqual(P.PitchEnv.PITCHENV_TAB,
                         tuple(_c_array(text, "pitchenv_tab")))

    def test_coarsemul_matches_pinned_source(self):
        text = self._read("Source/msfa/dx7note.cc")
        self.assertEqual(P.COARSEMUL, tuple(_c_array(text, "coarsemul")))

    def test_pitchmodsenstab_matches_pinned_source(self):
        text = self._read("Source/msfa/dx7note.cc")
        self.assertEqual(P.PITCHMODSENSTAB,
                         tuple(_c_array(text, "pitchmodsenstab")))

    def test_tuning_base_matches_pinned_source(self):
        text = self._read("Source/msfa/tuning.cc")
        self.assertIn("50857777", text)
        self.assertIn("(1 << 24) / 12", text)


# ---------------------------------------------------------------------------
# 8. LFO model-direct: waveforms, rate shaping, delay (lfo.cc)
# ---------------------------------------------------------------------------

class TestLfoWaveforms(unittest.TestCase):
    MASK24 = (1 << 24) - 1

    def _lfo(self, wave: int, rate: int = 99) -> P.Lfo:
        lfo = P.Lfo()
        lfo.reset([rate, 0, 0, 0, 0, wave])
        return lfo

    def test_range_bounds_all_waves(self):
        for wave in range(6):
            lfo = self._lfo(wave)
            for v in [lfo.getsample() for _ in range(3000)]:
                self.assertGreaterEqual(v, 0, f"wave {wave}")
                self.assertLessEqual(v, 1 << 24, f"wave {wave}")

    def _at(self, wave: int, phase: int) -> int:
        """Sample a waveform exactly at `phase` (zero delta_ so the
        getsample() pre-advance is a no-op)."""
        lfo = self._lfo(wave)
        lfo.delta_ = 0
        lfo.phase_ = phase
        return lfo.getsample()

    def test_triangle_identities(self):
        # exact transcription of lfo.cc:55-59 at probe phases
        for phase in (0, 1 << 20, 1 << 30, (1 << 31) - 1, 1 << 31,
                      3 << 30, 0xFFFFFF00):
            a = self._at(0, phase)
            x = (phase >> 7) ^ -(phase >> 31)
            self.assertEqual(a, x & self.MASK24,
                             f"triangle formula at {phase:#x} (lfo.cc:55-59)")
        # shape: trough at phase 0, peak at half period, monotone legs
        self.assertEqual(self._at(0, 0), 0)
        self.assertEqual(self._at(0, (1 << 31) - (1 << 7)), self.MASK24)
        self.assertEqual(self._at(0, 1 << 31), self.MASK24)
        rising = [self._at(0, i << 27) for i in range(8)]
        self.assertEqual(rising, sorted(rising))
        falling = [self._at(0, (1 << 31) + (i << 27)) for i in range(8)]
        self.assertEqual(falling, sorted(falling, reverse=True))

    def test_saw_complement_identity(self):
        up, down = self._lfo(2), self._lfo(1)
        for phase in (0, 123 << 16, 1 << 31, 0xFFFF8000):
            up.phase_ = down.phase_ = phase
            a, b = up.getsample(), down.getsample()
            self.assertEqual(a + b, self.MASK24,
                             "saw up + saw down == 2^24 - 1 (lfo.cc:61-63)")

    def test_square_two_valued_half_duty(self):
        lfo = self._lfo(3)
        vals = [lfo.getsample() for _ in range(4000)]
        self.assertEqual(set(vals), {0, 1 << 24})
        hi = sum(1 for v in vals if v == 1 << 24)
        self.assertLess(abs(hi - len(vals) / 2), 16)

    def test_sine_center_and_antiperiod(self):
        center = 1 << 23
        lo, hi = 1 << 40, -(1 << 40)
        for phase in range(0, 1 << 24, 4099):
            v = P.i32(center + (P.sin_lookup(phase) >> 1))
            lo, hi = min(lo, v), max(hi, v)
            # antiperiod within 1 LSB: sintab[i + 512] == -sintab[i]
            # (sin.cc:43); the 1024-entry table spans input 2^24, so the
            # half-period shift is 2^23. The 1-LSB slack comes from the
            # floor-shift asymmetry of the delta interpolation.
            self.assertAlmostEqual(
                P.sin_lookup(phase),
                -P.sin_lookup(P.u32(phase + (1 << 23))),
                delta=1, msg=f"sine antiperiod at {phase:#x}")
        self.assertLess(lo - center, -(1 << 22) // 2)
        self.assertGreater(hi - center, (1 << 22) // 2)

    def test_sample_hold_lcg_and_hold(self):
        lfo = self._lfo(5)
        lfo.randstate_ = 42
        lfo.phase_ = 0
        v1 = lfo.getsample()
        self.assertEqual(v1, ((42 ^ 0x80) + 1) << 16)
        self.assertEqual(lfo.getsample(), v1)      # held between wraps
        self.assertEqual(lfo.randstate_, 42)
        while lfo.phase_ + lfo.delta_ < (1 << 32):
            lfo.phase_ = P.u32(lfo.phase_ + lfo.delta_)
        lfo.getsample()       # next advance wraps: phase_ < delta_ -> LCG
        self.assertEqual(lfo.randstate_, (42 * 179 + 17) & 0xFF)

    def test_rate_shaping_deltas(self):
        for rate in (0, 50, 99):
            lfo = P.Lfo()
            lfo.reset([rate, 0, 0, 0, 0, 0])
            sr = 1 if rate == 0 else (165 * rate) >> 6
            sr *= 11 if sr < 160 else (11 + ((sr - 160) >> 4))
            self.assertEqual(lfo.delta_, P.u32(P.Lfo.unit_ * sr),
                             f"rate {rate} (lfo.cc:32-35)")

    def test_unit_48k(self):
        self.assertEqual(P.Lfo.unit_, 33587)
        self.assertEqual(P.Lfo.unit_,
                         int(64 * 25190424 / SAMPLE_RATE + 0.5))

    def test_delay_param_zero_full_immediately(self):
        lfo = P.Lfo()
        lfo.reset([60, 0, 0, 0, 0, 0])
        self.assertEqual(lfo.delayinc_, 0xFFFFFFFF)
        self.assertEqual(lfo.getdelay(), 0xFFFFFF)
        self.assertEqual(lfo.getdelay(), 1 << 24)   # saturates (lfo.cc:81-83)

    def test_delay_param_99_ramps_late(self):
        lfo = P.Lfo()
        lfo.reset([60, 99, 0, 0, 0, 0])
        early = {lfo.getdelay() for _ in range(1500)}
        self.assertEqual(early, {0})
        later = max(lfo.getdelay() for _ in range(1500))
        self.assertGreater(later, 0)


# ---------------------------------------------------------------------------
# 9. LFO sync (lfo.cc:92-97) + negative control
# ---------------------------------------------------------------------------

class TestLfoSync(unittest.TestCase):
    def test_keysync_resets_phase_when_sync_set(self):
        check_lfo_keysync_restarts_phase(P.Lfo)

    def test_keysync_resets_delay_always(self):
        for sync in (0, 1):
            lfo = P.Lfo()
            lfo.reset([60, 0, 0, 0, sync, 0])
            lfo.getdelay()
            self.assertEqual(lfo.delaystate_, 0xFFFFFFFF)
            lfo.keydown()
            self.assertEqual(lfo.delaystate_, 0)

    def test_no_sync_keeps_phase_but_resets_delay(self):
        a = P.Lfo()
        a.reset([99, 0, 0, 0, 0, 2])
        b = P.Lfo()               # twin that never sees keydown
        b.reset([99, 0, 0, 0, 0, 2])
        for _ in range(10):
            a.getsample()
            b.getsample()
        a.keydown()               # sync off: phase preserved (lfo.cc:93-95)
        for _ in range(8):
            self.assertEqual(a.getsample(), b.getsample())

    def test_negative_lfo_sync_ignored_fails(self):
        """Negative control: a model that ignores sync must fail the
        keysync trajectory check with the case named (issue #19)."""
        assert_check_fails(
            self, "LFO sync ignored",
            lambda: check_lfo_keysync_restarts_phase(_LfoSyncIgnored))


# ---------------------------------------------------------------------------
# 10. Rendered: static pitch (model vs pinned oracle PCM)
# ---------------------------------------------------------------------------

class TestRenderedStaticPitch(unittest.TestCase):
    LOUD = "op3.output_level"   # patch chunk 2 == engine op 2

    def test_midinote_60_matches_model(self):
        patch = make_patch(**{self.LOUD: 99})
        pcm = render_oracle(self, patch, "2400 note_on 60 100\n", 0.8)
        self.assertGreater(peak_abs(pcm, 0.2, 0.7), 0.01)
        note = P.NotePitch(patch, 60, transpose_mode="pinned_wrapper")
        increment = P.Freqlut.lookup(note.basepitch[2])
        expected = increment * SAMPLE_RATE / (1 << 24)
        track = interval_freqs(pcm, 0.2, 0.7)
        self.assertGreater(len(track), 20)
        for t, f, _dt in track:
            self.assertAlmostEqual(f, expected, delta=expected * TRAJ_REL_TOL,
                                   msg=f"t={t:.4f}s")

    def test_midinote_72_is_double_60(self):
        patch = make_patch(**{self.LOUD: 99})
        lo = render_oracle(self, patch, "2400 note_on 60 100\n", 0.6)
        hi = render_oracle(self, patch, "2400 note_on 72 100\n", 0.6)
        f_lo, f_hi = lsq_freq(lo, 0.2, 0.5), lsq_freq(hi, 0.2, 0.5)
        self.assertAlmostEqual(f_hi / f_lo, 2.0, delta=1e-3)

    def test_fixed_mode_render_ignores_keyboard(self):
        """Named acceptance case, rendered: fixed-frequency mode does not
        track the keyboard on the pinned oracle."""
        patch = make_patch(**{"op3.osc_mode": 1, "op3.osc_freq_coarse": 3,
                           self.LOUD: 99})
        lo = render_oracle(self, patch, "2400 note_on 60 100\n", 0.6)
        hi = render_oracle(self, patch, "2400 note_on 72 100\n", 0.6)
        f_lo, f_hi = lsq_freq(lo, 0.2, 0.5), lsq_freq(hi, 0.2, 0.5)
        self.assertAlmostEqual(f_lo, f_hi, delta=f_lo * 1e-3)
        self.assertAlmostEqual(f_lo, 1000.0, delta=2.0)   # 10^(3.00) Hz

    def test_ratio_mode_render_tracks_keyboard(self):
        patch = make_patch(**{"op3.osc_freq_coarse": 2, self.LOUD: 99})
        pcm = render_oracle(self, patch, "2400 note_on 60 100\n", 0.6)
        f = lsq_freq(pcm, 0.2, 0.5)
        self.assertAlmostEqual(f, 2 * F0_C4, delta=2 * F0_C4 * 1e-3)

    def test_transpose_does_not_shift_render_pinned_wrapper(self):
        """One-sided confirmation of registry perf-transpose-12 on a
        fresh render: transpose 36 renders the same pitch as 24 under
        the pinned wrapper (LFO inert here; frequency comparison)."""
        p24 = render_oracle(self, make_patch(**{self.LOUD: 99}),
                            "2400 note_on 60 100\n", 0.6)
        p36 = render_oracle(self, make_patch(transpose=36, **{self.LOUD: 99}),
                            "2400 note_on 60 100\n", 0.6)
        f24, f36 = lsq_freq(p24, 0.2, 0.5), lsq_freq(p36, 0.2, 0.5)
        self.assertAlmostEqual(f24, f36, delta=f24 * 1e-3)


# ---------------------------------------------------------------------------
# 11. Rendered: pitch EG trajectory (rising AND falling)
# ---------------------------------------------------------------------------

class TestRenderedPitchEnv(unittest.TestCase):
    def _render_case(self, rates, levels):
        patch = make_patch(**{"op3.output_level": 99, "peg_rates": rates,
                           "peg_levels": levels})
        pcm = render_oracle(self, patch, "2400 note_on 60 100\n", 1.2)
        self.assertGreater(peak_abs(pcm, 0.1, 1.1), 0.01,
                           "render too quiet for the estimator")
        lfo = P.Lfo()
        lfo.reset([patch[137], patch[138], 0, 0, patch[141], patch[142]])
        note = P.NotePitch(patch, 60, transpose_mode="pinned_wrapper")
        devs = model_frame_devs(note, lfo, 900)
        checked = 0
        for t, f, _dt in interval_freqs(pcm, 0.08, 1.1):
            k = frame_index(t) - ONSET_FRAME
            if not 0 <= k < len(devs):
                continue
            predicted = F0_C4 * 2 ** devs[k]
            self.assertAlmostEqual(f, predicted, delta=predicted * TRAJ_REL_TOL,
                                   msg=f"t={t:.4f}s frame {k}")
            checked += 1
        self.assertGreater(checked, 40, "too few estimator windows")
        return pcm

    def test_rising_pitch_eg(self):
        """Rising case: rates (20,20,20,99), levels (60,60,60,50): sweep
        up to +10<<19 (~+0.31 oct) then hold (issue #19)."""
        pcm = self._render_case([20, 20, 20, 99], [60, 60, 60, 50])
        early = lsq_freq(pcm, 0.05, 0.08)    # sweep barely started
        late = lsq_freq(pcm, 0.9, 1.1)       # held
        self.assertLess(early, F0_C4 * 1.01)
        self.assertGreater(late, F0_C4 * 1.2)
        held = F0_C4 * 2 ** ((P.PitchEnv.PITCHENV_TAB[60] << 19) / (1 << 24))
        self.assertAlmostEqual(late, held, delta=held * ENDPOINT_REL_TOL)

    def test_falling_pitch_eg(self):
        """Falling case: rates (20,20,20,99), levels (40,40,40,50):
        sweep down to -8<<19 (~-0.06 oct) then hold."""
        pcm = self._render_case([20, 20, 20, 99], [40, 40, 40, 50])
        early = lsq_freq(pcm, 0.05, 0.08)
        late = lsq_freq(pcm, 0.9, 1.1)
        self.assertGreater(early, F0_C4 * 0.99)
        self.assertLess(late, F0_C4 * 0.98)
        held = F0_C4 * 2 ** ((P.PitchEnv.PITCHENV_TAB[40] << 19) / (1 << 24))
        self.assertAlmostEqual(late, held, delta=held * ENDPOINT_REL_TOL)

    def test_rising_and_falling_renders_distinct(self):
        up = self._render_case([20, 20, 20, 99], [60, 60, 60, 50])
        down = self._render_case([20, 20, 20, 99], [40, 40, 40, 50])
        f_up, f_dn = lsq_freq(up, 0.6, 0.9), lsq_freq(down, 0.6, 0.9)
        self.assertGreater(f_up, f_dn * 1.2)


# ---------------------------------------------------------------------------
# 12. Rendered: LFO pitch-mod depth (vibrato extent vs model)
# ---------------------------------------------------------------------------

class TestRenderedLfoPM(unittest.TestCase):
    def _extent(self, tc, depth, sens, speed):
        patch = make_patch(**{"op3.output_level": 99,
                              "lfo_pitch_mod_depth": depth,
                              "lfo_pitch_mod_sensitivity": sens,
                              "lfo_speed": speed})
        pcm = render_oracle(tc, patch, "2400 note_on 60 100\n", 1.6)
        tc.assertGreater(peak_abs(pcm, 0.15, 1.5), 0.01)
        track = interval_freqs(pcm, 0.15, 1.5)
        tc.assertGreater(len(track), 100)
        devs = [math.log2(f / F0_C4) for _, f, _ in track]
        wsum = sum(dt for _, _, dt in track)
        center = 2 ** (sum(l2 * dt for (_, f, dt), l2
                           in zip(track, devs)) / wsum)
        return min(devs), max(devs), center, patch

    def test_vibrato_extent_matches_model(self):
        for depth, sens, speed in ((99, 7, 60), (50, 3, 60), (25, 7, 45)):
            lo, hi, center, patch = self._extent(self, depth, sens, speed)
            lfo = P.Lfo()
            lfo.reset([speed, 0, 0, 0, patch[141], patch[142]])
            note = P.NotePitch(patch, 60, transpose_mode="pinned_wrapper")
            mdevs = model_frame_devs(note, lfo, 1100)
            mlo, mhi = min(mdevs), max(mdevs)
            # one-sided-safe comparison: extents are phase-independent
            self.assertGreaterEqual(lo, mlo - EXTENT_TOL_OCT,
                                    f"depth={depth} sens={sens}: low side")
            self.assertLessEqual(hi, mhi + EXTENT_TOL_OCT,
                                 f"depth={depth} sens={sens}: high side")
            self.assertAlmostEqual(center, 1.0, delta=CENTER_REL_TOL,
                                   msg=f"depth={depth} center")
            self.assertLess(abs(lo - mlo), EXTENT_TOL_OCT,
                            f"depth={depth} sens={sens}: low approach")
            self.assertLess(abs(hi - mhi), EXTENT_TOL_OCT,
                            f"depth={depth} sens={sens}: high approach")

    def test_depth_zero_control_no_vibrato(self):
        lo, hi, center, _ = self._extent(self, 0, 0, 60)
        self.assertLess(hi - lo, VIB_CONTROL_OCT)
        self.assertAlmostEqual(center, 1.0, delta=1e-3)

    def test_extents_distinguish_depths(self):
        _, hi_a, _, _ = self._extent(self, 99, 7, 60)
        _, hi_b, _, _ = self._extent(self, 25, 7, 60)
        self.assertGreater(hi_a, hi_b + 0.3)


# ---------------------------------------------------------------------------

if __name__ == "__main__":
    unittest.main()
