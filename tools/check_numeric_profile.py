#!/usr/bin/env python3
"""N01 checker: validate spec/numeric-profile-v1.json against recomputation,
the pinned reference trees, and spec/contract-v1.json (issue #15).

Fails when:
  - the profile is missing required sections or any decision lacks an
    error_estimate or cost_estimate (issue #15 negative control);
  - the schedule arithmetic disagrees with independent recomputation;
  - the storage totals disagree with the per-field bit widths, or the cost
    lines disagree with the family anchors they cite;
  - the full 16-note storage estimate is missing (issue #15 negative control);
  - event timing disagrees with spec/contract-v1.json (D00 DEC-014);
  - the provisional Mark I tables do not regenerate byte-identically (or their
    rounding-boundary margins no longer support the determinism argument);
  - a cited line's content does not match the pinned tree, or a cited file's
    sha256 drifts from reference/manifest.json (dexed tree).

When a pinned tree is absent the citation checks are NOT_RUN (guarded skip,
reported explicitly, never counted as a pass). Exit codes: 0 pass, 1 check
failure, 2 could-not-run. Stdlib only.
"""

import hashlib
import json
import math
import os
import re
import struct
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROFILE_PATH = os.path.join(REPO_ROOT, "spec", "numeric-profile-v1.json")
CONTRACT_PATH = os.path.join(REPO_ROOT, "spec", "contract-v1.json")
MANIFEST_PATH = os.path.join(REPO_ROOT, "reference", "manifest.json")

DEXED_ROOT_DEFAULT = "/Users/joseph/dev/dexed"
ORACLE_ROOT_DEFAULT = "/Users/joseph/dx7-oracle-marki"

SHA256_RE = re.compile(r"^[0-9a-f]{64}$")

REQUIRED_SECTIONS = (
    "block", "phase", "operator_core", "feedback", "envelope", "lfo",
    "pitch_env", "routing_gating", "output", "event_timing", "note_tail",
    "schedule", "storage", "decision_index",
)
REQUIRED_SCHEDULE_KEYS = (
    "operator_evals_per_second", "operator_evals_per_frame",
    "clock_12m288", "clock_24m576", "op_loop_only_minimum_clock_hz",
    "design_point",
)


def load_json(path):
    try:
        with open(path, "r", encoding="utf-8") as fh:
            return json.load(fh), None
    except OSError as exc:
        return None, f"cannot read {path}: {exc}"
    except json.JSONDecodeError as exc:
        return None, f"{path} is not valid JSON: {exc}"


def norm(text):
    return " ".join(str(text).split())


def recomputed_schedule(premises, block):
    """Independent recomputation of every schedule fact (from first principles)."""
    rate = premises["sample_rate_hz"]
    notes = premises["polyphony"]
    ops = premises["operators_per_note"]
    n = block["n"]
    assert n == (1 << block["lg_n"]), "block.n must equal 1 << block.lg_n"
    frames = rate / n
    evals_per_sample = notes * ops
    evals_per_frame = evals_per_sample * n
    evals_per_second = evals_per_sample * rate
    clocks = {}
    for key, hz in (("clock_12m288", 12288000.0), ("clock_24m576", 24576000.0)):
        clocks[key] = {
            "hz": hz,
            "clks_per_sample": hz / rate,
            "clks_per_frame": hz / frames,
            "op_loop_floor_share": evals_per_frame / (hz / frames),
            "all_in_clks_per_op_eval": (hz / frames) / evals_per_frame,
        }
    return {
        "frames_per_second": frames,
        "operator_evals_per_sample": evals_per_sample,
        "operator_evals_per_frame": evals_per_frame,
        "operator_evals_per_second": evals_per_second,
        "op_loop_only_minimum_clock_hz": evals_per_second,
        **clocks,
    }


