#!/usr/bin/env python3
"""Recompute derived byte pins after a merge conflict or a tool edit (issue #326).

Usage:
  tools/repin.py            re-pin docs/reuse/catalog.json sha256 values from the
                            tree; report stale evidence/capabilities/rtl-core.json
                            coverage entries (report only, never edited here);
                            an absent record reports NOT_RUN, never "0 stale"
  tools/repin.py --stamp    additionally run
                            `tools/compile_capabilities.py --stamp rtl-core`
                            (~2 min: re-runs the benches and negative controls)

Only the 64-hex digest inside each pinned line is rewritten; every other byte
of the catalog (including each `reason`) is left alone. Upstream pins
(`components[].sha256`) are never touched: they hash upstream bytes, not this
tree. `--stamp` never fabricates a stamp: its exit status is the delegate's.
A fast-lane result is not a stamp.

Exit: 0 done (with --stamp: stamp PASS); the delegate's non-zero code is
propagated when the stamp does not PASS; 2 could not run.
"""

import argparse
import json
import os
import re
import subprocess
import sys

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from sha256_helpers import sha256_file  # noqa: E402
from verdict_errors import CouldNotRun  # noqa: E402

CATALOG = os.path.join("docs", "reuse", "catalog.json")
STAMP_RECORD = os.path.join("evidence", "capabilities", "rtl-core.json")
STAMP_NODE = "rtl-core"


def _pins(catalog):
    """Yield (kind, path, recorded_digest) for every tree-hash pin."""
    for entry in catalog.get("local_originals", []):
        yield "original", entry["path"], entry["sha256"]
    for comp in catalog.get("components", []):
        for rel, digest in comp.get("adapted_sha256", {}).items():
            yield "adapted", rel, digest


def repin_catalog(root):
    """Rewrite drifted digests in place; return [(path, old, new)]."""
    path = os.path.join(root, CATALOG)
    if not os.path.isfile(path):
        raise CouldNotRun(f"catalog not found: {path}")
    with open(path, "r", encoding="utf-8", newline="") as f:
        raw = f.read()
    try:
        catalog = json.loads(raw)
    except json.JSONDecodeError as exc:
        raise CouldNotRun(f"catalog unreadable (unresolved conflict?): {exc}")
    changed = []
    for kind, rel, old in _pins(catalog):
        full = os.path.join(root, rel)
        if not os.path.isfile(full):
            continue  # deleted/moved: a human decision, never auto-dropped
        new = sha256_file(full)
        if new == old:
            continue
        if kind == "original":
            pat = re.compile(r'("path":\s*"%s",\s*"sha256":\s*")%s(")'
                             % (re.escape(rel), old))
        else:
            pat = re.compile(r'("%s":\s*")%s(")' % (re.escape(rel), old))
        raw, n = pat.subn(lambda m: m.group(1) + new + m.group(2), raw)
        if n != 1:
            raise CouldNotRun(f"{rel}: expected exactly one pin line, "
                              f"found {n}; edit the catalog by hand")
        changed.append((rel, old, new))
    if changed:
        with open(path, "w", encoding="utf-8", newline="") as f:
            f.write(raw)
    return changed


def stale_coverage(root):
    """Paths whose rtl-core coverage digest no longer matches the tree.

    Returns None when the stamp record is absent: a check that did not
    run is NOT_RUN, never "0 stale entries" (CLAUDE.md reporting rule).
    """
    path = os.path.join(root, STAMP_RECORD)
    if not os.path.isfile(path):
        return None
    try:
        with open(path, "r", encoding="utf-8") as f:
            coverage = json.load(f).get("coverage", {})
    except json.JSONDecodeError as exc:
        raise CouldNotRun(f"{STAMP_RECORD} unreadable "
                          f"(unresolved conflict?): {exc}")
    stale = []
    for rel, digest in sorted(coverage.items()):
        full = os.path.join(root, rel)
        if not os.path.isfile(full) or sha256_file(full) != digest:
            stale.append(rel)
    return stale


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", default=os.path.dirname(_TOOLS_DIR))
    parser.add_argument("--stamp", action="store_true",
                        help="run compile_capabilities.py --stamp rtl-core")
    args = parser.parse_args(argv)
    root = os.path.abspath(args.root)
    try:
        # Read-only check first: a failure here must not leave the catalog
        # half-written with no record of what changed.
        stale = stale_coverage(root)
        changed = repin_catalog(root)
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}", file=sys.stderr)
        return 2
    for rel, old, new in changed:
        print(f"repinned {rel}: {old[:12]} -> {new[:12]}")
    print(f"catalog: {len(changed)} pin(s) updated")
    if stale is None:
        print(f"rtl-core coverage: NOT_RUN "
              f"(no {STAMP_RECORD} record; coverage not checked)")
    else:
        print(f"rtl-core coverage: {len(stale)} stale entr"
              f"{'y' if len(stale) == 1 else 'ies'}"
              + ("" if not stale else " (stamp required)"))
        for rel in stale:
            print(f"  stale: {rel}")
    if not args.stamp:
        return 0
    cmd = [sys.executable, os.path.join(root, "tools", "compile_capabilities.py"),
           "--root", root, "--stamp", STAMP_NODE]
    rc = subprocess.run(cmd).returncode
    print(f"stamp {STAMP_NODE}: {'PASS' if rc == 0 else f'FAIL (exit {rc})'}")
    return rc


if __name__ == "__main__":
    sys.exit(main())
