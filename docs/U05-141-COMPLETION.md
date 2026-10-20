# #141 completion record (issue #156)

Status vocabulary: PASS / FAIL / NOT_RUN / BLOCKED / NO_VERDICT / STALE.
This record covers review and qualification of the SHA-helper consolidation
delivered by PRs #142, #144 and #145. It is a refactor record. It makes no
RTL, synthesis, reference-agreement, hardware, original-DX7-fidelity or
musical-quality claim, and it changes no source.

**Overall: #141 is NOT closable from this record.** Acceptance criterion 2
(named maintainer review) is BLOCKED and the real-dev32 variant is NOT_RUN.
The full suite (`make test`) now has a result, with 79 skips reported as
NOT_RUN (see "Residual round" below). #141 stays open on the two remaining
conditions.

## Pinned identities

| Item | Identity |
|---|---|
| Source under test | `origin/main` `984d4c9785a4b14fd13665bd9205de7ea93aaa64` (worktree branch `feature/issue-156`, no source edits) |
| PR #142 | merge `b7fd0f4b03fc8d8344f0bf73ccbf9a0f5705eadd`, head `517f1958b2051202289cd256d3b2670ff92e729f` |
| PR #144 | merge `1791b656f830166b05d3d2e1312addef07fad26c`, head `12ef7935fc787709ae42d4814a01a298b2c8a9bb` |
| PR #145 | merge `c360d8c6164786ad01b0c50eaaad559685436c85`, head `10909b8f2f49b1b94dd659dfd6ec3a25458d43fc` |
| Host | `loom-worker-6`, Python 3.12.3, Verilator/yosys/iverilog on `PATH` (`~/.local/bin`), `DX7_ALLTHEWEB_ZIP` unset |

Cleanup file set = `git diff --name-only b7fd0f4^ c360d8c` (29 files).

## Later changes to main (AC 1, AC 5)

`git diff --name-only c360d8c 984d4c9` was intersected with the cleanup set.

- `tools/demo/*`, `tools/sha256_helpers.py`, `tests/test_sha256_helpers.py`,
  `tests/test_u05_demo.py`, `tests/test_u05_full_demo.py`: **unchanged** since
  `c360d8c`. The u05 component's receipts therefore apply to the same bytes.
