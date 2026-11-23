#!/usr/bin/env python3
"""H05 routing/feedback RTL synthesis: mapped cells/flops/area (issue #27).

Runs yosys over rtl/alg_router.v against the ciel gf180mcu 7t liberty
(D01-recorded path; the same install H01/H02 measured), maps flops with
dfflibmap and combinational logic with abc, and applies the gates:

  full   the router as committed. Gate: mapped flops > 4096 -- the two
         persistent 64 x 32 interconnect bus buffers are irreducible
         state (the N04 stale-bus facts: they are never cleared, every
         bit has a distinct next-value chain through the eval writes), so
         a build below this floor has lost bus state = FAIL. Chip area >
         0, sequential area > 0, cells > 0 are also required.
  strip  -DH05_STRIP_OBSERVABILITY: every output tied off. The gate must
         FAIL (yosys deletes every state flop) -- the negative control
         proving the flop gate has resolution, mirroring H01/H02.

The connection-table ROM representation is recorded from the transcript
(yosys MEMORY_MAP: the 192 x 8 table dissolves to combinational gates; no
BRAM exists in this flow).

Exit codes: 0 pass, 1 gate failure, 2 could-not-run (missing tool or PDK
path -- the message names what is missing). Stdlib only.
"""

import os
import re
import sys

import pdk_resolve
from single_module_stat import parse_single_module_stat

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DEFAULT_CIEL = None  # resolved by pdk_resolve (liberty sha256 pin)
SC_LIB_REL = pdk_resolve.SC_LIB_REL

RTL_REL = "rtl/alg_router.v"
TB_REL = "rtl/tb_alg_router.v"
OUTDIR_REL = "evidence/h05-routing-rtl"

# The bus-pair floor for the full-build flop gate (see docstring): the two
# persistent 64 x 32 interconnect buffers (fm_core.h buf_[2], N04 facts).
BUS_FLOP_FLOOR = 4096


_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402,F401
from sha256_helpers import sha256_file  # noqa: E402,F401
from yosys_run import (  # noqa: E402
    rom_representation as shared_rom_representation,
    yosys_version as _yosys_version)
from probe_synth import (  # noqa: E402
    ProbeSynthSpec, fmt_area, run_probe_synth_cli,
    run_probe_yosys)


def declared_state_bits(repo_root):
    """Sum of the RTL's declared register widths, parsed from the source
    (the audit surface; mapped flops legitimately land below this -- the
    yosys optimizer may exploit don't-care structure, and the contract the
    mapping must preserve is RTL behavior, proven bit-exact to the frozen
    model by tools/h05_compare.py on the pre-synthesis RTL)."""
    rtl = os.path.join(repo_root, RTL_REL)
    if not os.path.isfile(rtl):
        raise CouldNotRun(f"router RTL not found: {rtl}")
    with open(rtl, "r", encoding="utf-8") as f:
        text = f.read()
    total = 0
    detail = {}
    excluded = {}

    def account(decl, name_list, depth=1):
        nonlocal total
        m = re.match(r"reg\s+(?:signed\s+)?(?:\[(\d+):(\d+)\]\s*)?", decl)
        width = 1
        if m and m.group(1) is not None:
            width = int(m.group(1)) - int(m.group(2)) + 1
        for name in name_list:
            bits = width * depth
            if name == "alg_rom":
                # $readmemh-initialized table: maps to combinational
                # logic (rom_representation), not flops
                excluded[name] = bits
                continue
            if name == "rd_data_r":
                # combinational read-mux reg (always @*), not a flop
                excluded[name] = bits
                continue
            detail[name] = bits
            total += bits

    for m in re.finditer(
            r"^\s*(reg\s+(?:signed\s+)?(?:\[(\d+):(\d+)\]\s*)?"
            r"(\w+(?:\s*,\s*\w+)*))\s*;", text, re.M):
        decl = m.group(1)
        names = re.split(r"\s*,\s*", m.group(4))
        account(decl[:m.start(4) - m.start(1)], names)
    # arrays (buses, note buffer, op banks) parsed separately
    for m in re.finditer(
            r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*\[0:(\d+)\]\s*;",
            text, re.M):
        name = m.group(3)
        width = (int(m.group(1)) - int(m.group(2)) + 1) if m.group(1) else 1
        depth = int(m.group(4)) + 1
        account("reg " + (f"[{m.group(1)}:{m.group(2)}] " if m.group(1)
                          else ""), [name], depth)
    if total == 0:
        raise CheckFailure(f"no register declarations parsed from {RTL_REL}")
    return total, {"declared": detail, "excluded_non_flop": excluded}


