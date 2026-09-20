#!/usr/bin/env python3
"""R03 deterministic 32-patch development-manifest selector (issue #10).

Selects 32 DEVELOPMENT patches from the committed P02 archive catalog with
EXPLICIT feature coverage. This is a compatibility corpus, not a favorites
list: selection is fully deterministic and documented (docs/CORPUS-POLICY.md);
taste and curation are U02/U03 and are deliberately absent here.

Selection stages (in this order, see docs/CORPUS-POLICY.md):
  A. Algorithm coverage: for algorithm 0..31 in ascending order, the
     lexicographically-first valid voice instance with that algorithm
     (sort key: zip path, then bank slot with standalone voices first,
     then catalog order).
  B. Feature goals, in the fixed GOAL order below: a goal already covered by
     the selected set is skipped; otherwise the lexicographically-first
     unused valid instance satisfying the goal is appended (if the manifest
     has capacity) or swaps into its algorithm's slot (keeping that
     algorithm covered).
  C. Fill: remaining slots up to 32 take the lexicographically-first unused
     valid instances.

"Valid voice instance" = a decoded, anomaly-free voice instance from the
committed catalog: standalone `voice` entries and `bank` slot rows with
anomaly_count == 0. Stream-class instances are excluded from selection
(27 files / 1728 instances vs 307k bank instances; documented scope).
All 32 algorithms are covered by clean instances in this catalog, so no
fill is normally needed.

Feature decoding requires the pinned archive zip (read-only). The zip is
SHA-256-checked against the catalog pin before use. If the zip is absent,
selection exits 2 (NOT_RUN) naming the path (skip-with-reason);
`--validate` never needs the zip: it checks the committed manifest against
the committed catalog alone.

Exit codes: 0 PASS, 1 FAIL (drift, validation, or archive mismatch),
2 NOT_RUN (environment: missing catalog/zip).

Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import sys
import zipfile
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402

DEFAULT_CATALOG = REPO_ROOT / "corpus" / "archive-catalogs" / "alltheweb-catalog.json"
DEFAULT_MANIFEST = REPO_ROOT / "corpus" / "dev32.json"
DEFAULT_ZIP = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")

SCHEMA = "gf180-dx7.dev32/1"
MANIFEST_SIZE = 32

SAMPLE_RATE = 48000
NOTE_PITCH = 60

# Pitch-EG level 50 is the zero-deviation center: the MSFA pitch envelope
# table (Dexed Source/msfa/pitchenv.cc, Apache-2.0, external reference only)
# maps level 50 to a pitch offset of 0. A voice "has" a pitch EG when any
# level deviates from that center. Decode-only rule; no engine is involved.
PITCH_EG_CENTER = 50
# Feedback goals: high vs low ends of the 0-7 range.
HIGH_FEEDBACK_MIN = 4
LOW_FEEDBACK_MAX = 0
# A voice counts as "slow" (long-hold render profile) when any operator
# envelope rate drops to this level or below.
SLOW_RATE_MAX = 25

VELOCITY_STRIKE_VEL = 30


class Fail(Exception):
    """Exit 1: input drift or validation failure."""


class NotRun(Exception):
    """Exit 2: environment problem; nothing was verified."""


@dataclass(frozen=True)
class Features:
    """Feature set extracted from one decoded DX7 voice."""

    algorithm: int
    feedback: int
    lfo_wave: int
    pitch_eg_present: bool
    fixed_mode_operators: int
    velocity_sensitive: bool
    min_operator_rate: int

    @property
    def high_feedback(self) -> bool:
        return self.feedback >= HIGH_FEEDBACK_MIN

    @property
    def low_feedback(self) -> bool:
        return self.feedback <= LOW_FEEDBACK_MAX

    @property
    def slow(self) -> bool:
        return self.min_operator_rate <= SLOW_RATE_MAX

    def as_dict(self) -> dict:
        return {
            "algorithm": self.algorithm,
            "feedback": self.feedback,
            "lfo_wave": self.lfo_wave,
            "pitch_eg_present": self.pitch_eg_present,
            "fixed_mode_operators": self.fixed_mode_operators,
            "velocity_sensitive": self.velocity_sensitive,
            "high_feedback": self.high_feedback,
            "low_feedback": self.low_feedback,
            "min_operator_rate": self.min_operator_rate,
        }


def voice_features(v: sysex.Voice) -> Features:
    return Features(
        algorithm=v.algorithm,
        feedback=v.feedback,
        lfo_wave=v.lfo_wave,
        pitch_eg_present=any(
            lvl != PITCH_EG_CENTER
            for lvl in (v.pitch_eg_level_1, v.pitch_eg_level_2,
                        v.pitch_eg_level_3, v.pitch_eg_level_4)),
        fixed_mode_operators=sum(1 for op in v.operators if op.osc_mode == 1),
        velocity_sensitive=any(op.key_velocity_sensitivity > 0
                               for op in v.operators),
        min_operator_rate=min(min(op.rate_1, op.rate_2, op.rate_3, op.rate_4)
                              for op in v.operators),
    )


# Feature goals in fixed application order (docs/CORPUS-POLICY.md).
GOALS: tuple[tuple[str, object], ...] = (
    ("pitch_eg_present", lambda f: f.pitch_eg_present),
    ("lfo_wave_0", lambda f: f.lfo_wave == 0),
    ("lfo_wave_1", lambda f: f.lfo_wave == 1),
    ("lfo_wave_2", lambda f: f.lfo_wave == 2),
    ("lfo_wave_3", lambda f: f.lfo_wave == 3),
    ("fixed_frequency_op", lambda f: f.fixed_mode_operators > 0),
    ("high_feedback", lambda f: f.high_feedback),
    ("low_feedback", lambda f: f.low_feedback),
    ("velocity_sensitive", lambda f: f.velocity_sensitive),
)


@dataclass(frozen=True)
class Instance:
    """One voice instance from the catalog (standalone file or bank slot)."""

    path: str
    slot: int | None  # None for a standalone single-voice file
    canonical_hash: str
    name: str
    order: int

    @property
    def sort_key(self) -> tuple[str, int, int]:
        return (self.path, -1 if self.slot is None else self.slot, self.order)


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def load_catalog(path: Path) -> dict:
    if not path.is_file():
        raise NotRun(f"committed catalog not found: {path}")
    try:
        with path.open("r", encoding="utf-8") as fh:
            return json.load(fh)
    except (OSError, json.JSONDecodeError) as exc:
        raise Fail(f"catalog unreadable: {exc}")


def clean_instances(catalog: dict) -> list[Instance]:
    """All decoded, anomaly-free voice instances in catalog order."""
    hashes: list[str] = catalog["hashes"]
    names: list[str] = catalog["names"]
    out: list[Instance] = []
    for order, e in enumerate(catalog["entries"]):
        if e["c"] == "voice" and e["v"] == "decoded" and e["a"] == 0 \
                and e["h"] is not None:
            out.append(Instance(e["p"], None, e["h"], e["n"], order))
        elif e["c"] == "bank" and e["v"] == "decoded":
            w = e["w"]
            for k in range(0, len(w), 4):
                slot, hidx, nidx, nanom = w[k:k + 4]
                if nanom == 0 and hidx >= 0:
                    out.append(Instance(e["p"], slot, hashes[hidx],
                                        names[nidx], order))
    out.sort(key=lambda inst: inst.sort_key)
    return out


def compute_features(catalog: dict, zip_path: Path,
                     progress: bool = False) -> dict[str, Features]:
    """Decode every distinct canonical hash's features from the archive.

    Reads the zip read-only. The zip is pinned by SHA-256 against the
    catalog before any decode; features are functions of the canonical
    parameters, so one decode per hash suffices and instances of the same
    hash are never re-decoded.
    """
    archive_sha = catalog["archive"]["sha256"]
    if not zip_path.is_file():
        raise NotRun(
            f"pinned archive zip not found at {zip_path} (sha256 {archive_sha}"
            f" per catalog); selection requires the archive to decode voice"
            f" features — use --validate to check the committed manifest"
            f" without the zip")
    actual = sha256_file(zip_path)
    if actual != archive_sha:
        raise Fail(
            f"archive drift: {zip_path} sha256 {actual} != catalog pin "
            f"{archive_sha}; refusing to select from a different archive")
    hashes: list[str] = catalog["hashes"]
    feats: dict[str, Features] = {}
    pending: dict[str, list[tuple[str, int | None]]] = {}
    for e in catalog["entries"]:
        if e["c"] == "voice" and e["v"] == "decoded" and e["h"] is not None:
            pending.setdefault(e["p"], []).append((e["h"], None))
        elif e["c"] == "bank" and e["v"] == "decoded":
            w = e["w"]
            rows = [(hashes[w[k + 1]], w[k]) for k in range(0, len(w), 4)
                    if w[k + 1] >= 0]
            pending.setdefault(e["p"], []).extend(rows)
    with zipfile.ZipFile(zip_path) as zf:
        for i, path in enumerate(sorted(pending)):
            rows = pending[path]
            rows = [(h, slot) for h, slot in rows if h not in feats]
            if not rows:
                continue
            data = zf.read(path)
            if rows[0][1] is None:
                feats[rows[0][0]] = voice_features(sysex.decode_voice(data))
            else:
                bank = sysex.decode_bank(data)
                for h, slot in rows:
                    if h not in feats:
                        feats[h] = voice_features(bank.voices[slot])
            if progress and (i + 1) % 1000 == 0:
                print(f"[select] {i + 1}/{len(pending)} files,"
                      f" {len(feats)} hashes decoded",
                      file=sys.stderr, flush=True)
    return feats


def events_for(f: Features) -> tuple[list[dict], dict, str]:
    """Renderer-executable events + render settings for one voice.

    Profiles (docs/CORPUS-POLICY.md):
      slow_hold          any operator rate <= 25: 3 s hold, 6 s render
      velocity_contrast  velocity-sensitive: strikes at velocity 100 then 30
      default            single strike at velocity 100, 2 s hold, 4 s render
    """
    on = {"cmd": "note_on", "args": [NOTE_PITCH, 100]}
    off = {"cmd": "note_off", "args": [NOTE_PITCH]}
    if f.slow:
        events = [{**on, "pos": 4800}, {**off, "pos": 144000}]
        render = {"sample_rate": SAMPLE_RATE, "seconds": 6.0,
                  "samples": SAMPLE_RATE * 6, "profile": "slow_hold"}
    elif f.velocity_sensitive:
        soft = {"pos": 72000, "cmd": "note_on",
                "args": [NOTE_PITCH, VELOCITY_STRIKE_VEL]}
        events = [{**on, "pos": 4800}, {**off, "pos": 48000}, soft,
                  {**off, "pos": 120000}]
        render = {"sample_rate": SAMPLE_RATE, "seconds": 4.0,
                  "samples": SAMPLE_RATE * 4, "profile": "velocity_contrast"}
    else:
        events = [{**on, "pos": 4800}, {**off, "pos": 96000}]
        render = {"sample_rate": SAMPLE_RATE, "seconds": 4.0,
                  "samples": SAMPLE_RATE * 4, "profile": "default"}
    return events, render, render["profile"]


def select_entries(instances: list[Instance], feats: dict[str, Features],
                   size: int = MANIFEST_SIZE) -> tuple[list[dict], dict]:
    """Documented deterministic selection; returns entries + stage info."""
    if len({i.canonical_hash for i in instances}) < size:
        raise Fail(f"catalog offers fewer than {size} distinct clean voice"
                   " hashes; cannot build the manifest")
    used: set[str] = set()
    by_alg: dict[int, tuple[Instance, list[str]]] = {}
    appended: list[tuple[Instance, list[str]]] = []
    stage_info: dict = {"stage_a": {}, "stage_b_swaps": [],
                        "stage_b_appends": [], "stage_c_fills": 0,
                        "goal_holes": [], "algorithms_missing": []}

    def total() -> int:
        return len(by_alg) + len(appended)

    # Stage A: algorithm coverage.
    for inst in instances:
        alg = feats[inst.canonical_hash].algorithm
        if alg in by_alg or inst.canonical_hash in used:
            continue
        by_alg[alg] = (inst, [f"algorithm:{alg}"])
        used.add(inst.canonical_hash)
    stage_info["algorithms_missing"] = [a for a in range(32) if a not in by_alg]
    stage_info["stage_a"] = {
        str(a): by_alg[a][0].path for a in sorted(by_alg)}

    # Stage B: feature goals.
    selected = list(by_alg.values()) + appended

    def covered(pred) -> bool:
        return any(pred(feats[i.canonical_hash]) for i, _ in selected)

    for tag, pred in GOALS:
        if covered(pred):
            continue
        picked = False
        for inst in instances:
            if inst.canonical_hash in used or not pred(
                    feats[inst.canonical_hash]):
                continue
            alg = feats[inst.canonical_hash].algorithm
            if alg in by_alg:
                old, _ = by_alg[alg]
                used.discard(old.canonical_hash)
                by_alg[alg] = (inst, [f"algorithm:{alg}", tag])
                stage_info["stage_b_swaps"].append(
                    {"goal": tag, "algorithm": alg, "path": inst.path,
                     "slot": inst.slot, "replaced": old.path})
            elif total() < size:
                appended.append((inst, [tag]))
                stage_info["stage_b_appends"].append(
                    {"goal": tag, "path": inst.path, "slot": inst.slot})
            else:
                break
            used.add(inst.canonical_hash)
            picked = True
            break
        if not picked:
            stage_info["goal_holes"].append(tag)
        selected = list(by_alg.values()) + appended

    # Stage C: fill remaining slots.
    while total() < size:
        for inst in instances:
            if inst.canonical_hash not in used:
                appended.append((inst, ["fill"]))
                used.add(inst.canonical_hash)
                stage_info["stage_c_fills"] += 1
                break
        else:
            raise Fail("candidate universe exhausted before manifest size")

    entries = []
    idx = 0
    for a in sorted(by_alg):
        inst, tags = by_alg[a]
        entries.append((inst, tags))
    entries.extend(appended)
    out = []
    for idx, (inst, tags) in enumerate(entries):
        f = feats[inst.canonical_hash]
        events, render, profile = events_for(f)
        out.append({
            "index": idx,
            "path": inst.path,
            "slot": inst.slot,
            "name": inst.name,
            "canonical_hash": inst.canonical_hash,
            "anomaly_free": True,
            "selected_features": tags,
            "decoded_features": f.as_dict(),
            "events": events,
            "render": render,
        })
        idx += 1
    return out, stage_info


def compute_coverage(entries: list[dict]) -> dict:
    """Feature-coverage table recomputed from per-entry decoded features."""
    algs = sorted({e["decoded_features"]["algorithm"] for e in entries})
    # JSON object keys are strings; keep the table stable across round trips.
    lfo = {str(w): sum(1 for e in entries
                       if e["decoded_features"]["lfo_wave"] == w)
           for w in range(4)}
    profiles: dict[str, int] = {}
    for e in entries:
        profile = (e.get("render") or {}).get("profile")
        profiles[profile] = profiles.get(profile, 0) + 1
    return {
        "algorithms_covered": algs,
        "algorithms_missing": [a for a in range(32) if a not in algs],
        "algorithm_holes": [a for a in range(32) if a not in algs],
        "lfo_waves_covered": lfo,
        "lfo_wave_goals_unmet": [w for w in range(4) if lfo[str(w)] == 0],
        "fixed_mode_count": sum(1 for e in entries
                                if e["decoded_features"]
                                ["fixed_mode_operators"] > 0),
        "pitch_eg_count": sum(1 for e in entries
                              if e["decoded_features"]["pitch_eg_present"]),
        "high_feedback_count": sum(1 for e in entries
                                   if e["decoded_features"]["high_feedback"]),
        "low_feedback_count": sum(1 for e in entries
                                  if e["decoded_features"]["low_feedback"]),
        "velocity_sensitive_count": sum(
            1 for e in entries
            if e["decoded_features"]["velocity_sensitive"]),
        "render_profiles": dict(sorted(profiles.items(),
                                       key=lambda kv: str(kv[0]))),
    }


def build_manifest(catalog_path: Path, zip_path: Path,
                   progress: bool = False) -> dict:
    catalog = load_catalog(catalog_path)
    instances = clean_instances(catalog)
    if progress:
        print(f"[select] clean candidate instances: {len(instances)}",
              file=sys.stderr, flush=True)
    feats = compute_features(catalog, zip_path, progress=progress)
    entries, stage_info = select_entries(instances, feats)
    coverage = compute_coverage(entries)
    coverage["goal_holes"] = stage_info["goal_holes"]
    return {
        "schema": SCHEMA,
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/10",
        "generated_utc": datetime.now(timezone.utc).strftime(
            "%Y-%m-%dT%H:%M:%SZ"),
        "counts": {"entries": len(entries)},
        "selection": {
            "tool": "tools/select_dev32.py",
            "tool_sha256": sha256_file(Path(__file__).resolve()),
            "catalog": {
                "path": str(catalog_path.relative_to(REPO_ROOT)),
                "schema": catalog["schema"],
                "snapshot_sha256": sha256_file(catalog_path),
            },
            "catalog_snapshot_sha256": sha256_file(catalog_path),
            "archive": dict(catalog["archive"]),
            "selection_zip": {
                "path": str(zip_path),
                "sha256_verified": catalog["archive"]["sha256"],
                "note": "archive read-only for feature decoding; bytes are "
                        "never redistributed (catalog rights policy)",
            },
            "candidate_universe": {
                "clean_instances": len(instances),
                "distinct_clean_hashes": len({i.canonical_hash
                                              for i in instances}),
                "scope": "standalone voice entries + bank slot rows,"
                         " anomaly_count == 0; stream-class instances"
                         " excluded from selection",
            },
            "goals": [{"tag": tag} for tag, _ in GOALS],
            "stages": stage_info,
        },
        "feature_goal_definitions": {
            "pitch_eg_present": f"any pitch EG level != {PITCH_EG_CENTER}"
                                " (level center = zero deviation, MSFA"
                                " pitchenv table convention)",
            "lfo_wave_W": "voice LFO waveform == W (goals 0-3 per issue #10)",
            "fixed_frequency_op": "at least one operator osc_mode == 1"
                                  " (fixed frequency)",
            "high_feedback": f"feedback >= {HIGH_FEEDBACK_MIN} (0-7 range)",
            "low_feedback": f"feedback <= {LOW_FEEDBACK_MAX} (0-7 range)",
            "velocity_sensitive": "at least one operator"
                                  " key_velocity_sensitivity > 0",
        },
        "render_convention": {
            "note_pitch": NOTE_PITCH,
            "profiles": {
                "default": "note_on vel 100 @ 4800; note_off @ 96000; 4.0 s",
                "velocity_contrast": "note_on vel 100 @ 4800; note_off "
                                     "@ 48000; note_on vel 30 @ 72000; "
                                     "note_off @ 120000; 4.0 s",
                "slow_hold": f"any operator rate <= {SLOW_RATE_MAX}: "
                             "note_on vel 100 @ 4800; note_off @ 144000; "
                             "6.0 s",
            },
            "precedence": "slow_hold over velocity_contrast over default",
        },
        "coverage": coverage,
        "entries": entries,
    }


# ---------------------------------------------------------------------------
# Validation (zip-free): committed manifest against committed catalog.
# ---------------------------------------------------------------------------

EVENT_CMDS = {"note_on": 2, "note_off": 1}


def validate_events(entry: dict) -> list[str]:
    problems = []
    events = entry.get("events")
    if not isinstance(events, list) or not events:
        return [f"entry {entry.get('index')}: events missing or empty"]
    samples = entry.get("render", {}).get("samples")
    last = -1
    for ev in events:
        pos, cmd, args = ev.get("pos"), ev.get("cmd"), ev.get("args")
        if not isinstance(pos, int) or isinstance(pos, bool) or pos < 0:
            problems.append(f"entry {entry['index']}: bad event pos {pos!r}")
            continue
        if pos < last:
            problems.append(f"entry {entry['index']}: events not in"
                            " non-decreasing position order")
        last = pos
        if cmd not in EVENT_CMDS:
            problems.append(f"entry {entry['index']}: unknown event cmd"
                            f" {cmd!r}")
            continue
        if not isinstance(args, list) or len(args) != EVENT_CMDS[cmd] \
                or not all(isinstance(a, int) and not isinstance(a, bool)
                           for a in args):
            problems.append(f"entry {entry['index']}: bad args for {cmd}:"
                            f" {args!r}")
            continue
        if cmd == "note_on" and not (0 <= args[0] <= 127 and
                                     1 <= args[1] <= 127):
            problems.append(f"entry {entry['index']}: note_on out of range:"
                            f" {args!r}")
        if samples is not None and pos >= samples:
            problems.append(f"entry {entry['index']}: event pos {pos} at or"
                            f" past render end ({samples} samples)")
    return problems


def validate_render(entry: dict) -> list[str]:
    render = entry.get("render")
    if not isinstance(render, dict):
        return [f"entry {entry.get('index')}: render settings missing"]
    problems = []
    for key in ("sample_rate", "seconds", "samples", "profile"):
        if key not in render:
            problems.append(f"entry {entry['index']}: render.{key} missing")
    if problems:
        return problems
    if render["sample_rate"] != SAMPLE_RATE:
        problems.append(f"entry {entry['index']}: render.sample_rate "
                        f"{render['sample_rate']} != convention {SAMPLE_RATE}")
    expected = int(render["sample_rate"] * render["seconds"])
    if render["samples"] != expected:
        problems.append(f"entry {entry['index']}: render.samples "
                        f"{render['samples']} != rate*seconds {expected}")
    if render["seconds"] <= 0:
        problems.append(f"entry {entry['index']}: render.seconds must be"
                        " positive")
    return problems


def catalog_resolution_index(catalog: dict) -> dict[tuple[str, int | None], str]:
    """(path, slot) -> canonical_hash for every decoded voice instance."""
    idx: dict[tuple[str, int | None], str] = {}
    for e in catalog["entries"]:
        if e["c"] == "voice" and e["v"] == "decoded":
            idx[(e["p"], None)] = e["h"]
        elif e["c"] == "bank" and e["v"] == "decoded":
            hashes = catalog["hashes"]
            w = e["w"]
            for k in range(0, len(w), 4):
                slot, hidx = w[k], w[k + 1]
                if hidx >= 0:
                    idx[(e["p"], slot)] = hashes[hidx]
    return idx


def validate_manifest(manifest: dict, catalog: dict,
                      catalog_sha256: str) -> list[str]:
    """Full zip-free validation; returns a list of problems (empty = PASS)."""
    problems: list[str] = []
    if manifest.get("schema") != SCHEMA:
        problems.append(f"schema {manifest.get('schema')!r} != {SCHEMA!r}")
    entries = manifest.get("entries")
    if not isinstance(entries, list) or len(entries) != MANIFEST_SIZE:
        return [f"manifest must hold exactly {MANIFEST_SIZE} entries,"
                f" found {len(entries) if isinstance(entries, list) else 'no'}"
                " entry list"]
    hashes = catalog["hashes"]
    hash_set = set(hashes)
    if len({e.get("canonical_hash") for e in entries}) != MANIFEST_SIZE:
        problems.append("canonical hashes are not unique across entries")
    resolution = catalog_resolution_index(catalog)
    for e in entries:
        i = e.get("index")
        chash = e.get("canonical_hash")
        if not isinstance(chash, str) or len(chash) != 64:
            problems.append(f"entry {i}: canonical_hash missing/malformed")
            continue
        if chash not in hash_set:
            problems.append(f"entry {i}: canonical_hash {chash} does not"
                            " resolve in the committed catalog")
            continue
        key = (e.get("path"), e.get("slot"))
        if key not in resolution:
            problems.append(f"entry {i}: source path/slot {key!r} does not"
                            " resolve to a decoded catalog instance")
        elif resolution[key] != chash:
            problems.append(
                f"entry {i}: catalog hash at {key!r} is {resolution[key]}"
                f" != manifest {chash}")
        if not e.get("selected_features"):
            problems.append(f"entry {i}: selected_features missing/empty")
        if not e.get("decoded_features"):
            problems.append(f"entry {i}: decoded_features missing")
        if "name" not in e:
            problems.append(f"entry {i}: name missing")
        problems.extend(validate_events(e))
        problems.extend(validate_render(e))
    # Coverage table must match per-entry features recomputed independently.
    recomputed = compute_coverage(entries)
    recorded = manifest.get("coverage")
    if not isinstance(recorded, dict):
        problems.append("coverage table missing")
    else:
        for key, want in recomputed.items():
            if recorded.get(key) != want:
                problems.append(f"coverage.{key} mismatch: recorded "
                                f"{recorded.get(key)!r} != recomputed {want!r}")
    # Selection provenance must pin this exact catalog snapshot.
    selection = manifest.get("selection")
    if not isinstance(selection, dict):
        problems.append("selection provenance block missing")
    else:
        if selection.get("catalog_snapshot_sha256") != catalog_sha256:
            problems.append(
                "catalog drifted since manifest generation"
                f" (snapshot {selection.get('catalog_snapshot_sha256')}"
                f" != current {catalog_sha256}); manifest is STALE")
        archive = selection.get("archive") or {}
        if archive != catalog["archive"]:
            problems.append("selection.archive does not match the catalog"
                            " archive identity (missing archive identity"
                            " is not allowed)")
    return problems


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Select/validate the R03 32-patch development manifest")
    parser.add_argument("--catalog", type=Path, default=DEFAULT_CATALOG)
    parser.add_argument("--out", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--zip", type=Path, default=DEFAULT_ZIP,
                        help="pinned archive zip (selection only; read-only)")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST,
                        help="manifest to validate (--validate mode)")
    parser.add_argument("--validate", action="store_true",
                        help="validate an existing manifest (no zip needed)")
    parser.add_argument("--progress", action="store_true")
    args = parser.parse_args(argv)
    try:
        if args.validate:
            catalog = load_catalog(args.catalog)
            if not args.manifest.is_file():
                raise NotRun(f"manifest not found: {args.manifest}")
            try:
                manifest = json.loads(
                    args.manifest.read_text(encoding="utf-8"))
            except json.JSONDecodeError as exc:
                raise Fail(f"manifest is not valid JSON: {exc}")
            problems = validate_manifest(manifest, catalog,
                                         sha256_file(args.catalog))
            if problems:
                for p in problems:
                    print(f"dev32 FAIL: {p}", file=sys.stderr)
                return 1
            cov = manifest["coverage"]
            print(f"dev32 validate PASS: {manifest['counts']['entries']}"
                  f" entries, algorithms"
                  f" {len(cov['algorithms_covered'])}/32, pitch_eg="
                  f"{cov['pitch_eg_count']}, fixed={cov['fixed_mode_count']},"
                  f" lfo_waves={cov['lfo_waves_covered']}, velocity="
                  f"{cov['velocity_sensitive_count']}, profiles="
                  f"{cov['render_profiles']}")
            return 0
        manifest = build_manifest(args.catalog, args.zip,
                                  progress=args.progress)
        args.out.parent.mkdir(parents=True, exist_ok=True)
        with args.out.open("w", encoding="utf-8") as fh:
            json.dump(manifest, fh, indent=2, ensure_ascii=False)
            fh.write("\n")
        cov = manifest["coverage"]
        print(f"dev32 select PASS: wrote {args.out} "
              f"({manifest['counts']['entries']} entries), algorithms "
              f"{len(cov['algorithms_covered'])}/32, goal_holes="
              f"{cov['goal_holes']}, algorithm_holes="
              f"{cov['algorithms_missing']}")
        return 0
    except Fail as exc:
        print(f"dev32 FAIL: {exc}", file=sys.stderr)
        return 1
    except NotRun as exc:
        print(f"dev32 NOT_RUN: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
