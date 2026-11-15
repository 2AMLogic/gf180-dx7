# Skip-evidence snapshots (issue #206)

Generated evidence snapshots retained per issue #206's evidence-retention
requirement: a stamp pair of `numeric-profile` taken with and without the
pinned dexed tree, showing that the two records differ in reviewable fields
and that the treeless one never reads PASS-with-everything-verified.

All files here are **generated snapshots, not live records** — the live
evidence record for each node is `evidence/capabilities/<node>.json`, and
the live rendered views are `docs/CAPABILITIES.md`, `docs/capabilities.json`
and the README capability block. This folder exists so the with-tree state
(which requires `tools/fetch_dexed.sh`: a network clone plus submodules,
present only on machines that have fetched it) remains reviewable from the
commit history without re-fetching.

## What the pair shows

| field | with-tree (`*.with-tree.*`) | treeless (live record) |
| --- | --- | --- |
| `result` | PASS | NOT_RUN |
| `check_skip_reported` | true | true |
| citation NOT_RUN (check output tail) | 3 of 71 (unpublished `dx7-oracle-marki` tree) | 71 of 71 (pinned tree unavailable) |
| control `detected` | true | false |
| control `tests_run` | 11 | 11 |
| control `skipped` | 0 | 1 |

The with-tree record stays PASS under the node's declared
`skip_policy: "pass"` because the only remaining skip is the check-level
3-citation gap already named in the node's `exclusions`. The treeless stamp
forces NOT_RUN: the checker skipped every citation and the required control
reported `OK (skipped=1)` — the citation-line-rot clause of its `detects`
string never ran.

## How these were produced

On a Linux x86_64 fleet worker, from the feature/issue-206 worktree, with
the evaluator bytes of this PR final:

1. `tools/fetch_dexed.sh` (clones github.com/asb2m10/dexed pinned at
   `b6cf2e9190e216387ad96f977284ebbef765b447` + recursive submodules into
   the git-ignored `.oracle/dexed`), then
   `python3 tools/compile_capabilities.py --stamp numeric-profile`.
   `numeric-profile.with-tree.json` and `CAPABILITIES.with-tree.md` /
   `capabilities.with-tree.json` are the record and rendered views written
   at that moment.
2. `rm -rf .oracle` (treeless), then the same `--stamp numeric-profile`
   command — that record is the live
   `evidence/capabilities/numeric-profile.json` (copied here as
   `numeric-profile.treeless.json` for side-by-side review; the live path
   is canonical).
3. `python3 tools/compile_capabilities.py --stamp oracle-identity` on the
   treeless host exercises the wholly-skipped required control
   (`OK (skipped=2)`): the live
   `evidence/capabilities/oracle-identity.json` records `detected: false`
   with `tests_run: 2, skipped: 2` and result NOT_RUN.
