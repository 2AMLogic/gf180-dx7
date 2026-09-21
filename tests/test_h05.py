#!/usr/bin/env python3
"""H05 routing/feedback RTL tests (issue #27).

Covers, per the issue's acceptance:
  - the shared table artifact: deterministic regeneration from the model's
    ALGORITHMS table, committed bytes == regeneration, manifest sha-pin,
    independent decode agreement, exactly one feedback op per row;
  - the conformance run passes BIT-EXACT for all 32 algorithms x
    feedback {0,3,7} x 3 frames plus directed fb-seed / fb-off / gating
    runs (>= 6000 evals), live via tools/h05_compare.py when
    iverilog/vvp are present -- guarded skip naming the missing tool,
    never a silent pass;
  - NEGATIVE CONTROLS (issue #27), each live and each must FAIL the
    check it targets:
      * operator-order permutation via -DH05_MUTATE_OP_ORDER (table
        mutation: wrong operators gated/loaded) fails >= 28 of 32
        algorithms;
      * inverted feedback sign via -DH05_INVERT_FB_SIGN fails every
        feedback>0 run with a live feedback path;
      * one-block-delayed fb history via -DH05_DELAY_FB_COMMIT fails
        every feedback>0 run with a live feedback path;
      * a mutated table hex fails the sha-pin check (no sim needed);
  - the synthesis gates: mapped flops > 4096 (irreducible bus state)
    with area > 0, ROM mapping recorded, and the strip-observability
    control collapses state, live via tools/h05_synth.py when yosys +
    the ciel 7t liberty are present;
  - committed evidence: gates PASS/MET, artifact hashes not drifted
    (STALE detection), program.txt byte-identical to fresh generation;
  - RTL hygiene (default_nettype bookends, negative-control ifdefs) and
    the report doc's required anchors and claim boundary.

Stdlib only.
"""

import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "src"))
sys.path.insert(0, os.path.join(REPO, "tools"))

COMPARE_TOOL = os.path.join(REPO, "tools", "h05_compare.py")
SYNTH_TOOL = os.path.join(REPO, "tools", "h05_synth.py")
RTL_PATH = os.path.join(REPO, "rtl", "alg_router.v")
TB_PATH = os.path.join(REPO, "rtl", "tb_alg_router.v")
REPORT_PATH = os.path.join(REPO, "docs", "H05-ROUTING-RTL.md")
EVIDENCE = os.path.join(REPO, "evidence", "h05-routing-rtl")
HEX_PATH = os.path.join(REPO, "reference", "tables", "alg_table.hex")
MANIFEST_PATH = os.path.join(REPO, "reference", "tables",
                             "alg_manifest.json")

# D01-recorded local ciel install (explicit path; no PDK_ROOT / CLI)
CIEL_ROOT = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
             "gf180mcu/versions/"
             "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

MIN_EVALS = 6000
BUS_FLOP_FLOOR = 4096


def run_tool(script, extra, timeout=1200):
    return subprocess.run(["python3", script] + extra,
                          capture_output=True, text=True, cwd=REPO,
                          timeout=timeout)


def read_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


