"""Shared iverilog/vvp invocation helpers (issue #152).

One owner for the `iverilog -V` probe and the compile / `vvp` run / FAIL and
PASS-summary check skeleton that used to be copied into the H01/H04/H05/H06
compare tools. Each caller keeps its own defines, plus-args, summary regex and
result parsing, and passes its own `CouldNotRun` / `CheckFailure` classes and
message label, so what a tool raises is unchanged. Stdlib only.
"""

import os
import re
import shutil
import subprocess


def iverilog_version(iverilog, could_not_run):
    """First line of `iverilog -V`; raises `could_not_run` if it cannot be
    probed."""
    try:
        out = subprocess.run([iverilog, "-V"], capture_output=True,
                             text=True)
    except OSError:
        raise could_not_run(f"iverilog binary not found: {iverilog}")
    first = (out.stdout or out.stderr).strip().splitlines()
    if not first:
        raise could_not_run("iverilog -V produced no output")
    return first[0].strip()


def compile_and_run(iverilog, vvp, vvp_out, sources, defines, plusargs,
                    summary_re, cwd, timeout, could_not_run, failure,
                    label="", compile_tail=500, bench_tail=500,
                    summary_tail=300):
    """Compile `sources` to `vvp_out`, run it under `vvp` with `plusargs`,
    and return `(summary_match, output)`; `vvp_out` is removed afterwards.

    `label` (e.g. " (tag)") is appended to each failure message after the
    fixed phrase, and the `*_tail` arguments set how many trailing characters
    of output each message quotes. Raises `could_not_run` when a binary is not
    on PATH and `failure` on compile failure, nonzero exit / FAIL in the
    output, or a missing PASS summary."""
    if shutil.which(iverilog) is None:
        raise could_not_run(f"iverilog not found on PATH: {iverilog}")
    if shutil.which(vvp) is None:
        raise could_not_run(f"vvp not found on PATH: {vvp}")
    cmd = [iverilog, "-g2012", "-o", vvp_out] + list(defines) + list(sources)
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)
    if proc.returncode != 0:
        raise failure(f"iverilog compile failed{label}: "
                      + (proc.stderr or proc.stdout)[-compile_tail:])
    try:
        run = subprocess.run([vvp, vvp_out] + list(plusargs),
                             capture_output=True, text=True, cwd=cwd,
                             timeout=timeout)
        out = run.stdout + run.stderr
        if run.returncode != 0 or "FAIL" in out:
            raise failure(f"bench failed{label}: {out.strip()[-bench_tail:]}")
        m = re.search(summary_re, out)
        if not m:
            raise failure(
                f"bench produced no PASS summary{label}: {out[-summary_tail:]}")
    finally:
        try:
            os.remove(vvp_out)
        except OSError:
            pass
    return m, out
