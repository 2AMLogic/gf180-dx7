#!/usr/bin/env python3
"""U01 software audition/recall tool (issue #34).

One tool that selects an archived sound by stable identity (P02 canonical
hash), renders a fixed audition phrase set (spec/audition-phrases-v1.json,
plan section 6 protocol) through the external GPL oracle pinned in
reference/oracle-identity.json, records favorites, and recalls them by
re-render + per-phrase output hash equality.

Subcommands:
  select   --hash H | --name SUBSTR [--catalog PATH] [--limit N]
           Print matching catalog instances (path, bank, slot, name,
           canonical_hash) as JSON lines.
  render   --hash H [--phrase all|ID] --out DIR [--zip PATH]
           Extract the voice from the pinned zip, render each phrase into
           DIR/<phrase_id>/ (render.f32 + render.wav + render.json).
  favorite --hash H --note TEXT [--zip PATH]
           Render ALL phrases, record per-phrase output sha256 into
           corpus/favorites.json.
  recall   --hash H [--zip PATH]
           Re-render all phrases and verify per-phrase hash equality
           against the recorded favorite.

Integrity rules:
  - Recall is by canonical hash, never by mutable name.
  - The zip is hash-checked against the catalog archive pin; the extracted
    voice is re-hashed against the selected canonical identity; the oracle
    binary must match its pinned sha256 (a pin is never relaxed).
  - The favorites store is schema-validated on every load: bad JSON, a
    short/altered hash, or an unknown phrase id is STORE CORRUPTION and
    fails loudly instead of playing the wrong sound.

Dry rule: renders are compared dry. The tool records peak levels and never
alters gain, normalizes per candidate, or time-warps.

Exit codes: 0 PASS, 1 FAIL (mismatch, corruption, identity failure),
2 NOT_RUN (environment: missing oracle/zip/catalog, oracle rejected inputs).

Stdlib-only. No upstream bytes: this tool drives external GPL code as an
oracle from outside the repository (AGENTS.md licensing/source policy).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import zipfile
from datetime import datetime, timezone
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
REPO_ROOT = TOOLS_DIR.parent
sys.path.insert(0, str(TOOLS_DIR))
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402
from catalog_archive import canonical_hash  # noqa: E402
from render_reference import (  # noqa: E402
    RENDER_RATE,
    HarnessFail,
    HarnessNotRun,
    check_oracle_binary,
    enforce_nonsilent_guard,
    f32_to_pcm16_bytes,
    load_identity,
    sha256_file,
    write_wav16,
)

DEFAULT_IDENTITY = REPO_ROOT / "reference" / "oracle-identity.json"
DEFAULT_CATALOG = REPO_ROOT / "corpus" / "archive-catalogs" / \
    "alltheweb-catalog.json"
DEFAULT_PHRASES = REPO_ROOT / "spec" / "audition-phrases-v1.json"
DEFAULT_STORE = REPO_ROOT / "corpus" / "favorites.json"

PHRASES_SCHEMA = "gf180-dx7.audition-phrases/1"
STORE_SCHEMA = "gf180-dx7.favorites/1"
CATALOG_SCHEMA = "gf180-dx7.archive-catalog/1"

SILENT_THRESHOLD = 1e-6
DEFAULT_LIMIT = 200
TOOL = "tools/audition.py"

SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
PHRASE_ID_RE = re.compile(r"^[a-z0-9][a-z0-9-]*$")
ADDED_RE = re.compile(r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$")

EVENT_ARG_COUNTS = {"note_on": 2, "note_off": 1, "cc": 2, "pb": 2}

# Plan section 6 protocol, fixed at U01: these phrase ids must exist in the
# committed set or the phrase file does not satisfy the audition protocol.
REQUIRED_PHRASE_IDS = (
    "low-36-vel100", "mid-60-vel100", "high-84-vel100",
    "mid-60-vel32", "mid-60-vel80", "mid-60-vel127",
    "short-pluck-60", "long-hold-60", "release-tail-60",
    "chord-3note-60-64-67", "modwheel-sweep-60", "pitch-bend-60",
)


class AuditionFail(Exception):
    """Exit 1: a check failed; nothing is verified or recorded."""


class AuditionNotRun(Exception):
    """Exit 2: environment/usage problem; nothing was verified."""


class StoreCorruption(AuditionFail):
    """Exit 1: the favorites store is corrupt; recall must fail loudly."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


