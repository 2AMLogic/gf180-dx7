#!/usr/bin/env python3
"""Issue #86: does the synthesised exp_hsum agree with the simulated core?

`rtl/dx7_core.v` reads `exp_t3[85:30]`, `exp_t4[85:30]` and `exp_t5[81:26]`
from wires declared [70:0]/[70:0]/[65:0].  The Verilator-simulated core (the
H07 bit-exact reference) evaluates the missing bits as 0, which
tools/exp_range_proof.py proves is also the exact value.  This tool checks
what the synthesis flow makes of the same text, using the pinned ORFS
image's yosys (the binary the flow selects, see docs/H10-GF180-FEASIBILITY.md
section 6) unless --yosys is given.

Checks (each reports PASS / FAIL / NOT_RUN):

1. cone-lec (acceptance path (b), scoped to the exp_hsum adder cone).  The
   `exp_hsum` assignment is extracted verbatim from the RTL into a cone
   module whose inputs are exp_xq, exp_p2 and exp_t3/t4/t5 at their declared
   widths.  The cone is taken through the pinned synth.tcl pass order up to
   and including `setundef -zero` (then `abc` to generic gates), and a
   SAT miter proves or refutes equivalence against the gold cone in which
   the out-of-range bits are explicit zeros (the simulated-core semantics).
   PASS = equivalent for every input.
2. cone-lec-widened (positive control / candidate fix): the same with the
   three wires widened to [85:0]/[85:0]/[81:0] (zero-extended).  Must PASS,
   showing the harness can pass.
3. cone-lec-control (failure control required by the issue): the widened
   cone against a gold that sets one out-of-range bit of exp_t3 to 1.  Must
   FAIL (SAT finds a counterexample).
4. core-hsum-undef: on the FULL dx7_core, run the flow's opening passes
   (read_verilog -sv, hierarchy, proc, opt_expr) and inspect the RTLIL: is
   the wire driving `exp_hsum` tied to an all-x constant?  If so, the later
   `setundef -zero` makes the synthesised exp_hsum the constant 0 and the
   core FAILS agreement.  The widened scratch copy must still drive
   exp_hsum from an adder cell (control).

Exit: 0 = the as-written RTL agrees (cone-lec PASS, core driver not x) and
all controls behaved; 1 = disagreement found (controls behaved); 2 = NOT_RUN
or a control misbehaved (no verdict).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
DX7_CORE = REPO / "rtl" / "dx7_core.v"
PINNED_IMAGE = ("openroad/orfs@sha256:"
                "ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2")
PINNED_YOSYS = "/OpenROAD-flow-scripts/tools/install/yosys/bin/yosys"

HSUM_RE = re.compile(r"wire \[56:0\]\s+exp_hsum = .*?exp_t5\[81:26\]\};", re.S)
T_DECL = {  # name: (declared hi, widened hi)
    "exp_t3": (70, 85), "exp_t4": (70, 85), "exp_t5": (65, 81)}
OOR_GOLD = {  # verbatim read -> explicit-zero (simulated-core) form
    "exp_t3[85:30]": "{15'b0, exp_t3[70:30]}",
    "exp_t4[85:30]": "{15'b0, exp_t4[70:30]}",
    "exp_t5[81:26]": "{16'b0, exp_t5[65:26]}",
}

# Pinned image flow/scripts/synth.tcl (sha256 4725557a...) pass order for
# this design: line 125 (SYNTH_HIERARCHICAL=0), 195, 208, 249, 252, 258.
# extract_fa + the gf180 adder techmap (lines 222-227) and dfflibmap are
# omitted: the cone has no flops, and cell mapping happens after the point
# where the verdict is decided (see docs/EXP-RANGE-PROOF-86.md).
FLOW = """\
hierarchy -check -top cone
synth -flatten -run :fine
synth -top cone -run fine: -noabc
opt -purge
opt
setundef -zero
abc -g AND,NAND,OR,NOR,XOR,XNOR,MUX
opt_clean -purge
write_verilog -noattr {out}
"""


def cone_source(rtl: str, widened: bool) -> str:
    m = HSUM_RE.search(rtl)
    if not m:
        raise RuntimeError("exp_hsum assignment not found in RTL")
    body = m.group(0)
    ports = ["input [55:0] exp_xq", "input [45:0] exp_p2"]
    decls = []
    for name, (hi, whi) in T_DECL.items():
        if widened:
            ports.append(f"input [{hi}:0] {name}_in")
            decls.append(f"wire [{whi}:0] {name} = {{{whi - hi}'b0, {name}_in}};")
        else:
            ports.append(f"input [{hi}:0] {name}")
    return ("module cone(" + ", ".join(ports) + ", output [56:0] o);\n"
            + "\n".join(decls) + "\n" + body + "\nassign o = exp_hsum;\nendmodule\n")


def gold_source(rtl: str, bad: bool, widened_ports: bool) -> str:
    src = cone_source(rtl, widened=False)
    for k, v in OOR_GOLD.items():
        if k not in src:
            raise RuntimeError(f"expected read {k} not found")
        src = src.replace(k, v)
    if bad:  # failure control: one out-of-range bit of exp_t3 forced to 1
        src = src.replace("{15'b0, exp_t3[70:30]}", "{14'b0, 1'b1, exp_t3[70:30]}")
    if widened_ports:  # same port names as the widened cone
        for name in T_DECL:
            src = src.replace(f" {name},", f" {name}_in,").replace(
                f" {name})", f" {name}_in)")
            src = re.sub(rf"\b{name}\[", f"{name}_in[", src)
    return src.replace("module cone(", "module gold(")


def widened_rtl(rtl: str) -> str:
    out = rtl
    for name, (hi, whi) in T_DECL.items():
        pat = re.compile(rf"wire \[{hi}:0\](\s+){name}\b")
        out, n = pat.subn(rf"wire [{whi}:0]\g<1>{name}", out)
        if n != 1:
            raise RuntimeError(f"could not widen {name}")
    return out


class Yosys:
    def __init__(self, yosys: str | None, work: Path):
        self.work = work
        self.host = yosys
        if yosys is None and shutil.which("docker") is None:
            raise RuntimeError("docker not on PATH and no --yosys given")

    def run(self, script: str, name: str) -> tuple[int, str]:
        ys = self.work / f"{name}.ys"
        ys.write_text(script)
        if self.host:
            cmd = [self.host, "-s", str(ys)]
        else:
            cmd = ["docker", "run", "--rm", "--platform", "linux/amd64",
                   "-v", f"{REPO}:{REPO}", "-v", f"{self.work}:{self.work}",
                   "-w", str(REPO), PINNED_IMAGE, PINNED_YOSYS, "-s", str(ys)]
        p = subprocess.run(cmd, cwd=str(REPO), capture_output=True, text=True)
        log = p.stdout + p.stderr
        (self.work / f"{name}.log").write_text(log)
        return p.returncode, log

    def version(self) -> str:
        rc, log = self.run("", "version")
        m = re.search(r"Yosys \S+ \([^)]*\)", log)
        return m.group(0) if m else "unknown"


def lec(y: Yosys, gate_v: str, gold_v: str, name: str) -> dict:
    w = y.work
    (w / f"{name}_gate.v").write_text(gate_v)
    (w / f"{name}_gold.v").write_text(gold_v)
    mapped = w / f"{name}_mapped.v"
    rc, log = y.run(f"read_verilog {w}/{name}_gate.v\n" + FLOW.format(out=mapped),
                    f"{name}_synth")
    if rc != 0 or not mapped.exists():
        return {"status": "NOT_RUN", "reason": "synth failed", "log_tail": log[-800:]}
    const0 = bool(re.search(r"assign o = 57'h0+;", mapped.read_text()))
    rc, log = y.run(
        f"read_verilog {w}/{name}_gold.v\nread_verilog {mapped}\nproc\n"
        "miter -equiv -flatten -make_outputs gold cone m\nhierarchy -top m\n"
        "sat -prove trigger 0 -show-inputs -show-outputs -timeout 600 m\n",
        f"{name}_lec")
    if "SAT proof finished - no model found: SUCCESS" in log:
        st = "PASS"
    elif "SAT proof finished - model found: FAIL" in log:
        st = "FAIL"
    else:
        return {"status": "NOT_RUN", "reason": "no SAT verdict", "log_tail": log[-800:]}
    cex = None
    if st == "FAIL":
        cex = {}
        for sig in ("gate_o", "gold_o"):
            m = re.search(rf"\\{sig}\s+\S+\s+\S+\s+([01x]+)", log)
            cex[sig] = hex(int(m.group(1), 2)) if m and "x" not in m.group(1) else (
                m.group(1) if m else None)
    return {"status": st, "mapped_output_is_constant_zero": const0,
            "counterexample": cex}


def core_hsum_undef(y: Yosys, rtl_path: Path, name: str) -> dict:
    """Full dx7_core through the flow's opening passes (the canonicalize
    read, then `proc` + `opt_expr` as `synth -run :fine` begins).  Reports
    whether the wire driving exp_hsum has been replaced by an all-x constant
    -- which the later `setundef -zero` (synth.tcl line 252) turns into 0."""
    il = y.work / f"{name}.il"
    rc, log = y.run(
        f"read_verilog -defer -sv -I rtl {rtl_path} rtl/env_unit.v rtl/alg_router.v\n"
        "hierarchy -check -top dx7_core\nproc\nopt_expr\n"
        f"write_rtlil {il}\n", name)
    if rc != 0 or not il.exists():
        return {"exp_hsum_driver_all_x": None, "log_tail": log[-800:]}
    text = il.read_text()
    il.unlink()
    m = re.search(r"^\s*connect \\exp_hsum (\S+)(?: \[56:0\])?$", text, re.M)
    if not m:
        return {"exp_hsum_driver_all_x": None, "reason": "exp_hsum driver not found"}
    drv = m.group(1)
    if re.fullmatch(r"\d+'x+", drv):
        return {"exp_hsum_driver": drv, "exp_hsum_driver_all_x": True}
    xm = re.search(r"^\s*connect " + re.escape(drv) + r" (\d+'x+)$", text, re.M)
    cell = re.search(r"connect \\Y " + re.escape(drv) + r"$", text, re.M)
    return {"exp_hsum_driver": drv,
            "exp_hsum_driver_tied_to": xm.group(1) if xm else None,
            "exp_hsum_driver_is_cell_output": bool(cell),
            "exp_hsum_driver_all_x": bool(xm) and not cell}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--yosys", default=None,
                    help="host yosys binary (default: the pinned ORFS image's, via docker)")
    ap.add_argument("--rtl", type=Path, default=DX7_CORE)
    ap.add_argument("--keep", type=Path, default=None,
                    help="copy generated sources/logs here")
    args = ap.parse_args()
    rtl_path = args.rtl.resolve()
    rtl = rtl_path.read_text()
    out = {"check": "issue #86 exp_hsum synthesis agreement",
           "rtl_file": (str(rtl_path.relative_to(REPO)) if rtl_path.is_relative_to(REPO)
                        else str(rtl_path)),
           "rtl_sha256": hashlib.sha256(rtl.encode()).hexdigest(),
           "yosys_source": args.yosys or f"{PINNED_IMAGE} {PINNED_YOSYS}"}
    tmp = tempfile.mkdtemp(prefix="i86lec", dir="/tmp")
    work = Path(tmp).resolve()
    try:
        y = Yosys(args.yosys, work)
        out["yosys_version"] = y.version()
        wide_path = work / "dx7_core_widened.v"
        wide_path.write_text(widened_rtl(rtl))
        out["cone_lec"] = lec(y, cone_source(rtl, False), gold_source(rtl, False, False), "asis")
        out["cone_lec_widened"] = lec(y, cone_source(rtl, True),
                                      gold_source(rtl, False, True), "wide")
        out["cone_lec_control"] = lec(y, cone_source(rtl, True),
                                      gold_source(rtl, True, True), "ctrl")
        out["core_hsum_undef"] = core_hsum_undef(y, rtl_path, "core_asis")
        out["core_hsum_undef_widened"] = core_hsum_undef(y, wide_path, "core_wide")
    except RuntimeError as e:
        out.update(status="NOT_RUN", reason=str(e))
        print(json.dumps(out, indent=1))
        return 2
    finally:
        if args.keep:
            args.keep.mkdir(parents=True, exist_ok=True)
            for f in work.glob("*"):
                if f.suffix in (".v", ".ys") and "mapped" not in f.name \
                        and "widened" not in f.name:
                    shutil.copy(f, args.keep / f.name)
        shutil.rmtree(work, ignore_errors=True)

    controls_ok = (out["cone_lec_widened"]["status"] == "PASS"
                   and out["cone_lec_control"]["status"] == "FAIL"
                   and out["core_hsum_undef_widened"]["exp_hsum_driver_all_x"] is False)
    asis = out["cone_lec"]["status"]
    core = out["core_hsum_undef"]["exp_hsum_driver_all_x"]
    out["controls_behaved"] = controls_ok
    if not controls_ok or asis == "NOT_RUN" or core is None:
        out["status"] = "NO_VERDICT"
        rc = 2
    elif asis == "PASS" and core is False:
        out["status"] = "PASS"
        rc = 0
    else:
        out["status"] = "FAIL"
        rc = 1
    print(json.dumps(out, indent=1))
    return rc


if __name__ == "__main__":
    sys.exit(main())
