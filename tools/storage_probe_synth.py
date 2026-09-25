#!/usr/bin/env python3
"""H02 full-state storage probe: mapped synthesis + macro analysis (issue #21).

Runs yosys over rtl/storage_probe.v against the ciel gf180mcu 7t liberty
(D01-recorded path), maps flops with dfflibmap, maps combinational logic with
abc, and applies the flop-count gate: the mapped flop total MUST equal N01's
bit accounting (spec/numeric-profile-v1.json "storage": 96 x 325 operator +
16 x 446 note + 444 patch-shared + 1 registered observability flop = 38,781).
A mismatch means synthesis removed state -- FAIL, never a pass.

Modes:
  full   the probe as committed (observability live); the gate must PASS.
  strip  -DSTORAGE_PROBE_STRIP_OBSERVABILITY: every read/observability path
         removed. The gate must then FAIL (state deleted) -- that is the
         negative control proving observability is load-bearing. If the
         flop count still matches, the control has failed to demonstrate.

Also inventories the foundry gf180mcu_fd_ip_sram macros present in the local
ciel tree (Liberty + LEF, measured attributes) and derives macro tiling
options for the patch store and the full state image, with flop-vs-macro
comparisons per state class. Byte/bit accounting is recomputed from the spec
JSON and cross-checked against the RTL's own field parameters before any
synthesis runs.

Exit codes: 0 pass, 1 gate/check failure, 2 could-not-run (missing tool or
PDK path -- the message names what is missing). Stdlib only.
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DEFAULT_CIEL = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
                "gf180mcu/versions/"
                "54435919abffb937387ec956209f9cf5fd2dfbee")
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")
SRAM_LIB_REL = "gf180mcuD/libs.ref/gf180mcu_fd_ip_sram/lib"
SRAM_LEF_REL = "gf180mcuD/libs.ref/gf180mcu_fd_ip_sram/lef"

OBS_FLOPS = 1  # the registered XOR-reduction bit, documented separately
RTL_REL = "rtl/storage_probe.v"
TB_FILES = ("rtl/storage_probe.v", "rtl/storage_probe_tb.v")
SPEC_REL = "spec/numeric-profile-v1.json"
OUTDIR_REL = "evidence/h02-storage-probe"

SRAM_MACROS = (  # (macro suffix, capacity in bytes) -- the only 4 hardened
    ("sram64x8m8wm1", 64),
    ("sram128x8m8wm1", 128),
    ("sram256x8m8wm1", 256),
    ("sram512x8m8wm1", 512),
)


class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


# ---------------------------------------------------------------------------
# N01 bit accounting, recomputed from the machine-readable profile
# ---------------------------------------------------------------------------

def load_accounting(repo_root):
    spec_path = os.path.join(repo_root, SPEC_REL)
    if not os.path.isfile(spec_path):
        raise CouldNotRun(f"numeric profile not found: {spec_path}")
    with open(spec_path, "r", encoding="utf-8") as f:
        st = json.load(f)["storage"]

    def fields(dct):
        return [(k, int(v)) for k, v in dct.items()]

    op_fields = fields(st["per_operator_runtime_state_bits"])
    note_fields = fields(st["per_note_runtime_state_bits"])
    patch_fields = fields(st["patch_shared_state_bits"])
    ops = int(st["instances"]["operators"])
    notes = int(st["instances"]["notes"])

    op_w = sum(w for _, w in op_fields)
    note_w = sum(w for _, w in note_fields)
    patch_w = sum(w for _, w in patch_fields)
    replicated = ops * op_w + notes * note_w
    with_patch = replicated + patch_w
    totals = st["totals"]
    if totals["bits_per_operator"] != op_w or \
            totals["bits_per_note"] != note_w or \
            totals["patch_shared_bits"] != patch_w or \
            totals["total_bits"] != replicated:
        raise CheckFailure(
            "spec totals disagree with recomputed field sums "
            f"({op_w}/{note_w}/{patch_w}/{replicated} vs "
            f"{totals['bits_per_operator']}/{totals['bits_per_note']}/"
            f"{totals['patch_shared_bits']}/{totals['total_bits']})")
    return {
        "source": SPEC_REL,
        "section": "DECISIONS-NUMERIC-v1.md section 6 (N01 rev 1.1)",
        "op_fields": op_fields,
        "note_fields": note_fields,
        "patch_fields": patch_fields,
        "num_ops": ops,
        "num_notes": notes,
        "op_w": op_w,
        "note_w": note_w,
        "patch_w": patch_w,
        "replicated_bits": replicated,
        "patch_bits": patch_w,
        "state_bits_with_patch": with_patch,
        "obs_flops": OBS_FLOPS,
        "expected_mapped_flops": with_patch + OBS_FLOPS,
        "runtime_bytes": replicated // 8,
        "state_with_patch_bytes_ceil": -(-with_patch // 8),
        "patch_bytes_ceil": -(-patch_w // 8),
        "patch_bytes_exact": patch_w / 8.0,
    }


def parse_rtl_field_params(repo_root):
    """Pull the RTL's own field-width parameters (simple integer forms)."""
    rtl_path = os.path.join(repo_root, RTL_REL)
    if not os.path.isfile(rtl_path):
        raise CouldNotRun(f"probe RTL not found: {rtl_path}")
    with open(rtl_path, "r", encoding="utf-8") as f:
        text = f.read()
    pairs = re.findall(r"parameter\s+(W_\w+|NUM_OPS|NUM_NOTES)\s*=\s*(\d+)",
                       text)
    params = {name: int(val) for name, val in pairs}
    if not params:
        raise CheckFailure(f"no field parameters parsed from {RTL_REL}")
    return params


