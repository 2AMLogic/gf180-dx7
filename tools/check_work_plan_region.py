#!/usr/bin/env python3
"""Structural lint for the generated region of WORK_PLAN.md (issue #249).

The Guide role renders the plan body with render_plan_body() and
update_work_plan() compares that output byte-for-byte against the committed
<!-- guide:plan-body:start --> ... end region, so any hand-written edit to
the region guarantees a permanent render mismatch and a revert PR on every
subsequent tick. Two consecutive docs-maintenance PRs (#247, #248) did
exactly that (hand-added *(held)* bullet suffixes, a reworded Backlog
Balance row label, a rewritten ## Ready blurb). This lint turns each such
recurrence into a failing check instead of a merged regression.

It checks the STATIC, state-independent shape only:

  - the marker pair exists (exactly one of each, start before end);
  - the ## headings appear in render_plan_body() order;
  - each section's blurb equals the renderer's literal text (or is absent
    where the renderer emits none);
  - every bullet matches the renderer's format "- **#N**: title" with at
    most one trailing renderer suffix *(curated)* / *(architect)* /
    *(hermit)*; any other trailing annotation fails, in any order (a
    hand-added *(held)* last, or a second annotation placed before a valid
    suffix); a mid-title *(word)* is not inspected;
  - the Backlog Balance table carries exactly the renderer's ten row labels
    in order (count values are state-dependent and NOT checked).

It deliberately does NOT check agreement with live forge label state —
whether a section should hold bullets or _None._ right now is unknowable
offline, so this check proves shape fidelity, never content agreement.
Byte-fidelity to a live tick remains update_work_plan()'s job.

Expected headings, blurbs and row labels are held as one literal table
below, citing .loom/roles/guide.md render_plan_body() (lines 2004-2135 as
of 8094de7). A Loom resync that changes the renderer turns this check red,
which is the "edit renderer and committed file together" rule made
mechanical.

Exit codes: 0 shape ok, 1 shape violation, 2 could-not-run. Stdlib only.
"""

import os
import re
import sys

START_MARKER = "<!-- guide:plan-body:start -->"
END_MARKER = "<!-- guide:plan-body:end -->"

# One literal table: (heading, blurb) in render_plan_body() order. blurb is
# None where section() is called with an empty blurb (no blurb line emitted).
# Source: .loom/roles/guide.md render_plan_body() (2004-2135 as of 8094de7).
SECTIONS = [
    ("Operator Attention: Merge-Risk-Hold Pileup",
     "Judge-approved PRs under a hold (`loom:blocked` or `loom:operator`) "
     "— only a human merge decision is missing."),
    ("Very Important",
     "`loom:very-important` (one counts per workspace); every stage takes "
     "these first."),
    ("Important", "`loom:important`; taken before unlabelled work."),
    ("Ready", "Human-approved issues ready for implementation (`loom:issue`)."),
    ("In Progress", "Issues currently being built (`loom:building`)."),
    ("PRs Awaiting Review", "PRs waiting on Judge (`loom:review-requested`)."),
    ("Approved (Awaiting Merge)",
     "PRs that passed review and are queued for Champion auto-merge "
     "(`loom:pr`)."),
    ("Proposed", "Issues carrying `loom:curated`."),
    ("Proposed (Architect / Hermit)", None),
    ("Epics", None),
    ("Backlog Balance", None),
]

# Backlog Balance row labels, in render_plan_body() order (same source).
BACKLOG_ROWS = [
    "Operator merge-risk holds",
    "Very important",
    "Important",
    "Ready (`loom:issue`)",
    "In Progress (`loom:building`)",
    "PRs awaiting review",
    "Approved PRs awaiting merge",
    "Curated",
    "Architect / Hermit proposals",
    "Active epics",
]

TABLE_HEADER = "| Bucket | Count |"
TABLE_SEPARATOR = "|------|-------|"
NONE_BODY = "_None._"

