#!/usr/bin/env python3
"""Fast-lane guards for tools/hierarchy_stat.py: the ONE flop predicate.

Issue #211: tools/h10_synth.py used to carry its own copy of this parser
whose flop rule was r"__(dff|sdff|latq|icgt)" -- counting latches (latq)
and integrated clock gates (icgt) as flops -- while the shared module
counts "dff" substrings. H07 and H10 synthesize the SAME design, so the
two rules would make sibling H-items report different mapped flop counts
for one netlist the moment a map emits a latch or clock gate. These cases
pin the decided predicate on a synthetic hierarchical transcript whose
design hierarchy totals are 4x dffq_1 + 3x latq_1 + 2x icgtp_1 + 1x
nand2_1:

  - flop_total MUST be 4 (dff family only); latches and clock gates come
    back as separate labelled counts (latch_total 3, clockgate_total 2),
    never folded into the flop total;
  - the pre-#211 h10 rule yields 9 on the same transcript -- asserted
    here so a regression back to that predicate fails this test.

Issue #220 additionally pins the SHAPE contract between the parser's two
success paths: their key sets differ exactly by `top_module` (an empty
design has none), with the labelled counts present -- zero -- on both.

No yosys needed; stdlib only.
"""

import os
import re
import sys
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))

import hierarchy_stat  # noqa: E402


# A synthetic (but shape-faithful) hierarchical `stat -liberty`
# transcript: per-module blocks first (top NOT first), then the
# `=== design hierarchy ===` section closed by `Chip area for top
# module`. The hierarchy totals mix the three state/element families
# the two pre-#211 predicates disagreed about.
MIXED_TRANSCRIPT = """
=== sub ===

        3  150.100 cells
        3  150.100   gf180mcu_fd_sc_mcu7t5v0__dffq_1

   Chip area for module '\\sub': 150.100000
     of which used for sequential elements: 150.100000 (100.00%)

=== top ===

        7  350.250 cells
        7  350.250   gf180mcu_fd_sc_mcu7t5v0__dffq_1
        1        - submodules
        1        -   sub

   Chip area for module '\\top': 350.250000
     of which used for sequential elements: 350.250000 (100.00%)

=== design hierarchy ===

        +----------Count including submodules.
        |        +-Area including submodules.
        |        |
       10  500.400 top
        3  150.100 sub

        +----------Count including submodules.
        |        +-Area including submodules.
        |        |
       10  500.400 cells
        4   200.100   gf180mcu_fd_sc_mcu7t5v0__dffq_1
        3   150.200   gf180mcu_fd_sc_mcu7t5v0__latq_1
        2   100.050   gf180mcu_fd_sc_mcu7t5v0__icgtp_1
        1    50.050   gf180mcu_fd_sc_mcu7t5v0__nand2_1
        1  150.100 submodules
        1  150.100   sub

   Chip area for top module '\\top': 500.400000
     of which used for sequential elements: 350.300000 (70.00%)

"""


def old_h10_flop_total(st):
    """The pre-#211 tools/h10_synth.py predicate
    (re.search(r"__(dff|sdff|latq|icgt)", k)): kept here ONLY as the
    negative control -- what the flop total must NOT become."""
    return sum(v for k, v in st["cells_by_name"].items()
               if re.search(r"__(dff|sdff|latq|icgt)", k))


# Issue #220: the only key whose value is genuinely unknowable for an
# empty design -- an empty design has no top module, so no placeholder
# may be invented for it.
UNKNOWABLE_ON_EMPTY = {"top_module"}


class TestFlopPredicate(unittest.TestCase):
    def test_flop_total_is_dff_family_only(self):
        st = hierarchy_stat.parse_hierarchy_stat(MIXED_TRANSCRIPT, ValueError,
                                                 "#211")
        self.assertEqual(st["flop_total"], 4)
        self.assertEqual(st["dff_cells"],
                         {"gf180mcu_fd_sc_mcu7t5v0__dffq_1": 4})

    def test_latches_and_clockgates_are_separate_labelled_counts(self):
        """Latches and clock gates are reported, but never inside the
        flop total (issue #211: the two definitions must not blur)."""
        st = hierarchy_stat.parse_hierarchy_stat(MIXED_TRANSCRIPT, ValueError,
                                                 "#211")
        self.assertEqual(st["latch_total"], 3)
        self.assertEqual(st["latch_cells"],
                         {"gf180mcu_fd_sc_mcu7t5v0__latq_1": 3})
        self.assertEqual(st["clockgate_total"], 2)
        self.assertEqual(st["clockgate_cells"],
                         {"gf180mcu_fd_sc_mcu7t5v0__icgtp_1": 2})
        self.assertNotIn("gf180mcu_fd_sc_mcu7t5v0__latq_1", st["dff_cells"])
        self.assertNotIn("gf180mcu_fd_sc_mcu7t5v0__icgtp_1", st["dff_cells"])

    def test_pre_211_h10_predicate_would_count_9(self):
        """NEGATIVE CONTROL: the discarded h10 predicate counts 9 on this
        transcript -- 4 dffq + 3 latq + 2 icgtp. If parse_hierarchy_stat
        ever reverts to that rule, flop_total becomes 9 and the case
        above fails; this case records WHY it must not."""
        st = hierarchy_stat.parse_hierarchy_stat(MIXED_TRANSCRIPT, ValueError,
                                                 "#211")
        self.assertEqual(old_h10_flop_total(st), 9)
        self.assertNotEqual(st["flop_total"], old_h10_flop_total(st))


