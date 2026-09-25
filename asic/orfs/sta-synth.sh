#!/usr/bin/env bash
# sta-synth.sh -- run sta-synth.tcl (synthesis-stage STA, three 5 V corners) on a mapped netlist inside
# the pinned ORFS image (its standalone OpenSTA binary `sta`: no LEF/technology is needed for an
# unplaced netlist), against the liberty files of an explicit local PDK root (the same liberty
# the netlist was mapped with; hashes are recorded by tools/h10_collect.py).
#
#   ./sta-synth.sh <netlist.v> <log-out> [top]
#   PDK_ROOT=/path/to/ciel-root ./sta-synth.sh ...     (default: ~/.ciel)
#
# Repo/netlist paths inside the container equal host paths; the liberty directory is mounted at
# /pdklib (a same-path bind under ~/.ciel proved unreliable on Docker Desktop).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
NETLIST="$(cd "$(dirname "${1:?usage: sta-synth.sh <netlist.v> <log-out> [top]}")" && pwd -P)/$(basename "$1")"
LOG="${2:?usage: sta-synth.sh <netlist.v> <log-out> [top]}"
TOP="${3:-dx7_core}"
PDK_ROOT="$(cd "${PDK_ROOT:-$HOME/.ciel}" && pwd -P)"
LIBD="$PDK_ROOT/gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib"
LIBD="$(cd "$LIBD" && pwd -P)"
IMAGE="${ORFS_IMAGE:-openroad/orfs:26Q3-296-gda37dce1c@sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2}"
command -v docker >/dev/null || { echo "docker not on PATH" >&2; exit 2; }
for c in tt_025C_5v00 ss_125C_4v50 ff_n40C_5v50; do
  [ -f "$LIBD/gf180mcu_fd_sc_mcu7t5v0__$c.lib" ] || { echo "missing liberty $c under $LIBD" >&2; exit 2; }
done
NDIR="$(dirname "$NETLIST")"
exec docker run --rm --platform linux/amd64 \
  -v "$HERE:$HERE:ro" -v "$NDIR:$NDIR:ro" \
  --mount "type=bind,source=$LIBD,target=/pdklib,readonly" \
  -e NETLIST="$NETLIST" -e TOP="$TOP" -e SDC="$HERE/dx7core/constraint.sdc" \
  -e LIB_TT="/pdklib/gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib" \
  -e LIB_SS="/pdklib/gf180mcu_fd_sc_mcu7t5v0__ss_125C_4v50.lib" \
  -e LIB_FF="/pdklib/gf180mcu_fd_sc_mcu7t5v0__ff_n40C_5v50.lib" \
  "$IMAGE" /OpenROAD-flow-scripts/tools/install/OpenROAD/bin/sta -no_init -no_splash -exit "$HERE/sta-synth.tcl" \
  > "$LOG" 2>&1
