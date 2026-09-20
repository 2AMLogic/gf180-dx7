#!/usr/bin/env python3
"""R05 non-invasive reference trace capture (issue #12).

Drives the EXTERNAL GPL oracle (pinned in reference/oracle-identity.json)
TWICE on the same fixture inputs — once without --trace and once with — and
writes trace evidence only if the non-invasiveness gate holds:

  1. load + validate reference/trace-registry.json (the versioned tap spec;
     every unavailable tap must carry an explicit reason, never silent);
  2. load reference/oracle-identity.json and re-hash the pinned binary
     (abort on mismatch or absence);
  3. render the fixture untraced and traced (one fresh subprocess each);
  4. byte-identity gate: the two PCM outputs must be identical. A trace that
     perturbs the render is a broken instrument — the capture FAILS and NO
     artifact is written (non-invasiveness is a gate, not a claim);
  5. trace gate: the JSONL must carry exactly one valid line per 64-sample
     block with the registry's block-level fields (block order, 16-bit masks,
     active_notes == popcount(live_mask));
  6. only then write reference/traces/<fixture>.jsonl (verbatim oracle bytes)
     and reference/traces/<fixture>-meta.json (provenance + both PCM hashes +
     byte-identity verdict).

Exit codes: 0 PASS, 1 FAIL (gate or trace malformed; nothing written),
2 NOT_RUN (environment: missing oracle/fixture/registry, unusable inputs).

Scope note: this tool proves the pinned oracle does not perturb its own
output when tracing; it does not re-run the R01 Dexed-pin verifier
(tools/render_reference.py remains the R01-gated render path). It also does
not claim the reference is faithful to an original DX7.

Stdlib-only.
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
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_IDENTITY = REPO_ROOT / "reference" / "oracle-identity.json"
DEFAULT_REGISTRY = REPO_ROOT / "reference" / "trace-registry.json"
DEFAULT_FIXTURES = REPO_ROOT / "reference" / "fixtures"
DEFAULT_TRACES = REPO_ROOT / "reference" / "traces"

RENDER_RATE = 48000
RENDER_SECONDS = 4.0
BLOCK_SIZE = 64

COMMIT_RE = re.compile(r"^[0-9a-f]{40}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")

REQUIRED_TRACE_FIELDS = ("block", "lfo_value", "lfo_delay", "live_mask",
                         "keydown_mask", "active_notes")
# Columns comparable ACROSS oracle processes: the wrapper bookkeeping taps.
# lfo_value/lfo_delay are instance-dependent (upstream msfa leaves the LFO
# start state uninitialized — see reference/trace-registry.json) and are
# excluded from cross-run comparison.
DETERMINISTIC_TRACE_FIELDS = ("block", "live_mask", "keydown_mask",
                              "active_notes")
INT32_MIN, INT32_MAX = -(1 << 31), (1 << 31) - 1
MASK_MAX = 0xFFFF


class CaptureFail(Exception):
    """Exit 1: a gate failed; no artifact may be written."""


class CaptureNotRun(Exception):
    """Exit 2: environment/usage problem; nothing was verified."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def validate_registry(registry: dict) -> None:
    """Schema-check the versioned tap registry (issue #12: unavailable taps
    must be explicitly documented, with reason + what instrumenting
    requires)."""
    if not isinstance(registry, dict):
        raise CaptureFail("trace registry is not a JSON object")
    version = registry.get("registry_version")
    if not isinstance(version, int) or version < 1:
        raise CaptureFail("trace registry missing integer registry_version")
    taps = registry.get("taps")
    if not isinstance(taps, list) or not taps:
        raise CaptureFail("trace registry has no taps list")
    if "non_invasiveness_rule" not in registry:
        raise CaptureFail("trace registry missing non_invasiveness_rule")
    seen = set()
    for tap in taps:
        if not isinstance(tap, dict):
            raise CaptureFail("tap entry is not an object")
        name = tap.get("name")
        if not name or not isinstance(name, str):
            raise CaptureFail("tap without a name")
        if name in seen:
            raise CaptureFail(f"duplicate tap name: {name}")
        seen.add(name)
        granularity = tap.get("granularity")
        if granularity not in ("per_block", "per_render",
                               "per_operator_per_block",
                               "per_voice_per_block"):
            raise CaptureFail(f"tap {name}: bad granularity {granularity!r}")
        if not tap.get("semantics"):
            raise CaptureFail(f"tap {name}: missing semantics")
        available = tap.get("available")
        if not isinstance(available, bool):
            raise CaptureFail(f"tap {name}: missing boolean 'available'")
        if available:
            if not tap.get("stream"):
                raise CaptureFail(
                    f"tap {name}: available tap must name its stream")
        else:
            if not tap.get("reason"):
                raise CaptureFail(
                    f"tap {name}: unavailable tap must document why "
                    "(never silently omitted)")
            if not tap.get("requirements_to_make_available"):
                raise CaptureFail(
                    f"tap {name}: unavailable tap must document what "
                    "instrumenting requires")
            if tap.get("status") not in ("planned", "unavailable"):
                raise CaptureFail(
                    f"tap {name}: unavailable tap must be 'planned' or "
                    "'unavailable'")


