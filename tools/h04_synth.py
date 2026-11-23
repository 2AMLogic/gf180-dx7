#!/usr/bin/env python3
"""H04 envelope-unit synthesis: mapped cell/flop/area report (issue #26).

Runs yosys over rtl/env_unit.v against the ciel gf180mcu 7t liberty (the
same D01-recorded install H01/H02 measured), maps flops with dfflibmap and
combinational logic with abc, and applies the acceptance gates:

  full   the unit as committed. Gate: mapped flops > 133 -- the irreducible
         envelope slot floor (level_, targetlevel_, inc_, staticcount_
         4x32 b + ix_ 3 b + rising_ + down_: each bit has a distinct
         next-state function and every bit drives slot_rd_data and the
         state_obs fold, H01/H02 observability pattern), plus chip area > 0,
         sequential area > 0, and abc actually mapped (cells > 0). A build
         with state optimized away lands at 0 flops / 0 area and FAILS.
  strip  -DH04_STRIP_OBSERVABILITY: every output tied off. The gate must
         FAIL (yosys deletes every state flop) -- the negative control
         proving the flop gate has resolution, mirroring H01/H02.

Declared-vs-mapped flops are recorded with the reason they may differ
(working registers and observation-driven redundancy can merge; the
contract the mapping must preserve is RTL behavior, proven bit-exact to
the frozen model by tools/h04_compare.py on the pre-synthesis RTL).

What is reported and how it is labelled:
  - cell counts, flop counts, per-cell table, chip area: measured
    (yosys stat -liberty against the named liberty).
  - delay: NOT extractable -- this flow has no STA; abc's delay-driven
    mapper prints no achieved-delay number here. Recorded instead: the
    documented micro-sequencer structure (bounded 13-clock worst-case
    step, measured by the equivalence bench) and yosys ltp's generic-gate
    logic depth (a hint in 2-input-gate levels, NOT nanoseconds). All ns
    timing is UNMEASURED -- clock closure at 12.288/24.576 MHz is H10's
    measurement.

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool or
PDK path -- the message names what is missing). Stdlib only.
"""

import os
import re
import sys

import pdk_resolve
from single_module_stat import parse_single_module_stat

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DEFAULT_CIEL = None  # resolved by pdk_resolve (liberty sha256 pin)
SC_LIB_REL = pdk_resolve.SC_LIB_REL

RTL_REL = "rtl/env_unit.v"
TB_REL = "rtl/tb_env_unit.v"
OUTDIR_REL = "evidence/h04-env-rtl"

# Irreducible committed-state floor for the full-build flop gate: the
# N01 section-6 envelope slot (4x32 b + 3 b + 1 b + 1 b = 133).
ENV_SLOT_FLOOR = 133

# H03 budget row env_steps context (spec/core-contract-v1.json).
BUDGET_REF = {
    "row_id": "env_steps",
    "unit_clks_derived_estimate": 24,
    "count_per_frame": 96,
    "count_basis": "one step per operator per frame (N03 cadence; NUM-005)",
    "measured_worst_step_clks_source": "tools/h04_compare.py bench "
                                       "(max_step_cycles over the directed "
                                       "set incl. worst-rate updates)",
}

# Documented micro-sequencer structure of rtl/env_unit.v.
SEQUENCER_STAGES = [
    {"stage": "S_STEP_CAP", "content": "capture committed slot (the step's "
     "single read point, read-old) + staticcount_ -64 branch eval + "
     "decrement commit (write-new)", "cycles": 1},
    {"stage": "S_ADV_TGT..S_ADV_MUL2", "content": "advance micro-sequence: "
     "scaleoutlevel target + clamp; rising_ + qrate (rate*41>>6 + rate_"
     "scaling, clamp 63); statics hold raw + //20 constant-multiply; "
     "staticcount_ * 15414067 >> 24; inc_ shift + * 15414067 >> 24; commit",
     "cycles": 5},
    {"stage": "S_STEP_MOV", "content": "movement branch: rising jump floor "
     "+ ((17<<24 - level) >> 24)*inc clamp, or falling level - inc clamp; "
     "level commit; optional second advance launch", "cycles": 1},
    {"worst_case": "capture + advance + movement + second advance = 13 "
     "clocks per frame step incl. worst-case segment advances (measured: "
     "see compare_report.json step_cycle_gate)", "cycles": 13},
    {"budget": "H03 derived_estimate 24 clk/op/frame worst case; the "
     "measured 13 closes the contracted update slot with margin", "cycles":
     24},
]


_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402,F401
from sha256_helpers import sha256_file  # noqa: E402,F401
from yosys_run import yosys_version as _yosys_version  # noqa: E402
from probe_synth import (  # noqa: E402
    ProbeSynthSpec, fmt_area, run_probe_synth_cli,
    run_probe_yosys)


