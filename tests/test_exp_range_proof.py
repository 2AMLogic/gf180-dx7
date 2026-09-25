"""Issue #86: exp_t3/t4/t5 out-of-range-select range proof (fast lane).

Runs the unconditional static bound (tools/exp_range_proof.py --mode static)
on the committed RTL and keeps its failure controls live: deliberately
mis-sized selects / narrowed wires in an in-memory scratch copy must FAIL,
and an unrecognised write form must refuse (NOT_RUN), never PASS.
The exhaustive sweep and the simulator cross-checks are evidence runs, not
fast-lane tests (see docs/EXP-RANGE-PROOF-86.md).
"""

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
        self.assertEqual(t["exp_t3"]["out_of_range_bits"], "[85:71]")
        self.assertEqual(t["exp_t4"]["out_of_range_bits"], "[85:71]")
        self.assertEqual(t["exp_t5"]["out_of_range_bits"], "[81:66]")
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
        self.assertFalse(_terms(res)["exp_t3"]["exact_product_fits_wire"])

    def test_control_xq_slice_widened_fails(self):
        res = erp.static_proof(self._mutant("exp_xq <= exp_xp[109:60];",
                                            "exp_xq <= exp_xp[109:50];"))
        self.assertTrue(_fails(res))

    def test_control_t3_wire_narrowed_fails(self):
        res = erp.static_proof(self._mutant("wire [70:0]  exp_t3 ",
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


if __name__ == "__main__":
    unittest.main()
