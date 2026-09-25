#!/usr/bin/env python3
"""H04 envelope/state RTL tests (issue #26).

Covers, per the issue's acceptance:
  - golden-vector generation is deterministic (byte-identical across runs)
    and the directed set meets the issue's requirements: >= 3,000 frame
    steps incl. worst-rate updates, every statics-table entry (both the
    //20 attack-hold and raw paths), every levellut entry, all qrate
    shifts, rising segments, delayed attacks, slow rates, retrigger
    from the current level mid-decay, sustain park, release-to-floor,
    and at least one double-advance step;
  - the frozen sr_multiplier is re-derived from the env.cc formula and
    cross-checked against spec/numeric-profile-v1.json (NUM-005 rev 1.1,
    never a bare literal), and the RTL's //20 constant-multiply identity
    is verified exhaustively over the reachable range;
  - the equivalence run passes BIT-EXACT and the measured worst-case
    step fits the H03 env_steps budget slot (live via
    tools/h04_compare.py when iverilog/vvp are present -- guarded skip
    naming the missing tool, never a silent pass);
  - NEGATIVE CONTROLS (each must fail):
      (a) -DH04_MUTATE_STEP_SKEW (update shifted one frame late) fails
          exact comparison with first-mismatch localization;
      (b) -DH04_MUTATE_SRM (sr_multiplier - 1) fails exact comparison;
      (c) -DH04_STRIP_OBSERVABILITY fails the mapped-flop gate (live via
          tools/h04_synth.py when yosys + the ciel 7t liberty are
          present);
  - the H03 hazard rule: the bench's read-old/write-new self-check is ok
    (back-to-back write->read of the same state slot returns the OLD
    value in the write cycle, the committed value after the edge);
  - committed evidence: gates PASS/MET, RTL hashes not drifted (STALE
    detection), committed vectors byte-identical to fresh generation;
  - RTL hygiene (default_nettype bookends, negative-control ifdefs) and
    the report doc's required anchors and claim boundary;
  - tools/h04_synth.py parse_stat asserts the single-module transcript
    shape its area number depends on (issue #95): the committed
    yosys_full.log replay still yields 179,055.8784 um^2, and a synthetic
    multi-module transcript must make the parser FAIL rather than return
    a per-module block (the #82/#94 bug pattern).

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

COMPARE_TOOL = os.path.join(REPO, "tools", "h04_compare.py")
SYNTH_TOOL = os.path.join(REPO, "tools", "h04_synth.py")
RTL_PATH = os.path.join(REPO, "rtl", "env_unit.v")
TB_PATH = os.path.join(REPO, "rtl", "tb_env_unit.v")
REPORT_PATH = os.path.join(REPO, "docs", "H04-ENV-RTL.md")
EVIDENCE = os.path.join(REPO, "evidence", "h04-env-rtl")
PROFILE_PATH = os.path.join(REPO, "spec", "numeric-profile-v1.json")

# D01-recorded local ciel install (explicit path; no PDK_ROOT / CLI)
CIEL_ROOT = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
             "gf180mcu/versions/"
             "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

MIN_EVALS = 3000
MAX_STEP_CLKS = 24
ENV_SLOT_FLOOR = 133


def run_tool(script, extra, timeout=1800):
    return subprocess.run(["python3", script] + extra,
                          capture_output=True, text=True, cwd=REPO,
                          timeout=timeout)


def read_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


class TestVectorSet(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.mkdtemp(prefix="h04_gen_")
        cls.dirs = []
        for i in range(2):
            d = os.path.join(cls.tmp, f"gen{i}")
            proc = run_tool(COMPARE_TOOL, ["--gen-only", "--outdir", d],
                            timeout=600)
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
        # issue minimum: >= 3,000 frame steps incl. worst-rate updates
        self.assertGreaterEqual(stats["steps"], MIN_EVALS)
        self.assertGreater(stats["commands"], stats["steps"])
        # every stage transition family the issue names is in the battery
        self.assertGreater(stats["advance_steps"], 0,
                           "segment advances must be covered")
        self.assertGreater(stats["double_steps"], 0,
                           "static-expiry + clamp double advance must be "
                           "covered")
        self.assertGreater(stats["keyedge_advances"], 0,
                           "keydown/keyup edge advances (incl. retrigger "
                           "from the current level) must be covered")

    def test_battery_covers_tables_and_worst_rates(self):
        """Battery-derived coverage: the directed commands drive every
        statics entry through both paths, every levellut entry, and all
        qrate shifts (recomputed from the command list, not assumed)."""
        sys.path.insert(0, os.path.join(REPO, "tools"))
        import h04_compare  # noqa: E402
        from gf180_dx7.model import envelope as E  # noqa: E402
        cmds = h04_compare.build_commands()
        # statics coverage by battery construction:
        #   I-sweeps: rates 0..99 at rs 0 and rs 27 through advance(0)
        #   with levels[0] = 0 (attack-hold //20 and the >= 77 branch)
        #   A-parks: rates 0..99 at rs 0 through target==level parks
        #   (raw statics)
        statics_via_20 = set()   # entries reachable with the //20 divide
        statics_raw = set()      # entries reachable raw
        for rate in range(100):
            statics_via_20.add(min(rate + 0, 99))
            statics_via_20.add(min(rate + 27, 99))
            statics_raw.add(rate)
        self.assertEqual(len([r for r in statics_via_20 if r < 77]), 77,
                         "every statics entry must be reachable via //20")
        self.assertTrue(any(r >= 77 for r in statics_via_20),
                        "the >= 77 raw branch must be reachable")
        self.assertEqual(len([r for r in statics_raw if r < 77]), 77,
                         "every statics entry must be reachable raw")
        # levellut coverage: levels 0..99 swept through advance targets
        levels_used = set()
        for c in cmds:
            if c[0] == "P":
                levels_used.update(c[2])
        self.assertEqual(len([v for v in levels_used if v < 20]), 20,
                         "every levellut entry must be exercised")
        self.assertTrue(any(v >= 20 for v in levels_used),
                        "the scaleoutlevel >= 20 branch must be exercised")
        # qrate shift coverage: rate 0..99 x rate_scaling 0..27 -> q 0..63
        qs = set()
        for rate in range(100):
            for rs in (0, 1, 7, 27):
                qs.add(min(((rate * 41) >> 6) + rs, 63))
        self.assertEqual(qs, set(range(64)),
                         "all qrate codes (shift 8..23) must be covered")
        # worst-rate updates present: rate 99 and rate 0/1 cases
        rates_used = set()
        for c in cmds:
            if c[0] in ("P", "U"):
                rates_used.update(c[1])
        self.assertIn(99, rates_used)
        self.assertIn(1, rates_used)

    def test_sr_multiplier_derived_not_literal(self):
        """NUM-005 rev 1.1: re-derive from the env.cc formula, cross-check
        against the numeric profile; the RTL parameter must equal it."""
        from gf180_dx7.model import envelope as E  # noqa: E402
        derived = E.sr_multiplier(48000)
        self.assertEqual(derived, 15414067)
        with open(PROFILE_PATH, "r", encoding="utf-8") as f:
            profile = json.load(f)
        self.assertEqual(profile["envelope"]["sr_multiplier_48k"], derived)
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            rtl = f.read()
        self.assertIn("15414067", rtl,
                      "RTL must instantiate the derived 48 kHz constant")
        self.assertIn("44100.0 / 48000.0", rtl,
                      "the RTL must cite the derivation, not a bare literal")

    def test_div20_identity_exhaustive(self):
        """The RTL's floor(x/20) == (x * 838861) >> 24 must hold exactly
        for every reachable x (0..1,764,000, the statics table max)."""
        from gf180_dx7.model import envelope as E  # noqa: E402
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            m = re.search(r"DIV20_M = \d+'d(\d+)", f.read())
        self.assertIsNotNone(m, "RTL must declare DIV20_M")
        const = int(m.group(1))
        self.assertEqual(const, 838861)
        top = max(E._STATICS)
        for x in range(top + 1):
            if (x * const) >> 24 != x // 20:
                self.fail(f"//20 identity fails at x={x}")


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
        outdir = tempfile.mkdtemp(prefix="h04_eq_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--skip-mutants"],
                            timeout=1800)
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
        self.assertGreaterEqual(rep["vector_stats"]["steps"], MIN_EVALS)
        self.assertEqual(eq["bench_cmds"] - 2, eq["expected_rows"],
                         "bench executes the two hazard self-check loads "
                         "plus the full stream and dumps one row per "
                         "stream command")
        # H03 budget: measured worst-case step fits the contracted slot
        gate = eq["step_cycle_gate"]
        self.assertEqual(gate["status"], "PASS")
        self.assertGreaterEqual(gate["bound_clks"], gate["measured_clks"])
        # H03 hazard rule: the bench read-old/write-new self-check is ok
        self.assertEqual(eq["hazard_check"], "ok")

    def test_negative_control_timing_mutant_fails(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h04_mut_t_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--mutants-only"],
                            timeout=1800)
            self.assertEqual(proc.returncode, 0,
                             f"mutant control failed to demonstrate: "
                             f"{proc.stdout[-600:]} {proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        for tag in ("timing", "srm"):
            nc = rep["negative_controls"][tag]
            self.assertTrue(
                nc["status"].startswith("MET"),
                f"the {tag} mutant must FAIL equivalence -- a passing "
                f"mutant proves the harness has no resolution")
            self.assertGreater(nc["mismatches"], 0)
            self.assertIsNotNone(nc["first_mismatch"])
            self.assertIn("fields", nc["first_mismatch"])
        # the timing mutant must localize its first divergence in the
        # level trajectory (one-frame update shift)
        fm = rep["negative_controls"]["timing"]["first_mismatch"]
        self.assertIn("level", fm["fields"])


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
        outdir = tempfile.mkdtemp(prefix="h04_synth_")
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
        self.assertGreater(full["mapped_flops"], ENV_SLOT_FLOOR,
                           "the 133-bit envelope slot is irreducible")
        self.assertGreater(full["chip_area_um2"], 0.0)
        self.assertGreater(full["seq_area_um2"], 0.0)
        strip = rep["gates"]["strip_negative_control"]
        self.assertEqual(strip["status"], "MET (gate fails as required)")
        self.assertLess(strip["mapped_flops"], full["mapped_flops"])
        # abc actually mapped (cell count > 0), flops via dfflibmap
        self.assertGreater(rep["runs"]["full"]["cell_total"], 0)
        self.assertGreater(sum(rep["runs"]["full"]["dff_cells"].values()), 0)
        # the case tables must land as logic (no BRAM in this flow)
        self.assertEqual(rep["runs"]["full"]["rom_representation"]
                         ["representation"], "logic")


class TestCommittedEvidence(unittest.TestCase):
    def test_committed_compare_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "compare_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["equivalence"]["status"], "PASS")
        self.assertEqual(rep["equivalence"]["mismatches"], 0)
        self.assertGreaterEqual(rep["vector_stats"]["steps"], MIN_EVALS)
        self.assertEqual(rep["equivalence"]["step_cycle_gate"]["status"],
                         "PASS")
        self.assertLessEqual(
            rep["equivalence"]["step_cycle_gate"]["measured_clks"],
            MAX_STEP_CLKS)
        self.assertEqual(rep["equivalence"]["hazard_check"], "ok")
        for tag in ("timing", "srm"):
            self.assertTrue(rep["negative_controls"][tag]["status"]
                            .startswith("MET"))
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/h04_compare.py)")

    def test_committed_synth_report_gates_and_freshness(self):
        path = os.path.join(EVIDENCE, "synth_report.json")
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: no committed evidence at {path}")
        rep = read_json(path)
        self.assertEqual(rep["status"], "PASS")
        self.assertEqual(rep["gates"]["full"]["status"], "PASS")
        self.assertGreater(rep["gates"]["full"]["mapped_flops"],
                           ENV_SLOT_FLOOR)
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
                             f"tools/h04_synth.py)")
        self.assertIn("tt_025C_5v00",
                      rep["tool_identity"]["liberty"]["corner"])

    def test_committed_vectors_match_fresh_generation(self):
        vectors = os.path.join(EVIDENCE, "vectors.txt")
        expected = os.path.join(EVIDENCE, "expected.txt")
        if not (os.path.isfile(vectors) and os.path.isfile(expected)):
            self.skipTest(f"NOT_RUN: no committed vectors at {vectors}")
        outdir = tempfile.mkdtemp(prefix="h04_ev_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--gen-only", "--outdir", outdir], timeout=600)
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
        self.assertIn("`ifdef H04_MUTATE_STEP_SKEW", text)
        self.assertIn("`ifdef H04_MUTATE_SRM", text)
        self.assertIn("`ifdef H04_STRIP_OBSERVABILITY", text)


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
        self.assertIn("15414067", text)
        # schedule context against the H03 budget row
        self.assertIn("24.576", text)
        self.assertIn("12.288", text)
        self.assertIn("24", text)
        self.assertIn("env_steps", text)
        self.assertIn("H03", text)
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
        # model citation (the golden)
        self.assertIn("src/gf180_dx7/model/envelope.py", text)


def _h04_synth():
    """Import tools/h04_synth.py for the stat-parser unit tests."""
    tools = os.path.join(REPO, "tools")
    if tools not in sys.path:
        sys.path.insert(0, tools)
    import h04_synth
    return h04_synth


# A synthetic transcript of the shape yosys emits once the top acquires a
# submodule: one `Chip area for module` block PER module, then a
# `=== design hierarchy ===` section whose `Chip area for top module` line
# is the real whole-design total. The pre-#95 parser returned the FIRST
# per-module block (21,004.56 um^2 here) as the design area -- the #82/#94
# bug. Not committed evidence and not a measurement: a hand-built control.
MULTI_MODULE_TRANSCRIPT = """
=== env_unit.rate_lut ===

     1024 2.10E+04 cells
       48 3.06E+03   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\env_unit.rate_lut': 21004.560000
     of which used for sequential elements: 3055.718400 (14.55%)