# JSON field key (spec/numeric-profile-v1.json storage.*) -> RTL parameter
# name, in both documents' order. Kept explicit: the mapping itself is part
# of the audit surface.
FIELD_PARAM_NAMES = {
    "phase": "W_OP_PHASE", "freq": "W_OP_FREQ", "gain_out": "W_OP_GAIN_OUT",
    "level_in": "W_OP_LEVEL_IN", "env_level": "W_OP_ENV_LEVEL",
    "env_targetlevel": "W_OP_ENV_TARGET", "env_inc": "W_OP_ENV_INC",
    "env_staticcount": "W_OP_ENV_STATIC",
    "env_effective_outlevel": "W_OP_ENV_OUTLEVEL",
    "env_effective_rate_scaling": "W_OP_ENV_RATE_SC",
    "env_ix": "W_OP_ENV_IX", "env_rising": "W_OP_ENV_RISING",
    "env_down": "W_OP_ENV_DOWN",
    "basepitch_6": "W_N_BASEPITCH_6", "fb_buf_2": "W_N_FB_BUF_2",
    "pitchenv_level": "W_N_PENV_LEVEL",
    "pitchenv_targetlevel": "W_N_PENV_TARGET",
    "pitchenv_inc": "W_N_PENV_INC", "pitchenv_ix": "W_N_PENV_IX",
    "pitchenv_rising": "W_N_PENV_RISING", "pitchenv_down": "W_N_PENV_DOWN",
    "noteLogFreq": "W_N_NOTELOGFREQ", "playingMidiNote": "W_N_MIDI_NOTE",
    "midiChannel": "W_N_MIDI_CH", "mpePitchBend": "W_N_MPE_BEND",
    "mpePressure": "W_N_MPE_PRESSURE", "mpeTimbre": "W_N_MPE_TIMBRE",
    "voice_flags_keydown_sustained_live": "W_N_VOICE_FLAGS",
    "steal_rotation_age": "W_N_STEAL_AGE",
    "env_rates_4_x6ops": "W_P_ENV_RATES",
    "env_levels_4_x6ops": "W_P_ENV_LEVELS",
    "pitchenv_rates_4": "W_P_PENV_RATES",
    "pitchenv_levels_4": "W_P_PENV_LEVELS",
    "opmode_6": "W_P_OPMODE_6", "ampmodsens_6": "W_P_AMPMODSENS_6",
    "algorithm": "W_P_ALGORITHM", "fb_depth": "W_P_FB_DEPTH",
    "mod_depths_and_sens": "W_P_MODDEPTH_SENS",
}


