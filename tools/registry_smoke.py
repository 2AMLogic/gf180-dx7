#!/usr/bin/env python3
"""R06 registry smoke: run the registry's short subset against the pinned
external oracle (issue #13).

For every case in spec/compatibility-registry-v1.json's short_subset:

  1. build the synthetic patch bytes via the P01 codec (registry
     synthetic_base + the case's overrides), verifying a decode round trip;
  2. render one dry f32 render with the pinned oracle binary (identity pin
     re-hashed before any render);
  3. evaluate the case's property-level expectation where it is mechanically
     verifiable (non-silence, hash-distinctness versus partner cases,
     onset windows, all-sound-off silence, finite summation, applied-event
     counts);
  4. record manual expectations explicitly as NO_VERDICT (method "manual")
     — they are human listening judgments and are never guessed;
  5. blocked cases (no executable oracle path) are recorded BLOCKED with
     their reason; corpus-selector cases are recorded NOT_RUN (voice bytes
     resolve only from the pinned archive at challenge gates).

Writes reference/evidence/registry-smoke.json (committed evidence: per-case
verdict, PCM/voice/events hashes, input fingerprints, oracle identity pin
check). The run is fast by contract: the short subset must finish well
under 30 s.

Exit codes: 0 = every machine verdict PASS, 1 = any FAIL,
2 = environment (oracle absent/unpinned, registry unreadable).

No fidelity claim: oracle agreement is software-reference agreement only
(reference/oracle-protocol.md section 4, DEC-001). Stdlib-only.
"""

from __future__ import annotations

import argparse
import array
import hashlib
import json
import math
import subprocess
import sys
import tempfile
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402
from render_reference import (  # noqa: E402
    RENDER_RATE,
    HarnessFail,
    HarnessNotRun,
    check_oracle_binary,
    load_identity,
    read_f32,
)

sys.path.insert(0, str(REPO_ROOT / "tools"))
from registry_coverage import input_fingerprint, load_registry  # noqa: E402

DEFAULT_REGISTRY = REPO_ROOT / "spec" / "compatibility-registry-v1.json"
DEFAULT_IDENTITY = REPO_ROOT / "reference" / "oracle-identity.json"
DEFAULT_EVIDENCE = REPO_ROOT / "reference" / "evidence" / "registry-smoke.json"

SMOKE_SCHEMA = "gf180-dx7.registry-smoke/1"
SILENT_THRESHOLD = 1e-6
EXACT_SILENT_THRESHOLD = 1e-9
PEAK_BOUND = 16.0  # 16 voices x per-voice clip bound; finite-peak check

FIDELITY_NOTE = ("Software-reference agreement only (pinned Dexed Mark I "
                 "oracle); not original-DX7 fidelity and not a "
                 "musical-quality claim (DEC-001).")


class SmokeFail(Exception):
    """Exit 1: a case verdict failed (evidence still written)."""


# ---------------------------------------------------------------------------
# Patch building (P01 codec)


def build_voice(reg: dict, case_row: dict) -> sysex.Voice:
    """Synthetic base + case overrides -> Voice; raises HarnessFail on
    unknown fields or unencodable values."""
    base = reg["synthetic_base"]
    common = dict(base["common"])
    operator = dict(base["operator"])
    ops = {n: dict(operator) for n in range(1, 7)}
    for n_str, level in base.get("operator_output_levels", {}).items():
        ops[int(n_str)]["output_level"] = level

    patch = case_row.get("patch", {})
    name = patch.get("name")
    overrides = patch.get("overrides", {})
    for key, val in overrides.items():
        target, sep, field = key.partition(".")
        if sep and target == "op":
            for op in ops.values():
                op[field] = val
        elif sep and target.startswith("op") and target[2:].isdigit():
            ops[int(target[2:])][field] = val
        elif not sep and key in common:
            common[key] = val
        else:
            raise HarnessFail(
                f"case {case_row['id']}: override {key!r} does not name a "
                "synthetic base field")
    if name is not None:
        common["name_bytes"] = name.encode("ascii")[:10]
    voice = sysex.Voice(operators=[sysex.Operator(**ops[n]) for n in range(1, 7)],
                        **common)
    try:
        encoded = sysex.encode_voice(voice)
    except sysex.SysexError as exc:
        raise HarnessFail(f"case {case_row['id']}: P01 encode failed: {exc}")
    decoded = sysex.decode_voice(encoded)
    if decoded.anomalies:
        raise HarnessFail(
            f"case {case_row['id']}: decoded anomalies {decoded.anomalies}")
    return voice


# ---------------------------------------------------------------------------
# Property predicates (mechanical, property-level — never numeric vectors)


def _win(a: array.array, lo: float, hi: float) -> array.array:
    return a[int(RENDER_RATE * lo):int(RENDER_RATE * hi)]


