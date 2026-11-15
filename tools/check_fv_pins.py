#!/usr/bin/env python3
"""FV pin/provenance gate (issue #226).

Makefile's FV_PINS pins the toolchain that produced the committed
functional-verification evidence, and tools/functional_verification.py
compares `provenance` (and the recorded environment) verbatim on a re-run.
So a pin bump that does not regenerate the evidence would turn the FV lane
FAIL-on-provenance at the next wave gate -- the #185/#186 -> #209 -> #224
recurrence. This check makes that debt visible at bump time, in the fast
lane, by asserting that every version FV_PINS pins is the version the
committed evidence actually records:

  klayout-tools  <->  provenance.klt_version   (the stripped stdout of
                       `klt --version`, e.g. "klt 0.8.0" -- the version
                       token is compared, not the whole string)
  cocotb         <->  environment.cocotb_version

Files under evidence/t1-functional/ without a `provenance` block (the klt
request) carry no recorded toolchain and are skipped without a verdict.

A mismatch fails with the remedy (the REMEDY constant below): bump and
evidence must move together -- drop `.venv-fv` (the make venv rule does
not re-run while `.venv-fv/bin/klt` exists, so the old-pin venv would
silently re-record the old versions), `make functional-verification` to
recreate it from the new FV_PINS (that run shows the provenance mismatch),
then regenerate the evidence and both controls with `python3
tools/functional_verification.py --venv .venv-fv --write` (adding
`--control H07_MUTATE_GLITCH` / `--control H07_MUTATE_SUM_ORDER` for the
controls) -- `--write` is a flag of that script, not of the make targets
-- then update docs/reuse/catalog.json if any pinned tool file changed.

Exit codes: 0 pass, 1 check failure, 2 could-not-run
(tools/verdict_errors.py). Stdlib only.
"""

import argparse
import json
import os
import re
import sys

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402

REPO_ROOT = os.path.dirname(_TOOLS_DIR)
MAKEFILE = "Makefile"
EVIDENCE_DIR = os.path.join("evidence", "t1-functional")

# FV_PINS pin -> the report field that must name the same version. A pin
# outside this table is a checker gap (CouldNotRun, never a silent pass):
# extend this module when FV_PINS grows a new distribution.
PIN_RULES = {
    "klayout-tools": "klt_version",
    "cocotb": "cocotb_version",
}

KLT_VERSION_RE = re.compile(r"klt\s+(\S+)")
PIN_RE = re.compile(r"([A-Za-z0-9][A-Za-z0-9._-]*)==([0-9][A-Za-z0-9._+-]*)")

REMEDY = (
    "rm -rf .venv-fv; make functional-verification; "
    "python3 tools/functional_verification.py --venv .venv-fv --write; "
    "python3 tools/functional_verification.py --venv .venv-fv "
    "--control H07_MUTATE_GLITCH --write; "
    "python3 tools/functional_verification.py --venv .venv-fv "
    "--control H07_MUTATE_SUM_ORDER --write"
)


def parse_fv_pins(repo_root):
    """-> {distribution: version} from Makefile's FV_PINS assignment.

    Only quoted `distribution==version` pins are pins; any other
    non-comment text on the assignment (a dropped quote, a mangled pin)
    is unparseable -> CouldNotRun, never a partial or silent parse.
    Backslash continuations are joined; pin order and spacing are free.
    """
    path = os.path.join(repo_root, MAKEFILE)
    if not os.path.isfile(path):
        raise CouldNotRun(f"{MAKEFILE} not found under {repo_root}")
    try:
        with open(path, "r", encoding="utf-8") as fh:
            lines = fh.read().splitlines()
    except OSError as exc:
        raise CouldNotRun(f"cannot read {path}: {exc}")
    assignments = []
    i = 0
    while i < len(lines):
        m = re.match(r"^\s*FV_PINS\s*[:?+]?=\s*(.*)$", lines[i])
        if m:
            body = [m.group(1)]
            while body[-1].rstrip().endswith("\\") and i + 1 < len(lines):
                body[-1] = body[-1].rstrip()[:-1]
                i += 1
                body.append(lines[i])
            assignments.append(" ".join(body))
        i += 1
    if not assignments:
        raise CouldNotRun(
            f"no FV_PINS assignment found in {MAKEFILE}; cannot check "
            "the FV evidence pins")
    pins = {}
    for line in assignments:
        tokens = re.findall(r'"([^"]+)"', line)
        leftover = re.sub(r'"[^"]*"', "", line).split("#", 1)[0].strip()
        if leftover:
            raise CouldNotRun(
                f"FV_PINS in {MAKEFILE} is unparseable: unrecognised text "
                f"{leftover!r} outside the quoted pins")
        if not tokens:
            raise CouldNotRun(
                f"FV_PINS in {MAKEFILE} is unparseable: no quoted "
                '"distribution==version" pin found')
        for tok in tokens:
            m = PIN_RE.fullmatch(tok)
            if not m:
                raise CouldNotRun(
                    f"FV_PINS in {MAKEFILE} is unparseable: {tok!r} is not "
                    '"distribution==version"')
            dist, version = m.groups()
            if dist in pins and pins[dist] != version:
                raise CouldNotRun(
                    f"FV_PINS in {MAKEFILE} pins {dist} twice with "
                    f"different versions ({pins[dist]} vs {version})")
            pins[dist] = version
    return pins


