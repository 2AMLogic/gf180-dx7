#!/usr/bin/env python3
"""`klt functional-verification` evidence for the dx7_core bit-exact suite
(issue #150): regenerate, check, and negative-control the committed report.

Claim covered: (1) RTL == frozen integer model on the DR-0012 trio under
Icarus, over the declared frame prefix, via the H07 harness comparator.  It
says nothing about model-vs-reference agreement, synthesis, STA, or sound.

Modes
  check (default)  re-run the request into a scratch dir and compare the
                   verdicts and RTL hashes with evidence/t1-functional/
                   report.json.  Wall-clock fields and absolute paths are
                   not compared.
  --write          re-run and (re)write the committed report.
  --control NAME   run the same request against the -DNAME mutant build on
                   the cases that mutant must break and require a SAMPLE
                   MISMATCH there; writes evidence/t1-functional/
                   control-NAME.json with --write.

Statuses and exit codes
  PASS 0   re-run matches the committed report, every selected test passed
  FAIL 1   a verdict, count, or recorded environment differs, or a selected
           case failed at the comparator
  NOT_RUN / BLOCKED 2   klt, cocotb or iverilog is missing, or a selected
           case did not execute (never a pass)
  STALE 3  the committed report names different RTL hashes than the tree

Capability registration (issue #276).  Check mode is the registered
`rtl-exact-v1` command; it never writes the committed report.  A cocotb
`failed` also covers setup failures (the driver's "could not run"), so the
verdicts are composed per case: a measured comparator failure is FAIL even
when other cases did not execute, a selected case that did not execute is
NOT_RUN naming it, and the driver's STALE diagnosis for a stale pin outranks
both.  Check-mode output ends with a compact coverage record -- one token
per selected case (id, verdict, checked samples, first divergence when it
failed) and the dev cases this lane never grades -- printed LAST so the
capability stamp's bounded output tail (2,000 characters) retains all of it;
parse_coverage() recovers it.  The token GUARDED_SKIP is emitted exactly
when a dev or stress case did not execute in this lane (never graded, not
executed, or a missing tool) and never otherwise: the registry declares it
as this tool's guarded-skip contract, so a partial run (23 of 34 cases) is
stamped NOT_RUN under skip_policy not_run while FAIL and STALE stay visible.

The pinned toolchain is `klayout-tools==0.8.0` + `cocotb==2.1.0` in a venv
(`make functional-verification` creates .venv-fv inside the checkout); no
host tool is installed or changed.
"""

from __future__ import annotations

import argparse
import ast
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))

from sha256_helpers import sha256_file  # noqa: E402

EVID = REPO / "evidence" / "t1-functional"
REQUEST = EVID / "request.json"
REPORT = EVID / "report.json"
DR12 = REPO / "docs" / "decision-records" / "0012-exp-term-width-core-refreeze.md"
RTL_PIN = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]
RTL_ALL = RTL_PIN + ["rtl/tb_dx7_core.v"]
# Icarus plays ~2.4 s of wall clock per simulated 64-sample frame, and the
# dev cases' events reach block 1125 (dev32-*: ~45 min each).  The graded
# set is therefore the dev cases whose whole event trace lies at or before
# MAX_EVENT_BLOCK, compared over (last event block + TAIL_FRAMES) golden
# blocks so the compared window contains audible samples; the rest are
# reported NOT_RUN in provenance.not_graded (never PASS).  They remain
# covered by the Verilator/remote runs under evidence/h07-core/.
MAX_EVENT_BLOCK = 75
TAIL_FRAMES = 16

# The guarded-skip token declared for this tool in the capability registry
# (src/gf180_dx7/capabilities.py CHECKS["rtl-exact-v1"], issue #276).  It is
# printed if and only if check mode did not execute every dev and stress
# case, so a partial run can never read PASS under skip_policy not_run and
# an incidental NOT_RUN wording can never fabricate or hide a skip.
SKIP_TOKEN = "GUARDED_SKIP"
COVERAGE_MARK = "rtl-exact coverage"
# tools/fv_cocotb_dx7_core.py raises "<case>: could not run: <error>" when
# the H07 driver could not play or compare a case at all.
COULD_NOT_RUN_RE = re.compile(r": could not run: ")
# "... RTL != frozen model; first mismatches [{'index': 849, 'golden': 23300,
# 'actual': 23316, 'block': 11, 'sample': 17}] overrun=0 overflow=0"
MISMATCHES_RE = re.compile(r"first mismatches (\[.*?\]) overrun=", re.S)
OVERRUN_RE = re.compile(r"overrun=\d+ overflow=\d+")

