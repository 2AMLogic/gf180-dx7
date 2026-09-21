#!/usr/bin/env python3
"""N07 polyphonic-manager case runner (issue #23).

Renders the pinned external GPL oracle (reference/oracle-identity.json,
re-hashed before any render) and the D00 polyphonic manager
(src/gf180_dx7/model/poly.py) side by side and classifies each case:

  * WRAPPER-VALID cross-checks — <=16-voice cases that trigger no stealing
    and where D00 note-off matching coincides with the pinned wrapper's:
    the N06-punted registry cases ev-repeated-note, ev-retrigger,
    ev-sum-15, ev-steal-16 (16-note boundary; the wrapper still allocates
    every note there) and the dev32 velocity_contrast entries (two
    sequential notes; the release tail overlaps the second strike).
    Target: EXACT (byte-identical), same R04 comparator as N06.

  * DIVERGENCE case — ev-steal-17: the pinned wrapper DROPS the 17th note
    (registry finding alloc-17th-drop, docs/REGISTRY-POLICY.md section 5;
    re-confirmed live here byte-for-byte against ev-steal-16), while the
    D00 contract (DEC-017) STEALS deterministically. The mismatch is a
    DOCUMENTED POLICY DIFFERENCE: measured (max abs error, first
    divergence), classified, and never budgeted into equality — our
    product semantics are the D00 policy, not the wrapper quirk.

Evidence: reference/evidence/n07-poly/evidence.json (committed).

Exit codes: 0 = every wrapper-valid case EXACT and the divergence case
present with the wrapper drop confirmed; 1 = any wrapper-valid FAIL or a
missing/changed divergence; 2 = NOT_RUN (environment).

Software-reference agreement only — not original-DX7 fidelity, not RTL
agreement, not a musical claim (DEC-001, plan section 7). Stdlib-only.
"""

from __future__ import annotations

import argparse
import json
import sys
import tempfile
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model.poly import PolyManager  # noqa: E402
from render_reference import (  # noqa: E402
    HarnessFail, HarnessNotRun, check_oracle_binary, load_identity)
from registry_coverage import input_fingerprint, load_registry  # noqa: E402
from registry_smoke import build_voice  # noqa: E402

RATE = 48000
DEFAULT_IDENTITY = REPO / "reference" / "oracle-identity.json"
DEFAULT_REGISTRY = REPO / "spec" / "compatibility-registry-v1.json"
DEFAULT_DEV32 = REPO / "corpus" / "dev32.json"
DEFAULT_EVIDENCE = REPO / "reference" / "evidence" / "n07-poly"
DEFAULT_ARCHIVE = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")

SCHEMA = "gf180-dx7.n07-poly/1"
FIDELITY_NOTE = ("Software-reference agreement only (pinned Dexed Mark I "
                 "oracle); not original-DX7 fidelity, not RTL agreement, "
                 "not a musical-quality claim (DEC-001). D00 event policy "
                 "(docs/DECISIONS-v1.md DEC-014..DEC-023) is the model "
                 "semantics; the pinned wrapper is comparison-valid only "
                 "for <=16-voice cases with no stealing.")

# Registry cases the single-note N06 model punted as N07_SCOPE and that
# are wrapper-valid (no stealing triggered; note-off matching coincides).
WRAPPER_VALID_REGISTRY = [
    "ev-repeated-note",
    "ev-retrigger",
    "ev-sum-15",
    "ev-steal-16",
]

# dev32 velocity_contrast entries (two sequential notes; second strike
# lands during the first voice's release tail). Entry 3 is the primary
# row requested by N07; entries 6 and 30 add algorithm/LFO diversity
# (both wave-4 LFO: determinate under the pinned wrapper, finding F-2).
DEV32_VELOCITY_CONTRAST = [3, 6, 30]
DEV32_PRIMARY = 3

# The documented D00-vs-wrapper divergence case.
DIVERGENCE_CASE = "ev-steal-17"


def sha256_bytes(data: bytes) -> str:
    import hashlib
    return hashlib.sha256(data).hexdigest()


