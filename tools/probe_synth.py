"""Shared single-module probe synthesis driver (issue #291).

One owner for the CLI/PDK/yosys setup, the six-step single-module yosys
script (read_verilog [-D<strip>] / hierarchy -check / synth / [ltp] /
dfflibmap / abc / stat -liberty), parse_ltp, the full/strip flop gates,
the strip-observability negative control and the report skeleton that
tools/h01_synth.py, tools/h04_synth.py, tools/h05_synth.py and
tools/h06_synth.py each still carried after the #119/#141/#146/#176/#179
extractions. Everything genuinely per-probe -- constants,
declared_state_bits, parse_stat, rom_representation, the flop floor and
requirement text, extra report sections, the harness key name and the
stdout summary -- stays in the tool and is passed in via ProbeSynthSpec,
so each tool's CLI flags, exit codes, report schema and exported symbols
are unchanged.

Excluded on purpose: the hierarchical H07/H08/H10 tools and the storage
probe (hierarchy-total parsing, different report schemas, --from-logs).

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool
or PDK path -- the message names what is missing). Stdlib only.
"""

import argparse
import dataclasses
import json
import os
import re
import sys

import pdk_resolve
from sha256_helpers import sha256_file
from yosys_run import run_yosys_script


def parse_ltp(log):
    """Generic-gate logic depth from the ltp pass (hint only)."""
    m = re.search(r"Longest topological path in \S+ \(length=(\d+)\)", log)
    loops = len(re.findall(r"Detected loop", log))
    return {"generic_gate_depth": int(m.group(1)) if m else None,
            "loop_warnings": loops}


def fmt_area(v):
    """None-safe area formatting for the stdout summaries: 'n/a' when the
    transcript carried no such line (issue #295 -- a full run that maps
    nothing must print its summary and exit 1, not raise TypeError from
    :.1f on None). Report values are untouched; only stdout text."""
    return "n/a" if v is None else f"{v:.1f}"


def run_probe_yosys(yosys, repo_root, rtl_rel, top, strip_define, liberty,
                    strip, ltp, log_path, check_failure):
    """The four probes' one yosys script: read_verilog (with
    -D<strip_define> on the strip control), hierarchy -check -top, synth,
    the optional ltp hint, then dfflibmap / abc / stat against `liberty`;
    the run/log/failure tail is tools/yosys_run.py's (#146)."""
    rtl = os.path.join(repo_root, rtl_rel)
    script = [
        "read_verilog" + (f" -D{strip_define}" if strip else "")
        + f" {rtl}",
        f"hierarchy -check -top {top}",
        f"synth -top {top}",
    ]
    if ltp:
        # generic-gate logic depth hint (NOT a timing number)
        script.append("ltp")
    script += [
        f"dfflibmap -liberty {liberty}",
        f"abc -liberty {liberty}",
        f"stat -liberty {liberty}",
    ]
    return run_yosys_script(yosys, script, log_path, repo_root,
                            check_failure)


@dataclasses.dataclass(frozen=True)
class ProbeSynthSpec:
    """Everything run_probe_synth_cli needs that genuinely differs per
    probe. The callables are the tool's own functions and exception
    class, so failures raise the tool's classes and parsing uses the
    tool's module name -- the #119/#146 wiring, not a shared identity."""
    description: str          # argparse description (the tool's headline)
    repo_root: str
    outdir_rel: str
    schema: str
    issue: int
    planning_id: str
    claim_boundary: str
    constraints: str          # tool_identity.constraints
    rtl_rel: str
    tb_rel: str
    top: str                  # hierarchy/synth -top
    strip_define: str         # the strip control's -D define
    use_ltp: bool             # full run appends `ltp` and records its hint
    flop_floor: int
    flop_requirement: str     # the full gate's requirement sentence
    harness_key: str          # "equivalence_harness" / "conformance_harness"
    harness_value: str
    yosys_version: object     # callable(yosys) -> str
    check_failure: object     # the tool's CheckFailure class
    parse_stat: object        # callable(log) -> dict
    rom_representation: object  # callable(log) -> dict
    declared_state: object    # callable(repo_root) -> (bits, detail, extra)
    declared_note: str
    extra_sections: dict      # per-probe report sections
    print_summary: object     # callable(gates, runs, declared, extra, json)


