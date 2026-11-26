#!/usr/bin/env python3
"""Fast-lane guards for tools/single_module_stat.py (issues #95, #337).

The four probe tools tools/h01_synth.py, h04_synth.py, h05_synth.py and
h06_synth.py each expose a public `parse_stat` that delegates to the ONE
shared guard in tools/single_module_stat.py (issue #119). Each of them
must ASSERT the single-module transcript shape its area number silently
depended on (issue #95, part of #91): every probe is a flat single-module
design today, so the committed yosys_full.log carries exactly ONE `Chip
area for module` block and no `=== design hierarchy ===` section, and that
per-module block IS the whole-design total. Nothing in the pre-#95 parser
checked that, so a future submodule would have silently reintroduced the
#82 (h07) / #94 (h08) bug pattern -- the same pattern whose h07 instance
was alg_router's own block being read as the whole core.

Until issue #337 these five `test_synthparse_*` cases existed as four
structurally identical copies of TestSynthStatSingleModuleGuard at the tail
of tests/test_h01.py, test_h04.py, test_h05.py and test_h06.py (added by
#95/#102, extended once per file by #119). They differed only in per-probe
parameters, so they live here once, run for every probe through subTest: a
failing probe is reported by name and never hides another probe's result.

Coverage, per probe:
  - POSITIVE: the committed transcript is still single-module and the
    guarded parser still returns the recorded totals;
  - NEGATIVE CONTROL: a synthetic multi-module transcript must make
    parse_stat FAIL rather than return the first per-module block, with
    the resolution proof that the unguarded regex really would have
    returned that block from the same transcript;
  - NEGATIVE CONTROL: two per-module blocks and no hierarchy section (a
    truncated hierarchical stat) must FAIL too;
  - the committed strip-observability transcript (no area line at all)
    stays parseable as zeros / None;
  - NEGATIVE CONTROL (issue #119): a `Chip area for top module` line alone
    must FAIL with the probe tool's own CheckFailure and name its module.

The synthetic transcripts are generated per probe from the same numbers
the removed copies carried (committed totals plus one hypothetical
submodule); they are hand-built controls, not committed evidence and not
measurements.

tests/test_storage_probe.py keeps its own suite: tools/storage_probe_synth.py
is excluded from tools/single_module_stat.py on purpose (its parse_stat
returns dff_total/dff_area_um2).

No yosys needed; every case replays a committed or hand-built transcript,
so `-k synthparse` (tools/test_fast.sh) keeps selecting them. Stdlib only.
"""

import collections
import os
import re
import sys
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOLS = os.path.join(REPO, "tools")
if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)

import h01_synth  # noqa: E402
import h04_synth  # noqa: E402
import h05_synth  # noqa: E402
import h06_synth  # noqa: E402

# One row per probe: the tool whose parse_stat is exercised, its top
# module, its evidence directory, the totals recorded in the committed
# yosys_full.log, and the hypothetical submodule the synthetic transcripts
# give that top (its local area is the number a guard-less parser would
# hand back as the design area).
Probe = collections.namedtuple(
    "Probe",
    "name tool top evidence "
    "area_um2 seq_area_um2 cell_total flop_total "
    "sub sub_area_um2 sub_seq_area_um2 sub_cells sub_flops",
)

PROBES = (
    Probe("h01", h01_synth, "operator_probe",
          "evidence/h01-operator-probe",
          area_um2=46939.9616, seq_area_um2=6302.4192,
          cell_total=2407, flop_total=99,
          sub="op_kernel", sub_area_um2=8912.34, sub_seq_area_um2=2100.8064,
          sub_cells=512, sub_flops=33),
    Probe("h04", h04_synth, "env_unit", "evidence/h04-env-rtl",
          area_um2=179055.8784, seq_area_um2=30557.184,
          cell_total=8326, flop_total=480,
          sub="rate_lut", sub_area_um2=21004.56, sub_seq_area_um2=3055.7184,
          sub_cells=1024, sub_flops=48),
    Probe("h05", h05_synth, "alg_router", "evidence/h05-routing-rtl",
          area_um2=1148899.6288, seq_area_um2=471535.5456,
          cell_total=37930, flop_total=7407,
          sub="conn_lut", sub_area_um2=64218.88, sub_seq_area_um2=8148.096,
          sub_cells=2048, sub_flops=128),
    Probe("h06", h06_synth, "pitch_mod", "evidence/h06-pitch-rtl",
          area_um2=1195376.4032, seq_area_um2=156541.9072,
          cell_total=58904, flop_total=2459,
          sub="lfo_shape", sub_area_um2=52730.11, sub_seq_area_um2=4583.5776,
          sub_cells=1536, sub_flops=72),
)

