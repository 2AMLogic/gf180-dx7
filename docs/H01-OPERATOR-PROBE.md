# H01 — Synthesizable operator probe (mapped, gf180mcu)

Issue: #20 (epic #2) · Date: 2026-09-20 · Planning ID: H01
Depends on: N02 (issue #16: frozen operator model + pinned tables), D01
(tool/PDK inventory) · Probe RTL: `rtl/operator_probe.v` · Bench:
`rtl/tb_operator_probe.v` · Equivalence: `tools/h01_compare.py` ·
Synthesis: `tools/h01_synth.py` · Tests: `tests/test_h01.py` · Evidence:
`evidence/h01-operator-probe/`

**Claim advanced:** the operator kernel is implementable in the target flow
at a measured — not assumed — cost: one representative Mark I operator as
RTL, bit-exact to the frozen N02 integer model on a directed 4,794-eval
suite, surviving gf180mcu 7t synthesis with every output observable, at a
mapped **0.047 mm²** (46,939.96 µm², 2,407 cells) and a sustained
**1 clock per operator evaluation** (3-cycle pipeline latency).

**Claim boundary:** everything below is a *single-operator kernel probe*.
There is no place-and-route, no timing closure, no utilization, no
routability, no fit claim (no full-chip, no slot, no tile), no claim about
the shared 96-state core's storage or schedule (H02/H03/H10), and no
claim about audibility (listening records only, plan §7).

---

## 1. What was built

`rtl/operator_probe.v` — one sequential Mark I operator datapath,
RTL-exact to `src/gf180_dx7/model/operator.py::mki_sin` (contract
`gf180-dx7-integer-v1`; N02 report §2). The pinned GPL engine file
(EngineMkI.cpp) is the semantic specification only; no bytes are copied
(NUM-001). The two ROMs are `$readmemh`-loaded from
`reference/tables/sinlog_table.hex` and `reference/tables/sinexp_table.hex`
— the SAME bytes the Python model pins by SHA-256 in
`reference/tables/manifest.json` (sinlog `990c19e9…`, sinexp
`e515a71a…`; `tools/gen_tables.py --check` keeps them byte-pinned), and
`tests/test_h01.py` re-verifies the manifest before anything else runs.

Per-evaluation arithmetic (one eval = one `mkiSin` call, then the 32-bit
wrapping phase advance, then the NUM-010 output conversion):

```
phi       = phase[27:12]                              (uint16 truncation)
expVal    = sinLog(phi) + env (mod 2^16)              (log-domain level add)
isSigned  = expVal[15]; expVal &= 0x7FFF
result    = 4096 + sinExpTable[(expVal & 0x3FF) ^ 0x3FF]
result  >>= expVal >> 10
out       = isSigned ? (-result - 1) << 13 : result << 13      (int32)
val       = out >>> 4
clip_val  = val < -2^24 ? +32768 (pinned quirk) : val >= 2^24 ? +32767
          : val >>> 9
phase     <= phase + freq (mod 2^32)
```

**Structure** — the `ladder_dp_n.v` family pattern in its single-state-set
case (NCH = 1): one datapath, all arithmetic in one sequential core, state
explicitly registered, one evaluation accepted per clock. Pipeline (3
register stages, throughput 1 eval/clk, latency 3):

| Stage | Combinational content | Registers |
|---|---|---|
| 1 | phi fold (mirror XOR + quadrant sign) + sinlog ROM read, env capture | `s1_log[15:0]`, `s1_env[15:0]`, `s1_valid` |
| 2 | log-domain level add (mod 2^16) + sign split + sinexp ROM read + shift capture | `s2_exp[15:0]`, `s2_shift[4:0]`, `s2_sign`, `s2_valid` |
| 3 | 4096 bias + `>>shift` + sign reconstruct (`~result<<13`) + `>>>4`/clip/`>>>9` | `out_q[31:0]`, `clip_q[16:0]`, `out_valid_q` |
| — | phase register: `phase <= ld_phase ? phase_in : phase + freq` | `phase_q[31:0]` |
| — | observability: registered XOR reduction of all state bits | `obs_q` |

Handshake: `en=1` accepts one evaluation; its result appears with
`out_valid` exactly 3 cycles later, in acceptance order; `busy` covers
acceptance + in-flight stages; `ld_phase` (used while `en=0`) loads the
phase register for directed corner sequences. Both raw `out_raw[31:0]`
and `clip_val[16:0]` (17-bit two's complement; the negative-overflow
quirk value appears as +32768) are output ports, and `state_obs` folds
every state bit — **observability is load-bearing**, as in H02: the strip
control below proves synthesis removes everything when nothing is
observable. What the pattern's NCH = 2/4/8 dimension (state-set indexing
over 96 voice×operator states) becomes for this core is the shared-core
step that H02's access schedule and H03's sequencing own; this issue's
non-goals list says the same.

