# H04 — Envelope/state RTL (exact to N03, mapped, gf180mcu)

Issue: [#26](https://github.com/2AMLogic/gf180-dx7/issues/26) (epic #2) ·
Date: 2026-09-21 · Planning ID: H04 · Depends on: H03 (issue #25, the
interface law), N08 (frozen release), N03 (envelope model, issue #17),
H01/H02 (RTL style + state layout) · RTL: `rtl/env_unit.v` · Bench:
`rtl/tb_env_unit.v` · Equivalence: `tools/h04_compare.py` · Synthesis:
`tools/h04_synth.py` · Tests: `tests/test_h04.py` · Evidence:
`evidence/h04-env-rtl/`

**Claim advanced:** the operator-envelope slice of the core is RTL-exact
to the frozen N03 integer model on a directed 24,639-frame-step suite
(≥ 3,000 required) including worst-rate updates — 27,931 commands, 0
mismatches, bit-exact, no tolerance — with the H03 read-old/write-new
hazard rule verified, the measured worst-case update at **13 clocks
against the contracted 24-clock `env_steps` slot**, and the unit
surviving gf180mcu 7t synthesis with every state bit observable at a
mapped **0.179 mm²** (179,055.9 µm², 8,326 cells, 480 flops). Three
mutation controls demonstrably fail the gates they target.

**Claim boundary:** this is a *single-slot envelope unit*. There is no
place-and-route, no timing closure, no utilization, no fit claim (the
×96 replication, control-tail window integration and frame schedule are
H07/H10), no claim about the pitch-EG rows of H03's `env_unit` module
(pitch domain, H06 input), and no claim about audibility (listening
records only, plan §7). RTL-vs-frozen-model exactness is the bar
(AGENTS.md: the three claims stay separate); Dexed agreement is not
original-DX7 fidelity and this report makes no fidelity claim.

---

## 1. What was built

`rtl/env_unit.v` — the N03 integer operator envelope
(`src/gf180_dx7/model/envelope.py`, contract `gf180-dx7-integer-v1`;
docs/N03-ENVELOPE-MODEL.md §1.3, env.cc:63-151) as synthesizable RTL.
State members mirror N01 §6 member-for-member: `level_`, `targetlevel_`,
`inc_`, `staticcount_` (32 b each), `ix_` (3 b), `rising_`, `down_`
(1 b each) — the 133-bit envelope slice of H02's 325-bit operator bank.
`outlevel_`/`rate_scaling_` enter as committed **param registers**: they
are the per-note DERIVED bank fields (H02 `W_OP_ENV_OUTLEVEL` /
`W_OP_ENV_RATE_SC`); the note-on derivation
(ScaleVelocity/ScaleLevel/ScaleRate, dx7note.cc:163-179, float-gated
math host-side per NUM-008/DEC-010) is applied exactly once upstream by
the event path (H07 scope) — the unit consumes derived values only.

**sr_multiplier is derived, cited, and test-re-derived** (never a bare
literal): uint32 truncation of `(44100.0/48000.0)·(1<<24)` =
15,414,067 (env.cc:47-49; NUM-005 rev 1.1). `tests/test_h04.py`
re-derives it from the formula and cross-checks
`spec/numeric-profile-v1.json: envelope.sr_multiplier_48k` and the RTL
parameter on every run.

The statics `//20` floor division (attack-hold path, env.cc:137-139) is
the constant-multiply identity `floor(x/20) == (x·838861)>>24`, exact
for every reachable x (0..1,764,000, the statics table maximum);
`tests/test_h04.py` verifies the identity **exhaustively** over that
range against the RTL's declared constant.

**Interface** (one command accepted when idle; `busy`/`done`
handshake — the control-tail `tail_start`/`tail_done` integration is
H07): `PARAM` (load derived params), `INIT` (Env::init: level 0, down,
advance(0) — the polyphonic note-on re-init), `KEYDOWN d`
(env.cc:102-107 edge: down flip + advance(0|3) **from the current
level** — the retrigger path; no re-init happens here), `UPDATE`
(param refresh + the pinned advance(2) outlevel-overwrite quirk,
env.cc:166-167 — the dead intermediate sustain-target write is skipped;
the final committed state is identical and the quirk is recorded, not
silent), `STEP` (getsample: one frame step), `ADV k` (direct advance),
`LOAD` (direct slot image write). Every command's commit is visible in
the dump stream, one row per command.

**Micro-sequencer** (single clock domain, synchronous reset, all state
defined at reset per DEC-011):

| Stage | Content | Cycles |
|---|---|---:|
| `S_STEP_CAP` | capture committed slot (the step's single read point) + `staticcount_ − 64` branch + decrement commit | 1 |
| `S_ADV_TGT..MUL2` | advance: scaleoutlevel target + floor-16 clamp; `rising_` + qrate (`rate·41>>6 + rs`, clamp 63); statics hold raw + `//20`; `·15414067 >>24` (both multiplies); commit | 5 |
| `S_STEP_MOV` | movement: rising jump-floor 1716<<16 + `((17<<24 − level)>>24)·inc` + clamp-advance, or falling `level − inc` + clamp-advance | 1 |
| worst case | capture + advance + movement + **second** advance (a step can chain a static-expiry advance and a clamp advance, exactly as the model's sequential `getsample` does) | **13** |

A step therefore always fits the H03 `env_steps` derived-estimate slot
(24 clk) with margin — measured, not assumed (§2).

**Cadence:** one `STEP` per operator per frame, executed in the
control-tail window; per H03 §3 resolution 3 the tail's writes never
share an eval slot (eval reads are slot-disjoint), so the unit's
read-modify-write never collides with the eval pass.

## 2. Equivalence to the frozen model (bit-exact, no tolerance)

`tools/h04_compare.py` generates a directed command stream from the
PYTHON model (the golden: `Envelope` is replayed command-for-command),
runs `rtl/tb_env_unit.v` under iverilog on the identical stimulus, and
compares the full committed slot (level/target/inc/static/ix/rising/
down) after every command BIT-EXACT — any mismatch is an RTL defect to
fix, never a budget to loosen.

**Result: PASS — 27,931 commands / 24,639 frame steps, 0 mismatches**
(issue requires ≥ 3,000 steps), plus per-command full-state dumps.
Coverage is reported separately from agreement (gen_report.json):
788 steps launched a segment advance, 1 step executed the double
advance (static-expiry + clamp in one `getsample`), 493 key-edge
advances fired; the battery reaches **every statics entry through both
paths** (rates 0..99 at rs {0, 27} through the `levels[0]=0` attack
hold → the `//20` divide; rates 0..99 through target==level parks →
raw), **every levellut entry** plus the `>= 20` branch (levels 0..99
swept), **all 64 qrate codes** (shift 8..23), and the worst rates
(rate 99 saturation, rate 1's 940/1175 coefficients, rate 0 statics).

Directed scenarios port N03 §3's case families to the unit level: base
attack/decay/sustain/release-to-nonzero, fast attack with clamp
advance, static hold at a segment top, zero-level attack hold
(`//20`), delayed attack (statics[45]//20 ≈ 650-frame silent window
then slow rise), rate-1 unfinished rise (incl. the rs=1 mutation-flip
point), release mid-decay to the exact-silence floor (16<<16 park),
**retrigger from the current level** mid-decay (no re-init),
sustain park (ix-3 movement gate while down — the DEC-018 sustain
mechanism; the pedal branch itself lives in the note-off application,
H07), update-while-held (the advance(2) quirk), a 100-rate ×
4-rate-scaling sweep (16,000 steps), levellut/statics sweeps,
advance(4)/advance(5) parks and keyup-from-park corners.

**Schedule gate (H03 acceptance):** the bench measures cycles per
command; the worst `STEP` over the whole battery — including
worst-rate updates and both advance launches — is **13 clocks ≤ the
contracted 24** (`spec/core-contract-v1.json` budget row `env_steps`,
derived_estimate 24 clk/op/frame incl. segment advance). The measured
value closes the contracted slot with 45% margin; ×96 the tail
arithmetic is 1,248 clk/frame against the 2,304 budgeted. The frozen
contract stays at 24 until H05/H06 measurements land and H03 re-derives
the budget under its §6 DR process — recorded here, not silently
edited.

**Negative controls (issue #26), each demonstrably failing:**

| Control | Mechanism | Result (committed, compare_report.json) |
|---|---|---|
| Update-timing mutant | `-DH04_MUTATE_STEP_SKEW`: the movement commit is deferred one frame (applied at the next step's capture) — the visible level trajectory lags the model by one frame | **MET**: 18,784 mismatches, first localized at command 2, field `level` |
| sr_multiplier ±1 | `-DH04_MUTATE_SRM`: SR_MULTIPLIER − 1 (N03 §6: reachable coefficients flip) | **MET**: 6,958 mismatches, first localized at command 1 (an INIT advance), field `inc` |
| Strip observability | `-DH04_STRIP_OBSERVABILITY`: every output (incl. `busy`/`done`) tied off | **MET**: yosys deletes all state — 0 mapped flops vs 480; the flop gate FAILS as required |

Tool identity (sim): Icarus Verilog 13.0 (stable) (`iverilog -g2012`),
vvp; golden = `src/gf180_dx7/model/envelope.py` on CPython 3.x — RTL,
vector and expected hashes are in
`evidence/h04-env-rtl/compare_report.json`. Generation is
deterministic — byte-identical across runs, enforced by test.

## 3. Hazards verified (H03 §3)

The envelope slot is one H02-style bank slice with the contracted
read-old/write-new rules:

- **Read-old / write-new:** `slot_rd_data` is the committed state only;
  `slot_wr_en` commits at the clock edge. The bench self-check (run
  before every vector stream, result required `hazard=ok` by the
  comparator) drives a write and asserts the combinational read still
  returns the OLD image in the write cycle and the new image after the
  edge — a back-to-back write→read of the same state slot returns the
  old value per contract. Any violation FAILs the bench and therefore
  the harness.
- **No same-cycle RMW on one write port** (the fb_buf delayed-commit
  lesson, applied to any RMW): the step captures state once at entry
  (`S_STEP_CAP`) and commits once at the end — reads and writes are
  different cycles by construction; a step's two internal commits
  (decrement, advance) land on different edges with the advance's
  value winning, exactly as the model's sequential statements do.
- **Ownership:** external slot writes are accepted only while the unit
  is idle (the step owns the slot while busy); the control-tail window
  is slot-disjoint from the eval pass (H03 §3 resolution 3), so no
  control write ever shares an eval slot in integration.

## 4. Mapped result (measured, real cells)

Flow: `read_verilog; hierarchy -check -top env_unit; synth -top
env_unit; ltp; dfflibmap -liberty <7t>; abc -liberty <7t>; stat -liberty
<7t>`.

| Item | Value | Class |
|---|---|---|
| Yosys | 0.69+post, git sha1 `143eb14f9cc…`, native macOS (same install H01/H02 measured) | measured identity |
| Liberty | `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, ciel commit `54435919ab…`, sha256 `330aa943…` (full hash in the evidence JSON) | measured identity |
| Constraints | none (no SDC, no clock — probe flow per the issue); `DONT_USE` policy: none, `*_1` cells allowed (declared per D01 §4/§10.3) | declared |
| Mapped flops | **480 × `dffq_1`** = 30,557.2 µm² sequential | measured |
| Total mapped cells | 8,326 (abc-mapped; levellut + statics tables land as **logic** — yosys `proc_rom` extracts them as ROMs and MEMORY_MAP dissolves them to gates + registers; no BRAM exists in this flow, final stat shows no memory cells) | measured |
| Chip area | **179,055.9 µm² = 0.179 mm²** all-in | measured |
| Declared vs mapped state | 440 declared register bits → 480 mapped flops. The delta is the `proc_rom`/MEMORY_MAP table re-implementation (registered ROM logic) less merged next-state redundancy — behavior-preserving: the mapping must preserve RTL behavior, proven bit-exact to the frozen model on the pre-synthesis RTL (§2) | measured + documented derivation |

Gate results (issue #26 acceptance):

- **Full build: PASS** — mapped flops 480 > 133 (the irreducible
  envelope slot: each bit has a distinct next-state function and every
  bit drives `slot_rd_data`/`state_obs`), chip area > 0, sequential
  area > 0, abc actually mapped. A build with state optimized away
  would land at 0 flops / 0 area and FAIL.
- **Strip-observability negative control: MET** — with
  `-DH04_STRIP_OBSERVABILITY` (all outputs tied off, `busy`/`done`
  included) yosys deletes every state flop (0 mapped, `yosys_strip.log`);
  the flop gate fails exactly as the "optimized-away build" acceptance
  demands. Both directions run live in `tests/test_h04.py`.

Delay — what yosys gives, labelled honestly: **no STA exists in this
flow and abc's delay-driven mapper prints no achieved-delay number** —
no nanosecond timing is claimed from any source. Recorded hints only:
the micro-sequencer's bounded 13-cycle worst-case step structure (§1,
measured in simulation), and yosys `ltp`'s generic-gate logic depth of
685 after `synth` (2-input-gate levels, includes the constant-multiplier
cones and emits loop warnings — a hint, not a timing number). Whether
24.576 MHz closes at the ss corner remains **unmeasured** (D01 §2);
timing closure is H10's measurement in the ORFS flow.

## 5. What this does NOT say

- **No place-and-route, no timing closure, no utilization, no
  routability, no PDN/IR claim of any kind** — no clock constraint was
  applied; no nanosecond number exists in this report. Clock closure at
  12.288/24.576 MHz is H10's measurement.
- **No full-chip or integration fit claim** — 0.179 mm² is one
  single-slot envelope unit; the chip needs it ×96 with the H02 storage
  organization, the control-tail schedule, pitch-EG, the operator
  kernel, mixing, I2S, link and pads. The ×96 control-tail integration
  (tail_start/tail_done, 96 steps/frame windowed into the frame
  budget) is H07's to build and measure.
- **No pitch-EG claim** — H03's `env_unit` module row also names the 16
  pitch-EG steps/frame; those are pitch-domain (N05/NUM-008) and enter
  at H06. This unit implements the operator envelope exactly (the
  issue #26 brief's scope), and nothing here weakens the H03 module
  contract.
- **No claim that the sustain pedal is implemented inside the unit** —
  DEC-018's pedal branch lives in the note-off application (H07); the
  unit provides the exact mechanism the branch drives (down-edge
  gating, ix-3 hold, release-from-current-level), which is what the
  sustain scenarios exercise.
- **No post-synthesis netlist re-simulation** — the equivalence proof
  (§2) is on the pre-synthesis RTL, which is the artifact the mapping
  must preserve; yosys's mapping correctness is trusted at probe stage
  (same scope decision as H01/H02; gate-level LEC recorded as future
  work).
- **No original-DX7 fidelity, preset-quality, or musical-usefulness
  implication** — bit-exactness to the frozen integer model is the bar
  here; model-vs-pinned-reference is N03's measured budget, and
  soundness is listening records only (AGENTS.md).
- CI re-runs without iverilog/vvp/yosys/the local ciel install are
  guarded skips (NOT_RUN, naming the missing path), never silent passes.

## 6. Uncertainties

1. The mapped flop count (480) is yosys-version-dependent (the
   `proc_rom`/MEMORY_MAP table re-implementation dominates the delta
   from 440 declared bits). The acceptance gate is a principled floor
   (> 133, the irreducible slot) plus area > 0 plus the strip control
   at 0 — not a brittle exact-count equality.
2. `ltp`'s 685-level generic depth traverses abc-internal gate names,
   includes the multiplier feedback paths, and emits loop warnings; it
   is a structural hint and must not be quoted as a delay.
3. The 13-clock measured step bound holds for the directed battery;
   the FSM's worst case is structurally bounded at 13 (capture +
   2×5-cycle advances + movement), so no admissible input exceeds it —
   the bound is a property of the micro-sequencer, not of the vectors.
4. Single corner (`tt_025C_5v00`, 5 V) and single policy run; D01
   measures ±29–40 % DONT_USE-policy area sensitivity on sibling
   designs and TT hardens at 3.3 V — neither variation is explored.
5. Equivalence coverage is the directed battery (tables swept
   exhaustively at the battery level, all qrate codes, both advance
   launches); it is not a formal proof — a pathological input outside
   the reachable state space could in principle diverge, though the
   unit has no state outside the declared bits.
6. Evidence was produced on the recorded local tool/PDK install;
   re-running elsewhere must re-record tool identities with the report.

## 7. Evidence and reproduction

| Artifact | Identity |
|---|---|
| `evidence/h04-env-rtl/compare_report.json` | equivalence + budget + hazard gates, both mutant controls, vector coverage stats, tool identities (sha256s inside) |
| `evidence/h04-env-rtl/vectors.txt` / `expected.txt` | committed golden command stream + expected states (deterministic regeneration is test-enforced) |
| `evidence/h04-env-rtl/actual.txt` / `actual_timing.txt` / `actual_srm.txt` | RTL committed-state dumps, real build and both controls |
| `evidence/h04-env-rtl/gen_report.json` | vector-set stats + sr_multiplier derivation record |
| `evidence/h04-env-rtl/yosys_full.log` | full mapped transcript (ltp + stat inside) |
| `evidence/h04-env-rtl/yosys_strip.log` | strip-mode transcript — 0 cells |
| `evidence/h04-env-rtl/synth_report.json` | gates, runs, declared-vs-mapped accounting, tool/liberty/RTL identities |

Reproduce:

```sh
python3 tools/h04_compare.py                # golden gen + iverilog sim + bit-exact compare + both mutant controls
python3 tools/h04_synth.py                  # yosys full + strip + gates + report
python3 -m unittest tests.test_h04 -v       # all gates, controls, determinism, hazard
```

Statuses: equivalence **PASS** (bit-exact, 27,931 commands / 24,639
frame steps, 0 mismatches); step-cycle budget gate **PASS** (measured
13 clk ≤ 24 clk H03 `env_steps`); hazard rule **PASS** (read-old/
write-new self-check ok); update-timing mutant **MET** (18,784
mismatches, localized); sr_multiplier mutant **MET** (6,958
mismatches, localized); synthesis full gate **PASS** (480 flops,
0.179 mm², all state observable); strip control **MET** (0 flops);
P&R, timing closure, post-synthesis netlist re-simulation, ×96
integration, pitch-EG, CI re-run: **NOT_RUN / out of scope by design**.