def oracle_render(oracle_path: Path, voice_bytes: bytes, events: str,
                  seconds: float, tmp: Path, tag: str) -> bytes:
    vpath = tmp / f"{tag}-voice.syx"
    epath = tmp / f"{tag}-events.txt"
    opath = tmp / f"{tag}-out.f32"
    vpath.write_bytes(voice_bytes)
    epath.write_text(events, encoding="utf-8")
    proc = subprocess_run(oracle_path, vpath, epath, opath, seconds)
    if proc.returncode != 0:
        raise RuntimeError(f"oracle failed (exit {proc.returncode}): "
                           f"{proc.stderr.strip()}")
    data = opath.read_bytes()
    if len(data) != 4 * int(RATE * seconds):
        raise RuntimeError("oracle produced an unexpected length")
    return data


def subprocess_run(oracle_path, vpath, epath, opath, seconds):
    import subprocess
    return subprocess.run(
        [str(oracle_path), "--voice", str(vpath), "--events", str(epath),
         "--out", str(opath), "--rate", str(RATE),
         "--seconds", str(seconds)], capture_output=True, text=True)


def registry_row(oracle_path: Path, reg, cases, cid: str, tmp: Path,
                 tag: str) -> dict:
    """One wrapper-valid registry case: oracle vs PolyManager, exact."""
    case = cases[cid]
    voice = build_voice(reg, case)
    voice_bytes = sysex.encode_voice(voice)
    events = "\n".join(case["events"]) + "\n"
    seconds = float(case.get("render_seconds", 1.5))
    oracle_pcm = oracle_render(oracle_path, voice_bytes, events, seconds,
                               tmp, tag)
    manager = PolyManager([voice])
    model_pcm = manager.render(events, seconds)
    report = compare.compare_streams(oracle_pcm, model_pcm)
    primary = report.primary or {}
    row = {
        "id": cid,
        "class": "wrapper_valid",
        "voice_sha256": sha256_bytes(voice_bytes),
        "events_sha256": sha256_bytes(events.encode("utf-8")),
        "render_seconds": seconds,
        "input_fingerprint": input_fingerprint(case),
        "expect_property": case.get("expect", {}).get("property", ""),
        "oracle_pcm_sha256": report.sha256_a,
        "model_pcm_sha256": report.sha256_b,
        "verdict": "EXACT" if report.passed else "FAIL",
        "status": "PASS" if report.passed else "FAIL",
        "byte_identical": report.byte_identical,
        "max_abs_error": primary.get("max_abs_error"),
        "mismatch_count": primary.get("mismatch_count"),
        "first_divergent_sample": primary.get("first_divergent_sample"),
        "n_allocations": len(manager.alloc_log),
        "boundary_note": "16 slots are never exhausted here: the pinned "
                         "wrapper still allocates every note, so the "
                         "comparison triggers no stealing (D00 and the "
                         "wrapper coincide)",
    }
    return row