## 2. Equivalence to the frozen model (bit-exact, no tolerance)

`tools/h01_compare.py` generates a directed vector set from the PYTHON
model (the golden), runs `rtl/tb_operator_probe.v` under iverilog with the
same vectors, and compares expected vs actual **bit-exact** — any
mismatch is an RTL defect to fix, never a budget to loosen
(AGENTS.md honesty rules).

**Result: PASS — 4,794 evals, 0 mismatches** (issue requires ≥ 2,000),
plus 26 directed phase loads. Vector composition and coverage
(coverage reported separately from agreement):

| Coverage dimension | Measured |
|---|---:|
| Evals (issue minimum 2,000) | 4,794 |
| Distinct sinlog ROM addresses exercised | 1,024 / 1,024 |
| Distinct sinexp ROM addresses exercised | 1,024 / 1,024 |
| Negative outputs | 1,359 |
| Level 0 / level = ENV_MAX (16384) / 16-bit levels above ENV_MAX | 25 / 26 / 594 |
| Exact 2^24 cycle-boundary crossings (incl. the 0xFFFFFF→0 corner) | 1,007 |
| Evals at register phase 0xFFFFFFFF (32-bit wrap corner) | 4 |

Directed runs: a half-cycle-per-eval sweep that wraps the 32-bit register
exactly once; a one-index-per-eval walk covering every sinlog ROM address
and every quadrant mirror/sign transition; directed wrap corners
(0xFFFFFFFC→0, 0x00FFFFFC→0x01000000, negative-direction wrap, phi
0xFFFF→0x0000); level sweeps at fixed phase with sign-boundary crossings
computed from `sin_log_lookup`; five real N02 frames rendered through
`MarkIOperator.render_frame` (the `dgain` intra-frame ramp, gain advanced
before use, incl. a falling ramp and wrap frames) — the model's frame
functions generate the golden gains; and a fixed-seed pseudo-random block
(`random.Random(0xC0FFEE)`). Generation is deterministic — byte-identical
across runs, enforced by test and by the committed artifacts.

