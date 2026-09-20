# DR-0002: Reuse governance — pinned, provenance-recorded, requalified imports only

- **DR number:** 0002
- **Status:** Accepted
- **Date:** 2026-09-19
- **Deciders:** loom-builder (agent), pending owner review
- **Related issues:** #39 (A01)

## Context

This repository imports infrastructure (not DSP) from 2AMLogic sibling
repositories and external references. Two sibling records define the method:

- `gf180-torchsynth` DR-0005 ("Keep substrate reuse pinned and locally owned",
  commit `36bdc67faa1ff2509988b772efcca7a7eb4e7115`): provenance-before-import,
  hash-pinned copies, explicit local ownership, requalification after any
  covered change, and the finding that hash equality alone never qualifies.
- `gf180-polysynth` DR-0003 ("Sibling instrument repositories...", commit
  `b60e424ec570d5ef988c0b2c05dc71ba5eed1825`): the shared-substrate rule —
  one master per shared component; the org has zero submodules across ~50
  repositories, and an unchecked copy already shipped two I2S defects silently
  between siblings (polysynth PR #4).

The torchsynth reuse audit additionally demonstrated that plausible-looking
shared infrastructure can be semantically broken: the sibling scorecard passes
a required NaN after a finite metric, and the capability-DAG compiler accepts
any existing file as passed evidence. Neither hash equality nor a green exit
code catches that; only semantic negative controls do.

This repository's own split of concerns (RTL-vs-frozen-model exact;
model-vs-reference budgeted; sound quality only by listening) means imported
verification plumbing must never silently redefine what a pass is.

## Decision

Every import into a governed destination (`src/`, `rtl/`, `tools/`,
`spec/reference/tables/`) follows this gateset, in order:

1. **Catalog entry before import.** A component entry in
   `docs/reuse/catalog.json` (upstream repo, full commit, paths, SHA-256 of
   the pinned bytes, license, planned destination, named maintainer,
   requalification check, negative control) exists before the import PR lands.
   Rulings: `adopt`, `adapt`, `reject`, `reference-only`,
   `external-oracle-only`. Only `adopt`/`adapt` entries authorize an import.
2. **No submodules, no floating deps, no auto-sync.** Shared components are
   pinned reviewed copies or explicitly pinned package dependencies with
   recorded digests. Git submodules, floating versions for qualified runs,
   scheduled upstream synchronization, and unreviewed generated copies are
   forbidden. A submodule exception needs a replacement decision.
3. **One master per shared component.** When two repositories need the same
   component, exactly one repository holds the master; the other consumes a
   pinned, provenance-recorded copy or package. Two independently edited
   copies of one component must never exist.
4. **Provenance before import, per import.** The import PR records upstream
   repo/commit/path, original SHA-256 (computed from the pinned commit, not a
   moving branch), license text path and hash, destination, adapted SHA-256
   for `adapt` rulings, and a reproducible upstream-to-local diff rationale.
   Missing attribution or a hash mismatch refuses adoption.
5. **Hash equality never qualifies.** A matching SHA-256 establishes byte
   identity, nothing else. Each adopted component carries at least one
   requalification check that must actually run (exit 0/1/2: pass / fail /
   could-not-run; a check that did not run is never a pass) plus at least one
   negative control that demonstrably fails when the component breaks.
6. **Divergence and updates are owned.** The named maintainer reviews local
   changes; upstream updates compare old pin to proposed pin and requalify.
   Evidence tied to a superseded pin goes STALE; it is not silently inherited.
7. **Governed destinations are machine-checked.** `tools/check_reuse.py`
   fails when a source file exists in a governed destination without a
   covering `adopt`/`adapt` catalog entry, when an adopted file's bytes drift
   from its recorded hash, or when an `adapt` destination exists without a
   recorded adapted hash. This check is a negative control itself and is part
   of `tools/check_all.sh`.

DSP is out of scope for reuse regardless of provenance: no sibling implements
DX7 six-operator FM, DX envelopes, or the 32-algorithm tree. External GPL
engines (Dexed Mark I engine, VDX7, Hexter) remain comparison oracles and are
never copied into this Apache-2.0 repository.

## Consequences

- Every future import PR must touch `docs/reuse/catalog.json`; a PR that adds
  governed source without a catalog entry fails `tools/check_all.sh`.
- Catalog hashes must be recomputed from pinned commits when an entry is
  added or re-pinned; editing an adopted file requires a catalog update in
  the same PR.
- Rejected components (all sibling DSP, board constraints as-is, unreverified
  ORFS techmaps, gf180-sram macro without Liberty views) cannot be imported
  without a new decision record re-ruling them.
- The torchsynth audit's negative findings are inherited as adoption gates:
  no scorecard adoption without a finiteness gate; no capability-DAG adoption
  before its accepts-any-existing-file defect is fixed and controlled.
- Any weakening of this gateset (e.g. exempting a destination, dropping the
  0/1/2 convention) requires a new decision record.

## Evidence

- Catalog: `docs/reuse/catalog.json` (rulings, pinned commits, SHA-256 of
  every pinned file, computed with `git show <commit>:<path> | shasum -a 256`).
- Rulings table with reasons and inherited negative findings:
  `docs/REUSE-RULINGS.md`.
- Negative control: `tools/check_reuse.py`, exercised by
  `tests/test_check_reuse.py` (unrecorded file fails; drifted hash fails;
  compliant tree passes).
- Upstream governance sources: torchsynth DR-0005 and its 70-file audit
  catalog at commit `36bdc67faa1ff2509988b772efcca7a7eb4e7115`; polysynth
  DR-0003 at commit `b60e424ec570d5ef988c0b2c05dc71ba5eed1825`.
