# Work Log

Chronological record of merged pull requests and closed issues. Newest entries appear first.

### 2026-10-08

- **Issue #116** (closed): H08: regenerate evidence/h08-chassis/synth_report.json with the #94 hierarchy-total parser (heavy host)

### 2026-10-05

- **PR #125**: refactor: share H07/H08 hierarchy-total Yosys stat parser
- **PR #121**: Deduplicate single-module Yosys stat guards in H01/H04/H05/H06 (#119)
- **Issue #124** (closed): Consolidate H07 and H08 hierarchy-total stat parsers
- **Issue #119** (closed): Deduplicate single-module Yosys stat guards in H01/H04/H05/H06

### 2026-10-04

- **PR #120**: docs(i2s): correct stale I2S timing comments to measured geometry (#118)
- **Issue #118** (closed): Stale I2S timing comments in tb_synth_top.v and dx7_core.v (clk/4, clk/256, '5 clk')

### 2026-10-03

- **PR #117**: docs: amend I2S TX front-end contract to as-built 8/512 22-bit left-justified (#78)
- **PR #115**: evidence(h07): regenerate synth_report.json on the heavy host (#93)
- **Issue #78** (closed): H07/I2S: TX slot geometry deviates from the frozen front-end contract (decision: core fix vs contract amendment)
- **Issue #93** (closed): H07: regenerate synth_report.json on the heavy host with the #82 hierarchy-total parser

### 2026-10-02

- **Issue #91** (closed): H08 (and sibling synth tools) repeat the #82 parse_stat hierarchy bug: chassis report records alg_router's area as the total

### 2026-09-30

- **PR #105**: #98: DR-0012 exp() term-width refreeze — synthesis keeps the exp() unit (cone LEC PASS, 34/34 battery, mapped census BLOCKED)
- **Issue #98** (closed): Synthesis deletes the exp() unit: out-of-range exp_t3/4/5 reads make exp_hsum all-x, mapped core ignores LFO AM (escalation from #86)

### 2026-09-29

- **PR #114**: chore: resync installed Loom surfaces
- **PR #113**: docs(dr): DR-0013 fidelity first, area second (#85)
- **Issue #85** (closed): Owner decision: H10 mapped area is 14x the quarter slot. Choose the architecture/requirement direction for the 16-note core

### 2026-09-28

- **PR #112**: ci: move short and scheduled jobs off Blacksmith to GitHub-hosted

### 2026-09-27

- **PR #110**: gitignore: ignore .claude/skills/repo/logs/ so guard-hook logs don't block fleet resync
- **PR #107**: chore: drop unused numpy metrics extra from pyproject.toml
- **Issue #108** (closed): gitignore: add .claude/skills/repo/logs/ so guard-hook logs don't block fleet resync (2am#1184)
- **Issue #106** (closed): Remove unused numpy metrics extra from pyproject.toml

### 2026-09-25

- **PR #104**: test: prove the iverilog LFO-AMS x reaches the audio path (issue #96)
- **PR #102**: h01/h04/h05/h06/storage_probe synth: assert single-module stat shape before reporting a per-module area
- **PR #100**: fix: H08 synth parse_stat reads design-hierarchy totals, not alg_router's block (#94)
- **PR #99**: #86: exp_t3/4/5 out-of-range selects: synthesis deletes the exp() unit (FAIL; escalated as #98)
- **PR #97**: ci: add test-fast job as DR-0009 fast-lane backstop
- **PR #92**: H07: parse_stat reads the design-hierarchy totals, not alg_router's block (#82)
- **PR #90**: test: restate DR-0009 fast-lane budget with a measured Linux profile
- **PR #84**: feat(h10): gf180 mapped feasibility report (PR-B): 16-note core does not fit either fixed die; escalated
- **PR #83**: U03 scaffold: bank128 manifest schema + validator (part of #36)
- **Issue #96** (closed): iverilog treats exp_t3/t4/t5 out-of-range reads as X, not 0 — mapped-netlist/simulated-core divergence risk for LFO-AMS != 0
- **Issue #95** (closed): Synth tools h01/h04/h05/h06/storage_probe: add explicit single-module assertions (confirmed correct today, part of #91)
- **Issue #94** (closed): H08 synth tool: fix hierarchy-total parsing bug (confirmed multi-module, part of #91)
- **Issue #86** (closed): exp_t3/4/5 out-of-range selects: prove mapped (undef->0) matches the simulated core
- **Issue #89** (closed): CI's test job runs the full unfiltered suite, not tools/test_fast.sh — DR-0009 tiering norm is undocumented-as-unenforced in CI
- **Issue #82** (closed): H07 synth_report.json records alg_router's area/cells as the core total (parse_stat hierarchy bug)
- **Issue #87** (closed): Fast lane (make test-fast) takes ~9 min wall on the operator host vs DR-0009's seconds budget

### 2026-09-24

