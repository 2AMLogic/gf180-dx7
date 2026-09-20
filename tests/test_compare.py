"""Qualification tests for the R04 exact paired comparator (issue #11).

Analytic fixtures (stdlib math/struct generation) with known answers, a
mutation battery that must be caught with correct localization, the
metadata-level wrong-sample-rate-label control, and apparatus negative
controls that must produce exit 2 / NotRunnable — never a pass.
Secondary diagnostics are asserted insufficient to rescue a FAIL.

No oracle or network is required: everything is analytic.
"""

from __future__ import annotations

import importlib.util
import json
import math
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

from gf180_dx7 import compare  # noqa: E402

COMPARE_RENDERS = REPO / "tools" / "compare_renders.py"
COMPARE_EVIDENCE = REPO / "tools" / "compare_evidence.py"

RATE = 48000
N = 9600
FREQ = 220.0
AMP = 0.5


def load_tool(path: Path):
    spec = importlib.util.spec_from_file_location(path.stem, path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def run_cli(*extra: str) -> subprocess.CompletedProcess:
    return subprocess.run(
        [sys.executable, str(COMPARE_RENDERS), *extra],
        capture_output=True, text=True)


def sine() -> bytes:
    return compare.sine_f32(N, FREQ, AMP, sample_rate=RATE)


def samples_of(data: bytes) -> list:
    return list(struct.unpack("<%df" % (len(data) // 4), data))


class TestAnalyticFixtures(unittest.TestCase):
    """Known-answer fixtures: exactness verdicts and localized metrics."""

    def test_identical_sine_passes_byte_identical(self):
        a = sine()
        report = compare.compare_streams(a, a, sample_rate=RATE)
        self.assertEqual(report.verdict, "PASS")
        self.assertEqual(report.verdict_reason, "byte_identical")
        self.assertTrue(report.byte_identical)
        self.assertTrue(report.passed)
        self.assertIsNone(report.failure_class)
        primary = report.primary
        self.assertTrue(primary["exact_equal"])
        self.assertEqual(primary["mismatch_count"], 0)
        self.assertIsNone(primary["first_divergent_sample"])
        self.assertEqual(primary["max_abs_error"], 0.0)
        self.assertEqual(primary["rms_error"], 0.0)
        self.assertEqual(primary["mean_bias"], 0.0)
        self.assertIsNone(primary["snr_db"])
        self.assertEqual(primary["snr_reason"], "zero_error_infinite_snr")
        self.assertIsNotNone(report.secondary)
        self.assertEqual(report.secondary["role"],
                         compare.SECONDARY_ROLE)

    def test_stream_stats_match_analytic_values(self):
        report = compare.compare_streams(sine(), sine(), sample_rate=RATE)
        stats = report.stats_a
        self.assertEqual(stats["sample_count"], N)
        self.assertAlmostEqual(stats["peak_abs"], AMP, delta=1e-6)
        self.assertAlmostEqual(stats["rms"], AMP / math.sqrt(2), delta=1e-6)
        self.assertAlmostEqual(stats["dc_offset"], 0.0, delta=1e-9)
        self.assertEqual(report.sample_rate, RATE)

    def test_sign_of_zero_is_numeric_identity_not_byte_identity(self):
        a = compare.f32_bytes([0.25, 0.0, -0.25, 0.5])
        b = compare.f32_bytes([0.25, -0.0, -0.25, 0.5])
        self.assertNotEqual(a, b)
        report = compare.compare_streams(a, b, sample_rate=RATE)
        self.assertEqual(report.verdict, "PASS")
        self.assertFalse(report.byte_identical)
        self.assertIn("numerically_identical", report.verdict_reason)
        byte_mode = compare.compare_streams(a, b, exact_mode=False)
        self.assertEqual(byte_mode.verdict, "FAIL")

    def test_sample_rate_label_is_echoed_not_compared(self):
        a = sine()
        report = compare.compare_streams(a, a, sample_rate=44100)
        self.assertEqual(report.verdict, "PASS")
        self.assertEqual(report.to_dict()["inputs"]["sample_rate"], 44100)


class TestFailureClasses(unittest.TestCase):
    """Failure classes are explicit verdicts, never comparisons."""

    def test_nan_injection_is_non_finite_class(self):
        samples = samples_of(sine())
        samples[500] = float("nan")
        b = compare.f32_bytes(samples)
        report = compare.compare_streams(sine(), b, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        self.assertEqual(report.failure_class, "NON_FINITE")
        self.assertEqual(
            report.failure_detail["first_non_finite_sample"],
            {"a": None, "b": 500})
        self.assertFalse(report.passed)
        self.assertIsNone(report.primary)
        self.assertEqual(report.primary_not_computed_reason, "NON_FINITE")
        self.assertIsNone(report.stats_b)
        self.assertEqual(report.stats_not_computed_reason,
                         "non_finite_input")

    def test_inf_injection_is_non_finite_class(self):
        samples = samples_of(sine())
        samples[7] = float("inf")
        b = compare.f32_bytes(samples)
        report = compare.compare_streams(sine(), b, sample_rate=RATE)
        self.assertEqual(report.failure_class, "NON_FINITE")
        self.assertEqual(
            report.failure_detail["first_non_finite_sample"]["b"], 7)

    def test_length_mismatch_is_not_aligned_or_trimmed(self):
        b = sine() + compare.f32_bytes([0.0] * 4800)
        report = compare.compare_streams(sine(), b, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        self.assertEqual(report.failure_class, "LENGTH_MISMATCH")
        self.assertEqual(report.failure_detail["sample_count_delta"], 4800)
        self.assertIsNone(report.primary)
        self.assertIsNotNone(report.stats_a)
        self.assertIsNotNone(report.stats_b)
        self.assertIsNone(report.secondary)

    def test_empty_input_is_invalid_evidence_never_a_match(self):
        for a, b in ((b"", b""), (b"", sine()), (sine(), b"")):
            report = compare.compare_streams(a, b, sample_rate=RATE)
            self.assertEqual(report.verdict, "FAIL")
            self.assertEqual(report.failure_class, "EMPTY_INPUT")

    def test_not_runnable_odd_byte_count(self):
        with self.assertRaises(compare.NotRunnable):
            compare.compare_streams(sine(), b"\x01\x02\x03")
        with self.assertRaises(compare.NotRunnable):
            compare.compare_streams(b"\x01", sine())

    def test_not_runnable_invalid_sample_rate(self):
        with self.assertRaises(compare.NotRunnable):
            compare.compare_streams(sine(), sine(), sample_rate=0)
        with self.assertRaises(compare.NotRunnable):
            compare.compare_streams(sine(), sine(), sample_rate=-1)


class TestMutationLocalization(unittest.TestCase):
    """Each mutation is caught AND localized (no alignment happened)."""

    def test_one_sample_shift_diverges_immediately(self):
        samples = samples_of(sine())
        shifted = compare.f32_bytes(samples[1:] + samples[:1])
        report = compare.compare_streams(sine(), shifted, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        self.assertIn(report.primary["first_divergent_sample"], (0, 1))
        self.assertGreater(report.primary["mismatch_count"], N // 3)

    def test_gain_offset_max_abs_error_matches_expected(self):
        samples = samples_of(sine())
        gain = compare.f32_bytes([s * 1.001 for s in samples])
        report = compare.compare_streams(sine(), gain, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        peak_index = max(range(len(samples)), key=lambda i: abs(samples[i]))
        self.assertAlmostEqual(report.primary["max_abs_error"],
                               AMP * 0.001, delta=AMP * 0.001 * 0.1)
        self.assertEqual(report.primary["max_abs_error_index"], peak_index)
        self.assertAlmostEqual(report.primary["mean_bias"], 0.0,
                               delta=1e-4)

    def test_dc_offset_detected_via_mean_bias(self):
        samples = samples_of(sine())
        dc = compare.f32_bytes([s + 0.01 for s in samples])
        report = compare.compare_streams(sine(), dc, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        self.assertAlmostEqual(report.primary["mean_bias"], 0.01,
                               delta=1e-4)
        self.assertAlmostEqual(report.stats_b["dc_offset"], 0.01,
                               delta=1e-4)
        self.assertEqual(report.primary["first_divergent_sample"], 0)

    def test_one_byte_flip_localized_exactly(self):
        data = bytearray(sine())
        data[1000 * 4] ^= 0x01
        report = compare.compare_streams(sine(), bytes(data),
                                         sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        self.assertEqual(report.primary["first_divergent_sample"], 1000)
        self.assertEqual(report.primary["mismatch_count"], 1)
        self.assertEqual(report.primary["max_abs_error_index"], 1000)
        self.assertGreater(report.primary["max_abs_error"], 0.0)


class TestSecondaryNeverPasses(unittest.TestCase):
    """Secondary diagnostics aid triage; they never rescue a verdict."""

    def test_matching_secondary_cannot_rescue_gain_fail(self):
        samples = samples_of(sine())
        gain = compare.f32_bytes([s * 1.001 for s in samples])
        report = compare.compare_streams(sine(), gain, sample_rate=RATE)
        self.assertEqual(report.verdict, "FAIL")
        sec = report.secondary
        self.assertEqual(sec["onset_time"]["delta_samples"], 0)
        self.assertLess(sec["spectral_character_proxy"]["abs_diff"], 1e-9)
        self.assertGreater(
            sec["envelope_shape_proxy"]["max_relative_rms_diff"], 0.0)

    def test_secondary_structure(self):
        report = compare.compare_streams(sine(), sine(), sample_rate=RATE)
        sec = report.secondary
        self.assertEqual(sec["envelope_shape_proxy"]["window_samples"],
                         RATE // 20)
        self.assertEqual(sec["envelope_shape_proxy"]["n_windows"],
                         math.ceil(N / (RATE // 20)))
        self.assertEqual(sec["envelope_shape_proxy"]["max_relative_rms_diff"],
                         0.0)
        self.assertEqual(sec["onset_time"]["sample_a"], 1)
        self.assertEqual(sec["onset_time"]["delta_samples"], 0)
        self.assertAlmostEqual(sec["spectral_character_proxy"]["rate_a"],
                               sec["spectral_character_proxy"]["rate_b"])
        self.assertGreater(sec["spectral_character_proxy"]["rate_a"], 0.0)


class TestMutationBatteryCli(unittest.TestCase):
    """The CLI catches every mutation class with the right exit code."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="r04-test-")
        self.addCleanup(self.tmp.cleanup)
        self.dir = Path(self.tmp.name)
        self.good = self.dir / "good.f32"
        self.good.write_bytes(sine())

    def assert_cli(self, a: Path, b: Path, expected: int,
                   needle: str | None = None):
        proc = run_cli(str(a), str(b))
        self.assertEqual(proc.returncode, expected,
                         proc.stdout + proc.stderr)
        if needle is not None:
            self.assertIn(needle, proc.stdout + proc.stderr)
        return proc

    def test_identical_exit_zero(self):
        self.assert_cli(self.good, self.good, 0, "PASS")

    def test_gain_offset_exit_one(self):
        other = self.dir / "gain.f32"
        other.write_bytes(
            compare.f32_bytes([s * 1.001 for s in samples_of(sine())]))
        self.assert_cli(self.good, other, 1, "FAIL")

    def test_sample_shift_exit_one(self):
        other = self.dir / "shift.f32"
        other.write_bytes(
            compare.f32_bytes(samples_of(sine())[1:] + [0.0]))
        self.assert_cli(self.good, other, 1)

    def test_silent_tail_append_is_length_class(self):
        other = self.dir / "tail.f32"
        other.write_bytes(sine() + compare.f32_bytes([0.0] * 4800))
        proc = self.assert_cli(self.good, other, 1, "LENGTH_MISMATCH")
        self.assertIn("4800", proc.stdout)

    def test_truncate_is_length_class(self):
        other = self.dir / "trunc.f32"
        other.write_bytes(sine()[:len(sine()) - 2400 * 4])
        proc = self.assert_cli(self.good, other, 1, "LENGTH_MISMATCH")
        self.assertIn("-2400", proc.stdout)

    def test_one_byte_flip_exit_one_localized(self):
        other = self.dir / "flip.f32"
        data = bytearray(sine())
        data[1000 * 4] ^= 0x01
        other.write_bytes(bytes(data))
        proc = self.assert_cli(self.good, other, 1, "FAIL")
        self.assertIn("1000", proc.stdout)

    def test_nan_file_is_failure_class(self):
        other = self.dir / "nan.f32"
        samples = samples_of(sine())
        samples[500] = float("nan")
        other.write_bytes(compare.f32_bytes(samples))
        self.assert_cli(self.good, other, 1, "NON_FINITE")

    def test_json_report_written_and_parseable(self):
        other = self.dir / "gain.f32"
        other.write_bytes(
            compare.f32_bytes([s * 1.001 for s in samples_of(sine())]))
        out = self.dir / "report.json"
        proc = run_cli(str(self.good), str(other), "--json", str(out))
        self.assertEqual(proc.returncode, 1)
        report = json.loads(out.read_text(encoding="utf-8"))
        self.assertEqual(report["schema"], "paired-comparison-report")
        self.assertEqual(report["verdict"], "FAIL")
        self.assertEqual(report["primary"]["first_divergent_sample"], 1)


class TestApparatusNegativeControls(unittest.TestCase):
    """Absent/invalid inputs exit 2 (NOT_RUN), never a pass."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix="r04-test-")
        self.addCleanup(self.tmp.cleanup)
        self.dir = Path(self.tmp.name)

    def test_missing_input_file_exit_two(self):
        missing = self.dir / "does-not-exist.f32"
        proc = run_cli(str(missing), str(missing))
        self.assertEqual(proc.returncode, 2)
        self.assertIn("NOT_RUN", proc.stderr)

    def test_odd_byte_count_exit_two(self):
        good = self.dir / "good.f32"
        good.write_bytes(sine())
        odd = self.dir / "odd.f32"
        odd.write_bytes(b"\x01\x02\x03")
        proc = run_cli(str(good), str(odd))
        self.assertEqual(proc.returncode, 2)
        self.assertIn("NOT_RUN", proc.stderr)
        self.assertNotIn("PASS", proc.stdout)

    def test_directory_input_exit_two(self):
        proc = run_cli(str(self.dir), str(self.dir))
        self.assertEqual(proc.returncode, 2)
        self.assertIn("NOT_RUN", proc.stderr)

    def test_wrong_sample_rate_label_metadata_level(self):
        evidence = load_tool(COMPARE_EVIDENCE)
        meta = {
            "fixture": "smoke", "sample_rate": 48000,
            "output": {"samples": 10, "bytes": 40, "sha256": "0" * 64,
                       "format": "f"},
            "inputs": {"voice": {"sha256": "0" * 64},
                       "events": {"sha256": "0" * 64}},
            "oracle_identity": {"binary": {"sha256": "0" * 64}},
            "determinism": {"byte_identical": True},
        }
        self.assertEqual(evidence.check_metadata(meta, json.loads(
            json.dumps(meta))), [])
        wrong = json.loads(json.dumps(meta))
        wrong["sample_rate"] = 44100
        disagreements = evidence.check_metadata(meta, wrong)
        self.assertEqual(len(disagreements), 1)
        self.assertIn("sample_rate", disagreements[0])
        self.assertIn("48000", disagreements[0])
        self.assertIn("44100", disagreements[0])

    def test_metadata_drift_localized_per_field(self):
        evidence = load_tool(COMPARE_EVIDENCE)
        meta = {
            "fixture": "smoke", "sample_rate": 48000,
            "output": {"samples": 10, "bytes": 40, "sha256": "a" * 64,
                       "format": "f"},
            "inputs": {"voice": {"sha256": "b" * 64},
                       "events": {"sha256": "c" * 64}},
            "oracle_identity": {"binary": {"sha256": "d" * 64}},
            "determinism": {"byte_identical": True},
        }
        wrong_hash = json.loads(json.dumps(meta))
        wrong_hash["output"]["sha256"] = "e" * 64
        self.assertEqual(
            evidence.check_metadata(meta, wrong_hash),
            [f"output.sha256: committed {'a' * 64!r} != "
             f"fresh {'e' * 64!r}"])
        stale_meta = json.loads(json.dumps(meta))
        stale_meta["determinism"]["byte_identical"] = False
        self.assertIn("determinism.byte_identical",
                      evidence.check_metadata(meta, stale_meta)[0])


if __name__ == "__main__":
    unittest.main()
