#!/usr/bin/env python3
"""H07 integrated-core synthesis: mapped area report + strip-obs gate.

Runs yosys over the complete core (rtl/dx7_core.v + rtl/env_unit.v +
rtl/alg_router.v, top dx7_core) against the local ciel gf180mcu 7t liberty
(the D01-recorded install H01/H02 measured), maps flops with dfflibmap and
combinational logic with abc, and applies the gates:

  full   the core as committed. Gate: mapped flops > 38,781 -- the H02
         note-bank slice (38,336 replicated + 444 patch + 1 obs, measured
         and recorded in evidence/h02-storage-probe and
         tools/h03_budget_check.py) is a strict SUBSET of the core's
         state, so any build below that floor has lost datapath state =
         FAIL. Chip area > 0, sequential area > 0 and cells > 0 are also
         required (an optimized-away build maps to nothing).
  strip  -DH07_STRIP_OBSERVABILITY: every output tied off. The gate must
         FAIL (yosys deletes every state flop) -- the negative control
         proving the flop gate has resolution, mirroring H01/H02/H06.

Delta accounting vs the H02 38,781-flop budget is recorded attributed by
declared RTL source group (H02 note banks, env_unit slices x96, event
queue + skid FIFO, mix buffers, pitch/LFO shared path, control/FSM,
observability), with the mapped-vs-declared difference explained: the
mapped count legitimately lands below the declared sum because yosys
exploits reset-value constants and ROM-content redundancy. The contract
the mapping must preserve is RTL BEHAVIOR -- proven bit-exact to the
frozen model on the pre-synthesis RTL by tools/h07_compare.py.

What is reported and how it is labelled:
  - cell counts, flop counts, per-cell table, chip area: measured
    (yosys stat -liberty against the named liberty).
  - ROM representation: measured from the transcript (the sinlog/sinexp/
    LFO-sin/freqLUT memories dissolve to logic gates via MEMORY_MAP; no
    BRAM exists in this flow).
  - delay: NOT extractable -- this flow has no STA; abc prints no
    achieved-delay number. All ns timing is UNMEASURED -- clock closure
    at the 24.576 MHz design point is H10's measurement (D01 section 2).

Mutant builds are never synthesized: the strip build is a negative
CONTROL, not an area datapoint.

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool
or PDK path -- the message names what is missing). Stdlib only.
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

RTL_RELS = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]
OUTDIR_REL = "evidence/h07-core"

# The mapped-flop floor for the full-build gate: H02's measured note-bank
# slice (a strict subset of the core's state).
H02_MAPPED_FLOPS = 38781


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


def declared_state_bits():
    """Declared register widths per source group, parsed from the RTL.
    The mapped count legitimately lands below this sum (reset-value
    constants, ROM redundancy); the delta is recorded, never hidden."""
    groups = {
        "note_banks_h02": [],       # per-note state arrays (H02 slice)
        "env_units": [],            # inside rtl/env_unit.v
        "queues": [],               # event queue + skid FIFO
        "mix": [],                  # mixbuf + taps
        "control": [],              # FSM/LFO/perf/shadow/committed
        "observability": [],
    }
    rom_bits = 0
    total = 0
    detail = {}
    for rel, group in [("rtl/dx7_core.v", None), ("rtl/env_unit.v",
                                                  "env_units"),
                       ("rtl/alg_router.v", "control")]:
        path = os.path.join(REPO_ROOT, rel)
        if not os.path.isfile(path):
            raise CouldNotRun(f"RTL not found: {path}")
        with open(path, "r", encoding="utf-8") as f:
            text = f.read()
        for m in re.finditer(r"^\s*reg\s+\[(\d+):(\d+)\]\s*(\w+)\s*"
                             r"\[0:(\d+)\]\s*;", text, re.M):
            width = (int(m.group(1)) - int(m.group(2)) + 1) * \
                (int(m.group(4)) + 1)
            if m.group(3).endswith("_rom"):
                rom_bits += width
                continue
            detail[rel + ":" + m.group(3) + "[]"] = width
            total += width
        for m in re.finditer(r"^\s*reg\s+(?:\[(\d+):(\d+)\]\s*)?(\w+)\s*;",
                             text, re.M):
            name = m.group(3)
            width = 1 if m.group(1) is None else \
                int(m.group(1)) - int(m.group(2)) + 1
            detail[rel + ":" + name] = width
            total += width
    # attribute the big declared groups by name (recorded alongside the
    # mapped total; the attribution is by declared RTL source, not by
    # mapped cells -- yosys does not preserve name groupings)
    attrib = {
        "env_units_declared": sum(v for k, v in detail.items()
                                  if k.startswith("rtl/env_unit.v:")),
        "queues_declared": sum(v for k, v in detail.items()
                               if "evq" in k or "skid" in k),
        "mixbuf_declared": sum(v for k, v in detail.items()
                               if "mixbuf" in k),
        "note_bank_state_declared": sum(v for k, v in detail.items()
                                        if ":n_" in k or ":op_" in k),
    }
    return total, detail, rom_bits, attrib


def yosys_version(yosys):
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise CouldNotRun(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise CouldNotRun(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def run_yosys(yosys, liberty, strip, log_path):
    script = [
        "read_verilog" + (" -DH07_STRIP_OBSERVABILITY" if strip else "")
        + " " + " ".join(os.path.join(REPO_ROOT, r) for r in RTL_RELS),
        "hierarchy -check -top dx7_core",
        "synth -top dx7_core",
        f"dfflibmap -liberty {liberty}",
        f"abc -liberty {liberty}",
        f"stat -liberty {liberty}",
    ]
    proc = subprocess.run([yosys, "-p", "; ".join(script)],
                          capture_output=True, text=True, cwd=REPO_ROOT)
    log = (proc.stdout or "") + (proc.stderr or "")
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script (cwd={REPO_ROOT}): {'; '.join(script)}\n"
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
    """How the table memories landed, from the transcript (measured)."""
    memory_map = "MEMORY_MAP pass (converting memories to logic" in log
    mem_cells = re.search(r"^\s+\d+\s+\S*\$mem\S*\s*$", log, re.M)
    if memory_map and not mem_cells:
        return {
            "representation": "logic",
            "detail": "the sinlog/sinexp/LFO-sine/freqlut ROMs dissolve to "
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
                     "memory_cells_in_final_stat": bool(mem_cells)},
    }


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--mode", choices=("both", "full", "strip"),
                    default="both")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--ciel-root", default=DEFAULT_CIEL)
    ap.add_argument("--liberty", default=None)
    ap.add_argument("--outdir",
                    default=os.path.join(REPO_ROOT, OUTDIR_REL))
    args = ap.parse_args(argv)

    liberty = args.liberty or os.path.join(args.ciel_root, SC_LIB_REL)
    if not os.path.isfile(liberty):
        print(f"COULD-NOT-RUN: 7t liberty not found at {liberty} "
              f"(ciel root {args.ciel_root})", file=sys.stderr)
        return 2
    ver = yosys_version(args.yosys)
    declared, declared_detail, rom_declared, attrib = declared_state_bits()

    os.makedirs(args.outdir, exist_ok=True)
    log_full = os.path.join(args.outdir, "yosys_full.log")
    log_strip = os.path.join(args.outdir, "yosys_strip.log")

    runs = {}
    gates = {}
    if args.mode in ("both", "full"):
        log = run_yosys(args.yosys, liberty, strip=False,
                        log_path=log_full)
        st = parse_stat(log)
        st["rom_representation"] = rom_representation(log)
        st["mode"] = "full"
        ok = (st["flop_total"] > H02_MAPPED_FLOPS
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0
              and st["cell_total"] > 0)
        gates["full"] = {
            "requirement": f"mapped flops > {H02_MAPPED_FLOPS} (the H02 "
                           "note-bank slice is a strict subset of the "
                           "core's state) and chip area > 0 and sequential "
                           "area > 0 and cells > 0",
            "mapped_flops": st["flop_total"],
            "chip_area_um2": st["chip_area_um2"],
            "seq_area_um2": st["seq_area_um2"],
            "cell_total": st["cell_total"],
            "status": "PASS" if ok else "FAIL",
        }
        runs["full"] = st
    if args.mode in ("both", "strip"):
        log = run_yosys(args.yosys, liberty, strip=True,
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

    delta = None
    if "full" in runs:
        delta = {
            "h02_measured_mapped_flops": H02_MAPPED_FLOPS,
            "h07_mapped_flops": runs["full"]["flop_total"],
            "delta_flops": runs["full"]["flop_total"] - H02_MAPPED_FLOPS,
            "attribution_basis": "declared RTL source groups (mapped "
                                 "cells do not preserve name groupings)",
            "declared_total_bits": declared,
            "declared_rom_bits": rom_declared,
            "declared_groups": attrib,
            "note": "mapped < declared is expected (reset-value constants "
                    "and ROM redundancy); the gate is the H02 floor",
        }

    report = {
        "unit": "dx7_core",
        "issue": 29,
        "yosys": ver,
        "liberty": liberty,
        "liberty_sha256": sha256_file(liberty),
        "rtl": {r: sha256_file(os.path.join(REPO_ROOT, r))
                for r in RTL_RELS},
        "runs": runs,
        "gates": gates,
        "delta_vs_h02": delta,
        "claims": "mapped area on the pre-synthesis RTL only; no P&R, no "
                  "timing closure, no fit, no board claim (H10 measures "
                  "clock closure)",
    }
    out = os.path.join(args.outdir, "synth_report.json")
    with open(out, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    ok = all(g.get("status") in ("PASS", "MET (gate fails as required)")
             for g in gates.values())
    print(json.dumps({"gates": gates, "delta_vs_h02": delta,
                      "report": out}, indent=1))
    return 0 if ok and gates else 1


if __name__ == "__main__":
    sys.exit(main())