def check_rtl_matches_accounting(acc, params):
    """The RTL parameter block must mirror the JSON field list in order."""
    problems = []
    for group in ("op_fields", "note_fields", "patch_fields"):
        for key, w in acc[group]:
            name = FIELD_PARAM_NAMES[key]
            if params.get(name) != w:
                problems.append(f"{name} ({key}): RTL {params.get(name)} "
                                f"!= N01 {w}")
    if params.get("NUM_OPS") != acc["num_ops"]:
        problems.append(f"NUM_OPS: RTL {params.get('NUM_OPS')} "
                        f"!= N01 {acc['num_ops']}")
    if params.get("NUM_NOTES") != acc["num_notes"]:
        problems.append(f"NUM_NOTES: RTL {params.get('NUM_NOTES')} "
                        f"!= N01 {acc['num_notes']}")
    if problems:
        raise CheckFailure("RTL parameters drift from the N01 JSON "
                           "classification: " + "; ".join(problems))


# ---------------------------------------------------------------------------
# yosys
# ---------------------------------------------------------------------------

def yosys_version(yosys):
    try:
        out = subprocess.run([yosys, "-V"], capture_output=True, text=True)
    except OSError:
        raise CouldNotRun(f"yosys binary not found: {yosys}")
    if out.returncode != 0 or not out.stdout:
        raise CouldNotRun(f"yosys -V failed: {out.stderr.strip()[:200]}")
    return out.stdout.strip()


def run_yosys(yosys, repo_root, liberty, strip, abc, log_path):
    rtl = os.path.join(repo_root, RTL_REL)
    script = [
        "read_verilog" + (" -DSTORAGE_PROBE_STRIP_OBSERVABILITY" if strip
                          else "") + f" {rtl}",
        "hierarchy -check -top storage_probe",
        "synth -top storage_probe -nofsm",
        f"dfflibmap -liberty {liberty}",
    ]
    if abc:
        script.append(f"abc -liberty {liberty}")
    script.append(f"stat -liberty {liberty}")
    # -nofsm: the 38k-flop state image is not a protocol FSM and the FSM
    # pass only burns time on it. dfflibmap+abc+stat are the mapped record.
    proc = subprocess.run([yosys, "-p", "; ".join(script)],
                          capture_output=True, text=True)
    log = (proc.stdout or "") + (proc.stderr or "")
    with open(log_path, "w", encoding="utf-8") as f:
        f.write(f"# yosys script: {'; '.join(script)}\n" + log)
    if proc.returncode != 0:
        raise CheckFailure(
            f"yosys failed (rc={proc.returncode}); log at {log_path}: "
            + log.strip().splitlines()[-1][:300] if log.strip() else "")
    return log


HIER_MARK = "=== design hierarchy ==="
MODULE_AREA_RE = r"Chip area for module .*?: ([\d.]+)"
TOP_AREA_RE = r"Chip area for top module"


def assert_single_module_stat(log):
    """Assert the `stat` transcript really is single-module (issue #95).

    parse_stat reports the transcript's ONE `Chip area for module` block as
    the design total. That is correct only while storage_probe is a flat,
    single-module design -- which the committed
    evidence/h02-storage-probe/yosys_full.log is (exactly one `Chip area for
    module` line, no `=== design hierarchy ===` section; re-verified at
    implementation time). If storage_probe ever acquires submodules, yosys
    emits one block PER module plus a hierarchy section whose `Chip area for
    top module` line is the whole-design total, and taking a per-module block
    would silently under-report -- exactly the bug #82 (h07) and #94 (h08)
    had to fix. This assertion makes that drift FAIL loudly instead of
    shipping a wrong number; the fix when it fires is #94's hierarchy-total
    parser, never a looser regex or a relaxed assertion.

    Returns the per-module area strings found (0 or 1). An EMPTY list is
    legitimate, not an error: the strip-observability control maps nothing,
    so yosys prints no area line at all (chip_area_um2 stays None).
    """
    if HIER_MARK in log or re.search(TOP_AREA_RE, log):
        raise CheckFailure(
            "stat transcript is HIERARCHICAL ('=== design hierarchy ===' "
            "and/or a 'Chip area for top module' line present): storage_probe "
            "is no longer a single-module design, so a per-module 'Chip area "
            "for module' block is NOT the whole-design total. Refusing to "
            "report one as the design area (issues #82/#94/#95) -- parse the "
            "hierarchy total instead.")
    areas = re.findall(MODULE_AREA_RE, log)
    if len(areas) > 1:
        raise CheckFailure(
            f"stat transcript has {len(areas)} 'Chip area for module' blocks; "
            "storage_probe is expected to be a single-module design (exactly "
            "one). Refusing to report the first block as the design area "
            "(issues #82/#94/#95).")
    return areas


