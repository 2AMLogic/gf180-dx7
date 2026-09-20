#!/usr/bin/env python3
"""Archive cataloger: classify every entry of a DX7 SysEx zip collection.

Issue #9 / P02. Walks every zip entry, classifies it via the P01 codec
(src/gf180_dx7/sysex.py), computes a canonical per-voice hash with the
display name excluded, groups duplicate aliases (same canonical hash,
different names), and writes a JSON catalog. Nothing is dropped: every zip
entry appears in the catalog and the counters must reconcile exactly or the
tool exits nonzero without writing the output.

Usage:
  catalog_archive.py --zip PATH --out PATH [--max-files N]
      [--collection NAME] [--compiler NAME] [--retrieval NOTE]
      [--rights STATEMENT]

Classification (entry classes are mutually exclusive):
  voice       one decoded DX7 single-voice bulk dump (163 bytes)
  bank        one decoded DX7 32-voice bank bulk dump (4104 bytes)
  stream      2+ framed messages, every message decoded
  unsupported framed but not a supported DX7 form (reason classified)
  malformed   framing/length/checksum/read failure (reason classified)
  other       non-.syx file or directory entry (skipped, never decoded)

Canonical hash: sha256 over the 155-byte unpacked (VCED) voice body with the
10 name bytes (offsets 145..154) zeroed. Bank voices (128-byte packed VMEM
form) are converted to the unpacked body via the P01 codec before hashing,
so the same patch hashes identically whether it came from a single-voice
dump or from inside a bank. Container framing, byte-count header, channel
nibble and checksum are excluded. Out-of-range values are hashed as decoded
(never clamped).

Exit codes: 0 = catalog written and reconciled; 2 = usage error, unreadable
archive, or reconciliation failure (no output written).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
import zipfile
from collections import Counter
from dataclasses import replace
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.sysex import (  # noqa: E402
    BadChecksum,
    BadFraming,
    BadLength,
    Bank,
    SysexError,
    UnsupportedFormat,
    Voice,
)

SCHEMA = "gf180-dx7.archive-catalog/1"
DEFAULT_RIGHTS = ("redistribution rights not established; catalog metadata"
                  " only, no audio or bank bytes redistributed")
NAME_OFFSET = 145
NAME_LEN = 10
REASON_DETAIL_MAX = 160
TOP_GROUPS = 10
GROUP_NAMES_CAP = 16
HASH_SENTINEL = -1

_CLASS_KEYS = ("voice", "bank", "stream", "unsupported", "malformed", "other")


def canonical_bytes(v: Voice) -> bytes | None:
    """Unpacked 155-byte body with the name region zeroed, or None."""
    raw = v.raw_bytes
    if len(raw) == sysex.VOICE_DATA_LEN:
        return raw[:NAME_OFFSET] + b"\x00" * NAME_LEN
    if len(raw) == sysex.VOICE_PACKED_LEN or not raw:
        clean = replace(v, name_bytes=b"", anomalies=[])
        try:
            body = sysex.encode_voice(clean)[6:6 + sysex.VOICE_DATA_LEN]
        except SysexError:
            return None
        return body[:NAME_OFFSET] + b"\x00" * NAME_LEN
    return None


def canonical_hash(v: Voice) -> str | None:
    cb = canonical_bytes(v)
    return None if cb is None else hashlib.sha256(cb).hexdigest()


class _Intern:
    def __init__(self) -> None:
        self.items: list = []
        self._index: dict = {}

    def add(self, item) -> int:
        idx = self._index.get(item)
        if idx is None:
            idx = len(self.items)
            self._index[item] = idx
            self.items.append(item)
        return idx


class Cataloger:
    def __init__(self) -> None:
        self.entries: list[dict] = []
        self.hashes = _Intern()
        self.names = _Intern()
        self.by_class = Counter()
        self.reasons: dict[str, Counter] = {"malformed": Counter(),
                                            "unsupported": Counter()}
        self.voice_instances = {"standalone": 0, "in_banks": 0, "in_streams": 0}
        self.banks_in_streams = 0
        self.voices_with_anomalies = 0
        self.canonical_failures = 0
        self.groups: dict[str, dict[str, int]] = {}
        self.files = 0
        self.directories = 0

    def _group_add(self, chash: str | None, name: str) -> None:
        if chash is None:
            self.canonical_failures += 1
            return
        self.groups.setdefault(chash, {}).setdefault(name, 0)
        self.groups[chash][name] += 1

    def _note_voice(self, v: Voice) -> tuple[str | None, int]:
        chash = canonical_hash(v)
        if chash is not None:
            self.hashes.add(chash)
        if v.anomalies:
            self.voices_with_anomalies += 1
        return chash, len(v.anomalies)

    def _entry(self, info: zipfile.ZipInfo, **fields) -> dict:
        entry = {"p": info.filename, "s": info.file_size}
        entry.update(fields)
        self.entries.append(entry)
        return entry

    def classify(self, zf: zipfile.ZipFile, info: zipfile.ZipInfo) -> None:
        path = info.filename
        if info.is_dir():
            self.by_class["other"] += 1
            self.directories += 1
            self._entry(info, c="other", v="skipped", r="directory", rd=None)
            return
        self.files += 1
        suffix = Path(path).suffix.lower()
        if suffix != ".syx":
            self.by_class["other"] += 1
            self._entry(info, c="other", v="skipped", r="non_syx", rd=suffix)
            return
        try:
            data = zf.read(info)
        except (zipfile.BadZipFile, EOFError, OSError, RuntimeError,
                ValueError, NotImplementedError) as exc:
            self.by_class["malformed"] += 1
            self.reasons["malformed"]["zip_read_error"] += 1
            self._entry(info, c="malformed", v="rejected",
                        r="zip_read_error", rd=_brief(exc))
            return
        try:
            segments = sysex.split_stream(data)
        except BadFraming as exc:
            self.by_class["malformed"] += 1
            self.reasons["malformed"]["bad_framing"] += 1
            self._entry(info, c="malformed", v="rejected",
                        r="bad_framing", rd=_brief(exc))
            return
        if len(segments) == 1:
            self._classify_single(info, segments[0])
            return
        self._classify_stream(info, segments)

    def _classify_single(self, info: zipfile.ZipInfo, seg: bytes) -> None:
        try:
            obj = sysex.decode_message(seg)
        except UnsupportedFormat as exc:
            self.by_class["unsupported"] += 1
            self.reasons["unsupported"]["unsupported_format"] += 1
            self._entry(info, c="unsupported", v="rejected",
                        r="unsupported_format", rd=_brief(exc))
            return
        except BadChecksum as exc:
            self.by_class["malformed"] += 1
            self.reasons["malformed"]["bad_checksum"] += 1
            self._entry(info, c="malformed", v="rejected",
                        r="bad_checksum", rd=_brief(exc))
            return
        except BadLength as exc:
            self.by_class["malformed"] += 1
            self.reasons["malformed"]["bad_length"] += 1
            self._entry(info, c="malformed", v="rejected",
                        r="bad_length", rd=_brief(exc))
            return
        except BadFraming as exc:
            self.by_class["malformed"] += 1
            self.reasons["malformed"]["bad_framing"] += 1
            self._entry(info, c="malformed", v="rejected",
                        r="bad_framing", rd=_brief(exc))
            return
        if isinstance(obj, Bank):
            self.by_class["bank"] += 1
            rows: list[int] = []
            for slot, voice in enumerate(obj.voices):
                chash, nanom = self._note_voice(voice)
                self.voice_instances["in_banks"] += 1
                self._group_add(chash, voice.name)
                rows.extend((slot,
                             self.hashes.add(chash) if chash else HASH_SENTINEL,
                             self.names.add(voice.name), nanom))
            self._entry(info, c="bank", v="decoded", w=rows)
            return
        self.by_class["voice"] += 1
        self.voice_instances["standalone"] += 1
        chash, nanom = self._note_voice(obj)
        self._group_add(chash, obj.name)
        self._entry(info, c="voice", v="decoded", h=chash, n=obj.name, a=nanom)

    def _classify_stream(self, info: zipfile.ZipInfo,
                         segments: list[bytes]) -> None:
        decoded: list[Voice | Bank] = []
        for i, seg in enumerate(segments):
            try:
                decoded.append(sysex.decode_message(seg))
            except SysexError as exc:
                self.by_class["malformed"] += 1
                code = _reason_code(exc)
                self.reasons["malformed"][code] += 1
                detail = (f"message {i + 1}/{len(segments)}: {_brief(exc)};"
                          f" {len(decoded)}/{len(segments)} decoded")
                self._entry(info, c="malformed", v="rejected",
                            r=code, rd=detail)
                return
        self.by_class["stream"] += 1
        rows: list[int] = []
        bank_msgs = set()
        for i, obj in enumerate(decoded):
            if isinstance(obj, Bank):
                bank_msgs.add(i)
                self.banks_in_streams += 1
                for slot, voice in enumerate(obj.voices):
                    chash, nanom = self._note_voice(voice)
                    self.voice_instances["in_streams"] += 1
                    self._group_add(chash, voice.name)
                    rows.extend((i, slot,
                                 self.hashes.add(chash) if chash else HASH_SENTINEL,
                                 self.names.add(voice.name), nanom))
            else:
                chash, nanom = self._note_voice(obj)
                self.voice_instances["in_streams"] += 1
                self._group_add(chash, obj.name)
                rows.extend((i, -1,
                             self.hashes.add(chash) if chash else HASH_SENTINEL,
                             self.names.add(obj.name), nanom))
        self._entry(info, c="stream", v="decoded", w=rows)

    def counts(self, total_entries: int) -> dict:
        return {
            "total_entries": total_entries,
            "files": self.files,
            "directories": self.directories,
            "by_class": {k: self.by_class.get(k, 0) for k in _CLASS_KEYS},
            "malformed_by_reason": dict(sorted(self.reasons["malformed"].items())),
            "unsupported_by_reason": dict(sorted(
                self.reasons["unsupported"].items())),
            "voice_instances": dict(self.voice_instances),
            "banks_in_streams": self.banks_in_streams,
            "voices_total": sum(self.voice_instances.values()),
            "voices_with_anomalies": self.voices_with_anomalies,
            "distinct_canonical_hashes": len(self.hashes.items),
            "canonical_failures": self.canonical_failures,
        }


def _brief(exc: Exception) -> str:
    text = f"{type(exc).__name__}: {exc}" if not str(exc) else str(exc)
    return text[:REASON_DETAIL_MAX]


def _reason_code(exc: SysexError) -> str:
    for cls, code in ((BadChecksum, "bad_checksum"), (BadLength, "bad_length"),
                      (BadFraming, "bad_framing"),
                      (UnsupportedFormat, "unsupported_format")):
        if isinstance(exc, cls):
            return code
    return "other_codec_error"


def alias_summary(groups: dict[str, dict[str, int]]) -> dict:
    alias_groups = {h: names for h, names in groups.items() if len(names) >= 2}
    ranked = sorted(alias_groups.items(),
                    key=lambda kv: (-sum(kv[1].values()), kv[0]))
    top = []
    for chash, names in ranked[:TOP_GROUPS]:
        instances = sum(names.values())
        sorted_names = sorted(names)
        top.append({
            "canonical": chash,
            "instances": instances,
            "names": len(names),
            "aliases": len(names) - 1,
            "name_list": sorted_names[:GROUP_NAMES_CAP],
            "name_list_truncated": len(sorted_names) > GROUP_NAMES_CAP,
        })
    return {
        "distinct_canonical_hashes": len(groups),
        "groups_with_multiple_instances": sum(
            1 for names in groups.values() if sum(names.values()) >= 2),
        "alias_groups": len(alias_groups),
        "voice_instances_in_alias_groups": sum(
            sum(names.values()) for names in alias_groups.values()),
        "largest_alias_groups": top,
    }


def _recount(cat: dict) -> dict:
    """Independent tallies recomputed from the entry rows themselves."""
    hashes = cat["hashes"]
    names = cat["names"]
    by_class = Counter()
    reasons = {"malformed": Counter(), "unsupported": Counter()}
    vi = {"standalone": 0, "in_banks": 0, "in_streams": 0}
    files = dirs = banks_in_streams = anomalous = canonical_failures = 0
    groups: dict[str, dict[str, int]] = {}

    def voice(chash_idx: int, name_idx: int, nanom: int) -> None:
        nonlocal anomalous, canonical_failures
        if chash_idx == HASH_SENTINEL:
            canonical_failures += 1
        else:
            if not 0 <= chash_idx < len(hashes):
                raise ValueError(f"hash index {chash_idx} out of range")
            chash = hashes[chash_idx]
            groups.setdefault(chash, {}).setdefault(names[name_idx], 0)
            groups[chash][names[name_idx]] += 1
        if nanom > 0:
            anomalous += 1

    for e in cat["entries"]:
        by_class[e["c"]] += 1
        if e.get("r") == "directory":
            dirs += 1
        else:
            files += 1
        c = e["c"]
        if c == "voice":
            vi["standalone"] += 1
            if e["h"] is None:
                canonical_failures += 1
            else:
                groups.setdefault(e["h"], {}).setdefault(e["n"], 0)
                groups[e["h"]][e["n"]] += 1
            if e["a"] > 0:
                anomalous += 1
        elif c == "bank":
            w = e["w"]
            if len(w) % 4 or not w:
                raise ValueError(f"bank {e['p']}: row width not 4")
            for k in range(0, len(w), 4):
                slot, hidx, nidx, nanom = w[k:k + 4]
                if slot < 0:
                    raise ValueError(f"bank {e['p']}: negative slot")
                vi["in_banks"] += 1
                voice(hidx, nidx, nanom)
        elif c == "stream":
            w = e["w"]
            if len(w) % 5 or not w:
                raise ValueError(f"stream {e['p']}: row width not 5")
            bank_msgs = {w[k] for k in range(0, len(w), 5) if w[k + 1] >= 0}
            banks_in_streams += len(bank_msgs)
            for k in range(0, len(w), 5):
                vi["in_streams"] += 1
                voice(w[k + 2], w[k + 3], w[k + 4])
        elif c in ("malformed", "unsupported"):
            reasons[c][e["r"]] += 1
        elif c != "other":
            raise ValueError(f"unknown entry class {c!r}")
    return {
        "total_entries": len(cat["entries"]),
        "files": files,
        "directories": dirs,
        "by_class": {k: by_class.get(k, 0) for k in _CLASS_KEYS},
        "malformed_by_reason": dict(sorted(reasons["malformed"].items())),
        "unsupported_by_reason": dict(sorted(
            reasons["unsupported"].items())),
        "voice_instances": vi,
        "banks_in_streams": banks_in_streams,
        "voices_total": sum(vi.values()),
        "voices_with_anomalies": anomalous,
        "canonical_failures": canonical_failures,
        "duplicates": alias_summary(groups),
    }


def _check_reconciliation(cat: dict) -> None:
    """Raise ValueError unless every counter matches an independent recount."""
    counts = cat["counts"]
    entries = cat["entries"]
    problems: list[str] = []
    if len(entries) != counts["total_entries"]:
        problems.append(
            f"entries list has {len(entries)} rows but"
            f" total_entries={counts['total_entries']}")
    if sum(counts["by_class"].values()) != counts["total_entries"]:
        problems.append("by_class does not sum to total_entries")
    if counts["files"] + counts["directories"] != counts["total_entries"]:
        problems.append("files + directories != total_entries")
    seen = _recount(cat)
    for key in ("total_entries", "files", "directories", "by_class",
                "malformed_by_reason", "unsupported_by_reason",
                "voice_instances", "banks_in_streams", "voices_total",
                "voices_with_anomalies", "canonical_failures"):
        if seen[key] != counts[key]:
            problems.append(f"recount mismatch for {key}:"
                            f" {seen[key]!r} != {counts[key]!r}")
    if seen["by_class"]["voice"] != counts["voice_instances"]["standalone"]:
        problems.append("voice-class entries != standalone voice instances")
    if seen["by_class"]["bank"] * 32 != counts["voice_instances"]["in_banks"]:
        problems.append("bank-class entries * 32 != in_banks instances")
    if counts["distinct_canonical_hashes"] != len(cat["hashes"]):
        problems.append("distinct_canonical_hashes != len(hashes)")
    dup = cat["duplicates"]
    for key in ("distinct_canonical_hashes", "alias_groups",
                "voice_instances_in_alias_groups", "largest_alias_groups",
                "groups_with_multiple_instances"):
        if seen["duplicates"][key] != dup[key]:
            problems.append(f"duplicates recount mismatch for {key}")
    if problems:
        raise ValueError("catalog does not reconcile: " + "; ".join(problems))


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def build_catalog(zip_path: Path, collection: str, compiler: str,
                  retrieval: str, rights: str, max_files: int | None,
                  command: str, progress: bool = False) -> dict:
    if not zip_path.is_file():
        raise SystemExit(f"error: archive not found: {zip_path}")
    archive_sha = sha256_file(zip_path)
    archive_size = zip_path.stat().st_size
    cat: dict = {
        "schema": SCHEMA,
        "provenance": {
            "collection": collection,
            "compiler": compiler,
            "retrieval": retrieval,
            "rights": rights,
        },
        "archive": {
            "filename": zip_path.name,
            "size_bytes": archive_size,
            "sha256": archive_sha,
        },
        "run": {
            "tool": "tools/catalog_archive.py",
            "tool_sha256": sha256_file(Path(__file__).resolve()),
            "codec": "src/gf180_dx7/sysex.py",
            "codec_sha256": sha256_file(
                Path(__file__).resolve().parents[1] / "src/gf180_dx7/sysex.py"),
            "command": command,
            "max_files": max_files,
            "truncated": False,
            "python": sys.version.split()[0],
            "generated_utc": datetime.now(timezone.utc).strftime(
                "%Y-%m-%dT%H:%M:%SZ"),
        },
    }
    print(f"[catalog] archive pinned: file={zip_path.name} sha256={archive_sha}"
          f" size={archive_size}", file=sys.stderr, flush=True)
    cataloger = Cataloger()
    try:
        with zipfile.ZipFile(zip_path) as zf:
            infos = zf.infolist()
            entry_count = len(infos)
            cat["archive"]["entry_count"] = entry_count
            limit = entry_count if max_files is None else min(max_files,
                                                              entry_count)
            print(f"[catalog] archive pinned: entries={entry_count}",
                  file=sys.stderr, flush=True)
            for i in range(limit):
                cataloger.classify(zf, infos[i])
                done = i + 1
                if progress and (done % 1000 == 0 or done == limit):
                    print(f"[catalog] {done}/{limit} entries processed",
                          file=sys.stderr, flush=True)
    except zipfile.BadZipFile as exc:
        raise SystemExit(f"error: not a readable zip archive: {exc}") from exc
    cat["run"]["truncated"] = limit < entry_count
    cat["counts"] = cataloger.counts(limit)
    cat["duplicates"] = alias_summary(cataloger.groups)
    cat["hashes"] = cataloger.hashes.items
    cat["names"] = cataloger.names.items
    cat["entries"] = cataloger.entries
    try:
        _check_reconciliation(cat)
    except ValueError as exc:
        raise SystemExit(f"error: {exc}") from exc
    return cat


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--zip", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--max-files", type=int, default=None)
    parser.add_argument("--collection", default=None)
    parser.add_argument("--compiler", default="")
    parser.add_argument("--retrieval", default="local zip file")
    parser.add_argument("--rights", default=DEFAULT_RIGHTS)
    args = parser.parse_args(argv)
    argv_list = sys.argv[1:] if argv is None else argv
    collection = (args.collection if args.collection is not None
                  else args.zip.name)
    cat = build_catalog(args.zip, collection, args.compiler, args.retrieval,
                        args.rights, args.max_files,
                        command="catalog_archive.py " + " ".join(argv_list),
                        progress=True)
    out: Path = args.out
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("w", encoding="utf-8") as fh:
        json.dump(cat, fh, ensure_ascii=False, separators=(",", ":"))
    counts = cat["counts"]
    dups = cat["duplicates"]
    print(f"[catalog] written {out} sha256={sha256_file(out)}")
    print(f"[catalog] entries={counts['total_entries']} classes="
          f"{counts['by_class']}")
    print(f"[catalog] voice_instances={counts['voice_instances']} "
          f"distinct_hashes={counts['distinct_canonical_hashes']}")
    print(f"[catalog] alias_groups={dups['alias_groups']} "
          f"instances_in_alias_groups={dups['voice_instances_in_alias_groups']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