class TestAttribution(unittest.TestCase):
    def test_h10_attribution_fields(self):
        st = hierarchy_stat.parse_hierarchy_stat(
            MIXED_TRANSCRIPT, ValueError, "#211", attribution=True)
        self.assertEqual(st["module_local_area_um2"],
                         {"sub": 150.1, "top": 350.25})
        self.assertEqual(st["top_submodule_instances"], {"sub": 1})
        self.assertEqual(st["hierarchy_tree"], {
            "top": {"count_incl_sub": 10, "area_incl_sub_um2_3sf": 500.4},
            "sub": {"count_incl_sub": 3, "area_incl_sub_um2_3sf": 150.1},
        })

    def test_attribution_absent_by_default(self):
        """H07/H08 call without attribution: the dict must not carry the
        per-module fields (they are H10's opt-in extras)."""
        st = hierarchy_stat.parse_hierarchy_stat(MIXED_TRANSCRIPT, ValueError,
                                                 "#211")
        for k in ("module_local_area_um2", "top_submodule_instances",
                  "hierarchy_tree"):
            self.assertNotIn(k, st)

    def test_attribution_refuses_a_section_without_tree_headers(self):
        """NEGATIVE CONTROL: the pre-#211 inline conditional silently
        yielded an EMPTY tree when the two '+----------Count' headers
        were absent; the shared parser must refuse instead."""
        broken = MIXED_TRANSCRIPT.replace("+----------Count including "
                                          "submodules.\n", "", 1)
        with self.assertRaises(ValueError) as ctx:
            hierarchy_stat.parse_hierarchy_stat(broken, ValueError, "#211",
                                                attribution=True)
        self.assertIn("'+----------Count'", str(ctx.exception))
        # the same transcript still parses without attribution: the
        # refusal is scoped to the requested extras, not the totals.
        st = hierarchy_stat.parse_hierarchy_stat(broken, ValueError, "#211")
        self.assertEqual(st["flop_total"], 4)


class TestPathShapeSymmetry(unittest.TestCase):
    """Issue #220: the parser's two success paths (hierarchical and
    empty-design) must agree on every key whose value is knowable for
    an empty design, so a consumer written as `st["latch_total"]`
    cannot pass on a full transcript and KeyError on the strip
    control."""

    # Issue #220 edge case: the same mapped design with the latch and
    # clock-gate rows removed (counts adjusted to stay consistent:
    # 4 dffq + 1 nand2 = 5 cells, 250.150 um2) -- zero state cells, so
    # "key present, value zero" must hold on the hierarchical path too.
    ZERO_STATE_TRANSCRIPT = MIXED_TRANSCRIPT.replace(
        "        3   150.200   gf180mcu_fd_sc_mcu7t5v0__latq_1\n", ""
    ).replace(
        "        2   100.050   gf180mcu_fd_sc_mcu7t5v0__icgtp_1\n", ""
    ).replace(
        "       10  500.400 top\n", "        5  250.150 top\n"
    ).replace(
        "       10  500.400 cells\n", "        5  250.150 cells\n"
    ).replace(
        "Chip area for top module '\\top': 500.400000",
        "Chip area for top module '\\top': 250.150000",
    )

    def test_empty_and_hierarchical_paths_agree_on_knowable_keys(self):
        """The key-set difference between the two success paths is
        exactly `top_module`; a future key added to one path and not
        the other fails here instead of silently re-opening #220."""
        hier = hierarchy_stat.parse_hierarchy_stat(MIXED_TRANSCRIPT,
                                                   ValueError, "#220")
        # Guard against the silent fall-through: if the transcript's
        # `Chip area for top module` line ever stopped matching,
        # parse_hierarchy_stat would return the EMPTY path's dict and
        # the comparison below would become vacuous (empty vs empty).
        self.assertEqual(hier["totals_basis"], hierarchy_stat.HIER_BASIS)
        empty = hierarchy_stat._parse_empty_stat("", ValueError, "#220")
        self.assertEqual(set(hier) - set(empty), UNKNOWABLE_ON_EMPTY)
        self.assertEqual(set(empty) - set(hier), set())
        for k in ("latch_total", "clockgate_total"):
            self.assertEqual(empty[k], 0)
        self.assertEqual(empty["latch_cells"], {})
        self.assertEqual(empty["clockgate_cells"], {})

    def test_empty_transcript_reaches_empty_path_via_public_api(self):
        """The empty transcript must still fall through to
        _parse_empty_stat through parse_hierarchy_stat itself, and the
        labelled counts must be readable on the result."""
        st = hierarchy_stat.parse_hierarchy_stat("", ValueError, "#220")
        self.assertEqual(st["totals_basis"], hierarchy_stat.EMPTY_BASIS)
        self.assertEqual(st["latch_total"], 0)
        self.assertEqual(st["clockgate_total"], 0)

    def test_hierarchical_path_with_zero_state_cells_reports_zero_too(self):
        """A mapped design with NO latches and NO clock gates reports
        the labelled counts as 0/{} -- "key present, value zero" is not
        special to the empty path."""
        st = hierarchy_stat.parse_hierarchy_stat(self.ZERO_STATE_TRANSCRIPT,
                                                 ValueError, "#220")
        self.assertEqual(st["totals_basis"], hierarchy_stat.HIER_BASIS)
        self.assertEqual(st["latch_total"], 0)
        self.assertEqual(st["latch_cells"], {})
        self.assertEqual(st["clockgate_total"], 0)
        self.assertEqual(st["clockgate_cells"], {})
        self.assertEqual(st["flop_total"], 4)


if __name__ == "__main__":
    unittest.main()