def parse_stat(log):
    """Extract mapped cell counts / areas from a `stat -liberty` transcript.

    Single-module shape is asserted first (issue #95): the one per-module
    area block IS the design total only because storage_probe has no
    submodules.
    """
    areas = assert_single_module_stat(log)
    dff = {}
    for m in re.finditer(r"^\s+(\d+)\s+[\d.]+(?:E\+\d+)?\s+"
                         r"(\S*?(\w*dff\w*)_\d+)\s*$",
                         log, re.M):
        dff[m.group(2)] = dff.get(m.group(2), 0) + int(m.group(1))
    seq = re.search(r"of which used for sequential elements: ([\d.]+)", log)
    mapped = re.search(r"mapped (\d+) \$_DFF\S* cells", log)
    # chip/seq area lines are absent when the mapped design has no cells
    # at all (the strip negative control) -- optional, never an error.
    return {
        "dff_cells": dff,
        "dff_total": sum(dff.values()),
        "dff_area_um2": float(seq.group(1)) if seq else None,
        "chip_area_um2": float(areas[0]) if areas else None,
        "dfflibmap_mapped": int(mapped.group(1)) if mapped else None,
    }


# ---------------------------------------------------------------------------
# iverilog smoke bench
# ---------------------------------------------------------------------------

def run_smoke(iverilog, vvp, repo_root):
    if not shutil.which(iverilog):
        return {"status": "NOT_RUN",
                "reason": f"iverilog not found on PATH: {iverilog}"}
    tmp = tempfile.mkdtemp(prefix="h02_smoke_")
    vvp_out = os.path.join(tmp, "storage_probe_tb.vvp")
    srcs = [os.path.join(repo_root, s) for s in TB_FILES]
    try:
        c = subprocess.run([iverilog, "-g2012", "-o", vvp_out] + srcs,
                           capture_output=True, text=True)
        if c.returncode != 0:
            return {"status": "FAIL",
                    "reason": "iverilog compile failed: "
                              + (c.stderr or c.stdout)[-400:]}
        r = subprocess.run([vvp, vvp_out], capture_output=True, text=True,
                           timeout=300)
        out = r.stdout + r.stderr
        fails = [ln for ln in out.splitlines() if ln.startswith("FAIL")]
        passes = [ln for ln in out.splitlines()
                  if ln.startswith("PASS storage_probe_tb")]
        if passes and not fails:
            return {"status": "PASS", "summary": passes[0]}
        return {"status": "FAIL",
                "reason": "; ".join(fails) or "no PASS summary line",
                "output_tail": out[-400:]}
    except subprocess.TimeoutExpired:
        return {"status": "FAIL", "reason": "smoke bench timed out"}
    finally:
        try:
            os.remove(vvp_out)
            os.rmdir(tmp)
        except OSError:
            pass


# ---------------------------------------------------------------------------
# foundry SRAM macro inventory + tiling derivation
# ---------------------------------------------------------------------------

def parse_lef_size(path):
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    m = re.search(r"MACRO\s+(\S+)\s+.*?SIZE\s+([\d.]+)\s+BY\s+([\d.]+)\s*;",
                  text, re.S)
    if not m:
        raise CheckFailure(f"no MACRO/SIZE record in {path}")
    return m.group(1), float(m.group(2)), float(m.group(3))


