# H07 — Integrated 16-note polyphonic PCM core (issue #29)

Issue: #29 (epic #2) · Date: 2026-09-21 · Planning ID: H07
Depends on: H03 (contract), N08 (frozen release), H01/H04/H05/H06 (slices)
RTL: `rtl/dx7_core.v` · Bench: `rtl/tb_dx7_core.v` · Conformance:
`tools/h07_compare.py` (**planned, not yet committed**) · Tests:
`tests/test_h07.py` (**planned, not yet committed**)

**Claim advanced (TARGET, not yet met):** the complete 16-note RTL core
emits the full PCM stream bit-for-bit against the frozen N06+N07 model at
the H03-defined latency.

**STATUS OF THIS BRANCH: WIP — bring-up only.** Every verification status
below is **NOT_RUN** or **BLOCKED** as marked. Nothing in this document is
a conformance, synthesis, latency, or fidelity claim (AGENTS.md statuses;
issue #29 acceptance is unchecked in full). This revision records the
as-built architecture, the integration findings that shaped it, and the
exact remaining work, so the next session resumes at bring-up instead of
design.

## 1. What exists on `loom/h07-core` (this revision)

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
  OVERFLOW, FRESH, FRAME}` and two-flop reset sync. Compiles under
  `iverilog -g2012`; a directed smoke run (reset → commit → note-on →
  compute frames) walks the whole frame schedule and allocates a voice.
* `rtl/tb_dx7_core.v` — host-side bench: SPI master at SCK = clk/16, I2S
  DAC-model decoder + PCM dump (via the core's mixer→I2S load tap), and
  the vector-record pacing described in its header (patch/commit/event/
  soft-reset/run records with model-block arrival alignment).
* Negative-control `ifdef`s present in the RTL, each with a defined
  failure mode: `H07_MUTATE_GLITCH` (one-sample mix glitch, frame 12
  sample 17), `H07_MUTATE_SUM_ORDER` (sum-then-clip; the N08
  voice_sum_order_swap mutation), `H07_MUTATE_TICK_SKEW` (stale event
  read pointer), `H07_STRIP_OBSERVABILITY`. **None has yet been simulated**
  (NOT_RUN): each must demonstrably fail the conformance suite.

## 2. As-built frame schedule (single frame, measured in bring-up)

Measured on the smoke run at the 24.576 MHz design-point cadence
(512 clk/sample, 32,768 clk/frame): one note's full compute pass
(commit + event apply + envelope steps + LFO + per-note control tail ×6
ops + 64-sample router render + mix + readback + reclaim) completes in
~2,070 clocks — ~6.3 % of the frame. The worst-case 16-voice + full-burst
utilization is **NOT_RUN** (needs the conformance suite); the H03 budget
rows this must be reported against are §2 of
`docs/CONTRACT-CORE-v1.md`. OVERRUN is implemented as the sticky
defect detector and must be shown never-set across all vectors.

## 3. Integration findings (recorded, each with its resolution)

1. **`pitch_mod.v` cannot be instanced 16×** (isolate/localize/record per
   the issue): the frozen model's LFO is chip-global — `lfo.keydown()`
   fires on EVERY strike (sync restart + `delaystate_ = 0` always) and
   `select_patch` reconfigures it immediately — while pitch-EG state,
   basepitch and the pm/am depths are per-note and bound to the striking
   patch (DEC-019). Per-instance LFOs diverge from the shared one
   (keysync and delay-ramp trajectories under multi-strike renders). The
   core therefore integrates the per-note pitch path as shared logic over
   the note banks; every constant/table/formula is identical to
   `rtl/pitch_mod.v` / `src/gf180_dx7/model/pitch.py`. H06's unit and its
   evidence are unchanged.
2. **Op eval seam** (H05 uncertainty 2): `alg_router` consumes `eval_y`
   combinationally in the eval-request cycle, so the H01 kernel is reused
   without its 3-stage registers (`h07_op_kernel`, same pinned ROM bytes,
   same arithmetic; 1 eval/clk retained). `operator_probe.v` is untouched.
3. **env_unit rate_scaling width**: `scale_rate` returns up to (99·31)>>3
   = 382, but env_unit's `rs_q` field is 6 bits. Rather than modify the
   landed H04 unit, the H07 note-on derivation compensates exactly:
   send rs' = min(rs, 63) and per-segment rates r'_i = min(r_i + rs, 99)
   − 63 when rs ≥ 64; the model's qrate (clamped ≤ 63) and static-rate
   (`min(rate+rs, 99)`) paths are reproduced identically on every path
   env_unit exposes. The derivation is covered by the conformance suite
   once it runs (NOT_RUN).
4. **Transpose ruling reconciliation**: the frozen model binds
   `NotePitch(transpose_mode="pinned_wrapper")` — transpose is ignored —
   and N08 is H07's conformance oracle, so the core applies NO transpose
   shift (`l' = midinote_to_logfreq(note)`); N08 `perf-transpose-12`
   matches for that reason. H06's documented-mode ruling is a unit-level
   contract; the divergence is a recorded owner reconciliation item, not
   a silent choice (H03 §6 change-control applies to any future flip).
