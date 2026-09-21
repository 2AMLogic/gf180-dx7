#!/usr/bin/env python3
"""N08 frozen-threshold challenge run (issue #24).

Renders the PREREGISTERED challenge set (spec/release-vectors-v1.json,
chosen before any challenge output was inspected) through the frozen MODEL
— the model is the release — and, for every wrapper-valid case, compares
against the pinned external oracle at the FROZEN thresholds
(byte-identical, max_abs_error 0.0). Per-case verdicts are recorded in
reference/release/challenge-results.json, bound to the exact bundle.

Freeze protocol (enforced here):
  * the tool refuses to run unless the manifest carries
    thresholds_frozen_before_challenge = true and its spec binding matches
    the current preregistration file;
  * thresholds are read from the frozen manifest, never from this tool;
  * any challenge row that fails the frozen threshold is a BOUNDED
    FINDING recorded in the results and the release notes — the threshold
    is never retuned, the case is never swapped, and the RTL-vs-model
    claim (claim 1) is unaffected. A threshold change would be a decision
    record plus a full re-freeze (issue #24 stop/escalate).

Verdicts: AGREE_EXACT, FAIL (bounded finding at frozen thresholds),
MODEL_MISMATCH (the model no longer reproduces the frozen vector — a
release-consistency failure, not a challenge verdict), NOT_RUN
(environment; guarded, never a pass).

Exit codes: 0 = every oracle-comparable row AGREE_EXACT; 1 = any FAIL or
MODEL_MISMATCH (findings recorded); 2 = could not run (bundle missing or
oracle unavailable). Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare, sysex  # noqa: E402
from n08_build_release import (  # noqa: E402
    BuildFail, DEFAULT_OUT, SPEC_PATH, CaseSource, render_case, sha256_file,
)
from render_reference import (  # noqa: E402
    HarnessFail, HarnessNotRun, check_oracle_binary, load_identity,
)

SCHEMA = "gf180-dx7.n08-challenge/1"
RATE = 48000
PROTOCOL_NOTE = (
    "Thresholds and the challenge set were frozen in the preregistration "
    "(spec/release-vectors-v1.json) BEFORE any challenge render was "
    "inspected; this run evaluated the challenge at those frozen "
    "thresholds and records the verdicts unmodified. Thresholds were "
    "never tuned after inspection and never will be in place (issue #24 "
    "non-goals): a failure is a bounded finding, and any threshold change "
    "is a decision record plus a full re-freeze. The challenge runs "
    "happened at the frozen thresholds recorded in this bundle.")


def run_challenge(release_dir: Path, oracle_path: Path | None) -> tuple[
        int, dict]:
    manifest = json.loads((release_dir / "manifest.json").read_text(
        encoding="utf-8"))
    covered = manifest["covered"]
    if covered.get("thresholds_frozen_before_challenge") is not True:
        raise HarnessFail(
            "manifest is not marked thresholds_frozen_before_challenge; "
            "the challenge is forbidden until thresholds are frozen")
    if covered.get("spec", {}).get("sha256") != sha256_file(SPEC_PATH):
        raise HarnessFail(
            "preregistration spec drifted from the bundle binding; the "
            "challenge would not run at the frozen thresholds (re-freeze "
            "required)")
    thresholds = covered["thresholds"]["model_vs_oracle"]
    if thresholds.get("max_abs_error") != 0.0 or \
            thresholds.get("kind") != "byte_identical_f32le":
        raise HarnessFail(
            f"unexpected frozen threshold {thresholds!r}; refusing to "
            "evaluate against anything but the frozen byte-identity rule")

    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    challenge_ids = [e["id"] for e in spec["challenge_set"]["registry_cases"]]
    by_id = {c["id"]: c for c in covered["cases"]}
    source = CaseSource()
    voice_bytes_cache: dict[str, bytes] = {}

    rows = []
    n_agree = n_fail = n_model_mismatch = n_notrun = 0
    with tempfile.TemporaryDirectory(prefix="n08-challenge-") as td:
        oracle = None
        if oracle_path is not None:
            identity = load_identity(REPO / "reference" /
                                     "oracle-identity.json")
            check_oracle_binary(identity, oracle_path)
            oracle = _Oracle(oracle_path, Path(td))
        for case_id in challenge_ids:
            case = by_id.get(case_id)
            if case is None:
                rows.append({"id": case_id, "verdict": "MODEL_MISMATCH",
                             "status": "FAIL",
                             "detail": "challenge case missing from the "
                                       "bundle"})
                n_model_mismatch += 1
                continue
            # 1) the model must still reproduce its own frozen vector.
            try:
                rendered = render_case(source, case_id, case["origin"],
                                       case.get("dev32_index"))
            except (BuildFail, HarnessFail) as exc:
                rows.append({"id": case_id, "verdict": "NOT_RUN",
                             "status": "NOT_RUN", "detail": str(exc)})
                n_notrun += 1
                continue
            model_sha = hashlib.sha256(rendered["pcm"]).hexdigest()
            row = {
                "id": case_id,
                "axis": next(e["axis"] for e in
                             spec["challenge_set"]["registry_cases"]
                             if e["id"] == case_id),
                "model_pcm_sha256": model_sha,
                "frozen_vector_sha256": case["expected_pcm_sha256"],
                "oracle_comparable": case["oracle_comparable"],
                "threshold": "byte_identical_f32le (max_abs_error 0.0), "
                             "frozen before this run",
            }
            if model_sha != case["expected_pcm_sha256"] or \
                    rendered["trace"] != case["event_trace"]:
                row["verdict"] = "MODEL_MISMATCH"
                row["status"] = "FAIL"
                row["detail"] = ("the model no longer reproduces the frozen "
                                 "vector/trace; the release is STALE")
                n_model_mismatch += 1
                rows.append(row)
                continue
            if not case["oracle_comparable"]:
                row["verdict"] = "NOT_RUN"
                row["status"] = "NOT_RUN"
                row["detail"] = case["oracle_note"]
                n_notrun += 1
                rows.append(row)
                continue
            if oracle is None:
                row["verdict"] = "NOT_RUN"
                row["status"] = "NOT_RUN"
                row["detail"] = "pinned oracle binary unavailable"
                n_notrun += 1
                rows.append(row)
                continue
            # 2) wrapper-valid: compare against the pinned oracle at the
            #    frozen threshold.
            if case_id not in voice_bytes_cache:
                inp = source.resolve(case_id, case["origin"],
                                     case.get("dev32_index"))
                voice_bytes_cache[case_id] = inp["voice_bytes"]
            oracle_pcm = oracle.render(
                voice_bytes_cache[case_id],
                "\n".join(case["events"]) + "\n", case["render_seconds"])
            report = compare.compare_streams(oracle_pcm, rendered["pcm"])
            primary = report.primary or {}
            row["oracle_pcm_sha256"] = report.sha256_a
            row["byte_identical"] = report.byte_identical
            row["max_abs_error"] = primary.get("max_abs_error")
            row["mismatch_count"] = primary.get("mismatch_count")
            row["first_divergent_sample"] = \
                primary.get("first_divergent_sample")
            if report.passed:
                row["verdict"] = "AGREE_EXACT"
                row["status"] = "PASS"
                n_agree += 1
            else:
                row["verdict"] = "FAIL"
                row["status"] = "FAIL"
                row["detail"] = (
                    "BOUNDED FINDING at the frozen thresholds: the model "
                    "differs from the pinned oracle on this preregistered "
                    "challenge case; the threshold is not retuned, the case "
                    "is not swapped, and claim 1 (RTL-vs-model) is "
                    "unaffected")
                n_fail += 1
            rows.append(row)

    results = {
        "schema": SCHEMA,
        "bundle_sha256": manifest["bundle_sha256"],
        "thresholds_frozen_before_challenge": True,
        "thresholds_source": covered["spec"],
        "protocol_note": PROTOCOL_NOTE,
        "summary": {
            "challenge_cases": len(challenge_ids),
            "agree_exact": n_agree,
            "fail_bounded_findings": n_fail,
            "model_mismatch": n_model_mismatch,
            "not_run": n_notrun,
        },
        "cases": rows,
        "claims_note": ("Software-reference agreement only — not "
                        "original-DX7 fidelity, not RTL agreement, not a "
                        "musical claim (DEC-001, plan section 7)."),
    }
    code = 1 if (n_fail or n_model_mismatch) else 0
    return code, results


class _Oracle:
    """Pinned-oracle subprocess renders (identity re-checked by caller)."""

    def __init__(self, oracle_path: Path, tmp: Path) -> None:
        self.oracle = oracle_path
        self.tmp = tmp
        self.n = 0

    def render(self, voice_bytes: bytes, events: str, seconds: float) -> bytes:
        n = self.n
        self.n += 1
        vpath = self.tmp / f"c{n:03d}.syx"
        epath = self.tmp / f"c{n:03d}.events.txt"
        opath = self.tmp / f"c{n:03d}.f32"
        vpath.write_bytes(voice_bytes)
        epath.write_text(events, encoding="utf-8")
        proc = subprocess.run(
            [str(self.oracle), "--voice", str(vpath), "--events", str(epath),
             "--out", str(opath), "--rate", str(RATE),
             "--seconds", str(seconds)], capture_output=True, text=True)
        if proc.returncode != 0:
            raise HarnessFail(f"oracle failed (exit {proc.returncode}): "
                              f"{proc.stderr.strip()}")
        data = opath.read_bytes()
        if len(data) != 4 * int(RATE * seconds):
            raise HarnessFail("oracle produced an unexpected length")
        return data


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--release-dir", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--oracle", type=Path, default=None)
    parser.add_argument("--results", type=Path, default=None,
                        help="results path (default <release-dir>/"
                             "challenge-results.json)")
    args = parser.parse_args(argv)
    release_dir = Path(args.release_dir)
    if not (release_dir / "manifest.json").is_file():
        print(f"n08_challenge: NOT_RUN — no bundle at {release_dir}",
              file=sys.stderr)
        return 2
    oracle_path = args.oracle
    try:
        if oracle_path is None:
            oracle_path = Path(load_identity(REPO / "reference" /
                                             "oracle-identity.json")
                              ["binary"]["path"])
        code, results = run_challenge(release_dir, oracle_path)
    except HarnessNotRun as exc:
        print(f"n08_challenge: NOT_RUN — {exc}", file=sys.stderr)
        return 2
    except HarnessFail as exc:
        print(f"n08_challenge: FAIL — {exc}", file=sys.stderr)
        return 1
    out_path = args.results or (release_dir / "challenge-results.json")
    out_path.write_text(json.dumps(results, indent=2) + "\n",
                        encoding="utf-8")
    s = results["summary"]
    verdict = {0: "PASS", 1: "FAIL"}.get(code, "NOT_RUN")
    print(f"n08_challenge: {verdict} — {s['challenge_cases']} preregistered "
          f"challenge case(s) at frozen thresholds: {s['agree_exact']} "
          f"AGREE_EXACT, {s['fail_bounded_findings']} FAIL (bounded "
          f"findings), {s['model_mismatch']} MODEL_MISMATCH, "
          f"{s['not_run']} NOT_RUN; results: {out_path}")
    for row in results["cases"]:
        if row["verdict"] in ("FAIL", "MODEL_MISMATCH"):
            print(f"  {row['verdict']} [{row['id']}]: "
                  f"{row.get('detail', '')}")
    return code


if __name__ == "__main__":
    sys.exit(main())
