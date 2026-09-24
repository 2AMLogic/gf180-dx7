#!/usr/bin/env python3
"""U05 reproducible demo (issue #38): one command drives every SOFTWARE
acceptance row and writes the committed evidence:

  1. the committed fixture, clean, compact + full profiles
     (demo_fixture.py x2) -> demo-clean-<profile>.json
  2. all eight corruption modes with --expect-fail (the negative controls)
     -> demo-negative-<mode>.json
  3. the stress battery, including the required throttled-link underrun
     negative (stress.py) -> stress-results.json
  4. the dry version manifest, filled from the ACTUAL run (real shas,
     real pass/fail) -> demo-manifest.json, validated by
     tools/demo/check_manifest.py (placeholders refused at record time;
     a declared dry now is accepted with hardware rows NOT_RUN/named)
  5. the hardware NOT_RUN record (rows the operator must execute, each
     naming its blocking issue) -> hardware-not-run.json

Exit: 0 all software rows pass (the hardware rows are recorded, not
passed), 1 any software row fails, 2 an environment gate was not met.
A dry manifest written here claims exactly: the software path is ready;
the hardware rows are pending #31 (H09 FPGA capture) and #35 (listening
bank). It never claims playback on hardware, board latency, or timbre.
"""

from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import subprocess
import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
REPO = THIS_DIR.parents[1]
for _p in (str(THIS_DIR), str(REPO / "src"), str(REPO / "tools")):
    if _p not in sys.path:
        sys.path.insert(0, _p)

import check_manifest  # noqa: E402
import demo_fixture  # noqa: E402
import fixtures_dx7 as F  # noqa: E402
import stress  # noqa: E402

DRY_MANIFEST_CLAIM = check_manifest.DRY_CLAIM


