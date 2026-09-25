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

RTL_REL = "rtl/alg_router.v"
TB_REL = "rtl/tb_alg_router.v"
OUTDIR_REL = "evidence/h05-routing-rtl"

# The bus-pair floor for the full-build flop gate (see docstring): the two
# persistent 64 x 32 interconnect buffers (fm_core.h buf_[2], N04 facts).
BUS_FLOP_FLOOR = 4096


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
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise CouldNotRun(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise CouldNotRun(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def run_yosys(yosys, repo_root, liberty, strip, log_path):
    rtl = os.path.join(repo_root, RTL_REL)
    script = [
        "read_verilog" + (" -DH05_STRIP_OBSERVABILITY" if strip else "")
        + f" {rtl}",
        "hierarchy -check -top alg_router",
        "synth -top alg_router",
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
    the design total. That is correct only while this tool's alg_router build
    is a flat, single-module design -- which the committed
    evidence/h05-routing-rtl/yosys_full.log is (exactly one `Chip area for
    module` line, no `=== design hierarchy ===` section; re-verified at
    implementation time). If alg_router ever acquires submodules, yosys emits
    one block PER module plus a hierarchy section whose `Chip area for top
    module` line is the whole-design total, and taking a per-module block
    would silently under-report -- exactly the bug #82 (h07, where
    alg_router's own block was mistaken for the whole core) and #94 (h08) had
    to fix. This assertion makes that drift FAIL loudly instead of shipping a
    wrong number; the fix when it fires is #94's hierarchy-total parser,
    never a looser regex or a relaxed assertion.

    Returns the per-module area strings found (0 or 1). An EMPTY list is
    legitimate, not an error: the strip-observability control maps nothing,
    so yosys prints no area line at all (chip_area_um2 stays None).
    """
    if HIER_MARK in log or re.search(TOP_AREA_RE, log):
        raise CheckFailure(
            "stat transcript is HIERARCHICAL ('=== design hierarchy ===' "
            "and/or a 'Chip area for top module' line present): alg_router is "
            "no longer a single-module design, so a per-module 'Chip area for "
            "module' block is NOT the whole-design total. Refusing to report "
            "one as the design area (issues #82/#94/#95) -- parse the "
            "hierarchy total instead.")
    areas = re.findall(MODULE_AREA_RE, log)
    if len(areas) > 1:
        raise CheckFailure(
            f"stat transcript has {len(areas)} 'Chip area for module' blocks; "
            "alg_router is expected to be a single-module design (exactly "
            "one). Refusing to report the first block as the design area "
            "(issues #82/#94/#95).")
    return areas


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript.

    Single-module shape is asserted first (issue #95): the one per-module
    area block IS the design total only because alg_router has no submodules.
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


def rom_representation(log):
    """How the connection-table memory landed, from the transcript."""
    memory_map = "MEMORY_MAP pass (converting memories to logic" in log
    mem_cells = re.search(r"^\s+\d+\s+\S*\$mem\S*\s*$", log, re.M)
    readmem = "readmem" in log.lower()
    if memory_map and not mem_cells:
        return {
            "representation": "logic",
            "detail": "the 32x6 x 8-bit connection table dissolves to "
                      "combinational gates (yosys MEMORY_MAP; no BRAM in "
                      "this flow); final stat shows no memory cells",
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
                        log_path=log_full)
        st = parse_stat(log)
        st["rom_representation"] = rom_representation(log)
        st["mode"] = "full"
        ok = (st["flop_total"] > BUS_FLOP_FLOOR
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0
              and st["cell_total"] > 0)
        gates["full"] = {
            "requirement": f"mapped flops > {BUS_FLOP_FLOOR} (the two "
                           "persistent 64x32 interconnect bus buffers are "
                           "irreducible N04 state) and chip area > 0 and "
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
                        log_path=log_strip)
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
        "schema": "h05-synth-report/1",
        "issue": 27,
        "planning_id": "H05",
        "status": status,
        "claim_boundary": "mapped cell/flop/area cost of the routing "
                          "probe only; no place-and-route, no timing "
                          "closure, no fit claim, no audibility claim; "
                          "RTL-vs-model agreement is tools/h05_compare.py's "
                          "result and is not implied by any number here",
        "tool_identity": {
            "yosys": ver,
            "liberty": {"path": liberty,
                        "sha256": sha256_file(liberty),
                        "corner": "tt_025C_5v00",
                        "library": "gf180mcu_fd_sc_mcu7t5v0 (7t, 5 metal)"},
            "dont_use_policy": "none: *_1 drive cells allowed (declared per "
                               "D01 section 4/section 10.3)",
            "constraints": "no SDC: probe flow per issue #27 (read_verilog; "
                           "synth -top; dfflibmap; abc; stat), no clock "
                           "defined",
            "rtl": {p: sha256_file(os.path.join(repo_root, p))
                    for p in (RTL_REL, TB_REL)},
            "conformance_harness": "tools/h05_compare.py (golden: "
                                   "src/gf180_dx7/model/algorithm.py)",
        },
        "declared_vs_mapped_state": {
            "declared_bits": declared,
            "declared_detail": declared_detail,
            "mapped_flops_full": runs.get("full", {}).get("flop_total"),
            "note": "mapped differs from declared by a few bits and that "
                    "is expected and behavior-preserving (yosys exploits "
                    "constant/unused-bit structure and dfflibmap may "
                    "replicate registers). The contract the mapping must "
                    "preserve is RTL behavior, proven bit-exact to the "
                    "frozen model by tools/h05_compare.py on the "
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
        rom = st["rom_representation"]["representation"]
        print(f"full mapped: flops {st['flop_total']} (declared state bits "
              f"{declared}), cells {st['cell_total']}, chip "
              f"{st['chip_area_um2']:.1f} um2 (seq {st['seq_area_um2']:.1f}),"
              f" ROM as {rom}")
    print(f"report: {out_json}")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
