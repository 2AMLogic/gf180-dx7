# Decision record: Capability-DAG adoption (evidence-derived status views)

- **DR number:** 0004
- **Status:** Accepted (owner-directed backlog execution, 2026-09-20)
- **Date:** 2026-09-20
- **Deciders:** Repository owner (Joseph Turian), by owner-directed backlog
  execution of issue #45; design drafted by Loom Builder from the pinned
  torchsynth pattern and the sibling-audit rulings
- **Related issues:** #45 (C01), #39 (A01 audit), #1 (E1 epic); depends on
  #11 (R04) and #12 (R05), both landed

## Context

Issue #45 (C01) requires the capability DAG planned in
`docs/dx7-chip-plan-v0.1-2026-09-20.md` section 7: canonical claim nodes whose
verdicts are derived from stamped evidence, so that closing issues or writing
prose can never turn a node green. The reuse audit (A01, issue #39) catalogued
the sibling capability compiler and ruled it **reference-only**
(`docs/reuse/catalog.json`, component `capability-dag-compiler`;
`docs/REUSE-RULINGS.md` negative finding 2):

- Upstream: `2AMLogic/gf180-torchsynth` @ `36bdc67faa1ff2509988b772efcca7a7eb4e7115`
  (Apache-2.0; catalog-pinned paths: `tools/compile_capabilities.py`,
  `tests/test_capabilities.py`, `tests/test_capability_views.py`,
  `spec/schemas/capability-evidence-v1.schema.json`, with SHA-256 recorded in
  the catalog).
- Inherited defect (audit probe, reproduced): the upstream compiler accepted
  ANY existing evidence file as a pass — file existence is not evidence.
- Adoption was gated on: content-hash verification, schema validation, and a
  negative control that demonstrably fails.

R04 (#11) and R05 (#12) have landed, so stampable evidence now exists:
`tools/verify_reference.py`, `tools/render_selfcheck.py`,
`tools/compare_evidence.py`, `tools/capture_trace.py`,
`tools/check_reuse.py`, `tools/validate_dev32.py`, and the unittest
suites they are backed by.

## Decision

Adopt the capability-DAG **pattern** (engine-agnostic evidence DAG with
registered checks, coverage hashing, negative controls, and generated views)
as a locally authored, stdlib-only implementation, per the trace-capture
precedent (pattern adopted, no upstream bytes copied):

- `src/gf180_dx7/capabilities.py` — registry, state machine, views; `CHECKS`
  is the code-review boundary (declaring a name or file can never register an
  evaluator).
- `tools/compile_capabilities.py` — views regeneration, `--check` (CI
  staleness guard), `--strict` (future release gate: every node PASS;
  honestly red today), and the `--stamp` producer path, which is the only
  mode that executes anything and refuses unregistered checks.
- `spec/capabilities-v1.json` — seeded claim DAG (data, not code).
- Evidence records live under `evidence/capabilities/<node>.json` with
  schema `gf180-dx7.capability-evidence/1`.

The ruling in `docs/reuse/catalog.json` moves `capability-dag-compiler` from
`reference-only` to `adapt` (pattern re-expressed locally, no upstream bytes;
destinations byte-pinned). The audit's required fixes are implemented and
tested:

1. **Stamps only from a CHECKS whitelist.** A record whose `check` or `argv`
   is not registered exactly is refused (NO_VERDICT, never PASS);
   `--stamp` refuses unregistered checks outright.
2. **Every stamp carries content-hash coverage.** Records pin SHA-256 over
   the current bytes of node inputs, the check's implementation files, and
   the judge's own bytes (`EVALUATOR_INPUTS`, including the graph spec);
   dirty and untracked bytes count. Any byte change ⇒ STALE.
3. **Missing controls ⇒ NO_VERDICT, never PASS.** A node PASSes only with
   every required negative control recorded as executed AND detected
   (`detected=true`); a `detected=false` record is a NO_VERDICT, and an
   unexpected recorded control is refused.
4. **File existence proves nothing.** A missing evidence record is
   NO_VERDICT (never PASS); a planned node carrying an evidence file is
   refused; an unparseable or schema-violating record is refused.

Status precedence: local evidence first (absent ⇒ NO_VERDICT; refused ⇒
NO_VERDICT; coverage drift ⇒ STALE; check-fail ⇒ FAIL; NOT_RUN recorded as
NOT_RUN; controls gate PASS), then any prerequisite that is not PASS makes
the effective state BLOCKED while keeping the local state visible.

The seeded DAG nodes and their states at adoption (7 evidenced PASS, 8
declared not-yet-evidenced NO_VERDICT): contract → reference-pin →
oracle-identity → renderer → traces/comparator → corpus-dev32; registry,
numeric-profile, fixed-model, rtl-core, rtl-exact, fpga, gf180, instrument
are declared with `check: null` and cannot pass until a check is registered,
run, and stamped. CI runs `compile_capabilities.py --check` on pull requests;
`--strict` is documented as the future release gate and is not passing today.

## Consequences

- Verification statuses are machine-derived; AGENTS.md reporting rules
  (PASS/FAIL/NOT_RUN/BLOCKED/NO_VERDICT/STALE, coverage separate from
  agreement) are enforced by the compiler instead of prose.
- Closing an issue, adding a file, or editing this document cannot flip a
  node; only `--stamp` after a real registered-check run can produce PASS,
  and any later byte change reopens it as STALE.
- Changing the compiler, the module, or the graph stales ALL recorded
  evidence (the judge covers itself); the affected nodes must be re-stamped.
- `--strict` becomes the release gate only when its red nodes have been
  evidenced; weakening a node's claim or exclusions to get there is not a
  substitute for evidence.
- The DAG encodes capability claims only; it adds no schedule or issue-state
  semantics (the issue DAG in `spec/issue-dag-v1.json` remains the backlog
  view).

## Evidence

- Seeded node states at adoption (all stamped from real runs in this
  repository): contract, reference-pin, oracle-identity, renderer, traces,
  comparator, corpus-dev32 = PASS; registry, numeric-profile, fixed-model,
  rtl-core, rtl-exact, fpga, gf180, instrument = NO_VERDICT. Records:
  `evidence/capabilities/*.json`; rendered view: `docs/CAPABILITIES.md`.
- Negative controls kept live in `tests/test_capabilities.py`: fabricated
  pass without controls ⇒ NO_VERDICT; doctored covered byte ⇒ STALE;
  non-whitelisted check refused (evaluation and `--stamp`); control
  `detected=false` ⇒ NO_VERDICT; deterministic views + mermaid lint; README
  block equality and placement.
- Per-node controls recorded detected=true: pin-drift refusal
  (`tests.test_reference_manifest.TestPinnedTree`), silent-substitution and
  tampered-binary refusal (`tests.test_render_reference.TestNegativeControls`),
  perturbing-stub rejection (`tests.test_trace_capture.TestStubGate`),
  comparator mutation battery (`tests.test_compare.TestMutationBatteryCli`),
  mutated-manifest rejection (`tests.test_dev32.TestNegativeControls`).
- Bounded finding recorded: main's committed smoke render
  (`reference/evidence/smoke/`) predated the current oracle binary pin
  (binary rebuilt at R05; recorded `dc4c1b26…` vs pinned `6e17a540…`), so
  `tools/compare_evidence.py` failed on main as found. The pinned harness was
  re-run to refresh the committed evidence: PCM byte-identical
  (`render.f32`/`render.wav` hashes unchanged), only the recorded oracle
  metadata changed. No acceptance rule was weakened.
- Adoption verification: `python3 -m unittest discover -s tests` green;
  `bash tools/check_all.sh` green; `python3 tools/compile_capabilities.py
  --check` green; `python3 tools/render_issue_dag.py --check` green.
- Not claimed: nothing in this record asserts FPGA or gf180mcu synthesis,
  signoff, hardware playback, original-DX7 fidelity, or preset quality;
  the eight NO_VERDICT nodes state exactly that.
