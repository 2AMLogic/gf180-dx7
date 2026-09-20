"""R04 exact paired comparator for f32le PCM streams (issue #11).

Contract (normative elaboration in docs/COMPARE-CONTRACT.md):

  * PRIMARY rows are time-locked. Index 0 is the declared common time
    origin. This comparator NEVER aligns, trims, crops, pads, time-warps,
    resamples, or normalizes either stream, and never switches a
    per-patch reference: comparison renders are dry (plan section 7,
    reference/oracle-protocol.md section 4). A length mismatch is a
    failure class, never something to work around.
  * The verdict for the reference path is EXACTNESS only: PASS iff the
    two streams are byte-identical, or (exact_mode, the default)
    numerically identical -- for finite IEEE-754 binary32 values these
    differ only by the sign of zero. Error-budgeted comparisons
    (model-vs-reference) are out of scope here and arrive later.
  * Explicit failure classes, never silent comparisons:
      EMPTY_INPUT     -- one or both streams contain zero samples
      LENGTH_MISMATCH -- decodable streams with unequal sample counts
      NON_FINITE      -- NaN/infinity in either stream (first index
                         recorded per stream)
    An input whose byte length is not a multiple of 4 cannot be decoded
    as f32le at all: compare_streams raises NotRunnable (CLI exit 2,
    could-not-run), which is NOT_RUN, never a pass.
  * SECONDARY diagnostics (onset-time delta, envelope-shape proxy,
    zero-crossing-rate spectral-character proxy) aid triage ONLY. Every
    report labels them secondary, and they are never sufficient for a
    verdict: PASS is computed exclusively from time-locked sample
    identity.

Primary rows (error e[i] = b[i] - a[i], "candidate - reference"):
  first_divergent_sample, mismatch_count, exact_equal,
  max_abs_error, max_abs_error_index (earliest tie wins), mean_bias
  (mean signed error), rms_error, snr_db, and per-stream peak_abs /
  peak_index / dc_offset / rms. snr_db is epsilon-guarded by case
  analysis (PAIRED-METRICS.md table): degenerate silent cases are null
  with an explicit reason, never a fabricated finite dB value.

Adaptation provenance: the measurement discipline is adapted from the
2AMLogic/gf180-torchsynth paired-metrics component
(src/torchsynth_voice/paired_metrics.py + spec/PAIRED-METRICS.md,
pinned at 36bdc67faa1ff2509988b772efcca7a7eb4e7115, Apache-2.0;
catalog row "paired-comparator" in docs/REUSE-RULINGS.md and
docs/reuse/catalog.json). It is re-implemented here stdlib-only with no
upstream bytes imported (DR-0002: hash equality never qualifies an
adaptation). Accumulation follows the cited "scaled-fsum-binary64-v1"
convention. Requalification: tests/test_compare.py plus
tools/qualify_compare.py (evidence under reference/evidence/compare/).

Stdlib only.
"""

from __future__ import annotations

import hashlib
import math
import struct
from dataclasses import dataclass

VERSION = "1"
ESTIMATOR_NAME = "exact-time-locked-paired"

ONSET_THRESHOLD = 1e-5
ENVELOPE_WINDOW_SECONDS = 0.05
ENVELOPE_FLOOR = 1e-12

FAILURE_EMPTY = "EMPTY_INPUT"
FAILURE_LENGTH = "LENGTH_MISMATCH"
FAILURE_NON_FINITE = "NON_FINITE"

SECONDARY_ROLE = ("SECONDARY - triage only; never sufficient for PASS")
PRIMARY_ROLE = ("PRIMARY - time-locked; no alignment, trimming, "
                "resampling, or normalization")


class NotRunnable(Exception):
    """Inputs cannot be interpreted as f32le PCM (apparatus; CLI exit 2)."""


def f32_bytes(samples) -> bytes:
    """Pack a finite float sequence as little-endian binary32 PCM bytes."""
    return struct.pack("<%df" % len(samples), *[float(v) for v in samples])


