# DR-0011: Single-driver core refreeze — the frozen core becomes the refrozen single-driver revision

- **DR number:** 0011
- **Status:** Proposed
- **Date:** 2026-09-24
- **Deciders:** loom-builder (agent), pending owner ratification (ratification
  is a merge gate for this PR — see Decision 3)
- **Related issues:** #32 (H10, PR-A / core refreeze); supersedes the core pin
  of #30 (H08); evidence lives in `evidence/h10-legality/` and
  `evidence/h07-core/results-h10-merged-p3.json`

## Context

1. **The H10 requirement.** Issue #32 demands *mapped* (real-cell) feasibility
   results for this exact core on gf180mcu, through the pinned ORFS flow
   (`openroad/orfs:26Q3-296-gda37dce1c`, yosys 0.67, `2d1509d1b`, gf180 7t,
   `FLOW_VARIANT=natdie`). A core that the synthesis path refuses cannot be the
   frozen core for any mapped result.

2. **The failure of the frozen core.** The H07-frozen core
   (`rtl/dx7_core.v` = `335599eae6aada90f45c5738cf79d70098bb3a6cf97437927fcbdec10abfc917`,
   `rtl/env_unit.v` = `f61916cb6cc0c9b9434ff5007732e1636614c10450b0ec1a07d5f97592999e7b`,
   `rtl/alg_router.v` = `dda373aa918c0d6b5248b27902b8a127143daea28e69b7b63739779a7b2c467d`
   — the H07/H08 pin, PRs #73/#75/#76/#77) passed every simulation gate
   (34/34 batteries, bit-exact to the frozen model). Yet on the pinned flow it
   fails `do-yosys` (stage 1_2, full synthesis including gf180 mapping):
   final `check -assert` reports **"ERROR: Found 9 problems"** — nine
   `evq_cnt[0..8]` cells with multiple conflicting drivers
   (`_SDFFE_PP0P_` vs `_DFFE_PP_` Q ports); the string "multiple conflicting
   drivers" occurs 12,938 times across the full log, and no netlist statistics
   are produced (witness E, `evidence/h10-legality/witness-frozen-1_2-check.log`;
   full log sha256 `c646d1a6ef64085c60d52227fc2f74b840e2c7b3ad5d1403f098feabef071473`,
   5,080,150 B, lands in the H10 report bundle).

3. **Root cause.** In the merged H07 core, several registers are each written
   by two `always @(posedge clk)` blocks: in `dx7_core.v` — `shd_commit`,
   `evq_rp`, `evq_cnt`, plus `st_overflow` set from two blocks — and in
   `env_unit.v` — the slot registers (`level_q … down_q`) written both by the
   step FSM block and a second external slot-write block. Verilator and
   Icarus silently resolve multiple non-reset drivers by time-multiplexing the
   blocks in some fixed evaluation order, so **every simulator gate passed**;
   yosys rejects the netlist. A simulator-green core is not a frozen,
   synthesizable core.

4. **Supersession note (supersedes the 2026-09-23 "9 fatals" working note).**
   A 2026-09-23 working note recorded "9 fatals" on the frozen core at raw
   elaboration. This is superseded by witness B: raw host-yosys elaboration
   (Yosys 0.69+post, git `143eb14f9cc55d6f8927e68523b0c9d2166ed02c`) with
   `check -assert` on the same frozen revision reports **0 problems**
   (`evidence/h10-legality/witness-frozen-raw-check.log`, log hash
   `4c17d43339`). The nine problems exist only *after* mapping to gf180 cells
   (witness E); raw elaboration is clean. The two were conflated; the frozen
   core's elaboration was always clean and its failure is a mapping-stage
   multi-driver failure.

5. **Silent const-resolution finding (kept; the core argument).** The same raw
   elaboration (witness B) warns — warning only, not an error — at
   `rtl/dx7_core.v:1028-1030`:

   ```
   Warning: Range [85:30] select out of bounds on signal `\exp_t3': Setting 15 MSB bits to undef.
   Warning: Range [85:30] select out of bounds on signal `\exp_t4': Setting 15 MSB bits to undef.
   Warning: Range [81:26] select out of bounds on signal `\exp_t5': Setting 16 MSB bits to undef.
   ```

   The out-of-range `exp_t*` selects are **silently resolved to `undef`** by
   the synthesis toolchain with only a warning. No gate in the sim-only chain
   observes this class of divergence: a tool that zero-extends or constant-folds
   those bits would build different hardware while every simulator bit-exact
   test kept passing. This is why "the sim battery is green" can never by
   itself establish synthesis-path legality — the refreeze and its witness set
   exist precisely to put the synthesis path in the evidence loop. The refreeze
   does not alter the `exp_t*` bit widths; it records the finding so the H10
   report (PR-B) can close it with mapped, mapping-aware evidence rather than
   by assumption.

6. **The refrozen revision and its proof.** The single-driver merge
   (refrozen core: `rtl/dx7_core.v` =
   `34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58`,
   `rtl/env_unit.v` =
   `d91090cd473a4764179b87331b028dc2622e7677145e7519200fffca91f2aa28`;
   `alg_router.v` unchanged at the H07 pin; battery fingerprint of the three
   files `7bab93f514f49a3f6819818e66ed3e4581c00ebb0063082ef1e2b7f2f06bc188`):

   - `dx7_core.v`: the status/skid, config/push, and master-FSM `always`
     blocks are fused into ONE block in their original file order, so
     same-cycle last-writer-wins order is unchanged; `st_overflow` moves to a
     standalone set-only block latching both sources (skid-full DEC-014
     rejection and event-queue-full rejection); `evq_rp`/`evq_cnt` are written
     once, in the config/push merge tail, with push and pop each counted once;
     `shd_commit` is written once with explicit same-cycle priority (a commit
     clear in the same cycle as a set wins the clear).
   - `env_unit.v`: the external slot-write block is folded *last* into the
     step FSM block, so its override priority is unchanged from the two-block
     build.
   - Proof, not argument: the H07 battery on the refrozen core is **34/34**
     (`evidence/h07-core/results-h10-merged-p3.json`, tag `h10-merged-p3`,
     H03 §4.5 latency contract, sha256
     `813f3a9035db62f174ba5fe1ecf83ebed45d399dbfb98502c40f2c4a22a7c1e6`),
     and the H04 env equivalence re-run is bit-exact with its negative
     controls still failing as required (`evidence/h04-env-rtl/`).
   - Synthesis path, pinned flow, machine `repo-remote-gf180-dx7`:
     refrozen 1_1 canonicalize + `check -assert` clean (witness C); full
     `do-yosys` 1_2 PASS (`1_2_yosys.log` sha256
     `e5e706cd86eafdb413b87d3497ec76508a86696a8a353c9f6df1bed4048a76bb`);
     `1_synth` PASS (`1_synth.odb` sha256
     `5b8118c440a96a1c61d192578d00347601c6f7d4a76d912953e3399bba7576ee`,
     491,339,608 B); `2_1_floorplan` PASS (sha256
     `e6026ac7a5fe7e2ca6faf2b8624cc8811308b096833321c9ae9ae666629df481`).
     Place failed at a config-level error (instant 0.87 s abort,
     `[ERROR ORD-0007] …3_4_place_resized.odb does not exist`) — that
     iteration and all physical results are PR-B scope, recorded in
     `evidence/h10-legality/run-attempts.md` row H as BLOCKED.

7. **Recovery note (process).** During the 2026-09-23/24 park window, an
   orchestrator tree-sync `git checkout -- .` reverted the in-flight merge
   edits. The merge was re-applied byte-exactly from the original
   assert-guarded deterministic patch scripts and byte-checked against the
   flow's own captured RTLIL (see
   `H10-HEAVYWORK-PARKED-NOTICE.md`, box `repo-remote-gf180-dx7`). This DR's
   evidence all derives from the re-applied, fingerprint-verified bytes.

## Decision

1. **The frozen core is re-freezed** to the single-driver revisions named in
   Context 6 (dx7_core `34f93d2d…`, env_unit `d91090cd…`, alg_router
   `dda373aa…` unchanged). From merge of this PR, that trio is "the frozen
   core" for all downstream claims; the `335599ea…` pin stands as the
   historical H07/H08 record only.

2. **Claim boundary of this PR (PR-A).** It establishes *synthesizable
   legality* of the refrozen core: elaboration-clean (witnesses A/B), pinned
   flow 1_1/1_2/1_synth pass with a written synth database (Context 6). It
   claims **no timing, no fit, no pad or margin result** — those belong to
   the H10 mapped feasibility report (PR-B), where the report generator must
   refuse to emit a fit conclusion for any stage that did not run (issue #32
   negative control; row H of the run-attempts digest is BLOCKED, not omitted).

3. **Ratification gate.** This DR is *Proposed* by the builder. The refreeze
   changes what "the frozen core" means repository-wide (downstream pins:
   `tests/test_h08.py` CORE_SHA, `docs/H08-PIN.md`, `docs/reuse/catalog.json`
   are updated in this PR), so **operator ratification is required before
   merge**; until then this PR holds at judge approval.

4. **Evidence hygiene.** The heavy artifacts (full 1_2 logs, `1_synth.odb`,
   the 217 MB mapped netlist, floorplan odb) are hash-pinned here but not
   committed; they land with the H10 report bundle (`evidence/h10-gf180/`,
   PR-B). The digest `evidence/h10-legality/run-attempts.md` is the
   run-by-run record, including the superseded battery attempt (row I),
   the blocked place stage (row H), the one iverilog-13-devel deadlock
   recorded without a verdict (row L), and the re-certified H07 accept /
   shadow / synthesis-gate evidence on the refrozen core (rows K–M).

## Consequences

- **Enables:** any mapped PnR/timing work (PR-B) against a core the pinned
  flow accepts; the remaining place/floorplan effort proceeds on a frozen
  core that cannot move under it.
- **Forbids:** (a) citing the `335599ea…` pin as the current core after
  merge; (b) any fit/timing claim from this PR; (c) treating simulator
  bit-exactness alone as synthesis-path legality (that class of failure is
  now evidenced, witnesses B vs E); (d) assuming the `exp_t*` out-of-range
  select bits are zero or constant — they are synthesis-resolved `undef`
  pending the PR-B mapped result (Context 5).
- **STALE when this changes:** if the refrozen RTL is ever edited again, the
  battery must be re-run fresh (mixed partial artifacts are prohibited —
  row I exists as the standing negative example) and the H08/H10 pins
  re-pinned in the same PR.
- **H08 chassis:** `docs/H08-PIN.md` evidence remains the historical record
  on the pre-refreeze core; the chassis re-verifies on the refrozen core via
  the 34/34 battery (chassis files unmodified).