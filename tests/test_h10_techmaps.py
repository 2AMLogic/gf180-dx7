#!/usr/bin/env python3
"""Hermetic negative control for tools/verify_gf180_7t_techmaps.py.

Builds a fake image platform file pair + a fake 7t liberty in a temp tree
and asserts:
  1. committed techmaps = header + substituted image bytes  -> PASS (exit 0)
  2. a cell referenced but absent from the liberty            -> FAIL (exit 1)
  3. a byte drift between the image transform and the committed
     techmap (an unrecorded edit)                              -> FAIL (exit 1)
  4. if a real 7t tt liberty is available on this machine, the
     committed asic/orfs/gf180_7t techmaps pass the lib check.
Fast lane (DR-0009): stdlib only, sub-second.
"""

import os
import subprocess
import sys
import tempfile
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOL = os.path.join(REPO_ROOT, "tools", "verify_gf180_7t_techmaps.py")
FAKE_IMAGE = """(* techmap_celltype = "$fa" *)
module _tech_fa (A, B, C, X, Y);
  gf180mcu_fd_sc_mcu9t5v0__full_1 fa (.A(A), .B(B), .C(C), .X(X), .Y(Y));
  gf180mcu_fd_sc_mcu9t5v0__addh_1 ha (.A(A), .B(B), .X(X), .Y(Y));
endmodule
"""
FAKE_HEADER = "// provenance header line\n" * 3
FAKE_LIB = """cell(gf180mcu_fd_sc_mcu7t5v0__full_1) {
  area: 10 ;
}
cell(gf180mcu_fd_sc_mcu7t5v0__addh_1) {
  area: 10 ;
}
"""


class TestTechmapVerifier(unittest.TestCase):
    def _mk(self, tmp, image=FAKE_IMAGE, committed=None, lib=FAKE_LIB):
        imgdir = os.path.join(tmp, "image")
        tm = os.path.join(tmp, "techmap")
        os.makedirs(imgdir)
        os.makedirs(tm)
        open(os.path.join(imgdir, "cells_adders.v"), "w").write(image)
        open(os.path.join(imgdir, "cells_latch.v"), "w").write(image)
        body = image.replace("mcu9t5v0", "mcu7t5v0")
        if committed is None:
            committed = body
        open(os.path.join(tm, "cells_adders.v"), "w").write(
            FAKE_HEADER + committed)
        open(os.path.join(tm, "cells_latch.v"), "w").write(
            FAKE_HEADER + committed)
        libp = os.path.join(tmp, "tt.lib")
        open(libp, "w").write(lib)
        return tm, imgdir, libp

    def _run(self, *args):
        return subprocess.run([sys.executable, TOOL, *args],
                              capture_output=True, text=True)

    def test_pass_on_exact_transform_and_complete_lib(self):
        with tempfile.TemporaryDirectory() as tmp:
            tm, img, libp = self._mk(tmp)
            r = self._run("--techmap-dir", tm, "--lib", libp,
                          "--image-dir", img)
            self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
            self.assertIn("RESULT: PASS", r.stdout)

    def test_fail_when_cell_missing_from_liberty(self):
        with tempfile.TemporaryDirectory() as tmp:
            lib = FAKE_LIB.replace(
                "gf180mcu_fd_sc_mcu7t5v0__full_1",
                "gf180mcu_fd_sc_mcu7t5v0__full_2")  # referenced cell gone
            tm, img, libp = self._mk(tmp, lib=lib)
            r = self._run("--techmap-dir", tm, "--lib", libp,
                          "--image-dir", img)
            self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
            self.assertIn("NOT in the 7t tt liberty", r.stdout)

    def test_fail_on_transform_drift(self):
        with tempfile.TemporaryDirectory() as tmp:
            drifted = FAKE_IMAGE.replace(
                "mcu9t5v0__full_1", "mcu9t5v0__full_4").replace(
                "mcu9t5v0__addh_1", "mcu9t5v0__addh_4")
            # committed files carry the ORIGINAL transform body (full_1/addh_1,
            # which pass the lib check); the image source has drifted.
            original_body = FAKE_IMAGE.replace("mcu9t5v0", "mcu7t5v0")
            tm, img, libp = self._mk(tmp, image=drifted,
                                     committed=original_body)
            r = self._run("--techmap-dir", tm, "--lib", libp,
                          "--image-dir", img)
            self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
            self.assertIn("byte drift or unrecorded edit", r.stdout)

    def test_real_pdk_lib_check(self):
        sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))
        from verify_gf180_7t_techmaps import DEFAULT_LIB_LOCATIONS as default
        libp = next((p for p in default if p and os.path.isfile(p)), None)
        if libp is None:
            self.skipTest("no local 7t tt liberty on this machine "
                          "(lib check skipped; ran hermetically on the fake "
                          "liberty)")
        tm = os.path.join(REPO_ROOT, "asic", "orfs", "gf180_7t")
        r = self._run("--techmap-dir", tm, "--lib", libp)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)