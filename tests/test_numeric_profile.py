"""Tests for the N01 numeric/scheduling decision record (issue #15).

Recomputes every schedule and storage figure independently, verifies the
decision index carries error + cost estimates (the issue's negative control),
re-generates the provisional Mark I tables byte-identically, checks event
timing against D00 (spec/contract-v1.json), and verifies every msfa/oracle
citation against the pinned trees — file hashes via reference/manifest.json
and cited-line content in place. Negative controls mutate the profile and
must fail tools/check_numeric_profile.py.
"""

from __future__ import annotations

import copy
import importlib.util
import json
import math
import os
import struct
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
PROFILE_PATH = REPO / "spec" / "numeric-profile-v1.json"
CONTRACT_PATH = REPO / "spec" / "contract-v1.json"
MANIFEST_PATH = REPO / "reference" / "manifest.json"
CHECKER = REPO / "tools" / "check_numeric_profile.py"
DOC = REPO / "docs" / "DECISIONS-NUMERIC-v1.md"
DR = REPO / "docs" / "decision-records" / "0003-numeric-profile-v1.md"

DEXED_ROOT = Path(os.environ.get("DEXED_ROOT", "/Users/joseph/dev/dexed"))
ORACLE_ROOT = Path(os.environ.get("DX7_ORACLE_ROOT",
                                  "/Users/joseph/dx7-oracle-marki"))


def load_checker():
    spec = importlib.util.spec_from_file_location("check_numeric_profile",
                                                  CHECKER)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def load_json(path: Path):
    with open(path, "r", encoding="utf-8") as fh:
        return json.load(fh)


CHK = load_checker()
PROFILE = load_json(PROFILE_PATH)
CONTRACT = load_json(CONTRACT_PATH)
MANIFEST = load_json(MANIFEST_PATH)


def run_checker_on(profile):
    """Run the checker's full validation against a (possibly mutated) profile."""
    failures = []
    failures += CHK.check_schedule(profile)
    failures += CHK.check_derived_constants(profile)
    failures += CHK.check_storage(profile)
    failures += CHK.check_decisions(profile)
    failures += CHK.check_provisional_tables(profile)
    failures += CHK.check_event_timing(profile, CONTRACT)
    hashes, err = CHK.load_manifest_hashes()
    if err:
        raise AssertionError(err)
    cite_failures, _ = CHK.check_citations(
        profile, str(DEXED_ROOT), str(ORACLE_ROOT), hashes)
    return failures + cite_failures


class TestProfileSchema(unittest.TestCase):
    def test_required_sections_present(self):
        for section in CHK.REQUIRED_SECTIONS:
            self.assertIn(section, PROFILE)

    def test_doc_and_dr_exist(self):
        self.assertTrue(DOC.is_file(), DOC)
        self.assertTrue(DR.is_file(), DR)
        dr_text = DR.read_text(encoding="utf-8")
        self.assertIn("Status:", dr_text)
        self.assertIn("Proposed", dr_text)

    def test_profile_documents_the_msfa_correction(self):
        """The record must state the corrected adoption claim, not the
        briefing's 'msfa => zero budget' version."""
        text = DOC.read_text(encoding="utf-8")
        self.assertIn("EngineMkI is an integer engine", text)
        self.assertIn("fails as stated", text)