5. **I2S data width**: the task-pinned "16-bit left-justified" field
   cannot carry the frozen PCM — NUM-010 freezes the mix as the
   unclamped ≥20-bit integer sum, and required case ev-sum-15 peaks at
   +43181 (1.3176…) — so the as-built field is 24-bit left-justified
   carrying the exact mix. A 16-bit DAC slice is an H08 owner decision.
   **Never loosened: the conformance transport is the exact mix.**
6. **NUM-008 amp-mod exp** (dx7note.cc:288 float gate): the core computes
   `pt = int(exp(f32(x/2^18)·0.07 + 12.2))` with a fixed-point evaluator:
   exact U7.18 argument (domain: 0 ≤ x ≤ 2^24, where amd ≤ 2^24 and
   ams ≤ 2^24 make f32(x/2^18) exact — larger x requires ctrl_am outside
   the pinned defaults, a declared corner), exact float64 dyadic
   constants for 0.07/12.2/log2(e), a 64-entry 2^(i/64) table and a
   degree-5 polynomial (≤ ~3e-15 relative error). `int(exp(arg))`
   therefore matches the model wherever exp(arg) is farther than ~1e-5
   from an integer; the conformance harness must measure the
   boundary-distance margin over every evaluated point (NOT_RUN) and the
   planned `--audit-exp` sweep quantifies the domain-wide behavior.
   NUM-008 itself anticipated this shape ("amp-mod LUT … committed per
   event/frame by the control path").
7. **DEC-019 binding as-built**: per-note bound patch images (the model's
   actual mechanism) — no 4-entry ring, hence no eviction gap; H03's
   ring-bound conformance gap does not arise. Recorded as an as-built
   strengthening, not a contract edit.

## 4. Verification status (issue #29 acceptance)

| Acceptance item | Status |
|---|---|
| Every output sample bit-exact vs the frozen model at the defined latency | **NOT_RUN** — harness not yet committed |
| Reset determinism (repeated reset runs identical) | **NOT_RUN** |
| Burst events, held notes, 16-voice maximum summation | **NOT_RUN** |
| Two clean simulations artifact-hash identical | **NOT_RUN** |
| Negative control: one-sample glitch / wrong voice-sum order fails + localizes | **NOT_RUN** (controls present as `ifdef`s, unsimulated) |
| Bring-up smoke (compile + full frame schedule + voice allocation) | PASS (iverilog 13.0; this revision) |
| Mapped area vs H02's 38,781-flop accounting + strip-obs gate | **NOT_RUN** (`tools/h07_synth.py` planned) |
| Latency statement per H03 §4.5 | **NOT_RUN** |

## 5. Remaining work (ordered; the next session resumes here)

1. `tools/h07_compare.py`: case resolution via `tools/n08_build_release.py`
   (`CaseSource`; synthetic from the registry, dev32-03/06/30 from the
   pinned archive at `/Users/joseph/Downloads/DX7_AllTheWeb.zip`), the
   host-side frozen-integer frontend (per-op detune deltas and fine terms
   per dx7note.cc:46-53, frozen bend per the float-gated spot — reuse
   h06_compare.py's functions), the patch/register encoding (RTL map:
   0x10-0x15 OP_PITCH {detune,fine,coarse,mode}, 0x18-0x1D OP_FTERM,
   0x20-0x25/0x28-0x2D env rates/levels **of param k's env block (5−k)**,
   0x30-0x35/0x38-0x3D scale A/B, 0x40-0x43 globals incl. ams, 0x44/0x45
   PEG r/l, 0x42 COMMIT; events 0x00/0x01/0x02/0x03/0x04-0x09 deltas/
   0x0B-0x0F), full dev-set (30) + stress cases (max-voice sum, burst,
   reset-mid-render, held notes across a model-grammar `patch <n>` commit
   with poly.py as golden), bit-exact compare with sample localization,
   artifact hashes, exit 0/1/2, and the `--audit-exp` sweep.
2. Run conformance; fix divergences by module isolation (single-note
   cases first, then poly); **never loosen a check**.
3. `tools/h07_synth.py` (adapt h06_synth.py): total cells/flops/area,
   delta vs H02's 38,781 accounting attributed by source (queues ≈ 7k,
   mix buffers, allocator, control, observability), strip-obs gate.
