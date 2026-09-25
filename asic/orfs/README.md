# asic/orfs — gf180mcu mapped-feasibility flow for the dx7 core (H10)

OpenROAD-flow-scripts (ORFS) driver plus this repository's synthesis-stage
tools for the gf180mcu **mapped** feasibility evidence of issue #32 (H10).
The report is `docs/H10-GF180-FEASIBILITY.md`, generated from
`evidence/h10-gf180/bundle.json` by `tools/h10_report.py`.

## Files

| File | Origin |
|------|--------|
| `run-orfs.sh` | adapted from `gf180-parasynth` `pnr/orfs/run-orfs.sh` @ `cbcc8b9e` (upstream sha256 `a827c1c3…`, unchanged through `be870e8`); one adaptation: bind-mount `reference/tables/` read-only at the flow cwd so the RTL's `$readmemh` defaults resolve. Catalog entry `orfs-run-flow`. |
| `gf180_7t/cells_adders.v`, `gf180_7t/cells_latch.v` | rederived in-repo (DR-0010) from the pinned image's `flow/platforms/gf180/cells_{adders,latch}.v` by the single substitution `mcu9t5v0 → mcu7t5v0`; verified by `tools/verify_gf180_7t_techmaps.py` (transform + liberty cell check). Catalog entry `orfs-gf180-7t-techmaps-rederived`. |
| `gf180_7t/LICENSE-ORFS-BSD-3-Clause` | byte-identical copy of the image's `LICENSE_BUILD_RUN_SCRIPTS` (the license the techmaps ship under; DR-0010 item 4). |
| `dx7core/constraint.sdc` | adapted from `gf180-parasynth` `pnr/orfs/synth_top/constraint.sdc` @ `cbcc8b9e` (upstream sha256 `e426ffa4…`, unchanged through `be870e8`). The last three lines (`create_clock`, 20 % input/output delays) are verbatim; the clock period (40.692 ns, DR-0008) and the header comment are ours. Catalog entry `orfs-dx7core-design-and-synth-sta`. |
| `dx7core/config.mk` | adapted from `gf180-parasynth` `pnr/orfs/synth_top/config.mk` @ `cbcc8b9e` (upstream sha256 `5bf54b72…`, unchanged through `be870e8`). It keeps the upstream structure. 8 variable lines are verbatim: `PLATFORM`, `TRACK_OPTION`, `CORNER`, `DESIGN_DIR`, `SDC_FILE`, `SYNTH_MEMORY_MAX_BITS`, and the empty `PWR/GND_NETS_VOLTAGES`. The same settings are kept with changed whitespace for `ABC_AREA`, the 7t techmap overrides and `LEC_CHECK=0`. The gotcha comments are paraphrased from upstream. Ours: design/RTL/top, the dropped `SYNTH_SCRIPT` wrapper, no hard-coded die (passed per run), and the policy declarations. Catalog entry `orfs-dx7core-design-and-synth-sta`. |
| `sta-synth.tcl` | adapted from `gf180-parasynth` `pnr/orfs/sta-corners.tcl` @ `cbcc8b9e` (upstream sha256 `255deb05…`, unchanged through `be870e8`). 15 of its 43 lines are verbatim upstream: the three-corner setup, the clock/period lookup, and the whole per-corner `worst_slack`/`total_negative_slack`/`=== CORNER` block. Adaptations: it reads a standalone mapped netlist rather than the ORFS `6_final.odb`; ideal clock, no parasitics and no OpenRCX extraction; liberty paths come from the environment; a changed report set (setup paths ss/tt/ff, one ff hold path, no clock-skew report); a new header. Catalog entry `orfs-dx7core-design-and-synth-sta`. |
| `sta-synth.sh` | adapted from `gf180-parasynth` `pnr/orfs/run-orfs.sh` @ `cbcc8b9e` (upstream sha256 `a827c1c3…`). It reuses that script's wrapper idioms, with these lines verbatim: shebang, `set -euo pipefail`, the `HERE=` resolver, and the `IMAGE=${ORFS_IMAGE:-…}` default. The rest is new: argument handling, the liberty mount at `/pdklib`, and running the image's standalone `sta` on `sta-synth.tcl`. Catalog entry `orfs-dx7core-design-and-synth-sta`. |

Not imported in this increment (they serve stages that have not run here):
the sibling's `check-pdn.py`, `summarize.py` and `blockarea.py`, and the
post-route parts of `sta-corners.tcl` (RCX extraction, propagated clock,
`6_final` inputs). They are adopted when a placement/routing stage actually
runs and can requalify them.

## Pins

- Image: `openroad/orfs:26Q3-296-gda37dce1c@sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2` (linux/amd64; runs under emulation on Apple Silicon).
  OpenROAD self-reports `26Q3-1260-g06a5a02279`.
