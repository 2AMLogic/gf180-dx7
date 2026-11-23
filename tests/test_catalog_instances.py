"""Tests for the shared clean-instance enumeration (issue #285).

tools/catalog_instances.py owns the five-field Instance record and the
expansion of a P02 catalog into the ordered stream of eligible (decoded,
anomaly-free) voice instances. The dev32 and U02 selectors both import
it; these tests pin the rule on a small hand-authored fixture (out-of-
order paths and slot rows, two clean slots sharing a canonical hash, an
anomalous slot, a missing hash index, non-decoded and anomalous
standalone entries) and keep a live failure control: admitting the
anomalous slot must make the exact expectation assertion fail.

A cross-tool identity check guards the dedup itself: if either selector
grows a local copy of the record or the enumeration again, the exposed
symbols stop being the shared module's and this suite goes red.
"""

from __future__ import annotations

import copy
import importlib.util
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
if str(REPO / "tools") not in sys.path:
    sys.path.insert(0, str(REPO / "tools"))

import catalog_instances  # noqa: E402

SELECT = REPO / "tools" / "select_dev32.py"
U02 = REPO / "tools" / "u02_candidates.py"

# Distinct 64-hex stand-ins for P02 canonical hashes.
H0 = "aa" * 32
H1 = "bb" * 32
H2 = "cc" * 32
H3 = "dd" * 32

# Hand-authored fixture: entries deliberately NOT in sort order.
# z-bank row 1 is anomalous (a == 1); row 2 has no resolvable hash
# (hidx == -1); slot rows inside a-bank are given out of slot order;
# H0 appears in two clean slots and H2 in a clean slot plus a clean
# standalone voice (repeated hashes survive enumeration).
FIXTURE = {
    "schema": "gf180-dx7.archive-catalog/1",
    "hashes": [H0, H1, H2, H3],
    "names": ["Zed Pad", "Anom", "Ghost", "Third", "Repeat", "Fourth"],
    "entries": [
        {  # order 0
            "p": "DX7_AllTheWeb/z-bank.syx", "c": "bank", "v": "decoded",
            "w": [0, 0, 0, 0,   # slot 0 clean -> H0 "Zed Pad"
                  1, 1, 1, 1,   # slot 1 ANOMALOUS -> excluded
                  2, -1, 2, 0,  # slot 2 missing hash -> excluded
                  3, 2, 3, 0],  # slot 3 clean -> H2 "Third"
        },
        {  # order 1: clean standalone voice, repeats H2
            "p": "DX7_AllTheWeb/m-voice.syx", "c": "voice", "v": "decoded",
            "a": 0, "h": H2, "n": "Solo Name",
        },
        {  # order 2: slot rows out of order (5 before 4)
            "p": "DX7_AllTheWeb/a-bank.syx", "c": "bank", "v": "decoded",
            "w": [5, 0, 4, 0,   # slot 5 clean -> H0 "Repeat"
                  4, 3, 5, 0],  # slot 4 clean -> H3 "Fourth"
        },
        {  # anomalous standalone voice -> excluded
            "p": "DX7_AllTheWeb/n-voice.syx", "c": "voice", "v": "decoded",
            "a": 2, "h": H1, "n": "Broken",
        },
        {  # not decoded -> excluded
            "p": "DX7_AllTheWeb/p-voice.syx", "c": "voice", "v": "raw",
            "a": 0, "h": H3, "n": "Raw",
        },
        {  # whole bank not decoded -> excluded
            "p": "DX7_AllTheWeb/b-bank.syx", "c": "bank", "v": "failed",
            "w": [0, 0, 0, 0],
        },
        {  # decoded, anomaly-free, but no canonical hash -> excluded
            "p": "DX7_AllTheWeb/n2-voice.syx", "c": "voice", "v": "decoded",
            "a": 0, "h": None, "n": "NoHash",
        },
    ],
}

