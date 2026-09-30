#!/usr/bin/env python3
"""H10 evidence-bundle assembler (issue #32).

Builds the hash-pinned machine-readable bundle that tools/h10_report.py
consumes (evidence/h10-gf180/bundle.json) from COMMITTED evidence only:

  evidence/h10-gf180/policy-synth/ tools/h10_synth.py output: mapped
                                   synthesis under both DONT_USE policies
                                   (synth_report.json + trimmed yosys logs)
  evidence/h10-gf180/orfs-synth/   the pinned ORFS flow's own synthesis
                                   stage (1_synth.json, synth_stat.txt,
                                   trimmed 1_2 log), imported from an ORFS
                                   work tree with --import-orfs-work
  evidence/h10-gf180/sta-synth/    asic/orfs/sta-synth.sh logs: synthesis-
                                   stage STA at tt/ss/ff (ideal clock, no
                                   parasitics)
  evidence/h10-gf180/orfs/<variant>/  per-stage ORFS reports for
                                   floorplan/place/cts/route/corner STA,
                                   when such a run exists (imported the
                                   same way)

Rules this tool enforces:
  - every number in the bundle traces to a file under evidence/ whose
    sha256 is recorded next to it; h10_report.py re-verifies them (a
    drift exits 4). Nothing is pinned from the git-ignored work tree.
  - a stage with no evidence file is recorded NOT_RUN with the reason; a
    stage is never invented, a number is never carried forward from
    another stage, and a status is never upgraded.
  - a route stage's DRC count must be read from the report; a missing
    count is recorded as None (the fit gate then refuses), never 0.

Usage:
  tools/h10_collect.py [--import-orfs-work DIR] [--out PATH]
Stdlib only.
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EV_REL = "evidence/h10-gf180"
DESIGN = "dx7core"
PLATFORM = "gf180"
RTL_RELS = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]
ORFS_IMAGE = ("openroad/orfs:26Q3-296-gda37dce1c@sha256:"
              "ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2")

# D01 constraint inputs (docs/PHYSICAL-CONSTRAINTS.md section 1), with the
# two-slot die of the sibling calibration runs (gf180-parasynth
# docs/pnr-synth-top.md section 4.1: die 3,464,960 um^2, core
# 3,382,070 um^2, both fixed inputs there).
PACKAGING_BUDGETS = {
    "quarterslot": {
        "die_um_w": 1314.88, "die_um_h": 1317.12,
        "die_area_um2": 1731850.0,
        "core_area_um2": 1673400.0,
        "source": "D01 (docs/PHYSICAL-CONSTRAINTS.md section 1): "
                  "wafer.space quarter-slot seat inside the default pad "
                  "ring; this repo has not purchased a seat",
    },
    "twoslot": {
        "die_um_w": 1860.88, "die_um_h": 1862.00,
        "die_area_um2": 3464960.0,
        "core_area_um2": 3382070.0,
        "source": "gf180-parasynth docs/pnr-synth-top.md section 4.1 "
                  "(the sibling's fixed two-slot die; D01 section 6 quotes "
                  "its 3.4650 mm^2)",
    },
}

SIBLING = "2AMLogic/gf180-parasynth @ be870e81103bb4b4e000c3414270702cde181491"
SIB_EV = "pnr/orfs/evidence/synth_top"
FAMILY_CONTEXT = {
    "sibling_joined": {
        "design": "synth_top joined d1e5068 (parasynth drum engine chip)",
        "instances": 59289,
        "cell_area_um2": 1979380.0,
        "of_quarterslot_pct": 118.285,
        "twoslot_util_pct": 60.1,
        "drc_errors": 0,
        "ss_setup_wns_ns": 10.173,
        "ss_implied_fmax_mhz": 14.0,
        "clock_mhz": 12.288,
        "clock_period_ns": 81.38,
        "policy": "stock ORFS gf180 DONT_USE_CELLS = *_1, same pinned image",
        "provenance": (
            f"{SIBLING}: {SIB_EV}/joined-d1e5068/1_synth.json "
            "(sha256 87bfbe32d386eb8c74d6786b4dc567ea82524b8fbf4182daf2ec0eb2"
            "b59cfcbb), sta_corners.log (sha256 86529b731c4c23f7c08ff6519c92"
            "2df0c29d929229b4fab325e42506eed520f5), "
            f"{SIB_EV}/quarterslot-does-not-fit/2_1_floorplan.json (sha256 "
            "4ef83a89886b730a3e84f20d409c53f0865b0605c6681d82be0d73b7872ab3ae)"
            "; docs/pnr-synth-top.md sections 4-4.1. The issue names this "
            "the gf180-monosynth calibration: gf180-monosynth @ 395af2b "
            "carries byte-identical copies of these three files"),
    },
    "sibling_placeholder": {
        "design": "synth_top placeholder 2a88c35",
        "instances": 28810,
        "cell_area_um2": 925387.0,
        "quarterslot_routed_util_pct": 56.7,
        "provenance": f"{SIBLING}: docs/pnr-synth-top.md sections 3 and 6",
    },
    "dont_use_policy": {
        "sibling_pct": [29.1, 31.1],
        "sibling_detail": "placeholder 925,387 vs 717,049 um^2 (+29.1 %); "
                          "joined 1,979,380 vs 1,510,153 um^2 (+31.1 %); "
                          "ORFS synth re-run with DONT_USE_CELLS= (empty) "
                          "and nothing else different",
        "provenance": f"{SIBLING}: docs/pnr-synth-top.md section 6; "
                      f"{SIB_EV}/joined-d1e5068/1_synth_x1.json (sha256 "
                      "ba709998a82f9a864c84e12215cebfd28b078578c45b0d46eca84"
                      "63104cc7c9b)",
        "policy_source": "pinned image flow/platforms/gf180/config.mk line "
                         "24: 'export DONT_USE_CELLS = *_1' (plain '=', "
                         "overridable only on the make command line); file "
                         "sha256 1ab8a25c54e376c5aa2ef5f983f969cdd943cc7477"
                         "85f9a641bb47467e64a6d3",
    },
}

# DR-0012 (issue #98): this bundle's runs were all made on the DR-0011 core
# pin, whose mapped netlist is missing the whole exp() datapath (see FINDINGS
# below and docs/EXP-RANGE-PROOF-86.md).  Set to None -- and delete this
# comment -- only when the H10 runs have been redone on the current frozen
# pin, at which point the bundle stops carrying a supersession banner.
SUPERSEDED_BY = {
    "status": "STALE",
    "decision_record": "docs/decision-records/"
                       "0012-exp-term-width-core-refreeze.md",
    "issue": 98,
    "core_pin_measured":
        "34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58",
    "core_pin_current":
        "f33cecbd138aea2a869cb502dca9273b342c8f97be51c9a1e587f2eb4aba89fa",
    "reason": "Every run in this bundle synthesised the DR-0011 core, whose "
              "out-of-range `exp_t3`/`exp_t4`/`exp_t5` reads make the whole "
              "`exp_hsum` adder undef, so the pinned flow deleted the exp() "
              "datapath (0 mapped flops for `exp_y` .. `exp_pt_q`). Every "
              "area, instance count and utilisation figure here is a LOWER "
              "BOUND for the current frozen core, not its size. The "
              "direction of the non-fit verdicts cannot reverse by adding "
              "logic, so no conclusion here is withdrawn -- but no number "
              "may be cited as the refrozen core's size until these runs "
              "are redone on the current pin.",
}

UNACCOUNTED = [
    "pad ring and I/O cells: the quarter-slot core area is already inside "
    "the default pad ring (D01); the 9-pad H08 chassis (rtl/synth_top.v) "
    "is NOT in the synthesised unit -- only dx7_core is",
    "placement density headroom: a routable design sits well below 100 % "
    "cell utilisation (sibling: 56.7-60.1 % routed); no utilisation "
    "headroom is assumed or subtracted in the area-alone verdicts",
    "clock tree, timing-repair buffers, hold fixing, taps, endcaps, "
    "fillers (sibling: +2.6 % std-cell area over synthesis from place/"
    "resize/CTS; fillers/taps fill the rest of the rows)",
    "power distribution network straps/rings and their routing blockage",
    "routing congestion (5-metal gf180mcu; RT_MAX_LAYER=Metal4 on TT tiles)",
    "IR drop / electromigration, antenna, KLayout signoff DRC, LVS (the "
    "ORFS gf180 platform has no signoff DRC deck and no LVS; H11)",
    "any SRAM macro alternative for state storage (none requested: all "
    "state is flops, ROM tables are logic; D01 section 5 records no usable "
    "KB-scale macro)",
]

FINDINGS = [
    "**PR-A row H diagnosed (not a flow defect).** "
    "`evidence/h10-legality/run-attempts.md` row H recorded "
    "`do-3_5_place_dp` aborting with `[ERROR ORD-0007] ...3_4_place_resized"
    ".odb does not exist`. In the pinned image's `flow/Makefile` the phony "
    "`do-3_5_place_dp` target depends only on `$(OBJECTS_DIR)/copyright."
    "txt` (Makefile lines 403 and 507); the dependency on "
    "`3_4_place_resized.odb` exists only on the file-target rule. Invoking "
    "the `do-` step directly, without stages 3_1-3_4, produces exactly that "
    "error. It was an invocation-order error, not a flow or driver bug, and "
    "it is moot here: no placement stage can run on either fixed die "
    "(section 1).",
    "**`exp_t3`/`exp_t4`/`exp_t5` out-of-range selects (DR-0011 Context 5) "
    "-- CORRECTED, and the reason this report is STALE.** This run's "
    "canonicalize step reported three out-of-bounds warnings "
    "(`rtl/dx7_core.v:959-961`, 15/15/16 MSBs set to undef), and an earlier "
    "revision of this bullet concluded that the flow's `setundef -zero` "
    "(image `flow/scripts/synth.tcl` line 252) therefore ties those bits to "
    "constant 0. **That was wrong.** Issue #86 measured what the flow "
    "actually does: the yosys frontend makes the missing bits constant `x` "
    "inside the addends, and the first `opt_expr` (inside `proc`) folds each "
    "`$add` with an `x` operand bit to an ALL-x result -- IEEE 4-state `+`, "
    "applied even with `-keepdc` -- so `exp_hsum` is `x` before `setundef "
    "-zero` makes the whole sum the constant 0. The netlist behind every "
    "number in this report has **0** mapped flops for the exp() datapath "
    "(`exp_y` .. `exp_pt_q`): the AM/LFO exp() unit was deleted, and the "
    "mapped core applies no AM attenuation where the frozen model does. "
    "See `docs/EXP-RANGE-PROOF-86.md`. DR-0012 (issue #98) re-freezes the "
    "core with the three wires widened to `[85:0]`/`[85:0]`/`[81:0]`, which "
    "changes no simulated value and keeps the unit; every area and instance "
    "figure here is therefore a LOWER BOUND for the current frozen core.",
    "**H07 synth report area mislabel (#82).** `evidence/h07-core/"
    "synth_report.json` records `chip_area_um2 = 1,158,826 um^2`. That is "
    "`alg_router`'s module-local area; the same committed log's "
    "hierarchical top is 24,082,966 um^2 (`*_1` allowed, host yosys 0.57). "
    "It independently agrees with this run's `x1_allowed` 23,996,886 um^2 "
    "(within 0.4 %, different yosys build). The H07 number must not be "
    "read as the core's size.",
    "**Tool identity: the pinned image ships two yosys builds.** The image "
    "digest alone does not identify the yosys that ran; the invocation "
    "does. `/usr/local/bin/yosys` (on `PATH`) self-reports `Yosys 0.67 "
    "(git sha1 2d1509d1b)`, sha256 `21cf7fad1ccc...`. "
    "`/OpenROAD-flow-scripts/tools/install/yosys/bin/yosys` self-reports "
    "`Yosys 0.68+post (git sha1 UNKNOWN)`, sha256 `5cd52bc790d3...`. The "
    "ORFS flow selects the second: image `flow/scripts/variables.mk` lines "
    "117-120 default `YOSYS_EXE` to `tools/install/yosys/bin/yosys` outside "
    "a nix shell. Both runs of this report used the 0.68+post binary: the "
    "flow run by that default, and the policy A/B because it was passed "
    "that path explicitly (`--yosys`). Each run's banner in its committed "
    "log matches that binary's self-report; path and sha256 per run are in "
    "`bundle.tools.yosys_runs`. DR-0011's `0.67 (2d1509d1b)` is correct "
    "for its witnesses A and C, which called `yosys` from `PATH` (their "
    "committed logs print the 0.67 banner). Its flow `do-yosys` rows D/E/F "
    "ran through the flow Makefile and so, by the same default, ran "
    "0.68+post. Their committed evidence carries no banner, so this is "
    "inferred from the flow, not observed (addendum in `run-attempts.md` "
    "and DR-0011). OpenROAD self-reports `26Q3-1260-g06a5a02279`.",
]

# Yosys binaries in the pinned image (verified in the image by digest with
# `sha256sum` and `-V`, 2026-09-25). The flow's selection rule is recorded
# with the hash of the file that states it.
YOSYS_FLOW = {
    "path": "/OpenROAD-flow-scripts/tools/install/yosys/bin/yosys",
    "sha256": "5cd52bc790d39b1e59a88112e9132ef8de338c2a26e4b0df8222d937"
              "e65bab92",
    "self_reported": "Yosys 0.68+post (git sha1 UNKNOWN, Release, GNU "
                     "/usr/local/bin/wrapped-cc/g++ 11.4.0)",
}
YOSYS_PATH = {
    "path": "/usr/local/bin/yosys",
    "sha256": "21cf7fad1cccb4dea0e5c8fdb0085a7c766a3b401dbdd59d4316e6b6"
              "e820afd5",
    "self_reported": "Yosys 0.67 (git sha1 2d1509d1b, Release, GNU "
                     "/usr/local/bin/wrapped-cc/g++ 11.4.0)",
}
VARIABLES_MK = {
    "path": "/OpenROAD-flow-scripts/flow/scripts/variables.mk",
    "sha256": "3cba7ede6cdd606381df39dc3f4060ddc7927d4312a9cd79694490f1"
              "f19af46b",
    "lines": "117-120",
    "rule": "ifneq (${IN_NIX_SHELL},) YOSYS_EXE ?= $(shell command -v "
            "yosys) else YOSYS_EXE ?= $(abspath $(FLOW_HOME)/../tools/"
            "install/yosys/bin/yosys)",
}


def yosys_runs(ev):
    """Per-run yosys identity: binary path + sha256, how it was selected,
    and whether the banner in the run's committed evidence matches the
    binary's self-report. A mismatch or missing banner is recorded as such
    (banner_matches False / None); it is never assumed."""
    def banner_in_log(p):
        if not os.path.isfile(p):
            return None
        with open(p, errors="replace") as f:
            for line in f:
                s = line.strip()
                if s.startswith("Yosys ") and "(git sha1" in s:
                    return s
        return None

    flow_log = os.path.join(ev, "orfs-synth", "1_2_yosys.trimmed.log")
    rep = os.path.join(ev, "policy-synth", "synth_report.json")
    runs = {}
    b = banner_in_log(flow_log)
    runs["orfs_flow_synth"] = dict(
        YOSYS_FLOW,
        selected_by="ORFS flow default YOSYS_EXE (image %s lines %s, sha256 "
                    "%s); run-orfs.sh does not set YOSYS_EXE"
                    % (VARIABLES_MK["path"], VARIABLES_MK["lines"],
                       VARIABLES_MK["sha256"]),
        banner_evidence=rel(flow_log),
        banner=b,
        banner_matches=None if b is None else b == YOSYS_FLOW["self_reported"])
    r = load_json(rep)
    b = (r or {}).get("yosys")
    runs["policy_ab"] = dict(
        YOSYS_FLOW,
        selected_by="explicit: tools/h10_synth.py --yosys %s "
                    "(asic/orfs/README.md Run step 4)" % YOSYS_FLOW["path"],
        banner_evidence=rel(rep),
        banner=b,
        banner_matches=None if b is None else b == YOSYS_FLOW["self_reported"])
    return runs

UNPROVED_FIXED = [
    "synthesis-stage timing is ideal-clock, zero-wire-load, pre-resize; the "
    "floorplan-stage timing is wire-load-model, after the floorplan "
    "resizer, on a die the cells cannot occupy -- neither is post-route "
    "timing, and neither establishes closure or non-closure of a routed "
    "design",
    "the area-alone 'does not fit' verdicts need no place/route (cell area "
    "alone exceeds the core); they say nothing about what WOULD fit",
    "no architecture alternative (time-multiplexed envelopes, SRAM-backed "
    "state, reduced polyphony) was synthesised; their area is unmeasured",
    "no silicon, board, FPGA or original-hardware claim of any kind",
    "musical usefulness is a listening question, not a mapped-area question",
]


def unproved(bundle):
    ran = sorted({f"{v}.{k}" for v, vst in (bundle.get("stages") or {})
                  .items() for k in vst})
    later = [x for x in ran if not x.endswith(".floorplan")]
    head = [
        "no placement, CTS or routing ran for any die"
        + ("" if not later else f" except {later}")
        + "; no post-route timing exists at any corner",
    ]
    return head + UNPROVED_FIXED


STAGE_JSON = {
    "floorplan": "2_1_floorplan.json",
    "place": "3_5_place_dp.json",
    "cts": "4_1_cts.json",
    "route": "5_2_route.json",
}
FINISH_JSON = "6_report.json"
CORNER_RE = re.compile(
    r"=== CORNER (\w+) setup_wns_ns=(\S+) setup_tns_ns=(\S+) "
    r"hold_wns_ns=(\S+) hold_tns_ns=(\S+) implied_min_period_ns=(\S+) "
    r"implied_fmax_mhz=(\S+)")


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def rel(path):
    return os.path.relpath(path, REPO_ROOT)


def load_json(path):
    try:
        with open(path) as f:
            return json.load(f)
    except (FileNotFoundError, json.JSONDecodeError):
        return None


def pick(d, suffix):
    """ORFS metric JSONs key values like 'synth__design__instance__area';
    return the first value whose key ends with the suffix and carries no
    ':'-qualified class."""
    if not d:
        return None
    for k, v in d.items():
        if k.endswith(suffix) and ":" not in k.split("__")[-1]:
            return v
    return None


def pin(*paths):
    return {rel(p): sha256_file(p) for p in paths if os.path.isfile(p)}


# ---------------------------------------------------------------- synth ---
def policy_synth(ev):
    d = os.path.join(ev, "policy-synth")
    rep = os.path.join(d, "synth_report.json")
    r = load_json(rep)
    if r is None:
        return {"status": "NOT_RUN", "note": "no policy-synth report"}
    runs = r.get("runs") or {}
    stock, x1 = runs.get("orfs_stock") or {}, runs.get("x1_allowed") or {}
    if stock.get("status") != "PASS":
        return {"status": "FAIL" if stock else "NOT_RUN",
                "note": stock.get("reason", "orfs_stock policy run absent"),
                "evidence": pin(rep)}
    logs = [os.path.join(REPO_ROOT, x["log"]) for x in (stock, x1)
            if x.get("log")]
    policy_areas = {p: runs[p]["stdcell_area_um2"] for p in runs
                    if runs[p].get("status") == "PASS"}
    return {
        "status": "PASS",
        "mapped": True,
        "method": "tools/h10_synth.py: %s (the pinned image's "
                  "tools/install/yosys/bin/yosys; bundle.tools.yosys_runs) "
                  "synth + dfflibmap + abc against the gf180mcu 7t "
                  "tt_025C_5v00 liberty (sha256 %s), hierarchy kept; "
                  "check -assert clean" % (r.get("yosys", "?"),
                                          r.get("liberty_sha256", "?")),
        "policy": "orfs_stock (dont_use *_1)",
        "mapped_stdcell_area_um2": stock["stdcell_area_um2"],
        "seq_area_um2": stock.get("seq_area_um2"),
        "instance_count": stock["instance_count"],
        "flop_count": stock["flop_count"],
        "policy_areas_um2": policy_areas,
        "policy_instances": {p: runs[p]["instance_count"] for p in
                             policy_areas},
        "policy_flops": {p: runs[p]["flop_count"] for p in policy_areas},
        "dont_use_effect": r.get("dont_use_effect"),
        "module_local_area_um2": stock.get("module_local_area_um2"),
        "top_submodule_instances": stock.get("top_submodule_instances"),
        "netlists": {p: {"path": runs[p]["netlist"],
                         "sha256": runs[p]["netlist_sha256"],
                         "bytes": runs[p]["netlist_bytes"],
                         "committed": False} for p in policy_areas},
        "rtl": r.get("rtl"),
        "evidence": pin(rep, *logs),
    }


def orfs_synth(ev):
    d = os.path.join(ev, "orfs-synth")
    sj = os.path.join(d, "1_synth.json")
    m = load_json(sj)
    if m is None:
        return {"status": "NOT_RUN",
                "note": "no committed ORFS synth-stage metrics"}
    area = pick(m, "synth__design__instance__area__stdcell") or \
        pick(m, "synth__design__instance__area")
    inst = pick(m, "synth__design__instance__count__stdcell") or \
        pick(m, "synth__design__instance__count")
    errs = pick(m, "synth__flow__errors__count")
    files = [sj] + [os.path.join(d, f) for f in sorted(os.listdir(d))
                    if f != "1_synth.json" and not f.startswith(".")]
    meta = load_json(os.path.join(d, "run.json")) or {}
    # exact figures from the flow's own stat report (the JSON rounds area)
    stat_p = os.path.join(d, "synth_stat.txt")
    stat = open(stat_p).read() if os.path.isfile(stat_p) else ""
    exact = re.search(r"Chip area for (?:top )?module '\\?\w+': ([\d.]+)",
                      stat)
    seq = re.search(r"sequential elements: ([\d.]+)", stat)
    flops = sum(int(m_.group(1)) for m_ in re.finditer(
        r"^\s+(\d+)\s+\S+(?:\s+\d+\s+\S+)?\s+gf180mcu_\w+__(?:dff|sdff|latq)"
        r"\w*\s*$", stat, re.M))
    chk_p = os.path.join(d, "synth_check.txt")
    chk = open(chk_p).read() if os.path.isfile(chk_p) else ""
    check_ok = "Found and reported 0 problems" in chk
    if exact:
        area = float(exact.group(1))
    return {
        "status": "PASS" if (area and inst and not errs and check_ok)
        else "FAIL",
        "mapped": True,
        "method": f"pinned ORFS flow {ORFS_IMAGE}, platform gf180 "
                  "TRACK_OPTION=7t CORNER=TC, stock DONT_USE_CELLS=*_1, "
                  "yosys 0.68+post at the image's tools/install/yosys/bin/"
                  "yosys (flow default; bundle.tools.yosys_runs); flattened "
                  "netlist",
        "policy": "orfs_stock (DONT_USE_CELLS = *_1, image platform config)",
        "mapped_stdcell_area_um2": area,
        "seq_area_um2": float(seq.group(1)) if seq else None,
        "instance_count": inst,
        "flop_count": flops or None,
        "check": "0 problems" if check_ok else "check report missing or "
                 "not clean",
        "flow_errors": errs,
        "run": meta,
        "evidence": pin(*files),
    }


def sta_log(path):
    if not os.path.isfile(path):
        return None
    corners = {}
    text = open(path).read()
    for m in CORNER_RE.finditer(text):
        c, sw, stn, hw, htn, mp, fm = m.groups()
        corners[c] = {
            "setup_wns_ns": float(sw), "setup_tns_ns": float(stn),
            "hold_wns_ns": float(hw), "hold_tns_ns": float(htn),
            "implied_min_period_ns": float(mp),
            "implied_fmax_mhz": float(fm),
        }
    done = "=== DONE" in text
    st = {
        "status": "PASS" if (corners and done) else "FAIL",
        "corners": corners,
        "evidence": pin(path),
    }
    sec = text.split("=== WORST SETUP PATH corner ss", 1)
    if len(sec) == 2:
        sp = re.search(r"Startpoint: (\S+)", sec[1])
        ep = re.search(r"Endpoint: (\S+)", sec[1])
        if sp and ep:
            st["worst_path_ss"] = f"{sp.group(1)} -> {ep.group(1)}"
    return st


def synth_sta(ev):
    d = os.path.join(ev, "sta-synth")
    out = {}
    if os.path.isdir(d):
        for f in sorted(os.listdir(d)):
            if f.endswith(".log"):
                st = sta_log(os.path.join(d, f))
                st["label"] = ("synthesis stage: unplaced netlist, ideal "
                               "clock, no wire parasitics, no resizing")
                out[f[:-4]] = st
    return out


# ------------------------------------------------------------ variants ---
def variant_stages(ev, variant):
    d = os.path.join(ev, "orfs", variant)
    vst = {}
    if not os.path.isdir(d):
        return vst
    for name, fname in STAGE_JSON.items():
        path = os.path.join(d, fname)
        m = load_json(path)
        if m is None:
            continue
        extra = [os.path.join(d, f) for f in sorted(os.listdir(d))
                 if f.startswith(fname.split(".")[0]) and f != fname]
        fmax = pick(m, "timing__fmax")
        st = {"status": "PASS", "evidence": pin(path, *extra),
              "timing_corner": "tt (flow CORNER=TC, tt_025C_5v00)",
              "setup_wns_ns": pick(m, "timing__setup__ws"),
              "setup_tns_ns": pick(m, "timing__setup__tns"),
              "hold_wns_ns": pick(m, "timing__hold__ws"),
              "fmax_mhz": fmax / 1e6 if isinstance(fmax, (int, float))
              else None}
        if name == "floorplan":
            st["core_area_um2"] = pick(m, "design__core__area")
            st["die_area_um2"] = pick(m, "design__die__area")
            st["instance_area_um2"] = pick(m, "design__instance__area")
            u = pick(m, "design__instance__utilization")
            st["utilisation_pct"] = 100.0 * u if u is not None else None
            if u is None or u >= 1.0:
                # the stage ran and measured, but nothing can be placed
                st["status"] = "FAIL"
                st["note"] = (f"measured utilisation "
                              f"{st['utilisation_pct']:.1f} % of the fixed "
                              "core -- cells exceed the core; no placement "
                              "is possible" if u is not None else
                              "no utilisation in the floorplan report")
        if name == "place":
            # convergence is only claimed when the report says so
            st["converged"] = pick(m, "design__instance__count") is not None
        if name == "route":
            drc = pick(m, "route__drc_errors")
            st["drc_errors"] = drc  # None stays None: the gate refuses
            st["converged"] = drc is not None
            st["wirelength_um"] = pick(m, "route__wirelength")
        vst[name] = st
    sta = sta_log(os.path.join(d, "sta_corners.log"))
    if sta is not None:
        vst["sta"] = sta
    return vst


# ------------------------------------------------------------ import ----
def import_orfs_work(work, ev, variant="natdie"):
    """Copy the ORFS synth-stage artifacts of one work tree into the
    committed evidence layout (small files only; the netlist and odb are
    recorded by hash, not copied)."""
    L = os.path.join(work, "logs", PLATFORM, DESIGN, variant)
    R = os.path.join(work, "reports", PLATFORM, DESIGN, variant)
    S = os.path.join(work, "results", PLATFORM, DESIGN, variant)
    dst = os.path.join(ev, "orfs-synth")
    os.makedirs(dst, exist_ok=True)
    copied = []
    for src, name in [(os.path.join(L, "1_synth.json"), "1_synth.json"),
                      (os.path.join(R, "synth_stat.txt"), "synth_stat.txt"),
                      (os.path.join(R, "synth_check.txt"),
                       "synth_check.txt")]:
        if os.path.isfile(src):
            shutil.copyfile(src, os.path.join(dst, name))
            copied.append(name)
    log = os.path.join(L, "1_2_yosys.log")
    if os.path.isfile(log):
        # trimmed: warnings/errors + the final stat; full log by hash
        text = open(log, errors="replace").read()
        keep = [ln for ln in text.splitlines()
                if ln.startswith(("Warning", "ERROR", "Error"))]
        at = text.rfind("Printing statistics.")
        with open(os.path.join(dst, "1_2_yosys.trimmed.log"), "w") as f:
            f.write(f"# trimmed copy of {rel(log) if log.startswith(REPO_ROOT) else log}"
                    f" (sha256 {sha256_file(log)}, "
                    f"{os.path.getsize(log)} B, not committed)\n")
            f.write("\n".join(keep) + "\n# ---- final stat ----\n")
            f.write(text[at:] if at >= 0 else "")
        copied.append("1_2_yosys.trimmed.log")
    big = {}
    for name in ("1_2_yosys.v", "1_synth.odb"):
        p = os.path.join(S, name)
        if os.path.isfile(p):
            big[name] = {"sha256": sha256_file(p),
                         "bytes": os.path.getsize(p), "committed": False}
    with open(os.path.join(dst, "run.json"), "w") as f:
        json.dump({"image": ORFS_IMAGE, "flow_variant": variant,
                   "results_not_committed": big,
                   "rtl": {r: sha256_file(os.path.join(REPO_ROOT, r))
                           for r in RTL_RELS}},
                  f, indent=1, sort_keys=True)
        f.write("\n")
    # fixed-die floorplans (and any later stage reports) per variant
    for v in ("quarterslot", "twoslot", "natdie"):
        Lv = os.path.join(work, "logs", PLATFORM, DESIGN, v)
        fp = os.path.join(Lv, STAGE_JSON["floorplan"])
        if not os.path.isfile(fp):
            continue
        vd = os.path.join(ev, "orfs", v)
        os.makedirs(vd, exist_ok=True)
        shutil.copyfile(fp, os.path.join(vd, STAGE_JSON["floorplan"]))
        flog = os.path.join(Lv, "2_1_floorplan.log")
        if os.path.isfile(flog):
            shutil.copyfile(flog, os.path.join(vd, "2_1_floorplan.log"))
        copied.append(f"orfs/{v}/{STAGE_JSON['floorplan']}")
    return copied


def primary_synth(orfs, pol):
    """The fit gate's synth stage: the pinned ORFS flow's own synthesis
    (the netlist the flow floorplans) when it PASSed, else the policy run.
    policy_areas_um2 carries every measured mapped area (each a real
    mapped result) so the area-alone gate can use the most favourable."""
    areas = {}
    if orfs.get("status") == "PASS":
        areas["orfs_flow_stock"] = orfs["mapped_stdcell_area_um2"]
    if pol.get("status") == "PASS":
        for p, a in (pol.get("policy_areas_um2") or {}).items():
            areas[f"yosys_{p}"] = a
    if orfs.get("status") == "PASS":
        prim = dict(orfs)
        prim["source"] = "orfs_synth"
    elif pol.get("status") == "PASS":
        prim = dict(pol)
        prim["source"] = "policy_synth"
    else:
        return {"status": "NOT_RUN", "note": "no mapped synthesis result"}
    prim["policy_areas_um2"] = areas
    return prim


# ---------------------------------------------------------- acceptance ---
def acceptance_rows(bundle):
    s = bundle.get("synth") or {}
    ssta = bundle.get("synth_sta") or {}
    rows = []
    rows.append((
        "Results are mapped (real cells), not estimated; tools/PDK/"
        "constraints recorded",
        "PASS" if s.get("status") == "PASS" and s.get("mapped") else
        "NOT_RUN",
        f"primary: {s.get('source', 'n/a')} ({s.get('method', 'n/a')}); "
        "policy-synth mapped netlists under both policies; identities in "
        "bundle.tools; results are mapped cells, placement/routing NOT_RUN"))
    fps = {v: vst["floorplan"] for v, vst in
           (bundle.get("stages") or {}).items() if vst.get("floorplan")}
    later = [f"{v}.{k}" for v, vst in (bundle.get("stages") or {}).items()
             for k in ("place", "cts", "route", "sta")
             if (vst.get(k) or {}).get("status") == "PASS"]
    synth_ok = any(v.get("status") == "PASS" for v in ssta.values())
    rows.append((
        "Worst-case timing vs target clock reported per stage",
        "NO_VERDICT" if synth_ok else "NOT_RUN",
        ("synthesis stage (ideal clock) at ss/tt/ff" if synth_ok else
         "no synthesis-stage STA")
        + (f"; floorplan stage at tt on {sorted(fps)}" if fps else "")
        + ("; place/CTS/route/post-route: NOT_RUN" if not later else
           f"; later stages: {later}")
        + " -- reported for every stage that ran, but post-route timing "
          "(the deciding rows) does not exist"))
    rows.append((
        "Die/pad/margin budget stated with unaccounted items listed "
        "explicitly",
        "PASS" if bundle.get("unaccounted") and
        bundle.get("packaging_budgets") else "FAIL",
        "report section 3"))
    rows.append((
        "16-note requirement retained unless an owner decision record says "
        "otherwise",
        "PASS",
        "the synthesised unit is the 16-note core (96 env_unit instances = "
        "16 notes x 6 operators); no DR revises the goal; the area finding "
        "is escalated to the owner, not absorbed"))
    rows.append((
        "Negative control: 'fits' derived from estimates rather than mapped "
        "results fails review",
        "PASS",
        "tools/h10_report.py fit gate; live tests tests/test_h10_report.py "
        "(missing route, estimated synth, DRC>0, missing DRC count, "
        "negative slack, fabricated evidence all refuse)"))
    fx = bundle.get("family_context") or {}
    rows.append((
        "Contextualized against the family's measured quarter-slot data "
        "points; DONT_USE_CELLS policy difference stated with its measured "
        "area effect",
        "PASS" if fx.get("sibling_joined") and
        (bundle.get("policy_synth") or {}).get("dont_use_effect")
        else "NO_VERDICT",
        "report section 5: sibling numbers hash-pinned to their evidence "
        "files; this core's own policy A/B measured in policy-synth"))
    return rows


def measured_rtl(ev):
    """RTL hashes as recorded BY THE RUNS in this evidence tree.

    Never a re-hash of the working tree (issue #98): the bundle must say
    what was synthesised, not what happens to be checked out when it is
    collected.  Re-hashing silently re-stamped a stale bundle with a newer
    core's hashes, which would turn a STALE result into a false provenance
    claim.  `rtl_worktree_at_collect` keeps the working-tree hashes beside
    it so the drift is visible instead of hidden.
    """
    for rel in ("orfs-synth/run.json", "policy-synth/synth_report.json"):
        d = load_json(os.path.join(ev, *rel.split("/")))
        if isinstance(d, dict) and isinstance(d.get("rtl"), dict):
            return d["rtl"]
    return None


def build_bundle():
    ev = os.path.join(REPO_ROOT, EV_REL)
    b = {
        "design": "dx7_core",
        "target_clock_mhz": 24.576,
        "clock_period_ns": 40.692,
        "rtl": measured_rtl(ev),
        "rtl_worktree_at_collect": {
            r: sha256_file(os.path.join(REPO_ROOT, r)) for r in RTL_RELS},
        "tools": {
            "orfs_image": ORFS_IMAGE,
            "pdk": "gf180mcuD, gf180mcu_fd_sc_mcu7t5v0 (7-track, 5 V)",
            "yosys_self_reported": YOSYS_FLOW["self_reported"] + ", "
                                   "in-image at " + YOSYS_FLOW["path"],
            "yosys_runs": yosys_runs(ev),
            "yosys_not_used": dict(
                YOSYS_PATH,
                note="also in the pinned image, first on PATH; used by no "
                     "H10 PR-B run; used by DR-0011 witnesses A and C "
                     "(`sh -c 'yosys ...'`, their logs print this banner)"),
            "yosys_flow_selection": VARIABLES_MK,
            "openroad_self_reported": "26Q3-1260-g06a5a02279, in-image",
            "liberty_tt_sha256": "330aa9439255e3870ad7adfb987cd4d7bcfa7b6f"
                                 "9fb0cd0a712400e9bd80e38a",
            "platform_config_sha256": "1ab8a25c54e376c5aa2ef5f983f969cdd943"
                                      "cc747785f9a641bb47467e64a6d3",
            "host": "Apple Silicon macOS, Docker 29.8.0, linux/amd64 "
                    "emulation (flow synth 35:56 wall)",
            "sdc": rel(os.path.join(REPO_ROOT, "asic", "orfs", "dx7core",
                                    "constraint.sdc")),
        },
        "packaging_budgets": PACKAGING_BUDGETS,
        "policy_synth": policy_synth(ev),
        "orfs_synth": orfs_synth(ev),
        "synth_sta": synth_sta(ev),
        "stages": {v: variant_stages(ev, v)
                   for v in ("quarterslot", "twoslot", "natdie")},
        "family_context": FAMILY_CONTEXT,
        "unaccounted": UNACCOUNTED,
        "findings": FINDINGS,
        "superseded_by": SUPERSEDED_BY,
    }
    b["synth"] = primary_synth(b["orfs_synth"], b["policy_synth"])
    b["unproved"] = unproved(b)
    b["acceptance_rows"] = acceptance_rows(b)
    return b


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--import-orfs-work", default=None,
                    help="ORFS WORK_HOME to import synth-stage artifacts "
                         "from (natdie variant) before building the bundle")
    ap.add_argument("--out", default=os.path.join(REPO_ROOT, EV_REL,
                                                  "bundle.json"))
    args = ap.parse_args(argv)
    ev = os.path.join(REPO_ROOT, EV_REL)
    if args.import_orfs_work:
        if not os.path.isdir(args.import_orfs_work):
            print(f"work tree not found: {args.import_orfs_work}",
                  file=sys.stderr)
            return 2
        print("imported:", import_orfs_work(args.import_orfs_work, ev))
    bundle = build_bundle()
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w") as f:
        json.dump(bundle, f, indent=1, sort_keys=True)
        f.write("\n")
    print(f"bundle: {args.out}")
    for k in ("synth", "orfs_synth"):
        print(f"{k}: {bundle[k].get('status')}")
    print("synth_sta:", {k: v["status"] for k, v in
                         bundle["synth_sta"].items()})
    return 0


if __name__ == "__main__":
    sys.exit(main())