# mutant build -> cases it must break (tests/test_h07.py keeps the same
# mutants live against small vectors; here they run through the klt request).
# H07_MUTATE_TICK_SKEW is NOT here: it only drops the last write of a
# 167-write single-window burst, which no case in the graded set produces
# (measured: stress-burst and stress-sum16 both still PASS under it), so a
# klt control for it would be a defect.  Only tests/test_h07.py's small-burst
# vector keeps it live.
CONTROLS = {
    "H07_MUTATE_GLITCH": ["stress-sum16"],   # needs a sounding voice at frame 12
    "H07_MUTATE_SUM_ORDER": ["stress-sum16"],
}

# compared on both sides; everything else in the klt envelope is wall-clock
# or machine-local
ENV_KEYS = ("engine", "engine_version", "cocotb_version", "random_seed")

EXIT = {"PASS": 0, "FAIL": 1, "NOT_RUN": 2, "BLOCKED": 2, "STALE": 3}


def last_event_block(case: dict) -> int:
    return max((t["block"] for t in case["event_trace"]), default=0)


def split_dev_cases(cases: list) -> tuple:
    """-> (graded, not_graded) dev cases."""
    graded = [c for c in cases if last_event_block(c) <= MAX_EVENT_BLOCK]
    rest = [c for c in cases if last_event_block(c) > MAX_EVENT_BLOCK]
    return graded, rest


def dev_frames(case: dict) -> int:
    return last_event_block(case) + TAIL_FRAMES


def test_name(case_id: str) -> str:
    """The cocotb test name of a case (tools/fv_cocotb_dx7_core.py)."""
    return "case_" + re.sub(r"[^0-9A-Za-z]", "_", case_id)


def lane_cases() -> tuple:
    """-> (case ids this lane selects, dev case ids it never grades)."""
    import h07_compare as H
    graded, rest = split_dev_cases(H.load_dev_cases())
    return ([c["id"] for c in graded] + [s["id"] for s in H.STRESS_SET[1]],
            sorted(c["id"] for c in rest))


def rtl_hashes(root: Path = REPO) -> dict:
    return {rel: sha256_file(root / rel) for rel in RTL_ALL}


def dr12_pin(text: str) -> dict:
    """The DR-0012 pin table: rtl path -> sha256."""
    return dict(re.findall(r"\|\s*`(rtl/[a-z0-9_]+\.v)`\s*\|\s*`([0-9a-f]{64})`",
                           text))


def normalize(report: dict) -> dict:
    """The comparable core of a report: verdicts, counts, recorded
    environment, provenance.  No timing, no paths."""
    env = report.get("environment", {})
    return {
        "status": report.get("status"),
        "counts": [report.get(k) for k in
                   ("test_count", "passed_count", "failed_count",
                    "skipped_count")],
        "tests": sorted((t["name"], t["status"])
                        for t in report.get("tests", [])),
        "environment": {k: env.get(k) for k in ENV_KEYS},
        "provenance": report.get("provenance", {}),
    }


def compare(committed: dict, fresh: dict, current_hashes: dict) -> tuple:
    """-> (status, [reasons]).  STALE wins over a re-run: a report naming
    other RTL than the tree is not evidence for the tree, whatever a
    re-run says."""
    named = committed.get("provenance", {}).get("rtl_sha256")
    if named != current_hashes:
        diff = sorted(k for k in set(named or {}) | set(current_hashes)
                      if (named or {}).get(k) != current_hashes.get(k))
        return "STALE", [f"committed report names different RTL: {diff}"]
    if fresh.get("status") != "pass":
        bad = [t["name"] for t in fresh.get("tests", [])
               if t["status"] != "passed"]
        return "FAIL", [f"re-run status {fresh.get('status')}: {bad}"]
    a, b = normalize(committed), normalize(fresh)
    if a == b:
        return "PASS", []
    reasons = [f"{k}: committed={a[k]!r} rerun={b[k]!r}"
               for k in a if a[k] != b[k] and k != "tests"]
    ta, tb = dict(a["tests"]), dict(b["tests"])
    for name in sorted(set(ta) | set(tb)):
        if ta.get(name) != tb.get(name):
            reasons.append(f"test {name}: committed={ta.get(name)} "
                           f"rerun={tb.get(name)}")
    return "FAIL", reasons