class TestScheduleArithmetic(unittest.TestCase):
    """Recompute every schedule figure from first principles."""

    def test_operator_evals_per_second(self):
        rate = CONTRACT["sample_rate_hz"]
        notes = CONTRACT["polyphony"]
        ops = CONTRACT["operators"]
        self.assertEqual(rate, 48000)
        self.assertEqual(notes, 16)
        self.assertEqual(ops, 6)
        self.assertEqual(notes * ops * rate, 4_608_000)
        self.assertEqual(PROFILE["schedule"]["operator_evals_per_second"],
                         4_608_000)

    def test_frames_per_second(self):
        self.assertEqual(48000 / 64, 750)
        self.assertEqual(PROFILE["block"]["frames_per_second"], 750)
        self.assertEqual(PROFILE["block"]["n"], 1 << PROFILE["block"]["lg_n"])

    def test_evals_per_frame_and_per_sample(self):
        self.assertEqual(16 * 6, 96)
        self.assertEqual(96 * 64, 6144)
        self.assertEqual(PROFILE["schedule"]["operator_evals_per_sample"], 96)
        self.assertEqual(PROFILE["schedule"]["operator_evals_per_frame"], 6144)

    def test_clock_budgets_12m288(self):
        hz = 12_288_000
        self.assertEqual(hz / 48000, 256)          # clk/sample
        self.assertEqual(hz / 750, 16_384)         # clk/frame
        self.assertEqual(6144 / 16384, 0.375)      # op-loop floor share
        self.assertAlmostEqual(16384 / 6144, 2.6667, places=3)
        entry = PROFILE["schedule"]["clock_12m288"]
        self.assertEqual(entry["clks_per_sample"], 256)
        self.assertEqual(entry["clks_per_frame"], 16384)
        self.assertAlmostEqual(entry["op_loop_floor_share"], 0.375)
        self.assertAlmostEqual(entry["all_in_clks_per_op_eval"],
                               16384 / 6144, delta=1e-3)

    def test_clock_budgets_24m576(self):
        hz = 24_576_000
        self.assertEqual(hz / 48000, 512)
        self.assertEqual(hz / 750, 32_768)
        self.assertEqual(6144 / 32768, 0.1875)
        self.assertAlmostEqual(32768 / 6144, 5.3333, places=3)
        entry = PROFILE["schedule"]["clock_24m576"]
        self.assertEqual(entry["clks_per_sample"], 512)
        self.assertEqual(entry["clks_per_frame"], 32768)
        self.assertAlmostEqual(entry["op_loop_floor_share"], 0.1875)
        self.assertAlmostEqual(entry["all_in_clks_per_op_eval"],
                               32768 / 6144, delta=1e-3)

    def test_op_loop_only_minimum_clock(self):
        # >=1 clk per op-eval even with a fully shared datapath
        self.assertEqual(6144 * 750, 4_608_000)
        self.assertEqual(PROFILE["schedule"]["op_loop_only_minimum_clock_hz"],
                         4_608_000)
        self.assertEqual(4_608_000, 16 * 6 * 48_000)

    def test_sr_multiplier_derived_from_formula(self):
        # env.cc:47-49: sr_multiplier = (44100.0 / sampleRate) * (1 << 24),
        # assigned to uint32_t (truncation toward zero). Derive at 48 kHz;
        # the frozen value is checked against the derivation, never against a
        # restated literal.
        derived = math.floor(44100.0 / 48000.0 * (1 << 24))
        self.assertEqual(derived, 15_414_067)
        self.assertEqual(PROFILE["envelope"]["sr_multiplier_48k"], derived)
        self.assertEqual(PROFILE["premises"]["sample_rate_hz"], 48000)
        # negative control: v1's wrong constant differs from the derivation
        self.assertNotEqual(15_405_619, derived)

    def test_lfo_and_pitchenv_units_derived(self):
        # lfo.cc:26-29 / pitchenv.cc:22-24, (int32)(x + 0.5) truncation
        self.assertEqual(math.floor(64 * 25190424 / 48000 + 0.5),
                         PROFILE["lfo"]["unit_48k"])
        self.assertEqual(
            math.floor(64 * (1 << 24) / (21.3 * 48000) + 0.5),
            PROFILE["pitch_env"]["unit_48k"])

    def test_checker_recomputation_agrees(self):
        facts = CHK.recomputed_schedule(PROFILE["premises"], PROFILE["block"])
        self.assertEqual(facts["operator_evals_per_second"], 4_608_000)
        self.assertAlmostEqual(facts["clock_12m288"]["op_loop_floor_share"],
                               0.375)
        self.assertEqual(
            CHK.check_schedule(PROFILE), [],
            "checker must agree with independent recomputation")


