#!/usr/bin/env python3
"""H08 pin-level integration evidence runner.

Model (per issue #30 acceptance + docs/H08-PIN.md):
  PRIMARY   pin-decoded I2S stream == flat-interface H07 tap stream,
            byte-exact, for each committed H07 case (same logical command
            sequence, same pinned core). This is the "behavior identical via
            serialized pins" acceptance.
  INFO      pin stream vs the COMMITTED H07 golden, reported separately:
            a committed golden that no longer matches a fresh rerun of the
            flat bench against the pinned core is an H07-side evidence
            staleness finding (recorded), not an H08 defect.
  STATUS    pin-run meta: overrun/overflow = 0 (matrix), structural BCLK
            period 8 / LRCLK period 512 (as-built findings F-I2S-1/2).
  INJ       corrupted / truncated / invalid SPI transactions
            (bench vector protocol: P/C/E/S/T/R/W/Q/X/V/Y/Z):
              trunc47    live 47-bit (undersized) frame: DUT latches no
                         frame (bit_cnt restarts at next cs_fall) -> run
                         must equal the control vector (frame absent) and
                         the forensic frame-latch count must match.
              f0         full 48-bit F=0 frame sent in patch-write shape:
                         must be a no-op (stream == control, FRESH kept).
              freshctl   FRESH status bit stays 1 after F=0-only traffic
                         and clears to 0 after one accepted write
                         (partial/incomplete state observable via status).
              ovf        deliberate SCK rate violation (Y burst,
                         20 ps/bit < 194 boundary): 330 events in one
                         32768-clk frame overfill the depth-168 event
                         queue -> OVERFLOW must demonstrably SET (sticky),
                         OVERRUN must not set, and a pad reset (Z) must
                         clear OVERFLOW. (The depth-7 skid overflow is
                         unreachable at any SCK rate: drained every tick,
                         pushed at >= 194 clk/tx — finding F-OVF-1.)
              partial    first half of a patch block + commit: partial
                         image must be observable (stream differs from
                         full) and pin/flat must stay identical.
  Icarus shadow: NOT_RUN in this environment (iverilog 13.0 / 14.0-devel
  crash in codegen on dx7_core elaboration — documented in the summary;
  a test that cannot run is never a pass).

Usage:  python3 tools/h08_pin_check.py [--cases C1,C2] [--skip-inj]
"""
import argparse, json, os, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
H07 = os.path.join(ROOT, "evidence", "h07-core", "runs")
OUT = os.path.join(ROOT, "evidence", "h08-pin", "runs")
RES = os.path.join(ROOT, "results", "h08")
BLD = os.path.join(ROOT, "build", "h08")
FLAT = os.path.join(BLD, "simtbH.obj", "simtbH")
PIN = os.path.join(BLD, "simtbA.obj", "simtbA")
RTL = [os.path.join(ROOT, "rtl", f) for f in
       ("dx7_core.v", "env_unit.v", "alg_router.v")]

def build_if_stale(top, bench_rtls, exe):
    srcs = RTL + bench_rtls
    new = max(os.path.getmtime(s) for s in srcs)
    if os.path.exists(exe) and os.path.getmtime(exe) >= new:
        return
    os.makedirs(os.path.dirname(exe), exist_ok=True)
    cmd = ["verilator", "--binary", "--timing", "-j", "4", "--Wno-fatal",
           "--timescale-override", "1ns/1ps", "--x-initial", "0",
           "--x-assign", "unique", "-Mdir", os.path.dirname(exe),
           "-o", os.path.basename(exe), "--top-module", top] + srcs
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0 or not os.path.exists(exe):
        print("FATAL build (%s):\n%s" % (top, r.stdout[-3000:]),
              file=sys.stderr)
        sys.exit(2)

ALL_CASES = ["dir-base",
             "alg4-fb0", "alg4-fb3", "alg4-fb7",
             "alg6-fb0", "alg6-fb3", "alg6-fb7",
             "bd-coarse0", "bd-coarse31", "bd-detune0", "bd-detune14"]


