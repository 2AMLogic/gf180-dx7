#!/usr/bin/env python3
"""R04 reproducibility evidence: committed render vs fresh re-render.

Proves reproducibility end-to-end for one fixture (issue #11; the R02
harness itself stays untouched — render_reference.py is owned by #7):

  1. re-render the fixture with tools/render_reference.py into a temp
     directory (R01 verifier, oracle identity pin, silent-output guard,
     determinism double-render all re-run);
  2. cross-check the fresh render.json against the committed one
     (fixture, sample rate, format, sample/byte counts, output/voice/
     events hashes, oracle binary pin, determinism flag) — a wrong
     sample-rate label or any metadata drift is a FAIL, localized to
     the disagreeing field;
  3. run the exact paired comparator on the committed and fresh
     render.f32 — the verdict must be PASS.

Exit codes: 0 PASS (reproducible), 1 FAIL (metadata disagreement,
comparator FAIL, or harness FAIL), 2 NOT_RUN (harness could not run:
oracle/clone absent; committed evidence missing). A missing or silent
render can never become a pass here. This record establishes
software-reference reproducibility only — no fidelity or listening
claim (reference/oracle-protocol.md section 4). Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import compare  # noqa: E402

RENDER_REFERENCE = REPO / "tools" / "render_reference.py"
DEFAULT_EVIDENCE = REPO / "reference" / "evidence"

METADATA_FIELDS = (
    "fixture",
    "sample_rate",
    "output.samples",
    "output.bytes",
    "output.sha256",
    "output.format",
    "inputs.voice.sha256",
    "inputs.events.sha256",
    "oracle_identity.binary.sha256",
)


def _get(meta: dict, dotted: str):
    node = meta
    for part in dotted.split("."):
        if not isinstance(node, dict) or part not in node:
            raise KeyError(dotted)
        node = node[part]
    return node


def check_metadata(committed: dict, fresh: dict) -> list[str]:
    """Return localized disagreements between the two render.json dicts.

    Determinism flags must be True in both (a harness that did not prove
    its own double-render determinism is not reproducible evidence).
    """
    disagreements = []
    for field in METADATA_FIELDS:
        try:
            have = _get(committed, field)
            want = _get(fresh, field)
        except KeyError:
            disagreements.append(f"{field}: missing from a render.json")
            continue
        if have != want:
            disagreements.append(
                f"{field}: committed {have!r} != fresh {want!r}")
    for side, meta in (("committed", committed), ("fresh", fresh)):
        flag = _get(meta, "determinism.byte_identical")
        if flag is not True:
            disagreements.append(
                f"{side}.determinism.byte_identical: {flag!r} is not True")
    return disagreements


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def run_repro_check(fixture: str, evidence_root: Path) -> tuple[int, dict]:
    committed_dir = evidence_root / fixture
    committed_f32 = committed_dir / "render.f32"
    committed_json = committed_dir / "render.json"
    if not committed_f32.is_file() or not committed_json.is_file():
        return 2, {
            "schema": "reproducibility-check",
            "issue": 11,
            "fixture": fixture,
            "verdict": "NOT_RUN",
            "reason": f"committed evidence missing under {committed_dir}",
        }

    with tempfile.TemporaryDirectory(prefix="r04-repro-") as tmp:
        fresh_dir = Path(tmp) / "fresh"
        proc = subprocess.run(
            [sys.executable, str(RENDER_REFERENCE),
             "--fixture", fixture, "--evidence-dir", str(fresh_dir)],
            capture_output=True, text=True)
        if proc.returncode == 2:
            return 2, {
                "schema": "reproducibility-check",
                "issue": 11,
                "fixture": fixture,
                "verdict": "NOT_RUN",
                "reason": "render harness could not run (oracle/clone "
                          "absent?): " + proc.stderr.strip(),
            }
        if proc.returncode != 0:
            return 1, {
                "schema": "reproducibility-check",
                "issue": 11,
                "fixture": fixture,
                "verdict": "FAIL",
                "reason": "render harness FAIL: " + proc.stderr.strip(),
            }

        fresh_f32 = fresh_dir / fixture / "render.f32"
        fresh_json = fresh_dir / fixture / "render.json"
        committed_meta = json.loads(
            committed_json.read_text(encoding="utf-8"))
        fresh_meta = json.loads(fresh_json.read_text(encoding="utf-8"))
        disagreements = check_metadata(committed_meta, fresh_meta)

        record = {
            "schema": "reproducibility-check",
            "issue": 11,
            "fixture": fixture,
            "tool": "tools/compare_evidence.py",
            "comparator_version": compare.VERSION,
            "committed": {
                "render.f32.sha256": sha256_file(committed_f32),
                "render.json.sha256": sha256_file(committed_json),
                "output.sha256": committed_meta["output"]["sha256"],
                "sample_rate": committed_meta["sample_rate"],
            },
            "fresh": {
                "render.f32.sha256": sha256_file(fresh_f32),
                "render.json.sha256": sha256_file(fresh_json),
                "output.sha256": fresh_meta["output"]["sha256"],
                "sample_rate": fresh_meta["sample_rate"],
                "repo_git_head": fresh_meta.get("repo_git_head"),
            },
            "metadata_fields_checked": list(METADATA_FIELDS),
            "metadata_disagreements": disagreements,
        }
        if disagreements:
            record["verdict"] = "FAIL"
            record["reason"] = "render.json metadata disagreement"
            return 1, record

        report = compare.compare_streams(
            committed_f32.read_bytes(), fresh_f32.read_bytes(),
            sample_rate=committed_meta["sample_rate"])
        record["comparator"] = {
            "verdict": report.verdict,
            "verdict_reason": report.verdict_reason,
            "byte_identical": report.byte_identical,
            "sample_count": report.n_samples_a,
        }
        if not report.passed:
            record["verdict"] = "FAIL"
            record["reason"] = (
                f"comparator verdict {report.verdict} "
                f"({report.verdict_reason})")
            return 1, record

        record["verdict"] = "PASS"
        record["reason"] = ("fresh re-render byte-matches committed "
                            "evidence under the pinned oracle")
        return 0, record


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Compare committed render evidence against a fresh "
                    "re-render (R04 reproducibility)")
    parser.add_argument("--fixture", default="smoke",
                        help="fixture name under reference/evidence/ "
                             "(default: smoke)")
    parser.add_argument("--evidence-root", type=Path,
                        default=DEFAULT_EVIDENCE,
                        help=f"evidence root (default: {DEFAULT_EVIDENCE})")
    parser.add_argument("--json", type=Path, default=None,
                        help="write the full reproducibility record here")
    args = parser.parse_args(argv)

    rc, record = run_repro_check(args.fixture, args.evidence_root)
    if args.json is not None:
        try:
            args.json.write_text(
                json.dumps(record, indent=2, sort_keys=True) + "\n",
                encoding="utf-8")
        except OSError as exc:
            print(f"compare_evidence: NOT_RUN — cannot write {args.json}: "
                  f"{exc}", file=sys.stderr)
            return 2
    status = record.get("verdict", "NOT_RUN")
    print(f"compare_evidence: {status} — fixture {args.fixture}")
    for key in ("reason", "metadata_disagreements"):
        if record.get(key):
            print(f"  {key}: {record[key]}")
    if "comparator" in record:
        print(f"  comparator: {record['comparator']['verdict']} "
              f"({record['comparator']['verdict_reason']}), "
              f"byte_identical={record['comparator']['byte_identical']}")
    if args.json is not None:
        print(f"  record: {args.json}")
    return rc


if __name__ == "__main__":
    sys.exit(main())