def _sha256(path: Path) -> str:
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def _git() -> tuple[str, str]:
    sha = subprocess.run(["git", "rev-parse", "HEAD"], cwd=REPO,
                         capture_output=True, text=True, check=True).stdout.strip()
    branch = subprocess.run(["git", "branch", "--show-current"], cwd=REPO,
                            capture_output=True, text=True,
                            check=True).stdout.strip() or "detached"
    return sha, branch


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", default="reference/evidence/u05-demo")
    ap.add_argument("--skip", action="append", default=[],
                    choices=["demo", "stress", "manifest"],
                    help="skip a stage (for development; a committed "
                         "evidence set never skips)")
    args = ap.parse_args()
    out = Path(args.out)
    if not out.is_absolute():
        out = REPO / out
    out.mkdir(parents=True, exist_ok=True)

    failures: list[str] = []

    # --- fixtures + negatives --------------------------------------------
    fx, phrases = F.load_fixture(REPO / "spec" / "demo-audition-v1.json")
    for profile in ("compact", "full"):
        if "demo" in args.skip:
            break
        rp = out / f"demo-clean-{profile}.json"
        rc = demo_fixture.run_bench(REPO / "spec" / "demo-audition-v1.json",
                                    profile, "none", False,
                                    report_path=rp)
        status = json.loads(rp.read_text())["status"]
        print(f"  fixture {profile:8s} {status}")
        if rc != 0:
            failures.append(f"fixture {profile}: rc={rc} status={status}")

    negatives: dict[str, bool] = {}
    for mode in ["drop-deltas", "drop-restore", "late-window", "drop-bend",
                 "drop-bend-step", "burst", "throttle", "drop-note-off"]:
        if "demo" in args.skip:
            break
        rp = out / f"demo-negative-{mode}.json"
        rc = demo_fixture.run_bench(REPO / "spec" / "demo-audition-v1.json",
                                    "compact", mode, True, report_path=rp)
        rep = json.loads(rp.read_text())
        ok = rc == 0 and rep.get("expect_fail_verification", {}).get("passed")
        negatives[mode] = bool(ok)
        print(f"  negative {mode:14s} "
              f"{'VERIFIED' if ok else 'NOT VERIFIED'}")
        if not ok:
            failures.append(f"negative {mode}: rc={rc}")

    # --- stress battery ----------------------------------------------------
    stress_report = None
    if "stress" not in args.skip:
        voice = F.resolve_patch(fx)
        stress_report = stress.run_all(voice)
        rp = out / "stress-results.json"
        rp.write_text(json.dumps(stress_report, indent=2) + "\n")
        for name, r in stress_report["scenarios"].items():
            print(f"  stress   {name:24s} {r['status']}"
                  + (f"  rejections={r['rejections']}"
                     if r["throttled"] else ""))
            if r["status"] != "PASS":
                failures.append(f"stress {name}: {r['problems']}")

    # --- hardware NOT_RUN record ------------------------------------------
    hw_record = {
        "schema": "gf180-dx7.hardware-not-run/1",
        "issue": "#38 (U05) - hardware acceptance rows NOT run by this PR",
        "rows": [
            {"row": "sustained playing on the board, all patches",
             "status": "NOT_RUN",
             "blocked_by": "issue #35 (final U02 patch bank selection) and "
                           "issue #31 (H09 FPGA capture) - operator rows"},
            {"row": "board latency budget",
             "status": "NOT_RUN",
             "blocked_by": "issue #31 (H09 FPGA capture) - the board "
                           "latency measurement is operator-executed"},
            {"row": "reproducible recording capture",
             "status": "NOT_RUN",
             "blocked_by": "issue #31 (H09 FPGA capture) - the dry record "
                           "(this PR) exists; the board capture is "
                           "operator-executed"},
            {"row": "stuck notes / wrap / underrun on hardware",
             "status": "NOT_RUN",
             "blocked_by": "issue #31 (H09 FPGA capture) - software-side "
                           "equivalents pass (see stress-results.json); "
                           "the board audit is operator-executed"},
            {"row": "listening bank",
             "status": "BLOCKED",
             "blocked_by": "issue #35 (listening bank) - not a numeric "
                           "row; a listening record, operator-executed"}
        ],
        "note": "NOT_RUN is recorded, never a pass and never silently "
                "skipped: each row names the operator issue it is blocked "
                "on. The software-side rows this PR does run are listed "
                "with their evidence files.",
        "evidence": {
            "stress (software, mock link)": "stress-results.json",
            "fixture clean compact/full": "demo-clean-*.json",
            "negative controls": "demo-negative-*.json",
            "dry manifest": "demo-manifest.json"
        },
    }
    (out / "hardware-not-run.json").write_text(
        json.dumps(hw_record, indent=2) + "\n")

    # --- dry manifest -------------------------------------------------------
    if "manifest" not in args.skip:
        sha, branch = _git()
        patch = F.resolve_patch(fx)
        manifest = {
            "schema": check_manifest.SCHEMA,
            "run_id": "u05-demo-" + datetime.datetime.now(
                datetime.timezone.utc).strftime("%Y%m%dT%H%M%SZ"),
            "mode": "dry",
            "date": datetime.datetime.now(
                datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
            "claim": DRY_MANIFEST_CLAIM,
            "software": {
                "repo_sha": sha,
                "branch": branch,
                "python": ".".join(map(str, sys.version_info[:3])),
                "fixture": {"path": "spec/demo-audition-v1.json",
                            "sha256": _sha256(REPO / "spec" /
                                              "demo-audition-v1.json")},
                "phrase_set": {"path": "spec/audition-phrases-v2.json",
                               "sha256": _sha256(REPO / "spec" /
                                                 "audition-phrases-v2.json")},
                "patch": {"source": "tests/fixtures/valid_bank.syx slot 7 "
                                    "(BANKVCE007)",
                          "canonical_hash": F.canonical_hash(patch)},
                "tools": {
                    "demo_fixture.py":
                        {"sha256": _sha256(THIS_DIR / "demo_fixture.py")},
                    "stress.py": {"sha256": _sha256(THIS_DIR / "stress.py")}
                }
            },
            "results": {
                "demo_clean_compact": json.loads(
                    (out / "demo-clean-compact.json").read_text()
                )["status"],
                "demo_clean_full": json.loads(
                    (out / "demo-clean-full.json").read_text()
                )["status"],
                "stress": {k: {"status": v["status"],
                               **({"rejections_observed": v["rejections"]}
                                  if v["throttled"] else {})}
                           for k, v in
                           (stress_report or {}).get("scenarios", {}).items()},
                "negatives": negatives
            },
            "hardware": {
                "rtl_tag": "<RTL_TAG>",
                "bitstream_sha256": "<BITSTREAM_SHA256>",
                "bank_version": "<BANK_VERSION>",
                "capture": {"status": "NOT_RUN",
                            "blocked_by": "issue #31 - H09: FPGA board "
                            "capture pipeline (operator)"},
                "listening_bank": {"status": "NOT_RUN",
                                   "blocked_by": "issue #35 - listening "
                                   "bank on real hardware (operator)"},
                "board_latency": {"status": "BLOCKED",
                                  "blocked_by": "issue #31 - H09: board "
                                  "latency measurement (operator)"},
                "board_audit": {"status": "BLOCKED",
                                "blocked_by": "issue #31 - H09: board "
                                "waveform audit/capture (operator)"}
            }
        }
        problems = check_manifest.check(manifest)
        if problems:
            failures.append(f"manifest self-check: {problems}")
            print("  manifest INVALID:", problems)
        else:
            print("  manifest valid (dry)")
        rp = out / "demo-manifest.json"
        rp.write_text(json.dumps(manifest, indent=2) + "\n")

    print(json.dumps({"status": "FAIL" if failures else "PASS",
                      "out": str(out.relative_to(REPO)),
                      "failures": failures}, indent=2))
    return 0 if not failures else 1


if __name__ == "__main__":
    sys.exit(main())