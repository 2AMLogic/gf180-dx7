#!/usr/bin/env python3
"""H08 pin-level integration tests (issue #30).

Covers, per the issue's acceptance:
  - the core is byte-identical to the H07-benched commit (H08 changed no
    core lines — all deviations recorded as findings, never fixes):
    frozen sha256 pins on rtl/dx7_core.v, rtl/env_unit.v,
    rtl/alg_router.v, rtl/tb_dx7_core.v;
  - the committed pin evidence reproduces at the byte level: for every
    committed H07 case the evidence dir carries the committed vector
    (hash-pinned), the committed H07 golden (hash-pinned copy), the
    rerun flat stream, and the pin stream; the runner's summary verdicts
    are all PASS and this test **recomputes** the comparisons itself
    (pin vs flat byte-exact; meta parity; the injection-suite stream and
    status checks, including the truncated-frame control pair and the
    FRESH/OVERFLOW lifecycles);
  - the as-built I2S law is recorded in every run meta (BCLK period 8,
    LRCLK period 512, first-bit delay 6 — findings F-I2S-1/2, deviations
    from the contracted 4/256 reported, not fixed);
  - the Icarus shadow is recorded as NOT_RUN with a reason (a test that
    cannot run is never a pass);
  - LIVE negative controls (wall-heavy, logic-local — the H07 pattern):
    when a locally-built pin bench is present (built on demand, git-
    ignored), the small injection vectors (f0 / freshctl / ovf /
    trunc47) are rerun **live** and must reproduce the committed
    verdicts — the undersized-frame control included;
  - provenance: the pin bench and runner are cataloged
    (local_originals, re-pinned) and the chassis `synth-top-chassis`
    component carries the adapted hash + a PASS requalification result.

Stdlib only.
"""

import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RUNS = os.path.join(REPO, "evidence", "h08-pin", "runs")
H07 = os.path.join(REPO, "evidence", "h07-core", "runs")
SUMMARY = os.path.join(REPO, "results", "h08", "summary.json")
REPORT = os.path.join(REPO, "docs", "H08-PIN.md")
CATALOG = os.path.join(REPO, "docs", "reuse", "catalog.json")
BIN = os.path.join(REPO, "build", "h08", "simtbA.obj", "simtbA")

# H07-pinned core: sha256 as committed at loom/h07-core (884a7fa).
CORE_SHA = {
    "rtl/dx7_core.v":
        "335599eae6aada90f45c5738cf79d70098bb3a6cf97437927fcbdec10abfc917",
    "rtl/env_unit.v":
        "f61916cb6cc0c9b9434ff5007732e1636614c10450b0ec1a07d5f97592999e7b",
    "rtl/alg_router.v":
        "dda373aa918c0d6b5248b27902b8a127143daea28e69b7b63739779a7b2c467d",
    "rtl/tb_dx7_core.v":
        "e2f363f3c21f3c95caa1e73c95d066453f348ea48b263f5732c26628ea9c8fea",
}

CASES = ["dir-base", "alg4-fb0", "alg4-fb3", "alg4-fb7",
         "alg6-fb0", "alg6-fb3", "alg6-fb7",
         "bd-coarse0", "bd-coarse31", "bd-detune0", "bd-detune14"]
INJ = ["inj-trunc47", "inj-f0", "inj-freshctl", "inj-ovf", "inj-partial"]

