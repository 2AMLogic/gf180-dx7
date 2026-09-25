#!/usr/bin/env python3
"""H08 chassis synthesis: passive-wrapper overhead vs the H07 core build.

issue #30 acceptance: "the synthesized chassis meets resource targets --
chassis overhead vs the core evidenced, not estimated". H08 adds
rtl/synth_top.v, a PIN-NET-ONLY wrapper over the hash-pinned H07 core
(rtl/dx7_core.v + rtl/env_unit.v + rtl/alg_router.v, unchanged; one
instance, every port tied 1:1 to a pad, no clock generation, no reset
sync, no glue logic). The expected measurement is therefore ZERO added
state and ZERO added cells vs the committed H07 full build
(evidence/h07-core/synth_report.json, at the ciel gf180mcu 7t liberty;
its numbers are STALE pre-#82 per-module values until issue #93
regenerates it).

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
             EXCEED the committed H07 core full build (gates.full_vs_h07,
             hierarchy totals of the same core RTL on both sides, else
             NOT_RUN) -- a passive wrapper adds no state or logic;
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

Stat parsing (issue #94): every reported cell/flop/area number is the
`=== design hierarchy ===` total closed by the LAST `Chip area for top
module` line, never a per-module block (the pre-#94 parser recorded
alg_router's local block as the chassis total). Gate (b) is reported as
gates.full_vs_h07 and is NOT_RUN unless both sides are hierarchy totals
of the same core RTL.

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool
or PDK path -- the message names what is missing -- or a gate NOT_RUN).
Stdlib only.
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


HIER_MARK = "=== design hierarchy ==="
CELL_LINE = r"^\s+(\d+)\s+(\S+)\s+(gf180mcu\S+)\s*$"
HIER_BASIS = "design hierarchy section (includes submodules)"


def _parse_empty_stat(log):
    """The one legitimate hierarchy-total-free transcript: a design that
    mapped to NOTHING (the H07_STRIP_OBSERVABILITY control -- yosys
    deletes every cell, so no `Chip area` line is printed at all).
    Accepted only when the transcript really is cell-free; anything else
    raises rather than reporting a per-module number as the total."""
    if re.search(r"Chip area for module", log):
        raise CheckFailure(
            "stat transcript has per-module 'Chip area for module' blocks "
            "but no 'Chip area for top module' line: the hierarchical "
            "total is absent (truncated or non-hierarchical stat). "
            "Refusing to substitute a per-module area as the design total "
            "(issue #94).")
    stray = re.findall(CELL_LINE, log, re.M)
    if stray:
        raise CheckFailure(
            f"stat transcript reports {len(stray)} mapped cell line(s) but "
            "no hierarchical total and no chip-area line -- refusing to "
            "report an unanchored count (issue #94).")
    return {
        "cells_by_name": {},
        "cell_total": 0,
        "dff_cells": {},
        "flop_total": 0,
        "chip_area_um2": None,
        "seq_area_um2": None,
        "totals_basis": "empty design (no mapped cells in the transcript)",
    }


def parse_stat(log):
    """Whole-design mapped cell/area facts from a `stat -liberty`
    transcript (same parser contract as tools/h07_synth.py after #82).

    yosys prints one LOCAL block per module (`=== <module> ===`, "Chip
    area for module '\\<module>'") and then, for a multi-module design, a
    final `=== design hierarchy ===` section whose counts INCLUDE
    submodules, closed by `Chip area for top module '\\<top>'`. Only that
    last section describes the chassis as a whole.

    The pre-#94 parser took the FIRST `Chip area for module` match and
    summed cell counts across every block, so the committed H08 report
    recorded `alg_router`'s local area (1,158,154.592 um^2) and local
    cell count as the chassis total. This parser therefore:

      - anchors on `Chip area for top module` and takes the LAST match;
      - bounds cell/flop counting to the hierarchy section preceding it,
        so the per-module blocks cannot be added into the totals;
      - raises CheckFailure rather than falling back to any per-module
        number when the hierarchy totals are absent.
    """
    top_m = list(re.finditer(
        r"Chip area for top module '\\?([\w$]+)': ([\d.]+)", log))
    if not top_m:
        return _parse_empty_stat(log)
    top = top_m[-1]
    start = log.rfind(HIER_MARK, 0, top.start())
    if start < 0:
        raise CheckFailure(
            f"'Chip area for top module' found but no '{HIER_MARK}' "
            "section precedes it -- refusing to report per-module numbers "
            "as the design total (issue #94).")
    sect = log[start:top.end()]
    total = re.search(r"^\s+(\d+)\s+\S+\s+cells\s*$", sect, re.M)
    if not total:
        raise CheckFailure(
            "no hierarchical 'cells' total line inside the "
            f"'{HIER_MARK}' section (issue #94).")
    cells = {}
    for m in re.finditer(CELL_LINE, sect, re.M):
        # assignment, not accumulation: within the bounded hierarchy
        # section each cell type appears exactly once, already summed
        # over submodules by yosys.
        cells[m.group(3)] = int(m.group(1))
    seq = re.search(r"of which used for sequential elements: ([\d.]+)",
                    log[top.end():top.end() + 400])
    return {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)),
        "dff_cells": {k: v for k, v in sorted(cells.items()) if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "chip_area_um2": float(top.group(2)),
        "seq_area_um2": float(seq.group(1)) if seq else None,
        "top_module": top.group(1),
        "totals_basis": HIER_BASIS,
    }


CORE_RTL_RELS = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]


def relative_gate(st, h07_report, h08_core_rtl):
    """Gate (b): the chassis's mapped flops/cells do not exceed the H07
    core-top build, and the shortfall is reported as `*_culled`.

    Evaluated ONLY when both sides are comparable; otherwise NOT_RUN with
    the reason (a guarded skip, never a smoothed-over number):
      - both sides must be design-hierarchy totals: H08's from this
        parser, H07's only once its report carries the #82 parser's
        `totals_basis` tag (regeneration tracked by issue #93);
      - both builds must be of the same core RTL (sha256 of the core
        trio): comparing mapped counts across RTL revisions measures the
        RTL change, not the wrapper.
    `h08_core_rtl` is the {path: sha256} of the core RTL the H08 log was
    synthesized from, or None when that is unknown.
    """
    h07_full = h07_report["runs"]["full"]
    rec = {
        "requirement": ("(b) mapped flops and cell total DO NOT EXCEED the "
                        "committed H07 core full build, both sides "
                        "design-hierarchy totals of the same core RTL (a "
                        "wrapper adds no state or logic; the H08 top "
                        "exposes fewer ports, so any shortfall is "
                        "connection-state pruning of the unconnected "
                        "tap/status/debug outputs, reported as *_culled, "
                        "never overhead)"),
        "h07_totals_basis": h07_full.get("totals_basis"),
        "h08_totals_basis": st.get("totals_basis"),
        "h07_core_flops": None,
        "h07_core_cells": None,
        "mapped_flops": st["flop_total"],
        "cell_total": st["cell_total"],
        "flops_culled_vs_h07_top_build": None,
        "cells_culled_vs_h07_top_build": None,
        "overhead_chip_area_um2": None,
    }
    why = []
    if st.get("totals_basis") != HIER_BASIS:
        why.append("H08 side is not a design-hierarchy total "
                   f"(totals_basis={st.get('totals_basis')!r})")
    if h07_full.get("totals_basis") != HIER_BASIS:
        why.append("H07 side is STALE: evidence/h07-core/synth_report.json "
                   "predates the #82 hierarchy-total parser (no "
                   "totals_basis tag); regeneration on the heavy host is "
                   "issue #93")
    h07_rtl = {k: v for k, v in (h07_report.get("rtl") or {}).items()
               if k in CORE_RTL_RELS}
    if h08_core_rtl is None:
        why.append("the core RTL the H08 log was synthesized from is "
                   "unknown (no committed report recording it for this "
                   "log sha256)")
    elif h07_rtl != {k: h08_core_rtl.get(k) for k in CORE_RTL_RELS}:
        diff = sorted(k for k in CORE_RTL_RELS
                      if h07_rtl.get(k) != h08_core_rtl.get(k))
        why.append("H07 and H08 builds are of different core RTL "
                   f"({', '.join(diff)} sha256 differ): the comparison "
                   "would measure the RTL change, not the wrapper")
    if why:
        rec["status"] = "NOT_RUN"
        rec["not_run_reason"] = why
        return rec
    rec["h07_core_flops"] = h07_full["flop_total"]
    rec["h07_core_cells"] = h07_full["cell_total"]
    rec["flops_culled_vs_h07_top_build"] = (h07_full["flop_total"]
                                            - st["flop_total"])
    rec["cells_culled_vs_h07_top_build"] = (h07_full["cell_total"]
                                            - st["cell_total"])
    rec["overhead_chip_area_um2"] = ((st["chip_area_um2"] or 0.0)
                                     - (h07_full["chip_area_um2"] or 0.0))
    rec["culled_attribution"] = (
        "shortfall vs the H07 core-top build only: the chassis top does "
        "not expose the core's tap/status/debug outputs, so their driver "
        "logic is pruned; the wrapper itself adds no state or logic "
        "(structural check + do-not-exceed gates)")
    ok = (st["flop_total"] <= h07_full["flop_total"]
          and st["cell_total"] <= h07_full["cell_total"])
    rec["status"] = "PASS" if ok else "FAIL"
    return rec


def logged_core_rtl(outdir, log_path):
    """For --from-logs: the core RTL sha256s recorded (in the previously
    committed synth_report.json) for exactly this log file, or None."""
    prev = os.path.join(outdir, "synth_report.json")
    if not os.path.isfile(prev):
        return None
    try:
        with open(prev, encoding="utf-8") as f:
            rep = json.load(f)
        if rep["runs"]["full"]["log_sha256"] != sha256_file(log_path):
            return None
        return {k: rep["rtl"][k] for k in CORE_RTL_RELS}
    except (KeyError, TypeError, ValueError):
        return None


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
            core_rtl = logged_core_rtl(args.outdir, log_full)
        else:
            log = run_yosys(args.yosys, liberty, strip=False,
                            log_path=log_full)
            core_rtl = {r: sha256_file(os.path.join(REPO_ROOT, r))
                        for r in CORE_RTL_RELS}
        st = parse_stat(log)
        st["mode"] = "full"
        st["log_sha256"] = sha256_file(log_full)
        ok = (wrap_ok
              and st["flop_total"] > H02_MAPPED_FLOPS
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0)
        gates["full"] = {
            "requirement": ("passive pin map, absolute checks: (a) mapped "
                            "flops above the H02 floor (state present, "
                            "not optimized away); (c) the committed "
                            "wrapper source is structurally a pin map; "
                            "chip/sequential area > 0. Gate (b), the "
                            "do-not-exceed comparison vs the H07 core "
                            "build, is gates.full_vs_h07."),
            "wrapper_structure": {
                "file": WRAP_REL,
                "sha256": sha256_file(os.path.join(REPO_ROOT, WRAP_REL)),
                "status": "PASS" if wrap_ok else "FAIL",
                "detail": wrap_detail,
            },
            "mapped_flops": st["flop_total"],
            "cell_total": st["cell_total"],
            "chip_area_um2": st["chip_area_um2"],
            "seq_area_um2": st["seq_area_um2"],
            "totals_basis": st["totals_basis"],
            "status": "PASS" if ok else "FAIL",
        }
        gates["full_vs_h07"] = relative_gate(st, h07, core_rtl)
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
            "full_totals_basis": h07_full.get("totals_basis"),
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

    passing = ("PASS", "MET (control removes state as required)")
    statuses = [g.get("status") for g in gates.values()]
    print(json.dumps({"gates": gates, "report": out}, indent=1))
    if any(s not in passing and s != "NOT_RUN" for s in statuses):
        return 1
    if "NOT_RUN" in statuses:
        print("NOT_RUN: at least one gate could not be evaluated (see "
              "not_run_reason); this is not a pass", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())