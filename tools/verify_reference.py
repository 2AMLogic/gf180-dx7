#!/usr/bin/env python3
"""R01 verifier: reject drift of the pinned Dexed reference (issue #6).

Loads reference/manifest.json, re-hashes every pinned file from a local Dexed
clone, checks the clone is exactly at the pinned commit, and checks the pinned
toolchain. Stdlib-only.

Exit codes:
  0  PASS   pinned commit, every file hash, and toolchain match
  1  FAIL   drift: commit mismatch, file missing/modified, or toolchain
            mismatch; the first mismatch is reported explicitly
  2  NOT_RUN environment/usage error: missing/unusable --dexed-root, missing
            git, or invalid manifest
"""

import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_MANIFEST = REPO_ROOT / "reference" / "manifest.json"
DEFAULT_DEXED_ROOT = "/Users/joseph/dev/dexed"

EXIT_OK = 0
EXIT_DRIFT = 1
EXIT_ENV = 2

COMMIT_RE = re.compile(r"^[0-9a-f]{40}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
REQUIRED_MANIFEST_KEYS = ("manifest_version", "upstream", "engine", "files",
                          "toolchain", "drift_policy")
REQUIRED_FILE_KEYS = ("path", "sha256", "license_class", "license_basis",
                      "role")


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def load_manifest(manifest_path):
    """Return (manifest, error). error is a user-facing string or None."""
    try:
        with open(manifest_path, "r", encoding="utf-8") as fh:
            manifest = json.load(fh)
    except OSError as exc:
        return None, f"cannot read manifest {manifest_path}: {exc}"
    except json.JSONDecodeError as exc:
        return None, f"manifest {manifest_path} is not valid JSON: {exc}"

    for key in REQUIRED_MANIFEST_KEYS:
        if key not in manifest:
            return None, f"manifest is missing required key: {key}"

    commit = manifest.get("upstream", {}).get("commit", "")
    if not COMMIT_RE.match(commit):
        return None, f"upstream.commit is not a 40-hex sha: {commit!r}"

    files = manifest.get("files")
    if not isinstance(files, list) or not files:
        return None, "manifest files must be a non-empty list"

    seen = set()
    for entry in files:
        for key in REQUIRED_FILE_KEYS:
            if key not in entry:
                return None, f"file entry missing required key {key!r}: {entry}"
        if not SHA256_RE.match(entry["sha256"]):
            return None, f"bad sha256 for {entry['path']}: {entry['sha256']!r}"
        if entry["path"] in seen:
            return None, f"duplicate pinned path: {entry['path']}"
        seen.add(entry["path"])

    return manifest, None


def git_head(root):
    """Return (head_sha, error)."""
    try:
        proc = subprocess.run(
            ["git", "-C", str(root), "rev-parse", "HEAD"],
            capture_output=True, text=True, check=True)
    except FileNotFoundError:
        return None, "git executable not found"
    except subprocess.CalledProcessError as exc:
        detail = (exc.stderr or exc.stdout or "").strip().splitlines()
        msg = detail[0] if detail else f"git exited {exc.returncode}"
        return None, f"{root} is not a usable git clone: {msg}"
    return proc.stdout.strip(), None


def verify_commit(root, pinned_commit):
    """Return (mismatch_or_None, head)."""
    head, err = git_head(root)
    if err:
        return None, None  # environment error handled by caller
    if head != pinned_commit:
        return (f"commit drift: {root} HEAD is {head}, "
                f"manifest pins {pinned_commit}"), head
    return None, head


def verify_hashes(root, pinned_files):
    """Re-hash every pinned file under root.

    Returns a list of mismatch dicts; empty means every file matches.
    (Pure function of root contents — used directly by the negative control.)
    """
    mismatches = []
    for entry in pinned_files:
        path = Path(root) / entry["path"]
        if not path.is_file():
            mismatches.append({"path": entry["path"], "kind": "missing",
                               "expected": entry["sha256"], "actual": None})
            continue
        actual = sha256_file(path)
        if actual != entry["sha256"]:
            mismatches.append({"path": entry["path"], "kind": "hash",
                               "expected": entry["sha256"], "actual": actual})
    return mismatches


def verify_toolchain(toolchain):
    """Return a list of toolchain mismatch strings (empty == match)."""
    mismatches = []
    cxx = toolchain.get("cxx_compiler", {})
    pinned_line = cxx.get("version_line")
    if pinned_line:
        try:
            proc = subprocess.run(
                [cxx.get("command", "clang++"), "--version"],
                capture_output=True, text=True, check=True)
        except (FileNotFoundError, subprocess.CalledProcessError) as exc:
            mismatches.append(f"toolchain drift: cannot run pinned compiler "
                              f"{cxx.get('command', 'clang++')!r}: {exc}")
            return mismatches
        first_line = proc.stdout.splitlines()[0].strip() if proc.stdout else ""
        if first_line != pinned_line:
            mismatches.append(
                "toolchain drift: compiler version line is "
                f"{first_line!r}, manifest pins {pinned_line!r}")
    return mismatches


def report(commit_mismatch, hash_mismatches, tool_mismatches):
    all_problems = []
    if commit_mismatch:
        all_problems.append(commit_mismatch)
    all_problems.extend(hash_mismatches)
    all_problems.extend(tool_mismatches)
    first = all_problems[0]
    if isinstance(first, dict):
        print(f"FIRST MISMATCH: {first['kind']}: {first['path']}")
    else:
        print(f"FIRST MISMATCH: {first}")
    for problem in all_problems:
        if isinstance(problem, dict):
            print(f"  DRIFT {problem['kind']}: {problem['path']}")
            print(f"    expected sha256: {problem['expected']}")
            print(f"    actual:   {problem['actual']}")
        else:
            print(f"  DRIFT: {problem}")
    print(f"verify_reference: FAIL — {len(all_problems)} mismatch(es); "
          f"refusing to follow drifted reference")


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Verify the local Dexed clone against reference/manifest.json")
    parser.add_argument("--manifest", default=str(DEFAULT_MANIFEST),
                        help=f"path to manifest (default: {DEFAULT_MANIFEST})")
    parser.add_argument("--dexed-root", default=DEFAULT_DEXED_ROOT,
                        help=f"local Dexed clone (default: {DEFAULT_DEXED_ROOT})")
    args = parser.parse_args(argv)

    root = Path(args.dexed_root)
    if not root.is_dir():
        print(f"verify_reference: NOT_RUN — dexed root does not exist: {root}",
              file=sys.stderr)
        return EXIT_ENV

    manifest, err = load_manifest(args.manifest)
    if err:
        print(f"verify_reference: NOT_RUN — {err}", file=sys.stderr)
        return EXIT_ENV

    commit = manifest["upstream"]["commit"]
    files = manifest["files"]

    commit_mismatch, head = verify_commit(root, commit)
    if head is None and commit_mismatch is None:
        print("verify_reference: NOT_RUN — could not read git HEAD of "
              f"{root} (see prior message)", file=sys.stderr)
        return EXIT_ENV

    hash_mismatches = verify_hashes(root, files)
    tool_mismatches = verify_toolchain(manifest["toolchain"])

    problems = [commit_mismatch] + hash_mismatches + tool_mismatches
    if any(problems):
        report(commit_mismatch, hash_mismatches, tool_mismatches)
        return EXIT_DRIFT

    print(f"verify_reference: PASS — {root} at pinned commit {commit[:12]}, "
          f"{len(files)} files hash-match, toolchain matches")
    return EXIT_OK


if __name__ == "__main__":
    sys.exit(main())
