"""Tests for the shared single-module probe synthesis driver (issue #291).

Drives tools/h01_synth.py, tools/h04_synth.py, tools/h05_synth.py and
tools/h06_synth.py end-to-end through their main() against a MOCK yosys
executable and a stub liberty file: no host PDK, no live yosys, so these
cases run in the fast lane (DR-0009). They establish that the
consolidation onto tools/probe_synth.py preserved each tool's CLI, the
six-step yosys script, exit codes, gate verdicts and the exact report
skeleton of the committed evidence -- NOT that live yosys still agrees
(that is the live suites' guarded run, untouched here).

Coverage:
  - --mode both / full / strip per tool: exit codes and gate verdicts
    (full PASS, strip negative control MET; strip-only stays FAIL/exit 1
    because no gate can pass without the full run, the pre-#291 shape);
  - the report key tree matches the committed evidence
    synth_report.json exactly (recursively; cells_by_name/dff_cells keys
    are cell names and only their dict shape is pinned);
  - the mock yosys received the expected six-step script: read_verilog
    [-D<probe>_STRIP_OBSERVABILITY] <rtl>; hierarchy -check -top <top>;
    synth -top <top>; [ltp]; dfflibmap; abc; stat -liberty;
  - PdkNotFound still exits 2 with a COULD-NOT-RUN message;
  - a FULL transcript with absent chip-area and/or sequential-area
    lines (issue #295): summary printed with n/a, null areas in the
    report, gates.full FAIL, exit 1 -- never a TypeError from :.1f.

Live negative controls (must demonstrably fail the check they target):
  - a strip run that fails to reduce flops yields
    strip_negative_control status "NOT MET (gate still passes -- control
    failed)" and exit 1;
  - a full run at/below the flop floor yields gates.full FAIL and exit 1
    while area/cells stay positive (the floor, not the area terms, is
    what fires);
  - every full-gate conjunct has its own isolating fixture (issue #293):
    flops exactly AT the tool's floor constant (pinning `>` strict),
    chip area line absent or 0.0, sequential area 0.0, and cell total 0
    each FAIL the gate with the other conjuncts positive, while the
    unmutated gate still PASSes the positive fixture (a fixture that
    fails everything pins nothing);
  - a hierarchical stat transcript still raises CheckFailure from each
    tool's parse_stat (the #95 guard, replayed through the tools).
"""

import copy
import json
import os
import stat
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from io import StringIO

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOLS = os.path.join(REPO, "tools")
if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)

import h01_synth  # noqa: E402
import h04_synth  # noqa: E402
import h05_synth  # noqa: E402
import h06_synth  # noqa: E402

TOOLS_BY_NAME = {
    "h01": h01_synth,
    "h04": h04_synth,
    "h05": h05_synth,
    "h06": h06_synth,
}

# (module, issue, rtl rel, tb rel, top, strip define, ltp, outdir evidence)
PROBES = [
    ("h01", 20, "rtl/operator_probe.v", "rtl/tb_operator_probe.v",
     "operator_probe", "H01_STRIP_OBSERVABILITY", True,
     "evidence/h01-operator-probe"),
    ("h04", 26, "rtl/env_unit.v", "rtl/tb_env_unit.v",
     "env_unit", "H04_STRIP_OBSERVABILITY", True,
     "evidence/h04-env-rtl"),
    ("h05", 27, "rtl/alg_router.v", "rtl/tb_alg_router.v",
     "alg_router", "H05_STRIP_OBSERVABILITY", False,
     "evidence/h05-routing-rtl"),
    ("h06", 28, "rtl/pitch_mod.v", "rtl/tb_pitch_mod.v",
     "pitch_mod", "H06_STRIP_OBSERVABILITY", False,
     "evidence/h06-pitch-rtl"),
]

MOCK_FLOPS = 4200          # above every probe floor (max is H05's 4096)
MOCK_CELLS = 4400
MOCK_AREA = 1234.5
MOCK_SEQ_AREA = 500.25
MOCK_LTP_DEPTH = 11