def check_schedule(profile):
    facts = recomputed_schedule(profile["premises"], profile["block"])
    failures = []
    sched = profile["schedule"]
    for key in REQUIRED_SCHEDULE_KEYS:
        if key not in sched:
            failures.append(f"schedule missing required key: {key}")
    if failures:
        return failures
    pairs = (
        ("frames_per_second", "frames_per_second"),
        ("operator_evals_per_sample", "operator_evals_per_sample"),
        ("operator_evals_per_frame", "operator_evals_per_frame"),
        ("operator_evals_per_second", "operator_evals_per_second"),
        ("op_loop_only_minimum_clock_hz", "op_loop_only_minimum_clock_hz"),
    )
    for fact_key, prof_key in pairs:
        if abs(sched[prof_key] - facts[fact_key]) > 1e-6:
            failures.append(
                f"schedule.{prof_key} = {sched[prof_key]!r} but recomputation "
                f"gives {facts[fact_key]!r}")
    for key in ("clock_12m288", "clock_24m576"):
        got, want = sched[key], facts[key]
        if abs(got["hz"] - want["hz"]) > 1e-6:
            failures.append(f"schedule.{key}.hz drift")
        if abs(got["clks_per_sample"] - want["clks_per_sample"]) > 1e-6:
            failures.append(f"schedule.{key}.clks_per_sample = "
                            f"{got['clks_per_sample']} != {want['clks_per_sample']}")
        if abs(got["clks_per_frame"] - want["clks_per_frame"]) > 1e-6:
            failures.append(f"schedule.{key}.clks_per_frame = "
                            f"{got['clks_per_frame']} != {want['clks_per_frame']}")
        if abs(got["op_loop_floor_share"] - want["op_loop_floor_share"]) > 1e-6:
            failures.append(
                f"schedule.{key}.op_loop_floor_share = "
                f"{got['op_loop_floor_share']} != {want['op_loop_floor_share']}")
        if abs(got["all_in_clks_per_op_eval"]
               - want["all_in_clks_per_op_eval"]) > 1e-3:
            failures.append(
                f"schedule.{key}.all_in_clks_per_op_eval = "
                f"{got['all_in_clks_per_op_eval']} != "
                f"{want['all_in_clks_per_op_eval']}")
    return failures


def recomputed_storage_totals(storage):
    bits_per_op = sum(storage["per_operator_state_bits"].values())
    bits_per_note = sum(storage["per_note_state_bits"].values())
    ops = storage["instances"]["operators"]
    notes = storage["instances"]["notes"]
    total = bits_per_op * ops + bits_per_note * notes
    return {
        "bits_per_operator": bits_per_op,
        "bits_per_note": bits_per_note,
        "total_bits": total,
        "total_bytes": total / 8,
    }


AREA_PER_BIT_REGISTER = 118.0  # family-measured replicated-state anchor, um^2
AREA_PER_BIT_ROM = (1.9, 2.2)  # family-measured ROM-as-logic anchor, um^2
SRAM_MACRO_BYTES = 512
SRAM_MACRO_MM2 = 0.2094


def check_storage(profile):
    storage = profile["storage"]
    failures = []
    for section in ("per_operator_state_bits", "per_note_state_bits",
                    "instances", "totals", "cost_lines"):
        if section not in storage:
            failures.append(f"storage missing required section: {section}")
    if failures:
        failures.append(
            "full 16-note storage estimate missing (issue #15 negative "
            "control: a proposal without it fails review)")
        return failures
    want = recomputed_storage_totals(storage)
    totals = storage["totals"]
    for key in ("bits_per_operator", "bits_per_note", "total_bits"):
        if totals[key] != want[key]:
            failures.append(f"storage.totals.{key} = {totals[key]} but fields "
                            f"sum to {want[key]}")
    if abs(totals["total_bytes_approx"] - want["total_bytes"]) > 0.5:
        failures.append("storage.totals.total_bytes_approx disagrees with "
                        f"total_bits/8 = {want['total_bytes']}")
    reg_area = want["total_bits"] * AREA_PER_BIT_REGISTER / 1e6
    by_option = {line.get("option"): line for line in storage["cost_lines"]}
    line = by_option.get("all registers")
    if line is None:
        failures.append("storage cost line 'all registers' missing")
    else:
        if abs(line["area_mm2"] - reg_area) > 0.01:
            failures.append(
                f"'all registers' area_mm2 = {line['area_mm2']} but "
                f"{want['total_bits']} bits x {AREA_PER_BIT_REGISTER} um2/bit "
                f"= {reg_area:.2f}")
        if "family-measured" not in line.get("anchor", ""):
            failures.append("'all registers' line must cite the family-measured anchor")
        if line.get("label") != "derived total from family-measured per-bit anchor":
            failures.append("'all registers' line must be labelled derived vs family-measured")
    line = by_option.get("SRAM tiling")
    if line is None:
        failures.append("storage cost line 'SRAM tiling' missing")
    else:
        macros = math.ceil(want["total_bytes"] / SRAM_MACRO_BYTES)
        if line.get("macros_512b") != macros:
            failures.append(f"'SRAM tiling' macros_512b = {line.get('macros_512b')} "
                            f"but ceil({want['total_bytes']}/512) = {macros}")
        if abs(line["area_mm2"] - macros * SRAM_MACRO_MM2) > 0.01:
            failures.append("'SRAM tiling' area_mm2 disagrees with macro count "
                            f"x {SRAM_MACRO_MM2}")
    line = by_option.get("Mark I tables (2 x 1024 x 16 bit)")
    if line is None:
        failures.append("storage cost line 'Mark I tables (2 x 1024 x 16 bit)' missing")
    else:
        bits = profile["operator_core"]
        want_bits = (bits["sinlog_entries"] * bits["sinlog_entry_bits"]
                     + bits["sinexp_entries"] * bits["sinexp_entry_bits"])
        if line.get("bits") != want_bits:
            failures.append(f"'Mark I tables' bits = {line.get('bits')} but "
                            f"operator_core tables hold {want_bits}")
    return failures


