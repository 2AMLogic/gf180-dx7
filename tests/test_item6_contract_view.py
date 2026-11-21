"""Lock the item 6 grader input to the core contract, DR-0008 and statement."""

import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from gen_item6_view import derive_status, render_view
from sha256_helpers import sha256_file

CONTRACT = ROOT / "spec/core-contract-v1.json"
VIEW = ROOT / "spec/core-contract-v1.signoff.json"
DECISION = ROOT / "docs/decision-records/0008-core-schedule-contract.md"
STATEMENT = ROOT / "signoff/item6-not-applicable-statement.json"

# Preserve the owner's original wording even after a later ruling.
PROPOSED = """\
- **Status:** Proposed (owner sign-off requested on the clock-target choice:
  24.576 MHz design point with the 12.288 MHz fallback recorded as
  worst-case-infeasible); the remaining contract terms are
  Accepted-by-default per D00 section 0 unless the owner rules otherwise
- **Date:** 2026-09-20
"""


class Item6ContractViewTest(unittest.TestCase):
    def assert_matches_sources(self, view, dr_text):
        budget = json.loads(CONTRACT.read_bytes())["budget_worst_case"]
        statement = json.loads(STATEMENT.read_bytes())
        ids = [row["id"] for row in view["rows"]]
        self.assertEqual(
            ids, budget["required_row_ids"], "contract row IDs or order differ"
        )
        self.assertCountEqual(
            ids, [row["id"] for row in statement["rows"]], "statement coverage differs"
        )
        self.assertEqual(view["rows"], budget["rows"], "contract row content differs")
        self.assertEqual(
            view["contract_sha256"], sha256_file(CONTRACT), "contract hash differs"
        )
        self.assertEqual(
            view["status"], derive_status(dr_text), "DR-0008 status differs"
        )

    def test_committed_view_matches_sources(self):
        self.assert_matches_sources(json.loads(VIEW.read_bytes()), DECISION.read_text())

    def test_committed_bytes_reproduce(self):
        self.assertEqual(
            VIEW.read_bytes(), render_view(CONTRACT.read_bytes(), DECISION.read_text())
        )

    def test_dropped_row_is_rejected(self):
        view = json.loads(VIEW.read_bytes())
        view["rows"].pop()
        with self.assertRaisesRegex(AssertionError, "contract row IDs or order differ"):
            self.assert_matches_sources(view, DECISION.read_text())

    def test_one_byte_change_in_row_is_rejected(self):
        view = json.loads(VIEW.read_bytes())
        row = view["rows"][0]
        row["consumer"] = "!" + row["consumer"][1:]
        with self.assertRaisesRegex(AssertionError, "contract row content differs"):
            self.assert_matches_sources(view, DECISION.read_text())

    def test_hand_set_ratified_is_rejected_for_proposed_decision(self):
        view = json.loads(VIEW.read_bytes())
        view["status"] = "ratified"
        with self.assertRaisesRegex(AssertionError, "DR-0008 status differs"):
            self.assert_matches_sources(view, PROPOSED)

    def test_proposed_with_accepted_by_default(self):
        self.assertEqual(derive_status(PROPOSED), "proposed")

    def test_accepted_and_ratified_decisions(self):
        for word in ("Accepted", "Ratified"):
            with self.subTest(word=word):
                self.assertEqual(
                    derive_status(f"- **Status:** {word} (owner ruling)\n"), "ratified"
                )

    def test_absent_empty_or_ambiguous_status_cannot_ratify(self):
        for text in (
            "Accepted",
            "- **Status:**\n- **Date:** today",
            PROPOSED + "- **Status:** Ratified",
        ):
            with self.subTest(text=text), self.assertRaises(ValueError):
                derive_status(text)