# ---------------------------------------------------------------------------
# Phrase set


def validate_phrases(data) -> list[dict]:
    """Validate the committed phrase file; return normalized phrases."""
    if not isinstance(data, dict) or data.get("schema") != PHRASES_SCHEMA:
        raise AuditionFail(
            f"phrase file schema must be {PHRASES_SCHEMA!r}")
    if data.get("sample_rate") != RENDER_RATE:
        raise AuditionFail(
            f"phrase sample_rate must be {RENDER_RATE}")
    rows = data.get("phrases")
    if not isinstance(rows, list) or not rows:
        raise AuditionFail("phrase file has no phrases")
    phrases: list[dict] = []
    seen: set[str] = set()
    for row in rows:
        if not isinstance(row, dict):
            raise AuditionFail("phrase entry is not an object")
        pid = row.get("id")
        if not isinstance(pid, str) or not PHRASE_ID_RE.match(pid or ""):
            raise AuditionFail(f"bad phrase id {pid!r}")
        if pid in seen:
            raise AuditionFail(f"duplicate phrase id {pid!r}")
        seen.add(pid)
        description = row.get("description")
        if not isinstance(description, str) or not description.strip():
            raise AuditionFail(f"phrase {pid}: missing description")
        duration = row.get("duration_s")
        if not isinstance(duration, (int, float)) or \
                not 0 < duration <= 30:
            raise AuditionFail(f"phrase {pid}: bad duration_s {duration!r}")
        events = _validate_events(pid, row.get("events"))
        phrases.append({"id": pid, "description": description,
                        "duration_s": float(duration), "events": events})
    check_phrase_coverage(phrases)
    return phrases


def _validate_events(pid: str, raw) -> list[dict]:
    if not isinstance(raw, list) or not raw:
        raise AuditionFail(f"phrase {pid}: events missing or empty")
    events = []
    last_pos = -1
    for line in raw:
        if not isinstance(line, str):
            raise AuditionFail(f"phrase {pid}: event is not a string")
        parts = line.split()
        if len(parts) < 2:
            raise AuditionFail(f"phrase {pid}: malformed event {line!r}")
        cmd = parts[1]
        try:
            pos = int(parts[0])
            args = [int(v) for v in parts[2:]]
        except ValueError as exc:
            raise AuditionFail(
                f"phrase {pid}: non-integer in event {line!r}") from exc
        if pos < 0 or pos < last_pos:
            raise AuditionFail(
                f"phrase {pid}: event positions must be non-negative and "
                f"non-decreasing (line {line!r})")
        last_pos = pos
        if cmd not in EVENT_ARG_COUNTS:
            raise AuditionFail(f"phrase {pid}: unknown event cmd {cmd!r}")
        if len(args) != EVENT_ARG_COUNTS[cmd]:
            raise AuditionFail(
                f"phrase {pid}: {cmd} takes {EVENT_ARG_COUNTS[cmd]} arg(s), "
                f"got {len(args)} in {line!r}")
        if any(not 0 <= a <= 127 for a in args):
            raise AuditionFail(
                f"phrase {pid}: event arg out of 0-127 in {line!r}")
        if cmd == "note_on" and not 1 <= args[1] <= 127:
            raise AuditionFail(
                f"phrase {pid}: note_on velocity must be 1-127 in {line!r}")
        events.append({"pos": pos, "cmd": cmd, "args": args})
    if not any(e["cmd"] == "note_on" for e in events):
        raise AuditionFail(f"phrase {pid}: no note_on event")
    return events


def check_phrase_coverage(phrases: list[dict]) -> None:
    """The committed set must satisfy the plan section 6 protocol."""
    ids = {p["id"] for p in phrases}
    missing = [pid for pid in REQUIRED_PHRASE_IDS if pid not in ids]
    if missing:
        raise AuditionFail(
            "phrase set does not satisfy the plan section 6 protocol; "
            "missing: " + ", ".join(missing))