def first_divergence(error_message: str) -> dict:
    """The comparator's first sample mismatch from a failed test's message,
    or None unless the failure IS a sample mismatch (the driver's
    ``RTL != frozen model; first mismatches [...]`` assertion)."""
    found = MISMATCHES_RE.search(error_message or "")
    if not found:
        return None
    try:
        mismatches = ast.literal_eval(found.group(1))
    except (ValueError, SyntaxError):
        return None
    if (isinstance(mismatches, list) and mismatches
            and isinstance(mismatches[0], dict)
            and {"index", "golden", "actual"} <= set(mismatches[0])):
        return mismatches[0]
    return None


def divergence_note(divergence: dict) -> str:
    """Compact rendering of one sample mismatch."""
    note = (f"index={divergence['index']} golden={divergence['golden']}"
            f" actual={divergence['actual']}")
    if "block" in divergence and "sample" in divergence:
        note += f" block={divergence['block']} sample={divergence['sample']}"
    return note


def control_verdict(report: dict, expected_cases: list) -> tuple:
    """A negative control passes only if the mutant makes every selected
    test fail AT THE SAMPLE COMPARATOR (and at least one ran).  A cocotb
    ``failed`` alone proves nothing: a setup failure (build or playback
    error) fails the test the same way, so a failure without a sample
    mismatch is a control that did not reach the comparator (issue #276)."""
    tests = {t["name"]: t for t in report.get("tests", [])}
    want = {test_name(c) for c in expected_cases}
    if set(tests) != want:
        return False, f"ran {sorted(tests)} but expected {sorted(want)}"
    statuses = {name: t["status"] for name, t in tests.items()}
    if any(status != "failed" for status in statuses.values()):
        return False, f"mutant did NOT fail: {statuses}"
    divergences = {
        name: first_divergence(t.get("error_message"))
        for name, t in tests.items()
    }
    unreached = sorted(n for n, d in divergences.items() if d is None)
    if unreached:
        return False, ("no sample mismatch, the mutant did not reach the"
                       " comparator: " + ", ".join(unreached))
    return True, ("mutant fails every selected case at the sample comparator;"
                  " first divergence "
                  + "; ".join(f"{n} {divergence_note(d)}"
                              for n, d in sorted(divergences.items())))


def case_rows(fresh: dict, selected: list) -> list:
    """One coverage row per selected case of a fresh run.

    verdict PASS: the comparator found the whole window bit-exact (checked =
    compared samples); FAIL: a measured comparator failure (divergence = the
    first sample mismatch when there was one, else the overrun/overflow
    note); NOT_RUN: the case did not execute (absent from the run, skipped,
    or the driver's "could not run").  Zero simulator skips cannot establish
    full coverage: an absent case is NOT_RUN here whatever klt counted.
    """
    tests = {t["name"]: t for t in fresh.get("tests", [])}
    checked = (fresh.get("provenance") or {}).get("cases") or {}
    rows = []
    for cid in selected:
        row = {"case": cid, "verdict": "NOT_RUN", "checked": None,
               "divergence": None, "detail": None}
        test = tests.get(test_name(cid))
        if test is None:
            row["detail"] = "absent from the run"
        else:
            status = test.get("status")
            message = test.get("error_message") or ""
            if status == "passed":
                row["verdict"] = "PASS"
                row["checked"] = (checked.get(cid) or {}).get("checked_samples")
            elif status == "failed" and not COULD_NOT_RUN_RE.search(message):
                row["verdict"] = "FAIL"
                row["divergence"] = first_divergence(message)
                if row["divergence"] is None:
                    overrun = OVERRUN_RE.search(message)
                    row["detail"] = (
                        overrun.group(0) if overrun
                        else "failed without a sample mismatch"
                    )
            else:
                row["detail"] = (
                    "could not run" if status == "failed"
                    else f"status {status}"
                )
        rows.append(row)
    return rows


