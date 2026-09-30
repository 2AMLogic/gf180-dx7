#!/usr/bin/env python3
"""Issues #86/#98: does the synthesised exp_hsum agree with the simulated core?

`rtl/dx7_core.v` reads `exp_t3[85:30]`, `exp_t4[85:30]` and `exp_t5[81:26]`.
The exact products need only 68/60/52 bits (tools/exp_range_proof.py), so the
simulated core -- the H07 bit-exact reference -- evaluates every read bit at
or above the product width as 0.  This tool checks what the synthesis flow
makes of the same text, using the pinned ORFS image's yosys (the binary the
flow selects, see docs/H10-GF180-FEASIBILITY.md section 6) unless --yosys is
given.

The verdict therefore depends on the DECLARED widths of the three term wires
in the RTL under test, which this tool reads from the file rather than
assuming.  At the DR-0011 pin they were [70:0]/[70:0]/[65:0], narrower than
the reads: the missing bits are undef to the yosys frontend, its 4-state `+`
folds the whole adder to x, and `setundef -zero` then makes exp_hsum the
constant 0 (issue #86, FAIL).  DR-0012 widens them to [85:0]/[85:0]/[81:0]
(issue #98), which changes no simulated value and keeps the adder.

Every cone check drives the SAME primary inputs: exp_xq, exp_p2 and the
exp_t3/t4/t5 *products* at their exact widths [70:0]/[70:0]/[65:0] (the
values the frozen model produces).  The cone under test zero-extends each
product to the declared width of the RTL under test, then runs the verbatim
exp_hsum assignment.  The gold cone is always the simulated-core semantics:
the reads with explicit zeros above the product width.

Checks (each reports PASS / FAIL / NOT_RUN):

1. cone-lec (acceptance path (b), scoped to the exp_hsum adder cone): the
   cone at the declared widths of the RTL under test, through the pinned
   synth.tcl pass order up to and including `setundef -zero` (then `abc` to
   generic gates), SAT-mitered against the gold cone.  PASS = equivalent for
   every input.
2. cone-lec-widened (positive control): the same with the three wires forced
   to [85:0]/[85:0]/[81:0].  Must PASS, showing the harness can pass.
3. cone-lec-narrowed (live negative control, #98): the same with the three
   wires forced back to the DR-0011 widths [70:0]/[70:0]/[65:0], i.e. the
   as-written RTL.  Must FAIL -- it is the defect this tool exists to catch,
   and it stays demonstrable after the RTL is fixed.
4. cone-lec-control (failure control required by the issue): the widened
   cone against a gold that sets one out-of-range bit of exp_t3 to 1.  Must
   FAIL (SAT finds a counterexample).
5. core-hsum-undef: on the FULL dx7_core, run the flow's opening passes
   (read_verilog -sv, hierarchy, proc, opt_expr) and inspect the RTLIL: is
   the wire driving `exp_hsum` tied to an all-x constant?  If so, the later
   `setundef -zero` makes the synthesised exp_hsum the constant 0 and the
   core FAILS agreement.  Two scratch copies of the same core are the
   controls: widened must still be driven by an adder cell, narrowed
   (DR-0011 widths) must be all-x.

Exit: 0 = the RTL under test agrees (cone-lec PASS, core driver not x) and
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
# name: (exact-product hi, read hi).  The product hi bits are the widths the
# frozen model's exact products occupy (tools/exp_range_proof.py); the read hi
# is the top bit exp_hsum selects.  Both are properties of the arithmetic, not
# of any particular revision's declarations.
T_TERM = {"exp_t3": (70, 85), "exp_t4": (70, 85), "exp_t5": (65, 81)}
NARROW_HI = {k: v[0] for k, v in T_TERM.items()}   # DR-0011 declarations
WIDE_HI = {k: v[1] for k, v in T_TERM.items()}     # DR-0012 declarations
OOR_GOLD = {  # verbatim read -> explicit-zero (simulated-core) form
    "exp_t3[85:30]": "{15'b0, exp_t3_in[70:30]}",
    "exp_t4[85:30]": "{15'b0, exp_t4_in[70:30]}",
    "exp_t5[81:26]": "{16'b0, exp_t5_in[65:26]}",
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


def hsum_body(rtl: str) -> str:
    m = HSUM_RE.search(rtl)
    if not m:
        raise RuntimeError("exp_hsum assignment not found in RTL")
    return m.group(0)


def declared_hi(rtl: str) -> dict[str, int]:
    """Declared `wire [HI:0]` of each term wire, read from the RTL under test."""
    out = {}
    for name in T_TERM:
        m = re.findall(rf"wire \[(\d+):0\]\s+{name}\b", rtl)
        if len(m) != 1:
            raise RuntimeError(f"expected one `wire [HI:0] {name}`, found {len(m)}")
        out[name] = int(m[0])
    return out


def cone_source(rtl: str, decl_hi: dict[str, int]) -> str:
    """Cone whose primary inputs are the exact products at [70:0]/[70:0]/[65:0]
    (the frozen-model values), zero-extended to `decl_hi` before the verbatim
    exp_hsum assignment runs."""
    ports = ["input [55:0] exp_xq", "input [45:0] exp_p2"]
    decls = []
    for name, (phi, _) in T_TERM.items():
        hi = decl_hi[name]
        if hi < phi:
            raise RuntimeError(f"{name} declared [{hi}:0], narrower than its product")
        ports.append(f"input [{phi}:0] {name}_in")
        pad = "" if hi == phi else f"{{{hi - phi}'b0, "
        end = "" if hi == phi else "}"
        decls.append(f"wire [{hi}:0] {name} = {pad}{name}_in{end};")
    return ("module cone(" + ", ".join(ports) + ", output [56:0] o);\n"
            + "\n".join(decls) + "\n" + hsum_body(rtl)
            + "\nassign o = exp_hsum;\nendmodule\n")


def gold_source(rtl: str, bad: bool) -> str:
    """Simulated-core semantics: the reads with explicit zeros above the exact
    product width, on the same `_in` primary inputs the cone uses."""
    ports = ["input [55:0] exp_xq", "input [45:0] exp_p2"]
    ports += [f"input [{phi}:0] {n}_in" for n, (phi, _) in T_TERM.items()]
    body = hsum_body(rtl)
    for k, v in OOR_GOLD.items():
        if k not in body:
            raise RuntimeError(f"expected read {k} not found")
        body = body.replace(k, v)
    if bad:  # failure control: one out-of-range bit of exp_t3 forced to 1
        body = body.replace("{15'b0, exp_t3_in[70:30]}",
                            "{14'b0, 1'b1, exp_t3_in[70:30]}")
    return ("module gold(" + ", ".join(ports) + ", output [56:0] o);\n"
            + body + "\nassign o = exp_hsum;\nendmodule\n")


def retyped_rtl(rtl: str, decl_hi: dict[str, int]) -> str:
    """Scratch copy of the core with the three term wires redeclared."""
    out = rtl
    for name, hi in decl_hi.items():
        pat = re.compile(rf"wire \[\d+:0\](\s+){name}\b")
        out, n = pat.subn(rf"wire [{hi}:0]\g<1>{name}", out)
        if n != 1:
            raise RuntimeError(f"could not redeclare {name} (matched {n})")
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
    out = {"check": "issues #86/#98 exp_hsum synthesis agreement",
           "rtl_file": (str(rtl_path.relative_to(REPO)) if rtl_path.is_relative_to(REPO)
                        else str(rtl_path)),
           "rtl_sha256": hashlib.sha256(rtl.encode()).hexdigest(),
           "yosys_source": args.yosys or f"{PINNED_IMAGE} {PINNED_YOSYS}"}
    tmp = tempfile.mkdtemp(prefix="i86lec", dir="/tmp")
    work = Path(tmp).resolve()
    try:
        y = Yosys(args.yosys, work)
        out["yosys_version"] = y.version()
        decl = declared_hi(rtl)
        out["declared_term_widths"] = {k: f"[{v}:0]" for k, v in decl.items()}
        out["reads_within_declared_width"] = all(
            decl[k] >= rhi for k, (_, rhi) in T_TERM.items())
        wide_path = work / "dx7_core_widened.v"
        wide_path.write_text(retyped_rtl(rtl, WIDE_HI))
        narrow_path = work / "dx7_core_narrowed.v"
        narrow_path.write_text(retyped_rtl(rtl, NARROW_HI))
        gold = gold_source(rtl, False)
        out["cone_lec"] = lec(y, cone_source(rtl, decl), gold, "asis")
        out["cone_lec_widened"] = lec(y, cone_source(rtl, WIDE_HI), gold, "wide")
        out["cone_lec_narrowed"] = lec(y, cone_source(rtl, NARROW_HI), gold, "narrow")
        out["cone_lec_control"] = lec(y, cone_source(rtl, WIDE_HI),
                                      gold_source(rtl, True), "ctrl")
        out["core_hsum_undef"] = core_hsum_undef(y, rtl_path, "core_asis")
        out["core_hsum_undef_widened"] = core_hsum_undef(y, wide_path, "core_wide")
        out["core_hsum_undef_narrowed"] = core_hsum_undef(y, narrow_path, "core_narrow")
    except RuntimeError as e:
        out.update(status="NOT_RUN", reason=str(e))
        print(json.dumps(out, indent=1))
        return 2
    finally:
        if args.keep:
            args.keep.mkdir(parents=True, exist_ok=True)
            for f in work.glob("*"):
                if f.suffix in (".v", ".ys") and "mapped" not in f.name \
                        and not f.name.startswith("dx7_core"):
                    shutil.copy(f, args.keep / f.name)
        shutil.rmtree(work, ignore_errors=True)

    controls_ok = (out["cone_lec_widened"]["status"] == "PASS"
                   and out["cone_lec_narrowed"]["status"] == "FAIL"
                   and out["cone_lec_control"]["status"] == "FAIL"
                   and out["core_hsum_undef_widened"]["exp_hsum_driver_all_x"] is False
                   and out["core_hsum_undef_narrowed"]["exp_hsum_driver_all_x"] is True)
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
