# DR-0012: exp() term-width refreeze — the frozen core becomes the widened-`exp_t*` revision

- **DR number:** 0012
- **Status:** Proposed
- **Date:** 2026-09-25
- **Deciders:** loom-builder (agent), pending owner ratification (ratification
  is a merge gate for this PR — see Decision 4)
- **Related issues:** #98 (this refreeze; escalation from #86), #86 (the range
  proof and the synthesis finding), #96 (the Icarus all-x shadow, same root
  cause), #32 (H10 mapped feasibility, whose figures this makes STALE)
- **Relation to DR-0011:** extends and supersedes its **core pin** and its
  Consequence (d). Every other decision of DR-0011 (single-driver merge, claim
  boundary, evidence hygiene) stands unchanged.

## Context

1. **What DR-0011 left open.** DR-0011 Context 5 recorded, as a finding it
   deliberately did not fix, that `rtl/dx7_core.v` reads `exp_t3[85:30]`,
   `exp_t4[85:30]` and `exp_t5[81:26]` from wires declared `[70:0]`, `[70:0]`
   and `[65:0]`. It noted that the synthesis toolchain resolves the missing
   bits to `undef` with only a warning, and that no gate in the sim-only chain
   observes this class of divergence. Its Consequence (d) forbade *assuming*
   those bits were zero or constant, pending a mapped result.