def check_verdict(committed: dict, fresh: dict, current_hashes: dict,
                  selected: list) -> tuple:
    """-> (status, [reasons], rows) of check mode.

    STALE wins over a re-run (compare()'s rule: a report naming other RTL
    than the tree is not evidence for the tree).  Then a measured comparator
    failure is FAIL even when other cases did not execute or stay ungraded;
    only when nothing failed does a selected case that did not execute read
    NOT_RUN, naming it.  Otherwise compare()'s PASS/FAIL stands.
    """
    status, reasons = compare(committed, fresh, current_hashes)
    rows = case_rows(fresh, selected)
    if status == "STALE":
        return status, reasons, rows
    measured = [r for r in rows if r["verdict"] == "FAIL"]
    if measured:
        detail = [
            f"{r['case']}: "
            + (f"first divergence {divergence_note(r['divergence'])}"
               if r["divergence"] else r["detail"])
            for r in measured
        ]
        return "FAIL", reasons + detail, rows
    unexecuted = [r for r in rows if r["verdict"] == "NOT_RUN"]
    if unexecuted:
        return "NOT_RUN", [
            "selected case(s) did not execute: "
            + "; ".join(f"{r['case']} ({r['detail']})" for r in unexecuted)
        ], rows
    return status, reasons, rows


def row_token(row: dict) -> str:
    if row["verdict"] == "PASS":
        return f"{row['case']}:PASS:{row['checked']}"
    if row["verdict"] == "FAIL":
        detail = (divergence_note(row["divergence"]) if row["divergence"]
                  else row["detail"])
    else:
        detail = row["detail"]
    return f"{row['case']}:{row['verdict']}[{detail}]"


def coverage_lines(status: str, rows: list, ungraded: list,
                   reason: str) -> list:
    """The compact coverage record, printed LAST so the capability stamp's
    bounded output tail retains all of it: verdict, graded and ungraded
    counts and total checked samples; one token per selected case (id,
    verdict, checked samples, first divergence on FAIL); the ungraded ids
    with their reason."""
    total = sum(r["checked"] for r in rows if r["checked"])
    lines = [
        f"{COVERAGE_MARK}: verdict={status} graded={len(rows)}"
        f" ungraded={len(ungraded)} checked_samples={total}",
        f"{COVERAGE_MARK} graded: " + " ".join(row_token(r) for r in rows),
    ]
    if ungraded:
        lines.append(f"{COVERAGE_MARK} ungraded reason: {reason}")
        lines.append(f"{COVERAGE_MARK} ungraded: " + " ".join(ungraded))
    return lines


_COVERAGE_HEADER_RE = re.compile(
    r"^rtl-exact coverage: verdict=(\S+) graded=(\d+) ungraded=(\d+)"
    r" checked_samples=(\d+)$", re.M)
_COVERAGE_GRADED_RE = re.compile(r"^rtl-exact coverage graded: (.*)$", re.M)
_COVERAGE_UNGRADED_RE = re.compile(r"^rtl-exact coverage ungraded: (.*)$", re.M)
_COVERAGE_REASON_RE = re.compile(r"^rtl-exact coverage ungraded reason: (.*)$",
                                 re.M)
# One row token: an id, then PASS with the checked count, or a bracketed
# detail (which may contain spaces) for FAIL / NOT_RUN.
_ROW_RE = re.compile(
    r"(\S+?):(?:PASS:(\d+)|FAIL\[([^\]]*)\]|NOT_RUN\[([^\]]*)\])")


