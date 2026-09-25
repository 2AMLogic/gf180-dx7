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
| `dx7core/config.mk`, `dx7core/constraint.sdc` | this repository (H10). |
| `sta-synth.tcl`, `sta-synth.sh` | this repository (H10): synthesis-stage OpenSTA at tt/ss/ff (ideal clock, no parasitics) using the image's standalone `sta` binary. Output format matches the sibling's `sta-corners.tcl` `=== CORNER` lines. |

Not imported in this increment (they serve stages that have not run here):
the sibling's `sta-corners.tcl` (post-route multi-corner STA),
`check-pdn.py`, `summarize.py`, `blockarea.py`. They are adopted when a
placement/routing stage actually runs and can requalify them.

## Pins

- Image: `openroad/orfs:26Q3-296-gda37dce1c@sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2` (linux/amd64; runs under emulation on Apple Silicon).
  In-image binaries self-report `Yosys 0.68+post (git sha1 UNKNOWN)` and
  OpenROAD `26Q3-1260-g06a5a02279`. The digest is the identity of record;
  DR-0011's "yosys 0.67" does not match this binary (report section 6).
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
