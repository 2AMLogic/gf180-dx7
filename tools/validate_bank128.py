#!/usr/bin/env python3
"""Validate a U03 curated 128-patch bank manifest (issue #36).

This is the executable acceptance check for the U03 bank manifest schema
`gf180-dx7.bank128/1` (policy: docs/BANK128-POLICY.md). It validates a
manifest; it does not select, audition, or author one. Curated content comes
from human listening (U02, issue #35, then U03 expansion) — this tool only
decides whether a proposed manifest is complete, provenanced, hash-unique
(or alias-declared) and pinned to the committed P02 catalog.

Checks (zip-free, stdlib-only):
  - schema id and a revision integer (the bank is versioned independently
    of RTL milestones; nothing here references RTL);
  - exactly 128 entries with indices 0..127;
  - per entry: well-formed canonical hash (unhashable entries fail), name,
    category from the U02 family vocabulary, source/authorship, audition
    notes + listening-record reference + `selected` verdict, complete
    performance settings, and an *established* distribution status with a
    stated basis (unknown/unclear status fails: the stop/escalate rule says
    exclude and record the reason instead);
  - archive-sourced entries resolve in the committed P02 catalog at their
    own (path, slot) with the same canonical hash (unprovenanced entries
    fail);
  - duplicates: entries sharing a canonical hash fail unless every later
    member declares `duplicate_of` (the first member's index) and a
    non-empty `alias_note`; a `duplicate_of` that points at a different
    hash fails. Catalog alias names for each hash are reported;
  - variety: every U02 family is represented, and the recorded coverage
    table matches an independent recount;
  - exclusions list present, every exclusion carries a reason;
  - catalog pin: snapshot SHA-256 and archive identity match the committed
    catalog; drift makes the manifest STALE.

Canonical-hash identity catches exact parameter duplicates and renamed
aliases only. It cannot detect near-duplicates that differ by one
parameter; those remain a listening-record judgment (see policy doc).

Exit codes: 0 PASS, 1 FAIL or STALE, 2 NOT_RUN (manifest or catalog
missing). A run that did not happen is never reported as a pass.
"""

from __future__ import annotations

import argparse
import datetime as _dt
import hashlib
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
DEFAULT_CATALOG = REPO / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
DEFAULT_MANIFEST = REPO / "corpus" / "bank128.json"

SCHEMA = "gf180-dx7.bank128/1"
BANK_SIZE = 128

# U02 family vocabulary (tools/u02_candidates.py FAMILY_SPECS keys, same
# order). tests/test_bank128.py asserts the two stay identical.
CATEGORIES: tuple[str, ...] = (
    "ep-tine", "bass", "bell-mallet", "organ", "brass-reed", "pluck",
    "pad-string", "perc-texture",
)

SOURCE_KINDS = ("archive", "original")
VERDICT_SELECTED = "selected"
# An established status is required; anything else (missing, "unknown",
# "unclear", ...) fails and must be moved to `exclusions` with a reason.
DISTRIBUTION_STATUSES = ("redistributable", "restricted")

POLY_MONO = ("poly", "mono")
PORTAMENTO_MODES = ("retain", "follow", "fingered", "full-time")
CONTROLLERS = ("mod_wheel", "foot_control", "breath_control", "aftertouch")
CONTROLLER_TARGETS = ("pitch", "amplitude", "eg_bias")

SHA64 = re.compile(r"^[0-9a-f]{64}$")
BAD_STRINGS = {"", "tbd", "todo", "placeholder", "null", "none", "n/a", "?"}


class NotRun(Exception):
    """Environment problem: an input needed for validation is absent."""


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _text(value) -> bool:
    """True for a real, non-placeholder string."""
    return isinstance(value, str) and value.strip().lower() not in BAD_STRINGS


def _int_in(value, lo: int, hi: int) -> bool:
    return (isinstance(value, int) and not isinstance(value, bool)
            and lo <= value <= hi)


# --------------------------------------------------------------------------
# Catalog indices


def catalog_resolution_index(catalog: dict) -> dict[tuple[str, int | None], str]:
    """(path, slot) -> canonical hash for every decoded catalog voice.

    Standalone voices use slot None; bank slots use their 0..31 slot. Same
    resolution rule as tools/select_dev32.py (streams are not resolvable
    sources, matching the R03 corpus policy).
    """
    idx: dict[tuple[str, int | None], str] = {}
    hashes = catalog["hashes"]
    for e in catalog["entries"]:
        if e.get("v") != "decoded":
            continue
        if e["c"] == "voice":
            idx[(e["p"], None)] = e["h"]
        elif e["c"] == "bank":
            w = e["w"]
            for k in range(0, len(w), 4):
                slot, hidx = w[k], w[k + 1]
                if hidx >= 0:
                    idx[(e["p"], slot)] = hashes[hidx]
    return idx


