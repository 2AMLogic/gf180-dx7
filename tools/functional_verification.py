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
                   the cases that mutant must break and require FAIL there;
                   writes evidence/t1-functional/control-NAME.json with
                   --write.

Statuses and exit codes
  PASS 0   re-run matches the committed report, every test passed
  FAIL 1   a verdict, count, or recorded environment differs, or a test fails
  NOT_RUN / BLOCKED 2   klt, cocotb or iverilog is missing (never a pass)
  STALE 3  the committed report names different RTL hashes than the tree

The pinned toolchain is `klayout-tools==0.8.0` + `cocotb==2.1.0` in a venv
(`make functional-verification` creates .venv-fv inside the checkout); no
host tool is installed or changed.
"""

from __future__ import annotations

import argparse
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


def control_verdict(report: dict, expected_cases: list) -> tuple:
    """A negative control passes only if the mutant makes every selected
    test fail (and at least one ran)."""
    tests = {t["name"]: t["status"] for t in report.get("tests", [])}
    want = {"case_" + re.sub(r"[^0-9A-Za-z]", "_", c) for c in expected_cases}
    if set(tests) != want:
        return False, f"ran {sorted(tests)} but expected {sorted(want)}"
    ok = all(s == "failed" for s in tests.values())
    return ok, ("mutant fails every selected case" if ok
                else f"mutant did NOT fail: {tests}")


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
        print(f"NOT_RUN: {why}")
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
            print(f"NOT_RUN: no committed report at {REPORT}")
            return EXIT["NOT_RUN"]
        committed = json.loads(REPORT.read_text(encoding="utf-8"))
        status, reasons = compare(committed, fresh, rtl_hashes())
        print(status + ("" if not reasons else ": " + "; ".join(reasons)))
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
