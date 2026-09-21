"""N04 probes for the frozen integer algorithm/feedback model (issue #18).

Layers, per the issue's acceptance list:

1. Scripted-source validation — for EACH of the 32 algorithms the router is
   driven with scripted operator sources (a test seam replaces mkiSin with
   known sequences that are modulation-sensitive), and the observed call
   sequence (which op evaluated at which global sample with which
   modulation/feedback term) plus the master output is compared against an
   INDEPENDENT expectation computed from a hand-derived wiring table. The
   wiring table is a re-derivation from the citation
   (Source/msfa/fm_core.cc:29-62 + EngineMkI.cpp:295-363), not a restatement:
   the test also asserts the wiring table and the packed table decode to the
   same roles, so a transcription error on either side is caught.
2. Feedback semantics — shift mapping per feedback value (dx7note.cc:197),
   the algorithm 32 / algorithm 4 / algorithm 6 +2 shift rule
   (EngineMkI.cpp:330,337,343), the two-element history depth probed by a
   one-frame impulse (influence window exactly two samples of direct
   history reads), recirculation values, and the one-frame-per-render phase
   advance of serial-consumed operators (EngineMkI.cpp:331-339).
3. Reference sweep (guarded skip -> NOT_RUN when the pinned oracle is
   absent, never a silent pass): synthetic six-operator voices rendered for
   ALL 32 algorithms plus directed feedback cases (algorithms 4 and 6 at
   feedback 0/3/7, feedback-off paths, gating/has_contents cases, retrigger
   and release) — pinned oracle (hash-verified) vs this model, compared dry
   with src/gf180_dx7/compare.py. Declared budget: EXACT (byte-identical,
   max_abs_error == 0.0): every integer operation is mirrored and the
   tables are byte-identical (N02), so any nonzero error is a model defect,
   not a numeric budget.
4. Negative controls — each must demonstrably fail the check it targets:
   an operator-index permutation in the router; a one-sample-broken feedback
   history (reads delayed by one whole 64-sample block); and a corrupted
   carrier-sum accumulation (last-writer-wins, the observable corruption of
   sum-order semantics — pure reordering of the final integer sum is
   provably unobservable modulo 2^32, docs/N04-ALGORITHM-MODEL.md).

No fidelity claim: oracle agreement is software-reference agreement only
(reference/oracle-protocol.md section 4). Stdlib-only.
"""

from __future__ import annotations

import hashlib
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
for p in (str(REPO / "src"), str(REPO / "tools")):
    if p not in sys.path:
        sys.path.insert(0, p)

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model import algorithm as A  # noqa: E402
from gf180_dx7.model import tables as tmod  # noqa: E402
from gf180_dx7.model.algorithm import (  # noqa: E402
    ALGORITHMS, AlgorithmRouter, FmParams, decode_op, fb_shift_for)
from render_reference import (  # noqa: E402
    HarnessFail, HarnessNotRun, check_oracle_binary, load_identity, read_f32)

N_FRAMES = 3
PARKED_LEVEL = 15360 << 14      # frame gain2 = ENV_MAX - 15360 = 1024
GATED_LEVEL = 16 << 16          # env.cc:119 clamp floor: gain 16320 > 16284
K_LEVEL_THRESH = 16384 - 100    # EngineMkI.cpp:296
RATE = 48000
SWEEP_SECONDS = 2.0
EVIDENCE_PATH = REPO / "reference" / "evidence" / "n04-algorithm" / \
    "algorithm-sweep.json"
ORACLE_ABSENT = "pinned oracle binary absent or unpinned; sweep NOT_RUN"

# Scripted source: op k's output at global sample t with modulation term m.
BASE = [110 * (k + 1) for k in range(6)]
STEP = [7 * k + 3 for k in range(6)]
WMUL = 3


def scripted_y(op: int, t: int, mod: int) -> int:
    return BASE[op] + t * STEP[op] + WMUL * mod


# ---------------------------------------------------------------------------
# 1. Hand-derived wiring table (the re-derived connection table)
#------------------------------------------------------------------------------
# One row per algorithm (1..32); each row lists ops in engine param order
# (params_[0..5]); "<in>:<out>" with in: Z (no input bus), b1, b2; out:
# b1 / b1+ / b2 (interconnect buses, + = accumulate) and O (note output,
# always accumulate). "F" marks the feedback-loop op ((flags & 0xc0) == 0xc0).
# Derived BY HAND from fm_core.cc:29-62 decoded roles + EngineMkI.cpp:295-363
# dispatch, cross-checked against Dexed's own algorithm diagrams
# (Source/AlgoDisplay.cpp) for the feedback op and carrier positions.

