#!/usr/bin/env python3
"""H10 mapped synthesis of the refrozen core under both DONT_USE policies.

Issue #32 (H10). Maps the refrozen frozen core (DR-0011: rtl/dx7_core.v +
rtl/env_unit.v + rtl/alg_router.v, top dx7_core) onto the gf180mcu 7-track
5 V standard-cell library with yosys (dfflibmap + abc against the pinned
liberty), once per cell-use policy. The H10 evidence was produced with
the pinned ORFS image's own yosys (this script run inside the image with
--yosys/--liberty; see asic/orfs/README.md):

  orfs_stock  -dont_use *_1 on dfflibmap and abc -- the stock ORFS gf180
              platform policy (DONT_USE_CELLS = *_1), the policy of the
              sibling's routed family data points.
  x1_allowed  no dont_use -- every drive strength allowed (the sibling's
              klt/local-script policy).

Both runs use the same RTL, liberty, and script; the ONLY difference is
the -dont_use argument, so the area ratio between them is this core's
measured DONT_USE_CELLS effect under this recipe.

What is measured and how it is labelled:
  - hierarchical mapped totals (instances, std-cell area, sequential
    area, flops) are parsed from the `design hierarchy` section of
    `stat -liberty` -- the whole-top number, NOT the first per-module
    block (see issue filed against tools/h07_synth.py for that defect).
  - per-module local area and the submodule instance multiplicities are
    parsed too, so the report can attribute area (e.g. 96 x env_unit).
  - the mapped netlist (hundreds of MB) is written to --netlist-dir
    (default asic/orfs/work/h10-policy-synth, git-ignored) and recorded by
    sha256 + size only; the committed evidence is the stat transcript.
  - `check -assert` runs on the mapped design: a multi-driver or other
    structural problem fails the run (DR-0011's failure class).

This is the SYNTHESIS stage only. It makes no placement, routing, timing
or fit claim; tools/h10_report.py owns the fit gate.

Exit codes: 0 all requested runs mapped and parsed; 1 a run failed
(yosys error, check -assert, or unparseable stat); 2 could-not-run
(missing tool/liberty). Stdlib only.
"""

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import subprocess
import sys
import time

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RTL_RELS = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]
DEFAULT_PDK_ROOT = os.path.expanduser("~/.ciel")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")
OUTDIR_REL = "evidence/h10-gf180/policy-synth"
NETLIST_DIR_REL = "asic/orfs/work/h10-policy-synth"
POLICIES = {
    "orfs_stock": "*_1",
    "x1_allowed": None,
}
TIE_HI = "gf180mcu_fd_sc_mcu7t5v0__tieh Z"
TIE_LO = "gf180mcu_fd_sc_mcu7t5v0__tiel ZN"


class RunFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _float(tok):
    return float(tok)


def parse_hier_stat(text):
    """Parse the LAST `stat -liberty` transcript in `text`.

    Returns the whole-top totals from the `=== design hierarchy ===`
    section plus per-module local areas and the top's submodule
    multiplicities. Raises RunFailure if the hierarchy totals are absent
    (never falls back to a per-module number)."""
    top_m = list(re.finditer(
        r"Chip area for top module '\\?([\w$]+)': ([\d.]+)", text))
    if not top_m:
        raise RunFailure("no 'Chip area for top module' line -- the "
                         "hierarchical total is absent; refusing to "
                         "substitute a per-module number")
    top = top_m[-1]
    start = text.rfind("=== design hierarchy ===", 0, top.start())
    if start < 0:
        raise RunFailure("no '=== design hierarchy ===' section")
    sect = text[start:top.end() + 400]
    seq = re.search(r"of which used for sequential elements: ([\d.]+)",
                    text[top.end():top.end() + 400])
    cells_line = re.search(r"^\s+(\d+)\s+(\S+)\s+cells\s*$", sect, re.M)
    if not cells_line:
        raise RunFailure("no hierarchical 'cells' total line")
    by_cell = {}
    for m in re.finditer(r"^\s+(\d+)\s+(\S+)\s+(gf180mcu_\S+)\s*$", sect,
                         re.M):
        by_cell[m.group(3)] = int(m.group(1))
    # the hierarchy tree at the top of the section: "count area module"
    tree = {}
    for m in re.finditer(r"^\s+(\d+)\s+([\d.E+]+)\s+(\w+)\s*$",
                         sect.split("+----------Count including "
                                    "submodules.")[1]
                         .split("+----------Count")[0]
                         if sect.count("+----------Count") >= 2 else "",
                         re.M):
        tree[m.group(3)] = {"count_incl_sub": int(m.group(1)),
                            "area_incl_sub_um2_3sf": _float(m.group(2))}
    # submodule multiplicities of the top (the "submodules" block)
    subs = {}
    sm = re.search(r"^\s+\d+\s+\S+\s+submodules\s*$(.*?)(?:^\s*$)", sect,
                   re.M | re.S)
    if sm:
        for m in re.finditer(r"^\s+(\d+)\s+\S+\s+(\w+)\s*$", sm.group(1),
                             re.M):
            subs[m.group(2)] = int(m.group(1))
    # per-module local areas (from the per-module blocks preceding it)
    local = {}
    for m in re.finditer(r"Chip area for module '\\?([\w$]+)': ([\d.]+)",
                         text[:start]):
        local[m.group(1)] = float(m.group(2))
    flops = sum(v for k, v in by_cell.items()
                if re.search(r"__(dff|sdff|latq|icgt)", k))
    return {
        "top": top.group(1),
        "stdcell_area_um2": float(top.group(2)),
        "seq_area_um2": float(seq.group(1)) if seq else None,
        "instance_count": int(cells_line.group(1)),
        "flop_count": flops,
        "cells_by_name": dict(sorted(by_cell.items())),
        "module_local_area_um2": dict(sorted(local.items())),
        "top_submodule_instances": dict(sorted(subs.items())),
        "hierarchy_tree": tree,
    }


