#!/usr/bin/env python3
"""R02 fast self-check (issue #7), run by tools/check_all.sh.

Deliberately NOT the full render: CI has no oracle binary. The script

  1. always decodes reference/fixtures/smoke/voice.syx via the P01 codec and
     sanity-checks the non-silent parameter set, and parses events.txt;
  2. validates reference/oracle-identity.json schema;
  3. ONLY IF the oracle binary exists at its recorded path, re-hashes it
     against the pinned sha256.

Where the oracle binary is absent, step 3 is a guarded skip reported as
NOT_RUN (never as a pass) and the script still exits 0, so CI without the
oracle stays green. Stdlib-only.

Exit codes: 0 all checks that could run passed (absent-oracle step NOT_RUN),
1 a check failed.
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import sysex  # noqa: E402

IDENTITY = REPO / "reference" / "oracle-identity.json"
FIXTURE_DIR = REPO / "reference" / "fixtures" / "smoke"

COMMIT_RE = re.compile(r"^[0-9a-f]{40}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")

failures: list[str] = []


def check(condition: bool, message: str) -> None:
    if not condition:
        failures.append(message)


def sha256_file(path: Path) -> str:
    import hashlib
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def check_fixture() -> None:
    voice_path = FIXTURE_DIR / "voice.syx"
    events_path = FIXTURE_DIR / "events.txt"
    check(voice_path.is_file(), f"missing fixture voice: {voice_path}")
    check(events_path.is_file(), f"missing fixture events: {events_path}")
    if failures:
        return

    try:
        voice = sysex.decode_voice(voice_path.read_bytes())
    except sysex.SysexError as exc:
        failures.append(f"fixture voice does not decode via P01: {exc}")
        return
    check(not voice.anomalies,
          f"fixture voice has anomalies: {voice.anomalies}")
    check(voice.algorithm == 0, f"algorithm {voice.algorithm} != 0")
    check(voice.name == "R02SMOKE01", f"unexpected voice name {voice.name!r}")
    check(all(op.output_level >= 90 for op in voice.operators),
          "operator output levels below the non-silent fixture set")
    check(all(op.level_3 >= 85 for op in voice.operators),
          "operator sustain levels below the non-silent fixture set")
    check(all(op.rate_1 >= 90 for op in voice.operators),
          "operator attacks not fast (rate_1 < 90)")
    check(all(op.level_4 == 0 for op in voice.operators),
          "operator release levels not zero (level_4 != 0)")

    events = [ln.split() for ln in
              events_path.read_text(encoding="utf-8").splitlines()
              if ln.strip() and not ln.strip().startswith("#")]
    check(bool(events), "events.txt has no events")
    if events:
        positions = [int(e[0]) for e in events]
        check(positions == sorted(positions), "events not position-sorted")
        check(events[0][1] == "note_on" and events[0][0] == "2400"
              and events[0][2] == "60" and events[0][3] == "100",
              f"unexpected first event: {events[0]}")
        check(any(e[1] == "note_off" and e[0] == "96000" for e in events),
              "missing note_off at 96000")


def check_identity() -> bool:
    """Returns True if the binary check ran (oracle present)."""
    try:
        identity = json.loads(IDENTITY.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        failures.append(f"cannot read {IDENTITY}: {exc}")
        return False
    for key in ("source", "binary", "upstream", "toolchain_line", "boundary"):
        check(key in identity, f"oracle identity missing key: {key}")
    if failures:
        return False
    commit = identity["source"].get("git_commit", "")
    sha = identity["binary"].get("sha256", "")
    check(bool(COMMIT_RE.match(commit)),
          f"oracle source commit not 40-hex: {commit!r}")
    check(bool(SHA256_RE.match(sha)),
          f"oracle binary sha256 not 64-hex: {sha!r}")
    check(identity["upstream"].get("commit")
          == "b6cf2e9190e216387ad96f977284ebbef765b447",
          "oracle upstream is not the R01-pinned Dexed commit")
    check("external-oracle-only" in identity.get("boundary", ""),
          "boundary must state 'external-oracle-only'")
    check(identity["source"].get("license") == "GPL-3.0-or-later",
          "oracle source license must be GPL-3.0-or-later")
    if failures:
        return False

    binary = Path(identity["binary"]["path"])
    if not binary.is_file():
        print(f"oracle binary NOT_RUN (absent at {binary}; guarded skip — "
              "this environment has no external oracle)")
        return False
    actual = sha256_file(binary)
    check(actual == sha,
          f"oracle binary drift: {binary} sha256 {actual} != pinned {sha}")
    return True


def main() -> int:
    check_fixture()
    ran = check_identity()
    if failures:
        for f in failures:
            print(f"render_selfcheck: FAIL — {f}")
        print(f"render_selfcheck: FAIL — {len(failures)} problem(s)")
        return 1
    if ran:
        print("render_selfcheck: PASS — fixture decodes, oracle identity "
              "hash matches")
    else:
        print("render_selfcheck: PASS — fixture decodes, identity schema OK "
              "(binary hash check NOT_RUN: oracle absent)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
