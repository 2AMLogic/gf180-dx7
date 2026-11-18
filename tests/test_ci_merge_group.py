"""Merge-queue CI contract for .github/workflows/ci.yml (issue #256).

The chip merge queue (2AMLogic/2am#7605) fires this repo's required checks
on `merge_group` events. A workflow that does not declare the trigger, or a
job that skips on a `merge_group` run, is a silent drop: the queue would
wait forever on a check that never reports. This module pins the contract
with a stdlib-only scanner (this repo's tests use no PyYAML):

- ci.yml declares the bare `merge_group:` trigger (default types, no
  branch/path filter, no sub-keys),
- no trigger in the `on:` block carries a `paths:` or `paths-ignore:`
  filter (issue #261),
- the bare `pull_request:` trigger stays declared,
- every job/step `if:` conditioned on `'pull_request'` also admits
  `'merge_group'`.

`dag.yml` hosts no required context and is deliberately left untouched, so
it is not scanned here. The negative-control tests feed stale fixtures
through the same scanner and must fail: a control demonstrably fails the
check it targets (repo policy).
"""

import re
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
CI_YML = REPO / ".github" / "workflows" / "ci.yml"


class MergeGroupContractError(AssertionError):
    """Raised when workflow text violates the merge-queue CI contract."""


def _on_block(lines, name):
    """Return (start, end) line indexes of the top-level `on:` block."""
    for i, line in enumerate(lines):
        if re.match(r"^on:\s*(#.*)?$", line):
            end = len(lines)
            for j in range(i + 1, len(lines)):
                if lines[j].strip() and not lines[j][0].isspace():
                    end = j
                    break
            return i, end
    raise MergeGroupContractError(f"{name}: no top-level `on:` block found")


def check_merge_group_contract(text, name="workflow"):
    """Raise MergeGroupContractError unless text pins the merge-queue CI contract."""
    lines = text.splitlines()
    on_start, on_end = _on_block(lines, name)
    body = lines[on_start + 1:on_end]

    triggers = [k for k, ln in enumerate(body)
                if re.match(r"^ {2}\w[^:]*:\s*(#.*)?$", ln)]
    mg = [k for k in triggers if re.match(r"^ {2}merge_group:\s*(#.*)?$", body[k])]
    if not mg:
        raise MergeGroupContractError(
            f"{name}: `on:` does not declare a bare `merge_group:` trigger "
            "(default types, no branch/path filter)")
    for ln in body[mg[0] + 1:]:
        if not ln.strip():
            continue
        if ln[0].isspace():
            raise MergeGroupContractError(
                f"{name}: `merge_group:` trigger must be bare, saw sub-key "
                f"{ln.strip()!r}")
        break

    if not any(re.match(r"^ {2}pull_request:\s*(#.*)?$", ln) for ln in body):
        raise MergeGroupContractError(
            f"{name}: bare `pull_request:` trigger is missing from `on:`")

    for ln in body:
        m = re.match(r"^ +paths(-ignore)?:", ln)
        if m:
            raise MergeGroupContractError(
                f"{name}: `on:` block adds a `{m.group(0).strip()}` filter: "
                f"{ln.strip()!r}")

    for ln in lines:
        m = re.match(r"^\s*if:\s*(\S.*?)\s*$", ln)
        if m and "'pull_request'" in m.group(1) and "merge_group" not in m.group(1):
            raise MergeGroupContractError(
                f"{name}: job guard skips merge_group runs: {m.group(1)!r}")
    return True


# The pre-#256 ci.yml shape: no merge_group trigger (the silent drop).
FIXTURE_NO_TRIGGER = """\
name: stale
on:
  push:
    branches: [main]
  pull_request:

jobs:
  dag-fresh:
    if: github.event_name == 'pull_request' || github.event_name == 'merge_group'
    steps:
      - run: echo hi
"""

# merge_group declared but a job still guarded PR-only (the other silent drop).
FIXTURE_OLD_GUARD = """\
name: stale
on:
  push:
    branches: [main]
  pull_request:
  merge_group:

jobs:
  dag-fresh:
    if: github.event_name == 'pull_request'
    steps:
      - run: echo hi
"""

# The trigger itself filtered to a path subset (queue commits can skip it).
FIXTURE_MG_SUBKEY = """\
name: stale
on:
  push:
    branches: [main]
  pull_request:
  merge_group:
    paths:
      - "src/**"

jobs:
  test:
    steps:
      - run: echo hi
"""

