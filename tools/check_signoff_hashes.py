#!/usr/bin/env python3
"""Signoff item-9/10 hash-drift gate (issue #260).

`klt signoff --attest` writes a per-file sha256 `content_hash` into
signoff/evidence/item9.json (the six rtl/tb_*.v testbenches) and
item10.json (README.md, LICENSE, .github/workflows/ci.yml). Editing one
of those files without regenerating the matching envelope in the same
change makes CI's `signoff-fresh` job report `status: drifted` only
after it has built iverilog and Verilator (the #259 round-trip). This
check catches that class of drift locally, in seconds, with no
toolchain: for every attestation.checks[] entry with check == "present"
and a content_hash, it recomputes sha256 over the file's raw bytes and
compares.

Scope: hygiene/testbench-file hash drift ONLY. The envelopes also
record command results, pins and the inventory hash, which this check
does not recompute -- a PASS here is not a `signoff-fresh` pass and
must never be reported as one.

On drift the message names the file, the committed and current hashes,
and the fix (signoff/README.md -> "Regenerating"); the command block is
not copied here so that document stays the one source of truth.

Exit codes: 0 pass, 1 check failure, 2 could-not-run
(tools/verdict_errors.py). Stdlib only.
"""

import argparse
import hashlib
import json
import os
import sys

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from verdict_errors import CheckFailure, CouldNotRun  # noqa: E402

REPO_ROOT = os.path.dirname(_TOOLS_DIR)
ENVELOPES = ("signoff/evidence/item9.json", "signoff/evidence/item10.json")
HASH_ALGORITHM = "sha256-raw-bytes"
REMEDY = 'regenerate the envelope: signoff/README.md -> "Regenerating"'


def load_checks(repo_root, rel):
    """-> the checks[] list of one attestation envelope.

    An absent or unparseable envelope, a missing attestation block, or a
    hash_algorithm this checker cannot recompute (its whole premise is
    plain sha256 over raw bytes) is CouldNotRun, never a silent pass.
    """
    path = os.path.join(repo_root, rel)
    if not os.path.isfile(path):
        raise CouldNotRun(f"{rel} not found under {repo_root}")
    try:
        with open(path, "r", encoding="utf-8") as fh:
            envelope = json.load(fh)
    except (OSError, json.JSONDecodeError) as exc:
        raise CouldNotRun(f"{rel} is not readable JSON: {exc}")
    if not isinstance(envelope, dict):
        raise CouldNotRun(f"{rel}: top level is not a JSON object")
    attestation = envelope.get("attestation")
    if not isinstance(attestation, dict):
        raise CouldNotRun(f"{rel}: no attestation block")
    algo = attestation.get("hash_algorithm")
    if algo != HASH_ALGORITHM:
        raise CouldNotRun(
            f"{rel}: hash_algorithm is {algo!r}, not {HASH_ALGORITHM!r}; "
            "this checker only knows how to recompute sha256 over raw "
            "bytes, so it cannot verify this envelope")
    checks = attestation.get("checks")
    if not isinstance(checks, list):
        raise CouldNotRun(f"{rel}: attestation.checks is not a list")
    return checks


def check_envelope(repo_root, rel):
    """-> (reasons, compared) for one envelope. Every check == "present"
    entry with a content_hash is compared; a present entry without a
    hash is a stated failure, never a silent pass. command/pin entries
    carry no content hash and are skipped without a verdict."""
    reasons = []
    compared = 0
    for i, entry in enumerate(load_checks(repo_root, rel)):
        if not isinstance(entry, dict):
            raise CouldNotRun(f"{rel}: checks[{i}] is not a JSON object")
        if entry.get("check") != "present":
            continue
        path = entry.get("path")
        committed = entry.get("content_hash")
        if not isinstance(path, str) or not path:
            reasons.append(
                f"{rel}: checks[{i}] is a present check with no path; "
                f"cannot verify ({REMEDY})")
            continue
        if not isinstance(committed, str) or not committed:
            reasons.append(
                f"{rel}: {path} is a present check with no content_hash; "
                f"cannot verify ({REMEDY})")
            continue
        compared += 1
        try:
            with open(os.path.join(repo_root, path), "rb") as fh:
                current = "sha256:" + hashlib.sha256(fh.read()).hexdigest()
        except OSError as exc:
            reasons.append(
                f"{rel}: {path} is missing or unreadable ({exc}); the "
                f"committed envelope records {committed} ({REMEDY})")
            continue
        if current != committed:
            reasons.append(
                f"{rel}: {path} drifted: committed {committed} vs current "
                f"{current}; the file moved and the envelope did not "
                f"({REMEDY})")
    return reasons, compared


def run_check(repo_root):
    reasons = []
    compared = 0
    for rel in ENVELOPES:
        r, c = check_envelope(repo_root, rel)
        reasons += r
        compared += c
    if reasons:
        raise CheckFailure("\n  ".join(reasons))
    if compared == 0:
        raise CouldNotRun(
            "no check=present entry with a content_hash found in "
            f"{', '.join(ENVELOPES)}; nothing to compare")
    print(f"check_signoff_hashes: PASS ({compared} hashed path(s) across "
          f"{len(ENVELOPES)} envelope(s) match; detects hygiene/testbench "
          "hash drift only -- NOT a signoff-fresh pass)")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--repo-root", default=REPO_ROOT,
                        help="repo root (default: the checkout holding "
                        "this script)")
    args = parser.parse_args(argv)
    repo_root = os.path.abspath(args.repo_root)
    try:
        run_check(repo_root)
    except CheckFailure as exc:
        print(f"check_signoff_hashes: FAIL: {exc}", file=sys.stderr)
        return 1
    except CouldNotRun as exc:
        print(f"check_signoff_hashes: NOT_RUN: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
