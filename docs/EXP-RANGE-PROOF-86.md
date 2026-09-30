# exp_t3/4/5 out-of-range selects: simulated core vs mapped netlist (issue #86)

> **Resolved by DR-0012 (issue #98), 2026-09-25.** This document records the
> FAIL as measured on the DR-0011 pin `34f93d2d…`; it is the historical
> finding, not the current state of the core. The three wires are now
> declared `[85:0]`/`[85:0]`/`[81:0]`, the reads are in range, and the
> repaired result is recorded in
> `docs/decision-records/0012-exp-term-width-core-refreeze.md` with its own
> evidence under `evidence/issue-98-exp-refreeze/`. The controls in section 6
> stay live: the tools reproduce this FAIL on demand against a scratch copy
> at the DR-0011 widths. **Every command in section 7 that omits `--rtl` now
> runs against the repaired core and is expected to PASS** — see section 7.

**Verdict (on the DR-0011 pin): FAIL. The two disagree, and the issue's
stop/escalate condition applies.** The out-of-range bits are 0 in exact arithmetic for every
reachable operand, and the Verilator-simulated core evaluates them as 0.
Synthesis does something else. The pinned flow does **not** tie just those
15/15/16 bits to 0. Before `setundef -zero` runs, yosys applies IEEE 4-state
arithmetic: one x bit in an addend makes the whole sum x. So all of
`exp_hsum` becomes x, and then the constant 0. The mapped netlist has lost
the whole `exp()` datapath. On the gate level, every operator with nonzero
LFO amplitude-mod sensitivity gets **no** AM attenuation. The simulated
core, and the frozen model it matches, do apply it.

- Scope: `rtl/dx7_core.v` at the DR-0011 pin
  `34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58`
  (re-hashed for every run below).
- Claim class: claim (1) of `CLAUDE.md` (the RTL matches the frozen integer
  model), extended to the synthesised netlist. Nothing here concerns the
  software reference or sound quality.
- Evidence: `evidence/issue-86-exp-range-proof/`. Tools:
  `tools/exp_range_proof.py`, `tools/exp_hsum_lec.py`,
  `tools/exp_netlist_census.py`. Fast-lane test:
  `tests/test_exp_range_proof.py`.

## The code in question

`rtl/dx7_core.v:953-961`:

```verilog
wire [70:0]  exp_t3  = exp_p3 * 30'h2AAAAAAA;      // p3/6,  Q0.62
wire [70:0]  exp_t4  = exp_p4 * 28'h0AAAAAAA;      // p4/24, Q0.60
wire [65:0]  exp_t5  = exp_p5 * 26'h02222222;      // p5/120, Q0.58
wire [56:0]  exp_hsum = (57'd1 << 56)
    + {1'b0, exp_xq}
    + {13'b0, exp_p2[44:1]}
    + {13'b0, exp_t3[85:30]}        // bits 85:71 do not exist
    + {13'b0, exp_t4[85:30]}        // bits 85:71 do not exist
    + {13'b0, exp_t5[81:26]};       // bits 81:66 do not exist
```

Verilator lint of the core reports exactly these three `SELRANGE`s and no
others (`verilator-selrange-lint-2026-09-25.txt`). The pinned flow's
canonicalize log carries the matching three yosys warnings and no other
undef warnings.

## Summary of checks

| # | Check | Tool | Status |
|---|---|---|---|
| 1 | Range proof: the exact products fit their declared wires, so the out-of-range bits are 0 in exact arithmetic (acceptance path (a)) | `exp_range_proof.py --mode static`, unconditional | **PASS** |
| 1b | Same, measured over the whole clipped domain (16,777,217 points) | `exp_range_proof.py --mode sweep` | **PASS** |
| 2 | Verilator, running the real `dx7_core`: registers and `exp_hsum` vs the model (64 points) | `exp_range_proof.py --mode xcheck --sim verilator` | **PASS** |
| 2b | Icarus, same probe | `--sim iverilog` | **NO_VERDICT** (`exp_hsum` is x at 64/64 points; tracked in #96) |
| 3 | Formal equivalence of the synthesised `exp_hsum` cone vs the simulated-core cone, pinned yosys (acceptance path (b), scoped to the cone) | `exp_hsum_lec.py` | **FAIL**: mapped output is the constant 0 |
| 4 | Full core, pinned yosys, opening synth passes: what drives `exp_hsum`? | `exp_hsum_lec.py` | **FAIL**: tied to `69'x` |
| 5 | Full pinned ORFS `synth` of the core: exp-unit flops in the mapped netlist | `run-orfs.sh` + `exp_netlist_census.py` | **FAIL**: 0 datapath flops (see section 4) |
| - | Acceptance path (c): H07 battery on the gate-level netlist | - | **NOT_RUN**. Check 5 already shows the unit is absent, so there is nothing for it to run against |

## 1. The out-of-range bits are 0 in exact arithmetic (PASS)

### Static, unconditional bound

`tools/exp_range_proof.py --mode static` parses the RTL itself: the
declarations, every `<=` write to `exp_xq`/`exp_p2..p5`, the product wires
and the constants. It then derives a post-reset inductive invariant. Every
one of these registers is written only in two ways: the constant 0 at reset,
or one fixed slice of an unsigned product of already-bounded registers. The
parser refuses (NOT_RUN) on any other write form, so no write path can be
skipped silently.

| register | only non-reset write | invariant bound |
|---|---|---|
| `exp_xq` | `exp_xp[109:60]` (a 50-bit slice) | < 2^50 |
| `exp_p2` | `exp_p2w[100:56]`, where `exp_p2w = xq*xq` < 2^100 fits its 112-bit wire | < 2^44 |
| `exp_p3` | `exp_p3w[99:56]`, where `p2*xq` < 2^94 fits 100 bits | < 2^38 |
| `exp_p4` | `exp_p4w[87:56]`, where `p3*xq` < 2^88 fits 88 bits | < 2^32 |
| `exp_p5` | `exp_p5w[81:56]`, where `p4*xq` < 2^82 fits 82 bits | < 2^26 |

So `exp_p3 * 0x2AAAAAAA` < 2^68 (wire 71 bits, 3 bits headroom),
`exp_p4 * 0x0AAAAAAA` < 2^60 (71 bits, 11 spare) and
`exp_p5 * 0x02222222` < 2^52 (66 bits, 14 spare). In each case the wire holds
the exact product without truncation. Every bit at or above the wire width is
therefore 0 in exact arithmetic, and that includes all of the out-of-range
read bits. The bound does not depend on the FSM order, the `exp_y` clip or
the operand domain. It holds in every post-reset state.
(`static-proof-pass-2026-09-25.json`)

### Exhaustive sweep (tightness, cross-check)

`--mode sweep` transcribes the pipeline from `exp_y` to `exp_t3/t4/t5`
bit-exactly and runs it over every `exp_y` in `[0, 2^24]`. The largest exact
products need 66/58/49 bits, all at `exp_y = 11,946,402`, which is inside the
static bounds (68/60/52). Runtime was 705 s. (`exhaustive-sweep-pass-2026-09-25.json`)

## 2. What the simulators return

`evidence/issue-86-exp-range-proof/tb_exp_probe.v` instantiates the **real**
`dx7_core` and uses hierarchical `force` to drive its exp unit
(`exp_amd`/`exp_ams`/`exp_start`) at 64 points: 8 fixed, including the sweep
argmax, and 56 from an LCG. `--mode xcheck` checks the in-range values of
`exp_xq`, `exp_p2..p5` and `exp_t3/t4/t5` against the model. It also checks
`exp_hsum` two ways: against the model with the out-of-range bits = 0, and
against a model with the wires widened to exact products.

- **Verilator** 5.052, with the `tools/h07_compare.py:build_sim()` flags:
  64/64 match on all counts (`xcheck-verilator-2026-09-25.json`). Verilator
  evaluates the missing bits as 0, which section 1 shows is also the exact
  value. So the simulated core, and with it the H07 bit-exactness claim under
  Verilator, is consistent with the intended arithmetic.
- **Icarus** 13.0: every in-range value matches, but `exp_hsum` is x at 64/64
  points (`xcheck-iverilog-2026-09-25.json`). That is correct IEEE
  behaviour: an x operand bit makes the whole sum x. The consequences for the
  iverilog "canonical shadow" are tracked in **#96**. It has the same root
  cause as section 3, and the same fix resolves both.

## 3. What synthesis builds (FAIL)

The premise that "the mapped netlist ties those bits to 0" is false.

The yosys frontend turns the three missing ranges into constant x bits
inside the addends, `{13'b0, 15'bx, exp_t3[70:30]}`. The first `opt_expr`
then folds each `$add` that has an x-bit operand into an **all-x result**.
That is the IEEE 4-state semantics of `+`, and yosys applies it even with
`-keepdc`. `synth -run :fine` runs `opt_expr` as part of `proc`, which is
the first thing it does on the canonicalized design. So `exp_hsum` is x long
before `setundef -zero` (pinned `flow/scripts/synth.tcl` line 252) turns it
into the constant 0.

Evidence, all produced with the pinned image
(`openroad/orfs@sha256:ebc8142d…`) and its flow yosys `0.68+post` (sha256
`5cd52bc7…`, the binary the flow selects):

- **Check 3: cone equivalence (path (b)).** `tools/exp_hsum_lec.py` copies
  the `exp_hsum` assignment verbatim into a cone. Its inputs are
  `exp_xq`, `exp_p2` and `exp_t3/t4/t5` at their declared widths. The tool
  runs the cone through the pinned `synth.tcl` pass order up to `setundef
  -zero` and then generic `abc`. A SAT miter then compares the result with
  the gold cone, which uses explicit zeros for the missing bits (the
  simulated-core semantics).
  - **FAIL.** The mapped cone is `assign o = 57'h0`. Counterexample:
    gate `0x0`, gold `0x1000ffffffffffd`.
  - Positive control, the same wires widened to `[85:0]/[85:0]/[81:0]`:
    **PASS** (SAT: no model), so the harness can pass.
  - Failure control from the issue: one out-of-range bit of `exp_t3` set to 1
    in the gold copy: **FAIL**. Counterexample: gate `0x1ffe0ef520eca3f` vs
    gold `0x1ffe2ef520eca3f`, a difference of exactly one bit at position 41,
    where `exp_t3[71]` lands. The check resolves a single bit.
  (`exp-hsum-lec-pinned-2026-09-25.json`, cone sources in `lec-src/`)
- **Check 4: full core.** After the flow's canonicalize read and `proc`, the
  full `dx7_core` has `connect \exp_hsum $add$…:956$29131_Y [56:0]` and
  `connect $add$…:956$29131_Y 69'x`. Adds 29129 and 29130 are also `69'x`.
  With the three wires widened, `exp_hsum` stays driven by an `$add` cell
  (control).
- **Check 5: the real flow run.** `run-orfs.sh dx7core FLOW_VARIANT=natdie
  synth` ran for 32 min 33 s (exit 0). Its `1_2_yosys.v` (sha256
  `998a17ee…`) has **0** mapped flops for any exp datapath register:
  `exp_y`, `exp_t1`, `exp_arg`, `exp_zq`, `exp_k`, `exp_T`, `exp_xq`,
  `exp_p2..p5`, `exp_res` and `exp_pt_q`. The FSM (`exp_st`, 12 flops,
  recoded) and `exp_start` survive. Chip area is 32,941,031 µm²
  (`orfs-run-asis-2026-09-25.txt`, `orfs-census-asis-2026-09-25.json`).
  Section 4 covers the widened control run.

### What that does to the chip

`exp_hsum = 0` makes `exp_rp = exp_T * 0 = 0`, so `exp_res = 0`,
`exp_pt = 0`, `exp_sub = 0` and `exp_li = w_level` (`rtl/dx7_core.v:1049-1051`).
The core only enters the exp path when `n_ams[nn][opk] != 0`
(`rtl/dx7_core.v:2139`). So in the synthesised hardware, **every operator
whose patch has nonzero LFO AM sensitivity gets its level with no AM
attenuation.** The Verilator-simulated core and the frozen model do apply
it. The attenuation is nonzero even at AM depth 0: `exp_x = 0` still gives
`exp_pt ≈ 2^17.6`. So the difference is not limited to moving LFOs.
Operators with AMS = 0 do not use the path and are not affected.

## 4. Widened control run and area

To show the missing flops are *caused* by the out-of-range selects, the same
pinned flow was re-run on a scratch copy of the core. The only change is to
lines 953-955, which widen the three wires to `[85:0]/[85:0]/[81:0]`. Section
1 shows this changes no simulated value. The flow config differed only in the
RTL path (`orfs-run-widened-2026-09-25.txt`).

| run | canonicalize out-of-bounds warnings | exp datapath flops (`exp_y`..`exp_pt_q`) | chip area (µm²) |
|---|---|---|---|
| as written (DR-0011 pin) | 3 | **0** | 32,941,031 |
| widened scratch control | 0 | **564** (e.g. `exp_res` 62, `exp_zq` 66, `exp_p3` 39) | 34,839,310 |

The widened control keeps the unit, which also shows that the census regex
finds these flops when they exist. The difference, **+1,898,279 µm²
(+5.8 %)**, is roughly the area the as-written core loses to the folded exp
datapath under this flow. It measures synth-stage cell area only; there is
no placement or timing behind it. Any H10 figure taken from the as-written
pin undercounts by about that much.

The as-written run's 32,941,031 µm² is not the H10 report's `orfs_stock`
33,600,930 µm². That figure came from `tools/h10_synth.py`'s policy synth,
not from this `run-orfs.sh` flow run. This document makes no claim about
the gap between them.

## 5. Escalation (issue #86 stop/escalate)

The issue says: *"If the bits can be nonzero, the frozen core and the frozen
model may disagree with synthesised hardware. Stop and escalate before any
H10 re-run."* The literal trigger (the bits being nonzero) does not hold: they
are provably 0. The harm the condition guards against has happened anyway,
by a different route. **The frozen core, as synthesised by the pinned flow,
disagrees with the frozen model and with the simulated core.** This PR stops
there:

- **No RTL change here.** The candidate fix is to widen the three wires to
  `[85:0]/[85:0]/[81:0]`, or to zero-pad the reads explicitly. Section 1
  shows it does not change any value the simulated core produces. Check 3's
  positive control shows the widened cone synthesises to the gold function.
  It still re-freezes the core, so it needs a new DR (superseding or extending
  DR-0011) and a fresh H07 battery run under the DR-0011 STALE rule. Filed
  as **#98**.
- **No H10 re-run until that lands.** H10's mapped area figures
  (`docs/H10-GF180-FEASIBILITY.md`) were measured on a netlist without the
  exp datapath, so they undercount the core. The direction of H10's verdict
  (does not fit either die) cannot improve by adding logic. The numbers are
  lower bounds, not the core's size. That report is generated from its
  evidence bundle (`tools/h10_report.py`, freshness-tested), so it is not
  hand-edited here. Its section 6 bullet on this topic ("ties those bits to
  constant 0") is incomplete, and the correction belongs to the escalation
  issue's H10 re-run.
- The draft of this document had concluded that "`setundef -zero` reproduces
  the mathematically correct value". That was wrong, and this finding
  replaces it. #96's premise that "Verilator matches the mapped netlist" is
  wrong for the same reason. A correction was posted there.

## 6. Failure controls (all demonstrated)

| control | target check | result |
|---|---|---|
| `exp_p3 <= exp_p3w[99:50]` (mis-sized select) | static | FAIL: t3 needs 74 bits (`nc-p3-slice-widened.*`) |
| `exp_xq <= exp_xp[109:50]` (mis-sized upstream select) | static | FAIL (`nc-xq-slice-widened.*`) |
| `wire [63:0] exp_t3` (narrowed wire) | static | FAIL: 68 > 64 (`nc-t3-wire-narrowed.json`) |
| same narrowed wire, on the real core | Verilator xcheck | FAIL at all 64 points (`nc-t3-wire-narrowed-xcheck-verilator.json`) |
| `exp_p3 <= exp_p3w[99:56] \| (44'd1 << 42)` | static | NOT_RUN: parser refuses an unknown write form (`nc-p3-bit42-forced.*`) |
| stray bit 42 in `exp_p3` (model) | sweep | FAIL: t3 needs 72 bits (`sweep-nc-mutate-bit42-fail-2026-09-25.json`) |
| one out-of-range `exp_t3` bit set to 1 in the gold copy | cone LEC | FAIL (section 3) |
| wires widened | cone LEC / full-core driver / ORFS census | PASS / driven by a cell / exp flops present (section 4) |

The static-proof controls and the pinned-RTL bound stay live in
`make test-fast` (`tests/test_exp_range_proof.py`, about 2 s).

## 7. Reproduce

The core in the tree is the **repaired** one (DR-0012). These commands
therefore report the repaired state; the FAIL this document records is
reproduced by pointing the tools at a scratch copy at the DR-0011 widths.

```sh
# Repaired core (expected PASS):
python3 tools/exp_range_proof.py                          # static proof (<1 s)
python3 tools/exp_range_proof.py --mode sweep             # ~10 min
python3 tools/exp_range_proof.py --mode xcheck --sim verilator
python3 tools/exp_range_proof.py --mode xcheck --sim iverilog
python3 tools/exp_hsum_lec.py                             # pinned image via docker
ORFS_WORK=/tmp/w ./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=natdie synth   # ~35 min emulated
python3 tools/exp_netlist_census.py /tmp/w/results/gf180/dx7core/natdie/1_2_yosys.v \
    --stat /tmp/w/reports/gf180/dx7core/natdie/synth_stat.txt

# This document's FAIL, reproduced (expected FAIL, exit 1):
mkdir -p build/i98 && git show <dr0011-commit>:rtl/dx7_core.v > build/i98/dx7_core_dr0011.v
python3 tools/exp_hsum_lec.py --rtl build/i98/dx7_core_dr0011.v
```

`tools/exp_hsum_lec.py` also carries the DR-0011 widths as a built-in live
negative control on every run (`cone_lec_narrowed`,
`core_hsum_undef_narrowed`), so the failure stays demonstrable without a
scratch file.

Tool identities and input hashes: `tool-versions-2026-09-25.txt` (this
document's runs) and `evidence/issue-98-exp-refreeze/` (the repaired core's).

## 8. What remains unproved

- Check 3 proves non-equivalence of the `exp_hsum` cone only, not full-core
  equivalence. The full-core evidence (checks 4 and 5) is structural: the
  driver is x, and the flops are absent. That is enough to show disagreement.
  It says nothing about the rest of the core.
- No gate-level simulation was run (path (c)). After the fix, a gate-level
  or formal check that the exp unit **is** present and correct is still
  needed. Check 3's positive control covers only the widened cone.
- The static lint audit shows these are the only constant out-of-range
  selects. Other possible x sources, such as out-of-range variable indices or
  uninitialised reads that synthesis could fold the same way, were not
  audited.
- Whether the H07 corpus contains AMS ≠ 0 operators, and therefore whether
  a gate-level H07 run would have caught this, was not checked here. See #96.
