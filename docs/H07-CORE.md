# H07 — Integrated 16-note polyphonic PCM core (issue #29)

Issue: #29 (epic #2) · Date: 2026-09-22 · Planning ID: H07
Depends on: H03 (contract), N08 (frozen release), H01/H04/H05/H06 (slices)
RTL: `rtl/dx7_core.v` · Bench: `rtl/tb_dx7_core.v` · Conformance:
`tools/h07_compare.py` · Synthesis: `tools/h07_synth.py` · Tests:
`tests/test_h07.py`

**Claim advanced (met on the pre-synthesis RTL, evidence committed):** the
complete 16-note RTL core emits the full PCM stream BIT-EXACT against the
frozen N08 release vectors (30-case dev set) and the frozen integer model
(preregistered stress set) at the H03-defined latency, with OVERRUN and
OVERFLOW never set, and two clean simulations artifact-hash identical.
Mapped-area evidence exists against the H02 38,781-flop accounting with a
passing strip-observability control. This is a simulation + mapped-area
claim only — no P&R, timing, board, or original-DX7 fidelity claim (§6).

## 1. What exists on `loom/h07-core`

* `rtl/dx7_core.v` — the integrated core: SPI slave (48-bit frames,
  skid FIFO depth 8×49), continuous drain into patch shadow (SEC=1) and an
  ordered event queue (SEC=0, depth 168 = the 24.576 MHz per-frame write
  bound), atomic patch commit + event application at the frame tick
  (DEC-014/NUM-011), voice allocation (free slot → DEC-016 same-pitch
  reuse → DEC-017 release-then-oldest steal), DEC-015 retrigger, DEC-018
  sustain, CC 120/123, SOFT_RESET, 96 × `env_unit` (H04), a shared
  time-multiplexed per-note pitch path (pitch-EG, chip-global LFO, PM/AM
  composition, Freqlut), one shared H05 `alg_router` around a
  combinational H01 op kernel, the NUM-010 unclamped mixer (>>4 / clip
  ±2^24 with the +32768 quirk / >>9, per-voice clip then integer sum),
  double-buffered I2S master (BCLK = clk/4, LRCLK = clk/256, data delayed
  1 BCLK, left-justified), status word `{0xD7, 1, OVERRUN, QUEUE,
  OVERFLOW, FRESH, FRAME}` and two-flop reset sync.
* `rtl/tb_dx7_core.v` — host-side bench: SPI master (SCK = clk/16, inside
  the contracted SCK ≤ clk/4 bound), I2S DAC-model decoder + PCM dump via
  the core's mixer→I2S load tap, and the vector-record pacing described in
  §5 (patch/commit/event/soft-reset/run records with model-block arrival
  alignment).
* `tools/h07_compare.py` — the conformance harness: case resolution via
  `tools/n08_build_release.py` (`CaseSource`; synthetic from
  `spec/compatibility-registry-v1.json`, dev32-03/06/30 from the pinned
  archive, voice bytes frozen in `evidence/h07-core/cases.json`, each row
  hash-checked against the N08 manifest), the host-side frozen-integer
  frontend (per-op detune deltas and fine terms per dx7note.cc:46-53,
  frozen bend per the float-gated spot — the same functions as
  `h06_compare.py`), the §5 register encoding, vector emission, the
  bit-exact comparator with first-mismatch localization
  (sample/block/index), per-event latency recording, artifact hashes,
  exit 0/1/2, `--audit-exp`, and the bounded-slice mode used by the
  iverilog canonical shadow.
* `tools/h07_synth.py` — mapped-area report + strip-obs gate (yosys,
  dfflibmap + abc against the ciel gf180mcu 7t liberty).
* `tests/test_h07.py` — harness machinery, committed-evidence freshness,
  live negative controls, determinism, interface guards, doc anchors.
* Negative-control builds, each demonstrably failing (see §4):
  `H07_MUTATE_GLITCH` (one-sample mix glitch, frame 12 sample 17 — the
  comparator LOCALIZES it), `H07_MUTATE_SUM_ORDER` (sum-then-clip; the
  N08 `voice_sum_order_swap` mutation), `H07_MUTATE_TICK_SKEW` (stale
  event read pointer), `H07_STRIP_OBSERVABILITY` (synthesis flop gate).

