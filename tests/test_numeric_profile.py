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
import re
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

DEXED_ROOT = Path(os.environ.get("DEXED_ROOT", REPO / ".oracle" / "dexed"))
ORACLE_ROOT = Path(os.environ.get("DX7_ORACLE_ROOT",
                                  REPO / ".oracle" / "dx7-oracle-marki"))


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
    # Citation checks need the pinned dexed/oracle trees; on machines without
    # them the dedicated tests above skip (unittest.skipUnless), so the
    # mutation harness must skip citations too rather than fail spuriously.
    if DEXED_ROOT.is_dir():
        hashes, err = CHK.load_manifest_hashes()
        if err:
            raise AssertionError(err)
        cite_failures, _ = CHK.check_citations(
            profile, str(DEXED_ROOT), str(ORACLE_ROOT), hashes)
        failures += cite_failures
    return failures


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
    @unittest.skipUnless(DEXED_ROOT.is_dir(),
                         "NOT_RUN: pinned dexed clone unavailable; run "
                         "tools/fetch_dexed.sh")
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

    @unittest.skipUnless(ORACLE_ROOT.is_dir(),
                         "NOT_RUN: oracle tree unavailable at "
                         f"{ORACLE_ROOT} (harness source is not published; "
                         "set DX7_ORACLE_ROOT)")
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

    def test_derivation_product_token_mutation_fails(self):
        # Issue #212: mutating only the product token (15414067.2 -> 15414068.2)
        # while the floor token stays intact must fail the derivation check.
        profile = copy.deepcopy(PROFILE)
        profile["envelope"]["sr_multiplier_derivation"] = \
            "(44100.0 / 48000.0) * 2^24 = 15414068.2 -> floor = 15414067"
        failures = run_checker_on(profile)
        self.assertTrue(
            any("sr_multiplier_derivation" in f for f in failures), failures)

    def test_derivation_floor_token_mutation_fails(self):
        # Issue #212: mutating only the floor token must fail too.
        profile = copy.deepcopy(PROFILE)
        profile["envelope"]["sr_multiplier_derivation"] = \
            "(44100.0 / 48000.0) * 2^24 = 15414067.2 -> floor = 15414068"
        failures = run_checker_on(profile)
        self.assertTrue(
            any("sr_multiplier_derivation" in f for f in failures), failures)

    def test_derivation_field_deleted_fails(self):
        # Issue #212: a missing derivation must FAIL, never silently skip.
        profile = copy.deepcopy(PROFILE)
        del profile["envelope"]["sr_multiplier_derivation"]
        failures = run_checker_on(profile)
        self.assertTrue(
            any("sr_multiplier_derivation" in f for f in failures), failures)

    # --- issue #222: prose strings restating values the checker recomputes.
    # Each control mutates only the prose copy; the numeric sibling stays
    # correct, so any failure must come from the new regenerate-and-compare
    # assertions. Dict-mutation only, not gated on DEXED_ROOT. ---

    def test_num007_title_lfo_unit_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        for dec in profile["decision_index"]:
            if dec["id"] == "NUM-007":
                dec["title"] = dec["title"].replace("33587", "33588")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-007" in f and "unit_48k" in f for f in failures),
            failures)

    def test_num008_title_pitch_unit_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        for dec in profile["decision_index"]:
            if dec["id"] == "NUM-008":
                dec["title"] = dec["title"].replace("1050", "1051")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-008" in f and "unit_48k" in f for f in failures),
            failures)

    def test_storage_claim_total_bits_mutation_fails(self):
        # containment only: the claim also carries the frozen historical
        # 'exact 32416 bits', which must stay wrong and is never matched
        profile = copy.deepcopy(PROFILE)
        profile["storage"]["claim"] = \
            PROFILE["storage"]["claim"].replace("38336 bits", "38337 bits")
        failures = run_checker_on(profile)
        self.assertTrue(any("storage.claim" in f for f in failures), failures)

    def test_storage_claim_area_mm2_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["storage"]["claim"] = \
            PROFILE["storage"]["claim"].replace("4.5236 mm2", "4.9236 mm2")
        failures = run_checker_on(profile)
        self.assertTrue(any("storage.claim" in f for f in failures), failures)

    def test_correction_note_frame_rate_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["schedule"]["correction_note"] = \
            PROFILE["schedule"]["correction_note"].replace("750/s", "751/s")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("correction_note" in f for f in failures), failures)

    def test_correction_note_clock_per_frame_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["schedule"]["correction_note"] = PROFILE["schedule"][
            "correction_note"].replace("16384 clk/frame", "16385 clk/frame")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("correction_note" in f for f in failures), failures)

    def test_correction_note_per_sample_views_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["schedule"]["correction_note"] = PROFILE["schedule"][
            "correction_note"].replace("256/512 clk", "257/512 clk")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("correction_note" in f for f in failures), failures)

    def test_num002_cost_estimate_clock_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        for dec in profile["decision_index"]:
            if dec["id"] == "NUM-002":
                dec["cost_estimate"] = dec["cost_estimate"].replace(
                    "16384 clk/frame", "16385 clk/frame")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-002" in f and "cost_estimate" in f for f in failures),
            failures)

    def test_num003_cost_estimate_evals_mutation_fails(self):
        profile = copy.deepcopy(PROFILE)
        for dec in profile["decision_index"]:
            if dec["id"] == "NUM-003":
                dec["cost_estimate"] = dec["cost_estimate"].replace(
                    "96 x", "97 x")
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-003" in f and "cost_estimate" in f for f in failures),
            failures)

    def test_num005_cost_estimate_evals_and_clock_mutations_fail(self):
        for token, replacement in (("96 x", "97 x"),
                                   ("96 steps/frame", "97 steps/frame"),
                                   ("16384 clk/frame", "16385 clk/frame")):
            profile = copy.deepcopy(PROFILE)
            for dec in profile["decision_index"]:
                if dec["id"] == "NUM-005":
                    dec["cost_estimate"] = dec["cost_estimate"].replace(
                        token, replacement)
            failures = run_checker_on(profile)
            self.assertTrue(
                any("NUM-005" in f and "cost_estimate" in f
                    for f in failures),
                f"mutating {token!r} in NUM-005 cost_estimate: {failures}")

    def test_missing_num007_decision_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["decision_index"] = [
            d for d in profile["decision_index"] if d["id"] != "NUM-007"]
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-007" in f and "missing" in f for f in failures),
            failures)

    def test_missing_num008_decision_fails(self):
        profile = copy.deepcopy(PROFILE)
        profile["decision_index"] = [
            d for d in profile["decision_index"] if d["id"] != "NUM-008"]
        failures = run_checker_on(profile)
        self.assertTrue(
            any("NUM-008" in f and "missing" in f for f in failures),
            failures)

    def test_missing_cost_restatement_decisions_fail(self):
        for did in ("NUM-002", "NUM-003", "NUM-005"):
            profile = copy.deepcopy(PROFILE)
            profile["decision_index"] = [
                d for d in profile["decision_index"] if d["id"] != did]
            failures = run_checker_on(profile)
            self.assertTrue(
                any(did in f and "missing" in f for f in failures),
                f"deleted {did}: {failures}")

    def test_missing_storage_claim_fails(self):
        profile = copy.deepcopy(PROFILE)
        del profile["storage"]["claim"]
        failures = run_checker_on(profile)
        self.assertTrue(
            any("storage.claim" in f and "missing" in f for f in failures),
            failures)

    def test_missing_correction_note_fails(self):
        profile = copy.deepcopy(PROFILE)
        del profile["schedule"]["correction_note"]
        failures = run_checker_on(profile)
        self.assertTrue(
            any("correction_note" in f and "missing" in f for f in failures),
            failures)

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

    @unittest.skipUnless(DEXED_ROOT.is_dir(),
                         "NOT_RUN: pinned dexed clone unavailable; run "
                         "tools/fetch_dexed.sh")
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