WIRING_ROWS = (
    ("Z>b1F", "b1>b1", "b1>b1", "b1>O", "Z>b1", "b1>O"),   # 1
    ("Z>b1", "b1>b1", "b1>b1", "b1>O", "Z>b1F", "b1>O"),   # 2
    ("Z>b1F", "b1>b1", "b1>O", "Z>b1", "b1>b1", "b1>O"),   # 3
    ("Z>b1F", "b1>b1", "b1>O", "Z>b1", "b1>b1", "b1>O"),   # 4
    ("Z>b1F", "b1>O", "Z>b1", "b1>O", "Z>b1", "b1>O"),     # 5
    ("Z>b1F", "b1>O", "Z>b1", "b1>O", "Z>b1", "b1>O"),     # 6
    ("Z>b1F", "b1>b1", "Z>b1+", "b1>O", "Z>b1", "b1>O"),   # 7
    ("Z>b1", "b1>b1", "Z>b1+F", "b1>O", "Z>b1", "b1>O"),   # 8
    ("Z>b1", "b1>b1", "Z>b1+", "b1>O", "Z>b1F", "b1>O"),   # 9
    ("Z>b1", "Z>b1+", "b1>O", "Z>b1F", "b1>b1", "b1>O"),   # 10
    ("Z>b1F", "Z>b1+", "b1>O", "Z>b1", "b1>b1", "b1>O"),   # 11
    ("Z>b1", "Z>b1+", "Z>b1+", "b1>O", "Z>b1F", "b1>O"),   # 12
    ("Z>b1F", "Z>b1+", "Z>b1+", "b1>O", "Z>b1", "b1>O"),   # 13
    ("Z>b1F", "Z>b1+", "b1>b1", "b1>O", "Z>b1", "b1>O"),   # 14
    ("Z>b1", "Z>b1+", "b1>b1", "b1>O", "Z>b1F", "b1>O"),   # 15
    ("Z>b1F", "b1>b1", "Z>b2", "b2>b1+", "Z>b1+", "b1>O"),  # 16
    ("Z>b1", "b1>b1", "Z>b2", "b2>b1+", "Z>b1+F", "b1>O"),  # 17
    ("Z>b1", "b1>b1", "b1>b1", "Z>b1+F", "Z>b1+", "b1>O"),  # 18
    ("Z>b1F", "b1>O", "b1>O", "Z>b1", "b1>b1", "b1>O"),    # 19
    ("Z>b1", "Z>b1+", "b1>O", "Z>b1F", "b1>O", "b1>O"),    # 20
    ("Z>b1", "b1>O", "b1>O", "Z>b1F", "b1>O", "b1>O"),     # 21
    ("Z>b1F", "b1>O", "b1>O", "b1>O", "Z>b1", "b1>O"),     # 22
    ("Z>b1F", "b1>O", "b1>O", "Z>b1", "b1>O", "Z>O"),      # 23
    ("Z>b1F", "b1>O", "b1>O", "b1>O", "Z>O", "Z>O"),       # 24
    ("Z>b1F", "b1>O", "b1>O", "Z>O", "Z>O", "Z>O"),        # 25
    ("Z>b1F", "Z>b1+", "b1>O", "Z>b1", "b1>O", "Z>O"),     # 26
    ("Z>b1", "Z>b1+", "b1>O", "Z>b1F", "b1>O", "Z>O"),     # 27
    ("Z>O", "Z>b1F", "b1>b1", "b1>O", "Z>b1", "b1>O"),     # 28
    ("Z>b1F", "b1>O", "Z>b1", "b1>O", "Z>O", "Z>O"),       # 29
    ("Z>O", "Z>b1F", "b1>b1", "b1>O", "Z>O", "Z>O"),       # 30
    ("Z>b1F", "b1>O", "Z>O", "Z>O", "Z>O", "Z>O"),         # 31
    ("Z>OF", "Z>O", "Z>O", "Z>O", "Z>O", "Z>O"),           # 32
)

_IN = {"Z": 0, "b1": 1, "b2": 2}


def _parse_wiring(rows):
    ops = []
    for spec in rows:
        inp, dst = spec.split(">")
        isfb = dst.endswith("F")
        if isfb:
            dst = dst[:-1]
        ops.append((_IN[inp], dst, isfb))
    return ops


WIRING = [(_parse_wiring(rows), rows) for rows in WIRING_ROWS]

# EngineMkI.cpp:301-305,327-348: the serial-form algorithms (0-based) and
# their consumed op counts when feedback is on; ALGO 32 gets +2 shift.
SERIAL_OPS = {3: 3, 5: 2}
SPECIAL_PLUS2 = frozenset((3, 5, 31))


# ---------------------------------------------------------------------------
# Independent expectation evaluator (from the wiring table, not the router)
#------------------------------------------------------------------------------


