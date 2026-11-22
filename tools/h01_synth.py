#!/usr/bin/env python3
"""H01 operator-probe synthesis: mapped area/delay report (issue #20).

Runs yosys over rtl/operator_probe.v against the ciel gf180mcu 7t liberty
(D01-recorded path; the same install H02 measured), maps flops with
dfflibmap and combinational logic with abc, and applies the acceptance
gates (issue #20: "synthesis completes with all outputs observable"; "a
build with outputs optimized away fails acceptance"):

  full   the probe as committed. Gate: mapped flops > 32 -- the 32-bit
         phase accumulator is irreducible (each bit has a distinct
         next-state function and every bit feeds the ROM address and the
         observability reduction), so any build below this floor has lost
         datapath state = FAIL. Chip area > 0 and sequential area > 0 are
         also required (an optimized-away build maps to nothing).
  strip  -DH01_STRIP_OBSERVABILITY: every output tied off. The gate must
         FAIL (yosys deletes every state flop) -- the negative control
         proving the flop gate has resolution, mirroring H02.

Declared-vs-mapped flops are recorded with the reason they differ: the
mapped count is legitimately smaller because yosys exploits ROM-content
constants and sign-extension redundancy (behavior-preserving; the
equivalence harness tools/h01_compare.py proves RTL-vs-model bit-exactness
on the pre-synthesis RTL, which is the contract the mapping must preserve).

What is reported and how it is labelled:
  - cell counts, flop counts, per-cell table, chip area: measured
    (yosys stat -liberty against the named liberty).
  - ROM representation: measured from the transcript (memories dissolve
    to logic gates via MEMORY_MAP; no BRAM in this flow) -- recorded as
    "logic", per the issue's "map to logic or be reported as ROM".
  - delay: NOT extractable -- this flow has no STA; abc's delay-driven
    mapper prints no achieved-delay number here. Recorded instead:
    (a) the documented pipeline structure (3 register stages, 1 eval/clock
    throughput), and (b) yosys ltp's generic-gate logic depth after synth
    (a hint in 2-input-gate levels, NOT nanoseconds). All ns timing is
    UNMEASURED -- clock closure at 12.288/24.576 MHz is H10's measurement.

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

RTL_REL = "rtl/operator_probe.v"
TB_REL = "rtl/tb_operator_probe.v"
OUTDIR_REL = "evidence/h01-operator-probe"

# The phase-accumulator floor for the full-build flop gate (see docstring).
PHASE_FLOP_FLOOR = 32

# N01 schedule reference points (spec/numeric-profile-v1.json "schedule";
# docs/DECISIONS-NUMERIC-v1.md section 7) quoted for the cycles/operator
# context recorded in the report.
SCHEDULE_REF = {
    "operator_evals_per_second": 4608000,
    "operator_evals_per_sample": 96,
    "clock_12m288_hz": 12288000,
    "clock_12m288_clks_per_sample": 256,
    "clock_12m288_all_in_clks_per_op_eval": 2.6667,
    "clock_24m576_hz": 24576000,
    "clock_24m576_clks_per_sample": 512,
    "clock_24m576_all_in_clks_per_op_eval": 5.3333,
    "design_point": "24.576 MHz; 12.288 MHz fallback only if the shared "
                    "operator core measures <= 2 clk/eval",
    "source": "spec/numeric-profile-v1.json schedule; "
              "DECISIONS-NUMERIC-v1.md section 7",
}

# Documented pipeline structure of rtl/operator_probe.v (probe stages).
PIPELINE_STAGES = [
    {"stage": 1, "content": "phi fold (mirror XOR + quadrant sign) + "
                            "sinlog ROM read + env capture",
     "registers": "s1_log[15:0], s1_env[15:0], s1_valid"},
    {"stage": 2, "content": "log-domain level add (mod 2^16) + sign split + "
                            "sinexp ROM read + shift capture",
     "registers": "s2_exp[15:0], s2_shift[4:0], s2_sign, s2_valid"},
    {"stage": 3, "content": "4096 bias + >>shift + sign reconstruct "
                            "(~result<<13) + >>>4 / clip(+-2^24) / >>>9",
     "registers": "out_q[31:0], clip_q[16:0], out_valid_q"},
    {"phase": "phase register", "content": "phase <= ld_phase ? phase_in : "
                                           "phase_q + freq (mod 2^32)",
     "registers": "phase_q[31:0]"},
    {"observability": "state_obs", "content": "registered XOR reduction of "
                                              "all state bits",
     "registers": "obs_q"},
]


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
    (the audit surface; mapped flops legitimately land below this -- see
    module docstring)."""
    rtl = os.path.join(repo_root, RTL_REL)
    if not os.path.isfile(rtl):
        raise CouldNotRun(f"probe RTL not found: {rtl}")
    with open(rtl, "r", encoding="utf-8") as f:
        text = f.read()
    total = 0
    detail = {}
    for m in re.finditer(r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*;",
                         text, re.M):
        name = m.group(3)
        if m.group(1) is None:
            width = 1
        else:
            width = int(m.group(1)) - int(m.group(2)) + 1
        detail[name] = width
        total += width
    if total == 0:
        raise CheckFailure(f"no register declarations parsed from {RTL_REL}")
    return total, detail