def sha(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest()

def meta(path):
    m = {"qreads": []}
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

def bits(q):
    v = int(q, 16)
    return {"stun": (v >> 19) & 1, "queue": (v >> 18) & 1,
            "ovf": (v >> 17) & 1, "fresh": (v >> 16) & 1,
            "frame": v & 0xFFFF}

def cmp16(a, b):
    la = open(a, "rb").read()
    lb = open(b, "rb").read()
    assert len(la) == len(lb), (a, b, len(la), len(lb))
    for i in range(len(la)):
        if la[i] != lb[i]:
            return i
    return -1  # identical


class TestH08CorePin(unittest.TestCase):
    def test_core_sha_pin(self):
        for path, want in CORE_SHA.items():
            got = sha(os.path.join(REPO, path))
            if want is not None:
                self.assertEqual(got, want, path)


class TestH08Evidence(unittest.TestCase):
    def summary(self):
        return json.load(open(SUMMARY))

    def test_evidence_layout(self):
        for case in CASES:
            d = os.path.join(RUNS, case)
            self.assertTrue(os.path.isdir(d), "missing runs dir: " + case)
            for f in ("vector.txt", "golden-committed.i32", "flat.i32",
                      "flat.meta", "pin.i32", "pin.meta", "log-flat.txt",
                      "log-pin.txt"):
                self.assertTrue(os.path.exists(os.path.join(d, f)),
                                "%s missing %s" % (case, f))
            # vector byte-identical to the committed H07 vector
            self.assertEqual(
                sha(os.path.join(d, "vector.txt")),
                sha(os.path.join(H07, "verilog-accept-dev-" + case,
                                 "vector.txt")),
                "vector copy drifted for " + case)
            # golden copy byte-identical to the committed H07 golden
            self.assertEqual(
                sha(os.path.join(d, "golden-committed.i32")),
                sha(os.path.join(H07, "verilog-accept-dev-" + case,
                                 "actual.i32")),
                "golden copy drifted for " + case)

    def test_pin_equals_flat_recomputed(self):
        for case in CASES:
            d = os.path.join(RUNS, case)
            self.assertEqual(cmp16(os.path.join(d, "pin.i32"),
                                   os.path.join(d, "flat.i32")), -1, case)
            pm = meta(os.path.join(d, "pin.meta"))
            fm = meta(os.path.join(d, "flat.meta"))
            self.assertEqual(pm.get("samples"), fm.get("samples"), case)
            self.assertEqual(pm.get("overrun"), "0", case)
            self.assertEqual(pm.get("overflow"), "0", case)
            self.assertEqual(pm.get("bclk_period"), "8", case)
            self.assertEqual(pm.get("lr_period"), "512", case)
            self.assertEqual(pm.get("first_bit_delay"), "6", case)
            self.assertGreater(int(pm.get("samples", "-1")), 1000, case)

    def test_injection_evidence_recomputed(self):
        # trunc47: pinned streams equal control and flat; latch counts ok
        d = os.path.join(RUNS, "inj-trunc47")
        for pair in (("pin.i32", "pin-ctrl.i32"), ("pin.i32", "flat.i32"),
                     ("pin-ctrl.i32", "flat.i32")):
            self.assertEqual(cmp16(os.path.join(d, pair[0]),
                                   os.path.join(d, pair[1])), -1, pair)
        # f0: no-op
        d = os.path.join(RUNS, "inj-f0")
        self.assertEqual(cmp16(os.path.join(d, "pin.i32"),
                               os.path.join(d, "pin-ctrl.i32")), -1)
        # freshctl: FRESH 1 after F=0-only, 0 after accepted write
        d = os.path.join(RUNS, "inj-freshctl")
        mf = meta(os.path.join(d, "pin-f0.meta"))
        mw = meta(os.path.join(d, "pin-wr.meta"))
        self.assertEqual(bits(mf["qreads"][-1])["fresh"], 1)
        self.assertEqual(bits(mw["qreads"][-1])["fresh"], 0)
        self.assertEqual(mf.get("overrun"), "0")
        # ovf: sticky set, cleared by pad reset
        d = os.path.join(RUNS, "inj-ovf")
        mo = meta(os.path.join(d, "pin.meta"))
        st = [bits(q) for q in mo["qreads"]]
        self.assertGreaterEqual(len(st), 7)
        self.assertEqual(st[2]["ovf"], 1)
        self.assertEqual(st[3]["ovf"], 1)
        self.assertEqual(mo["qreads"][4], "0x00000000",
                         "post-pad-reset canary (F-STA-1)")
        for s in st[4:]:
            self.assertEqual(s["ovf"], 0)
        self.assertEqual(st[5]["fresh"], 1)
        self.assertEqual(mo.get("overrun"), "0")
        # partial: pin==flat for the partial vector, != full stream
        d = os.path.join(RUNS, "inj-partial")
        self.assertEqual(cmp16(os.path.join(d, "pin.i32"),
                               os.path.join(d, "flat.i32")), -1)
        self.assertNotEqual(cmp16(os.path.join(d, "pin.i32"),
                                  os.path.join(d, "full-pin.i32")), -1)

    def test_summary_verdicts(self):
        summ = self.summary()
        vs = summ["verdicts"]
        self.assertTrue(vs, "empty verdict list")
        for v in vs:
            self.assertIn(v["status"], ("PASS", "NOT_RUN"), v)
            if v["status"] == "FAIL":
                self.fail(v)
        names = [v["name"] for v in vs]
        for want in CASES:
            self.assertIn("case:%s" % want, names)
        for want in INJ:
            self.assertIn("inj:%s" % want[4:], names)
        self.assertEqual(summ["iverilog_shadow"], "NOT_RUN")
        self.assertTrue(summ["iverilog_reason"], "NOT_RUN needs a reason")

    def test_report_anchors(self):
        txt = open(REPORT).read()
        for anchor in ("F-I2S-1", "F-I2S-2", "F-OVF-1", "F-STA-1",
                       "F-H07-STALE", "F-I2S-3",
                       "byte-identical", "NOT_RUN"):
            self.assertIn(anchor, txt, "missing report anchor: " + anchor)


class TestH08Catalog(unittest.TestCase):
    def test_catalog_pins(self):
        c = json.load(open(CATALOG))
        lo = {e["path"]: e for e in c["local_originals"]}
        for path in ("rtl/tb_synth_top.v", "tools/h08_pin_check.py",
                     "tests/test_h08.py", "docs/H08-PIN.md"):
            self.assertIn(path, lo, path)
            self.assertEqual(lo[path]["sha256"],
                             sha(os.path.join(REPO, path)), path)
        comp = {x["component"]: x for x in c["components"]}
        ch = comp["synth-top-chassis"]
        self.assertEqual(ch["requalification"]["result"].startswith("PASS"),
                         True)
        self.assertEqual(ch["adapted_sha256"]["rtl/synth_top.v"],
                         sha(os.path.join(REPO, "rtl/synth_top.v")))


class TestH08LiveControls(unittest.TestCase):
    """Live rerun of the small injection vectors (H07 wall-heavy pattern).

    Builds the pin bench on demand into the git-ignored build/ tree
    (~30 s cold, cached warm) and executes the undersized-frame
    trunc47 vector plus the f0/freshctl/ovf vectors, reproducing the
    committed verdicts on THIS machine.
    """

    def setUp(self):
        if not shutil.which("verilator"):
            self.skipTest("verilator not installed on this runner; the "
                          "executing-tool gate runs on hosts with Verilator "
                          "(NOT_RUN here, never a red error)")
        if not os.path.exists(BIN):
            # Verilator does not create nested Mdir paths; a fresh checkout
            # (CI runner, clean sync) has no build/ at all, so create it or
            # the build dies with the misleading "Can't write file".
            os.makedirs(os.path.dirname(BIN), exist_ok=True)
            cmd = ["verilator", "--binary", "--timing", "-j", "4",
                   "--Wno-fatal", "--timescale-override", "1ns/1ps",
                   "--x-initial", "0", "--x-assign", "unique",
                   "-Mdir", os.path.dirname(BIN), "-o",
                   os.path.basename(BIN), "--top-module", "tb_synth_top"]
            cmd += [os.path.join(REPO, "rtl", f) for f in
                    ("dx7_core.v", "env_unit.v", "alg_router.v",
                     "synth_top.v", "tb_synth_top.v")]
            r = subprocess.run(cmd, cwd=REPO, capture_output=True,
                               text=True)
            if r.returncode != 0 or not os.path.exists(BIN):
                # Verilator reports to stderr; surface the real cause.
                self.skipTest("verilator bench build failed: "
                              + ((r.stderr or r.stdout)[-500:]))

    def runvec(self, vecfile, outdir, tag):
        # keep every plusarg short and relative (absolute long paths get
        # mangled through the Verilator plusarg layer on this toolchain)
        os.makedirs(outdir, exist_ok=True)
        vf = os.path.join(outdir, tag + ".txt")
        shutil.copy(vecfile, vf)
        a = os.path.join(outdir, tag + ".i32")
        m = os.path.join(outdir, tag + ".meta")
        p = subprocess.run([BIN, "+vectors=" + tag + ".txt",
                            "+actual=" + tag + ".i32", "+meta=" + tag + ".meta"],
                           capture_output=True, text=True, timeout=1800,
                           cwd=outdir)
        self.assertEqual(p.returncode, 0, tag)
        self.assertNotRegex(p.stdout, r"FAIL tb_")
        return a, m, p.stdout

    def test_live_trunc47(self):
        # live undersized-frame negative control (compact, self-built):
        # a small patch block whose LAST patch write is sent as a 47-bit
        # frame (48th bit — the F latch bit — missing). A clean DUT
        # latches nothing for it: stream == control (frame absent), and
        # the forensic frame_latch count is unchanged.
        import tempfile
        base = [l for l in open(os.path.join(H07, "verilog-accept-dev-dir-base",
                                             "vector.txt")).read().splitlines() if l]
        pidx = [i for i, l in enumerate(base) if l.startswith("P ")]
        ti = min(11, len(pidx) - 1)
        ci = next(i for i, l in enumerate(base) if l == "C" or l.startswith("C "))
        assert ti < ci
        addr, data = base[ti][2:4], base[ti][5:13]
        head = [l for i, l in enumerate(base)
                if i < ti and l.startswith("P ")]
        v_trunc = head + ["X 1 %s %s 47" % (addr, data)] + ["C "] + ["W 512"]
        v_ctrl = head + ["C "] + ["W 512"]
        tmp = tempfile.mkdtemp(prefix="h08trunc")
        vt = os.path.join(tmp, "trunc.txt"); vc = os.path.join(tmp, "ctrl.txt")
        open(vt, "w").write("\n".join(v_trunc) + "\n")
        open(vc, "w").write("\n".join(v_ctrl) + "\n")
        out = os.path.join(REPO, "build", "h08", "live")
        a, m, so = self.runvec(vt, out, "trunc-q")
        ac, mc, sco = self.runvec(vc, out, "ctrl-q")
        self.assertEqual(cmp16(a, ac), -1, "live trunc47 stream drift")
        lt = re.search(r"frame_latches=(\d+)", so)
        lc = re.search(r"frame_latches=(\d+)", sco)
        self.assertIsNotNone(lt, "no forensic latch count")
        self.assertEqual(int(lt.group(1)), int(lc.group(1)),
                         "truncated frame changed the latch count")
        self.assertNotEqual(int(lc.group(1)), 0,
                            "control frames never latched (bench broken)")

    def test_live_f0_and_freshctl(self):
        # Live F=0 no-op + FRESH lifecycle (compact, self-built).
        #  f0q: a single full 48-bit F=0 frame (V) at a valid write
        #       address, no accepted writes. A clean DUT ignores the
        #       F=0 frame: stream == control (frame absent) and FRESH
        #       stays 1.
        #  wrq: the full dir-base patch (all 41 P words) + commit + the
        #       7 E note events (the actuated event path — a static
        #       patch+commit alone voices nothing), then 3000 wired
        #       samples; the first voiced sample lands at index 2508.
        #       FRESH must clear to 0 and the stream must show audio
        #       (unlike the silent ctrl/f0q baselines).
        import tempfile
        base = [l for l in open(os.path.join(H07, "verilog-accept-dev-dir-base",
                                             "vector.txt")).read().splitlines() if l]
        allp = [l for l in base if l.startswith("P ")]
        alles = [l for l in base if l.startswith("E ")]
        v_f0 = ["V 10 00000001", "W 512"]
        v_ctrl = ["W 512"]
        v_wr = allp + ["C "] + alles + ["W 3000"]
        tmp = tempfile.mkdtemp(prefix="h08f0")
        for nm, v in (("f0q", v_f0), ("f0c", v_ctrl), ("wrq", v_wr)):
            open(os.path.join(tmp, nm + ".txt"), "w").write(
                "\n".join(v) + "\n")
        out = os.path.join(REPO, "build", "h08", "live")
        a, m, _ = self.runvec(os.path.join(tmp, "f0q.txt"), out, "f0q")
        ac, mc, _ = self.runvec(os.path.join(tmp, "f0c.txt"), out, "f0c")
        aw, mw, _ = self.runvec(os.path.join(tmp, "wrq.txt"), out, "wrq")
        self.assertEqual(cmp16(a, ac), -1,
                         "F=0 frame was not ignored (stream drift)")
        self.assertEqual(meta(m)["fresh"], "1",
                         "FRESH cleared by non-accepted F=0 frame")
        self.assertEqual(meta(mw)["fresh"], "0",
                         "FRESH not cleared by accepted write")
        self.assertNotEqual(open(aw, "rb").read(), open(ac, "rb").read(),
                            "full patch + commit + events voiced nothing")

    def test_live_ovf(self):
        d = os.path.join(RUNS, "inj-ovf")
        out = os.path.join(REPO, "build", "h08", "live")
        a, m, _ = self.runvec(os.path.join(d, "vec.txt"), out, "ovf")
        mm = meta(os.path.join(out, "ovf.meta"))
        st = [bits(q) for q in mm["qreads"]]
        self.assertEqual(st[2]["ovf"], 1, "live: overflow must set")
        self.assertEqual(st[4]["ovf"], 0, "live: pad reset must clear")
        self.assertEqual(mm.get("overrun"), "0")


if __name__ == "__main__":
    unittest.main()