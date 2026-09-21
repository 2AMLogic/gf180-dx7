# DR-0007: The frozen fixed-model release v1 (release-vectors-v1) — what is frozen, the challenge protocol, STALE rules, and the authorization of exact RTL work

- **DR number:** 0007
- **Status:** Proposed
- **Date:** 2026-09-20
- **Deciders:** loom-builder (agent), pending owner review
- **Related issues:** #24 (N08); grounds #2 epic H-series conformance (H03+);
  builds on #22 (N06), #23 (N07), #13 (R06), #15 (N01 numeric profile)

## Decision

The frozen fixed-model release **`release-vectors-v1`** is adopted. The
release consists of:

1. **The preregistration** — `spec/release-vectors-v1.json`: the development
   set, the challenge set, the acceptance thresholds, and the exclusion
   list, frozen **before any challenge render was inspected**. Its binding
   hashes pin the exact registry, numeric profile, dev32 corpus, and model
   module bytes it was frozen against.
2. **The bundle** — `reference/release/` (built by
   `tools/n08_build_release.py`): one dry f32le PCM vector per case
   (`vectors/<case>.f32`), and `manifest.json` holding the case list with
   per-case expected PCM sha256 and block-level event-application traces,
   the model identity (module file hashes), the arithmetic-profile pointer,
   the frozen thresholds, the derived per-property error budgets, the
   excluded-case record, and the **bundle hash**
   `81bcc43cd49a29f35b21370733bb9888dd5641862501b441136e122485f54f8c`.
3. **The frozen challenge record** —
   `reference/release/challenge-results.json`: the preregistered challenge
   set rendered through the model and compared against the pinned oracle
   **at the frozen thresholds**, bound to the bundle hash.
4. **The live mutation battery** — `tools/n08_mutations.py` and its
   committed results `reference/release/mutation-results.json` (also bound
   to the bundle hash).

The verification entry point is `tools/n08_verify_release.py`
(exit 0 = intact, 1 = tamper/drift/threshold failure naming the case,
2 = bundle missing). This is the oracle every H-series RTL conformance run
executes against.

## What is frozen

- **The model:** the merged N06+N07 tree
  (`src/gf180_dx7/sysex.py` + `src/gf180_dx7/model/{__init__,operator,
  envelope,algorithm,pitch,tables,integrated,poly}.py`), byte-pinned by
  sha256 in both the preregistration binding and the bundle manifest. Every
  bundle case renders through the D00 poly manager around the integrated
  single-note composition; the 23 single-note dev cases additionally
  assert byte-equality of the two layers (`integrated_equal: true`).
- **The arithmetic profile:** `spec/numeric-profile-v1.json` by sha256;
  the per-property error budgets recorded in the manifest are DERIVED from
  that file (its `decision_index` error estimates and float-gated spots),
  never tuned.
- **The vectors:** 51 cases (30 dev + 21 challenge), 15,456,000 vector
  bytes, 138 applied events with block-level application traces.
- **The thresholds:** RTL/model vector conformance is **sha256-exact**
  (claim 1 — no numeric tolerance exists or is tolerated); the
  model-vs-oracle challenge threshold is **byte-identical f32le
  (max_abs_error 0.0)** on every wrapper-valid row (claim 2, evaluated
  once at the frozen challenge run). The manifest carries
  `thresholds_frozen_before_challenge: true`.

## The challenge protocol (ordering, explicitly)

The plan (section 7) and issue #24 forbid tuning thresholds after
inspecting challenge failures. The ordering was:

1. The challenge set and thresholds were **preregistered** in
   `spec/release-vectors-v1.json` before any challenge output existed;
   the builder refuses to run unless
   `thresholds_frozen_before_challenge` is true and the binding hashes
   match the tree.
2. The challenge run (`tools/n08_challenge.py`) **consumed** the frozen
   thresholds from the bundle manifest (and refuses anything but the
   frozen byte-identity rule), then recorded per-case verdicts at those
   thresholds.
3. **This commit adds the thresholds and the challenge results together**;
   any later threshold change alters the preregistration file, breaks the
   manifest's spec binding and the bundle hash, and is thereby detectable.
   The results record the protocol statement verbatim.

**Challenge outcome at the frozen thresholds: 21/21 AGREE_EXACT** —
byte-identical to the pinned oracle on every preregistered challenge case,
including `ampmod-ams3` (the amp-mod sensitivity path with a non-inert
LFO, flagged NO_VERDICT at N06) and `osc-keysync0`. **Zero bounded
findings** arose from the challenge. Had any row failed, it would have been
recorded here as a bounded finding — no retuning, no case swap.

## STALE rules (invalidation)

Any change to a pinned model module, to `spec/numeric-profile-v1.json`, or
to the preregistration `spec/release-vectors-v1.json` **invalidates the
bundle**: `tools/n08_verify_release.py` fails (model-identity / profile /
spec-binding checks), and **every H-series conformance status derived from
this release becomes STALE** until a full re-freeze is performed. A
re-freeze that changes thresholds, the case selection, or the model is a
**new decision record + full re-freeze**, never an in-place edit (issue #24
stop/escalate). Rebuilding the bundle without any such change is safe and
must be byte-identical (determinism: no timestamps; the only provenance
field, `built_at_commit`, is excluded from the bundle hash by design).

## Authorization statement

This release **authorizes exact RTL work (H03+)**: RTL conformance now has
a stable, tamper-evident oracle — the committed vectors and the frozen
model they were rendered from. The RTL-vs-frozen-model claim must be
**exact** (sha256-identical PCM after declared scheduling/latency); no
numeric tolerance exists for it. This authorization is for claim 1 only
and establishes nothing about original-DX7 fidelity, synthesis, physical
fit, or audibility (DEC-001, DEC-024, plan section 7); Dexed agreement is
not original-DX7 fidelity.

## Honest limits of this freeze

- **dev32-03/06/30 are archive-gated** for re-rendering (voice bytes
  resolve from the pinned DX7_AllTheWeb archive, which is never
  redistributed). Their committed vectors are environment-independent;
  re-rendering those rows in a fresh environment is guarded NOT_RUN, never
  a silent pass. The other 48 cases re-render anywhere.
- **The challenge set is single-note.** The registry offers no fresh
  synthetic polyphonic case beyond the dev set (the 17th-note case is the
  documented DEC-017-vs-wrapper divergence, the corpus material is
  archive-gated). Polyphonic semantics are covered by the dev set plus the
  N07 evidence, not by an independent challenge — a recorded gap, not a
  hidden one.
- **Inherited unproved:** model-vs-reference agreement on keysync-0
  LFO-active renders (N06 finding F-4, C++-indeterminate reference value);
  `lfo-keysync0` is excluded from the bundle for exactly that reason.
- The freeze makes **no listening-based claim**; preset quality and musical
  usefulness require listening records (plan section 7).