def catalog_names_by_hash(catalog: dict) -> dict[str, set[str]]:
    """canonical hash -> every display name the catalog records for it."""
    hashes, names = catalog["hashes"], catalog["names"]
    out: dict[str, set[str]] = {}
    for e in catalog["entries"]:
        if e.get("v") != "decoded":
            continue
        if e["c"] == "voice":
            out.setdefault(e["h"], set()).add(e["n"])
            continue
        w = e.get("w") or []
        width = 4 if e["c"] == "bank" else 5
        for k in range(0, len(w), width):
            row = w[k:k + width]
            hidx, nidx = row[-3], row[-2]
            if hidx >= 0:
                out.setdefault(hashes[hidx], set()).add(names[nidx])
    return out


# --------------------------------------------------------------------------
# Per-entry checks


def check_source(i, entry: dict, resolution: dict) -> list[str]:
    src = entry.get("source")
    if not isinstance(src, dict):
        return [f"entry {i}: source block missing (unprovenanced)"]
    problems = []
    kind = src.get("kind")
    if kind not in SOURCE_KINDS:
        problems.append(f"entry {i}: source.kind {kind!r} not in"
                        f" {list(SOURCE_KINDS)}")
    if not _text(src.get("authorship")):
        problems.append(f"entry {i}: source.authorship missing/placeholder"
                        " (record 'unknown' explicitly if the author is"
                        " not known)")
    if kind == "archive":
        path, slot = src.get("catalog_path"), src.get("catalog_slot", "absent")
        if not _text(path) or slot == "absent":
            problems.append(f"entry {i}: archive source needs catalog_path"
                            " and catalog_slot (null for a standalone voice)")
            return problems
        key = (path, slot)
        chash = entry.get("canonical_hash")
        if key not in resolution:
            problems.append(f"entry {i}: source {key!r} does not resolve to"
                            " a decoded catalog voice (unprovenanced)")
        elif resolution[key] != chash:
            problems.append(f"entry {i}: catalog hash at {key!r} is"
                            f" {resolution[key]} != manifest {chash}")
    elif kind == "original":
        if not _text(src.get("record")):
            problems.append(f"entry {i}: original source needs a `record`"
                            " reference to its committed provenance record")
    return problems


def check_audition(i, entry: dict) -> list[str]:
    aud = entry.get("audition")
    if not isinstance(aud, dict):
        return [f"entry {i}: audition block missing"]
    problems = []
    if not _text(aud.get("notes")):
        problems.append(f"entry {i}: audition.notes missing/placeholder")
    if not _text(aud.get("listening_record")):
        problems.append(f"entry {i}: audition.listening_record reference"
                        " missing")
    if aud.get("verdict") != VERDICT_SELECTED:
        problems.append(f"entry {i}: audition.verdict"
                        f" {aud.get('verdict')!r} != {VERDICT_SELECTED!r}")
    return problems


def check_performance(i, entry: dict) -> list[str]:
    perf = entry.get("performance")
    if not isinstance(perf, dict):
        return [f"entry {i}: performance settings missing"]
    problems = []
    if perf.get("poly_mono") not in POLY_MONO:
        problems.append(f"entry {i}: performance.poly_mono not in"
                        f" {list(POLY_MONO)}")
    for key, hi in (("pitch_bend_range", 12), ("pitch_bend_step", 12),
                    ("portamento_time", 99)):
        if not _int_in(perf.get(key), 0, hi):
            problems.append(f"entry {i}: performance.{key} must be an"
                            f" integer 0..{hi}")
    if perf.get("portamento_mode") not in PORTAMENTO_MODES:
        problems.append(f"entry {i}: performance.portamento_mode not in"
                        f" {list(PORTAMENTO_MODES)}")
    if not isinstance(perf.get("portamento_glissando"), bool):
        problems.append(f"entry {i}: performance.portamento_glissando must"
                        " be a boolean")
    for ctl in CONTROLLERS:
        c = perf.get(ctl)
        if not isinstance(c, dict):
            problems.append(f"entry {i}: performance.{ctl} missing")
            continue
        if not _int_in(c.get("range"), 0, 99):
            problems.append(f"entry {i}: performance.{ctl}.range must be an"
                            " integer 0..99")
        assign = c.get("assign")
        if (not isinstance(assign, list)
                or any(a not in CONTROLLER_TARGETS for a in assign)
                or len(set(assign)) != len(assign)):
            problems.append(f"entry {i}: performance.{ctl}.assign must be a"
                            f" duplicate-free subset of"
                            f" {list(CONTROLLER_TARGETS)}")
    return problems