# A path filter on any trigger of the required-check workflow.
FIXTURE_PUSH_PATHS = """\
name: stale
on:
  push:
    branches: [main]
    paths:
      - "src/**"
  pull_request:
  merge_group:

jobs:
  test:
    steps:
      - run: echo hi
"""

# A path filter in the other direction (`paths-ignore:` filters runs just as
# effectively as `paths:`), on a non-merge_group trigger (issue #261).
FIXTURE_PUSH_PATHS_IGNORE = """\
name: stale
on:
  push:
    branches: [main]
  pull_request:
    paths-ignore: ["**/*.md"]
  merge_group:

jobs:
  test:
    steps:
      - run: echo hi
"""


class CiMergeGroupContractTest(unittest.TestCase):
    def test_merge_group_contract_on_committed_ci_yml(self):
        check_merge_group_contract(
            CI_YML.read_text(), name=".github/workflows/ci.yml")

    def test_merge_group_trigger_declared_bare(self):
        text = CI_YML.read_text()
        lines = text.splitlines()
        on_start, on_end = _on_block(lines, ".github/workflows/ci.yml")
        body = lines[on_start + 1:on_end]
        self.assertTrue(
            any(re.match(r"^ {2}merge_group:\s*$", ln) for ln in body),
            f"bare merge_group trigger missing from `on:` block: {body!r}")

    def test_merge_group_on_block_has_no_path_filter(self):
        text = CI_YML.read_text()
        lines = text.splitlines()
        on_start, on_end = _on_block(lines, ".github/workflows/ci.yml")
        for ln in lines[on_start + 1:on_end]:
            m = re.match(r"^ +paths(-ignore)?:", ln)
            if m:
                self.fail(f"unexpected `{m.group(0).strip()}` filter: "
                          f"{ln.strip()!r}")

    def test_merge_group_pr_only_guards_admit_merge_group(self):
        text = CI_YML.read_text()
        guards = [ln.strip() for ln in text.splitlines()
                  if re.match(r"^\s*if:.*'pull_request'", ln)]
        self.assertEqual(len(guards), 3,
                         f"expected the 3 PR guards (dag-fresh, "
                         f"capabilities-fresh, signoff-fresh), saw {guards!r}")
        for guard in guards:
            self.assertIn("merge_group", guard,
                          f"guard does not admit merge_group: {guard!r}")

    def test_merge_group_negative_control_missing_trigger_fails(self):
        with self.assertRaises(MergeGroupContractError) as cm:
            check_merge_group_contract(FIXTURE_NO_TRIGGER, name="no-trigger")
        self.assertIn("bare `merge_group:` trigger", str(cm.exception))

    def test_merge_group_negative_control_old_pr_only_guard_fails(self):
        with self.assertRaises(MergeGroupContractError) as cm:
            check_merge_group_contract(FIXTURE_OLD_GUARD, name="old-guard")
        self.assertIn("skips merge_group runs", str(cm.exception))

    def test_merge_group_negative_control_merge_group_subkey_fails(self):
        with self.assertRaises(MergeGroupContractError) as cm:
            check_merge_group_contract(FIXTURE_MG_SUBKEY, name="mg-subkey")
        self.assertIn("must be bare", str(cm.exception))

    def test_merge_group_negative_control_push_path_filter_fails(self):
        with self.assertRaises(MergeGroupContractError) as cm:
            check_merge_group_contract(FIXTURE_PUSH_PATHS, name="push-paths")
        self.assertIn("`paths:` filter", str(cm.exception))

    def test_merge_group_negative_control_paths_ignore_filter_fails(self):
        with self.assertRaises(MergeGroupContractError) as cm:
            check_merge_group_contract(
                FIXTURE_PUSH_PATHS_IGNORE, name="pr-paths-ignore")
        self.assertIn("`paths-ignore:` filter", str(cm.exception))
        # Same widened check catches `paths-ignore:` under `push:` (issue #261).
        with self.assertRaises(MergeGroupContractError) as cm_push:
            check_merge_group_contract(
                FIXTURE_PUSH_PATHS.replace("paths:", "paths-ignore:"),
                name="push-paths-ignore")
        self.assertIn("`paths-ignore:` filter", str(cm_push.exception))
        # A commented-out line is inert, exactly as for `paths:`: the regex
        # anchors on the key itself, not a `#`-prefixed mention.
        commented = FIXTURE_PUSH_PATHS_IGNORE.replace(
            '    paths-ignore: ["**/*.md"]', '    # paths-ignore: ["**/*.md"]')
        check_merge_group_contract(commented, name="commented-paths-ignore")


if __name__ == "__main__":
    unittest.main()