def load_phrases(path: Path) -> list[dict]:
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = json.load(fh)
    except OSError as exc:
        raise AuditionNotRun(f"cannot read phrase file {path}: {exc}")
    except json.JSONDecodeError as exc:
        raise AuditionFail(f"phrase file {path} is not valid JSON: {exc}")
    return validate_phrases(data)


def phrase_by_id(phrases: list[dict], pid: str) -> dict:
    for p in phrases:
        if p["id"] == pid:
            return p
    raise AuditionFail(
        f"unknown phrase {pid!r}; valid ids: "
        + ", ".join(p["id"] for p in phrases))


def events_file_text(phrase: dict) -> str:
    """Deterministic events.txt content for one phrase."""
    lines = [
        "# gf180-dx7 U01 audition phrase (issue #34)",
        f"# phrase: {phrase['id']}",
        f"# description: {phrase['description']}",
        "# positions are absolute sample offsets at 48000 Hz; the external",
        "# oracle applies every event before the 64-sample block containing",
        "# its position (reference/oracle-identity.json).",
    ]
    for e in phrase["events"]:
        lines.append(
            f"{e['pos']} {e['cmd']} " + " ".join(str(a) for a in e["args"]))
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Catalog


def load_catalog(path: Path) -> dict:
    if not path.is_file():
        raise AuditionNotRun(
            f"catalog not found: {path} (build it with "
            "tools/catalog_archive.py or pass --catalog)")
    try:
        with open(path, "r", encoding="utf-8") as fh:
            cat = json.load(fh)
    except json.JSONDecodeError as exc:
        raise AuditionFail(f"catalog {path} is not valid JSON: {exc}")
    if not isinstance(cat, dict) or cat.get("schema") != CATALOG_SCHEMA:
        raise AuditionFail(f"catalog {path} schema is not {CATALOG_SCHEMA!r}")
    for key in ("entries", "hashes", "names"):
        if not isinstance(cat.get(key), list):
            raise AuditionFail(f"catalog {path} is missing list {key!r}")
    return cat


def _voice_row(pid: str, e: dict, cat: dict, hidx: int, nidx: int) -> tuple:
    hashes, names = cat["hashes"], cat["names"]
    if not 0 <= hidx < len(hashes) or not 0 <= nidx < len(names):
        raise AuditionFail(
            f"catalog entry {e.get('p')!r} ({pid}): hash/name index out of "
            "range (catalog is corrupt)")
    return hashes[hidx], names[nidx]


def select_instances(cat: dict, chash: str | None = None,
                     name_substr: str | None = None) -> list[dict]:
    """Catalog instances matching a canonical hash or a name substring.

    Standalone voices report bank=null/slot=null; bank voices report the
    containing file as bank and the slot; stream voices add the message
    index. Names are matched case-insensitively; identity is never inferred
    from a name.
    """
    out: list[dict] = []
    needle = name_substr.lower() if name_substr is not None else None
    for e in cat["entries"]:
        cls = e.get("c")
        if cls == "voice":
            if e.get("h") is None:
                continue
            if chash is not None and e["h"] != chash:
                continue
            if needle is not None and needle not in e["n"].lower():
                continue
            out.append({"path": e["p"], "bank": None, "slot": None,
                        "message": None, "name": e["n"],
                        "canonical_hash": e["h"]})
        elif cls == "bank":
            w = e.get("w") or []
            for k in range(0, len(w), 4):
                slot, hidx, nidx, _nanom = w[k:k + 4]
                ch, name = _voice_row("bank", e, cat, hidx, nidx)
                if chash is not None and ch != chash:
                    continue
                if needle is not None and needle not in name.lower():
                    continue
                out.append({"path": e["p"], "bank": e["p"], "slot": slot,
                            "message": None, "name": name,
                            "canonical_hash": ch})
        elif cls == "stream":
            w = e.get("w") or []
            for k in range(0, len(w), 5):
                msg, slot, hidx, nidx, _nanom = w[k:k + 5]
                ch, name = _voice_row("stream", e, cat, hidx, nidx)
                if chash is not None and ch != chash:
                    continue
                if needle is not None and needle not in name.lower():
                    continue
                out.append({"path": e["p"], "bank": e["p"],
                            "slot": None if slot < 0 else slot,
                            "message": msg, "name": name,
                            "canonical_hash": ch})
    return out