def check_report(label, report, pins):
    """-> [reasons] for one committed report dict. Every FV_PINS pin is
    compared; a malformed or missing recorded version is a stated
    failure, never a silent pass."""
    reasons = []
    for dist, field in PIN_RULES.items():
        if dist not in pins:
            continue
        pinned = pins[dist]
        where = f"{label}"
        if dist == "klayout-tools":
            provenance = report.get("provenance")
            if not isinstance(provenance, dict):
                reasons.append(
                    f"{where}: no provenance block; cannot compare the "
                    f"klayout-tools pin {pinned} "
                    f"(regenerate: {REMEDY})")
                continue
            recorded = provenance.get("klt_version")
            if not isinstance(recorded, str) or not recorded.strip():
                reasons.append(
                    f"{where}: provenance.klt_version is missing or not a "
                    f"string; FV_PINS pins klayout-tools=={pinned} "
                    f"(regenerate: {REMEDY})")
                continue
            m = KLT_VERSION_RE.fullmatch(recorded.strip())
            if not m:
                reasons.append(
                    f"{where}: provenance.klt_version {recorded!r} is not "
                    '"klt <version>"; FV_PINS pins klayout-tools=='
                    f"{pinned} "
                    f"(regenerate: {REMEDY})")
                continue
            token = m.group(1)
            if token != pinned:
                reasons.append(
                    f"{where}: provenance.klt_version records klt "
                    f"{token}, but FV_PINS pins klayout-tools=={pinned}; "
                    "the committed evidence no longer names the pinned "
                    f"toolchain (regenerate: {REMEDY})")
        elif field == "cocotb_version":
            environment = report.get("environment")
            if not isinstance(environment, dict):
                reasons.append(
                    f"{where}: no environment block; cannot compare the "
                    f"cocotb pin {pinned} "
                    f"(regenerate: {REMEDY})")
                continue
            recorded = environment.get("cocotb_version")
            if not isinstance(recorded, str) or not recorded.strip():
                reasons.append(
                    f"{where}: environment.cocotb_version is missing or "
                    f"not a string; FV_PINS pins cocotb=={pinned} "
                    f"(regenerate: {REMEDY})")
            elif recorded.strip() != pinned:
                reasons.append(
                    f"{where}: environment.cocotb_version records "
                    f"{recorded.strip()}, but FV_PINS pins "
                    f"cocotb=={pinned}; the committed evidence no longer "
                    f"names the pinned toolchain (regenerate: {REMEDY})")
        else:
            raise CouldNotRun(
                f"no comparison rule implemented for FV_PINS pin {dist} "
                f"-> {field}; extend tools/check_fv_pins.py")
    return reasons


def run_check(repo_root):
    pins = parse_fv_pins(repo_root)
    unknown = sorted(set(pins) - set(PIN_RULES))
    if unknown:
        raise CouldNotRun(
            f"FV_PINS pins {unknown} but this checker has no comparison "
            f"rule for it (known: {sorted(PIN_RULES)}); extend "
            "tools/check_fv_pins.py")
    evdir = os.path.join(repo_root, EVIDENCE_DIR)
    if not os.path.isdir(evdir):
        raise CouldNotRun(
            f"{EVIDENCE_DIR}/ not found under {repo_root}; no FV evidence "
            "to check")
    names = sorted(n for n in os.listdir(evdir) if n.endswith(".json"))
    reasons = []
    checked = 0
    skipped = []
    for name in names:
        path = os.path.join(evdir, name)
        label = f"{EVIDENCE_DIR}/{name}"
        try:
            with open(path, "r", encoding="utf-8") as fh:
                report = json.load(fh)
        except (OSError, json.JSONDecodeError) as exc:
            reasons.append(f"{label}: not readable JSON: {exc}")
            continue
        if not isinstance(report, dict):
            reasons.append(f"{label}: top level is not a JSON object")
            continue
        provenance = report.get("provenance")
        if not isinstance(provenance, dict):
            # e.g. request.json: no recorded toolchain, no verdict of its
            # own -- the pin gate is about committed *reports*.
            skipped.append(label)
            continue
        checked += 1
        reasons += check_report(label, report, pins)
    if reasons:
        raise CheckFailure("\n  ".join(reasons))
    if checked == 0:
        raise CouldNotRun(
            f"no report under {EVIDENCE_DIR}/ carries a provenance block; "
            "nothing to compare FV_PINS against")
    print(f"check_fv_pins: PASS ({len(pins)} FV_PINS pin(s) x {checked} "
          f"committed report(s) with provenance; {len(skipped)} skipped "
          f"without provenance: {', '.join(skipped)})")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--repo-root", default=REPO_ROOT,
                        help="repo root (default: the checkout holding "
                        "this script)")
    args = parser.parse_args(argv)
    repo_root = os.path.abspath(args.repo_root)
    try:
        run_check(repo_root)
    except CheckFailure as exc:
        print(f"check_fv_pins: FAIL: {exc}", file=sys.stderr)
        return 1
    except CouldNotRun as exc:
        print(f"check_fv_pins: NOT_RUN: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