def expected_run(alg: int, feedback: int, n_frames: int = N_FRAMES,
                 gated=()):
    """Recompute the expected scripted run from the wiring table.

    Returns (calls, frames, fb_final): calls is the full ordered list of
    (op, t, mod, y) the router must produce; frames the per-frame master
    outputs; fb_final the two-element history after the run.
    """
    ops, _ = WIRING[alg]
    fb_on = feedback != 0
    shift = fb_shift_for(feedback)
    serial = SERIAL_OPS.get(alg) if fb_on else None
    gated = set(gated)
    bus1 = [0] * 64   # stale contents persist across frames (fm_core buf_)
    bus2 = [0] * 64
    y0, y1 = 0, 0     # fb_buf_[2] (dx7note.h:77)
    calls = []
    frames = []

    def w32(v):
        """C int32 wrap (two's complement), as in EngineMkI's int math."""
        v &= 0xFFFFFFFF
        return v - (1 << 32) if v >= (1 << 31) else v

    def src(op, mod):
        # the scripted seam's global sample counter is the call index: the
        # driver records t = len(calls), and serial-form ops interleave
        # within one sample, so the evaluator uses the same definition.
        t = len(calls)
        y = scripted_y(op, t, mod)
        calls.append((op, t, mod, y))
        return y

    for _ in range(n_frames):
        hc = [True, False, False]          # EngineMkI.cpp:298, per frame
        frame = [0] * 64
        op = 0
        while op < 6:
            if serial and op == 0 and 0 not in gated:
                # inline serial form (EngineMkI.cpp:327-340)
                s = min(shift + 2, 16)
                for i in range(64):
                    mod = w32(y0 + y1) >> (s + 1)   # :227/:271
                    y0 = y1
                    ya = src(0, mod)
                    yb = src(1, ya)
                    final = src(2, yb) if serial == 3 else yb
                    frame[i] = w32(final)           # :240/:289
                    y1 = final
                hc[0] = True
                op += serial
                continue
            inp, dst, isfb = ops[op]
            add = dst in ("b1+", "O")
            outbus = {"b1": 1, "b1+": 1, "b2": 2, "O": 0}[dst]
            if serial and op == 0:
                # EngineMkI.cpp:301-305 patches op0 to 0xc4 whenever fb_on,
                # regardless of later gating: dest becomes the output bus
                # with add, so a gated op0 leaves has_contents[0] alone.
                dst, add, outbus = "O", True, 0
            if op in gated:
                if not add:                 # EngineMkI.cpp:358-360
                    hc[outbus] = False
                op += 1
                continue
            add_eff = add and hc[outbus]    # :320-322
            inbus = inp
            for i in range(64):
                if isfb and fb_on:
                    s = min(shift + 2, 16) if alg in SPECIAL_PLUS2 else shift
                    mod = w32(y0 + y1) >> (s + 1)   # :196
                    y0 = y1
                    y = src(op, mod)
                    y1 = y
                else:
                    if inbus == 0:               # :324 (no input bus)
                        mod = 0
                    elif (inbus == 1 and hc[1]) or (inbus == 2 and hc[2]):
                        mod = (bus1 if inbus == 1 else bus2)[i]
                    else:                        # empty input bus -> pure
                        mod = 0
                    y = src(op, mod)
                # output[i] = y + adder[i]; adder = add ? output : zeros
                # (EngineMkI.cpp:158,163,179,199) in C int32 arithmetic.
                if dst == "O":
                    frame[i] = w32(frame[i] + y) if add_eff else y
                elif dst == "b1":
                    bus1[i] = y
                elif dst == "b1+":
                    bus1[i] = w32(bus1[i] + y) if add_eff else y
                elif dst == "b2":
                    bus2[i] = y
            hc[outbus] = True                    # :357
            op += 1
        frames.append(frame)
    return calls, frames, [w32(y0), w32(y1)]


# ---------------------------------------------------------------------------
# Router driver with the scripted-source seam
#------------------------------------------------------------------------------


def drive_router(alg: int, feedback: int, n_frames: int = N_FRAMES,
                 gated=(), *, router=None, params=None, fb_buf=None):
    if router is None:
        router = AlgorithmRouter(None, None)  # tables unused: sin_fn scripted
    if params is None:
        params = [FmParams() for _ in range(6)]
        for k, p in enumerate(params):
            p.freq = 1000 * (k + 1)
            p.level_in = GATED_LEVEL if k in gated else PARKED_LEVEL
    if fb_buf is None:
        fb_buf = [0, 0]
    calls = []
    frames = []

    def sin_fn(op, i, phase_plus, gain, mod):
        y = scripted_y(op, len(calls), mod)
        calls.append((op, len(calls), mod, y))
        return y

    shift = fb_shift_for(feedback)
    for _ in range(n_frames):
        out = [0] * 64
        router.render(out, params, alg, fb_buf, shift, sin_fn=sin_fn)
        frames.append(out)
    return calls, frames, fb_buf, params


def scripted_case_ok(alg: int, feedback: int, n_frames: int = N_FRAMES,
                     gated=(), *, router=None, params=None, fb_buf=None):
    """True iff the router's scripted run equals the wiring-table expectation
    in call sequence, master outputs, final history, and phase bookkeeping."""
    calls, frames, fb_final, params = drive_router(
        alg, feedback, n_frames, gated, router=router, params=params,
        fb_buf=fb_buf)
    exp_calls, exp_frames, exp_fb = expected_run(alg, feedback, n_frames,
                                                 gated)
    if calls != exp_calls:
        return False
    if frames != exp_frames:
        return False
    if fb_final != exp_fb:
        return False
    for p in params:  # every op advances exactly one frame per render
        if p.phase != ((p.freq << 6) * n_frames) % (1 << 32):
            return False
    return True


# ---------------------------------------------------------------------------
# Pinned oracle harness (guarded)
#------------------------------------------------------------------------------


def _oracle():
    try:
        identity = load_identity(REPO / "reference" / "oracle-identity.json")
        path = Path(identity["binary"]["path"])
        check_oracle_binary(identity, path)
        return path
    except (HarnessFail, HarnessNotRun):
        return None


ORACLE = _oracle()


