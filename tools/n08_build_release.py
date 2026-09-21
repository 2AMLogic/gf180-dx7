#!/usr/bin/env python3
"""N08 frozen-release bundle builder (issue #24).

Builds the frozen fixed-model release bundle deterministically from the
preregistration spec/release-vectors-v1.json:

  reference/release/vectors/<case>.f32   one dry f32le PCM vector per case
  reference/release/manifest.json        case list, per-case sha256, block-
                                         level event traces, model identity
                                         (module file hashes), arithmetic
                                         profile pointer, frozen thresholds,
                                         and the bundle hash

Determinism: two runs over the same tree produce byte-identical outputs.
The manifest contains no timestamp; the only provenance field
(provenance.built_at_commit, the git HEAD at build time) is explicitly
EXCLUDED from the bundle hash, so a rebuild at a later commit reproduces
the same covered content.

What is rendered: every case of the preregistered dev + challenge sets,
through the frozen D00 model (PolyManager around the integrated
single-note composition). For the single-note dev cases the build also
asserts byte-equality of the two layers (PolyManager vs render_integrated)
and records `integrated_equal: true` per case.

Freeze discipline: the builder validates the spec's binding hashes
(registry, numeric profile, dev32, model modules) against the current tree
and refuses to build on any drift (STALE -> re-freeze with a decision
record). --only builds a subset bundle (determinism testing); it must be a
subset of the preregistration.

Exit codes: 0 built; 1 failure; 2 environment (missing archive for
archive-gated rows is NOT fatal — those rows are recorded archive_gated).

The frozen model is the release: this tool establishes the RTL conformance
oracle (claim 1) and nothing about original-DX7 fidelity, RTL conformance
itself, synthesis, fit, or audibility (DEC-001, DEC-024, plan section 7).
Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import platform
import subprocess
import sys
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.model.poly import PolyManager, parse_events as poly_parse_events  # noqa: E402
from gf180_dx7.model.integrated import render_integrated  # noqa: E402
from n06_compare import dev32_voice_bytes, events_text, reference_lfo_note  # noqa: E402
from registry_coverage import input_fingerprint, load_registry  # noqa: E402
from registry_smoke import build_voice  # noqa: E402

SPEC_PATH = REPO / "spec" / "release-vectors-v1.json"
REGISTRY_PATH = REPO / "spec" / "compatibility-registry-v1.json"
PROFILE_PATH = REPO / "spec" / "numeric-profile-v1.json"
DEV32_PATH = REPO / "corpus" / "dev32.json"
DEFAULT_OUT = REPO / "reference" / "release"
DEFAULT_ARCHIVE = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")

SCHEMA = "gf180-dx7.release-vectors-manifest/1"
RELEASE_ID = "release-vectors-v1"
RATE = 48000
MODEL_MODULES = [
    "src/gf180_dx7/__init__.py",
    "src/gf180_dx7/sysex.py",
    "src/gf180_dx7/model/__init__.py",
    "src/gf180_dx7/model/operator.py",
    "src/gf180_dx7/model/envelope.py",
    "src/gf180_dx7/model/algorithm.py",
    "src/gf180_dx7/model/pitch.py",
    "src/gf180_dx7/model/tables.py",
    "src/gf180_dx7/model/integrated.py",
    "src/gf180_dx7/model/poly.py",
]


class BuildFail(Exception):
    """Exit 1: the bundle cannot be built from the current tree."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def canonical_json(obj) -> bytes:
    return json.dumps(obj, sort_keys=True, separators=(",", ":"),
                      ensure_ascii=True).encode("utf-8")


def bundle_hash(covered: dict) -> str:
    return sha256_bytes(canonical_json(covered))


# ---------------------------------------------------------------------------
# Case resolution (registry synthetic + dev32 corpus)


