#!/usr/bin/env python3
"""H01 operator-probe tests (issue #20).

Covers, per the issue's acceptance:
  - golden-vector generation is deterministic (byte-identical across runs)
    and the directed set meets the issue's requirements: >= 2000 evals,
    negative outputs, level 0, max level, exact 24-bit cycle and 32-bit
    register wrap corners, and full 1024-address coverage of both ROMs;
  - the pinned tables the RTL's $readmemh loads are manifest-verified
    (the SAME bytes the Python model uses);
  - the equivalence run passes BIT-EXACT (live via tools/h01_compare.py
    when iverilog/vvp are present -- guarded skip naming the missing tool,
    never a silent pass);
  - the MUTATE_ADDRWIDTH negative control runs and FAILS equivalence
    (proving the harness has resolution);
  - the synthesis gates: mapped flops > 0 with area > 0 (an
    optimized-away build FAILS) and the strip-observability control
    collapses state, run live via tools/h01_synth.py when yosys + the
    ciel 7t liberty are present;
  - committed evidence: gates PASS/MET, artifact hashes not drifted
    (STALE detection), committed vectors byte-identical to fresh
    generation;
  - RTL hygiene (default_nettype bookends, negative-control ifdefs) and
    the report doc's required anchors and claim boundary.

Stdlib only.
"""

import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "src"))

COMPARE_TOOL = os.path.join(REPO, "tools", "h01_compare.py")
SYNTH_TOOL = os.path.join(REPO, "tools", "h01_synth.py")
RTL_PATH = os.path.join(REPO, "rtl", "operator_probe.v")
TB_PATH = os.path.join(REPO, "rtl", "tb_operator_probe.v")
REPORT_PATH = os.path.join(REPO, "docs", "H01-OPERATOR-PROBE.md")
EVIDENCE = os.path.join(REPO, "evidence", "h01-operator-probe")
TABLES_DIR = os.path.join(REPO, "reference", "tables")

# D01-recorded local ciel install (explicit path; no PDK_ROOT / CLI)
CIEL_ROOT = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
             "gf180mcu/versions/"
             "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

MIN_EVALS = 2000


def run_tool(script, extra, timeout=900):
    return subprocess.run(["python3", script] + extra,
                          capture_output=True, text=True, cwd=REPO,
                          timeout=timeout)


def read_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


