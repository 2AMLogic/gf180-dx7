"""Issues #86/#98: exp_t3/t4/t5 term-width range proof (fast lane).

Runs the unconditional static bound (tools/exp_range_proof.py --mode static)
on the committed RTL and keeps its failure controls live: deliberately
mis-sized selects / narrowed wires in an in-memory scratch copy must FAIL,
and an unrecognised write form must refuse (NOT_RUN), never PASS.
The exhaustive sweep and the simulator cross-checks are evidence runs, not
fast-lane tests (see docs/EXP-RANGE-PROOF-86.md).

Also carries the DR-0012 structural guard (issue #98): every `exp_hsum` read
must be inside the declared width of its wire.  At the DR-0011 pin three of
them were not, and nothing in the simulation-only gate could see it -- yosys
turned the missing bits into undef, folded the whole adder to x and then
zeroed it, deleting the exp() datapath from the mapped netlist while every
simulator stayed bit-exact.  This test fails on any revision that
reintroduces such a read, without needing yosys or a simulator.
"""

import re
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))

import exp_range_proof as erp  # noqa: E402

RTL = (REPO / "rtl" / "dx7_core.v").read_text()


def _terms(res):
    return {t["term"]: t for t in res["terms"]}


def _fails(res):
    bad = [t for t in res["terms"] if not t["exact_product_fits_wire"]]
    trunc = [w for s in res["chain"] for w in s["writes"] if w.get("src_product_truncated")]
    return bool(bad or trunc)


class ExpRangeProofStatic(unittest.TestCase):
    def test_pinned_rtl_passes_with_expected_bounds(self):
        res = erp.static_proof(RTL)
        self.assertFalse(_fails(res))
        t = _terms(res)
        self.assertEqual(t["exp_t3"]["exact_product_max_bits"], 68)
        self.assertEqual(t["exp_t4"]["exact_product_max_bits"], 60)
        self.assertEqual(t["exp_t5"]["exact_product_max_bits"], 52)
        # DR-0012 (#98): the wires now cover their reads, so the static
        # proof has no out-of-range range left to report.
        self.assertIsNone(t["exp_t3"]["out_of_range_bits"])
        self.assertIsNone(t["exp_t4"]["out_of_range_bits"])
        self.assertIsNone(t["exp_t5"]["out_of_range_bits"])
        self.assertEqual(t["exp_t3"]["declared_width"], 86)
        self.assertEqual(t["exp_t4"]["declared_width"], 86)
        self.assertEqual(t["exp_t5"]["declared_width"], 82)
        self.assertEqual(res["bounds_bits"],
                         {"exp_xq": 50, "exp_p2": 44, "exp_p3": 38,
                          "exp_p4": 32, "exp_p5": 26})

    def _mutant(self, old, new):
        self.assertEqual(RTL.count(old), 1, old)
        return RTL.replace(old, new)

    def test_control_p3_slice_widened_fails(self):
        res = erp.static_proof(self._mutant("exp_p3 <= exp_p3w[99:56];",
                                            "exp_p3 <= exp_p3w[99:50];"))
        self.assertTrue(_fails(res))
        # The proof reacts: exp_t3's exact product goes 68 -> 74 bits, past
        # the DR-0011 71-bit wire.  DR-0012's 86-bit wire absorbs that, so
        # what fails here is the truncation this mutation causes downstream
        # (exp_p4w = exp_p3*exp_xq no longer fits its 88-bit wire).  Both
        # readings are recorded so a later width change cannot quietly turn
        # this control green.
        self.assertEqual(_terms(res)["exp_t3"]["exact_product_max_bits"], 74)
        self.assertTrue(any(w.get("src_product_truncated")
                            for s in res["chain"] for w in s["writes"]))

    def test_control_xq_slice_widened_fails(self):
        res = erp.static_proof(self._mutant("exp_xq <= exp_xp[109:60];",
                                            "exp_xq <= exp_xp[109:50];"))
        self.assertTrue(_fails(res))

    def test_control_t3_wire_narrowed_fails(self):
        res = erp.static_proof(self._mutant("wire [85:0]  exp_t3 ",
                                            "wire [63:0]  exp_t3 "))
        self.assertFalse(_terms(res)["exp_t3"]["exact_product_fits_wire"])

    def test_control_unrecognised_write_refuses(self):
        src = self._mutant("exp_p3 <= exp_p3w[99:56];",
                           "exp_p3 <= exp_p3w[99:56] | (44'd1 << 42);")
        with self.assertRaises(erp.ParseError):
            erp.static_proof(src)

    def test_sampled_pipeline_within_static_bound(self):
        res = erp.static_proof(RTL)
        sw = erp.sweep(stride=65537)
        t = _terms(res)
        for k in ("t3", "t4", "t5"):
            self.assertLessEqual(sw["max"][k + "_full"],
                                 t["exp_" + k]["exact_product_max"])

    def test_sweep_control_mutation_fails(self):
        sw = erp.sweep(mutate_bit=42, stride=65537)
        self.assertGreater(sw["max_bits"]["t3_full"], 71)


