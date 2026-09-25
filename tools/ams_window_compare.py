#!/usr/bin/env python3
"""Conformance compare of one H07 dev case over a window that CONTAINS
the live note (issue #96).

Why this exists.  The recorded iverilog canonical shadow was run as
``--frames 24``, which caps the *compare window* but not the vector.
Every dev case's first event is at block 75, so the shadow compared only
pre-note-on silence: 30 of its 34 compared windows contain no nonzero
golden sample at all (see ``tools/h07_ams_audit.py``).  A pass over such
a window cannot distinguish a correct render from an all-x one, because
``rtl/tb_dx7_core.v`` dumps each sample with ``$fwrite(..., "%c", ...)``
and iverilog renders an unknown byte as NUL.

Comparing a *full-length* dev case under iverilog costs ~2 h on one core
(the vector plays to block 1875).  This tool compares the same case over
a short window that still reaches past the note-on, by dropping the event
trace rows that occur after the window and shortening the tail.  The
comparison itself is the harness's own ``compare_case()`` against the
untouched frozen golden prefix -- no golden is regenerated, recomputed or
relaxed here.

**Why truncating the trace is sound, and how that is checked rather than
asserted.**  The core is causal, so an event at block 750 cannot change a
sample rendered at block 100; dropping it must leave the compared prefix
bit-identical.  That is not taken on faith: run this tool on an AMS = 0
case (e.g. ``dev32-03``, whose event trace is identical to
``dev32-30``'s) and it must PASS bit-exactly against the frozen golden
prefix under the same simulator.  If truncation were unsound, that
control would fail too.  ``tests/test_ams_audit.py`` keeps the recorded
control live.

Expected outcomes on the frozen DR-0011 core:

* ``--case dev32-30 --tool iverilog`` -> **FAIL**, first mismatch in the
  first block after the note-on, actual = 0 where the golden is audio
  (the x-corrupted samples dumped as NUL).
* ``--case dev32-30 --tool verilator`` -> **PASS** (same window).
* ``--case dev32-03 --tool iverilog`` -> **PASS** (AMS = 0 control; also
  the truncation control).

Exit codes: 0 the compare ran and matched its declared expectation, 1 it
ran and did not, 2 could-not-run (NOT_RUN).  Stdlib only.
"""

from __future__ import annotations

import argparse
import copy
import hashlib
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

N = 64
DR0011_PIN = "34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58"


class NotRun(Exception):
    pass


def sha256_file(path: Path) -> str:
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def tool_version(tool: str) -> str | None:
    """First line of the simulator's own --version banner (evidence)."""
    import subprocess
    flag = "-V" if tool == "iverilog" else "--version"
    try:
        p = subprocess.run([tool, flag], capture_output=True, text=True)
        return (p.stdout or p.stderr).splitlines()[0].strip()
    except Exception:                                 # noqa: BLE001
        return None


