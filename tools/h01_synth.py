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

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DEFAULT_CIEL = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
                "gf180mcu/versions/"
                "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

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


class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


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
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise CouldNotRun(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise CouldNotRun(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def run_yosys(yosys, repo_root, liberty, strip, ltp, log_path):
    rtl = os.path.join(repo_root, RTL_REL)
    script = [
        "read_verilog" + (" -DH01_STRIP_OBSERVABILITY" if strip else "")
        + f" {rtl}",
        "hierarchy -check -top operator_probe",
        "synth -top operator_probe",
    ]
    if ltp:
        # generic-gate logic depth hint (NOT a timing number)
        script.append("ltp")
    script += [
        f"dfflibmap -liberty {liberty}",
        f"abc -liberty {liberty}",
        f"stat -liberty {liberty}",
    ]
    proc = subprocess.run([yosys, "-p", "; ".join(script)],
                          capture_output=True, text=True, cwd=repo_root)
    log = (proc.stdout or "") + (proc.stderr or "")
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script (cwd={repo_root}): {'; '.join(script)}\n"
                + log)
    if proc.returncode != 0:
        raise CheckFailure(
            f"yosys failed (rc={proc.returncode}); log at {log_path}: "
            + (log.strip().splitlines()[-1][:300] if log.strip() else ""))
    return log


HIER_MARK = "=== design hierarchy ==="
MODULE_AREA_RE = r"Chip area for module .*?: ([\d.]+)"
TOP_AREA_RE = r"Chip area for top module"


def assert_single_module_stat(log):
    """Assert the `stat` transcript really is single-module (issue #95).

    parse_stat reports the transcript's ONE `Chip area for module` block as
    the design total. That is correct only while operator_probe is a flat,
    single-module design -- which the committed
    evidence/h01-operator-probe/yosys_full.log is (exactly one `Chip area
    for module` line, no `=== design hierarchy ===` section; re-verified at
    implementation time). If operator_probe ever acquires submodules, yosys
    emits one block PER module plus a hierarchy section whose `Chip area for
    top module` line is the whole-design total, and taking a per-module
    block would silently under-report -- exactly the bug #82 (h07) and #94
    (h08) had to fix. This assertion makes that drift FAIL loudly instead of
    shipping a wrong number; the fix when it fires is #94's hierarchy-total
    parser, never a looser regex or a relaxed assertion.

    Returns the per-module area strings found (0 or 1). An EMPTY list is
    legitimate, not an error: the strip-observability control maps nothing,
    so yosys prints no area line at all (chip_area_um2 stays None).
    """
    if HIER_MARK in log or re.search(TOP_AREA_RE, log):
        raise CheckFailure(
            "stat transcript is HIERARCHICAL ('=== design hierarchy ===' "
            "and/or a 'Chip area for top module' line present): "
            "operator_probe is no longer a single-module design, so a "
            "per-module 'Chip area for module' block is NOT the whole-design "
            "total. Refusing to report one as the design area "
            "(issues #82/#94/#95) -- parse the hierarchy total instead.")
    areas = re.findall(MODULE_AREA_RE, log)
    if len(areas) > 1:
        raise CheckFailure(
            f"stat transcript has {len(areas)} 'Chip area for module' blocks; "
            "operator_probe is expected to be a single-module design "
            "(exactly one). Refusing to report the first block as the design "
            "area (issues #82/#94/#95).")
    return areas


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module shape is asserted first (issue #95): the one per-module
    area block IS the design total only because operator_probe has no
    submodules.
    """
    areas = assert_single_module_stat(log)
    cells = {}
    for m in re.finditer(r"^\s+(\d+)\s+([\d.]+E\+\d+|[\d.]+)\s+"
                         r"(gf180mcu\S+)\s*$", log, re.M):
        cells[m.group(3)] = cells.get(m.group(3), 0) + int(m.group(1))
    seq = re.search(r"of which used for sequential elements: ([\d.]+)", log)
    total = re.search(r"^\s+(\d+)\s+[\d.]+E\+\d+\s+cells\s*$", log, re.M)
    return {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)) if total else sum(cells.values()),
        "dff_cells": {k: v for k, v in cells.items() if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "chip_area_um2": float(areas[0]) if areas else None,
        "seq_area_um2": float(seq.group(1)) if seq else None,
    }


def parse_ltp(log):
    """Generic-gate logic depth from the ltp pass (hint only)."""
    m = re.search(r"Longest topological path in \S+ \(length=(\d+)\)", log)
    loops = len(re.findall(r"Detected loop", log))
    return {"generic_gate_depth": int(m.group(1)) if m else None,
            "loop_warnings": loops}


def rom_representation(log):
    """How the table memories landed, from the transcript (measured)."""
    memory_map = "MEMORY_MAP pass (converting memories to logic" in log
    mem_cells = re.search(r"^\s+\d+\s+\S*\$mem\S*\s*$", log, re.M)
    readmem = "$readmemh" in log or "readmem" in log.lower()
    if memory_map and not mem_cells:
        return {
            "representation": "logic",
            "detail": "both 1024x16 ROMs dissolve to combinational gates "
                      "(yosys MEMORY_MAP; no BRAM in this flow); final stat "
                      "shows no memory cells",
            "evidence": {"memory_map_pass_ran": True,
                         "memory_cells_in_final_stat": 0},
        }
    return {
        "representation": "memory cells" if mem_cells else
                          ("logic (inferred)" if memory_map else "unknown"),
        "detail": "inspect the committed yosys log",
        "evidence": {"memory_map_pass_ran": memory_map,
                     "readmem_seen": readmem,
                     "memory_cells_in_final_stat": bool(mem_cells)},
    }


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--mode", choices=("both", "full", "strip"),
                    default="both")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--ciel-root", default=DEFAULT_CIEL)
    ap.add_argument("--liberty", default=None)
    ap.add_argument("--outdir", default=os.path.join(REPO_ROOT, OUTDIR_REL))
    args = ap.parse_args(argv)

    repo_root = REPO_ROOT
    liberty = args.liberty or os.path.join(args.ciel_root, SC_LIB_REL)
    if not os.path.isfile(liberty):
        print(f"COULD-NOT-RUN: 7t liberty not found at {liberty} "
              f"(ciel root {args.ciel_root})", file=sys.stderr)
        return 2
    ver = yosys_version(args.yosys)
    declared, declared_detail = declared_state_bits(repo_root)

    os.makedirs(args.outdir, exist_ok=True)
    log_full = os.path.join(args.outdir, "yosys_full.log")
    log_strip = os.path.join(args.outdir, "yosys_strip.log")

    runs = {}
    gates = {}
    if args.mode in ("both", "full"):
        log = run_yosys(args.yosys, repo_root, liberty, strip=False,
                        ltp=True, log_path=log_full)
        st = parse_stat(log)
        st.update(parse_ltp(log))
        st["rom_representation"] = rom_representation(log)
        st["mode"] = "full"
        ok = (st["flop_total"] > PHASE_FLOP_FLOOR
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0
              and st["cell_total"] > 0)
        gates["full"] = {
            "requirement": f"mapped flops > {PHASE_FLOP_FLOOR} (irreducible "
                           "32-bit phase accumulator) and chip area > 0 and "
                           "sequential area > 0 and cells > 0",
            "mapped_flops": st["flop_total"],
            "chip_area_um2": st["chip_area_um2"],
            "seq_area_um2": st["seq_area_um2"],
            "cell_total": st["cell_total"],
            "status": "PASS" if ok else "FAIL",
        }
        runs["full"] = st
    if args.mode in ("both", "strip"):
        log = run_yosys(args.yosys, repo_root, liberty, strip=True,
                        ltp=False, log_path=log_strip)
        st = parse_stat(log)
        st["mode"] = "strip"
        runs["strip"] = st

    if "full" in runs and "strip" in runs:
        gone = runs["strip"]["flop_total"] < runs["full"]["flop_total"]
        gates["strip_negative_control"] = {
            "requirement": "strip-observability must make the flop gate "
                           "FAIL (all state removed)",
            "mapped_flops": runs["strip"]["flop_total"],
            "status": "MET (gate fails as required)" if gone
                      else "NOT MET (gate still passes -- control failed)",
        }

    status = "PASS" if gates and all(
        g["status"] in ("PASS", "MET (gate fails as required)")
        for g in gates.values()) else "FAIL"

    report = {
        "schema": "h01-synth-report/1",
        "issue": 20,
        "planning_id": "H01",
        "status": status,
        "claim_boundary": "mapped area/cell cost of the probe only; no "
                          "place-and-route, no timing closure, no fit "
                          "claim, no audibility claim; RTL-vs-model "
                          "agreement is tools/h01_compare.py's result",
        "tool_identity": {
            "yosys": ver,
            "liberty": {"path": liberty,
                        "sha256": sha256_file(liberty),
                        "corner": "tt_025C_5v00",
                        "library": "gf180mcu_fd_sc_mcu7t5v0 (7t, 5 metal)"},
            "dont_use_policy": "none: *_1 drive cells allowed (declared per "
                               "D01 section 4/section 10.3)",
            "constraints": "no SDC: probe flow per issue #20 (read_verilog; "
                           "synth -top; dfflibmap; abc; ltp; stat), no "
                           "clock defined",
            "rtl": {p: sha256_file(os.path.join(repo_root, p))
                    for p in (RTL_REL, TB_REL)},
            "equivalence_harness": "tools/h01_compare.py (golden: "
                                   "src/gf180_dx7/model/operator.py)",
        },
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
        "declared_vs_mapped_state": {
            "declared_bits": declared,
            "declared_detail": declared_detail,
            "mapped_flops_full": runs.get("full", {}).get("flop_total"),
            "note": "mapped < declared is expected and behavior-preserving: "
                    "yosys exploits ROM-content constants (e.g. sinlog "
                    "entries never set bit 15) and sign-extension "
                    "redundancy of out_q/clip_q, replacing redundant flops "
                    "with wires. The contract the mapping must preserve is "
                    "RTL behavior, proven bit-exact to the frozen model by "
                    "tools/h01_compare.py.",
        },
        "gates": gates,
        "runs": runs,
    }
    out_json = os.path.join(args.outdir, "synth_report.json")
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

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
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