def run_probe_synth_cli(spec, argv=None):
    """The four probes' shared main(): CLI, PDK resolve, full/strip mapped
    runs, the flop gate + strip negative control, the report skeleton and
    the 0/1/2 exit code -- each tool's exact pre-#291 behavior."""
    ap = argparse.ArgumentParser(description=spec.description)
    ap.add_argument("--mode", choices=("both", "full", "strip"),
                    default="both")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--ciel-root", default=None)
    ap.add_argument("--liberty", default=None)
    ap.add_argument("--outdir", default=os.path.join(spec.repo_root,
                                                    spec.outdir_rel))
    args = ap.parse_args(argv)

    repo_root = spec.repo_root
    try:
        liberty, args.ciel_root = pdk_resolve.resolve_liberty(
            args.liberty, args.ciel_root)
    except pdk_resolve.PdkNotFound as e:
        print(f"COULD-NOT-RUN: {e}", file=sys.stderr)
        return 2
    ver = spec.yosys_version(args.yosys)
    declared, declared_detail, declared_extra = spec.declared_state(repo_root)

    os.makedirs(args.outdir, exist_ok=True)
    log_full = os.path.join(args.outdir, "yosys_full.log")
    log_strip = os.path.join(args.outdir, "yosys_strip.log")

    runs = {}
    gates = {}
    if args.mode in ("both", "full"):
        log = run_probe_yosys(args.yosys, repo_root, spec.rtl_rel, spec.top,
                              spec.strip_define, liberty, strip=False,
                              ltp=spec.use_ltp, log_path=log_full,
                              check_failure=spec.check_failure)
        st = spec.parse_stat(log)
        if spec.use_ltp:
            st.update(parse_ltp(log))
        st["rom_representation"] = spec.rom_representation(log)
        st["mode"] = "full"
        ok = (st["flop_total"] > spec.flop_floor
              and (st["chip_area_um2"] or 0.0) > 0.0
              and (st["seq_area_um2"] or 0.0) > 0.0
              and st["cell_total"] > 0)
        gates["full"] = {
            "requirement": spec.flop_requirement,
            "mapped_flops": st["flop_total"],
            "chip_area_um2": st["chip_area_um2"],
            "seq_area_um2": st["seq_area_um2"],
            "cell_total": st["cell_total"],
            "status": "PASS" if ok else "FAIL",
        }
        runs["full"] = st
    if args.mode in ("both", "strip"):
        log = run_probe_yosys(args.yosys, repo_root, spec.rtl_rel, spec.top,
                              spec.strip_define, liberty, strip=True,
                              ltp=False, log_path=log_strip,
                              check_failure=spec.check_failure)
        st = spec.parse_stat(log)
        st["mode"] = "strip"
        runs["strip"] = st

    if "full" in runs and "strip" in runs:
        gone = runs["strip"]["flop_total"] < runs["full"]["flop_total"]
        gates["strip_negative_control"] = {
            "requirement": "strip-observability must make the flop gate "
                           "FAIL (all state removed)",
            "mapped_flops": runs["strip"]["flop_total"],
            "status": "MET (gate fails as required)" if gone
                      else "NOT MET (gate still passes -- control failed)",
        }

    status = "PASS" if gates and all(
        g["status"] in ("PASS", "MET (gate fails as required)")
        for g in gates.values()) else "FAIL"

    report = {
        "schema": spec.schema,
        "issue": spec.issue,
        "planning_id": spec.planning_id,
        "status": status,
        "claim_boundary": spec.claim_boundary,
        "tool_identity": {
            "yosys": ver,
            "liberty": {"path": liberty,
                        "sha256": sha256_file(liberty),
                        "corner": "tt_025C_5v00",
                        "library": "gf180mcu_fd_sc_mcu7t5v0 (7t, 5 metal)"},
            "dont_use_policy": "none: *_1 drive cells allowed (declared per "
                               "D01 section 4/section 10.3)",
            "constraints": spec.constraints,
            "rtl": {p: sha256_file(os.path.join(repo_root, p))
                    for p in (spec.rtl_rel, spec.tb_rel)},
            spec.harness_key: spec.harness_value,
        },
    }
    report.update(spec.extra_sections)
    report["declared_vs_mapped_state"] = {
        **declared_extra,
        "declared_bits": declared,
        "declared_detail": declared_detail,
        "mapped_flops_full": runs.get("full", {}).get("flop_total"),
        "note": spec.declared_note,
    }
    report["gates"] = gates
    report["runs"] = runs
    out_json = os.path.join(args.outdir, "synth_report.json")
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    spec.print_summary(gates, runs, declared, declared_extra, out_json)
    return 0 if status == "PASS" else 1