def check_distribution(i, entry: dict) -> list[str]:
    dist = entry.get("distribution")
    if not isinstance(dist, dict):
        return [f"entry {i}: distribution status missing — exclude the"
                " patch and record the reason instead"]
    problems = []
    if dist.get("status") not in DISTRIBUTION_STATUSES:
        problems.append(f"entry {i}: distribution.status"
                        f" {dist.get('status')!r} is not established"
                        f" ({list(DISTRIBUTION_STATUSES)}) — exclude the"
                        " patch and record the reason instead")
    if not _text(dist.get("basis")):
        problems.append(f"entry {i}: distribution.basis missing/placeholder")
    return problems


# --------------------------------------------------------------------------
# Whole-manifest checks


def check_duplicates(entries: list[dict], names_by_hash: dict) -> tuple[list[str], list[dict]]:
    """Identical canonical hashes must be declared aliases."""
    problems: list[str] = []
    groups: dict[str, list[dict]] = {}
    for e in entries:
        h = e.get("canonical_hash")
        if isinstance(h, str) and SHA64.match(h):
            groups.setdefault(h, []).append(e)
    by_index = {e.get("index"): e for e in entries}
    report = []
    for h, members in groups.items():
        if len(members) < 2:
            continue
        members = sorted(members, key=lambda m: m.get("index"))
        first = members[0].get("index")
        for m in members[1:]:
            i = m.get("index")
            if m.get("duplicate_of") != first or not _text(m.get("alias_note")):
                problems.append(
                    f"entry {i}: canonical hash {h[:12]} duplicates entry"
                    f" {first} without a declared alias (needs"
                    f" duplicate_of={first} and a non-empty alias_note)")
        report.append({
            "canonical_hash": h,
            "indices": [m.get("index") for m in members],
            "catalog_alias_names": sorted(names_by_hash.get(h, ())),
        })
    for e in entries:
        target = e.get("duplicate_of")
        if target is None:
            continue
        other = by_index.get(target)
        if other is None or other is e:
            problems.append(f"entry {e.get('index')}: duplicate_of {target!r}"
                            " does not name another entry")
        elif other.get("canonical_hash") != e.get("canonical_hash"):
            problems.append(f"entry {e.get('index')}: duplicate_of {target}"
                            " points at an entry with a different canonical"
                            " hash (false alias declaration)")
    return problems, report


def compute_coverage(entries: list[dict]) -> dict:
    counts = {c: 0 for c in CATEGORIES}
    for e in entries:
        if e.get("category") in counts:
            counts[e["category"]] += 1
    dist: dict[str, int] = {}
    for e in entries:
        s = (e.get("distribution") or {}).get("status")
        if isinstance(s, str):
            dist[s] = dist.get(s, 0) + 1
    return {"by_category": counts,
            "by_distribution_status": dict(sorted(dist.items()))}


