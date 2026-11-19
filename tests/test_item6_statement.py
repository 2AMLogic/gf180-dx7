"""Row-coverage test for signoff/item6-not-applicable-statement.json (#203).

The statement is a COVERAGE statement, not a `not_applicable` grade: item 6
stays unmet / no_evidence until DR-0008 is ratified and the manifest step
(#264) lands. This test only checks that the statement names exactly the
rows of spec/core-contract-v1.json budget_worst_case.rows.

Live controls (each asserted to fail the check it targets): a dropped row,
an unknown row, a duplicate id, and an empty class/reason.
"""

import copy
import json
import pathlib
import unittest

ROOT = pathlib.Path(__file__).resolve().parent.parent
SPEC = ROOT / "spec" / "core-contract-v1.json"
STATEMENT = ROOT / "signoff" / "item6-not-applicable-statement.json"


def spec_ids(spec):
    return [r["id"] for r in spec["budget_worst_case"]["rows"]]


def coverage_problems(spec_row_ids, statement):
    """Return a list of problems; empty means the statement covers the spec exactly."""
    problems = []
    rows = statement.get("rows")
    if not isinstance(rows, list) or not rows:
        return ["statement has no rows list"]
    ids = []
    for row in rows:
        for key in ("id", "class", "reason"):
            v = row.get(key)
            if not isinstance(v, str) or not v.strip():
                problems.append(f"row {row.get('id')!r}: empty or missing {key}")
        ids.append(row.get("id"))
    dupes = sorted({i for i in ids if ids.count(i) > 1}, key=str)
    if dupes:
        problems.append(f"duplicate ids: {dupes}")
    uncovered = sorted(set(spec_row_ids) - set(ids))
    unknown = sorted(set(ids) - set(spec_row_ids), key=str)
    if uncovered:
        problems.append(f"uncovered_row_ids: {uncovered}")
    if unknown:
        problems.append(f"unknown_row_ids: {unknown}")
    return problems


class Item6StatementTest(unittest.TestCase):
    def setUp(self):
        self.spec = json.loads(SPEC.read_text())
        self.ids = spec_ids(self.spec)
        self.statement = json.loads(STATEMENT.read_text())

    def test_spec_row_set_is_the_required_set(self):
        self.assertEqual(self.ids, self.spec["budget_worst_case"]["required_row_ids"])
        self.assertTrue(self.ids)

    def test_committed_statement_covers_spec_exactly(self):
        self.assertEqual(coverage_problems(self.ids, self.statement), [])

    def test_control_dropped_row_fails(self):
        s = copy.deepcopy(self.statement)
        dropped = s["rows"].pop(0)["id"]
        problems = coverage_problems(self.ids, s)
        self.assertTrue(any("uncovered_row_ids" in p and dropped in p for p in problems))

    def test_control_unknown_row_fails(self):
        s = copy.deepcopy(self.statement)
        s["rows"].append({"id": "not_a_spec_row", "class": "structural", "reason": "x"})
        problems = coverage_problems(self.ids, s)
        self.assertTrue(any("unknown_row_ids" in p and "not_a_spec_row" in p for p in problems))

    def test_control_duplicate_id_fails(self):
        s = copy.deepcopy(self.statement)
        s["rows"].append(copy.deepcopy(s["rows"][0]))
        self.assertTrue(any("duplicate" in p for p in coverage_problems(self.ids, s)))

    def test_control_empty_fields_fail(self):
        for key in ("class", "reason"):
            s = copy.deepcopy(self.statement)
            s["rows"][0][key] = "  "
            self.assertTrue(
                any(f"empty or missing {key}" in p for p in coverage_problems(self.ids, s)),
                key,
            )


if __name__ == "__main__":
    unittest.main()
