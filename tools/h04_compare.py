#!/usr/bin/env python3
"""H04 envelope-unit equivalence harness (issue #26).

The golden is the FROZEN PYTHON MODEL (src/gf180_dx7/model/envelope.py,
contract gf180-dx7-integer-v1, N03); the device under test is the RTL
(rtl/env_unit.v). The bar is bit-exactness -- no tolerance: any mismatch
means the RTL is wrong, never a loosened check.

Flow:
  1. generate a directed command stream (>= 3,000 frame steps incl.
     worst-rate updates): every stage transition incl. rising segments,
     static holds, delayed attacks, slow rates, retrigger-from-current-
     level mid-decay, sustain park, release-to-floor, update-while-held,
     a 100-rate x 4-rate-scaling sweep, full statics-table sweeps (both
     the /20 attack-hold and raw paths), levellut sweep, advance/park
     micro-corners; execute it against the model and write vectors.txt +
     expected.txt (full slot state after every command);
  2. compile rtl/env_unit.v + rtl/tb_env_unit.v with iverilog and run it
     on the identical stimulus; the bench dumps committed state per
     command and self-checks the H03 read-old/write-new hazard rule;
  3. compare expected vs actual BIT-EXACT with first-mismatch
     localization (command index, field);
  4. gate the bench's measured cycles-per-step against the H03 budget row
     env_steps (derived_estimate 24 clk incl. segment advance);
  5. NEGATIVE CONTROLS (issue #26), each MUST fail equivalence:
       -DH04_MUTATE_STEP_SKEW  update shifted one frame late (localized);
       -DH04_MUTATE_SRM        sr_multiplier - 1 (reachable coefficients
                               flip, N03 section 6).

Exit codes: 0 pass (equivalence PASS, budget gate PASS, hazard ok, and
both mutant controls MET), 1 check failure, 2 could-not-run (missing
iverilog/vvp; the message names what is missing). Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO_ROOT, "src")
if SRC not in sys.path:
    sys.path.insert(0, SRC)

from gf180_dx7.model import envelope as E  # noqa: E402

RTL_REL = "rtl/env_unit.v"
TB_REL = "rtl/tb_env_unit.v"
OUTDIR_REL = "evidence/h04-env-rtl"
MIN_EVALS = 3000
MAX_STEP_CLKS = 24          # H03 budget row env_steps (derived_estimate)

M32 = 0xFFFFFFFF
FIELDS = ("level", "target", "inc", "static", "ix", "rising", "down")


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


def golden_target(level_99: int, outlevel: int) -> int:
    """Segment target, mirroring env.cc:113-121 (model-checked)."""
    actual = ((E.scale_out_level(level_99) >> 1) << 6) + outlevel - 4256
    return (actual if actual >= 16 else 16) << 16


# ---------------------------------------------------------------------------
# Directed command stream (deterministic; the golden model judges every cmd)
# ---------------------------------------------------------------------------

def build_commands():
    """Directed envelope battery. Command tuples:
    ("P", rates4, levels4, outlevel, rs) / ("I",) / ("K", d) / ("U",)
    / ("S",) / ("A", k) / ("L", lv, tg, inc, st, ix, ri, dn)."""
    cmds = []

    def P(r, l, out, rs):
        cmds.append(("P", tuple(r), tuple(l), out, rs))

    def I():
        cmds.append(("I",))

    def K(d):
        cmds.append(("K", d))

    def U(r, l, out, rs):
        cmds.append(("U", tuple(r), tuple(l), out, rs))

    def S(n):
        for _ in range(n):
            cmds.append(("S",))

    def A(k):
        cmds.append(("A", k))

    def L(lv, tg, inc, st, ix, ri, dn):
        cmds.append(("L", lv, tg, inc, st, ix, ri, dn))

    OUT99 = E.scale_out_level(99) << 5        # derived-carrier outlevel
    OUT68 = E.scale_out_level(68) << 5

    # 1. base: attack, decay L1->L2->L3, sustain, release to nonzero L4
    P([80, 50, 60, 50], [99, 60, 70, 40], OUT99, 0)
    I()
    S(250)
    K(0)
    S(150)

    # 2. attack_fast: rate-99 attack, clamp advance, steep decay
    P([99, 99, 60, 50], [99, 50, 70, 40], OUT99, 0)
    I()
    S(50)
    K(0)
    S(30)

    # 3. static_hold_top: park at L2 (rates[2]=76 -> statics[76] hold),
    #    expiry advance + falling clamp advance in ONE step (double),
    #    release-complete park, then keyup from the parked state
    P([50, 99, 76, 99], [99, 70, 70, 50], OUT99, 0)
    I()
    S(430)
    K(0)
    S(40)

    # 4. zero_level_static: attack hold statics[30]//20, jump-floor rise
    P([30, 70, 60, 50], [0, 80, 60, 40], OUT99, 0)
    I()
    S(400)

    # 5. delayed_attack: statics[45]//20 hold, expiry, slow unfinished rise
    P([45, 45, 60, 50], [0, 99, 70, 40], OUT99, 0)
    I()
    S(900)
    # 5b. rate-1 hold entered (decrement-only frames; 81 k-frame hold)
    P([1, 70, 60, 50], [0, 99, 70, 40], OUT99, 0)
    I()
    S(300)

    # 6. slow_rate1_rise: qrate 0 (inc 940), unfinished 3.2 s-class rise
    P([1, 99, 60, 50], [99, 70, 60, 40], OUT99, 0)
    I()
    S(2048)
    # 6b. rate 1 with rate_scaling 1 (the N03 +-1-mutation flip point)
    P([1, 99, 60, 50], [99, 70, 60, 40], OUT99, 1)
    I()
    S(300)

    # 7. release_middecay: keyup mid-decay, L4 = 0 -> release to the
    #    clamp floor (16 << 16) and park
    P([80, 50, 60, 70], [99, 60, 70, 0], OUT99, 0)
    I()
    S(120)
    K(0)
    S(150)

    # 8. retrigger_middecay: keydown from the CURRENT level (no re-init)
    P([80, 50, 60, 50], [99, 60, 70, 40], OUT99, 0)
    I()
    S(80)
    K(0)
    S(40)
    K(1)
    S(150)

    # 9. sustain_park: decay into the ix-3 sustain hold (movement gated
    #    while down), release from the parked level (DEC-018 branch)
    P([80, 50, 40, 60], [99, 60, 70, 30], OUT99, 0)
    I()
    S(300)
    K(0)
    S(100)

    # 10. update_while_held: param refresh + the pinned advance(2)
    #     outlevel-overwrite quirk (env.cc:166-167)
    P([80, 50, 60, 50], [99, 60, 70, 40], OUT99, 0)
    I()
    S(30)
    U([70, 40, 50, 50], [99, 80, 60, 40], OUT68, 3)
    S(80)
    K(0)
    S(60)

    # 11. worst-rate sweep: every rate x rate_scaling {0, 1, 7, 27},
    #     attack/decay region + release (16,000 frame steps)
    for rs in (0, 1, 7, 27):
        for rate in range(100):
            P([rate] * 4, [99, 50, 70, 0], OUT99, rs)
            I()
            S(30)
            K(0)
            S(10)

    # 12. levellut sweep: every level 0..99 through segment targets
    #     (levellut[0..19] and the >= 20 branch), park + falling legs
    for lv in range(100):
        P([60] * 4, [lv] * 4, OUT99, 0)
        I()                                # advance(0) with levels[0]=lv
        S(3)
        tgt = golden_target(lv, OUT99)
        for k in (1, 2, 3):
            P([60] * 4, [lv] * 4, OUT99, 0)
            L(tgt, tgt, 12345, 0, k, 0, 0 if k == 3 else 1)
            A(k)                           # static park (target == level)
            S(2)

    # 13. statics sweeps. (a) attack-hold //20 path: ix 0 + levels[0] = 0
    #     -> staticrate = rate + rs covers every table entry and the
    #     >= 77 branch; (b) raw path: park at target == level, segment 1
    #     (no //20), staticrate = rate covers every entry raw
    for rate in range(100):
        P([rate] * 4, [0, 80, 60, 40], OUT99, 0)
        I()
    for rate in range(100):
        P([rate] * 4, [0, 80, 60, 40], OUT99, 27)
        I()
    for rate in range(100):
        tgt80 = golden_target(80, OUT99)
        P([rate] * 4, [80, 80, 80, 80], OUT99, 0)
        L(tgt80, tgt80, 12345, 0, 1, 0, 1)
        A(1)
        S(2)

    # 14. advance/park corners: ix-only commits (advance(4)/advance(5)),
    #     static decrement at parked ix, keyup from parked states
    for k in (4, 5):
        L(1 << 24, 2 << 24, 3 << 20, 100, k, 0, 1)
        A(k)
        S(3)
    L(1 << 24, 2 << 24, 3 << 20, 200, 4, 0, 1)
    K(0)                                   # down-edge advance(3) from park
    S(10)
    L(1 << 24, 2 << 24, 3 << 20, 100, 5, 0, 0)
    S(3)                                   # expiry advance(6) at ix 5

    # 15. rate_scaling sweep 0..27 x rate {0, 37, 99}: qrate-clamp and
    #     staticrate-clamp boundaries
    for rs in range(28):
        for rate in (0, 37, 99):
            P([rate] * 4, [99, 50, 70, 40], OUT99, rs)
            I()
            S(12)
            K(0)
            S(6)

    return cmds


class GoldenDriver:
    """Replays the command stream against the frozen model."""

    def __init__(self):
        self.env = E.Envelope()
        self.rates = [0, 0, 0, 0]
        self.levels = [0, 0, 0, 0]
        self.out = 0
        self.rs = 0
        self.prev_ix = 0
        self.stats = {"steps": 0, "advance_steps": 0, "double_steps": 0,
                      "keyedge_advances": 0}

    def apply(self, cmd):
        e = self.env
        kind = cmd[0]
        if kind == "P":
            _, r, l, out, rs = cmd
            self.rates, self.levels, self.out, self.rs = list(r), list(l), \
                out, rs
            e.rates_, e.levels_ = list(r), list(l)
            e.outlevel_, e.rate_scaling_ = out, rs
        elif kind == "I":
            e.init(self.rates, self.levels, self.out, self.rs)
        elif kind == "K":
            before = e.down_
            e.keydown(bool(cmd[1]))
            if before != bool(cmd[1]):
                self.stats["keyedge_advances"] += 1
        elif kind == "U":
            _, r, l, out, rs = cmd
            e.update(list(r), list(l), out, rs)
        elif kind == "S":
            self.prev_ix = e.ix_
            e.getsample()
            self.stats["steps"] += 1
            d = e.ix_ - self.prev_ix
            if d >= 1:
                self.stats["advance_steps"] += 1
            if d >= 2:
                self.stats["double_steps"] += 1
        elif kind == "A":
            e.advance(cmd[1])
        elif kind == "L":
            _, lv, tg, inc, st, ix, ri, dn = cmd
            e.level_, e.targetlevel_ = lv, tg
            e.inc_, e.staticcount_ = inc, st
            e.ix_, e.rising_, e.down_ = ix, bool(ri), bool(dn)
        else:
            raise CheckFailure(f"unknown command {cmd!r}")
        return self.state_tuple()

    def state_tuple(self):
        e = self.env
        return (e.level_ & M32, e.targetlevel_ & M32, e.inc_ & M32,
                e.staticcount_ & M32, e.ix_ & 7, int(bool(e.rising_)),
                int(bool(e.down_)))


# ---------------------------------------------------------------------------
# Vector / expected writers (deterministic bytes)
# ---------------------------------------------------------------------------

def vector_text(cmds):
    out = [
        "# h04 envelope-unit directed vectors (issue #26)",
        "# generated by tools/h04_compare.py from the FROZEN PYTHON MODEL",
        "# golden: src/gf180_dx7/model/envelope.py "
        "(contract gf180-dx7-integer-v1, N03)",
        "# P r0 r1 r2 r3 l0 l1 l2 l3 out rs  load derived param registers",
        "# I   init (level=0, down=1, advance(0))",
        "# K d keydown(d): down-edge advance from the current level",
        "# U r0 r1 r2 r3 l0 l1 l2 l3 out rs"
        "  update: param refresh + advance(2) while down",
        "# S   getsample: one 64-sample frame step",
        "# A k advance(k)",
        "# L lv tg inc st ix r d  direct slot load",
    ]
    for c in cmds:
        if c[0] == "P":
            out.append("P " + " ".join("%02x" % v for v in c[1])
                       + " " + " ".join("%02x" % v for v in c[2])
                       + " %04x %02x" % (c[3], c[4]))
        elif c[0] == "U":
            out.append("U " + " ".join("%02x" % v for v in c[1])
                       + " " + " ".join("%02x" % v for v in c[2])
                       + " %04x %02x" % (c[3], c[4]))
        elif c[0] in ("I", "S"):
            out.append(c[0])
        elif c[0] == "K":
            out.append("K %d" % c[1])
        elif c[0] == "A":
            out.append("A %d" % c[1])
        elif c[0] == "L":
            out.append("L %08x %08x %08x %08x %x %x %x" % c[1:])
        else:
            raise CheckFailure(f"unknown command {c!r}")
    return "\n".join(out) + "\n"


def expected_text(expected):
    out = [
        "# h04 expected committed slot state (golden model; BIT-EXACT bar)",
        "# <level hex8> <target hex8> <inc hex8> <static hex8>"
        " <ix hex1> <rising hex1> <down hex1>  per command, stream order",
    ]
    for row in expected:
        out.append("%08x %08x %08x %08x %x %x %x" % row)
    return "\n".join(out) + "\n"


def parse_result_text(path):
    rows = []
    with open(path, "r", encoding="ascii") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 7 or any(
                    len(p) > 8 or
                    not re.fullmatch(r"[0-9a-fA-F]+", p) for p in parts):
                raise CheckFailure(
                    f"{path}:{lineno}: malformed result line {line!r}")
            rows.append((int(parts[0], 16), int(parts[1], 16),
                         int(parts[2], 16), int(parts[3], 16),
                         int(parts[4], 16), int(parts[5], 16),
                         int(parts[6], 16)))
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


def run_sim(iverilog, vvp, define, tag, outdir):
    """Compile + run the bench; return (actual_rows, meta)."""
    if shutil.which(iverilog) is None:
        raise CouldNotRun(f"iverilog not found on PATH: {iverilog}")
    if shutil.which(vvp) is None:
        raise CouldNotRun(f"vvp not found on PATH: {vvp}")
    tmp = tempfile.mkdtemp(prefix="h04_sim_")
    vvp_out = os.path.join(tmp, f"tb_{tag}.vvp")
    actual_path = os.path.join(outdir,
                               f"actual{('_' + tag) if tag else ''}.txt")
    cmd = [iverilog, "-g2012", "-o", vvp_out]
    if define:
        cmd.append(define)
    cmd += [os.path.join(REPO_ROOT, RTL_REL), os.path.join(REPO_ROOT, TB_REL)]
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=REPO_ROOT)
    if proc.returncode != 0:
        raise CheckFailure(f"iverilog compile failed ({tag}): "
                           + (proc.stderr or proc.stdout)[-500:])
    vectors = os.path.join(outdir, "vectors.txt")
    try:
        run = subprocess.run([vvp, vvp_out, f"+vectors={vectors}",
                              f"+actual={actual_path}"],
                             capture_output=True, text=True, cwd=REPO_ROOT,
                             timeout=1800)
        out = run.stdout + run.stderr
        if run.returncode != 0 or "FAIL" in out:
            raise CheckFailure(f"bench failed ({tag}): {out.strip()[-500:]}")
        m = re.search(r"PASS tb_env_unit \(cmds=(\d+) steps=(\d+) "
                      r"max_step_cycles=(\d+) max_cmd_cycles=(\d+) "
                      r"hazard=(\w+)\)", out)
        if not m:
            raise CheckFailure(
                f"bench produced no PASS summary ({tag}): {out[-300:]}")
        rows = parse_result_text(actual_path)
    finally:
        try:
            os.remove(vvp_out)
        except OSError:
            pass
    return rows, {
        "bench_cmds": int(m.group(1)),
        "bench_steps": int(m.group(2)),
        "bench_max_step_cycles": int(m.group(3)),
        "bench_max_cmd_cycles": int(m.group(4)),
        "bench_hazard": m.group(5),
        "actual_file": actual_path,
        "vvp_stdout_tail": out.strip()[-200:],
    }


def compare_bit_exact(expected, actual, cmds):
    """BIT-EXACT comparison; returns (mismatches, first_mismatch|None)."""
    mismatches = []
    first = None
    if len(actual) != len(expected):
        mismatches.append({
            "cmd_index": min(len(expected), len(actual)),
            "error": f"row-count mismatch: expected {len(expected)} rows, "
                     f"RTL dumped {len(actual)}",
        })
    for n in range(min(len(expected), len(actual))):
        e_row, a_row = expected[n], actual[n]
        if e_row != a_row:
            bad = [FIELDS[i] for i in range(7)
                   if (e_row[i] & M32) != (a_row[i] & M32)]
            mm = {
                "cmd_index": n,
                "command": " ".join(str(x) for x in cmds[n]),
                "fields": bad,
                "expected": [f"{v & M32:x}" for v in e_row],
                "actual": [f"{v & M32:x}" for v in a_row],
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
    ap.add_argument("--skip-mutants", action="store_true",
                    help="skip both negative-control mutant builds")
    ap.add_argument("--mutants-only", action="store_true",
                    help="run only the negative-control mutant builds")
    ap.add_argument("--iverilog", default="iverilog")
    ap.add_argument("--vvp", default="vvp")
    args = ap.parse_args(argv)

    cmds = build_commands()
    driver = GoldenDriver()
    expected = [driver.apply(c) for c in cmds]
    stats = driver.stats
    stats["commands"] = len(cmds)
    stats["min_evals_required"] = MIN_EVALS
    if stats["steps"] < MIN_EVALS:
        print(f"FAIL: vector set has {stats['steps']} frame steps "
              f"< required {MIN_EVALS}", file=sys.stderr)
        return 1

    os.makedirs(args.outdir, exist_ok=True)
    vectors_path = os.path.join(args.outdir, "vectors.txt")
    expected_path = os.path.join(args.outdir, "expected.txt")
    with open(vectors_path, "w", encoding="ascii") as f:
        f.write(vector_text(cmds))
    with open(expected_path, "w", encoding="ascii") as f:
        f.write(expected_text(expected))

    if args.gen_only:
        report = {
            "schema": "h04-gen-report/1",
            "issue": 26,
            "planning_id": "H04",
            "golden": "src/gf180_dx7/model/envelope.py "
                      "(gf180-dx7-integer-v1, N03)",
            "sr_multiplier": E.SR_MULTIPLIER_48K,
            "sr_multiplier_formula": "uint32 truncation of "
                                     "(44100.0/sample_rate)*(1<<24); "
                                     "at 48 kHz: floor(15414067.2)",
            "stats": stats,
            "vectors_sha256": sha256_file(vectors_path),
            "expected_sha256": sha256_file(expected_path),
        }
        with open(os.path.join(args.outdir, "gen_report.json"), "w",
                  encoding="utf-8") as f:
            json.dump(report, f, indent=1, sort_keys=True)
            f.write("\n")
        print(f"h04_compare: generated {stats['commands']} commands "
              f"({stats['steps']} frame steps) -> {vectors_path}")
        return 0

    try:
        ident = tool_identity(args.iverilog)
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}", file=sys.stderr)
        return 2

    report = {
        "schema": "h04-compare-report/1",
        "issue": 26,
        "planning_id": "H04",
        "claim_boundary": "RTL-vs-frozen-model bit-exactness + measured "
                          "step-cycle bound only; no synthesis, timing, "
                          "fit, or audibility claim",
        "tool_identity": {
            "iverilog": ident,
            "python": sys.version.split()[0],
            "rtl": {p: sha256_file(os.path.join(REPO_ROOT, p))
                    for p in (RTL_REL, TB_REL)},
            "sr_multiplier": E.SR_MULTIPLIER_48K,
        },
        "vector_stats": stats,
        "vectors_sha256": sha256_file(vectors_path),
        "expected_sha256": sha256_file(expected_path),
    }

    mutants = (
        ("timing", "H04_MUTATE_STEP_SKEW",
         "update shifted one frame late (movement commit deferred to the "
         "next step's capture)"),
        ("srm", "H04_MUTATE_SRM",
         "sr_multiplier - 1 (N03 section 6: reachable inc_ coefficients "
         "flip)"),
    )

    status = "FAIL"
    try:
        if not args.mutants_only:
            actual, meta = run_sim(args.iverilog, args.vvp, None, "",
                                   args.outdir)
            mismatches, first = compare_bit_exact(expected, actual, cmds)
            budget_ok = meta["bench_max_step_cycles"] <= MAX_STEP_CLKS
            hazard_ok = meta["bench_hazard"] == "ok"
            report["equivalence"] = dict(
                meta,
                expected_rows=len(expected),
                actual_rows=len(actual),
                mismatches=len(mismatches),
                first_mismatch=first,
                step_cycle_gate={
                    "bound_clks": MAX_STEP_CLKS,
                    "basis": "H03 spec/core-contract-v1.json budget row "
                             "env_steps (derived_estimate 24 clk/op/frame "
                             "worst case incl. segment advance); measured "
                             "over the directed set incl. worst-rate "
                             "updates",
                    "measured_clks": meta["bench_max_step_cycles"],
                    "status": "PASS" if budget_ok else "FAIL",
                },
                hazard_check=meta["bench_hazard"],
                status="PASS" if (not mismatches and budget_ok
                                  and hazard_ok) else "FAIL",
            )

        if not args.skip_mutants:
            report["negative_controls"] = {}
            for tag, define, describe in mutants:
                m_actual, m_meta = run_sim(args.iverilog, args.vvp,
                                           f"-D{define}", tag, args.outdir)
                m_mismatch, m_first = compare_bit_exact(expected, m_actual,
                                                        cmds)
                met = len(m_mismatch) > 0
                report["negative_controls"][tag] = dict(
                    m_meta,
                    define=define,
                    description=describe,
                    mismatches=len(m_mismatch),
                    first_mismatch=m_first,
                    status="MET (mutant fails equivalence as required)"
                    if met else "NOT MET (mutant passed -- harness has no "
                                "resolution)",
                )

        eq_ok = args.mutants_only or \
            report["equivalence"]["status"] == "PASS"
        nc_ok = args.skip_mutants or args.mutants_only or all(
            v["status"].startswith("MET")
            for v in report.get("negative_controls", {}).values())
        status = "PASS" if (eq_ok and nc_ok) else "FAIL"
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
              f"({eq['expected_rows']} cmds / {stats['steps']} steps, "
              f"{eq['mismatches']} mismatches)")
        if eq["first_mismatch"]:
            fm = eq["first_mismatch"]
            print(f"  first mismatch at cmd {fm.get('cmd_index')} "
                  f"({fm.get('command')}) fields={fm.get('fields')}: "
                  f"expected={fm.get('expected')} "
                  f"actual={fm.get('actual')}"
                  + (f" [{fm['error']}]" if fm.get("error") else ""))
        gate = eq["step_cycle_gate"]
        print(f"step-cycle gate: {gate['status']} "
              f"(measured {gate['measured_clks']} clk <= "
              f"{gate['bound_clks']} clk H03 budget)")
        print(f"hazard: {eq['hazard_check']}")
    if "negative_controls" in report:
        for tag, nc in report["negative_controls"].items():
            print(f"negative control {tag}: {nc['status']} "
                  f"({nc['mismatches']} mismatches)")
    print(f"report: {os.path.join(args.outdir, 'compare_report.json')}")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