# Generic single-module mapped transcript: satisfies every probe's
# parse_stat (one area block, no hierarchy), rom_representation (the
# MEMORY_MAP line; one proc_rom extraction for H04's local parser) and --
# for H01/H04 -- the ltp hint. Flops exceed every floor.
FULL_TRANSCRIPT = f"""Mapping memory $auto$proc_rom$001 in module top
Executing MEMORY_MAP pass (converting memories to logic and gates)
Longest topological path in top (length={MOCK_LTP_DEPTH}):

   {MOCK_FLOPS} 5.00E+04   gf180mcu_fd_sc_mcu7t5v0__dffq_1
     200 1.00E+04   gf180mcu_fd_sc_mcu7t5v0__and2_1

   {MOCK_CELLS} 6.00E+04 cells
   Chip area for module '\\top': {MOCK_AREA}
     of which used for sequential elements: {MOCK_SEQ_AREA} (40.00%)
"""

# The strip control maps nothing: no cells, no area line (the committed
# strip logs' shape -- parseable, zeros/None).
STRIP_TRANSCRIPT = """Executing MEMORY_MAP pass (converting memories to logic and gates)
strip build: every output tied off; all state removed
"""

# Issue #295: a FULL transcript can legitimately carry no
# 'Chip area for module' line and/or no sequential-elements line (the
# committed strip logs are a real yosys transcript with no area line at
# all), so chip_area_um2 / seq_area_um2 reach _print_summary as None.
CHIP_AREA_LINE = f"   Chip area for module '\\top': {MOCK_AREA}\n"
SEQ_AREA_LINE = (f"     of which used for sequential elements: "
                 f"{MOCK_SEQ_AREA} (40.00%)\n")


def full_transcript_without(chip=False, seq=False):
    """FULL_TRANSCRIPT minus the chosen area line(s)."""
    t = FULL_TRANSCRIPT
    if chip:
        t = t.replace(CHIP_AREA_LINE, "")
    if seq:
        t = t.replace(SEQ_AREA_LINE, "")
    return t

# Hierarchical transcript (module names generic): the #95 guard must
# refuse it from every probe's parse_stat.
HIERARCHICAL_TRANSCRIPT = """
=== top.sub ===

   Chip area for module '\\top.sub': 12.500000

=== design hierarchy ===

   Chip area for top module '\\top': 34.500000
"""

# Each probe's flop floor lives on its own tool module; read it from
# the attribute (never a literal) so a floor bump retracks here.
FLOOR_ATTRS = {
    "h01": "PHASE_FLOP_FLOOR",
    "h04": "ENV_SLOT_FLOOR",
    "h05": "BUS_FLOP_FLOOR",
    "h06": "FLOP_FLOOR",
}


def stat_transcript(flops, cells, chip_area, seq_area):
    """A parseable single-module mapped transcript isolating one full-gate
    conjunct: `flops` dff cells plus a `cells` summary line, and chip /
    sequential area lines (None = line absent, 0.0 = zero-valued line).
    Otherwise the same shape as FULL_TRANSCRIPT (MEMORY_MAP + ltp lines),
    so every probe's parse_stat / rom_representation / parse_ltp accepts
    it."""
    chip = ("" if chip_area is None
            else f"   Chip area for module '\\top': {chip_area}\n")
    seq = ("" if seq_area is None else
           f"     of which used for sequential elements: {seq_area}"
           " (40.00%)\n")
    return f"""Mapping memory $auto$proc_rom$001 in module top
Executing MEMORY_MAP pass (converting memories to logic and gates)
Longest topological path in top (length={MOCK_LTP_DEPTH}):

   {flops} 5.00E+04   gf180mcu_fd_sc_mcu7t5v0__dffq_1
     200 1.00E+04   gf180mcu_fd_sc_mcu7t5v0__and2_1

   {cells} 6.00E+04 cells
{chip}{seq}"""


