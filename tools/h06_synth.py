#!/usr/bin/env python3
"""H06 pitch/modulation unit synthesis: mapped area report (issue #28).

Runs yosys over rtl/pitch_mod.v against the local ciel gf180mcu 7t liberty
(the D01-recorded install H01/H02 measured), maps flops with dfflibmap and
combinational logic with abc, and applies the probe gates:

  full   the unit as committed. Gate: mapped flops > 192 -- the six
         registered 32-bit phase-increment outputs alone are irreducible
         (each is a distinct per-frame product of LFO state, pitch-EG
         state, per-op basepitch and the pinned freqlut table), so any
         build below this floor has lost datapath state = FAIL. Chip
         area > 0, sequential area > 0 and cells > 0 are also required
         (an optimized-away build maps to nothing).
  strip  -DH06_STRIP_OBSERVABILITY: every output tied off. The gate must
         FAIL (yosys deletes every state flop) -- the negative control
         proving the flop gate has resolution, mirroring H01/H02.

Declared-vs-mapped flops are recorded with the reason they differ: the
mapped count is legitimately smaller because yosys exploits reset-value
constants (e.g. never-written high bits of the peg/inc registers) and
ROM-content redundancy. The contract the mapping must preserve is RTL
BEHAVIOR -- proven bit-exact to the frozen model on the pre-synthesis RTL
by tools/h06_compare.py (4,332-frame directed suite + 4 mutant controls).

What is reported and how it is labelled:
  - cell counts, flop counts, per-cell table, chip area: measured
    (yosys stat -liberty against the named liberty).
  - ROM representation: measured from the transcript (the freqlut and
    LFO-sin memories dissolve to logic gates via MEMORY_MAP; no BRAM
    exists in this flow).
  - control-tail cycles per frame: measured by the equivalence harness
    (bench maxcycles); quoted here only as a pointer to
    evidence/h06-pitch-rtl/compare_report.json.
  - delay: NOT extractable -- this flow has no STA; abc prints no
    achieved-delay number. All ns timing is UNMEASURED -- clock closure
    at the 24.576 MHz design point is H10's measurement (D01 section 2).

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool
or PDK path -- the message names what is missing). Stdlib only.
"""

import os
import re
import sys

import pdk_resolve
from single_module_stat import parse_single_module_stat

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DEFAULT_CIEL = None  # resolved by pdk_resolve (liberty sha256 pin)
SC_LIB_REL = pdk_resolve.SC_LIB_REL

RTL_REL = "rtl/pitch_mod.v"
TB_REL = "rtl/tb_pitch_mod.v"
OUTDIR_REL = "evidence/h06-pitch-rtl"

# The registered-output floor for the full-build flop gate (see docstring):
# six 32-bit phase-increment output registers.
FLOP_FLOOR = 192


_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402,F401
from sha256_helpers import sha256_file  # noqa: E402,F401
from yosys_run import (  # noqa: E402
    rom_representation as shared_rom_representation,
    yosys_version as _yosys_version)
from probe_synth import (  # noqa: E402
    ProbeSynthSpec, run_probe_synth_cli, run_probe_yosys)


def declared_state_bits(repo_root):
    """Sum of the RTL's declared register widths, parsed from the source
    (the audit surface; mapped flops legitimately land below this --
    see module docstring). Handles packed vectors and unpacked arrays."""
    rtl = os.path.join(repo_root, RTL_REL)
    if not os.path.isfile(rtl):
        raise CouldNotRun(f"pitch RTL not found: {rtl}")
    with open(rtl, "r", encoding="utf-8") as f:
        text = f.read()
    total = 0
    rom_bits = 0
    detail = {}
    # unpacked arrays: reg [hi:lo] name [0:N]; the $readmemh ROM arrays are
    # initialization containers (they map to logic, not state) and are
    # reported separately
    for m in re.finditer(r"^\s*reg\s+\[(\d+):(\d+)\]\s*(\w+)\s*"
                         r"\[0:(\d+)\]\s*;", text, re.M):
        width = (int(m.group(1)) - int(m.group(2)) + 1) * \
            (int(m.group(4)) + 1)
        if m.group(3).endswith("_rom"):
            rom_bits += width
            continue
        detail[m.group(3) + "[]"] = width
        total += width
    # packed single regs: reg [hi:lo] name;
    for m in re.finditer(r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*;",
                         text, re.M):
        name = m.group(3)
        width = 1 if m.group(1) is None else \
            int(m.group(1)) - int(m.group(2)) + 1
        detail[name] = width
        total += width
    if total == 0:
        raise CheckFailure(f"no register declarations parsed from {RTL_REL}")
    return total, detail, rom_bits