def yosys_version(yosys):
    return _yosys_version(yosys, CouldNotRun)


def run_yosys(yosys, repo_root, liberty, strip, ltp, log_path):
    return run_probe_yosys(yosys, repo_root, RTL_REL, "operator_probe",
                           "H01_STRIP_OBSERVABILITY", liberty, strip, ltp,
                           log_path, CheckFailure)


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module only (issue #95): the one per-module area block IS the
    design total only because operator_probe has no submodules; a hierarchical
    or multi-block transcript raises CheckFailure. Guard and parsing are
    shared in single_module_stat.py (issue #119).
    """
    return parse_single_module_stat(log, "operator_probe", CheckFailure)


ROM_DETAIL = (
    "both 1024x16 ROMs dissolve to combinational gates (yosys"
    " MEMORY_MAP; no BRAM in this flow); final stat shows no "
    "memory cells")


def rom_representation(log):
    """How the table memories landed, from the transcript (measured)."""
    return shared_rom_representation(log, ROM_DETAIL, readmem=True)


def _print_summary(gates, runs, declared, declared_extra, out_json):
    for name, g in gates.items():
        print(f"gate {name}: {g['status']}")
    if "full" in runs:
        st = runs["full"]
        rom = st["rom_representation"]["representation"]
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared}), cells {st['cell_total']}, chip "
              f"{st['chip_area_um2']:.1f} um2 "
              f"(seq {st['seq_area_um2']:.1f}), ROM as {rom}")
        if st.get("generic_gate_depth") is not None:
            print(f"ltp generic-gate depth hint: {st['generic_gate_depth']} "
                  f"(not a timing number; no STA in this flow)")
    print(f"report: {out_json}")


SPEC = ProbeSynthSpec(
    description=__doc__.splitlines()[0],
    repo_root=REPO_ROOT,
    outdir_rel=OUTDIR_REL,
    schema="h01-synth-report/1",
    issue=20,
    planning_id="H01",
    claim_boundary="mapped area/cell cost of the probe only; no "
                   "place-and-route, no timing closure, no fit "
                   "claim, no audibility claim; RTL-vs-model "
                   "agreement is tools/h01_compare.py's result",
    constraints="no SDC: probe flow per issue #20 (read_verilog; "
                "synth -top; dfflibmap; abc; ltp; stat), no "
                "clock defined",
    rtl_rel=RTL_REL,
    tb_rel=TB_REL,
    top="operator_probe",
    strip_define="H01_STRIP_OBSERVABILITY",
    use_ltp=True,
    flop_floor=PHASE_FLOP_FLOOR,
    flop_requirement=f"mapped flops > {PHASE_FLOP_FLOOR} (irreducible "
                     "32-bit phase accumulator) and chip area > 0 and "
                     "sequential area > 0 and cells > 0",
    harness_key="equivalence_harness",
    harness_value="tools/h01_compare.py (golden: "
                  "src/gf180_dx7/model/operator.py)",
    yosys_version=yosys_version,
    check_failure=CheckFailure,
    parse_stat=parse_stat,
    rom_representation=rom_representation,
    declared_state=lambda root: (*declared_state_bits(root), {}),
    declared_note="mapped < declared is expected and behavior-preserving: "
                  "yosys exploits ROM-content constants (e.g. sinlog "
                  "entries never set bit 15) and sign-extension "
                  "redundancy of out_q/clip_q, replacing redundant flops "
                  "with wires. The contract the mapping must preserve is "
                  "RTL behavior, proven bit-exact to the frozen model by "
                  "tools/h01_compare.py.",
    extra_sections={
        "pipeline_structure": PIPELINE_STAGES,
        "cycles_per_operator": {
            "throughput_clks_per_eval": 1,
            "latency_clks": 3,
            "note": "one evaluation accepted per clock once the 3-stage "
                    "pipeline is full (measured by the equivalence "
                    "harness: every eval stream sustains 1 eval/clk)",
            "schedule_context": SCHEDULE_REF,
            "budget_input": "the kernel's 1 clk/eval leaves 1.67 clk/eval "
                            "(96 of 256 used) at 12.288 MHz and 4.33 "
                            "clk/eval (96 of 512) at 24.576 MHz for state "
                            "access, control and output -- the N01 section-7 "
                            "all-in question; whether the shared core with "
                            "H02 storage sustains <= 2.67 stays unproven "
                            "here (that schedule is H03/H10)",
        },
    },
    print_summary=_print_summary,
)


def main(argv=None):
    return run_probe_synth_cli(SPEC, argv)


if __name__ == "__main__":
    sys.exit(main())