def parse_liberty_area(path, cell_name):
    """The `area` attribute inside `cell(<cell_name>) { ... }` (measured)."""
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    m = re.search(r"cell\s*\(\s*%s\s*\)\s*\{" % re.escape(cell_name), text)
    if not m:
        return None
    m2 = re.search(r"^\s*area\s*:\s*([\d.]+)\s*;", text[m.end():m.end() + 50000],
                   re.M)
    return float(m2.group(1)) if m2 else None


def macro_inventory(ciel_root):
    """Measured macro attributes from the local ciel tree (D01 section 5)."""
    lib_dir = os.path.join(ciel_root, SRAM_LIB_REL)
    lef_dir = os.path.join(ciel_root, SRAM_LEF_REL)
    if not os.path.isdir(lef_dir):
        raise CouldNotRun(f"SRAM LEF directory not found: {lef_dir}")
    inv = []
    for suffix, cap in SRAM_MACROS:
        lef_path = os.path.join(lef_dir, f"gf180mcu_fd_ip_sram__{suffix}.lef")
        if not os.path.isfile(lef_path):
            raise CouldNotRun(f"SRAM LEF not found: {lef_path}")
        name, w, h = parse_lef_size(lef_path)
        lib_path = os.path.join(lib_dir,
                                f"gf180mcu_fd_ip_sram__{suffix}"
                                "__tt_025C_5v00.lib")
        entry = {
            "macro": name,
            "capacity_bytes_measured": cap,
            "lef_size_um": {"width": w, "height": h},
            "lef_area_um2_derived": round(w * h, 4),
            "lef_source": lef_path,
            "liberty_area_um2_measured":
                parse_liberty_area(lib_path, name) if os.path.isfile(lib_path)
                else None,
            "liberty_source": lib_path if os.path.isfile(lib_path) else None,
        }
        if entry["liberty_area_um2_measured"] is not None:
            cross = abs(entry["liberty_area_um2_measured"]
                        - entry["lef_area_um2_derived"])
            entry["lef_vs_liberty_area_delta_um2"] = round(cross, 4)
        inv.append(entry)
    return inv


def min_area_tiling(need_bytes, inv):
    """Minimum-area macro mix reaching >= need_bytes (exact DP, derived)."""
    caps = [e["capacity_bytes_measured"] for e in inv]
    areas = [e["lef_area_um2_derived"] for e in inv]
    names = [e["macro"] for e in inv]
    limit = need_bytes + max(caps)
    inf = float("inf")
    best = [inf] * (limit + 1)
    choice = [None] * (limit + 1)
    best[0] = 0.0
    for b in range(1, limit + 1):
        for i, cap in enumerate(caps):
            if b - cap >= 0 and best[b - cap] + areas[i] < best[b]:
                best[b] = best[b - cap] + areas[i]
                choice[b] = i
    target = min(range(need_bytes, limit + 1), key=lambda b: best[b])
    counts = [0] * len(caps)
    b = target
    while b > 0:
        counts[choice[b]] += 1
        b -= caps[choice[b]]
    return {
        "bytes_needed": need_bytes,
        "mix": {names[i]: counts[i] for i in range(len(names))
                if counts[i]},
        "macro_count": sum(counts),
        "bytes_capacity": sum(caps[i] * counts[i] for i in range(len(caps))),
        "area_um2_derived": round(best[target], 4),
        "label": "derived (exact DP over measured LEF areas)",
    }


