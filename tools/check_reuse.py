#!/usr/bin/env python3
"""Negative control for reuse governance (DR-0002, issue #39).

Fails when:
  - a source file exists under a governed destination (src/, rtl/, tools/,
    spec/reference/tables/) without a covering adopt/adapt entry in
    docs/reuse/catalog.json (or a catalog exemption);
  - an adopted destination file's bytes drift from the recorded upstream
    SHA-256 (adopt) or the recorded adapted SHA-256 (adapt);
  - an adapt destination exists without a recorded adapted SHA-256 (a
    landed import must carry its provenance record in the same PR);
  - a reject/reference-only/external-oracle-only entry declares a
    destination, or the catalog's hashes disagree with its path lists;
  - (with --verify-upstream) a recorded hash disagrees with the bytes at the
    pinned upstream commit in a local sibling checkout.

Hash equality establishes byte identity only -- it never qualifies a
component. Exit codes: 0 pass, 1 check failure, 2 could-not-run.
Stdlib only.
"""

import argparse
import hashlib
import json
import os
import subprocess
import sys

GOVERNED_DIRS = ("src", "rtl", "tools", "spec/reference/tables")
SOURCE_EXTENSIONS = {
    ".py", ".v", ".sv", ".vh", ".svh", ".tcl", ".sh", ".mk",
    ".sdc", ".lpf", ".c", ".h", ".cpp", ".hpp", ".f",
}
TABLES_DIR = "spec/reference/tables"
IMPORT_RULINGS = ("adopt", "adapt")
VALID_RULINGS = ("adopt", "adapt", "reject", "reference-only",
                 "external-oracle-only")


class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def load_catalog(repo_root, catalog_path):
    if not os.path.isfile(catalog_path):
        raise CouldNotRun(f"catalog not found: {catalog_path}")
    try:
        with open(catalog_path, "r", encoding="utf-8") as f:
            catalog = json.load(f)
    except (OSError, json.JSONDecodeError) as exc:
        raise CouldNotRun(f"catalog unreadable: {exc}")
    if not isinstance(catalog, dict) or \
            not isinstance(catalog.get("components"), list):
        raise CouldNotRun("catalog has no components list")
    return catalog


def iter_source_files(repo_root, exempt):
    """Yield governed source files as repo-relative paths."""
    for gov in GOVERNED_DIRS:
        base = os.path.join(repo_root, gov)
        for dirpath, dirnames, filenames in os.walk(base):
            dirnames[:] = sorted(d for d in dirnames
                                 if d != "__pycache__" and not d.startswith("."))
            for name in sorted(filenames):
                if name.startswith("."):
                    continue
                rel = os.path.relpath(os.path.join(dirpath, name), repo_root)
                rel = rel.replace(os.sep, "/")
                if rel in exempt:
                    continue
                ext = os.path.splitext(name)[1].lower()
                # the tables directory governs every file: tables are bytes too
                if rel.startswith(TABLES_DIR + "/") or ext in SOURCE_EXTENSIONS:
                    yield rel


def destination_matches(dest, rel):
    if dest.endswith("/"):
        return rel.startswith(dest)
    return dest == rel


def upstream_for(entry, basename):
    hits = [p for p in entry["upstream"]["path"]
            if os.path.basename(p) == basename]
    if len(hits) != 1:
        raise CheckFailure(
            f"{entry['component']}: destination file {basename!r} does not map "
            f"to exactly one upstream path (found {len(hits)})")
    return hits[0]


def check_catalog_schema(components):
    seen = set()
    for entry in components:
        name = entry.get("component", "<unnamed>")
        if not name or name in seen:
            raise CheckFailure(f"duplicate or missing component name: {name!r}")
        seen.add(name)
        ruling = entry.get("ruling")
        if ruling not in VALID_RULINGS:
            raise CheckFailure(f"{name}: invalid ruling {ruling!r}")
        upstream = entry.get("upstream") or {}
        paths = upstream.get("path")
        sha = entry.get("sha256")
        if not isinstance(paths, list) or not isinstance(sha, dict):
            raise CheckFailure(f"{name}: upstream.path / sha256 malformed")
        if sorted(paths) != sorted(sha):
            raise CheckFailure(
                f"{name}: sha256 keys do not match upstream paths")
        for p, digest in sha.items():
            if not (isinstance(digest, str) and len(digest) == 64
                    and all(ch in "0123456789abcdef" for ch in digest)):
                raise CheckFailure(f"{name}: bad sha256 for {p}")
        dest = entry.get("destination")
        if not isinstance(dest, list) or \
                any(not isinstance(d, str) or d.startswith("/") or ".." in d.split("/")
                    for d in dest):
            raise CheckFailure(f"{name}: destination malformed")
        if ruling not in IMPORT_RULINGS and dest:
            raise CheckFailure(
                f"{name}: ruling {ruling!r} must not declare destinations "
                f"(non-import components cannot own governed paths)")
        adapted = entry.get("adapted_sha256", {})
        if ruling == "adapt" and adapted and \
                not set(adapted) <= set(dest):
            raise CheckFailure(
                f"{name}: adapted_sha256 keys outside declared destinations")


def check_coverage(files, components):
    covered = {}
    for entry in components:
        if entry["ruling"] not in IMPORT_RULINGS:
            continue
        for dest in entry["destination"]:
            covered[dest] = entry["component"]
    unmatched = []
    for rel in files:
        if not any(destination_matches(dest, rel) for dest in covered):
            unmatched.append(rel)
    if unmatched:
        raise CheckFailure(
            "source file(s) under governed destinations with no covering "
            "adopt/adapt catalog entry (import without provenance record?): "
            + ", ".join(sorted(unmatched)))


