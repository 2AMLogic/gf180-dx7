"""Shared yosys invocation helpers (issue #146).

One owner for the `yosys -V` probe and the `yosys -p` run / log-writing /
failure-raising tail that used to be copied into every H*/storage-probe
synth tool. Each caller keeps its own script construction and passes its own
`CouldNotRun` / `CheckFailure` classes, so what a tool raises is unchanged.
Stdlib only.
"""

import re
import subprocess


def yosys_version(yosys, could_not_run):
    """`yosys -V` stdout; raises `could_not_run` if it cannot be probed."""
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise could_not_run(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise could_not_run(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def run_yosys_script(yosys, script, log_path, cwd, failure):
    """Run `yosys -p "; ".join(script)` in `cwd`, write the transcript to
    `log_path` (first line records the script and cwd), and return the log.
    A nonzero exit raises `failure` naming the log and its last line."""
    joined = "; ".join(script)
    proc = subprocess.run([yosys, "-p", joined], capture_output=True,
                          text=True, cwd=cwd)
    log = (proc.stdout or "") + (proc.stderr or "")
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script (cwd={cwd}): {joined}\n" + log)
    if proc.returncode != 0:
        raise failure(
            f"yosys failed (rc={proc.returncode}); log at {log_path}: "
            + (log.strip().splitlines()[-1][:300] if log.strip() else ""))
    return log


def rom_representation(log, detail, readmem=False):
    """How table memories landed, classified from a yosys transcript.

    `detail` is the tool's own sentence naming its ROMs, used only when the
    MEMORY_MAP pass ran and no `$mem` cell survives in the final stat.
    `readmem=True` adds `readmem_seen` to the fallback evidence (H01/H05).
    """
    memory_map = "MEMORY_MAP pass (converting memories to logic" in log
    mem_cells = re.search(r"^\s+\d+\s+\S*\$mem\S*\s*$", log, re.M)
    if memory_map and not mem_cells:
        return {
            "representation": "logic",
            "detail": detail,
            "evidence": {"memory_map_pass_ran": True,
                         "memory_cells_in_final_stat": 0},
        }
    evidence = {"memory_map_pass_ran": memory_map}
    if readmem:
        evidence["readmem_seen"] = "readmem" in log.lower()
    evidence["memory_cells_in_final_stat"] = bool(mem_cells)
    return {
        "representation": "memory cells" if mem_cells else
                          ("logic (inferred)" if memory_map else "unknown"),
        "detail": "inspect the committed yosys log",
        "evidence": evidence,
    }
