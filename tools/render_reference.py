#!/usr/bin/env python3
"""R02 headless reference renderer (issue #7).

Drives the EXTERNAL GPL oracle (pinned in reference/oracle-identity.json,
built from the R01-pinned Dexed Mark I sources outside this repository) to
produce one dry comparison render of a named fixture:

  1. run tools/verify_reference.py (abort unless it passes);
  2. load reference/oracle-identity.json and re-hash the oracle binary at its
     recorded path (abort on mismatch or absence);
  3. render the fixture (reference/fixtures/<name>/voice.syx + events.txt)
     with the oracle;
  4. enforce the silent-output guard: a render with max|sample| < 1e-6 while
     the events claim an active note FAILS and no evidence is written;
  5. determinism check: a SECOND fresh subprocess renders the same inputs to
     a temp path; evidence is written only if the two outputs are
     byte-identical;
  6. write reference/evidence/<fixture>/render.f32 (raw f32le mono PCM),
     render.wav (16-bit PCM convenience copy) and render.json (provenance).

Exit codes: 0 PASS, 1 FAIL (drift, guard, decode, or determinism),
2 NOT_RUN (environment: missing oracle/fixture, unusable reference tree).

No fidelity claim is made or implied: oracle agreement is software-reference
agreement only (reference/oracle-protocol.md section 4). The "listen to the
output" acceptance item is a HUMAN step and is recorded NOT_RUN here.

Stdlib-only.
"""

from __future__ import annotations

import argparse
import array
import hashlib
import json
import math
import os
import re
import struct
import subprocess
import sys
import tempfile
import wave
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_IDENTITY = REPO_ROOT / "reference" / "oracle-identity.json"
DEFAULT_FIXTURES = REPO_ROOT / "reference" / "fixtures"
DEFAULT_EVIDENCE = REPO_ROOT / "reference" / "evidence"
VERIFY_REFERENCE = REPO_ROOT / "tools" / "verify_reference.py"

RENDER_RATE = 48000
RENDER_SECONDS = 4.0
RENDER_SAMPLES = int(RENDER_RATE * RENDER_SECONDS)
SILENT_THRESHOLD = 1e-6
LISTENING_STATUS = "NOT_RUN: pending human audition"

