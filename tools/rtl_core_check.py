#!/usr/bin/env python3
"""rtl-core capability check: four per-module RTL benches (issue #275).

Runs the four registered comparison drivers -- H01 operator, H04 envelope,
H05 routing/feedback, H06 pitch/modulation -- each into its own scratch
output directory, then reads each fresh compare_report.json back (vector
generation alone cannot grade an RTL run) and prints one compact summary
line per module: bench id, expected and actual counts, mismatches, and
the first divergence when present. The summary block is printed LAST so
the capability stamp's bounded output tail (last 2,000 characters)
retains every module's result even though the drivers' own output is
verbose.

Verdict composition (issue #275 acceptance):
  * a bench that measured a mismatch (driver exit 1 / report FAIL)
    makes the whole check FAIL, even when another bench skipped;
  * otherwise a bench that could not run (driver exit 2 -- e.g. a
    missing iverilog/vvp -- or a report without its required
    equivalence/conformance section) makes the whole check NOT_RUN,
    naming the bench and its reason;
  * exit 0 only when all four benches executed exact comparisons with
    their detected controls.

The drivers run WITHOUT --skip-mutants/--skip-mutate, so every bench
negative control stays live -- including the H05 feedback-delay mutant
and the operator-order mutant the registered control stamps separately.

The combined output carries the token GUARDED_SKIP exactly when at least
one bench did not run, and never otherwise (guarded-skip contract,
issues #232/#274); the drivers' own COULD-NOT-RUN wording never
fabricates it.

Exit codes: 0 = four exact benches; 1 = measured mismatch or a bench
control not met; 2 = could not run (missing tool / skipped required
bench). Stdlib only.
"""

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# One entry per module bench: the driver, the module label, and the
# report section that MUST be present for the bench to have done its
# required work (H01/H04/H06 report equivalence rows; H05 reports
# conformance records).
BENCHES = (
    {"id": "h01", "module": "operator",
     "tool": "tools/h01_compare.py", "section": "equivalence"},
    {"id": "h04", "module": "envelope",
     "tool": "tools/h04_compare.py", "section": "equivalence"},
    {"id": "h05", "module": "routing/feedback",
     "tool": "tools/h05_compare.py", "section": "conformance"},
    {"id": "h06", "module": "pitch/modulation",
     "tool": "tools/h06_compare.py", "section": "equivalence"},
)

PASS = "PASS"
FAIL = "FAIL"
NOT_RUN = "NOT_RUN"
SKIP_TOKEN = "GUARDED_SKIP"
DEFAULT_SCRATCH = os.path.join("build", "capabilities", "rtl-core")
FIRST_DIV_MAX = 240


def emit_guarded_skip(count: int, bench_ids) -> None:
    """Emit GUARDED_SKIP exactly when benches did not run (issue #275).

    The capability registry declares GUARDED_SKIP as this tool's
    guarded-skip token: it is printed if and only if at least one
    required bench reported NOT_RUN, so a fully executed run can never
    fabricate a skip and a COULD-NOT-RUN wording can never hide one.
    """
    if count > 0:
        names = ", ".join(bench_ids)
        print(f"{SKIP_TOKEN}: {count} bench(es) did not run "
              f"(NOT_RUN benches are never passes): {names}")


def could_not_run_reason(text: str) -> str | None:
    """The driver's own COULD-NOT-RUN line, if it emitted one."""
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("COULD-NOT-RUN:"):
            return line
    return None


def _short_status(status: object) -> str:
    """First token of a status like 'MET (mutant fails ...)' -> 'MET'."""
    return str(status).split(" (")[0].strip()


def controls_note(report: dict) -> str:
    """Compact detected-control statuses for the summary line."""
    notes = []
    nc = report.get("negative_control")
    if isinstance(nc, dict):
        notes.append("control=" + _short_status(nc.get("status", "?")))
    for key in ("negative_controls", "mutants"):
        section = report.get(key)
        if isinstance(section, dict) and section:
            notes.append(key + "=" + ",".join(
                f"{name}:{_short_status(entry.get('status', '?'))}"
                for name, entry in sorted(section.items())))
    return " ".join(notes)


def first_divergence(section: dict) -> str | None:
    """Compact, bounded rendering of the section's first mismatch."""
    first = section.get("first_mismatch")
    if not first:
        return None
    compact = json.dumps(first, sort_keys=True, separators=(",", ":"))
    if len(compact) > FIRST_DIV_MAX:
        compact = compact[:FIRST_DIV_MAX] + "..."
    return compact