# --- issue #222 enumeration guard -----------------------------------------
#
# The class this closes: a prose string that restates a value the checker
# recomputes, where mutating the prose copy is invisible to the checker.
# The guard recomputes the full derived set (recomputed_schedule(), the three
# check_derived_constants() formulas, recomputed_storage_totals(), reg_area,
# plus the patch_shared sum check_storage() recomputes from the per-field
# widths), token-matches each value -- in the exact form the checker asserts it
# (int(...) casts, the :.4f area, str() of the exact product) -- against every
# non-structural string value in the profile, and requires every hit to be
# classified in RESTATEMENT_TABLE as "covered" (a checker assertion pins it)
# or ("exempt", one-line reason). A hit in neither bucket, a table entry that
# no longer hits, a covered entry whose mutation the checker misses, or an
# exempt entry whose mutation the checker catches, all fail.
#
# Class boundary, by construction: integer-token form only. Unit-converted
# mentions (e.g. '12.288 MHz' for clock_12m288.hz) and derived-of-derived
# figures (e.g. 3072 bits = 96 x 32) are outside the class; structural fields
# (sha256 digests, pinned commits, citation line ranges) are not prose and are
# excluded from the scan.
#
# This is a test, not a checker feature: the checker's exit-code contract and
# its PASS summary line are untouched.

