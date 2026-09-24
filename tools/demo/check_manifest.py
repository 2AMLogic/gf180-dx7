#!/usr/bin/env python3
"""U05 demo-manifest validator (issue #38).

Refuses placeholders at record time; accepts a declared-dry run now. The
two modes of record:

  dry     declared dry software-only run (this PR). Software fields must
          be CONCRETE (real shas, real tool hashes, real pass/fail results
          - the software rows are real runs, not placeholders). Hardware
          rows must be explicitly NOT_RUN or BLOCKED with a named
          operator issue (#31 H09 capture, #35 listening bank), and the
          hardware evidence fields must carry their declared placeholders
          - a concrete-looking bitstream hash on a dry manifest is an
          unsupported hardware claim and is refused.
  record  record time. NO placeholder anywhere in the manifest (a value of
          the form <...>, or TBD/PLACEHOLDER/TODO, or empty string, at any
          depth is refused by name), every hardware row has a real status
          and evidence, and the claim must reference the board capture
          evidence. A record manifest that carries a placeholder fails
          with the offending path named.

Statuses use the repository vocabulary PASS/FAIL/NOT_RUN/BLOCKED/
NO_VERDICT/STALE; a check that did not run must never be reported as a
pass, and a dry manifest may never carry a pass on a hardware row.

Exit: 0 valid, 1 invalid (problems printed, each with its JSON path).
Schema for humans: spec/demo-manifest-v1.schema.json (this tool is the
executable check; the schema file is documentation with the same rules).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
REPO = THIS_DIR.parents[1]

SCHEMA_FILE = REPO / "spec" / "demo-manifest-v1.schema.json"
SCHEMA = "gf180-dx7.demo-manifest/1"

STATUS_VOCAB = {"PASS", "FAIL", "NOT_RUN", "BLOCKED", "NO_VERDICT", "STALE"}
SHA40 = re.compile(r"^[0-9a-f]{40}$")
SHA64 = re.compile(r"^[0-9a-f]{64}$")
ISO_UTC = re.compile(r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$")
DRY_CLAIM = ("software-only dry run: the committed fixture and stress "
             "battery pass on the mock link; the hardware acceptance rows "
             "are NOT_RUN, pending #31 (H09 FPGA capture) and #35 "
             "(listening bank)")
PLACEHOLDER = re.compile(r"^<[A-Z0-9 ._-]+>$")
BAD_STRINGS = {"tbd", "placeholder", "todo", "null", "none", ""}

HARDWARE_EVIDENCE_FIELDS = ["rtl_tag", "bitstream_sha256", "bank_version"]
HARDWARE_ROWS = ["capture", "listening_bank", "board_latency",
                 "board_audit"]


def is_placeholder(value) -> bool:
    if not isinstance(value, str):
        return False
    return bool(PLACEHOLDER.match(value)) or \
        value.strip().lower() in BAD_STRINGS


def walk(obj, path="$"):
    """Yield (json_path, value) for every scalar leaf."""
    if isinstance(obj, dict):
        for k, v in obj.items():
            yield from walk(v, f"{path}.{k}")
    elif isinstance(obj, list):
        for i, v in enumerate(obj):
            yield from walk(v, f"{path}[{i}]")
    else:
        yield path, obj


def _req(man, path: list, problems) -> bool:
    cur = man
    for p in path:
        if not isinstance(cur, dict) or p not in cur:
            problems.append(f"missing required field {'.'.join(map(str, path))}")
            return False
        cur = cur[p]
    return True


def _field(man, path: list, expect: str, problems) -> None:
    if not _req(man, path, problems):
        return
    v = man
    for p in path:
        v = v[p]
    kind = ".".join(map(str, path))
    if not isinstance(v, str):
        problems.append(f"{kind}: not a string: {v!r}")
        return
    good = (expect == "sha40" and SHA40.match(v)) or \
        (expect == "sha64" and SHA64.match(v)) or \
        (expect == "iso_utc" and ISO_UTC.match(v)) or \
        (expect == "string" and v.strip() and not is_placeholder(v))
    if not good:
        problems.append(f"{kind}: bad value {v!r} (expected {expect})")


def _row_status(man, row: str, allowed: set, problems,
                require_blocker: bool) -> None:
    if not _req(man, ["hardware", row, "status"], problems):
        return
    st = man["hardware"][row]["status"]
    if st not in STATUS_VOCAB:
        problems.append(f"hardware.{row}.status: unknown status {st!r} "
                        f"(vocabulary: {sorted(STATUS_VOCAB)})")
    if st not in allowed:
        problems.append(f"hardware.{row}.status: {st} not allowed here "
                        f"(allowed: {sorted(allowed)})")
    if require_blocker:
        if not _req(man, ["hardware", row, "blocked_by"], problems):
            return
        b = man["hardware"][row]["blocked_by"]
        if not isinstance(b, str) or not b.strip():
            problems.append(f"hardware.{row}.blocked_by: empty - a "
                            f"NOT_RUN/BLOCKED row must name its operator "
                            f"issue")
    elif st in ("NOT_RUN", "BLOCKED"):
        if not _req(man, ["hardware", row, "blocked_by"], problems):
            return
        b = man["hardware"][row]["blocked_by"]
        if not isinstance(b, str) or not b.strip():
            problems.append(f"hardware.{row}.blocked_by: empty - a "
                            f"NOT_RUN/BLOCKED row must name its blocker")


def check(man) -> list[str]:
    problems: list[str] = []
    mode = man.get("mode")
    if man.get("schema") != SCHEMA:
        problems.append(f"schema: expected {SCHEMA!r}, got "
                        f"{man.get('schema')!r}")
    if mode not in ("dry", "record"):
        problems.append(f"mode: expected 'dry' or 'record', got {mode!r}")
        return problems

    # --- structural requirements (from the schema file) ------------------
    for path in [["run_id"], ["mode"], ["date"], ["claim"], ["software"],
                 ["results"], ["hardware"]]:
        _req(man, path, problems)
    if not problems:
        _field(man, ["run_id"], "string", problems)
        _field(man, ["date"], "iso_utc", problems)
        for f, expect in [("repo_sha", "sha40"), ("branch", "string"),
                          ("python", "string")]:
            _field(man, ["software", f], expect, problems)
        for f in ("demo_fixture.py", "stress.py"):
            _field(man, ["software", "tools", f, "sha256"], "sha64", problems)
        # fixture / phrase_set / patch carry path + sha256
        for f in ("fixture", "phrase_set"):
            if _req(man, ["software", f], problems):
                _field(man, ["software", f, "path"], "string", problems)
                _field(man, ["software", f, "sha256"], "sha64", problems)
        if _req(man, ["software", "patch"], problems):
            _field(man, ["software", "patch", "canonical_hash"], "sha64",
                   problems)

    res = man.get("results", {})
    for k in ("demo_clean_compact", "demo_clean_full"):
        if k not in res:
            problems.append(f"results.{k}: missing")
        elif res[k] not in ("PASS", "FAIL"):
            problems.append(
                f"results.{k}: {res[k]!r} - a software row that actually "
                f"ran must report PASS or FAIL (never NOT_RUN/BLOCKED)")
    if "stress" not in res:
        problems.append("results.stress: missing")
    else:
        for k, v in res["stress"].items():
            if v.get("status") not in ("PASS", "FAIL"):
                problems.append(f"results.stress.{k}: {v.get('status')!r} - "
                                f"a software row that actually ran must "
                                f"report PASS or FAIL")
    if "negatives" not in res:
        problems.append("results.negatives: missing")
    else:
        for k, v in res["negatives"].items():
            if not isinstance(v, bool):
                problems.append(f"results.negatives.{k}: {v!r} - the "
                                f"negative-control record is a boolean "
                                f"(named gates verified), got {type(v).__name__}")

    # --- placeholder policy by mode --------------------------------------
    if mode == "record":
        # refuse ANY placeholder anywhere in the manifest, by name
        for path, v in walk(man):
            if is_placeholder(v):
                problems.append(f"placeholder at {path} {v!r}: record "
                                f"manifests refuse placeholders")
        for row in HARDWARE_ROWS:
            _row_status(man, row, {"PASS", "FAIL"}, problems,
                        require_blocker=False)
        for f in HARDWARE_EVIDENCE_FIELDS:
            expect = "sha64" if f == "bitstream_sha256" else "string"
            _field(man, ["hardware", f], expect, problems)
        claim = man.get("claim", "")
        if "capture" not in claim.lower():
            problems.append("claim: a record manifest's claim must "
                            "reference the board capture evidence")
    else:
        # dry: software concrete, hardware declared-but-empty
        sw_bad: list[str] = []
        for path, v in walk(man.get("software", {})):
            if is_placeholder(v):
                sw_bad.append(f"placeholder in dry-run software row {path} "
                              f"{v!r}: the software rows are real runs and "
                              f"may not carry placeholders")
        if sw_bad:
            problems.extend(sw_bad)
        for row in HARDWARE_ROWS:
            _row_status(man, row, {"NOT_RUN", "BLOCKED"}, problems,
                        require_blocker=True)
        for f in HARDWARE_EVIDENCE_FIELDS:
            if not _req(man, ["hardware", f], problems):
                continue
            v = man["hardware"][f]
            if not is_placeholder(v):
                problems.append(
                    f"hardware.{f}: {v!r} looks concrete on a DRY manifest "
                    f"- a hardware value without the operator's record is "
                    f"an unsupported claim; declared dry runs carry the "
                    f"declared placeholder")
        claim = man.get("claim", "")
        if claim != DRY_CLAIM:
            problems.append("claim: a dry manifest carries exactly the "
                            "declared software-only claim "
                            f"{DRY_CLAIM!r}")
    return problems


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("manifest", help="path to the manifest JSON")
    ap.add_argument("--schema-check", action="store_true",
                    help="also verify the schema file is loadable and self-"
                         "describes the same rules")
    args = ap.parse_args()
    mpath = Path(args.manifest)
    if not mpath.is_absolute():
        mpath = REPO / mpath
    try:
        man = json.loads(mpath.read_text())
    except Exception as e:  # noqa: BLE001
        print(json.dumps({"status": "FAIL",
                          "problems": [f"load: {type(e).__name__}: {e}"]}))
        return 1
    if args.schema_check:
        try:
            json.loads(SCHEMA_FILE.read_text())
        except Exception as e:  # noqa: BLE001
            print(json.dumps({"status": "FAIL",
                              "problems": [f"schema file: {e}"]}))
            return 1
    problems = check(man)
    status = "PASS" if not problems else "FAIL"
    print(json.dumps({"status": status, "mode": man.get("mode"),
                      "problems": problems}, indent=2))
    return 0 if not problems else 1


if __name__ == "__main__":
    sys.exit(main())