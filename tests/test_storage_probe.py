#!/usr/bin/env python3
"""H02 storage probe tests (issue #21).

Covers, per the issue's acceptance:
  - N01 bit accounting recomputed from spec/numeric-profile-v1.json and
    cross-checked against the RTL's own field-width parameters (the RTL must
    mirror the JSON classification field-for-field, in order);
  - the yosys flop-count gate and the strip-observability negative control,
    run live via tools/storage_probe_synth.py --fast when yosys + the ciel 7t
    liberty are present (guarded skip naming the missing path, never a
    silent pass);
  - committed mapped evidence: gates PASS/MET, abc actually run, RTL hashes
    not drifted (STALE detection);
  - macro tiling arithmetic recomputed independently from the ciel LEF
    files and compared against the committed tool report;
  - the iverilog smoke bench (readback of all 113 banks);
  - the report doc's required anchor citations and claim boundary;
  - tools/storage_probe_synth.py parse_stat asserts the single-module
    transcript shape its area number depends on (issue #95): the
    committed yosys_full.log replay still yields 4,115,369.9776 um^2, and
    a synthetic multi-module transcript must make the parser FAIL rather
    than return a per-module block (the #82/#94 bug pattern).

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
SPEC_PATH = os.path.join(REPO, "spec", "numeric-profile-v1.json")
RTL_PATH = os.path.join(REPO, "rtl", "storage_probe.v")
TB_PATH = os.path.join(REPO, "rtl", "storage_probe_tb.v")
TOOL_PATH = os.path.join(REPO, "tools", "storage_probe_synth.py")
REPORT_PATH = os.path.join(REPO, "docs", "H02-STORAGE-REPORT.md")
EVIDENCE_PATH = os.path.join(REPO, "evidence", "h02-storage-probe",
                             "storage_synth_report.json")

# D01-recorded local ciel install (explicit path; no PDK_ROOT / CLI)
CIEL_ROOT = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
             "gf180mcu/versions/"
             "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")
SRAM_LEF_REL = "gf180mcuD/libs.ref/gf180mcu_fd_ip_sram/lef"
SRAM_MACROS = ("sram64x8m8wm1", "sram128x8m8wm1", "sram256x8m8wm1",
               "sram512x8m8wm1")

# JSON field key -> RTL parameter (independent copy of the audit mapping)
FIELD_PARAM_NAMES = {
    "phase": "W_OP_PHASE", "freq": "W_OP_FREQ", "gain_out": "W_OP_GAIN_OUT",
    "level_in": "W_OP_LEVEL_IN", "env_level": "W_OP_ENV_LEVEL",
    "env_targetlevel": "W_OP_ENV_TARGET", "env_inc": "W_OP_ENV_INC",
    "env_staticcount": "W_OP_ENV_STATIC",
    "env_effective_outlevel": "W_OP_ENV_OUTLEVEL",
    "env_effective_rate_scaling": "W_OP_ENV_RATE_SC",
    "env_ix": "W_OP_ENV_IX", "env_rising": "W_OP_ENV_RISING",
    "env_down": "W_OP_ENV_DOWN",
    "basepitch_6": "W_N_BASEPITCH_6", "fb_buf_2": "W_N_FB_BUF_2",
    "pitchenv_level": "W_N_PENV_LEVEL",
    "pitchenv_targetlevel": "W_N_PENV_TARGET",
    "pitchenv_inc": "W_N_PENV_INC", "pitchenv_ix": "W_N_PENV_IX",
    "pitchenv_rising": "W_N_PENV_RISING", "pitchenv_down": "W_N_PENV_DOWN",
    "noteLogFreq": "W_N_NOTELOGFREQ", "playingMidiNote": "W_N_MIDI_NOTE",
    "midiChannel": "W_N_MIDI_CH", "mpePitchBend": "W_N_MPE_BEND",
    "mpePressure": "W_N_MPE_PRESSURE", "mpeTimbre": "W_N_MPE_TIMBRE",
    "voice_flags_keydown_sustained_live": "W_N_VOICE_FLAGS",
    "steal_rotation_age": "W_N_STEAL_AGE",
    "env_rates_4_x6ops": "W_P_ENV_RATES",
    "env_levels_4_x6ops": "W_P_ENV_LEVELS",
    "pitchenv_rates_4": "W_P_PENV_RATES",
    "pitchenv_levels_4": "W_P_PENV_LEVELS",
    "opmode_6": "W_P_OPMODE_6", "ampmodsens_6": "W_P_AMPMODSENS_6",
    "algorithm": "W_P_ALGORITHM", "fb_depth": "W_P_FB_DEPTH",
    "mod_depths_and_sens": "W_P_MODDEPTH_SENS",
}


def load_spec_storage():
    with open(SPEC_PATH, "r", encoding="utf-8") as f:
        return json.load(f)["storage"]


def parse_param_expr(text, name):
    """Full (possibly multi-line) expression of one parameter declaration."""
    m = re.search(r"parameter\s+%s\s*=\s*" % name, text)
    assert m, f"parameter {name} not found"
    start = m.end()
    nxt = re.search(r"parameter\s+\w+\s*=", text[start:])
    end = start + (nxt.start() if nxt else len(text) - start)
    seg = text[start:end]
    seg = " ".join(re.sub(r"//[^\n]*", "", seg).split())
    if seg.endswith(","):
        seg = seg[:-1]
    return seg


def parse_rtl_params():
    with open(RTL_PATH, "r", encoding="utf-8") as f:
        text = f.read()
    ints = {name: int(val) for name, val in re.findall(
        r"parameter\s+(W_\w+|NUM_OPS|NUM_NOTES)\s*=\s*(\d+)", text)}
    exprs = {name: parse_param_expr(text, name)
             for name in ("OP_W", "NOTE_W", "PATCH_W", "OP_ALL_W",
                          "NOTE_ALL_W")}
    return ints, exprs


def eval_param_expr(expr, values):
    """Evaluate a parameter expression of identifiers, integers and '+'."""
    def sub(m):
        token = m.group(0)
        if token.isdigit():
            return token
        if token in values:
            return str(values[token])
        raise AssertionError(f"unknown identifier in parameter expr: {token}")
    cleaned = re.sub(r"//[^\n]*", "", expr)
    if not re.fullmatch(r"[\w+\s*]+", cleaned):
        raise AssertionError(f"unexpected syntax in parameter expr: {expr!r}")
    return eval(re.sub(r"\w+", sub, cleaned))  # identifiers only, checked


def cieL_lib_path():
    return os.path.join(CIEL_ROOT, SC_LIB_REL)


def parse_lef_sizes():
    """(macro, width, height) per SRAM macro LEF, from the ciel tree."""
    out = {}
    lef_dir = os.path.join(CIEL_ROOT, SRAM_LEF_REL)
    for suffix in SRAM_MACROS:
        path = os.path.join(lef_dir, f"gf180mcu_fd_ip_sram__{suffix}.lef")
        with open(path, "r", encoding="utf-8") as f:
            text = f.read()
        m = re.search(r"SIZE\s+([\d.]+)\s+BY\s+([\d.]+)\s*;", text)
        assert m, f"no SIZE record in {path}"
        out[suffix] = (float(m.group(1)), float(m.group(2)))
    return out


def brute_min_area_tiling(need_bytes, areas_by_cap):
    """Independent enumeration (vs the tool's DP): try every mix."""
    caps = sorted(areas_by_cap)
    best = None
    ranges = [range(need_bytes // cap + 2) for cap in caps]
    for counts in __import__("itertools").product(*ranges):
        capacity = sum(c * cap for c, cap in zip(counts, caps))
        if capacity < need_bytes:
            continue
        area = sum(c * areas_by_cap[cap] for c, cap in zip(counts, caps))
        if best is None or area < best[0]:
            best = (area, dict(zip(caps, counts)))
    return best


class TestN01Accounting(unittest.TestCase):
    def test_bit_accounting_recomputed_from_spec(self):
        st = load_spec_storage()
        op = sum(st["per_operator_runtime_state_bits"].values())
        note = sum(st["per_note_runtime_state_bits"].values())
        patch = sum(st["patch_shared_state_bits"].values())
        self.assertEqual(op, 325)
        self.assertEqual(note, 446)
        self.assertEqual(patch, 444)
        self.assertEqual(st["instances"]["operators"], 96)
        self.assertEqual(st["instances"]["notes"], 16)
        self.assertEqual(96 * op + 16 * note, 38336)
        self.assertEqual(96 * op + 16 * note + patch, 38780)
        self.assertEqual(st["totals"]["total_bits"], 38336)
        self.assertEqual(st["totals"]["bits_per_operator"], op)
        self.assertEqual(st["totals"]["bits_per_note"], note)
        self.assertEqual(st["totals"]["patch_shared_bits"], patch)
        # the N01 replicated-vs-patch classification stays intact
        self.assertIn("38336 bits", st["claim"])
        self.assertIn("bounded finding", st["claim"])

    def test_rtl_parameters_mirror_the_json_classification(self):
        st = load_spec_storage()
        ints, exprs = parse_rtl_params()
        for json_key in ("per_operator_runtime_state_bits",
                         "per_note_runtime_state_bits",
                         "patch_shared_state_bits"):
            for field, width in st[json_key].items():
                param = FIELD_PARAM_NAMES[field]
                self.assertEqual(ints.get(param), width,
                                 f"{param} ({field}) must equal the JSON "
                                 f"classification width {width}")
        self.assertEqual(ints["NUM_OPS"], 96)
        self.assertEqual(ints["NUM_NOTES"], 16)
        # the RTL's derived class widths must evaluate to the N01 sums
        values = dict(ints)
        for param, want in (("OP_W", 325), ("NOTE_W", 446),
                            ("PATCH_W", 444),
                            ("OP_ALL_W", 96 * 325),
                            ("NOTE_ALL_W", 16 * 446)):
            self.assertEqual(eval_param_expr(exprs[param], values), want,
                             f"{param} expression must evaluate to {want}")
            values[param] = want

    def test_rtl_carries_default_nettype_bookends(self):
        with open(RTL_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        self.assertIn("`default_nettype none", text)
        self.assertTrue(text.rstrip().endswith("`default_nettype wire"))


class TestSynthGates(unittest.TestCase):
    """Live yosys runs (fast mode) + the committed evidence record."""

    @classmethod
    def setUpClass(cls):
        cls.yosys = shutil.which("yosys")
        cls.lib = cieL_lib_path()
        missing = []
        if cls.yosys is None:
            missing.append("yosys binary not on PATH")
        if not os.path.isfile(cls.lib):
            missing.append(f"ciel 7t liberty not found at {cls.lib}")
        cls.reason = "; ".join(missing)
        cls.can_run = not missing

    def test_flop_gate_and_negative_control_live(self):
        if not self.can_run:
            self.skipTest(f"NOT_RUN (guarded skip): {self.reason}")
        outdir = tempfile.mkdtemp(prefix="h02_test_")
        try:
            proc = subprocess.run(
                ["python3", TOOL_PATH, "--fast", "--skip-smoke",
                 "--outdir", outdir],
                capture_output=True, text=True, timeout=1200)
            self.assertEqual(proc.returncode, 0,
                             f"tool failed: {proc.stdout[-800:]} "
                             f"{proc.stderr[-800:]}")
            with open(os.path.join(outdir, "storage_synth_report.json")) as f:
                rep = json.load(f)
        finally:
            shutil.rmtree(outdir, ignore_errors=True)
        self.assertEqual(rep["gates"]["full"]["status"], "PASS")
        self.assertEqual(rep["gates"]["full"]["mapped_flops"], 38781)
        # accounting: 38336 replicated + 444 patch + 1 documented obs flop
        self.assertEqual(rep["accounting_recomputed"]["expected_mapped_flops"],
                         38336 + 444 + 1)
        strip = rep["gates"]["strip_negative_control"]
        self.assertEqual(strip["status"], "MET (gate fails as required)")
        self.assertLess(strip["mapped_flops"],
                        rep["gates"]["full"]["mapped_flops"],
                        "strip mode must remove state for the control to "
                        "demonstrate anything")

    def test_committed_evidence_gates_and_freshness(self):
        if not os.path.isfile(EVIDENCE_PATH):
            self.skipTest(f"NOT_RUN: no committed evidence at {EVIDENCE_PATH}")
        with open(EVIDENCE_PATH) as f:
            rep = json.load(f)
        self.assertEqual(rep["gates"]["full"]["status"], "PASS")
        self.assertEqual(rep["gates"]["full"]["mapped_flops"], 38781)
        self.assertEqual(rep["gates"]["strip_negative_control"]["status"],
                         "MET (gate fails as required)")
        # the committed full run must be the abc-mapped record, not --fast
        self.assertTrue(rep["runs"]["full"]["abc_mapped"])
        self.assertGreater(rep["runs"]["full"]["chip_area_um2"],
                           rep["runs"]["full"]["dff_area_um2"])
        # STALE detection: evidence hashes must match the tree's RTL bytes
        import hashlib
        for rel, digest in rep["tool_identity"]["rtl"].items():
            with open(os.path.join(REPO, rel), "rb") as f:
                actual = hashlib.sha256(f.read()).hexdigest()
            self.assertEqual(actual, digest,
                             f"{rel} drifted from committed evidence "
                             f"(evidence is STALE -- regenerate with "
                             f"tools/storage_probe_synth.py)")
        self.assertIn("tt_025C_5v00", rep["tool_identity"]["liberty"]["corner"])


class TestMacroTiling(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.lef_dir = os.path.join(CIEL_ROOT, SRAM_LEF_REL)
        cls.can_run = os.path.isdir(cls.lef_dir)
        cls.reason = (f"ciel SRAM LEF directory not found at {cls.lef_dir}"
                      if not cls.can_run else "")

    def test_tiling_arithmetic_recomputed_independently(self):
        if not self.can_run:
            self.skipTest(f"NOT_RUN (guarded skip): {self.reason}")
        sizes = parse_lef_sizes()
        areas_by_cap = {
            int(re.match(r"sram(\d+)x8", s).group(1)): w * h
            for s, (w, h) in sizes.items()}  # key = capacity in BYTES
        self.assertEqual(set(areas_by_cap), {64, 128, 256, 512})

        # runtime state: 38,336 bits = 4,792 B exactly
        runtime_bytes = 38336 // 8
        self.assertEqual(runtime_bytes, 4792)
        area, mix = brute_min_area_tiling(runtime_bytes, areas_by_cap)
        # 9 x 512 B cover 4,608 B; the 184 B remainder needs one 256 B
        # macro (cheaper than a 10th 512 B, and cheaper than any wider mix)
        self.assertEqual(mix[512], 9)
        self.assertEqual(mix[256], 1)
        self.assertAlmostEqual(area, 9 * areas_by_cap[512]
                               + areas_by_cap[256], places=3)
        # patch store: 444 bits = 55.5 B -> one 64 B macro suffices
        area_p, mix_p = brute_min_area_tiling(56, areas_by_cap)
        self.assertEqual(mix_p[64], 1)
        self.assertAlmostEqual(area_p, areas_by_cap[64], places=3)

        # cross-check against the committed tool report if present
        if os.path.isfile(EVIDENCE_PATH):
            with open(EVIDENCE_PATH) as f:
                rep = json.load(f)
            macro = rep["macro_analysis"]
            self.assertEqual(macro["runtime_state_4792B"]["min_area_mix"]
                             ["area_um2_derived"], round(area, 4))
            self.assertEqual(macro["runtime_state_4792B"]["min_area_mix"]
                             ["macro_count"],
                             sum(mix.values()))
            self.assertEqual(macro["patch_store_56B"]["min_area_mix"]
                             ["area_um2_derived"], round(area_p, 4))
            # measured identities: LEF SIZE product vs liberty area agree
            for entry in macro["macros_measured"]:
                sfx = entry["macro"].split("__")[-1]
                w, h = sizes[sfx]
                self.assertAlmostEqual(entry["lef_area_um2_derived"],
                                       w * h, places=3)
                self.assertAlmostEqual(entry["liberty_area_um2_measured"],
                                       w * h, delta=1.0)

    def test_patch_store_flops_beat_the_smallest_macro(self):
        # 444 patch flops at the mapped dffq_1 area vs one 64 B macro
        per_flop = 63.6608  # measured, liberty tt_025C_5v00 (D01 section 4)
        if os.path.isfile(EVIDENCE_PATH):
            with open(EVIDENCE_PATH) as f:
                rep = json.load(f)
            verdicts = rep["flop_vs_macro_verdicts"]
            self.assertEqual(verdicts["patch_shared_444b"]["verdict"],
                             "flops")
            self.assertAlmostEqual(
                verdicts["patch_shared_444b"]["mapped_flop_area_um2_measured"],
                444 * per_flop, places=2)


class TestSmokeBench(unittest.TestCase):
    def test_iverilog_readback_smoke(self):
        iverilog = shutil.which("iverilog")
        vvp = shutil.which("vvp")
        if iverilog is None or vvp is None:
            self.skipTest("NOT_RUN (guarded skip): iverilog/vvp not on PATH")
        tmp = tempfile.mkdtemp(prefix="h02_tb_")
        try:
            vvp_out = os.path.join(tmp, "tb.vvp")
            proc = subprocess.run(
                [iverilog, "-g2012", "-o", vvp_out, RTL_PATH, TB_PATH],
                capture_output=True, text=True, timeout=120)
            self.assertEqual(proc.returncode, 0,
                             f"iverilog failed: {proc.stderr[-500:]}")
            run = subprocess.run([vvp, vvp_out], capture_output=True,
                                 text=True, timeout=300)
            out = run.stdout + run.stderr
            self.assertIn("PASS storage_probe_tb", out)
            self.assertNotIn("FAIL storage_probe_tb", out)
        finally:
            shutil.rmtree(tmp, ignore_errors=True)


class TestReportDoc(unittest.TestCase):
    def test_report_cites_family_anchors_and_claim_boundary(self):
        if not os.path.isfile(REPORT_PATH):
            self.skipTest(f"NOT_RUN: {REPORT_PATH} not present")
        with open(REPORT_PATH, "r", encoding="utf-8") as f:
            text = f.read()
        for anchor in ("118", "235 flops/voice", "120", "512 B"):
            self.assertIn(anchor, text,
                          f"report must cite the family anchor {anchor!r}")
        self.assertIn("N01", text)
        self.assertIn("16-note", text)
        # the claim boundary must be explicit
        self.assertRegex(text, r"(?i)does NOT say")
        flat = " ".join(text.split()).lower()
        for boundary in ("no place-and-route", "no timing closure",
                         "no full-chip fit claim"):
            self.assertIn(boundary, flat,
                          f"report must explicitly disclaim: {boundary!r}")


def _storage_probe_synth():
    """Import tools/storage_probe_synth.py for the stat-parser unit tests."""
    tools = os.path.join(REPO, "tools")
    if tools not in sys.path:
        sys.path.insert(0, tools)
    import storage_probe_synth
    return storage_probe_synth


# A synthetic transcript of the shape yosys emits once the top acquires a
# submodule: one `Chip area for module` block PER module, then a
# `=== design hierarchy ===` section whose `Chip area for top module` line
# is the real whole-design total. The pre-#95 parser returned the FIRST
# per-module block (98,432.11 um^2 here) as the design area -- the #82/#94
# bug. Not committed evidence and not a measurement: a hand-built control.
MULTI_MODULE_TRANSCRIPT = """
=== storage_probe.note_bank ===

     2048 9.84E+04 cells
      576 3.67E+04   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\storage_probe.note_bank': 98432.110000
     of which used for sequential elements: 36676.339200 (37.26%)

=== storage_probe ===

   118657 4.12E+06 cells
    38781 2.47E+06   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\storage_probe': 4115369.977600
     of which used for sequential elements: 2468829.484800 (59.99%)

=== design hierarchy ===

        +----------Count including submodules.
        |        +-Area including submodules.
        |        |
   120705 4.21E+06 storage_probe
     2048 9.84E+04   storage_probe.note_bank

   120705 4.21E+06 cells
    39357 2.51E+06   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for top module '\\storage_probe': 4213802.087600
     of which used for sequential elements: 2505505.824000 (59.46%)
"""

TWO_BLOCK_TRANSCRIPT = """
=== storage_probe.note_bank ===

   Chip area for module '\\storage_probe.note_bank': 98432.110000
     of which used for sequential elements: 36676.339200 (37.26%)

=== storage_probe ===

   Chip area for module '\\storage_probe': 4115369.977600
     of which used for sequential elements: 2468829.484800 (59.99%)
"""


class TestSynthStatSingleModuleGuard(unittest.TestCase):
    """tools/storage_probe_synth.py parse_stat must ASSERT the single-module
    shape its area number silently depended on (issue #95, part of #91).

    storage_probe is a flat single-module design today: the committed
    evidence/h02-storage-probe/yosys_full.log carries exactly ONE `Chip area
    for module` line and no `=== design hierarchy ===` section, so that
    per-module block IS the whole-design total and the recorded
    4,115,369.9776 um^2 is correct. Nothing in the pre-#95 parser checked
    that, so a future submodule would have silently reintroduced the #82
    (h07) / #94 (h08) bug pattern. These cases replay the committed
    transcript -- no synthesis, no heavy host, so they run in the fast lane.
    """

    SYNTH_EVIDENCE = os.path.join(REPO, "evidence", "h02-storage-probe")
    FULL_LOG = os.path.join(SYNTH_EVIDENCE, "yosys_full.log")
    STRIP_LOG = os.path.join(SYNTH_EVIDENCE, "yosys_strip.log")
    COMMITTED_AREA_UM2 = 4115369.9776
    COMMITTED_DFF_AREA_UM2 = 2468829.4848
    COMMITTED_DFF_TOTAL = 38781
    # the per-module block a guard-less parser would hand back from the
    # synthetic multi-module transcript below
    SUBMODULE_LOCAL_AREA = 98432.11

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
                         "evidence/h02-storage-probe/yosys_full.log is no "
                         "longer single-module: this tool needs #94's "
                         "hierarchy-total parser, not an assertion")
        self.assertNotIn("=== design hierarchy ===", log)
        self.assertNotIn("Chip area for top module", log)
        st = _storage_probe_synth().parse_stat(log)
        self.assertEqual(st["chip_area_um2"], self.COMMITTED_AREA_UM2)
        self.assertEqual(st["dff_area_um2"], self.COMMITTED_DFF_AREA_UM2)
        self.assertEqual(st["dff_total"], self.COMMITTED_DFF_TOTAL)

    def test_synthparse_multi_module_transcript_fails_loudly(self):
        """NEGATIVE CONTROL: per-module block + a `=== design hierarchy ===`
        section -> parse_stat must FAIL, not return the per-module number
        the pre-#95 parser returned here (98,432.11 um^2)."""
        h = _storage_probe_synth()
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
        h = _storage_probe_synth()
        with self.assertRaises(h.CheckFailure) as ctx:
            h.parse_stat(TWO_BLOCK_TRANSCRIPT)
        self.assertIn("2 'Chip area for module' blocks", str(ctx.exception))

    def test_synthparse_strip_control_transcript_stays_parseable(self):
        """The strip-observability control maps NOTHING, so yosys prints no
        area line at all. That hierarchy-free, area-free transcript must
        stay parseable (zeros / None) or the negative control the H02 flop
        gate depends on could not be evaluated."""
        st = _storage_probe_synth().parse_stat(self._log(self.STRIP_LOG))
        self.assertEqual(st["dff_total"], 0)
        self.assertIsNone(st["chip_area_um2"])
        self.assertIsNone(st["dff_area_um2"])


if __name__ == "__main__":
    unittest.main()