STRUCTURAL_SUFFIXES = ("sha256", "sha256_uint16le", "pinned_commit", "lines")

# Tokens the recomputed-value scan would never produce hits for, but which
# are seeded into the table (with reasons) because they look like
# restatements and must stay wrong: the deliberate historical records and
# the Mark I source constants quoted in prose.
_WATCHED_TOKENS = ("32416", "213", "16284", "16368")

# Known boundary limitation: intermediate counts whose integer form is not
# distinctive (the SRAM macro count = 10, hits '10-bit', '>> 10', 'H10',
# '10%') are outside token-match class; a coincidental equality on such a
# token cannot be told apart from a restatement mechanically.

_COVERED = {
    ("envelope.sr_multiplier_derivation", "15414067"):
        "covered",  # #212/#221: full regeneration equality
    ("envelope.sr_multiplier_derivation", "15414067.2"):
        "covered",  # same assertion pins the exact product too
    ("decision_index[6].title", "33587"):
        "covered",  # NUM-007 title vs lfo.unit_48k
    ("decision_index[7].title", "1050"):
        "covered",  # NUM-008 title vs pitch_env.unit_48k
    ("storage.claim", "38336"):
        "covered",  # storage.claim vs recomputed total_bits
    ("storage.claim", "4.5236"):
        "covered",  # storage.claim vs reg_area formatted :.4f
    ("schedule.correction_note", "750"):
        "covered",  # correction_note vs frames_per_second
    ("schedule.correction_note", "16384"):
        "covered",  # correction_note vs clock_12m288.clks_per_frame
    ("schedule.correction_note", "256"):
        "covered",  # per-sample views vs clock_12m288.clks_per_sample
    ("schedule.correction_note", "512"):
        "covered",  # per-sample views vs clock_24m576.clks_per_sample
    ("decision_index[1].cost_estimate", "16384"):
        "covered",  # NUM-002 vs clock_12m288.clks_per_frame
    ("decision_index[2].cost_estimate", "96"):
        "covered",  # NUM-003 vs operator_evals_per_sample
    ("decision_index[4].cost_estimate", "96"):
        "covered",  # NUM-005 vs operator_evals_per_sample
    ("decision_index[4].cost_estimate", "16384"):
        "covered",  # NUM-005 vs clock_12m288.clks_per_frame
    ("storage.cost_lines[0].verdict", "444"):
        "covered",  # 'all registers' verdict vs recomputed patch_shared
}