2. **What the mapped result was (issue #86, `docs/EXP-RANGE-PROOF-86.md`).**
   Both halves of the open question were answered, and the answer was worse
   than either candidate:

   - The bits **are** 0 in exact arithmetic. `tools/exp_range_proof.py --mode
     static` derives an unconditional post-reset inductive bound from the RTL
     text: the exact products need 68/60/52 bits against 71/71/66-bit wires, so
     every bit at or above the wire width is 0 in every post-reset state. The
     exhaustive sweep over all 16,777,217 clipped `exp_y` values agrees
     (66/58/49 bits reachable). Verilator, driving the real core, returns
     exactly those values at 64/64 probe points.
   - The pinned flow does **not** tie just those bits to 0. The yosys frontend
     makes them constant `x` inside the addends; the first `opt_expr` (inside
     `proc`, where `synth -run :fine` begins) folds each `$add` with an `x`
     operand bit to an **all-x** result — IEEE 4-state `+`, applied even with
     `-keepdc` — so `exp_hsum` is `x` long before `setundef -zero` (pinned
     `flow/scripts/synth.tcl` line 252) turns it into the constant **0**.
   - Consequence in the mapped netlist: `exp_hsum = 0` ⇒ `exp_rp = 0` ⇒
     `exp_res = 0` ⇒ `exp_pt = 0` ⇒ `exp_li = w_level`. A full pinned
     `run-orfs.sh dx7core FLOW_VARIANT=natdie synth` produced a netlist with
     **0** flops for every exp datapath register (`exp_y` … `exp_pt_q`); only
     the FSM survived. **Every operator whose patch has nonzero LFO AM
     sensitivity gets its level with no AM attenuation**, while the frozen
     model and the Verilator-simulated core do apply it — including at AM
     depth 0, where `exp_x = 0` still yields `exp_pt ≈ 2^17.6`.

   That is a violation of claim (1) of `CLAUDE.md` (the RTL matches the frozen
   integer model), extended to the netlist the frozen RTL synthesises to. It is
   also the root cause of #96: Icarus returns `x` for `exp_hsum` for the same
   IEEE reason, so the "canonical shadow" could not render those cases.

3. **Why a simulator-green core could hide it.** Verilator (with the H07 flags)
   evaluates an out-of-range read as 0 and never propagates `x` out of the
   adder, so all 34 H07 batteries stayed bit-exact on a core whose mapped form
   had lost a whole arithmetic unit. Neither bit-exactness nor a clean
   `check -assert` is evidence about this class. Only the synthesis path in the
   evidence loop — or a structural check of the RTL text — can see it.

4. **The change.** `rtl/dx7_core.v` lines 953-955 (DR-0011 pin) become:

   ```verilog
   wire [85:0]  exp_t3  = exp_p3 * 30'h2AAAAAAA;      // p3/6,  Q0.62
   wire [85:0]  exp_t4  = exp_p4 * 28'h0AAAAAAA;      // p4/24, Q0.60
   wire [81:0]  exp_t5  = exp_p5 * 26'h02222222;      // p5/120, Q0.58
   ```

   plus an explanatory comment. The `exp_hsum` expression, the reads, the
   pipeline, the FSM and every other line of the core are untouched. By
   Context 2's bound the added high bits are 0 in every reachable state, so
   **no simulated value changes** — that is the property, not an assumption,
   and it is re-measured in Evidence rather than inferred.

## Decision

1. **The frozen core is re-freezed** to the widened-`exp_t*` revision:

   | file | sha256 |
   |---|---|
   | `rtl/dx7_core.v` | `f33cecbd138aea2a869cb502dca9273b342c8f97be51c9a1e587f2eb4aba89fa` |
   | `rtl/env_unit.v` | `d91090cd473a4764179b87331b028dc2622e7677145e7519200fffca91f2aa28` (unchanged) |
   | `rtl/alg_router.v` | `dda373aa918c0d6b5248b27902b8a127143daea28e69b7b63739779a7b2c467d` (unchanged) |

   From merge of this PR that trio is "the frozen core" for all downstream
   claims. The DR-0011 pin `34f93d2d…` stands as the historical record of the
   single-driver refreeze only, and the H07/H08 pin `335599ea…` before it.
   Downstream pins updated in this PR: `tests/test_h08.py` CORE_SHA,
   `docs/H08-PIN.md`, `docs/reuse/catalog.json`, `tools/exp_range_proof.py`.

2. **DR-0011 Consequence (d) is discharged and replaced.** The `exp_t*`
   out-of-range select bits are no longer synthesis-resolved `undef` pending a
   result: there are no out-of-range bits. The standing rule that replaces it
   is Decision 3.

3. **A read past a declared width is a defect in this repository, not a
   style issue** — even where the missing bits are provably 0, because the
   synthesis frontend does not evaluate them the way the simulators do. The
   rule is enforced structurally, not by convention:
   `tests/test_exp_range_proof.py::NoOutOfRangeSelects` scans `rtl/dx7_core.v`
   for any `name[hi:lo]` whose `hi` exceeds the declared width of `name` and
   fails on the first one, in the fast lane, with no simulator or synthesis
   tool required. Its own negative controls re-narrow the three term wires to
   their DR-0011 declarations (must be caught) and narrow an unrelated
   part-selected wire (must also be caught, so the guard is not special-cased).

4. **Ratification gate.** This DR is *Proposed* by the builder. Re-freezing
   changes what "the frozen core" means repository-wide, so **operator
   ratification is required before merge**; until then this PR holds at judge
   approval. This is the mechanism DR-0011 itself used (its Decision 3).

5. **Claim boundary of this PR.** It establishes, for the refrozen revision
   `f33cecbd…` itself: simulation bit-exactness against the frozen model (H07
   battery, fresh), byte-identical captured PCM versus the DR-0011 pin on
   every case of that battery, cone-level formal agreement between the
   synthesised and the simulated `exp_hsum` under the pinned image's yosys,
   and — in the same pinned yosys, on the whole core — an `exp_hsum` driven by
   a real adder cell instead of a constant `x`.

   It does **not** establish a mapped-netlist census on this exact pin. That
   run is **BLOCKED on this host** (Evidence row F): the pinned flow's
   `do-yosys` needed 11.4 GB on the successful #86 widened run, the Docker VM
   here offers 7.75 GB total with ~3.8 GB already resident, and the attempt
   was killed inside `techmap` at 5.2 GB. The standing pinned-flow mapped
   census for the widening is #86's, made on a scratch copy that differs from
   this pin only in comments (`evidence/issue-86-exp-range-proof/orfs-census-
   widened-2026-09-25.json`: 564 exp datapath flops vs 0 as-written). That is
   evidence about the *widening*, not about this file's hash, and it is
   reported that way: the census on the refrozen pin is NOT_RUN here and is
   required together with the H10 re-run (Decision 6, and the follow-up issue
   named in Consequences).

   It claims **no timing, no fit, no placement, no routing, no signoff, no
   silicon and no musical result**. The synthesis-stage cell-area delta quoted
   in Evidence is synth-stage cell area from #86's runs only; there is no
   placement or timing behind it.

6. **H10 is STALE, not corrected here.** Every figure in
   `docs/H10-GF180-FEASIBILITY.md` was measured on a netlist missing the exp
   datapath, so its areas and instance counts are **lower bounds** for the
   refrozen core, not its size. The report is generated from
   `evidence/h10-gf180/bundle.json` and freshness-tested, so it is not
   hand-edited: this PR adds a `stale` record to the bundle and regenerates
   the report, which now carries the supersession banner. Its section 6 bullet
   ("the mapped netlist ties those bits to constant 0") is superseded by
   Context 2 and by the same banner. The direction of H10's verdict cannot
   improve by adding logic, so no conclusion is reversed — but the numbers
   must not be cited as the refrozen core's size until the H10 run is redone
   on this pin.

## Consequences

- **Enables:** a mapped netlist whose exp() unit exists, so any future
  gate-level or PnR work is done against a core that computes what the frozen
  model computes; #96's Icarus shadow, which the same fix resolves.
- **Forbids:** (a) citing the `34f93d2d…` pin as the current core after merge;
  (b) citing any `docs/H10-GF180-FEASIBILITY.md` figure as the refrozen core's
  area, instance count or utilisation until H10 is re-run on this pin;
  (c) reintroducing any part-select that exceeds its declared width, on the
  strength of a range proof or otherwise (Decision 3);
  (d) treating simulator bit-exactness, or a clean `check -assert`, as evidence
  about the mapped netlist's arithmetic (DR-0011's rule, now with a second,
  independent instance behind it).