def ams_of(case_id: str, cases: dict, cache) -> list[int]:
    from gf180_dx7 import sysex
    import h07_compare as H
    voice = sysex.decode_voice(H.case_voice_bytes(cases[case_id], cache))
    return [op.amp_mod_sensitivity for op in voice.operators]


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--case", default="dev32-30")
    ap.add_argument("--tool", choices=("iverilog", "verilator"),
                    default="iverilog")
    ap.add_argument("--window-blocks", type=int, default=120,
                    help="compared golden blocks (default 120: the note-on "
                         "is at block 75, so 45 blocks of live audio)")
    ap.add_argument("--tag", default=None)
    ap.add_argument("--out", type=Path, default=None)
    args = ap.parse_args()

    try:
        import h07_compare as H
        from gf180_dx7 import sysex
        from gf180_dx7.model.algorithm import voice_patch

        cache = json.loads(H.CACHE.read_text(encoding="utf-8")) \
            if H.CACHE.exists() else None
        cases = {c["id"]: c for c in H.load_dev_cases()}
        if args.case not in cases:
            raise NotRun(f"{args.case}: not an H07 dev corpus case")
        case = copy.deepcopy(cases[args.case])
        ams = ams_of(args.case, cases, cache)

        kept = [r for r in case["event_trace"]
                if r["block"] < args.window_blocks]
        dropped = [r["block"] for r in case["event_trace"]
                   if r["block"] >= args.window_blocks]
        if not kept:
            raise NotRun(f"{args.case}: no event inside a "
                         f"{args.window_blocks}-block window")
        case["event_trace"] = kept

        body = voice_patch(sysex.decode_voice(
            H.case_voice_bytes(cases[args.case], cache)))
        vtext, _ = H.build_dev_vector(case, body, frames=args.window_blocks)
        n = min(case["_n"], args.window_blocks * N)
        golden = H.golden_ints_from_f32(case["_f32"], case["_sha"],
                                        case["_n"])[:n]

        tag = args.tag or f"issue96-window-{args.tool}"
        work = H.OUTDIR / "sim" / f"{tag}-build"
        binary = H.build_sim(args.tool, work, [])
        afile, _, meta = H.play_sim(args.tool, binary, vtext,
                                    f"{tag}-dev-{args.case}")
        res = H.compare_case([{"ints": golden}], afile, meta)

        import struct
        acts = afile.read_bytes()
        acts = struct.unpack("<%di" % (len(acts) // 4), acts)
        base = int(meta.get("commits", ["0"])[0])
        off = (base + 2) * N
        window = acts[off:off + n]

        report = {
            "check": "issue #96: H07 conformance compare over a window that "
                     "contains the live note",
            "case": args.case,
            "tool": args.tool,
            "tool_version": tool_version(args.tool),
            "window_blocks": args.window_blocks,
            "compared_samples": n,
            "first_event_block": min(r["block"] for r in kept),
            "event_blocks_kept": [r["block"] for r in kept],
            "event_blocks_dropped_after_window": dropped,
            "amp_mod_sensitivity_op1_to_op6": ams,
            "ams_nonzero": any(ams),
            "rtl_sha256": sha256_file(REPO / "rtl" / "dx7_core.v"),
            "rtl_matches_dr0011_pin":
                sha256_file(REPO / "rtl" / "dx7_core.v") == DR0011_PIN,
            "actual_sha256": sha256_file(afile),
            "overrun": int(meta.get("overrun", ["1"])[0]),
            "overflow": int(meta.get("overflow", ["1"])[0]),
            "state_obs": (meta.get("state_obs") or [None])[0],
            "golden_nonzero_samples": sum(1 for v in golden if v != 0),
            "actual_nonzero_samples": sum(1 for v in window if v != 0),
            "pass": res["pass"],
            "checked": res["checked"],
            "mismatches": res["mismatches"],
        }
        if res["mismatches"]:
            report["first_mismatch_block"] = res["mismatches"][0]["block"]

        # Declared expectation: a nonzero-AMS case under iverilog must NOT
        # pass this window (that is the whole point -- it is the negative
        # control the recorded shadow never ran); everything else must.
        expect_pass = not (args.tool == "iverilog" and any(ams))
        problems = []
        if expect_pass and not res["pass"]:
            problems.append(
                "%s/%s was expected to be bit-exact over this window and is "
                "not; the window truncation or the harness itself is "
                "suspect, not just the AM path" % (args.case, args.tool))
        if not expect_pass and res["pass"]:
            problems.append(
                "%s/%s passed a window containing live audio on a nonzero-AMS "
                "patch; issue #96's x-propagation finding does not reproduce "
                "through the conformance harness and must be re-derived"
                % (args.case, args.tool))
        report["expect_pass"] = expect_pass
        report["status"] = "FAIL" if problems else "PASS"
        report["problems"] = problems
        report["note"] = (
            "status is the verdict on the DECLARED EXPECTATION, not on the "
            "case: the iverilog/nonzero-AMS row is expected to be a "
            "conformance FAIL, and `pass: false` there is the finding."
        )

        text = json.dumps(report, indent=1)
        print(text)
        if args.out:
            args.out.parent.mkdir(parents=True, exist_ok=True)
            args.out.write_text(text + "\n", encoding="utf-8")
        return 0 if report["status"] == "PASS" else 1
    except NotRun as exc:
        print(json.dumps({"status": "NOT_RUN", "problems": [str(exc)]},
                         indent=1))
        return 2
    except Exception as exc:                          # noqa: BLE001
        print(json.dumps({"status": "NOT_RUN",
                          "problems": [f"{type(exc).__name__}: {exc}"]},
                         indent=1))
        return 2


if __name__ == "__main__":
    sys.exit(main())