_EXEMPT = {
    ("storage.claim", "32416"):
        ("exempt", "superseded v1 total quoted as a deliberate historical "
                   "record ('exact 32416 bits'); must stay wrong"),
    ("schedule.correction_note", "213"):
        ("exempt", "superseded wrong clk/frame figure quoted as the "
                   "corrected-away record; must stay wrong"),
    ("routing_gating.gate", "16284"):
        ("exempt", "Mark I k_level_thresh quoted from the cited source; not "
                   "recomputed by this checker"),
    ("note_tail.output_neutrality", "16284"):
        ("exempt", "Mark I k_level_thresh quoted from the cited source; not "
                   "recomputed by this checker"),
    ("note_tail.output_neutrality", "16368"):
        ("exempt", "Mark I gain2 constant quoted from the cited source; not "
                   "recomputed by this checker"),
    ("decision_index[8].title", "16284"):
        ("exempt", "Mark I k_level_thresh quoted from the cited source; not "
                   "recomputed by this checker"),
    ("storage.cost_lines[0].verdict", "32416"):
        ("exempt", "superseded v1 subset total quoted as a deliberate "
                   "historical record; must stay wrong"),
    ("output.quirk_negative_overflow_maps_positive", "32768"):
        ("exempt", "Mark I clip constant 0x8000; coincidental equality with "
                   "clock_24m576.clks_per_frame and the recomputed Mark I "
                   "table bits"),
    ("storage.cost_lines[1].anchor", "512"):
        ("exempt", "SRAM macro byte ceiling (SRAM_MACRO_BYTES); coincidental "
                   "equality with clock_24m576.clks_per_sample"),
}

RESTATEMENT_TABLE = {**_COVERED, **_EXEMPT}

_IDX_RE = re.compile(r"^([^\[]+)((?:\[\d+\])*)$")


def _derived_tokens():
    """Recompute the derived value set, in the token form the checker asserts."""
    rate = PROFILE["premises"]["sample_rate_hz"]
    n = PROFILE["block"]["n"]
    facts = CHK.recomputed_schedule(PROFILE["premises"], PROFILE["block"])
    totals = CHK.recomputed_storage_totals(PROFILE["storage"])
    patch_shared = sum(PROFILE["storage"]["patch_shared_state_bits"].values())
    reg_area = totals["total_bits"] * CHK.AREA_PER_BIT_REGISTER / 1e6
    exact = (44100.0 / rate) * (1 << 24)
    return {
        "envelope.sr_multiplier_48k": str(math.floor(exact)),
        "envelope.sr_multiplier_derivation product": str(exact),
        "lfo.unit_48k": str(math.floor(n * 25190424 / rate + 0.5)),
        "pitch_env.unit_48k":
            str(math.floor(n * (1 << 24) / (21.3 * rate) + 0.5)),
        "storage.bits_per_operator": str(totals["bits_per_operator"]),
        "storage.bits_per_note": str(totals["bits_per_note"]),
        "storage.total_bits": str(totals["total_bits"]),
        "storage.total_bytes": str(int(totals["total_bytes"])),
        "storage.patch_shared_bits": str(patch_shared),
        "storage.reg_area (:.4f)": f"{reg_area:.4f}",
        "schedule.frames_per_second": str(int(facts["frames_per_second"])),
        "schedule.operator_evals_per_sample":
            str(facts["operator_evals_per_sample"]),
        "schedule.operator_evals_per_frame":
            str(facts["operator_evals_per_frame"]),
        "schedule.operator_evals_per_second":
            str(facts["operator_evals_per_second"]),
        "schedule.op_loop_only_minimum_clock_hz":
            str(facts["op_loop_only_minimum_clock_hz"]),
        "schedule.clock_12m288.hz": str(int(facts["clock_12m288"]["hz"])),
        "schedule.clock_12m288.clks_per_sample":
            str(int(facts["clock_12m288"]["clks_per_sample"])),
        "schedule.clock_12m288.clks_per_frame":
            str(int(facts["clock_12m288"]["clks_per_frame"])),
        "schedule.clock_24m576.hz": str(int(facts["clock_24m576"]["hz"])),
        "schedule.clock_24m576.clks_per_sample":
            str(int(facts["clock_24m576"]["clks_per_sample"])),
        "schedule.clock_24m576.clks_per_frame":
            str(int(facts["clock_24m576"]["clks_per_frame"])),
    }


