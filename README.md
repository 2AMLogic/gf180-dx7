# gf180-dx7

A hardware canary for a DX7-compatible six-operator FM synthesis engine,
targeting the GlobalFoundries **gf180mcu** open PDK.

Proposed first product: a 16-note, six-operator, 32-algorithm DX7-compatible
engine that plays imported DX7 patches continuously, responds correctly to
velocity and performance controls, recalls a curated sound bank, and produces
verified digital audio. The RTL must match a frozen integer model exactly.

The initial executable comparison target is a pinned **Dexed Mark I**
configuration with documented differences. The RTL-vs-model comparison must
be exact; model-vs-reference comparisons use declared error budgets.

## Status

Planning. The executable backlog (35 dependency-linked tasks in three epics)
is tracked in GitHub issues. Architecture, reference stack, sound-library
strategy, verification contract, and starter briefs are defined in
[docs/dx7-chip-plan-v0.1-2026-09-20.md](docs/dx7-chip-plan-v0.1-2026-09-20.md).

Epics:

- **E1 — Reference and playable software** (#1): pinned reference renderer, SysEx
  codec, catalog, comparator, traces, compatibility registry.
- **E2 — Verified core and physical feasibility** (#2): integer models, operator
  and storage synthesis probes, fixed-model release, exact RTL, pin-level
  integration, FPGA capture, gf180 mapping.
- **E3 — Curated instrument** (#3): audition/recall tool, reviewed 32-patch bank,
  curated 128-patch bank, embedded host, end-to-end demonstration.

## Ground rules

Three judgments are kept separate throughout:

1. the RTL matches the model;
2. the model reproduces the chosen reference;
3. the instrument sounds good.

Passing one never establishes the others.
