# gf180-dx7 — agent instructions

Open-source canary for a DX7-compatible six-operator, 32-algorithm FM
synthesis engine, targeting gf180mcu. Proposed first product: 16-note
polyphony, one timbre, 48 kHz, external host/controller/DAC/storage.

- The architecture, reference stack, verification contract, and issue briefs
  are defined in `docs/dx7-chip-plan-v0.1-2026-09-20.md`. Issue bodies are
  normative where they tighten the plan.
- The initial executable comparison target is a pinned Dexed Mark I
  configuration. Pin the exact commit, engine mode, sample rate, and runtime
  in the reference manifest (R01) before rendering; do not silently follow
  upstream Dexed. Dexed agreement is not original-DX7 fidelity.
- Keep three claims separate and never infer one from another: (1) the RTL
  matches the frozen integer model exactly; (2) the model reproduces the
  pinned software reference within declared budgets; (3) the instrument
  sounds good. Numeric tests never establish musical usefulness; listening
  records do.
- Comparison renders are dry. Do not normalize each render independently,
  time-warp comparisons, or switch reference engines per patch.
- Licensing: Dexed is GPL-3.0-or-later in `Source/EngineMkI.cpp` while its
  `msfa/` subset is Apache-2.0; VDX7 is GPL-3.0-or-later; Hexter is
  GPL-2.0-or-later. Record file/table-level provenance before adopting any
  third-party code or table, and choose an explicit compatible source policy
  rather than inheriting one through a port.
- Do not claim FPGA or gf180mcu synthesis, place-and-route, signoff, or
  hardware playback, original-hardware fidelity, or preset quality without a
  committed evidence record that establishes exactly that claim. Source
  presence, test counts, closed issues, and generated report files establish
  nothing by themselves.
- Distinguish reference-vs-reference repeatability (exact under a defined
  environment), RTL-vs-frozen-model (must be exact), and model-vs-reference
  (declared error budgets). Hardware audio captures need their own
  alignment/calibration procedure.
- Report verification statuses as PASS, FAIL, NOT_RUN, BLOCKED, NO_VERDICT,
  or STALE, and report coverage separately from agreement. A test that did
  not run must never be reported as a pass.
- Every coding issue names one outcome, its prerequisites, exact inputs, an
  acceptance check, a relevant failure control, evidence to retain, and a
  stop/escalate condition (plan section 10). PRs state what behavior changed,
  which acceptance case moved, how it was checked, and what remains unproved.
- Keep live negative controls (wrong operator order, broken feedback
  history, ADSR substituted for a DX envelope, silent/stale stubs); a
  control must demonstrably fail the check it targets.
- Do not weaken a product goal (e.g. 16-note polyphony) or acceptance rule
  to make a task pass; record a bounded finding and block only the affected
  dependency.
- Keep `AGENTS.md` and `CLAUDE.md` substantively identical outside their
  Loom-managed marker blocks.

<!-- BEGIN LOOM ORCHESTRATION (AGENTS) -->
This repository uses [Loom](https://github.com/rjwalters/loom) for AI-powered development orchestration (dual-runtime: Claude Code reads `CLAUDE.md`; OpenAI Codex CLI and other AGENTS.md-aware runtimes read this file). See the Loom repository for the full guide (roles, labels, worktrees, configuration). When installed, Loom also writes a locally-substituted copy of the runtime-neutral guide to `.loom/AGENTS.md`.
<!-- END LOOM ORCHESTRATION (AGENTS) -->