def pick_instance(instances: list[dict], chash: str) -> dict:
    """Deterministic single instance for extraction: standalone voice first,
    then bank slot, then stream instance (catalog order within each kind)."""
    standalone = [i for i in instances if i["bank"] is None]
    banks = [i for i in instances
             if i["bank"] is not None and i["message"] is None]
    streams = [i for i in instances if i["message"] is not None]
    for group in (standalone, banks, streams):
        if group:
            return group[0]
    raise AuditionFail(
        f"canonical hash {chash} has no catalog instance")


# ---------------------------------------------------------------------------
# Zip extraction


def resolve_zip(args_zip: str | None, cat: dict) -> Path:
    """Locate the pinned archive: --zip, then $DX7_ALLTHEWEB_ZIP, then the
    catalog-recorded filename in a few standard local locations."""
    filename = cat.get("archive", {}).get("filename")
    if not filename:
        raise AuditionFail("catalog archive block has no filename")
    candidates: list[Path] = []
    if args_zip:
        candidates.append(Path(args_zip).expanduser())
    env = os_env_zip()
    if env:
        candidates.append(Path(env).expanduser())
    for root in (REPO_ROOT, REPO_ROOT / "corpus",
                 REPO_ROOT / "corpus" / "archives",
                 Path.home() / "Downloads"):
        candidates.append(root / filename)
    for cand in candidates:
        if cand.is_file():
            verify_zip_pin(cand, cat)
            return cand
    raise AuditionNotRun(
        f"archive {filename!r} not found; tried: "
        + ", ".join(str(c) for c in candidates)
        + ". Pass --zip PATH (the file is hash-checked against the catalog "
        "archive pin; it is not redistributed by this repository)")


def os_env_zip() -> str | None:
    return os.environ.get("DX7_ALLTHEWEB_ZIP")


def verify_zip_pin(zip_path: Path, cat: dict) -> None:
    expected = cat.get("archive", {}).get("sha256")
    if not expected:
        raise AuditionFail("catalog archive block has no sha256 pin")
    actual = sha256_file(zip_path)
    if actual != expected:
        raise AuditionFail(
            f"archive {zip_path} sha256 {actual} != catalog pin {expected} "
            "(wrong or corrupt archive; refusing to extract voices)")


def extract_voice_bytes(zf: zipfile.ZipFile, instance: dict,
                        chash: str) -> bytes:
    """Extract one instance from the open zip as a canonical 163-byte
    single-voice dump, and prove it decodes to the selected identity."""
    try:
        data = zf.read(instance["path"])
    except (zipfile.BadZipFile, EOFError, OSError, RuntimeError,
            ValueError, NotImplementedError) as exc:
        raise AuditionFail(
            f"cannot read {instance['path']!r} from the archive: {exc}")
    try:
        if instance["message"] is not None:
            segs = sysex.split_stream(data)
            if not 0 <= instance["message"] < len(segs):
                raise AuditionFail(
                    f"stream message index {instance['message']} out of "
                    f"range for {instance['path']!r}")
            obj = sysex.decode_message(segs[instance["message"]])
        else:
            obj = sysex.decode_message(data)
    except sysex.SysexError as exc:
        raise AuditionFail(
            f"catalog says {instance['path']!r} decodes but the codec "
            f"rejects it (catalog/archive drift): {exc}")
    if isinstance(obj, sysex.Bank):
        if not isinstance(instance["slot"], int) or \
                not 0 <= instance["slot"] < len(obj.voices):
            raise AuditionFail(
                f"bad slot {instance['slot']!r} for bank "
                f"{instance['path']!r}")
        voice = obj.voices[instance["slot"]]
    else:
        voice = obj
    voice_bytes = sysex.encode_voice(voice)
    actual = canonical_hash(voice)
    if actual != chash:
        raise AuditionFail(
            f"extracted voice from {instance['path']!r} hashes {actual} != "
            f"selected canonical identity {chash} (catalog/archive drift; "
            "refusing to render the wrong sound)")
    return voice_bytes


