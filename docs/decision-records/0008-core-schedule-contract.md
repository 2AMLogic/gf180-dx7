# DR-0008: The core interface/schedule contract v1 — worst-case budget verdict, design point, and H03 STALE rules

- **DR number:** 0008
- **Status:** Proposed (owner sign-off requested on the clock-target choice:
  24.576 MHz design point with the 12.288 MHz fallback recorded as
  worst-case-infeasible); the remaining contract terms are
  Accepted-by-default per D00 section 0 unless the owner rules otherwise
- **Date:** 2026-09-20
- **Deciders:** loom-builder (agent), pending owner review
- **Related issues:** #25 (H03); grounds #2 epic H-series RTL (H04–H07);
  builds on #24 (N08 freeze, DR-0007), #20 (H01), #21 (H02), #15 (N01),
  #4 (D00)

## Context

Issue #25 requires a proven bounded per-frame schedule — worst case, never
average — before RTL modules are written. The measured inputs:

- **H01** (issue #20): the Mark I operator kernel sustains **1 clk/eval**
  (3-stage pipeline, 4,794/4,794 bit-exact), removing the arithmetic-kernel
  risk N01 §7 carried at 2–3 clk/eval.
- **H02** (issue #21): the 38,336-bit replicated state maps to 38,781 flops
  / 4.115 mm² on a probe structure with the **2R+1W per eval slot** port
  pattern; 1RW SRAM macros cannot serve that schedule (flop verdict); the
  quarter-slot area finding remains an open owner architecture decision.
- **N08** (issue #24, DR-0007): the frozen vector release is the conformance
  oracle for every module.
- **D01** (issue #5): 24.576 MHz ss-corner closure is unproven (the sibling
  design implies 14.0 MHz ss); timing is H10's measurement.

## Decision

`docs/CONTRACT-CORE-v1.md` + `spec/core-contract-v1.json` are adopted as the
normative core interface/schedule contract:

1. **Worst-case budget verdict (headline).** The line-by-line per-frame
   budget demands **17,260 clocks @12.288 MHz against 16,384 available
   (105.35%, shortfall 876) — 12.288 MHz does NOT close the worst case** —
   and **17,344 @24.576 MHz against 32,768 (52.93%, margin 15,424) — the
   design point closes**. The budget accounts every consumer at worst case:
   6,144 measured-cost op evals, 96 envelope steps with worst-case segment
   advance, frequency updates, LFO/pitch-EG, mod application, control-tail
   commits, the worst admissible event burst (16 note-ons + 16 note-offs +
   16 controller events), patch commit, 1,024 convert+mix operations, I2S
   loads, link drain, and controller overhead. An idle frame would cost
   12,332 clocks (75.3%) — closing 12.288 MHz on that number is precisely
   the average-case argument issue #25 forbids. `tools/h03_budget_check.py`
   recomputes every row and enforces the recorded verdicts; the negative
   controls (mutated 2 clk/eval row, burst-stripped budget, fake PASS
   verdict, drifted evidence hash) demonstrably fail it.
2. **Clock target:** **24.576 MHz design point** (the one owner sign-off
   item of this DR). The 12.288 MHz fallback is recorded as
   worst-case-infeasible; reopening it requires an architecture change or an
   explicit owner polyphony ruling — never a silent weakening (DEC-003).
   No timing claim is made for either clock (H10 owns ss-corner closure).
3. **Interface terms** (each cites its D00 decision id in the contract):
   events commit at 64-sample frame boundaries (DEC-014/NUM-011) through a
   depth-8 skid FIFO continuously drained into a shadow image (depth derived
   from the SCK ≤ f_core/4 bound, never guessed), overflow = explicit
   rejection/backpressure (DEC-014); patch load commits atomically into a
   4-entry generation ring with held notes keeping their starting patch
   (DEC-019 — with the declared >3-commits bound recorded for owner ruling);
   the DR-0007-pattern status word carries OVERRUN/QUEUE/OVERFLOW/FRESH/FRAME;
   reset → fresh → image boot with two-flop pad sync (DEC-011/DEC-020);
   event-sample → SDATA latency defined measurably (≤ 2 frames = 128 samples)
   for H07/H09; H02-style load-bearing observability plus optional
   non-invasive block taps; no per-sample host dependency (DEC-010).
4. **Module boundaries** for H04–H07 (frame_controller, op_core,
   state_banks, env_unit, pitch_mod_unit, algorithm_router, mixer, spi_ctl,
   i2s_tx), each with clocking, reset, handshake, and its N08 conformance
   vector subset.

## Consequences and STALE rules

- Any change to the contract **after H04–H07 implementation starts** needs a
  new DR and marks the affected RTL evidence **STALE**.
- Drift of any pinned artifact (N08 bundle, numeric profile, H01/H02
  evidence) fails the budget checker and marks every H03-derived budget
  STALE (DR-0007's rules cascade).
- The `derived_estimate` unit costs are planning upper bounds: H04–H06 must
  measure them and the budget must be re-run against measured values before
  H07 integration claims the schedule.
- This DR makes **no** synthesis/timing/fit/fidelity/audibility claim
  (DEC-013, DEC-024); the H02 area finding stays open for owner decision.

## Evidence

- `docs/CONTRACT-CORE-v1.md` (§2 line-by-line budget, §3 port schedule,
  §4 interface, §5 modules, §6 STALE rules)
- `spec/core-contract-v1.json` (machine-readable rows/verdicts/queues)
- `tools/h03_budget_check.py` (recomputation + hash gates; PASS)
- `tests/test_h03_contract.py` (24 cases incl. four live negative controls)
- Measured anchors: `evidence/h01-operator-probe/` (1 clk/eval),
  `evidence/h02-storage-probe/` (38,781 flops, 2R+1W),
  `reference/release/manifest.json` (bundle `81bcc43c…`)
