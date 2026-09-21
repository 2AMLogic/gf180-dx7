# H05 — Routing/feedback RTL (issue #27)

Issue: #27 (epic #2) · Date: 2026-09-21 · Planning ID: H05
Depends on: H03 (`docs/CONTRACT-CORE-v1.md`, issue #25), N08 frozen release
(`reference/release/`, issue #24), N04 (issue #18: `algorithm.py` — this
issue's semantic spec), H01 (issue #20: RTL style + harness pattern) ·
Router RTL: `rtl/alg_router.v` · Bench: `rtl/tb_alg_router.v` ·
Conformance: `tools/h05_compare.py` · Synthesis: `tools/h05_synth.py` ·
Tests: `tests/test_h05.py` · Evidence: `evidence/h05-routing-rtl/`

**Claim advanced:** the routing/feedback layer of the H03 module
`algorithm_router` is implementable in RTL and is **bit-exact to the
frozen N04 integer model** on a directed 132,288-eval suite covering all
32 algorithm graphs × feedback {0, 3, 7} × 3 frames (block-level feedback
recirculation) plus fb-history-seed, fb-off-untouched and gating /
has_contents runs — including the H03-contracted eval-slot ordering and
the staged delayed feedback-history commit — and survives gf180mcu 7t
synthesis with every output observable at a mapped **1.149 mm²**
(1,148,899.6 µm², 37,930 cells, 7,407 mapped flops). Targeted mutations
(operator-order permutation, inverted feedback sign, one-block-delayed
history, mutated table hex) each demonstrably fail the check they target.

**Claim boundary:** this is a *routing-layer probe*. The operator kernel
arithmetic is the eval seam's other side and is H01's measured scope
(1 clk/eval, 4,794/4,794 bit-exact there); no integration claim (H07), no
pitch-path claim (H06), no place-and-route, no timing closure, no fit
claim, and no audibility claim (listening records only, plan §7). The bar
here is exactly the first of AGENTS.md's three separated claims: RTL
matches the frozen integer model. Dexed agreement and original-DX7
fidelity are not touched by any number below.

---

## 1. What was built

`rtl/alg_router.v` — the 32-algorithm Mark I router, one-for-one with
`AlgorithmRouter.render` (`src/gf180_dx7/model/algorithm.py`, contract
`gf180-dx7-integer-v1`; `docs/N04-ALGORITHM-MODEL.md` §§2–4). The pinned
GPL engine file (EngineMkI.cpp) is the semantic specification only; no
bytes are copied (NUM-001).

### 1.1 Table regeneration identity (one shared artifact)

The 32-row connection table ROM is `$readmemh`-loaded from
`reference/tables/alg_table.hex`, **generated** by
`tools/h05_compare.py --gen-tables` FROM the model's `ALGORITHMS` tuple —
the model and the RTL share one table artifact, and a sha-pin manifest
(`reference/tables/alg_manifest.json`, schema
`gf180-dx7/table-manifest-v1`, table sha256 `2d9abc6abcd0…`) pins the
bytes. This is the gf180-parasynth `gen_tables` discipline (single
generator, `.hex` + sha-pinned manifest, `--check` re-runs the generation
byte-for-byte; docs/REUSE-RULINGS.md), the same discipline N02's
`tools/gen_tables.py` uses for the operator tables. `--check-tables`
re-runs the generation in memory and fails on any drift; **every compare
run performs the check before simulating**, so a hand-edited or mutated
hex never silently becomes the shared bytes. `tests/test_h05.py` re-runs
the regeneration identity and decodes the ROM independently against
`decode_op(ALGORITHMS)` (including the exactly-one-feedback-op-per-row
DX7 structure fact).

### 1.2 Interface and structure

Per-frame program: `cfg_algorithm[4:0]`, `cfg_feedback[4:0]`
(fb_shift = feedback ≠ 0 ? 8 − feedback : 16, dx7note.cc:197), six
per-op loads (`phase`, `freq`, `level_in[27:0]`, `gain_out`), and the
committed 2-element feedback history (`cfg_fb0/1` — the note-bank store
in H03's terms). `start` renders one 64-sample frame; `frame_done`
pulses; a combinational readout mux exposes 0..63 note output, 64..69 op
phases, 70..75 op `gain_out`, 76/77 committed history.

**The operator kernel is the eval seam.** Each cycle with `eval_req` the
router presents `(eval_op, eval_phase_in = phase + mod, eval_gain,
eval_mod)` and consumes `eval_y` (int32) combinationally — the same seam
the model calls `sin_fn(op, i, phase_plus, gain, mod)`. Conformance
drives the seam with the SAME scripted source on both sides; in
integration the seam is the H01 operator probe (1 eval/clk — the H03
budget's operator-eval row).

State (the N04 facts): two **never-cleared** 64×32 interconnect buses
(stale contents persist across frames; `has_contents` add-suppression is
what makes them safe), a per-frame-cleared 64×32 note output buffer
(caller-zeroed audiobuf), 6×(32 b phase + 32 b freq + 28 b level_in +
16 b gain_out), the committed fb pair, `has_contents[3]` (init 1,0,0 per
frame, EngineMkI.cpp:298), and the form working registers.

Mirrored semantics, with the model's citations:
ops strictly in param order 0..5, one eval per clock (the H03/H01
contracted eval-slot ordering); row decode inbus/outbus/add/fb-loop with
the ALG 4/6 op0→0xC4 patch when feedback is on (:301-305); the Mark I
gate `gain1 = gain_out==0 ? 16383 : gain_out`, `gain2 = 16384 −
(level_in >> 14)`, render iff `gain1 ≤ 16284 ∨ gain2 ≤ 16284`, `gain_out`
committed either way (:314-318, NUM-009); add forced off onto an empty
destination and empty input bus falling back to pure (:320-324);
carrier sum `out[i] = y + out[i]` in op order; the ALG 4/6 inline serial
chains (op0(fb)→op1[→op2]→output, out[i] = y overwrite, the pinned
full-delta downstream gains :224/:266-267) and the ALG 32
`min(fb_shift+2, 16)` rule (:343); every op's committed phase advances
exactly one frame per render (`freq << 6`), gated and serial-consumed
ops included (:331-339 + :361).

## 2. Schedule-delay handling (the H03 contract)

`docs/CONTRACT-CORE-v1.md` §3 resolves the same-slot read-modify-write
hazard for the feedback history by **staged delayed writeback**: a slot's
new value rides a stage register and takes the write port at a later
slot. The router implements exactly that: the committed history pair is
**read at the fb form's dispatch** into stage registers (`y0_w`, `y1_w`),
all intra-frame feedback arithmetic runs on the stages, and the commit
(`fb0_q/fb1_q ← y0_w/y1_w`) happens at the form's **end slot** — the
read-modify-write never shares a slot, matching the model's
read-at-frame-start / write-inside-the-fb-form (:196/:203-204,
:227/:242-243, :271/:291-292). Later ops never read the history before
the commit, so committing at the fb form's end is exact against the
model (only one fb op exists per algorithm).

The eval-slot ordering is the contract's: one eval per clock in param
order, serial forms consuming their ops inline within a sample (3 evals
for ALG 4, 2 for ALG 6), exactly as `render` dispatches. The conformance
stream compares `(op, phase_in, gain, mod)` **per eval in issue order**,
so any reordering, stall, or extra/missing eval is a mismatch — the
"schedule-induced delays" acceptance item is checked by construction,
not inferred. Throughput is 1 eval/clk (one eval every cycle of a form),
consistent with the H03 operator-eval row (6,144 clk/frame of 32,768 at
the 24.576 MHz design point); no timing claim attaches to this number
(§5).

## 3. Equivalence to the frozen model (bit-exact, no tolerance)

`tools/h05_compare.py` drives the frozen `AlgorithmRouter` (the golden)
with a scripted int32 operator source `y = w32(BASE[op] + t·STEP[op] +
3·mod)` — modulation-sensitive, the model battery's seam — over **116
deterministic runs**: all 32 algorithms × feedback {0, 3, 7} × 3 frames
(block-level recirculation: frame N+1's fb terms consume frame N's
committed history), plus 12 fb-history-seed runs (frame-start read path,
two-element depth window, +2 shifts), 2 fb-off runs with a poisoned
history (must come back untouched), and 6 gating/has_contents scenarios
(N04's list: gated fb op with the 0xC4 semantics, forced-off bus add,
bus2-writer gated). The bench streams the RTL's eval records, per-sample
note outputs, and per-frame state readouts; the Python comparator judges
bit-exact record-by-record with first-mismatch localization
(record line, run, eval index, algorithm, op, expected vs actual).

**Result: PASS — 116 runs, 348 frames, 132,288 evals, 159,548 stream
records, 0 mismatches** (issue requires ≥ 6,000 evals). Coverage
reported separately from agreement:

| Coverage dimension | Measured |
|---|---:|
| Runs (96 matrix + 20 directed) | 116 |
| Matrix coverage | 32 algorithms × feedback {0,3,7}, all PASS |
| Frames rendered | 348 (3 per run — cross-frame history recirculation) |
| Operator evaluations | 132,288 (min 6,000) |
| Algorithms exercising the plain fb form / serial forms / +2 rule | 26 / ALG 4 & 6 / ALG 4, 6, 32 |
| Gated-op scenarios (gate + has_contents fallbacks) | 6 |
| Eval-record fields compared per eval | op, phase_in, gain, mod |
| Per-frame state compared | 64 note outputs, 6 phases, 6 gain_out, fb pair |

Tool identity (sim): Icarus Verilog 13.0 (stable) (`iverilog -g2012`),
vvp, golden = `src/gf180_dx7/model/algorithm.py` on CPython 3.x — hashes
of RTL, program, table artifact, and golden stream are in
`evidence/h05-routing-rtl/compare_report.json`.

## 4. Mapped result (measured, real cells)

Flow: `read_verilog; hierarchy -check -top alg_router; synth -top
alg_router; dfflibmap -liberty <7t>; abc -liberty <7t>; stat -liberty
<7t>`.

| Item | Value | Class |
|---|---|---|
| Yosys | 0.69+post, git sha1 `143eb14f9cc…`, native macOS (same install H01/H02 measured) | measured identity |
| Liberty | `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, ciel commit `54435919ab…`, sha256 in the report JSON | measured identity |
| Constraints | none (no SDC, no clock — probe flow per the issue); `DONT_USE` policy: none, `*_1` cells allowed (declared per D01 §4/§10.3) | declared |
| Mapped flops | **7,407 × `dffq_1`** = 471,535.5 µm² sequential | measured |
| Total mapped cells | 37,930 (leading: nand2 9,199, oai21 7,478, dffq 7,407, xor3 2,876, xor2 1,761, mux4 1,461) | measured |
| Chip area | **1,148,899.6 µm² = 1.149 mm²** all-in | measured |
| Connection-table ROM | **logic** — the 192×8 table dissolves to combinational gates (yosys MEMORY_MAP; no BRAM in this flow); final stat shows no memory cells | measured |
| Declared vs mapped state | 7,403 declared flop bits (ROM array + read-mux reg excluded as non-flop with reasons in the report) → 7,407 mapped; the small difference is expected and behavior-preserving | measured + documented |

Gate results (issue #27): **full build PASS** — mapped flops 7,407 >
4,096 (the two persistent 64×32 bus buffers are irreducible N04 state:
never cleared, every bit observable through the readout mux or the
state_obs fold), area > 0, cells > 0. A build that lost the bus state or
had outputs optimized away would land below the floor / at 0 area and
FAIL.

Timing: **no STA exists in this flow and no nanosecond number is
claimed from any source.** Whether 24.576 MHz closes at the ss corner
remains unmeasured (D01 §2; H10's measurement in the ORFS flow). The
1-eval/clk throughput statement is a structural/simulational fact, not a
timing closure result.

## 5. Negative controls (each live, each must fail its check)

All four run in `tests/test_h05.py` / `tools/h05_compare.py` on every
invocation of the suite (guarded only on missing tools → NOT_RUN naming
the tool, never a silent pass):

| Control | Build / action | Required | Observed |
|---|---|---|---|
| Operator-order permutation | `-DH05_MUTATE_OP_ORDER`: operator STATE addressing permuted 0..5→5..0, table row order and eval labels unchanged (the model control's "permuted params view": wrong operators gated, loaded, ramped, advanced) | fails ≥ 28 of 32 algorithms | **fails 32/32** (2,130 mismatching records; first at run 0) |
| Inverted feedback sign | `-DH05_INVERT_FB_SIGN`: the fb term is negated (`mod = −scaled_fb`) | every fb>0 run with a live feedback path fails | **77/77 fail** (52,824 records; the 5 exempt runs gate the fb op itself — no feedback path, provably invisible) |
| One-block-delayed history | `-DH05_DELAY_FB_COMMIT`: the history commit is promoted one block late; frame N reads frame N−2 results | every fb>0 run with a live feedback path fails | **77/77 fail** (19,115 records) |
| Mutated table hex | a flipped table byte (and a corrupted manifest pin) fed to `--check-tables` | sha-pin check FAILS before any simulation | **fails with "sha-pin check FAILED"** |

The strip-observability synthesis control completes the set:
`-DH05_STRIP_OBSERVABILITY` ties off every output — including the eval
stream — and yosys deletes every state flop and cell (**0 / 0**,
`yosys_strip.log`), so the flop gate fails exactly as required. The
clean battery is asserted passing first, so every mutant failure is
attributable to its injected defect. Feedback-0 runs legitimately
survive the feedback mutants (no history is read or written when
fb_shift = 16, :299) — the control criteria exclude only those runs and
the fb-inert gated runs, by declared rule, never per-case.

## 6. What this does NOT say

- **No place-and-route, no timing closure, no utilization, no
  routability, no PDN/IR claim of any kind** — no clock constraint was
  applied; no nanosecond number exists in this report. Clock closure at
  12.288/24.576 MHz is unmeasured (H10).
- **No full-chip or slot fit claim** — 1.149 mm² is one routing-layer
  probe (including its 4,096-bit bus probe state and 2,048-bit note
  buffer that integration shares/streams differently); the H02/H03
  storage and schedule integration is H07's.
- **No operator-kernel claim** — the eval seam's other side is scripted;
  H01's kernel equivalence is the arithmetic anchor and is not
  re-established here. No post-synthesis netlist re-simulation: the
  equivalence proof is on the pre-synthesis RTL (H01 §5 discipline);
  gate-level LEC remains future work.
- **No original-DX7 fidelity, no preset-quality, no musical-usefulness
  claim** — bit-exactness to the frozen N04 model is the bar; the
  model-vs-pinned-reference budget is N04's result; soundness is
  listening records only (the three claims stay separate).
- **No integration claim** (H07: envelope/pitch/mixer around this
  router), and no N08 end-to-end vector claim yet: the N08 subsets named
  for `algorithm_router` in the H03 contract (`alg-05/16/31`,
  `alg4-fb0/3/7`, `alg6-fb0/3/7`) are covered at the model-seam level by
  this suite's scenario structure; full PCM-level N08 agreement is H07's
  integrated measurement, which this issue does not preempt.
- CI re-runs without iverilog/vvp/yosys/the local ciel install are
  guarded skips (NOT_RUN, naming the missing path), never silent passes.

## 7. Uncertainties

1. The mapped cell/area count varies by a few gates across abc runs
   (source-ordering sensitivity, observed 37,930–38,102 cells across
   reruns); the gate is the principled floor (flops > 4,096, area > 0,
   strip at 0), not an exact-count equality.
2. The eval response is same-cycle combinational in this probe; H07's
   integration wraps the 3-stage H01 pipeline, where the router must
   present eval requests `n` cycles ahead. The FSM's request stream is
   already strictly ordered, but the pipelined-skid adaptation is
   unproven here and is integration work.
3. Scripted-source coverage is modulation-sensitive but not exhaustive:
   a pathological graph behavior outside the 116-run space could in
   principle diverge. The frozen-table decode is checked independently,
   and every dispatch decision is observable in the compared stream.
4. Equivalence is measured on the pre-synthesis RTL; the synthesis
   result relies on yosys mapping correctness (probe-stage trust, H01
   §5).
5. Evidence was produced on the recorded local tool/PDK install;
   re-running elsewhere must re-record tool identities with the reports.

## 8. Evidence and reproduction

| Artifact | Role |
|---|---|
| `reference/tables/alg_table.hex` + `alg_manifest.json` | generated, sha-pinned model/RTL shared table (regeneration identity test-enforced) |
| `evidence/h05-routing-rtl/program.txt` | committed directed run program (deterministic regeneration is test-enforced) |
| `evidence/h05-routing-rtl/compare_report.json` | conformance + mutant-control gates, coverage stats, tool identities, RTL hashes |
| `evidence/h05-routing-rtl/synth_report.json` | mapped gates, cells/flops/area, ROM representation, declared-vs-mapped accounting |
| `evidence/h05-routing-rtl/yosys_full.log` / `yosys_strip.log` | full + strip transcripts |

Reproduce:

```sh
python3 tools/h05_compare.py --gen-tables     # regenerate the shared table + pin
python3 tools/h05_compare.py                  # golden + sim + bit-exact compare + 3 mutant controls
python3 tools/h05_compare.py --check-tables   # sha-pin check only
python3 tools/h05_synth.py                    # yosys full + strip + gates + report
python3 -m unittest tests.test_h05 -v         # all gates, controls, determinism
```

Statuses: conformance **PASS** (bit-exact, 132,288 evals / 159,548
records, 0 mismatches, 32×3 matrix complete); operator-order, fb-sign,
fb-delay and mutated-hex negative controls **MET**; synthesis full gate
**PASS** (7,407 flops, 1.149 mm², ROM as logic, all outputs
observable); strip control **MET** (0 cells); P&R, timing closure,
post-synthesis re-simulation, integration (H07), pitch (H06): **NOT_RUN
/ out of scope by design**.
