#!/usr/bin/env python3
"""Store a run's bulky evidence in the artifact store (issue #178).

    store_evidence.py <bench> <run-id> <staging-dir> --summary <md>

Uploads every file under <staging-dir> with `klt artifact put` and writes
manifests/artifacts/<bench>-<run-id>.manifest.json plus a Markdown summary
(name, size, sha256 per object) at --summary. Fails closed: with no
$KLT_ARTIFACT_BROKER, no `klt`, an empty staging dir, an existing manifest
(append-only), or any failed put, it exits nonzero (2 could-not-run, 1
failure) and writes neither the manifest nor the summary.

Manifests are public and evidence bytes private; this script never prints or
records the broker URL. Exit codes: 0 stored, 1 failure, 2 could-not-run.
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402
import check_artifact_manifests as cam  # noqa: E402

import json  # noqa: E402

ID_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]*$")


def staged_files(staging):
    out = []
    for dirpath, _dirs, files in os.walk(staging):
        for f in files:
            full = os.path.join(dirpath, f)
            out.append((os.path.relpath(full, staging).replace(os.sep, "/"),
                        full))
    return sorted(out)


def summary_text(bench, run_id, manifest):
    lines = [f"# Stored evidence: {bench} / {run_id}", "",
             "Bytes are in the artifact store; the manifest "
             f"`manifests/artifacts/{bench}-{run_id}.manifest.json` names "
             "each object by sha256. Fetch with `klt artifact get`.", "",
             "| name | size (bytes) | sha256 |", "|---|---|---|"]
    for e in manifest["entries"]:
        lines.append(f"| `{e['name']}` | {e['size']} | `{e['sha256']}` |")
    return "\n".join(lines) + "\n"


def store(bench, run_id, staging, summary, root):
    for label, val in (("bench", bench), ("run-id", run_id)):
        if not ID_RE.match(val):
            raise CouldNotRun(f"bad {label} {val!r}")
    if not os.environ.get("KLT_ARTIFACT_BROKER"):
        raise CouldNotRun("KLT_ARTIFACT_BROKER unset; nothing stored")
    klt = shutil.which("klt")
    if not klt:
        raise CouldNotRun("klt not on PATH; nothing stored")
    if not os.path.isdir(staging):
        raise CouldNotRun(f"staging dir {staging} does not exist")
    files = staged_files(staging)
    if not files:
        raise CouldNotRun(f"staging dir {staging} is empty")
    final = os.path.join(root, cam.MANIFEST_DIR, f"{bench}-{run_id}{cam.SUFFIX}")
    if os.path.exists(final):
        raise CouldNotRun(f"{final} exists; manifests are append-only")
    if os.path.exists(summary):
        raise CouldNotRun(f"{summary} exists; refusing to overwrite")

    with tempfile.TemporaryDirectory(prefix="store-evidence-") as tmp:
        tmp_manifest = os.path.join(tmp, "m.json")
        for name, full in files:
            proc = subprocess.run(
                [klt, "artifact", "put", "--manifest", tmp_manifest,
                 "--name", name, full],
                capture_output=True, text=True, cwd=root)
            if proc.returncode != 0:
                raise CheckFailure(f"klt artifact put failed for {name} "
                                   f"(exit {proc.returncode}): "
                                   f"{proc.stderr.strip()[:500]}")
        with open(tmp_manifest, "r", encoding="utf-8") as fh:
            text = fh.read()
        cam.validate_manifest(os.path.basename(final), text,
                              cam.tracked_paths(root))
        manifest = json.loads(text)
        have = {e.get("name") for e in manifest["entries"]}
        if have != {n for n, _ in files}:
            raise CheckFailure("manifest entries do not match staged files")
        os.makedirs(os.path.dirname(final), exist_ok=True)
        os.makedirs(os.path.dirname(os.path.abspath(summary)), exist_ok=True)
        tmp_summary = os.path.join(tmp, "summary.md")
        with open(tmp_summary, "w", encoding="utf-8") as fh:
            fh.write(summary_text(bench, run_id, manifest))
        shutil.move(tmp_manifest, final)
        shutil.move(tmp_summary, summary)
    print(f"store_evidence: stored {len(files)} object(s); manifest {final}")
    print("store_evidence: not hash-verified here; run "
          "`klt artifact verify` on a fleet worker")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("bench")
    ap.add_argument("run_id")
    ap.add_argument("staging")
    ap.add_argument("--summary", required=True)
    ap.add_argument("--root", default=cam.REPO_ROOT, help=argparse.SUPPRESS)
    args = ap.parse_args(argv)
    try:
        store(args.bench, args.run_id, args.staging, args.summary, args.root)
    except CheckFailure as exc:
        print(f"store_evidence: FAIL: {exc}", file=sys.stderr)
        return 1
    except CouldNotRun as exc:
        print(f"store_evidence: NOT_RUN: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
