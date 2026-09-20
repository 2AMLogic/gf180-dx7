# H02 — Full-state storage feasibility probe (mapped, gf180mcu)

Issue: #21 (epic #2) · Date: 2026-09-20 · Planning ID: H02
Depends on: D01 (library inventory), N01 (state widths + schedule, §6/§7 of
`DECISIONS-NUMERIC-v1.md`) · Probe RTL: `rtl/storage_probe.v` ·
Tool: `tools/storage_probe_synth.py` · Tests: `tests/test_storage_probe.py` ·
Evidence: `evidence/h02-storage-probe/`

**Claim advanced:** the 16-note state image has a plausible path to a real
gf180mcu storage mapping — measured here against the actual 7t cell library
and the actual foundry SRAM macro views, not assumed from FPGA block RAM.

**Claim boundary:** everything below is a *storage* probe result. There is no
place-and-route, no timing closure, no utilization, no full-chip fit claim,
and no claim about the FM datapath's area or fidelity (H03/H10/N02+).

---

## 1. Byte/bit accounting (reconciled to N01 §6)

Recomputed by `tools/storage_probe_synth.py::load_accounting` from
`spec/numeric-profile-v1.json` (`storage`) and re-derived independently in
`tests/test_storage_probe.py`; the RTL's own field-width parameters are
cross-checked field-for-field against the JSON classification (38 fields, in
order), so a width drift fails the suite before synthesis runs.

| Class | Source (N01 §6) | Instances | Bits each | Bits |
|---|---|---:|---:|---:|
| Operator runtime state | `FmOpParams` + `Env` + derived env params (13 fields) | 96 | 325 | 31,200 |
| Note runtime state | `Dx7Note` + `PitchEnv` + voice bookkeeping (16 fields) | 16 | 446 | 7,136 |
| **Replicated runtime total** | | | | **38,336** (= 4,792 B exactly) |
| Patch-shared store | rates/levels/modes/mods, one copy (9 fields) | 1 | 444 | 444 (55.5 B) |
| **Probe state total** | | | | **38,780** |
| Registered observability flop (XOR reduction) | probe structure | 1 | 1 | 1 |
| **Expected mapped flops** | | | | **38,781** |