class TestTableArtifact(unittest.TestCase):
    """The generated, sha-pinned model/RTL shared table (parasynth
    gen_tables discipline)."""

    def test_regeneration_is_deterministic_and_committed_bytes_match(self):
        import h05_compare
        fresh = h05_compare.build_alg_hex()
        with open(HEX_PATH, "r", encoding="ascii") as f:
            committed = f.read()
        self.assertEqual(fresh, committed,
                         "committed alg_table.hex drifted from "
                         "deterministic regeneration (STALE)")
        m = h05_compare.build_alg_manifest(fresh)
        self.assertEqual(m["sha256"],
                         hashlib.sha256(fresh.encode("ascii")).hexdigest())
        with open(MANIFEST_PATH, "r", encoding="utf-8") as f:
            committed_manifest = json.load(f)
        self.assertEqual(m, committed_manifest,
                         "alg_manifest.json drifted from regeneration")

    def test_check_tables_passes_on_committed_artifact(self):
        import h05_compare
        manifest = h05_compare.check_tables()  # raises on drift
        self.assertEqual(manifest["entries"], 192)
        self.assertEqual(manifest["bits"], 8)

    def test_table_decodes_to_the_model_algorithms(self):
        from gf180_dx7.model.algorithm import ALGORITHMS, decode_op
        tokens = []
        with open(HEX_PATH, "r", encoding="ascii") as f:
            for line in f:
                line = line.split("//")[0].strip()
                if line:
                    tokens.extend(line.split())
        self.assertEqual(len(tokens), 192)
        rom = [int(t, 16) for t in tokens]
        for alg in range(32):
            for k in range(6):
                self.assertEqual(rom[alg * 6 + k], ALGORITHMS[alg][k],
                                 f"ROM row {alg + 1} op {k + 1} differs "
                                 f"from the model table")
        # DX7 structure: exactly one feedback op per row
        for alg in range(32):
            fbs = [k for k in range(6)
                   if decode_op(ALGORITHMS[alg][k])[3]]
            self.assertEqual(len(fbs), 1, f"algorithm {alg + 1}")


class TestTableNegativeControl(unittest.TestCase):
    """A mutated table hex must FAIL the sha-pin check (no sim needed)."""

    def test_mutated_hex_fails_sha_pin_check(self):
        import h05_compare
        with open(HEX_PATH, "r", encoding="ascii") as f:
            text = f.read()
        # flip one table byte: c1 -> c2 in the first row
        mutated = text.replace("c1 11 11 14 01 14", "c2 11 11 14 01 14", 1)
        self.assertNotEqual(mutated, text)
        tmp = tempfile.mkdtemp(prefix="h05_hexmut_")
        try:
            hex_path = os.path.join(tmp, "alg_table.hex")
            man_path = os.path.join(tmp, "alg_manifest.json")
            with open(hex_path, "w", encoding="ascii") as f:
                f.write(mutated)
            shutil.copy(MANIFEST_PATH, man_path)
            with self.assertRaises(h05_compare.CheckFailure) as ctx:
                h05_compare.check_tables(hex_path=hex_path,
                                         manifest_path=man_path)
            self.assertIn("sha-pin", str(ctx.exception))
        finally:
            shutil.rmtree(tmp, ignore_errors=True)

    def test_mutated_manifest_fails_sha_pin_check(self):
        import h05_compare
        with open(HEX_PATH, "r", encoding="ascii") as f:
            text = f.read()
        tmp = tempfile.mkdtemp(prefix="h05_manmut_")
        try:
            hex_path = os.path.join(tmp, "alg_table.hex")
            man_path = os.path.join(tmp, "alg_manifest.json")
            with open(hex_path, "w", encoding="ascii") as f:
                f.write(text)
            manifest = read_json(MANIFEST_PATH)
            manifest["sha256"] = "0" * 64
            with open(man_path, "w", encoding="utf-8") as f:
                json.dump(manifest, f)
            with self.assertRaises(h05_compare.CheckFailure):
                h05_compare.check_tables(hex_path=hex_path,
                                         manifest_path=man_path)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)


