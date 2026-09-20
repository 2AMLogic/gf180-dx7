"""Validation for spec/physical-constraints-v1.json (issue #5, D01).

Rules enforced:
- Required top-level keys and per-item keys.
- Every item status is one of the declared enum values.
- Every confirmed item cites a source cite-key that appears verbatim in
  docs/PHYSICAL-CONSTRAINTS.md (the inventory document).
- Negative control (issue #5): an inventory entry claiming a memory option is
  usable, without macro views / timing evidence, must FAIL validation.
"""

import json
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
DOC = REPO / "docs" / "PHYSICAL-CONSTRAINTS.md"
SPEC = REPO / "spec" / "physical-constraints-v1.json"

REQUIRED_TOP = {"version", "generated", "status_enum", "doc", "items"}
REQUIRED_ITEM = {"id", "category", "status", "claim", "source"}
ALLOWED_CATEGORIES = {
    "shuttle", "clock", "board", "cell_library",
    "memory", "physical_flow", "eda_access", "dac",
}


class InventoryError(ValueError):
    pass


def validate(data, doc_text):
    """Raise InventoryError if the inventory violates the D01 rules."""
    for key in REQUIRED_TOP:
        if key not in data:
            raise InventoryError(f"missing top-level key: {key}")
    enum = set(data["status_enum"])
    if enum != {"confirmed", "assumed", "inaccessible"}:
        raise InventoryError(f"bad status_enum: {sorted(enum)}")
    items = data["items"]
    if not isinstance(items, list) or not items:
        raise InventoryError("items must be a non-empty list")
    seen = set()
    for item in items:
        for key in REQUIRED_ITEM:
            if key not in item:
                raise InventoryError(f"{item.get('id', '?')}: missing key {key}")
        iid = item["id"]
        if iid in seen:
            raise InventoryError(f"duplicate item id: {iid}")
        seen.add(iid)
        if item["status"] not in enum:
            raise InventoryError(f"{iid}: status {item['status']!r} not in enum")
        if item["category"] not in ALLOWED_CATEGORIES:
            raise InventoryError(f"{iid}: bad category {item['category']!r}")
        if not item["claim"].strip() or not item["source"].strip():
            raise InventoryError(f"{iid}: claim and source must be non-empty")
        if item["status"] == "confirmed" and item["source"] not in doc_text:
            raise InventoryError(
                f"{iid}: confirmed item cites {item['source']!r}, "
                "which does not appear in the inventory doc"
            )
        # Issue #5 negative control: a memory option declared usable must
        # carry macro-view / timing (Liberty) evidence in the entry itself.
        if (
            item["category"] == "memory"
            and item["status"] == "confirmed"
            and "usable" in item["claim"].lower()
        ):
            evidence = (item["claim"] + " " + item.get("measurement", "")).lower()
            if "liberty" not in evidence:
                raise InventoryError(
                    f"{iid}: claims a usable memory macro without Liberty/"
                    "timing-view evidence — fails review (issue #5 negative control)"
                )
    return True


class TestPhysicalConstraints(unittest.TestCase):
    def setUp(self):
        self.doc_text = DOC.read_text(encoding="utf-8")
        self.data = json.loads(SPEC.read_text(encoding="utf-8"))

    def test_spec_and_doc_exist(self):
        self.assertTrue(DOC.is_file(), f"{DOC} missing")
        self.assertTrue(SPEC.is_file(), f"{SPEC} missing")

    def test_inventory_validates(self):
        self.assertTrue(validate(self.data, self.doc_text))

    def test_statuses_in_enum(self):
        enum = set(self.data["status_enum"])
        for item in self.data["items"]:
            self.assertIn(item["status"], enum, item["id"])

    def test_required_keys_present(self):
        for item in self.data["items"]:
            for key in REQUIRED_ITEM:
                self.assertIn(key, item, f"{item.get('id')}: {key}")

    def test_required_inventory_sections_present(self):
        text = self.doc_text
        for section in (
            "What this does NOT say",
            "Open questions",
            "Shuttle and padframe",
            "Core clock candidates",
            "Memory options",
            "No die-fit claim",
        ):
            self.assertIn(section, text, f"doc section missing: {section}")

    def test_no_die_fit_claim_for_dx7(self):
        # The inventory must not assert the DX7 core fits anywhere.
        for phrase in ("DX7 core fits", "fits the DX7 core"):
            self.assertNotIn(phrase, self.doc_text)
        self.assertIn("H10", self.doc_text)

    # --- negative controls: each must demonstrably fail its check ---

    def test_negative_control_confirmed_without_source_in_doc(self):
        data = json.loads(json.dumps(self.data))
        data["items"].append({
            "id": "neg-unsourced", "category": "shuttle",
            "status": "confirmed", "claim": "a seat is purchased",
            "source": "no-such-source:anywhere",
        })
        with self.assertRaises(InventoryError):
            validate(data, self.doc_text)

    def test_negative_control_memory_usable_without_views(self):
        data = json.loads(json.dumps(self.data))
        data["items"].append({
            "id": "neg-memory-no-views", "category": "memory",
            "status": "confirmed",
            "claim": "custom 256x32 macro is usable as KB-scale RAM",
            "source": "2AMLogic/gf180-sram:README.md",
        })
        with self.assertRaises(InventoryError):
            validate(data, self.doc_text)

    def test_negative_control_bad_status(self):
        data = json.loads(json.dumps(self.data))
        data["items"][0]["status"] = "verified-true"
        with self.assertRaises(InventoryError):
            validate(data, self.doc_text)

    def test_negative_control_duplicate_id(self):
        data = json.loads(json.dumps(self.data))
        data["items"].append(dict(data["items"][0]))
        with self.assertRaises(InventoryError):
            validate(data, self.doc_text)


if __name__ == "__main__":
    unittest.main()