DFF = "gf180mcu_fd_sc_mcu7t5v0__dffq_1"


def _yosys_name(module):
    """The module name between quotes as yosys prints it: one backslash
    escapes the hierarchy separator."""
    return "'" + "\\" + module + "'"


def _area_lines(kind, module, area, seq):
    """The two area lines yosys prints for `kind` ('module' or 'top
    module'): the chip area, then the sequential share of it."""
    return (
        f"    Chip area for {kind} {_yosys_name(module)}: {area:.6f}\n"
        f"      of which used for sequential elements: {seq:.6f}"
        f" ({100.0 * seq / area:.2f}%)\n"
    )


def _module_block(module, cells, area, flops, seq):
    """One whole per-module block: its cells line, one dff line, then its
    area lines."""
    return (
        f"=== {module} ===\n\n"
        f"      {cells} {area:.2E} cells\n"
        f"{flops:>10} {seq:.2E}   {DFF}\n\n"
        + _area_lines("module", module, area, seq)
    )


def multi_module_transcript(p):
    """A synthetic transcript of the shape yosys emits once the top
    acquires a submodule: one `Chip area for module` block PER module,
    then a `=== design hierarchy ===` section whose `Chip area for top
    module` line is the real whole-design total. The top's own block is
    its committed one, so the FIRST per-module block is the submodule's --
    the number the pre-#95 parser returned as the design area (the #82/#94
    bug)."""
    sub = f"{p.top}.{p.sub}"
    area = p.area_um2 + p.sub_area_um2
    seq = p.seq_area_um2 + p.sub_seq_area_um2
    cells = p.cell_total + p.sub_cells
    flops = p.flop_total + p.sub_flops
    return (
        _module_block(sub, p.sub_cells, p.sub_area_um2, p.sub_flops,
                      p.sub_seq_area_um2)
        + _module_block(p.top, p.cell_total, p.area_um2, p.flop_total,
                        p.seq_area_um2)
        + "=== design hierarchy ===\n\n"
        "        +----------Count including submodules.\n"
        "        |        +-Area including submodules.\n"
        "        |        |\n"
        f"{cells:>9} {area:.2E} {p.top}\n"
        f"{p.sub_cells:>10} {p.sub_area_um2:.2E}   {sub}\n\n"
        f"{cells:>9} {area:.2E} cells\n"
        f"{flops:>10} {seq:.2E}   {DFF}\n\n"
        + _area_lines("top module", p.top, area, seq)
    )


def two_block_transcript(p):
    """The same two modules' area blocks with NO hierarchy section: a
    hierarchical stat truncated before its `=== design hierarchy ===` part."""
    sub = f"{p.top}.{p.sub}"
    return (
        f"=== {sub} ===\n\n"
        + _area_lines("module", sub, p.sub_area_um2, p.sub_seq_area_um2)
        + f"\n=== {p.top} ===\n\n"
        + _area_lines("module", p.top, p.area_um2, p.seq_area_um2)
    )