class TestStorageEstimate(unittest.TestCase):
    """Recompute the 16-note state totals from the classified field tables."""

    def test_bits_per_operator(self):
        fields = PROFILE["storage"]["per_operator_runtime_state_bits"]
        self.assertEqual(
            sum(fields.values()),
            PROFILE["storage"]["totals"]["bits_per_operator"])
        # omitted from v1's subset; restored per the judge finding on PR #52
        for member in ("freq", "level_in", "env_effective_outlevel",
                       "env_effective_rate_scaling"):
            self.assertIn(member, fields)

    def test_bits_per_note(self):
        fields = PROFILE["storage"]["per_note_runtime_state_bits"]
        self.assertEqual(sum(fields.values()),
                         PROFILE["storage"]["totals"]["bits_per_note"])

    def test_patch_shared_bits(self):
        fields = PROFILE["storage"]["patch_shared_state_bits"]
        self.assertEqual(sum(fields.values()),
                         PROFILE["storage"]["totals"]["patch_shared_bits"])

    def test_total_16_note_state(self):
        totals = PROFILE["storage"]["totals"]
        ops = PROFILE["storage"]["instances"]["operators"]
        notes = PROFILE["storage"]["instances"]["notes"]
        want = (totals["bits_per_operator"] * ops
                + totals["bits_per_note"] * notes)
        self.assertEqual(totals["total_bits"], want)
        self.assertEqual(totals["replicated_total_with_patch_shared_bits"],
                         want + totals["patch_shared_bits"])
        self.assertAlmostEqual(totals["total_bytes_approx"],
                               totals["total_bits"] / 8, delta=0.5)
        # the bounded finding must survive the recount
        self.assertGreater(totals["total_bits"] * 118.0 / 1e6,
                           1.6734)  # quarter-slot core (D01 section 1)

    def test_cost_lines_follow_family_anchors(self):
        self.assertEqual(CHK.check_storage(PROFILE), [])
        totals = PROFILE["storage"]["totals"]
        registers_mm2 = totals["total_bits"] * 118.0 / 1e6
        self.assertAlmostEqual(registers_mm2, 4.524, places=3)
        macros = math.ceil((totals["total_bits"] / 8) / 512)
        self.assertEqual(macros, 10)
        doc = DOC.read_text(encoding="utf-8")
        self.assertIn("118", doc)
        self.assertIn("family-measured", doc)
        self.assertIn("derived", doc)

    def test_storage_estimate_present_for_full_state(self):
        """Issue #15 negative control: the full 16-note estimate must exist."""
        for section in ("classification_policy",
                        "per_operator_runtime_state_bits",
                        "per_note_runtime_state_bits",
                        "patch_shared_state_bits",
                        "instances", "totals", "cost_lines"):
            self.assertIn(section, PROFILE["storage"])