def yosys_version(yosys):
    return _yosys_version(yosys, CouldNotRun)


def run_yosys(yosys, repo_root, liberty, strip, log_path):
    return run_probe_yosys(yosys, repo_root, RTL_REL, "alg_router",
                           "H05_STRIP_OBSERVABILITY", liberty, strip, False,
                           log_path, CheckFailure)


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module only (issue #95): the one per-module area block IS the
    design total only because alg_router has no submodules; a hierarchical
    or multi-block transcript raises CheckFailure. Guard and parsing are
    shared in single_module_stat.py (issue #119).
    """
    return parse_single_module_stat(log, "alg_router", CheckFailure)


ROM_DETAIL = (
    "the 32x6 x 8-bit connection table dissolves to "
    "combinational gates (yosys MEMORY_MAP; no BRAM in this "
    "flow); final stat shows no memory cells")


def rom_representation(log):
    """How the connection-table memory landed, from the transcript."""
    return shared_rom_representation(log, ROM_DETAIL, readmem=True)


def _print_summary(gates, runs, declared, declared_extra, out_json):
    for name, g in gates.items():
        print(f"gate {name}: {g['status']}")
    if "full" in runs:
        st = runs["full"]
        rom = st["rom_representation"]["representation"]
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared}), cells {st['cell_total']}, chip "
              f"{fmt_area(st['chip_area_um2'])} um2 "
              f"(seq {fmt_area(st['seq_area_um2'])}),"
              f" ROM as {rom}")
    print(f"report: {out_json}")


SPEC = ProbeSynthSpec(
    description=__doc__.splitlines()[0],
    repo_root=REPO_ROOT,
    outdir_rel=OUTDIR_REL,
    schema="h05-synth-report/1",
    issue=27,
    planning_id="H05",
    claim_boundary="mapped cell/flop/area cost of the routing "
                   "probe only; no place-and-route, no timing "
                   "closure, no fit claim, no audibility claim; "
                   "RTL-vs-model agreement is tools/h05_compare.py's "
                   "result and is not implied by any number here",
    constraints="no SDC: probe flow per issue #27 (read_verilog; "
                "synth -top; dfflibmap; abc; stat), no clock "
                "defined",
    rtl_rel=RTL_REL,
    tb_rel=TB_REL,
    top="alg_router",
    strip_define="H05_STRIP_OBSERVABILITY",
    use_ltp=False,
    flop_floor=BUS_FLOP_FLOOR,
    flop_requirement=f"mapped flops > {BUS_FLOP_FLOOR} (the two "
                     "persistent 64x32 interconnect bus buffers are "
                     "irreducible N04 state) and chip area > 0 and "
                     "sequential area > 0 and cells > 0",
    harness_key="conformance_harness",
    harness_value="tools/h05_compare.py (golden: "
                  "src/gf180_dx7/model/algorithm.py)",
    yosys_version=yosys_version,
    check_failure=CheckFailure,
    parse_stat=parse_stat,
    rom_representation=rom_representation,
    declared_state=lambda root: (*declared_state_bits(root), {}),
    declared_note="mapped differs from declared by a few bits and that "
                  "is expected and behavior-preserving (yosys exploits "
                  "constant/unused-bit structure and dfflibmap may "
                  "replicate registers). The contract the mapping must "
                  "preserve is RTL behavior, proven bit-exact to the "
                  "frozen model by tools/h05_compare.py on the "
                  "pre-synthesis RTL.",
    extra_sections={},
    print_summary=_print_summary,
)


def main(argv=None):
    return run_probe_synth_cli(SPEC, argv)


if __name__ == "__main__":
    sys.exit(main())
