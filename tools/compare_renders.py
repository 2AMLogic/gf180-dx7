#!/usr/bin/env python3
"""R04 CLI: exact paired comparison of two f32le PCM render files (issue #11).

  compare_renders.py <a.f32> <b.f32> [--json OUT] [--sample-rate N]
                     [--byte-exact]

Exit codes: 0 PASS (streams identical), 1 FAIL (samples differ, or an
explicit failure class: EMPTY_INPUT / LENGTH_MISMATCH / NON_FINITE),
2 NOT_RUN (apparatus: missing/unreadable input, or a byte length that is
not a multiple of 4 and therefore not decodable as f32le). Exit 2 is
never a pass.

The comparison is time-locked and dry: no alignment, trimming,
resampling, or level normalization on primary rows; verdicts are
exactness only. See docs/COMPARE-CONTRACT.md and
src/gf180_dx7/compare.py. Stdlib-only.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import compare  # noqa: E402


def _read(path: Path) -> bytes:
    return path.read_bytes()


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Exact paired comparison of two f32le renders (R04)")
    parser.add_argument("a", type=Path, help="first raw f32le PCM file")
    parser.add_argument("b", type=Path, help="second raw f32le PCM file")
    parser.add_argument("--json", type=Path, default=None,
                        help="write the full comparison report JSON here")
    parser.add_argument("--sample-rate", type=int, default=48000,
                        help="sample-rate label recorded in the report "
                             "(default: 48000)")
    parser.add_argument("--byte-exact", action="store_true",
                        help="require byte equality instead of numeric "
                             "identity (sign-of-zero no longer excused)")
    args = parser.parse_args(argv)

    try:
        data_a = _read(args.a)
        data_b = _read(args.b)
    except OSError as exc:
        print(f"compare_renders: NOT_RUN — cannot read inputs: {exc}",
              file=sys.stderr)
        return 2

    try:
        report = compare.compare_streams(
            data_a, data_b, sample_rate=args.sample_rate,
            exact_mode=not args.byte_exact)
    except compare.NotRunnable as exc:
        print(f"compare_renders: NOT_RUN — {exc}", file=sys.stderr)
        return 2

    if args.json is not None:
        try:
            args.json.write_text(
                json.dumps(report.to_dict(), indent=2, sort_keys=True)
                + "\n", encoding="utf-8")
        except OSError as exc:
            print(f"compare_renders: NOT_RUN — cannot write {args.json}: "
                  f"{exc}", file=sys.stderr)
            return 2

    d = report.to_dict()
    primary = d["primary"] or {}
    print(f"compare_renders: {report.verdict} — {report.verdict_reason}")
    print(f"  inputs: {args.a} ({report.n_samples_a} samples) vs "
          f"{args.b} ({report.n_samples_b} samples), "
          f"sample_rate {report.sample_rate}")
    if report.failure_class:
        print(f"  failure_class: {report.failure_class} "
              f"{report.failure_detail}")
    if primary:
        print(f"  byte_identical: {report.byte_identical}, "
              f"first_divergent_sample: "
              f"{primary['first_divergent_sample']}, "
              f"mismatch_count: {primary['mismatch_count']}")
        print(f"  max_abs_error: {primary['max_abs_error']:.6g} at sample "
              f"{primary['max_abs_error_index']}, mean_bias: "
              f"{primary['mean_bias']:.6g}, rms_error: "
              f"{primary['rms_error']:.6g}, snr_db: "
              f"{primary['snr_db']} ({primary['snr_reason']})")
    if report.secondary:
        sec = report.secondary
        print(f"  secondary (triage only): onset delta "
              f"{sec['onset_time']['delta_samples']} samples, envelope "
              f"max rel diff {sec['envelope_shape_proxy']['max_relative_rms_diff']:.3g}, "
              f"ZCR diff {sec['spectral_character_proxy']['abs_diff']:.3g}")
    if args.json is not None:
        print(f"  report: {args.json}")
    return 0 if report.passed else 1


if __name__ == "__main__":
    sys.exit(main())