def dev32_rows(oracle_path: Path, archive: Path, tmp: Path) -> list[dict]:
    """dev32 velocity_contrast entries; guarded NOT_RUN without the
    pinned archive (corpus policy: bytes never redistributed)."""
    manifest = json.loads(DEFAULT_DEV32.read_text(encoding="utf-8"))
    by_index = {e["index"]: e for e in manifest["entries"]}
    rows: list[dict] = []
    try:
        zf = zipfile.ZipFile(archive)
    except (OSError, zipfile.BadZipFile) as exc:
        for idx in DEV32_VELOCITY_CONTRAST:
            rows.append({
                "id": f"dev32-{idx:02d}", "class": "wrapper_valid",
                "verdict": "NOT_RUN", "status": "NOT_RUN",
                "reason": f"pinned archive unavailable ({exc}); dev32 "
                          "voice bytes resolve only from the pinned "
                          "archive",
            })
        return rows
    from n06_compare import dev32_voice_bytes, events_text
    with zf:
        for idx in DEV32_VELOCITY_CONTRAST:
            entry = by_index[idx]
            voice_bytes = dev32_voice_bytes(zf, entry)
            events = events_text(entry["events"])
            seconds = float(entry["render"]["seconds"])
            oracle_pcm = oracle_render(oracle_path, voice_bytes, events,
                                       seconds, tmp, f"dev{idx:02d}")
            manager = PolyManager([sysex.decode_voice(voice_bytes)])
            model_pcm = manager.render(events, seconds)
            report = compare.compare_streams(oracle_pcm, model_pcm)
            primary = report.primary or {}
            rows.append({
                "id": f"dev32-{idx:02d}",
                "class": "wrapper_valid",
                "primary": idx == DEV32_PRIMARY,
                "profile": entry["render"]["profile"],
                "name": entry["name"],
                "voice_sha256": sha256_bytes(voice_bytes),
                "events_sha256": sha256_bytes(events.encode("utf-8")),
                "render_seconds": seconds,
                "oracle_pcm_sha256": report.sha256_a,
                "model_pcm_sha256": report.sha256_b,
                "verdict": "EXACT" if report.passed else "FAIL",
                "status": "PASS" if report.passed else "FAIL",
                "byte_identical": report.byte_identical,
                "max_abs_error": primary.get("max_abs_error"),
                "mismatch_count": primary.get("mismatch_count"),
                "first_divergent_sample": primary.get(
                    "first_divergent_sample"),
                "n_allocations": len(manager.alloc_log),
                "boundary_note": "velocity_contrast: the second strike "
                                 "lands during the first voice's release "
                                 "tail (two live voices, 15 free slots); "
                                 "no stealing on either side",
            })
    return rows


