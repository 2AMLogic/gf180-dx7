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
    the report doc's required anchors and claim boundary;
  - tools/h05_synth.py parse_stat asserts the single-module transcript
    shape its area number depends on (issue #95): the committed
    yosys_full.log replay still yields 1,148,899.6288 um^2, and a
    synthetic multi-module transcript must make the parser FAIL rather
    than return a per-module block (the #82/#94 bug pattern).

Stdlib only.
"""

import hashlib
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


def _h05_synth():
    """Import tools/h05_synth.py for the stat-parser unit tests."""
    tools = os.path.join(REPO, "tools")
    if tools not in sys.path:
        sys.path.insert(0, tools)
    import h05_synth
    return h05_synth


# A synthetic transcript of the shape yosys emits once the top acquires a
# submodule: one `Chip area for module` block PER module, then a
# `=== design hierarchy ===` section whose `Chip area for top module` line
# is the real whole-design total. The pre-#95 parser returned the FIRST
# per-module block (64,218.88 um^2 here) as the design area -- the #82/#94
# bug (in #82 it was this very module's block mistaken for the whole core).
# Not committed evidence and not a measurement: a hand-built control.
MULTI_MODULE_TRANSCRIPT = """
=== alg_router.conn_lut ===

     2048 6.42E+04 cells
      128 8.15E+03   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\alg_router.conn_lut': 64218.880000
     of which used for sequential elements: 8148.096000 (12.69%)

=== alg_router ===

    37930 1.15E+06 cells
     7407 4.72E+05   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\alg_router': 1148899.628800
     of which used for sequential elements: 471535.545600 (41.04%)

=== design hierarchy ===

        +----------Count including submodules.
        |        +-Area including submodules.
        |        |
    39978 1.21E+06 alg_router
     2048 6.42E+04   alg_router.conn_lut

    39978 1.21E+06 cells
     7535 4.80E+05   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for top module '\\alg_router': 1213118.508800
     of which used for sequential elements: 479683.641600 (39.54%)
"""

TWO_BLOCK_TRANSCRIPT = """
=== alg_router.conn_lut ===

   Chip area for module '\\alg_router.conn_lut': 64218.880000
     of which used for sequential elements: 8148.096000 (12.69%)

=== alg_router ===

   Chip area for module '\\alg_router': 1148899.628800
     of which used for sequential elements: 471535.545600 (41.04%)
"""


class TestSynthStatSingleModuleGuard(unittest.TestCase):
    """tools/h05_synth.py parse_stat must ASSERT the single-module shape its
    area number silently depended on (issue #95, part of #91).

    This tool's alg_router build is a flat single-module design today: the
    committed evidence/h05-routing-rtl/yosys_full.log carries exactly ONE
    `Chip area for module` line and no `=== design hierarchy ===` section,
    so that per-module block IS the whole-design total and the recorded
    1,148,899.6288 um^2 is correct. Nothing in the pre-#95 parser checked
    that, so a future submodule would have silently reintroduced the #82
    (h07) / #94 (h08) bug pattern -- the same pattern whose h07 instance was
    alg_router's own block being read as the whole core. These cases replay
    the committed transcript -- no synthesis, no heavy host, fast lane.
    """

    FULL_LOG = os.path.join(EVIDENCE, "yosys_full.log")
    STRIP_LOG = os.path.join(EVIDENCE, "yosys_strip.log")
    COMMITTED_AREA_UM2 = 1148899.6288
    COMMITTED_SEQ_AREA_UM2 = 471535.5456
    COMMITTED_CELL_TOTAL = 37930
    COMMITTED_FLOP_TOTAL = 7407
    # the per-module block a guard-less parser would hand back from the
    # synthetic multi-module transcript below
    SUBMODULE_LOCAL_AREA = 64218.88

    def _log(self, path):
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: {path} absent")
        with open(path, "r", encoding="utf-8") as f:
            return f.read()

    def test_synthparse_committed_log_single_module_keeps_area(self):
        """POSITIVE: the committed transcript is still single-module, and
        the guarded parser still returns today's recorded numbers."""
        log = self._log(self.FULL_LOG)
        self.assertEqual(log.count("Chip area for module"), 1,
                         "evidence/h05-routing-rtl/yosys_full.log is no "
                         "longer single-module: this tool needs #94's "
                         "hierarchy-total parser, not an assertion")
        self.assertNotIn("=== design hierarchy ===", log)
        self.assertNotIn("Chip area for top module", log)
        st = _h05_synth().parse_stat(log)
        self.assertEqual(st["chip_area_um2"], self.COMMITTED_AREA_UM2)
        self.assertEqual(st["seq_area_um2"], self.COMMITTED_SEQ_AREA_UM2)
        self.assertEqual(st["cell_total"], self.COMMITTED_CELL_TOTAL)
        self.assertEqual(st["flop_total"], self.COMMITTED_FLOP_TOTAL)

    def test_synthparse_multi_module_transcript_fails_loudly(self):
        """NEGATIVE CONTROL: per-module block + a `=== design hierarchy ===`
        section -> parse_stat must FAIL, not return the per-module number
        the pre-#95 parser returned here (64,218.88 um^2)."""
        h = _h05_synth()
        with self.assertRaises(h.CheckFailure) as ctx:
            h.parse_stat(MULTI_MODULE_TRANSCRIPT)
        self.assertIn("hierarch", str(ctx.exception).lower())
        # and the control has resolution: the unguarded regex really would
        # have returned a per-module block from this same transcript
        self.assertEqual(
            float(re.search(r"Chip area for module .*?: ([\d.]+)",
                            MULTI_MODULE_TRANSCRIPT).group(1)),
            self.SUBMODULE_LOCAL_AREA)

    def test_synthparse_two_module_blocks_no_hierarchy_fail(self):
        """NEGATIVE CONTROL: two per-module blocks and no hierarchy section
        (a truncated hierarchical stat) must FAIL too -- taking the first
        block would under-report the design."""
        h = _h05_synth()
        with self.assertRaises(h.CheckFailure) as ctx:
            h.parse_stat(TWO_BLOCK_TRANSCRIPT)
        self.assertIn("2 'Chip area for module' blocks", str(ctx.exception))

    def test_synthparse_strip_control_transcript_stays_parseable(self):
        """The strip-observability control maps NOTHING, so yosys prints no
        area line at all. That hierarchy-free, area-free transcript must
        stay parseable (zeros / None) or the negative control the H05 gate
        depends on could not be evaluated."""
        st = _h05_synth().parse_stat(self._log(self.STRIP_LOG))
        self.assertEqual(st["cell_total"], 0)
        self.assertEqual(st["flop_total"], 0)
        self.assertIsNone(st["chip_area_um2"])
        self.assertIsNone(st["seq_area_um2"])


if __name__ == "__main__":
    unittest.main()