def write_fake_yosys(td):
    """A mock yosys: `-V` reports a fixed version; `-p <script>` appends
    the script to $MOCK_DIR/scripts.log and cats the transcript selected
    by the strip define. Not committed evidence and not a measurement: a
    hand-built control fixture."""
    p = os.path.join(td, "mock_yosys")
    with open(p, "w", encoding="utf-8") as f:
        f.write(
            "#!/bin/sh\n"
            'if [ "$1" = "-V" ]; then\n'
            '  echo "Yosys 0.42.42 (mock probe_synth fixture)"\n'
            "  exit 0\n"
            "fi\n"
            'printf \'%s\\n\' "$2" >> "$MOCK_DIR/scripts.log"\n'
            'case "$2" in\n'
            '  *STRIP_OBSERVABILITY*) cat "$MOCK_DIR/strip.log";;\n'
            '  *) cat "$MOCK_DIR/full.log";;\n'
            "esac\n")
    os.chmod(p, os.stat(p).st_mode | stat.S_IXUSR)
    return p


class ProbeSynthMockTest(unittest.TestCase):
    """End-to-end main() runs per probe against the mock yosys."""

    def setUp(self):
        self._td = tempfile.TemporaryDirectory(prefix="probe_synth_")
        self.td = self._td.name
        self.liberty = os.path.join(self.td, "stub.lib")
        with open(self.liberty, "w", encoding="utf-8") as f:
            f.write("/* stub liberty: hashed, never parsed */\n")
        self.yosys = write_fake_yosys(self.td)
        self.mock_dir = os.path.join(self.td, "mock")
        os.makedirs(self.mock_dir)
        os.environ["MOCK_DIR"] = self.mock_dir
        self.outdir = os.path.join(self.td, "out")

    def tearDown(self):
        del os.environ["MOCK_DIR"]
        self._td.cleanup()

    def set_transcripts(self, full=FULL_TRANSCRIPT, strip=STRIP_TRANSCRIPT):
        with open(os.path.join(self.mock_dir, "full.log"), "w",
                  encoding="utf-8") as f:
            f.write(full)
        with open(os.path.join(self.mock_dir, "strip.log"), "w",
                  encoding="utf-8") as f:
            f.write(strip)
        scripts = os.path.join(self.mock_dir, "scripts.log")
        if os.path.exists(scripts):
            os.remove(scripts)

    def run_main(self, module, extra):
        out, err = StringIO(), StringIO()
        with redirect_stdout(out), redirect_stderr(err):
            rc = module.main(extra)
        return rc, out.getvalue(), err.getvalue()

    def report(self, outdir=None):
        with open(os.path.join(outdir or self.outdir, "synth_report.json"),
                  "r", encoding="utf-8") as f:
            return json.load(f)

    # ---- report-shape comparison against the committed evidence ----

    DYNAMIC_CHILD_KEYS = {"cells_by_name", "dff_cells"}  # keys are cell names

    def assert_same_key_tree(self, got, want, path="report"):
        self.assertIsInstance(got, dict, f"{path}: expected a dict")
        self.assertEqual(
            set(got), set(want),
            f"{path}: key sets differ; missing={sorted(set(want) - set(got))}"
            f" extra={sorted(set(got) - set(want))}")
        for k, w in want.items():
            if k in self.DYNAMIC_CHILD_KEYS:
                self.assertIsInstance(got[k], dict, f"{path}.{k}")
                continue
            if isinstance(w, dict) and w:
                self.assert_same_key_tree(got[k], w, f"{path}.{k}")

    def evidence_want(self, evidence_rel, mode):
        with open(os.path.join(REPO, evidence_rel, "synth_report.json"),
                  "r", encoding="utf-8") as f:
            want = json.load(f)
        want = copy.deepcopy(want)
        if mode == "full":
            del want["runs"]["strip"]
            del want["gates"]["strip_negative_control"]
        elif mode == "strip":
            del want["runs"]["full"]
            want["gates"] = {}
        return want

    def base_args(self):
        return ["--yosys", self.yosys, "--liberty", self.liberty,
                "--outdir", self.outdir]

    # ---- the acceptance cases ----

    def test_mode_both_full_and_strip_verdicts_and_key_tree(self):
        for name, _issue, rtl, tb, top, define, ltp, evidence in PROBES:
            with self.subTest(probe=name):
                self.set_transcripts()
                rc, out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args())
                self.assertEqual(rc, 0, f"{name}: {out}")
                rep = self.report()
                self.assertEqual(rep["status"], "PASS")
                self.assertEqual(rep["gates"]["full"]["status"], "PASS")
                self.assertEqual(
                    rep["gates"]["strip_negative_control"]["status"],
                    "MET (gate fails as required)")
                self.assertEqual(set(rep["runs"]), {"full", "strip"})
                self.assertEqual(
                    set(rep["gates"]), {"full", "strip_negative_control"})
                self.assert_same_key_tree(
                    rep, self.evidence_want(evidence, "both"))
                # measured values flowed from the mock transcript
                full = rep["runs"]["full"]
                self.assertEqual(full["flop_total"], MOCK_FLOPS)
                self.assertEqual(full["cell_total"], MOCK_CELLS)
                self.assertEqual(full["chip_area_um2"], MOCK_AREA)
                self.assertEqual(full["seq_area_um2"], MOCK_SEQ_AREA)
                self.assertEqual(full["mode"], "full")
                self.assertEqual(full["rom_representation"]
                                 ["representation"], "logic")
                self.assertEqual(rep["runs"]["strip"]["flop_total"], 0)
                self.assertEqual(rep["runs"]["strip"]["mode"], "strip")
                self.assertIn("gate full: PASS", out)
                self.assertIn("gate strip_negative_control: MET", out)
                self.assertIn("report:", out)
                # the six-step script the mock actually received
                rtl_abs = os.path.join(REPO, rtl)
                lib = self.liberty
                steps = [f"read_verilog {rtl_abs}",
                         f"hierarchy -check -top {top}",
                         f"synth -top {top}"]
                if ltp:
                    steps.append("ltp")
                steps += [f"dfflibmap -liberty {lib}",
                          f"abc -liberty {lib}",
                          f"stat -liberty {lib}"]
                strip_steps = [f"read_verilog -D{define} {rtl_abs}",
                               f"hierarchy -check -top {top}",
                               f"synth -top {top}"] + steps[-3:]
                with open(os.path.join(self.mock_dir, "scripts.log"),
                          "r", encoding="utf-8") as f:
                    got = f.read().splitlines()
                self.assertEqual(
                    got, ["; ".join(steps), "; ".join(strip_steps)])

    def test_mode_full_passes_without_strip_gate(self):
        for name, _issue, _rtl, _tb, _top, _d, _ltp, evidence in PROBES:
            with self.subTest(probe=name):
                self.set_transcripts()
                rc, out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertEqual(rc, 0, f"{name}: {out}")
                rep = self.report()
                self.assertEqual(rep["status"], "PASS")
                self.assertEqual(rep["gates"]["full"]["status"], "PASS")
                self.assertEqual(set(rep["gates"]), {"full"})
                self.assertEqual(set(rep["runs"]), {"full"})
                self.assert_same_key_tree(
                    rep, self.evidence_want(evidence, "full"))

    def test_mode_strip_fails_with_no_passable_gate(self):
        # Pre-#291 shape preserved verbatim: with no full run there is no
        # gate that can pass, so status is FAIL and the exit code is 1.
        for name, _issue, _rtl, _tb, _top, _d, _ltp, evidence in PROBES:
            with self.subTest(probe=name):
                self.set_transcripts()
                rc, _out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "strip"])
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(rep["status"], "FAIL")
                self.assertEqual(rep["gates"], {})
                self.assertEqual(set(rep["runs"]), {"strip"})
                self.assert_same_key_tree(
                    rep, self.evidence_want(evidence, "strip"))

    def test_ltp_hint_recorded_only_for_ltp_probes(self):
        self.set_transcripts()
        self.run_main(TOOLS_BY_NAME["h01"], self.base_args())
        self.assertEqual(self.report()["runs"]["full"]
                         ["generic_gate_depth"], MOCK_LTP_DEPTH)
        self.assertEqual(self.report()["runs"]["full"]["loop_warnings"], 0)
        self.set_transcripts()
        self.run_main(TOOLS_BY_NAME["h05"], self.base_args())
        self.assertNotIn("generic_gate_depth", self.report()["runs"]["full"])

    def test_pdk_not_found_exits_2(self):
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts()
                rc, _out, err = self.run_main(
                    TOOLS_BY_NAME[name],
                    ["--yosys", self.yosys,
                     "--ciel-root", os.path.join(self.td, "no-pdk"),
                     "--outdir", self.outdir])
                self.assertEqual(rc, 2)
                self.assertIn("COULD-NOT-RUN:", err)

    # ---- live negative controls ----

    def test_control_strip_run_that_fails_to_reduce_flops(self):
        # The strip transcript reports the SAME flops as the full build:
        # the negative control must detect that the gate still passes.
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts(strip=FULL_TRANSCRIPT)
                rc, _out, _err = self.run_main(TOOLS_BY_NAME[name],
                                               self.base_args())
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(
                    rep["gates"]["strip_negative_control"]["status"],
                    "NOT MET (gate still passes -- control failed)")
                self.assertEqual(rep["status"], "FAIL")

    def test_control_full_run_at_or_below_flop_floor_fails(self):
        # Zero flops while area/cells stay positive: the FAIL must come
        # from the flop floor, not from the area terms.
        at_floor = """     200 1.00E+04   gf180mcu_fd_sc_mcu7t5v0__and2_1

     200 1.00E+04 cells
   Chip area for module '\\top': 1234.5
     of which used for sequential elements: 500.25 (40.00%)
"""
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts(full=at_floor)
                rc, _out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(rep["gates"]["full"]["status"], "FAIL")
                self.assertEqual(rep["runs"]["full"]["flop_total"], 0)
                self.assertEqual(rep["runs"]["full"]["cell_total"], 200)
                self.assertEqual(rep["status"], "FAIL")

    def test_control_unmutated_gate_passes_positive_fixture(self):
        # Failure control for the conjunct fixtures below: the same
        # harness against the unmutated gate must still PASS the positive
        # fixture -- a fixture that fails everything pins nothing.
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts()
                rc, _out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertEqual(rc, 0)
                self.assertEqual(
                    self.report()["gates"]["full"]["status"], "PASS")

    def test_control_full_run_at_exact_flop_floor_fails(self):
        # flops == the tool's own floor constant with area/cells
        # positive: pins `>` strict (the >= mutation must fail here).
        for name, module in TOOLS_BY_NAME.items():
            floor = getattr(module, FLOOR_ATTRS[name])
            with self.subTest(probe=name, floor=floor):
                self.set_transcripts(full=stat_transcript(
                    flops=floor, cells=MOCK_CELLS, chip_area=MOCK_AREA,
                    seq_area=MOCK_SEQ_AREA))
                rc, _out, _err = self.run_main(
                    module, self.base_args() + ["--mode", "full"])
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(rep["gates"]["full"]["status"], "FAIL")
                full = rep["runs"]["full"]
                self.assertEqual(full["flop_total"], floor)
                self.assertEqual(full["chip_area_um2"], MOCK_AREA)
                self.assertEqual(full["seq_area_um2"], MOCK_SEQ_AREA)
                self.assertEqual(full["cell_total"], MOCK_CELLS)

    def test_control_full_run_zero_chip_area_fails(self):
        # Positive flops/cells/seq-area with the chip area line present
        # but 0.0 -- and, as the edge case, absent entirely (None): only
        # the chip-area conjunct can fire.
        for chip_area in (0.0, None):
            for name in TOOLS_BY_NAME:
                with self.subTest(probe=name, chip_area=chip_area):
                    self.set_transcripts(full=stat_transcript(
                        flops=MOCK_FLOPS, cells=MOCK_CELLS,
                        chip_area=chip_area, seq_area=MOCK_SEQ_AREA))
                    # Both branches exit 1: an absent area line prints
                    # "n/a" (fmt_area, issue #295) instead of raising.
                    rc, _out, _err = self.run_main(
                        TOOLS_BY_NAME[name],
                        self.base_args() + ["--mode", "full"])
                    self.assertEqual(rc, 1)
                    rep = self.report()
                    self.assertEqual(rep["gates"]["full"]["status"],
                                     "FAIL")
                    self.assertEqual(rep["runs"]["full"]["chip_area_um2"],
                                     chip_area)

    def test_control_full_run_zero_seq_area_fails(self):
        # Positive flops/cells/chip-area with `of which used for
        # sequential elements: 0.0`: only the seq-area conjunct can fire.
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts(full=stat_transcript(
                    flops=MOCK_FLOPS, cells=MOCK_CELLS,
                    chip_area=MOCK_AREA, seq_area=0.0))
                rc, _out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(rep["gates"]["full"]["status"], "FAIL")
                self.assertEqual(rep["runs"]["full"]["seq_area_um2"], 0.0)

    def test_control_full_run_zero_cells_fails(self):
        # Positive flops and areas with a zero `<N> <area> cells`
        # summary line: only the cell-total conjunct can fire.
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts(full=stat_transcript(
                    flops=MOCK_FLOPS, cells=0, chip_area=MOCK_AREA,
                    seq_area=MOCK_SEQ_AREA))
                rc, _out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertEqual(rc, 1)
                rep = self.report()
                self.assertEqual(rep["gates"]["full"]["status"], "FAIL")
                self.assertEqual(rep["runs"]["full"]["cell_total"], 0)

    def test_control_hierarchical_transcript_raises_check_failure(self):
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                mod = TOOLS_BY_NAME[name]
                with self.assertRaises(mod.CheckFailure):
                    mod.parse_stat(HIERARCHICAL_TRANSCRIPT)

    # ---- issue #295: absent area lines in a FULL run ----

    def test_full_run_absent_area_lines_exits_1_not_typeerror(self):
        # A full run whose transcript has no 'Chip area for module' line
        # and/or no sequential-elements line maps no area: the gate FAILs,
        # the report keeps null areas, and main() must return the
        # documented 1 (gate failure) with the summary printed -- not
        # raise TypeError formatting None with :.1f (issue #295).
        cases = {
            "chip-absent": (True, False, None, MOCK_SEQ_AREA),
            "seq-absent": (False, True, MOCK_AREA, None),
            "both-absent": (True, True, None, None),
        }
        for name in TOOLS_BY_NAME:
            for case, (drop_chip, drop_seq, want_chip,
                       want_seq) in cases.items():
                with self.subTest(probe=name, case=case):
                    self.set_transcripts(
                        full=full_transcript_without(chip=drop_chip,
                                                     seq=drop_seq))
                    rc, out, _err = self.run_main(
                        TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                                 "full"])
                    self.assertEqual(rc, 1, f"{name}/{case}: {out}")
                    self.assertIn("full mapped:", out)
                    self.assertIn("report:", out)
                    rep = self.report()
                    self.assertEqual(rep["gates"]["full"]["status"], "FAIL")
                    self.assertEqual(rep["status"], "FAIL")
                    self.assertEqual(rep["runs"]["full"]["chip_area_um2"],
                                     want_chip)
                    self.assertEqual(rep["runs"]["full"]["seq_area_um2"],
                                     want_seq)

    def test_full_run_present_area_summary_text_unchanged(self):
        # With both area lines present the summary text is byte-identical
        # to the pre-#291 'X.X um2 (seq Y.Y)' form (issue #295).
        for name in TOOLS_BY_NAME:
            with self.subTest(probe=name):
                self.set_transcripts()
                _rc, out, _err = self.run_main(
                    TOOLS_BY_NAME[name], self.base_args() + ["--mode",
                                                             "full"])
                self.assertIn(
                    f"chip {MOCK_AREA:.1f} um2 "
                    f"(seq {MOCK_SEQ_AREA:.1f})", out)


if __name__ == "__main__":
    unittest.main()
