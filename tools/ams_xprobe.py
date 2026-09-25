#!/usr/bin/env python3
"""LFO-AMS X-propagation probe for the simulated core (issue #96).

Issue #86 proved that the out-of-range part-selects at
`rtl/dx7_core.v:953-961` (`exp_t3[85:30]`, `exp_t4[85:30]`,
`exp_t5[81:26]`, read from wires declared `[70:0]`/`[70:0]`/`[65:0]`) carry
a *true* value of 0 for every reachable operand, and that Icarus Verilog
nonetheless returns `x` for them while Verilator folds them to 0.  #86
showed that at the exp() unit's boundary, by hierarchical force.

This tool asks the next question: does that `x` actually reach the audio
path of the *whole* core, on a real patch, driven through the real SPI
port -- i.e. is the hazard live rather than theoretical?

Mechanics: the chosen H07 corpus patch is serialised into the core's SPI
register map exactly as `tools/h07_compare.py` does (same `page_writes()`
/ `note_on_writes()` helpers), written as 48-bit SPI frames into two memh
files, and played by `evidence/issue-96-ams-xprobe/tb_ams_xprobe.v`, which
watches `exp_hsum`, `exp_pt`, `exp_li`, `w_li`, `op_li[0][*]` and
`tap_mix` for 4-state unknowns during a bounded window after the note-on.

Why a dedicated probe and not the conformance dump: `rtl/tb_dx7_core.v`
writes each sample with `$fwrite(..., "%c", ...)`, and iverilog renders an
`x` byte as NUL -- so an unknown sample lands in `actual.i32` as a plain
0 and is compared as the integer 0.  The unknown is destroyed before the
comparator ever sees it.  This probe reads the 4-state value directly.

Controls (both must come back clean for the positive result to mean what
it claims):

* ``--zero-ams``    zeroes every operator's `amp_mod_sensitivity` nibble in
                    the same patch.  `dx7_core.v:2139` gates the exp() unit
                    on `n_ams[nn][opk] != 0`, so the unit is never started
                    and no unknown may appear.
* ``--rtl PATH``    runs the same probe against a scratch RTL copy (e.g.
                    one with `exp_t3`/`exp_t4` widened to `[85:0]` and
                    `exp_t5` to `[81:0]`), where the selects are in range.
                    Use ``--widened`` to have that copy generated here.

Exit codes: 0 the probe ran and its report was written, 1 the probe ran
and a declared expectation was violated, 2 could-not-run (NOT_RUN).
Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

TB = REPO / "evidence" / "issue-96-ams-xprobe" / "tb_ams_xprobe.v"
DX7_CORE = REPO / "rtl" / "dx7_core.v"
OTHER_RTL = ("rtl/env_unit.v", "rtl/alg_router.v")
DR0011_PIN = "34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58"

R_COMMIT = 0x42
R_GLOBC = 0x43


class NotRun(Exception):
    pass


def sha256_file(path: Path) -> str:
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


# --------------------------------------------------------------------------
# patch resolution (the H07 corpus, via the same helpers the harness uses)
# --------------------------------------------------------------------------

def corpus_body(case_id: str) -> list[int]:
    """The 155-byte unpacked voice body of one H07 dev corpus case."""
    import h07_compare as H
    from gf180_dx7 import sysex
    from gf180_dx7.model.algorithm import voice_patch

    cache = json.loads(H.CACHE.read_text(encoding="utf-8")) \
        if H.CACHE.exists() else None
    cases = {c["id"]: c for c in H.load_dev_cases()}
    if case_id not in cases:
        raise NotRun(f"{case_id}: not an H07 dev corpus case "
                     f"({', '.join(sorted(cases))})")
    try:
        vb = H.case_voice_bytes(cases[case_id], cache)
    except Exception as exc:                      # noqa: BLE001
        raise NotRun(f"{case_id}: {exc}") from exc
    return voice_patch(sysex.decode_voice(vb))


def body_ams(body: list[int]) -> list[int]:
    """Per-operator amp_mod_sensitivity in *patch-body* order.

    The 155-byte body stores operators OP6 first (body[0..20] is OP6), the
    same order `page_writes()` packs into GLOB_C, so index 0 here is OP6
    and index 5 is OP1 -- the reverse of `sysex.Voice.operators`.
    """
    return [body[k * 21 + 14] & 3 for k in range(6)]


def zero_ams(body: list[int]) -> list[int]:
    out = list(body)
    for k in range(6):
        out[k * 21 + 14] = 0
    return out


def spi_frames(body: list[int], note: int, velocity: int
               ) -> tuple[list[int], list[int]]:
    """(patch-page frames incl. COMMIT, note-on frames) as 48-bit words.

    Frame layout is the contracted {F, 6'b0, SEC, A[7:0], D[31:0]} of
    rtl/dx7_core.v -- identical to what tools/h07_compare.py's vector
    grammar makes rtl/tb_dx7_core.v emit for `P`/`C`/`E` records.
    """
    import h07_compare as H

    def frame(sec: int, addr: int, data: int) -> int:
        return (1 << 47) | ((sec & 1) << 40) | ((addr & 0xFF) << 32) \
            | (data & 0xFFFFFFFF)

    page = [frame(1, a, d) for a, d in H.page_writes(body)]
    page.append(frame(1, R_COMMIT, 0))
    ev = [frame(0, a, d) for a, d in H.note_on_writes(body, note, velocity)]
    return page, ev


# --------------------------------------------------------------------------
# RTL variants
# --------------------------------------------------------------------------

WIDEN = (
    (r"wire \[70:0\]  exp_t3", "wire [85:0]  exp_t3"),
    (r"wire \[70:0\]  exp_t4", "wire [85:0]  exp_t4"),
    (r"wire \[65:0\]  exp_t5", "wire [81:0]  exp_t5"),
)


def widened_rtl(src: Path, dst: Path) -> Path:
    """A scratch copy with the three product wires widened so that the
    existing reads are in range.  Issue #86 proved the extra bits are 0 for
    every reachable operand, so this changes no *value* -- only whether the
    simulator has a bit to return.  Scratch only: never the frozen RTL."""
    text = src.read_text(encoding="utf-8")
    for pat, rep in WIDEN:
        text, n = re.subn(pat, rep, text, count=1)
        if n != 1:
            raise NotRun(f"widening pattern {pat!r} did not match {src}")
    dst.write_text(text, encoding="utf-8")
    return dst


# --------------------------------------------------------------------------
# simulation
# --------------------------------------------------------------------------

PROBE_RE = re.compile(r"^PROBE ")


def variant_label(args) -> str:
    bits = [args.sim, "widened" if args.widened else "as-frozen"]
    if args.zero_ams:
        bits.append("AMS=0")
    return " ".join(bits)


def run_probe(sim: str, rtl: Path, page: list[int], ev: list[int],
              runframes: int, workdir: Path) -> dict:
    workdir.mkdir(parents=True, exist_ok=True)
    pfile = workdir / "patch.memh"
    efile = workdir / "events.memh"
    pfile.write_text("".join("%012x\n" % f for f in page), encoding="utf-8")
    efile.write_text("".join("%012x\n" % f for f in ev), encoding="utf-8")

    srcs = [str(TB), str(rtl)] + [str(REPO / f) for f in OTHER_RTL]
    if shutil.which(sim) is None:
        raise NotRun(f"{sim} not on PATH")
    if sim == "iverilog":
        exe = workdir / "probe.vvp"
        build = ["iverilog", "-g2012", "-s", "tb_ams_xprobe", "-o", str(exe)]
        build += srcs
        runc = ["vvp", "-n", str(exe)]
    else:
        # the same flags tools/h07_compare.py:build_sim() uses
        build = ["verilator", "--binary", "--timing", "-j", "2",
                 "--Wno-fatal", "--timescale-override", "1ns/1ps",
                 "--x-initial", "0", "--x-assign", "unique",
                 "--Mdir", str(workdir / "vl"), "-o", "probe",
                 "--top-module", "tb_ams_xprobe"] + srcs
        runc = [str(workdir / "vl" / "probe")]
    runc += [f"+frames={pfile}", f"+events={efile}",
             f"+runframes={runframes}"]
    for cmd in (build, runc):
        p = subprocess.run(cmd, cwd=str(REPO), capture_output=True,
                           text=True)
        if p.returncode != 0:
            raise NotRun(f"{cmd[0]} failed (rc={p.returncode}): "
                         f"{p.stdout[-1500:]}{p.stderr[-1500:]}")
    if "PROBE-DONE" not in p.stdout:
        raise NotRun(f"probe did not reach PROBE-DONE: {p.stdout[-1500:]}")
    out: dict = {"stdout": [ln for ln in p.stdout.splitlines()
                            if PROBE_RE.match(ln)]}
    for ln in out["stdout"]:
        for tok in ln.split()[1:]:
            if "=" in tok:
                k, v = tok.split("=", 1)
                try:
                    out[k] = int(v)
                except ValueError:
                    out[k] = v
    return out


# --------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--case", default="dev32-30",
                    help="H07 dev corpus case id (default dev32-30)")
    ap.add_argument("--sim", choices=("iverilog", "verilator"),
                    default="iverilog")
    ap.add_argument("--rtl", type=Path, default=DX7_CORE,
                    help="dx7_core RTL under test (default the frozen "
                         "rtl/dx7_core.v)")
    ap.add_argument("--widened", action="store_true",
                    help="negative control: generate and probe a scratch "
                         "copy with exp_t3/t4/t5 widened so the reads are "
                         "in range (never touches the frozen RTL)")
    ap.add_argument("--zero-ams", action="store_true",
                    help="negative control: zero every operator's "
                         "amp_mod_sensitivity so the exp() unit is never "
                         "started")
    ap.add_argument("--note", type=int, default=60)
    ap.add_argument("--velocity", type=int, default=100)
    ap.add_argument("--runframes", type=int, default=12,
                    help="wire frames to run after the note-on")
    ap.add_argument("--out", type=Path, default=None,
                    help="write the JSON report here (default stdout only)")
    args = ap.parse_args()

    try:
        body = corpus_body(args.case)
        ams = body_ams(body)
        if args.zero_ams:
            body = zero_ams(body)
        page, ev = spi_frames(body, args.note, args.velocity)

        with tempfile.TemporaryDirectory() as td:
            work = Path(td)
            rtl = args.rtl.resolve()
            if args.widened:
                rtl = widened_rtl(rtl, work / "dx7_core_widened.v")
            res = run_probe(args.sim, rtl, page, ev, args.runframes, work)
            rtl_sha = sha256_file(rtl)

        variant = ("widened" if args.widened else "as-frozen")
        report = {
            "check": "issue #96 LFO-AMS X-propagation probe "
                     "(whole-core, real SPI stimulus)",
            "case": args.case,
            "simulator": args.sim,
            "rtl_variant": variant,
            "rtl_sha256": rtl_sha,
            "frozen_rtl_sha256": sha256_file(DX7_CORE),
            "frozen_rtl_matches_dr0011_pin":
                sha256_file(DX7_CORE) == DR0011_PIN,
            "patch_amp_mod_sensitivity_body_order_op6_first": ams,
            "patch_amp_mod_sensitivity_op1_first": list(reversed(ams)),
            "patch_ams_zeroed": bool(args.zero_ams),
            "note": [args.note, args.velocity],
            "run_frames_after_note_on": args.runframes,
            "result": {k: v for k, v in res.items() if k != "stdout"},
            "probe_lines": res["stdout"],
        }
        # Verdict.  An unknown anywhere in the AM datapath is a finding; a
        # clean control run is what makes the finding attributable.
        #
        # `exp_hsum` is combinational and its out-of-range bits are x on the
        # as-frozen RTL UNCONDITIONALLY -- even at reset, even when the
        # exp() unit is idle -- so an x there is expected in every as-frozen
        # run and is not by itself the finding.  The finding is PROPAGATION:
        # exp_pt / exp_li / w_li / op_li / tap_mix going unknown, which only
        # happens when the AM gate (`dx7_core.v:2139`, n_ams != 0) starts
        # the unit.  Widening the wires must remove the x at the source too.
        hsum_x = res.get("hsum", -1) >= 0
        prop_x = (any(res.get(k, -1) >= 0
                      for k in ("pt", "li", "wli", "mix"))
                  or res.get("opli_slot0_unknown_ops", 0) > 0
                  or res.get("unknown_samples", 0) > 0)
        #
        # Only ONE configuration is expected to show unknowns: iverilog, on
        # the as-frozen RTL, with the patch's AMS left nonzero.  Each of the
        # three other configurations removes exactly one link of the causal
        # chain and must come back clean:
        #   --zero-ams  removes the gate    (dx7_core.v:2139)
        #   --widened   removes the source  (dx7_core.v:953-961)
        #   --sim verilator                 (folds the reads to 0)
        expect_unknowns = (args.sim == "iverilog" and not args.widened
                           and not args.zero_ams)
        expect_hsum_x = args.sim == "iverilog" and not args.widened
        report["expect_unknowns"] = expect_unknowns
        problems: list[str] = []
        if expect_unknowns:
            if not hsum_x:
                problems.append(
                    "as-frozen iverilog run shows a DEFINED exp_hsum; the "
                    "out-of-range reads resolved here, contradicting #86's "
                    "recorded xcheck")
            if not prop_x:
                problems.append(
                    "as-frozen iverilog run propagated NO unknown on a "
                    "nonzero-AMS patch; the #96 hypothesis does not "
                    "reproduce here and the divergence has another cause")
        else:
            if prop_x:
                problems.append(
                    "control run (%s) still propagates unknowns into the "
                    "audio path; the attribution of the unknowns to the "
                    "out-of-range exp_t3/t4/t5 reads does not hold and "
                    "must be re-derived" % variant_label(args))
            if hsum_x and not expect_hsum_x:
                problems.append(
                    "control run (%s) still shows an unknown exp_hsum: the "
                    "x source was not removed" % variant_label(args))
        report["status"] = "FAIL" if problems else "PASS"
        report["problems"] = problems
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


if __name__ == "__main__":
    sys.exit(main())