def validate_manifest(manifest: dict, catalog: dict,
                      catalog_sha256: str) -> tuple[list[str], list[str], dict]:
    """Returns (problems, stale_problems, info). Empty lists = PASS."""
    problems: list[str] = []
    stale: list[str] = []
    info: dict = {}
    if manifest.get("schema") != SCHEMA:
        problems.append(f"schema {manifest.get('schema')!r} != {SCHEMA!r}")
    if not _int_in(manifest.get("revision"), 1, 1 << 31):
        problems.append("revision must be a positive integer (the bank is"
                        " versioned independently of RTL)")

    pin = manifest.get("catalog")
    if not isinstance(pin, dict):
        problems.append("catalog pin block missing")
    else:
        if pin.get("snapshot_sha256") != catalog_sha256:
            stale.append("catalog drifted since manifest generation"
                         f" (snapshot {pin.get('snapshot_sha256')} !="
                         f" current {catalog_sha256}); manifest is STALE")
        if pin.get("archive") != catalog.get("archive"):
            stale.append("catalog.archive does not match the committed"
                         " catalog's archive identity; manifest is STALE")

    exclusions = manifest.get("exclusions")
    if not isinstance(exclusions, list):
        problems.append("exclusions list missing (record excluded wanted"
                        " patches with reasons; an empty list is allowed)")
    else:
        for k, x in enumerate(exclusions):
            if not isinstance(x, dict) or not _text(x.get("name")) \
                    or not _text(x.get("reason")):
                problems.append(f"exclusion {k}: needs name and reason")

    entries = manifest.get("entries")
    if not isinstance(entries, list):
        return problems + ["entries list missing"], stale, info
    if len(entries) != BANK_SIZE:
        problems.append(f"bank must hold exactly {BANK_SIZE} entries, found"
                        f" {len(entries)}")
    indices = [e.get("index") if isinstance(e, dict) else None
               for e in entries]
    if indices != list(range(len(entries))):
        problems.append("entry indices must be 0..N-1 in order")
    entries = [e for e in entries if isinstance(e, dict)]

    resolution = catalog_resolution_index(catalog)
    names_by_hash = catalog_names_by_hash(catalog)
    for e in entries:
        i = e.get("index")
        chash = e.get("canonical_hash")
        if not isinstance(chash, str) or not SHA64.match(chash):
            problems.append(f"entry {i}: canonical_hash missing/malformed"
                            " (unhashable entry)")
        if not _text(e.get("name")):
            problems.append(f"entry {i}: name missing")
        if e.get("category") not in CATEGORIES:
            problems.append(f"entry {i}: category {e.get('category')!r} not"
                            f" in {list(CATEGORIES)}")
        problems.extend(check_source(i, e, resolution))
        problems.extend(check_audition(i, e))
        problems.extend(check_performance(i, e))
        problems.extend(check_distribution(i, e))

    dup_problems, dup_report = check_duplicates(entries, names_by_hash)
    problems.extend(dup_problems)
    info["duplicate_groups"] = dup_report

    recomputed = compute_coverage(entries)
    info["coverage"] = recomputed
    missing = [c for c, n in recomputed["by_category"].items() if n == 0]
    if missing:
        problems.append(f"variety: categories with no entry: {missing}")
    recorded = manifest.get("coverage")
    if recorded != recomputed:
        problems.append(f"coverage mismatch: recorded {recorded!r} !="
                        f" recomputed {recomputed!r}")
    return problems, stale, info


def verdict(problems: list[str], stale: list[str]) -> str:
    if problems:
        return "FAIL"
    if stale:
        return "STALE"
    return "PASS"


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    ap.add_argument("--catalog", type=Path, default=DEFAULT_CATALOG)
    ap.add_argument("--report", type=Path,
                    help="write a JSON validation log here")
    args = ap.parse_args(argv)
    try:
        if not args.catalog.is_file():
            raise NotRun(f"catalog not found: {args.catalog}")
        if not args.manifest.is_file():
            raise NotRun(f"manifest not found: {args.manifest} (the curated"
                         " bank content is produced from U02 listening"
                         " records, issue #35)")
        catalog = json.loads(args.catalog.read_text(encoding="utf-8"))
        catalog_sha = sha256_file(args.catalog)
        try:
            manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            manifest, problems, stale, info = None, [f"manifest is not valid"
                                                     f" JSON: {exc}"], [], {}
        else:
            problems, stale, info = validate_manifest(manifest, catalog,
                                                      catalog_sha)
    except NotRun as exc:
        print(f"bank128 NOT_RUN: {exc}", file=sys.stderr)
        return 2

    status = verdict(problems, stale)
    for p in problems:
        print(f"bank128 FAIL: {p}", file=sys.stderr)
    for p in stale:
        print(f"bank128 STALE: {p}", file=sys.stderr)
    if status == "PASS":
        cov = info["coverage"]
        print(f"bank128 validate PASS: {BANK_SIZE} entries, revision"
              f" {manifest['revision']}, categories {cov['by_category']},"
              f" distribution {cov['by_distribution_status']},"
              f" declared alias groups {len(info['duplicate_groups'])}")
    if args.report:
        log = {
            "schema": "gf180-dx7.bank128-validation/1",
            "status": status,
            "generated_utc": _dt.datetime.now(_dt.timezone.utc)
            .strftime("%Y-%m-%dT%H:%M:%SZ"),
            "tool": "tools/validate_bank128.py",
            "tool_sha256": sha256_file(Path(__file__)),
            "manifest": str(args.manifest),
            "manifest_sha256": sha256_file(args.manifest),
            "catalog_sha256": catalog_sha,
            "problems": problems,
            "stale": stale,
            "info": info,
        }
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(log, indent=2) + "\n",
                               encoding="utf-8")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