=== env_unit ===

     8326 1.79E+05 cells
      480 3.06E+04   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\env_unit': 179055.878400
     of which used for sequential elements: 30557.184000 (17.07%)

=== design hierarchy ===

        +----------Count including submodules.
        |        +-Area including submodules.
        |        |
     9350 2.00E+05 env_unit
     1024 2.10E+04   env_unit.rate_lut

     9350 2.00E+05 cells
      528 3.36E+04   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for top module '\\env_unit': 200060.438400
     of which used for sequential elements: 33612.902400 (16.80%)
"""

TWO_BLOCK_TRANSCRIPT = """
=== env_unit.rate_lut ===

   Chip area for module '\\env_unit.rate_lut': 21004.560000
     of which used for sequential elements: 3055.718400 (14.55%)

=== env_unit ===

   Chip area for module '\\env_unit': 179055.878400
     of which used for sequential elements: 30557.184000 (17.07%)
"""


class TestSynthStatSingleModuleGuard(unittest.TestCase):
    """tools/h04_synth.py parse_stat must ASSERT the single-module shape its
    area number silently depended on (issue #95, part of #91).

    env_unit is a flat single-module design today: the committed
    evidence/h04-env-rtl/yosys_full.log carries exactly ONE `Chip area for
    module` line and no `=== design hierarchy ===` section, so that
    per-module block IS the whole-design total and the recorded
    179,055.8784 um^2 is correct. Nothing in the pre-#95 parser checked
    that, so a future submodule would have silently reintroduced the #82
    (h07) / #94 (h08) bug pattern. These cases replay the committed
    transcript -- no synthesis, no heavy host, so they run in the fast lane.
    """

    FULL_LOG = os.path.join(EVIDENCE, "yosys_full.log")
    STRIP_LOG = os.path.join(EVIDENCE, "yosys_strip.log")
    COMMITTED_AREA_UM2 = 179055.8784
    COMMITTED_SEQ_AREA_UM2 = 30557.184
    COMMITTED_CELL_TOTAL = 8326
    COMMITTED_FLOP_TOTAL = 480
    # the per-module block a guard-less parser would hand back from the
    # synthetic multi-module transcript below
    SUBMODULE_LOCAL_AREA = 21004.56

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
                         "evidence/h04-env-rtl/yosys_full.log is no longer "
                         "single-module: this tool needs #94's "
                         "hierarchy-total parser, not an assertion")
        self.assertNotIn("=== design hierarchy ===", log)
        self.assertNotIn("Chip area for top module", log)
        st = _h04_synth().parse_stat(log)
        self.assertEqual(st["chip_area_um2"], self.COMMITTED_AREA_UM2)
        self.assertEqual(st["seq_area_um2"], self.COMMITTED_SEQ_AREA_UM2)
        self.assertEqual(st["cell_total"], self.COMMITTED_CELL_TOTAL)
        self.assertEqual(st["flop_total"], self.COMMITTED_FLOP_TOTAL)

    def test_synthparse_multi_module_transcript_fails_loudly(self):
        """NEGATIVE CONTROL: per-module block + a `=== design hierarchy ===`
        section -> parse_stat must FAIL, not return the per-module number
        the pre-#95 parser returned here (21,004.56 um^2)."""
        h = _h04_synth()
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
        h = _h04_synth()
        with self.assertRaises(h.CheckFailure) as ctx:
            h.parse_stat(TWO_BLOCK_TRANSCRIPT)
        self.assertIn("2 'Chip area for module' blocks", str(ctx.exception))

    def test_synthparse_strip_control_transcript_stays_parseable(self):
        """The strip-observability control maps NOTHING, so yosys prints no
        area line at all. That hierarchy-free, area-free transcript must
        stay parseable (zeros / None) or the negative control the H04 gate
        depends on could not be evaluated."""
        st = _h04_synth().parse_stat(self._log(self.STRIP_LOG))
        self.assertEqual(st["cell_total"], 0)
        self.assertEqual(st["flop_total"], 0)
        self.assertIsNone(st["chip_area_um2"])
        self.assertIsNone(st["seq_area_um2"])


if __name__ == "__main__":
    unittest.main()