- **STALE when this changes:** if the refrozen RTL is edited again, the H07
  battery must be re-run fresh (no mixing of partial artifacts — DR-0011 row I
  is the standing negative example), the cone LEC and the mapped-netlist
  census re-run, and the H08/H10 pins re-pinned in the same PR.
- **Costs:** the exp datapath is real logic that the as-written core was not
  paying for. The synthesis-stage cell-area increase is recorded in Evidence
  and is a correction of an undercount, not a regression.
- **Open, tracked, not assumed:** the pinned-flow mapped census on this pin and
  the H10 re-run on top of it are **issue #103** (blocked on a synthesis host
  that can give the container ≥ 12 GB — Evidence row F). Until #103 lands,
  every H10 figure stays a lower bound and no mapped-netlist number may be
  attributed to `f33cecbd…`.

## Evidence

All runs on `repo-local-gf180-dx7` (Apple Silicon; the pinned image runs under
`linux/amd64` emulation), 2026-09-25. Input core
`f33cecbd138aea2a869cb502dca9273b342c8f97be51c9a1e587f2eb4aba89fa` unless
stated. Retained under `evidence/issue-98-exp-refreeze/`.

Full per-row detail, artifacts, tool hashes and the two negative-control runs:
`evidence/issue-98-exp-refreeze/README.md`. Row letters below are that file's.