def yosys_version(yosys):
    return _yosys_version(yosys, CouldNotRun)


def run_yosys(yosys, repo_root, liberty, strip, log_path):
    return run_probe_yosys(yosys, repo_root, RTL_REL, "pitch_mod",
                           "H06_STRIP_OBSERVABILITY", liberty, strip, False,
                           log_path, CheckFailure)


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module only (issue #95): the one per-module area block IS the
    design total only because pitch_mod has no submodules; a hierarchical
    or multi-block transcript raises CheckFailure. Guard and parsing are
    shared in single_module_stat.py (issue #119).
    """
    return parse_single_module_stat(log, "pitch_mod", CheckFailure)


ROM_DETAIL = (
    "the 1025x32 freqlut ROM and the 2048x26 LFO sine ROM "
    "dissolve to combinational gates (yosys MEMORY_MAP; no "
    "BRAM in this flow); final stat shows no memory cells")


def rom_representation(log):
    """How the table memories landed, from the transcript (measured)."""
    return shared_rom_representation(log, ROM_DETAIL)


def _declared_state(root):
    total, detail, rom_bits = declared_state_bits(root)
    return total, detail, {"rom_init_bits": rom_bits}


def _print_summary(gates, runs, declared, declared_extra, out_json):
    rom_declared = declared_extra.get("rom_init_bits", 0)
    for name, g in gates.items():
        print(f"gate {name}: {g['status']}")
    if "full" in runs:
        st = runs["full"]
        rom = st["rom_representation"]["representation"]
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared} + {rom_declared} ROM init bits), cells "
              f"{st['cell_total']}, chip "
              f"{st['chip_area_um2']:.1f} um2 "
              f"(seq {st['seq_area_um2']:.1f}), ROMs as {rom}")
    print(f"report: {out_json}")


SPEC = ProbeSynthSpec(
    description=__doc__.splitlines()[0],
    repo_root=REPO_ROOT,
    outdir_rel=OUTDIR_REL,
    schema="h06-synth-report/1",
    issue=28,
    planning_id="H06",
    claim_boundary="mapped area/cell cost of the pitch/mod unit "
                   "only; no place-and-route, no timing closure, no "
                   "fit claim, no audibility claim; RTL-vs-model "
                   "agreement is tools/h06_compare.py's result",
    constraints="no SDC: probe flow per issue #28 (read_verilog; "
                "synth -top; dfflibmap; abc; stat), no clock "
                "defined",
    rtl_rel=RTL_REL,
    tb_rel=TB_REL,
    top="pitch_mod",
    strip_define="H06_STRIP_OBSERVABILITY",
    use_ltp=False,
    flop_floor=FLOP_FLOOR,
    flop_requirement=f"mapped flops > {FLOP_FLOOR} (six registered "
                     "32-bit phase-increment outputs are irreducible) "
                     "and chip area > 0 and sequential area > 0 and "
                     "cells > 0",
    harness_key="equivalence_harness",
    harness_value="tools/h06_compare.py (golden: "
                  "src/gf180_dx7/model/pitch.py)",
    yosys_version=yosys_version,
    check_failure=CheckFailure,
    parse_stat=parse_stat,
    rom_representation=rom_representation,
    declared_state=_declared_state,
    declared_note="mapped < declared is expected and behavior-preserving: "
                  "yosys exploits reset-value constants (never-written "
                  "high bits of peg/inc registers) and ROM-content "
                  "redundancy, replacing redundant flops with wires. The "
                  "contract the mapping must preserve is RTL behavior, "
                  "proven bit-exact to the frozen model by "
                  "tools/h06_compare.py.",
    extra_sections={
        "control_tail_cycles": {
            "measured_source": "evidence/h06-pitch-rtl/compare_report.json "
                               "(bench maxcycles; worst frame over the "
                               "4,332-frame directed suite)",
            "h03_rows_covered": "budget_worst_case rows pitch_eg_steps (12 "
                                "derived), lfo_frame (100 derived), "
                                "pitch_mod_application (8 derived), "
                                "freq_updates (8/op derived); the measured "
                                "per-frame total is the H04/H07 integration "
                                "input -- this probe does not re-run the "
                                "budget tool (no contract edit)",
        },
    },
    print_summary=_print_summary,
)


def main(argv=None):
    return run_probe_synth_cli(SPEC, argv)


if __name__ == "__main__":
    sys.exit(main())
