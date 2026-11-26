# DR-0008: The core interface/schedule contract v1 — worst-case budget verdict, design point, and H03 STALE rules

- **DR number:** 0008
- **Status:** Ratified 2026-11-26 by the operator (attended `/fleet:decide`
  r20, on #264): the 24.576 MHz design point, with the 12.288 MHz fallback
  recorded as worst-case-infeasible under the H03 planning costs. H10 owns
  timing, and DR-0013 defers the physical rows.
- **Prior disposition:** before ratification, the contract terms other than
  the clock target were in force under D00 section 0's default acceptance.
- **Date:** 2026-09-20 (ratified 2026-11-26)
- **Deciders:** loom-builder (agent); ratified by the operator (rjwalters)
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

1. **Schedule budget.** At 16 active voices, the recorded H03 vector has
   16 note-ons, 16 note-offs and 16 controller events. Its 16 rows sum to
   **17,260 of 16,384 clocks at 12.288 MHz** (105.35%, shortfall 876) and
   **17,344 of 32,768 at 24.576 MHz** (52.93%, margin 15,424). The
   fallback therefore fails the planning budget on the specified burst
   alone. The margin at the design point applies only to the listed rows
   and counts. The SPI interface also admits more expensive bursts, as the
   review probe below demonstrates. The requirement to service every
   admissible frame at 16 voices and 48 kHz remains open.
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

### Event admission at the design point

The H03 checker reproduces the listed vector. Voice count limits how many
notes can sound together. The RTL also dispatches raw `EA_NOTEON` commands
to the allocator when all voices are occupied. With the other H03 rows
fixed, replacing the 4,864 event-application clocks with 100 note-ons at
224 clocks each gives **34,880 clocks**, 2,112 over the frame budget.
Charging exactly the 100 drains and omitting the 64-clock patch commit
for the probe below still gives **34,748 clocks**. The corresponding
168-note-on case costs **50,048 clocks**. This is a planning-budget
**FAIL** for the all-admissible-frame claim.

At 194 core clocks per SPI transaction, a transaction already in flight
can complete at the start of a frame. A window of `F` clocks can contain
`1 + floor((F − 1) / 194)` completions. That gives **169** at 24.576 MHz
and **85** at 12.288 MHz. H03's drain row counts 168 and 84 whole periods.
Correcting that row alone would add one clock to each listed total.

The pin-driven Verilator 5.050 probe starts with 16 live voices and sends
back-to-back raw note-ons across the opening frame boundary. It checks the
arrival frame and spacing, subsequent queue pops, full-voice allocations,
and sticky status bits. Note-on is the most expensive event in the H03
table. The burst of 168 fills the RTL event queue. The 169th arrival tests
the difference between link capacity and event admission.

| SPI note-ons arriving in one frame | Applied with all voices occupied | Rejected | OVERFLOW | OVERRUN |
| ---: | ---: | ---: | ---: | ---: |
| 100 | 100 | 0 | 0 | 0 |
| 168 | 168 | 0 | 0 | 0 |
| 169 | 168 | 1 | 1 | 0 |

Run from the repository root. Each invocation exits nonzero if its row
differs. The all-input RTL deadline has **NO_VERDICT**. A new decision
record must reconcile the H03 contract, machine-readable rows and checker
with the admission rules. [H03 correction #339](https://github.com/2AMLogic/gf180-dx7/issues/339)

```sh
mkdir -p build
verilator --binary --timing -j 4 -Wno-fatal --timescale-override 1ns/1ps \
  --Mdir build/h03-admission --top-module tb_event_admission \
  rtl/dx7_core.v rtl/env_unit.v rtl/alg_router.v \
  evidence/h03-event-admission/tb_event_admission.sv
for burst in 100 168 169; do
  build/h03-admission/Vtb_event_admission "+burst=$burst" || exit 1
done
```

### Board integration

A board supplying only 12.288 MHz needs an additional clock source or a
multiplier for this core's 24.576 MHz design point. The VS1053b, a MIDI
synthesizer and audio codec, accepts 12.288 MHz at XTALI for 48 kHz
playback in its standard clock range. That gives the VS1053b a clocking
advantage on such a board. A like-for-like comparison of the 16-voice,
six-operator workload remains open for the operator's market decision.
[VS1053b datasheet, pp. 1 and 7](https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf)

TI lists both system clocks for the PCM5102A at 48 kHz. That DAC samples
serial data on rising BCK edges. The core emits successive payload bits
in half-BCLK cells, so a direct connection loses alternate bits. An
adapter must recover the 22-bit word and serialize it in a format the
DAC accepts. [H08 pin evidence](../H08-PIN.md#i2s-format-contract-vs-as-built-the-finding-record) · [TI PCM5102A datasheet, pp. 14 and 24](https://www.ti.com/lit/ds/symlink/pcm5102a.pdf)

### H03 inputs and checks

- `docs/CONTRACT-CORE-v1.md` (§2 line-by-line budget, §3 port schedule,
  §4 interface, §5 modules, §6 STALE rules)
- `spec/core-contract-v1.json` (machine-readable rows/verdicts/queues)
- `tools/h03_budget_check.py` (recomputation + hash gates; PASS)
- `tests/test_h03_contract.py` (24 cases incl. four live negative controls)
- Measured anchors: `evidence/h01-operator-probe/` (1 clk/eval),
  `evidence/h02-storage-probe/` (38,781 flops, 2R+1W),
  `reference/release/manifest.json` (bundle `81bcc43c…`)
