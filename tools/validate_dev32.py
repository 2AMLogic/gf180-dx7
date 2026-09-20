#!/usr/bin/env python3
"""R03 render-proof for the 32-patch development manifest (issue #10).

1. validate the committed corpus/dev32.json against the committed P02
   catalog (tools/select_dev32.py --validate logic; zip-free);
2. with --render-sample N (default 3): extract the FIRST N manifest entries
   in manifest order from the pinned archive zip, re-derive each voice as a
   single-voice SysEx message via the P01 codec, re-check its canonical hash
   against the manifest, and render it with the EXTERNAL GPL oracle pinned
   in reference/oracle-identity.json using that entry's own events and
   render settings;
3. enforce the non-silent guard on every render and record output hashes,
   peaks and identities in corpus/dev32-renderproof.json (committed).

Scope: a RENDERABILITY proof (every sampled entry renders non-silently
through the pinned oracle). It is not a comparison, not an agreement claim,
and not a musical-quality claim. Full-32 rendering is a later gate; oracle
determinism and comparison apparatus qualification are R02/R04.

Exit codes: 0 PASS, 1 FAIL (validation, hash drift, silent output),
2 NOT_RUN (environment: missing manifest/catalog/zip/oracle).

Stdlib-only.
"""

from __future__ import annotations

import argparse
import importlib.util
import json
import subprocess
import sys
import tempfile
import zipfile
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402

DEFAULT_CATALOG = REPO_ROOT / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
DEFAULT_MANIFEST = REPO_ROOT / "corpus" / "dev32.json"
DEFAULT_PROOF = REPO_ROOT / "corpus" / "dev32-renderproof.json"
DEFAULT_ZIP = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")
DEFAULT_IDENTITY = REPO_ROOT / "reference" / "oracle-identity.json"

SCHEMA = "gf180-dx7.dev32-renderproof/1"
SILENT_THRESHOLD = 1e-6


class Fail(Exception):
    """Exit 1: a check failed; no proof is written."""


class NotRun(Exception):
    """Exit 2: environment problem; nothing was verified."""


def load_module(name: str, path: Path):
    if not path.is_file():
        raise NotRun(f"required module missing: {path}")
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod  # dataclass processing resolves its own module
    spec.loader.exec_module(mod)  # type: ignore[union-attr]
    return mod


SELECT = load_module("dev32_select", REPO_ROOT / "tools" / "select_dev32.py")
RENDER_REF = load_module("dev32_render_reference",
                         REPO_ROOT / "tools" / "render_reference.py")
CATALOG_ARCHIVE = load_module("dev32_catalog_archive",
                              REPO_ROOT / "tools" / "catalog_archive.py")


def sha256_file(path: Path) -> str:
    return SELECT.sha256_file(path)


def events_text(events: list[dict]) -> str:
    """Serialize manifest events to oracle events.txt line format."""
    lines = ["# gf180-dx7 dev32 render-proof events (issue #10)",
             "# positions are absolute sample offsets at the entry's rate"]
    for ev in events:
        args = " ".join(str(a) for a in ev["args"])
        lines.append(f"{ev['pos']} {ev['cmd']} {args}")
    return "\n".join(lines) + "\n"


def extract_voice_bytes(zf, entry: dict) -> bytes:
    """Single-voice SysEx message bytes for a manifest entry, hash-checked."""
    path, slot, want = entry["path"], entry["slot"], entry["canonical_hash"]
    data = zf.read(path)
    if slot is None:
        voice = sysex.decode_voice(data)
        message = data
    else:
        bank = sysex.decode_bank(data)
        if not 0 <= slot < len(bank.voices):
            raise Fail(f"entry {entry['index']}: slot {slot} outside bank"
                       f" {path}")
        voice = bank.voices[slot]
        message = sysex.encode_voice(voice)
    got = CATALOG_ARCHIVE.canonical_hash(voice)
    if got != want:
        raise Fail(
            f"entry {entry['index']}: canonical hash drift for {path}"
            f" slot={slot}: archive decodes to {got}, manifest pins {want}")
    return message