- **yosys: the image ships two builds, and the digest alone does not say
  which one ran.** The invocation decides it:

  | Binary | Self-report | sha256 | Used by |
  |--------|-------------|--------|---------|
  | `/OpenROAD-flow-scripts/tools/install/yosys/bin/yosys` | `Yosys 0.68+post (git sha1 UNKNOWN)` | `5cd52bc790d39b1e59a88112e9132ef8de338c2a26e4b0df8222d937e65bab92` | the ORFS flow (image `flow/scripts/variables.mk` lines 117-120: `YOSYS_EXE` defaults to this path outside a nix shell) and `tools/h10_synth.py` (passed explicitly with `--yosys`, Run step 4). **Every H10 PR-B result.** |
  | `/usr/local/bin/yosys` (first on `PATH`) | `Yosys 0.67 (git sha1 2d1509d1b)` | `21cf7fad1cccb4dea0e5c8fdb0085a7c766a3b401dbdd59d4316e6b6e820afd5` | a bare `yosys` in the container, such as DR-0011 witnesses A/C. No H10 PR-B result. |

  Per-run path, sha256 and a banner check against the committed log are in
  `evidence/h10-gf180/bundle.json` `tools.yosys_runs` (report section 6).
- Platform `gf180`, `TRACK_OPTION=7t`, `CORNER=TC` (`tt_025C_5v00`), stock
  `DONT_USE_CELLS = *_1` (image `flow/platforms/gf180/config.mk` line 24;
  plain `=`, overridable only on the make command line).
- Clock: 40.692 ns (24.576 MHz, DR-0008 design point), I/O delays 20 % of
  the period (`dx7core/constraint.sdc`).
- ROM tables as logic: `SYNTH_MEMORY_MAX_BITS=65536`; no SRAM macro.

## Run

```sh
IMG=openroad/orfs:26Q3-296-gda37dce1c@sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2
LIBD=$(cd ~/.ciel/gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib && pwd -P)
R=$(pwd -P)
# 1. pinned-flow synthesis (stage 1) of the refrozen core (~37 min under emulation)
./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=natdie synth
# 2. fixed-die floorplans: seed each variant with the natdie synthesis database
for v in quarterslot twoslot; do d=asic/orfs/work/results/gf180/dx7core; mkdir -p $d/$v
  for f in 1_synth.odb 1_synth.sdc 1_2_yosys.sdc clock_period.txt; do ln -f $d/natdie/$f $d/$v/$f; done; done
./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=quarterslot \
    "DIE_AREA='0 0 1314.88 1317.12'" "CORE_AREA='10.64 11.76 1304.24 1305.36'" do-2_1_floorplan
./asic/orfs/run-orfs.sh dx7core FLOW_VARIANT=twoslot \
    "DIE_AREA='0 0 1860.88 1862.00'" "CORE_AREA='10.64 11.76 1850.24 1850.24'" do-2_1_floorplan
# 3. synthesis-stage STA on the flow's own netlist
./asic/orfs/sta-synth.sh asic/orfs/work/results/gf180/dx7core/natdie/1_2_yosys.v \
    evidence/h10-gf180/sta-synth/orfs_natdie.log dx7_core
# 4. DONT_USE policy A/B + module attribution, with the image's own yosys
docker run --rm --platform linux/amd64 -v "$R:$R" -w "$R" \
    --mount type=bind,source=$LIBD,target=/pdklib,readonly $IMG \
    python3 tools/h10_synth.py --yosys /OpenROAD-flow-scripts/tools/install/yosys/bin/yosys \
    --liberty /pdklib/gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib
# 5. bundle + report
python3 tools/h10_collect.py --import-orfs-work asic/orfs/work
python3 tools/h10_report.py
```

The die areas are the D01 quarter slot and the sibling's two-slot die
(the same `DIE_AREA`/`CORE_AREA` rectangles the sibling used). Placement,
CTS and routing are **not run**: on both fixed dies the ORFS floorplan
measures utilisation far above 100 %, so no placement exists to run. A
natural-die run (`DIE_AREA`/`CORE_AREA` sized from the measured area, ~57
mm² at the sibling's ~58 % utilisation) is the remaining work for
post-route timing; it was not attempted here.

## What this directory establishes

Only what the committed evidence under `evidence/h10-gf180/` records:
mapped synthesis area/instances and synthesis-stage timing. Nothing about
placement, routing, post-route timing, signoff, functional correctness
(H02/H07's evidence), original-DX7 fidelity, or musical quality.

`work/` is git-ignored build output; large results (netlists, `.odb`) are
recorded by sha256 in the evidence, not committed.