class TestSynthStatSingleModuleGuard(unittest.TestCase):
    """Each probe tool's parse_stat must ASSERT the single-module shape its
    area number silently depended on (issue #95, part of #91).

    Every case runs once per probe (PROBES) inside subTest, replaying
    committed or hand-built transcripts -- no synthesis, no heavy host, so
    they run in the fast lane. A missing committed log is a NOT_RUN skip
    that names the absent path, never a pass.
    """

    def _log(self, path):
        if not os.path.isfile(path):
            self.skipTest(f"NOT_RUN: {path} absent")
        with open(path, "r", encoding="utf-8") as f:
            return f.read()

    def _committed(self, p, name):
        return self._log(os.path.join(REPO, p.evidence, name))

    def test_synthparse_committed_log_single_module_keeps_area(self):
        """POSITIVE: the committed transcript is still single-module, and
        the guarded parser still returns today's recorded numbers."""
        for p in PROBES:
            with self.subTest(probe=p.name):
                log = self._committed(p, "yosys_full.log")
                self.assertEqual(log.count("Chip area for module"), 1,
                                 f"{p.evidence}/yosys_full.log is no longer "
                                 "single-module: this tool needs #94's "
                                 "hierarchy-total parser, not an assertion")
                self.assertNotIn("=== design hierarchy ===", log)
                self.assertNotIn("Chip area for top module", log)
                st = p.tool.parse_stat(log)
                self.assertEqual(st["chip_area_um2"], p.area_um2)
                self.assertEqual(st["seq_area_um2"], p.seq_area_um2)
                self.assertEqual(st["cell_total"], p.cell_total)
                self.assertEqual(st["flop_total"], p.flop_total)

    def test_synthparse_multi_module_transcript_fails_loudly(self):
        """NEGATIVE CONTROL: per-module block + a `=== design hierarchy ===`
        section -> parse_stat must FAIL, not return the per-module number
        the pre-#95 parser returned here."""
        for p in PROBES:
            with self.subTest(probe=p.name):
                transcript = multi_module_transcript(p)
                with self.assertRaises(p.tool.CheckFailure) as ctx:
                    p.tool.parse_stat(transcript)
                self.assertIn("hierarch", str(ctx.exception).lower())
                # and the control has resolution: the unguarded regex
                # really would have returned a per-module block from this
                # same transcript
                self.assertEqual(
                    float(re.search(r"Chip area for module .*?: ([\d.]+)",
                                    transcript).group(1)),
                    p.sub_area_um2)

    def test_synthparse_two_module_blocks_no_hierarchy_fail(self):
        """NEGATIVE CONTROL: two per-module blocks and no hierarchy section
        (a truncated hierarchical stat) must FAIL too -- taking the first
        block would under-report the design."""
        for p in PROBES:
            with self.subTest(probe=p.name):
                with self.assertRaises(p.tool.CheckFailure) as ctx:
                    p.tool.parse_stat(two_block_transcript(p))
                self.assertIn("2 'Chip area for module' blocks",
                              str(ctx.exception))

    def test_synthparse_strip_control_transcript_stays_parseable(self):
        """The strip-observability control maps NOTHING, so yosys prints no
        area line at all. That hierarchy-free, area-free transcript must
        stay parseable (zeros / None) or the negative control each probe's
        gate depends on could not be evaluated."""
        for p in PROBES:
            with self.subTest(probe=p.name):
                st = p.tool.parse_stat(self._committed(p, "yosys_strip.log"))
                self.assertEqual(st["cell_total"], 0)
                self.assertEqual(st["flop_total"], 0)
                self.assertIsNone(st["chip_area_um2"])
                self.assertIsNone(st["seq_area_um2"])

    def test_synthparse_top_area_only_transcript_fails_loudly(self):
        """NEGATIVE CONTROL (issue #119): a `Chip area for top module` line
        alone (no hierarchy header, no per-module block) must FAIL with this
        tool's CheckFailure and name its module."""
        for p in PROBES:
            with self.subTest(probe=p.name):
                with self.assertRaises(p.tool.CheckFailure) as ctx:
                    p.tool.parse_stat(
                        "Chip area for top module '\\top': 123.0\n")
                self.assertIn(p.top, str(ctx.exception))


if __name__ == "__main__":
    unittest.main()