**Negative control (issue #20): MET.** Rebuilding with
`-DMUTATE_ADDRWIDTH` (sinlog table address truncated 10 → 9 bits, an
`ifdef` twist in the RTL) produces **2,220 / 4,794 mismatches** — the
harness demonstrably fails the mutated operator, with first-mismatch
localization (eval index, phase-before, freq, env, expected vs actual
out/clip). The equivalence suite therefore has resolution; a passing
mutant would fail the gate.

Tool identity (sim): Icarus Verilog 13.0 (stable) (`iverilog -g2012`),
vvp, golden = `src/gf180_dx7/model/operator.py` on CPython 3.x — hashes of
RTL, vectors, and expected are in
`evidence/h01-operator-probe/compare_report.json`.

## 3. Mapped result (measured, real cells)

Flow (per issue #20): `read_verilog; hierarchy -check -top
operator_probe; synth -top operator_probe; ltp; dfflibmap -liberty <7t>;
abc -liberty <7t>; stat -liberty <7t>`.

| Item | Value | Class |
|---|---|---|
| Yosys | 0.69+post, git sha1 `143eb14f9cc…`, native macOS (same install H02 measured) | measured identity |
| Liberty | `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, ciel commit `54435919ab…`, sha256 in the evidence JSON | measured identity |
| Constraints | none (no SDC, no clock — probe flow per the issue); `DONT_USE` policy: none, `*_1` cells allowed (declared per D01 §4/§10.3) | declared |
| Mapped flops | **99 × `dffq_1`** = 6,302.42 µm² (13.43 % of chip) | measured |
| Total mapped cells | 2,407 (27 distinct cells; leading: nand2 596, nor2 248, nand3 201, aoi21 184, oai21 181) | measured |
| Chip area | **46,939.96 µm² = 0.047 mm²** all-in (flops + both ROMs-as-logic + datapath + clip) | measured |
| ROM representation | **logic** — both 1024×16 ROMs dissolve to combinational gates (yosys MEMORY_MAP; no BRAM exists in this flow); final stat shows no memory cells. Consistent with the family's ROM-as-logic anchor (D01 §5: ≈ 1.9–2.2 µm²/stored bit ⇒ 32768 table bits ≈ 0.062–0.072 mm² derived): this probe's *whole* datapath mapped to 0.047 mm², i.e. abc's gf180 mapping came out denser than the sibling anchor's table implementations. The anchor stays the planning number; this is one new datapoint, not a refutation. | measured / derived comparison |
| Declared vs mapped state | 139 declared register bits → 99 mapped flops. The difference is expected and behavior-preserving: yosys exploits ROM-content constants (sinlog entries never set bit 15) and the sign-extension redundancy of `out_q`/`clip_q`, replacing redundant flops with wires. The contract the mapping must preserve is RTL *behavior* — proven bit-exact to the frozen model (§2) on the pre-synthesis RTL. | measured + documented derivation |

Gate results (issue #20 acceptance):

- **Full build: PASS** — mapped flops 99 > 32 (the irreducible 32-bit
  phase-accumulator floor: each phase bit has a distinct next-state
  function and feeds the ROM address and observability fold), chip area
  > 0, sequential area > 0, abc actually mapped (2,407 cells). A build
  with outputs optimized away would land at 0 flops / 0 area and FAIL.
- **Strip-observability negative control: MET** — with
  `-DH01_STRIP_OBSERVABILITY` (all outputs tied off) yosys deletes every
  state flop and cell (0 / 0, `yosys_strip.log`), so the flop gate fails
  exactly as the "optimized-away build" acceptance demands. Both
  directions run live in `tests/test_h01.py`.

Delay — what yosys gives, labelled honestly:

- **No STA exists in this flow, and abc's delay-driven mapper prints no
  achieved-delay number** — no nanosecond timing is claimed from any
  source.
- Recorded hints only: the flop-to-flop structure is the 3 documented
  pipeline stages (§1), and yosys `ltp` after `synth` reports a
  generic-gate logic depth of 195 (2-input-gate levels; includes the
  phase-adder feedback traversal and emits loop warnings — a hint, not a
  timing number).
- Whether 24.576 MHz closes at the ss corner remains **unmeasured**
  (D01 §2 records the same open item); timing closure is H10's
  measurement, in the ORFS flow, at the chosen corner.

## 4. Cycles/operator and the N01 schedule (the H03 schedule input)

N01 §schedule arithmetic (spec/numeric-profile-v1.json `schedule`;
DECISIONS-NUMERIC-v1 §7): 4,608,000 operator evals/s = 96 per sample;
**256 clk/sample at 12.288 MHz ⇒ all-in budget 2.67 clk/op-eval**;
**512 clk/sample at 24.576 MHz ⇒ 5.33 clk/op-eval**; design point
24.576 MHz, with 12.288 MHz viable only if the shared core sustains
≤ 2 clk/eval.

| Kernel measurement (this probe) | Value |
|---|---:|
| Sustained throughput | **1 clk / operator evaluation** |
| Pipeline latency | 3 clks |
| Kernel share of the 256-clk sample budget (12.288 MHz) | 96 of 256 → **1.67 clk/eval left** for state access + control + output |
| Kernel share of the 512-clk sample budget (24.576 MHz) | 96 of 512 → **4.33 clk/eval left** |
| Mapped kernel cost | 0.047 mm² (+ H02's storage probe: 4.115 mm² state machinery) |

Reading against the schedule honestly: the probe removes the arithmetic
kernel from the risk list — the multiply-free Mark I composition (two
parallel ROM reads + add + shift) really does sustain 1 eval/clk, better
than the 2–3 clk/eval N01 §7 assumed for it. What the probe does **not**
measure is everything around the kernel: reading/writing one 325-bit
operator bank + one 446-bit note bank per eval slot (H02 §4's access
pattern), the envelope/freqlut/LFO control share, and output conversion.
So the 12.288 MHz all-in question (≤ 2.67 clk/op-eval) stays open with
1.67 clk/eval of measured headroom to fill — H03's scheduler decides it
with real numbers; the 24.576 MHz design point has 4.33 clk/eval of
headroom and remains the plan default. Nothing here makes the projected
per-frame schedule impossible even optimistically — the issue's stop rule
does not trigger; no finding to escalate.

## 5. What this does NOT say

- **No place-and-route, no timing closure, no utilization, no
  routability, no PDN/IR claim of any kind** — no clock constraint was
  applied; no nanosecond number exists in this report. Clock closure at
  12.288/24.576 MHz is H10's measurement.
- **No full-chip fit claim** — 0.047 mm² is one single-operator kernel;
  the chip needs the kernel × state (H02), scheduling, mixing, I2S,
  control and pads. Nothing here says the core fits a quarter slot or a
  TT tile group.
- **No claim about the shared 96-state core** — this is the NCH = 1
  kernel: its state is one operator's phase register. Per-state storage,
  bank access and the N-way interleaved schedule are H02/H03's to
  measure and build.
- **No post-synthesis netlist re-simulation** — the equivalence proof
  (§2) is on the pre-synthesis RTL, which is the artifact the mapping
  must preserve; yosys's mapping correctness is trusted at probe stage.
  A gate-level netlist simulation or formal LEC against the RTL would
  strengthen this and is recorded as future work, not claimed.
- **No original-DX7 fidelity, preset-quality, or musical-usefulness
  implication** — bit-exactness to the frozen model is the bar here;
  model-vs-pinned-reference is N02's measured-exact budget, and
  soundness is listening records only (AGENTS.md: the three claims stay
  separate).
- CI re-runs without iverilog/vvp/yosys/the local ciel install are
  guarded skips (NOT_RUN, naming the missing path), never silent passes.

## 6. Uncertainties

1. The mapped flop count (99) is yosys-version-dependent: it depends on
   how much ROM-constant/sign-extension redundancy the optimizer
   exploits. The acceptance gate is therefore a principled floor
   (> 32, the irreducible phase accumulator) plus area > 0 plus the
   strip control at 0 — not a brittle exact-count equality like H02's
   (H02's state image has no logic structure to exploit; this datapath
   does).
2. `ltp`'s 195-level generic depth is a structural hint: it traverses
   abc-internal gate names, includes the phase feedback path, and emits
   loop warnings. It is not a delay and must not be quoted as one.
3. Single corner (`tt_025C_5v00`, 5 V) and single policy run; D01
   measures ±29–40 % DONT_USE-policy area sensitivity on sibling designs
   and TT hardens at 3.3 V — neither variation is explored here.
4. `env` inputs above ENV_MAX (594 vectors) exercise the model's uint16
   semantics; the real envelope (N03) supplies 14-bit gains only. The
   extra coverage is deliberate (adder-wrap corners), not a claim that
   the envelope produces such values.
5. Equivalence coverage is measured per-ROM-address (1024/1024 both
   tables) plus directed corners; it is not a formal proof — a
   pathological input outside the vector set could in principle diverge,
   though the datapath has no state outside the 139 declared bits.
6. Evidence was produced on the recorded local tool/PDK install;
   re-running elsewhere must re-record tool identities with the report.

## 7. Evidence and reproduction

| Artifact | Identity |
|---|---|
| `evidence/h01-operator-probe/compare_report.json` | equivalence + negative-control gates, vector coverage stats, tool identities (sha256s inside) |
| `evidence/h01-operator-probe/vectors.txt` / `expected.txt` | committed golden vector set (deterministic regeneration is test-enforced) |
| `evidence/h01-operator-probe/actual.txt` / `actual_mutant.txt` | RTL outputs, real build and MUTATE_ADDRWIDTH control |
| `evidence/h01-operator-probe/yosys_full.log` | full mapped transcript (ltp + stat inside) |
| `evidence/h01-operator-probe/yosys_strip.log` | strip-mode transcript — 0 cells |
| `evidence/h01-operator-probe/synth_report.json` | gates, runs, declared-vs-mapped accounting, tool/liberty/RTL identities |

Reproduce:

```sh
python3 tools/h01_compare.py                # golden gen + iverilog sim + bit-exact compare + mutant control
python3 tools/h01_synth.py                  # yosys full + strip + gates + report
python3 -m unittest tests.test_h01 -v       # all gates, controls, determinism
```

Statuses: equivalence **PASS** (bit-exact, 4,794 evals, 0 mismatches);
address-width negative control **MET** (2,220 mismatches on the mutant);
synthesis full gate **PASS** (99 flops, 0.047 mm², all outputs
observable); strip control **MET** (0 cells); P&R, timing closure,
post-synthesis netlist re-simulation, shared-core schedule, CI re-run:
**NOT_RUN / out of scope by design**.