def summarize(bench: dict, exit_code: int, text: str,
              report: dict | None) -> dict:
    """Normalize one bench run into the compact per-module summary.

    The verdict comes from the driver's exit code cross-checked against
    the report it wrote: exit 2 is NOT_RUN (the reason names the missing
    work), a report without its required section is a skipped bench
    (NOT_RUN), and only an exit-0 report whose section agrees exactly is
    PASS. Everything else -- a measured mismatch, a control not met, an
    inconsistent report -- is FAIL, never silently absorbed.
    """
    summary = {
        "bench": bench["id"],
        "module": bench["module"],
        "verdict": FAIL,
        "expected": None,
        "actual": None,
        "mismatches": None,
        "first_divergence": None,
        "reason": None,
    }
    if exit_code == 2:
        summary["verdict"] = NOT_RUN
        summary["reason"] = (
            could_not_run_reason(text)
            or "driver exited 2 without a COULD-NOT-RUN line"
        )
        return summary
    if report is None:
        if exit_code == 0:
            # Exit 0 without a report is a bench that skipped its
            # required work (e.g. a --gen-only invocation), never a pass.
            summary["verdict"] = NOT_RUN
            summary["reason"] = (
                "skipped required bench: no compare_report.json written"
            )
        else:
            # A nonzero, non-2 exit without a report is a crash, not a
            # guarded skip: it reads FAIL so a broken driver can never
            # hide behind the NOT_RUN path.
            summary["reason"] = (
                f"driver exited {exit_code} without writing "
                f"compare_report.json"
            )
        return summary
    section = report.get(bench["section"])
    if not isinstance(section, dict):
        summary["verdict"] = NOT_RUN
        summary["reason"] = (
            f"skipped required bench: report has no "
            f"'{bench['section']}' section"
        )
        return summary
    expected = section.get("expected_rows", section.get("expected_records"))
    actual = section.get("actual_rows", section.get("actual_records"))
    mismatches = section.get("mismatches", section.get("mismatch_records"))
    summary.update(
        expected=expected, actual=actual, mismatches=mismatches,
        first_divergence=first_divergence(section),
    )
    exact = (
        exit_code == 0
        and report.get("status") == PASS
        and section.get("status") == PASS
        and mismatches == 0
    )
    if exact:
        summary["verdict"] = PASS
    else:
        summary["verdict"] = FAIL
        if section.get("status") != PASS or mismatches:
            summary["reason"] = (
                f"{bench['section']} status="
                f"{section.get('status')}, mismatches={mismatches}"
            )
        else:
            summary["reason"] = (
                f"driver exit {exit_code}, report status="
                f"{report.get('status')}"
            )
    return summary


def summary_line(summary: dict, report: dict | None) -> str:
    """One compact line per module, retained in the stamp's output tail."""
    label = f"rtl-core {summary['bench']} {summary['module']}"
    verdict = summary["verdict"]
    if verdict == NOT_RUN:
        return f"{label}: NOT_RUN reason={summary['reason']}"
    parts = [
        f"expected={summary['expected']}",
        f"actual={summary['actual']}",
        f"mismatches={summary['mismatches']}",
    ]
    if summary["first_divergence"]:
        parts.append("first_divergence=" + summary["first_divergence"])
    note = controls_note(report) if report is not None else ""
    if note:
        parts.append(note)
    if verdict == FAIL and summary["reason"]:
        parts.append(f"reason={summary['reason']}")
    return f"{label}: {verdict} " + " ".join(parts)


def run_bench(bench: dict, outdir: str, iverilog: str, vvp: str):
    """Run one driver into its scratch dir; echo and return its output."""
    argv = [
        sys.executable, os.path.join(REPO_ROOT, bench["tool"]),
        "--outdir", outdir,
        "--iverilog", iverilog,
        "--vvp", vvp,
    ]
    print(f"[rtl-core] $ {' '.join(argv)}", flush=True)
    completed = subprocess.run(
        argv, cwd=REPO_ROOT, capture_output=True, text=True, errors="replace"
    )
    text = completed.stdout + completed.stderr
    sys.stdout.write(text)
    if text and not text.endswith("\n"):
        sys.stdout.write("\n")
    sys.stdout.flush()
    return completed.returncode, text


def read_report(outdir: str) -> dict | None:
    path = os.path.join(outdir, "compare_report.json")
    try:
        with open(path, "r", encoding="utf-8") as f:
            report = json.load(f)
    except (OSError, ValueError) as exc:
        print(f"[rtl-core] unreadable {path}: {exc}", file=sys.stderr)
        return None
    return report if isinstance(report, dict) else None


def compose(summaries: list[dict]) -> str:
    """Aggregate verdict: a measured mismatch outranks a skip (issue #275)."""
    if any(s["verdict"] == FAIL for s in summaries):
        return FAIL
    if any(s["verdict"] == NOT_RUN for s in summaries):
        return NOT_RUN
    return PASS


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--scratch", default=DEFAULT_SCRATCH,
                    help="scratch root for per-bench vectors and reports "
                         "(default: %s)" % DEFAULT_SCRATCH)
    ap.add_argument("--iverilog", default="iverilog",
                    help="iverilog executable forwarded to every bench")
    ap.add_argument("--vvp", default="vvp",
                    help="vvp executable forwarded to every bench")
    args = ap.parse_args(argv)

    scratch = os.path.join(REPO_ROOT, args.scratch)
    summaries: list[dict] = []
    reports: list[dict | None] = []
    for bench in BENCHES:
        outdir = os.path.join(scratch, bench["id"])
        exit_code, text = run_bench(
            bench, outdir, args.iverilog, args.vvp)
        report = read_report(outdir)
        summaries.append(summarize(bench, exit_code, text, report))
        reports.append(report)

    shown = (
        os.path.relpath(scratch, REPO_ROOT)
        if os.path.commonpath([scratch, REPO_ROOT]) == REPO_ROOT
        else scratch
    )
    print()
    print(f"rtl-core summary ({len(summaries)} benches, scratch {shown}):")
    for summary, report in zip(summaries, reports):
        print(summary_line(summary, report))
    skipped = [s for s in summaries if s["verdict"] == NOT_RUN]
    if skipped:
        emit_guarded_skip(len(skipped), [s["bench"] for s in skipped])
    verdict = compose(summaries)
    print(f"rtl-core verdict: {verdict}")
    if verdict == FAIL:
        return 1
    if verdict == NOT_RUN:
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