def check_decisions(profile):
    failures = []
    decisions = profile.get("decision_index")
    if not isinstance(decisions, list) or not decisions:
        return ["decision_index missing or empty"]
    for dec in decisions:
        did = dec.get("id", "<unnamed>")
        for field in ("error_estimate", "cost_estimate"):
            value = dec.get(field)
            if not isinstance(value, str) or not value.strip():
                failures.append(
                    f"decision {did} lacks a {field} (issue #15 negative "
                    f"control: review rejects any decision lacking an error "
                    f"or cost estimate)")
        if not dec.get("citations"):
            failures.append(f"decision {did} has no citations into the pinned trees")
    return failures


def regen_sinlog_table():
    """Regenerate sinLogTable per EngineMkI.cpp:83-86 operation order."""
    table = []
    min_margin = float("inf")
    for i in range(1024):
        x = struct.unpack("f", struct.pack(
            "f", math.sin(((0.5 + i) / 1024.0) * (math.pi / 2.0))))[0]
        v = -1024.0 * math.log2(x)
        min_margin = min(min_margin, abs(v - math.floor(v) - 0.5))
        frac = v - math.floor(v)
        q = math.floor(v) + (1 if (frac > 0.5 or (frac == 0.5 and v > 0)) else 0)
        table.append(q)
    return table, min_margin


def regen_sinexp_table():
    """Regenerate sinExpTable per EngineMkI.cpp:88-92 operation order."""
    table = []
    min_margin = float("inf")
    for i in range(1024):
        fi = struct.unpack("f", struct.pack("f", i / 1024.0))[0]
        v = (2.0 ** fi - 1.0) * 4096.0
        min_margin = min(min_margin, abs(v - math.floor(v) - 0.5))
        frac = v - math.floor(v)
        q = math.floor(v) + (1 if (frac > 0.5 or (frac == 0.5 and v > 0)) else 0)
        table.append(q)
    return table, min_margin


def check_provisional_tables(profile):
    failures = []
    prov = profile.get("provisional_tables", {})
    for key, regen in (("sinlog_table", regen_sinlog_table),
                       ("sinexp_table", regen_sinexp_table)):
        spec = prov.get(key)
        if spec is None:
            failures.append(f"provisional_tables.{key} missing")
            continue
        table, margin = regen()
        digest = hashlib.sha256(struct.pack("<1024H", *table)).hexdigest()
        if digest != spec["sha256_uint16le"]:
            failures.append(
                f"provisional_tables.{key} regenerated sha256 {digest} != "
                f"pinned {spec['sha256_uint16le']}")
        if table[0] != spec["first_entry"] or table[-1] != spec["last_entry"]:
            failures.append(f"provisional_tables.{key} endpoint entries drifted")
        # The determinism argument needs the rounding margin to stay orders of
        # magnitude above double-ULP libm disagreement (~1e-12 here).
        if margin < spec["min_half_boundary_margin"] * 0.5:
            failures.append(
                f"provisional_tables.{key} half-boundary margin {margin} fell "
                f"below the recorded determinism margin "
                f"{spec['min_half_boundary_margin']}")
        if margin < 1e-6:
            failures.append(
                f"provisional_tables.{key} margin {margin} no longer supports "
                "the cross-platform determinism argument")
    return failures


def check_event_timing(profile, contract):
    failures = []
    timing = profile.get("event_timing", {})
    if timing.get("dec014_event_application") != contract.get("event_application"):
        failures.append(
            f"event_timing.dec014_event_application "
            f"({timing.get('dec014_event_application')!r}) != contract-v1 "
            f"event_application ({contract.get('event_application')!r})")
    if "explicit_rejection" not in contract.get("event_queue_overflow", ""):
        failures.append("contract-v1 event_queue_overflow wording changed; "
                        "re-check event_timing.overflow alignment")
    if contract.get("silence_floor_owner") == "N01" and "note_tail" not in profile:
        failures.append("contract-v1 delegates the silence floor to N01 but "
                        "the profile has no note_tail section")
    if timing.get("commit_granularity_samples") != 64:
        failures.append("event_timing.commit_granularity_samples must be 64 "
                        "(the oracle block policy)")
    return failures


