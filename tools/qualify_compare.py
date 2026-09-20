#!/usr/bin/env python3
"""R04 comparator qualification report generator (issue #11 evidence).

Builds analytic fixtures (stdlib math/struct sines) and known-bad
mutations, runs the exact comparator (library, CLI, and metadata
cross-check) against each, and records expected vs actual verdicts with
localization. The report is written to
reference/evidence/compare/qualification-r04.json by default and exit
code is 0 only when every case resolves exactly as expected; any
divergence exits 1. Apparatus negative controls (missing input file,
odd byte count) must yield exit 2 / NotRunnable — never a pass.

The report establishes apparatus qualification only: the comparator
demonstrably detects each known-bad mutation class. It makes no
fidelity, synthesis, or musical-quality claim. tests/test_compare.py
independently enforces the same expectations as live regression
controls. Stdlib-only.
"""

from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare  # noqa: E402
import compare_evidence  # noqa: E402

COMPARE_RENDERS = REPO / "tools" / "compare_renders.py"
DEFAULT_OUT = REPO / "reference" / "evidence" / "compare" / "qualification-r04.json"

RATE = 48000
N = 9600
FREQ = 220.0
AMP = 0.5
FLIP_SAMPLE = 1000
NAN_SAMPLE = 500
INF_SAMPLE = 700