# ---------------------------------------------------------------------------
# Oracle rendering


def check_oracle(identity_path: Path) -> dict:
    """Load the pin and hash the oracle binary; never relax a pin."""
    identity = load_identity(identity_path)
    oracle_path = Path(identity["binary"]["path"])
    check_oracle_binary(identity, oracle_path)
    return identity


def render_one_phrase(oracle_path: Path, identity: dict, phrase: dict,
                      voice_bytes: bytes, subdir: Path) -> dict:
    """Render one phrase into subdir; return its record (hash + peak)."""
    subdir.mkdir(parents=True, exist_ok=True)
    voice_path = subdir / "voice.syx"
    events_path = subdir / "events.txt"
    out_path = subdir / "render.f32"
    meta_path = subdir / "oracle_meta.json"
    voice_path.write_bytes(voice_bytes)
    events_text = events_file_text(phrase)
    events_path.write_text(events_text, encoding="utf-8")

    cmd = [str(oracle_path), "--voice", str(voice_path),
           "--events", str(events_path), "--out", str(out_path),
           "--rate", str(RENDER_RATE), "--seconds", str(phrase["duration_s"]),
           "--meta", str(meta_path)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode == 2:
        raise AuditionNotRun(
            f"oracle rejected inputs for phrase {phrase['id']}: "
            f"{proc.stderr.strip()}")
    if proc.returncode != 0:
        raise AuditionFail(
            f"oracle failed on phrase {phrase['id']} "
            f"(exit {proc.returncode}): {proc.stderr.strip()}")

    from array import array
    data = out_path.read_bytes()
    if len(data) % 4:
        raise AuditionFail(
            f"phrase {phrase['id']}: render.f32 size {len(data)} is not a "
            "multiple of 4")
    samples = array("f", data)
    # Silent-output guard (same policy as R02): a silent render while the
    # phrase claims an active note is a broken audition, never a quiet one.
    peak = enforce_nonsilent_guard(
        samples, [{"cmd": e["cmd"]} for e in phrase["events"]],
        threshold=SILENT_THRESHOLD)
    write_wav16(subdir / "render.wav", samples, RENDER_RATE)

    record = {
        "phrase": phrase["id"],
        "output": {"file": "render.f32", "sha256": sha256_bytes(data),
                   "bytes": len(data), "samples": len(samples),
                   "format": "pcm_f32le mono (raw, headerless)"},
        "peak_abs": peak,
    }
    metadata = {
        "tool": TOOL,
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/34",
        "phrase": phrase["id"],
        "description": phrase["description"],
        "duration_s": phrase["duration_s"],
        "sample_rate": RENDER_RATE,
        "oracle_command": cmd,
        "oracle_identity": {
            "binary_path": str(oracle_path),
            "binary_sha256": identity["binary"]["sha256"],
            "source_git_commit": identity["source"]["git_commit"],
        },
        "inputs": {
            "voice": {"path": str(voice_path),
                      "sha256": sha256_bytes(voice_bytes)},
            "events": {"path": str(events_path),
                       "sha256": sha256_bytes(events_text.encode("utf-8"))},
        },
        "dry_rule": "dry render: peak recorded, gain never altered; "
                    "no per-candidate normalization or time-warp",
        "silent_output_guard": {
            "threshold_max_abs": SILENT_THRESHOLD,
            "triggered": False,
            "observed_peak_abs": peak,
        },
        **record,
    }
    (subdir / "render.json").write_text(
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
        encoding="utf-8")
    return record


def render_phrases(oracle_path: Path, identity: dict, voice_bytes: bytes,
                   phrases: list[dict], out_dir: Path) -> dict[str, dict]:
    """Render every phrase; return {phrase_id: record}."""
    out_dir.mkdir(parents=True, exist_ok=True)
    records: dict[str, dict] = {}
    for phrase in phrases:
        records[phrase["id"]] = render_one_phrase(
            oracle_path, identity, phrase, voice_bytes,
            out_dir / phrase["id"])
    return records


# ---------------------------------------------------------------------------
# Favorites store


def validate_store(data, phrases: list[dict] | None) -> dict:
    """Schema-validate the favorites store; raise StoreCorruption on any
    deviation (bad JSON is handled by the caller)."""
    if not isinstance(data, dict) or data.get("schema") != STORE_SCHEMA:
        raise StoreCorruption(
            f"STORE CORRUPTION: favorites schema must be {STORE_SCHEMA!r}")
    favs = data.get("favorites")
    if not isinstance(favs, dict):
        raise StoreCorruption(
            "STORE CORRUPTION: favorites store has no favorites object")
    known_ids = {p["id"] for p in phrases} if phrases else None
    for chash, entry in favs.items():
        if not isinstance(chash, str) or not SHA256_RE.match(chash):
            raise StoreCorruption(
                f"STORE CORRUPTION: favorites key {chash!r} is not a 64-hex "
                "sha256 canonical hash")
        if not isinstance(entry, dict) or \
                set(entry.keys()) != {"note", "added", "phrase_hashes"}:
            raise StoreCorruption(
                f"STORE CORRUPTION: favorite {chash[:12]}… must have exactly "
                "note, added, phrase_hashes")
        if not isinstance(entry["note"], str) or not entry["note"].strip():
            raise StoreCorruption(
                f"STORE CORRUPTION: favorite {chash[:12]}… has an empty or "
                "non-string note")
        if not isinstance(entry["added"], str) or \
                not ADDED_RE.match(entry["added"]):
            raise StoreCorruption(
                f"STORE CORRUPTION: favorite {chash[:12]}… has a malformed "
                "added timestamp (want YYYY-MM-DDTHH:MM:SSZ)")
        ph = entry["phrase_hashes"]
        if not isinstance(ph, dict) or not ph:
            raise StoreCorruption(
                f"STORE CORRUPTION: favorite {chash[:12]}… has empty or "
                "malformed phrase_hashes")
        for pid, digest in ph.items():
            if known_ids is not None and pid not in known_ids:
                raise StoreCorruption(
                    f"STORE CORRUPTION: favorite {chash[:12]}… references "
                    f"phrase {pid!r} which is not in the committed phrase "
                    "set")
            if not isinstance(digest, str) or not SHA256_RE.match(digest):
                raise StoreCorruption(
                    f"STORE CORRUPTION: favorite {chash[:12]}… phrase "
                    f"{pid!r} hash {digest!r} is not a 64-hex sha256")
    return data


def load_store(path: Path, phrases: list[dict] | None = None) -> dict:
    """Load the favorites store (missing file = empty store)."""
    if not path.is_file():
        return {"schema": STORE_SCHEMA, "favorites": {}}
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = json.load(fh)
    except json.JSONDecodeError as exc:
        raise StoreCorruption(
            f"STORE CORRUPTION: favorites store {path} is not valid JSON: "
            f"{exc}")
    return validate_store(data, phrases)


def save_store(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(
        json.dumps(data, indent=2, sort_keys=True, ensure_ascii=False) + "\n",
        encoding="utf-8")
    tmp.replace(path)


# ---------------------------------------------------------------------------
# Subcommands


def cmd_select(args) -> int:
    if (args.hash is None) == (args.name is None):
        raise AuditionFail(
            "select needs exactly one of --hash or --name")
    if args.hash is not None and not SHA256_RE.match(args.hash):
        raise AuditionFail(
            f"--hash {args.hash!r} is not a 64-hex sha256 canonical hash")
    cat = load_catalog(args.catalog)
    instances = select_instances(cat, chash=args.hash,
                                name_substr=args.name)
    shown = instances if args.limit == 0 else instances[:args.limit]
    for inst in shown:
        print(json.dumps(inst, ensure_ascii=False))
    print(
        f"select: {len(shown)} shown, {len(instances)} matched "
        f"({'hash ' + (args.hash or '') if args.hash else 'name ~ ' + args.name})",
        file=sys.stderr)
    return 0


def _prepare_render(args) -> tuple[dict, list[dict], Path, bytes, Path,
                                  dict]:
    """Shared prelude for render/favorite/recall: phrases, catalog
    identity lookup, pinned zip, pinned oracle, extracted voice."""
    if not SHA256_RE.match(args.hash):
        raise AuditionFail(
            f"--hash {args.hash!r} is not a 64-hex sha256 canonical hash")
    phrases = load_phrases(args.phrases)
    if getattr(args, "phrase", "all") != "all":
        phrases = [phrase_by_id(phrases, args.phrase)]
    cat = load_catalog(args.catalog)
    instances = select_instances(cat, chash=args.hash)
    if not instances:
        raise AuditionFail(
            f"canonical hash {args.hash} is not in the catalog {args.catalog}"
            " (select --name is discovery only; recall is by identity)")
    instance = pick_instance(instances, args.hash)
    zip_path = resolve_zip(args.zip, cat)
    identity = check_oracle(args.identity)
    oracle_path = Path(identity["binary"]["path"])
    with zipfile.ZipFile(zip_path) as zf:
        voice_bytes = extract_voice_bytes(zf, instance, args.hash)
    return instance, phrases, zip_path, voice_bytes, oracle_path, identity


def cmd_render(args) -> int:
    instance, phrases, zip_path, voice_bytes, oracle_path, identity = \
        _prepare_render(args)
    out_dir = Path(args.out)
    records = render_phrases(oracle_path, identity, voice_bytes, phrases,
                             out_dir)
    summary = {
        "tool": TOOL,
        "command": "render",
        "canonical_hash": args.hash,
        "instance": instance,
        "archive": {"path": str(zip_path),
                    "sha256": sha256_file(zip_path)},
        "phrases": records,
    }
    (out_dir / "render-summary.json").write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n",
        encoding="utf-8")
    for pid, rec in records.items():
        print(f"render: {pid}: sha256 {rec['output']['sha256'][:16]}… "
              f"peak {rec['peak_abs']:.4f}")
    print(f"render: PASS — {len(records)} phrase(s) for {args.hash[:12]}… "
          f"in {out_dir} (dry; gain untouched)")
    return 0


def cmd_favorite(args) -> int:
    if not args.note.strip():
        raise AuditionFail("--note must be non-empty")
    instance, phrases, zip_path, voice_bytes, oracle_path, identity = \
        _prepare_render(args)
    store = load_store(args.store, phrases)
    with tempfile.TemporaryDirectory(prefix="u01-favorite-") as tmp:
        records = render_phrases(oracle_path, identity, voice_bytes,
                                 phrases, Path(tmp))
    entry = {
        "note": args.note,
        "added": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "phrase_hashes": {pid: rec["output"]["sha256"]
                          for pid, rec in records.items()},
    }
    existed = args.hash in store["favorites"]
    store["favorites"][args.hash] = entry
    save_store(args.store, store)
    action = "updated" if existed else "added"
    print(f"favorite: {action} {args.hash[:12]}… "
          f"({len(entry['phrase_hashes'])} phrase hashes recorded; "
          f"note: {args.note!r})")
    print(f"favorite: renders were verified non-silent then discarded; "
          f"recall re-renders and compares hashes")
    return 0


def cmd_recall(args) -> int:
    if not args.store.is_file():
        raise AuditionFail(
            f"favorites store {args.store} does not exist; nothing to "
            "recall (record one with favorite --hash H --note TEXT)")
    phrases = load_phrases(args.phrases)
    store = load_store(args.store, phrases)
    favs = store["favorites"]
    if args.hash not in favs:
        raise AuditionNotRun(
            f"canonical hash {args.hash} is not in the favorites store "
            f"({len(favs)} favorite(s) present); recall is by identity — "
            "favorite it first")
    entry = favs[args.hash]
    instance, phrases_all, zip_path, voice_bytes, oracle_path, identity = \
        _prepare_render(args)
    with tempfile.TemporaryDirectory(prefix="u01-recall-") as tmp:
        records = render_phrases(oracle_path, identity, voice_bytes,
                                 phrases_all, Path(tmp))
    mismatches = []
    for pid, digest in sorted(entry["phrase_hashes"].items()):
        actual = records[pid]["output"]["sha256"]
        if actual != digest:
            mismatches.append((pid, digest, actual))
    if mismatches:
        print(f"recall: FAIL — {args.hash[:12]}… does not reproduce its "
              f"recorded renders ({len(mismatches)} of "
              f"{len(entry['phrase_hashes'])} phrases differ):",
              file=sys.stderr)
        for pid, expected, actual in mismatches:
            print(f"  {pid}: recorded {expected} != re-rendered {actual}",
                  file=sys.stderr)
        print("recall: the sound identity hashes agree but the audio does "
              "not; the oracle, phrase set, or store drifted — refusing to "
              "play the wrong sound", file=sys.stderr)
        return 1
    print(f"recall: PASS — {args.hash[:12]}… note {entry['note']!r}: all "
          f"{len(entry['phrase_hashes'])} phrase renders byte-identical "
          "to the recorded favorite")
    return 0


# ---------------------------------------------------------------------------


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="U01 audition/recall tool (issue #34)")
    parser.add_argument("--catalog", type=Path, default=DEFAULT_CATALOG,
                        help=f"archive catalog (default: {DEFAULT_CATALOG})")
    parser.add_argument("--phrases", type=Path, default=DEFAULT_PHRASES,
                        help=f"phrase set (default: {DEFAULT_PHRASES})")
    parser.add_argument("--store", type=Path, default=DEFAULT_STORE,
                        help=f"favorites store (default: {DEFAULT_STORE})")
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY,
                        help="oracle identity file")
    parser.add_argument("--zip", default=None,
                        help="path to the pinned archive zip (else "
                             "$DX7_ALLTHEWEB_ZIP or the catalog filename in "
                             "standard locations)")
    sub = parser.add_subparsers(dest="command", required=True)

    p_sel = sub.add_parser("select", help="find catalog instances")
    p_sel.add_argument("--hash", default=None,
                       help="canonical hash (64 hex)")
    p_sel.add_argument("--name", default=None,
                       help="case-insensitive name substring (discovery "
                            "only; never an identity)")
    p_sel.add_argument("--limit", type=int, default=DEFAULT_LIMIT,
                       help="max rows printed (0 = unlimited, default "
                            f"{DEFAULT_LIMIT})")

    def add_common(p):
        p.add_argument("--hash", required=True,
                       help="canonical hash (64 hex)")
    p_ren = sub.add_parser("render", help="render phrases to a directory")
    add_common(p_ren)
    p_ren.add_argument("--phrase", default="all",
                       help="'all' or a phrase id (default: all)")
    p_ren.add_argument("--out", required=True, help="output directory")
    p_fav = sub.add_parser("favorite", help="record a favorite")
    add_common(p_fav)
    p_fav.add_argument("--note", required=True, help="listening note")
    p_rec = sub.add_parser("recall", help="verify a favorite reproduces")
    add_common(p_rec)

    args = parser.parse_args(argv)
    try:
        if args.command == "select":
            return cmd_select(args)
        if args.command == "render":
            return cmd_render(args)
        if args.command == "favorite":
            return cmd_favorite(args)
        if args.command == "recall":
            return cmd_recall(args)
        parser.error(f"unknown command {args.command!r}")
    except (AuditionFail, HarnessFail) as exc:
        print(f"audition: FAIL — {exc}", file=sys.stderr)
        return 1
    except (AuditionNotRun, HarnessNotRun) as exc:
        print(f"audition: NOT_RUN — {exc}", file=sys.stderr)
        return 2
    return 2


if __name__ == "__main__":
    sys.exit(main())
