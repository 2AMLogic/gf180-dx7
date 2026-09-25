#!/usr/bin/env python3
"""exp_t3/exp_t4/exp_t5 term-width range proof (issues #86, #98).

`rtl/dx7_core.v` computes the degree-5 Taylor-series terms of the AM/LFO
`exp()` unit as `exp_tk = exp_pk * C_k`, then reads `exp_t3[85:30]`,
`exp_t4[85:30]` and `exp_t5[81:26]` into `exp_hsum`.

The question this tool answers is whether the exact (infinite-precision)
product `p_k * C_k` is always below 2^W, W being the declared wire width.
If it is, the wire holds the exact product with no truncation, and every bit
at or above the product's own width -- including the top read bits -- is 0.

That proof carries two loads:

* At the DR-0011 pin the wires were declared [70:0]/[70:0]/[65:0], narrower
  than the reads.  The proof is what established that the missing bits were
  0 in exact arithmetic, so the Verilator-simulated core was right, while
  synthesis (which treats them as undef, folds the adder to x, then zeroes
  it) was building something else -- issue #86, docs/EXP-RANGE-PROOF-86.md.
* DR-0012 (issue #98) widens the declarations to [85:0]/[85:0]/[81:0] so the
  reads are in range.  The same bound is what makes that a no-op for every
  simulated value, and it stays live to catch any later change (a narrowed
  wire, a widened upstream slice) that would reintroduce truncation.

`out_of_range_bits` in the static result is therefore `None` on the current
core, and the term still reports its exact-product bound and headroom.

Two independent checks
----------------------
1. ``--mode static`` (default, < 1 s): an UNCONDITIONAL inductive-invariant
   bound, derived by parsing the declarations, slice assignments and
   constants directly out of the RTL file under test.  It does not assume
   the FSM ordering, the `exp_y` clip, or any operand domain: `exp_xq` is
   only ever written with a 50-bit slice (or 0 at reset), so it is < 2^50
   in every post-reset state; every downstream register is only ever written
   with a fixed slice of an unsigned product of already-bounded registers
   (or 0 at reset), so monotone interval propagation gives a bound valid in
   every post-reset state.  The parser refuses (exit 2) if a register has
   any assignment form it does not recognise, so the bound cannot silently
   skip a write path.

2. ``--mode sweep`` (~7 min): an exhaustive bit-exact transcription of the
   whole pipeline `exp_y -> exp_t3/t4/t5` over all 16,777,217 values of the
   clipped domain `[0, 2^24]`, measuring the tight reachable maxima.  It is
   a tightness measurement and a cross-check of the static bound (the swept
   maxima must not exceed the static bound); the static bound is the proof.

Negative controls
-----------------
* ``--rtl PATH`` runs the static proof against a scratch RTL copy.  A
  deliberately mis-sized select (e.g. `exp_xq <= exp_xp[109:50]`) or a
  narrowed product wire (e.g. `wire [63:0] exp_t3`) must make it FAIL.
* ``--mode sweep --mutate BIT`` ORs a stray 1 into bit BIT of `exp_p3`
  (mirrors the scratch RTL mutation `exp_p3 <= exp_p3w[99:56] | (44'd1 << 42)`)
  and must make the sweep FAIL.

Exit codes: 0 PASS, 1 FAIL, 2 could-not-run (NOT_RUN).  Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
DX7_CORE = REPO / "rtl" / "dx7_core.v"
# Frozen-core pins.  DR-0011 froze the single-driver revision; DR-0012
# (issue #98) re-freezes to the widened-exp_t* revision, which changes no
# simulated value but keeps the exp() datapath through synthesis.
DR0011_PIN = "34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58"
FROZEN_PIN = "f33cecbd138aea2a869cb502dca9273b342c8f97be51c9a1e587f2eb4aba89fa"

# Chain of registers analysed, in dependency order.  Each is written only
# with `<name> <= <src>[hi:lo];` (plus a constant-0 reset).
CHAIN_REGS = ("exp_xq", "exp_p2", "exp_p3", "exp_p4", "exp_p5")
TERMS = ("exp_t3", "exp_t4", "exp_t5")


class ParseError(Exception):
    pass


# --------------------------------------------------------------------------
# Static (unconditional) proof
# --------------------------------------------------------------------------

def _decl_width(rtl: str, name: str) -> int:
    m = re.findall(r"\b(?:reg|wire)\s*\[(\d+):0\]\s*" + re.escape(name) + r"\b", rtl)
    if len(m) != 1:
        raise ParseError(f"expected exactly one [N:0] declaration of {name}, found {len(m)}")
    return int(m[0]) + 1


def _reg_writes(rtl: str, name: str) -> list[str]:
    return [s.strip() for s in re.findall(r"\b" + re.escape(name) + r"\s*<=\s*([^;]+);", rtl)]


def _product_wire(rtl: str, name: str) -> tuple[int, str, str]:
    """Return (width, lhs_operand, rhs_operand_text) for `wire [W:0] name = a * b;`."""
    m = re.findall(r"\bwire\s*\[(\d+):0\]\s*" + re.escape(name)
                   + r"\s*=\s*([^*;]+?)\s*\*\s*([^*;]+?)\s*;", rtl)
    if len(m) != 1:
        raise ParseError(f"expected exactly one `wire [W:0] {name} = a * b;`, found {len(m)}")
    w, a, b = m[0]
    return int(w) + 1, a, b


def _const(text: str) -> int | None:
    m = re.fullmatch(r"(\d+)'([hdb])([0-9A-Fa-f_]+)", text)
    if not m:
        return None
    width, base, digits = int(m.group(1)), m.group(2), m.group(3).replace("_", "")
    v = int(digits, {"h": 16, "d": 10, "b": 2}[base])
    if v >= (1 << width):
        raise ParseError(f"constant {text} does not fit its width")
    return v


def static_proof(rtl: str) -> dict:
    """Unconditional post-reset interval bound on exp_xq..exp_p5 and
    exp_t3/t4/t5.  Raises ParseError if the RTL has any unrecognised form."""
    bound: dict[str, int] = {}
    steps = []

    for reg in CHAIN_REGS:
        rw = _decl_width(rtl, reg)
        writes = _reg_writes(rtl, reg)
        if not writes:
            raise ParseError(f"no assignments to {reg}")
        reg_max = 0
        detail = []
        for wtxt in writes:
            c = _const(wtxt)
            if c is not None:
                if c != 0:
                    raise ParseError(f"{reg} written with nonzero constant {wtxt}")
                detail.append({"write": wtxt, "kind": "reset-zero", "max": 0})
                continue
            m = re.fullmatch(r"([A-Za-z_]\w*)\[(\d+):(\d+)\]", wtxt)
            if not m:
                raise ParseError(f"unrecognised write to {reg}: `{wtxt}`")
            src, hi, lo = m.group(1), int(m.group(2)), int(m.group(3))
            sw, a, b = _product_wire(rtl, src)
            if hi >= sw:
                raise ParseError(f"{reg} <= {src}[{hi}:{lo}] is itself out of range (width {sw})")
            # Source bound: exact-product bound when both operands are
            # already-bounded chain registers, else the wire's own width.
            if a in bound and b in bound:
                prod_max = bound[a] * bound[b]
                truncates = prod_max >= (1 << sw)
                src_max = min(prod_max, (1 << sw) - 1)
                src_basis = f"{a}_max * {b}_max"
            else:
                truncates = None
                src_max = (1 << sw) - 1
                src_basis = f"width of {src} ({sw} bits); operands not bounded here"
            sl = hi - lo + 1
            v = min(src_max >> lo, (1 << sl) - 1, (1 << rw) - 1)
            reg_max = max(reg_max, v)
            detail.append({"write": wtxt, "kind": "slice", "src": src, "src_width": sw,
                           "src_operands": [a, b], "src_bound_basis": src_basis,
                           "src_max_bits": src_max.bit_length(),
                           "src_product_truncated": truncates,
                           "slice_width": sl, "max": v, "max_bits": v.bit_length()})
        bound[reg] = reg_max
        steps.append({"reg": reg, "declared_width": rw, "writes": detail,
                      "invariant_max": reg_max, "invariant_max_bits": reg_max.bit_length()})

    hsum = re.search(r"exp_hsum\s*=([^;]+);", rtl)
    if not hsum:
        raise ParseError("exp_hsum expression not found")
    terms = []
    for t in TERMS:
        tw, a, ctext = _product_wire(rtl, t)
        c = _const(ctext)
        if c is None or a not in bound:
            raise ParseError(f"{t} is not `<bounded reg> * <constant>`")
        reads = re.findall(re.escape(t) + r"\[(\d+):(\d+)\]", hsum.group(1))
        if len(reads) != 1:
            raise ParseError(f"expected one part-select of {t} in exp_hsum, found {len(reads)}")
        rhi, rlo = int(reads[0][0]), int(reads[0][1])
        prod_max = bound[a] * c
        ok = prod_max < (1 << tw)
        terms.append({
            "term": t, "operand": a, "constant": hex(c), "declared_width": tw,
            "read": f"[{rhi}:{rlo}]",
            "out_of_range_bits": f"[{rhi}:{tw}]" if rhi >= tw else None,
            "exact_product_max": prod_max, "exact_product_max_bits": prod_max.bit_length(),
            "headroom_bits": tw - prod_max.bit_length(),
            "exact_product_fits_wire": ok,
        })
    return {"chain": steps, "terms": terms,
            "bounds_bits": {k: v.bit_length() for k, v in bound.items()}}


# --------------------------------------------------------------------------
# Exhaustive sweep (tightness measurement / cross-check)
# --------------------------------------------------------------------------

# rtl/dx7_core.v:850-853 (DR-0011 pin 34f93d2d...)
EXP_M07 = 0x11EB851EB851EC    # 56 bits: 0.07 * 2^56
EXP_M122 = 0x18666666666666  # 54 bits: 12.2 * 2^49
EXP_MLG2 = 0x171547652B82FE  # 53 bits: log2(e) * 2^52
EXP_LN2 = 0xB17217F7D1CF79   # 56 bits: ln(2) * 2^56
Y_MAX = 1 << 24               # FSM state 4'd0 clip
T_WIDTH = {"t3": 71, "t4": 71, "t5": 66}


def _mask(x: int, width: int) -> int:
    return x & ((1 << width) - 1)


def pipeline(y: int, mutate_bit: int | None = None) -> dict:
    """Bit-exact transcription of rtl/dx7_core.v exp_y -> exp_t3/t4/t5."""
    y = _mask(y, 26)
    t1 = _mask(y * EXP_M07, 80)                  # exp_t1 <= exp_p1
    arg = _mask(((t1 >> 14) & ((1 << 65) - 1)) + (EXP_M122 << 11), 66)
    zq = _mask(_mask(arg * EXP_MLG2, 120) >> 52, 67)
    xq = _mask(_mask((zq & ((1 << 54) - 1)) * EXP_LN2, 110) >> 60, 56)
    p2 = (_mask(xq * xq, 112) >> 56) & ((1 << 45) - 1)
    p3 = (_mask(p2 * xq, 100) >> 56) & ((1 << 44) - 1)
    if mutate_bit is not None:
        p3 |= (1 << mutate_bit)
    p4 = (_mask(p3 * xq, 88) >> 56) & ((1 << 32) - 1)
    p5 = (_mask(p4 * xq, 82) >> 56) & ((1 << 26) - 1)
    return {"xq": xq, "p2": p2, "p3": p3, "p4": p4, "p5": p5,
            "t3_full": p3 * 0x2AAAAAAA, "t4_full": p4 * 0x0AAAAAAA,
            "t5_full": p5 * 0x02222222}


def sweep(mutate_bit: int | None = None, stride: int = 1) -> dict:
    keys = ("xq", "p2", "p3", "p4", "p5", "t3_full", "t4_full", "t5_full")
    mx = dict.fromkeys(keys, 0)
    arg = dict.fromkeys(keys, 0)
    ys = list(range(0, Y_MAX + 1, stride))
    if ys[-1] != Y_MAX:
        ys.append(Y_MAX)
    for y in ys:
        r = pipeline(y, mutate_bit)
        for k in keys:
            if r[k] > mx[k]:
                mx[k], arg[k] = r[k], y
    return {"points": len(ys), "stride": stride,
            "max_bits": {k: mx[k].bit_length() for k in keys},
            "max": mx, "argmax_y": arg}


# --------------------------------------------------------------------------
# Simulator cross-check (the real dx7_core, driven via tb_exp_probe.v)
# --------------------------------------------------------------------------

TB = REPO / "evidence" / "issue-86-exp-range-proof" / "tb_exp_probe.v"
OTHER_RTL = ("rtl/env_unit.v", "rtl/alg_router.v")
M56 = (1 << 56) - 1


def model_hsum(r: dict, exact: bool) -> int:
    """exp_hsum from the model.  exact=False: out-of-range read bits are 0
    (the mapped netlist's setundef -zero); exact=True: the read bits are
    taken from the exact, unbounded product (a hypothetically widened wire)."""
    t3 = r["t3_full"] if exact else _mask(r["t3_full"], T_WIDTH["t3"])
    t4 = r["t4_full"] if exact else _mask(r["t4_full"], T_WIDTH["t4"])
    t5 = r["t5_full"] if exact else _mask(r["t5_full"], T_WIDTH["t5"])
    s = ((1 << 56) + r["xq"] + ((r["p2"] >> 1) & ((1 << 44) - 1))
         + ((t3 >> 30) & M56) + ((t4 >> 30) & M56) + ((t5 >> 26) & M56))
    return _mask(s, 57)


def xcheck(sim: str, rtl: Path, workdir: Path) -> dict:
    import shutil
    import subprocess
    workdir.mkdir(parents=True, exist_ok=True)
    srcs = [str(TB), str(rtl)] + [str(REPO / f) for f in OTHER_RTL]
    if shutil.which(sim) is None:
        raise ParseError(f"{sim} not on PATH")
    if sim == "iverilog":
        exe = workdir / "tb.vvp"
        build = ["iverilog", "-g2012", "-s", "tb_exp_probe", "-o", str(exe)] + srcs
        runc = ["vvp", "-n", str(exe)]
    else:
        # Same simulator flags as tools/h07_compare.py:build_sim().
        build = ["verilator", "--binary", "--timing", "-j", "4", "--Wno-fatal",
                 "--timescale-override", "1ns/1ps", "--x-initial", "0",
                 "--x-assign", "unique", "--Mdir", str(workdir / "vl"),
                 "-o", "simtb", "--top-module", "tb_exp_probe"] + srcs
        runc = [str(workdir / "vl" / "simtb")]
    for cmd in (build, runc):
        p = subprocess.run(cmd, cwd=str(REPO), capture_output=True, text=True)
        if p.returncode != 0:
            raise ParseError(f"{cmd[0]} failed: {p.stderr[-2000:]}")
    lines = [ln for ln in p.stdout.splitlines() if ln.startswith("Y=")]
    if len(lines) != 64:
        raise ParseError(f"expected 64 probe lines, got {len(lines)}")
    fields = ("xq", "p2", "p3", "p4", "p5", "t3", "t4", "t5")
    inrange_mism, hsum_x, hsum_zero_mism, hsum_exact_mism = [], 0, [], []
    for ln in lines:
        kv = dict(tok.split("=", 1) for tok in ln.split())
        y = int(kv["Y"])
        r = pipeline(y)
        for f in fields:
            want = r[f] if f[0] != "t" else _mask(r[f + "_full"], T_WIDTH[f])
            if int(kv[f], 16) != want:
                inrange_mism.append((y, f, kv[f], hex(want)))
        h = kv["hsum"].lower()
        if any(c in h for c in "xz"):
            hsum_x += 1
            continue
        hv = int(h, 16)
        if hv != model_hsum(r, exact=False):
            hsum_zero_mism.append((y, h, hex(model_hsum(r, False))))
        if hv != model_hsum(r, exact=True):
            hsum_exact_mism.append((y, h, hex(model_hsum(r, True))))
    return {"simulator": sim, "points": len(lines),
            "inrange_mismatches": inrange_mism,
            "hsum_unknown_points": hsum_x,
            "hsum_vs_oor_zero_mismatches": hsum_zero_mism,
            "hsum_vs_exact_widened_mismatches": hsum_exact_mism}


# --------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--mode", choices=("static", "sweep", "xcheck"), default="static")
    ap.add_argument("--rtl", type=Path, default=DX7_CORE,
                    help="dx7_core RTL file (static/xcheck; default rtl/dx7_core.v)")
    ap.add_argument("--sim", choices=("verilator", "iverilog"), default="verilator",
                    help="xcheck simulator")
    ap.add_argument("--stride", type=int, default=1,
                    help="sweep stride (1 = exhaustive; >1 is a smoke check only)")
    ap.add_argument("--mutate", type=int, default=None, metavar="BIT",
                    help="sweep negative control: OR 1 into exp_p3 bit BIT")
    args = ap.parse_args()

    if not args.rtl.exists():
        print(json.dumps({"status": "NOT_RUN", "reason": f"{args.rtl} not found"}))
        return 2
    rtl_bytes = args.rtl.read_bytes()
    digest = hashlib.sha256(rtl_bytes).hexdigest()
    out = {"check": f"issue #86 exp_t3/t4/t5 out-of-range-select range proof ({args.mode})",
           "rtl_file": (str(args.rtl.resolve().relative_to(REPO))
                        if args.rtl.resolve().is_relative_to(REPO) else str(args.rtl)),
           "rtl_sha256": digest,
           "rtl_matches_dr0011_pin": digest == DR0011_PIN,
           "rtl_matches_frozen_pin": digest == FROZEN_PIN,
           "python": sys.version.split()[0]}

    if args.mode == "static":
        try:
            res = static_proof(rtl_bytes.decode())
        except ParseError as e:
            out.update(status="NOT_RUN", reason=f"parse: {e}")
            print(json.dumps(out, indent=1))
            return 2
        problems = [f"{t['term']}: exact product needs {t['exact_product_max_bits']} bits, "
                    f"declared wire is {t['declared_width']} bits"
                    for t in res["terms"] if not t["exact_product_fits_wire"]]
        for s in res["chain"]:
            for w in s["writes"]:
                if w.get("src_product_truncated"):
                    problems.append(f"{s['reg']}: source product {w['src']} can exceed its "
                                    f"{w['src_width']}-bit wire (truncation)")
        out.update(result=res)
    elif args.mode == "xcheck":
        import tempfile
        try:
            with tempfile.TemporaryDirectory(prefix="i86x") as td:
                res = xcheck(args.sim, args.rtl.resolve(), Path(td))
        except ParseError as e:
            out.update(status="NOT_RUN", reason=str(e))
            print(json.dumps(out, indent=1))
            return 2
        out.update(result=res)
        problems = []
        if res["inrange_mismatches"]:
            problems.append(f"{len(res['inrange_mismatches'])} in-range register/term mismatches")
        if res["hsum_vs_oor_zero_mismatches"]:
            problems.append(f"{len(res['hsum_vs_oor_zero_mismatches'])} exp_hsum points differ "
                            "from the out-of-range-bits-are-0 model")
        if res["hsum_vs_exact_widened_mismatches"]:
            problems.append(f"{len(res['hsum_vs_exact_widened_mismatches'])} exp_hsum points differ "
                            "from the exact (widened-product) model")
        if not problems and res["hsum_unknown_points"]:
            # The simulator returned X/Z for exp_hsum: no verdict on it.
            out.update(status="NO_VERDICT", problems=[
                f"exp_hsum unknown (x/z) at {res['hsum_unknown_points']}/{res['points']} points; "
                "in-range bits matched"])
            print(json.dumps(out, indent=1))
            return 1
    else:
        t0 = time.time()
        res = sweep(args.mutate, args.stride)
        out.update(mutate_bit=args.mutate, result=res,
                   runtime_s=round(time.time() - t0, 1))
        problems = [f"{k}: reachable exact product needs {res['max_bits'][k + '_full']} bits, "
                    f"declared wire is {w} bits"
                    for k, w in T_WIDTH.items() if res["max_bits"][k + "_full"] > w]
    out.update(status="PASS" if not problems else "FAIL", problems=problems)
    print(json.dumps(out, indent=1))
    return 0 if not problems else 1


if __name__ == "__main__":
    sys.exit(main())