class TestConformance(unittest.TestCase):
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

    def test_conformance_bit_exact_live(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h05_conf_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--skip-mutants"],
                            timeout=1200)
            self.assertEqual(proc.returncode, 0,
                             f"harness failed: {proc.stdout[-600:]} "
                             f"{proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        cf = rep["conformance"]
        self.assertEqual(cf["status"], "PASS")
        self.assertEqual(cf["mismatch_records"], 0,
                         "any mismatch means the RTL is wrong -- never a "
                         "loosened check")
        self.assertEqual(cf["failed_runs"], 0)
        self.assertGreaterEqual(rep["vector_stats"]["evals"], MIN_EVALS)
        # the 32 x {0,3,7} matrix: every row present and passing
        per_run = rep["conformance"]["per_run"]
        meta_alg_fb = []
        # rebuild the run order from the program (deterministic)
        import h05_compare
        _program, _stream, meta, _stats = h05_compare.golden()
        self.assertEqual(len(meta), len(per_run))
        for m, r in zip(meta, per_run):
            if m["kind"] == "matrix":
                meta_alg_fb.append((m["algorithm"], m["feedback"]))
                self.assertEqual(r["status"], "PASS",
                                 f"matrix run alg {m['algorithm']} "
                                 f"fb {m['feedback']} failed")
        self.assertEqual(sorted(set(meta_alg_fb)),
                         sorted((a + 1, fb) for a in range(32)
                                for fb in (0, 3, 7)),
                         "the 32 x 3 matrix is not fully covered")
        self.assertEqual(rep["vector_stats"]["algorithms"], 32)
        self.assertEqual(rep["vector_stats"]["feedback_settings"],
                         [0, 3, 7])

    def test_negative_control_oporder_fails_widely(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h05_ord_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--mutate-only=oporder"],
                            timeout=1200)
            self.assertEqual(proc.returncode, 0,
                             f"control failed to demonstrate: "
                             f"{proc.stdout[-600:]} {proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        nc = rep["mutants"]["oporder"]
        self.assertTrue(nc["status"].startswith("MET"),
                        "the operator-order mutant must FAIL "
                        "conformance -- a passing mutant proves the "
                        "harness has no resolution")
        self.assertGreaterEqual(nc["failed_algorithm_count"], 28,
                                "issue #27: the table mutation must fail "
                                "on at least 28 of 32 algorithms")
        self.assertIsNotNone(nc["first_mismatch"])

    def test_negative_control_fbsign_fails(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h05_sig_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--mutate-only=fbsign"],
                            timeout=1200)
            self.assertEqual(proc.returncode, 0, proc.stdout[-600:])
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        nc = rep["mutants"]["fbsign"]
        self.assertTrue(nc["status"].startswith("MET"))
        self.assertGreater(nc["mismatch_records"], 0)
        self.assertEqual(nc["surviving_feedback_positive_runs"], [],
                         "every feedback>0 run with a live feedback path "
                         "must fail under the sign inversion")

    def test_negative_control_fbdelay_fails(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h05_dly_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--mutate-only=fbdelay"],
                            timeout=1200)
            self.assertEqual(proc.returncode, 0, proc.stdout[-600:])
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        nc = rep["mutants"]["fbdelay"]
        self.assertTrue(nc["status"].startswith("MET"))
        self.assertGreater(nc["mismatch_records"], 0)
        self.assertEqual(nc["surviving_feedback_positive_runs"], [])


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
        outdir = tempfile.mkdtemp(prefix="h05_synth_")
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
        # an optimized-away or bus-losing build must FAIL this gate
        self.assertGreater(full["mapped_flops"], BUS_FLOP_FLOOR)
        self.assertGreater(full["chip_area_um2"], 0.0)
        self.assertGreater(full["seq_area_um2"], 0.0)
        strip = rep["gates"]["strip_negative_control"]
        self.assertEqual(strip["status"], "MET (gate fails as required)")
        self.assertLess(strip["mapped_flops"], full["mapped_flops"])
        # ROM representation recorded (issue: logic or ROM -- say which)
        self.assertEqual(rep["runs"]["full"]["rom_representation"]
                         ["representation"], "logic")
        self.assertGreater(rep["runs"]["full"]["cell_total"], 0)
        self.assertGreater(sum(rep["runs"]["full"]["dff_cells"].values()),
                           0)


class TestCommittedEvidence(unittest.TestCase):
    def test_committed_compare_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "compare_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["conformance"]["status"], "PASS")
        self.assertEqual(rep["conformance"]["mismatches"]
                         if "mismatches" in rep["conformance"]
                         else rep["conformance"]["mismatch_records"], 0)
        self.assertGreaterEqual(rep["vector_stats"]["evals"], MIN_EVALS)
        for name in ("oporder", "fbsign", "fbdelay"):
            self.assertTrue(rep["mutants"][name]["status"]
                            .startswith("MET"),
                            f"committed control {name} not MET")
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/h05_compare.py)")

    def test_committed_synth_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "synth_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["gates"]["full"]["status"], "PASS")
        self.assertGreater(rep["gates"]["full"]["mapped_flops"],
                           BUS_FLOP_FLOOR)
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
                             f"tools/h05_synth.py)")
        self.assertIn("tt_025C_5v00",
                      rep["tool_identity"]["liberty"]["corner"])

    def test_committed_program_matches_fresh_generation(self):
        program = os.path.join(EVIDENCE, "program.txt")
        if not os.path.isfile(program):
            self.skipTest(f"NOT_RUN: no committed program at {program}")
        outdir = tempfile.mkdtemp(prefix="h05_ev_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--gen-only", "--outdir", outdir], timeout=600)
            self.assertEqual(proc.returncode, 0, proc.stderr[-400:])
            with open(os.path.join(outdir, "program.txt"), "rb") as f:
                fresh = f.read()
            with open(program, "rb") as f:
                committed = f.read()
            self.assertEqual(
                fresh, committed,
                "committed program.txt drifted from deterministic "
                "regeneration (STALE)")
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
        self.assertIn("`ifdef H05_MUTATE_OP_ORDER", text)
        self.assertIn("`ifdef H05_INVERT_FB_SIGN", text)
        self.assertIn("`ifdef H05_DELAY_FB_COMMIT", text)
        self.assertIn("`ifdef H05_STRIP_OBSERVABILITY", text)

    def test_rom_loads_the_generated_shared_artifact(self):
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        self.assertIn("reference/tables/alg_table.hex", text)
        self.assertIn("$readmemh", text)