def unpack(data: bytes) -> list:
    return list(struct.unpack("<%df" % (len(data) // 4), data))


def library_case(name, expect, a_bytes, b_bytes):
    def run():
        report = compare.compare_streams(a_bytes, b_bytes, sample_rate=RATE)
        d = report.to_dict()
        checks = [("verdict", d["verdict"] == expect["verdict"])]
        if "failure_class" in expect:
            checks.append(("failure_class",
                           d["failure_class"] == expect["failure_class"]))
        primary = d["primary"] or {}
        detail = d["failure_detail"] or {}
        if "first_divergent_in" in expect:
            checks.append((
                "first_divergent_sample",
                primary.get("first_divergent_sample")
                in expect["first_divergent_in"]))
        if "max_abs_error_index" in expect:
            checks.append(("max_abs_error_index",
                           primary.get("max_abs_error_index")
                           == expect["max_abs_error_index"]))
        if "max_abs_error_abs" in expect:
            target, tol = expect["max_abs_error_abs"]
            checks.append(("max_abs_error_within_tolerance",
                           abs(primary.get("max_abs_error", 0.0) - target)
                           <= tol))
        if "mean_bias_abs" in expect:
            target, tol = expect["mean_bias_abs"]
            checks.append(("mean_bias_within_tolerance",
                           abs(primary.get("mean_bias", 0.0) - target)
                           <= tol))
        if "byte_identical" in expect:
            checks.append(("byte_identical",
                           d["byte_identical"] == expect["byte_identical"]))
        if "sample_count_delta" in expect:
            checks.append(("sample_count_delta",
                           detail.get("sample_count_delta")
                           == expect["sample_count_delta"]))
        if "first_non_finite" in expect:
            checks.append(("first_non_finite_sample",
                           detail.get("first_non_finite_sample")
                           == expect["first_non_finite"]))
        return {"kind": "library", "actual": {
            "verdict": d["verdict"],
            "verdict_reason": d["verdict_reason"],
            "failure_class": d["failure_class"],
            "failure_detail": detail,
            "primary": primary,
            "byte_identical": d["byte_identical"],
        }}, checks
    return name, expect, run


def cli_case(name, expect, a_path: Path, b_path: Path, report_path: Path):
    def run():
        proc = subprocess.run(
            [sys.executable, str(COMPARE_RENDERS),
             str(a_path), str(b_path), "--json", str(report_path)],
            capture_output=True, text=True)
        checks = [("cli_exit_code", proc.returncode == expect["exit"])]
        if "verdict" in expect:
            report_verdict = None
            if report_path.is_file():
                report_verdict = json.loads(
                    report_path.read_text(encoding="utf-8"))["verdict"]
            checks.append(("json_report_verdict",
                           report_verdict == expect["verdict"]))
        return {"kind": "cli", "actual": {
            "exit_code": proc.returncode,
        }}, checks
    return name, expect, run


def metadata_case(name, expect, committed_meta: dict, tampered_meta: dict):
    def run():
        disagreements = compare_evidence.check_metadata(
            committed_meta, tampered_meta)
        checks = [("disagreement_listed", any(
            expect["field"] in d for d in disagreements))]
        return {"kind": "metadata", "actual": {
            "disagreements": disagreements}}, checks
    return name, expect, run


def run_cases(tmp: Path) -> list:
    base = compare.sine_f32(N, FREQ, AMP, sample_rate=RATE)
    base_samples = unpack(base)
    peak_index = max(range(len(base_samples)),
                     key=lambda i: abs(base_samples[i]))
    half = compare.f32_bytes([0.25, 0.0, -0.25, 0.5])
    sign_zero = compare.f32_bytes([0.25, -0.0, -0.25, 0.5])

    gain = compare.f32_bytes([s * 1.001 for s in base_samples])
    shifted = compare.f32_bytes(
        base_samples[1:] + base_samples[:1])
    dc = compare.f32_bytes([s + 0.01 for s in base_samples])
    flipped = bytearray(base)
    flipped[FLIP_SAMPLE * 4] ^= 0x01
    nan_samples = list(base_samples)
    nan_samples[NAN_SAMPLE] = float("nan")
    inf_samples = list(base_samples)
    inf_samples[INF_SAMPLE] = float("inf")

    good = tmp / "good.f32"
    good.write_bytes(base)
    gain_path = tmp / "gain.f32"
    gain_path.write_bytes(gain)
    odd = tmp / "odd.f32"
    odd.write_bytes(b"\x01\x02\x03")
    missing = tmp / "does-not-exist.f32"

    def run_cli_pair(a_path: Path, b_path: Path, expect):
        proc = subprocess.run(
            [sys.executable, str(COMPARE_RENDERS),
             str(a_path), str(b_path)],
            capture_output=True, text=True)
        return {"kind": "cli", "actual": {"exit_code": proc.returncode}}, [
            ("cli_exit_code", proc.returncode == expect["exit"])]

    cases = [
        library_case(
            "fixture_identical_sine",
            {"verdict": "PASS", "byte_identical": True}, base, base),
        library_case(
            "fixture_sign_of_zero_numeric_identity",
            {"verdict": "PASS", "byte_identical": False}, half, sign_zero),
        library_case(
            "mut_gain_x1.001",
            {"verdict": "FAIL", "first_divergent_in": (1,),
             "max_abs_error_abs": (AMP * 0.001, AMP * 0.001 * 0.1),
             "max_abs_error_index": peak_index},
            base, gain),
        library_case(
            "mut_shift_1_sample",
            {"verdict": "FAIL", "first_divergent_in": (0, 1)},
            base, shifted),
        library_case(
            "mut_dc_offset_+0.01",
            {"verdict": "FAIL", "mean_bias_abs": (0.01, 0.001)},
            base, dc),
        library_case(
            "mut_silent_tail_append_4800",
            {"verdict": "FAIL", "failure_class": "LENGTH_MISMATCH",
             "sample_count_delta": 4800},
            base, base + compare.f32_bytes([0.0] * 4800)),
        library_case(
            "mut_truncate_2400",
            {"verdict": "FAIL", "failure_class": "LENGTH_MISMATCH",
             "sample_count_delta": -2400},
            base, base[:len(base) - 2400 * 4]),
        library_case(
            "mut_one_byte_flip",
            {"verdict": "FAIL", "first_divergent_in": (FLIP_SAMPLE,),
             "max_abs_error_index": FLIP_SAMPLE},
            base, bytes(flipped)),
        library_case(
            "mut_nan_injection",
            {"verdict": "FAIL", "failure_class": "NON_FINITE",
             "first_non_finite": {"a": None, "b": NAN_SAMPLE}},
            base, compare.f32_bytes(nan_samples)),
        library_case(
            "mut_inf_injection",
            {"verdict": "FAIL", "failure_class": "NON_FINITE",
             "first_non_finite": {"a": None, "b": INF_SAMPLE}},
            base, compare.f32_bytes(inf_samples)),
    ]

    committed_meta = {
        "fixture": "smoke",
        "sample_rate": RATE,
        "output": {"samples": N, "bytes": N * 4, "sha256": "0" * 64,
                   "format": "pcm_f32le mono (raw, headerless)"},
        "inputs": {"voice": {"sha256": "0" * 64},
                   "events": {"sha256": "0" * 64}},
        "oracle_identity": {"binary": {"sha256": "0" * 64}},
        "determinism": {"byte_identical": True},
    }
    wrong_rate = json.loads(json.dumps(committed_meta))
    wrong_rate["sample_rate"] = 44100
    cases.append(metadata_case(
        "mut_wrong_sample_rate_label_metadata",
        {"field": "sample_rate"}, committed_meta, wrong_rate))

    cases.append(cli_case(
        "ctrl_identical_sine_cli", {"exit": 0, "verdict": "PASS"},
        good, good, tmp / "r_identical.json"))
    cases.append(cli_case(
        "ctrl_gain_mutation_cli", {"exit": 1, "verdict": "FAIL"},
        good, gain_path, tmp / "r_gain.json"))

    def missing_run():
        return run_cli_pair(missing, missing, {"exit": 2})
    cases.append(("ctrl_missing_input_file_exit2", {"exit": 2},
                  missing_run))

    def odd_run():
        return run_cli_pair(good, odd, {"exit": 2})
    cases.append(("ctrl_odd_byte_count_exit2", {"exit": 2}, odd_run))

    results = []
    for name, expect, run in cases:
        actual, checks = run()
        ok = all(passed for _, passed in checks)
        results.append({
            "case": name,
            "expected": dict(expect),
            "actual": actual,
            "checks": [{"check": c, "ok": ok_i} for c, ok_i in checks],
            "caught_as_expected": ok,
        })
    return results


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Generate the R04 comparator qualification report")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT,
                        help=f"report path (default: {DEFAULT_OUT})")
    args = parser.parse_args(argv)

    with tempfile.TemporaryDirectory(prefix="r04-qualify-") as tmp:
        results = run_cases(Path(tmp))
    all_ok = all(r["caught_as_expected"] for r in results)

    report = {
        "schema": "compare-qualification",
        "schema_version": 1,
        "issue": 11,
        "generated_at": datetime.datetime.now(
            datetime.timezone.utc).isoformat(),
        "tool": {
            "module": "src/gf180_dx7/compare.py",
            "estimator": compare.ESTIMATOR_NAME,
            "version": compare.VERSION,
            "module_sha256": hashlib.sha256(
                (REPO / "src" / "gf180_dx7" / "compare.py").read_bytes()
            ).hexdigest(),
            "python": sys.version.split()[0],
        },
        "fixture_params": {"sample_rate": RATE, "n_samples": N,
                           "freq_hz": FREQ, "amplitude": AMP},
        "claim_scope": ("apparatus qualification only: the comparator "
                        "detects each known-bad mutation class and never "
                        "passes invalid inputs. No fidelity, synthesis, "
                        "or musical-quality claim."),
        "cases": results,
        "summary": {
            "cases": len(results),
            "caught_as_expected": sum(r["caught_as_expected"]
                                      for r in results),
            "all_expected": all_ok,
        },
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n",
                        encoding="utf-8")
    for r in results:
        status = "ok" if r["caught_as_expected"] else "MISSED"
        print(f"qualify_compare: [{status:6s}] {r['case']}")
    print(f"qualify_compare: report written to {args.out}")
    print(f"qualify_compare: {'PASS' if all_ok else 'FAIL'} — "
          f"{report['summary']['caught_as_expected']}/{len(results)} "
          "cases as expected")
    return 0 if all_ok else 1


if __name__ == "__main__":
    sys.exit(main())