| row | check | tool / binary | result |
|---|---|---|---|
| A | static range proof | `tools/exp_range_proof.py --mode static` | **PASS** — no out-of-range bits remain; exact products 68/60/52 bits in 86/86/82-bit wires, headroom 18/26/30 bits |
| B | probe cross-check, Verilator 5.052 | `--mode xcheck --sim verilator` | **PASS**, 64/64 points, 0 unknown |
| C | probe cross-check, Icarus 13.0 | `--mode xcheck --sim iverilog` | **PASS**, 64/64 points, **0** unknown — was `NO_VERDICT` with 64/64 unknown on the DR-0011 pin (#96's root cause) |
| D | Verilator lint | `verilator --lint-only -Wall` | **PASS** — 0 `SELRANGE` (188 warnings total; 3 `SELRANGE` traded for 3 `UNUSEDSIGNAL` on the never-read low bits, no waiver added) |
| D′ | lint control at DR-0011 widths | same | **FAIL as required** — 3 `SELRANGE` |
| E | cone LEC + whole-core driver, pinned image `sha256:ebc8142da6d6…`, `Yosys 0.68+post` | `tools/exp_hsum_lec.py` | **PASS**, exit 0: SAT-equivalent for every input; whole-core `exp_hsum` driven by an `$add` cell, not `69'x` |
| E′ | the same tool on the DR-0011 file (required failure control) | `--rtl build/i98/dx7_core_dr0011.v` | **FAIL as required**, exit 1: mapped cone constant `57'h0`, driver tied to `69'x` |
| E″ | built-in live controls on every run | idem | widened **PASS**; narrowed (DR-0011 widths) **FAIL**; gold with one out-of-range bit forced to 1 **FAIL**; `controls_behaved: true` |
| F | pinned-flow **mapped census on this pin** | `run-orfs.sh dx7core natdie synth` | **BLOCKED** — yosys `SIGKILL`ed inside `techmap` (exit 247 = `sys.exit(-9)`), peak RSS 5.2 GiB, Docker VM 7.75 GiB with ~3.8 GiB resident elsewhere; #86's successful widened run needed 11.4 GiB. Reproduced twice. Tracked as #103 |
| F′ | standing pinned-flow census **for the widening** (#86, scratch copy differing only in comments) | `tools/exp_netlist_census.py` | as-written **0** exp datapath flops / 32,941,030.7 µm²; widened **564** flops / 34,839,310.1 µm² (+5.8 % synth-stage cell area) |
| G | H07 battery, fresh, Verilator | `tools/h07_compare.py --set both --tag dr0012-p1` | **PASS 34/34**, `overrun`/`overflow` 0 everywhere, every latency inside the contracted window |
| G′ | same battery vs the DR-0011-pin baseline run | per-case `actual_sha256` | **34/34 byte-identical** — no simulated value changed (this issue's stop/escalate condition, measured) |
| H | H07 Icarus on the two AMS ≠ 0 cases (`dev32-06`, `dev32-30`) | `--tool iverilog --tag dr0012-iv-ams` | see README row H; the full 34-case Icarus shadow on this pin is **NOT_RUN** |
| I | cone LEC under the host `Yosys 0.69+post` (tool-independence only) | `tools/exp_hsum_lec.py --yosys …` | see README row I |
| J | structural guard, fast lane, no tools | `tests/test_exp_range_proof.py::NoOutOfRangeSelects` | **PASS**, with both negative controls failing as required (DR-0011 narrowing; an unrelated narrowed part-selected wire) |

## Reproduce

```sh
# Rows A-D, J -- seconds to minutes, no docker.
python3 tools/exp_range_proof.py --mode static
python3 tools/exp_range_proof.py --mode xcheck --sim verilator
python3 tools/exp_range_proof.py --mode xcheck --sim iverilog
verilator --lint-only -Wall -Wno-fatal -Irtl \
    rtl/dx7_core.v rtl/env_unit.v rtl/alg_router.v   # 0 SELRANGE
python3 -m unittest tests.test_exp_range_proof -v

# Rows E, E', E'' -- the pinned image's yosys, ~20 min each under emulation.
python3 tools/exp_hsum_lec.py                     # exit 0 expected
mkdir -p build/i98
git show <dr0011-commit>:rtl/dx7_core.v > build/i98/dx7_core_dr0011.v
python3 tools/exp_hsum_lec.py --rtl build/i98/dx7_core_dr0011.v   # must FAIL (exit 1)

# Rows G, G' -- the fresh battery, ~65 min with --jobs 4.
python3 tools/h07_compare.py --set both --tool verilator --tag dr0012-p1 --jobs 4
# then compare per-case actual_sha256 against the DR-0011-pin baseline
# evidence/h07-core/results-h10-merged-p3.json (expect 34/34 identical).

# Row H -- Icarus, tens of minutes PER CASE on an Apple-Silicon host.
python3 tools/h07_compare.py --set dev --cases dev32-30,dev32-06 \
    --tool iverilog --tag dr0012-iv-ams

# Row F -- NEEDS >= 12 GB available to the container (see issue #103).
# On a host that cannot give it that, this is killed inside techmap (exit 247)
# and must be reported BLOCKED, never FAIL and never skipped-as-pass.
ORFS_WORK=/tmp/w ./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=natdie synth
python3 tools/exp_netlist_census.py /tmp/w/results/gf180/dx7core/natdie/1_2_yosys.v \
    --stat /tmp/w/reports/gf180/dx7core/natdie/synth_stat.txt
```