4. `tests/test_h07.py`: conformance (guarded skips naming missing tools),
   the four negative controls live, determinism double-run, interface
   guards, evidence freshness.
5. Latency measurement per H03 §4.5 (event arrival frame → first affected
   SDATA slot; expected (64−p)+64), OVERRUN-never-sets over all vectors.
6. Evidence under `evidence/h07-core/`, catalog byte-pins, this document
   completed, PR with What changed / Acceptance case moved / How checked
   / What remains unproved + "Closes #29".

## 6. What this does NOT say

No conformance claim, no synthesis/timing/fit claim, no latency claim, no
board/P&R claim, no original-DX7 fidelity or musical-usefulness claim.
The smoke run proves the design runs its schedule and allocates voices —
it says nothing about numeric agreement with the frozen model.

## 6. Session notes (orchestrator, 2026-09-22): extracted packer map + remote runbook

### Host-side word packing (verified against dx7_core.v decode logic)

- `0x10+k` OP_PITCH[k]: [7:0]=mode, [15:8]=coarse, [23:16]=fine, [31:24]=detune
- `0x18+k` OP_FTERM[k]: signed addend applied **only when fine != 0**
  (`dx7_core.v` ~1350: `dv_bpr = (dv_fine != 0) ? dv_bpr1 + fterm : dv_bpr1`);
  ratio-mode/fine-0 ops take 0. Fine terms per dx7note.cc:46-53 via
  h06_compare.py's functions.
- `0x20+k` ERATES[k]: [7:0]=R1, [15:8]=R2, [23:16]=R3, [31:24]=R4 (raw bytes, 7-bit)
- `0x28+k` ELEVELS[k]: L1 [6:0], L2 [14:8], L3 [22:16], L4 [30:24]
- `0x30+k` SCALE_A[k]: [7:0]=breakpoint, [10:8]=kvs, [12:11]=right curve,
  [14:13]=left curve, [22:15]=right depth, [30:23]=left depth
- `0x38+k` SCALE_B[k]: [7:0]=output level, [15:8]=rate scaling
- `0x40` GLOB_A: [2:0]=pm-sens idx (patch[143]&7), [15:8]=am depth,
  [23:16]=pm depth, [31:24]=transpose (stored, NOT applied — pinned-wrapper semantics)
- `0x41` GLOB_B: [7:0]=lfo rate, [15:8]=lfo delay, [16]=lfo sync,
  [19:17]=lfo wave, [20]=osc key sync, [23:21]=feedback, [28:24]=algorithm
- `0x43` GLOB_C: per-op AMS, 2 bits each: ams_k at [2k+1:2k], k=0..5
- `0x44`/`0x45` PEG rates/levels (4×7-bit packed, LSB-first per index)
- `0x42` COMMIT; env block index = **5−k** (doc §5 mapping)
- events (page 0): NOTEON D=[14:8]=vel,[6:0]=note; NOTEOFF D=[6:0]=note;
  CC/PB per tb; DD0-5 = per-note frozen detune deltas (NUM-008)

### Remote runbook (AWS repo-remote, 2am#999 path)

- Box: `ssh repo-remote-gf180-dx7` (i-0c0a073ffb9db41d5, m5.2xlarge, 8 vCPU/30 GB;
  EBS survives idle-shutdown stops; restart via
  `.claude/skills/repo/scripts/repo-remote.sh up --yes --json` — IP may change).
- Toolchain: `export PATH=$HOME/oss-cad-suite/bin:$PATH` (Verilator 5.041,
  yosys, iverilog). Sync: `rsync -az --delete --exclude .git <worktree>/ repo-remote-gf180-dx7:~/h07/`.
- Compile proven on-box: `iverilog -g2012 -o /tmp/smoke.vvp rtl/dx7_core.v rtl/env_unit.v
  rtl/alg_router.v rtl/tb_dx7_core.v` (run from repo root so ROM paths resolve).
- Efficiency rules: heavy sim/synth ONLY remote; sweeps under Verilator, final
  acceptance shadow under iverilog (klayout-tools #2223 convention); local runs
  limited to `make test-fast` + affected module (DR-0009).