# "- **#N**: title" — the only bullet shape render_plan_body() emits.
BULLET_RE = re.compile(r"^- \*\*#\d+\*\*: (.+)$")
# A trailing " *(word)*" annotation on a title, e.g. the hand-added
# "*(held)*" from PRs #247/#248. Validated against VALID_SUFFIXES because a
# plain `.+` title regex would greedily swallow any suffix and let the
# annotation through. Applied twice in the bullet check: once to the title
# (the one renderer suffix), then to the remainder after stripping a valid
# suffix, so a second trailing annotation before a valid suffix also fails
# (issue #251). Mid-title *(word)* is never inspected.
SUFFIX_RE = re.compile(r" \*\(([A-Za-z][A-Za-z ]*)\)\*$")
VALID_SUFFIXES = {"curated", "architect", "hermit"}
COUNT_RE = re.compile(r"^\d+$")


def lint_lines(lines):
    """Return a list of shape-violation messages ([] means shape ok)."""
    errors = []
    starts = [i for i, line in enumerate(lines) if line.strip() == START_MARKER]
    ends = [i for i, line in enumerate(lines) if line.strip() == END_MARKER]
    if not starts:
        errors.append("missing '<!-- guide:plan-body:start -->' marker — "
                      "cannot locate the generated region")
    elif len(starts) > 1:
        errors.append("duplicate '<!-- guide:plan-body:start -->' markers at "
                      f"lines {[i + 1 for i in starts]}")
    if not ends:
        errors.append("missing '<!-- guide:plan-body:end -->' marker — "
                      "cannot locate the generated region")
    elif len(ends) > 1:
        errors.append("duplicate '<!-- guide:plan-body:end -->' markers at "
                      f"lines {[i + 1 for i in ends]}")
    if not starts or not ends:
        return errors
    s_idx, e_idx = starts[0], ends[0]
    if e_idx <= s_idx:
        errors.append(f"end marker (line {e_idx + 1}) appears before start "
                      f"marker (line {s_idx + 1})")
        return errors
    region = lines[s_idx + 1:e_idx]

    headings = [(j, line) for j, line in enumerate(region)
                if line.startswith("## ")]
    if len(headings) != len(SECTIONS):
        errors.append(
            f"found {len(headings)} '## ' headings in the generated region, "
            f"render_plan_body() emits {len(SECTIONS)} "
            f"({', '.join(name for name, _ in SECTIONS)})")
    pairs = list(zip(headings, SECTIONS))
    bounds = [j for j, _ in headings] + [len(region)]
    for k, ((j, line), (name, blurb)) in enumerate(pairs):
        lineno = s_idx + 1 + j + 1  # 1-based file line of the heading
        expected_line = f"## {name}"
        if line != expected_line:
            errors.append(
                f"line {lineno}: heading {line!r} does not match the "
                f"renderer heading {expected_line!r} at position {k + 1} "
                "(wrong name, out of order, or extra spacing)")
            continue
        body = region[j + 1:bounds[k + 1]]
        _check_section_body(errors, body, name, blurb, is_last=(
            k == len(pairs) - 1), heading_lineno=lineno, s_idx=s_idx)
    return errors