def _peak(a: array.array, lo: float, hi: float) -> float:
    seg = _win(a, lo, hi)
    return max((abs(s) for s in seg), default=0.0)


def _rms(a: array.array, lo: float, hi: float) -> float:
    seg = _win(a, lo, hi)
    if not seg:
        return 0.0
    return math.sqrt(sum(s * s for s in seg) / len(seg))


MANUAL_METHOD = "manual-listening"


def evaluate_property(pred: str, case_row: dict, pcm: array.array,
                      meta: dict | None, hashes: dict[str, str]) -> tuple[bool, str]:
    """Evaluate one machine predicate. Returns (passed, detail). Manual
    predicates are handled by the caller."""
    cid = case_row["id"]
    seconds = float(case_row.get("render_seconds", 1.5))
    if pred == "non_silent_after_onset":
        peak = _peak(pcm, 0.06, seconds)
        return peak >= SILENT_THRESHOLD, f"peak(0.06-{seconds}s)={peak:.6g}"
    if pred == "finite_peak":
        finite = all(math.isfinite(s) for s in pcm)
        peak = max((abs(s) for s in pcm), default=0.0)
        return finite and peak <= PEAK_BOUND, f"finite={finite} peak={peak:.6g}"
    if pred == "rms_rises":
        early, late = _rms(pcm, 0.0, 0.1), _rms(pcm, 0.3, 0.5)
        return late > early, f"rms(0-0.1)={early:.6g} rms(0.3-0.5)={late:.6g}"
    if pred == "fades_to_quiet":
        early, late = _rms(pcm, 0.05, 0.2), _rms(pcm, 0.6, 1.0)
        return late < 0.25 * early, \
            f"rms(0.05-0.2)={early:.6g} rms(0.6-1.0)={late:.6g}"
    if pred == "sustains":
        early, late = _rms(pcm, 0.05, 0.2), _rms(pcm, 0.6, 1.0)
        return late > 0.5 * early, \
            f"rms(0.05-0.2)={early:.6g} rms(0.6-1.0)={late:.6g}"
    if pred == "onset_absent_early":
        peak = _peak(pcm, 0.05, 0.4)
        return peak < SILENT_THRESHOLD, f"peak(0.05-0.4)={peak:.6g}"
    if pred == "rises_later":
        peak = _peak(pcm, 1.2, min(1.9, seconds))
        return peak > 0.01, f"peak(1.2-{min(1.9, seconds)}s)={peak:.6g}"
    if pred == "tail_present_at_end":
        peak = _peak(pcm, 0.9, 1.0)
        return peak > 0.01, f"peak(0.9-1.0)={peak:.6g}"
    if pred == "silent_at_end":
        peak = _peak(pcm, 0.9, 1.0)
        return peak < SILENT_THRESHOLD, f"peak(0.9-1.0)={peak:.6g}"
    if pred == "tail_decays":
        before, after = _peak(pcm, 0.8, 1.0), _peak(pcm, 1.4, 1.5)
        return after < before, f"peak(1.4-1.5)={after:.6g} < peak(0.8-1.0)={before:.6g}"
    if pred == "silent_after_cc120":
        pos = next((int(line.split()[0]) for line in case_row["events"]
                    if line.split()[1:2] == ["cc"] and
                    line.split()[2:3] == ["120"]), None)
        if pos is None:
            return False, "no cc 120 event found"
        before = _peak(pcm, 0.06, max(pos / RENDER_RATE - 0.05, 0.07))
        after_lo = min((pos + 0.1 * RENDER_RATE) / RENDER_RATE, seconds)
        after = _peak(pcm, after_lo, seconds)
        return before >= SILENT_THRESHOLD and after < EXACT_SILENT_THRESHOLD, \
            f"before={before:.6g} after({after_lo:.2f}s-)={after:.6g}"
    if pred.startswith("distinct_hash_vs:"):
        other = pred.split(":", 1)[1]
        if other not in hashes:
            return False, f"partner case {other!r} has no render"
        return hashes[other] != hashes[cid], \
            f"{hashes[cid][:12]} vs {other}:{hashes[other][:12]}"
    if pred.startswith("identical_hash_vs:"):
        other = pred.split(":", 1)[1]
        if other not in hashes:
            return False, f"partner case {other!r} has no render"
        return hashes[other] == hashes[cid], \
            f"{hashes[cid][:12]} == {other}:{hashes[other][:12]}"
    if pred.startswith("late_peak_gt_case:"):
        other = pred.split(":", 1)[1]
        if other not in pcms:
            return False, f"partner case {other!r} has no render"
        mine, theirs = _peak(pcm, 1.2, 1.4), _peak(pcms[other], 1.2, 1.4)
        return mine > theirs, f"{mine:.6g} > {other}:{theirs:.6g}"
    if pred.startswith("late_peak_lt_case:"):
        other = pred.split(":", 1)[1]
        if other not in pcms:
            return False, f"partner case {other!r} has no render"
        mine, theirs = _peak(pcm, 0.55, 1.4), _peak(pcms[other], 0.55, 1.4)
        return mine < theirs, f"{mine:.6g} < {other}:{theirs:.6g}"
    if pred.startswith("events_applied_count:"):
        want = int(pred.split(":", 1)[1])
        got = (meta or {}).get("events_applied")
        return got == want, f"events_applied={got} want={want}"
    raise HarnessFail(f"case {cid}: unknown property predicate {pred!r}")