class TestProvisionalTables(unittest.TestCase):
    """Regenerate the Mark I 16-bit tables and the determinism margins."""

    def test_sinlog_table(self):
        table, margin = CHK.regen_sinlog_table()
        spec = PROFILE["provisional_tables"]["sinlog_table"]
        digest = CHK.hashlib.sha256(struct.pack("<1024H", *table)).hexdigest()
        self.assertEqual(digest, spec["sha256_uint16le"])
        self.assertEqual(table[0], spec["first_entry"])
        self.assertEqual(table[-1], spec["last_entry"])
        # monotone nonincreasing (it is -log2 of an increasing sine)
        self.assertTrue(all(table[i] >= table[i + 1] for i in range(1023)))
        self.assertGreater(margin, spec["min_half_boundary_margin"] * 0.5)
        self.assertGreater(margin, 1e-6)

    def test_sinexp_table(self):
        table, margin = CHK.regen_sinexp_table()
        spec = PROFILE["provisional_tables"]["sinexp_table"]
        digest = CHK.hashlib.sha256(struct.pack("<1024H", *table)).hexdigest()
        self.assertEqual(digest, spec["sha256_uint16le"])
        self.assertEqual(table[0], spec["first_entry"])
        self.assertEqual(table[-1], spec["last_entry"])
        self.assertTrue(all(table[i] < table[i + 1] for i in range(1023)))
        self.assertGreater(margin, spec["min_half_boundary_margin"] * 0.5)
        self.assertGreater(margin, 1e-6)

    def test_determinism_margins_beat_libm_ulp_by_orders_of_magnitude(self):
        # The cross-platform argument: any conforming double libm agrees to
        # ~1e-12 on these arguments; the rounding margins must dominate.
        self.assertGreater(
            PROFILE["provisional_tables"]["sinlog_table"]["min_half_boundary_margin"],
            1e-8)
        self.assertGreater(
            PROFILE["provisional_tables"]["sinexp_table"]["min_half_boundary_margin"],
            1e-8)


class TestEventTimingVsDec014(unittest.TestCase):
    def test_event_application_matches_contract(self):
        self.assertEqual(
            PROFILE["event_timing"]["dec014_event_application"],
            CONTRACT["event_application"])

    def test_queue_and_overflow_rules_unchanged_from_dec014(self):
        self.assertIn("explicit_rejection",
                      CONTRACT["event_queue_overflow"])
        self.assertIn("never silent drop",
                      PROFILE["event_timing"]["overflow"])

    def test_commit_granularity_is_the_64_sample_frame(self):
        self.assertEqual(PROFILE["event_timing"]["commit_granularity_samples"],
                         64)
        self.assertEqual(
            CHK.check_event_timing(PROFILE, CONTRACT), [])

    def test_silence_floor_ownership_frozen_here(self):
        self.assertEqual(CONTRACT["silence_floor_owner"], "N01")
        tail = PROFILE["note_tail"]
        self.assertEqual(tail["hold_frames"], 64)
        self.assertIn("16 << 16", tail["reclaimable_predicate"])


class TestDecisionIndex(unittest.TestCase):
    def test_every_decision_has_error_and_cost_estimate(self):
        self.assertEqual(CHK.check_decisions(PROFILE), [])
        for dec in PROFILE["decision_index"]:
            self.assertTrue(dec["error_estimate"].strip(), dec["id"])
            self.assertTrue(dec["cost_estimate"].strip(), dec["id"])

    def test_twelve_decisions_with_expected_ids(self):
        ids = [d["id"] for d in PROFILE["decision_index"]]
        self.assertEqual(ids, [f"NUM-{i:03d}" for i in range(1, 13)])

    def test_adoption_strategy_flagged_for_owner_signoff(self):
        self.assertEqual(PROFILE["status"]["adoption_strategy"],
                         "proposed_owner_signoff")


