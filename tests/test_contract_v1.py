"""Contract v1 sanity tests (schema-less, stdlib only).

Lockstep checks between spec/contract-v1.json and docs/DECISIONS-v1.md:
required keys/values must be present, every DEC id in the JSON must appear in
the decision document (and vice versa), and the forbidden-omission negative
control must hold — a contract missing sample rate, polyphony, the reference
engine, or the effects policy fails.
"""

import json
import re
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
CONTRACT_PATH = REPO_ROOT / "spec" / "contract-v1.json"
DECISIONS_PATH = REPO_ROOT / "docs" / "DECISIONS-v1.md"

# Negative control (issue #4): omitting any of these must fail review.
FORBIDDEN_OMISSIONS = {
    "sample rate": {"json_key": "sample_rate_hz", "md_pattern": r"48[,\u202f ]?000\s*Hz|48\s*kHz"},
    "polyphony": {"json_key": "polyphony", "md_pattern": r"16[ -]note|16\s+simultaneous\s+notes"},
    "reference engine": {"json_key": "reference_profile", "md_pattern": r"Dexed\s+Mark\s+I"},
    "effects policy": {"json_key": "effects_policy", "md_pattern": r"dry"},
}

PROPOSED_DECISIONS = ["DEC-004", "DEC-012", "DEC-017"]


class TestContractV1(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.raw = CONTRACT_PATH.read_text(encoding="utf-8")
        cls.contract = json.loads(cls.raw)
        cls.decisions_md = DECISIONS_PATH.read_text(encoding="utf-8")

    # --- required keys and values -----------------------------------------

    def test_required_scalar_values(self):
        expected = {
            "sample_rate_hz": 48000,
            "polyphony": 16,
            "timbres": 1,
            "algorithms": 32,
            "operators": 6,
            "profile": "dx7-compatible-v1",
            "reference_profile": "dexed-marki",
            "event_application": "next_frame_boundary",
            "event_latency_bound_frames": 1,
            "retrigger": "envelope_retrigger_from_current_level",
            "same_pitch_repeat": "steal_oldest_matching_voice",
            "sustain": "hold_until_pedal_release_midi_semantics",
            "patch_change_while_held": (
                "old_patch_until_note_off_atomic_frame_boundary_commit"
            ),
            "osc_lfo_phase": "free_running_except_patch_sync_param_restart",
            "forced_kill": "all_sound_off_or_explicit_reset_only",
            "physical_target": "gf180mcu_no_slot_fit_assertion",
        }
        for key, value in expected.items():
            self.assertIn(key, self.contract, f"missing required key: {key}")
            self.assertEqual(
                self.contract[key], value, f"wrong value for {key}"
            )

    def test_forbidden_omissions_present_in_json_and_markdown(self):
        """Negative control: omitting any of these must fail."""
        for label, spec in FORBIDDEN_OMISSIONS.items():
            self.assertIn(
                spec["json_key"],
                self.contract,
                f"forbidden omission in JSON: {label}",
            )
            self.assertIsNotNone(
                re.search(spec["md_pattern"], self.decisions_md),
                f"forbidden omission in DECISIONS-v1.md: {label}",
            )

    def test_effects_policy_states_dry_comparisons(self):
        self.assertIn("dry", self.contract["effects_policy"])

    def test_no_original_hardware_fidelity_claim(self):
        self.assertIs(self.contract["original_hardware_fidelity_claim"], False)
        self.assertIn("no_original_hardware_fidelity_claim", self.contract["exclusions"])
        self.assertIn("Dexed agreement is not original-DX7 fidelity", self.decisions_md)

    def test_performance_controls_present(self):
        required = {
            "note_on", "note_off", "velocity", "sustain", "pitch_bend",
            "modulation_wheel", "channel_aftertouch", "volume", "all_sound_off",
        }
        self.assertTrue(required.issubset(set(self.contract["performance_controls"])))

    def test_preset_bank_counts(self):
        banks = self.contract["preset_banks"]
        self.assertEqual(banks["initial_listening_bank_patches"], 32)
        self.assertEqual(banks["shipping_curated_bank_patches"], 128)

    def test_proposed_owner_may_veto_set(self):
        self.assertEqual(PROPOSED_DECISIONS, sorted(self.contract["proposed_owner_may_veto"]))
        for dec in PROPOSED_DECISIONS:
            self.assertIn(dec, self.contract["decisions"])
        for dec in PROPOSED_DECISIONS:
            self.assertIn("Proposed (owner may veto)", self.decisions_md)

    def test_licensing_recorded_as_owner_ruled(self):
        licensing = self.contract["licensing"]
        self.assertEqual(licensing["repo_license"], "Apache-2.0")
        self.assertIs(licensing["per_file_headers"], False)
        self.assertEqual(
            licensing["gpl_engine_role"],
            "external_comparison_oracles_only_never_copied",
        )
        self.assertIn("never copied into this repository", self.decisions_md)

    # --- DEC id lockstep ---------------------------------------------------

    def json_dec_ids(self):
        return set(re.findall(r"DEC-\d{3}", self.raw))

    def test_markdown_contains_every_json_dec_id(self):
        json_ids = self.json_dec_ids()
        self.assertEqual(len(json_ids), 25)
        missing = sorted(json_ids - set(re.findall(r"DEC-\d{3}", self.decisions_md)))
        self.assertFalse(
            missing, f"DEC ids in contract JSON but missing from DECISIONS-v1.md: {missing}"
        )

    def test_markdown_dec_ids_all_present_in_json(self):
        extra = sorted(set(re.findall(r"DEC-\d{3}", self.decisions_md)) - self.json_dec_ids())
        self.assertFalse(
            extra, f"DEC ids in DECISIONS-v1.md but missing from contract JSON: {extra}"
        )

    def test_change_control_statement_present(self):
        self.assertIn("requires a new", self.decisions_md)
        self.assertIn("decision record", self.decisions_md)
        self.assertIn("STALE marking", self.decisions_md)


if __name__ == "__main__":
    unittest.main()