def sine_f32(n_samples: int, freq_hz: float, amplitude: float,
             phase_rad: float = 0.0, sample_rate: int = 48000) -> bytes:
    """Analytic fixture: a pure sine as f32le bytes (stdlib math/struct)."""
    if n_samples < 0:
        raise ValueError("n_samples must be nonnegative")
    return f32_bytes([
        amplitude * math.sin(2.0 * math.pi * freq_hz * i / sample_rate
                             + phase_rad)
        for i in range(n_samples)
    ])


def _scaled_fsum(values) -> float:
    """scaled-fsum-binary64-v1: scale by max|v|, fsum, restore units."""
    scale = 0.0
    for v in values:
        a = -v if v < 0 else v
        if a > scale:
            scale = a
    if scale == 0.0:
        return 0.0
    return math.fsum(v / scale for v in values) * scale


def _stream_stats(values) -> dict:
    n = len(values)
    peak, peak_index = 0.0, 0
    for i, v in enumerate(values):
        a = -v if v < 0 else v
        if a > peak:
            peak, peak_index = a, i
    dc = _scaled_fsum(values) / n if n else 0.0
    rms = (math.sqrt(_scaled_fsum([v * v for v in values]) / n)
           if n else 0.0)
    return {"sample_count": n, "peak_abs": peak, "peak_index": peak_index,
            "dc_offset": dc, "rms": rms}


def _snr_db(ref_rms: float, err_rms: float):
    """Log-difference SNR with silent-case refusal (no fabricated dB)."""
    if err_rms == 0.0 and ref_rms == 0.0:
        return None, "undefined_both_silent"
    if err_rms == 0.0:
        return None, "zero_error_infinite_snr"
    if ref_rms == 0.0:
        return None, "silent_reference_negative_infinity"
    return 20.0 * (math.log10(ref_rms) - math.log10(err_rms)), "measured"


def _paired_primary(a, b, ref_rms: float) -> dict:
    n = len(a)
    err = [b[i] - a[i] for i in range(n)]
    first_divergent = None
    mismatch_count = 0
    max_abs_error, max_abs_index = 0.0, 0
    for i, e in enumerate(err):
        if a[i] != b[i]:
            mismatch_count += 1
            if first_divergent is None:
                first_divergent = i
        ae = -e if e < 0 else e
        if ae > max_abs_error:
            max_abs_error, max_abs_index = ae, i
    mean_bias = _scaled_fsum(err) / n
    rms_error = math.sqrt(_scaled_fsum([e * e for e in err]) / n)
    snr, snr_reason = _snr_db(ref_rms, rms_error)
    return {
        "exact_equal": mismatch_count == 0,
        "mismatch_count": mismatch_count,
        "first_divergent_sample": first_divergent,
        "max_abs_error": max_abs_error,
        "max_abs_error_index": max_abs_index if mismatch_count else None,
        "mean_bias": mean_bias,
        "rms_error": rms_error,
        "snr_db": snr,
        "snr_reason": snr_reason,
        "error_sign": "candidate_minus_reference",
    }


def _onset_index(values) -> int | None:
    for i, v in enumerate(values):
        if (v if v >= 0 else -v) > ONSET_THRESHOLD:
            return i
    return None


def _zero_crossing_rate(values) -> float:
    if len(values) < 2:
        return 0.0
    crossings = sum(
        1 for i in range(len(values) - 1)
        if (values[i] >= 0.0) != (values[i + 1] >= 0.0))
    return crossings / (len(values) - 1)