def parse_coverage(text: str) -> dict:
    """Recover the coverage record from captured check-mode output, or
    None when its header is absent (a truncated tail or a run that never
    reached the record).  Mirrors coverage_lines(): header counts, one row
    per selected case ({case, verdict, checked, detail}) and the ungraded
    ids with their reason."""
    header = _COVERAGE_HEADER_RE.search(text)
    graded = _COVERAGE_GRADED_RE.search(text)
    if not header or not graded:
        return None
    rows = []
    # finditer + groups(): a non-participating alternative is None here,
    # where findall would hand back an indistinguishable empty string.
    for match in _ROW_RE.finditer(graded.group(1)):
        cid, checked, fail, not_run = match.groups()
        if checked:
            row = {"verdict": "PASS", "checked": int(checked), "detail": None}
        elif fail is not None:
            row = {"verdict": "FAIL", "checked": None, "detail": fail}
        else:
            row = {"verdict": "NOT_RUN", "checked": None, "detail": not_run}
        row["case"] = cid
        rows.append(row)
    ungraded = _COVERAGE_UNGRADED_RE.search(text)
    reason = _COVERAGE_REASON_RE.search(text)
    return {
        "verdict": header.group(1),
        "graded_count": int(header.group(2)),
        "ungraded_count": int(header.group(3)),
        "checked_samples": int(header.group(4)),
        "rows": rows,
        "ungraded": ungraded.group(1).split() if ungraded else [],
        "ungraded_reason": reason.group(1) if reason else None,
    }


def emit_guarded_skip(note: str) -> None:
    """Emit GUARDED_SKIP with the reason work did not run (issue #276).

    The capability registry declares GUARDED_SKIP as this tool's
    guarded-skip token: the stamp records a skip only when it appears, so
    it is printed exactly when check mode did not execute every case.
    """
    print(f"{SKIP_TOKEN}: {note}")


def skip_note(rows: list, ungraded: list) -> str:
    """Why check mode did not execute every case, or None when it did."""
    unexecuted = [r["case"] for r in rows if r["verdict"] == "NOT_RUN"]
    if not unexecuted and not ungraded:
        return None
    parts = []
    if unexecuted:
        parts.append("selected but not executed: " + ", ".join(unexecuted))
    if ungraded:
        parts.append(f"never graded, events past block {MAX_EVENT_BLOCK}: "
                     + ", ".join(ungraded))
    return (f"{len(unexecuted) + len(ungraded)} case(s) NOT_RUN in this lane"
            " (never passes); " + "; ".join(parts))


def tooling(venv: Path) -> tuple:
    """-> (klt path | None, why-not)."""
    klt = venv / "bin" / "klt"
    if not klt.exists():
        return None, f"{klt} missing (run `make functional-verification`)"
    for b in ("iverilog", "vvp"):
        if shutil.which(b) is None:
            return None, f"{b} not found on PATH"
    probe = subprocess.run([str(venv / "bin" / "python"), "-c",
                            "import cocotb"], capture_output=True)
    if probe.returncode != 0:
        return None, f"cocotb not importable in {venv}"
    return klt, ""


def run_klt(klt: Path, request: Path, env: dict) -> dict:
    proc = subprocess.run([str(klt), "functional-verification", str(request),
                           "--format", "json"], cwd=str(REPO), env=env,
                          capture_output=True, text=True)
    try:
        return json.loads(proc.stdout)
    except json.JSONDecodeError:
        sys.stderr.write(proc.stdout[-2000:] + proc.stderr[-2000:])
        raise SystemExit("BLOCKED: klt produced no JSON report")


def klt_version(klt: Path) -> str:
    return subprocess.run([str(klt), "--version"], capture_output=True,
                          text=True).stdout.strip()


