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

## Reuse vs build new

The DX7 DSP (six operators, DX envelopes, 32 algorithms, feedback) is built
new — no sibling implements it. What is reused from the Apache-2.0 sibling
repositories, each adoption with a provenance record and requalification:

- **gf180-parasynth / gf180-monosynth** (same design): SPI control link
  (`spi_ctl.v` + decision record 0007), I2S transmitter and pin-level
  decode harness, chip chassis (single-clock 256-cycle frame, tick/go,
  overrun), the N-channel shared-datapath precedent (`ladder_dp_n.v`,
  bit-exact at NCH=1/2/4/8), the working gf180mcu ORFS flow, ULX3S FPGA
  flow, and measured gf180mcu area calibrations.
- **gf180-torchsynth**: the pinning stack (commit-archive + SHA-256
  manifest + drift-rejecting verifier + runtime lock), non-perturbing
  trace capture, no-alignment paired comparator, case-registry/scorecard
  pattern, generic fixed-point primitives, and reuse-governance template
  (decision record 0005).
- **External GPL engines** (Dexed Mark I, VDX7, Hexter) are comparison
  oracles only — never copied into this Apache-2.0 repository.

## License

Apache License 2.0 (repo-level, per 2AMLogic convention). See
[LICENSE](LICENSE).

<!-- ISSUEDAG:BEGIN -->
## Backlog progress (issue DAG)

Generated from [spec/issue-dag-v1.json](spec/issue-dag-v1.json) by `tools/render_issue_dag.py` — do not hand-edit.

| CLOSED | OPEN | UNKNOWN | TOTAL |
| ---: | ---: | ---: | ---: |
| 14 | 22 | 0 | 36 |

These are issue states, not verification verdicts: a closed issue is not a capability claim, and no numeric PASS/coverage status is shown here. Evidence-derived capability status arrives with the fixed model and comparator (plan section 7).