class TestVectorSet(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.mkdtemp(prefix="h01_gen_")
        cls.dirs = []
        for i in range(2):
            d = os.path.join(cls.tmp, f"gen{i}")
            proc = run_tool(COMPARE_TOOL, ["--gen-only", "--outdir", d],
                            timeout=300)
            if proc.returncode != 0:
                raise AssertionError(
                    f"--gen-only failed: {proc.stdout[-400:]} "
                    f"{proc.stderr[-400:]}")
            cls.dirs.append(d)

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(cls.tmp, ignore_errors=True)

    def test_generation_is_deterministic(self):
        for name in ("vectors.txt", "expected.txt", "gen_report.json"):
            with open(os.path.join(self.dirs[0], name), "rb") as f:
                a = f.read()
            with open(os.path.join(self.dirs[1], name), "rb") as f:
                b = f.read()
            self.assertEqual(a, b,
                             f"{name} regeneration is not byte-identical")

    def test_vector_set_meets_issue_requirements(self):
        stats = read_json(
            os.path.join(self.dirs[0], "gen_report.json"))["stats"]
        self.assertGreaterEqual(stats["evals"], MIN_EVALS)
        # corner cases required by the issue brief
        self.assertGreater(stats["negative_outputs"], 0,
                           "vector set must include negative outputs")
        self.assertGreater(stats["env_zero"], 0, "level 0 must be covered")
        self.assertGreater(stats["env_env_max"], 0,
                           "max level (ENV_MAX 16384) must be covered")
        self.assertGreater(stats["cycle_boundary_24bit_crossings"], 0,
                           "24-bit cycle-wrap corner must be covered")
        self.assertGreater(stats["register_wrap_32bit_at_max"], 0,
                           "32-bit register wrap corner must be covered")
        # ROM byte coverage: every address of both tables exercised
        self.assertEqual(stats["distinct_sinlog_addrs"], 1024)
        self.assertEqual(stats["distinct_sinexp_addrs"], 1024)

    def test_rtl_rom_bytes_are_the_manifest_pinned_tables(self):
        from gf180_dx7.model.tables import load_pinned  # noqa: E402
        tabs = load_pinned(TABLES_DIR)  # raises TableDrift on drift
        self.assertEqual(tabs["sinlog_table"].bits, 16)
        self.assertEqual(tabs["sinexp_table"].bits, 16)
        self.assertEqual(len(tabs["sinlog_table"].values), 1024)
        self.assertEqual(len(tabs["sinexp_table"].values), 1024)


class TestEquivalence(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.iverilog = shutil.which("iverilog")
        cls.vvp = shutil.which("vvp")
        missing = []
        if cls.iverilog is None:
            missing.append("iverilog not on PATH")
        if cls.vvp is None:
            missing.append("vvp not on PATH")
        cls.reason = ("NOT_RUN (guarded skip): "
                      + "; ".join(missing) if missing else "")
        cls.can_run = not missing

    def test_equivalence_bit_exact_live(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h01_eq_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--skip-mutate"],
                            timeout=900)
            self.assertEqual(proc.returncode, 0,
                             f"harness failed: {proc.stdout[-600:]} "
                             f"{proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        eq = rep["equivalence"]
        self.assertEqual(eq["status"], "PASS")
        self.assertEqual(eq["mismatches"], 0,
                         "any mismatch means the RTL is wrong -- never a "
                         "loosened check")
        self.assertEqual(eq["expected_rows"], eq["actual_rows"])
        self.assertGreaterEqual(eq["expected_rows"], MIN_EVALS)
        self.assertEqual(eq["bench_captured"], eq["expected_rows"])

    def test_negative_control_mutant_fails_equivalence(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h01_mut_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--mutate-only"],
                            timeout=900)
            self.assertEqual(proc.returncode, 0,
                             f"mutant control failed to demonstrate: "
                             f"{proc.stdout[-600:]} {proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        nc = rep["negative_control"]
        self.assertTrue(nc["status"].startswith("MET"),
                        "the address-width mutant must FAIL equivalence -- "
                        "a passing mutant proves the harness has no "
                        "resolution")
        self.assertGreater(nc["mismatches"], 0)
        self.assertIsNotNone(nc["first_mismatch"])
        self.assertIn("expected_out", nc["first_mismatch"])


class TestSynthesis(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.yosys = shutil.which("yosys")
        cls.lib = os.path.join(CIEL_ROOT, SC_LIB_REL)
        missing = []
        if cls.yosys is None:
            missing.append("yosys binary not on PATH")
        if not os.path.isfile(cls.lib):
            missing.append(f"ciel 7t liberty not found at {cls.lib}")
        cls.reason = ("NOT_RUN (guarded skip): "
                      + "; ".join(missing) if missing else "")
        cls.can_run = not missing

    def test_synth_gates_live(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h01_synth_")
        try:
            proc = run_tool(SYNTH_TOOL, ["--outdir", outdir], timeout=1800)
            self.assertEqual(proc.returncode, 0,
                             f"synth tool failed: {proc.stdout[-600:]} "
                             f"{proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "synth_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        full = rep["gates"]["full"]
        self.assertEqual(full["status"], "PASS")
        # an optimized-away build (0 flops / 0 area) must FAIL this gate
        self.assertGreater(full["mapped_flops"], 0)
        self.assertGreater(full["mapped_flops"], 32,
                           "the 32-bit phase accumulator is irreducible")
        self.assertGreater(full["chip_area_um2"], 0.0)
        strip = rep["gates"]["strip_negative_control"]
        self.assertEqual(strip["status"], "MET (gate fails as required)")
        self.assertLess(strip["mapped_flops"], full["mapped_flops"])
        # ROM representation recorded (issue: logic or ROM -- say which)
        self.assertEqual(rep["runs"]["full"]["rom_representation"]
                         ["representation"], "logic")
        # abc actually mapped (cell count > 0), flops via dfflibmap
        self.assertGreater(rep["runs"]["full"]["cell_total"], 0)
        self.assertGreater(sum(rep["runs"]["full"]["dff_cells"].values()), 0)


class TestCommittedEvidence(unittest.TestCase):
    def test_committed_compare_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "compare_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["equivalence"]["status"], "PASS")
        self.assertEqual(rep["equivalence"]["mismatches"], 0)
        self.assertGreaterEqual(rep["equivalence"]["expected_rows"],
                                MIN_EVALS)
        self.assertTrue(rep["negative_control"]["status"]
                        .startswith("MET"))
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/h01_compare.py)")

    def test_committed_synth_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "synth_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["gates"]["full"]["status"], "PASS")
        self.assertGreater(rep["gates"]["full"]["mapped_flops"], 0)
        self.assertGreater(rep["gates"]["full"]["chip_area_um2"], 0.0)
        self.assertEqual(rep["gates"]["strip_negative_control"]["status"],
                         "MET (gate fails as required)")
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/h01_synth.py)")
        self.assertIn("tt_025C_5v00",
                      rep["tool_identity"]["liberty"]["corner"])

    def test_committed_vectors_match_fresh_generation(self):
        vectors = os.path.join(EVIDENCE, "vectors.txt")
        expected = os.path.join(EVIDENCE, "expected.txt")
        if not (os.path.isfile(vectors) and os.path.isfile(expected)):
            self.skipTest(f"NOT_RUN: no committed vectors at {vectors}")
        outdir = tempfile.mkdtemp(prefix="h01_ev_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--gen-only", "--outdir", outdir], timeout=300)
            self.assertEqual(proc.returncode, 0, proc.stderr[-400:])
            for name in ("vectors.txt", "expected.txt"):
                with open(os.path.join(outdir, name), "rb") as f:
                    fresh = f.read()
                with open(os.path.join(EVIDENCE, name), "rb") as f:
                    committed = f.read()
                self.assertEqual(
                    fresh, committed,
                    f"committed {name} drifted from deterministic "
                    f"regeneration (STALE)")
        finally:
            shutil.rmtree(outdir, ignore_errors=True)


class TestRtlHygiene(unittest.TestCase):
    def test_default_nettype_bookends(self):
        for path in (RTL_PATH, TB_PATH):
            with open(path, "r", encoding="utf-8") as f:
                text = f.read()
            self.assertIn("`default_nettype none", text, path)
            self.assertTrue(text.rstrip().endswith("`default_nettype wire"),
                            path)

    def test_negative_control_ifdefs_present(self):
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        self.assertIn("`ifdef MUTATE_ADDRWIDTH", text)
        self.assertIn("`ifdef H01_STRIP_OBSERVABILITY", text)


class TestReportDoc(unittest.TestCase):
    def test_report_cites_measurements_and_claim_boundary(self):
        if not os.path.isfile(REPORT_PATH):
            self.skipTest(f"NOT_RUN: {REPORT_PATH} not present")
        with open(REPORT_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        # measured identities the issue demands
        self.assertIn("yosys", text)
        self.assertIn("tt_025C_5v00", text)
        self.assertIn("iverilog", text)
        # cycles/operator against both frame budgets (issue acceptance)
        self.assertIn("24.576", text)
        self.assertIn("12.288", text)
        self.assertIn("2.67", text)
        self.assertIn("5.33", text)
        # schedule section citation
        self.assertIn("schedule", text)
        self.assertIn("N02", text)
        # mapped area reported
        self.assertRegex(text, r"[0-9]+\.[0-9]+\s*mm")
        # unmeasured items labelled, claim boundary explicit
        self.assertRegex(text, r"(?i)does NOT say")
        flat = " ".join(text.split()).lower()
        for boundary in ("no place-and-route", "no timing closure",
                         "no fit claim"):
            self.assertIn(boundary, flat,
                          f"report must explicitly disclaim: {boundary!r}")
        self.assertIn("unmeasured", flat)
        # tables manifest citation (the SAME bytes rule)
        self.assertIn("reference/tables/manifest.json", text)


if __name__ == "__main__":
    unittest.main()