class OracleRenderer:
    """Renders (voice, events) with the pinned oracle; caches by input hash
    inside one process (the sweep + directed cases share renders)."""

    def __init__(self, oracle_path):
        self.oracle = oracle_path
        self.tmp = tempfile.TemporaryDirectory(prefix="n04-alg-")
        self.cache = {}
        self.runs = []

    def close(self):
        self.tmp.cleanup()

    def render(self, voice: sysex.Voice, events: str, seconds: float) -> bytes:
        voice_bytes = sysex.encode_voice(voice)
        key = (hashlib.sha256(voice_bytes).hexdigest(),
               hashlib.sha256(events.encode()).hexdigest(), seconds)
        if key in self.cache:
            return self.cache[key]
        vpath = Path(self.tmp.name) / f"v-{key[0][:12]}.syx"
        epath = Path(self.tmp.name) / f"e-{key[1][:12]}.txt"
        opath = Path(self.tmp.name) / f"o-{key[0][:12]}-{key[1][:12]}.f32"
        vpath.write_bytes(voice_bytes)
        epath.write_text(events, encoding="utf-8")
        proc = subprocess.run(
            [str(self.oracle), "--voice", str(vpath), "--events", str(epath),
             "--out", str(opath), "--rate", str(RATE),
             "--seconds", str(seconds)], capture_output=True, text=True)
        if proc.returncode != 0:
            raise AssertionError(
                f"oracle failed (exit {proc.returncode}): {proc.stderr}")
        data = opath.read_bytes()
        if len(data) != 4 * int(RATE * seconds):
            raise AssertionError("oracle produced an unexpected length")
        self.runs.append({
            "voice_sha256": key[0], "events_sha256": key[1],
            "seconds": seconds,
            "pcm_sha256": hashlib.sha256(data).hexdigest(),
        })
        self.cache[key] = data
        return data


def sweep_op(tl: int) -> sysex.Operator:
    """Audible parked-envelope operator (rates/levels 99 park every segment
    at its target from frame 1, N02 analytics; osc bytes neutral)."""
    return sysex.Operator(
        rate_1=99, rate_2=99, rate_3=99, rate_4=99,
        level_1=99, level_2=99, level_3=99, level_4=99,
        break_point=40, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=0,
        amp_mod_sensitivity=0, key_velocity_sensitivity=0,
        output_level=tl, osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0,
        osc_detune=7)


def sweep_op_release0(tl: int) -> sysex.Operator:
    """Parked envelope with release level 0: after note_off the envelope
    parks at the 16<<16 clamp floor, gain 16368 > 16284, so the op is
    gate-skipped exactly (EngineMkI.cpp:314-318) and contributes zero."""
    op = sweep_op(tl)
    op.level_4 = 0
    return op


def sweep_voice(algorithm: int, feedback: int, *, osc_key_sync: int = 1,
                gated_params=(), release0: bool = False) -> sysex.Voice:
    """All-six-operators-audible voice for one algorithm (0-based byte).
    gated_params are ENGINE param indices made envelope-silent (TL 0 ->
    parked clamp floor -> the Mark I gate skips them); engine param k is
    repo operators[5-k] (the P01 unpacked-block order, sysex.py)."""
    opf = sweep_op_release0 if release0 else sweep_op
    ops = [opf(0 if (5 - j) in set(gated_params) else 80) for j in range(6)]
    return sysex.Voice(
        operators=ops,
        pitch_eg_rate_1=0, pitch_eg_rate_2=0, pitch_eg_rate_3=0,
        pitch_eg_rate_4=0, pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=algorithm, feedback=feedback, osc_key_sync=osc_key_sync,
        lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=0, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name_bytes=b"N04SWEEP", channel_byte=0)


def model_render(voice: sysex.Voice, events: str, seconds: float) -> bytes:
    return A.render_note(voice, events, seconds=seconds, rate=RATE)


# ---------------------------------------------------------------------------
# 2. unittest classes
#------------------------------------------------------------------------------


class TestConnectionTable(unittest.TestCase):
    """The packed table, its decode, and the hand-derived wiring must agree;
    exactly one feedback op per algorithm (the DX7 structure)."""

    def test_exactly_one_feedback_op_per_algorithm(self):
        for alg in range(32):
            fbs = [k for k in range(6) if decode_op(ALGORITHMS[alg][k])[3]]
            self.assertEqual(len(fbs), 1, f"algorithm {alg + 1}")
            wiring = WIRING[alg][0]
            self.assertEqual([k for k in range(6) if wiring[k][2]], fbs)

    def test_wiring_table_matches_packed_decode(self):
        """Transcription guard: the hand-derived wiring and the packed table
        (fm_core.cc:29-62) must encode identical roles."""
        for alg in range(32):
            wiring = WIRING[alg][0]
            for k in range(6):
                inbus, outbus, add, isfb = decode_op(ALGORITHMS[alg][k])
                w_in, w_dst, w_fb = wiring[k]
                self.assertEqual(inbus, w_in, f"alg {alg + 1} op {k + 1}")
                self.assertEqual(isfb, w_fb, f"alg {alg + 1} op {k + 1}")
                dst_outbus = {"b1": 1, "b1+": 1, "b2": 2, "O": 0}[w_dst]
                self.assertEqual(outbus, dst_outbus,
                                 f"alg {alg + 1} op {k + 1}")
                dst_add = w_dst in ("b1+", "O")
                self.assertEqual(add, dst_add, f"alg {alg + 1} op {k + 1}")

    def test_serial_and_plus2_dispatch_facts(self):
        """EngineMkI.cpp:301-305,327-348: only ALGOs 4/6 replace op0 flags
        (0xc4) with feedback on; only ALGOs 4/6/32 take min(shift+2, 16)."""
        self.assertEqual(SERIAL_OPS, {3: 3, 5: 2})
        self.assertEqual(SPECIAL_PLUS2, frozenset((3, 5, 31)))
        for alg in (3, 5):
            self.assertEqual(ALGORITHMS[alg][0] & 0xC0, 0xC0)
        # ALGO 32's op0 is already routed to the output bus with add
        self.assertEqual(ALGORITHMS[31][0], 0xC4)