```mermaid
graph TD
  E1["E1 epic (issue 1)"]:::epic
  E2["E2 epic (issue 2)"]:::epic
  E3["E3 epic (issue 3)"]:::epic
  A01["A01 (issue 39) CLOSED"]:::closed
  D00["D00 (issue 4) CLOSED"]:::closed
  D01["D01 (issue 5) CLOSED"]:::closed
  H01["H01 (issue 20) OPEN"]:::open
  H02["H02 (issue 21) OPEN"]:::open
  H03["H03 (issue 25) OPEN"]:::open
  H04["H04 (issue 26) OPEN"]:::open
  H05["H05 (issue 27) OPEN"]:::open
  H06["H06 (issue 28) OPEN"]:::open
  H07["H07 (issue 29) OPEN"]:::open
  H08["H08 (issue 30) OPEN"]:::open
  H09["H09 (issue 31) OPEN"]:::open
  H10["H10 (issue 32) OPEN"]:::open
  H11["H11 (issue 33) OPEN"]:::open
  N01["N01 (issue 15) CLOSED"]:::closed
  N02["N02 (issue 16) OPEN"]:::open
  N03["N03 (issue 17) OPEN"]:::open
  N04["N04 (issue 18) OPEN"]:::open
  N05["N05 (issue 19) OPEN"]:::open
  N06["N06 (issue 22) OPEN"]:::open
  N07["N07 (issue 23) OPEN"]:::open
  N08["N08 (issue 24) OPEN"]:::open
  P01["P01 (issue 8) CLOSED"]:::closed
  P02["P02 (issue 9) CLOSED"]:::closed
  R01["R01 (issue 6) CLOSED"]:::closed
  R02["R02 (issue 7) CLOSED"]:::closed
  R03["R03 (issue 10) CLOSED"]:::closed
  R04["R04 (issue 11) CLOSED"]:::closed
  R05["R05 (issue 12) CLOSED"]:::closed
  R06["R06 (issue 13) CLOSED"]:::closed
  R07["R07 (issue 14) CLOSED"]:::closed
  U01["U01 (issue 34) CLOSED"]:::closed
  U02["U02 (issue 35) OPEN"]:::open
  U03["U03 (issue 36) OPEN"]:::open
  U04["U04 (issue 37) OPEN"]:::open
  U05["U05 (issue 38) OPEN"]:::open
  E1 -.-> A01
  E1 -.-> D00
  E1 -.-> D01
  E2 -.-> H01
  N02 --> H01
  E2 -.-> H02
  N01 --> H02
  D01 --> H02
  E2 -.-> H03
  N08 --> H03
  H01 --> H03
  H02 --> H03
  E2 -.-> H04
  H03 --> H04
  E2 -.-> H05
  H03 --> H05
  E2 -.-> H06
  H03 --> H06
  E2 -.-> H07
  H04 --> H07
  H05 --> H07
  H06 --> H07
  H01 --> H07
  E2 -.-> H08
  H07 --> H08
  E2 -.-> H09
  H08 --> H09
  D01 --> H09
  E2 -.-> H10
  H07 --> H10
  D01 --> H10
  E2 -.-> H11
  H10 --> H11
  E2 -.-> N01
  R04 --> N01
  R05 --> N01
  D01 --> N01
  E2 -.-> N02
  N01 --> N02
  E2 -.-> N03
  N01 --> N03
  E2 -.-> N04
  N02 --> N04
  E2 -.-> N05
  N01 --> N05
  E2 -.-> N06
  N03 --> N06
  N04 --> N06
  N05 --> N06
  E2 -.-> N07
  N06 --> N07
  E2 -.-> N08
  N07 --> N08
  R06 --> N08
  E1 -.-> P01
  D00 --> P01
  E1 -.-> P02
  P01 --> P02
  E1 -.-> R01
  D00 --> R01
  E1 -.-> R02
  R01 --> R02
  E1 -.-> R03
  P02 --> R03
  R02 --> R03
  E1 -.-> R04
  R02 --> R04
  E1 -.-> R05
  R02 --> R05
  E1 -.-> R06
  P01 --> R06
  R05 --> R06
  E1 -.-> R07
  R03 --> R07
  R04 --> R07
  E3 -.-> U01
  R02 --> U01
  P02 --> U01
  E3 -.-> U02
  U01 --> U02
  R03 --> U02
  E3 -.-> U03
  U02 --> U03
  E3 -.-> U04
  P01 --> U04
  H03 --> U04
  E3 -.-> U05
  U04 --> U05
  H09 --> U05
  U02 --> U05
  classDef epic fill:purple,color:white;
  classDef closed fill:green,color:white;
  classDef open fill:darkgray,color:black;
  classDef unknown fill:orange,color:black;
```

<!-- ISSUEDAG:END -->

<!-- CAPABILITIES:BEGIN -->
## Capability status (evidence-derived)

Generated from [spec/capabilities-v1.json](spec/capabilities-v1.json) by `tools/compile_capabilities.py` — do not hand-edit. Evidence-derived node states; issue closure, file existence, and prose never establish a capability claim. Per-node claims, coverage, and controls: [docs/CAPABILITIES.md](docs/CAPABILITIES.md).

| PASS | FAIL | NOT_RUN | BLOCKED | NO_VERDICT | STALE |
| ---: | ---: | ---: | ---: | ---: | ---: |
| 7 | 0 | 0 | 6 | 2 | 0 |

```mermaid
graph TD
  n0["comparator: PASS"]
  n1["contract: PASS"]
  n2["corpus-dev32: PASS"]
  n3["fixed-model: BLOCKED"]
  n4["fpga: BLOCKED"]
  n5["gf180: BLOCKED"]
  n6["instrument: BLOCKED"]
  n7["numeric-profile: NO_VERDICT"]
  n8["oracle-identity: PASS"]
  n9["reference-pin: PASS"]
  n10["registry: NO_VERDICT"]
  n11["renderer: PASS"]
  n12["rtl-core: BLOCKED"]
  n13["rtl-exact: BLOCKED"]
  n14["traces: PASS"]
  n11 --> n0
  n0 --> n2
  n0 --> n3
  n7 --> n3
  n14 --> n3
  n13 --> n4
  n13 --> n5
  n2 --> n6
  n10 --> n6
  n0 --> n7
  n1 --> n7
  n9 --> n8
  n1 --> n9
  n14 --> n10
  n8 --> n11
  n3 --> n12
  n12 --> n13
  n11 --> n14
```

<!-- CAPABILITIES:END -->
