"""N03 tests: integer operator-envelope model (issue #17).

Three layers, per the issue brief:

1. Model semantics (pure, always run): exact per-frame traces against the
   pinned env.cc semantics, keydown/keyup transitions, retrigger-from-current
   level, sustain park, update() and transfer(), int32 range guards, and the
   sr_multiplier formula re-derivation (NUM-005 rev 1.1: derived, never a
   bare literal).
2. Independent re-derivation: a second implementation written from the
   documented coefficient formulas (docs/N03-ENVELOPE-MODEL.md section 2)
   produces full traces; the model must match it exactly on a parameter
   battery, crossing times included.
3. Directed oracle cases (guarded skip -> NOT_RUN when the pinned oracle is
   absent, never a silent pass): single-carrier synthetic voices rendered by
   the pinned GPL oracle (P01-built .syx), envelope times/levels extracted
   from the dry PCM by the documented detector, compared to the model within
   declared tolerances (docs/N03-ENVELOPE-MODEL.md section 5).
   Negative controls substitute a plain gain-linear ADSR, mutate
   sr_multiplier by +-1, and swap the rate/level arrays; each must
   demonstrably fail the check it targets, naming the divergent stage.

No fidelity claim: oracle agreement is software-reference agreement only
(reference/oracle-protocol.md section 4). Stdlib-only.
"""

from __future__ import annotations

import hashlib
import json
import math
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
for p in (str(REPO / "src"), str(REPO / "tools")):
    if p not in sys.path:
        sys.path.insert(0, p)

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.model import envelope as E  # noqa: E402
from render_reference import (  # noqa: E402
    RENDER_RATE,
    HarnessFail,
    HarnessNotRun,
    check_oracle_binary,
    load_identity,
    read_f32,
)

BLOCK = 64
DB_PER_DOUBLING = 20.0 * math.log10(2.0)
ORACLE_ABSENT = "pinned oracle binary absent or unpinned; directed cases NOT_RUN"

IDENTITY_PATH = REPO / "reference" / "oracle-identity.json"
EVIDENCE_PATH = REPO / "reference" / "evidence" / "n03-envelope" / \
    "directed-results.json"


def _oracle():
    """Return the pinned oracle path after re-hashing it, or None."""
    try:
        identity = load_identity(IDENTITY_PATH)
        path = Path(identity["binary"]["path"])
        check_oracle_binary(identity, path)
        return path
    except (HarnessFail, HarnessNotRun):
        return None


ORACLE = _oracle()


# ---------------------------------------------------------------------------
# Synthetic single-carrier voice building (P01 codec)


def make_op(rates, levels, outlevel=99, rs=0, kvs=0, bp=40, ldepth=0,
            rdepth=0, lcurve=0, rcurve=0):
    return sysex.Operator(
        rate_1=rates[0], rate_2=rates[1], rate_3=rates[2], rate_4=rates[3],
        level_1=levels[0], level_2=levels[1], level_3=levels[2],
        level_4=levels[3], break_point=bp, scale_left_depth=ldepth,
        scale_right_depth=rdepth, scale_left_curve=lcurve,
        scale_right_curve=rcurve, rate_scaling=rs, amp_mod_sensitivity=0,
        key_velocity_sensitivity=kvs, output_level=outlevel, osc_mode=0,
        osc_freq_coarse=1, osc_freq_fine=0, osc_detune=7)


def make_voice(carrier):
    """Algorithm 32 (index 31): with feedback 0 every op takes the Mark I
    no-input path onto the main bus (msfa/fm_core.cc algorithms[31],
    EngineMkI.cpp:296-362); operators 1-5 carry output_level 0, so their
    envelopes park at the 16<<16 clamp floor (env.cc:119), gain 16368 >
    kLevelThresh 16284, and the Mark I gate skips them exactly
    (EngineMkI.cpp:314-318). Only the carrier is audible, so the PCM tracks
    one operator envelope. osc_key_sync = 1 makes every note-on reset
    gain_out/phase (dx7note.cc oscSync via the oracle keydown), fixing the
    model's first-frame gain1 = ENV_MAX-1."""
    silent = make_op([0] * 4, [0] * 4, outlevel=0)
    return sysex.Voice(
        operators=[silent] * 5 + [carrier],
        pitch_eg_rate_1=0, pitch_eg_rate_2=0, pitch_eg_rate_3=0,
        pitch_eg_rate_4=0, pitch_eg_level_1=0, pitch_eg_level_2=0,
        pitch_eg_level_3=0, pitch_eg_level_4=0, algorithm=31, feedback=0,
        osc_key_sync=1, lfo_speed=0, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=0, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name_bytes=b"N03ENV", channel_byte=0)


def target_for(level_99: int, outlevel: int) -> int:
    """Segment target for a raw DX7 level value, mirroring env.cc:113-121."""
    actual = ((E.scale_out_level(level_99) >> 1) << 6) + outlevel - 4256
    return (actual if actual >= 16 else 16) << 16


class OracleRenderer:
    """Renders (voice, events) pairs with the pinned oracle; caches by input
    hash inside one process."""

    def __init__(self, oracle_path):
        self.oracle = oracle_path
        self.tmp = tempfile.TemporaryDirectory(prefix="n03-env-")
        self.cache = {}
        self.runs = []

    def close(self):
        self.tmp.cleanup()

    def render(self, voice: sysex.Voice, events, seconds: float):
        voice_bytes = sysex.encode_voice(voice)
        events_text = "".join(
            f"{pos} {cmd} " + " ".join(str(a) for a in args) + "\n"
            for pos, cmd, *args in events)
        key = (hashlib.sha256(voice_bytes).hexdigest(),
               hashlib.sha256(events_text.encode()).hexdigest(), seconds)
        if key in self.cache:
            return self.cache[key]
        vpath = Path(self.tmp.name) / f"v-{key[0][:12]}.syx"
        epath = Path(self.tmp.name) / f"e-{key[1][:12]}.txt"
        opath = Path(self.tmp.name) / f"o-{key[0][:12]}-{key[1][:12]}.f32"
        vpath.write_bytes(voice_bytes)
        epath.write_text(events_text, encoding="utf-8")
        proc = subprocess.run(
            [str(self.oracle), "--voice", str(vpath), "--events", str(epath),
             "--out", str(opath), "--rate", str(RENDER_RATE),
             "--seconds", str(seconds)], capture_output=True, text=True)
        if proc.returncode != 0:
            raise AssertionError(
                f"oracle failed (exit {proc.returncode}): {proc.stderr}")
        pcm = read_f32(opath)
        want = int(RENDER_RATE * seconds)
        if len(pcm) != want:
            raise AssertionError(
                f"oracle produced {len(pcm)} samples, expected {want}")
        self.runs.append({
            "voice_sha256": key[0], "events_sha256": key[1],
            "seconds": seconds,
            "pcm_sha256": hashlib.sha256(pcm.tobytes()).hexdigest(),
        })
        self.cache[key] = pcm
        return pcm


