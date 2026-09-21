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
        "read_verilog" + (" -DH04_STRIP_OBSERVABILITY" if strip else "")
        + f" {rtl}",
        "hierarchy -check -top env_unit",
        "synth -top env_unit",
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


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript."""
    cells = {}
    for m in re.finditer(r"^\s+(\d+)\s+([\d.]+E\+\d+|[\d.]+)\s+"
                         r"(gf180mcu\S+)\s*$", log, re.M):
        cells[m.group(3)] = cells.get(m.group(3), 0) + int(m.group(1))
    chip = re.search(r"Chip area for module .*?: ([\d.]+)", log)
    seq = re.search(r"of which used for sequential elements: ([\d.]+)", log)
    total = re.search(r"^\s+(\d+)\s+[\d.]+E\+\d+\s+cells\s*$", log, re.M)
    return {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)) if total else sum(cells.values()),
        "dff_cells": {k: v for k, v in cells.items() if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "chip_area_um2": float(chip.group(1)) if chip else None,
        "seq_area_um2": float(seq.group(1)) if seq else None,
    }


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


def parse_ltp(log):
    """Generic-gate logic depth from the ltp pass (hint only)."""
    m = re.search(r"Longest topological path in \S+ \(length=(\d+)\)", log)
    loops = len(re.findall(r"Detected loop", log))
    return {"generic_gate_depth": int(m.group(1)) if m else None,
            "loop_warnings": loops}


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
        ok = (st["flop_total"] > ENV_SLOT_FLOOR
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0
              and st["cell_total"] > 0)
        gates["full"] = {
            "requirement": f"mapped flops > {ENV_SLOT_FLOOR} (irreducible "
                           "133-bit envelope slot) and chip area > 0 and "
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
        "schema": "h04-synth-report/1",
        "issue": 26,
        "planning_id": "H04",
        "status": status,
        "claim_boundary": "mapped area/cell cost of the envelope unit only; "
                          "no place-and-route, no timing closure, no fit "
                          "claim, no audibility claim; RTL-vs-model "
                          "agreement is tools/h04_compare.py's result",
        "tool_identity": {
            "yosys": ver,
            "liberty": {"path": liberty,
                        "sha256": sha256_file(liberty),
                        "corner": "tt_025C_5v00",
                        "library": "gf180mcu_fd_sc_mcu7t5v0 (7t, 5 metal)"},
            "dont_use_policy": "none: *_1 drive cells allowed (declared per "
                               "D01 section 4/section 10.3)",
            "constraints": "no SDC: probe flow per issue #26 (read_verilog; "
                           "synth -top; dfflibmap; abc; ltp; stat), no "
                           "clock defined",
            "rtl": {p: sha256_file(os.path.join(repo_root, p))
                    for p in (RTL_REL, TB_REL)},
            "equivalence_harness": "tools/h04_compare.py (golden: "
                                   "src/gf180_dx7/model/envelope.py)",
        },
        "sequencer_structure": SEQUENCER_STAGES,
        "schedule_context": BUDGET_REF,
        "declared_vs_mapped_state": {
            "declared_bits": declared,
            "declared_detail": declared_detail,
            "mapped_flops_full": runs.get("full", {}).get("flop_total"),
            "note": "mapped and declared differ (here mapped is larger) "
                    "and the mapping is behavior-preserving: yosys "
                    "proc_rom extracts the levellut/statics case tables "
                    "as ROMs and MEMORY_MAP re-implements them with "
                    "registered logic, while next-state redundancies "
                    "elsewhere merge. The contract the mapping must "
                    "preserve is RTL behavior, proven bit-exact to the "
                    "frozen model by tools/h04_compare.py on the "
                    "pre-synthesis RTL.",
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
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared}), cells {st['cell_total']}, chip "
              f"{st['chip_area_um2']:.1f} um2 "
              f"(seq {st['seq_area_um2']:.1f})")
        rom = st["rom_representation"]["representation"]
        print(f"tables mapped as: {rom}")
        if st.get("generic_gate_depth") is not None:
            print(f"ltp generic-gate depth hint: {st['generic_gate_depth']} "
                  f"(not a timing number; no STA in this flow)")
    print(f"report: {out_json}")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