- **PR #70**: Adopt Renovate dependency security policy (14-day quarantine)
- **PR #80**: H10 (PR-A): single-driver core refreeze (DR-0011) — frozen core re-pinned, synthesis-path legality evidenced
- **PR #71**: .github/workflows: Migrate workflows to Blacksmith runners
- **PR #79**: U05: software-side demo prep — fixture, 3-way cross-check bench, stress with throttled-underrun negative, manifest, NOT_RUN hardware record (#38)
- **Issue #38** (closed): U05: Complete instrument demonstration

### 2026-09-23

- **PR #77**: H08: SPI/I2S pin-level integration — 9-pad chassis, pin==flat byte-exact on all 11 vectors, injection suite 16/16
- **PR #76**: H07 recert: fresh 34/34 evidence bound to the merged tip
- **PR #75**: H07: fresh re-verification evidence + session findings (follow-up to #73)
- **PR #73**: H07: integrated polyphonic PCM core
- **PR #72**: U02 prep: listening kit
- **Issue #1** (closed): Epic: E1 — Reference and playable software
- **Issue #74** (closed): U07: fast-lane test_audition skip probes ERROR instead of guarded-skip under macOS TCC denial
- **Issue #30** (closed): H08: SPI/I2S pin-level integration
- **Issue #29** (closed): H07: Integrated polyphonic PCM core

### 2026-09-22

- **PR #69**: U04: embedded host against mock core
- **Issue #37** (closed): U04: Embedded host against a mock core

### 2026-09-21

- **PR #68**: H06: pitch/modulation RTL
- **PR #67**: H05: routing/feedback RTL
- **PR #66**: H04: envelope/state RTL
- **PR #65**: H03: core interface/schedule contract
- **PR #64**: N08: frozen fixed-model release
- **PR #63**: N07: polyphonic state/event manager
- **PR #62**: N06: integrated single-note fixed model
- **PR #61**: N04: algorithm/feedback model
- **Issue #28** (closed): H06: Pitch/modulation RTL or host binding
- **Issue #27** (closed): H05: Routing/feedback RTL
- **Issue #26** (closed): H04: Envelope/state RTL
- **Issue #25** (closed): H03: Core interface/schedule contract
- **Issue #24** (closed): N08: Frozen fixed-model release and vectors
- **Issue #23** (closed): N07: Polyphonic state/event manager
- **Issue #22** (closed): N06: Integrated single-note fixed model
- **Issue #18** (closed): N04: Integer algorithm/feedback model

### 2026-09-20

- **PR #60**: H01: synthesizable operator probe
- **PR #58**: N03: integer envelope model
- **PR #59**: H02: full-state storage feasibility probe
- **PR #57**: N05: integer pitch/modulation model
- **PR #56**: N02: integer operator model
- **PR #54**: C01: capability DAG and status views
- **PR #55**: R07: oracle-disagreement report
- **PR #53**: R06: directed compatibility registry
- **PR #52**: N01: numeric/scheduling decision record
- **PR #51**: U01: software audition/recall tool
- **PR #50**: R03: first 32-patch development manifest
- **PR #49**: R04: comparator and apparatus qualification
- **PR #48**: R05: non-invasive reference trace adapter
- **PR #47**: R02: headless reference renderer
- **PR #46**: R01: pinned reference manifest
- **PR #44**: P02: archive cataloger
- **PR #43**: P01: DX7 SysEx codec
- **PR #42**: A01: reuse audit catalog, rulings, and negative-control check (closes #39)
- **PR #41**: D01: physical constraint inventory
- **PR #40**: D00: product/fidelity contract v1
- **Issue #20** (closed): H01: Synthesizable operator probe
- **Issue #17** (closed): N03: Integer operator-envelope model
- **Issue #21** (closed): H02: Full-state storage feasibility probe
- **Issue #19** (closed): N05: Integer pitch and modulation model
- **Issue #16** (closed): N02: Integer phase/operator model
- **Issue #45** (closed): C01: Evidence-derived capability DAG and status views (torchsynth-pattern, with inherited fixes)
- **Issue #14** (closed): R07: Targeted oracle-disagreement report
- **Issue #13** (closed): R06: Complete directed compatibility registry
- **Issue #15** (closed): N01: Numeric/scheduling decision record
- **Issue #34** (closed): U01: Software audition/recall tool
- **Issue #10** (closed): R03: First 32-patch development manifest
- **Issue #11** (closed): R04: Comparator and apparatus qualification
- **Issue #12** (closed): R05: Non-invasive reference trace adapter
- **Issue #7** (closed): R02: Minimal headless reference renderer
- **Issue #6** (closed): R01: Pinned reference manifest
- **Issue #9** (closed): P02: Archive cataloger
- **Issue #8** (closed): P01: SysEx codec — lossless supported import/export
- **Issue #39** (closed): A01: Sibling infrastructure reuse audit (pin, qualify, record provenance before import)
- **Issue #5** (closed): D01: Physical constraint inventory
- **Issue #4** (closed): D00: Product and fidelity profile decision record
