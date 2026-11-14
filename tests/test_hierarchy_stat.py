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


if __name__ == "__main__":
    unittest.main()