class CaseSource:
    """Resolves preregistered case ids to renderable inputs."""

    def __init__(self) -> None:
        self.registry = load_registry(REGISTRY_PATH)
        self.registry_cases = {c["id"]: c for c in self.registry["cases"]}
        self.dev32 = json.loads(DEV32_PATH.read_text(encoding="utf-8"))
        self.dev32_by_index = {e["index"]: e for e in self.dev32["entries"]}

    def resolve(self, case_id: str, origin: str,
                dev32_index: int | None = None) -> dict:
        """Return {voice_bytes, events (canonical lines), seconds, patch}."""
        if origin == "registry":
            case = self.registry_cases.get(case_id)
            if case is None:
                raise BuildFail(f"case {case_id!r} not in the registry")
            if case.get("blocked"):
                raise BuildFail(f"case {case_id!r} is registry-BLOCKED")
            if case["patch"].get("selector") != "synthetic":
                raise BuildFail(
                    f"case {case_id!r}: corpus-selector cases are not part "
                    "of the preregistered sets")
            voice = build_voice(self.registry, case)
            voice_bytes = sysex.encode_voice(voice)
            events = list(case["events"])
            seconds = float(case.get("render_seconds", 1.5))
            patch = {"selector": "synthetic",
                     "overrides": case["patch"].get("overrides", {})}
            if case["patch"].get("name") is not None:
                patch["name"] = case["patch"]["name"]
        elif origin == "dev32":
            entry = self.dev32_by_index.get(dev32_index)
            if entry is None:
                raise BuildFail(f"dev32 index {dev32_index} not found")
            archive = Path(DEFAULT_ARCHIVE)
            try:
                zf = zipfile.ZipFile(archive)
            except (OSError, zipfile.BadZipFile) as exc:
                raise BuildFail(
                    f"dev32-{dev32_index:02d}: pinned archive unavailable "
                    f"({exc}); voice bytes resolve only from the archive")
            with zf:
                voice_bytes = dev32_voice_bytes(zf, entry)
            events = events_text(entry["events"]).splitlines()
            seconds = float(entry["render"]["seconds"])
            patch = {"selector": "corpus",
                     "canonical_hash": entry["canonical_hash"],
                     "dev32_index": dev32_index,
                     "dev32_name": entry["name"]}
        else:
            raise BuildFail(f"unknown case origin {origin!r}")
        events = sorted(events, key=lambda ln: int(ln.split()[0]))
        return {"voice_bytes": voice_bytes, "events": events,
                "seconds": seconds, "patch": patch}


