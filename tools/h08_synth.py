#!/usr/bin/env python3
"""H08 chassis synthesis: passive-wrapper overhead vs the H07 core build.

issue #30 acceptance: "the synthesized chassis meets resource targets --
chassis overhead vs the core evidenced, not estimated". H08 adds
rtl/synth_top.v, a PIN-NET-ONLY wrapper over the hash-pinned H07 core
(rtl/dx7_core.v + rtl/env_unit.v + rtl/alg_router.v, unchanged; one
instance, every port tied 1:1 to a pad, no clock generation, no reset
sync, no glue logic). The expected measurement is therefore ZERO added
state and ZERO added cells vs the committed H07 full build
(evidence/h07-core/synth_report.json, 138,490 mapped flops / 36,803 cells
/ 1,158,154.592 um2 chip area at the ciel gf180mcu 7t liberty).

Runs yosys over the chassis (top synth_top) with the same script shape as
tools/h07_synth.py (read_verilog -sv, hierarchy -check, synth, dfflibmap,
abc, stat -liberty) and applies gates. Framing note: H07's committed full
build synthesized the core AS THE TOP (top dx7_core, every core port
top-exposed). The chassis top (synth_top) exposes only the 9 pads, so the
core's unconnected tap/status/debug output ports lose their top exposure
and yosys prunes their driver logic. A passive pin map therefore can only
NEUTRALLY CHASE OR REMOVE top-exposed logic, never add any:

  full   the chassis as committed. Gates:
         (a) mapped flops stay above the H02 38,781 floor (state present,
             not optimized away); (b) mapped flops AND cell total do NOT
             EXCEED the committed H07 core full build (138,490 flops /
             36,803 cells) -- a passive wrapper adds no state or logic;
             any shortfall vs H07 is reported as attributed
             connection-state pruning of the unconnected tap/status/debug
             outputs (culled), never as overhead; (c) the committed
             wrapper source is structurally a pin map (exactly one
             dx7_core instance, no procedural/continuous assignments, no
             reg/integer/wire declarations in the module body, all nine
             pads present in the port map).
  strip  -DH07_STRIP_OBSERVABILITY (the H07 define: every core output
         tied off). The flop count must DROP vs full -- the negative
         control proving this measurement has resolution, mirroring
         H07's strip-observability control.

What is reported and how it is labelled:
  - cell counts, flop counts, chip area: measured (yosys stat -liberty
    against the named liberty).
  - chassis additions: zero, by the structural no-logic check plus the
    <= gates; the delta vs the H07 core-top build is reported as
    `*_culled` with its attribution (connection-state pruning), since
    the two builds differ in top exposure.
  - No P&R, no timing closure, no fit, no board claim. Real placement
    and routing is H10's evidence; this is mapped-area on the
    pre-synthesis RTL only, like H07's.

Mutant builds are never synthesized: the strip build is a negative
CONTROL, not an area datapoint.

--from-logs skips the yosys runs and re-parses the committed
yosys_full.log / yosys_strip.log (their sha256s are recorded in the
report), so the gate verdicts are re-checkable without the synthesis
wall time.

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

RTL_RELS = ["rtl/synth_top.v", "rtl/dx7_core.v", "rtl/env_unit.v",
            "rtl/alg_router.v"]
WRAP_REL = "rtl/synth_top.v"
PADS = ["ref_clk", "rst_n", "spi_cs_n", "spi_sck", "spi_mosi", "spi_miso",
        "i2s_bclk", "i2s_lrclk", "i2s_d"]
OUTDIR_REL = "evidence/h08-chassis"
H07_REPORT_REL = "evidence/h07-core/synth_report.json"

# The mapped-flop floor: H02's measured note-bank slice (a strict subset
# of the core's state). Same floor as tools/h07_synth.py.
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
        "read_verilog -sv" + (" -DH07_STRIP_OBSERVABILITY" if strip else "")
        + " " + " ".join(os.path.join(REPO_ROOT, r) for r in RTL_RELS),
        "hierarchy -check -top synth_top",
        "synth -top synth_top",
        f"dfflibmap -liberty {liberty}",
        f"abc -liberty {liberty}",
        f"stat -liberty {liberty}",
    ]
    joined = "; ".join(script)
    proc = subprocess.run([yosys, "-p", joined], capture_output=True,
                          text=True, cwd=REPO_ROOT)
    log = (proc.stdout or "") + (proc.stderr or "")
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script (cwd={REPO_ROOT}): {joined}\n" + log)
    if proc.returncode != 0:
        raise CheckFailure(
            f"yosys failed (rc={proc.returncode}); log at {log_path}: "
            + (log.strip().splitlines()[-1][:300] if log.strip() else ""))
    return log


def parse_stat(log):
    """Mapped cell/area facts from a `stat -liberty` transcript
    (same parser contract as tools/h07_synth.py)."""
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


def check_wrapper_structure(path):
    """Structural proof that the committed wrapper is a pin map.

    Exactly one dx7_core instance, no procedural or continuous logic in
    the module body, no data-register declarations, and all nine pads
    present in the port map. Returns (ok, detail).
    """
    txt = open(path, encoding="utf-8").read()
    lines = [re.sub(r"//.*$", "", ln) for ln in txt.splitlines()]
    code = "\n".join(lines)
    if "module synth_top" not in code:
        return False, "no module synth_top found"
    # module header: from 'module synth_top' to the first line that is
    # exactly ');' (the port list ends there; the body follows)
    m = re.search(r"module synth_top.*?\n\);\n", code, re.S)
    if not m:
        return False, "module header not found"
    body = code[m.end():code.rfind("endmodule")]
    ids = re.findall(r"\b[A-Za-z_][A-Za-z0-9_]*\b", body)
    forb = {"always", "always_comb", "always_ff", "always_latch",
            "assign", "initial", "integer", "reg", "wire", "function",
            "task", "localparam", "parameter", "begin", "case", "if",
            "else", "for", "end", "$unit"}
    hit = sorted(set(ids) & forb)
    ninst = body.count("dx7_core")
    miss = sorted(p for p in PADS if p not in ids)
    ok = (ninst == 1 and not hit and not miss
          and "u_core" in body)
    return ok, ("instances=%d forbidden_tokens=%s missing_pads=%s" %
                (ninst, hit or "-", miss or "-"))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--mode", choices=("both", "full", "strip"),
                    default="both")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--ciel-root", default=DEFAULT_CIEL)
    ap.add_argument("--liberty", default=None)
    ap.add_argument("--from-logs", action="store_true",
                    help="skip the yosys runs; re-parse the committed "
                         "yosys_full.log / yosys_strip.log in --outdir "
                         "(their sha256s are recorded in the report)")
    ap.add_argument("--outdir",
                    default=os.path.join(REPO_ROOT, OUTDIR_REL))
    args = ap.parse_args(argv)

    liberty = args.liberty or os.path.join(args.ciel_root, SC_LIB_REL)
    if not os.path.isfile(liberty):
        print(f"COULD-NOT-RUN: 7t liberty not found at {liberty} "
              f"(ciel root {args.ciel_root})", file=sys.stderr)
        return 2
    ver = yosys_version(args.yosys)
    h07_path = os.path.join(REPO_ROOT, H07_REPORT_REL)
    if not os.path.isfile(h07_path):
        print(f"COULD-NOT-RUN: committed H07 report missing at "
              f"{H07_REPORT_REL}", file=sys.stderr)
        return 2
    h07 = json.load(open(h07_path))
    h07_full = h07["runs"]["full"]

    os.makedirs(args.outdir, exist_ok=True)
    log_full = os.path.join(args.outdir, "yosys_full.log")
    log_strip = os.path.join(args.outdir, "yosys_strip.log")

    runs = {}
    gates = {}
    wrap_ok, wrap_detail = check_wrapper_structure(
        os.path.join(REPO_ROOT, WRAP_REL))
    if args.mode in ("both", "full"):
        log_full = os.path.join(args.outdir, "yosys_full.log")
        if args.from_logs:
            if not os.path.isfile(log_full):
                print(f"COULD-NOT-RUN: {log_full} missing for "
                      "--from-logs; run yosys first", file=sys.stderr)
                return 2
            log = open(log_full, encoding="utf-8").read()
        else:
            log = run_yosys(args.yosys, liberty, strip=False,
                            log_path=log_full)
        st = parse_stat(log)
        st["mode"] = "full"
        st["log_sha256"] = sha256_file(log_full)
        culled_flops = h07_full["flop_total"] - st["flop_total"]
        culled_cells = h07_full["cell_total"] - st["cell_total"]
        ok = (wrap_ok
              and st["flop_total"] > H02_MAPPED_FLOPS
              and st["flop_total"] <= h07_full["flop_total"]
              and st["cell_total"] <= h07_full["cell_total"]
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0)
        gates["full"] = {
            "requirement": ("passive pin map: (a) mapped flops above the "
                            "H02 floor (state present, not optimized "
                            "away); (b) mapped flops and cell total DO "
                            "NOT EXCEED the committed H07 core full build "
                            "(a wrapper adds no state or logic; the H08 "
                            "top exposes fewer ports, so any shortfall is "
                            "connection-state pruning of the unconnected "
                            "tap/status/debug outputs, reported as "
                            "*_culled, never overhead); (c) the committed "
                            "wrapper source is structurally a pin map; "
                            "chip/sequential area > 0"),
            "wrapper_structure": {
                "file": WRAP_REL,
                "sha256": sha256_file(os.path.join(REPO_ROOT, WRAP_REL)),
                "status": "PASS" if wrap_ok else "FAIL",
                "detail": wrap_detail,
            },
            "h07_core_flops": h07_full["flop_total"],
            "h07_core_cells": h07_full["cell_total"],
            "mapped_flops": st["flop_total"],
            "cell_total": st["cell_total"],
            "flops_culled_vs_h07_top_build": culled_flops,
            "cells_culled_vs_h07_top_build": culled_cells,
            "culled_attribution": ("shortfall vs the H07 core-top build "
                                   "only: the chassis top does not "
                                   "expose the core's tap/status/debug "
                                   "outputs, so their driver logic is "
                                   "pruned; the wrapper itself adds no "
                                   "state or logic (structural check + "
                                   "do-not-exceed gates)"),
            "chip_area_um2": st["chip_area_um2"],
            "overhead_chip_area_um2": (
                (st["chip_area_um2"] or 0.0)
                - (h07_full["chip_area_um2"] or 0.0)),
            "status": "PASS" if ok else "FAIL",
        }
        runs["full"] = st
    if args.mode in ("both", "strip"):
        log_strip = os.path.join(args.outdir, "yosys_strip.log")
        if args.from_logs:
            if not os.path.isfile(log_strip):
                print(f"COULD-NOT-RUN: {log_strip} missing for "
                      "--from-logs; run yosys first", file=sys.stderr)
                return 2
            log = open(log_strip, encoding="utf-8").read()
        else:
            log = run_yosys(args.yosys, liberty, strip=True,
                            log_path=log_strip)
        st = parse_stat(log)
        st["mode"] = "strip"
        st["log_sha256"] = sha256_file(log_strip)
        runs["strip"] = st

    if "full" in runs and "strip" in runs:
        gone = (runs["strip"]["flop_total"]
                < runs["full"]["flop_total"])
        gates["strip_negative_control"] = {
            "requirement": ("strip-observability must make the flop count "
                           "drop vs the full chassis build (all "
                           "observable state removed)"),
            "mapped_flops": runs["strip"]["flop_total"],
            "status": "MET (control removes state as required)" if gone
                      else "NOT MET (flops unchanged -- control failed)",
        }

    report = {
        "unit": "synth_top",
        "issue": 30,
        "yosys": ver,
        "liberty": liberty,
        "liberty_sha256": sha256_file(liberty),
        "rtl": {r: sha256_file(os.path.join(REPO_ROOT, r))
                for r in RTL_RELS},
        "h07_report_ref": {
            "path": H07_REPORT_REL,
            "sha256": sha256_file(h07_path),
            "full_flops": h07_full["flop_total"],
            "full_cells": h07_full["cell_total"],
            "full_chip_area_um2": h07_full.get("chip_area_um2"),
        },
        "runs": runs,
        "gates": gates,
        "claims": ("mapped area on the pre-synthesis RTL only; no P&R, no "
                   "timing closure, no fit, no board claim (H10 owns "
                   "placement, routing and clock closure); the chassis "
                   "is a passive pin map over the hash-pinned H07 core"),
    }
    out = os.path.join(args.outdir, "synth_report.json")
    with open(out, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    ok = all(g.get("status") in
             ("PASS", "MET (control removes state as required)")
             for g in gates.values())
    print(json.dumps({"gates": gates, "report": out}, indent=1))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())