def _secondary(a, b, sample_rate: int) -> dict:
    n = len(a)
    window = max(1, round(sample_rate * ENVELOPE_WINDOW_SECONDS))
    window_rms_a, window_rms_b = [], []
    max_rel_diff, argmax_start = 0.0, None
    for start in range(0, n, window):
        stop = min(start + window, n)
        count = stop - start
        ra = math.sqrt(math.fsum(v * v for v in a[start:stop]) / count)
        rb = math.sqrt(math.fsum(v * v for v in b[start:stop]) / count)
        window_rms_a.append(ra)
        window_rms_b.append(rb)
        rel = abs(ra - rb) / max(ra, rb, ENVELOPE_FLOOR)
        if rel > max_rel_diff:
            max_rel_diff, argmax_start = rel, start
    onset_a = _onset_index(a)
    onset_b = _onset_index(b)
    onset_delta = (onset_b - onset_a
                   if onset_a is not None and onset_b is not None else None)
    zcr_a = _zero_crossing_rate(a)
    zcr_b = _zero_crossing_rate(b)
    return {
        "role": SECONDARY_ROLE,
        "onset_time": {
            "threshold_abs": ONSET_THRESHOLD,
            "sample_a": onset_a,
            "sample_b": onset_b,
            "delta_samples": onset_delta,
        },
        "envelope_shape_proxy": {
            "window_samples": window,
            "window_seconds": window / sample_rate,
            "n_windows": len(window_rms_a),
            "max_relative_rms_diff": max_rel_diff,
            "argmax_window_start": argmax_start,
        },
        "spectral_character_proxy": {
            "metric": "zero_crossing_rate",
            "rate_a": zcr_a,
            "rate_b": zcr_b,
            "abs_diff": abs(zcr_a - zcr_b),
        },
    }


@dataclass(frozen=True)
class Report:
    verdict: str
    verdict_reason: str
    failure_class: str | None
    failure_detail: dict | None
    byte_identical: bool
    exact_mode: bool
    sample_rate: int
    n_samples_a: int
    n_samples_b: int
    sha256_a: str
    sha256_b: str
    stats_a: dict | None
    stats_b: dict | None
    stats_not_computed_reason: str | None
    primary: dict | None
    primary_not_computed_reason: str | None
    secondary: dict | None
    secondary_not_computed_reason: str | None

    @property
    def passed(self) -> bool:
        return self.verdict == "PASS"

    def to_dict(self) -> dict:
        return {
            "schema": "paired-comparison-report",
            "schema_version": 1,
            "estimator": {"name": ESTIMATOR_NAME, "version": VERSION},
            "verdict": self.verdict,
            "verdict_reason": self.verdict_reason,
            "failure_class": self.failure_class,
            "failure_detail": self.failure_detail,
            "byte_identical": self.byte_identical,
            "exact_mode": self.exact_mode,
            "inputs": {
                "sample_rate": self.sample_rate,
                "sample_count_a": self.n_samples_a,
                "sample_count_b": self.n_samples_b,
                "sha256_a": self.sha256_a,
                "sha256_b": self.sha256_b,
                "format": "pcm_f32le mono (raw, headerless)",
            },
            "stream_stats": {
                "a": self.stats_a,
                "b": self.stats_b,
                "not_computed_reason": self.stats_not_computed_reason,
            },
            "primary": self.primary,
            "primary_role": PRIMARY_ROLE if self.primary is not None else None,
            "primary_not_computed_reason": self.primary_not_computed_reason,
            "secondary": self.secondary,
            "secondary_role": (SECONDARY_ROLE
                               if self.secondary is not None else None),
            "secondary_not_computed_reason":
                self.secondary_not_computed_reason,
        }


def _failure(failure_class, detail, sha_a, sha_b, exact_mode, sample_rate,
             n_a, n_b, stats_a, stats_b,
             stats_not_computed_reason=None) -> Report:
    return Report(
        verdict="FAIL", verdict_reason=failure_class,
        failure_class=failure_class, failure_detail=detail,
        byte_identical=False, exact_mode=exact_mode,
        sample_rate=sample_rate, n_samples_a=n_a, n_samples_b=n_b,
        sha256_a=sha_a, sha256_b=sha_b,
        stats_a=stats_a, stats_b=stats_b,
        stats_not_computed_reason=stats_not_computed_reason,
        primary=None, primary_not_computed_reason=failure_class,
        secondary=None, secondary_not_computed_reason=failure_class)