def load_manifest_hashes():
    manifest, err = load_json(MANIFEST_PATH)
    if err:
        return None, err
    return {entry["path"]: entry["sha256"] for entry in manifest["files"]}, None


def check_citations(profile, dexed_root, oracle_root, manifest_hashes):
    """Return (failures, not_run_count). Hash checks apply to the dexed tree
    via the R01 manifest; the oracle tree's main.cpp is byte-pinned in the
    profile's premises."""
    failures = []
    not_run = 0
    roots = {
        "dexed": dexed_root,
        "oracle": oracle_root,
    }
    tree_presence = {name: bool(root and os.path.isdir(root))
                     for name, root in roots.items()}
    for dec in profile.get("decision_index", []):
        for cite in dec.get("citations", []):
            tree = cite.get("tree")
            path = cite.get("path")
            where = f"{dec.get('id')} citation {tree}:{path}:{cite.get('lines')}"
            if tree not in roots:
                failures.append(f"{where}: unknown tree {tree!r}")
                continue
            if not tree_presence[tree]:
                not_run += 1
                continue
            full = os.path.join(roots[tree], path)
            if not os.path.isfile(full):
                failures.append(f"{where}: file missing from pinned tree")
                continue
            if tree == "dexed":
                actual = hashlib.sha256(open(full, "rb").read()).hexdigest()
                expected = manifest_hashes.get(path)
                if expected is None:
                    failures.append(f"{where}: path not pinned in reference/manifest.json")
                elif actual != expected:
                    failures.append(
                        f"{where}: sha256 drift (manifest {expected}, tree {actual})")
            elif tree == "oracle" and path == "main.cpp":
                pinned = (profile.get("premises", {}).get("oracle_tree", {})
                          .get("main_cpp_sha256"))
                actual = hashlib.sha256(open(full, "rb").read()).hexdigest()
                if pinned and actual != pinned:
                    failures.append(f"{where}: oracle main.cpp sha256 drift")
            first, last = cite.get("lines"), None
            if "-" in str(first):
                first, last = str(first).split("-", 1)
            first, last = int(first), int(last or first)
            with open(full, "r", encoding="utf-8", errors="replace") as fh:
                lines = fh.readlines()
            if last > len(lines):
                failures.append(f"{where}: cited lines beyond end of file")
                continue
            window = [norm(line) for line in lines[first - 1:last]]
            expect = norm(cite.get("expect", ""))
            if expect and not any(expect in line for line in window):
                failures.append(
                    f"{where}: expected {cite.get('expect')!r} not found in "
                    f"cited lines {first}-{last} (citation rot)")
    return failures, not_run


def main(argv=None):
    profile, err = load_json(PROFILE_PATH)
    if err:
        print(f"check_numeric_profile: COULD-NOT-RUN — {err}", file=sys.stderr)
        return 2
    contract, err = load_json(CONTRACT_PATH)
    if err:
        print(f"check_numeric_profile: COULD-NOT-RUN — {err}", file=sys.stderr)
        return 2

    failures = []
    for section in REQUIRED_SECTIONS:
        if section not in profile:
            failures.append(f"profile missing required section: {section}")
    if failures:
        print("FAIL: " + failures[0])
        for f in failures:
            print(f"  {f}")
        return 1

    failures += check_schedule(profile)
    failures += check_storage(profile)
    failures += check_decisions(profile)
    failures += check_provisional_tables(profile)
    failures += check_event_timing(profile, contract)

    dexed_root = os.environ.get("DEXED_ROOT", DEXED_ROOT_DEFAULT)
    oracle_root = os.environ.get("DX7_ORACLE_ROOT", ORACLE_ROOT_DEFAULT)
    manifest_hashes, err = load_manifest_hashes()
    if err:
        print(f"check_numeric_profile: COULD-NOT-RUN — {err}", file=sys.stderr)
        return 2
    cite_failures, not_run = check_citations(
        profile, dexed_root, oracle_root, manifest_hashes)
    failures += cite_failures

    if failures:
        print(f"FAIL: {failures[0]}")
        for f in failures:
            print(f"  {f}")
        return 1

    decisions = len(profile["decision_index"])
    note = (f"citation checks NOT_RUN for {not_run} citation(s) "
            "(pinned tree unavailable; guarded skip)"
            if not_run else
            f"all {sum(len(d['citations']) for d in profile['decision_index'])} "
            "citations verified against the pinned trees")
    print(f"numeric profile check PASS: schedule + storage recomputed, "
          f"{decisions} decisions carry error/cost estimates, {note}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