def run_sim(exe, vec, actual, meta, timeout=2400):
    p = subprocess.Popen([exe, "+vectors=" + vec,
                          "+actual=" + actual, "+meta=" + meta],
                         stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = p.communicate(timeout=timeout)[0].decode()
    ok = (p.returncode == 0)
    if re.search(r"FAIL tb_", out):
        ok = False
    return ok, out

def h07_vector(case):
    d = os.path.join(H07, "verilog-accept-dev-" + case)
    v = os.path.join(d, "vector.txt")
    if not os.path.exists(v):
        print("FATAL missing committed H07 vector: %s" % v, file=sys.stderr)
        sys.exit(2)
    return v, d

def parse_meta(path):
    m = {"qreads": []}
    if os.path.exists(path):
        for line in open(path):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            k, _, v = line.partition(" ")
            if k == "qread":
                m["qreads"].append(v)
            else:
                m[k] = v
    return m

def cmp_bytes(a, b):
    if not (os.path.exists(a) and os.path.exists(b)):
        return None
    la = open(a, "rb").read()
    lb = open(b, "rb").read()
    if len(la) != len(lb):
        return ("len", len(la), len(lb))
    for i in range(len(la)):
        if la[i] != lb[i]:
            return ("diff", i)
    return ("identical", len(la))

def status_bits(q):
    """qread word -> dict of the decoded status bits (bit layout traced:
    {D7,1,stun[19],queue[18],ovf[17],fresh[16],frame[15:0]} — wait: per
    dx7_core source: {8'hD7, 4'h1, st_stun, st_queue, st_overflow,
    st_fresh, frame_ctr} so bits 19,18,17,16."""
    v = int(q, 16)
    return {"stun": (v >> 19) & 1, "queue": (v >> 18) & 1,
            "ovf": (v >> 17) & 1, "fresh": (v >> 16) & 1,
            "frame": v & 0xFFFF}

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--cases", default=",".join(ALL_CASES))
    ap.add_argument("--skip-inj", action="store_true")
    ap.add_argument("--skip-build", action="store_true")
    args = ap.parse_args()
    cases = [c for c in args.cases.split(",") if c]
    os.makedirs(OUT, exist_ok=True)
    os.makedirs(RES, exist_ok=True)
    if not args.skip_build:
        build_if_stale("tb_dx7_core",
                       [os.path.join(ROOT, "rtl", "tb_dx7_core.v")], FLAT)
        build_if_stale("tb_synth_top",
                       [os.path.join(ROOT, "rtl", "synth_top.v"),
                        os.path.join(ROOT, "rtl", "tb_synth_top.v")], PIN)

    results = {}
    verdicts = []

    # ---------------- committed H07 matrix ----------------
    for case in cases:
        r = os.path.join(OUT, case)
        os.makedirs(r, exist_ok=True)
        hvec, hd = h07_vector(case)
        g = os.path.join(hd, "actual.i32")
        shutil.copy(hvec, os.path.join(r, "vector.txt"))
        shutil.copy(g, os.path.join(r, "golden-committed.i32"))

        fok, fout = run_sim(FLAT, hvec, os.path.join(r, "flat.i32"),
                            os.path.join(r, "flat.meta"))
        pok, pout = run_sim(PIN, hvec, os.path.join(r, "pin.i32"),
                            os.path.join(r, "pin.meta"))
        open(os.path.join(r, "log-flat.txt"), "w").write(fout)
        open(os.path.join(r, "log-pin.txt"), "w").write(pout)

        pm = parse_meta(os.path.join(r, "pin.meta"))
        fm = parse_meta(os.path.join(r, "flat.meta"))
        cf = cmp_bytes(os.path.join(r, "pin.i32"), os.path.join(r, "flat.i32"))
        cg = cmp_bytes(os.path.join(r, "pin.i32"), g)
        bad = []
        if not fok or not pok:
            bad.append("rc/FAIL-flag flat=%s pin=%s" % (fok, pok))
        if cf is None or cf[0] != "identical":
            bad.append("pin != flat: %s" % (cf,))
        for k in ("samples", "overrun", "overflow"):
            if k in fm and pm.get(k) != fm.get(k):
                bad.append("pin meta %s=%s flat %s" %
                           (k, pm.get(k), fm.get(k)))
        if pm.get("overrun") not in (None, "0") or \
           pm.get("overflow") not in (None, "0"):
            bad.append("pin overrun/overflow set: %s / %s" %
                       (pm.get("overrun"), pm.get("overflow")))
        if pm.get("bclk_period") != "8" or pm.get("lr_period") != "512":
            bad.append("pin bclk/lr period %s/%s" %
                       (pm.get("bclk_period"), pm.get("lr_period")))
        status = "FAIL" if bad else "PASS"
        verdicts.append((
            "case:%s" % case, status,
            "; ".join(bad) if bad else
            "pin==flat byte-exact (%d samples); committed golden %s" %
            (int(pm.get("samples", "?").split()[0])
             if pm.get("samples") else -1,
             "identical" if cg and cg[0] == "identical" else
             ("diff@%s" % (cg[1],) if cg and cg[0] == "diff" else str(cg)))))
        results[case] = {"pin_vs_flat": cf,
                         "pin_vs_committed_golden": cg,
                         "pin_samples": pm.get("samples"),
                         "flat_samples": fm.get("samples"),
                         "pin_overrun": pm.get("overrun"),
                         "pin_overflow": pm.get("overflow"),
                         "pin_qread_last": pm["qreads"][-1]
                         if pm["qreads"] else None}

    # ---------------- injections ----------------
    if not args.skip_inj:
        base_vec, _ = h07_vector("dir-base")
        lines = [l for l in open(base_vec).read().splitlines() if l]
        pidx = [i for i, l in enumerate(lines) if l.startswith("P ")]
        ci = next(i for i, l in enumerate(lines) if l == "C" or l.startswith("C "))
        pblk = [i for i in pidx if i < ci]

        def pfields(line):
            # "P <addr:2> <data:8>"
            addr = line[2:4]; data = line[5:13]
            return addr, data

        # --- trunc47: the last P line becomes a 47-bit frame (X)
        ti = pblk[-1]
        addr, data = pfields(lines[ti])
        v_trunc = lines[:ti] + ["X 1 %s %s 47" % (addr, data)] + \
            lines[ti+1:]
        v_ctrl = lines[:ti] + lines[ti+1:]
        r = os.path.join(OUT, "inj-trunc47"); os.makedirs(r, exist_ok=True)
        vt = os.path.join(r, "vec-trunc.txt"); vc = os.path.join(r, "vec-ctrl.txt")
        open(vt, "w").write("\n".join(v_trunc) + "\n")
        open(vc, "w").write("\n".join(v_ctrl) + "\n")
        fok, fout = run_sim(FLAT, vc, os.path.join(r, "flat.i32"),
                            os.path.join(r, "flat.meta"))
        pok, pout = run_sim(PIN, vt, os.path.join(r, "pin.i32"),
                            os.path.join(r, "pin.meta"))
        pokc, poutc = run_sim(PIN, vc, os.path.join(r, "pin-ctrl.i32"),
                              os.path.join(r, "pin-ctrl.meta"))
        open(os.path.join(r, "log.txt"), "w").write(
            "== trunc ==\n" + pout + "== ctrl ==\n" + poutc)
        mtr = re.search(r"frame_latches=(\d+)", pout)
        mct = re.search(r"frame_latches=(\d+)", poutc)
        frc_t = int(mtr.group(1)) if mtr else None
        frc_c = int(mct.group(1)) if mct else None
        cf = cmp_bytes(os.path.join(r, "pin.i32"),
                       os.path.join(r, "pin-ctrl.i32"))
        cf2 = cmp_bytes(os.path.join(r, "pin.i32"), os.path.join(r, "flat.i32"))
        bad = []
        if not (fok and pok and pokc):
            bad.append("rc/FAIL-flag")
        if cf is None or cf[0] != "identical":
            bad.append("trunc run != control: %s" % (cf,))
        if cf2 is None or cf2[0] != "identical":
            bad.append("trunc run != flat: %s" % (cf2,))
        if frc_t is None or frc_c is None or frc_t != frc_c:
            bad.append("frame-latch count %s vs %s (trunc must not latch)"
                       % (frc_t, frc_c))
        verdicts.append(("inj:trunc47", "FAIL" if bad else "PASS",
                         "; ".join(bad) if bad else
                         "47-bit frame latches nothing: stream==control"
                         "==flat, latches %s==%s" % (frc_t, frc_c)))
        results["inj-trunc47"] = {"pin_vs_ctrl": cf, "pin_vs_flat": cf2,
                                  "frc_trunc": frc_t, "frc_ctrl": frc_c}

        # --- f0: last P line becomes a full 48-bit F=0 frame (V)
        v_f0 = lines[:ti] + ["V %s %s" % (addr, data)] + lines[ti+1:]
        r = os.path.join(OUT, "inj-f0"); os.makedirs(r, exist_ok=True)
        open(os.path.join(r, "vec-f0.txt"), "w").write("\n".join(v_f0) + "\n")
        shutil.copy(vc, os.path.join(r, "vec-ctrl.txt"))
        pok, pout = run_sim(PIN, os.path.join(r, "vec-f0.txt"),
                            os.path.join(r, "pin.i32"),
                            os.path.join(r, "pin.meta"))
        pokc, poutc = run_sim(PIN, os.path.join(r, "vec-ctrl.txt"),
                              os.path.join(r, "pin-ctrl.i32"),
                              os.path.join(r, "pin-ctrl.meta"))
        open(os.path.join(r, "log.txt"), "w").write(pout)
        cf = cmp_bytes(os.path.join(r, "pin.i32"),
                       os.path.join(r, "pin-ctrl.i32"))
        bad = []
        if not (pok and pokc):
            bad.append("rc/FAIL-flag")
        if cf is None or cf[0] != "identical":
            bad.append("F=0 frame altered stream: %s" % (cf,))
        verdicts.append(("inj:f0", "FAIL" if bad else "PASS",
                         "; ".join(bad) if bad else
                         "F=0 frame is a no-op (stream==control)"))
        results["inj-f0"] = {"pin_vs_ctrl": cf}

        # --- freshctl: FRESH bit observability (status bit 16)
        r = os.path.join(OUT, "inj-freshctl"); os.makedirs(r, exist_ok=True)
        open(os.path.join(r, "vec-f0.txt"), "w").write(
            "Q\nQ\nV 10 00000001\nQ\nQ\nW 8\n")
        open(os.path.join(r, "vec-wr.txt"), "w").write(
            "Q\nQ\nP 10 00000001\nQ\nQ\nW 8\n")
        pok, pout = run_sim(PIN, os.path.join(r, "vec-f0.txt"),
                            os.path.join(r, "pin-f0.i32"),
                            os.path.join(r, "pin-f0.meta"))
        pokc, poutc = run_sim(PIN, os.path.join(r, "vec-wr.txt"),
                              os.path.join(r, "pin-wr.i32"),
                              os.path.join(r, "pin-wr.meta"))
        mf = parse_meta(os.path.join(r, "pin-f0.meta"))
        mw = parse_meta(os.path.join(r, "pin-wr.meta"))
        sf = [status_bits(q) for q in mf["qreads"]]
        sw = [status_bits(q) for q in mw["qreads"]]
        # reads: 1: stale-0 (loaded at reset), 2: status after txn1 (post-Q1),
        # 3: after txn3 (post V/P), 4: after txn5 (final pre-loop), + end read
        q_f_last = sf[-1] if sf else None
        q_w_last = sw[-1] if sw else None
        bad = []
        if not (pok and pokc):
            bad.append("rc/FAIL-flag")
        if q_f_last is None or q_w_last is None:
            bad.append("qread missing (f0=%s wr=%s)" % (q_f_last, q_w_last))
        else:
            if q_f_last["fresh"] != 1:
                bad.append("FRESH not 1 after F=0-only traffic: %s"
                           % mf["qreads"][-1])
            if not (q_f_last["ovf"] == 0 and q_f_last["queue"] == 0):
                bad.append("unexpected bits after F=0-only: %s"
                           % mf["qreads"][-1])
            if q_w_last["fresh"] != 0:
                bad.append("FRESH not cleared by accepted write: %s"
                           % mw["qreads"][-1])
        verdicts.append(("inj:freshctl", "FAIL" if bad else "PASS",
                         "; ".join(bad) if bad else
                         "FRESH stays 1 (F=0-only), clears on accepted write"))
        results["inj-freshctl"] = {
            "qreads_f0": mf["qreads"], "qreads_wr": mw["qreads"]}

        # --- ovf: Y burst overfills EVQ (168) within one frame; sticky;
        # pad reset clears; overrun never
        r = os.path.join(OUT, "inj-ovf"); os.makedirs(r, exist_ok=True)
        ov = os.path.join(r, "vec.txt")
        open(ov, "w").write(
            "Q\nQ\n"          # baseline status reads (read2 = post-reset)
            "Y 0 330 30 00000001\n"  # 330 events @ 98 clk in one frame
            "Q\nQ\n"          # read4 must show ovf=1
            "Z 60\n"          # pad reset clears sticky flags
            "Q\nQ\nW 8\n")    # read6/7 must show ovf=0 fresh=1
        pok, pout = run_sim(PIN, ov, os.path.join(r, "pin.i32"),
                            os.path.join(r, "pin.meta"))
        open(os.path.join(r, "log.txt"), "w").write(pout)
        mo = parse_meta(os.path.join(r, "pin.meta"))
        st = [status_bits(q) for q in mo["qreads"]]
        # qread index map (F-STA-1, empirically fitted on 3 runs):
        #  r[i] = the value latched at the PREVIOUS transaction end
        #  (miso_sh reloads status at transaction end; the reset arm of
        #  the DUT's miso_sh FSM re-arms the 0x00000000 canary, so the
        #  first read after any (pad) reset is 0x00000000 and the real
        #  post-reset word arrives one read later).
        #  0 canary 0, 1 post-reset (fresh=1), 2 post-burst (ovf=1),
        #  3 sticky re-read (ovf=1), 4 post-pad-reset canary 0,
        #  5 post-reset re-arm (fresh=1, ovf=0), 6 end-of-loop
        bad = []
        if not pok:
            bad.append("rc/FAIL-flag")
        if mo.get("overrun") != "0":
            bad.append("st_overrun forensic: %s" % mo.get("overrun"))
        if len(st) < 7:
            bad.append("need >=7 status reads, got %d" % len(st))
        else:
            if st[2]["ovf"] != 1:
                bad.append("OVERFLOW not set after 330-event burst: %s"
                           % mo["qreads"][2])
            if st[3]["ovf"] != 1:
                bad.append("OVERFLOW not sticky on re-read: %s"
                           % mo["qreads"][3])
            if mo["qreads"][4] != "0x00000000":
                bad.append("post-pad-reset canary not 0x00000000: %s"
                           % mo["qreads"][4])
            if st[5]["fresh"] != 1 or st[5]["ovf"] != 0:
                bad.append("FRESH not restored / OVERFLOW not cleared by "
                           "pad reset: %s" % mo["qreads"][5])
            for i in (4, 5, 6):
                if st[i]["ovf"] != 0:
                    bad.append("OVERFLOW still set post reset (idx %d): %s"
                               % (i, mo["qreads"][i]))
        verdicts.append(("inj:ovf", "FAIL" if bad else "PASS",
                         "; ".join(bad) if bad else
                         "EVQ overfill sets sticky OVERFLOW (qread2/3=1); "
                         "pad reset clears (qread4+), FRESH restored; "
                         "OVERRUN never (forensic 0)"))
        results["inj-ovf"] = {"qreads": mo["qreads"]}

        # --- partial: drop the first half of the patch block, commit
        half = len(pblk) // 2
        drop = set(pblk[:half])
        v_part = [l for i, l in enumerate(lines) if i not in drop]
        r = os.path.join(OUT, "inj-partial"); os.makedirs(r, exist_ok=True)
        open(os.path.join(r, "vec.txt"), "w").write("\n".join(v_part) + "\n")
        shutil.copy(os.path.join(OUT, "dir-base", "pin.i32"),
                    os.path.join(r, "full-pin.i32"))
        fok, fout = run_sim(FLAT, os.path.join(r, "vec.txt"),
                            os.path.join(r, "flat.i32"),
                            os.path.join(r, "flat.meta"))
        pok, pout = run_sim(PIN, os.path.join(r, "vec.txt"),
                            os.path.join(r, "pin.i32"),
                            os.path.join(r, "pin.meta"))
        open(os.path.join(r, "log.txt"), "w").write(pout)
        cf = cmp_bytes(os.path.join(r, "pin.i32"), os.path.join(r, "flat.i32"))
        cfull = cmp_bytes(os.path.join(r, "pin.i32"),
                          os.path.join(r, "full-pin.i32"))
        bad = []
        if not (fok and pok):
            bad.append("rc/FAIL-flag")
        if cf is None or cf[0] != "identical":
            bad.append("pin != flat for partial patch: %s" % (cf,))
        if cfull is not None and cfull[0] == "identical":
            bad.append("partial-commit stream identical to full (unobservable)")
        verdicts.append(("inj:partial", "FAIL" if bad else "PASS",
                         "; ".join(bad) if bad else
                         "partial image observable (diff@%s) and pin==flat"
                         % ((cfull[1],) if cfull
                            and cfull[0] == "diff" else "-")))
        results["inj-partial"] = {"pin_vs_flat": cf, "pin_vs_full": cfull}

    # ---------------- summary ----------------
    summ = {"verdicts": [{"name": n, "status": s, "detail": d}
                         for (n, s, d) in verdicts],
            "runs": results,
            "iverilog_shadow": "NOT_RUN",
            "iverilog_reason": ("iverilog 13.0 and 14.0-devel both crash "
                                "during codegen on dx7_core elaboration "
                                "('Code generator failure: -1'); reproduces "
                                "on the committed H07-benched core, so this "
                                "is a pre-existing environment regression, "
                                "not an H08 defect. Verilator 5.052 is the "
                                "sole executing tool.")}
    open(os.path.join(RES, "summary.json"), "w").write(
        json.dumps(summ, indent=1))
    fails = 0
    for (n, s, d) in verdicts:
        print("%-20s %-11s %s" % (n, s, d))
        if s not in ("PASS", "NOT_RUN"):
            fails += 1
    print("TOTAL %d  PASS %d  FAIL %d" %
          (len(verdicts), sum(1 for (_, s, _) in verdicts if s == "PASS"),
           fails))
    return 1 if fails else 0

if __name__ == "__main__":
    sys.exit(main())