pcms: dict[str, array.array] = {}


# ---------------------------------------------------------------------------
# Smoke run


def run_smoke(reg: dict, registry_path: Path, oracle_path: Path,
              identity_path: Path, evidence_path: Path, out_dir: Path) -> int:
    identity = load_identity(identity_path)
    check_oracle_binary(identity, oracle_path)
    pin_ok = True

    cases = {c["id"]: c for c in reg["cases"]}
    short = reg["short_subset"]
    rows: list[dict] = []
    hashes: dict[str, str] = {}
    pcms.clear()

    # First pass: render every executable short-subset case.
    renders: dict[str, tuple[array.array, dict | None, bytes, bytes]] = {}
    for cid in short:
        c = cases[cid]
        voice = build_voice(reg, c)
        voice_bytes = sysex.encode_voice(voice)
        if sysex.decode_voice(voice_bytes).anomalies:
            raise HarnessFail(f"case {cid}: round-trip anomalies")
        events_text = "\n".join(c["events"]) + "\n"
        events_bytes = events_text.encode("utf-8")
        vpath = out_dir / f"{cid}.syx"
        epath = out_dir / f"{cid}.events.txt"
        opath = out_dir / f"{cid}.f32"
        mpath = out_dir / f"{cid}.meta.json"
        vpath.write_bytes(voice_bytes)
        epath.write_bytes(events_bytes)
        seconds = float(c.get("render_seconds", 1.5))
        proc = subprocess.run(
            [str(oracle_path), "--voice", str(vpath), "--events", str(epath),
             "--out", str(opath), "--meta", str(mpath),
             "--rate", str(RENDER_RATE), "--seconds", str(seconds)],
            capture_output=True, text=True)
        if proc.returncode != 0:
            raise HarnessFail(
                f"case {cid}: oracle failed (exit {proc.returncode}): "
                f"{proc.stderr.strip()}")
        pcm = read_f32(opath)
        if len(pcm) != int(RENDER_RATE * seconds):
            raise HarnessFail(
                f"case {cid}: oracle produced {len(pcm)} samples, expected "
                f"{int(RENDER_RATE * seconds)}")
        meta = None
        if mpath.is_file():
            meta = json.loads(mpath.read_text(encoding="utf-8"))
        hashes[cid] = hashlib.sha256(pcm.tobytes()).hexdigest()
        pcms[cid] = pcm
        renders[cid] = (pcm, meta, voice_bytes, events_bytes)

    # Second pass: evaluate expectations.
    n_fail = 0
    for cid in short:
        c = cases[cid]
        expect = c.get("expect", {})
        prop = expect.get("property", "")
        also = list(expect.get("also", []))
        predicates = [prop] + also
        machine = [p for p in predicates
                   if not p.startswith("manual:")]
        manual = [p for p in predicates if p.startswith("manual:")]
        fingerprint = input_fingerprint(c)

        if c.get("blocked"):
            rows.append({
                "id": cid, "verdict": "BLOCKED", "manual": True,
                "property": prop, "description": expect.get("description", ""),
                "reason": c.get("blocked_reason"),
                "input_fingerprint": fingerprint,
                "oracle_identity_sha256_ok": pin_ok,
            })
            continue

        pcm, meta, voice_bytes, events_bytes = renders[cid]
        row = {
            "id": cid,
            "property": prop,
            "description": expect.get("description", ""),
            "input_fingerprint": fingerprint,
            "voice_sha256": hashlib.sha256(voice_bytes).hexdigest(),
            "events_sha256": hashlib.sha256(events_bytes).hexdigest(),
            "pcm_sha256": hashes[cid],
            "peak_abs": max((abs(s) for s in pcm), default=0.0),
            "render_seconds": c.get("render_seconds", 1.5),
            "events_applied": (meta or {}).get("events_applied"),
            "oracle_identity_sha256_ok": pin_ok,
        }
        failures = []
        details = []
        for pred in machine:
            ok, detail = evaluate_property(pred, c, pcm, meta, hashes)
            details.append({"predicate": pred, "passed": ok, "detail": detail})
            if not ok:
                failures.append(f"{pred} ({detail})")
        if failures:
            row["verdict"] = "FAIL"
            row["failures"] = failures
            n_fail += 1
        elif manual:
            row["verdict"] = "NO_VERDICT"
            row["manual"] = True
            row["method"] = MANUAL_METHOD
            row["manual_properties"] = manual
            row["note"] = ("manual expectation requires a human listening "
                           "record; machine preconditions passed and are "
                           "never treated as the judgment")
        else:
            row["verdict"] = "PASS"
        row["checks"] = details
        rows.append(row)

    # Blocked / corpus cases NOT in the short subset: registered, not run.
    for cid, c in cases.items():
        if cid in short:
            continue
        fingerprint = input_fingerprint(c)
        expect = c.get("expect", {})
        if c.get("blocked"):
            rows.append({"id": cid, "verdict": "BLOCKED", "manual": True,
                         "property": expect.get("property", ""),
                         "reason": c.get("blocked_reason"),
                         "input_fingerprint": fingerprint,
                         "oracle_identity_sha256_ok": pin_ok})
        elif c["patch"]["selector"] == "corpus":
            rows.append({"id": cid, "verdict": "NOT_RUN", "manual": True,
                         "property": expect.get("property", ""),
                         "reason": "corpus voice bytes resolve from the pinned "
                                   "archive at challenge gates, not in the smoke",
                         "input_fingerprint": fingerprint,
                         "oracle_identity_sha256_ok": pin_ok})

    counts = {"PASS": 0, "FAIL": 0, "NOT_RUN": 0, "BLOCKED": 0, "NO_VERDICT": 0}
    for row in rows:
        counts[row["verdict"]] += 1
    evidence = {
        "schema": SMOKE_SCHEMA,
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/13",
        "registry": str(registry_path.relative_to(REPO_ROOT)),
        "registry_sha256": hashlib.sha256(
            registry_path.read_bytes()).hexdigest(),
        "oracle": {
            "identity_path": str(identity_path.relative_to(REPO_ROOT)),
            "binary_path": str(oracle_path),
            "pinned_sha256": identity["binary"]["sha256"],
            "observed_sha256_match": pin_ok,
        },
        "sample_rate": RENDER_RATE,
        "summary": {
            "cases_total": len(cases),
            "short_subset": len(short),
            "n_rendered": len(renders),
            "n_run": counts["PASS"] + counts["FAIL"],
            "n_pass": counts["PASS"],
            "n_fail": counts["FAIL"],
            "n_manual": counts["NO_VERDICT"],
            "n_blocked": counts["BLOCKED"],
            "n_not_run": counts["NOT_RUN"],
        },
        "cases": sorted(rows, key=lambda r: r["id"]),
        "fidelity_note": FIDELITY_NOTE,
        "determinism_note": ("single render per case; LFO-active renders are "
                             "instance-dependent across oracle processes "
                             "(reference/trace-registry.json), so cross-case "
                             "hash claims are one-sided difference claims and "
                             "identity claims only pair LFO-inert renders"),
        "stale_rule": ("an evidence row whose input_fingerprint differs from the "
                       "current case fingerprint is STALE (see "
                       "docs/REGISTRY-POLICY.md)"),
    }
    evidence_path.parent.mkdir(parents=True, exist_ok=True)
    evidence_path.write_text(
        json.dumps(evidence, indent=2, sort_keys=False) + "\n", encoding="utf-8")

    print(f"registry_smoke: {'PASS' if n_fail == 0 else 'FAIL'} — ran "
          f"{counts['PASS'] + counts['FAIL']} case(s): {counts['PASS']} pass, "
          f"{counts['FAIL']} fail, {counts['NO_VERDICT']} manual, "
          f"{counts['BLOCKED']} blocked, {counts['NOT_RUN']} not-run; "
          f"evidence: {evidence_path}")
    return 0 if n_fail == 0 else 1


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Run the registry short subset against the pinned oracle")
    parser.add_argument("--registry", type=Path, default=DEFAULT_REGISTRY)
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY)
    parser.add_argument("--evidence", type=Path, default=DEFAULT_EVIDENCE)
    parser.add_argument("--oracle", type=Path, default=None)
    args = parser.parse_args(argv)

    try:
        reg = load_registry(args.registry)
    except Exception as exc:
        print(f"registry_smoke: NOT_RUN — cannot read registry: {exc}",
              file=sys.stderr)
        return 2

    oracle_path = args.oracle
    try:
        if oracle_path is None:
            oracle_path = Path(load_identity(args.identity)["binary"]["path"])
        with tempfile.TemporaryDirectory(prefix="r06-smoke-") as tmp:
            return run_smoke(reg, args.registry, oracle_path, args.identity,
                             args.evidence, Path(tmp))
    except HarnessNotRun as exc:
        print(f"registry_smoke: NOT_RUN — {exc}", file=sys.stderr)
        return 2
    except HarnessFail as exc:
        print(f"registry_smoke: FAIL — {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