## 2. As-built frame schedule

Measured on the bring-up smoke run at the 24.576 MHz design-point cadence
(512 clk/sample, 32,768 clk/frame): one note's full compute pass
(commit + event apply + envelope steps + LFO + per-note control tail ×6
ops + 64-sample router render + mix + readback + reclaim) completes in
~2,070 clocks — ~6.3 % of the frame. The worst-case utilization evidence
required by `docs/CONTRACT-CORE-v1.md` §2 is the OVERRUN sticky defect
detector: it reads 0 after every frame of every conformance vector —
including the 16-voice maximum-summation chord, the 167-write single-frame
burst, held notes across a mid-render patch commit, and soft-reset
mid-render (2,361,344 compared samples per full run, ×2 runs, plus the
33 of 34 cases that completed in the iverilog shadow — see the §4
correction). OVERRUN-never-sets is therefore demonstrated over the
complete vector set, not a sample.

## 3. Integration findings (each with its resolution)

1. **`pitch_mod.v` cannot be instanced 16×**: the frozen model's LFO is
   chip-global — `lfo.keydown()` fires on EVERY strike (sync restart +
   `delaystate_ = 0` always) and `select_patch` reconfigures it
   immediately — while pitch-EG state, basepitch and the pm/am depths are
   per-note and bound to the striking patch (DEC-019). Per-instance LFOs
   diverge from the shared one (keysync and delay-ramp trajectories under
   multi-strike renders). The core integrates the per-note pitch path as
   shared logic over the note banks. RESOLVED: the shared path is
   bit-exact across the whole vector set.
2. **Op eval seam** (H05 uncertainty 2): `alg_router` consumes `eval_y`
   combinationally in the eval-request cycle, so the H01 kernel is reused
   without its 3-stage registers (`h07_op_kernel`, same pinned ROM bytes,
   same arithmetic; 1 eval/clk retained). RESOLVED: all algorithm/
   feedback cases (alg4/alg6 ×fb 0/3/7, alg-05/16/31) bit-exact.
3. **env_unit rate_scaling width**: `scale_rate` returns up to (99·31)>>3
   = 382, but env_unit's `rs_q` field is 6 bits. The H07 note-on
   derivation compensates exactly: send rs' = min(rs, 63) and
   per-segment rates r'_i = min(r_i + rs, 99) − 63 when rs ≥ 64.
   RESOLVED: covered by the conformance suite (bd-coarse0/31,
   bd-detune0/14 and the full dev set bit-exact).
4. **Transpose ruling reconciliation**: the frozen model binds
   `NotePitch(transpose_mode="pinned_wrapper")` — transpose is ignored —
   and N08 is H07's conformance oracle, so the core applies NO transpose
   shift (`l' = midinote_to_logfreq(note)`); GLOB_A stores the transpose
   byte without applying it (§5). N08 `perf-transpose-12` matches for
   that reason. H06's documented-mode ruling is a unit-level contract;
   the divergence is a recorded owner reconciliation item, not a silent
   choice (H03 §6 change-control applies to any future flip). RESOLVED
   as-built (stored-not-applied), evidence bit-exact.
5. **I2S data width**: the task-pinned "16-bit left-justified" field
   cannot carry the frozen PCM — NUM-010 freezes the mix as the
   unclamped ≥20-bit integer sum, and required case ev-sum-15 peaks at
   +43181 (1.3176…) — so the as-built field is 24-bit left-justified
   carrying the exact mix. A 16-bit DAC slice is an H08 owner decision.
   **Never loosened: the conformance transport is the exact mix.**
   RESOLVED as-built; ev-sum-15 bit-exact end-to-end through the 24-bit
   I2S field.
6. **NUM-008 amp-mod exp** (dx7note.cc:288 float gate): the core computes
   `pt = int(exp(f32(x/2^18)·0.07 + 12.2))` with a fixed-point evaluator:
   exact U7.18 argument (domain: 0 ≤ x ≤ 2^24), exact float64 dyadic
   constants, a 64-entry 2^(i/64) table and a degree-5 polynomial
   (≤ ~3e-15 relative error). RESOLVED: the `--audit-exp` sweep
   (`evidence/h07-core/audit-exp.json`, 4,100 domain points, stride
   4093) reports minimum boundary distance 4.46e-4 at x = 4,923,879 and
   ZERO points within 1e-9 of an integer — two orders of magnitude of
   margin over the ~1e-6 evaluator error — and every conformance vector
   (which exercises the amp-mod path) is bit-exact.
