"""Shared iverilog/vvp invocation, result-parsing and CLI/report helpers
(issue #152, #300).

One owner for the `iverilog -V` probe, the compile / `vvp` run / FAIL and
PASS-summary check skeleton, the hex result-file parser and the compare
tools' CLI/report-write skeleton that used to be copied into the
H01/H04/H05/H06 compare tools. Each caller keeps its own defines,
plus-args, summary regex and report contents, and passes its own
`CouldNotRun` / `CheckFailure` classes where a run is invoked, so what a
tool raises is unchanged. The parser raises the shared
`verdict_errors.CheckFailure` (every caller imports the same class
object). Stdlib only.
"""

import argparse
import json
import os
import re
import shutil
import subprocess

from verdict_errors import CheckFailure


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


def _hex_width_ok(token, max_width):
    if max_width is None:
        return True
    if isinstance(max_width, int):
        return len(token) <= max_width
    return len(token) in max_width


def parse_hex_rows(path, ncols, max_width=None):
    """Parse the shared hex result-file format into a list of int tuples.

    Skips blank lines and `#` comment lines; every other line must split
    into exactly `ncols` hex tokens. `max_width` optionally constrains the
    token widths: an int rejects tokens longer than it, a tuple/set rejects
    tokens whose width is not listed (h01 allows widths 5 and 8, h04 bounds
    width at 8, h06 does not check). Raises `CheckFailure` with the shared
    message ``{path}:{lineno}: malformed result line ...`` on a malformed
    line.
    """
    rows = []
    with open(path, "r", encoding="ascii") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != ncols or any(
                    not _hex_width_ok(p, max_width) or
                    not re.fullmatch(r"[0-9a-fA-F]+", p) for p in parts):
                raise CheckFailure(
                    f"{path}:{lineno}: malformed result line {line!r}")
            rows.append(tuple(int(p, 16) for p in parts))
    return rows


def add_common_args(ap, outdir_default,
                    gen_only_help="write vectors + expected + gen report, "
                                  "no sim"):
    """Add the CLI flags shared by the H-probe compare tools to `ap`:
    `--outdir`, `--gen-only`, `--iverilog` and `--vvp`. Per-probe flags
    stay with each tool's `main()`; `gen_only_help` lets a tool describe
    its own --gen-only artifacts (h05 writes a program, not vectors)."""
    ap.add_argument("--outdir", default=outdir_default)
    ap.add_argument("--gen-only", action="store_true", help=gen_only_help)
    ap.add_argument("--iverilog", default="iverilog")
    ap.add_argument("--vvp", default="vvp")


def write_json_report(outdir, name, report):
    """Write `report` as `<outdir>/<name>` with the shared compare-tool
    JSON convention (indent=1, sort_keys=True, trailing newline) and
    return the path."""
    path = os.path.join(outdir, name)
    with open(path, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")
    return path