- `tools/h01|h04|h05|h06_compare.py`, `tools/h01|h04|h05|h06|h07|h08|h10|storage_probe_synth.py`,
  `docs/reuse/catalog.json`: changed later by `a36bbd2`, `79f4b35`/`bc77c11`
  (#152, shared iverilog/yosys helpers) and `7d24e5b` (#150). These are separate
  increments. They do not alter the `sha256_file` import lines this cleanup
  added, and nothing here inherits a #152/#150 receipt.
- `tools/h07_compare.py` is one of the 14 excluded files. It changed in
  `7d24e5b` (functional-verification report, #150), not in any of the three
  cleanup increments. It still carries its private `sha256_file`.
- The other 13 excluded files are byte-identical to `b7fd0f4^` (`git diff --quiet`).

## Per-AC status

| AC | Status | Basis |
|---|---|---|
| 1. Pin identities; map receipts; check later changes | PASS | Identities above. Receipt map below. Later-change check above. |
| 2. Named maintainer review (DR-0002, `u05-demo-fixture-bench`) | **BLOCKED: maintainer review** | See below. |
| 3. Demo/wave qualification and full-suite requirement | PARTIAL: demo PASS, dev32 variant NOT_RUN, `make test` NOT_RUN | See commands below. |
| 4. Negative controls fail their target; clean-pass controls pass | PASS | See below. |
| 5. Independent digest/dependency verification; separate unrelated changes | PASS | See below. |
| 6. 14 excluded files preserved; no evidence regenerated | PASS | See below. |
| 7. Submit through review; close #141 only if all verified | NOT closable | AC 2 BLOCKED. #141 left open. |

### AC 1: receipts for #142 / #144 / #145

Builder reports are the PR bodies. "Independently repeated" means a Judge or
this record re-ran it.

| Check | #142 | #144 | #145 | Repeated here |
|---|---|---|---|---|
| `make test-fast` 463 tests, 12 skipped | builder report | builder report | builder report | **yes** (`984d4c9`, below) |
| Pin check separate from `check_reuse.py` | Judge repeated (digest) | Judge repeated (174 pins) | Judge repeated (2 adapted pins) | **yes** (178 pins, below) |
| First-chunk-only helper control | builder report (4 failures) | builder report (20 F + 1 E) | builder report (21 F + 1 E) | **yes** (below) |
| 2047-file before/after digest comparison | n/a | builder report | n/a | no (not re-derived; AC 5 re-checks the 14 excluded files and pins directly) |
| Dependency audit (importers, evidence digest `e303e113…`) | n/a | builder report | builder report | no (see AC 5 for the repeated part) |
| `tests/test_u05_full_demo.py` | NOT_RUN | NOT_RUN | NOT_RUN | **yes** (below) |
| `make test` | NOT_RUN | NOT_RUN | NOT_RUN | NOT_RUN (below) |
| Maintainer sign-off | not given | not given | not given | BLOCKED (below) |

### AC 2: maintainer review — BLOCKED

The catalog `maintainer` field for `u05-demo-fixture-bench` names the author of
issue #38 (U05). That GitHub login is `turian`. DR-0002 gate 6 requires the
named maintainer to review local changes to an adapted component, and #145
changed `tools/demo/fixtures_dx7.py` and re-pinned its `adapted_sha256`.

No maintainer review of that change exists. The Judge approvals on #145 and the
Champion merge comment each say they are not that sign-off, and the operator's
scope ruling is not that sign-off either. This PR requests the review from
`turian` and records nothing as given. `docs/reuse/catalog.json` is untouched.
Condition to clear: a review by the named maintainer, quoted with a link.

### AC 3: qualification runs

All run from the worktree at `984d4c9`, single process each, no parallelism.

| Command | Result |
|---|---|
| `make test-fast` | PASS: Ran 463 tests in 51.6 s, OK, skipped=12. **Fast lane only, not a full-suite pass.** |
| `python3 -m unittest -v tests.test_u05_full_demo` | PASS for 2 gating tests (full-profile clean run with counts matching the pins, and the full-profile drop-note-off negative control). `TestDev32Variant.test_env_gate_reports_honestly` passes by asserting the gate: it reports `NOT_RUN` because `DX7_ALLTHEWEB_ZIP` is unset. **The real-dev32-patch run is NOT_RUN** (pinned AllTheWeb archive absent on this host). Ran 3 tests, OK. |
| `python3 -m unittest -v tests.test_u05_demo` | PASS: Ran 19, OK |
| `python3 -m unittest -v tests.test_sha256_helpers` | PASS: Ran 7, OK |
| `python3 -m unittest -v tests.test_check_reuse` | PASS: Ran 10, OK |
| `python3 tools/check_reuse.py` | PASS: 38 components, 178 byte-pinned originals, governed destinations clean (not offered as proof for its own refactor) |
| `make test` (full suite, ~15 min) | **NOT_RUN.** It is a judge-approval / wave-gate suite (DR-0009). This worker is a shared 8-vCPU host at load ~8.8, and the suite includes renders and synthesis. It was not started, so there is no full-suite result. |

### AC 4: negative controls

Run in a throwaway `git archive` copy of `984d4c9` under `/tmp` (deleted
afterwards), so no tracked file in the worktree was modified.

- Clean baseline, worktree: `tests.test_sha256_helpers tests.test_u05_demo tests.test_check_reuse tests.test_registry tests.test_dev32` = Ran 97, OK, skipped=1.
- Control: `sha256_file` changed to hash only the first chunk (`h.update(fh.read(CHUNK_SIZE))`). The same 97 tests then gave `FAILED (failures=24, errors=1, skipped=1)`. The control fails the assertions it targets, and the clean baseline passes.
- U05 bench controls (inside `test_u05_demo` / `test_u05_full_demo`, all passing on the clean tree): every corruption mode trips its named gates (`test_every_corruption_mode_verifies`), `--expect-fail` on a clean run is refused, an under-delivering fixture and a one-note audition are refused, and the throttled-underrun negative is non-vacuous. The full-profile control `drop-note-off` passes its `expect_fail_verification`.
- Not repeated: the #142/#144/#145 first-chunk controls were run against their own trees. They are superseded by the repeat above, which is against the current tree.

### AC 5: independent digest and dependency checks

- A separate `python3 -I` script (plain `hashlib`, no `check_reuse.py`) recomputed every `local_originals` pin: 178 entries, 0 mismatches. It also recomputed the two `u05-demo-fixture-bench` `adapted_sha256` entries: `tools/demo/fixtures_dx7.py` `d74a7a7f8e11…` and `tools/demo/demo_fixture.py` `d2c5a798f627…` both match.
- Scope of the search: the catalog only. The repeated part of the dependency boundary is that `tools/demo/*` and the helper are unchanged since `c360d8c` and that the 13 unchanged excluded files are byte-identical to `b7fd0f4^`. The full direct/transitive evidence-dependency search (all tracked blobs, basenames, runtime-hashing sites, `tool_sha256` keys) was done by the #144/#145 builders and is **not re-derived here**. Untracked and out-of-repo consumers were not searched by anyone.

### AC 6: excluded files

The 14 files that still define a private `sha256_file` are `ams_xprobe`, `capture_trace`, `catalog_archive`, `compare_evidence`, `h07_compare`, `h10_collect`, `h10_report`, `n08_build_release`, `render_reference`, `render_selfcheck`, `select_dev32`, `u02_candidates`, `validate_dev32` and `verify_reference` (all `tools/*.py`). Thirteen are unchanged from `b7fd0f4^`. `h07_compare.py` differs only because of #150 (above). No committed evidence, corpus or release file is changed by this PR (the diff is this document only).

## What remains unproved

- Maintainer review of the #145 adapt-record change (BLOCKED).
- The full suite (`make test`) and the real-dev32-patch demo run (NOT_RUN).
- The builder-reported 2047-file digest comparison and the full evidence-dependency search (not re-derived).
- Everything outside link-contract conformance of the mock core: no audio, RTL, hardware or fidelity claim.

## Residual round (issue #156 revision, source `e3fa221199a2638d47d1c7b3a137018583523e8f`)

Run 2026-10-20 on `loom-worker-2` (not the host named above), no source edits.

| ID | Status | Basis |
|---|---|---|
| Stale-receipt check | PASS (no receipt STALE) | `git diff --name-only 984d4c9 e3fa221 -- tools/demo tools/sha256_helpers.py tests/test_u05_demo.py tests/test_u05_full_demo.py tests/test_sha256_helpers.py` is empty. The AC 1/3/4/6 receipts above apply to the same bytes. |
| R-A maintainer review | **BLOCKED: maintainer review** | Re-checked: `gh api .../pulls/157/reviews` returns 0; `.../issues/157/comments` has only the fleet bot; `.../pulls/157/comments` is empty. Nothing from `turian`. `docs/reuse/catalog.json` untouched. |
| R-B real-dev32 variant | **NOT_RUN** | `DX7_ALLTHEWEB_ZIP` unset; pinned archive absent. |
| R-C `make test` | PASS with 79 skips (NOT_RUN) | Below. |
| R-D digest comparison and dependency search | PASS (scoped as stated) | Below. |

### R-C: full suite

Command: `nice make test` (= `python3 -m unittest discover -s tests -v`),
single process, run once from the worktree at `e3fa221`. Started
2026-10-20T01:38:23Z, load average about 4. Result: `Ran 929 tests in 224.996s`,
`OK (skipped=79)`. This is the full suite, not the fast lane (`make test-fast`
was 463 tests, 12 skipped). Measured wall time was about 4 min here, not the
~15 min DR-0009 estimates.

The 79 skips are NOT_RUN, not passes. Reasons, by count: pinned/external oracle
binary absent or hash-drifted (about 50, all the live-oracle sweeps, directed
envelope/algorithm cases, and `test_integrated.TestLiveOracle`); Dexed clone or
tree unavailable (11); ciel 7t liberty or SRAM LEF absent, so the live
synthesis gates did not run (6); pinned AllTheWeb archive absent (3); heavy host
`repo-remote-gf180-dx7` unreachable (1); live ams probe not enabled (1);
local renders absent (1). This run therefore says nothing about oracle
agreement, synthesis or the real dev32 patches.

### R-D: independent digest comparison and dependency search

Both scripts are `python3 -I`, plain `hashlib` and `git show`, no
`check_reuse.py`, kept outside the repo.

- Digest comparison: SHA-256 of every tracked blob at `b7fd0f4^` (2045 files)
  and `c360d8c` (2047 files). Added 2 (`tools/sha256_helpers.py`,
  `tests/test_sha256_helpers.py`), removed 0, changed 27. The changed set is
  exactly `git diff --name-only b7fd0f4^ c360d8c` minus the 2 added: 19 tool
  files, 7 test files, `docs/reuse/catalog.json` and `WORK_LOG.md`. The other
  2018 files are byte-identical, including all 953 files under
  `evidence/`, `corpus/`, `reference/` and `renders`-style trees.
- Dependency search over all 2047 tracked blobs at `c360d8c`: for each of the
  19 changed tool files, the full SHA-256 and git blob SHA-1 at both `b7fd0f4^`
  and `c360d8c` were searched as text. The only file containing any of them is
  `docs/reuse/catalog.json`, and only the new (`c360d8c`) digests. No tracked
  file anywhere records a pre-cleanup digest of a migrated tool, so no committed
  evidence pins the old bytes.
- Basename search of non-`.py` files: 57 files mention a changed tool by name
  (docs, decision records, evidence and `reference/` READMEs/JSON, `rtl/*.v`
  comments, `spec/*.json`, `tools/*.sh`). These are prose or path references;
  none carries a digest of the tool (the digest search above is negative).
- Runtime-hashing and `tool_sha256` keys: 7 files. Six are excluded files or
  corpus outputs that hash `select_dev32`, `catalog_archive` and
  `u02_candidates` (not migrated). `tools/validate_bank128.py` is migrated and
  records `tool_sha256: sha256_file(Path(__file__))` at run time. Its output
  changes only if the file bytes change, as before; its `sha256_file` is now
  imported with identical semantics (guarded by `tests.test_sha256_helpers`).
  No committed file stores that value.
- Not searched: untracked files, ignored renders, other machines, and external
  consumers that might hold an old tool digest.