def load_registry(path: Path) -> dict:
    try:
        registry = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise CaptureNotRun(f"cannot read trace registry {path}: {exc}")
    except json.JSONDecodeError as exc:
        raise CaptureFail(f"trace registry {path} is not valid JSON: {exc}")
    validate_registry(registry)
    return registry


def load_identity(path: Path) -> dict:
    try:
        identity = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise CaptureNotRun(f"cannot read oracle identity {path}: {exc}")
    except json.JSONDecodeError as exc:
        raise CaptureFail(f"oracle identity {path} is not valid JSON: {exc}")
    try:
        if not COMMIT_RE.match(identity["source"]["git_commit"]):
            raise KeyError("source.git_commit is not a 40-hex sha")
        if not SHA256_RE.match(identity["binary"]["sha256"]):
            raise KeyError("binary.sha256 is not a 64-hex sha")
        if "external-oracle-only" not in identity.get("boundary", ""):
            raise KeyError("boundary must state 'external-oracle-only'")
    except (KeyError, TypeError) as exc:
        raise CaptureFail(f"oracle identity {path} is incomplete: {exc}")
    return identity


def check_oracle_binary(expected_sha: str, oracle_path: Path) -> None:
    if not oracle_path.is_file():
        raise CaptureNotRun(
            f"oracle binary not found at {oracle_path} (pinned sha256 "
            f"{expected_sha}); the oracle lives outside this repository and "
            "must be built/verified there first")
    actual = sha256_file(oracle_path)
    if actual != expected_sha:
        raise CaptureFail(
            f"oracle identity mismatch: {oracle_path} sha256 {actual} != "
            f"pinned {expected_sha} — refusing to capture with an unpinned "
            "binary")


def run_oracle(oracle: Path, voice: Path, events: Path, out: Path,
               trace: Path | None) -> subprocess.CompletedProcess:
    cmd = [str(oracle), "--voice", str(voice), "--events", str(events),
           "--out", str(out), "--rate", str(RENDER_RATE),
           "--seconds", str(RENDER_SECONDS)]
    if trace is not None:
        cmd += ["--trace", str(trace)]
    return subprocess.run(cmd, capture_output=True, text=True)


def parse_trace(path: Path, expected_blocks: int) -> list:
    """Validate the oracle's JSONL trace: exactly one line per 64-sample
    block, sequential block indices, registry field set, in-range values,
    active_notes == popcount(live_mask)."""
    try:
        text = path.read_text(encoding="utf-8")
    except OSError as exc:
        raise CaptureFail(f"cannot read trace {path}: {exc}")
    records = []
    for lineno, line in enumerate(
            (ln for ln in text.splitlines() if ln.strip()), 1):
        try:
            rec = json.loads(line)
        except json.JSONDecodeError as exc:
            raise CaptureFail(f"trace line {lineno}: not valid JSON ({exc})")
        if not isinstance(rec, dict) or set(rec) != set(REQUIRED_TRACE_FIELDS):
            raise CaptureFail(
                f"trace line {lineno}: fields {sorted(rec) if isinstance(rec, dict) else rec} "
                f"!= expected {sorted(REQUIRED_TRACE_FIELDS)}")
        for field in REQUIRED_TRACE_FIELDS:
            if not isinstance(rec[field], int) or isinstance(rec[field], bool):
                raise CaptureFail(f"trace line {lineno}: {field} is not an int")
        if rec["block"] != lineno - 1:
            raise CaptureFail(
                f"trace line {lineno}: block index {rec['block']} out of "
                "sequence")
        for field in ("live_mask", "keydown_mask"):
            if not 0 <= rec[field] <= MASK_MAX:
                raise CaptureFail(
                    f"trace line {lineno}: {field}={rec[field]} outside "
                    f"16-bit range")
        if rec["active_notes"] != bin(rec["live_mask"]).count("1"):
            raise CaptureFail(
                f"trace line {lineno}: active_notes={rec['active_notes']} != "
                f"popcount(live_mask)={bin(rec['live_mask']).count('1')}")
        for field in ("lfo_value", "lfo_delay"):
            if not INT32_MIN <= rec[field] <= INT32_MAX:
                raise CaptureFail(
                    f"trace line {lineno}: {field} outside int32 range")
        records.append(rec)
    if len(records) != expected_blocks:
        raise CaptureFail(
            f"trace has {len(records)} block lines, expected "
            f"{expected_blocks} (one per 64-sample block)")
    return records