class TestCitations(unittest.TestCase):
    @unittest.skipUnless(DEXED_ROOT.is_dir(), "pinned dexed clone unavailable")
    def test_dexed_citations_hash_and_line_match(self):
        hashes = {entry["path"]: entry["sha256"] for entry in MANIFEST["files"]}
        failures, not_run = CHK.check_citations(PROFILE, str(DEXED_ROOT),
                                                str(ORACLE_ROOT), hashes)
        oracle_citations = sum(
            1 for dec in PROFILE["decision_index"]
            for cite in dec["citations"] if cite["tree"] == "oracle")
        if not ORACLE_ROOT.is_dir():
            # guarded skip: oracle citations NOT_RUN, dexed ones still checked
            self.assertEqual(not_run, oracle_citations)
        else:
            self.assertEqual(not_run, 0)
        self.assertEqual(failures, [])

    @unittest.skipUnless(ORACLE_ROOT.is_dir(), "oracle tree unavailable")
    def test_oracle_citations_match(self):
        hashes = {entry["path"]: entry["sha256"] for entry in MANIFEST["files"]}
        failures, _ = CHK.check_citations(PROFILE, str(DEXED_ROOT),
                                          str(ORACLE_ROOT), hashes)
        oracle_failures = [f for f in failures if "oracle:" in f]
        self.assertEqual(oracle_failures, [])

    def test_manifest_pins_every_cited_dexed_file(self):
        pinned = {entry["path"] for entry in MANIFEST["files"]}
        for dec in PROFILE["decision_index"]:
            for cite in dec["citations"]:
                if cite["tree"] == "dexed":
                    self.assertIn(cite["path"], pinned,
                                  f"{dec['id']} cites unpinned {cite['path']}")

    def test_output_chain_matches_manifest_render_scaling(self):
        scaling = next(s for s in MANIFEST["settings"]
                       if s["name"] == "render_scaling")
        self.assertIn(">> 4", scaling["value"].replace(">>4", ">> 4"))
        self.assertIn("0x8000", scaling["value"])
        self.assertIn("val >> 9", str(
            [c["expect"] for d in PROFILE["decision_index"]
             for c in d["citations"]]))


class TestNegativeControls(unittest.TestCase):
    """A control must demonstrably fail the check it targets."""

    def test_missing_cost_estimate_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["decision_index"][3]["cost_estimate"] = ""
        failures = run_checker_on(profile)
        self.assertTrue(any("cost_estimate" in f for f in failures), failures)

    def test_missing_error_estimate_fails(self):
        profile = copy.deepcopy(PROFILE)
        del profile["decision_index"][5]["error_estimate"]
        failures = run_checker_on(profile)
        self.assertTrue(any("error_estimate" in f for f in failures), failures)

    def test_sr_multiplier_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["envelope"]["sr_multiplier_48k"] = 15_405_619  # v1's wrong value
        failures = run_checker_on(profile)
        self.assertTrue(any("sr_multiplier" in f for f in failures), failures)

    def test_missing_full_state_storage_fails(self):
        profile = copy.deepcopy(PROFILE)
        del profile["storage"]["totals"]
        failures = run_checker_on(profile)
        self.assertTrue(any("16-note storage estimate" in f for f in failures),
                        failures)

    def test_schedule_arithmetic_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["schedule"]["operator_evals_per_frame"] = 6145
        failures = run_checker_on(profile)
        self.assertTrue(any("operator_evals_per_frame" in f for f in failures),
                        failures)

    def test_event_timing_mismatch_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["event_timing"]["dec014_event_application"] = "per_sample"
        failures = run_checker_on(profile)
        self.assertTrue(any("event_application" in f for f in failures),
                        failures)

    def test_table_byte_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["provisional_tables"]["sinlog_table"]["sha256_uint16le"] = \
            "0" * 64
        failures = run_checker_on(profile)
        self.assertTrue(any("regenerated sha256" in f for f in failures),
                        failures)

    def test_citation_line_rot_fails(self):
        profile = copy.deepcopy(PROFILE)
        for dec in profile["decision_index"]:
            if dec["id"] == "NUM-003":
                for cite in dec["citations"]:
                    if cite["path"].endswith("fm_core.cc"):
                        cite["expect"] = "THIS LINE DOES NOT EXIST"
        failures = run_checker_on(profile)
        self.assertTrue(any("not found in cited lines" in f for f in failures),
                        failures)


class TestCheckerCli(unittest.TestCase):
    def test_cli_passes_on_the_committed_tree(self):
        import subprocess
        import sys
        proc = subprocess.run(
            [sys.executable, str(CHECKER)], capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
        self.assertIn("PASS", proc.stdout)


if __name__ == "__main__":
    unittest.main()