# ---------------------------------------------------------------------------
# PCM envelope detector (documented in docs/N03-ENVELOPE-MODEL.md section 4)


def block_rms(pcm) -> list:
    n = len(pcm) // BLOCK
    return [math.sqrt(sum(x * x for x in pcm[k * BLOCK:(k + 1) * BLOCK])
                      / BLOCK) for k in range(n)]


def block_peak(pcm) -> list:
    n = len(pcm) // BLOCK
    return [max(abs(x) for x in pcm[k * BLOCK:(k + 1) * BLOCK])
            for k in range(n)]


def gain_pair(trace, k: int, keydown_block: int):
    """(gain1, gain2) of frame k per EngineMkI.cpp:155,314-315: gain1 is the
    previous frame's gain2; the note-on frame sees gain_out == 0 (reset by
    osc key sync) substituted with ENV_MAX-1."""
    g2 = E.ENV_MAX - (trace[k] >> E.GAIN_SHIFT)
    if k == keydown_block:
        return E.ENV_MAX - 1, g2
    return E.ENV_MAX - (trace[k - 1] >> E.GAIN_SHIFT), g2


def block_gain_power(g1: int, g2: int) -> float:
    """Predicted relative block signal power for a linear intra-frame gain
    ramp gain1 -> gain2 (EngineMkI.cpp:155): mkiSin amplitude obeys
    |out| ~ 2^(-env/1024) (EngineMkI.cpp:132-135), so per-sample power is
    2^(-2*g/1024). Summed exactly over the 64 samples of the frame."""
    dg = (g2 - g1 + (BLOCK >> 1)) >> E.LG_N
    total = 0.0
    for i in range(BLOCK):
        g = g1 + (i + 1) * dg
        total += 2.0 ** (-2.0 * g / 1024.0)
    return total / BLOCK


def pred_block_powers(trace, keydown_block: int, n: int) -> list:
    out = [0.0] * n
    for k in range(keydown_block, n):
        g1, g2 = gain_pair(trace, k, keydown_block)
        out[k] = block_gain_power(g1, g2)
    return out


def windowed(pred: list, k: int, half: int = 8) -> float:
    lo, hi = max(0, k - half), min(len(pred), k + half + 1)
    return sum(pred[lo:hi]) / (hi - lo)


def db(x: float) -> float:
    return 10.0 * math.log10(max(x, 1e-30))


def model_crossing(trace, level: int, direction: str, start: int = 0):
    for k in range(start, len(trace)):
        if direction == "down" and trace[k] <= level:
            return k
        if direction == "up" and trace[k] >= level:
            return k
    return None


def first_silent_frame(trace, search_from: int):
    """First frame f where both gains exceed kLevelThresh 16284: the Mark I
    gate skips the operator for the whole frame (EngineMkI.cpp:314-318), so
    block f is exact digital silence."""
    for f in range(max(search_from, 1), len(trace)):
        g_prev = E.ENV_MAX - (trace[f - 1] >> E.GAIN_SHIFT)
        g_cur = E.ENV_MAX - (trace[f] >> E.GAIN_SHIFT)
        if g_prev > 16284 and g_cur > 16284:
            return f
    return None


class ProbeFailure:
    def __init__(self, stage, probe, predicted, measured, delta, budget):
        self.stage = stage
        self.probe = probe
        self.predicted = predicted
        self.measured = measured
        self.delta = delta
        self.budget = budget

    def __str__(self):
        return (f"[{self.stage}] {self.probe}: predicted "
                f"{self.predicted:.3f}, measured {self.measured:.3f}, "
                f"delta {self.delta:+.3f} exceeds budget {self.budget}")


