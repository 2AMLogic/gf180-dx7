#!/usr/bin/env python3
"""R07 targeted oracle-disagreement driver (issue #14).

Renders a SMALL diagnostic set (six cases chosen for likely cross-
implementation disagreement) through TWO external GPL oracles and
compares the renders with the R04 exact paired comparator
(src/gf180_dx7/compare.py):

  * reference:  dx7-oracle (pinned Dexed Mark I wrapper,
                reference/oracle-identity.json);
  * second:     hexter-oracle (pinned hexter DSSI engine wrapper,
                reference/second-oracle-identity.json).

HONESTY RULES (normative; enforced by the report schema validator here
and by review per the issue's negative control):

  * Cross-engine comparisons are EXPECTED to fail exactness. The FAIL
    verdict is the finding, never a defect of the harness. Nothing here
    or in docs/R07-DISAGREEMENT.md adjudicates which implementation is
    physically correct: that needs original-hardware evidence (or a
    third independent source), which this investigation does not have.
  * Every per-case explanation is labeled a HYPOTHESIS with a named
    behavior/seam; hypotheses cite both pinned sources as plausibility
    evidence only.
  * No engine averaging, no per-patch reference switching, no
    alignment/normalization: compare.py runs in exact mode, dry
    (docs/COMPARE-CONTRACT.md).
  * Event positions are pre-quantized to 64-sample block starts so BOTH
    engines receive identical effective event times (the reference
    oracle applies events at block boundaries; positions used here are
    exact multiples of 64, so quantization is the identity).
  * The corpus case is integrity-checked against corpus/dev32.json
    (canonical hash, name-excluded) before use; archive bytes are never
    redistributed (corpus policy: evidence keeps provenance JSON, not
    third-party voice bytes, for that case).

Outputs (default): reference/evidence/r07/<case>/ with inputs/, the
oracles' own meta JSONs, compare.json (schema
"r07-disagreement-case/1") and the raw renders (gitignored: only hashes
and comparison reports are committed). Plus summary.json.

Exit codes: 0 all cases completed (each comparison itself FAILs by
design; "completed" means both renders produced and compared);
1 harness failure (identity drift, render error, silent guard,
integrity check); 2 NOT_RUN (environment: an oracle binary or the
corpus archive is unavailable — guarded skips, never silent passes).

Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import compare, sysex  # noqa: E402

REFERENCE_IDENTITY = REPO / "reference" / "oracle-identity.json"
SECOND_IDENTITY = REPO / "reference" / "second-oracle-identity.json"
DEV32 = REPO / "corpus" / "dev32.json"
DEFAULT_ARCHIVE = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")
DEFAULT_EVIDENCE = REPO / "reference" / "evidence" / "r07"

RATE = 48000
SECONDS = 3.0
BLOCK = 64
NOTE_ON_POS = 4800
NOTE_OFF_POS = 96000
SILENT_THRESHOLD = 1e-6

ISSUE_URL = "https://github.com/2AMLogic/gf180-dx7/issues/14"
SCHEMA = "r07-disagreement-case/1"
NON_ADJUDICATION = (
    "No engine is adjudicated as physically correct. These are two "
    "software implementations; cross-implementation disagreement is "
    "recorded as localized findings (hypotheses), and any claim about "
    "original DX7 hardware would require hardware evidence this "
    "investigation does not have.")

CASE_EVENTS = [
    {"cmd": "note_on", "args": [60, 100], "pos": NOTE_ON_POS},
    {"cmd": "note_off", "args": [60], "pos": NOTE_OFF_POS},
]


# --------------------------------------------------------------------
# Environment verification (identities)
# --------------------------------------------------------------------

def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _check_identity(identity_path: Path) -> dict:
    """Re-hash the pinned binary; returns a verification record."""
    record = {"identity_path": str(identity_path), "status": "NOT_RUN",
              "detail": ""}
    try:
        identity = json.loads(identity_path.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        record["detail"] = f"identity unreadable: {exc}"
        return record
    record["identity"] = identity.get("id")
    binary = Path(identity["binary"]["path"])
    if not binary.exists():
        record["detail"] = f"oracle binary absent at {binary}"
        return record
    actual = sha256_file(binary)
    record["binary_sha256_actual"] = actual
    if actual != identity["binary"]["sha256"]:
        record["status"] = "FAIL"
        record["detail"] = "binary sha256 drift against pinned identity"
        return record
    record["status"] = "PASS"
    record["detail"] = "binary hash matches pinned identity"
    if "git_commit" in identity.get("source", {}):
        source_path = Path(identity["source"]["path"])
        if source_path.exists():
            try:
                head = subprocess.run(
                    ["git", "-C", str(source_path), "rev-parse", "HEAD"],
                    capture_output=True, text=True, timeout=30,
                    check=True).stdout.strip()
                record["source_head"] = head
                if head != identity["source"]["git_commit"]:
                    record["status"] = "FAIL"
                    record["detail"] = (
                        f"source HEAD {head[:12]} != pinned "
                        f"{identity['source']['git_commit'][:12]}")
            except (OSError, subprocess.SubprocessError) as exc:
                record["source_head_check"] = f"NOT_RUN: {exc}"
        else:
            record["source_head_check"] = (
                f"NOT_RUN: source tree absent at {source_path}")
    return record


# --------------------------------------------------------------------
# Diagnostic case construction
# --------------------------------------------------------------------

def _op(rates=(50, 50, 50, 50), levels=(99, 99, 99, 99), ol=75, coarse=1,
        fine=0, detune=7, osc_mode=0, kvs=2, rs=0) -> sysex.Operator:
    return sysex.Operator(
        rate_1=rates[0], rate_2=rates[1], rate_3=rates[2], rate_4=rates[3],
        level_1=levels[0], level_2=levels[1], level_3=levels[2],
        level_4=levels[3],
        break_point=99, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0, rate_scaling=rs,
        amp_mod_sensitivity=0, key_velocity_sensitivity=kvs,
        output_level=ol, osc_mode=osc_mode, osc_freq_coarse=coarse,
        osc_freq_fine=fine, osc_detune=detune)


def _voice(ops, algorithm, feedback, name) -> sysex.Voice:
    assert len(name) <= 10
    return sysex.Voice(
        operators=ops,
        pitch_eg_rate_1=99, pitch_eg_rate_2=99, pitch_eg_rate_3=99,
        pitch_eg_rate_4=99,
        pitch_eg_level_1=50, pitch_eg_level_2=50, pitch_eg_level_3=50,
        pitch_eg_level_4=50,
        algorithm=algorithm, feedback=feedback, osc_key_sync=1,
        lfo_speed=35, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0, transpose=0,
        name=name, name_bytes=name.encode("ascii").ljust(10, b" ")[:10],
        channel_byte=0)


STEADY = dict(rates=(50, 50, 50, 50), levels=(99, 99, 99, 99))

# Hypotheses are plausibility readings of the two PINNED sources, cited
# per case. They are HYPOTHESES, not adjudications (see module docstring).
HYPOTHESES = {
    "fb-alg4": {
        "seam": "feedback coefficient mapping + feedback history depth",
        "hypothesis": "Feedback-heavy operator chains diverge because the "
            "engines map the 3-bit feedback parameter differently and keep "
            "different feedback histories: Dexed msfa uses fb_shift = "
            "8 - feedback with a TWO-sample averaged history ((y0+y) >> "
            "(shift+1)) inside 64-sample kernels (dexed dx7note.cc:196-197, "
            "dx7note.cc:28, fm_op_kernel.cc compute_fb, fm_core.cc:114-118); "
            "hexter uses aux_feedbk = feedback/(2*pi)*0.18 with a ONE-sample "
            "history and an acknowledged missing per-algorithm feedback "
            "scaling (hexter dx7_voice.c:1390 '-FIX- feedback_scaling', "
            "dx7_voice_render.c:128-131).",
        "expect_if_confirmed": "Divergence should grow over the note "
            "duration (feedback compounds) and be far larger than in the "
            "feedback-0 corpus case; spectral character (ZCR proxy) should "
            "shift with the feedback-brightened spectrum.",
    },
    "fb-alg6": {
        "seam": "feedback coefficient mapping + feedback history depth "
                "(second algorithm topology)",
        "hypothesis": "Same feedback seam as fb-alg4, exercised through a "
            "different algorithm topology, to check the finding is not "
            "specific to one routing: the feedback operator wraps into a "
            "different carrier structure, so matching measured divergence "
            "class would localize the cause to the feedback path itself "
            "rather than one algorithm table entry.",
        "expect_if_confirmed": "Magnitude class consistent with fb-alg4 "
            "(both feedback 7); a large class mismatch would instead point "
            "at the algorithm tables (hexter dx7_voice_render.c switch vs "
            "dexed msfa fm_core.cc algorithms[]).",
    },
    "detune-extreme": {
        "seam": "oscillator detune curve at range extremes (raw 0 and 14)",
        "hypothesis": "The two carriers (algorithm 1 = carriers op1+op3, "
            "dexed msfa fm_core.h carriers table / hexter "
            "dx7_voice.c:dx7_voice_carriers) are detuned to the extremes of "
            "the DX7 detune field (raw 0 vs 14, center 7). The engines "
            "convert detune to frequency with different approximations "
            "(dexed msfa dx7note.cc pitchdetune/opus_aux tables vs hexter "
            "dx7_voice.c frequency arithmetic), so the beat frequency "
            "between carriers should differ measurably while both remain "
            "musically 'detuned'.",
        "expect_if_confirmed": "First divergence within the note, envelope "
            "shape proxy close (similar amplitude ramp), ZCR proxy "
            "difference small but nonzero (slightly different beat rate), "
            "error growing as relative phase walks.",
    },
    "slow-envelope": {
        "seam": "envelope rate/level coefficient fit at rate=1 extremes",
        "hypothesis": "With all rates = 1 and a nonzero final level "
            "(level[3]=50/60), the near-zero-slew end of the envelope "
            "coefficient tables is exercised: dexed msfa env.cc interpolates "
            "a 4-phase keyframe table per 64-sample block, hexter derives "
            "frame durations from dx7_voice_eg_rate_rise/decay_duration "
            "tables with pre/post compensation (dx7_voice.c "
            "dx7_op_eg_set_rate). Tiny coefficient differences accumulate "
            "over seconds, so first divergence may be late and rms error "
            "small relative to peak.",
        "expect_if_confirmed": "Small-to-moderate rms_error class with "
            "envelope-shape proxy showing growing window RMS relative "
            "difference over time; onset identical (both start at the "
            "note-on block).",
    },
    "fixed-frequency-high": {
        "seam": "fixed-frequency mode conversion (coarse/fine to Hz)",
        "hypothesis": "Fixed-mode coarse=15 fine=99 sits near the top of "
            "the fixed-frequency range: dexed msfa computes fixed pitch via "
            "its frequency tables (dx7note.cc / freqlut), hexter via "
            "fixed_freq_multiplier = tuning/440 and its own fixed-note "
            "formula (dx7_voice.c dx7_voice_recalculate_freq_and_inc). "
            "Any small ratio mismatch becomes a large absolute Hz error at "
            "high coarse, visible as immediate divergence and ZCR shift.",
        "expect_if_confirmed": "Divergence from the very first sounding "
            "block, ZCR proxy difference notably larger than the corpus "
            "case, envelope onset still aligned.",
    },
    "corpus-shrpsynth": {
        "seam": "holistic (all seams at once; real catalog voice)",
        "hypothesis": "A real R03 corpus voice (dev32 entry 0, "
            "SHRPSYNTH, algorithm 0, feedback 1, LFO wave 3) combines "
            "seams; it anchors how large the compounded disagreement is "
            "relative to the targeted synthetic cases. With feedback 1 the "
            "feedback seam contributes little; the expected dominant "
            "contributors are the envelope coefficient fit and any output "
            "scaling difference.",
        "expect_if_confirmed": "Magnitude class between the mild and "
            "feedback cases; used as the anchor for N01 budget discussion "
            "(see docs/R07-DISAGREEMENT.md), not as an adjudication.",
    },
}


def synthetic_cases() -> list[dict]:
    """Five parameter-authored cases (original voice bytes, committable)."""
    steady = dict(STEADY)
    chain = lambda ol, **kw: _op(ol=ol, **kw)  # noqa: E731
    return [
        {
            "name": "fb-alg4",
            "rationale": "feedback-heavy: algorithm 4 (index 3), feedback 7",
            "voice": _voice(
                [chain(99), chain(75), chain(75), chain(75), chain(75),
                 chain(75)], 3, 7, "FB-ALG4"),
        },
        {
            "name": "fb-alg6",
            "rationale": "feedback-heavy: algorithm 6 (index 5), feedback 7",
            "voice": _voice(
                [chain(99), chain(75), chain(75), chain(75), chain(75),
                 chain(75)], 5, 7, "FB-ALG6"),
        },
        {
            "name": "detune-extreme",
            "rationale": "detune extremes ±7 on the two algorithm-1 "
                         "carriers (op1 det 0, op3 det 14)",
            "voice": _voice(
                [_op(ol=99, detune=0), chain(60, detune=7),
                 _op(ol=99, detune=14), chain(60, detune=7),
                 chain(60, detune=7), chain(60, detune=7)],
                0, 0, "DETUNE-EX"),
        },
        {
            "name": "slow-envelope",
            "rationale": "slow envelope: all rates 1, nonzero final levels",
            "voice": _voice(
                [_op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 50), ol=99),
                 _op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 60), ol=75),
                 _op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 60), ol=75),
                 _op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 60), ol=75),
                 _op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 60), ol=75),
                 _op(rates=(1, 1, 1, 1), levels=(99, 99, 99, 60), ol=75)],
                0, 0, "SLOW-ENV"),
        },
        {
            "name": "fixed-frequency-high",
            "rationale": "fixed-frequency high coarse (op1 fixed coarse 15 "
                         "fine 99)",
            "voice": _voice(
                [_op(ol=99, osc_mode=1, coarse=15, fine=99),
                 chain(75), chain(99), chain(75), chain(75), chain(75)],
                0, 0, "FIXED-HI"),
        },
    ]


def canonical_hash_of_voice(v: sysex.Voice) -> str:
    """Name-excluded canonical hash (same definition as R03
    tools/catalog_archive.py: unpacked body with name region zeroed)."""
    body = sysex.encode_voice(v)[6:6 + sysex.VOICE_DATA_LEN]
    return hashlib.sha256(body[:145] + b"\x00" * 10).hexdigest()


def corpus_case(archive_path: Path) -> tuple[dict, str]:
    """Load dev32 entry 0 from the pinned archive, integrity-checked.

    Returns (case, canonical_hash). Raises RuntimeError on any drift:
    the archive hash must match the dev32 pin and the extracted voice's
    canonical (name-excluded) hash must match the dev32 entry.
    """
    dev32 = json.loads(DEV32.read_text(encoding="utf-8"))
    entry = dev32["entries"][0]
    archive_pin = dev32["selection"]["archive"]
    if not archive_path.exists():
        raise FileNotFoundError(
            f"pinned archive not found at {archive_path}")
    actual = sha256_file(archive_path)
    if actual != archive_pin["sha256"]:
        raise RuntimeError(
            f"archive drift: {archive_path} sha256 {actual} != pinned "
            f"{archive_pin['sha256']}")
    with zipfile.ZipFile(archive_path) as zf:
        raw = zf.read(entry["path"])
    msgs = sysex.split_stream(raw)
    if len(msgs) != 1 or len(msgs[0]) != sysex.BANK_MSG_LEN:
        raise RuntimeError(
            f"{entry['path']}: expected one 4104-byte bank, got "
            f"{[len(m) for m in msgs]}")
    bank = sysex.decode_bank(msgs[0])
    voice = bank.voices[entry["slot"]]
    chash = canonical_hash_of_voice(voice)
    if chash != entry["canonical_hash"]:
        raise RuntimeError(
            f"canonical hash drift for {entry['path']} slot "
            f"{entry['slot']}: {chash} != dev32 {entry['canonical_hash']}")
    case = {
        "name": "corpus-shrpsynth",
        "rationale": "R03 corpus voice, mid-pitch (dev32 entry 0)",
        "voice": voice,
        "voice_form": "single (re-encoded from bank slot via P01 codec)",
        "corpus_provenance": {
            "archive": {"filename": archive_pin["filename"],
                        "sha256": actual,
                        "entry": entry["path"], "slot": entry["slot"]},
            "canonical_hash": chash,
            "dev32_index": entry["index"],
            "policy": "archive read-only; bytes not redistributed; this "
                      "evidence directory deliberately stores no third-"
                      "party voice bytes for this case (only this "
                      "provenance record + events + render hashes)",
        },
    }
    return case, chash


def build_cases(archive_path: Path | None) -> tuple[list[dict], list[str]]:
    """All diagnostic cases. Archive-dependent failures degrade the corpus
    case to NOT_RUN (recorded in `notes`), never silently drop it."""
    cases = []
    notes = []
    cases.extend(synthetic_cases())
    if archive_path is None:
        notes.append("corpus-shrpsynth: NOT_RUN (no archive path given)")
        return cases, notes
    try:
        case, chash = corpus_case(archive_path)
        case["canonical_hash"] = chash
        cases.append(case)
    except (FileNotFoundError, RuntimeError, sysex.SysexError,
            zipfile.BadZipFile) as exc:
        notes.append(f"corpus-shrpsynth: NOT_RUN ({exc})")
    return cases, notes


def events_text(events: list[dict]) -> str:
    lines = []
    for e in sorted(events, key=lambda e: e["pos"]):
        if e["pos"] % BLOCK:
            raise RuntimeError(
                f"event position {e['pos']} is not a multiple of the "
                f"{BLOCK}-sample block (both oracles must see identical "
                "effective event times)")
        if e["cmd"] == "note_on":
            lines.append(f"{e['pos']} note_on {e['args'][0]} {e['args'][1]}")
        elif e["cmd"] == "note_off":
            lines.append(f"{e['pos']} note_off {e['args'][0]}")
        else:
            raise RuntimeError(f"unsupported event {e['cmd']}")
    return "\n".join(lines) + "\n"


# --------------------------------------------------------------------
# Rendering
# --------------------------------------------------------------------

class RenderError(RuntimeError):
    pass


def _run_oracle(exe: Path, voice: bytes, events: str, out_path: Path,
                meta_path: Path, seconds: float, label: str) -> dict:
    with tempfile.TemporaryDirectory(prefix="r07-") as tmp:
        tmp = Path(tmp)
        voice_path = tmp / "voice.syx"
        events_path = tmp / "events.txt"
        voice_path.write_bytes(voice)
        events_path.write_text(events, encoding="ascii")
        cmd = [str(exe), "--voice", str(voice_path), "--events",
               str(events_path), "--out", str(out_path), "--meta",
               str(meta_path), "--rate", str(RATE), "--seconds",
               str(seconds)]
        try:
            proc = subprocess.run(cmd, capture_output=True, text=True,
                                  timeout=180)
        except (OSError, subprocess.SubprocessError) as exc:
            raise RenderError(f"{label}: render failed to run: {exc}") from exc
        if proc.returncode != 0:
            raise RenderError(
                f"{label}: exit {proc.returncode}: "
                f"{proc.stderr.strip() or proc.stdout.strip()}")
    data = out_path.read_bytes()
    if len(data) != int(RATE * seconds) * 4:
        raise RenderError(
            f"{label}: render is {len(data)} bytes, expected "
            f"{int(RATE * seconds) * 4}")
    samples = struct.unpack("<%df" % (len(data) // 4), data)
    peak = max(abs(v) for v in samples)
    if peak < SILENT_THRESHOLD:
        raise RenderError(
            f"{label}: silent-output guard tripped (max |sample| "
            f"{peak} < {SILENT_THRESHOLD}) while events claim an active "
            "note")
    return {"exit": 0, "stderr": proc.stderr.strip(), "peak_abs": peak,
            "bytes": len(data)}


# --------------------------------------------------------------------
# Comparison + diagnosis
# --------------------------------------------------------------------

def magnitude_class(rms_error: float, ref_rms: float,
                    second_rms: float) -> str:
    if ref_rms == 0.0 and second_rms == 0.0:
        return "both_silent"
    if ref_rms == 0.0 or second_rms == 0.0:
        return "one_stream_silent"
    if rms_error == 0.0:
        return "identical"
    ratio_db = 20.0 * __import__("math").log10(rms_error / ref_rms)
    if ratio_db > -6.0:
        return "signal_scale"
    if ratio_db > -40.0:
        return "below_signal"
    if ratio_db > -90.0:
        return "far_below_signal"
    return "near_identical"


def divergence_class(first_divergent, note_on_pos: int, rate: int) -> str:
    if first_divergent is None:
        return "never"
    if first_divergent < note_on_pos:
        return "pre_onset"
    if first_divergent < note_on_pos + BLOCK:
        return "on_note_on_block"
    if first_divergent < rate:  # within the first second after 0
        return "early"
    if first_divergent < note_on_pos + rate:
        return "mid"
    return "late"


def compare_case(case: dict, ref_bytes: bytes, second_bytes: bytes,
                 ref_meta: dict, second_meta: dict, identities: dict,
                 events: list[dict] | None = None) -> dict:
    events = CASE_EVENTS if events is None else events
    report = compare.compare_streams(ref_bytes, second_bytes,
                                     sample_rate=RATE, exact_mode=True)
    d = report.to_dict()
    primary = d["primary"] or {}
    ref_rms = (d["stream_stats"]["a"] or {}).get("rms", 0.0)
    second_rms = (d["stream_stats"]["b"] or {}).get("rms", 0.0)
    diag = {
        "magnitude_class": magnitude_class(
            primary.get("rms_error", 0.0) or 0.0, ref_rms, second_rms),
        "divergence_class": divergence_class(
            primary.get("first_divergent_sample"), NOTE_ON_POS, RATE),
        "rms_reference": ref_rms,
        "rms_second": second_rms,
        "note": "diagnosis rows are triage aids (compare.py SECONDARY "
                "role applies to onset/envelope/ZCR); the FAIL verdict is "
                "expected for cross-engine comparison and is the finding",
    }
    hypothesis = dict(HYPOTHESES[case["name"]])
    hypothesis["status"] = "HYPOTHESIS"
    return {
        "schema": SCHEMA,
        "issue": ISSUE_URL,
        "case": case["name"],
        "rationale": case["rationale"],
        "render_config": {"sample_rate": RATE, "seconds": SECONDS,
                          "events": events,
                          "event_policy": "positions are exact 64-sample "
                                          "block multiples; identical "
                                          "effective times for both "
                                          "engines"},
        "engines": identities,
        "expected_verdict": "FAIL (cross-engine exactness failure is the "
                            "expected result; exactness PASS applies only "
                            "within one implementation)",
        "report": d,
        "diagnosis": diag,
        "hypothesis": hypothesis,
        "non_claims": [NON_ADJUDICATION],
    }


def validate_case_report(wrapper: dict) -> list[str]:
    """Schema validator for r07-disagreement-case/1.

    Encodes the issue's negative control: a report that asserted
    'implementation X is wrong' (or omitted the non-adjudication
    statement / hypothesis labeling) must FAIL validation. Used by
    tests/test_r07.py.
    """
    problems = []
    if wrapper.get("schema") != SCHEMA:
        problems.append("schema field missing or wrong")
    if wrapper.get("issue") != ISSUE_URL:
        problems.append("issue field missing or wrong")
    for key in ("case", "rationale", "render_config", "engines", "report",
                "diagnosis", "hypothesis"):
        if key not in wrapper:
            problems.append(f"missing key: {key}")
    if "engines" in wrapper:
        for side in ("reference", "second"):
            eng = wrapper["engines"].get(side, {})
            if not eng.get("identity_id") or not eng.get("binary_sha256"):
                problems.append(f"engines.{side} lacks identity_id/"
                                "binary_sha256")
    rep = wrapper.get("report") or {}
    if rep.get("schema") != "paired-comparison-report":
        problems.append("embedded report is not a paired-comparison-report")
    if rep.get("verdict") != "FAIL":
        problems.append(
            "cross-engine report verdict must be FAIL (expected "
            "exactness failure); a PASS here would indicate engine "
            "mixing or identical engines")
    if rep.get("primary_role") != compare.PRIMARY_ROLE:
        problems.append("primary role statement missing")
    if rep.get("secondary_role") != compare.SECONDARY_ROLE:
        problems.append("secondary role statement missing")
    hyp = wrapper.get("hypothesis") or {}
    if hyp.get("status") != "HYPOTHESIS":
        problems.append("hypothesis must be labeled status=HYPOTHESIS")
    if not hyp.get("seam"):
        problems.append("hypothesis must name a behavior/seam")
    if NON_ADJUDICATION not in (wrapper.get("non_claims") or []):
        problems.append("non-adjudication statement missing")
    return problems


# --------------------------------------------------------------------
# Driver
# --------------------------------------------------------------------

def run(evidence_dir: Path, archive_path: Path | None, seconds: float,
        only_cases: list[str] | None = None,
        events: list[dict] | None = None) -> tuple[int, dict]:
    events = CASE_EVENTS if events is None else events
    ref_check = _check_identity(REFERENCE_IDENTITY)
    second_check = _check_identity(SECOND_IDENTITY)
    for name, check in (("reference", ref_check), ("second", second_check)):
        if check["status"] != "PASS":
            print(f"r07_compare: NOT_RUN — {name} oracle identity check: "
                  f"{check['status']}: {check['detail']}", file=sys.stderr)
            return 2, {"statuses": {name: check}}

    ref_identity = json.loads(
        REFERENCE_IDENTITY.read_text(encoding="utf-8"))
    second_identity = json.loads(
        SECOND_IDENTITY.read_text(encoding="utf-8"))
    identities = {
        "reference": {
            "identity_id": ref_identity["id"],
            "binary_sha256": ref_check["binary_sha256_actual"],
            "source_git_commit": ref_identity["source"]["git_commit"],
            "upstream": ref_identity["upstream"]["project"],
            "engine_mode": ref_identity["upstream"]["engine_mode"],
        },
        "second": {
            "identity_id": second_identity["id"],
            "binary_sha256": second_check["binary_sha256_actual"],
            "source_git_commit": second_identity["source"]["git_commit"],
            "upstream": second_identity["upstream"]["project"],
            "engine_mode": second_identity["engine"]["mode"],
        },
    }

    cases, notes = build_cases(archive_path)
    if only_cases:
        cases = [c for c in cases if c["name"] in only_cases]
        wanted = set(only_cases)
        missing = [n for n in only_cases if n not in {c["name"] for c in cases}]
        for n in missing:
            notes.append(f"{n}: NOT_RUN (case not built; see earlier notes)")

    evidence_dir.mkdir(parents=True, exist_ok=True)
    summary = {
        "schema": "r07-disagreement-summary/1",
        "issue": ISSUE_URL,
        "generated_by": "tools/r07_compare.py",
        "identities": identities,
        "identity_checks": {"reference": ref_check, "second": second_check},
        "render_config": {"sample_rate": RATE, "seconds": seconds,
                          "events": events},
        "cases": {},
        "notes": notes,
        "non_claims": [NON_ADJUDICATION],
    }

    failures = 0
    for case in cases:
        name = case["name"]
        if name in ("corpus-shrpsynth",) and any(
                n.startswith(name) and "NOT_RUN" in n for n in notes):
            continue
        if only_cases and name not in wanted:
            continue
        cdir = evidence_dir / name
        (cdir / "inputs").mkdir(parents=True, exist_ok=True)
        is_corpus = "corpus_provenance" in case
        voice = sysex.encode_voice(case["voice"])
        if is_corpus:
            # corpus policy: NO third-party voice bytes in evidence
            (cdir / "inputs" / "corpus-source.json").write_text(
                json.dumps(case["corpus_provenance"], indent=2,
                           sort_keys=True) + "\n", encoding="utf-8")
        else:
            (cdir / "inputs" / "voice.syx").write_bytes(voice)
        events_txt = events_text(events)
        (cdir / "inputs" / "events.txt").write_text(events_txt,
                                                    encoding="ascii")

        case_summary = {"status": "PASS", "notes": []}
        try:
            ref_render = _run_oracle(
                Path(ref_identity["binary"]["path"]), voice, events_txt,
                cdir / "render-ref.f32", cdir / "render-ref-meta.json",
                seconds, "reference")
            second_render = _run_oracle(
                Path(second_identity["binary"]["path"]), voice, events_txt,
                cdir / "render-second.f32",
                cdir / "render-second-meta.json", seconds, "second")
        except RenderError as exc:
            print(f"r07_compare: FAIL — {name}: {exc}", file=sys.stderr)
            summary["cases"][name] = {
                "status": "FAIL", "error": str(exc)}
            failures += 1
            continue

        ref_bytes = (cdir / "render-ref.f32").read_bytes()
        second_bytes = (cdir / "render-second.f32").read_bytes()
        wrapper = compare_case(
            case, ref_bytes, second_bytes,
            json.loads((cdir / "render-ref-meta.json").read_text()),
            json.loads((cdir / "render-second-meta.json").read_text()),
            identities, events)
        wrapper["renders"] = {
            "reference": {"path": "render-ref.f32",
                          "sha256": hashlib.sha256(ref_bytes).hexdigest(),
                          "bytes": len(ref_bytes),
                          "peak_abs": ref_render["peak_abs"]},
            "second": {"path": "render-second.f32",
                       "sha256": hashlib.sha256(second_bytes).hexdigest(),
                       "bytes": len(second_bytes),
                       "peak_abs": second_render["peak_abs"]},
        }
        wrapper["render_run_logs"] = {"reference": ref_render,
                                      "second": second_render}
        problems = validate_case_report(wrapper)
        if problems:
            case_summary["status"] = "FAIL"
            case_summary["notes"] = problems
            failures += 1
        (cdir / "compare.json").write_text(
            json.dumps(wrapper, indent=2, sort_keys=True) + "\n",
            encoding="utf-8")
        d = wrapper["diagnosis"]
        p = wrapper["report"]["primary"] or {}
        print(f"r07_compare: {name}: diverge_from="
              f"{p.get('first_divergent_sample')} "
              f"({d['divergence_class']}), rms_error="
              f"{p.get('rms_error'):.6g}, magnitude="
              f"{d['magnitude_class']}, snr_db={p.get('snr_db')}")
        summary["cases"][name] = case_summary

    total = len(cases)
    summary["overall"] = {
        "cases_completed": total - failures - len(
            [n for n in notes if "NOT_RUN" in n]),
        "cases_failed": failures,
        "cases_not_run": len([n for n in notes if "NOT_RUN" in n]),
    }
    (evidence_dir / "summary.json").write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n",
        encoding="utf-8")
    if failures:
        return 1, summary
    return 0, summary


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="R07 targeted oracle-disagreement driver (issue #14)")
    parser.add_argument("--evidence-dir", type=Path, default=DEFAULT_EVIDENCE,
                        help="evidence output directory")
    parser.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE,
                        help="pinned DX7_AllTheWeb.zip for the corpus case")
    parser.add_argument("--no-archive", action="store_true",
                        help="run synthetic cases only (corpus case "
                             "recorded NOT_RUN)")
    parser.add_argument("--seconds", type=float, default=SECONDS,
                        help="render length in seconds (default 3.0)")
    parser.add_argument("--cases", nargs="*", default=None,
                        help="subset of case names to run")
    parser.add_argument("--fixture", action="store_true",
                        help="tiny end-to-end validation run: first "
                             "synthetic case at 0.25 s into a temp dir; "
                             "used by tests/test_r07.py")
    args = parser.parse_args(argv)

    if args.fixture:
        ref_check = _check_identity(REFERENCE_IDENTITY)
        second_check = _check_identity(SECOND_IDENTITY)
        if ref_check["status"] != "PASS":
            print(f"r07_compare fixture: skip — reference oracle "
                  f"unavailable ({ref_check['detail']})", file=sys.stderr)
            return 2
        if second_check["status"] != "PASS":
            print(f"r07_compare fixture: skip — second oracle unavailable "
                  f"({second_check['detail']})", file=sys.stderr)
            return 2
        with tempfile.TemporaryDirectory(prefix="r07-fixture-") as tmp:
            fixture_events = [
                {"cmd": "note_on", "args": [60, 100], "pos": 4800},
                {"cmd": "note_off", "args": [60], "pos": 11520},
            ]
            code, summary = run(
                Path(tmp), None, 0.256, only_cases=["fb-alg4"],
                events=fixture_events)
            problems = []
            case_json = Path(tmp) / "fb-alg4" / "compare.json"
            if case_json.exists():
                wrapper = json.loads(case_json.read_text(encoding="utf-8"))
                problems = validate_case_report(wrapper)
            else:
                problems = ["fixture produced no compare.json"]
            if code or problems:
                print(f"r07_compare fixture: FAIL ({code}, {problems})",
                      file=sys.stderr)
                return 1
            print(f"r07_compare fixture: OK "
                  f"(cases={list(summary['cases'])})")
            return 0

    archive = None if args.no_archive else args.archive
    code, _ = run(args.evidence_dir, archive, args.seconds, args.cases)
    return code


if __name__ == "__main__":
    sys.exit(main())
