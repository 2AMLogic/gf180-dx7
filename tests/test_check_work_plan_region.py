"""Tests for tools/check_work_plan_region.py — the #249 generated-region lint.

The lint checks the static, state-independent shape of the
<!-- guide:plan-body:start --> ... end region of WORK_PLAN.md against a
literal table of render_plan_body()'s headings, blurbs, bullet format and
Backlog Balance row labels. These tests hold an INDEPENDENT hand-written
fixture (not generated from the lint's table), so drift on either side turns
a case red.

Cases:
  (a) a well-shaped region (with _None._ empty sections, curated/architect/
      hermit suffixes, titles containing * and |, the full table) -> exit 0;
  (b) the three PR #248 deviations, each failing for its own reason:
      a hand-added *(held)* bullet suffix, a reworded Backlog Balance row
      label, a rewritten ## Ready blurb (distinct messages);
  (c) a missing start / end marker reported as a failure, never a silent
      pass;
  (d) headings out of renderer order -> exit 1;
  (e) the real committed WORK_PLAN.md -> exit 0 (acceptance case).

Shape only: none of this establishes agreement with live forge label state.
"""

import os
import subprocess
import sys
import tempfile
import unittest

CHECKER = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "tools", "check_work_plan_region.py")

START_MARKER = "<!-- guide:plan-body:start -->"
END_MARKER = "<!-- guide:plan-body:end -->"

# Independent fixture mirroring render_plan_body()'s emitted shape
# (.loom/roles/guide.md, 2004-2135 as of 8094de7). Includes the listed edge
# cases: empty sections (_None._), all three valid bullet suffixes, and a
# title containing both * and |.
REGION = """## Operator Attention: Merge-Risk-Hold Pileup

Judge-approved PRs under a hold (`loom:blocked` or `loom:operator`) — only a human merge decision is missing.

_None._

## Very Important

`loom:very-important` (one counts per workspace); every stage takes these first.

_None._

## Important

`loom:important`; taken before unlabelled work.

- **#2**: Epic: E2 — Verified core and physical feasibility

## Ready

Human-approved issues ready for implementation (`loom:issue`).

_None._

## In Progress

Issues currently being built (`loom:building`).

- **#7**: Fix `a*b` routing | table edge case

## PRs Awaiting Review

PRs waiting on Judge (`loom:review-requested`).

_None._

## Approved (Awaiting Merge)

PRs that passed review and are queued for Champion auto-merge (`loom:pr`).

_None._

## Proposed

Issues carrying `loom:curated`.

- **#2**: Epic: E2 — Verified core and physical feasibility *(curated)*

## Proposed (Architect / Hermit)

- **#161**: Consolidate parse_events *(architect)*
- **#208**: Consolidate Freqlut implementations *(hermit)*

## Epics

- **#2**: Epic: E2 — Verified core and physical feasibility

## Backlog Balance

| Bucket | Count |
|------|-------|
| Operator merge-risk holds | 0 |
| Very important | 0 |
| Important | 1 |
| Ready (`loom:issue`) | 0 |
| In Progress (`loom:building`) | 1 |
| PRs awaiting review | 0 |
| Approved PRs awaiting merge | 0 |
| Curated | 1 |
| Architect / Hermit proposals | 2 |
| Active epics | 1 |"""

WORK_PLAN = f"""# Work Plan

This roadmap is generated from the current GitHub label state by the Loom Guide role.

{START_MARKER}
{REGION}
{END_MARKER}
"""

HELD_SUFFIX_MUTATION = (
    "- **#2**: Epic: E2 — Verified core and physical feasibility\n\n"
    "## Backlog Balance")
HELD_SUFFIX_MUTATED = (
    "- **#2**: Epic: E2 — Verified core and physical feasibility *(held)*\n\n"
    "## Backlog Balance")
READY_ROW_MUTATION = "| Ready (`loom:issue`) | 0 |"
READY_ROW_MUTATED = "| Ready for pickup | 0 |"
READY_BLURB_MUTATION = (
    "Human-approved issues ready for implementation (`loom:issue`).")
READY_BLURB_MUTATED = "Issues that are good to go for building."