def same_size_tilings(need_bytes, inv):
    out = []
    for e in inv:
        n = -(-need_bytes // e["capacity_bytes_measured"])
        out.append({
            "macro": e["macro"],
            "macro_count_derived": n,
            "bytes_capacity_derived": n * e["capacity_bytes_measured"],
            "area_um2_derived": round(n * e["lef_area_um2_derived"], 4),
            "label": "derived from measured LEF area",
        })
    return out


def macro_analysis(acc, ciel_root):
    inv = macro_inventory(ciel_root)
    runtime_flops = acc["replicated_bits"]
    patch_flops = acc["patch_bits"]
    return {
        "label_note": "LEF SIZE / liberty area = measured; counts, mixes and "
                      "comparisons = derived. Per-note and per-operator "
                      "runtime classes stay in flops regardless of macro "
                      "area: 1RW macros cannot serve the per-slot read of "
                      "one op bank + one note bank and the same-cycle "
                      "writeback at the assumed 1 eval/cycle access, and "
                      "banking 38,336 random-access bits across 512-byte "
                      "macros serializes the schedule (see report).",
        "macros_measured": inv,
        "runtime_state_4792B": {
            "same_size_options": same_size_tilings(acc["runtime_bytes"], inv),
            "min_area_mix": min_area_tiling(acc["runtime_bytes"], inv),
        },
        "state_with_patch_4848B": {
            "min_area_mix": min_area_tiling(
                acc["state_with_patch_bytes_ceil"], inv),
        },
        "patch_store_56B": {
            "same_size_options": same_size_tilings(
                acc["patch_bytes_ceil"], inv),
            "min_area_mix": min_area_tiling(acc["patch_bytes_ceil"], inv),
        },
        "flop_equivalents": {
            "runtime_state_bits": runtime_flops,
            "patch_bits": patch_flops,
        },
    }


def flop_vs_macro_verdicts(acc, stat_full, macro):
    """Per-class flop-vs-macro comparison (mapped flops vs measured LEF)."""
    dff_area = stat_full["dff_area_um2"] or 0.0
    per_flop = dff_area / stat_full["dff_total"] if stat_full["dff_total"] else 0.0
    patch_macro = macro["patch_store_56B"]["min_area_mix"]
    runtime_macro = macro["state_with_patch_4848B"]["min_area_mix"]
    patch_flop_area = acc["patch_bits"] * per_flop
    runtime_flop_area = acc["replicated_bits"] * per_flop
    return {
        "per_mapped_flop_um2_measured": round(per_flop, 4),
        "patch_shared_444b": {
            "mapped_flop_area_um2_measured": round(patch_flop_area, 4),
            "min_macro_area_um2_derived": patch_macro["area_um2_derived"],
            "min_macro_mix": patch_macro["mix"],
            "verdict": "flops" if patch_flop_area
                       < patch_macro["area_um2_derived"] else "macro",
        },
        "runtime_state_38336b": {
            "mapped_flop_area_um2_measured": round(runtime_flop_area, 4),
            "min_macro_area_um2_derived": runtime_macro["area_um2_derived"],
            "min_macro_mix": runtime_macro["mix"],
            "port_rule": "flops for the schedule (1 eval/cycle random "
                         "access + same-slot writeback; 1RW macros cannot); "
                         "macro area shown for the area-only comparison the "
                         "issue asks for",
        },
    }


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--mode", choices=("both", "full", "strip"),
                    default="both")
    ap.add_argument("--fast", action="store_true",
                    help="skip abc (flop count + dff area unchanged; "
                         "chip area then covers mapped flops only)")
    ap.add_argument("--yosys", default="yosys")
    ap.add_argument("--iverilog", default="iverilog")
    ap.add_argument("--vvp", default="vvp")
    ap.add_argument("--ciel-root", default=DEFAULT_CIEL)
    ap.add_argument("--liberty", default=None)
    ap.add_argument("--outdir", default=os.path.join(REPO_ROOT, OUTDIR_REL))
    ap.add_argument("--skip-smoke", action="store_true")
    ap.add_argument("--no-macros", action="store_true")
    args = ap.parse_args(argv)

    repo_root = REPO_ROOT
    liberty = args.liberty or os.path.join(args.ciel_root, SC_LIB_REL)
    if not os.path.isfile(liberty):
        print(f"COULD-NOT-RUN: 7t liberty not found at {liberty} "
              f"(ciel root {args.ciel_root})", file=sys.stderr)
        return 2
    ver = yosys_version(args.yosys)

    acc = load_accounting(repo_root)
    params = parse_rtl_field_params(repo_root)
    check_rtl_matches_accounting(acc, params)

    os.makedirs(args.outdir, exist_ok=True)
    log_full = os.path.join(args.outdir, "yosys_full.log")
    log_strip = os.path.join(args.outdir, "yosys_strip.log")

    runs = {}
    gates = {}
    if args.mode in ("both", "full"):
        log = run_yosys(args.yosys, repo_root, liberty, strip=False,
                        abc=not args.fast, log_path=log_full)
        st = parse_stat(log)
        ok = st["dff_total"] == acc["expected_mapped_flops"]
        gates["full"] = {
            "requirement": f"mapped flops == {acc['expected_mapped_flops']} "
                           f"({acc['replicated_bits']} replicated + "
                           f"{acc['patch_bits']} patch + "
                           f"{acc['obs_flops']} obs, documented separately)",
            "mapped_flops": st["dff_total"],
            "status": "PASS" if ok else "FAIL",
        }
        st["mode"] = "full"
        st["abc_mapped"] = not args.fast
        runs["full"] = st
    if args.mode in ("both", "strip"):
        log = run_yosys(args.yosys, repo_root, liberty, strip=True,
                        abc=not args.fast, log_path=log_strip)
        st = parse_stat(log)
        gone = st["dff_total"] != acc["expected_mapped_flops"]
        gates["strip_negative_control"] = {
            "requirement": "strip-observability must make the flop-count "
                           "gate FAIL (state removed)",
            "mapped_flops": st["dff_total"],
            "status": "MET (gate fails as required)" if gone
                      else "NOT MET (gate still passes -- control failed)",
        }
        st["mode"] = "strip"
        st["abc_mapped"] = not args.fast
        runs["strip"] = st

    smoke = {"status": "NOT_RUN", "reason": "--skip-smoke"} \
        if args.skip_smoke else run_smoke(args.iverilog, args.vvp, repo_root)

    macro = None
    verdicts = None
    if not args.no_macros:
        try:
            macro = macro_analysis(acc, args.ciel_root)
            if "full" in runs:
                verdicts = flop_vs_macro_verdicts(acc, runs["full"], macro)
        except CouldNotRun as exc:
            macro = {"status": "NOT_RUN", "reason": str(exc)}

    report = {
        "schema": "h02-storage-probe-report/1",
        "issue": 21,
        "planning_id": "H02",
        "status": "PASS" if all(
            g["status"] in ("PASS", "MET (gate fails as required)")
            for g in gates.values()) and gates else "FAIL",
        "claim_boundary": "mapped storage cost of the probe only; no "
                          "place-and-route, no timing closure, no "
                          "full-chip fit, no functional-fidelity claim",
        "tool_identity": {
            "yosys": ver,
            "liberty": {"path": liberty,
                        "sha256": sha256_file(liberty),
                        "corner": "tt_025C_5v00",
                        "library": "gf180mcu_fd_sc_mcu7t5v0 (7t, 5 metal)"},
            "dont_use_policy": "none: *_1 drive cells allowed (declared per "
                               "D01 section 4/section 10.3)",
            "constraints": "no SDC: storage probe, no clock defined; "
                           "synth -nofsm; dfflibmap + abc -liberty",
            "rtl": {p: sha256_file(os.path.join(repo_root, p))
                    for p in TB_FILES},
        },
        "accounting_recomputed": acc,
        "gates": gates,
        "runs": runs,
        "smoke_bench": smoke,
        "macro_analysis": macro,
        "flop_vs_macro_verdicts": verdicts,
    }
    out_json = os.path.join(args.outdir, "storage_synth_report.json")
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    for name, g in gates.items():
        print(f"gate {name}: {g['status']} "
              f"(mapped flops {g['mapped_flops']})")
    print(f"smoke bench: {smoke['status']}")
    if verdicts:
        pv = verdicts["patch_shared_444b"]
        print(f"patch store verdict: {pv['verdict']} "
              f"(flops {pv['mapped_flop_area_um2_measured']} um2 vs macro "
              f"{pv['min_macro_area_um2_derived']} um2)")
    if "full" in runs:
        st = runs["full"]
        per_bit = (st["chip_area_um2"] / acc["state_bits_with_patch"]
                   if st["chip_area_um2"] else 0.0)
        print(f"full mapped: dff {st['dff_total']} "
              f"({st['dff_area_um2']:.1f} um2), chip "
              f"{st['chip_area_um2']:.1f} um2, "
              f"{per_bit:.2f} um2/state-bit all-in")
    print(f"report: {out_json}")
    return 0 if report["status"] == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