COMMIT_RE = re.compile(r"^[0-9a-f]{40}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


class HarnessFail(Exception):
    """Exit 1: a check failed; evidence must not be written."""


class HarnessNotRun(Exception):
    """Exit 2: environment/usage problem; nothing was verified."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def read_f32(path) -> array.array:
    """Read raw f32le PCM into an array('f')."""
    data = Path(path).read_bytes()
    if len(data) % 4:
        raise HarnessFail(
            f"{path}: f32 size {len(data)} is not a multiple of 4")
    return array.array("f", data)


def parse_events(text: str) -> list:
    """Parse events.txt lines: '<pos> <cmd> <args...>'; '#' comments."""
    events = []
    for lineno, line in enumerate(text.splitlines(), 1):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split()
        try:
            pos = int(parts[0])
            args = [int(v) for v in parts[2:]]
        except ValueError as exc:
            raise HarnessFail(
                f"events line {lineno}: bad integer ({exc})") from exc
        events.append({"pos": pos, "cmd": parts[1], "args": args,
                       "line": lineno})
    if not events:
        raise HarnessFail("events file contains no events")
    positions = [e["pos"] for e in events]
    if positions != sorted(positions):
        raise HarnessFail("events are not in non-decreasing position order")
    for e in events:
        e["block"] = e["pos"] // 64  # oracle applies events at 64-sample blocks
    return events


def claims_active_note(events: list) -> bool:
    """True if any event claims a note is struck (conservative guard input)."""
    return any(e["cmd"] == "note_on" for e in events)


def enforce_nonsilent_guard(samples: array.array, events: list,
                            threshold: float = SILENT_THRESHOLD) -> None:
    """Reject a silent substitution: max|sample| < threshold while the events
    claim an active note. Raises HarnessFail (evidence must not be written).
    """
    peak = max((abs(s) for s in samples), default=0.0)
    if claims_active_note(events) and peak < threshold:
        raise HarnessFail(
            f"silent-output guard: max|sample|={peak:.3g} < {threshold:g} "
            "while events claim an active note — refusing to write evidence "
            "(silent substitution rejected)")
    return peak


def load_identity(path: Path) -> dict:
    try:
        with open(path, "r", encoding="utf-8") as fh:
            identity = json.load(fh)
    except OSError as exc:
        raise HarnessNotRun(f"cannot read oracle identity {path}: {exc}")
    except json.JSONDecodeError as exc:
        raise HarnessFail(f"oracle identity {path} is not valid JSON: {exc}")
    try:
        binary = identity["binary"]
        source = identity["source"]
        if not COMMIT_RE.match(source["git_commit"]):
            raise KeyError("source.git_commit is not a 40-hex sha")
        if not SHA256_RE.match(binary["sha256"]):
            raise KeyError("binary.sha256 is not a 64-hex sha")
        if "external-oracle-only" not in identity.get("boundary", ""):
            raise KeyError("boundary must state 'external-oracle-only'")
        for key in ("license",):
            if not source.get(key):
                raise KeyError(f"source.{key} missing")
        if not identity.get("upstream", {}).get("commit"):
            raise KeyError("upstream.commit missing")
        if not identity.get("toolchain_line"):
            raise KeyError("toolchain_line missing")
    except KeyError as exc:
        raise HarnessFail(f"oracle identity {path} is incomplete: {exc}")
    return identity


def check_oracle_binary(identity: dict, oracle_path: Path) -> None:
    """The oracle must exist and hash-match the pin, wherever it is run from
    (--oracle overrides which file is executed, never the expected hash)."""
    if not oracle_path.is_file():
        raise HarnessNotRun(
            f"oracle binary not found at {oracle_path} (pinned sha256 "
            f"{identity['binary']['sha256']}); the oracle lives outside this "
            "repository and must be built/verified there first")
    actual = sha256_file(oracle_path)
    if actual != identity["binary"]["sha256"]:
        raise HarnessFail(
            f"oracle identity mismatch: {oracle_path} sha256 {actual} != "
            f"pinned {identity['binary']['sha256']} — refusing to render "
            "with an unpinned binary")


def run_verify_reference(dexed_root: str | None) -> None:
    """Step 1: the R01 verifier gates every render. Exit 1 (drift) -> FAIL,
    exit 2 (environment) or anything else -> NOT_RUN."""
    cmd = [sys.executable, str(VERIFY_REFERENCE)]
    if dexed_root:
        cmd += ["--dexed-root", dexed_root]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    sys.stdout.write(proc.stdout)
    sys.stderr.write(proc.stderr)
    if proc.returncode == 0:
        return
    if proc.returncode == 1:
        raise HarnessFail(
            "verify_reference.py reported drift (exit 1); refusing to render")
    raise HarnessNotRun(
        f"verify_reference.py did not run (exit {proc.returncode}); "
        "render is NOT_RUN")


def run_oracle(oracle: Path, voice: Path, events: Path, out: Path,
               meta: Path | None) -> subprocess.CompletedProcess:
    cmd = [str(oracle), "--voice", str(voice), "--events", str(events),
           "--out", str(out), "--rate", str(RENDER_RATE),
           "--seconds", str(RENDER_SECONDS)]
    if meta is not None:
        cmd += ["--meta", str(meta)]
    return subprocess.run(cmd, capture_output=True, text=True)


def decode_fixture_voice(voice_path: Path) -> dict:
    """Sanity: the fixture voice must decode via the P01 codec (issue #8)."""
    src = REPO_ROOT / "src"
    if str(src) not in sys.path:
        sys.path.insert(0, str(src))
    from gf180_dx7 import sysex
    try:
        voice = sysex.decode_voice(voice_path.read_bytes())
    except sysex.SysexError as exc:
        raise HarnessFail(f"fixture voice does not decode via P01: {exc}")
    return {
        "name": voice.name,
        "algorithm": voice.algorithm,
        "feedback": voice.feedback,
        "anomalies": list(voice.anomalies),
        "operator_output_levels": [op.output_level for op in voice.operators],
    }


def render_stats(samples: array.array) -> dict:
    peak = max((abs(s) for s in samples), default=0.0)
    rms = math.sqrt(sum(s * s for s in samples) / len(samples)) if samples else 0.0
    dc = sum(samples) / len(samples) if samples else 0.0
    return {"peak_abs": peak, "rms": rms, "dc_offset": dc}


def f32_to_pcm16_bytes(samples: array.array) -> bytes:
    """Convenience-copy conversion: clamp to [-1, 1], scale by 32767,
    round-half-to-even (Python round), saturate to int16."""
    out = bytearray()
    for s in samples:
        v = round(max(-1.0, min(1.0, s)) * 32767)
        v = max(-32768, min(32767, v))
        out += struct.pack("<h", v)
    return bytes(out)


def write_wav16(path: Path, samples: array.array, rate: int) -> None:
    """WAV convenience copy is WAVE_FORMAT_PCM 16-bit integer, mono: the
    stdlib wave module cannot write WAVE_FORMAT_IEEE_FLOAT, and the raw
    render.f32 remains the exact evidence of record."""
    with wave.open(str(path), "wb") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(rate)
        wf.writeframes(f32_to_pcm16_bytes(samples))


def git_repo_state() -> dict:
    def git(*args):
        try:
            proc = subprocess.run(["git", "-C", str(REPO_ROOT)] + list(args),
                                  capture_output=True, text=True, check=True)
        except (OSError, subprocess.CalledProcessError):
            return None
        return proc.stdout.strip()

    head = git("rev-parse", "HEAD")
    porcelain = git("status", "--porcelain")
    return {"repo_git_head": head,
            "repo_dirty_at_render": bool(porcelain) if porcelain is not None
            else None}


def render_fixture(fixture: str, oracle_path: Path, identity_path: Path,
                   evidence_dir: Path, dexed_root: str | None) -> int:
    # Step 1: R01 verifier.
    run_verify_reference(dexed_root)

    # Step 2: oracle identity.
    identity = load_identity(identity_path)
    check_oracle_binary(identity, oracle_path)

    # Fixture inputs.
    fixture_dir = DEFAULT_FIXTURES / fixture
    voice_path = fixture_dir / "voice.syx"
    events_path = fixture_dir / "events.txt"
    for p in (voice_path, events_path):
        if not p.is_file():
            raise HarnessNotRun(f"fixture file missing: {p}")
    voice_bytes = voice_path.read_bytes()
    events_text = events_path.read_text(encoding="utf-8")
    events = parse_events(events_text)
    decode_summary = decode_fixture_voice(voice_path)

    # Steps 3-5: two fresh oracle subprocesses, guard, byte-compare.
    with tempfile.TemporaryDirectory(prefix="r02-render-") as tmp:
        tmp = Path(tmp)
        out1, out2 = tmp / "render1.f32", tmp / "render2.f32"
        meta1 = tmp / "oracle_meta1.json"
        proc1 = run_oracle(oracle_path, voice_path, events_path, out1, meta1)
        if proc1.returncode == 2:
            raise HarnessNotRun(
                f"oracle rejected the inputs (exit 2): {proc1.stderr.strip()}")
        if proc1.returncode != 0:
            raise HarnessFail(
                f"oracle failed (exit {proc1.returncode}): "
                f"{proc1.stderr.strip()}")

        samples1 = read_f32(out1)
        if len(samples1) != RENDER_SAMPLES:
            raise HarnessFail(
                f"oracle produced {len(samples1)} samples, expected "
                f"{RENDER_SAMPLES}")
        guard_peak = enforce_nonsilent_guard(samples1, events)

        proc2 = run_oracle(oracle_path, voice_path, events_path, out2, None)
        if proc2.returncode != 0:
            raise HarnessFail(
                f"determinism re-render failed (exit {proc2.returncode}): "
                f"{proc2.stderr.strip()}")
        data1, data2 = out1.read_bytes(), out2.read_bytes()
        byte_identical = data1 == data2
        if not byte_identical:
            raise HarnessFail(
                "determinism check failed: two fresh oracle subprocesses "
                "disagree byte-wise; no evidence written")

        oracle_meta = None
        if meta1.is_file():
            try:
                oracle_meta = json.loads(meta1.read_text(encoding="utf-8"))
            except json.JSONDecodeError:
                oracle_meta = None

    # Step 6: evidence (only after guard + determinism passed).
    evidence_dir.mkdir(parents=True, exist_ok=True)
    f32_path = evidence_dir / "render.f32"
    wav_path = evidence_dir / "render.wav"
    json_path = evidence_dir / "render.json"
    f32_path.write_bytes(data1)
    samples = array.array("f", data1)
    write_wav16(wav_path, samples, RENDER_RATE)

    metadata = {
        "renderer": "tools/render_reference.py",
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/7",
        "fixture": fixture,
        "oracle_command": [str(oracle_path), "--voice", str(voice_path),
                           "--events", str(events_path), "--out", "<temp>",
                           "--rate", str(RENDER_RATE), "--seconds",
                           str(RENDER_SECONDS), "--meta", "<temp>"],
        "oracle_exit_code": proc1.returncode,
        "oracle_stdout": proc1.stdout.strip(),
        "inputs": {
            "voice": {"path": str(voice_path),
                      "sha256": sha256_bytes(voice_bytes)},
            "events": {"path": str(events_path),
                       "sha256": sha256_bytes(events_text.encode("utf-8"))},
        },
        "voice_decode_p01": decode_summary,
        "events_parsed": [{k: e[k] for k in ("pos", "cmd", "args", "block")}
                          for e in events],
        "output": {"file": "render.f32", "sha256": sha256_bytes(data1),
                   "bytes": len(data1), "samples": len(samples1),
                   "format": "pcm_f32le mono (raw, headerless)"},
        "wav_convenience_copy": {
            "file": "render.wav",
            "format": "WAVE_FORMAT_PCM, 16-bit integer, mono "
                      "(stdlib wave cannot write IEEE float; render.f32 is "
                      "the exact evidence of record)",
            "conversion": "clamp to [-1,1], scale by 32767, "
                          "round-half-to-even, saturate to int16",
            "sha256": sha256_file(wav_path),
        },
        "sample_rate": RENDER_RATE,
        "seconds": RENDER_SECONDS,
        "oracle_identity": identity,
        "identity_verified": True,
        "determinism": {
            "protocol": "second render in a fresh subprocess, byte-compare",
            "byte_identical": byte_identical,
        },
        "silent_output_guard": {
            "threshold_max_abs": SILENT_THRESHOLD,
            "triggered": False,
            "observed_peak_abs": guard_peak,
        },
        "stats": render_stats(samples),
        "oracle_meta_echo": oracle_meta,
        "event_to_block_policy": identity.get(
            "event_to_block_policy",
            "events applied at 64-sample block starts"),
        **git_repo_state(),
        "listening": LISTENING_STATUS,
        "fidelity_note": identity.get(
            "fidelity_note",
            "Software-reference agreement only; not original-DX7 fidelity "
            "and not a musical-quality claim."),
    }
    json_path.write_text(json.dumps(metadata, indent=2, sort_keys=True) + "\n",
                         encoding="utf-8")

    print(f"render_reference: PASS — {fixture}: "
          f"{len(samples1)} samples, peak {metadata['stats']['peak_abs']:.4f}, "
          f"determinism byte-identical, guard clear")
    print(f"  evidence: {f32_path}")
    print(f"  listening: {LISTENING_STATUS}")
    return 0


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Render a fixture with the pinned external oracle (R02)")
    parser.add_argument("--fixture", default="smoke",
                        help="fixture name under reference/fixtures/ "
                             "(default: smoke)")
    parser.add_argument("--oracle", type=Path, default=None,
                        help="oracle binary override (must still hash-match "
                             "the pinned sha256)")
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY,
                        help=f"oracle identity file "
                             f"(default: {DEFAULT_IDENTITY})")
    parser.add_argument("--evidence-dir", type=Path, default=DEFAULT_EVIDENCE,
                        help=f"evidence output directory "
                             f"(default: {DEFAULT_EVIDENCE})")
    parser.add_argument("--dexed-root", default=os.environ.get("DEXED_ROOT"),
                        help="local Dexed clone for verify_reference.py "
                             "(default: env DEXED_ROOT or verifier default)")
    args = parser.parse_args(argv)

    oracle_path = args.oracle
    try:
        if oracle_path is None:
            oracle_path = Path(
                load_identity(args.identity)["binary"]["path"])
        return render_fixture(args.fixture, oracle_path, args.identity,
                              args.evidence_dir / args.fixture,
                              args.dexed_root)
    except HarnessFail as exc:
        print(f"render_reference: FAIL — {exc}", file=sys.stderr)
        return 1
    except HarnessNotRun as exc:
        print(f"render_reference: NOT_RUN — {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
