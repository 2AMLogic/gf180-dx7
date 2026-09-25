# Issue #98 / DR-0012 evidence — the refrozen core keeps its exp() unit

Input under test: the DR-0012 frozen core, `rtl/dx7_core.v` sha256
`f33cecbd138aea2a869cb502dca9273b342c8f97be51c9a1e587f2eb4aba89fa`, with
`rtl/env_unit.v` `d91090cd…` and `rtl/alg_router.v` `dda373aa…` unchanged.
Superseded input, used as the negative control throughout: the DR-0011 pin
`34f93d2d…`. Tool identities, simulator versions, image digest and the host's
Docker capacity: `tool-versions-2026-09-25.txt`. All runs 2026-09-25 on
`repo-local-gf180-dx7` (Apple Silicon; the pinned image runs `linux/amd64`
under emulation).

Statuses are PASS / FAIL / NOT_RUN / BLOCKED / NO_VERDICT / STALE as defined in
`AGENTS.md`. **Read row F before citing anything about the mapped netlist.**

| # | check | status | artifact |
|---|---|---|---|
| A | static range proof on the refrozen core | PASS | `static-proof-refrozen-2026-09-25.json` |
| B | RTL-vs-simulator cross-check, Verilator | PASS | `xcheck-verilator-refrozen-2026-09-25.json` |
| C | RTL-vs-simulator cross-check, Icarus | PASS | `xcheck-iverilog-refrozen-2026-09-25.json` |
| D | Verilator lint: no `SELRANGE` | PASS | `verilator-lint-refrozen-2026-09-25.txt` |
| D′ | lint control: the same core at DR-0011 widths | FAIL as required (3 `SELRANGE`) | `verilator-lint-nc-dr0011-widths-2026-09-25.txt`, `nc-dr0011-widths.diff` |
| E | pinned-image cone LEC + full-core `exp_hsum` driver | PASS (exit 0, all controls behaved) | `exp-hsum-lec-pinned-refrozen-2026-09-25.json`, `lec-src/` |
| E′ | the same tool on the DR-0011 file (required failure control) | FAIL as required (exit 1) | `exp-hsum-lec-pinned-nc-dr0011-2026-09-25.json` |
| F | pinned-flow **mapped netlist census on this pin** | **BLOCKED** (host memory) | `orfs-synth-attempt-refrozen-2026-09-25.txt` |
| G | H07 battery, fresh, Verilator, 34 cases | PASS 34/34 | `../h07-core/results-dr0012-p1.json` |
| G′ | same battery vs the DR-0011-pin baseline, per-case captured PCM | 34/34 byte-identical | `../h07-core/results-h10-merged-p3.json` |
| G″ | second clean battery on the same pin (DR-0011's two-clean-runs rule) | PASS 34/34, artifact-hash identical to G | `../h07-core/results-dr0012-p2.json` |
| H | H07 battery, Icarus, on this pin | **NOT_RUN** (host throughput) | — |
| I | host-yosys 0.69+post cone LEC, independent of the pinned image | PASS (same verdict, different yosys) | `exp-hsum-lec-host-refrozen-2026-09-25.json` |
| K | live control for the weakened H07 synth-report gate | FAIL as required when the heavy host is reachable | `h07-synth-report-stale-control-2026-09-25.txt` |

## A — static range proof

`tools/exp_range_proof.py --mode static`, exit 0. The exact products need
68 / 60 / 52 bits against the now-86 / 86 / 82-bit wires, so
`out_of_range_bits` is `null` for all three terms and the headroom is
18 / 26 / 30 bits. `rtl_matches_frozen_pin: true`. This is the property that
makes the widening a no-op for every simulated value; rows B, C and G′ measure
that it is.

## B, C — simulator cross-checks

`tools/exp_range_proof.py --mode xcheck --sim {verilator,iverilog}`, 64 probe
points each, both PASS with `hsum_unknown_points: 0`.

The Icarus row is the interesting one. On the DR-0011 pin the same check was
`NO_VERDICT` with `hsum_unknown_points: 64`
(`../issue-86-exp-range-proof/xcheck-iverilog-2026-09-25.json`): Icarus applies
IEEE 4-state `+` to the out-of-range read bits and returns `x` for the whole
sum, which is why the canonical Icarus shadow could not render the AM cases
(issue #96). On the refrozen core it is 0/64 — the same root cause, fixed once.

## D, D′ — lint

`verilator --lint-only -Wall -Wno-fatal -Irtl rtl/dx7_core.v rtl/env_unit.v
rtl/alg_router.v` on the refrozen core: **0** `SELRANGE`. The scratch control
(the identical file with only the three declarations put back to `[70:0]`,
`[70:0]`, `[65:0]` — see the diff, scratch sha256 `fb6ce475…`) reports **3**.

Honest note on the rest of the lint: the warning total is 188 in both runs.
Widening trades the 3 `SELRANGE` warnings for 3 `UNUSEDSIGNAL` warnings —
`exp_t3[29:0]`, `exp_t4[29:0]`, `exp_t5[25:0]`. Those low fractional bits were
never read before either (`exp_hsum` starts its reads at bit 30 / 30 / 26); the
out-of-range select simply stopped Verilator from reporting them. No lint
waiver was added.

## E, E′ — formal agreement under the pinned image's yosys

`tools/exp_hsum_lec.py` (pinned image `sha256:ebc8142da6d6…`, its
`tools/install/yosys/bin/yosys`, `Yosys 0.68+post`), exit 0 on the refrozen
core:

| sub-check | refrozen core | DR-0011 file (control run) |
|---|---|---|
| `cone_lec` (SAT miter, gate vs simulated-core gold) | PASS | FAIL, mapped cone is constant `57'h0` |
| `cone_lec_widened` (positive control) | PASS | PASS |
| `cone_lec_narrowed` (live negative control, DR-0011 widths) | FAIL as required | FAIL as required |
| `cone_lec_control` (gold with one out-of-range bit forced to 1) | FAIL as required | FAIL as required |
| `core_hsum_undef` on the **whole** core | driver is an `$add` cell, `all_x: false` | driver tied to `69'x` |
| `controls_behaved` | true | true |

So on this pin the `exp_hsum` adder survives the flow's opening passes
(`read_verilog -sv`, `hierarchy`, `proc`, `opt_expr`) as real logic, and the
adder cone is proved equivalent to the simulated-core semantics for **every**
input rather than for sampled ones. The two required failure controls both
still fail: the as-written DR-0011 RTL (E′, exit 1) and a gold with one
out-of-range bit set to 1 (`cone_lec_control`).

## F — mapped netlist census on this pin: BLOCKED, not PASS and not FAIL

`ORFS_WORK=… ./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=natdie synth` on the
refrozen core was attempted twice on this host and **killed both times inside
`techmap`**, after ~4 min 24 s, with `make` reporting `Error 247`:

- 247 is not a yosys diagnostic. The flow runs yosys through
  `flow/scripts/run_command.py`, whose last line is `sys.exit(run())` returning
  `proc.returncode`; a child killed by `SIGKILL` gives `-9`, and `sys.exit(-9)`
  exits 247. The run was killed, not failed.
- yosys' own `getrusage` peak at the kill was 5,423,872 KB (5.2 GiB).
- The Docker VM here has `MemTotal` 8,317,267,968 B (7.75 GiB), of which
  ~3.8 GiB was resident in unrelated long-lived containers during the attempt
  (`docker stats` sampled throughout).
- The successful pinned-flow run of the *same* widening in #86 peaked at
  11,411,552 KB (11.4 GiB) —
  `../issue-86-exp-range-proof/orfs-run-widened-2026-09-25.txt`.

So this is a host-capacity block, with a measured ceiling, not a result about
the design. **The standing pinned-flow mapped census for the widening is #86's**,
made on a scratch copy that differs from this pin only in comments:

| netlist | exp datapath flops (`exp_y` … `exp_pt_q`) | all flop instances | synth-stage chip area µm² |
|---|---|---|---|
| as-written DR-0011 core (`../issue-86-exp-range-proof/orfs-census-asis-2026-09-25.json`) | **0** | 1756 | 32,941,030.7 |
| widened scratch copy (`…/orfs-census-widened-2026-09-25.json`) | **564** | 2349 | 34,839,310.1 |

That is evidence about *the widening*, not about this file's hash, and it must
not be quoted as a census of the refrozen pin. Re-running it on this pin — and
re-running H10 on top of it — is issue #103, which needs a host that can give
the container ≥ 12 GB. Until then `docs/H10-GF180-FEASIBILITY.md` is STALE and
its figures are lower bounds (DR-0012 Decision 6).

## G, G′ — the H07 battery, fresh, on the refrozen core

`tools/h07_compare.py --set both --tool verilator --tag dr0012-p1 --jobs 4`,
exit 0, **34/34 PASS**, `overrun` and `overflow` 0 in every case, every latency
measurement inside the contracted window. DR-0011's STALE rule (re-run fresh,
never mix partial artifacts) is satisfied: this is one uninterrupted run of the
whole battery on the refrozen RTL, not a patch over an older one.

The corpus does exercise the path this issue is about: `dev32-06` and
`dev32-30` have a nonzero per-operator LFO amplitude-mod sensitivity
(`amp_mod_sensitivity = 3` on operator 6), so `exp_pt` is applied for those
cases. `dev32-30` is also the case the committed Icarus shadow could not run
at all (#96).

G′ is the direct test of this issue's stop/escalate condition ("if the widened
RTL changes any Verilator-simulated value, stop"). Comparing per-case
`actual_sha256` against the baseline battery run made on the DR-0011 pin
(`results-h10-merged-p3.json`, `rtl_sha256` `7bab93f5…` = the DR-0011 trio):
**34 identical, 0 different**. The captured PCM is byte-for-byte the same.

G″ is DR-0011's other standing requirement for accept evidence: **two** clean
runs whose per-case artifact hashes are identical. `results-dr0012-p2.json`
(tag `dr0012-p2`, run with `--embed-prior` the p1 results) is 34/34 PASS with
`determinism_run2` embedded and zero cases differing from p1. So on the
refrozen pin there are 68 case-runs, all byte-identical to each other **and**
to the pre-refreeze baseline. `tests/test_h07.py::TestConformanceEvidence`
reads the newest-pin results file and still fails if its `rtl_sha256` is not
the current three-file fingerprint — that assertion is what caught the stale
`results-verilog-accept.json` in CI on this PR, exactly as intended.

## H — the Icarus battery on this pin: NOT_RUN, with the reason measured

**NOT_RUN. No Icarus verdict on the refrozen pin is claimed here.**

Two attempts were made at the two AMS ≠ 0 cases (`dev32-06`, `dev32-30`), the
second with exactly the committed shadow's parameters
(`--tool iverilog --frames 24 --jobs 2`; the generated `vector.txt` is
byte-identical to `../h07-core/runs/iverilog-shadow-dev-dev32-06/vector.txt`).
Both were abandoned: those two cases carry their last event at wire frame 1875,
so the simulation has to reach ~120,000 samples, and Icarus — which interprets
the whole 16-voice core — advanced ~6.4 frames/min on this host (192 of 1875
frames in 30 min of wall time, two cases in parallel). That is ≈ 4.5 h per
case-pair, against ~1 min/case for Verilator. The committed 34-case shadow was
produced on a remote Linux host (`../h07-core/remote-run-timeline-2026-09-23.txt`),
not here.

What *is* established about Icarus on this pin is row C, which needs no full
render: the probe cross-check returns `hsum_unknown_points: 0`, where the
DR-0011 pin returned 64/64 unknown. That is the root cause behind issue #96.
Completing the Icarus shadow on this pin (and closing #96 against it) needs a
host like the one that produced the committed shadow.

## I — host-yosys cross-check

The same cone LEC driven by the host's `Yosys 0.69+post` instead of the pinned
image's 0.68+post, exit 0, as a tool-independence check on row E: `cone_lec`
PASS, whole-core driver an `$add` cell, `cone_lec_narrowed` FAIL with the mapped
cone constant `57'h0`, `cone_lec_control` FAIL, `controls_behaved: true` — the
same verdicts on a different yosys build, so the behaviour is not an artefact of
one binary. It has no authority over the pinned flow; row E is the result that
matters for anything mapped.

## What this directory does NOT establish

- Nothing about timing, fit, placement, routing, signoff or silicon. The area
  numbers in row F are synthesis-stage cell area from #86's runs.
- No claim that the mapped netlist of **this pin** was measured (row F).
- Nothing about musical quality or original-DX7 fidelity; bit-exactness to the
  frozen model is claim (1) of `AGENTS.md` only.

## K — the one assertion this PR weakened, with its control

`tests/test_h07.py::TestStripObsControl::test_committed_synth_report_gates`
asserted that `evidence/h07-core/synth_report.json` describes the current RTL.
The refreeze makes that false, and the report cannot be regenerated except on
`repo-remote-gf180-dx7` (yosys + the ciel gf180 7t liberty). The assertion is
now a STALE / NOT_RUN report on a host that cannot regenerate it and a **FAIL**
on a host that can — the same shape the #82 parser-drift check and the #94 H08
gate already use.

Because that trades a hard failure for a guarded skip, the skip is controlled:
with `remote_reachable()` forced true, the drift **fails**, and the artifact
records the run. The skip message itself always names the drift (both hashes)
and the command that clears it, so the condition is never invisible.
