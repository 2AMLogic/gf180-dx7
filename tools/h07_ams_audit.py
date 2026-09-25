#!/usr/bin/env python3
"""H07 corpus LFO-AMS coverage + simulator-artifact audit (issue #96).

Three questions, answered from committed artifacts only (no simulation):

1. **AMS census** -- which H07 corpus patches have a nonzero per-operator
   `amp_mod_sensitivity`?  That field is the exact gate on the AM/LFO
   `exp()` unit: `dx7_core.v:2139` starts the unit iff
   `n_ams[nn][opk] != 0`, and `ampsens_tab()` (`dx7_core.v:821-828`) maps
   AMS 0 -> 0 and AMS 1/2/3 -> nonzero.  So "no operator of this patch has
   AMS != 0" is equivalent to "this case never evaluates exp()".
   Covered: all 30 dev cases (`evidence/h07-core/cases.json` +
   `reference/release/manifest.json`) **and** the 4 preregistered stress
   cases, whose voices come from the frozen registry rather than that file
   (`tools/h07_compare.py:_stress_voice`).

2. **Compared-window coverage** -- for a recorded results file, does each
   case's *compared* sample window actually contain any audio from a live
   note?  The iverilog canonical shadow is run as a bounded prefix slice
   (`--frames N`), which truncates the COMPARE WINDOW but not the vector:
   a case whose first event lands after block N is compared only over
   pre-note-on silence.  A `pass` on such a window says nothing about the
   synthesis path, and nothing at all about the AM path.

3. **Artifact divergence** -- where the same case has both an iverilog and
   a Verilator dump committed under `evidence/h07-core/runs/`, how many
   samples of the overlapping prefix actually differ?  `h07_compare.py`'s
   own docstring requires any Verilator/iverilog artifact divergence to be
   reported as a finding rather than silently accepted; the comparator
   only ever looked at the compared window, so a divergence outside it was
   never surfaced.

Exit codes: 0 no finding, 1 a finding was recorded (coverage gap and/or
artifact divergence), 2 could-not-run (NOT_RUN).  Stdlib only.
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

RUNS = REPO / "evidence" / "h07-core" / "runs"
N = 64          # samples per block / wire frame


class NotRun(Exception):
    pass


# --------------------------------------------------------------------------
# 1. AMS census
# --------------------------------------------------------------------------

def _ams_of_voice_bytes(vb: bytes) -> list[int]:
    """Per-operator amp_mod_sensitivity in OP1..OP6 order."""
    from gf180_dx7 import sysex
    voice = sysex.decode_voice(vb)
    return [op.amp_mod_sensitivity for op in voice.operators]


def ams_census() -> dict:
    import h07_compare as H

    cache = json.loads(H.CACHE.read_text(encoding="utf-8")) \
        if H.CACHE.exists() else None
    out: dict[str, dict] = {}
    for case in H.load_dev_cases():
        cid = case["id"]
        try:
            vb = H.case_voice_bytes(case, cache)
        except Exception as exc:                  # noqa: BLE001
            raise NotRun(f"{cid}: {exc}") from exc
        ams = _ams_of_voice_bytes(vb)
        out[cid] = {"set": "dev", "ams_op1_to_op6": ams,
                    "ams_nonzero": any(ams)}
    for spec in H.STRESS_SET[1]:
        sid = spec["id"]
        blob = H._stress_voice(sid)
        # stress-patchcommit carries two concatenated registry voices
        n_voices = len(blob) // 163 if len(blob) % 163 == 0 else 1
        per = [_ams_of_voice_bytes(blob[i * 163:(i + 1) * 163])
               for i in range(n_voices)] if n_voices > 1 \
            else [_ams_of_voice_bytes(blob)]
        flat = [v for voice in per for v in voice]
        out[sid] = {"set": "stress", "voices": n_voices,
                    "ams_op1_to_op6_per_voice": per,
                    "ams_nonzero": any(flat)}
    return out


# --------------------------------------------------------------------------
# 2. compared-window coverage
# --------------------------------------------------------------------------

def first_event_block() -> dict:
    import h07_compare as H
    out = {}
    for case in H.load_dev_cases():
        blocks = [r["block"] for r in case["event_trace"]]
        out[case["id"]] = min(blocks) if blocks else None
    for spec in H.STRESS_SET[1]:
        evs = list(spec.get("events", [])) + list(spec.get("patch_events", []))
        out[spec["id"]] = min(e[0] // N for e in evs) if evs else None
    return out


def golden_ints(cid: str) -> list[int] | None:
    """The frozen golden sample stream of one corpus case, as integers."""
    import h07_compare as H

    f32 = H.VECTORS / f"{cid}.f32"
    if f32.exists():
        data = f32.read_bytes()
        vals = struct.unpack("<%df" % (len(data) // 4), data)
        return [int(round(v * 32768.0)) for v in vals]
    segs = sorted(H.STRESS_DIR.glob(f"{cid}.seg*.i32"))
    if segs:
        out: list[int] = []
        for p in segs:
            d = p.read_bytes()
            out.extend(struct.unpack("<%di" % (len(d) // 4), d))
        return out
    return None


def window_coverage(results_path: Path) -> dict:
    """Per case: is the COMPARED window of `results_path` vacuous?

    A window whose golden samples are all zero can only distinguish "0"
    from "not 0".  iverilog renders an unknown sample as NUL through
    `rtl/tb_dx7_core.v`'s `$fwrite(..., "%c", ...)` dump, so an x-corrupted
    render compares EQUAL over such a window and is recorded as a pass.
    """
    if not results_path.exists():
        raise NotRun(f"{results_path} not found")
    res = json.loads(results_path.read_text(encoding="utf-8"))
    firsts = first_event_block()
    rows = {}
    for cid, row in res["cases"].items():
        checked = row.get("checked")
        blocks = None if checked is None else checked // N
        golden = golden_ints(cid)
        if checked is None:
            verdict, all_zero = "NOT_RUN", None
        elif golden is None:
            verdict, all_zero = "no_golden", None
        else:
            all_zero = not any(golden[:checked])
            verdict = "golden_silence" if all_zero else "golden_has_audio"
        rows[cid] = {"pass": row.get("pass"), "checked": checked,
                     "compared_blocks": blocks,
                     "first_event_block": firsts.get(cid),
                     "compared_golden_all_zero": all_zero,
                     "window": verdict}
        if "error" in row:
            rows[cid]["error"] = row["error"]
    return {"tool": res.get("tool"), "tag": res.get("tag"),
            "rtl_sha256": res.get("rtl_sha256"), "cases": rows}


# --------------------------------------------------------------------------
# 3. artifact divergence
# --------------------------------------------------------------------------

def _ints(path: Path) -> tuple[int, ...]:
    data = path.read_bytes()
    n = len(data) // 4
    return struct.unpack("<%di" % n, data[:n * 4])


def artifact_divergence(a_tag: str, b_tag: str) -> dict:
    rows = {}
    for adir in sorted(RUNS.glob(f"{a_tag}-*")):
        rel = adir.name[len(a_tag) + 1:]
        bdir = RUNS / f"{b_tag}-{rel}"
        if not (adir / "actual.i32").exists() \
                or not (bdir / "actual.i32").exists():
            continue
        cid = rel.split("-", 1)[1] if rel.split("-", 1)[0] in ("dev", "stress") \
            else rel
        av, bv = _ints(adir / "actual.i32"), _ints(bdir / "actual.i32")
        n = min(len(av), len(bv))
        diff = [i for i in range(n) if av[i] != bv[i]]
        rows[cid] = {
            "overlap_samples": n,
            "diverging_samples": len(diff),
            "first_diverging_index": diff[0] if diff else None,
            f"{a_tag}_nonzero": sum(1 for x in av if x != 0),
            f"{b_tag}_nonzero": sum(1 for x in bv if x != 0),
            f"{a_tag}_state_obs": _meta(adir).get("state_obs"),
            f"{b_tag}_state_obs": _meta(bdir).get("state_obs"),
        }
    return {"a": a_tag, "b": b_tag, "cases": rows}


def _meta(d: Path) -> dict:
    p = d / "meta.txt"
    if not p.exists():
        return {}
    out = {}
    for line in p.read_text(encoding="utf-8").splitlines():
        parts = line.split()
        if len(parts) >= 2:
            out[parts[0]] = parts[1] if len(parts) == 2 else parts[1:]
    return out


# --------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--results", type=Path,
                    default=REPO / "evidence" / "h07-core"
                    / "results-iverilog-shadow.json",
                    help="results file whose compared windows are audited")
    ap.add_argument("--a-tag", default="iverilog-shadow")
    ap.add_argument("--b-tag", default="verilog-accept")
    ap.add_argument("--out", type=Path, default=None)
    args = ap.parse_args()

    args.results = args.results.resolve()
    try:
        census = ams_census()
        cov = window_coverage(args.results)
        div = artifact_divergence(args.a_tag, args.b_tag)
    except NotRun as exc:
        print(json.dumps({"status": "NOT_RUN", "problems": [str(exc)]},
                         indent=1))
        return 2

    ams_nonzero = sorted(c for c, r in census.items() if r["ams_nonzero"])
    # A nonzero-AMS case whose compared window is silence-only (or that
    # never produced a compare at all) is UNCOVERED: the recorded verdict
    # for it says nothing about the AM path.
    uncovered = sorted(
        cid for cid in ams_nonzero
        if cov["cases"].get(cid, {}).get("window") in
        ("golden_silence", "NOT_RUN", "no_golden", None))
    vacuous = sorted(c for c, r in cov["cases"].items()
                     if r["window"] == "golden_silence")
    diverging = sorted(c for c, r in div["cases"].items()
                       if r["diverging_samples"] > 0)

    problems = []
    if vacuous:
        problems.append(
            "%d of %d compared windows in %s are golden-silence (every "
            "compared golden sample is 0): over such a window an "
            "x-corrupted render, dumped as NUL by rtl/tb_dx7_core.v's "
            "\"%%c\" writer, compares EQUAL and is recorded as a pass"
            % (len(vacuous), len(cov["cases"]), args.results.name))
    if uncovered:
        problems.append(
            "nonzero-AMS corpus case(s) %s have no nonzero compared golden "
            "sample in %s: their recorded verdicts carry NO information "
            "about the AM/exp() path" % (", ".join(uncovered),
                                         args.results.name))
    if diverging:
        problems.append(
            "committed %s vs %s dumps differ for case(s) %s; "
            "tools/h07_compare.py requires any Verilator/iverilog artifact "
            "divergence to be reported as a finding"
            % (args.a_tag, args.b_tag, ", ".join(diverging)))

    report = {
        "check": "issue #96 H07 corpus LFO-AMS coverage + artifact audit",
        "results_file": str(args.results.relative_to(REPO)),
        "ams_census": census,
        "ams_nonzero_cases": ams_nonzero,
        "compared_window": cov,
        "vacuous_window_cases": vacuous,
        "uncovered_nonzero_ams_cases": uncovered,
        "artifact_divergence": div,
        "diverging_cases": diverging,
        "status": "FINDING" if problems else "CLEAN",
        "problems": problems,
    }
    text = json.dumps(report, indent=1)
    print(text)
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(text + "\n", encoding="utf-8")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