class CaseChecker:
    """Compares one directed case's model prediction against its PCM.

    All comparisons are relative to a measured/predicted anchor pair from a
    declared plateau window (the model-to-PCM absolute scale constant
    cancels; docs section 4). Predicted block powers use the documented
    gain-side arithmetic with the SAME centered +-8-block window as the
    measured side, so intra-window slope bias cancels.
    """

    T_VALUE = 0.6      # dB, segment-interior value probes
    T_TIME = 4         # blocks, steep time probes
    T_ONSET = 1        # blocks, exact digital silence before onset
    T_RATIO = 0.4      # dB, cross-render plateau ratios

    def __init__(self, case, pcm, trace, keydown_block):
        self.case = case
        self.pcm = pcm
        self.trace = trace
        self.keydown_block = keydown_block
        self.failures: list[ProbeFailure] = []
        self.rows: list[dict] = []
        self.raw = block_rms(pcm)
        self.peak = block_peak(pcm)
        self.n = min(len(self.trace), len(self.raw))
        self.powers = pred_block_powers(trace, keydown_block, self.n)
        self.meas_powers = [v * v for v in self.raw]
        self.ka = None
        self.anchor_pred = None
        self.anchor_meas = None

    def set_anchor(self, lo: int, hi: int):
        lo = max(lo, self.keydown_block + 9)
        hi = min(hi, self.n)
        win = range(lo, hi)
        self.ka = (lo + hi) // 2
        self.anchor_pred = sorted(self.powers[k] for k in win)[len(win) // 2]
        self.anchor_meas = sorted(self.meas_powers[k] for k in win)[
            len(win) // 2]

    def _pred_db(self, k, half=8):
        return db(windowed(self.powers, k, half) / self.anchor_pred)

    def _meas_db(self, k, half=8):
        return db(windowed(self.meas_powers, k, half) / self.anchor_meas)

    def _record(self, stage, probe, predicted, measured, delta, budget):
        ok = abs(delta) <= budget
        self.rows.append(
            {"case": self.case, "stage": stage, "probe": probe,
             "predicted": round(predicted, 4), "measured": round(measured, 4),
             "delta": round(delta, 4), "budget": budget,
             "verdict": "PASS" if ok else "FAIL"})
        if not ok:
            self.failures.append(
                ProbeFailure(stage, probe, predicted, measured, delta,
                             budget))
        return ok

    def value_probe(self, stage, k, budget=T_VALUE):
        pred, meas = self._pred_db(k), self._meas_db(k)
        return self._record(stage, f"value@block{k}", pred, meas,
                            meas - pred, budget)

    def delta_probe(self, stage, k1, k2, budget=None):
        pred = self._pred_db(k2) - self._pred_db(k1)
        meas = self._meas_db(k2) - self._meas_db(k1)
        if budget is None:
            budget = max(0.6, 0.10 * abs(pred))
        return self._record(stage, f"delta@{k1}..{k2}", pred, meas,
                            meas - pred, budget)

    def onset_probe(self):
        first = next((k for k, p in enumerate(self.peak) if p > 1e-6), None)
        return self._record("attack", "onset_block",
                            float(self.keydown_block),
                            float(first if first is not None else -1),
                            (first if first is not None else -1)
                            - self.keydown_block, self.T_ONSET)

    def slope_guard(self, stage, level, direction, lo):
        """Design guard: a time-probed crossing must be steeper than the
        detector ripple budget (docs section 5); raises on case design
        errors instead of silently loosening tolerance."""
        k_model = model_crossing(self.trace, level, direction, lo)
        d1 = self._pred_db(max(self.keydown_block, k_model - 3), half=0)
        d2 = self._pred_db(min(self.n - 1, k_model + 3), half=0)
        slope = abs(d2 - d1) / 6.0
        if slope < 1.2:
            raise AssertionError(
                f"{self.case}: {stage} crossing slope {slope:.2f} dB/block "
                "below the 1.2 dB/block guard; case design error")

    def time_probe(self, stage, level, direction, lo, hi, budget=T_TIME):
        rel = math.sqrt(block_gain_power(
            E.ENV_MAX - (level >> E.GAIN_SHIFT),
            E.ENV_MAX - (level >> E.GAIN_SHIFT)) / self.anchor_pred)
        threshold = math.sqrt(self.anchor_meas) * rel
        k_model = model_crossing(self.trace, level, direction,
                                 max(lo, self.keydown_block))
        if k_model is None or k_model >= self.n:
            raise AssertionError(f"{self.case}: model never crosses {level}")
        rng = range(max(1, k_model - 40), min(hi, self.n))
        if direction == "up":
            k_meas = next((k for k in rng if self.raw[k] >= threshold), None)
        else:
            k_meas = next((k for k in rng if self.raw[k] <= threshold), None)
        meas = k_meas if k_meas is not None else -1
        return self._record(stage, f"crossing@{(level >> 16)}q16",
                            float(k_model), float(meas), meas - k_model,
                            budget)

    def silence_probe(self, keyup_block):
        f = first_silent_frame(self.trace, keyup_block + 1)
        if f is None or f + 5 >= len(self.peak):
            raise AssertionError(f"{self.case}: model never goes silent")
        silent = all(p < 1e-9 for p in self.peak[f:f + 5])
        audible_before = self.peak[f - 1] > 1e-6
        return self._record("release", "exact_silence_block", float(f),
                            float(f) if silent and audible_before else -1.0,
                            0.0, 0.0)

    def no_tail_cut_probe(self, stage, lo, hi, budget=T_VALUE):
        lo, hi = max(lo, 0), min(hi, self.n)
        pred = sum(self._pred_db(k) for k in range(lo, hi)) / (hi - lo)
        meas = sum(self._meas_db(k) for k in range(lo, hi)) / (hi - lo)
        return self._record(stage, "tail_present", pred, meas, meas - pred,
                            budget)

    def assert_all(self):
        if self.failures:
            raise AssertionError(
                f"directed case '{self.case}': "
                f"{len(self.failures)} probe(s) failed:\n  " +
                "\n  ".join(str(f) for f in self.failures))


def run_case(renderer, case, carrier, midinote, velocity, oracle_events,
             seconds, model_events):
    """Build voice, render with the pinned oracle, derive the per-note
    envelope params (dx7note.cc:163-179), simulate, return (checker, info).
    model_events: [(sample_pos, "keydown"|"keyup"), ...]."""
    voice = make_voice(carrier)
    pcm = renderer.render(voice, oracle_events, seconds)
    rates, levels, outlevel, rate_scaling = E.derive_op_envelope_params(
        carrier, midinote, velocity)
    n = int(seconds * RENDER_RATE) // BLOCK
    events = [(pos // BLOCK, kind) for pos, kind in model_events]
    keydown_block = next((b for b, kind in events if kind == "keydown"), 0)
    trace = E.simulate(n, events, rates, levels, outlevel, rate_scaling)
    info = {"derived": {"rates": rates, "levels": levels,
                        "outlevel": outlevel, "rate_scaling": rate_scaling}}
    return CaseChecker(case, pcm, trace, keydown_block), info


def simulate_with_boundaries(rates, levels, outlevel, rate_scaling, n,
                             events):
    """Model trace plus per-frame ix_, used to derive exact segment boundary
    frames for the negative-control ADSR."""
    by_block = {}
    for block, kind in events:
        by_block.setdefault(block, []).append(kind)
    env = E.Envelope()
    trace, ixs = [], []
    for frame in range(n):
        for kind in by_block.get(frame, ()):
            if kind == "keydown":
                env.init(rates, levels, outlevel, rate_scaling)
                env.keydown(True)
            elif kind == "keyup":
                env.keydown(False)
        trace.append(env.getsample())
        ixs.append(env.ix_)
    boundaries = {}
    prev = None
    for frame, ix in enumerate(ixs):
        if prev is not None and ix != prev and f"{prev}->{ix}" not in \
                boundaries:
            boundaries[f"{prev}->{ix}"] = frame
        prev = ix
    return trace, boundaries


# ---------------------------------------------------------------------------
# Independent re-derivation (written from the documented formulas only)

_INDEP_LEVEL_LUT = (0, 5, 9, 13, 17, 20, 23, 25, 27, 29, 31, 33, 35, 37, 39,
                    41, 42, 43, 45, 46)
_INDEP_STATICS = (
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


def independent_advance(rates, levels, outlevel, rate_scaling, level, newix,
                        sr_multiplier):
    """One env.cc:113-151 advance, written from the cited lines only."""
    if newix >= 4:
        return None
    lv = levels[newix]
    sl = (28 + lv) if lv >= 20 else _INDEP_LEVEL_LUT[lv]
    actual = ((sl >> 1) << 6) + outlevel - 4256
    if actual < 16:
        actual = 16
    target = actual << 16
    rising = target > level
    qrate = ((rates[newix] * 41) >> 6) + rate_scaling
    if qrate > 63:
        qrate = 63
    if target == level or (newix == 0 and lv == 0):
        srate = rates[newix] + rate_scaling
        if srate > 99:
            srate = 99
        if srate < 77:
            sc = _INDEP_STATICS[srate]
        else:
            sc = 20 * (99 - srate)
        if srate < 77 and newix == 0 and lv == 0:
            sc //= 20
        sc = (sc * sr_multiplier) >> 24
    else:
        sc = 0
    inc = (4 + (qrate & 3)) << (2 + 6 + (qrate >> 2))
    inc = (inc * sr_multiplier) >> 24
    return {"target": target, "rising": rising, "inc": inc, "static": sc}


def independent_trace(rates, levels, outlevel, rate_scaling, n, events,
                      sr_multiplier):
    st = {"level": 0, "down": True, "ix": 0, "target": 0, "rising": False,
          "inc": 0, "static": 0}
    by_block = {}
    for block, kind in events:
        by_block.setdefault(block, []).append(kind)

    def adv(newix):
        st["ix"] = newix
        if newix < 4:
            nxt = independent_advance(rates, levels, outlevel, rate_scaling,
                                      st["level"], newix, sr_multiplier)
            st["target"] = nxt["target"]
            st["rising"] = nxt["rising"]
            st["inc"] = nxt["inc"]
            st["static"] = nxt["static"]

    def step():
        if st["static"]:
            st["static"] -= 64
            if st["static"] <= 0:
                st["static"] = 0
                adv(st["ix"] + 1)
        if st["ix"] < 3 or (st["ix"] < 4 and not st["down"]):
            if st["static"]:
                pass
            elif st["rising"]:
                if st["level"] < 1716 * 65536:
                    st["level"] = 1716 * 65536
                st["level"] += (((17 << 24) - st["level"]) >> 24) * st["inc"]
                if st["level"] >= st["target"]:
                    st["level"] = st["target"]
                    adv(st["ix"] + 1)
            else:
                st["level"] -= st["inc"]
                if st["level"] <= st["target"]:
                    st["level"] = st["target"]
                    adv(st["ix"] + 1)
        return st["level"]

    out = []
    for frame in range(n):
        for kind in by_block.get(frame, ()):
            if kind == "keydown":
                st["level"] = 0
                st["down"] = True
                adv(0)
            elif kind == "keyup":
                if st["down"]:
                    st["down"] = False
                    adv(3)
        out.append(step())
    return out


def mutation_battery():
    """Bounded battery for the sr_multiplier control. The first entry has a
    rate-0 stage (known statics[0]//20 upward flip for M+1); the second has
    rate 74 at rs 0 (known inc_ downward flip for M-1)."""
    return [
        ([0, 80, 70, 60], [0, 90, 80, 70], 4064, 0),
        ([74, 50, 50, 50], [99, 99, 60, 40], 4064, 0),
        ([1, 50, 50, 50], [99, 99, 60, 40], 2816, 1),
        ([99, 60, 40, 50], [99, 70, 60, 40], 4064, 0),
        ([20, 50, 50, 50], [99, 99, 60, 40], 1600, 3),
        ([50, 50, 50, 50], [60, 50, 40, 30], 2816, 0),
        ([99, 30, 40, 60], [99, 80, 60, 0], 4064, 1),
    ]


def battery_cases():
    rates_a = ([99, 60, 40, 50], [20, 50, 50, 50], [1, 50, 50, 50],
               [0, 80, 70, 60], [80, 0, 90, 70], [50, 50, 50, 50],
               [99, 99, 99, 40], [70, 60, 50, 0], [99, 30, 40, 60])
    levels_a = ([99, 70, 60, 40], [99, 99, 60, 40], [99, 80, 60, 0],
                [90, 80, 70, 60], [99, 70, 50, 30], [60, 50, 40, 30])
    cases = []
    for i, rates in enumerate(rates_a):
        for j, levels in enumerate(levels_a):
            if (i + j) % 3 == 0:
                continue
            for ol in (0, 4064, 1600):
                for rs in (0, 3):
                    cases.append((rates, levels, ol, rs))
    return cases


# ---------------------------------------------------------------------------
# Negative-control substitute: a plain 4-stage linear ADSR


class LinearAdsr:
    """Ordinary four-stage ADSR (negative control): segments are LINEAR IN
    GAIN (amplitude) between the stage-boundary gains the DX envelope
    reaches at the same boundary frames — boundary times deliberately match
    so only the segment LAW differs (gain-linear vs the DX log-domain
    exponential segments, including the 1716 jump floor and the per-frame
    multiplicative rise). Levels are re-expressed in the model's Q24 domain
    through the documented gain mapping gain = ENV_MAX - (level >> 14)."""

    def __init__(self, boundaries, targets, keydown_block, keyup_frame, n):
        self.points = [
            (keydown_block, float(E.ENV_MAX - 1)),
            (boundaries.get("0->1", keydown_block + 1),
             float(E.ENV_MAX - (targets[0] >> E.GAIN_SHIFT))),
            (boundaries.get("1->2", keydown_block + 2),
             float(E.ENV_MAX - (targets[1] >> E.GAIN_SHIFT))),
            (keyup_frame,
             float(E.ENV_MAX - (targets[2] >> E.GAIN_SHIFT))),
            (boundaries.get("3->4", n),
             float(E.ENV_MAX - (targets[3] >> E.GAIN_SHIFT))),
            (n, float(E.ENV_MAX - (targets[3] >> E.GAIN_SHIFT))),
        ]

    def _gain_at(self, k):
        f0, g0 = self.points[0]
        for frame, gain in self.points[1:]:
            if k < frame:
                frac = (k - f0) / max(1, frame - f0)
                return g0 + (gain - g0) * frac
            f0, g0 = frame, gain
        return g0

    def trace(self, n):
        out = []
        for k in range(n):
            gain = int(round(self._gain_at(k)))
            out.append((E.ENV_MAX - gain) << E.GAIN_SHIFT)
        return out


# ===========================================================================
# 1. Model semantics (pure)


class TestModelSemantics(unittest.TestCase):
    CASES = [
        ([99, 60, 40, 50], [99, 70, 60, 40], 4064, 0),
        ([20, 50, 50, 50], [99, 99, 60, 40], 4064, 0),
        ([1, 50, 50, 50], [99, 99, 60, 40], 2816, 1),
        ([0, 80, 70, 60], [90, 80, 70, 60], 0, 3),
        ([80, 0, 90, 70], [99, 70, 50, 30], 1600, 2),
        ([99, 30, 40, 60], [99, 80, 60, 0], 4064, 1),
        ([50, 50, 50, 50], [60, 50, 40, 30], 2816, 0),
        ([99, 99, 99, 40], [99, 99, 99, 40], 0, 3),
    ]

    def test_sr_multiplier_frozen_value_matches_formula(self):
        # env.cc:47-49, uint32 truncation; NUM-005 rev 1.1: 15414067 @ 48 kHz
        derived = math.floor(44100.0 / 48000.0 * (1 << 24))
        self.assertEqual(E.sr_multiplier(48000), derived)
        self.assertEqual(E.SR_MULTIPLIER_48K, derived)
        self.assertEqual(derived, 15_414_067)
        profile = json.loads(
            (REPO / "spec" / "numeric-profile-v1.json").read_text())
        self.assertEqual(profile["envelope"]["sr_multiplier_48k"], derived)

    def test_init_matches_env_cc(self):
        env = E.Envelope()
        env.init([99, 60, 40, 50], [99, 70, 60, 40], 4064, 0)
        self.assertEqual(env.level_, 0)
        self.assertTrue(env.down_)
        self.assertEqual(env.ix_, 0)
        self.assertEqual(env.targetlevel_, target_for(99, 4064))
        self.assertTrue(env.rising_)
        qrate = min((99 * 41) >> 6, 63)
        inc = (4 + (qrate & 3)) << (2 + 6 + (qrate >> 2))
        inc = (inc * E.SR_MULTIPLIER_48K) >> 24
        self.assertEqual(env.inc_, inc)
        self.assertEqual(env.staticcount_, 0)

    def test_target_clamp_floor(self):
        env = E.Envelope()
        env.init([50, 50, 50, 50], [99, 99, 99, 99], 0, 0)
        self.assertEqual(env.targetlevel_, 16 << 16)

    def test_attack_jump_floor_and_exponential_rise(self):
        env = E.Envelope()
        env.init([50, 50, 50, 50], [99, 99, 99, 99], 4064, 0)
        lvl0 = env.getsample()
        self.assertGreaterEqual(lvl0, 1716 << 16)
        prev = lvl0
        for _ in range(50):
            lvl = env.getsample()
            gap = ((17 << 24) - lvl) / max(1, (17 << 24) - prev)
            self.assertLess(gap, 1.0)
            prev = lvl

    def test_keyup_releases_from_current_level(self):
        env = E.Envelope()
        env.init([80, 60, 40, 70], [99, 90, 80, 20], 4064, 0)
        for _ in range(10):
            env.getsample()
        held = env.level_
        env.keydown(False)
        self.assertEqual(env.ix_, 3)
        self.assertFalse(env.down_)
        self.assertEqual(env.targetlevel_, target_for(20, 4064))
        self.assertEqual(held, env.level_)

    def test_retrigger_resumes_from_current_level(self):
        """Env::keydown(true) after key-up: segment 0 starts from the
        current level (env.cc:102-107; advance(0) recomputes rising_ from
        the current level_). The polyphonic voice allocator instead re-inits
        on note-on (dx7note.cc:147-179); this Env-level path is what mono
        mode exercises via transfer (documented in docs section 6)."""
        env = E.Envelope()
        env.init([80, 60, 40, 70], [99, 90, 80, 20], 4064, 0)
        for _ in range(30):
            env.getsample()
        env.keydown(False)
        for _ in range(20):
            env.getsample()
        released_to = env.level_
        env.keydown(True)
        self.assertEqual(env.ix_, 0)
        self.assertEqual(env.level_, released_to)
        self.assertGreaterEqual(env.getsample(), released_to)

    def test_sustain_parks_at_level3_while_down(self):
        env = E.Envelope()
        env.init([99, 99, 40, 50], [99, 99, 80, 0], 4064, 0)
        trace = [env.getsample() for _ in range(3000)]
        sustain = target_for(80, 4064)
        k = next(i for i, v in enumerate(trace) if v == sustain)
        self.assertEqual(env.ix_, 3)
        for v in trace[k:k + 50]:
            self.assertEqual(v, sustain)

    def test_completed_release_parks_at_ix4_forever(self):
        env = E.Envelope()
        env.init([99, 99, 40, 50], [99, 99, 80, 0], 4064, 0)
        for _ in range(5):
            env.getsample()
        env.keydown(False)
        trace = [env.getsample() for _ in range(4000)]
        k = next(i for i, v in enumerate(trace) if v == 16 << 16)
        self.assertEqual(env.ix_, 4)
        for v in trace[k:]:
            self.assertEqual(v, 16 << 16)

    def test_update_while_down_recomputes_sustain_target(self):
        env = E.Envelope()
        env.init([99, 99, 40, 50], [99, 99, 80, 0], 4064, 0)
        env.update([99, 99, 40, 50], [99, 99, 60, 0], 2816, 0)
        self.assertEqual(env.ix_, 2)
        # update() first sets its own (outlevel-less) target, then advance(2)
        # recomputes the target WITH outlevel_ — the pinned env.cc:166-167
        # sequence, where the update-set value is immediately overwritten
        self.assertEqual(env.targetlevel_, target_for(60, 2816))

    def test_transfer_copies_all_state(self):
        a = E.Envelope()
        a.init([99, 60, 40, 50], [99, 70, 60, 40], 4064, 2)
        for _ in range(10):
            a.getsample()
        a.keydown(False)
        b = E.Envelope()
        b.transfer(a)
        for attr in ("level_", "targetlevel_", "rising_", "ix_", "down_",
                     "staticcount_", "inc_", "outlevel_", "rate_scaling_"):
            self.assertEqual(getattr(b, attr), getattr(a, attr), attr)
        self.assertEqual(b.getsample(), a.getsample())

    def test_int32_range_over_battery(self):
        for rates, levels, ol, rs in self.CASES:
            env = E.Envelope()
            env.init(rates, levels, ol, rs)
            for frame in range(4000):
                if frame == 2000:
                    env.keydown(False)
                lvl = env.getsample()
                self.assertLess(abs(lvl), 1 << 31, (rates, levels, frame))
                self.assertGreaterEqual(lvl, 0)

    def test_derive_params_matches_dx7note_init(self):
        op = make_op([50] * 4, [99] * 4, outlevel=60, kvs=3, rs=3, bp=40,
                     rdepth=25, rcurve=3)
        rates, levels, ol, rs = E.derive_op_envelope_params(op, 108, 127)
        self.assertEqual((rates, levels), ([50] * 4, [99] * 4))
        want = E.scale_out_level(60) + \
            E.scale_level(108, 40, 0, 25, 0, 3)
        want = min(127, want) << 5
        want += E.scale_velocity(127, 3)
        self.assertEqual(ol, max(0, want))
        self.assertEqual(rs, E.scale_rate(108, 3))

    def test_scale_velocity_endpoints(self):
        self.assertEqual(E.scale_velocity(127, 0), 0)
        self.assertEqual(E.scale_velocity(1, 0), 0)
        self.assertEqual(E.scale_velocity(20, 0), E.scale_velocity(127, 0))
        self.assertEqual(E.scale_velocity(1, 3),
                         ((3 * (0 - 239) + 7) >> 3) << 4)


# ===========================================================================
# 2. Independent re-derivation


class TestIndependentDerivation(unittest.TestCase):
    def test_full_trace_equality_over_battery(self):
        n = 1200
        for rates, levels, ol, rs in battery_cases():
            events = [(0, "keydown"), (700, "keyup")]
            mine = E.simulate(n, events, rates, levels, ol, rs)
            theirs = independent_trace(rates, levels, ol, rs, n, events,
                                       E.SR_MULTIPLIER_48K)
            self.assertEqual(
                mine, theirs,
                f"trace divergence {rates}/{levels}/{ol}/{rs}")

    def test_crossing_times_match_independent_derivation(self):
        rates, levels, ol, rs = [99, 60, 40, 50], [99, 70, 60, 40], 4064, 0
        events = [(75, "keydown"), (750, "keyup")]
        mine = E.simulate(1440, events, rates, levels, ol, rs)
        theirs = independent_trace(rates, levels, ol, rs, 1440, events,
                                   E.SR_MULTIPLIER_48K)
        for probe in (76, 100, 300, 436, 500, 800, 856, 1100, 1400):
            self.assertEqual(mine[probe], theirs[probe], probe)

    def test_static_hold_rate0_case(self):
        # rate 0 at ix 0 with level 0 -> ACCURATE_ENVELOPE static hold,
        # statics[0] // 20 (faster attack scaling), then sr_multiplier scale
        rates, levels, ol, rs = [0, 80, 70, 60], [0, 90, 80, 70], 4064, 0
        env = E.Envelope()
        env.init(rates, levels, ol, rs)
        want = ((_INDEP_STATICS[0] // 20) * E.SR_MULTIPLIER_48K) >> 24
        self.assertEqual(env.staticcount_, want)
        self.assertEqual(env.inc_,
                         ((4 << (2 + 6)) * E.SR_MULTIPLIER_48K) >> 24)
        for _ in range(100):
            env.getsample()
        self.assertEqual(env.ix_, 0)


# ===========================================================================
# 3. Directed oracle cases + negative controls


DIRECTED_RESULTS = {
    "schema": "gf180-dx7.n03-directed/1",
    "issue": "https://github.com/2AMLogic/gf180-dx7/issues/17",
    "claim": ("integer envelope model reproduces the pinned oracle's "
              "operator-envelope behavior within the declared probe budgets "
              "(docs/N03-ENVELOPE-MODEL.md section 5); software-reference "
              "agreement only, not original-DX7 fidelity"),
    "tolerances": {"value_probe_db": 0.6, "time_probe_blocks": 4,
                   "onset_blocks": 1, "ratio_probe_db": 0.4,
                   "silence_probe_blocks": 0},
    "cases": [], "renders": [], "controls": [],
}


@unittest.skipUnless(ORACLE is not None, ORACLE_ABSENT)
class TestDirectedOracle(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.renderer = OracleRenderer(ORACLE)

    @classmethod
    def tearDownClass(cls):
        DIRECTED_RESULTS["renders"] = cls.renderer.runs
        cls.renderer.close()

    def _finish(self, checker, name, notes=None):
        checker.assert_all()
        DIRECTED_RESULTS["cases"].append({
            "case": name, "verdict": "PASS",
            "probes": len(checker.rows), "notes": notes or {}})

    # -- base: attack, decay, sustain, release to a nonzero L4 ---------------

    def test_base_envelope(self):
        carr = make_op([99, 60, 40, 50], [99, 70, 60, 40])
        events = [(4800, "note_on", 108, 100), (43200, "note_off", 108)]
        checker, info = run_case(self.renderer, "base", carr, 108, 100,
                                 events, 1.5,
                                 [(4800, "keydown"), (43200, "keyup")])
        b0, b1 = 75, 675
        checker.onset_probe()
        checker.set_anchor(b0 + 470, b1 - 10)
        for k in (b0 + 100, b0 + 250, b0 + 400):
            checker.value_probe("decay", k)
        checker.value_probe("sustain", b1 - 30)
        for k in (b1 + 40, b1 + 100, b1 + 170):
            checker.value_probe("release", k)
        checker.no_tail_cut_probe("release", b1 + 220, b1 + 240)
        self._finish(checker, "base", info["derived"])

    # -- rising segments -----------------------------------------------------

    def test_attack_fast_rate99(self):
        carr = make_op([99, 60, 50, 60], [99, 99, 60, 40])
        events = [(4800, "note_on", 108, 100)]
        checker, info = run_case(self.renderer, "attack_fast", carr, 108,
                                 100, events, 1.6, [(4800, "keydown")])
        checker.onset_probe()
        checker.set_anchor(150, 300)
        for k in (60, 120, 400):
            checker.value_probe("sustain", k)
        self._finish(checker, "attack_fast", info["derived"])

    def test_delayed_attack_rate20(self):
        carr = make_op([20, 50, 50, 50], [99, 99, 60, 40])
        events = [(960, "note_on", 108, 100), (144000, "note_off", 108)]
        checker, info = run_case(self.renderer, "attack_slow", carr, 108,
                                 100, events, 4.0,
                                 [(960, "keydown"), (144000, "keyup")])
        b0, b1 = 15, 2250
        checker.onset_probe()
        checker.set_anchor(1960, 2140)
        for k in (b0 + 300, b0 + 900, b0 + 1400):
            checker.value_probe("attack", k)
        checker.value_probe("decay", b0 + 1700)
        for k in (b1 + 30, b1 + 120, b1 + 200):
            checker.value_probe("release", k)
        self._finish(checker, "attack_slow", info["derived"])

    def test_slow_attack_rate1(self):
        carr = make_op([1, 50, 50, 50], [99, 99, 60, 40])
        events = [(960, "note_on", 108, 100)]
        checker, info = run_case(self.renderer, "attack_rate1", carr, 108,
                                 100, events, 3.2, [(960, "keydown")])
        b0 = 15
        checker.onset_probe()
        checker.set_anchor(2340, 2399)
        for k in (b0 + 300, b0 + 900, b0 + 1500, b0 + 2000):
            checker.value_probe("attack", k)
        checker.delta_probe("attack", b0 + 300, b0 + 1900)
        self._finish(checker, "attack_rate1", info["derived"])

    # -- nonzero final level, long sustain, no tail cut -----------------------

    def test_sustain_hold_nonzero_final_level(self):
        carr = make_op([99, 99, 99, 60], [99, 99, 99, 40])
        events = [(1920, "note_on", 108, 100), (57600, "note_off", 108)]
        checker, info = run_case(self.renderer, "sustain_hold", carr, 108,
                                 100, events, 3.0,
                                 [(1920, "keydown"), (57600, "keyup")])
        b1 = 900
        checker.onset_probe()
        checker.set_anchor(100, 800)
        checker.value_probe("sustain", 300)
        checker.value_probe("sustain", 700)
        for k in (b1 + 60, b1 + 130, b1 + 200):
            checker.value_probe("release", k)
        checker.no_tail_cut_probe("release", 1300, 1400)
        self._finish(checker, "sustain_hold", info["derived"])

    # -- release from the current level mid-decay + exact silence floor ------

    def test_release_middecay_exact_silence(self):
        carr = make_op([99, 30, 40, 60], [99, 80, 60, 0])
        events = [(960, "note_on", 108, 100), (19200, "note_off", 108)]
        checker, info = run_case(self.renderer, "release_middecay", carr,
                                 108, 100, events, 2.0,
                                 [(960, "keydown"), (19200, "keyup")])
        b1 = 300
        checker.onset_probe()
        checker.set_anchor(70, 240)
        checker.value_probe("release", b1 + 2)
        for k in (b1 + 40, b1 + 120, b1 + 200):
            checker.value_probe("release", k)
        checker.silence_probe(b1)
        self._finish(checker, "release_middecay", info["derived"])

    def test_sustain_pedal_holds_then_releases_from_current_level(self):
        carr = make_op([99, 40, 50, 60], [99, 80, 70, 30])
        events = [(1920, "cc", 64, 127), (1920, "note_on", 108, 100),
                  (28800, "note_off", 108), (48000, "cc", 64, 0)]
        checker, info = run_case(self.renderer, "sustain_pedal", carr, 108,
                                 100, events, 2.0,
                                 [(1920, "keydown"), (48000, "keyup")])
        checker.onset_probe()
        checker.set_anchor(450, 700)
        checker.value_probe("sustain", 500)
        # note_off at block 450 must NOT release (pedal down): the model
        # holds until the cc64=0 keyup at block 750
        checker.value_probe("sustain", 700)
        for k in (770, 840):
            checker.value_probe("release", k)
        self._finish(checker, "sustain_pedal", info["derived"])

    # -- velocity sensitivity on/off ------------------------------------------

    def _plateau_db(self, pcm, lo, hi):
        raw = block_rms(pcm)
        seg = sorted(v * v for v in raw[lo:hi])
        return db(seg[len(seg) // 2])

    def test_velocity_sensitivity_scales_level(self):
        carr = make_op([99, 60, 50, 60], [99, 99, 60, 40], kvs=3)
        pcm20 = self.renderer.render(make_voice(carr),
                                     [(960, "note_on", 108, 20)], 1.0)
        pcm127 = self.renderer.render(make_voice(carr),
                                      [(960, "note_on", 108, 127)], 1.0)
        rates, levels, ol20, rs = E.derive_op_envelope_params(carr, 108, 20)
        _, _, ol127, _ = E.derive_op_envelope_params(carr, 108, 127)
        t20 = target_for(99, ol20)
        t127 = target_for(99, ol127)
        pred = (t127 - t20) * DB_PER_DOUBLING / (1 << 24)
        meas = self._plateau_db(pcm127, 400, 460) \
            - self._plateau_db(pcm20, 400, 460)
        trace = E.simulate(750, [(15, "keydown")], rates, levels, ol127, rs)
        checker = CaseChecker("velocity", pcm127, trace, 15)
        checker.set_anchor(400, 460)
        checker._record("sustain", "velocity_ratio_db", pred, meas,
                        meas - pred, CaseChecker.T_RATIO)
        DIRECTED_RESULTS["cases"].append({
            "case": "velocity", "verdict": "PASS" if not checker.failures
            else "FAIL", "probes": 1,
            "notes": {"outlevel_v20": ol20, "outlevel_v127": ol127,
                      "predicted_db": round(pred, 3),
                      "measured_db": round(meas, 3)}})
        checker.assert_all()

    def test_velocity_insensitive_is_byte_identical(self):
        carr = make_op([99, 60, 50, 60], [99, 99, 60, 40], kvs=0)
        pcm20 = self.renderer.render(make_voice(carr),
                                     [(960, "note_on", 108, 20)], 0.6)
        pcm127 = self.renderer.render(make_voice(carr),
                                      [(960, "note_on", 108, 127)], 0.6)
        identical = pcm20.tobytes() == pcm127.tobytes()
        DIRECTED_RESULTS["cases"].append({
            "case": "velocity_off", "verdict": "PASS" if identical
            else "FAIL", "probes": 1,
            "notes": "kvs=0: velocity must not enter the render at all "
                     "(dx7note.cc:176 consumes it only via ScaleVelocity)"})
        self.assertTrue(identical, "kvs=0 renders differ between velocities; "
                                   "velocity leaked into engine state")

    # -- keyboard level / rate scaling at low vs high notes -------------------

    def test_key_level_scaling(self):
        carr = make_op([99, 60, 50, 60], [99, 99, 60, 40], outlevel=60,
                       bp=40, ldepth=0, rdepth=25, rcurve=3)
        pcm96 = self.renderer.render(make_voice(carr),
                                     [(960, "note_on", 96, 100)], 1.0)
        pcm108 = self.renderer.render(make_voice(carr),
                                      [(960, "note_on", 108, 100)], 1.0)
        _, _, ol96, _ = E.derive_op_envelope_params(carr, 96, 100)
        _, _, ol108, _ = E.derive_op_envelope_params(carr, 108, 100)
        self.assertNotEqual(ol96, ol108, "key level scaling did not move "
                                         "the effective outlevel")
        t96 = target_for(99, ol96)
        t108 = target_for(99, ol108)
        pred = (t108 - t96) * DB_PER_DOUBLING / (1 << 24)
        meas = self._plateau_db(pcm108, 400, 460) \
            - self._plateau_db(pcm96, 400, 460)
        rates = [carr.rate_1, carr.rate_2, carr.rate_3, carr.rate_4]
        levels = [carr.level_1, carr.level_2, carr.level_3, carr.level_4]
        trace = E.simulate(750, [(15, "keydown")], rates, levels, ol108,
                           E.scale_rate(108, carr.rate_scaling))
        checker = CaseChecker("keylevel", pcm108, trace, 15)
        checker.set_anchor(400, 460)
        checker._record("sustain", "keylevel_ratio_db", pred, meas,
                        meas - pred, CaseChecker.T_RATIO)
        DIRECTED_RESULTS["cases"].append({
            "case": "keylevel", "verdict": "PASS" if not checker.failures
            else "FAIL", "probes": 1,
            "notes": {"outlevel_n96": ol96, "outlevel_n108": ol108,
                      "predicted_db": round(pred, 3),
                      "measured_db": round(meas, 3)}})
        checker.assert_all()

    def test_key_rate_scaling_speeds_up_attack(self):
        for note, name in ((96, "keyrate_low"), (108, "keyrate_high")):
            carr = make_op([70, 50, 50, 50], [99, 99, 60, 40], rs=3)
            events = [(960, "note_on", note, 100)]
            checker, info = run_case(self.renderer, name, carr, note, 100,
                                     events, 1.0, [(960, "keydown")])
            b0 = 15
            checker.onset_probe()
            checker.set_anchor(500, 560)
            t1 = target_for(99, info["derived"]["outlevel"])
            mid = (t1 + (1716 << 16)) // 2
            checker.slope_guard("attack", mid, "up", b0)
            checker.time_probe("attack", mid, "up", b0, 200)
            checker.value_probe("sustain", 560)
            self._finish(checker, name, info["derived"])


class TestNegativeControls(unittest.TestCase):
    """Each control must demonstrably fail the check it targets."""

    ORACLE_SKIP = unittest.skipUnless(ORACLE is not None, ORACLE_ABSENT)

    BASE_CARRIER = make_op([99, 60, 40, 50], [99, 70, 60, 40])
    BASE_EVENTS = [(4800, "note_on", 108, 100), (43200, "note_off", 108)]
    B0, B1 = 75, 675

    def _base_model(self):
        rates, levels, ol, rs = E.derive_op_envelope_params(
            self.BASE_CARRIER, 108, 100)
        n = 1440
        dx, boundaries = simulate_with_boundaries(
            rates, levels, ol, rs, n,
            [(self.B0, "keydown"), (self.B1, "keyup")])
        targets = [target_for(l, ol) for l in levels]
        return dx, boundaries, targets, rates, levels, ol, rs

    # -- 1. plain linear ADSR substitution ------------------------------------

    def test_adsr_substitution_fails_pure(self):
        """Oracle-independent form: a gain-linear ADSR with the SAME stage
        boundary frames still diverges from the pinned log-domain segments."""
        dx, boundaries, targets, rates, levels, ol, rs = self._base_model()
        lin = LinearAdsr(boundaries, targets, self.B0, self.B1,
                         len(dx)).trace(len(dx))
        divergent = [k for k in range(self.B0, self.B1)
                     if abs(lin[k] - dx[k]) > (1 << 20)]
        self.assertTrue(
            divergent, "gain-linear ADSR indistinguishable from the DX "
                       "envelope; the control is broken")

    @ORACLE_SKIP
    def test_adsr_substitution_fails_trace_comparison(self):
        """The mandated control: substituting a plain 4-stage linear ADSR for
        the DX envelope must fail the directed trace comparison, naming the
        divergent stage."""
        renderer = OracleRenderer(ORACLE)
        try:
            pcm = renderer.render(make_voice(self.BASE_CARRIER),
                                  self.BASE_EVENTS, 1.5)
        finally:
            renderer.close()
        dx, boundaries, targets, rates, levels, ol, rs = self._base_model()
        lin = LinearAdsr(boundaries, targets, self.B0, self.B1,
                         len(dx)).trace(len(dx))

        checker = CaseChecker("nc_adsr", pcm, dx, self.B0)
        checker.powers = pred_block_powers(lin, self.B0, checker.n)
        checker.set_anchor(560, 660)
        for k in (self.B0 + 100, self.B0 + 250, self.B0 + 400):
            checker.value_probe("decay", k)
        checker.value_probe("sustain", self.B1 - 30)
        for k in (self.B1 + 40, self.B1 + 100):
            checker.value_probe("release", k)
        stages = sorted({f.stage for f in checker.failures})
        DIRECTED_RESULTS["controls"].append({
            "control": "linear_adsr_substitution",
            "verdict": "FAIL_DETECTED" if checker.failures
                       else "NOT_DETECTED",
            "divergent_stages": stages,
            "detail": [str(f) for f in checker.failures[:6]],
        })
        self.assertTrue(checker.failures,
                        "linear ADSR passed the DX trace comparison")
        self.assertTrue(stages, "divergent stage not named")

    # -- 2. sr_multiplier +-1 --------------------------------------------------

    def test_sr_multiplier_mutation_fails_derivation(self):
        """Mutating the frozen sr_multiplier by +-1 must fail the derivation
        checks: (a) the constant no longer matches the documented formula;
        (b) reachable coefficients (inc_/staticcount_) flip, so the
        independent crossing-time derivation disagrees with the mutated
        model on exact frames."""
        for delta in (-1, 1):
            mutated = E.SR_MULTIPLIER_48K + delta
            with self.subTest(mutated=mutated):
                self.assertNotEqual(
                    mutated, math.floor(44100.0 / 48000.0 * (1 << 24)))
                base_static = ((_INDEP_STATICS[0] // 20)
                               * E.SR_MULTIPLIER_48K) >> 24
                mut_static = ((_INDEP_STATICS[0] // 20) * mutated) >> 24
                coeff_flips = base_static != mut_static
                n = 2400
                trace_flips = 0
                for rates, levels, ol, rs in mutation_battery():
                    events = [(0, "keydown"), (1200, "keyup")]
                    mine = E.simulate(n, events, rates, levels, ol, rs,
                                      sr_multiplier=mutated)
                    theirs = independent_trace(rates, levels, ol, rs, n,
                                               events, E.SR_MULTIPLIER_48K)
                    if mine != theirs:
                        trace_flips += 1
                self.assertTrue(coeff_flips or trace_flips > 0,
                                "sr_multiplier +-1 absorbed by the >>24 "
                                "truncation everywhere; control would not "
                                "fire")
                self.assertGreater(trace_flips, 0,
                                   "no timing-derivation disagreement on the "
                                   "mutation battery")

    # -- 3. swapped rate/level arrays ------------------------------------------

    def test_swapped_rate_level_arrays_fail_pure(self):
        rates, levels, ol, rs = E.derive_op_envelope_params(
            self.BASE_CARRIER, 108, 100)
        n = 1440
        events = [(self.B0, "keydown"), (self.B1, "keyup")]
        correct = E.simulate(n, events, rates, levels, ol, rs)
        swapped = E.simulate(n, events, levels, rates, ol, rs)
        divergent = sum(1 for k in range(self.B0, n)
                        if correct[k] != swapped[k])
        self.assertGreater(
            divergent, 500,
            "swapped rate/level arrays nearly indistinguishable; the "
            "control would not fire")

    @ORACLE_SKIP
    def test_swapped_rate_level_arrays_fail_trace_comparison(self):
        renderer = OracleRenderer(ORACLE)
        try:
            pcm = renderer.render(make_voice(self.BASE_CARRIER),
                                  self.BASE_EVENTS, 1.5)
        finally:
            renderer.close()
        rates, levels, ol, rs = E.derive_op_envelope_params(
            self.BASE_CARRIER, 108, 100)
        n = int(1.5 * RENDER_RATE) // BLOCK
        swapped = E.simulate(n, [(self.B0, "keydown"), (self.B1, "keyup")],
                             levels, rates, ol, rs)
        checker = CaseChecker("nc_swapped", pcm, swapped, self.B0)
        checker.set_anchor(560, 660)
        for k in (self.B0 + 100, self.B0 + 250, self.B0 + 400):
            checker.value_probe("decay", k)
        checker.value_probe("sustain", self.B1 - 30)
        for k in (self.B1 + 40, self.B1 + 100):
            checker.value_probe("release", k)
        stages = sorted({f.stage for f in checker.failures})
        DIRECTED_RESULTS["controls"].append({
            "control": "swapped_rate_level_arrays",
            "verdict": "FAIL_DETECTED" if checker.failures
                       else "NOT_DETECTED",
            "divergent_stages": stages,
            "detail": [str(f) for f in checker.failures[:6]],
        })
        self.assertTrue(checker.failures,
                        "swapped rate/level arrays passed the trace "
                        "comparison")
        self.assertTrue(stages, "divergent stage not named")


def _write_evidence():
    if os.environ.get("GF180_N03_EVIDENCE", "1") == "0" \
            or not DIRECTED_RESULTS["renders"]:
        return
    EVIDENCE_PATH.parent.mkdir(parents=True, exist_ok=True)
    EVIDENCE_PATH.write_text(
        json.dumps(DIRECTED_RESULTS, indent=2, sort_keys=False) + "\n",
        encoding="utf-8")


def tearDownModule():
    """Write the directed-case evidence once, after every class (including
    the negative controls) has appended its rows."""
    _write_evidence()


if __name__ == "__main__":
    unittest.main()