def compare_streams(a: bytes, b: bytes, sample_rate: int = 48000,
                    exact_mode: bool = True) -> Report:
    """Exactly compare two raw f32le mono PCM byte streams, time-locked.

    Raises NotRunnable when either input cannot be decoded as f32le
    (byte length not a multiple of 4) or sample_rate is invalid.
    Otherwise returns a Report; see the module docstring for the
    contract (no alignment/trim/resample/normalization, exactness
    verdict, explicit failure classes, secondary-never-passes).
    """
    if not isinstance(sample_rate, int) or isinstance(sample_rate, bool) \
            or sample_rate <= 0:
        raise NotRunnable(f"invalid sample_rate label: {sample_rate!r}")
    for name, data in (("a", a), ("b", b)):
        if not isinstance(data, (bytes, bytearray)):
            raise NotRunnable(f"stream {name!r} must be raw bytes")
        if len(data) % 4:
            raise NotRunnable(
                f"stream {name!r}: byte length {len(data)} is not a "
                "multiple of 4 (not decodable as f32le)")
    a, b = bytes(a), bytes(b)
    sha_a = hashlib.sha256(a).hexdigest()
    sha_b = hashlib.sha256(b).hexdigest()
    n_a, n_b = len(a) // 4, len(b) // 4
    values_a = struct.unpack("<%df" % n_a, a)
    values_b = struct.unpack("<%df" % n_b, b)

    if n_a == 0 or n_b == 0:
        empty = [name for name, n in (("a", n_a), ("b", n_b)) if n == 0]
        return _failure(
            FAILURE_EMPTY, {"empty_streams": empty}, sha_a, sha_b,
            exact_mode, sample_rate, n_a, n_b,
            _stream_stats(values_a), _stream_stats(values_b))

    first_bad = {"a": None, "b": None}
    for name, values in (("a", values_a), ("b", values_b)):
        for i, v in enumerate(values):
            if not math.isfinite(v):
                first_bad[name] = i
                break
    if first_bad["a"] is not None or first_bad["b"] is not None:
        return _failure(
            FAILURE_NON_FINITE, {"first_non_finite_sample": first_bad},
            sha_a, sha_b, exact_mode, sample_rate, n_a, n_b,
            None, None, "non_finite_input")

    if n_a != n_b:
        return _failure(
            FAILURE_LENGTH,
            {"sample_count_delta": n_b - n_a,
             "note": "no common prefix is scored; streams are not "
                     "aligned, trimmed, or padded"},
            sha_a, sha_b, exact_mode, sample_rate, n_a, n_b,
            _stream_stats(values_a), _stream_stats(values_b))

    stats_a = _stream_stats(values_a)
    stats_b = _stream_stats(values_b)
    primary = _paired_primary(values_a, values_b, stats_a["rms"])
    secondary = _secondary(values_a, values_b, sample_rate)
    byte_identical = a == b
    if byte_identical:
        verdict, reason = "PASS", "byte_identical"
    elif primary["exact_equal"] and exact_mode:
        verdict = "PASS"
        reason = "numerically_identical (non-byte-identical sign-of-zero only)"
    else:
        verdict, reason = "FAIL", "samples_differ"
    if not exact_mode and not byte_identical:
        reason = "samples_differ (byte-exact mode)"
    return Report(
        verdict=verdict, verdict_reason=reason, failure_class=None,
        failure_detail=None, byte_identical=byte_identical,
        exact_mode=exact_mode, sample_rate=sample_rate,
        n_samples_a=n_a, n_samples_b=n_b,
        sha256_a=sha_a, sha256_b=sha_b,
        stats_a=stats_a, stats_b=stats_b, stats_not_computed_reason=None,
        primary=primary, primary_not_computed_reason=None,
        secondary=secondary, secondary_not_computed_reason=None)
