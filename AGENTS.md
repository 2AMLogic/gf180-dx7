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
- Licensing/source policy (org convention, repo-level Apache-2.0, no per-file
  headers): GPL-3.0-or-later code (Dexed `Source/EngineMkI.cpp`, VDX7) and
  GPL-2.0-or-later code (Hexter) must never be copied into this repository;
  those engines are external comparison oracles only. Apache-2.0 material
  (Dexed `msfa/`, any 2AMLogic sibling component) may be adopted with a
  provenance record (upstream commit, path, hashes, license, destination,
  adaptations) plus requalification; hash equality alone never qualifies.
  Dexed agreement is not original-DX7 fidelity.
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
- Test tiering ([DR-0009](docs/decision-records/0009-test-tiering-fast-lane-budget.md)):
  `make test-fast` (seconds) is the default gate for builder/merge loops when
  Verilator is not on `PATH` (every CI runner today); with Verilator present
  (an opted-in operator/dev host) budget tens of seconds — DR-0009 measures
  and explains the gap. The full suite (`make test`, ~15 min,
  renders+synthesis) runs at judge approval and wave gates. Never report a
  skipped or fast-lane result as a full-suite pass.
- Keep `AGENTS.md` and `CLAUDE.md` substantively identical outside their
  Loom-managed marker blocks.

<!-- BEGIN LOOM ORCHESTRATION (AGENTS) -->
This repository uses [Loom](https://github.com/rjwalters/loom) for AI-powered development orchestration (dual-runtime: Claude Code reads `CLAUDE.md`; OpenAI Codex CLI and other AGENTS.md-aware runtimes read this file). See the Loom repository for the full guide (roles, labels, worktrees, configuration). When installed, Loom also writes a locally-substituted copy of the runtime-neutral guide to `.loom/AGENTS.md`.
<!-- END LOOM ORCHESTRATION (AGENTS) -->

<!-- BEGIN REPO-SKILLS -->
This repository has [Repo Skills](https://github.com/rjwalters/repo) v0.11.18 installed —
general repository hygiene and environment commands invoked as `/repo:<command>`. Run
`/repo:help` for the command list, or see `.claude/skills/repo/SKILL.md` for the full
guide. Hygiene commands apply safe, reversible fixes by default and report each
change; run with `--ask` to review first, and `--prune` to allow irreversible
removals. Managed by `install.sh` — edit outside the markers only.
<!-- END REPO-SKILLS -->