def check_hashes(repo_root, components):
    for entry in components:
        if entry["ruling"] not in IMPORT_RULINGS:
            continue
        name = entry["component"]
        ruling = entry["ruling"]
        adapted = entry.get("adapted_sha256", {})
        for dest in entry["destination"]:
            if dest.endswith("/"):
                base = os.path.join(repo_root, dest)
                if not os.path.isdir(base):
                    continue
                for dirpath, _dirnames, filenames in os.walk(base):
                    for fn in sorted(filenames):
                        rel = os.path.relpath(
                            os.path.join(dirpath, fn), repo_root)
                        rel = rel.replace(os.sep, "/")
                        check_one(repo_root, entry, ruling, adapted, rel, name)
            else:
                if os.path.isfile(os.path.join(repo_root, dest)):
                    check_one(repo_root, entry, ruling, adapted, dest, name)


def check_one(repo_root, entry, ruling, adapted, rel, name):
    actual = sha256_file(os.path.join(repo_root, rel))
    if ruling == "adopt":
        expected = entry["sha256"].get(upstream_for(entry, os.path.basename(rel)))
        if actual != expected:
            raise CheckFailure(
                f"{name}: adopted file {rel} drifted from the pinned "
                f"upstream bytes (hash mismatch)")
    else:  # adapt
        expected = adapted.get(rel)
        if expected is None:
            raise CheckFailure(
                f"{name}: adapt destination {rel} exists without a recorded "
                f"adapted sha256; add provenance to docs/reuse/catalog.json "
                f"in the import PR")
        if actual != expected:
            raise CheckFailure(
                f"{name}: adapted file {rel} drifted from its recorded "
                f"adapted sha256; update the catalog in the same PR")


def check_originals(repo_root, originals):
    """Byte-pin locally authored governed files (judge follow-up on PR #44).

    Exemptions alone would pass any file once listed; originals carry a
    recorded sha256 that must keep matching, so local code stays inside the
    drift-detection coverage set.
    """
    for entry in originals:
        rel = entry["path"]
        path = os.path.join(repo_root, rel)
        if not os.path.isfile(path):
            raise CheckFailure(f"original {rel} is byte-pinned in the catalog "
                               f"but missing from the tree")
        actual = sha256_file(path)
        if actual != entry.get("sha256"):
            raise CheckFailure(
                f"original {rel} drifted from its recorded sha256; update "
                f"docs/reuse/catalog.json in the same PR")


def find_sibling_checkout(repo_root, repo_name, commit):
    name = repo_name.rstrip("/").split("/")[-1]
    roots = [repo_root + "/..", repo_root + "/../2amlogic"]
    env = os.environ.get("GF180_SIBLING_ROOTS")
    if env:
        roots = env.split(os.pathsep) + roots
    for root in roots:
        candidate = os.path.normpath(os.path.join(root, name))
        if not os.path.isdir(os.path.join(candidate, ".git")):
            continue
        probe = subprocess.run(
            ["git", "-C", candidate, "cat-file", "-e", commit + "^{commit}"],
            capture_output=True)
        if probe.returncode == 0:
            return candidate
    return None


def verify_upstream(repo_root, components):
    checked, skipped = 0, 0
    for entry in components:
        commit = entry["upstream"].get("commit")
        if not commit or not entry["sha256"]:
            continue
        repo = entry["upstream"]["repo"]
        checkout = find_sibling_checkout(repo_root, repo, commit)
        if checkout is None:
            print(f"  upstream not locally available, skipped: {repo} @ {commit[:12]}")
            skipped += 1
            continue
        for path, digest in sorted(entry["sha256"].items()):
            out = subprocess.run(
                ["git", "-C", checkout, "show", f"{commit}:{path}"],
                capture_output=True)
            if out.returncode != 0:
                raise CheckFailure(
                    f"{entry['component']}: pinned path {path} unreadable in "
                    f"{checkout} at {commit[:12]}")
            actual = hashlib.sha256(out.stdout).hexdigest()
            if actual != digest:
                raise CheckFailure(
                    f"{entry['component']}: catalog hash for {path} does not "
                    f"match bytes at {repo}@{commit[:12]} (catalog drift)")
            checked += 1
    print(f"  upstream hashes verified against pinned commits: {checked} file(s); "
          f"checkouts unavailable: {skipped} component(s)")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--repo-root", default=os.path.dirname(
        os.path.dirname(os.path.abspath(__file__))))
    parser.add_argument("--catalog", default=None,
                        help="catalog path (default <repo-root>/docs/reuse/catalog.json)")
    parser.add_argument("--verify-upstream", action="store_true",
                        help="also re-hash pinned files from local sibling checkouts")
    args = parser.parse_args(argv)
    repo_root = os.path.abspath(args.repo_root)
    catalog_path = args.catalog or os.path.join(
        repo_root, "docs", "reuse", "catalog.json")

    try:
        catalog = load_catalog(repo_root, catalog_path)
        components = catalog["components"]
        originals = catalog.get("local_originals", [])
        exempt = {e["path"] for e in originals}
        exempt |= {e["path"] for e in catalog.get("local_files_exempt", [])}
        check_catalog_schema(components)
        files = list(iter_source_files(repo_root, exempt))
        check_coverage(files, components)
        check_hashes(repo_root, components)
        check_originals(repo_root, originals)
        if args.verify_upstream:
            verify_upstream(repo_root, components)
    except CheckFailure as exc:
        print(f"FAIL: {exc}")
        return 1
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}")
        return 2

    print(f"reuse check PASS: {len(components)} catalog components, "
          f"{len(originals)} byte-pinned originals, "
          f"governed destinations clean")
    return 0


if __name__ == "__main__":
    sys.exit(main())