def render_entry(oracle: Path, zf, entry: dict, tmp: Path,
                 executed: dict) -> dict:
    message = extract_voice_bytes(zf, entry)
    voice_path = tmp / "voice.syx"
    events_path = tmp / "events.txt"
    out_path = tmp / "render.f32"
    voice_path.write_bytes(message)
    text = events_text(entry["events"])
    events_path.write_text(text, encoding="utf-8")

    render = entry["render"]
    cmd = [str(oracle), "--voice", str(voice_path), "--events",
           str(events_path), "--out", str(out_path),
           "--rate", str(render["sample_rate"]),
           "--seconds", str(render["seconds"])]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode == 2:
        raise NotRun(f"oracle rejected entry {entry['index']} inputs"
                     f" (exit 2): {proc.stderr.strip()}")
    if proc.returncode != 0:
        raise Fail(f"oracle failed on entry {entry['index']}"
                   f" (exit {proc.returncode}): {proc.stderr.strip()}")
    samples = RENDER_REF.read_f32(out_path)
    if len(samples) != render["samples"]:
        raise Fail(f"entry {entry['index']}: oracle produced "
                   f"{len(samples)} samples, entry declares "
                   f"{render['samples']}")
    peak = RENDER_REF.enforce_nonsilent_guard(samples, entry["events"],
                                              SILENT_THRESHOLD)
    stats = RENDER_REF.render_stats(samples)
    return {
        "index": entry["index"],
        "path": entry["path"],
        "slot": entry["slot"],
        "name": entry["name"],
        "canonical_hash": entry["canonical_hash"],
        "voice_message_sha256": RENDER_REF.sha256_bytes(message),
        "events_text_sha256": RENDER_REF.sha256_bytes(text.encode("utf-8")),
        "events": entry["events"],
        "render": render,
        "output": {
            "format": "pcm_f32le mono (raw, headerless)",
            "samples": len(samples),
            "sha256": RENDER_REF.sha256_bytes(out_path.read_bytes()),
            "peak_abs": stats["peak_abs"],
            "rms": stats["rms"],
        },
        "non_silent_guard": {
            "threshold_max_abs": SILENT_THRESHOLD,
            "triggered": False,
            "observed_peak_abs": peak,
        },
        "executed_binary": dict(executed),
    }


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Validate dev32 and prove sample renderability (R03)")
    parser.add_argument("--catalog", type=Path, default=DEFAULT_CATALOG)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--zip", type=Path, default=DEFAULT_ZIP)
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY)
    parser.add_argument("--oracle", type=Path, default=None,
                        help="oracle binary override (must still hash-match"
                        " the pinned sha256; the recorded identity pin is"
                        " never relaxed)")
    parser.add_argument("--render-sample", type=int, default=3,
                        help="how many manifest entries to render (0 ="
                        " validation only; default 3)")
    parser.add_argument("--out", type=Path, default=DEFAULT_PROOF)
    args = parser.parse_args(argv)
    try:
        catalog = SELECT.load_catalog(args.catalog)
        if not args.manifest.is_file():
            raise NotRun(f"manifest not found: {args.manifest}")
        try:
            manifest = json.loads(
                args.manifest.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            raise Fail(f"manifest is not valid JSON: {exc}")
        catalog_sha = sha256_file(args.catalog)
        problems = SELECT.validate_manifest(manifest, catalog, catalog_sha)
        if problems:
            for p in problems:
                print(f"dev32-renderproof FAIL: {p}", file=sys.stderr)
            raise Fail(f"manifest validation failed with {len(problems)}"
                       " problem(s)")

        renders: list[dict] = []
        identity = None
        if args.render_sample > 0:
            identity = RENDER_REF.load_identity(args.identity)
            oracle = Path(args.oracle) if args.oracle \
                else Path(identity["binary"]["path"])
            RENDER_REF.check_oracle_binary(identity, oracle)
            executed = {"path": str(oracle),
                        "sha256": identity["binary"]["sha256"],
                        "note": "hash-verified against the pinned identity"
                                " before rendering"}
            if not args.zip.is_file():
                raise NotRun(
                    f"pinned archive zip not found at {args.zip}; render"
                    " proof requires it to extract the manifest voices")
            actual_zip = sha256_file(args.zip)
            if actual_zip != manifest["selection"]["archive"]["sha256"]:
                raise Fail(f"archive drift: {args.zip} sha256 {actual_zip}"
                           f" != manifest pin")
            picks = manifest["entries"][:args.render_sample]
            if len(picks) < args.render_sample:
                raise Fail("manifest has fewer entries than"
                           f" --render-sample {args.render_sample}")
            with zipfile.ZipFile(args.zip) as zf:
                with tempfile.TemporaryDirectory(
                        prefix="dev32-renderproof-") as tmp:
                    tmp = Path(tmp)
                    for entry in picks:
                        proof = render_entry(oracle, zf, entry, tmp,
                                             executed)
                        renders.append(proof)
                        print(f"dev32-renderproof: entry {proof['index']}"
                              f" ({proof['name'].strip()}): peak "
                              f"{proof['output']['peak_abs']:.4f}, "
                              f"sha256 {proof['output']['sha256'][:16]}...")

        proof_doc = {
            "schema": SCHEMA,
            "issue": "https://github.com/2AMLogic/gf180-dx7/issues/10",
            "generated_utc": datetime.now(timezone.utc).strftime(
                "%Y-%m-%dT%H:%M:%SZ"),
            "manifest": {"path": str(args.manifest),
                         "sha256": sha256_file(args.manifest)},
            "catalog": {"path": str(args.catalog),
                        "sha256": catalog_sha},
            "validated": True,
            "validation_problems": [],
            "render_sample_size": args.render_sample,
            "selection_rule": "first N entries in manifest order",
            "full_32_gate": {
                "run": False,
                "note": "full-32 rendering is a later gate; this proof "
                        "samples the manifest only",
            },
            "oracle_identity": identity,
            "scope": "renderability proof: non-silent output through the "
                     "pinned oracle using each entry's own events and "
                     "render settings; not a comparison, agreement, or "
                     "musical-quality claim (R02/R04 own those)",
            "fidelity_note": (identity or {}).get(
                "fidelity_note",
                "Software-reference agreement only; not original-DX7"
                " fidelity."),
            "renders": renders,
        }
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(json.dumps(proof_doc, indent=2) + "\n",
                            encoding="utf-8")
        print(f"dev32-renderproof PASS: manifest validated,"
              f" {len(renders)}/{args.render_sample} sampled entries"
              f" rendered non-silent; wrote {args.out}")
        return 0
    except Fail as exc:
        print(f"dev32-renderproof FAIL: {exc}", file=sys.stderr)
        return 1
    except NotRun as exc:
        print(f"dev32-renderproof NOT_RUN: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