def provenance(klt: Path, request: Path, defines: list,
               workdir: Path) -> dict:
    import h07_compare as H
    pin = dr12_pin(DR12.read_text(encoding="utf-8"))
    hashes = rtl_hashes()
    _, rest = split_dev_cases(H.load_dev_cases())
    cases = {}
    for f in sorted((workdir / "cases").glob("*.json")):
        c = json.loads(f.read_text(encoding="utf-8"))
        cases[c.pop("case")] = c
    return {
        "claim": "RTL == frozen integer model (claim 1) on the DR-0012 trio "
                 "under Icarus; not model-vs-reference, synthesis, STA, "
                 "or sound",
        "rtl_sha256": hashes,
        "dr0012_pin_matches": all(hashes.get(k) == v for k, v in pin.items())
        and set(RTL_PIN) <= set(pin),
        "request_sha256": sha256_file(request),
        "klt_version": klt_version(klt),
        "defines": defines,
        "cases": cases,
        "not_graded": {
            "status": "NOT_RUN",
            "reason": f"events past block {MAX_EVENT_BLOCK}: Icarus playback "
                      "is ~2.4 s per frame, too long for this lane; see "
                      "evidence/h07-core/ for the Verilator/remote runs",
            "cases": sorted(c["id"] for c in rest)},
        "frame_rule": f"dev: last event block + {TAIL_FRAMES}; "
                      "stress: preregistered length",
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawTextHelpFormatter)
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--control", choices=sorted(CONTROLS))
    ap.add_argument("--venv", default=str(REPO / ".venv-fv"))
    args = ap.parse_args()

    klt, why = tooling(Path(args.venv))
    if klt is None:
        if args.control or args.write:
            print(f"NOT_RUN: {why}")
        else:
            emit_guarded_skip(f"check did not run: {why}")
        return EXIT["NOT_RUN"]

    env = dict(os.environ)
    scratch = Path(tempfile.mkdtemp(prefix="fv-dx7-", dir=str(REPO / "build"))) \
        if (REPO / "build").is_dir() else Path(tempfile.mkdtemp(prefix="fv-"))
    try:
        env["FV_WORKDIR"] = str(scratch / "work")
        # klt writes .klt/ next to the request, so run a copy in scratch
        # with the request-relative paths re-based
        req = json.loads(REQUEST.read_text(encoding="utf-8"))
        rq = scratch / "request.json"
        req["sources"] = [os.path.relpath(REPO / s[6:], scratch)
                          if s.startswith("../../") else s
                          for s in req["sources"]]
        req["testbench"]["search_path"] = os.path.relpath(REPO / "tools",
                                                          scratch)
        defines = []
        if args.control:
            defines = [args.control]
            env["FV_DEFINES"] = args.control
            req["testbench"]["testcase"] = [
                "case_" + re.sub(r"[^0-9A-Za-z]", "_", c)
                for c in CONTROLS[args.control]]
        rq.write_text(json.dumps(req, indent=2) + "\n", encoding="utf-8")

        fresh = run_klt(klt, rq, env)
        fresh["provenance"] = provenance(klt, REQUEST, defines,
                                    Path(env["FV_WORKDIR"]))
        if args.control:
            ok, msg = control_verdict(fresh, CONTROLS[args.control])
            out = EVID / f"control-{args.control}.json"
            if args.write:
                out.write_text(json.dumps(scrub(fresh), indent=1,
                                          sort_keys=True) + "\n",
                               encoding="utf-8")
            print(("CONTROL_OK " if ok else "CONTROL_DEFECT ") + msg)
            return 0 if ok else 1

        if args.write:
            REPORT.write_text(json.dumps(scrub(fresh), indent=1,
                                         sort_keys=True) + "\n",
                              encoding="utf-8")
            print(f"WROTE {REPORT} status={fresh.get('status')}")
            return 0 if fresh.get("status") == "pass" else 1

        if not REPORT.exists():
            emit_guarded_skip(f"check did not run: no committed report at"
                              f" {REPORT}")
            return EXIT["NOT_RUN"]
        committed = json.loads(REPORT.read_text(encoding="utf-8"))
        selected, ungraded = lane_cases()
        status, reasons, rows = check_verdict(committed, fresh,
                                              rtl_hashes(), selected)
        # Reasons first: the coverage record must stay the last thing the
        # stamp's bounded output tail retains.
        if reasons:
            print("; ".join(reasons))
        for line in coverage_lines(status, rows, ungraded,
                                   fresh["provenance"]["not_graded"]["reason"]):
            print(line)
        note = skip_note(rows, ungraded)
        if note:
            emit_guarded_skip(note)
        print(status)
        return EXIT[status]
    finally:
        if not os.environ.get("FV_KEEP"):
            shutil.rmtree(scratch, ignore_errors=True)


def scrub(report: dict) -> dict:
    """Drop machine-local paths from the committed copy (wall-clock fields
    stay as informational evidence; compare() ignores them)."""
    out = json.loads(json.dumps(report))
    env = out.get("environment", {})
    for k in ("results_xml", "test_log", "build_log"):
        env.pop(k, None)
    return out


if __name__ == "__main__":
    sys.exit(main())
