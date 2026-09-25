#!/usr/bin/env python3
"""Fast-lane guards for tools/h10_synth.py (issue #32, H10 policy A/B).

A. The yosys script loads the liberty as blackbox cell definitions
   (`read_liberty -lib`) BEFORE any mapping pass. Without it, `check
   -assert` flags every net driven by a mapped liberty-cell output as
   "used but has no driver" -- the 2026-09-24 false failure (13,757
   problems on this core) this test pins against regression.
B. Only the -dont_use argument differs between the two policies (the A/B
   is only a policy measurement if everything else is identical).
C. parse_hier_stat reads the whole-top total from the design-hierarchy
   section, never the first per-module block (the #82 defect class): a
   transcript whose first module is not the top must yield the top total.
D. Negative control for C: a transcript with per-module blocks but no
   hierarchy total is refused (RunFailure), not silently read per-module.

Stdlib only; no yosys needed.
"""

import os
import sys
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))

import h10_synth  # noqa: E402

LIB = "/pdklib/tt.lib"

# A REAL `stat -liberty` transcript (Yosys 0.69+post 143eb14f9, gf180mcu 7t tt
# liberty, a two-module probe design: `top` instantiates `sub`), with the
# per-module blocks reordered so the NON-top module comes first -- the order
# yosys 0.57 emits (alphabetical), which is what made tools/h07_synth.py read
# alg_router's local area as the core's (#82).
TRANSCRIPT = """
9. Printing statistics.

=== sub ===

        +----------Local Count, excluding submodules.
        |        +-Local Area, excluding submodules.
        |        | 
        3        - wires
        9        - wire bits
        3        - public wires
        9        - public wire bits
        3        - ports
        9        - port bits
        4  272.205 cells
        4  272.205   gf180mcu_fd_sc_mcu7t5v0__dffq_2

   Chip area for module '\\sub': 272.204800
     of which used for sequential elements: 272.204800 (100.00%)

=== top ===

        +----------Local Count, excluding submodules.
        |        +-Local Area, excluding submodules.
        |        | 
        5        - wires
       17        - wire bits
        4        - public wires
       13        - public wire bits
        3        - ports
        9        - port bits
        6  298.547 cells
        2   26.342   gf180mcu_fd_sc_mcu7t5v0__clkinv_2
        4  272.205   gf180mcu_fd_sc_mcu7t5v0__dffq_2
        1        - submodules
        1        -   sub

   Chip area for module '\\top': 298.547200
     of which used for sequential elements: 272.204800 (91.18%)

=== design hierarchy ===

        +----------Count including submodules.
        |        +-Area including submodules.
        |        | 
       10  570.752 top
        4  272.205 sub

        +----------Count including submodules.
        |        +-Area including submodules.
        |        | 
        8        - wires
       26        - wire bits
        7        - public wires
       22        - public wire bits
        6        - ports
       18        - port bits
        -        - memories
        -        - memory bits
        -        - processes
       10  570.752 cells
        2   26.342   gf180mcu_fd_sc_mcu7t5v0__clkinv_2
        8   544.41   gf180mcu_fd_sc_mcu7t5v0__dffq_2
        1  272.205 submodules
        1  272.205   sub

   Chip area for top module '\\top': 570.752000
     of which used for sequential elements: 544.409600 (95.38%)

"""


class TestScript(unittest.TestCase):
    def test_a_liberty_loaded_before_mapping(self):
        for pol, du in h10_synth.POLICIES.items():
            s = h10_synth.yosys_script(LIB, du, "/tmp/x.v")
            self.assertEqual(s[0], f"read_liberty -lib {LIB}", pol)
            first_map = min(i for i, c in enumerate(s)
                            if c.startswith(("dfflibmap", "abc")))
            self.assertLess(0, first_map)
            self.assertLess(s.index("check -assert"), len(s))

    def test_b_policies_differ_only_in_dont_use(self):
        a = h10_synth.yosys_script(LIB, h10_synth.POLICIES["orfs_stock"],
                                   "/tmp/x.v")
        b = h10_synth.yosys_script(LIB, h10_synth.POLICIES["x1_allowed"],
                                   "/tmp/x.v")
        strip = [c.replace(" -dont_use *_1", "") for c in a]
        self.assertEqual(strip, b)
        self.assertTrue(any("-dont_use *_1" in c for c in a))


class TestParse(unittest.TestCase):
    def test_c_top_total_not_first_module(self):
        st = h10_synth.parse_hier_stat(TRANSCRIPT)
        # the first per-module block is `sub` (272.2048); the top total wins
        self.assertEqual(st["top"], "top")
        self.assertEqual(st["stdcell_area_um2"], 570.752)
        self.assertEqual(st["instance_count"], 10)
        self.assertEqual(st["seq_area_um2"], 544.4096)
        self.assertEqual(st["flop_count"], 8)
        self.assertEqual(st["module_local_area_um2"],
                         {"sub": 272.2048, "top": 298.5472})
        self.assertEqual(st["top_submodule_instances"], {"sub": 1})

    def test_d_no_hierarchy_total_refused(self):
        head = TRANSCRIPT.split("=== design hierarchy ===")[0]
        with self.assertRaises(h10_synth.RunFailure):
            h10_synth.parse_hier_stat(head)


if __name__ == "__main__":
    unittest.main()