class TestReportDoc(unittest.TestCase):
    def test_report_cites_measurements_and_claim_boundary(self):
        if not os.path.isfile(REPORT_PATH):
            self.skipTest(f"NOT_RUN: {REPORT_PATH} not present")
        with open(REPORT_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        # measured identities + the shared-artifact identity
        self.assertIn("yosys", text)
        self.assertIn("tt_025C_5v00", text)
        self.assertIn("iverilog", text)
        self.assertIn("reference/tables/alg_table.hex", text)
        self.assertIn("alg_manifest.json", text)
        self.assertIn("gen_tables", text)
        self.assertIn("h05_compare.py --gen-tables", text)
        # schedule-delay handling + H03 contract citation
        self.assertIn("delayed", text.lower())
        self.assertIn("CONTRACT-CORE-v1", text)
        self.assertIn("24.576", text)
        # conformance matrix + mapped area reported
        self.assertRegex(text, r"32\s*[x\u00d7]\s*3")
        self.assertRegex(text, r"[0-9]+\.[0-9]+\s*mm")
        # controls + claim boundary
        self.assertIn("H05_MUTATE_OP_ORDER", text)
        self.assertIn("H05_INVERT_FB_SIGN", text)
        self.assertIn("H05_DELAY_FB_COMMIT", text)
        self.assertIn("H05_STRIP_OBSERVABILITY", text)
        self.assertRegex(text, r"(?i)does NOT say")
        flat = " ".join(text.split()).lower()
        for boundary in ("no place-and-route", "no timing closure",
                         "no fit claim"):
            self.assertIn(boundary, flat,
                          f"report must explicitly disclaim: {boundary!r}")
        self.assertIn("unmeasured", flat)


if __name__ == "__main__":
    unittest.main()