def _prose_strings(profile):
    """(dotted path, string) for every non-structural string value."""
    def walk(node, path):
        if isinstance(node, str):
            leaf = path.rsplit(".", 1)[-1]
            if not leaf.endswith(STRUCTURAL_SUFFIXES):
                yield path, node
        elif isinstance(node, dict):
            for key, value in node.items():
                yield from walk(value, f"{path}.{key}" if path else key)
        elif isinstance(node, list):
            for i, value in enumerate(node):
                yield from walk(value, f"{path}[{i}]")
    yield from walk(profile, "")


def _bump_token(token):
    """Bump the last digit of every digit-run: 33587 -> 33588, 4.5236 -> 5.5237, 15414067.2 -> 15414068.3."""
    return re.sub(r"(\d)(?!\d)",
                  lambda m: str((int(m.group(1)) + 1) % 10), token)


def _mutate_string_at(profile, path, token, replacement):
    """Replace token in the string at dotted path ([i] = list index).
    Returns False when the string no longer contains the token."""
    parts = path.split(".")
    parent = profile
    for part in parts[:-1]:
        match = _IDX_RE.match(part)
        parent = parent[match.group(1)]
        for idx in re.findall(r"\[(\d+)\]", match.group(2)):
            parent = parent[int(idx)]
    match = _IDX_RE.match(parts[-1])
    name = match.group(1)
    idxs = [int(i) for i in re.findall(r"\[(\d+)\]", match.group(2))]
    container, key = (parent[name], idxs[-1]) if idxs else (parent, name)
    if token not in container[key]:
        return False
    container[key] = container[key].replace(token, replacement)
    return True


class TestProseRestatementEnumerationGuard(unittest.TestCase):
    """Mechanically closes the prose-restatement class (issue #222)."""

    def test_every_derived_token_hit_is_classified(self):
        tokens = _derived_tokens()
        for i, tok in enumerate(_WATCHED_TOKENS):
            tokens[f"watched {i}"] = tok
        hits = set()
        for path, text in _prose_strings(PROFILE):
            for tok in tokens.values():
                if re.search(rf"(?<!\d){re.escape(tok)}(?!\d)", text):
                    hits.add((path, tok))
        unclassified = sorted(hits - set(RESTATEMENT_TABLE))
        self.assertEqual(
            unclassified, [],
            "new prose restatement(s) of a recomputed value; add a checker "
            "assertion + negative control and mark the entry covered, or "
            "exempt it with a one-line reason")
        stale = sorted(set(RESTATEMENT_TABLE) - hits)
        self.assertEqual(
            stale, [],
            "RESTATEMENT_TABLE entries that no longer hit any prose string; "
            "the table must mirror the committed prose")

    def test_covered_entries_are_live_assertions(self):
        for (path, tok), state in sorted(RESTATEMENT_TABLE.items()):
            if state != "covered":
                continue
            profile = copy.deepcopy(PROFILE)
            self.assertTrue(
                _mutate_string_at(profile, path, tok, _bump_token(tok)),
                f"{path} no longer contains {tok}")
            failures = run_checker_on(profile)
            self.assertTrue(
                failures,
                f"covered entry ({path}, {tok}) mutated but the checker "
                "stayed silent: its assertion was lost")

    def test_exempt_entries_stay_invisible(self):
        for (path, tok), state in sorted(RESTATEMENT_TABLE.items()):
            if not (isinstance(state, tuple) and state[0] == "exempt"):
                continue
            profile = copy.deepcopy(PROFILE)
            self.assertTrue(
                _mutate_string_at(profile, path, tok, _bump_token(tok)),
                f"{path} no longer contains {tok}")
            failures = run_checker_on(profile)
            self.assertEqual(
                failures, [],
                f"exempt entry ({path}, {tok}) [{state[1]}] mutated but the "
                f"checker failed: {failures[:2]}")


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