class CheckWorkPlanRegionTestCase(unittest.TestCase):
    def run_checker(self, text):
        with tempfile.NamedTemporaryFile(
                "w", suffix=".md", delete=False, encoding="utf-8") as f:
            f.write(text)
            path = f.name
        self.addCleanup(os.unlink, path)
        return subprocess.run(
            [sys.executable, CHECKER, path], capture_output=True, text=True)

    def assert_ok(self, proc):
        self.assertEqual(
            proc.returncode, 0,
            f"expected exit 0, got {proc.returncode}: "
            f"{proc.stdout}{proc.stderr}")
        return proc.stdout + proc.stderr

    def assert_fail(self, proc, needle):
        self.assertEqual(
            proc.returncode, 1,
            f"expected exit 1, got {proc.returncode}: "
            f"{proc.stdout}{proc.stderr}")
        output = proc.stdout + proc.stderr
        self.assertIn(needle, output)
        return output

    def mutated(self, old, new):
        text = WORK_PLAN.replace(old, new)
        self.assertNotEqual(text, WORK_PLAN, f"mutation did not apply: {old!r}")
        return text

    def test_pass_fixture(self):
        self.assert_ok(self.run_checker(WORK_PLAN))

    def test_real_work_plan_passes(self):
        """The committed WORK_PLAN.md in this checkout must be shape-ok."""
        proc = subprocess.run(
            [sys.executable, CHECKER], capture_output=True, text=True)
        self.assert_ok(proc)

    def test_held_suffix_fails(self):
        """PR #248 deviation 1: hand-added *(held)* bullet annotation."""
        out = self.assert_fail(
            self.run_checker(self.mutated(HELD_SUFFIX_MUTATION,
                                          HELD_SUFFIX_MUTATED)),
            "unrendered annotation")
        self.assertIn("*(held)*", out)

    def test_reworded_backlog_row_fails(self):
        """PR #248 deviation 2: reworded Backlog Balance row label."""
        self.assert_fail(
            self.run_checker(self.mutated(READY_ROW_MUTATION,
                                          READY_ROW_MUTATED)),
            "bucket label")

    def test_rewritten_ready_blurb_fails(self):
        """PR #248 deviation 3: hand-rewritten ## Ready section blurb."""
        self.assert_fail(
            self.run_checker(self.mutated(READY_BLURB_MUTATION,
                                          READY_BLURB_MUTATED)),
            "blurb mismatch")

    def test_three_controls_fail_with_distinct_messages(self):
        held = self.hold_message(self.mutated(HELD_SUFFIX_MUTATION,
                                              HELD_SUFFIX_MUTATED))
        row = self.hold_message(self.mutated(READY_ROW_MUTATION,
                                             READY_ROW_MUTATED))
        blurb = self.hold_message(self.mutated(READY_BLURB_MUTATION,
                                               READY_BLURB_MUTATED))
        self.assertNotEqual(held, row)
        self.assertNotEqual(row, blurb)
        self.assertNotEqual(held, blurb)

    def hold_message(self, text):
        proc = self.run_checker(text)
        self.assertEqual(
            proc.returncode, 1,
            f"control did not fail: {proc.stdout}{proc.stderr}")
        return proc.stdout + proc.stderr

    def test_missing_start_marker_fails(self):
        bad = WORK_PLAN.replace(f"{START_MARKER}\n", "")
        self.assertNotEqual(bad, WORK_PLAN)
        self.assert_fail(self.run_checker(bad),
                         "missing '<!-- guide:plan-body:start -->' marker")

    def test_missing_end_marker_fails(self):
        bad = WORK_PLAN.replace(f"{END_MARKER}\n", "")
        self.assertNotEqual(bad, WORK_PLAN)
        self.assert_fail(self.run_checker(bad),
                         "missing '<!-- guide:plan-body:end -->' marker")

    def test_both_markers_missing_fail(self):
        bad = WORK_PLAN.replace(f"{START_MARKER}\n", "").replace(
            f"{END_MARKER}\n", "")
        self.assertNotEqual(bad, WORK_PLAN)
        out = self.assert_fail(self.run_checker(bad), "missing")
        self.assertIn("guide:plan-body:start", out)
        self.assertIn("guide:plan-body:end", out)

    def test_headings_out_of_renderer_order_fails(self):
        bad = self.mutated(
            "## Very Important\n\n`loom:very-important`",
            "## Important (misplaced)\n\n`loom:very-important`")
        self.assert_fail(self.run_checker(bad),
                         "does not match the renderer heading")

    def test_missing_file_could_not_run(self):
        proc = subprocess.run(
            [sys.executable, CHECKER,
             os.path.join(tempfile.mkdtemp(), "absent.md")],
            capture_output=True, text=True)
        self.assertEqual(proc.returncode, 2)


if __name__ == "__main__":
    unittest.main()
