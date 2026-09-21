#!/usr/bin/env python3
"""N08 mutation-detection battery against the frozen release (issue #24).

Applies each preregistered MODEL mutation — a classic defect class a
conforming implementation could suffer — and proves that the release
verification (tools/n08_verify_release.py re-render pass) CATCHES it: the
mutated model's render must diverge from the frozen vectors, and the
verifier must FAIL naming the case. A mutation that verify does not catch
is a hole in the freeze and fails the battery.

Mutations (applied at runtime to the frozen model's seams; no source file
is modified, patches are restored after each mutation):

  param_shuffle        operator envelope parameters shuffled (wrong op ->
                       envelope mapping; the "wrong operator order" control)
  wrong_gain           output conversion >>8 instead of >>9 (+6 dB datapath
                       gain error)
  broken_feedback      feedback history delayed by one block (broken fb
                       history control)
  silent_stub          every voice frame renders silence (dead/stale module)
  voice_sum_order_swap the per-voice clip and the voice sum are applied in
                       the swapped order (sum-then-clip instead of the
                       frozen clip-then-sum); only detectable where voices
                       actually sum past the per-voice clip rails, so it
                       runs on ev-sum-15

Results are committed to reference/release/mutation-results.json, bound to
the exact bundle (bundle_sha256).

Exit codes: 0 = every requested mutation detected; 1 = any mutation
UNDETECTED (a freeze hole) or bundle/verify problem; 2 = could not run.
Stdlib-only.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7.model import algorithm as algorithm_mod  # noqa: E402
from gf180_dx7.model import integrated as integrated_mod  # noqa: E402
from gf180_dx7.model import operator as operator_mod  # noqa: E402
from gf180_dx7.model import poly as poly_mod  # noqa: E402
from gf180_dx7.model.operator import N  # noqa: E402
from gf180_dx7.model.pitch import i32  # noqa: E402
from n08_build_release import DEFAULT_OUT, sha256_file  # noqa: E402
from n08_verify_release import verify_release  # noqa: E402

SCHEMA = "gf180-dx7.n08-mutation-results/1"

# Cases per mutation: cheap dev-set bundle cases chosen so the defect class
# MUST change the PCM (voice_sum_order_swap needs a multi-voice case that
# actually sums past the per-voice clip rails -> ev-sum-15).
MUTATIONS = [
    {
        "id": "param_shuffle",
        "description": "operator envelope rates/levels reversed per op "
                       "(wrong operator -> envelope parameter mapping)",
        "seam": "gf180_dx7.model.algorithm.derive_op_envelope_params "
                "(+ integrated namespace)",
        "cases": ["dir-base", "ev-repeated-note"],
    },
    {
        "id": "wrong_gain",
        "description": "output conversion shifts >>8 instead of >>9 "
                       "(+6 dB datapath gain error)",
        "seam": "gf180_dx7.model.operator.clip_val_domain (all importer "
                "namespaces)",
        "cases": ["dir-base", "ev-pitch-bend-up"],
    },
    {
        "id": "broken_feedback",
        "description": "per-note feedback history delayed by one block "
                       "(block N+1 reads block N-1's history)",
        "seam": "gf180_dx7.model.algorithm.AlgorithmRouter.render fb_buf "
                "handling",
        "cases": ["dir-base", "alg4-fb7"],
    },
    {
        "id": "silent_stub",
        "description": "every voice frame renders exact silence (dead or "
                       "stale module stub)",
        "seam": "gf180_dx7.model.poly.PolyManager.compute_frame",
        "cases": ["dir-base", "ev-sustain-pedal"],
    },
    {
        "id": "voice_sum_order_swap",
        "description": "mix pipeline order swapped: voices summed raw then "
                       "clipped once, instead of the frozen per-voice clip "
                       "then exact integer sum",
        "seam": "gf180_dx7.model.poly.PolyManager._block_clips",
        "cases": ["ev-sum-15"],
        "slow": True,
    },
]


# ---------------------------------------------------------------------------
# Patch helpers (runtime model mutations; always restored)


def _patch(target, name, replacement, originals):
    originals.append((target, name, target.__dict__.get(name, _MISSING)))
    setattr(target, name, replacement)


def _restore(originals) -> None:
    for target, name, value in reversed(originals):
        if value is _MISSING:
            delattr(target, name)
        else:
            setattr(target, name, value)
    originals.clear()


_MISSING = object()


def apply_param_shuffle(originals) -> None:
    orig = algorithm_mod.derive_op_envelope_params

    def shuffled(opv, midinote, velocity):
        rates, levels, outlevel, rate_scaling = orig(opv, midinote, velocity)
        return list(reversed(rates)), list(reversed(levels)), outlevel, \
            rate_scaling

    _patch(algorithm_mod, "derive_op_envelope_params", shuffled, originals)
    _patch(integrated_mod, "derive_op_envelope_params", shuffled, originals)


def apply_wrong_gain(originals) -> None:
    def wrong_gain_clip(block):
        out = []
        for v in block:
            val = operator_mod._i32(v) >> 4
            if val < -(1 << 24):
                out.append(0x8000)
            elif val >= (1 << 24):
                out.append(0x7FFF)
            else:
                out.append(val >> 8)      # MUTATION: was >> 9
        return out

    _patch(operator_mod, "clip_val_domain", wrong_gain_clip, originals)
    _patch(integrated_mod, "clip_val_domain", wrong_gain_clip, originals)
    _patch(poly_mod, "clip_val_domain", wrong_gain_clip, originals)


def apply_broken_feedback(originals) -> None:
    orig_render = algorithm_mod.AlgorithmRouter.render
    cache: dict[int, list[int]] = {}

    def delayed_render(self, output, params, alg, fb_buf, feedback_shift,
                       sin_fn=None):
        key = id(fb_buf)
        pending = [fb_buf[0], fb_buf[1]]      # history from block N-1
        stale = cache.get(key, [0, 0])        # history from block N-2
        fb_buf[0], fb_buf[1] = stale[0], stale[1]
        orig_render(self, output, params, alg, fb_buf, feedback_shift,
                    sin_fn)
        cache[key] = pending

    _patch(algorithm_mod.AlgorithmRouter, "render", delayed_render,
           originals)


def apply_silent_stub(originals) -> None:
    def silent_frame(self, note, lfo_val, lfo_delay, stubs=None):
        return [0] * N

    _patch(poly_mod.PolyManager, "compute_frame", silent_frame, originals)


def apply_voice_sum_order_swap(originals) -> None:
    def sum_then_clip(self, stubs=None):
        self._consume_refresh_voice()
        lfo_val = self.lfo.getsample()
        lfo_delay = self.lfo.getdelay()
        raw = [0] * N
        for slot in self.slots:
            if not slot.live:
                continue
            buf = self.compute_frame(slot.note, lfo_val, lfo_delay, stubs)
            for j in range(N):
                raw[j] = i32(raw[j] + buf[j])
        self._reclaim_pass()
        return operator_mod.clip_val_domain(raw)   # MUTATION: clip once, late

    _patch(poly_mod.PolyManager, "_block_clips", sum_then_clip, originals)


_APPLY = {
    "param_shuffle": apply_param_shuffle,
    "wrong_gain": apply_wrong_gain,
    "broken_feedback": apply_broken_feedback,
    "silent_stub": apply_silent_stub,
    "voice_sum_order_swap": apply_voice_sum_order_swap,
}


# ---------------------------------------------------------------------------
# Battery


def run_battery(release_dir: Path, mutation_ids: list[str]) -> tuple[
        list[dict], list[str]]:
    """Run each requested mutation; returns (results, problems)."""
    manifest = json.loads((release_dir / "manifest.json").read_text(
        encoding="utf-8"))
    known = {c["id"] for c in manifest["covered"]["cases"]}
    results: list[dict] = []
    problems: list[str] = []
    for mutation in MUTATIONS:
        mid = mutation["id"]
        if mid not in mutation_ids:
            continue
        cases = mutation["cases"]
        unknown = [c for c in cases if c not in known]
        if unknown:
            problems.append(f"{mid}: cases outside the bundle: {unknown}")
            continue
        originals: list = []
        try:
            _APPLY[mid](originals)
            code, rows = verify_release(release_dir, only=cases,
                                        rerender=True)
        finally:
            _restore(originals)
        mismatching = sorted({r["case"] for r in rows
                              if r["check"] == "rerender"
                              and r["status"] == "FAIL"})
        # The battery rewrites mutation-results.json (bound to this bundle)
        # after the run, so a stale binding on THAT file is expected here;
        # every other non-rerender failure is a real problem.
        other_failures = [r for r in rows if r["status"] == "FAIL"
                          and r["check"] != "rerender"
                          and r["check"] != "mutation-results.json"]
        detected = (code == 1 and bool(mismatching)
                    and not other_failures)
        row = {
            "id": mid,
            "description": mutation["description"],
            "seam": mutation["seam"],
            "cases_rendered": cases,
            "verify_exit": code,
            "mismatching_cases": mismatching,
            "detected": detected,
        }
        if not detected:
            row["note"] = ("UNDETECTED MUTATION — the release verification "
                           "did not catch this defect class; this is a hole "
                           "in the freeze")
            problems.append(f"{mid}: UNDETECTED (verify_exit={code}, "
                            f"mismatching={mismatching}, other_failures="
                            f"{[r['check'] for r in other_failures]})")
        results.append(row)
    return results, problems


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--release-dir", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--mutations", default=None,
                        help="comma-separated subset (default: all)")
    parser.add_argument("--out", type=Path, default=None,
                        help="results path (default <release-dir>/"
                             "mutation-results.json)")
    args = parser.parse_args(argv)
    release_dir = Path(args.release_dir)
    ids = ([s.strip() for s in args.mutations.split(",")]
           if args.mutations else [m["id"] for m in MUTATIONS])
    if not (release_dir / "manifest.json").is_file():
        print(f"n08_mutations: NOT_RUN — no bundle at {release_dir}",
              file=sys.stderr)
        return 2
    bundle = json.loads((release_dir / "manifest.json").read_text(
        encoding="utf-8"))["bundle_sha256"]
    results, problems = run_battery(release_dir, ids)
    record = {
        "schema": SCHEMA,
        "bundle_sha256": bundle,
        "results": results,
        "all_detected": bool(results) and all(r["detected"] for r in results),
        "negative_control_note": (
            "each mutation is applied to the frozen model at runtime and "
            "must be caught by tools/n08_verify_release.py's re-render pass "
            "naming the case; a clean bundle alone proves nothing — this "
            "battery proves the verification fails loudly under realistic "
            "defect classes"),
    }
    out_path = args.out or (release_dir / "mutation-results.json")
    out_path.write_text(json.dumps(record, indent=2) + "\n",
                        encoding="utf-8")
    if problems:
        for p in problems:
            print(f"n08_mutations: FAIL — {p}")
        print(f"n08_mutations: FAIL — {sum(1 for r in results if not r['detected'])}"
              f" undetected mutation(s) of {len(results)}; results: {out_path}")
        return 1
    print(f"n08_mutations: PASS — {len(results)}/{len(results)} mutation(s) "
          f"detected by the release verification; results: {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
