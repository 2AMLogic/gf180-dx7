#!/usr/bin/env python3
"""H01 operator-probe equivalence harness (issue #20).

The golden is the FROZEN PYTHON MODEL (src/gf180_dx7/model/operator.py,
contract gf180-dx7-integer-v1); the device under test is the RTL
(rtl/operator_probe.v). The bar is bit-exactness -- no tolerance: any
mismatch means the RTL (or its ROM bytes) is wrong, never a loosened check.

Flow:
  1. load the pinned N02 tables via tables.load_pinned (SHA-256 checked
     against reference/tables/manifest.json -- the SAME bytes the RTL's
     $readmemh loads) and generate a directed vector set (>= 2000 evals:
     phase sweeps, level sweeps, sign-boundary crossings, 24-bit cycle and
     32-bit register wrap corners, real N02 frame gain ramps, seeded random
     coverage) into vectors.txt + expected.txt;
  2. compile rtl/operator_probe.v + rtl/tb_operator_probe.v with iverilog
     and run it; the bench dumps the RTL's actual outputs;
  3. compare expected vs actual BIT-EXACT with first-mismatch localization;
  4. NEGATIVE CONTROL (issue #20): rebuild with -DMUTATE_ADDRWIDTH (the
     sinlog table address truncated to 9 bits) and require the equivalence
     check to FAIL -- proving the harness has resolution.

Exit codes: 0 pass (equivalence PASS and mutant control MET), 1 check
failure (mismatch on the real build, or the mutant unexpectedly agreed),
2 could-not-run (missing iverilog/vvp; the message names what is missing).
Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO_ROOT, "src")
if SRC not in sys.path:
    sys.path.insert(0, SRC)

from gf180_dx7.model import operator as op_model  # noqa: E402
from gf180_dx7.model.operator import (  # noqa: E402
    MarkIOperator, clip_val_domain, mki_sin, sin_log_lookup)
from gf180_dx7.model.tables import load_pinned  # noqa: E402

TABLES_DIR = os.path.join(REPO_ROOT, "reference", "tables")
RTL_REL = "rtl/operator_probe.v"
TB_REL = "rtl/tb_operator_probe.v"
OUTDIR_REL = "evidence/h01-operator-probe"
MIN_EVALS = 2000

M32 = 0xFFFFFFFF
M17 = 0x1FFFF


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


# ---------------------------------------------------------------------------
# Directed vector set (deterministic; the golden model judges every eval)
# ---------------------------------------------------------------------------

def build_vectors():
    """Generate (records, expected, phases, stats).

    records:  ("L", phase) phase-register load, or ("E", freq, env) eval
    expected: one (out_raw, clip_val) per eval, from the frozen model
    phases:   the phase value consumed by each eval (localization aid)
    """
    tabs = load_pinned(TABLES_DIR)  # raises TableDrift on any byte drift
    log_tab = tabs["sinlog_table"].values
    exp_tab = tabs["sinexp_table"].values

    records = []
    expected = []
    phases = []
    state = {"phase": 0}

    def load(phase):
        phase &= M32
        records.append(("L", phase, 0))
        state["phase"] = phase

    def ev(freq, env):
        freq &= M32
        env &= 0xFFFF
        phases.append(state["phase"])
        y = mki_sin(state["phase"], env, log_tab, exp_tab)
        expected.append((y, clip_val_domain([y])[0]))
        records.append(("E", freq, env))
        state["phase"] = (state["phase"] + freq) & M32

    # A. half-cycle-per-eval sweep: exactly one full 32-bit register wrap,
    #    crossing the 2^24 cycle boundary every other eval.
    load(0x00000000)
    for _ in range(512):
        ev(0x00800000, 1024)

    # A2. directed ROM-index walk: phi advances by one table index per
    #     eval, covering ALL 1024 sinlog ROM addresses (and every quadrant
    #     mirror/sign transition) with the exact bytes the model pins.
    load(0x00000000)
    for _ in range(1024):
        ev(0x00001000, 1024)

    # B. fine sweep with a rising level: 1024 evals, env 0 -> 16352 step 16.
    load(0x00000000)
    for i in range(1024):
        ev(0x00123457, (i * 16) & 0x3FFF)

    # C. directed wrap corners.
    load(0xFFFFFFFC)                     # 32-bit register wrap ...FF -> 0
    for _ in range(8):
        ev(1, 2048)
    load(0x00FFFFFC)                     # 24-bit cycle wrap 0xFFFFFF -> 0
    for _ in range(8):
        ev(1, 2048)
    load(0x00FFF000)                     # phi 0xFFFF -> 0x0000 quadrant edge
    for _ in range(8):
        ev(0x00001000, 4096)
    load(0x7FFFFFFC)                     # phi 0xFFFF mid-register sign edge
    for _ in range(4):
        ev(1, 4096)
    load(0x00000000)                     # negative-direction 32-bit wrap
    for _ in range(8):
        ev(0xFFFFFFFF, 8192)
    load(0x00000001)
    for _ in range(4):
        ev(0xFFFFFFFF, 8192)

    # D. level sweep at fixed phase (freq = 0): level 0, ENV_MAX, the full
    #    16-bit domain, and env values crossing the 0x8000 sign boundary.
    for phi in (0, 1, 512, 1023, 1024, 2047, 2048, 3071, 3072, 4095):
        load(phi << 12)
        for e in (0, 1, 2, 3, 16382, 16383, 16384, 16385,
                  32767, 32768, 32769, 49151, 49152, 65535):
            ev(0, e)
        sl = sin_log_lookup(phi << 12, log_tab)
        star = (0x8000 - sl) & 0xFFFF    # exact sign-boundary crossing
        for d in (-3, -2, -1, 0, 1, 2, 3):
            ev(0, (star + d) & 0xFFFF)
    load(0x02468ACE)
    for i in range(1024):                # dense level ramp at fixed phase
        ev(0, i & 0xFFFF)

    # E. real N02 frames: MarkIOperator.render_frame semantics (dgain ramp,
    #    gain advanced before each sample, carried phase), incl. a falling
    #    ramp, a max-gain frame and frames that wrap the register.
    frames = [
        (0x00000000, 0x00091444, 1024, 1024),   # ~A4 increment, flat gain
        (0x12345678, 0x00091444, 0, 4096),      # rising ramp
        (0xCAFEBABE, 0xFFFFFFFF, 16384, 0),     # falling ramp, freq wraps
        (0xFFFFFFFF, 0x00000001, 8192, 8192),   # wrap-corner frame
        (0x01000000, 0x00400000, 1024, 16384),  # quarter-cycle per eval
    ]
    for phase0, freq, g1, g2 in frames:
        op = MarkIOperator(phase0)
        block = op.render_frame(log_tab, exp_tab, freq, g1, g2)
        load(phase0)
        dgain = (g2 - g1 + (op_model.N >> 1)) >> op_model.LG_N
        gain = g1
        for y in block:
            gain += dgain                       # model: advance before use
            assert y == mki_sin(state["phase"], gain & 0xFFFF,
                                log_tab, exp_tab)
            ev(freq, gain & 0xFFFF)

    # F. seeded pseudo-random coverage (deterministic: fixed seed).
    rng = random.Random(0xC0FFEE)
    special_f = (0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF,
                 0x00001000, 0x00FFFFFF, 0x00FFF000)
    special_e = (0, 1, 16383, 16384, 16385, 32767, 32768, 65535,
                 0x7FFF, 0x8000, 0x8001)
    load(rng.randrange(1 << 32))
    for _ in range(640):
        f = rng.choice(special_f) if rng.random() < 0.25 \
            else rng.getrandbits(32)
        e = rng.choice(special_e) if rng.random() < 0.25 \
            else rng.getrandbits(16)
        ev(f, e)

    # Coverage is reported separately from agreement: distinct ROM
    # addresses the vector set exercises (replaying the model composition).
    sl_addrs = set()
    ex_addrs = set()
    for (_kind, _f, env), p in zip((r for r in records if r[0] == "E"),
                                   phases):
        phi = (p >> 12) & 0xFFFF
        sl = sin_log_lookup(phi, log_tab)
        sl_addrs.add((phi & 0x3FF) ^ (0x3FF if phi & (1 << 10) else 0))
        ev15 = (sl + env) & 0x7FFF
        ex_addrs.add((~ev15) & 0x3FF)

    evals_fp = [(r[1], p) for r, p in zip((q for q in records
                                           if q[0] == "E"), phases)]
    stats = {
        "evals": len(expected),
        "loads": sum(1 for r in records if r[0] == "L"),
        "negative_outputs": sum(1 for y, _ in expected if y < 0),
        "env_zero": sum(1 for r in records
                        if r[0] == "E" and r[2] == 0),
        "env_env_max": sum(1 for r in records
                           if r[0] == "E" and r[2] == 16384),
        "env_above_env_max": sum(1 for r in records
                                 if r[0] == "E" and r[2] > 16384),
        "cycle_boundary_24bit_crossings": sum(
            1 for f, p in evals_fp
            if (p >> 24) != ((p + f) >> 24)),
        "register_wrap_32bit_at_max": sum(
            1 for p in phases if p == 0xFFFFFFFF),
        "distinct_sinlog_addrs": len(sl_addrs),
        "distinct_sinexp_addrs": len(ex_addrs),
        "sinlog_addrs_total": 1024,
        "sinexp_addrs_total": 1024,
        "min_evals_required": MIN_EVALS,
    }
    return records, expected, phases, stats


# ---------------------------------------------------------------------------
# Vector / expected file writers (deterministic bytes)
# ---------------------------------------------------------------------------

def vector_text(records):
    out = [
        "# h01 operator-probe directed vectors (issue #20)",
        "# generated by tools/h01_compare.py from the FROZEN PYTHON MODEL",
        "# golden: src/gf180_dx7/model/operator.py "
        "(contract gf180-dx7-integer-v1)",
        "# ROM bytes: reference/tables/*.hex pinned by "
        "reference/tables/manifest.json",
        "#   sinlog sha256 990c19e90732efe712a19ba4272f97c7c9d884ac0e4e19450a"
        "1067165d8aa8a8",
        "#   sinexp sha256 e515a71ae736d92dcb3fd36973dea486c96d1521f1a0bab1f"
        "917be2c4ec07794",
        "# L <phase hex8>            load phase register (while en=0)",
        "# E <freq hex8> <env hex4>  one evaluation: out = mkiSin(phase, env)"
        "; phase += freq",
    ]
    for rec in records:
        if rec[0] == "L":
            out.append("L %08x" % rec[1])
        else:
            out.append("E %08x %04x" % (rec[1], rec[2]))
    return "\n".join(out) + "\n"


def expected_text(expected):
    out = [
        "# h01 expected outputs (golden model; BIT-EXACT bar, no tolerance)",
        "# <out_raw hex8> <clip_val hex5>  per eval, acceptance order",
    ]
    for y, cv in expected:
        out.append("%08x %05x" % (y & M32, cv & M17))
    return "\n".join(out) + "\n"


def parse_result_text(path):
    rows = []
    with open(path, "r", encoding="ascii") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 2 or any(
                    len(p) not in (5, 8) or
                    not re.fullmatch(r"[0-9a-fA-F]+", p) for p in parts):
                raise CheckFailure(
                    f"{path}:{lineno}: malformed result line {line!r}")
            rows.append((int(parts[0], 16), int(parts[1], 16)))
    return rows


# ---------------------------------------------------------------------------
# simulation
# ---------------------------------------------------------------------------

def tool_identity(iverilog):
    try:
        out = subprocess.run([iverilog, "-V"], capture_output=True,
                             text=True)
    except OSError:
        raise CouldNotRun(f"iverilog binary not found: {iverilog}")
    first = (out.stdout or out.stderr).strip().splitlines()
    if not first:
        raise CouldNotRun("iverilog -V produced no output")
    return first[0].strip()


def run_sim(iverilog, vvp, mutate, outdir):
    """Compile + run the bench; return (actual_rows, sim_meta)."""
    if shutil.which(iverilog) is None:
        raise CouldNotRun(f"iverilog not found on PATH: {iverilog}")
    if shutil.which(vvp) is None:
        raise CouldNotRun(f"vvp not found on PATH: {vvp}")
    vvp_out = os.path.join(tempfile.mkdtemp(prefix="h01_sim_"),
                           "tb_mutant.vvp" if mutate else "tb.vvp")
    actual_path = os.path.join(outdir,
                               "actual_mutant.txt" if mutate
                               else "actual.txt")
    cmd = [iverilog, "-g2012", "-o", vvp_out]
    if mutate:
        cmd.append("-DMUTATE_ADDRWIDTH")
    cmd += [os.path.join(REPO_ROOT, RTL_REL), os.path.join(REPO_ROOT, TB_REL)]
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=REPO_ROOT)
    if proc.returncode != 0:
        raise CheckFailure("iverilog compile failed: "
                           + (proc.stderr or proc.stdout)[-500:])
    vectors = os.path.join(outdir, "vectors.txt")
    try:
        run = subprocess.run([vvp, vvp_out, f"+vectors={vectors}",
                              f"+actual={actual_path}"],
                             capture_output=True, text=True, cwd=REPO_ROOT,
                             timeout=600)
        out = run.stdout + run.stderr
        if run.returncode != 0 or "FAIL" in out:
            raise CheckFailure(f"bench failed: {out.strip()[-500:]}")
        m = re.search(r"PASS tb_operator_probe \(evals=(\d+) loads=(\d+) "
                      r"captured=(\d+)\)", out)
        if not m:
            raise CheckFailure(f"bench produced no PASS summary: {out[-300:]}")
        rows = parse_result_text(actual_path)
    finally:
        try:
            os.remove(vvp_out)
        except OSError:
            pass
    return rows, {
        "bench_evals": int(m.group(1)),
        "bench_loads": int(m.group(2)),
        "bench_captured": int(m.group(3)),
        "actual_file": actual_path,
        "vvp_stdout_tail": out.strip()[-200:],
    }


def compare_bit_exact(expected, actual, records, phases):
    """BIT-EXACT comparison; returns (mismatches, first_mismatch|None)."""
    mismatches = []
    first = None
    eval_inputs = []
    _pi = 0
    for _r in records:
        if _r[0] == "E":
            eval_inputs.append((_r[1], _r[2], phases[_pi]))
            _pi += 1
    count = len(expected)
    if len(actual) != count:
        mismatches.append({
            "eval_index": min(count, len(actual)),
            "error": f"row-count mismatch: expected {count} rows, "
                     f"RTL dumped {len(actual)}",
        })
    for n in range(min(count, len(actual))):
        e_out, e_clip = expected[n]
        a_out, a_clip = actual[n]
        if (e_out & M32) != (a_out & M32) or \
                (e_clip & M17) != (a_clip & M17):
            freq, env, phase_before = eval_inputs[n]
            mm = {
                "eval_index": n,
                "phase_before": "%08x" % phase_before,
                "freq": "%08x" % freq,
                "env": "%04x" % env,
                "expected_out": "%08x" % (e_out & M32),
                "actual_out": "%08x" % (a_out & M32),
                "expected_clip": "%05x" % (e_clip & M17),
                "actual_clip": "%05x" % (a_clip & M17),
            }
            mismatches.append(mm)
            if first is None:
                first = mm
    return mismatches, first


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--outdir", default=os.path.join(REPO_ROOT, OUTDIR_REL))
    ap.add_argument("--gen-only", action="store_true",
                    help="write vectors + expected + gen report, no sim")
    ap.add_argument("--skip-mutate", action="store_true",
                    help="skip the MUTATE_ADDRWIDTH negative control")
    ap.add_argument("--mutate-only", action="store_true",
                    help="run only the negative-control mutant build")
    ap.add_argument("--iverilog", default="iverilog")
    ap.add_argument("--vvp", default="vvp")
    args = ap.parse_args(argv)

    try:
        records, expected, phases, stats = build_vectors()
    except Exception as exc:  # TableDrift or model error: could not run
        print(f"COULD-NOT-RUN: golden generation failed: {exc}",
              file=sys.stderr)
        return 2
    if stats["evals"] < MIN_EVALS:
        print(f"FAIL: vector set has {stats['evals']} evals "
              f"< required {MIN_EVALS}", file=sys.stderr)
        return 1

    os.makedirs(args.outdir, exist_ok=True)
    vectors_path = os.path.join(args.outdir, "vectors.txt")
    expected_path = os.path.join(args.outdir, "expected.txt")
    with open(vectors_path, "w", encoding="ascii") as f:
        f.write(vector_text(records))
    with open(expected_path, "w", encoding="ascii") as f:
        f.write(expected_text(expected))

    if args.gen_only:
        report = {
            "schema": "h01-gen-report/1",
            "issue": 20,
            "planning_id": "H01",
            "golden": "src/gf180_dx7/model/operator.py "
                      "(gf180-dx7-integer-v1)",
            "tables_manifest": os.path.join("reference", "tables",
                                            "manifest.json"),
            "stats": stats,
            "vectors_sha256": sha256_file(vectors_path),
            "expected_sha256": sha256_file(expected_path),
        }
        with open(os.path.join(args.outdir, "gen_report.json"), "w",
                  encoding="utf-8") as f:
            json.dump(report, f, indent=1, sort_keys=True)
            f.write("\n")
        print(f"h01_compare: generated {stats['evals']} evals "
              f"({stats['loads']} loads) -> {vectors_path}")
        return 0

    try:
        ident = tool_identity(args.iverilog)
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}", file=sys.stderr)
        return 2

    report = {
        "schema": "h01-compare-report/1",
        "issue": 20,
        "planning_id": "H01",
        "claim_boundary": "RTL-vs-frozen-model bit-exactness only; no "
                          "synthesis, timing, fit, or audibility claim",
        "tool_identity": {
            "iverilog": ident,
            "python": sys.version.split()[0],
            "rtl": {p: sha256_file(os.path.join(REPO_ROOT, p))
                    for p in (RTL_REL, TB_REL)},
            "tables_manifest": os.path.join("reference", "tables",
                                            "manifest.json"),
        },
        "vector_stats": stats,
        "vectors_sha256": sha256_file(vectors_path),
        "expected_sha256": sha256_file(expected_path),
    }

    status = "FAIL"
    try:
        if not args.mutate_only:
            actual, sim_meta = run_sim(args.iverilog, args.vvp,
                                       mutate=False, outdir=args.outdir)
            mismatches, first = compare_bit_exact(expected, actual, records,
                                                  phases)
            report["equivalence"] = dict(
                sim_meta,
                expected_rows=len(expected),
                actual_rows=len(actual),
                mismatches=len(mismatches),
                first_mismatch=first,
                status="PASS" if not mismatches else "FAIL",
            )

        if not args.skip_mutate:
            m_actual, m_meta = run_sim(args.iverilog, args.vvp,
                                       mutate=True, outdir=args.outdir)
            m_mismatch, m_first = compare_bit_exact(expected, m_actual,
                                                    records, phases)
            met = len(m_mismatch) > 0
            report["negative_control"] = dict(
                m_meta,
                define="MUTATE_ADDRWIDTH (sinlog address width 10 -> 9)",
                mismatches=len(m_mismatch),
                first_mismatch=m_first,
                status="MET (mutant fails equivalence as required)" if met
                else "NOT MET (mutant passed -- harness has no resolution)",
            )

        eq_ok = args.mutate_only or \
            report["equivalence"]["status"] == "PASS"
        nc_ok = args.skip_mutate or args.mutate_only or \
            report["negative_control"]["status"].startswith("MET")
        mut_ok = not args.mutate_only or \
            report["negative_control"]["status"].startswith("MET")
        status = "PASS" if (eq_ok and nc_ok and mut_ok) else "FAIL"
        report["status"] = status
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}", file=sys.stderr)
        return 2
    except CheckFailure as exc:
        report["status"] = "FAIL"
        report["error"] = str(exc)
        status = "FAIL"

    with open(os.path.join(args.outdir, "compare_report.json"), "w",
              encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    if "equivalence" in report:
        eq = report["equivalence"]
        print(f"equivalence: {eq['status']} "
              f"({eq['expected_rows']} evals, {eq['mismatches']} mismatches)")
        if eq["first_mismatch"]:
            fm = eq["first_mismatch"]
            print(f"  first mismatch at eval {fm.get('eval_index')} "
                  f"(phase={fm.get('phase_before')} "
                  f"freq={fm.get('freq')} env={fm.get('env')}): "
                  f"expected out={fm.get('expected_out')} "
                  f"clip={fm.get('expected_clip')}, "
                  f"actual out={fm.get('actual_out')} "
                  f"clip={fm.get('actual_clip')}"
                  + (f" [{fm['error']}]" if fm.get("error") else ""))
    if "negative_control" in report:
        nc = report["negative_control"]
        print(f"negative control: {nc['status']} "
              f"({nc['mismatches']} mismatches)")
    print(f"report: {os.path.join(args.outdir, 'compare_report.json')}")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
