#!/usr/bin/env python3
"""Artifact-manifest gate (issue #178).

Bulky evidence bytes live in the fleet artifact store; git keeps only a
manifest per run under manifests/artifacts/ naming each object by sha256
(`klt artifact`, evidence private / manifests public).

  check   (default; what tools/check_all.sh and CI run) -- schema-only, NOT
          hash-verified. Fails on: a malformed manifest, a `uri` that is not
          `artifact:<key>` with <key> ending in the entry's sha256, any
          `s3://` or `arn:aws:` string anywhere in a manifest, duplicate
          entry names, or an entry whose `name` is also a git-tracked path.
  verify  needs the broker ($KLT_ARTIFACT_BROKER) and `klt`; it runs
          `klt artifact verify` per manifest. Without a broker it is NOT_RUN
          (exit 2), never a pass. Worker-side only: CI has no broker.

Exit codes: 0 pass, 1 check failure, 2 could-not-run. Stdlib only.
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402

REPO_ROOT = os.path.dirname(_TOOLS_DIR)
MANIFEST_DIR = os.path.join("manifests", "artifacts")
SUFFIX = ".manifest.json"

SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
FORBIDDEN = ("s3://", "arn:aws:")
ENTRY_REQUIRED = {"sha256", "size", "kind", "producer", "uri"}
ENTRY_ALLOWED = ENTRY_REQUIRED | {"name", "media_type"}
PRODUCER_KEYS = {"tool", "version", "loom.repo", "commit"}


def validate_manifest(label, text, tracked):
    """Raise CheckFailure unless the manifest text is well formed."""
    for needle in FORBIDDEN:
        if needle in text:
            raise CheckFailure(f"{label}: contains forbidden {needle!r}")
    try:
        data = json.loads(text)
    except ValueError as exc:
        raise CheckFailure(f"{label}: not valid JSON: {exc}")
    if (not isinstance(data, dict) or data.get("schema_version") != 1
            or set(data) != {"schema_version", "entries"}
            or not isinstance(data["entries"], list)):
        raise CheckFailure(
            f"{label}: top level must be exactly "
            "{schema_version: 1, entries: [...]}")
    seen = set()
    for i, entry in enumerate(data["entries"]):
        where = f"{label}: entries[{i}]"
        if not isinstance(entry, dict):
            raise CheckFailure(f"{where}: not an object")
        keys = set(entry)
        if not ENTRY_REQUIRED <= keys or not keys <= ENTRY_ALLOWED:
            raise CheckFailure(f"{where}: fields {sorted(keys)} do not match "
                               "the manifest schema")
        sha = entry["sha256"]
        if not isinstance(sha, str) or not SHA256_RE.match(sha):
            raise CheckFailure(f"{where}: sha256 is not 64 lowercase hex")
        size = entry["size"]
        if isinstance(size, bool) or not isinstance(size, int) or size < 0:
            raise CheckFailure(f"{where}: size must be a non-negative int")
        if entry["kind"] != "evidence":
            raise CheckFailure(f"{where}: kind must be 'evidence'")
        prod = entry["producer"]
        if not isinstance(prod, dict) or set(prod) != PRODUCER_KEYS:
            raise CheckFailure(f"{where}: producer must have exactly "
                               f"{sorted(PRODUCER_KEYS)}")
        uri = entry["uri"]
        if not isinstance(uri, str) or not uri.startswith("artifact:"):
            raise CheckFailure(f"{where}: uri must be 'artifact:<key>', "
                               f"got {uri!r}")
        if not uri.endswith(sha):
            raise CheckFailure(f"{where}: uri key does not end in the "
                               "entry's sha256")
        for opt in ("name", "media_type"):
            if opt in entry and not (isinstance(entry[opt], str)
                                     and entry[opt]):
                raise CheckFailure(f"{where}: {opt} must be a non-empty "
                                   "string")
        name = entry.get("name")
        if name is not None:
            if name in seen:
                raise CheckFailure(f"{where}: duplicate name {name!r}")
            seen.add(name)
            if name in tracked:
                raise CheckFailure(f"{where}: name {name!r} is also a "
                                   "git-tracked path (bytes must not be in git)")
    return len(data["entries"])


def tracked_paths(root):
    try:
        proc = subprocess.run(["git", "-C", root, "ls-files", "-z"],
                              capture_output=True, check=True)
    except (OSError, subprocess.CalledProcessError) as exc:
        raise CouldNotRun(f"cannot list tracked files in {root}: {exc}")
    return {p for p in proc.stdout.decode("utf-8", "replace").split("\0") if p}


def find_manifests(root, paths):
    if paths:
        return list(paths)
    mdir = os.path.join(root, MANIFEST_DIR)
    if not os.path.isdir(mdir):
        return []
    return sorted(os.path.join(mdir, f) for f in os.listdir(mdir)
                  if f.endswith(SUFFIX))


def run_check(root, paths):
    manifests = find_manifests(root, paths)
    tracked = tracked_paths(root)
    entries = 0
    for path in manifests:
        try:
            with open(path, "r", encoding="utf-8") as fh:
                text = fh.read()
        except OSError as exc:
            raise CouldNotRun(f"cannot read {path}: {exc}")
        entries += validate_manifest(os.path.relpath(path, root), text,
                                     tracked)
    print(f"check_artifact_manifests: PASS (schema-only, NOT hash-verified: "
          f"{len(manifests)} manifest(s), {entries} entr(ies))")


def run_verify(root, paths):
    run_check(root, paths)
    if not os.environ.get("KLT_ARTIFACT_BROKER"):
        raise CouldNotRun("KLT_ARTIFACT_BROKER unset; verify is NOT_RUN")
    klt = shutil.which("klt")
    if not klt:
        raise CouldNotRun("klt not on PATH; verify is NOT_RUN")
    for path in find_manifests(root, paths):
        proc = subprocess.run([klt, "artifact", "verify", path],
                              capture_output=True, text=True)
        sys.stdout.write(proc.stdout)
        if proc.returncode == 0:
            continue
        sys.stderr.write(proc.stderr)
        if proc.returncode == 3:
            raise CheckFailure(f"{path}: klt artifact verify found a "
                               "missing or mismatched object")
        raise CouldNotRun(f"{path}: klt artifact verify exit "
                          f"{proc.returncode} (no verdict)")
    print("check_artifact_manifests: verify PASS (hash-verified via broker)")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("args", nargs="*", metavar="[check|verify] [MANIFEST...]",
                    help="command (default check), then manifest files "
                    "(default: manifests/artifacts/*)")
    ap.add_argument("--root", default=REPO_ROOT, help=argparse.SUPPRESS)
    args = ap.parse_intermixed_args(argv)
    rest = list(args.args)
    args.command = rest.pop(0) if rest and rest[0] in ("check", "verify") \
        else "check"
    args.manifests = rest
    try:
        if args.command == "verify":
            run_verify(args.root, args.manifests)
        else:
            run_check(args.root, args.manifests)
    except CheckFailure as exc:
        print(f"check_artifact_manifests: FAIL: {exc}", file=sys.stderr)
        return 1
    except CouldNotRun as exc:
        print(f"check_artifact_manifests: NOT_RUN: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
