"""Shared diagnosis of a failed Verilator bench build (H07 / H08 live tests).

After a cold project-bench build fails, build a known-good, repository-
independent timed module with the same Verilator and the same environment
(inherited CXX / MAKEFLAGS) in a fresh temporary directory:

* smoke build FAILS    -> TOOLCHAIN: the C++ toolchain is unusable, so the
  live case is NOT_RUN (skip) and carries the compiler diagnostic.
* smoke build PASSES   -> PROJECT: the project bench is broken; FAIL with
  the original diagnostic.
* smoke could not run  -> INCONCLUSIVE: stays a hard FAIL, never a skip.
"""
import os
import shutil
import subprocess
import tempfile

TOOLCHAIN, PROJECT, INCONCLUSIVE = "TOOLCHAIN", "PROJECT", "INCONCLUSIVE"

SMOKE_V = """\
module smoke_top;
  initial begin
    #1;
    $display("SMOKE_OK");
    $finish;
  end
endmodule
"""


def _tail(text, n=1500):
    return (text or "")[-n:]


def diagnose(vlt, timeout=600, run=subprocess.run):
    """Return (verdict, message) from the independent Verilator smoke probe."""
    tmp = tempfile.mkdtemp(prefix="vlt-smoke-")
    try:
        src = os.path.join(tmp, "smoke_top.v")
        with open(src, "w") as f:
            f.write(SMOKE_V)
        mdir = os.path.join(tmp, "obj")
        cmd = [vlt, "--binary", "--timing", "--Wno-fatal",
               "--timescale-override", "1ns/1ps", "--Mdir", mdir,
               "-o", "smoke", "--top-module", "smoke_top", src]
        try:
            b = run(cmd, cwd=tmp, capture_output=True, text=True,
                    timeout=timeout)
        except (OSError, subprocess.TimeoutExpired) as e:
            return INCONCLUSIVE, f"smoke probe did not complete: {e!r}"
        exe = os.path.join(mdir, "smoke")
        if b.returncode != 0 or not os.path.exists(exe):
            return TOOLCHAIN, ("independent timed-module build also failed "
                               "(C++ toolchain unusable; try CXX=<compiler>"
                               " e.g. `make test CXX=clang++`): "
                               + _tail(b.stderr or b.stdout))
        try:
            r = run([exe], cwd=tmp, capture_output=True, text=True,
                    timeout=timeout)
        except (OSError, subprocess.TimeoutExpired) as e:
            return INCONCLUSIVE, f"smoke run did not complete: {e!r}"
        if r.returncode != 0 or "SMOKE_OK" not in (r.stdout or ""):
            return INCONCLUSIVE, ("smoke binary built but did not run "
                                  "cleanly: " + _tail(r.stdout + r.stderr))
        return PROJECT, ("independent timed-module build and run succeeded; "
                         "the project bench failure is real")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