class TestScriptedRouting(unittest.TestCase):
    """Scripted-source validation of all 32 graphs against the hand-derived
    wiring expectations (issue acceptance item 1)."""

    def test_all_32_algorithms_feedback_on(self):
        for alg in range(32):
            with self.subTest(algorithm=alg + 1, feedback=7):
                self.assertTrue(scripted_case_ok(alg, 7),
                                f"algorithm {alg + 1} fb 7 mismatch")

    def test_all_32_algorithms_feedback_off(self):
        for alg in range(32):
            with self.subTest(algorithm=alg + 1, feedback=0):
                self.assertTrue(scripted_case_ok(alg, 0),
                                f"algorithm {alg + 1} fb 0 mismatch")

    def test_serial_forms_fb_on_follow_wiring(self):
        """ALGOs 4/6 with feedback: the inline serial form must produce the
        in-sample chain op0(fb) -> op1 [-> op2] -> output."""
        for alg, count in ((3, 3), (5, 2)):
            calls, frames, fb_final, _ = drive_router(alg, 7)
            # op0..count-1 evaluated interleaved per sample; the seam's
            # global t is the call index, so sample = t // count
            first_frame = [c for c in calls if c[1] < 64 * count]
            ops_in_order = [c[0] for c in first_frame]
            prefix = ([0, 1, 2] if count == 3 else [0, 1]) * 2
            self.assertEqual(ops_in_order[:len(prefix)], prefix)
            # op1's mod at sample i is op0's same-sample output; op2's is
            # op1's (EngineMkI.cpp:237,281,286)
            by_sample = {}
            for op, t, mod, y in calls[:64 * count]:
                by_sample.setdefault(t // count, {})[op] = (mod, y)
            for i in range(64):
                s = by_sample[i]
                self.assertEqual(s[1][0], s[0][1],
                                 f"alg {alg + 1}: op1 input != op0 output")
                if count == 3:
                    self.assertEqual(s[2][0], s[1][1],
                                     f"alg {alg + 1}: op2 input != op1 output")

    def test_gating_and_has_contents_scenarios(self):
        """Gated ops skip rendering but advance phase; empty output buses
        force add off (EngineMkI.cpp:320-322); empty input buses force the
        pure form (:324)."""
        scenarios = [
            (0, 7, (0,)),    # ALG 1: fb op gated; chain falls back to pure
            (6, 7, (0, 1)),  # ALG 7: op2's bus-add must be forced off
            (15, 7, (2,)),   # ALG 16: bus2 writer gated -> pure add onto b1
            (3, 7, (0,)),    # ALG 4: serial skipped, generic per-op path
            (5, 7, (0,)),    # ALG 6: serial skipped, generic per-op path
            (31, 7, (0,)),   # ALG 32: gated add carrier
        ]
        for alg, fb, gated in scenarios:
            with self.subTest(algorithm=alg + 1, feedback=fb, gated=gated):
                self.assertTrue(scripted_case_ok(alg, fb, gated=gated))

    def test_gated_op_silent_but_phase_advanced(self):
        router = AlgorithmRouter(None, None)
        calls, frames, _, params = drive_router(31, 7, 2, gated=(0,),
                                                router=router)
        self.assertFalse(any(c[0] == 0 for c in calls),
                         "gated op must not evaluate")
        self.assertEqual(params[0].phase, (1000 << 6) * 2)
        for k in range(1, 6):
            self.assertTrue(any(c[0] == k for c in calls))


class TestFeedbackSemantics(unittest.TestCase):
    """Feedback history depth, shift mapping, +2 rules, phase skips."""

    def test_fb_shift_mapping(self):
        """fb_shift = feedback != 0 ? 8 - feedback : 16 (dx7note.cc:197)."""
        self.assertEqual([fb_shift_for(f) for f in range(8)],
                         [16, 7, 6, 5, 4, 3, 2, 1])

    def test_history_depth_two_direct_reads(self):
        """Inject a one-frame impulse into the history (fb_buf seed) and
        track its direct reads: with a zero-output source the impulse
        appears in exactly the first two samples' feedback terms — the
        two-element history depth (dx7note.h:77; EngineMkI.cpp:191-204)."""
        router = AlgorithmRouter(None, None)
        params = [FmParams() for _ in range(6)]
        for k, p in enumerate(params):
            p.freq = 1000 * (k + 1)
            p.level_in = PARKED_LEVEL
        fb_buf = [0, 1 << 20]  # impulse K in the y slot only
        observed = []

        def zero_source(op, i, phase_plus, gain, mod):
            observed.append((op, mod))
            return 0

        router.render([0] * 64, params, 31, fb_buf,
                      fb_shift_for(7), sin_fn=zero_source)
        kshift = min(fb_shift_for(7) + 2, 16)  # ALGO 32: +2 rule
        fb_reads = [mod for op, mod in observed if op == 0]
        self.assertEqual(fb_reads[0], (1 << 20) >> (kshift + 1))
        self.assertEqual(fb_reads[1], (1 << 20) >> (kshift + 1))
        self.assertEqual(fb_reads[2:], [0] * 62)  # depth exhausted
        self.assertTrue(all(mod == 0 for op, mod in observed if op != 0))
        self.assertEqual(fb_buf, [0, 0])

    def test_recirculation_values(self):
        """With the scripted source the recirculated feedback terms follow
        the pinned recursion exactly (recomputed by hand here): the impulse
        in the y slot recirculates through y0/y1 with the (+2) shift of
        ALGO 32 and decays through the scripted outputs."""
        router = AlgorithmRouter(None, None)
        params = [FmParams() for _ in range(6)]
        for k, p in enumerate(params):
            p.freq = 1000 * (k + 1)
            p.level_in = PARKED_LEVEL
        K = 1 << 20
        s = min(fb_shift_for(7) + 2, 16) + 1

        def w32(v):
            v &= 0xFFFFFFFF
            return v - (1 << 32) if v >= (1 << 31) else v

        expected_mods = []
        y_prev2, y_prev1 = 0, K
        for i in range(64):
            mod = w32(y_prev2 + y_prev1) >> s
            expected_mods.append(mod)
            y = scripted_y(0, i, mod)
            y_prev2, y_prev1 = y_prev1, y
        calls, _, _, _ = drive_router(31, 7, 1, fb_buf=[0, K])
        fb_calls = [c for c in calls if c[0] == 0]
        self.assertEqual([c[2] for c in fb_calls], expected_mods)
        self.assertEqual([c[3] for c in fb_calls],
                         [scripted_y(0, i, m)
                          for i, m in enumerate(expected_mods)])

    def test_plus2_shift_and_serial_prefixes(self):
        """ALGO 32 (EngineMkI.cpp:343) and the ALGO 4/6 serial forms
        (:330,337) use min(feedback_shift+2, 16); other fb ops use the
        plain shift (:347). The fb op's first feedback term from a seeded
        history exposes the effective shift."""
        cases = (
            (31, 7, 0, False, min(fb_shift_for(7) + 2, 16)),
            (31, 1, 0, False, min(fb_shift_for(1) + 2, 16)),
            (0, 7, 0, False, fb_shift_for(7)),
            (6, 7, 0, False, fb_shift_for(7)),
            (7, 7, 2, False, fb_shift_for(7)),
            (3, 7, 0, True, min(fb_shift_for(7) + 2, 16)),
            (5, 7, 0, True, min(fb_shift_for(7) + 2, 16)),
        )
        for alg, fb, fb_pos, serial, want_shift in cases:
            with self.subTest(algorithm=alg + 1, feedback=fb):
                router = AlgorithmRouter(None, None)
                params = [FmParams() for _ in range(6)]
                for k, p in enumerate(params):
                    p.freq = 1000 * (k + 1)
                    p.level_in = PARKED_LEVEL
                fb_buf = [0, 256]
                seen = []

                def rec(op, i, phase_plus, gain, mod):
                    seen.append((op, mod))
                    return 0

                router.render([0] * 64, params, alg, fb_buf,
                              fb_shift_for(fb), sin_fn=rec)
                first = next(entry for entry in seen if entry[0] == fb_pos)
                self.assertEqual(first[1], 256 >> (want_shift + 1))
                if serial:
                    count = SERIAL_OPS[alg]
                    self.assertEqual([op for op, _ in seen[:count]],
                                     list(range(count)))

    def test_serial_consumed_ops_advance_one_frame(self):
        """EngineMkI.cpp:331-332,338 + :361: every operator — including the
        serial-consumed ones — advances exactly freq << LG_N per render."""
        for alg in (3, 5):
            _, _, _, params = drive_router(alg, 7, 4)
            for k, p in enumerate(params):
                self.assertEqual(p.phase, ((1000 * (k + 1)) << 6) * 4,
                                 f"alg {alg + 1} param {k}")

    def test_fb_off_flags_take_pure_form(self):
        """fb_on = feedback_shift < 16 (EngineMkI.cpp:299): with feedback 0
        the F-flagged ops render pure (no history reads at all)."""
        router = AlgorithmRouter(None, None)
        params = [FmParams() for _ in range(6)]
        for k, p in enumerate(params):
            p.freq = 1000 * (k + 1)
            p.level_in = PARKED_LEVEL
        fb_buf = [12345, -6789]  # would poison the fb terms if read
        seen = []

        def rec(op, i, phase_plus, gain, mod):
            seen.append(mod)
            return 0

        router.render([0] * 64, params, 31, fb_buf, 16, sin_fn=rec)
        self.assertEqual(len(seen), 6 * 64)  # all six carriers evaluate
        self.assertTrue(all(mod == 0 for mod in seen))
        self.assertEqual(fb_buf, [12345, -6789])  # untouched


class TestNegativeControls(unittest.TestCase):
    """Each control must demonstrably fail the check it targets."""

    def test_controls_are_sensitive_to_real_defects(self):
        """Sanity: the scripted battery passes on the clean router, so any
        mutant failure below is attributable to the injected defect."""
        for alg in range(32):
            self.assertTrue(scripted_case_ok(alg, 7))

    def test_control_operator_index_permutation_fails(self):
        """Permuted operator indices in the router must fail the scripted
        battery (AGENTS.md negative-control list; issue #18). Distinct
        per-position envelope levels make the gate state position-dependent,
        so a permutation moves the gate to the wrong operators."""
        perm = [5, 4, 3, 2, 1, 0]

        class Permuted(list):
            def __getitem__(self, i):
                return list.__getitem__(self, perm[i % 6])

        def make_params():
            params = [FmParams() for _ in range(6)]
            for k, p in enumerate(params):
                p.freq = 1000 * (k + 1)
                p.level_in = GATED_LEVEL if k in (0, 3) else PARKED_LEVEL
            return params

        # clean run: the battery with these gates must pass for all 32
        for alg in range(32):
            self.assertTrue(scripted_case_ok(alg, 7, gated=(0, 3)),
                            f"clean gated battery failed at alg {alg + 1}")

        failures = []
        for alg in range(32):
            params = make_params()
            calls, frames, _, _ = drive_router(
                alg, 7, gated=(0, 3),
                router=AlgorithmRouter(None, None),
                params=Permuted(params))
            exp_calls, exp_frames = expected_run(alg, 7, gated=(0, 3))[:2]
            if calls != exp_calls or frames != exp_frames:
                failures.append(alg)
        self.assertGreaterEqual(len(failures), 28,
                                f"operator permutation not caught broadly: "
                                f"only {failures} failed")
        self.assertIn(31, failures)  # ALGO 32: pure carriers, op identity
        self.assertIn(0, failures)   # ALGO 1: serial chain identity

    def test_control_one_block_delayed_feedback_history_fails(self):
        """Delaying the fb history reads by one 64-sample block (a
        one-sample-broken history at block granularity) must fail the
        feedback checks."""
        class DelayedFbBuf:
            def __init__(self):
                self._vis = [0, 0]
                self._pending = None
                self._s0 = 0

            def __getitem__(self, k):
                return self._vis[k]

            def __setitem__(self, k, v):
                if k == 0:
                    self._s0 = v
                else:
                    if self._pending is not None:
                        self._vis = self._pending
                    self._pending = [self._s0, v]

        failures = []
        for alg in (0, 1, 3, 5, 7, 8, 27, 31):
            buf = DelayedFbBuf()
            calls, frames, _, _ = drive_router(alg, 7, 3, fb_buf=buf)
            exp_calls, exp_frames, _ = expected_run(alg, 7, 3)
            if calls != exp_calls or frames != exp_frames:
                failures.append(alg)
        self.assertEqual(len(failures), 8,
                         "one-block-delayed feedback history not caught: "
                         f"only {failures} failed")

    def test_control_carrier_sum_last_writer_wins_fails(self):
        """Corrupting the carrier-sum accumulation to last-writer-wins (the
        observable corruption of sum-order semantics; pure reordering of the
        final integer sum is unobservable modulo 2^32) must fail every
        multi-carrier algorithm."""

        class LastWriterRouter(AlgorithmRouter):
            @staticmethod
            def _mix(out, i, y, add):
                out[i] = y

        multi = [0, 1, 2, 4, 6, 15, 22, 24, 31]  # >= 2 output carriers
        failures = []
        for alg in multi:
            ok = scripted_case_ok(alg, 7, router=LastWriterRouter(None, None))
            if not ok:
                failures.append(alg)
        self.assertEqual(failures, multi,
                         "carrier-sum corruption not caught exactly")


# ---------------------------------------------------------------------------
# 3. Reference sweep vs the pinned oracle (guarded)
#------------------------------------------------------------------------------

SWEEP_RESULTS = {
    "schema": "gf180-dx7/n04-algorithm-sweep-v1",
    "issue": "https://github.com/2AMLogic/gf180-dx7/issues/18",
    "model_contract": "gf180-dx7-integer-v1",
    "comparator": "src/gf180_dx7/compare.py exact-time-locked-paired v1 "
                  "(no alignment/normalization)",
    "declared_budget": "exact (byte-identical PCM, max_abs_error == 0.0) "
                       "per case: every integer operation mirrored over "
                       "byte-identical tables (N02); a nonzero error is a "
                       "model defect, not a numeric budget",
    "claim": "the N04 integer router reproduces the pinned oracle's "
             "algorithm routing, carrier sums, gating and feedback "
             "semantics exactly on every rendered case; software-reference "
             "agreement only, not original-DX7 fidelity",
    "voice": "synthetic six-operator voice: rates/levels 99 (parked "
             "constant envelopes), osc coarse 1/fine 0/detune 7, neutral "
             "LFO/pitch-EG/mod bytes, osc_key_sync 1, note_on 60 vel 100 "
             "at sample 0; per-case algorithm/feedback/gating overrides",
    "cases": [],
    "controls": [
        {"control": "operator-index permutation in the router",
         "target": "TestScriptedRouting battery", "required": "must fail",
         "observed": "fails 30/32 algorithms (assert >= 28)"},
        {"control": "one-sample-broken feedback history (reads delayed by "
                    "one 64-sample block)",
         "target": "TestFeedbackSemantics + scripted fb cases",
         "required": "must fail", "observed": "fails all 8 probed cases"},
        {"control": "carrier-sum corruption (last-writer-wins)",
         "target": "multi-carrier algorithms",
         "required": "must fail", "observed": "fails all 9 probed cases"},
    ],
    "renders": [],
}


@unittest.skipUnless(ORACLE is not None, ORACLE_ABSENT)
class TestReferenceSweep(unittest.TestCase):
    """Pinned oracle vs model, dry f32le, exact, per algorithm."""

    @classmethod
    def setUpClass(cls):
        cls.renderer = OracleRenderer(ORACLE)

    @classmethod
    def tearDownClass(cls):
        SWEEP_RESULTS["renders"] = cls.renderer.runs
        cls.renderer.close()

    def _check_case(self, name, algorithm, feedback, events=None,
                    seconds=SWEEP_SECONDS, **voice_kw):
        events = events or "0 note_on 60 100\n"
        voice = sweep_voice(algorithm, feedback, **voice_kw)
        oracle_pcm = self.renderer.render(voice, events, seconds)
        model_pcm = model_render(voice, events, seconds)
        report = compare.compare_streams(oracle_pcm, model_pcm)
        row = {
            "case": name,
            "algorithm_1based": algorithm + 1,
            "feedback": feedback,
            "seconds": seconds,
            "events": events.strip(),
            "voice_syx_sha256": hashlib.sha256(
                sysex.encode_voice(voice)).hexdigest(),
            "oracle_render_sha256": hashlib.sha256(oracle_pcm).hexdigest(),
            "model_render_sha256": hashlib.sha256(model_pcm).hexdigest(),
            "byte_identical": report.byte_identical,
            "max_abs_error": (report.primary or {}).get("max_abs_error"),
            "mismatch_count": (report.primary or {}).get("mismatch_count"),
            "first_divergent_sample":
                (report.primary or {}).get("first_divergent_sample"),
            "verdict": report.verdict,
        }
        SWEEP_RESULTS["cases"].append(row)
        self.assertEqual(report.verdict, "PASS",
                         f"{name}: verdict {report.verdict} "
                         f"({report.verdict_reason})")
        self.assertTrue(report.byte_identical, f"{name}: not byte-identical")
        self.assertEqual((report.primary or {}).get("max_abs_error"), 0.0,
                         f"{name}: nonzero error")
        return report

    def test_all_32_algorithms_exact(self):
        for alg in range(32):
            with self.subTest(algorithm=alg + 1):
                self._check_case(f"alg{alg + 1}_fb7", alg, 7)

    def test_algorithm_4_feedback_sweep(self):
        """Directed: ALGO 4 serial fb form at fb 0 (fb-off generic/quirk
        path), 3, and 7 (already covered by the sweep)."""
        self._check_case("alg4_fb0", 3, 0)
        self._check_case("alg4_fb3", 3, 3)

    def test_algorithm_6_feedback_sweep(self):
        self._check_case("alg6_fb0", 5, 0)
        self._check_case("alg6_fb3", 5, 3)

    def test_feedback_off_generic_fb_ops(self):
        """fb_on = shift < 16: F-flagged ops must take the pure form with
        feedback 0 (mid-chain op for ALG 8; the output op for ALG 32)."""
        self._check_case("alg8_fb0", 7, 0)
        self._check_case("alg32_fb0", 31, 0)

    def test_gating_has_contents_cases(self):
        """Envelope-silent modulators exercise the gate + has_contents
        fallbacks against the oracle (TL 0 -> clamp floor -> gate)."""
        self._check_case("alg1_gate_op0", 0, 7, gated_params=(0,))
        self._check_case("alg16_gate_op2", 15, 7, gated_params=(2,))

    def test_retrigger_fresh_slot_shared_engine(self):
        """note_off -> note_on: the oracle claims a fresh Dx7Note slot
        (constructor phase/gain_out/fb_buf zeros) while the engine's buses
        persist; the released first note parks at the 16<<16 floor and
        contributes exactly zero (NUM-012)."""
        events = ("0 note_on 60 100\n48000 note_off 60\n"
                  "96000 note_on 60 100\n")
        self._check_case("alg32_retrigger", 31, 7, events=events,
                         seconds=3.0, release0=True)

    def test_release_through_serial_feedback(self):
        """ALGO 6 fb 7: audible release through the two-op serial feedback
        form (note_off 1 s into a 2 s render)."""
        events = "0 note_on 60 100\n48000 note_off 60\n"
        self._check_case("alg6_release", 5, 7, events=events)


def _write_evidence():
    if not SWEEP_RESULTS["cases"]:
        return
    EVIDENCE_PATH.parent.mkdir(parents=True, exist_ok=True)
    EVIDENCE_PATH.write_text(
        json.dumps(SWEEP_RESULTS, indent=2, sort_keys=False) + "\n",
        encoding="utf-8")


def tearDownModule():
    """Persist the sweep evidence once, after all oracle cases ran (no
    write when the oracle is absent — a skipped suite records nothing)."""
    _write_evidence()


if __name__ == "__main__":
    unittest.main()
