#!/usr/bin/env python3
"""U02 deterministic candidate pre-selection + one-sitting kit (issue #35).

Turns the U02 human-listening gate (one reviewed listening bank) into ONE
sitting for the owner: pick ~3 candidates per plan-section-6 sound family
from the committed P02 archive catalog, render the fixed U01 phrase set for
each through the pinned external oracle, and emit a listening kit (blank
verdict columns; the human fills them in).

Subcommands:
  select     (default) run the deterministic selection; write
             corpus/u02-candidates/candidates.json and
             corpus/u02-candidates/LISTENING-KIT.md. Requires the pinned
             archive zip (decode-only) -- exits 2 naming it when absent.
  validate   zip-free re-check of the committed candidates.json against the
             committed catalog (resolution, exclusions, coverage) plus a
             byte-compare of the committed kit against a regeneration.
  kit        regenerate LISTENING-KIT.md from the committed candidates.json
             (deterministic; only content, no clock reads).
  render-kit for every candidate, run tools/audition.py render --phrase all
             into corpus/u02-candidates/renders/<hash12>-<name>/ (idempotent:
             complete render dirs are skipped). Verifies the pinned oracle
             identity BEFORE the batch; exits 2 naming the missing piece.

Selection is deterministic and documented -- no randomness, no taste:

  Universe: decoded, anomaly-free voice instances from the committed catalog
  (standalone voice entries + bank slot rows, anomaly_count == 0), same
  scope as docs/CORPUS-POLICY.md; stream-class instances are excluded for
  extraction simplicity (documented scope, not a coverage hole). Features
  are functions of the canonical parameters, so one decode per distinct
  canonical hash suffices. The zip is SHA-256-checked against the catalog
  archive pin before any decode.

  Exclusions: every canonical hash in corpus/dev32.json (already exercised
  by the development corpus) and every hash in corpus/favorites.json
  (already audited). A hash is shortlisted at most once across families.

  Families (plan section 6 / issue #35): EPs/tines, basses, bells/mallets,
  organs, brass/reeds, plucks, pads/strings, percussion/textures.

  Per family: hard constraints (a voice must satisfy all of them to be a
  strict candidate) then scored items (see FAMILY_SPECS below -- every
  threshold is printed in candidates.json under selection.heuristics so the
  shortlist rule travels with the data). Ranking: (-score, catalog sort key
  (zip path, standalone-first then slot, catalog order), canonical hash);
  the top 3 distinct hashes are shortlisted. If fewer than 3 voices pass
  the hard constraints, the remaining slots take the best-scoring voices
  that fail some hard constraint, marked "relaxed" with the constraint
  named -- recorded per family in coverage. A family with zero candidates
  of any kind is a hard coverage failure (exit 1).

  NO taste judgment is made anywhere in this tool: heuristics shortlist,
  humans decide (AGENTS.md: listening records are the only evidence for
  musical usefulness).

Kit rule: renders are DRY (the oracle records peaks; gain is never
altered, no per-candidate normalization, no time-warp). Identity is the
P02 canonical hash everywhere; names are display sugar only.

Exit codes: 0 PASS, 1 FAIL (drift, coverage hole, render failure),
2 NOT_RUN (environment: missing catalog/zip/oracle).

Stdlib-only. No upstream bytes: the external GPL oracle is driven from
outside the repository via tools/audition.py (AGENTS.md licensing policy).
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
import re
import subprocess
import sys
import zipfile
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.model.algorithm import ALGORITHMS  # noqa: E402

DEFAULT_CATALOG = REPO_ROOT / "corpus" / "archive-catalogs" / \
    "alltheweb-catalog.json"
DEFAULT_DEV32 = REPO_ROOT / "corpus" / "dev32.json"
DEFAULT_FAVORITES = REPO_ROOT / "corpus" / "favorites.json"
DEFAULT_OUT_DIR = REPO_ROOT / "corpus" / "u02-candidates"
CANDIDATES_SCHEMA = "gf180-dx7.u02-candidates/1"
PER_FAMILY = 3

ISSUE_URL = "https://github.com/2AMLogic/gf180-dx7/issues/35"
PLAN_REF = "docs/dx7-chip-plan-v0.1-2026-09-20.md section 6 (issue #35)"
TOOL = "tools/u02_candidates.py"

PITCH_EG_CENTER = 50

# Suffix used by tests to strip the only clock line from regenerated kits.
KIT_TIMESTAMP_PREFIX = "Generated: "


class Fail(Exception):
    """Exit 1: a check failed or coverage is broken."""


class NotRun(Exception):
    """Exit 2: environment problem; nothing was verified."""


# ---------------------------------------------------------------------------
# Derived voice facts (decode-only)


def live_ops(v: sysex.Voice) -> list[sysex.Operator]:
    return [op for op in v.operators if op.output_level > 0]


def live_carriers(v: sysex.Voice) -> list[tuple[int, sysex.Operator]]:
    """(1-based DX7 operator number, operator) for live operators whose
    algorithm descriptor writes the note-output bus (outbus 0). Descriptor
    index k is engine param k = voice.operators[5 - k] (model/algorithm.py
    NoteState.keydown: unpacked block k = op 6-k)."""
    out = []
    for k, flags in enumerate(ALGORITHMS[v.algorithm]):
        if flags & 0x03 == 0:
            op = v.operators[5 - k]
            if op.output_level > 0:
                out.append((6 - k, op))
    return out


def live_mods(v: sysex.Voice) -> list[sysex.Operator]:
    carrier_ids = {id(op) for _, op in live_carriers(v)}
    return [op for op in live_ops(v) if id(op) not in carrier_ids]


def velocity_sensitive(v: sysex.Voice) -> bool:
    return any(op.key_velocity_sensitivity > 0 for op in live_ops(v))


def pitch_eg_present(v: sysex.Voice) -> bool:
    return any(lvl != PITCH_EG_CENTER
               for lvl in (v.pitch_eg_level_1, v.pitch_eg_level_2,
                           v.pitch_eg_level_3, v.pitch_eg_level_4))


def inharmonic(v: sysex.Voice) -> bool:
    return any(op.osc_freq_coarse != 1 or op.osc_freq_fine >= 10 or
               op.osc_detune != 7 for op in live_ops(v))


# ---------------------------------------------------------------------------
# Family specifications: hard constraints + scored items.
# Each predicate receives (v, ctx) with ctx = precomputed derived facts.
# Strings are templates formatted with the same ctx dict (+ numbers), so the
# JSON heuristic block, the why-shortlist reasons and this file cannot drift.


@dataclass(frozen=True)
class FamilySpec:
    key: str
    label: str            # issue #35 family label
    listen: str           # what the human should listen for (kit text)
    hard: tuple[tuple[str, object], ...]
    items: tuple[tuple[str, int, object], ...]


def _ctx(v: sysex.Voice) -> dict:
    carriers = live_carriers(v)
    mods = live_mods(v)
    loudest = max(carriers, key=lambda t: t[1].output_level)[1] if carriers \
        else None
    second = sorted((op.output_level for _, op in carriers), reverse=True)
    return {
        "v": v,
        "carriers": carriers,
        "mods": mods,
        "n_carriers": len(carriers),
        "loudest": loudest,
        "second_gap": (second[0] - second[1]) if len(second) >= 2 else None,
        "carrier_mass": sum(op.output_level for _, op in carriers),
        "mod_mass": sum(op.output_level for op in mods),
        "max_mod": max((op.output_level for op in mods), default=0),
        "min_carrier_r1": min((op.rate_1 for _, op in carriers), default=99),
        "max_carrier_r1": max((op.rate_1 for _, op in carriers), default=0),
        "min_carrier_r4": min((op.rate_4 for _, op in carriers), default=99),
        "max_carrier_r4": max((op.rate_4 for _, op in carriers), default=0),
        "min_carrier_l4": min((op.level_4 for _, op in carriers), default=99),
        "vs": velocity_sensitive(v),
        "peg": pitch_eg_present(v),
        "inharm": inharmonic(v),
        "fixed_live": sum(1 for op in live_ops(v) if op.osc_mode == 1),
        "unison": len({(op.osc_freq_coarse, op.osc_freq_fine, op.osc_detune)
                       for _, op in carriers}) < len(carriers)
        if carriers else False,
        "detune_width": len({op.osc_detune for _, op in carriers}) >= 2
        if carriers else False,
        "flat_sustain": all(op.level_3 == op.level_4 for _, op in carriers)
        if carriers else False,
    }


def _fmt(template: str, ctx: dict) -> str:
    v = ctx["v"]
    loudest = ctx["loudest"]
    return template.format(
        alg=v.algorithm + 1, fb=v.feedback,
        n=ctx["n_carriers"],
        l1=loudest.level_1 if loudest else 0,
        l4=loudest.level_4 if loudest else 0,
        r1=loudest.rate_1 if loudest else 0,
        mm=ctx["mod_mass"], cm=ctx["carrier_mass"],
        maxmod=ctx["max_mod"],
        mr1=ctx["min_carrier_r1"], xr1=ctx["max_carrier_r1"],
        mr4=ctx["min_carrier_r4"], xr4=ctx["max_carrier_r4"],
        ml4=ctx["min_carrier_l4"],
        fixed=ctx["fixed_live"],
        lfo_pm=v.lfo_pitch_mod_depth,
    )


FAMILY_SPECS: tuple[FamilySpec, ...] = (
    FamilySpec(
        key="ep-tine", label="EPs/tines",
        listen="bell-like tine attack over a rounded body; strike transient "
               "vs sustain balance; velocity response should open the "
               "tine, not just get louder",
        hard=(
            ("exactly two live carriers", lambda c: c["n_carriers"] == 2),
            ("feedback within 0-5", lambda c: c["v"].feedback <= 5),
            ("velocity-sensitive", lambda c: c["vs"]),
        ),
        items=(
            ("exactly two live carriers (algorithm {alg})", 2,
             lambda c: c["n_carriers"] == 2),
            ("moderate feedback {fb}", 1,
             lambda c: 1 <= c["v"].feedback <= 4),
            ("decaying sustain (loudest carrier {l1} -> {l4})", 1,
             lambda c: c["loudest"] is not None and
             c["loudest"].level_1 - c["loudest"].level_4 >= 20),
            ("medium attack (loudest carrier rate1 {r1})", 1,
             lambda c: c["loudest"] is not None and
             40 <= c["loudest"].rate_1 <= 85),
            ("velocity-sensitive carrier", 1, lambda c: c["vs"]),
            ("tine-like release (loudest carrier rate4 <= 60)", 1,
             lambda c: c["loudest"] is not None and
             c["loudest"].rate_4 <= 60),
        ),
    ),
    FamilySpec(
        key="bass", label="basses",
        listen="fundamental weight at MIDI 36; attack definition; whether "
               "the low register stays tuned and non-muddy",
        hard=(
            ("fast attack (every live carrier rate1 >= 55)",
             lambda c: c["min_carrier_r1"] >= 55),
            ("single dominant carrier (loudest >= 60, gap >= 20)",
             lambda c: c["loudest"] is not None and
             c["loudest"].output_level >= 60 and
             (c["second_gap"] is None or c["second_gap"] >= 20)),
            ("feedback within 0-6", lambda c: c["v"].feedback <= 6),
        ),
        items=(
            ("single dominant carrier (loudest level >= 60)", 2,
             lambda c: c["loudest"] is not None and
             c["loudest"].output_level >= 60 and
             (c["second_gap"] is None or c["second_gap"] >= 20)),
            ("fast attack (all live carriers rate1 >= 60)", 1,
             lambda c: c["min_carrier_r1"] >= 60),
            ("low output tilt: modulator mass {mm} >= carrier mass {cm}", 1,
             lambda c: c["mod_mass"] >= c["carrier_mass"]),
            ("decayed sustain (loudest carrier level4 {l4} <= 45)", 1,
             lambda c: c["loudest"] is not None and
             c["loudest"].level_4 <= 45),
            ("tight growl (feedback {fb} <= 3)", 1,
             lambda c: c["v"].feedback <= 3),
        ),
    ),
    FamilySpec(
        key="bell-mallet", label="bells/mallets",
        listen="strike transient, inharmonic shimmer, how long the decay "
               "rings at high register; release tail behaviour",
        hard=(
            ("long release (min live-carrier rate4 <= 45)",
             lambda c: c["min_carrier_r4"] <= 45),
            ("decays to near-silence (min live-carrier level4 <= 30)",
             lambda c: c["min_carrier_l4"] <= 30),
            ("high modulation index (max modulator level >= 70)",
             lambda c: c["max_mod"] >= 70),
        ),
        items=(
            ("high modulation index (max modulator level {maxmod})", 2,
             lambda c: c["max_mod"] >= 70),
            ("inharmonic partials (non-unison ratio/detune)", 1,
             lambda c: c["inharm"]),
            ("struck attack (all live carriers rate1 >= 60)", 1,
             lambda c: c["min_carrier_r1"] >= 60),
            ("long decay to silence (min carrier level4 {ml4} <= 15)", 1,
             lambda c: c["min_carrier_l4"] <= 15),
            ("long release tail (min carrier rate4 {mr4} <= 35)", 1,
             lambda c: c["min_carrier_r4"] <= 35),
        ),
    ),
    FamilySpec(
        key="organ", label="organs",
        listen="steady sustain without wobble; drawbar-style timbre steps "
               "across registers; attack should be immediate",
        hard=(
            ("sustained carriers (min live-carrier level4 >= 65)",
             lambda c: c["min_carrier_l4"] >= 65),
            ("immediate attack (max live-carrier rate1 >= 60)",
             lambda c: c["max_carrier_r1"] >= 60),
            ("low feedback (<= 2)", lambda c: c["v"].feedback <= 2),
        ),
        items=(
            ("sustained carriers (min level4 {ml4})", 2,
             lambda c: c["min_carrier_l4"] >= 65),
            ("drawbar stack ({n} live carriers)", 1,
             lambda c: c["n_carriers"] >= 3),
            ("same-pitch carriers (unison drawbars)", 1,
             lambda c: c["unison"]),
            ("no decay sag (level3 == level4 on all carriers)", 1,
             lambda c: c["flat_sustain"]),
            ("clean tone (feedback {fb} <= 1)", 1,
             lambda c: c["v"].feedback <= 1),
        ),
    ),
    FamilySpec(
        key="brass-reed", label="brass/reeds",
        listen="breath/reed grit at the attack, sustain stability under "
               "mod-wheel, velocity dynamics",
        hard=(
            ("sustained carriers (min live-carrier level4 >= 50)",
             lambda c: c["min_carrier_l4"] >= 50),
            ("medium attack (max live-carrier rate1 35-90)",
             lambda c: 35 <= c["max_carrier_r1"] <= 90),
            ("some feedback (>= 1)", lambda c: c["v"].feedback >= 1),
        ),
        items=(
            ("sustained carriers (min level4 {ml4})", 1,
             lambda c: c["min_carrier_l4"] >= 50),
            ("1-2 carrier voicing ({n})", 1,
             lambda c: 1 <= c["n_carriers"] <= 2),
            ("feedback growl ({fb})", 1, lambda c: 2 <= c["v"].feedback <= 5),
            ("medium attack (max carrier rate1 {xr1})", 1,
             lambda c: 40 <= c["max_carrier_r1"] <= 75),
            ("velocity dynamics present", 1, lambda c: c["vs"]),
            ("deep modulation (max modulator level {maxmod} >= 60)", 1,
             lambda c: c["max_mod"] >= 60),
        ),
    ),
    FamilySpec(
        key="pluck", label="plucks",
        listen="attack snap, decay speed, whether the pluck repeats "
               "cleanly at low velocities",
        hard=(
            ("fast attack (every live carrier rate1 >= 65)",
             lambda c: c["min_carrier_r1"] >= 65),
            ("percussive decay (min live-carrier level4 <= 35)",
             lambda c: c["min_carrier_l4"] <= 35),
        ),
        items=(
            ("percussive attack (all live carriers rate1 >= 70)", 1,
             lambda c: c["min_carrier_r1"] >= 70),
            ("steep decay (loudest carrier {l1} -> {l4})", 1,
             lambda c: c["loudest"] is not None and
             c["loudest"].level_1 - c["loudest"].level_4 >= 45),
            ("high modulation index (max modulator level {maxmod} >= 70)", 1,
             lambda c: c["max_mod"] >= 70),
            ("velocity dynamics present", 1, lambda c: c["vs"]),
            ("ring-out release (max carrier rate4 {xr4} <= 60)", 1,
             lambda c: c["max_carrier_r4"] <= 60),
        ),
    ),
    FamilySpec(
        key="pad-string", label="pads/strings",
        listen="slow attack bloom, sustain evenness, detune width/ beating; "
               "watch for phasey or muddy chords",
        hard=(
            ("slow attack (max live-carrier rate1 <= 50)",
             lambda c: c["max_carrier_r1"] <= 50),
            ("sustained carriers (min live-carrier level4 >= 55)",
             lambda c: c["min_carrier_l4"] >= 55),
        ),
        items=(
            ("slow attack (max carrier rate1 {xr1})", 2,
             lambda c: c["max_carrier_r1"] <= 40),
            ("sustained carriers (min level4 {ml4} >= 55)", 1,
             lambda c: c["min_carrier_l4"] >= 55),
            ("detuned carriers (width/beating)", 1,
             lambda c: c["detune_width"]),
            ("wide voicing ({n} live carriers)", 1,
             lambda c: c["n_carriers"] >= 3),
            ("vibrato wired (LFO pitch-mod depth {lfo_pm} >= 15)", 1,
             lambda c: c["v"].lfo_pitch_mod_depth >= 15),
        ),
    ),
    FamilySpec(
        key="perc-texture", label="percussion/textures",
        listen="fixed-frequency sweeps/noise-like behaviour, attack "
               "transients, whether textures evolve under mod-wheel/bend",
        hard=(
            ("fixed-frequency operators (>= 2) or extreme feedback (>= 6)",
             lambda c: c["fixed_live"] >= 2 or c["v"].feedback >= 6),
        ),
        items=(
            ("fixed-frequency operators ({fixed})", 2,
             lambda c: c["fixed_live"] >= 2),
            ("one fixed-frequency operator", 1,
             lambda c: c["fixed_live"] == 1),
            ("extreme feedback ({fb})", 2, lambda c: c["v"].feedback >= 6),
            ("high modulation index (max modulator level {maxmod} >= 85)", 1,
             lambda c: c["max_mod"] >= 85),
            ("fast strike (all live carriers rate1 >= 60)", 1,
             lambda c: c["min_carrier_r1"] >= 60),
            ("pitch-EG sweep present", 1, lambda c: c["peg"]),
            ("inharmonic partials (non-unison ratio/detune)", 1,
             lambda c: c["inharm"]),
        ),
    ),
)

FAMILY_ORDER = tuple(f.key for f in FAMILY_SPECS)
FAMILY_BY_KEY = {f.key: f for f in FAMILY_SPECS}


def family_score(spec: FamilySpec, v: sysex.Voice) -> tuple[int, list[str],
                                                            bool]:
    """(-free) score, matched-reason strings, hard_ok for one voice."""
    ctx = _ctx(v)
    hard_ok = all(pred(ctx) for _, pred in spec.hard)
    matched = []
    score = 0
    for template, points, pred in spec.items:
        if pred(ctx):
            score += points
            matched.append(_fmt(template, ctx))
    return score, matched, hard_ok


# ---------------------------------------------------------------------------
# Catalog / zip plumbing (same conventions as tools/select_dev32.py)


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


@dataclass(frozen=True)
class Instance:
    path: str
    slot: int | None  # None for a standalone single-voice file
    canonical_hash: str
    name: str
    order: int

    @property
    def sort_key(self) -> tuple[str, int, int]:
        return (self.path, -1 if self.slot is None else self.slot, self.order)


def load_json(path: Path) -> dict:
    if not path.is_file():
        raise NotRun(f"required input not found: {path}")
    try:
        with path.open("r", encoding="utf-8") as fh:
            return json.load(fh)
    except (OSError, json.JSONDecodeError) as exc:
        raise Fail(f"{path} unreadable: {exc}")


def clean_instances(cat: dict) -> list[Instance]:
    """Decoded, anomaly-free voice instances (standalone + bank slots)."""
    hashes, names = cat["hashes"], cat["names"]
    out: list[Instance] = []
    for order, e in enumerate(cat["entries"]):
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


def decode_voices(cat: dict, zip_path: Path,
                  progress: bool = False) -> dict[str, sysex.Voice]:
    """Decode one Voice per distinct canonical hash (features are functions
    of canonical parameters). The zip is pin-checked before any read."""
    archive_sha = cat["archive"]["sha256"]
    if not zip_path.is_file():
        raise NotRun(
            f"pinned archive zip not found at {zip_path} (catalog pin sha256"
            f" {archive_sha}); candidate selection must decode voice "
            f"parameters from the archive -- pass --zip PATH (the zip is "
            f"local-only and is not redistributed by this repository)")
    actual = sha256_file(zip_path)
    if actual != archive_sha:
        raise Fail(f"archive drift: {zip_path} sha256 {actual} != catalog "
                   f"pin {archive_sha}; refusing to select from a different "
                   f"archive")
    hashes = cat["hashes"]
    pending: dict[str, list[tuple[str, int | None]]] = {}
    for e in cat["entries"]:
        if e["c"] == "voice" and e["v"] == "decoded" and e["a"] == 0 \
                and e["h"] is not None:
            pending.setdefault(e["p"], []).append((e["h"], None))
        elif e["c"] == "bank" and e["v"] == "decoded":
            w = e["w"]
            rows = [(hashes[w[k + 1]], w[k]) for k in range(0, len(w), 4)
                    if w[k + 1] >= 0 and w[k + 3] == 0]
            pending.setdefault(e["p"], []).extend(rows)
    voices: dict[str, sysex.Voice] = {}
    with zipfile.ZipFile(zip_path) as zf:
        for i, path in enumerate(sorted(pending)):
            rows = [(h, slot) for h, slot in pending[path] if h not in voices]
            if not rows:
                continue
            try:
                data = zf.read(path)
                if rows[0][1] is None:
                    voices[rows[0][0]] = sysex.decode_voice(data)
                else:
                    bank = sysex.decode_bank(data)
                    for h, slot in rows:
                        if h not in voices:
                            voices[h] = bank.voices[slot]
            except sysex.SysexError as exc:
                raise Fail(f"catalog says {path!r} decodes clean but the "
                           f"codec rejected it (catalog/archive drift): "
                           f"{exc}")
            if progress and (i + 1) % 1000 == 0:
                print(f"[u02] {i + 1}/{len(pending)} files, "
                      f"{len(voices)} hashes decoded", file=sys.stderr,
                      flush=True)
    return voices


# ---------------------------------------------------------------------------
# Selection


def _excluded_hashes(dev32_path: Path, favorites_path: Path) -> dict[str, set]:
    dev32 = load_json(dev32_path)
    favs = load_json(favorites_path)
    dev_hashes = {e["canonical_hash"] for e in dev32.get("entries", [])}
    fav_hashes = set((favs.get("favorites") or {}).keys())
    return {"dev32": dev_hashes, "favorites": fav_hashes}


def best_instance_per_hash(instances: list[Instance]) -> dict[str, Instance]:
    best: dict[str, Instance] = {}
    for inst in instances:  # already sorted by sort_key
        best.setdefault(inst.canonical_hash, inst)
    return best


def select_candidates(cat: dict, zip_path: Path,
                      progress: bool = False) -> dict:
    instances = clean_instances(cat)
    if progress:
        print(f"[u02] clean candidate instances: {len(instances)}",
              file=sys.stderr, flush=True)
    voices = decode_voices(cat, zip_path, progress=progress)
    excluded = _excluded_hashes(DEFAULT_DEV32, DEFAULT_FAVORITES)
    best_inst = best_instance_per_hash(instances)

    pool = {h: inst for h, inst in best_inst.items()
            if h in voices and h not in excluded["dev32"]
            and h not in excluded["favorites"]}

    candidates: list[dict] = []
    used: set[str] = set()
    coverage_families: dict[str, dict] = {}
    for spec in FAMILY_SPECS:
        scored: list[tuple[int, str, str, list[str], bool]] = []
        for h, inst in pool.items():
            if h in used:
                continue
            score, matched, hard_ok = family_score(spec, voices[h])
            scored.append((-score, inst.sort_key, h, matched, hard_ok))
        strict = sorted(s for s in scored if s[4])
        relaxed = sorted(s for s in scored if not s[4])
        picked = strict[:PER_FAMILY]
        relaxed_used = max(0, PER_FAMILY - len(picked))
        picked += relaxed[:relaxed_used]
        for negscore, sort_key, h, matched, hard_ok in picked:
            used.add(h)
            inst = pool[h]
            candidates.append({
                "family": spec.key,
                "canonical_hash": h,
                "name": inst.name,
                "catalog": {"path": inst.path, "slot": inst.slot},
                "heuristics": {
                    "score": -negscore,
                    "matched": matched,
                    "hard_constraints_met": hard_ok,
                    "rank_sort_key": list(sort_key),
                },
            })
        coverage_families[spec.key] = {
            "label": spec.label,
            "selected": len(picked),
            "strict_pool": len(strict),
            "relaxed": relaxed_used > 0,
            "hole": len(picked) == 0,
        }
        if progress:
            print(f"[u02] family {spec.key}: picked {len(picked)} "
                  f"(strict pool {len(strict)})", file=sys.stderr, flush=True)

    holes = [k for k, c in coverage_families.items() if c["hole"]]
    if holes:
        raise Fail("family coverage holes (no candidate at all): "
                   + ", ".join(holes))

    candidates.sort(key=lambda c: (FAMILY_ORDER.index(c["family"]),
                                   c["catalog"]["path"],
                                   -1 if c["catalog"]["slot"] is None
                                   else c["catalog"]["slot"],
                                   c["canonical_hash"]))
    for idx, cand in enumerate(candidates):
        cand["index"] = idx
        cand["voice_summary"] = _voice_summary(voices[cand["canonical_hash"]])
        cand["render_dir"] = render_dir_for(cand)
        # field order for the committed file
        ordered = {k: cand[k] for k in (
            "index", "family", "canonical_hash", "name", "catalog",
            "heuristics", "voice_summary", "render_dir")}
        cand.clear()
        cand.update(ordered)

    return {
        "schema": CANDIDATES_SCHEMA,
        "issue": ISSUE_URL,
        "plan_reference": PLAN_REF,
        "generated_utc": datetime.now(timezone.utc).strftime(
            "%Y-%m-%dT%H:%M:%SZ"),
        "counts": {
            "candidates": len(candidates),
            "families": len(FAMILY_SPECS),
            "per_family": PER_FAMILY,
        },
        "selection": {
            "tool": TOOL,
            "tool_sha256": sha256_file(Path(__file__).resolve()),
            "catalog": {
                "path": str(DEFAULT_CATALOG.relative_to(REPO_ROOT)),
                "schema": cat["schema"],
                "snapshot_sha256": sha256_file(DEFAULT_CATALOG),
            },
            "archive": dict(cat["archive"]),
            "exclusions": {
                "dev32": {
                    "path": str(DEFAULT_DEV32.relative_to(REPO_ROOT)),
                    "hashes": len(excluded["dev32"]),
                    "reason": "already exercised by the development corpus",
                },
                "favorites": {
                    "path": str(DEFAULT_FAVORITES.relative_to(REPO_ROOT)),
                    "hashes": len(excluded["favorites"]),
                    "reason": "already audited favorites",
                },
            },
            "candidate_universe": {
                "clean_instances": len(instances),
                "distinct_clean_hashes": len(best_inst),
                "decoded_hashes": len(voices),
                "eligible_after_exclusions": len(pool),
                "scope": "standalone voice entries + bank slot rows with "
                         "anomaly_count == 0; stream-class instances "
                         "excluded (documented scope, as in R03)",
            },
            "identity_rule": "selection and rendering are keyed by the P02 "
                             "canonical hash; names are display sugar only",
            "heuristics": {f.key: {
                "hard": [t for t, _ in f.hard],
                "scored": [{"desc": t, "points": p} for t, p, _ in f.items],
            } for f in FAMILY_SPECS},
            "ranking": "per family: hard-constraint pass first, then "
                       "(-score, catalog sort key (zip path, standalone-"
                       "first then slot, catalog order), canonical hash); "
                       "top {n} distinct hashes; shortfalls relax the hard "
                       "constraints and are marked".format(n=PER_FAMILY),
            "no_taste": "heuristics shortlist, humans decide; this tool "
                        "records zero listening judgments",
        },
        "coverage": {
            "families": coverage_families,
            "families_missing": holes,
        },
        "candidates": candidates,
    }


def _voice_summary(v: sysex.Voice) -> dict:
    return {
        "algorithm": v.algorithm,
        "algorithm_1based": v.algorithm + 1,
        "feedback": v.feedback,
        "carriers_1based": [num for num, _ in live_carriers(v)],
        "carrier_output_levels": [op.output_level for _,
                                  op in live_carriers(v)],
        "max_modulator_level": max((op.output_level for op in live_mods(v)),
                                   default=0),
        "lfo_wave": v.lfo_wave,
        "lfo_pitch_mod_depth": v.lfo_pitch_mod_depth,
        "pitch_eg_present": pitch_eg_present(v),
        "fixed_mode_operators": sum(1 for op in live_ops(v)
                                    if op.osc_mode == 1),
        "velocity_sensitive": velocity_sensitive(v),
    }


# ---------------------------------------------------------------------------
# Kit emission (deterministic: pure function of the candidates dict)


def slugify(name: str) -> str:
    slug = re.sub(r"[^A-Za-z0-9_-]+", "-", name.strip()).strip("-")
    return slug[:24] if slug else "unnamed"


def render_dir_for(cand: dict) -> str:
    return "corpus/u02-candidates/renders/" + \
        "{0}-{1}".format(cand["canonical_hash"][:12],
                         slugify(cand["name"]))


def emit_kit(cand: dict) -> str:
    """Deterministic LISTENING-KIT.md; a pure function of `cand`."""
    sel = cand["selection"]
    cov = cand["coverage"]
    lines: list[str] = []
    w = lines.append
    w("# U02 listening kit — one sitting (issue #35)")
    w("")
    w(f"{KIT_TIMESTAMP_PREFIX}{cand['generated_utc']}  ")
    w(f"Manifest: `corpus/u02-candidates/candidates.json` "
      f"({cand['schema']}, {cand['counts']['candidates']} candidates = "
      f"{cand['counts']['families']} families x "
      f"{cand['counts']['per_family']})  ")
    w(f"Catalog snapshot: `{sel['catalog']['path']}` sha256 "
      f"`{sel['catalog']['snapshot_sha256'][:16]}…`  ")
    w(f"Archive pin: `{sel['archive']['filename']}` sha256 "
      f"`{sel['archive']['sha256'][:16]}…` (local-only; not redistributed)")
    w("")
    w("## What this kit is (and is not)")
    w("")
    w("- Candidates across the plan-section-6 families were shortlisted by "
      "documented deterministic heuristics over decoded parameters "
      "(`candidates.json` → `selection.heuristics`). **Heuristics select; "
      "humans decide.** No taste judgment was made by the tool or its "
      "author — your listening records are the point of this kit.")
    w("- Excluded: the dev32 development patches "
      f"({sel['exclusions']['dev32']['hashes']} hashes) and already-audited "
      f"favorites ({sel['exclusions']['favorites']['hashes']} hash(es)).")
    w("- Identity is the P02 canonical hash everywhere; names are display "
      "sugar. Per-candidate provenance (archive path / bank slot / hash) is "
      "in `candidates.json`.")
    w("- All renders are **dry** oracle renders (pinned Dexed Mark I binary, "
      "hash verified): peaks are recorded, never altered. No normalization, "
      "no time-warp, no engine switching per patch.")
    w("- Oracle renders prove selection and reproduction only. Dexed "
      "agreement is not original-DX7 fidelity, and nothing here establishes "
      "that a patch sounds good — that is exactly what this sitting "
      "produces.")
    w("")
    w("## One-sitting protocol (~45–60 min)")
    w("")
    w("0. **Generate the audio** (idempotent, a few minutes — 288 oracle "
      "renders):")
    w("   ```sh")
    w("   python3 tools/u02_candidates.py render-kit")
    w("   ```")
    w("   The pinned oracle identity is verified before the batch; missing "
      "zip/oracle exits 2 naming the file.")
    w("1. **Listen family by family, candidates in table order.** For each "
      "candidate play the 12 phrase renders in this order:")
    w("   `mid-60-vel100` (anchor) → `low-36-vel100` → `high-84-vel100` → "
      "`mid-60-vel32` / `mid-60-vel80` / `mid-60-vel127` (velocity "
      "response) → `short-pluck-60` / `long-hold-60` / `release-tail-60` "
      "(envelope) → `chord-3note-60-64-67` → `modwheel-sweep-60` → "
      "`pitch-bend-60`.")
    w("   Each render dir has one subdirectory per phrase; listen to "
      "`render.wav`.")
    w("2. **What to listen for, per family:**")
    for spec in FAMILY_SPECS:
        w(f"   - **{spec.label}** — {spec.listen}.")
    w("3. **Record a verdict per candidate in the table below** "
      "(`character` / `velocity response` / `keyboard range` / `verdict` / "
      "`notes`). U02 acceptance requires every entry to carry listening "
      "notes from the phrase set and a verdict — **selected** or "
      "**rejected with reason**. Candidate count is 24; U02's bank is 32 "
      "patches, so also note near-misses worth pulling from the catalog "
      "later.")
    w("4. **Technical failures are compatibility bugs, not taste.** Stuck "
      "notes, wrong envelope behavior, silence where a note should sound — "
      "file an issue against the owning E1/E2 issue and reference it in "
      "`notes`; never absorb it into “don't like it”.")
    w("5. **Promote keepers as you go** (records the favorite + per-phrase "
      "hashes for later recall):")
    w("   ```sh")
    w("   python3 tools/audition.py favorite --hash <64-hex> --note \"...\"")
    w("   python3 tools/audition.py recall   --hash <64-hex>   # later: "
      "must reproduce byte-identically")
    w("   ```")
    w("6. **Transfer the completed table** into the bank manifest + "
      "listening records and close #35 with the evidence links. This kit "
      "does **not** close #35 — the listening itself is the human gate; "
      "this kit only makes it turnkey.")
    w("")
    w("## Candidate table")
    w("")
    w("| # | family | name | hash | why shortlisted | renders | character "
      "| velocity response | keyboard range | verdict | notes |")
    w("|---|--------|------|------|-----------------|---------|-----------|"
      "-------------------|---------------|---------|-------|")
    for c in cand["candidates"]:
        why = "; ".join(c["heuristics"]["matched"][:4])
        if not c["heuristics"]["hard_constraints_met"]:
            why += " *(relaxed constraints)*"
        w(f"| {c['index']} | {FAMILY_BY_KEY[c['family']].label} "
          f"| {c['name'].strip()} | `{c['canonical_hash'][:12]}` | {why} "
          f"| `{c['render_dir']}/` |  |  |  |  |  |")
    w("")
    w("## Coverage")
    w("")
    w("| family | selected | strict pool | relaxed |")
    w("|--------|----------|-------------|---------|")
    for key in FAMILY_ORDER:
        c = cov["families"][key]
        w(f"| {c['label']} | {c['selected']} | {c['strict_pool']} | "
          f"{'yes' if c['relaxed'] else 'no'} |")
    w("")
    w("## Provenance")
    w("")
    w(f"- Selection tool: `{sel['tool']}` (sha256 "
      f"`{sel['tool_sha256'][:16]}…`), deterministic, no randomness.")
    w(f"- Catalog: `{sel['catalog']['path']}` schema {sel['catalog']['schema']}"
      f", snapshot sha256 `{sel['catalog']['snapshot_sha256'][:16]}…`; "
      f"universe {sel['candidate_universe']['clean_instances']} clean "
      f"instances / {sel['candidate_universe']['distinct_clean_hashes']} "
      f"distinct hashes, {sel['candidate_universe']['eligible_after_exclusions']}"
      " eligible after exclusions.")
    w(f"- Archive: `{sel['archive']['filename']}` sha256 "
      f"`{sel['archive']['sha256'][:16]}…` — voice bytes are never "
      "redistributed; renders are derived audio of archived patches "
      "(catalog rights statement applies).")
    w("- Phrase set: `spec/audition-phrases-v1.json` (U01, fixed; the tool "
      "refuses improvised phrase sets).")
    w("- Oracle: pinned in `reference/oracle-identity.json`; every render "
      "re-verifies the binary hash. External GPL oracle only — never copied "
      "into this repository.")
    w("- **Dry-render reminder:** renders are dry; peaks recorded, gain "
      "untouched. Compare candidates without normalization tricks.")
    w("")
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# validate (zip-free) + kit regeneration


def _strip_timestamps(text: str) -> str:
    """Remove the only clock-derived lines (kit header + JSON field) so a
    regenerated artifact byte-compares against the committed one."""
    json_re = re.compile(r'^\s*"generated_utc":\s*"\d{4}-\d{2}-\d{2}'
                         r'T\d{2}:\d{2}:\d{2}Z",?\s*$')
    kept = [line for line in text.splitlines()
            if not line.startswith(KIT_TIMESTAMP_PREFIX)
            and not json_re.match(line)]
    return "\n".join(kept) + "\n"


def resolution_index(cat: dict) -> dict[tuple[str, int | None], tuple]:
    """(path, slot) -> (canonical_hash, name) for decoded clean instances."""
    hashes, names = cat["hashes"], cat["names"]
    idx: dict[tuple[str, int | None], tuple] = {}
    for e in cat["entries"]:
        if e["c"] == "voice" and e["v"] == "decoded" and e["a"] == 0 \
                and e["h"] is not None:
            idx[(e["p"], None)] = (e["h"], e["n"])
        elif e["c"] == "bank" and e["v"] == "decoded":
            w = e["w"]
            for k in range(0, len(w), 4):
                slot, hidx, nidx, nanom = w[k:k + 4]
                if nanom == 0 and hidx >= 0:
                    idx[(e["p"], slot)] = (hashes[hidx], names[nidx])
    return idx


def validate_candidates(cand: dict, cat: dict,
                        catalog_sha: str) -> list[str]:
    """Zip-free validation of a candidates.json against the committed
    catalog. Returns a list of problems (empty = PASS)."""
    problems: list[str] = []
    if cand.get("schema") != CANDIDATES_SCHEMA:
        problems.append(f"schema {cand.get('schema')!r} != "
                        f"{CANDIDATES_SCHEMA!r}")
        return problems
    cands = cand.get("candidates")
    if not isinstance(cands, list) or not cands:
        problems.append("candidates missing or empty")
        return problems
    excluded = _excluded_hashes(DEFAULT_DEV32, DEFAULT_FAVORITES)
    res = resolution_index(cat)
    seen_hashes: set[str] = set()
    per_family: dict[str, int] = {}
    for c in cands:
        i = c.get("index")
        h = c.get("canonical_hash")
        fam = c.get("family")
        if fam not in FAMILY_BY_KEY:
            problems.append(f"candidate {i}: unknown family {fam!r}")
            continue
        per_family[fam] = per_family.get(fam, 0) + 1
        if not isinstance(h, str) or not re.fullmatch(r"[0-9a-f]{64}", h):
            problems.append(f"candidate {i}: canonical_hash malformed")
            continue
        if h in seen_hashes:
            problems.append(f"candidate {i}: duplicate hash {h[:12]}…")
        seen_hashes.add(h)
        if h in excluded["dev32"]:
            problems.append(f"candidate {i}: hash {h[:12]}… is a dev32 "
                            f"patch (must be excluded)")
        if h in excluded["favorites"]:
            problems.append(f"candidate {i}: hash {h[:12]}… is an already-"
                            f"audited favorite (must be excluded)")
        cat_entry = c.get("catalog") or {}
        key = (cat_entry.get("path"), cat_entry.get("slot"))
        if key not in res:
            problems.append(f"candidate {i}: catalog instance {key!r} does "
                            f"not resolve in the committed catalog")
        elif res[key][0] != h:
            problems.append(f"candidate {i}: catalog hash at {key!r} is "
                            f"{res[key][0][:12]}… != candidate {h[:12]}…")
        elif not c.get("name"):
            problems.append(f"candidate {i}: name missing")
        heur = c.get("heuristics") or {}
        if not isinstance(heur.get("matched"), list) or \
                not heur["matched"]:
            problems.append(f"candidate {i}: why-shortlist reasons missing")
        if "score" not in heur:
            problems.append(f"candidate {i}: heuristic score missing")
        if not isinstance(c.get("voice_summary"), dict):
            problems.append(f"candidate {i}: voice_summary missing")
        rd = c.get("render_dir")
        if rd != render_dir_for(c):
            problems.append(f"candidate {i}: render_dir {rd!r} != expected")
    # coverage: all families present, counts consistent
    missing = [k for k in FAMILY_ORDER if per_family.get(k, 0) == 0]
    if missing:
        problems.append("families with no candidates: " + ", ".join(missing))
    cov_fams = (cand.get("coverage") or {}).get("families") or {}
    recomputed = {
        "candidates": len(cands),
        "families": len(cov_fams) if cov_fams else len(FAMILY_ORDER),
    }
    recorded = cand.get("counts") or {}
    for key, want in recomputed.items():
        if recorded.get(key) != want:
            problems.append(f"counts.{key} {recorded.get(key)!r} != "
                            f"recomputed {want!r}")
    if sel := cand.get("selection"):
        if sel.get("catalog", {}).get("snapshot_sha256") != catalog_sha:
            problems.append("catalog drifted since selection (snapshot "
                            "mismatch); candidates.json is STALE")
    else:
        problems.append("selection provenance block missing")
    return problems


def check_kit_matches(cand: dict, kit_path: Path) -> str | None:
    """Regenerate the kit and byte-compare (minus the timestamp line).
    Returns None on match, else a problem string."""
    if not kit_path.is_file():
        return f"kit file missing: {kit_path}"
    committed = kit_path.read_text(encoding="utf-8")
    regenerated = emit_kit(cand)
    if _strip_timestamps(committed) != _strip_timestamps(regenerated):
        return (f"kit file {kit_path} does not match a deterministic "
                f"regeneration from candidates.json (byte-compare minus "
                f"timestamp lines)")
    return None


# ---------------------------------------------------------------------------
# render-kit


def load_audition_module():
    path = REPO_ROOT / "tools" / "audition.py"
    spec = importlib.util.spec_from_file_location("u02_audition", path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules["u02_audition"] = mod
    spec.loader.exec_module(mod)
    return mod


def preflight_oracle() -> None:
    """Verify the pinned oracle identity BEFORE the batch (never relax a
    pin). Raises NotRun/Fail per the harness conventions."""
    audition = load_audition_module()
    identity = audition.load_identity(audition.DEFAULT_IDENTITY)
    oracle_path = Path(identity["binary"]["path"])
    audition.check_oracle_binary(identity, oracle_path)
    print(f"[u02] oracle verified: {oracle_path} sha256 "
          f"{identity['binary']['sha256'][:16]}…")


def render_kit(cand: dict, zip_arg: str | None) -> int:
    phrases = load_audition_module().load_phrases(
        REPO_ROOT / "spec" / "audition-phrases-v1.json")
    n_phrases = len(phrases)
    renders_root = REPO_ROOT / "corpus" / "u02-candidates" / "renders"
    renders_root.mkdir(parents=True, exist_ok=True)
    done = skipped = 0
    for c in cand["candidates"]:
        out_dir = REPO_ROOT / c["render_dir"]
        marker = out_dir / "render-summary.json"
        complete = marker.is_file() and all(
            (out_dir / p["id"] / "render.wav").is_file()
            for p in phrases)
        if complete:
            skipped += 1
            continue
        cmd = [sys.executable, str(REPO_ROOT / "tools" / "audition.py"),
               "render", "--hash", c["canonical_hash"],
               "--phrase", "all", "--out", str(out_dir)]
        if zip_arg:
            cmd += ["--zip", zip_arg]
        proc = subprocess.run(cmd)
        if proc.returncode == 2:
            raise NotRun(f"render environment incomplete for "
                         f"{c['canonical_hash'][:12]}… (see audition output)")
        if proc.returncode != 0:
            raise Fail(f"audition render failed for "
                       f"{c['canonical_hash'][:12]}… (exit "
                       f"{proc.returncode})")
        done += 1
        print(f"[u02] rendered {done + skipped}/"
              f"{len(cand['candidates'])}: {c['name'].strip()} "
              f"({c['canonical_hash'][:12]}…)", flush=True)
    print(f"render-kit: PASS — {done} rendered, {skipped} already complete; "
          f"{len(cand['candidates']) * n_phrases} phrase renders under "
          f"{renders_root} (dry)")
    return 0


# ---------------------------------------------------------------------------


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="U02 candidate pre-selection + one-sitting listening "
                    "kit (issue #35)")
    parser.add_argument("--catalog", type=Path, default=DEFAULT_CATALOG)
    parser.add_argument("--zip", default=None,
                        help="path to the pinned archive zip (else "
                             "$DX7_ALLTHEWEB_ZIP or the catalog filename in "
                             "standard locations)")
    parser.add_argument("--out-dir", type=Path, default=DEFAULT_OUT_DIR)
    parser.add_argument("--progress", action="store_true")
    sub = parser.add_subparsers(dest="command")
    sub.add_parser("select", help="select candidates and emit the kit")
    sub.add_parser("validate",
                   help="zip-free revalidation of committed artifacts")
    p_kit = sub.add_parser("kit",
                           help="regenerate LISTENING-KIT.md from "
                                "candidates.json")
    p_kit.add_argument("--candidates", type=Path, default=None)
    p_rk = sub.add_parser("render-kit", help="render every candidate "
                                             "(idempotent)")
    p_rk.add_argument("--candidates", type=Path, default=None)
    args = parser.parse_args(argv)
    command = args.command or "select"
    try:
        if command == "select":
            cat = load_json(args.catalog)
            cand = select_candidates(cat, resolve_zip(args.zip, cat),
                                     progress=args.progress)
            args.out_dir.mkdir(parents=True, exist_ok=True)
            cpath = args.out_dir / "candidates.json"
            cpath.write_text(
                json.dumps(cand, indent=2, ensure_ascii=False) + "\n",
                encoding="utf-8")
            kpath = args.out_dir / "LISTENING-KIT.md"
            kpath.write_text(emit_kit(cand), encoding="utf-8")
            n = cand["counts"]["candidates"]
            rel = sum(1 for c in cand["coverage"]["families"].values()
                      if c["relaxed"])
            print(f"u02 select PASS: {cpath} ({n} candidates, "
                  f"{cand['counts']['families']} families x "
                  f"{PER_FAMILY}, {rel} relaxed families) + {kpath}")
            return 0
        if command == "validate":
            cat = load_json(args.catalog)
            cand = load_json(args.out_dir / "candidates.json")
            problems = validate_candidates(cand, cat,
                                           sha256_file(args.catalog))
            kit_problem = check_kit_matches(
                cand, args.out_dir / "LISTENING-KIT.md")
            if kit_problem:
                problems.append(kit_problem)
            if problems:
                for p in problems:
                    print(f"u02 validate FAIL: {p}", file=sys.stderr)
                return 1
            print(f"u02 validate PASS: "
                  f"{cand['counts']['candidates']} candidates, all 8 "
                  f"families covered, no dev32/favorite overlap, every "
                  f"entry resolves in the catalog; kit regenerates "
                  f"byte-identically (minus timestamp)")
            return 0
        if command == "kit":
            cand = load_json(args.candidates or
                             args.out_dir / "candidates.json")
            kpath = args.out_dir / "LISTENING-KIT.md"
            kpath.write_text(emit_kit(cand), encoding="utf-8")
            print(f"u02 kit PASS: wrote {kpath}")
            return 0
        if command == "render-kit":
            preflight_oracle()
            cand = load_json(args.candidates or
                             args.out_dir / "candidates.json")
            return render_kit(cand, args.zip)
        parser.error(f"unknown command {command!r}")
    except Fail as exc:
        print(f"u02 FAIL: {exc}", file=sys.stderr)
        return 1
    except NotRun as exc:
        print(f"u02 NOT_RUN: {exc}", file=sys.stderr)
        return 2
    return 2


def resolve_zip(args_zip: str | None, cat: dict) -> Path:
    """Locate the pinned archive zip: --zip, then $DX7_ALLTHEWEB_ZIP, then
    the catalog-recorded filename in standard local locations (same search
    order as tools/audition.py)."""
    filename = cat.get("archive", {}).get("filename")
    if not filename:
        raise Fail("catalog archive block has no filename")
    candidates: list[Path] = []
    if args_zip:
        candidates.append(Path(args_zip).expanduser())
    if env := os.environ.get("DX7_ALLTHEWEB_ZIP"):
        candidates.append(Path(env).expanduser())
    for root in (REPO_ROOT, REPO_ROOT / "corpus",
                 REPO_ROOT / "corpus" / "archives",
                 Path.home() / "Downloads"):
        candidates.append(root / filename)
    for cand in candidates:
        if cand.is_file():
            return cand
    raise NotRun(
        f"archive {filename!r} not found; tried: "
        + ", ".join(str(c) for c in candidates)
        + ". Pass --zip PATH (the file is hash-checked against the catalog "
          "archive pin; it is local-only and is not redistributed by this "
          "repository)")


if __name__ == "__main__":
    sys.exit(main())
