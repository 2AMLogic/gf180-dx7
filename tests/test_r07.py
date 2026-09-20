"""R07 tests: driver validation, identity pinning, report schema (issue #14).

Coverage notes (coverage is reported separately from agreement):
  * These tests validate the DRIVER and its guards. They do not re-run
    the full six-case comparison and never establish engine agreement.
  * Oracle-dependent tests skip with a reason when a pinned oracle
    binary is absent (NOT_RUN, never a pass).
  * Negative controls in this file MUST keep failing the check they
    target: cross-engine PASS detection, non-adjudication omission,
    corpus drift refusal, block-alignment refusal.

Stdlib only.
"""

from __future__ import annotations

import hashlib
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
import zipfile
from dataclasses import replace as dc_replace
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import compare, sysex  # noqa: E402

_spec = importlib.util.spec_from_file_location(
    "r07_compare", REPO / "tools" / "r07_compare.py")
r07 = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(r07)

SECOND_IDENTITY = r07.SECOND_IDENTITY
REFERENCE_IDENTITY = r07.REFERENCE_IDENTITY


def _identity_ready(identity_path: Path) -> tuple[bool, str]:
    try:
        identity = json.loads(identity_path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return False, f"{identity_path} unreadable"
    binary = Path(identity.get("binary", {}).get("path", ""))
    if not binary.exists():
        return False, f"oracle binary absent at {binary}"
    return True, binary.as_posix()


SECOND_READY, SECOND_WHERE = _identity_ready(SECOND_IDENTITY)
REF_READY, REF_WHERE = _identity_ready(REFERENCE_IDENTITY)
ARCHIVE_PRESENT = r07.DEFAULT_ARCHIVE.exists()


class TestSecondOracleIdentity(unittest.TestCase):
    """The second oracle must be pinned like the first (external-only)."""

    def setUp(self):
        self.identity = json.loads(
            SECOND_IDENTITY.read_text(encoding="utf-8"))

    def test_schema_fields(self):
        for key in ("identity_version", "id", "issue", "purpose", "source",
                    "binary", "upstream", "engine", "toolchain_line",
                    "determinism", "boundary", "fidelity_note",
                    "verification", "pinned_at"):
            self.assertIn(key, self.identity, key)
        self.assertEqual(self.identity["identity_version"], 1)
        self.assertEqual(self.identity["id"], "R07-second-oracle-identity")
        self.assertIn("issues/14", self.identity["issue"])

    def test_external_only_boundary(self):
        self.assertIn("external-oracle-only", self.identity["boundary"])
        self.assertIn("GPL-2.0", self.identity["source"]["license"])
        self.assertIn("external-oracle-only",
                      self.identity["source"]["role"])

    def test_pins_are_well_formed(self):
        self.assertRegex(self.identity["source"]["git_commit"],
                         r"^[0-9a-f]{40}$")
        self.assertRegex(self.identity["upstream"]["commit"],
                         r"^[0-9a-f]{40}$")
        self.assertRegex(self.identity["binary"]["sha256"],
                         r"^[0-9a-f]{64}$")
        self.assertEqual(self.identity["upstream"]["project"],
                         "hexter (DSSI software synthesizer)")
        self.assertEqual(self.identity["upstream"]["release"],
                         "version 1.1.1")

    def test_engine_configuration_recorded(self):
        engine = self.identity["engine"]
        self.assertIn("fixed-point", engine["mode"])
        self.assertEqual(engine["sample_rate"], 48000)
        self.assertEqual(engine["nugget_size"], 64)
        self.assertEqual(engine["tuning_hz"], 4.4e2)

    @unittest.skipUnless(SECOND_READY, f"second oracle absent: {SECOND_WHERE}")
    def test_binary_hash_matches_pin(self):
        record = r07._check_identity(SECOND_IDENTITY)
        self.assertEqual(record["status"], "PASS", record["detail"])

    @unittest.skipUnless(REF_READY, f"reference oracle absent: {REF_WHERE}")
    def test_reference_identity_still_passes(self):
        record = r07._check_identity(REFERENCE_IDENTITY)
        self.assertEqual(record["status"], "PASS", record["detail"])


class TestCaseConstruction(unittest.TestCase):
    """Diagnostic set: deterministic, decodable, block-aligned, wave-safe."""

    def test_six_cases_built(self):
        cases = r07.synthetic_cases()
        names = [c["name"] for c in cases]
        self.assertEqual(len(names), 5)
        for expected in ("fb-alg4", "fb-alg6", "detune-extreme",
                         "slow-envelope", "fixed-frequency-high"):
            self.assertIn(expected, names)

    def test_case_bytes_deterministic_and_decodable(self):
        cases_a = r07.synthetic_cases()
        cases_b = r07.synthetic_cases()
        for ca, cb in zip(cases_a, cases_b):
            va = sysex.encode_voice(ca["voice"])
            vb = sysex.encode_voice(cb["voice"])
            self.assertEqual(va, vb, ca["name"])
            voice = sysex.decode_voice(va)
            self.assertEqual(voice.anomalies, [], ca["name"])
            self.assertEqual(len(va), sysex.VOICE_MSG_LEN, ca["name"])
            self.assertEqual(va[:6], b"\xf0\x43\x00\x00\x01\x1b", ca["name"])

    def test_feedback_cases_have_feedback_7(self):
        cases = {c["name"]: c for c in r07.synthetic_cases()}
        self.assertEqual(cases["fb-alg4"]["voice"].feedback, 7)
        self.assertEqual(cases["fb-alg4"]["voice"].algorithm, 3)
        self.assertEqual(cases["fb-alg6"]["voice"].feedback, 7)
        self.assertEqual(cases["fb-alg6"]["voice"].algorithm, 5)

    def test_detune_extremes_present(self):
        cases = {c["name"]: c for c in r07.synthetic_cases()}
        dets = sorted(op.osc_detune for op in
                      cases["detune-extreme"]["voice"].operators)
        self.assertEqual((dets[0], dets[-1]), (0, 14))

    def test_slow_envelope_nonzero_final_level(self):
        cases = {c["name"]: c for c in r07.synthetic_cases()}
        for op in cases["slow-envelope"]["voice"].operators:
            self.assertEqual(
                (op.rate_1, op.rate_2, op.rate_3, op.rate_4), (1, 1, 1, 1))
            self.assertGreater(op.level_4, 0)

    def test_fixed_frequency_high_case(self):
        cases = {c["name"]: c for c in r07.synthetic_cases()}
        op1 = cases["fixed-frequency-high"]["voice"].operators[0]
        self.assertEqual(op1.osc_mode, 1)
        self.assertEqual(op1.osc_freq_coarse, 15)

    def test_no_sample_hold_lfo(self):
        """Determinism guard: no case may use LFO wave 4 (rand() path)."""
        for case in r07.synthetic_cases():
            self.assertNotEqual(case["voice"].lfo_wave, 4, case["name"])
            self.assertEqual(case["voice"].lfo_pitch_mod_depth, 0,
                             case["name"])
            self.assertEqual(case["voice"].lfo_amp_mod_depth, 0,
                             case["name"])

    def test_events_block_aligned(self):
        events = r07.events_text(r07.CASE_EVENTS)
        for line in events.strip().splitlines():
            pos = int(line.split()[0])
            self.assertEqual(pos % 64, 0, line)

    def test_events_refuse_unaligned_position(self):
        bad = [{"cmd": "note_on", "args": [60, 100], "pos": 4801}]
        with self.assertRaises(RuntimeError):
            r07.events_text(bad)

    def test_case_events_match_driver_convention(self):
        self.assertEqual(r07.CASE_EVENTS[0]["pos"], 4800)
        self.assertEqual(r07.CASE_EVENTS[1]["pos"], 96000)


class TestCorpusCase(unittest.TestCase):
    """Corpus case integrity (archive-level; guarded, never silent)."""

    def test_canonical_hash_matches_catalog_definition(self):
        """Same definition as R03 tools/catalog_archive.py canonical_hash:
        unpacked 155-byte body with the 10-byte name region zeroed."""
        cases = r07.synthetic_cases()
        v = cases[0]["voice"]
        body = sysex.encode_voice(v)[6:6 + sysex.VOICE_DATA_LEN]
        expected = hashlib.sha256(
            body[:145] + b"\x00" * 10).hexdigest()
        self.assertEqual(r07.canonical_hash_of_voice(v), expected)

    def test_canonical_hash_detects_mutation(self):
        """Negative control: a mutated voice must change the canonical
        hash (this is the check corpus_case() enforces against dev32)."""
        cases = r07.synthetic_cases()
        v = cases[0]["voice"]
        mutated = dc_replace(
            v, operators=[dc_replace(v.operators[0], rate_1=49)]
            + list(v.operators[1:]))
        self.assertNotEqual(r07.canonical_hash_of_voice(v),
                            r07.canonical_hash_of_voice(mutated))
    def test_corpus_case_refuses_drifted_archive(self):
        """Negative control: any archive whose bytes do not match the
        dev32 pin must be refused, even if it contains a well-formed
        bank."""
        voice = r07.synthetic_cases()[0]["voice"]
        bank = sysex.Bank(voices=[voice] * 32)
        blob = sysex.encode_bank(bank)
        with tempfile.TemporaryDirectory() as tmp:
            fake = Path(tmp) / "fake.zip"
            with zipfile.ZipFile(fake, "w") as zf:
                zf.writestr("DX7_AllTheWeb/Aminet/105.syx", blob)
            with self.assertRaises((RuntimeError, FileNotFoundError)):
                r07.corpus_case(fake)

    @unittest.skipUnless(ARCHIVE_PRESENT,
                         f"pinned archive absent: {r07.DEFAULT_ARCHIVE}")
    def test_corpus_case_integrity_passes(self):
        case, chash = r07.corpus_case(r07.DEFAULT_ARCHIVE)
        self.assertEqual(case["name"], "corpus-shrpsynth")
        dev32 = json.loads(r07.DEV32.read_text(encoding="utf-8"))
        self.assertEqual(chash,
                         dev32["entries"][0]["canonical_hash"])

    @unittest.skipUnless(ARCHIVE_PRESENT,
                         f"pinned archive absent: {r07.DEFAULT_ARCHIVE}")
    def test_build_cases_includes_corpus(self):
        cases, notes = r07.build_cases(r07.DEFAULT_ARCHIVE)
        names = [c["name"] for c in cases]
        self.assertEqual(len(names), 6)
        self.assertEqual(notes, [])
        corpus = cases[-1]
        self.assertEqual(corpus["voice"].name.strip(), "SHRPSYNTH")

    def test_build_cases_notes_corpus_not_run_without_archive(self):
        cases, notes = r07.build_cases(None)
        self.assertEqual(len(cases), 5)
        self.assertTrue(any("NOT_RUN" in n for n in notes))


def _make_wrapper(verdict: str = "FAIL") -> dict:
    """A well-formed cross-engine case report for schema validation."""
    a = compare.sine_f32(8192, 220.0, 0.5)
    b = compare.sine_f32(8192, 220.0, 0.55)
    report = compare.compare_streams(a, b, sample_rate=r07.RATE,
                                     exact_mode=True)
    d = report.to_dict()
    d["verdict"] = verdict  # for the negative-control variant only
    return {
        "schema": r07.SCHEMA,
        "issue": r07.ISSUE_URL,
        "case": "fb-alg4",
        "rationale": "fixture",
        "render_config": {"sample_rate": r07.RATE, "seconds": 0.25,
                          "events": r07.CASE_EVENTS},
        "engines": {
            "reference": {"identity_id": "R02-oracle-identity",
                          "binary_sha256": "0" * 64},
            "second": {"identity_id": "R07-second-oracle-identity",
                       "binary_sha256": "0" * 64},
        },
        "expected_verdict": "FAIL (cross-engine exactness failure is the "
                            "expected result)",
        "report": d,
        "diagnosis": {"magnitude_class": "signal_scale",
                      "divergence_class": "on_note_on_block"},
        "hypothesis": {"status": "HYPOTHESIS",
                       "seam": "feedback coefficient mapping",
                       "hypothesis": "fixture text"},
        "non_claims": [r07.NON_ADJUDICATION],
    }


class TestReportSchema(unittest.TestCase):
    """validate_case_report encodes the issue's negative controls."""

    def test_well_formed_report_validates(self):
        self.assertEqual(r07.validate_case_report(_make_wrapper()), [])

    def test_rejects_pass_verdict(self):
        """Negative control: a cross-engine report claiming PASS (exact
        agreement between different engines) must be rejected."""
        problems = r07.validate_case_report(_make_wrapper(verdict="PASS"))
        self.assertTrue(any("PASS" in p for p in problems), problems)

    def test_rejects_missing_non_adjudication(self):
        wrapper = _make_wrapper()
        wrapper["non_claims"] = ["we decided engine X is wrong"]
        problems = r07.validate_case_report(wrapper)
        self.assertTrue(any("non-adjudication" in p for p in problems))

    def test_rejects_unlabeled_hypothesis(self):
        wrapper = _make_wrapper()
        wrapper["hypothesis"]["status"] = "FACT"
        problems = r07.validate_case_report(wrapper)
        self.assertTrue(any("HYPOTHESIS" in p for p in problems))

    def test_rejects_missing_seam(self):
        wrapper = _make_wrapper()
        del wrapper["hypothesis"]["seam"]
        problems = r07.validate_case_report(wrapper)
        self.assertTrue(any("seam" in p for p in problems))

    def test_rejects_missing_engine_identity(self):
        wrapper = _make_wrapper()
        wrapper["engines"]["second"] = {"identity_id": "x"}
        problems = r07.validate_case_report(wrapper)
        self.assertTrue(any("engines.second" in p for p in problems))

    def test_diagnosis_classes(self):
        self.assertEqual(
            r07.divergence_class(4800, 4800, r07.RATE),
            "on_note_on_block")
        self.assertEqual(r07.divergence_class(None, 4800, r07.RATE),
                         "never")
        self.assertEqual(r07.divergence_class(100, 4800, r07.RATE),
                         "pre_onset")
        self.assertEqual(r07.magnitude_class(0.6, 1.0, 1.0),
                         "signal_scale")
        self.assertEqual(r07.magnitude_class(0.0, 1.0, 1.0), "identical")
        self.assertEqual(r07.magnitude_class(0.0, 0.0, 0.0),
                         "both_silent")


class TestHypotheses(unittest.TestCase):
    """Every case must carry a labeled, seam-named, cited hypothesis."""

    def test_all_cases_covered(self):
        cases = [c["name"] for c in r07.synthetic_cases()] + \
            ["corpus-shrpsynth"]
        self.assertEqual(sorted(r07.HYPOTHESES), sorted(cases))

    def test_hypotheses_are_labeled_and_cited(self):
        for name, hyp in r07.HYPOTHESES.items():
            self.assertTrue(hyp.get("seam"), name)
            self.assertTrue(hyp.get("hypothesis"), name)
            self.assertTrue(hyp.get("expect_if_confirmed"), name)
        # compare_case() must label every built hypothesis HYPOTHESIS
        wrapper = _make_wrapper()
        self.assertEqual(wrapper["hypothesis"]["status"], "HYPOTHESIS")

    def test_compare_case_labels_hypothesis(self):
        a = compare.sine_f32(8192, 220.0, 0.5)
        b = compare.sine_f32(8192, 220.0, 0.55)
        identities = {
            "reference": {"identity_id": "R02-oracle-identity",
                          "binary_sha256": "0" * 64},
            "second": {"identity_id": "R07-second-oracle-identity",
                       "binary_sha256": "0" * 64},
        }
        case = {"name": "fb-alg4", "rationale": "fixture"}
        wrapper = r07.compare_case(case, a, b, {}, {}, identities)
        self.assertEqual(r07.validate_case_report(wrapper), [])
        self.assertEqual(wrapper["hypothesis"]["status"], "HYPOTHESIS")

    def test_non_adjudication_statement_present(self):
        self.assertIn("no engine is adjudicated",
                      r07.NON_ADJUDICATION.lower())


class TestEvidenceIntegrity(unittest.TestCase):
    """Committed evidence stays consistent with local renders (STALE
    detection): if the (gitignored) PCM is present, its hash must match
    the committed compare.json."""

    def test_committed_reports_match_local_renders_when_present(self):
        r07_dir = REPO / "reference" / "evidence" / "r07"
        if not r07_dir.exists():
            self.skipTest("evidence directory absent")
        checked = 0
        for compare_json in sorted(r07_dir.glob("*/compare.json")):
            wrapper = json.loads(compare_json.read_text(encoding="utf-8"))
            cdir = compare_json.parent
            for side in ("reference", "second"):
                pcm = cdir / (
                    "render-ref.f32" if side == "reference"
                    else "render-second.f32")
                if not pcm.exists():
                    continue  # PCM is gitignored; absence is normal
                digest = hashlib.sha256(pcm.read_bytes()).hexdigest()
                self.assertEqual(
                    digest, wrapper["renders"][side]["sha256"],
                    f"{pcm} drifted against committed hash")
                checked += 1
        if checked == 0:
            self.skipTest("no local renders present (gitignored); hashes "
                          "unverifiable here, regeneration reproduces them")


class TestFixtureEndToEnd(unittest.TestCase):
    """Driver validation on a tiny fixture run (guarded skips)."""

    @unittest.skipUnless(
        SECOND_READY and REF_READY,
        f"oracle binaries absent (ref: {REF_WHERE}, second: {SECOND_WHERE})")
    def test_fixture_run(self):
        proc = subprocess.run(
            [sys.executable, str(REPO / "tools" / "r07_compare.py"),
             "--fixture"],
            capture_output=True, text=True, timeout=600)
        self.assertEqual(proc.returncode, 0,
                         f"stdout: {proc.stdout}\nstderr: {proc.stderr}")
        self.assertIn("r07_compare fixture: OK", proc.stdout)

    def test_driver_importable_standalone(self):
        self.assertEqual(r07.SCHEMA, "r07-disagreement-case/1")
        self.assertTrue(r07.NON_ADJUDICATION)


if __name__ == "__main__":
    unittest.main()