# Declaration of a plain unsigned vector wire/reg: `wire [HI:0] name`.
DECL_RE = re.compile(r"\b(?:wire|reg)\s*\[(\d+):0\]\s+([A-Za-z_]\w*)")
# A part-select read: `name[HI:LO]`.
SELECT_RE = re.compile(r"\b([A-Za-z_]\w*)\[(\d+):(\d+)\]")


def _oor_selects(rtl: str) -> list[tuple[str, int, int]]:
    """Every `name[hi:lo]` in the file whose `hi` exceeds the declared width
    of `name`.  Names declared more than once, or not declared as a plain
    `[HI:0]` vector, are skipped -- the guard only judges what it can read
    unambiguously."""
    seen: dict[str, int | None] = {}
    for hi, name in DECL_RE.findall(rtl):
        seen[name] = None if name in seen else int(hi)
    bad = []
    for name, hi, lo in SELECT_RE.findall(rtl):
        decl = seen.get(name)
        if decl is not None and int(hi) > decl:
            bad.append((name, int(hi), decl))
    return sorted(set(bad))


class NoOutOfRangeSelects(unittest.TestCase):
    """DR-0012 structural guard (#98).

    A read past a wire's declared width is undef to the yosys frontend; its
    4-state `+` then folds the containing adder to all-x, and `setundef
    -zero` makes it the constant 0.  The whole exp() datapath disappeared
    from the mapped netlist that way while every simulator gate stayed
    green (#86).  Simulation alone cannot catch this class, so the guard is
    textual and runs in the fast lane with no tools.
    """

    def test_core_has_no_out_of_range_part_selects(self):
        self.assertEqual(_oor_selects(RTL), [])

    def test_guard_catches_the_dr0011_narrowing(self):
        """Live negative control: re-narrowing the three term wires to their
        DR-0011 declarations must make this guard fail."""
        src = RTL
        for old, new in (("wire [85:0]  exp_t3 ", "wire [70:0]  exp_t3 "),
                         ("wire [85:0]  exp_t4 ", "wire [70:0]  exp_t4 "),
                         ("wire [81:0]  exp_t5 ", "wire [65:0]  exp_t5 ")):
            self.assertEqual(src.count(old), 1, old)
            src = src.replace(old, new)
        self.assertEqual(_oor_selects(src),
                         [("exp_t3", 85, 70), ("exp_t4", 85, 70),
                          ("exp_t5", 81, 65)])

    def test_guard_catches_an_unrelated_narrowing(self):
        """The guard is not special-cased to exp_t3/4/5: narrowing another
        wire that is part-selected must also be caught."""
        old, new = "wire [119:0] exp_zp ", "wire [100:0] exp_zp "
        self.assertEqual(RTL.count(old), 1, old)
        self.assertEqual(_oor_selects(RTL.replace(old, new)),
                         [("exp_zp", 118, 100)])


if __name__ == "__main__":
    unittest.main()
