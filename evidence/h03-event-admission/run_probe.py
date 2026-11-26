"""Build and run the H03 SPI event-admission Verilator probe (issue #339).

Modes:
  --write   compile tb_event_admission.sv, run all probe scenarios, and write
            evidence/h03-event-admission/results.json
  --check   compile and re-run all probe scenarios, comparing the build
            options, source hashes and frame-by-frame results. Build parallelism
            (-j) may vary by host. The recorded Verilator version describes the
            original capture so other versions can also check the same results.
            Exits 2 on missing verilator and 3 on a failed bench build (before
            any probe case runs), so the caller can tell a bench that could
            not run from one that ran and disagreed.

Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
EVIDENCE_DIR = Path(__file__).resolve().parent
TB_PATH = EVIDENCE_DIR / "tb_event_admission.sv"
RESULTS_PATH = EVIDENCE_DIR / "results.json"
BUILD_DIR = Path("build/h03-admission")


class BuildFailure(RuntimeError):
    """The Verilator bench build failed, so no probe case has run."""

    EXIT_CODE = 3


SOURCES = (
    "rtl/dx7_core.v",
    "rtl/env_unit.v",
    "rtl/alg_router.v",
    "src/gf180_dx7/host.py",
    "evidence/h03-event-admission/tb_event_admission.sv",
)

CASES = (
    {
        "id": "burst_100_sustained",
        "plusargs": ["+burst=100"],
        "patch": "sustained_op6_l99",
        "stimulus": "16 voices pre-occupied (notes 40..55), then 100 back-to-back raw SEC=0 EA_NOTEON writes at 194-clk spacing starting at Frame 2 offset 32671 (straddling Frame 2 -> Frame 3)",
    },
    {
        "id": "burst_168_sustained",
        "plusargs": ["+burst=168"],
        "patch": "sustained_op6_l99",
        "stimulus": "16 voices pre-occupied (notes 40..55), then 168 back-to-back raw SEC=0 EA_NOTEON writes at 194-clk spacing starting at Frame 2 offset 32671 (exact EVQ_DEPTH fill)",
    },
    {
        "id": "burst_169_sustained",
        "plusargs": ["+burst=169"],
        "patch": "sustained_op6_l99",
        "stimulus": "16 voices pre-occupied (notes 40..55), then 169 back-to-back raw SEC=0 EA_NOTEON writes at 194-clk spacing starting at Frame 2 offset 32671 (168 admitted, 169th rejected at evq_full)",
    },
    {
        "id": "burst_100_reset_patch",
        "plusargs": ["+burst=100", "+reset_patch"],
        "patch": "silent_reset_patch",
        "stimulus": "16 voices pre-occupied under silent reset patch (note_silent=0xffff), then 100 raw SEC=0 EA_NOTEON writes; allocator rejects steals in 2 clk without completing strikes",
    },
    {
        "id": "phase_max_170",
        "plusargs": ["+mode=phase_max"],
        "patch": "sustained_op6_l99",
        "stimulus": "170 back-to-back raw EA_NOTEON writes starting at Frame 2 offset 32575: 169 completions land in Frame 3 at exact offsets 0, 194, ..., 32592, and the 170th lands across the boundary at Frame 4 offset 18",
    },
    {
        "id": "skid_straddle_169",
        "plusargs": ["+mode=skid_straddle"],
        "patch": "sustained_op6_l99",
        "stimulus": "169 back-to-back raw EA_NOTEON writes starting at Frame 2 offset 32574: completion #0 lands at Frame 2 offset 32767 (skid_close=1 across boundary), drains at Frame 3 offset 0 before F_EVPOP (popped in Frame 3), followed by 168 completions/drains in Frame 3 with zero overflow",
    },
    {
        "id": "sim_pop_reject_170",
        "plusargs": ["+mode=sim_pop_reject"],
        "patch": "sustained_op6_l99",
        "stimulus": "170 back-to-back raw EA_NOTEON writes starting at Frame 2 offset 32751: 168 writes fill EVQ in Frame 3 (offsets 176..32574); write #169 completes at Frame 4 offset 0 (= 32768 from Frame 3 start) and drains at Frame 4 offset 1 simultaneously with wc_pop_fire, where evq_full rejects it; write #170 at offset 194 drains at 195 after the pop freed a slot and is admitted and struck in Frame 4",
    },
    {
        "id": "carry_replenish_337",
        "plusargs": ["+mode=carry_replenish"],
        "patch": "sustained_op6_l99",
        "stimulus": "337 back-to-back raw EA_NOTEON writes starting at Frame 2 offset 32752: 168 writes fill EVQ in Frame 3; 169 writes complete in Frame 4 (offsets 1..32593, drains 2..32594) with zero rejections because wc_pop_fire at offset 1 freed space first; Frame 4 pops and completes 194 strikes (168 opening + 26 mid-pop admissions) and carries 143 into Frame 5",
    },
    {
        "id": "carry_cc123_337",
        "plusargs": ["+mode=carry_cc123"],
        "patch": "sustained_op6_l99",
        "stimulus": "Same 337-write carry_replenish sequence and timing, with the first raw note-on replaced by CC123 (all notes off). With slot 15 held the release scan wraps, allowing one additional admission to be popped in Frame 4.",
    },
)

NOT_RUN_CASES = (
    {
        "id": "clk_12m288_rtl_probe",
        "clock_hz": 12288000,
        "status": "NOT_RUN",
        "reason": "rtl/dx7_core.v hardcodes sample_clk to 0..511 (512 core clocks/sample = 32,768 clocks/frame, the 24.576 MHz @ 48 kHz design point); no 256-clock/sample 12.288 MHz @ 48 kHz RTL configuration exists, and changing RTL scheduling is out of scope for issue #339.",
    },
    {
        "id": "skid_fifo_stage_overflow_compliant_sck",
        "clock_hz": 24576000,
        "status": "NOT_RUN",
        "reason": "Under compliant SPI timing (>= 194 core clocks/transaction) and unconditional 1-write/clock skid drain, skid_cnt never exceeds 1 (depth 8), so skid_full rejection is unreachable without violating SCK <= f_core/4 (rate-violating injection is covered separately by H08).",
    },
    {
        "id": "all_patch_and_event_mix_rtl_wcet_24m576",
        "clock_hz": 24576000,
        "status": "NOT_RUN",
        "reason": "The probe exercises sustained and silent-reset patches up to 194 occupied strikes/frame; an exhaustive worst-case RTL cycle proof across all 32 algorithms, 96-op AM-sensitivity exp() paths, refresh_pending, and controller/note-off interleavings was not run, so all_admissible_frames_rtl_deadline remains NO_VERDICT.",
    },
)

FRAME_RE = re.compile(r"^FRAME\s+(\d+)\s+(.*)$")


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def parse_frames(stdout: str) -> list[dict]:
    frames = []
    for line in stdout.splitlines():
        m = FRAME_RE.match(line.strip())
        if not m:
            continue
        frame_rec: dict[str, int | str | None] = {"frame": int(m.group(1))}
        for tok in m.group(2).split():
            k, v = tok.split("=", 1)
            frame_rec[k] = int(v)
        # Identify rejection stage explicitly per frame
        if frame_rec["skid_rej"] and frame_rec["evq_rej"]:
            stage = "skid_and_evq"
        elif frame_rec["skid_rej"]:
            stage = "skid_fifo"
        elif frame_rec["evq_rej"]:
            stage = "event_queue"
        else:
            stage = "none"
        frame_rec["rejection_stage"] = stage
        frames.append(frame_rec)
    if not frames:
        raise RuntimeError(f"no FRAME lines parsed from simulator output:\n{stdout}")
    return frames


def build_and_run(jobs: int = 2) -> dict:
    ver_out = (
        subprocess.run(
            ["verilator", "--version"], capture_output=True, text=True, check=True
        )
        .stdout.strip()
        .splitlines()[0]
    )

    (REPO_ROOT / BUILD_DIR).mkdir(parents=True, exist_ok=True)
    build_cmd = [
        "verilator",
        "--binary",
        "--timing",
        "-j",
        str(jobs),
        "-Wno-fatal",
        "--timescale-override",
        "1ns/1ps",
        "--Mdir",
        str(BUILD_DIR),
        "--top-module",
        "tb_event_admission",
        "rtl/dx7_core.v",
        "rtl/env_unit.v",
        "rtl/alg_router.v",
        str(TB_PATH.relative_to(REPO_ROOT)),
    ]
    proc = subprocess.run(
        build_cmd, cwd=str(REPO_ROOT), capture_output=True, text=True, check=False
    )
    if proc.returncode != 0:
        raise BuildFailure(
            f"verilator build failed (exit {proc.returncode}):\n"
            f"{proc.stdout[-2000:]}\n{proc.stderr[-2000:]}"
        )
    binary = BUILD_DIR / "Vtb_event_admission"
    runs = []
    for case in CASES:
        cmd = [str(binary), *case["plusargs"]]
        r = subprocess.run(
            cmd, cwd=str(REPO_ROOT), capture_output=True, text=True, check=False
        )
        if r.returncode != 0:
            raise RuntimeError(
                f"case {case['id']} failed (exit {r.returncode}):\n"
                f"{r.stdout}\n{r.stderr}"
            )
        frames = parse_frames(r.stdout)
        runs.append(
            {
                "id": case["id"],
                "status": "PASS",
                "clock_hz": 24576000,
                "frame_clks": 32768,
                "patch": case["patch"],
                "plusargs": case["plusargs"],
                "reproduce_cmd": shlex.join(cmd),
                "stimulus": case["stimulus"],
                "max_busy_clks": max(int(f["busy_clks"]) for f in frames),
                "min_deadline_slack_clks": min(
                    32768 - int(f["busy_clks"]) for f in frames
                ),
                "final_sticky_overflow": int(frames[-1]["overflow"]),
                "final_sticky_overrun": int(frames[-1]["overrun"]),
                "frames": frames,
            }
        )

    return {
        "schema": "gf180-dx7.h03-event-admission-probe/1",
        "issue": 339,
        "decision_record": "docs/decision-records/0015-h03-admission-budget-reconciliation.md",
        "simulator": {
            "tool": "verilator",
            "version": ver_out,
            "build_command": shlex.join(build_cmd),
        },
        "source_sha256": {rel: sha256_file(REPO_ROOT / rel) for rel in SOURCES},
        "verdicts": {
            "exercised_cases_24m576": "PASS",
            "all_admissible_frames_rtl_deadline_24m576": "NO_VERDICT",
            "all_admissible_frames_rtl_deadline_12m288": "NO_VERDICT",
            "clk_12m288_rtl_execution": "NOT_RUN",
        },
        "exercised_runs": runs,
        "not_run_cases": list(NOT_RUN_CASES),
    }


def build_options(command: str) -> list[str]:
    """Keep the command fragments on either side of the numeric job count."""
    return re.split(r" -j [0-9]+ ", command, maxsplit=1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    grp = ap.add_mutually_exclusive_group(required=True)
    grp.add_argument("--write", action="store_true", help="write results.json")
    grp.add_argument("--check", action="store_true", help="verify results.json")
    ap.add_argument("-j", "--jobs", type=int, default=2)
    args = ap.parse_args(argv)

    if not shutil.which("verilator"):
        print("NOT_RUN: verilator not found on PATH", file=sys.stderr)
        return 2

    try:
        fresh = build_and_run(jobs=args.jobs)
    except BuildFailure as exc:
        print(f"BUILD FAILURE: {exc}", file=sys.stderr)
        print(
            "no probe case ran; the toolchain-vs-project diagnosis is in "
            "tests/test_h03_admission_probe.py",
            file=sys.stderr,
        )
        return BuildFailure.EXIT_CODE
    rendered = json.dumps(fresh, indent=2) + "\n"
    if args.write:
        RESULTS_PATH.write_text(rendered, encoding="utf-8")
        print(
            f"wrote {RESULTS_PATH.relative_to(REPO_ROOT)} ({len(fresh['exercised_runs'])} runs)"
        )
        return 0

    if not RESULTS_PATH.is_file():
        print(f"FAIL: missing {RESULTS_PATH}", file=sys.stderr)
        return 1
    committed = json.loads(RESULTS_PATH.read_text(encoding="utf-8"))
    for key in ("source_sha256", "verdicts", "exercised_runs", "not_run_cases"):
        if committed.get(key) != fresh.get(key):
            print(f"FAIL: results.json mismatch in {key}", file=sys.stderr)
            return 1
    simulator = committed.get("simulator", {})
    if simulator.get("tool") != "verilator":
        print("FAIL: simulator.tool mismatch", file=sys.stderr)
        return 1
    if build_options(simulator.get("build_command", "")) != build_options(
        fresh["simulator"]["build_command"]
    ):
        print("FAIL: simulator.build_command mismatch", file=sys.stderr)
        return 1
    print(
        f"h03 admission probe check PASS ({len(fresh['exercised_runs'])} runs reproduced)"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