def _check_section_body(errors, body, name, blurb, is_last, heading_lineno,
                        s_idx):
    """Check one section's lines after its heading against section()."""
    offset = heading_lineno + 1  # file line number of body[0]
    if not body or body[0] != "":
        errors.append(f"section {name!r} (line {heading_lineno}): missing "
                      "blank line after the heading")
        return
    body = body[1:]
    if blurb is not None:
        if not body or body[0] != blurb:
            got = body[0] if body else "<section truncated>"
            errors.append(
                f"section {name!r} (line {offset + 1}): blurb mismatch — "
                f"render_plan_body() emits {blurb!r}, committed region has "
                f"{got!r}")
            return
        body = body[1:]
        if not body or body[0] != "":
            errors.append(f"section {name!r} (line {offset + 2}): missing "
                          "blank line after the blurb")
            return
        body = body[1:]
    if is_last:
        if body and body[-1] == "":
            errors.append(f"section {name!r}: trailing blank line before the "
                          "end marker — render_plan_body() emits none")
            return
    else:
        if body and body[-1] == "":
            body = body[:-1]  # the renderer's inter-section separator
        if body and body[-1] == "":
            errors.append(f"section {name!r}: more than one blank line before "
                          "the next section heading")
            return
    if not body:
        errors.append(f"section {name!r} (line {offset + 1}): empty section "
                      "body — render_plan_body() emits a body or _None._")
        return
    if name == "Backlog Balance":
        _check_table(errors, body, offset)
        return
    if body == [NONE_BODY]:
        return
    for k, line in enumerate(body):
        lineno = offset + k + 1
        if line == "":
            errors.append(f"section {name!r} (line {lineno}): blank line "
                          "inside the section body")
            continue
        if line == NONE_BODY:
            errors.append(f"section {name!r} (line {lineno}): {NONE_BODY!r} "
                          "mixed with other body lines")
            continue
        m = BULLET_RE.match(line)
        if not m:
            errors.append(
                f"section {name!r} (line {lineno}): line does not match the "
                f"renderer bullet format '- **#N**: title': {line!r}")
            continue
        sm = SUFFIX_RE.search(m.group(1))
        if sm and sm.group(1).strip() not in VALID_SUFFIXES:
            errors.append(
                f"section {name!r} (line {lineno}): unrendered annotation "
                f"*({sm.group(1).strip()})* — render_plan_body() emits only "
                "*(curated)*, *(architect)* or *(hermit)* bullet suffixes")
        elif sm:
            # The trailing suffix is a valid renderer one; strip it and
            # re-apply SUFFIX_RE to the remainder: a second trailing
            # annotation (a hand annotation placed before the suffix, or a
            # doubled suffix) is also unrendered — render_plan_body()
            # appends exactly one suffix.
            sm2 = SUFFIX_RE.search(m.group(1)[:sm.start()])
            if sm2:
                errors.append(
                    f"section {name!r} (line {lineno}): unrendered annotation "
                    f"*({sm2.group(1).strip()})* — render_plan_body() appends "
                    "at most one bullet suffix; a second trailing annotation "
                    "never renders")


def _check_table(errors, body, offset):
    """Check the Backlog Balance table shape (labels in order; counts free)."""
    name = "Backlog Balance"
    expected = [TABLE_HEADER, TABLE_SEPARATOR] + [
        f"| {label} | <count> |" for label in BACKLOG_ROWS]
    if len(body) != len(expected):
        errors.append(
            f"section {name!r} (line {offset + 1}): table has {len(body)} "
            f"rows, render_plan_body() emits {len(expected)} "
            "(header, separator, 10 buckets)")
        return
    if body[0] != TABLE_HEADER or body[1] != TABLE_SEPARATOR:
        errors.append(
            f"section {name!r} (line {offset + 1}): table header mismatch — "
            f"render_plan_body() emits {TABLE_HEADER!r}, "
            f"{TABLE_SEPARATOR!r}, committed region has {body[0]!r}, "
            f"{body[1]!r}")
        return
    for k, line in enumerate(body[2:]):
        lineno = offset + 2 + k + 1
        cells = [c.strip() for c in line.split("|")]
        if (len(cells) != 4 or cells[0] != "" or cells[3] != ""
                or not COUNT_RE.match(cells[2])):
            errors.append(
                f"section {name!r} (line {lineno}): row does not match the "
                f"renderer table-row format '| Bucket | Count |': {line!r}")
            continue
        if cells[1] != BACKLOG_ROWS[k]:
            errors.append(
                f"section {name!r} (line {lineno}): bucket label "
                f"{cells[1]!r} != render_plan_body() label "
                f"{BACKLOG_ROWS[k]!r}")


def main(argv):
    path = argv[1] if len(argv) > 1 else os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        "WORK_PLAN.md")
    try:
        with open(path, encoding="utf-8") as f:
            lines = f.read().splitlines()
    except OSError as exc:
        print(f"{path}: could not read ({exc})", file=sys.stderr)
        return 2
    errors = lint_lines(lines)
    if errors:
        for err in errors:
            print(f"WORK_PLAN.md generated region: {err}", file=sys.stderr)
        print(f"WORK_PLAN.md generated region: FAIL ({len(errors)} shape "
              "violation(s) vs render_plan_body())", file=sys.stderr)
        return 1
    print("WORK_PLAN.md generated region: shape ok (headings, blurbs, "
          "bullet format and Backlog Balance labels match render_plan_body())")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
