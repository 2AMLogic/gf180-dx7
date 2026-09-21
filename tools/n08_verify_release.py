#!/usr/bin/env python3
"""N08 frozen-release verifier (issue #24) — the H-series RTL conformance gate.

Verifies the committed bundle at reference/release/:

  1. manifest integrity (schema, preregistration binding, thresholds copy);
  2. bundle hash over the covered content (case records, thresholds, budgets,
     model identity, vector shas);
  3. every vector file present with its recorded sha256;
  4. model identity: the frozen module file hashes match the current tree —
     any model/profile/spec drift FAILS (STALE rule: the bundle is invalid
     for a changed model and every H-series conformance status becomes
     STALE until a re-freeze);
  5. re-render of each case through the frozen model must reproduce the
     recorded vector sha256 AND the recorded block-level event trace
     (archive-gated dev32 rows are guarded NOT_RUN when the pinned archive
     is absent — reported loudly, never a pass);
  6. the committed challenge and mutation records (when present) must be
     bound to this exact bundle (bundle_sha256 match).

Exit codes: 0 = bundle intact and every performed check passes (NOT_RUN
rows reported, never as passes); 1 = tamper / drift / threshold failure —
the failing case is named; 2 = could not run (bundle or manifest missing).

This tool verifies the frozen model release only. It establishes nothing
about original-DX7 fidelity, RTL conformance itself, synthesis, fit, or
audibility. Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from n08_build_release import (  # noqa: E402
    BuildFail, DEFAULT_OUT, MODEL_MODULES, PROFILE_PATH, RELEASE_ID,
    SPEC_PATH, sha256_file, render_case, CaseSource, bundle_hash,
)

CHALLENGE_RESULTS = "challenge-results.json"
MUTATION_RESULTS = "mutation-results.json"


def verify_release(release_dir: Path, *, only: list[str] | None = None,
                   rerender: bool = True) -> tuple[int, list[dict]]:
    """Verify the bundle; returns (exit_code, rows).

    rows are {check, case?, status: PASS|FAIL|NOT_RUN, detail}.
    """
    rows: list[dict] = []
    release_dir = Path(release_dir)
    manifest_path = release_dir / "manifest.json"
    if not release_dir.is_dir() or not manifest_path.is_file():
        print(f"n08_verify_release: NOT_RUN — no bundle at {release_dir} "
              "(the committed release must exist)", file=sys.stderr)
        return 2, rows
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (json.JSONDecodeError, OSError) as exc:
        rows.append({"check": "manifest", "status": "FAIL",
                     "detail": f"manifest unreadable: {exc}"})
        return 1, rows
    covered = manifest.get("covered")
    if not isinstance(covered, dict) or \
            manifest.get("release_id") != RELEASE_ID:
        rows.append({"check": "manifest", "status": "FAIL",
                     "detail": "manifest schema/release_id mismatch"})
        return 1, rows

    def fail(check: str, detail: str, case: str | None = None) -> None:
        row = {"check": check, "status": "FAIL", "detail": detail}
        if case:
            row["case"] = case
        rows.append(row)

    # 1+2: manifest integrity + bundle hash -----------------------------
    if manifest.get("bundle_sha256") != bundle_hash(covered):
        fail("bundle_hash",
             "bundle hash mismatch: the covered content does not hash to "
             "manifest.bundle_sha256 (tampered or hand-edited manifest)")
    spec_sha = sha256_file(SPEC_PATH)
    if covered.get("spec", {}).get("sha256") != spec_sha:
        fail("spec_binding",
             f"preregistration spec drifted ({spec_sha[:12]}) from the "
             "bundle's recorded binding; thresholds are no longer the "
             "frozen ones (STALE — re-freeze required)")
    if covered.get("thresholds_frozen_before_challenge") is not True:
        fail("thresholds", "thresholds_frozen_before_challenge is not true")

    # 3: vector files ----------------------------------------------------
    vectors = covered.get("vectors", {})
    for case_id, entry in sorted(vectors.items()):
        vpath = release_dir / entry["file"]
        if not vpath.is_file():
            fail("vector_file", f"vector file missing: {entry['file']}",
                 case=case_id)
            continue
        got = sha256_file(vpath)
        if got != entry["sha256"]:
            fail("vector_file",
                 f"vector bytes tampered: sha256 {got[:12]} != recorded "
                 f"{entry['sha256'][:12]} ({entry['file']})", case=case_id)
        else:
            rows.append({"check": "vector_file", "case": case_id,
                         "status": "PASS", "detail": entry["file"]})

    # 4: model identity / profile drift (STALE rule) ----------------------
    for mod, want in covered.get("model_identity", {}).get(
            "modules", {}).items():
        got = sha256_file(REPO / mod)
        if got != want:
            fail("model_identity",
                 f"model module {mod} drifted ({got[:12]} != {want[:12]}); "
                 "the bundle is invalid for this tree — every H-series "
                 "conformance status is STALE until a full re-freeze "
                 "(decision record required)")
    profile_sha = sha256_file(PROFILE_PATH)
    if covered.get("numeric_profile", {}).get("sha256") != profile_sha:
        fail("numeric_profile",
             f"numeric profile drifted ({profile_sha[:12]}); error budgets "
             "and the frozen profile pointer no longer match (STALE)")

    # 6: committed challenge/mutation records must bind to this bundle ----
    for name in (CHALLENGE_RESULTS, MUTATION_RESULTS):
        rpath = release_dir / name
        if not rpath.is_file():
            continue
        try:
            record = json.loads(rpath.read_text(encoding="utf-8"))
        except (json.JSONDecodeError, OSError) as exc:
            fail(name, f"committed record unreadable: {exc}")
            continue
        if record.get("bundle_sha256") != manifest.get("bundle_sha256"):
            fail(name,
                 "committed record is bound to a different bundle "
                 f"({str(record.get('bundle_sha256'))[:12]} != "
                 f"{manifest.get('bundle_sha256', '')[:12]})")

    # 5: re-render through the frozen model -------------------------------
    cases = [c for c in covered.get("cases", [])
             if c.get("status") != "NOT_RUN"]
    if only is not None:
        want = set(only)
        cases = [c for c in cases if c["id"] in want]
        missing = want - {c["id"] for c in cases}
        if missing:
            fail("selection", f"--only names cases outside the bundle: "
                              f"{sorted(missing)}")
    if rerender:
        source: CaseSource | None = None
        for case in cases:
            case_id = case["id"]
            if source is None:
                source = CaseSource()
            try:
                rendered = render_case(
                    source, case_id, case["origin"], case.get("dev32_index"))
            except BuildFail as exc:
                if "archive unavailable" in str(exc):
                    rows.append({"check": "rerender", "case": case_id,
                                 "status": "NOT_RUN", "detail": str(exc)})
                    continue
                fail("rerender", str(exc), case=case_id)
                continue
            got = hashlib.sha256(rendered["pcm"]).hexdigest()
            if got != case["expected_pcm_sha256"]:
                fail("rerender",
                     f"re-rendered PCM sha256 {got[:12]} != frozen vector "
                     f"{case['expected_pcm_sha256'][:12]} — the model does "
                     "not reproduce the frozen release",
                     case=case_id)
            elif rendered["trace"] != case["event_trace"]:
                fail("rerender",
                     "block-level event trace diverged from the frozen "
                     "trace", case=case_id)
            else:
                rows.append({"check": "rerender", "case": case_id,
                             "status": "PASS",
                             "detail": "pcm sha256 + trace reproduce"})

    exit_code = 1 if any(r["status"] == "FAIL" for r in rows) else 0
    return exit_code, rows


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--release-dir", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--only", default=None,
                        help="comma-separated case ids to re-render (bundle "
                             "integrity is always checked in full)")
    parser.add_argument("--no-rerender", action="store_true",
                        help="skip the model re-render pass (integrity only)")
    args = parser.parse_args(argv)
    only = [s.strip() for s in args.only.split(",")] if args.only else None
    code, rows = verify_release(args.release_dir, only=only,
                                rerender=not args.no_rerender)
    n_pass = sum(1 for r in rows if r["status"] == "PASS")
    n_notrun = sum(1 for r in rows if r["status"] == "NOT_RUN")
    n_fail = sum(1 for r in rows if r["status"] == "FAIL")
    for r in rows:
        if r["status"] == "FAIL":
            name = f" [{r['case']}]" if r.get("case") else ""
            print(f"FAIL{name}: {r['check']} — {r['detail']}")
        elif r["status"] == "NOT_RUN":
            name = f" [{r['case']}]" if r.get("case") else ""
            print(f"NOT_RUN{name}: {r.get('detail', '')}")
    verdict = {0: "PASS", 1: "FAIL", 2: "NOT_RUN"}[code]
    print(f"n08_verify_release: {verdict} — bundle {args.release_dir}: "
          f"{n_pass} check(s) passed, {n_notrun} NOT_RUN, {n_fail} FAILED")
    return code


if __name__ == "__main__":
    sys.exit(main())
