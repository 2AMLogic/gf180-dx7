#!/usr/bin/env python3
"""Decode stats for a .syx file: read-only probe (issue #8 / P01).

Usage: sysex_probe.py <path-to-.syx> [...]

Prints per-message classified decode results and aggregate stats.
Exit codes: 0 = every message decoded (anomalies may be reported);
1 = at least one message produced a classified codec error; 2 = file could
not be read or the whole file failed framing (no message boundary found).
Writes nothing.
"""

from __future__ import annotations

import hashlib
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.sysex import BadChecksum, BadFraming, BadLength, SysexError  # noqa: E402
from gf180_dx7.sysex import UnsupportedFormat  # noqa: E402

_ERROR_NAMES = {BadFraming: "BadFraming", BadChecksum: "BadChecksum",
                BadLength: "BadLength", UnsupportedFormat: "UnsupportedFormat"}


def error_name(exc: SysexError) -> str:
    for cls, name in _ERROR_NAMES.items():
        if isinstance(exc, cls):
            return name
    return type(exc).__name__


def probe_file(path: Path) -> tuple[int, int]:
    """Returns (decoded_count, error_count)."""
    try:
        data = path.read_bytes()
    except OSError as exc:
        print(f"error: cannot read {path}: {exc}")
        raise SystemExit(2)
    print(f"file: {path}")
    print(f"  size {len(data)} bytes  sha256={hashlib.sha256(data).hexdigest()}")
    try:
        segments = sysex.split_stream(data)
    except BadFraming as exc:
        print(f"  framing: FAILED ({exc})")
        print("result: 0 messages decoded, file framing invalid")
        return 0, 1
    decoded = errors = 0
    offset = 0
    for i, seg in enumerate(segments):
        where = f"message #{i} at offset {offset} ({len(seg)} bytes)"
        try:
            obj = sysex.decode_message(seg)
        except SysexError as exc:
            errors += 1
            print(f"  {where}: {error_name(exc)}: {exc}")
        else:
            decoded += 1
            if isinstance(obj, sysex.Bank):
                flagged = sum(1 for v in obj.voices if v.anomalies)
                print(f"  {where}: bank, 32 voices, "
                      f"first={obj.voices[0].name!r} last={obj.voices[31].name!r}"
                      f", voices with anomalies: {flagged}")
            else:
                print(f"  {where}: voice {obj.name!r}, "
                      f"algorithm {obj.algorithm}, feedback {obj.feedback}"
                      + (f", anomalies: {len(obj.anomalies)}"
                         if obj.anomalies else ""))
        offset += len(seg)
    print(f"result: {decoded} decoded, {errors} classified error(s), "
          f"{len(segments)} message(s) total")
    return decoded, errors


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(__doc__)
        return 2
    total_decoded = total_errors = 0
    for arg in argv[1:]:
        decoded, errors = probe_file(Path(arg))
        total_decoded += decoded
        total_errors += errors
    if total_errors:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
