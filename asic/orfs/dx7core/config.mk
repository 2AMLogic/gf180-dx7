# OpenROAD-flow-scripts design config: dx7core -- the H07 integrated polyphonic DX7 core
# (rtl/dx7_core.v + rtl/env_unit.v + rtl/alg_router.v, top dx7_core) -- gf180mcu, 7-track, 5.0 V.
# Adapted from 2AMLogic/gf180-parasynth pnr/orfs/synth_top/config.mk @ cbcc8b9e (Apache-2.0);
# provenance: docs/reuse/catalog.json entry orfs-dx7core-design-and-synth-sta.
#
# Run with ../run-orfs.sh dx7core   (see ../README.md). Die is a FIXED INPUT per run,
# passed on the make command line (FLOW_VARIANT / DIE_AREA / CORE_AREA / PLACE_DENSITY);
# utilisation is the measured quantity, never a set target (fixed-die methodology).
#
# Policy declarations (all required by docs/PHYSICAL-CONSTRAINTS.md open question 3):
#   * DONT_USE: the ORFS gf180 platform's stock policy is DONT_USE_CELLS=*_1 (drive-1
#     cells excluded; the platform file is later than this one in make's view and its
#     plain-`=` values cannot be overridden here -- sibling run-orfs.sh README gotcha).
#     Every number in this run's evidence is under that policy; numbers under a
#     *_1-permissive policy are not comparable.
#   * Clock: 24.576 MHz (40.692 ns), the DR-0008 design point (512 clocks per 48 kHz
#     frame). constraint.sdc records it. The 12.288 MHz fallback is NOT this run's
#     clock; implied-fmax from the corner sweep is the comparability answer.
#   * ROM-as-logic: SYNTH_MEMORY_MAX_BITS=65536, the sibling flow's value -- the core's
#     three tables (sinlog 1024x16, sinexp 1024x16, freqlut 1025x32) dissolve to
#     combinational logic, matching the H07 pre-synthesis run and the D01 section 5
#     storage anchors (1.9-2.2 um^2 per stored ROM bit). No SRAM macro is requested.
#   * ABC_AREA=1 (area-driven mapping, the flow default stated for the record).
export PLATFORM        = gf180
export TRACK_OPTION    = 7t
# TC = tt_025C_5v00 (the flow's optimisation corner; ss_125C_4v50 is the HEADLINE corner
# for feasibility per family practice; ../sta-synth.tcl reports it at the synthesis stage).
export CORNER          = TC
export DESIGN_NAME     = dx7_core
export DESIGN_NICKNAME = dx7core

DESIGN_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
export VERILOG_FILES   = $(abspath $(DESIGN_DIR)/../../../rtl/dx7_core.v) \
                         $(abspath $(DESIGN_DIR)/../../../rtl/env_unit.v) \
                         $(abspath $(DESIGN_DIR)/../../../rtl/alg_router.v)
export VERILOG_INCLUDE_DIRS = $(DESIGN_DIR)/../../../rtl/
export TOP             = dx7_core
export SDC_FILE        = $(DESIGN_DIR)/constraint.sdc
export SYNTH_MEMORY_MAX_BITS = 65536
# Table IP: NOT via VERILOG_DEFINES. The ORFS flow's verilog_defaults -add {*}$env()
# line applies only the FIRST -D word (verified 2026-09-23 in the pinned 26Q3 image;
# a second word silently does not define its macro), so multi-define table addressing
# is unreliable through that hook. Instead run-orfs.sh bind-mounts
# reference/tables/ at /OpenROAD-flow-scripts/flow/reference/tables (ro), which makes
# the RTL `ifndef defaults (reference/tables/*.hex, yosys-relative) resolve directly.
export ABC_AREA        = 1
# The stock ORFS gf180 platform techmaps hard-code 9t cell names; use the 7t
# files rederived in-repo from the pinned image's own platform files (single
# documented token substitution, per-cell liberty verification, DR-0010).
export ADDER_MAP_FILE  = $(DESIGN_DIR)/../gf180_7t/cells_adders.v
export LATCH_MAP_FILE  = $(DESIGN_DIR)/../gf180_7t/cells_latch.v
# The stock flow handles the two SystemVerilog size casts in dx7_core.v itself: the
# canonicalize step reads all VERILOG_FILES with `read_verilog -defer -sv` (image
# /OpenROAD-flow-scripts/flow/scripts/synth_preamble.tcl; the flow's yosys is the image's
# tools/install/yosys/bin/yosys, 0.68+post -- not the 0.67 on PATH). No custom
# SYNTH_SCRIPT is needed, so the stock synthesis pipeline (liberty load, dfflibmap,
# abc, opt) runs unmodified.
# The image sets LEC_CHECK=1 with a Kepler formal binary that dies with "illegal
# instruction" under Docker's amd64 emulation on Apple Silicon (sibling run gotcha);
# formal equivalence is not this issue's claim.
export LEC_CHECK       = 0
# Skip the static IR-drop analysis in finish: the platform setRC.tcl only defines via
# resistances at WC, so at TC analyze_power_grid aborts ([ERROR PSM-0021] zero via
# resistance) after final artifacts are written (sibling run gotcha). IR is H11's
# concern; its absence is recorded, not hidden.
export PWR_NETS_VOLTAGES =
export GND_NETS_VOLTAGES =