def declared_state_bits(repo_root):
    """Sum of the RTL's declared register widths, parsed from the source
    (the audit surface; mapped flops legitimately land below this)."""
    rtl = os.path.join(repo_root, RTL_REL)
    if not os.path.isfile(rtl):
        raise CouldNotRun(f"envelope-unit RTL not found: {rtl}")
    with open(rtl, "r", encoding="utf-8") as f:
        text = f.read()
    total = 0
    detail = {}
    for m in re.finditer(r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*;",
                         text, re.M):
        name = m.group(3)
        width = 1 if m.group(1) is None else \
            int(m.group(1)) - int(m.group(2)) + 1
        detail[name] = detail.get(name, 0) + width
        total += width
    for m in re.finditer(r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*\[0:(\d+)\]\s*;",
                         text, re.M):
        name = m.group(3)
        width = (int(m.group(1)) - int(m.group(2)) + 1) * \
            (int(m.group(4)) + 1)
        detail[name] = detail.get(name, 0) + width
        total += width
    if total == 0:
        raise CheckFailure(f"no register declarations parsed from {RTL_REL}")
    return total, detail


def yosys_version(yosys):
    return _yosys_version(yosys, CouldNotRun)


def run_yosys(yosys, repo_root, liberty, strip, ltp, log_path):
    return run_probe_yosys(yosys, repo_root, RTL_REL, "env_unit",
                           "H04_STRIP_OBSERVABILITY", liberty, strip, ltp,
                           log_path, CheckFailure)


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module only (issue #95): the one per-module area block IS the
    design total only because env_unit has no submodules; a hierarchical
    or multi-block transcript raises CheckFailure. Guard and parsing are
    shared in single_module_stat.py (issue #119).
    """
    return parse_single_module_stat(log, "env_unit", CheckFailure)


def rom_representation(log):
    """How the levellut/statics tables landed, from the transcript
    (measured): yosys proc_rom extracts the case tables as ROMs and
    MEMORY_MAP dissolves them to logic + registers (no BRAM in this flow)."""
    roms = re.findall(r"Mapping memory (\$auto\$proc_rom[^ ]*) in module",
                      log)
    mem_cells = re.search(r"^\s+\d+\s+\S*\$mem\S*\s*$", log, re.M)
    return {
        "representation": "logic",
        "detail": "levellut + statics case tables extracted as ROMs by "
                  "yosys proc_rom (%d found) and dissolved to combinational "
                  "gates + registers by MEMORY_MAP; no BRAM exists in this "
                  "flow and the final stat shows no memory cells" % len(roms),
        "evidence": {"proc_rom_extractions": len(roms),
                     "memory_cells_in_final_stat": bool(mem_cells)},
    }


def _print_summary(gates, runs, declared, declared_extra, out_json):
    for name, g in gates.items():
        print(f"gate {name}: {g['status']}")
    if "full" in runs:
        st = runs["full"]
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared}), cells {st['cell_total']}, chip "
              f"{fmt_area(st['chip_area_um2'])} um2 "
              f"(seq {fmt_area(st['seq_area_um2'])})")
        rom = st["rom_representation"]["representation"]
        print(f"tables mapped as: {rom}")
        if st.get("generic_gate_depth") is not None:
            print(f"ltp generic-gate depth hint: {st['generic_gate_depth']} "
                  f"(not a timing number; no STA in this flow)")
    print(f"report: {out_json}")


SPEC = ProbeSynthSpec(
    description=__doc__.splitlines()[0],
    repo_root=REPO_ROOT,
    outdir_rel=OUTDIR_REL,
    schema="h04-synth-report/1",
    issue=26,
    planning_id="H04",
    claim_boundary="mapped area/cell cost of the envelope unit only; "
                   "no place-and-route, no timing closure, no fit "
                   "claim, no audibility claim; RTL-vs-model "
                   "agreement is tools/h04_compare.py's result",
    constraints="no SDC: probe flow per issue #26 (read_verilog; "
                "synth -top; dfflibmap; abc; ltp; stat), no "
                "clock defined",
    rtl_rel=RTL_REL,
    tb_rel=TB_REL,
    top="env_unit",
    strip_define="H04_STRIP_OBSERVABILITY",
    use_ltp=True,
    flop_floor=ENV_SLOT_FLOOR,
    flop_requirement=f"mapped flops > {ENV_SLOT_FLOOR} (irreducible "
                     "133-bit envelope slot) and chip area > 0 and "
                     "sequential area > 0 and cells > 0",
    harness_key="equivalence_harness",
    harness_value="tools/h04_compare.py (golden: "
                  "src/gf180_dx7/model/envelope.py)",
    yosys_version=yosys_version,
    check_failure=CheckFailure,
    parse_stat=parse_stat,
    rom_representation=rom_representation,
    declared_state=lambda root: (*declared_state_bits(root), {}),
    declared_note="mapped and declared differ (here mapped is larger) "
                  "and the mapping is behavior-preserving: yosys "
                  "proc_rom extracts the levellut/statics case tables "
                  "as ROMs and MEMORY_MAP re-implements them with "
                  "registered logic, while next-state redundancies "
                  "elsewhere merge. The contract the mapping must "
                  "preserve is RTL behavior, proven bit-exact to the "
                  "frozen model by tools/h04_compare.py on the "
                  "pre-synthesis RTL.",
    extra_sections={
        "sequencer_structure": SEQUENCER_STAGES,
        "schedule_context": BUDGET_REF,
    },
    print_summary=_print_summary,
)


def main(argv=None):
    return run_probe_synth_cli(SPEC, argv)


if __name__ == "__main__":
    sys.exit(main())