# The exact ordered (path, slot, canonical_hash, name, order) stream the
# fixture must produce: sort key is (path, standalone-first then slot,
# catalog order).
EXPECTED = [
    ("DX7_AllTheWeb/a-bank.syx", 4, H3, "Fourth", 2),
    ("DX7_AllTheWeb/a-bank.syx", 5, H0, "Repeat", 2),
    ("DX7_AllTheWeb/m-voice.syx", None, H2, "Solo Name", 1),
    ("DX7_AllTheWeb/z-bank.syx", 0, H0, "Zed Pad", 0),
    ("DX7_AllTheWeb/z-bank.syx", 3, H2, "Third", 0),
]

ANOMALOUS_TUPLE = ("DX7_AllTheWeb/z-bank.syx", 1, H1, "Anom", 0)


def tuples(catalog: dict) -> list[tuple]:
    return [(i.path, i.slot, i.canonical_hash, i.name, i.order)
            for i in catalog_instances.clean_instances(catalog)]


def load_tool(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    spec.loader.exec_module(mod)  # type: ignore[union-attr]
    return mod


class TestCleanEnumeration(unittest.TestCase):
    """The shared rule, pinned on the hand-authored fixture."""

    def test_expected_ordered_tuples(self):
        self.assertEqual(tuples(FIXTURE), EXPECTED)

    def test_repeated_hashes_survive(self):
        hashes = [t[2] for t in tuples(FIXTURE)]
        self.assertEqual(hashes.count(H0), 2,
                         "two clean slots sharing H0 must both survive")
        self.assertEqual(hashes.count(H2), 2,
                         "clean slot + clean standalone sharing H2 must "
                         "both survive")

    def test_excluded_rows_absent(self):
        got = tuples(FIXTURE)
        self.assertNotIn(ANOMALOUS_TUPLE, got)
        for excluded in (
                ("DX7_AllTheWeb/z-bank.syx", 2, H0, "Ghost", 0),  # no hash
                ("DX7_AllTheWeb/n-voice.syx", None, H1, "Broken", 3),
                ("DX7_AllTheWeb/p-voice.syx", None, H3, "Raw", 4),
                ("DX7_AllTheWeb/b-bank.syx", 0, H0, "Zed Pad", 5),
                ("DX7_AllTheWeb/n2-voice.syx", None, None, "NoHash", 6)):
            self.assertNotIn(excluded, got)

    def test_control_admitting_anomalous_slot_fails_expectation(self):
        """Live failure control: with the anomalous slot temporarily
        admitted (row anomaly zeroed), the exact expectation assertion
        above must FAIL -- proving the assertion has teeth and that the
        anomaly filter is what keeps the anomalous tuple out."""
        admitted = copy.deepcopy(FIXTURE)
        w = admitted["entries"][0]["w"]
        w[4 * 1 + 3] = 0  # slot 1 row: anomaly -> 0
        got = tuples(admitted)
        self.assertIn(ANOMALOUS_TUPLE, got,
                      "control is dead: admitting the slot changed nothing")
        with self.assertRaises(AssertionError):
            self.assertEqual(got, EXPECTED)


class TestSelectorsShareTheRule(unittest.TestCase):
    """Both selectors must expose the shared implementation itself."""

    @classmethod
    def setUpClass(cls):
        cls.dev32 = load_tool(SELECT, "select_dev32_shared_check")
        cls.u02 = load_tool(U02, "u02_candidates_shared_check")

    def test_clean_instances_is_one_function(self):
        self.assertIs(self.dev32.clean_instances,
                      catalog_instances.clean_instances)
        self.assertIs(self.u02.clean_instances,
                      catalog_instances.clean_instances)

    def test_instance_record_is_one_class(self):
        self.assertIs(self.dev32.Instance, catalog_instances.Instance)
        self.assertIs(self.u02.Instance, catalog_instances.Instance)

    def test_shared_rule_matches_fixture_through_both_selectors(self):
        for mod in (self.dev32, self.u02):
            self.assertEqual(
                [(i.path, i.slot, i.canonical_hash, i.name, i.order)
                 for i in mod.clean_instances(FIXTURE)],
                EXPECTED)


if __name__ == "__main__":
    unittest.main()