def divergence_row(oracle_path: Path, reg, cases, tmp: Path) -> dict:
    """ev-steal-17: render both sides, measure, classify."""
    case = cases[DIVERGENCE_CASE]
    case16 = cases["ev-steal-16"]
    voice = build_voice(reg, case)
    voice_bytes = sysex.encode_voice(voice)
    events17 = "\n".join(case["events"]) + "\n"
    events16 = "\n".join(case16["events"]) + "\n"
    seconds = float(case.get("render_seconds", 1.5))
    oracle17 = oracle_render(oracle_path, voice_bytes, events17, seconds,
                             tmp, "div17")
    oracle16 = oracle_render(oracle_path, voice_bytes, events16, seconds,
                             tmp, "div16")
    manager = PolyManager([voice])
    model17 = manager.render(events17, seconds)
    report = compare.compare_streams(oracle17, model17)
    primary = report.primary or {}
    steals = [e for e in manager.alloc_log if e["kind"] == "steal-dec017"]
    wrapper_drop_confirmed = oracle16 == oracle17
    row = {
        "id": DIVERGENCE_CASE,
        "class": "policy_divergence",
        "voice_sha256": sha256_bytes(voice_bytes),
        "events_sha256": sha256_bytes(events17.encode("utf-8")),
        "render_seconds": seconds,
        "oracle_pcm_sha256": report.sha256_a,
        "model_pcm_sha256": report.sha256_b,
        "comparison": "DIFFER",
        "verdict": "POLICY_DIFFERENCE",
        "status": "PASS",
        "classification": (
            "documented policy difference: the pinned wrapper drops the "
            "17th note (registry finding alloc-17th-drop; oracle render "
            "byte-identical to the 16-note render: "
            f"{str(wrapper_drop_confirmed).lower()}), while the product "
            "policy is D00 DEC-017 deterministic stealing. Our model "
            "implements the D00 contract; this row is a measured "
            "divergence record, NOT a defect and NOT a budgeted equality "
            "(no agreement claim is made or implied on this case)"),
        "wrapper_alloc_17th_drop_confirmed": wrapper_drop_confirmed,
        "max_abs_error": primary.get("max_abs_error"),
        "mismatch_count": primary.get("mismatch_count"),
        "first_divergent_sample": primary.get("first_divergent_sample"),
        "first_divergent_block": (primary["first_divergent_sample"] // 64
                                  if primary.get("first_divergent_sample")
                                  is not None else None),
        "model_allocations": {
            "n_allocations": len(manager.alloc_log),
            "steal_entries": steals,
            "decision": "DEC-017: prefer voices in release, then the "
                        "oldest sounding voice (earliest note-on "
                        "sequence); the 17th note re-struck the oldest "
                        "held voice (slot 0, pitch 60)",
        },
        "registered_wrapper_expectation": case16.get("expect", {})
        .get("property", ""),
    }
    return row


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--oracle", type=Path, default=None)
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY)
    parser.add_argument("--evidence-dir", type=Path, default=DEFAULT_EVIDENCE)
    parser.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
    parser.add_argument("--registry-only", action="store_true")
    args = parser.parse_args(argv)

    try:
        identity = load_identity(args.identity)
        oracle_path = args.oracle or Path(identity["binary"]["path"])
        check_oracle_binary(identity, oracle_path)
    except HarnessFail as exc:
        print(f"n07_compare: FAIL — {exc}", file=sys.stderr)
        return 1
    except HarnessNotRun as exc:
        print(f"n07_compare: NOT_RUN — {exc}", file=sys.stderr)
        return 2

    reg = load_registry(DEFAULT_REGISTRY)
    cases = {c["id"]: c for c in reg["cases"]}
    out_path = args.evidence_dir / "evidence.json"

    n_problems = 0
    with tempfile.TemporaryDirectory(prefix="n07-compare-") as td:
        tmp = Path(td)
        registry_rows = [registry_row(oracle_path, reg, cases, cid, tmp,
                                      f"reg{i}")
                         for i, cid in enumerate(WRAPPER_VALID_REGISTRY)]
        dev_rows = ([] if args.registry_only
                    else dev32_rows(oracle_path, args.archive, tmp))
        div = divergence_row(oracle_path, reg, cases, tmp)

        flat_valid = registry_rows + dev_rows
        n_problems += sum(1 for r in flat_valid if r["verdict"] != "EXACT")
        if div["verdict"] != "POLICY_DIFFERENCE" or \
                not div["wrapper_alloc_17th_drop_confirmed"]:
            n_problems += 1

        by_verdict = {}
        for r in flat_valid:
            by_verdict[r["verdict"]] = by_verdict.get(r["verdict"], 0) + 1
        evidence = {
            "schema": SCHEMA,
            "issue": "https://github.com/2AMLogic/gf180-dx7/issues/23",
            "model": "src/gf180_dx7/model/poly.py (N07 D00 polyphonic "
                     "state/event manager around the N06 integrated "
                     "model)",
            "policy": "docs/DECISIONS-v1.md DEC-014..DEC-023 (D00 event "
                      "policy); stealing per DEC-017, repeated notes per "
                      "DEC-016, retrigger per DEC-015, sustain per "
                      "DEC-018, patch binding per DEC-019",
            "comparator": "src/gf180_dx7/compare.py (exact mode, dry; "
                          "docs/COMPARE-CONTRACT.md)",
            "oracle": {
                "identity_path": "reference/oracle-identity.json",
                "pinned_sha256": identity["binary"]["sha256"],
                "observed_sha256_match": True,
            },
            "sample_rate": RATE,
            "wrapper_validity_boundary": (
                "the pinned wrapper drops the 17th note (alloc-17th-drop, "
                "docs/REGISTRY-POLICY.md section 5) and matches note-offs "
                "by channel while mpeEnabled; the D00 model is compared "
                "against it only on <=16-voice cases that trigger no "
                "stealing and where the note-off matchings coincide"),
            "summary": {
                "wrapper_valid_total": len(flat_valid),
                "wrapper_valid_by_verdict": by_verdict,
                "policy_divergence": 1 if div else 0,
                "not_run": sum(1 for r in flat_valid
                               if r["verdict"] == "NOT_RUN"),
            },
            "sections": {
                "wrapper_valid_registry": registry_rows,
                "wrapper_valid_dev32_velocity_contrast": dev_rows,
                "policy_divergence": [div],
            },
            "fidelity_note": FIDELITY_NOTE,
        }
        args.evidence_dir.mkdir(parents=True, exist_ok=True)
        out_path.write_text(json.dumps(evidence, indent=2) + "\n",
                            encoding="utf-8")

    verdict = "PASS" if n_problems == 0 else "FAIL"
    print(f"n07_compare: {verdict} — evidence: {out_path}")
    return 0 if n_problems == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
