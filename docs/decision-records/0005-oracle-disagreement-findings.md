# DR-0005: Oracle-disagreement findings — second oracle is a localization tool, not an interchangeable reference

- **DR number:** 0005
- **Status:** Proposed
- **Date:** 2026-09-20
- **Deciders:** loom-builder (agent), pending owner review
- **Related issues:** #14 (R07)

## Context

R07 compared a six-case diagnostic set rendered through two independent
software DX7 implementations: the project reference (pinned Dexed Mark I
oracle, `reference/oracle-identity.json`) and a second oracle (pinned
hexter 1.1.1, `reference/second-oracle-identity.json`,
`theabolton/hexter@1cf1bfea5962f7c9726e0cf809b762b3b2655225`,
GPL-2.0-or-later, external-oracle-only). Cross-engine disagreement was
signal-scale in every case (`reference/evidence/r07/`), and each case was
localized to a named seam with a cited, labeled hypothesis
(`docs/R07-DISAGREEMENT.md`):

- F1 feedback: coefficient mapping + history depth differ (Dexed
  two-sample-averaged history, shift = 8 − feedback; hexter one-sample
  history, feedback/(2π)·0.18, missing per-algorithm scaling);
  topology-sensitive level/spectrum differences.
- F2 envelope: at rate = 1 the low-slew coefficient fits differ by a small
  constant factor (hexter ≈ 2–3× level during ultra-slow attack).
- F3 detune: detune-to-frequency conversion differs at range extremes
  (different beat rate on raw 0 vs 14 carriers).
- F4 fixed-frequency: near the top of the fixed range the engines converge
  to slightly different Hz (energy-matched, waveform de-cohered).
- F5 corpus anchor: a real dev32 voice tracks in envelope shape (≤ 5 %
  window RMS) but differs in waveform phase/spectral structure (ZCR −35 %
  relative).

No hardware evidence exists in this repository; therefore nothing above
adjudicates physical correctness, and none of the findings rises to
invalidating a D00 decision (all are implementation-convention/tuning
differences between software engines, per the issue's stop/escalate bar).

## Decision

1. The second oracle (hexter-oracle) is approved for LOCALIZATION and
   directional sanity use only. It must never be used as an exactness
   reference, never averaged with the reference engine, and never
   substituted per patch; the project reference remains the R01/R02-pinned
   Dexed Mark I oracle.
2. N-series model-vs-reference error budgets must be declared PER SEAM
   (at minimum: feedback mapping, envelope coefficient fit at rate
   extremes, detune curve extremes, fixed-mode frequency conversion),
   quoting F1–F4 as the observed cross-engine spread, not a global
   average.
3. Any claim that one engine is "wrong" or "correct" is barred without
   third-source or original-hardware evidence (AGENTS.md; issue #14
   negative control). Reports are required to label explanations as
   hypotheses; `tools/r07_compare.py validate_case_report()` enforces
   this mechanically.

## Consequences

- Enables: targeted N-series tolerance rows; a reusable pinned second
  oracle + driver (`tools/r07_compare.py`) for future seam probes.
- Forbids: treating hexter agreement/disagreement as a regression gate;
  second-oracle PCM as reference evidence; averaging engines.
- STALE when: either identity file changes (re-run the driver; regenerate
  `reference/evidence/r07/` and this record's numbers), or when hardware
  evidence arrives (then re-adjudicate F1–F5 explicitly).

## Evidence

- `docs/R07-DISAGREEMENT.md` — per-case table, hypotheses, citations.
- `reference/evidence/r07/` — inputs, comparison JSONs (schema
  `r07-disagreement-case/1`), render hashes, run logs, summary.
- `reference/second-oracle-identity.json` — pinned hexter-oracle build
  (source commit, binary sha256, engine configuration, determinism notes).
- Cited source seams: dexed `Source/msfa/dx7note.cc:196-197`,
  `dx7note.cc:28`, `fm_core.cc:114-118`, `fm_op_kernel.cc compute_fb`
  (pinned R01 commit `b6cf2e9190e216387ad96f977284ebbef765b447`); hexter
  `src/dx7_voice.c:1390`, `src/dx7_voice_render.c:128-131` (pinned
  `1cf1bfea5962f7c9726e0cf809b762b3b2655225`).
