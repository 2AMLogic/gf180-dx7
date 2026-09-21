#!/usr/bin/env python3
"""H06 pitch/modulation RTL tests (issue #28).

Covers, per the issue's acceptance:
  - golden-vector generation is deterministic (byte-identical across runs)
    and meets the issue requirements: >= 4000 frame evals (and >= 4000
    note-active frames), ratio + fixed coverage, coarse/detune/fine
    extremes, pitch-EG up/down, all six LFO waveforms, PM depth cases,
    keysync restarts, bend events, documented-mode transpose shifts, and
    frames with zero host traffic (the split is exercised, not assumed);
  - the pinned ROM bytes are verified: freqlut via the tables manifest
    (the SAME bytes the RTL $readmemh-loads) and the regenerated LFO sine
    table byte-for-byte against build_sintab();
  - the conformance run passes BIT-EXACT (live via tools/h06_compare.py
    when iverilog/vvp are present -- guarded skip naming the missing tool,
    never a silent pass);
  - NEGATIVE CONTROLS, each of which must FAIL the exact compare:
    * 1-LSB phase-increment mutation (issue-named boundary-value control),
    * fixed-mode-tracks-keyboard mutant (N05 named control),
    * LFO-sync-ignored mutant (N05 named control),
    * per-sample host streaming split violation (stalled increments);
  - the static per-sample-host-dependency guard: the RTL input interface
    equals the contracted per-event register port set exactly; a synthetic
    interface that adds a per-sample host stream port FAILS the guard;
  - the link budget: worst-burst host traffic (16 note-ons x 7 writes +
    16 note-offs + 16 controller events = 144) fits the 24.576 MHz design
    point bound (168 writes/frame); patch-commit load fits the declared
    load cost; the committed vector stream respects the per-frame bound;
  - the synthesis gates (flops > 192 with area > 0; strip-observability
    control collapses state), live via tools/h06_synth.py when yosys +
    the ciel 7t liberty are present;
  - committed evidence: gates PASS/MET, artifact hashes not drifted
    (STALE detection), committed vectors byte-identical to fresh
    generation;
  - the report doc's required anchors (split decisions, transpose ruling
    cross-ref, tables manifest citation, claim boundary).

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

COMPARE_TOOL = os.path.join(REPO, "tools", "h06_compare.py")
SYNTH_TOOL = os.path.join(REPO, "tools", "h06_synth.py")
RTL_PATH = os.path.join(REPO, "rtl", "pitch_mod.v")
TB_PATH = os.path.join(REPO, "rtl", "tb_pitch_mod.v")
REPORT_PATH = os.path.join(REPO, "docs", "H06-PITCH-RTL.md")
EVIDENCE = os.path.join(REPO, "evidence", "h06-pitch-rtl")
TABLES_DIR = os.path.join(REPO, "reference", "tables")

# D01-recorded local ciel install (explicit path; no PDK_ROOT / CLI)
CIEL_ROOT = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
             "gf180mcu/versions/"
             "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

MIN_FRAMES = 4000
FLOP_FLOOR = 192

# The contracted per-event input set of the pitch/mod unit (H03 sections
# 4.2/4.7/5): clock/reset, the per-event cfg write port (one 32-bit write
# per link write) and the payload-less control-tail frame tick. NOTHING
# else -- in particular no per-sample host data path (DEC-010).
CONTRACTED_INPUTS = {"clk", "rst", "cfg_wr", "cfg_addr", "cfg_wdata",
                     "frame_tick"}


def run_tool(script, extra, timeout=900):
    return subprocess.run(["python3", script] + extra,
                          capture_output=True, text=True, cwd=REPO,
                          timeout=timeout)


def read_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def parse_input_ports(text, defines=()):
    """Extract the module's input port names, honoring `ifdef/`else/
    `endif blocks against the given define set (a preprocessor-aware
    parse: the same file yields different interfaces per build)."""
    ports = []
    stack = []           # each entry: True if currently included
    for raw in text.splitlines():
        line = raw.strip()
        m = re.match(r"`ifdef\s+(\w+)", line)
        if m:
            stack.append(m.group(1) in defines)
            continue
        if re.match(r"`ifndef\s+(\w+)", line):
            stack.append(False)
            continue
        if re.match(r"`else\b", line):
            if stack:
                stack[-1] = not stack[-1]
            continue
        if re.match(r"`endif\b", line):
            if stack:
                stack.pop()
            continue
        if stack and not all(stack):
            continue
        m = re.match(r",?\s*input\s+wire\s+(?:\[[^\]]+\]\s*)?(\w+)", line)
        if m:
            ports.append(m.group(1))
            continue
        m = re.match(r",?\s*input\s+(\w+)", line)
        if m:
            ports.append(m.group(1))
    return ports