def yosys_version(yosys):
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise CouldNotRun(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise CouldNotRun(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def yosys_script(liberty, dont_use, netlist):
    du = f" -dont_use {dont_use}" if dont_use else ""
    return [
        # Load the liberty cells as blackbox definitions FIRST (the ORFS
        # flow's synth_stdcells.tcl does the same). Without port directions
        # for the mapped cells, `check -assert` reports every net driven by
        # a liberty-cell output as "used but has no driver" -- a false
        # failure (13,757 of them on this core, 2026-09-24 attempt).
        f"read_liberty -lib {liberty}",
        "read_verilog -sv " + " ".join(os.path.join(REPO_ROOT, r)
                                       for r in RTL_RELS),
        "hierarchy -check -top dx7_core",
        "synth -top dx7_core",
        f"dfflibmap -liberty {liberty}{du}",
        f"abc -liberty {liberty}{du}",
        "setundef -zero",
        f"hilomap -singleton -hicell {TIE_HI} -locell {TIE_LO}",
        "opt_clean -purge",
        "check -assert",
        f"stat -liberty {liberty}",
        f"write_verilog -noattr -noexpr {netlist}",
    ]


def run_policy(yosys, liberty, policy, outdir, netlist_dir):
    dont_use = POLICIES[policy]
    netlist = os.path.join(netlist_dir, f"dx7_core.{policy}.v")
    log_path = os.path.join(outdir, f"yosys_{policy}.log")
    script = yosys_script(liberty, dont_use, netlist)
    t0 = time.time()
    proc = subprocess.run([yosys, "-q", "-p", "; ".join(script),
                           "-T", "-l", log_path + ".full"],
                          capture_output=True, text=True, cwd=REPO_ROOT)
    wall = time.time() - t0
    full = ""
    if os.path.isfile(log_path + ".full"):
        with open(log_path + ".full", encoding="utf-8",
                  errors="replace") as f:
            full = f.read()
    # Committed log: the script, every warning/error line, and the final
    # stat transcript (the full pass-by-pass log is hundreds of MB; its
    # sha256 is recorded in the report).
    keep = [ln for ln in full.splitlines()
            if ln.startswith(("Warning", "ERROR", "Error"))]
    stat_at = full.rfind("Printing statistics.")
    stat_txt = full[stat_at:] if stat_at >= 0 else ""
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script (cwd={REPO_ROOT}):\n")
        for s in script:
            f.write(f"#   {s}\n")
        f.write(f"# full log sha256 "
                f"{sha256_file(log_path + '.full') if full else 'n/a'} "
                f"({len(full.encode())} B, not committed)\n")
        f.write(f"# wall {wall:.1f} s, rc {proc.returncode}\n")
        f.write("# ---- warnings/errors ----\n")
        f.write("\n".join(keep) + "\n")
        f.write("# ---- final stat ----\n")
        f.write(stat_txt)
    if proc.returncode != 0:
        tail = (proc.stderr or proc.stdout or "").strip().splitlines()
        raise RunFailure(f"{policy}: yosys rc={proc.returncode}: "
                         + (tail[-1][:300] if tail else "no output"))
    st = parse_hier_stat(stat_txt)
    x1_used = {k: v for k, v in st["cells_by_name"].items()
               if k.endswith("_1")}
    st.update({
        "policy": policy,
        "dont_use": dont_use,
        "x1_cells_used": sum(x1_used.values()),
        "wall_s": round(wall, 1),
        "check_assert": "PASS",
        "log": os.path.relpath(log_path, REPO_ROOT),
        "full_log_sha256": sha256_file(log_path + ".full"),
        "netlist": os.path.relpath(netlist, REPO_ROOT),
        "netlist_sha256": sha256_file(netlist),
        "netlist_bytes": os.path.getsize(netlist),
        "netlist_committed": False,
    })
    os.remove(log_path + ".full")
    if dont_use and st["x1_cells_used"]:
        raise RunFailure(f"{policy}: dont_use {dont_use} leaked "
                         f"{st['x1_cells_used']} x1 cells")
    return st


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--policy", choices=("both",) + tuple(POLICIES),
                    default="both")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--pdk-root", default=DEFAULT_PDK_ROOT)
    ap.add_argument("--liberty", default=None,
                    help="explicit tt_025C_5v00 liberty path (overrides "
                         "--pdk-root; used when running inside the pinned "
                         "image with only the lib directory mounted)")
    ap.add_argument("--outdir", default=os.path.join(REPO_ROOT, OUTDIR_REL))
    ap.add_argument("--netlist-dir",
                    default=os.path.join(REPO_ROOT, NETLIST_DIR_REL))
    args = ap.parse_args(argv)
    liberty = args.liberty or os.path.join(args.pdk_root, SC_LIB_REL)
    try:
        if not os.path.isfile(liberty):
            raise CouldNotRun(f"liberty not found: {liberty}")
        ver = yosys_version(args.yosys)
    except CouldNotRun as e:
        print(f"COULD-NOT-RUN: {e}", file=sys.stderr)
        return 2
    os.makedirs(args.outdir, exist_ok=True)
    os.makedirs(args.netlist_dir, exist_ok=True)
    pols = list(POLICIES) if args.policy == "both" else [args.policy]
    runs = {}
    rc = 0
    # The policies are independent single-threaded yosys processes: run
    # them concurrently (each writes only its own log/netlist).
    with concurrent.futures.ThreadPoolExecutor(len(pols)) as ex:
        futs = {p: ex.submit(run_policy, args.yosys, liberty, p,
                             args.outdir, args.netlist_dir) for p in pols}
        for p, fut in futs.items():
            try:
                runs[p] = fut.result()
                runs[p]["status"] = "PASS"
            except RunFailure as e:
                print(f"FAIL: {e}", file=sys.stderr)
                runs[p] = {"policy": p, "status": "FAIL", "reason": str(e)}
                rc = 1
    report = {
        "issue": 32,
        "stage": "synthesis (yosys, mapped to real 7t cells)",
        "design": "dx7_core",
        "yosys": ver,
        "liberty": os.path.basename(liberty),
        "liberty_sha256": sha256_file(liberty),
        "rtl": {r: sha256_file(os.path.join(REPO_ROOT, r))
                for r in RTL_RELS},
        "runs": runs,
        "claims": "mapped synthesis only: real-cell instance counts and "
                  "cell area at tt_025C_5v00; no placement, routing, "
                  "timing closure or fit claim",
    }
    if all(r.get("stdcell_area_um2") for r in runs.values()) and \
            {"orfs_stock", "x1_allowed"} <= set(runs):
        a, b = (runs["orfs_stock"]["stdcell_area_um2"],
                runs["x1_allowed"]["stdcell_area_um2"])
        report["dont_use_effect"] = {
            "orfs_stock_um2": a, "x1_allowed_um2": b,
            "ratio": round(a / b, 4),
            "pct": round(100.0 * (a / b - 1.0), 2),
            "basis": "same RTL/liberty/script; only -dont_use differs",
        }
    out = os.path.join(args.outdir, "synth_report.json")
    with open(out, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")
    summary = {p: {k: r.get(k) for k in ("stdcell_area_um2",
                                          "instance_count", "flop_count",
                                          "wall_s", "status")}
               for p, r in runs.items()}
    print(json.dumps({"runs": summary,
                      "dont_use_effect": report.get("dont_use_effect")},
                     indent=1))
    return rc


if __name__ == "__main__":
    sys.exit(main())