This is N01 rev 1.1's relabeled classification (v1's "exact 32,416 bits" was
a mislabeled subset omitting `FmOpParams.freq`/`level_in` and the per-note
derived env params). Patch bits are excluded from replication exactly as
justified member-by-member in N01 §6. **Not counted anywhere:** event-queue
depth (H03's number per NUM-011), tables (ROM-as-logic, N01 §6 cost lines),
control/DSP registers (not storage-classified yet).

## 2. What was built

`rtl/storage_probe.v` — one synthesizable module carrying the entire
projected state image: write-bank / read-mux organization, **one write port**
(`wr_cls` picks operator bank / note bank / patch) and the read structure the
N01 schedule needs at the assumed **1 evaluation/cycle access**: each cycle
reads one 325-bit operator bank and the owning 446-bit note bank; the patch
image is directly readable (event-time commits). Bank decode is per-bank and
constant-offset (no address-arithmetic multiplier trees — the first
elaboration attempt built 32-bit multipliers into every bank offset and did
not finish; the committed decode is structural).

**Observability is load-bearing.** Every state bit feeds its bank's read mux
or the patch bus, and the mux outputs are XOR-folded into `state_obs`, one
registered reduction bit. Synthesis therefore cannot remove unused state.
The negative control proves the mechanism: compile with
`-DSTORAGE_PROBE_STRIP_OBSERVABILITY` (all read/observability paths removed)
and yosys deletes **every** state flop — the flop-count gate then FAILS
(`yosys_strip.log`: 0 cells). `tests/test_storage_probe.py` runs both
directions live. Per-field detectability is structural: dropping any field
from the RTL drops the mapped flop count below 38,781 and the gate fails.

## 3. Mapped result (measured, real cells)

Flow: `yosys -p "read_verilog …; hierarchy -check -top storage_probe; synth
-top storage_probe -nofsm; dfflibmap -liberty <7t liberty>; abc -liberty
<7t liberty>; stat -liberty <7t liberty>"` (`-nofsm`: the state image is not
a protocol FSM; the pass only burns time on 38k flops).

| Item | Value | Class |
|---|---|---|
| Yosys | 0.69+post, git sha1 `143eb14f9cc…`, native macOS | measured identity |
| Liberty | `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, ciel commit `54435919ab…`, sha256 `330aa9439255e387…` (full hash in the evidence JSON) | measured identity |
| Constraints | none (no SDC, no clock): storage probe only; `DONT_USE` policy: none — `*_1` drive cells allowed (declared per D01 §4/§10.3) | declared |
| Mapped flops | **38,781 × `dffq_1`** = 2,468,829.5 µm² = **2.469 mm²** | **measured** |
| Enable muxes | 38,780 × `mux2_1` (exactly one per state bit — D01 §4: the 7t library has no enable flop; 63.66 + 28.54 = 92.20 µm²/bit-enable pair) | measured |
| Total mapped cells | 100,681 (27 distinct cells) | measured |
| Chip area (flops + write decode + read muxes + XOR tree) | 4,115,370.0 µm² = **4.115 mm²**, of which 60.0 % sequential | measured |
| Per-state-bit, all-in | 4,115,370 / 38,780 = **106.13 µm²/bit** | measured on this structure |
| Flop-only floor | 63.66 µm²/bit | measured |

Icarus Verilog 13.0 smoke bench: all 113 banks written and read back
bit-exact through the ports, observability live (`PASS storage_probe_tb`).

Gate results (issue #21 acceptance):

- **Full mode: PASS** — mapped flops 38,781 == 38,336 + 444 + 1 (obs flop
  documented separately). Nothing was optimized away.
- **Strip-observability negative control: MET** — gate FAILS as required
  (mapped flops 0).

## 4. Access schedule at the assumed 1-eval/cycle access

N01 §7 frame arithmetic: 750 frames/s; **6,144 op-evals/frame**
(96 evals/sample × 64 samples); design point 24.576 MHz → 32,768 clocks/frame
(18.75 % op-loop share); fallback 12.288 MHz → 16,384 (37.5 %). The probe's
port structure serves the 1-eval/cycle access pattern:

| Cycle(s) in a 64-sample frame | Read ports (2R) | Write port (1W) |
|---|---|---|
| 0 (frame boundary) | patch image + control state | event/patch commit, LFO/pitch-EG updates |
| 1 … 6,144 — eval slots: for sample s = 0…63, note n = 0…15, op i = 0…5 | op bank (n,i) + note bank (n) | delayed writeback of the op bank evaluated 96 slots earlier (stage register) |
| 6,145 … 6,256 — control tail | — | 96 operator-envelope commits + 16 pitch-EG commits (once per frame, NUM-002/NUM-005) |
| 6,257 … 32,768 | — | idle / output drain / pipeline margin |

Write traffic totals ≈ 6,144 + 96 + 16 + event-time patch ≤ ~6,300 writes
against 32,768 available cycles — count-feasible at both candidate clocks.

**Conflicts, identified honestly (issue negative control):**

1. **Same-slot read-modify-write is impossible with one write port.** The
   feedback history (`fb_buf_`) is read and written within its note's
   evaluation slot. The schedule above resolves this by delayed commit (the
   new value rides a stage register to the next slot of the same bank).
   Any architecture needing same-cycle RMW — or two writes in one cycle —
   needs a second write port.
2. **1RW macros cannot serve this schedule.** A foundry macro has one port;
   the schedule needs 2 reads + 1 write per eval cycle. Banking 38,336
   random-access bits across 512 B macros serializes the op loop. This is
   why per-note and per-operator runtime state stays in flops regardless of
   the area comparison below.
3. The 1-eval/cycle access is the probe's assumption. N01 §7 budgets 2–3
   clk/eval all-in for the shared op core; a slower core *reduces* required
   storage bandwidth (same ports, fewer accesses/s), so the port conclusion
   is conservative.

## 5. Flops vs foundry SRAM macros, per state class

Foundry macro inventory (measured from the local ciel tree, LEF SIZE ×
cross-checked against Liberty `area` — delta 0.0 µm² on all four; the
custom 2AMLogic 256×32 macro has **no Liberty views** and is integrated
nowhere, so it rules nothing in or out here; `gf180-sram` macros cap at
**512 B**):

| Macro | Capacity | LEF size (µm) | Area (µm²) | µm²/byte |
|---|---:|---|---:|---:|
| `gf180mcu_fd_ip_sram__sram64x8m8wm1` | 64 B | 431.86 × 232.88 | 100,571.6 | 1,571.4 |
| `…sram128x8m8wm1` | 128 B | 431.86 × 268.88 | 116,118.5 | 907.2 |
| `…sram256x8m8wm1` | 256 B | 431.86 × 340.88 | 147,212.4 | 575.0 |
| `…sram512x8m8wm1` | 512 B | 431.86 × 484.88 | 209,400.3 | 409.0 |

Tiling options (counts derived from the measured areas above):

| Store | Option | Macros | Area | vs mapped flops |
|---|---|---:|---:|---|
| Patch store, 444 b = 55.5 B | 1 × 64 B macro | 1 | 100,571.6 µm² | **patch flops win 3.6×**: 444 × 63.66 = 28,265.4 µm² — **verdict: flops** (also zero schedule pressure: event-time commit only) |
| Runtime state, 38,336 b = 4,792 B | 10 × 512 B (uniform) | 10 | 2,094,002.8 µm² = 2.094 mm² | runtime flops-only 2.4405 mm² |
| Runtime state | 9 × 512 B + 1 × 256 B (min-area mix) | 10 | 2,031,814.9 µm² = 2.032 mm² | 16.7 % under flops-only — **but schedule-infeasible (§4.2) and bank-select/mux cost unmodelled** |
| Everything incl. patch, 4,848 B | 9 × 512 B + 1 × 256 B | 10 | 2,031,814.9 µm² | same 10 macros; the 55.5 B of patch never changes the macro count |

**Macro-vs-flop verdict for the shared store: flops, on both area and
schedule.** The 444-bit patch store is far below even the smallest macro's
capacity; one 64 B macro costs 3.6× the mapped flops it would replace. The
runtime classes stay flops for the port reasons of §4 even though a
macro-tiling *area-only* comparison is mildly favorable (−17 % against the
flop-only number, before paying the unmodelled decode/mux/timing cost D01
records as a gap, and with no family precedent of an integrated gf180 SRAM
macro at any size — D01 §5: KB-scale on-chip RAM with timing views is
*inaccessible*).

## 6. Bounded finding vs the quarter slot (and the family anchors)

Family anchors (measured in sibling gf180mcu flows, quoted via D01 §5 and
the issue's prior-art list — they are *their* designs' calibrations, never
transferred silently): replicated state ≈ **118 µm²/bit** all-in
(ladder-channel); polysynth voice **235 flops/voice** = 14,960 µm²; FIFO
storage **120 µm²/bit**; `dffq_1` 63.66 µm²/flop.

**New measurement (first family datapoint at 16 voices — not an
extrapolation of the mono or 4-voice datapoints):** the complete 16-note
state image maps to **4.115 mm²** all-in on this probe's structure
(**4.068 mm²** replicated-only, derived at the measured 106.13 µm²/bit),
i.e. **2.46×** the 1.6734 mm² wafer.space quarter-slot core (replicated-only
2.43×). The measured per-bit all-in (106.13) lands ~10 % under the family's
118 µm²/bit anchor — a different design's overhead mix brackets ours from
above; the flop-only floor (63.66) and this probe's all-in (106.13) bound
what a production organization could pay.

**Bounded finding (strengthens N01 §6's, now on mapped cells instead of a
per-bit anchor):** the 16-note runtime state cannot be all-register in a
quarter slot — nor in a 4×4 TinyTapeout tile group (≈1.06 mm², worse). This
is an area-pressure finding **for H03/H10**, not a license to shrink
polyphony: the 16-note goal stands (DEC-003 is not a relaxation knob). Per
the issue's stop rule this is the recorded trigger for an owner architecture
decision (delayed-commit single-write-port organization vs multi-port flop
arrays vs a separately-budgeted macro-integration effort) before H03/H07
build on a storage plan. The macro path's area case rests on views that
exist locally but are integrated in no chip; treating macros as solved would
be the same mistake D01 was written to prevent.

## 7. What this does NOT say

- **No place-and-route, no timing closure, no utilization, no
  routability, no PDN/IR claim of any kind** — no clock constraint was
  applied; nothing here says any path closes at 24.576 MHz or 12.288 MHz
  (that is H01/H10, in the ORFS flow, at the chosen corner).
- **No full-chip fit claim.** This probe is storage only: the DSP, mixing,
  I2S, control and pad/route overhead are neither measured nor estimated
  here. "4.115 mm² of state machinery" + "quarter slot = 1.6734 mm²" is a
  finding about state storage, not a verdict on the chip.
- **No functional-fidelity claim** — the probe computes nothing; agreement
  with the frozen model is N-series scope. The smoke bench proves the
  storage structure reads and writes what was written, nothing more.
- **No preset-quality, musical-usefulness, or original-DX7-fidelity
  implication.**
- **No claim that the foundry SRAM macros are integrable** — Liberty views
  exist locally (measured), but no gf180 SRAM macro is integrated in any
  family chip; bank-select/mux cost is unmodelled (D01 §5).
- Combinational area depends on the `abc` run and the declared `DONT_USE`
  policy (none here); D01 measures ±29–40 % policy sensitivity on sibling
  designs. Flop counts and flop area are policy-independent in this run
  (everything maps to `dffq_1`).

## 8. Uncertainties

1. The all-in per-bit number is probe-structure-specific: it includes
   full-width read muxes and the observability XOR tree. A production
   datapath may share or shrink these; honest range 63.66 (flop-only floor)
   to ~118 µm²/bit (family anchor) with this measurement at 106.13.
2. The 1-eval/cycle access is an assumption (§4.3); a 2–3 clk/eval core
   lowers required bandwidth but not the flop-vs-macro verdict.
3. Bank granularity (96×325 / 16×446) is this probe's choice; re-banking
   changes mux shape, not the 38,780-bit image.
4. Event-queue depth is excluded (H03 owns it, NUM-011); control-path
   registers are not yet storage-classified.
5. Single corner (`tt_025C_5v00`) and single policy run; ss-corner timing
   and TT's 3.3 V environment (D01 §2/§10) unexplored — no timing claim
   either way.
6. Evidence was produced on the recorded local tool/PDK install; CI
   re-runs are guarded skips until exercised (D01 §6, "CI assumed").

## 9. Evidence and reproduction

| Artifact | Identity |
|---|---|
| `evidence/h02-storage-probe/storage_synth_report.json` | gates, runs, accounting, macro analysis, tool/liberty/RTL identities (sha256s inside) |
| `evidence/h02-storage-probe/yosys_full.log` | full mapped transcript (sha256 `03d6f9316ba0e387…`) |
| `evidence/h02-storage-probe/yosys_strip.log` | strip-mode transcript — 0 cells (sha256 `8fe855ec63e58245…`) |

Reproduce:

```sh
python3 tools/storage_probe_synth.py                 # full + strip + macros + smoke
python3 -m unittest tests.test_storage_probe -v      # gates, controls, arithmetic
iverilog -g2012 -o /tmp/h02.vvp rtl/storage_probe.v rtl/storage_probe_tb.v && vvp /tmp/h02.vvp
```

Statuses: full gate **PASS**; strip negative control **MET** (gate fails as
required); smoke bench **PASS**; P&R, timing, fit, CI re-run: **NOT_RUN /
out of scope by design**.