7. **DEC-019 binding as-built**: per-note bound patch images (the model's
   actual mechanism) — no 4-entry ring, hence no eviction gap; H03's
   ring-bound conformance gap does not arise. RESOLVED as-built;
   stress-patchcommit (held note across a `patch <n>` commit, poly.py
   golden) bit-exact.
8. **LFO wave 4 must mirror the pinned wrapper, not the regenerated
   table** (found by the suite, fixed in b06696e): the first integration
   used the NUM-007 regenerated sine table for LFO wave 4, but the N08
   conformance oracle renders wave 4 as the CONSTANT 1<<23 — the pinned
   wrapper's `State.init` never calls `Sin::init()`, so msfa's `sintab`
   stays zero-initialized (integrated.py:109-130, finding F-2; the
   `PinnedWrapperLfo` mirror is the frozen model's actual behavior). The
   defect is ramp-gated: PM (`pmd = pmdepth·lfo_delay`) and AM
   (`amod_1 = (amdepth·lfo_delay)>>8 …`) are both zero until the LFO
   delay ramp crosses 2^31, so dev32-30 "SOFT TOUCH" (wave 4, pm depth
   16, am depth 10, delay 50) matched exactly for 235 blocks after the
   strike and diverged ±1–2 LSBs at the ramp onset (first mismatch
   golden block 235, then FM-feedback decorrelation), while dev32-06
   (wave 4, depths 0) passed vacuously. Diagnosis was by module
   isolation: a frame-by-frame diff of the RTL's `lfo_dstate`/`lfo_del_q`
   against the model's `Lfo.getdelay()` trace showed the LFO state
   sequence EXACT, localizing the defect to the wave-4 output value; the
   fix mirrors `PinnedWrapperLfo` (wave 4 → constant 2^23, phase still
   advancing) and removes the dead sine ROM. `rtl/pitch_mod.v` (H06
   unit-level contract, NUM-007 documented behavior) is intentionally
   untouched — same reconciliation shape as finding 4. RESOLVED; full
   34-case set re-run bit-exact on the fixed RTL.
9. **yosys front-end**: dx7_core.v uses two SystemVerilog size casts
   (`8'(...)` at the DEC-019 strike binding) that iverilog -g2012 and
   verilator accept but yosys' plain `read_verilog` rejects; the synth
   flow reads with `-sv`. RESOLVED (tools/h07_synth.py).

## 4. Verification status (issue #29 acceptance)

> **Core-pin note (2026-09-25, DR-0012, issue #98).** The rows below were
> measured on earlier core revisions; the frozen core is now the
> widened-`exp_t*` revision `f33cecbd…`
> (`docs/decision-records/0012-exp-term-width-core-refreeze.md`). The
> conformance battery was **re-run fresh on that pin** — `results-dr0012-p1`
> and `results-dr0012-p2` (34/34 PASS each, two clean runs artifact-hash
> identical, `determinism_run2` embedded in p2), and every per-case
> `actual_sha256` is byte-identical to the DR-0011-pin run, so no simulated
> value moved. Two rows are **not** re-established on that pin and say so:
> the mapped-area row (the committed `synth_report.json` was produced on a
> superseded revision, and yosys + the ciel 7t liberty live on
> `repo-remote-gf180-dx7`) and the iverilog shadow row
> (`results-iverilog-shadow.json` is on a superseded revision; Icarus
> throughput on the DR-0012 builder host made a re-run infeasible — see
> `evidence/issue-98-exp-refreeze/README.md` rows C and H, where the
> tool-independent Icarus statement on the current pin is recorded instead).

| Acceptance item | Status |
|---|---|
| Every output sample bit-exact vs the frozen model at the defined latency | **PASS** — 30/30 dev cases (frozen N08 f32 vectors, dyadic ×32768 mapping) + 4/4 stress cases (frozen integer model goldens), 2,361,344 samples/run, zero mismatches (`results-verilog-accept.json`) |
| Reset determinism (repeated reset runs identical) | **PASS** — stress-reset renders segment A, SOFT_RESET mid-render, re-commit, segment B; ×2 full-run artifacts byte-identical (`determinism_run2`) |
| Burst events, held notes, 16-voice maximum summation | **PASS** — stress-burst (8 on/off pairs in one frame window), stress-patchcommit (held note across commit 13), stress-sum16 (16-voice chord, unclamped sum) all bit-exact |
| Two clean simulations artifact-hash identical | **PASS** — every per-case artifact sha256 identical between `verilog-accept` and `verilog-accept-run2` (embedded in the results file; both committed) |
| Negative control: one-sample glitch / wrong voice-sum order fails + localizes | **PASS** — `H07_MUTATE_GLITCH` fails and localizes to the exact injected sample (tests/test_h07.py, live build); `H07_MUTATE_SUM_ORDER` fails on a clipping 4-voice vector; `H07_MUTATE_TICK_SKEW` fails on a 167-write burst; all three run in the committed suite |
| Bring-up smoke (compile + full frame schedule + voice allocation) | PASS (iverilog 13.0; superseded by the full suite above) |
| Mapped area vs H02's 38,781-flop accounting + strip-obs gate | **PASS** — 90,427 mapped flops (> 38,781), 1,063,163 cells, chip area 24,082,966.3 µm², sequential 5,756,655.2 µm²; strip-obs control maps 0 flops — gate fails as required. These are the **whole-core hierarchy totals** of the committed `yosys_full.log` (`=== design hierarchy ===`, closed by `Chip area for top module '\dx7_core'`), re-derived from that transcript by the corrected `tools/h07_synth.py` parser (#82). The committed `synth_report.json` still carries the pre-#82 parse — `alg_router`'s module-local area/cells and an across-blocks flop double count — and is **STALE** for those four numeric fields until it is regenerated on the heavy host (`repo-remote-gf180-dx7`); the gate verdict is unchanged either way (90,427 > 38,781). `tests/test_h07.py::TestStripObsControl::test_committed_synth_report_area_is_the_hierarchy_total` reports that regeneration as NOT_RUN rather than passing it. **Since DR-0012 (issue #98) the report is also on a superseded core pin** (`34f93d2d…`, not the frozen `f33cecbd…`), and that pin difference alone would understate the core: the DR-0011 core's mapped netlist is missing the whole exp() datapath (`docs/EXP-RANGE-PROOF-86.md`). `test_committed_synth_report_gates` now reports that drift as STALE / NOT_RUN on a host that cannot regenerate it, and FAILs on one that can |
| Latency statement per H03 §4.5 | **PASS** — 113 events: measured arrival→first-affected-SDATA distance is exactly (128 − p) samples = (64 − p) + 64, inside the contracted window [(64 − p), (64 − p) + 65] for every event; worst observed 128 samples (p = 0) |

Simulator tiers (klayout-tools #2223 convention): the sweeps and
determinism runs are Verilator 5.041 (remote oss-cad-suite); the final
acceptance shadow is iverilog 13.0, bounded to the first 24 golden blocks
per case (`results-iverilog-shadow.json`).

> **CORRECTION (issue #96, 2026-09-25) — this paragraph previously read
> "34/34 PASS — zero Verilator/iverilog divergence (every case bit-exact
> under both)". Both halves were wrong and the iverilog tier is STALE for
> coverage.** The committed file records 33 passes and one `CouldNotRun`
> (`dev32-30`), not 34/34. And the two corpus cases with a nonzero
> per-operator LFO amp-mod sensitivity (`dev32-06`, `dev32-30`) are **not**
> bit-exact under both: their committed iverilog dumps differ from the
> Verilator dumps on 113,735 / 115,073 of the 120,063 overlapping samples,
> because iverilog evaluates the out-of-range `exp_t3/t4/t5` reads
> (`dx7_core.v:953-961`) as x and that x reaches the mixer. It went
> unreported because `--frames 24` bounds the *compare window*, not the
> vector: every dev case's first event is at block 37 or later, so 30 of
> the 34 compared windows contain only golden silence, and iverilog's
> `$fwrite("%c", …)` dump renders an unknown sample as NUL — so an
> x-corrupted render compares equal there. Re-compared through this same
> harness over a 120-block window (the note-on is at block 75), both cases
> report a bit-exactness **FAIL** under iverilog 13.0 (stable) — 0 of
> 2,856 / 2,864 nonzero golden samples reproduced, first mismatch in block
> 75 — while the same window passes under Verilator and the AMS = 0
> sibling `dev32-03` passes under iverilog. What the iverilog tier does
> establish is bit-exactness over the three stress windows that contain
> audio and over `dev32-03`'s blocks 0..119 (all AMS = 0); it establishes
> nothing about the dev set's audio past block 120, and nothing good about
> the AM/exp() path. Full analysis, probe, and controls:
> [`AMS-XPROBE-96.md`](AMS-XPROBE-96.md). The Verilator acceptance
> evidence below is unaffected. The RTL width fix is tracked in #98.

Wall time on `repo-remote-gf180-dx7` (m5.2xlarge, 8 vCPU): full dev+stress
Verilator run 4,226 s (run 1) / 4,092 s (run 2); iverilog shadow
(24-frame slices) 18,071 s; yosys full+strip ~35 min. Local runs were
limited to the negative-control suite (~3 min) and single-case probes
([DR-0009](decision-records/0009-test-tiering-fast-lane-budget.md)).

## 5. Host-side register map, vector grammar, run protocol

### Register words (SEC=1 page; verified against the dx7_core.v decode)

- `0x10+k` OP_PITCH[k]: [7:0]=mode, [15:8]=coarse, [23:16]=fine, [31:24]=detune
- `0x18+k` OP_FTERM[k]: signed addend applied **only when fine != 0**
  (`dx7_core.v`: `dv_bpr = (dv_fine != 0) ? dv_bpr1 + fterm : dv_bpr1`);
  ratio-mode/fine-0 ops take 0. Fine terms per dx7note.cc:46-53 via
  h06_compare.py's functions.
- `0x20+k` ERATES[k]: [7:0]=R1, [15:8]=R2, [23:16]=R3, [31:24]=R4 (raw bytes, 7-bit)
- `0x28+k` ELEVELS[k]: L1 [6:0], L2 [14:8], L3 [22:16], L4 [30:24]
- `0x30+k` SCALE_A[k]: [7:0]=breakpoint, [10:8]=kvs, [12:11]=right curve,
  [14:13]=left curve, [22:15]=right depth, [30:23]=left depth
- `0x38+k` SCALE_B[k]: [7:0]=output level, [15:8]=rate scaling
- `0x40` GLOB_A: [2:0]=pm-sens idx (patch[143]&7), [15:8]=am depth,
  [23:16]=pm depth, [31:24]=transpose (stored, NOT applied — pinned-wrapper
  semantics, finding 4)
- `0x41` GLOB_B: [7:0]=lfo rate, [15:8]=lfo delay, [16]=lfo sync,
  [19:17]=lfo wave, [20]=osc key sync, [23:21]=feedback, [28:24]=algorithm
- `0x43` GLOB_C: per-op AMS, 2 bits each: ams_k at [2k+1:2k], k=0..5
- `0x44`/`0x45` PEG rates/levels (4×7-bit packed, LSB-first per index)
- `0x42` COMMIT; op k's envelope block is register index **5−k** (the
  model's envs[k] = operators[5−k] twist)
- events (SEC=0 queue): NOTEON D=[14:8]=vel,[6:0]=note; NOTEOFF
  D=[6:0]=note; CC/PB per the tb grammar; DD0-5 = per-note frozen detune
  deltas (NUM-008)

### Vector grammar (rtl/tb_dx7_core.v header)

`P <addr> <data>` patch write · `C [block]` commit (paced) ·
`E <block> <addr> <data>` event, delivered so it ARRIVES during wire
frame base+block and pops at that frame's closing tick · `S [block]`
soft reset · `R <frames>` / `W <samples>` run records. Golden block b of
a commit segment based at wire frame F lands at dump index
(F + b + 2)·64. Frozen N08 f32 goldens map to integers by the exact
dyadic relation c = f32·32768 (|c| < 2^21); the comparator rejects any
non-dyadic golden value.

### Committed evidence and run protocol

* `evidence/h07-core/results-dr0012-p1.json` / `results-dr0012-p2.json` —
  the two clean 34-case Verilator runs on the **current** frozen core
  (DR-0012, issue #98; p2 embeds p1's per-case hashes as
  `determinism_run2`). `tests/test_h07.py::TestConformanceEvidence` reads
  the newest of these and fails if its `rtl_sha256` is not the current
  three-file fingerprint.
* `evidence/h07-core/results-verilog-accept.json` — the same battery on the
  superseded DR-0011 pin (tag `verilog-accept`) with `determinism_run2`
  embedded, kept as the historical record;
  `results-verilog-accept-run2.json` — its second clean run;
  `results-iverilog-shadow.json` — the iverilog canonical shadow;
  `audit-exp.json` — the NUM-008 exp-boundary sweep; `synth_report.json`
  + `yosys_full.log` + `yosys_strip.log` — mapped area + strip control;
  `cases.json` — frozen dev-case voice bytes (manifest-hash-checked);
  `stress/` — byte-frozen model goldens + manifest; `runs/<tag>-<case>/`
  — per-case vector, PCM dump, meta.
* Reproduce remotely: `rsync` the worktree to `~/h07`, then
  `python3 tools/h07_compare.py --set both --tool verilator --tag <t>
  --jobs 6`, twice for determinism, then `--tool iverilog --frames 24`
  for the shadow, then `python3 tools/h07_synth.py --liberty <7t.lib>`.
* Freshness: the results files embed `rtl_sha256` of
  `rtl/{dx7_core,env_unit,alg_router}.v`; tests/test_h07.py fails on any
  drift.

## 6. What this does NOT say

No place-and-route, timing-closure, or fit claim (abc has no STA; clock
closure at 24.576 MHz is H10's measurement). No FPGA or gf180mcu
silicon/board claim. No original-DX7 fidelity claim and no
musical-usefulness claim: bit-exactness against the pinned Dexed-derived
frozen model is a software-agreement result (Dexed agreement is not
original-DX7 fidelity), and preset quality is a listening-record matter.
No synth/preset quality claim follows from any number above.

## 7. Session notes

### Remote runbook (AWS repo-remote, 2am#999 path)

- Box: `ssh repo-remote-gf180-dx7` (i-0c0a073ffb9db41d5, m5.2xlarge, 8 vCPU/30 GB;
  EBS survives idle-shutdown stops; restart via
  `.claude/skills/repo/scripts/repo-remote.sh up --yes --json` — IP may change).
- Toolchain: `export PATH=$HOME/oss-cad-suite/bin:$PATH` (Verilator 5.041,
  yosys, iverilog). Liberty:
  `~/pdk/gf180mcu_fd_sc_mcu7t5v0/gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`
  (sha256 330aa9439255…, byte-identical to the local ciel install).
- Sync: `rsync -az --delete --exclude .git <worktree>/ repo-remote-gf180-dx7:~/h07/`.
- Efficiency rules: heavy sim/synth ONLY remote; sweeps under Verilator, final
  acceptance shadow under iverilog; local runs limited to the negative-control
  suite + affected module
  ([DR-0009](decision-records/0009-test-tiering-fast-lane-budget.md)).

### Debugging trail (dev32-30, finding 8)

1. Remote acceptance run (pre-fix tree): 33/34; dev32-30 first mismatch
   dump index 19996 → golden block 310, sample 28 (an earlier pre-fix run
   had failed at block 78 before the detune-delta index fix landed).
2. No model event anywhere near block 310 (events at blocks 75/750/1125/
   1875) → not event-triggered; envelope state settled → per-frame pitch/
   mod path suspected; the 28-sample lag before the first visible diff
   pointed at a block-boundary frequency change surfacing later at a
   sin-table edge.
3. Minimal repro (same patch, strike at block 2): divergence at golden
   block 235, sample 9 — the LFO delay ramp onset frame (delay byte 50 →
   `delaystate_` crosses 2^31 at frame 233–235 post-strike).
4. Frame-by-frame RTL (`lfo_dstate`, `lfo_del_q` displayed in a throwaway
   instrumented copy) vs a monkey-patched `Lfo.getdelay()` model trace:
   state sequences EXACT → defect localized to the wave-4 output value →
   `PinnedWrapperLfo` reconciliation (finding 8), fix, full re-run.

The superseded pre-fix remote results (results-accept1/sweep1/detfix of
the earlier session, pre-detune-fix tree) were consumed by the tree
re-sync; their findings are recorded in the b06696e/af780eb commit
messages and finding 8 above. The committed acceptance evidence is the
fixed-RTL run pair + shadow only.
### 2026-09-23 re-verification session (pi-h07; PR #73 tip d88ccae)

* Triage of the two uncommitted leftovers from the prior session:
  (a) the work tree held a partial ~265-line "AWS pivot" rewrite of
  `tools/h07_compare.py` replacing the committed 1035-line harness
  (broken/incomplete: guessed `f32~2^15` amplitude slice instead of the
  exact dyadic x32768; latency pinned from data in 64-sample steps instead
  of the H03 section 4.5 window; no CC/PB/soft-reset, stress,
  OVERRUN/OVERFLOW, or artifact-hash support) — discarded, committed
  harness restored; (b) the untracked scratch dir `evidence/h07-core/run/`
  (prior single-case probe artifacts) was moved out of the tree. No RTL or
  comparison logic changed in this session.
* Acceptance re-run from the clean tree: the remote execution box's tree was
  rsynced and sha256-verified byte-identical (4 RTL files, release manifest,
  case cache, results). Two clean full Verilator runs (tags
  `verilog-accept-lh07-r1`, `-r2`; --jobs 4, Verilator 5.041): 34/34 dev +
  stress bit-exact, 2,361,344 compared samples per run, OVERRUN/OVERFLOW 0
  everywhere, 113/113 latency events in-window at distance exactly
  128 - p. Every per-case dump-artifact sha256 in BOTH runs is identical to
  the original pair (`verilog-accept[-run2]`) committed with the evidence;
  consolidated run-manifest sha256 = 81d1ec68b2714a41083b71b17ff4fc37d822d6b933b1d952f1125a627101bd6b
  for both new runs (== original pair) — the artifacts are a deterministic
  function of (tree, vector, toolchain).
* iverilog shadow (24 frames, tag `iverilog-shadow-lh07`): not recorded from this session: the fresh 24-frame shadow re-run was still in flight (27/34 as of 11:37Z) under sustained shared-box contention when the evidence commit was made; the committed `results-iverilog-shadow.json` (34/34, same tree bytes) remains the standing shadow record..
* Live negative controls from this tree (`python3 -m unittest tests.test_h07 -v`,
  remote box): glitch injects a one-sample +1 at frame 12 / sample 17 and the
  clean comparator FAILS and localizes exactly that sample; sum-order mutant
  (clip-then-sum vs sum-then-clip of the same 4 I32 partials) FAILS on the
  clipping burst vector; strip-observability synthesis gate maps 0 flops.
  Full transcript: `evidence/h07-core/test-h07-live-controls-2026-09-23.txt`.
* Venue + timing: this session's local host measured load ~122 on 10 CPUs
  (sim-infeasible in budget); heavy runs used the already-running repo-remote
  m5.2xlarge (per execution-venue rules; no new provisioning). r1 06:52-08:17Z
  (85 min), r2 08:17-09:33Z (76 min), under ongoing shared-box contention
  from a concurrent H08 builder's sim sweep (its runs live under its own
  tags/trees; none of this session's artifacts touched them).
* Shared-host findings (environment, no action on this PR's claims):
  (i) an unattributed working-tree edit of `tools/n08_build_release.py`
  (DX7_ARCHIVE env fallback, inert to conformance — the cache resolves all
  dev32 rows and no archive is opened) appeared at ~06:11Z and was reverted;
  (ii) the worktree directory was removed externally at ~08:05Z (branch
  intact; restored via `git worktree add` — file-set re-checked out from the
  same commit, so committed bytes were unaffected);
  (iii) the branch was force-rebased onto updated main (U02 prep #72) between
  sessions — verified: H07-relevant diff 884a7fa..d88ccae is exactly 0 bytes
  (rtl/, tools/h07*, tests/test_h07.py, evidence/h07-core/, H07 docs, release
  manifest), so pre-rebase run outputs remain valid evidence for the tip.