def interface_guard(ports):
    """The per-sample-host-dependency guard. Returns a failure message if
    the port set is NOT exactly the contracted per-event interface."""
    extra = sorted(set(ports) - CONTRACTED_INPUTS)
    missing = sorted(CONTRACTED_INPUTS - set(ports))
    if extra:
        return ("interface adds non-contracted input port(s) %s -- the "
                "H03 split forbids host data paths beyond the per-event "
                "cfg write port (DEC-010, contract 4.7)" % extra)
    if missing:
        return ("interface is missing contracted input port(s) %s"
                % missing)
    return None


class TestVectorSet(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.mkdtemp(prefix="h06_gen_")
        cls.dirs = []
        for i in range(2):
            d = os.path.join(cls.tmp, "gen%d" % i)
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
        for name in ("vectors.txt", "expected.txt"):
            with open(os.path.join(self.dirs[0], name), "rb") as f:
                a = f.read()
            with open(os.path.join(self.dirs[1], name), "rb") as f:
                b = f.read()
            self.assertEqual(a, b,
                             f"{name} regeneration is not byte-identical")

    def test_vector_set_meets_issue_requirements(self):
        gen_report = os.path.join(self.dirs[0], "gen_report.json")
        self.assertTrue(os.path.isfile(gen_report))
        stats = read_json(gen_report)["vector_stats"]
        self.assertGreaterEqual(stats["frames"], MIN_FRAMES)
        self.assertGreaterEqual(stats["active_frames"], MIN_FRAMES,
                                "note-active frame evals must also cover "
                                "the issue minimum")
        # N05 directed-case coverage
        for wave in range(6):
            self.assertGreater(stats["wave_frames"][wave], 0,
                               f"LFO waveform {wave} must be covered")
        self.assertGreater(stats["fixed_op_frames"], 0,
                           "fixed-mode ops must be covered")
        self.assertGreater(stats["coarse0_frames"], 0)
        self.assertGreater(stats["coarse31_frames"], 0)
        self.assertGreater(stats["detune0_frames"], 0)
        self.assertGreater(stats["detune14_frames"], 0)
        self.assertGreater(stats["keysync_noteons"], 0)
        self.assertGreater(stats["shifted_noteons"], 0,
                           "documented-mode transpose shifts must be "
                           "covered")
        self.assertGreater(stats["bend_events"], 0)
        self.assertGreater(stats["no_traffic_frames"], 0,
                           "frames with zero host traffic must be covered "
                           "(the core advances autonomously, DEC-010)")
        self.assertGreater(stats["negative_lookup_inputs"], 0,
                           "negative logfreq lookups must be covered")
        self.assertGreater(stats["burst_frames"], 0,
                           "burst event timing must be covered")
        self.assertGreater(stats["distinct_freqlut_ix"], 512,
                           "the freqlut index domain must be broadly "
                           "exercised")

    def test_rtl_rom_bytes_are_the_manifest_pinned_tables(self):
        from gf180_dx7.model.tables import load_pinned  # noqa: E402
        tabs = load_pinned(TABLES_DIR)  # raises TableDrift on drift
        self.assertEqual(tabs["freqlut_table"].entries, 1025)
        # the LFO sine table hex must regenerate byte-identically
        from gf180_dx7.model.pitch import build_sintab  # noqa: E402
        want = build_sintab()
        raw = open(os.path.join(TABLES_DIR, "lfo_sin_table.hex"),
                   "r", encoding="ascii").read().split()
        got = [int(t, 16) for t in raw]
        got = [v - (1 << 26) if v >= (1 << 25) else v for v in got]
        self.assertEqual(got, want,
                         "lfo_sin_table.hex drifted from build_sintab()")


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
        outdir = tempfile.mkdtemp(prefix="h06_eq_")
        try:
            proc = run_tool(COMPARE_TOOL,
                            ["--outdir", outdir, "--skip-mutants"],
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
        self.assertGreaterEqual(eq["expected_rows"], MIN_FRAMES)
        self.assertGreater(eq["bench_max_cycles_per_frame"], 0)

    def test_negative_controls_fail_equivalence(self):
        if not self.can_run:
            self.skipTest(self.reason)
        outdir = tempfile.mkdtemp(prefix="h06_mut_")
        try:
            proc = run_tool(COMPARE_TOOL, ["--outdir", outdir],
                            timeout=1800)
            self.assertEqual(proc.returncode, 0,
                             f"harness failed: {proc.stdout[-600:]} "
                             f"{proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "compare_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        self.assertEqual(rep["equivalence"]["status"], "PASS")
        controls = rep["negative_controls"]
        self.assertEqual(set(controls),
                         {"lsb", "fixedkb", "sync", "persample"})
        for name in ("lsb", "fixedkb", "sync", "persample"):
            c = controls[name]
            self.assertTrue(c["status"].startswith("MET"),
                            f"negative control {name} must FAIL the exact "
                            f"compare -- a passing mutant proves the "
                            f"harness has no resolution")
            self.assertGreater(c["mismatches"], 0)
            self.assertIsNotNone(c["first_mismatch"])


class TestInterfaceContract(unittest.TestCase):
    """Static per-sample-host-dependency guard (issue #28 acceptance:
    an implementation that per-sample streams across the boundary fails)."""

    RTL_TEXT = None

    @classmethod
    def setUpClass(cls):
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            cls.RTL_TEXT = f.read()

    def test_default_build_interface_is_exactly_contracted(self):
        ports = parse_input_ports(self.RTL_TEXT)
        failure = interface_guard(ports)
        self.assertIsNone(failure, failure)

    def test_guard_has_resolution_against_a_streaming_interface(self):
        # the negative-control build adds the illegal per-sample port; the
        # same guard must FAIL it (a guard that passes anything is no
        # control at all)
        mutant_ports = parse_input_ports(
            self.RTL_TEXT, defines=("H06_PER_SAMPLE_HOST",))
        self.assertIn("host_stream_valid", mutant_ports)
        failure = interface_guard(mutant_ports)
        self.assertIsNotNone(failure,
                             "the guard must FAIL a per-sample streaming "
                             "interface (split violation)")
        self.assertIn("host_stream_valid", failure)

    def test_frame_tick_is_payload_less_and_cfg_is_narrow(self):
        # structural statement of the split: the tick carries no data and
        # the only host data port is the 8-bit-address/32-bit-data
        # per-event write port
        self.assertRegex(self.RTL_TEXT,
                         r"input\s+wire\s+frame_tick")
        self.assertRegex(self.RTL_TEXT,
                         r"input\s+wire\s+cfg_wr,")
        self.assertRegex(self.RTL_TEXT,
                         r"input\s+wire\s+\[7:0\]\s+cfg_addr")
        self.assertRegex(self.RTL_TEXT,
                         r"input\s+wire\s+\[31:0\]\s+cfg_wdata")

    def test_hygiene_bookends_and_control_ifdefs(self):
        self.assertIn("`default_nettype none", self.RTL_TEXT)
        self.assertTrue(self.RTL_TEXT.rstrip().endswith(
            "`default_nettype wire"))
        for guard in ("MUTATE_INCREMENT_LSB",
                      "MUTATE_FIXED_TRACKS_KEYBOARD",
                      "MUTATE_LFO_SYNC_IGNORED",
                      "H06_PER_SAMPLE_HOST",
                      "H06_STRIP_OBSERVABILITY"):
            self.assertIn("`ifdef " + guard, self.RTL_TEXT,
                          f"negative-control ifdef {guard} must be present")


class TestLinkBudget(unittest.TestCase):
    """Issue #28 acceptance: worst-case host link traffic respects the
    H03 budget (contract sections 2 and 4.2)."""

    def test_burst_write_math_fits_design_point(self):
        # contracted writes per event: note-on = 6 per-op detune deltas
        # + 1 command; note-off = 1 command; controller event = 1 write
        note_on = 7
        burst = 16 * note_on + 16 * 1 + 16 * 1
        self.assertLessEqual(burst, 168,
                             "worst admissible burst must fit the "
                             "24.576 MHz design-point link capacity")
        # the 12.288 MHz fallback (84) does not fit this burst: recorded,
        # consistent with the H03 headline FAIL verdict for that clock
        self.assertEqual(burst, 144)
        self.assertGreater(burst, 84)

    def test_committed_vector_stream_respects_per_frame_bound(self):
        vectors = os.path.join(EVIDENCE, "vectors.txt")
        if not os.path.isfile(vectors):
            self.skipTest(f"NOT_RUN: no committed vectors at {vectors}")
        per_frame = []
        count = 0
        with open(vectors, "r", encoding="ascii") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                if line == "F":
                    per_frame.append(count)
                    count = 0
                elif line.startswith("W"):
                    count += 1
        per_frame.append(count)
        worst = max(per_frame)
        self.assertLessEqual(
            worst, 168,
            f"committed stream bursts at {worst} writes/frame -- over the "
            f"24.576 MHz design-point capacity")

    def test_patch_commit_load_fits_declared_load_cost(self):
        # 6 OP_CFG + 6 OP_FTERM + PEG rates + PEG levels + LFO params +
        # PM depth + PM sens + transpose + AM depth + commit command
        load_writes = 6 + 6 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1
        self.assertLessEqual(load_writes, 216,
                             "patch load must absorb within the declared "
                             "contract 4.1 cost")


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
        outdir = tempfile.mkdtemp(prefix="h06_synth_")
        try:
            proc = run_tool(SYNTH_TOOL, ["--outdir", outdir], timeout=3600)
            self.assertEqual(proc.returncode, 0,
                             f"synth tool failed: {proc.stdout[-600:]} "
                             f"{proc.stderr[-600:]}")
            rep = read_json(os.path.join(outdir, "synth_report.json"))
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        full = rep["gates"]["full"]
        self.assertEqual(full["status"], "PASS")
        self.assertGreater(full["mapped_flops"], FLOP_FLOOR,
                           "the six registered 32-bit increments are "
                           "irreducible")
        self.assertGreater(full["chip_area_um2"], 0.0)
        self.assertGreater(full["cell_total"], 0)
        strip = rep["gates"]["strip_negative_control"]
        self.assertEqual(strip["status"], "MET (gate fails as required)")
        self.assertLess(strip["mapped_flops"], full["mapped_flops"])
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
        self.assertGreaterEqual(rep["equivalence"]["expected_rows"],
                                MIN_FRAMES)
        for name, c in rep["negative_controls"].items():
            self.assertTrue(c["status"].startswith("MET"),
                            f"committed negative control {name} not MET")
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/h06_compare.py)")

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
                             f"tools/h06_synth.py)")
        self.assertIn("tt_025C_5v00",
                      rep["tool_identity"]["liberty"]["corner"])

    def test_committed_vectors_match_fresh_generation(self):
        vectors = os.path.join(EVIDENCE, "vectors.txt")
        expected = os.path.join(EVIDENCE, "expected.txt")
        if not (os.path.isfile(vectors) and os.path.isfile(expected)):
            self.skipTest(f"NOT_RUN: no committed vectors at {vectors}")
        outdir = tempfile.mkdtemp(prefix="h06_ev_")
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


class TestReportDoc(unittest.TestCase):
    def test_report_doc_present_with_required_anchors(self):
        if not os.path.isfile(REPORT_PATH):
            self.skipTest(f"NOT_RUN: {REPORT_PATH} not present")
        with open(REPORT_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        # split decisions + ruling cross-references the issue demands
        for anchor in ("DEC-010", "DEC-014", "DEC-019", "DEC-022",
                       "DEC-023", "NUM-008", "CONTRACT-CORE-v1",
                       "perf-transpose-12", "N05", "H03"):
            self.assertIn(anchor, text, f"doc must cite {anchor}")
        # tables identity (the SAME bytes rule)
        self.assertIn("reference/tables/manifest.json", text)
        self.assertIn("freqlut_table.hex", text)
        self.assertIn("lfo_sin_table.hex", text)
        # measured results recorded
        self.assertIn("4,332", text)
        self.assertIn("24.576", text)
        self.assertRegex(text, r"[0-9,]+\s*µm|mm")
        # claim boundary + unmeasured items labelled
        self.assertRegex(text, r"(?i)does NOT say")
        flat = " ".join(text.split()).lower()
        for boundary in ("no place-and-route", "no timing closure",
                         "no fit claim", "no original-dx7 fidelity"):
            self.assertIn(boundary, flat,
                          f"report must explicitly disclaim: {boundary!r}")
        self.assertIn("unmeasured", flat)
        # negative controls named
        for control in ("MUTATE_INCREMENT_LSB",
                        "MUTATE_FIXED_TRACKS_KEYBOARD",
                        "MUTATE_LFO_SYNC_IGNORED",
                        "H06_PER_SAMPLE_HOST",
                        "H06_STRIP_OBSERVABILITY"):
            self.assertIn(control, text)


if __name__ == "__main__":
    unittest.main()