class TracedPolyManager(PolyManager):
    """PolyManager that records the block-level event application log."""

    def __init__(self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self._n08_block = 0
        self.event_trace: list[dict] = []
        self.events_applied = 0

    def apply_event(self, cmd: str, args: tuple[int, ...]) -> None:
        super().apply_event(cmd, args)
        self.event_trace.append({
            "block": self._n08_block,
            "event": " ".join([cmd] + [str(a) for a in args]),
        })
        self.events_applied += 1

    def _block_clips(self, stubs=None):
        clips = super()._block_clips(stubs)
        self._n08_block += 1
        return clips


def render_case(source: CaseSource, case_id: str, origin: str,
                dev32_index: int | None) -> dict:
    """One case render through the frozen model + its application trace."""
    inp = source.resolve(case_id, origin, dev32_index)
    voice = sysex.decode_voice(inp["voice_bytes"])
    manager = TracedPolyManager([voice])
    pcm = manager.render("\n".join(inp["events"]) + "\n", inp["seconds"])
    parsed = poly_parse_events("\n".join(inp["events"]))
    if len(parsed) != len(manager.event_trace):
        raise BuildFail(f"case {case_id}: trace/event count mismatch")
    trace = []
    for (pos, cmd, args), applied in zip(parsed, manager.event_trace):
        line = " ".join([str(pos), cmd] + [str(a) for a in args])
        if applied["event"] != " ".join([cmd] + [str(a) for a in args]):
            raise BuildFail(f"case {case_id}: trace alignment broken")
        trace.append({"block": applied["block"], "event": line})
    out = {
        "voice_bytes": inp["voice_bytes"],
        "pcm": pcm,
        "events": inp["events"],
        "seconds": inp["seconds"],
        "patch": inp["patch"],
        "trace": trace,
        "events_applied": manager.events_applied,
        "alloc_log": list(manager.alloc_log),
    }
    if origin == "dev32":
        out["lfo_note"] = reference_lfo_note(inp["voice_bytes"])
    return out


def oracle_comparability(case_id: str, origin: str, rendered: dict) -> \
        tuple[bool, str]:
    """Wrapper-validity / determinacy of the model-vs-oracle comparison."""
    if origin == "dev32":
        note = rendered["lfo_note"]
        if note["reference_class"] == "reference_indeterminate_lfo_phase":
            return False, ("reference-side LFO phase indeterminate "
                           "(R05/F-4); no derivable oracle value")
        return True, ("determinate reference render (N07 cross-checked "
                      "wrapper-valid)")
    # registry synthetic cases: the preregistered sets exclude every
    # indeterminate/divergent case (lfo-keysync0, ev-steal-17), so the
    # render is wrapper-valid by selection.
    return True, "wrapper-valid single-note registry case (determinate)"


def derive_error_budgets(profile: dict) -> dict:
    """Error budgets per property, DERIVED from the frozen numeric profile
    (recorded, never tuned): claim 1 is exact by contract; claim 2 carries
    the profile's per-decision error estimates and float-gated spots."""
    decisions = [
        {"id": d["id"], "title": d["title"], "error_class": d["error_class"],
         "error_estimate": d["error_estimate"]}
        for d in profile.get("decision_index", [])
    ]
    float_gated = [
        {"id": "float-gated:" + s["spot"], "cadence": s["cadence"],
         "model_policy": s["model_policy"],
         "error_estimate": s["derived_bound"]}
        for s in profile.get("float_gated_reference_spots", [])
    ]
    return {
        "claim_1_rtl_vs_model": {
            "budget": "EXACT (sha256-identical PCM); no numeric budget "
                      "exists or is tolerated (plan section 7)",
        },
        "claim_2_model_vs_reference": {
            "source": "spec/numeric-profile-v1.json decision_index + "
                      "float_gated_reference_spots (sha256 recorded in "
                      "covered.numeric_profile)",
            "decisions": decisions,
            "float_gated_spots": float_gated,
            "challenge_evaluation": "byte-identical (max_abs_error 0.0) at "
                                    "the frozen thresholds on every "
                                    "oracle-comparable row; non-exact rows "
                                    "are bounded findings, never budgets",
        },
    }


# ---------------------------------------------------------------------------
# Manifest assembly


def spec_selection(spec: dict) -> list[tuple[str, str, int | None]]:
    """The preregistered (case_id, origin, dev32_index) selection, in the
    spec's order (dev first, then challenge)."""
    selection: list[tuple[str, str, int | None]] = []
    dev = spec["dev_set"]
    for cid in dev["registry_single_note"]:
        selection.append((cid, "registry", None))
    for cid in dev["registry_poly"]:
        selection.append((cid, "registry", None))
    for idx in dev["dev32_indices"]:
        selection.append((f"dev32-{idx:02d}", "dev32", idx))
    for entry in spec["challenge_set"]["registry_cases"]:
        selection.append((entry["id"], "registry", None))
    return selection


def load_spec() -> dict:
    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    if spec.get("id") != RELEASE_ID:
        raise BuildFail(f"spec id mismatch: {spec.get('id')!r}")
    if not spec.get("thresholds_frozen_before_challenge"):
        raise BuildFail(
            "spec is not marked thresholds_frozen_before_challenge; "
            "refusing to build (freeze discipline)")
    binding = spec.get("binding", {})
    checks = [
        (binding.get("registry_path"), binding.get("registry_sha256")),
        (binding.get("numeric_profile_path"),
         binding.get("numeric_profile_sha256")),
        (binding.get("dev32_path"), binding.get("dev32_sha256")),
    ]
    for path, want in checks:
        if not path or not want:
            raise BuildFail("spec binding entry missing")
        got = sha256_file(REPO / path)
        if got != want:
            raise BuildFail(
                f"{path} drifted from the frozen preregistration binding "
                f"({got[:12]} != {want[:12]}); the bundle requires a "
                "conscious re-freeze (decision record), not a rebuild")
    for mod, want in binding.get("model_modules", {}).items():
        got = sha256_file(REPO / mod)
        if got != want:
            raise BuildFail(
                f"model module {mod} drifted from the frozen preregistration "
                f"({got[:12]} != {want[:12]}); any model change invalidates "
                "the bundle (STALE) and requires a full re-freeze")
    return spec


def build_bundle(out_dir: Path, *, only: list[str] | None = None,
                 archive_for_dev32: bool = True) -> dict:
    """Build the release bundle; returns the summary. Writes vectors/<id>.f32
    and manifest.json under out_dir."""
    spec = load_spec()
    source = CaseSource()
    selection = spec_selection(spec)
    if only is not None:
        want = set(only)
        have = {cid for cid, _, _ in selection}
        missing = want - have
        if missing:
            raise BuildFail(
                f"--only cases outside the preregistration: {sorted(missing)}")
        selection = [s for s in selection if s[0] in want]

    out_dir = Path(out_dir)
    (out_dir / "vectors").mkdir(parents=True, exist_ok=True)

    profile_sha = sha256_file(PROFILE_PATH)
    modules = {m: sha256_file(REPO / m) for m in MODEL_MODULES}
    for mod, want in (spec.get("binding", {}).get("model_modules") or {}).items():
        if modules.get(mod) != want:
            raise BuildFail(f"model module {mod} drifted mid-build")

    cases: list[dict] = []
    vectors: dict[str, dict] = {}
    for case_id, origin, dev32_index in selection:
        row: dict = {
            "id": case_id,
            "set": "challenge" if dev32_index is None and _is_challenge(
                spec, case_id) else "dev",
            "origin": origin,
        }
        if origin == "dev32":
            row["dev32_index"] = dev32_index
        try:
            rendered = render_case(source, case_id, origin, dev32_index)
        except BuildFail as exc:
            if "archive unavailable" in str(exc):
                row.update(status="NOT_RUN", reason=str(exc),
                           archive_gated=True)
                cases.append(row)
                continue
            raise
        comparable, why = oracle_comparability(case_id, origin, rendered)
        pcm = rendered["pcm"]
        vec_rel = f"vectors/{case_id}.f32"
        (out_dir / vec_rel).write_bytes(pcm)
        record = {
            "id": case_id,
            "set": row["set"],
            "origin": origin,
            "patch": rendered["patch"],
            "patch_sha256": sha256_bytes(rendered["voice_bytes"]),
            "events": rendered["events"],
            "events_sha256": sha256_bytes(
                ("\n".join(rendered["events"]) + "\n").encode("utf-8")),
            "render_seconds": rendered["seconds"],
            "expected_pcm_sha256": sha256_bytes(pcm),
            "samples": len(pcm) // 4,
            "event_trace": rendered["trace"],
            "events_applied": rendered["events_applied"],
            "oracle_comparable": comparable,
            "oracle_note": why,
            "input_fingerprint": _fingerprint(case_id, origin,
                                              dev32_index, source),
        }
        if origin == "dev32":
            record["dev32_index"] = dev32_index
        if rendered["alloc_log"]:
            record["alloc_log"] = rendered["alloc_log"]
        if row["set"] == "dev" and origin == "registry" and \
                case_id in spec["dev_set"]["registry_single_note"]:
            voice = sysex.decode_voice(rendered["voice_bytes"])
            integrated_pcm = render_integrated(
                voice, "\n".join(rendered["events"]) + "\n",
                seconds=rendered["seconds"])
            if integrated_pcm != pcm:
                raise BuildFail(
                    f"case {case_id}: the poly manager and the integrated "
                    "single-note model disagree; the frozen tree must not "
                    "be released")
            record["integrated_equal"] = True
        cases.append(record)
        vectors[case_id] = {"file": vec_rel,
                            "sha256": record["expected_pcm_sha256"],
                            "samples": record["samples"],
                            "bytes": len(pcm)}

    covered = {
        "schema": SCHEMA,
        "release_id": RELEASE_ID,
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/24",
        "sample_rate": RATE,
        "model_identity": {
            "modules": modules,
            "renderer": "src/gf180_dx7/model/poly.py PolyManager around "
                        "src/gf180_dx7/model/integrated.py (the frozen N06+"
                        "N07 tree)",
        },
        "numeric_profile": {
            "path": "spec/numeric-profile-v1.json",
            "sha256": profile_sha,
        },
        "spec": {"path": "spec/release-vectors-v1.json",
                 "sha256": sha256_file(SPEC_PATH)},
        "thresholds": spec["thresholds"],
        "thresholds_frozen_before_challenge": True,
        "threshold_source_note": "thresholds are copied verbatim from the "
                                 "preregistration spec (sha256 bound above); "
                                 "they were frozen before the challenge run",
        "error_budgets": derive_error_budgets(
            json.loads(PROFILE_PATH.read_text(encoding="utf-8"))),
        "excluded_cases": spec["excluded_cases"],
        "cases": cases,
        "vectors": vectors,
    }
    manifest = {
        "schema": SCHEMA,
        "release_id": RELEASE_ID,
        "covered": covered,
        "bundle_sha256": bundle_hash(covered),
        "provenance": {
            "built_at_commit": _git_head(),
            "builder": "tools/n08_build_release.py",
            "builder_sha256": sha256_file(Path(__file__).resolve()),
            "python": platform.python_version(),
            "note": "provenance is NOT covered by the bundle hash; the "
                    "covered content above is the frozen identity",
        },
    }
    (out_dir / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return {
        "out_dir": out_dir,
        "n_cases": sum(1 for c in cases if c.get("status") != "NOT_RUN"),
        "n_not_run": sum(1 for c in cases if c.get("status") == "NOT_RUN"),
        "n_dev": sum(1 for c in cases if c.get("set") == "dev"),
        "n_challenge": sum(1 for c in cases if c.get("set") == "challenge"),
        "bundle_sha256": manifest["bundle_sha256"],
        "total_vector_bytes": sum(v["bytes"] for v in vectors.values()),
    }


def _is_challenge(spec: dict, case_id: str) -> bool:
    return any(e["id"] == case_id
               for e in spec["challenge_set"]["registry_cases"])


def _fingerprint(case_id: str, origin: str, dev32_index: int | None,
                 source: CaseSource) -> str | None:
    try:
        if origin == "registry":
            return input_fingerprint(source.registry_cases[case_id])
        entry = source.dev32_by_index[dev32_index]
        return input_fingerprint({
            "patch": {"selector": "corpus",
                      "canonical_hash": entry["canonical_hash"]},
            "events": events_text(entry["events"]).splitlines(),
            "render_seconds": entry["render"]["seconds"],
        })
    except Exception:
        return None


def _git_head() -> str | None:
    try:
        out = subprocess.run(["git", "rev-parse", "HEAD"], cwd=REPO,
                             capture_output=True, text=True)
        return out.stdout.strip() if out.returncode == 0 else None
    except OSError:
        return None


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT,
                        help="output directory (default reference/release)")
    parser.add_argument("--only", default=None,
                        help="comma-separated case ids; must be a subset of "
                             "the preregistration (determinism testing)")
    args = parser.parse_args(argv)
    only = [s.strip() for s in args.only.split(",")] if args.only else None
    try:
        summary = build_bundle(args.out, only=only)
    except BuildFail as exc:
        print(f"n08_build_release: FAIL — {exc}", file=sys.stderr)
        return 1
    print(
        f"n08_build_release: PASS — {summary['n_cases']} case vector(s) "
        f"({summary['n_dev']} dev / {summary['n_challenge']} challenge, "
        f"{summary['n_not_run']} NOT_RUN), {summary['total_vector_bytes']} "
        f"vector bytes, bundle_sha256 {summary['bundle_sha256'][:16]}…, "
        f"manifest: {summary['out_dir'] / 'manifest.json'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