def deterministic_columns_sha256(records: list) -> str:
    """Hash of the cross-process-comparable columns (see
    DETERMINISTIC_TRACE_FIELDS). Two captures of the same fixture must agree
    here even though the LFO columns are instance-dependent."""
    canon = "".join(
        json.dumps({k: r[k] for k in DETERMINISTIC_TRACE_FIELDS},
                   sort_keys=True) + "\n"
        for r in records)
    return sha256_bytes(canon.encode("utf-8"))


def run_capture(oracle: Path, voice: Path, events: Path, registry: dict,
                registry_path: Path, trace_out: Path, meta_out: Path,
                expected_binary_sha: str | None, identity: dict | None,
                fixture: str | None = None) -> dict:
    """Two fresh oracle subprocesses (untraced, traced), byte-identity gate,
    trace validation, then artifact write. With expected_binary_sha=None the
    binary pin is skipped (used only by tests driving stub oracles; the CLI
    always pins)."""
    if expected_binary_sha is not None:
        check_oracle_binary(expected_binary_sha, oracle)

    for p in (voice, events):
        if not p.is_file():
            raise CaptureNotRun(f"fixture input missing: {p}")
    voice_bytes = voice.read_bytes()
    events_bytes = events.read_bytes()

    with tempfile.TemporaryDirectory(prefix="r05-capture-") as tmp:
        tmp = Path(tmp)
        out_plain, out_traced = tmp / "plain.f32", tmp / "traced.f32"
        trace_raw = tmp / "trace.jsonl"

        proc = run_oracle(oracle, voice, events, out_plain, None)
        if proc.returncode == 2:
            raise CaptureNotRun(
                f"oracle rejected the inputs (exit 2): {proc.stderr.strip()}")
        if proc.returncode != 0:
            raise CaptureFail(
                f"untraced oracle run failed (exit {proc.returncode}): "
                f"{proc.stderr.strip()}")

        proc = run_oracle(oracle, voice, events, out_traced, trace_raw)
        if proc.returncode == 2:
            raise CaptureNotRun(
                f"oracle rejected the traced inputs (exit 2): "
                f"{proc.stderr.strip()}")
        if proc.returncode != 0:
            raise CaptureFail(
                f"traced oracle run failed (exit {proc.returncode}): "
                f"{proc.stderr.strip()}")

        plain, traced = out_plain.read_bytes(), out_traced.read_bytes()
        if len(plain) != len(traced):
            raise CaptureFail(
                f"non-invasiveness gate: untraced render is {len(plain)} "
                f"bytes, traced render is {len(traced)} bytes")
        if plain != traced:
            # Non-invasiveness is a gate, not a claim: refuse every artifact.
            raise CaptureFail(
                "non-invasiveness gate FAILED: tracing perturbed the render "
                "(traced PCM differs byte-wise from untraced PCM); refusing "
                "to write trace artifacts")

        n_samples = len(plain) // 4
        if len(plain) % 4 or n_samples % BLOCK_SIZE:
            raise CaptureFail(
                f"render is {len(plain)} bytes: not whole f32 samples in "
                f"64-sample blocks")
        records = parse_trace(trace_raw, n_samples // BLOCK_SIZE)

        trace_bytes = trace_raw.read_bytes()

    # All gates passed — only now may artifacts be written.
    trace_out.parent.mkdir(parents=True, exist_ok=True)
    trace_out.write_bytes(trace_bytes)

    available = [t["name"] for t in registry["taps"] if t["available"]]
    unavailable = [t["name"] for t in registry["taps"] if not t["available"]]
    meta = {
        "capture": "tools/capture_trace.py",
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/12",
        "fixture": fixture,
        "registry": {
            "path": str(registry_path),
            "sha256": sha256_file(registry_path),
            "registry_version": registry["registry_version"],
            "available_taps": available,
            "unavailable_taps": unavailable,
        },
        "oracle_binary": {"path": str(oracle),
                          "sha256": sha256_file(oracle)},
        "oracle_identity": identity,
        "identity_verified": expected_binary_sha is not None,
        "inputs": {
            "voice": {"path": str(voice),
                      "sha256": sha256_bytes(voice_bytes)},
            "events": {"path": str(events),
                       "sha256": sha256_bytes(events_bytes)},
        },
        "sample_rate": RENDER_RATE,
        "seconds": RENDER_SECONDS,
        "samples": n_samples,
        "pcm": {
            "untraced": {"sha256": sha256_bytes(plain), "bytes": len(plain),
                         "samples": n_samples,
                         "format": "pcm_f32le mono (raw, headerless)"},
            "traced": {"sha256": sha256_bytes(traced), "bytes": len(traced)},
        },
        "byte_identity": {
            "identical": True,
            "protocol": "same fixture rendered twice by one oracle binary "
                        "(fresh subprocess each): once without --trace and "
                        "once with; f32 bytes compared exactly",
            "status": "PASS",
        },
        "non_invasiveness": "PASS (gate enforced before artifact write)",
        "trace": {
            "file": trace_out.name,
            "sha256": sha256_bytes(trace_bytes),
            "blocks": len(records),
            "block_size": BLOCK_SIZE,
            "fields": list(REQUIRED_TRACE_FIELDS),
            "first_live_block": next(
                (r["block"] for r in records if r["live_mask"]), None),
            "live_blocks": sum(1 for r in records if r["live_mask"]),
            "stability": {
                "deterministic_columns": list(DETERMINISTIC_TRACE_FIELDS),
                "deterministic_columns_sha256":
                    deterministic_columns_sha256(records),
                "instance_dependent_columns": ["lfo_value", "lfo_delay"],
                "note": "lfo_value/lfo_delay read the upstream msfa LFO, "
                        "whose start state is uninitialized (see "
                        "reference/trace-registry.json); they are comparable "
                        "within this captured render only",
            },
        },
    }
    meta_out.write_text(json.dumps(meta, indent=2, sort_keys=True) + "\n",
                        encoding="utf-8")
    return meta


def capture_fixture(fixture: str, oracle_path: Path, identity_path: Path,
                    registry_path: Path, traces_dir: Path) -> int:
    registry = load_registry(registry_path)
    identity = load_identity(identity_path)
    oracle = oracle_path if oracle_path is not None else Path(
        identity["binary"]["path"])
    check_oracle_binary(identity["binary"]["sha256"], oracle)

    fixture_dir = DEFAULT_FIXTURES / fixture
    voice, events = fixture_dir / "voice.syx", fixture_dir / "events.txt"
    if not voice.is_file() or not events.is_file():
        raise CaptureNotRun(f"fixture file missing under {fixture_dir}")

    meta = run_capture(
        oracle, voice, events, registry, registry_path,
        traces_dir / f"{fixture}.jsonl", traces_dir / f"{fixture}-meta.json",
        expected_binary_sha=identity["binary"]["sha256"],
        identity=identity, fixture=fixture)

    print(f"capture_trace: PASS — {fixture}: {meta['trace']['blocks']} block "
          f"lines, traced/untraced PCM byte-identical "
          f"({meta['pcm']['untraced']['sha256'][:12]}…), registry v"
          f"{meta['registry']['registry_version']} "
          f"({len(meta['registry']['available_taps'])} available / "
          f"{len(meta['registry']['unavailable_taps'])} documented-unavailable "
          "taps)")
    print(f"  trace: {traces_dir / (fixture + '.jsonl')}")
    return 0


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Non-invasive reference trace capture (R05, issue #12)")
    parser.add_argument("--fixture", default="smoke",
                        help="fixture name under reference/fixtures/ "
                             "(default: smoke)")
    parser.add_argument("--oracle", type=Path, default=None,
                        help="oracle binary override (must still hash-match "
                             "the pinned sha256)")
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY,
                        help=f"oracle identity file "
                             f"(default: {DEFAULT_IDENTITY})")
    parser.add_argument("--registry", type=Path, default=DEFAULT_REGISTRY,
                        help=f"tap registry (default: {DEFAULT_REGISTRY})")
    parser.add_argument("--traces-dir", type=Path, default=DEFAULT_TRACES,
                        help=f"trace artifact directory "
                             f"(default: {DEFAULT_TRACES})")
    args = parser.parse_args(argv)
    try:
        return capture_fixture(args.fixture, args.oracle, args.identity,
                               args.registry, args.traces_dir)
    except CaptureFail as exc:
        print(f"capture_trace: FAIL — {exc}", file=sys.stderr)
        return 1
    except CaptureNotRun as exc:
        print(f"capture_trace: NOT_RUN — {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
