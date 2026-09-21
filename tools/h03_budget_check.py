#!/usr/bin/env python3
"""H03 budget checker: recompute the core-contract worst-case budget from
first principles and the MEASURED inputs, and fail on drift (issue #25).

Fails when:
  - the frame arithmetic (6,144 op evals/frame, 750 frames/s, both clock
    targets) disagrees with independent recomputation from the premises;
  - any budget row's clocks disagree with unit x count, any required row is
    missing, or the totals / utilizations / verdicts disagree with the
    recomputation (the recorded 12.288 MHz FAIL and 24.576 MHz PASS verdicts
    are enforced -- a budget that "closes" only by removing worst-burst rows
    fails here);
  - the measured anchors drift: the op-eval unit is not the H01-measured
    1 clk/eval, the H01/H02 evidence artifacts' sha256 or their recorded
    measured constants (throughput, mapped flops) disagree, the N08 bundle
    hash drifts, or the numeric-profile hash drifts;
  - the frozen sr_multiplier disagrees with its cited formula evaluated at
    the premise sample rate (env.cc:47-49);
  - the queue-depth / drain-count derivation arithmetic is inconsistent;
  - a DEC-id citation (budget rows, interface blocks, modules) does not exist
    in spec/contract-v1.json.

Exit codes: 0 pass, 1 check failure, 2 could-not-run. Stdlib only.
"""

import hashlib
import json
import os
import re
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC_PATH = os.path.join(REPO_ROOT, "spec", "core-contract-v1.json")
CONTRACT_PATH = os.path.join(REPO_ROOT, "spec", "contract-v1.json")
PROFILE_PATH = os.path.join(REPO_ROOT, "spec", "numeric-profile-v1.json")

MEASURED_CLKS_PER_EVAL = 1          # H01 (evidence/h01-operator-probe)
MEASURED_H02_MAPPED_FLOPS = 38781   # H02 (38,336 replicated + 444 patch + 1 obs)
SR_MULTIPLIER_48K = 15414067        # floor((44100/48000) * 2^24), env.cc:47-49

CLOCKS = (("12m288", 12288000.0), ("24m576", 24576000.0))
CLOCK_KEYS = {name: f"clock_{name}" for name, _ in CLOCKS}


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def load_json(path):
    try:
        with open(path, "r", encoding="utf-8") as fh:
            return json.load(fh), None
    except OSError as exc:
        return None, f"cannot read {path}: {exc}"
    except json.JSONDecodeError as exc:
        return None, f"{path} is not valid JSON: {exc}"


def recomputed_premises(premises):
    """First-principles frame arithmetic (independent of every stored number)."""
    rate = premises["sample_rate_hz"]
    notes = premises["polyphony"]
    ops = premises["operators_per_note"]
    n = premises["frame_samples"]
    frames = rate / n
    evals_frame = notes * ops * n
    return {
        "frames_per_second": frames,
        "operator_evals_per_frame": evals_frame,
        "operator_evals_per_second": evals_frame * frames,
        "op_loop_only_minimum_clock_hz": evals_frame * frames,
        "clocks": {name: {"hz": hz,
                          "clks_per_frame": hz / frames,
                          "clks_per_sample": hz / rate}
                   for name, hz in CLOCKS},
    }


def row_clocks(row, key):
    """A row's clocks for clock `key` ('12m288' / '24m576').

    Rows are unit x count unless the row carries an explicit per-clock count
    override (count_12m288 / count_24m576) with matching clocks_* fields.
    """
    count_key = f"count_{key}"
    clocks_key = f"clocks_{key}"
    if count_key in row:
        if clocks_key not in row:
            return None, f"row {row.get('id')}: {count_key} present without {clocks_key}"
        if row[clocks_key] != row["unit_clks"] * row[count_key]:
            return None, (f"row {row.get('id')}: {clocks_key}="
                          f"{row[clocks_key]} != unit {row['unit_clks']} x "
                          f"{count_key} {row[count_key]}")
        return row[clocks_key], None
    if clocks_key in row:
        return None, f"row {row.get('id')}: {clocks_key} present without {count_key}"
    return row["unit_clks"] * row["count"], None


def check_budget(spec, fails):
    policy = spec.get("budget_worst_case", {})
    rows = policy.get("rows", [])
    required = policy.get("required_row_ids", [])
    by_id = {}
    for row in rows:
        rid = row.get("id")
        if not rid or rid in by_id:
            fails.append(f"budget row missing or duplicate id: {rid!r}")
            continue
        by_id[rid] = row
    for rid in required:
        if rid not in by_id:
            fails.append(f"required worst-case budget row missing: {rid}")
    if "op_evals" in by_id:
        row = by_id["op_evals"]
        if row.get("unit_class") != "measured":
            fails.append("op_evals unit_class must be 'measured' (H01 anchor)")
        if row.get("unit_clks") != MEASURED_CLKS_PER_EVAL:
            fails.append(
                f"op_evals unit_clks = {row.get('unit_clks')!r} but the H01 "
                f"measured throughput is {MEASURED_CLKS_PER_EVAL} clk/eval")
        if row.get("count") != spec["premises"]["polyphony"] * \
                spec["premises"]["operators_per_note"] * \
                spec["premises"]["frame_samples"]:
            fails.append("op_evals count disagrees with premises arithmetic")

    totals = {}
    for key, _ in CLOCKS:
        total = 0
        for row in rows:
            clocks, err = row_clocks(row, key)
            if err:
                fails.append(err)
                continue
            if "count_12m288" not in row and "count_24m576" not in row:
                if row.get("clocks") != row["unit_clks"] * row["count"]:
                    fails.append(
                        f"row {row.get('id')}: clocks={row.get('clocks')} != "
                        f"unit {row['unit_clks']} x count {row['count']}")
            total += clocks or 0
        totals[key] = total

    facts = recomputed_premises(spec["premises"])
    rec_totals = policy.get("totals", {})
    for key, _ in CLOCKS:
        frame_clks = facts["clocks"][key]["clks_per_frame"]
        total = totals[key]
        util = total / frame_clks
        margin = frame_clks - total
        want_verdict = "PASS" if margin >= 0 else "FAIL"
        ck = key.capitalize().replace("m", "m")  # 12m288 -> 12m288
        ck = f"clocks_{key}"
        pairs = (
            (f"clocks_{key}", total),
            (f"frame_clks_{key}", frame_clks),
            (f"utilization_{key}", util),
            (f"margin_{key}", margin),
        )
        for name, want in pairs:
            got = rec_totals.get(name)
            if got is None:
                fails.append(f"totals.{name} missing")
            elif abs(got - want) > 1e-6:
                fails.append(
                    f"totals.{name} = {got!r} but recomputation gives {want!r}")
        got = rec_totals.get(f"verdict_{key}")
        if got != want_verdict:
            fails.append(
                f"totals.verdict_{key} = {got!r} but the worst-case "
                f"recomputation gives {want_verdict!r} "
                f"(margin {margin:+.0f} of {frame_clks:.0f} clocks)")
    # the recorded headline must name the failing fallback clock
    headline = rec_totals.get("headline", "")
    if "12.288 MHz DOES NOT close" not in headline:
        fails.append(
            "totals.headline must state the 12.288 MHz worst-case failure "
            "verbatim (issue #25: the shortfall is the headline)")
    return facts


def idle_frame_clocks(spec):
    """Clocks the frame would demand with no event burst (the forbidden
    average-case framing; reported for the record, never a verdict)."""
    burst_ids = ("note_on_application", "note_off_application",
                 "controller_events", "patch_commit")
    total = spec["budget_worst_case"]["totals"]["clocks_12m288"]
    rows = {r.get("id"): r for r in spec["budget_worst_case"]["rows"]}
    burst = sum(rows[rid]["clocks"] for rid in burst_ids if rid in rows)
    return total - burst


def check_dependencies(spec, fails):
    deps = spec.get("depends_on", {})
    for name, artifact in (
            ("h01_synth_report", deps.get("h01_evidence", {}).get("synth_report")),
            ("h01_compare_report", deps.get("h01_evidence", {}).get("compare_report")),
            ("h02_storage_report", deps.get("h02_evidence", {}).get("storage_synth_report"))):
        if not artifact:
            fails.append(f"depends_on.{name} missing")
            continue
        path = os.path.join(REPO_ROOT, artifact["path"])
        if not os.path.isfile(path):
            fails.append(f"depends_on.{name}: artifact missing: {artifact['path']}")
            continue
        actual = sha256_file(path)
        if actual != artifact.get("sha256"):
            fails.append(
                f"depends_on.{name}: sha256 drifted from the recorded "
                f"{artifact.get('sha256')[:12]}… to {actual[:12]}… — every "
                f"H03-derived budget is STALE until re-derived")
    # measured constants must agree with the artifacts' own recorded values
    h01 = os.path.join(REPO_ROOT, "evidence", "h01-operator-probe",
                       "synth_report.json")
    if os.path.isfile(h01):
        data, err = load_json(h01)
        if err:
            fails.append(f"h01 synth_report unreadable: {err}")
        else:
            throughput = (data.get("cycles_per_operator", {})
                          .get("throughput_clks_per_eval"))
            if throughput != MEASURED_CLKS_PER_EVAL:
                fails.append(
                    f"H01 synth_report records throughput {throughput!r} "
                    f"clk/eval, not the contracted {MEASURED_CLKS_PER_EVAL}")
    h02 = os.path.join(REPO_ROOT, "evidence", "h02-storage-probe",
                       "storage_synth_report.json")
    if os.path.isfile(h02):
        data, err = load_json(h02)
        if err:
            fails.append(f"h02 storage report unreadable: {err}")
        else:
            flops = data.get("gates", {}).get("full", {}).get("mapped_flops")
            if flops != MEASURED_H02_MAPPED_FLOPS:
                fails.append(
                    f"H02 report records {flops!r} mapped flops, not the "
                    f"contracted {MEASURED_H02_MAPPED_FLOPS}")
    release = os.path.join(REPO_ROOT, "reference", "release", "manifest.json")
    if os.path.isfile(release):
        data, err = load_json(release)
        if err:
            fails.append(f"N08 release manifest unreadable: {err}")
        else:
            want = deps.get("n08_release", {}).get("bundle_sha256")
            got = data.get("bundle_sha256")
            if want and got != want:
                fails.append(
                    f"N08 bundle hash drifted: manifest {str(got)[:12]}… != "
                    f"recorded {str(want)[:12]}…")
    profile_sha = deps.get("numeric_profile_sha256")
    if profile_sha and os.path.isfile(PROFILE_PATH):
        actual = sha256_file(PROFILE_PATH)
        if actual != profile_sha:
            fails.append(
                f"numeric profile sha256 drifted ({actual[:12]}… != "
                f"{profile_sha[:12]}…): all budget premises STALE")


def check_env_constant(spec, fails):
    """The envelope cadence constant must equal its cited formula (N03)."""
    path = PROFILE_PATH
    if not os.path.isfile(path):
        fails.append(f"numeric profile missing: {path}")
        return
    profile, err = load_json(path)
    if err:
        fails.append(f"numeric profile unreadable: {err}")
        return
    rate = spec["premises"]["sample_rate_hz"]
    want = int((44100.0 / rate) * (1 << 24))  # uint32 truncation, env.cc:47-49
    if want != SR_MULTIPLIER_48K:
        fails.append(
            f"checker's own sr_multiplier re-derivation {want} disagrees with "
            f"the frozen {SR_MULTIPLIER_48K}")
    got = (profile.get("envelope", {}) or {}).get("sr_multiplier_48k")
    if got != want:
        fails.append(
            f"numeric profile sr_multiplier_48k = {got!r} but "
            f"floor((44100.0/{rate}) * 2^24) = {want} (env.cc:47-49)")


def check_link_derivation(spec, fails):
    """Queue depth and drain counts must follow from the link arithmetic."""
    ev = spec.get("event_interface", {})
    depth = ev.get("queue_depth_skid")
    period = ev.get("min_transaction_period_clks")
    drain_rate = ev.get("drain_rate_writes_per_clk")
    if depth is None:
        fails.append("event_interface.queue_depth_skid missing (never guessed, "
                     "DEC-014)")
        return
    if period is None or drain_rate is None:
        fails.append("event_interface drain derivation numbers missing "
                     "(min_transaction_period_clks / drain_rate_writes_per_clk)")
        return
    # depth must absorb any burst the link can deliver while the drain runs:
    # arrival period >> drain period means a single-entry skid suffices, and
    # depth 8 gives an 8x margin; the invariant checked is the headroom ratio
    if period < drain_rate:
        fails.append("arrival period faster than the drain rate: the depth "
                     "derivation is invalid, overflow would be reachable")
    if depth * period < 8 * period:
        fails.append("skid queue depth below the derived bound (8 entries)")
    want12 = 16384 // period
    want24 = 32768 // period
    rows = {r.get("id"): r for r in spec.get("budget_worst_case", {}).get("rows", [])}
    drain = rows.get("write_queue_drain", {})
    if drain.get("count_12m288") != want12 or drain.get("count_24m576") != want24:
        fails.append(
            f"write_queue_drain counts {drain.get('count_12m288')}/"
            f"{drain.get('count_24m576')} disagree with floor(frame/{period}) = "
            f"{want12}/{want24}")
    lat = spec.get("latency", {})
    if lat.get("commit_bound_frames") != 1:
        fails.append("latency.commit_bound_frames must be 1 (DEC-014/NUM-011)")
    if lat.get("worst_case_to_sdata_samples") != 128:
        fails.append("latency.worst_case_to_sdata_samples must be 128 "
                     "(<= 2 frames, the declared measured bound)")


DEC_RE = re.compile(r"\bDEC-\d{3}\b")


def check_dec_citations(spec, fails):
    contract, err = load_json(CONTRACT_PATH)
    if err:
        fails.append(f"product contract unreadable: {err}")
        return
    known = set(contract.get("decisions", {}))
    where = []
    for row in spec.get("budget_worst_case", {}).get("rows", []):
        where.append((f"budget row {row.get('id')}", row.get("dec_ids", [])))
    for section in ("patch_interface", "event_interface", "status_word",
                    "reset_boot"):
        where.append((section, spec.get(section, {}).get("dec_ids", [])))
    for mod in spec.get("modules", []):
        where.append((f"module {mod.get('name')}", mod.get("dec_ids", [])))
    for label, ids in where:
        for dec in ids:
            if dec not in known:
                fails.append(f"{label}: cited {dec} does not exist in "
                             f"spec/contract-v1.json")


def check_spec(spec, repo_root=REPO_ROOT):
    """Check an in-memory spec dict (importable for negative controls)."""
    fails = []
    for section in ("premises", "clock_targets", "budget_worst_case",
                    "storage_ports", "patch_interface", "event_interface",
                    "status_word", "reset_boot", "latency", "modules",
                    "stale_rules", "depends_on"):
        if section not in spec:
            fails.append(f"required section missing: {section}")
    if fails:
        return fails
    try:
        facts = check_budget(spec, fails)
        # clock targets must match the frame arithmetic
        targets = spec.get("clock_targets", {})
        for role, key in (("design_point", "24m576"),
                          ("fallback", "12m288")):
            t = targets.get(role, {})
            f = facts["clocks"][key]
            if abs(t.get("clks_per_frame", -1) - f["clks_per_frame"]) > 1e-6:
                fails.append(
                    f"clock_targets.{role}.clks_per_frame = "
                    f"{t.get('clks_per_frame')!r} != {f['clks_per_frame']:.0f}")
        if targets.get("design_point", {}).get("hz") != 24576000.0:
            fails.append("design point must be 24.576 MHz (headline finding)")
        check_dependencies(spec, fails)
        check_env_constant(spec, fails)
        check_link_derivation(spec, fails)
        check_dec_citations(spec, fails)
    except KeyError as exc:
        fails.append(f"malformed contract: missing key {exc}")
    return fails


def main(argv=None):
    spec, err = load_json(SPEC_PATH)
    if err:
        print(f"COULD-NOT-RUN: {err}")
        return 2
    fails = check_spec(spec)
    if fails:
        print("FAIL: h03 core-contract budget/citation checks:")
        for f in fails:
            print(f"  - {f}")
        return 1
    totals = spec["budget_worst_case"]["totals"]
    idle = idle_frame_clocks(spec)
    print("h03 core-contract check PASS")
    print(f"  worst case @12.288 MHz: {totals['clocks_12m288']}/{totals['frame_clks_12m288']:.0f} clk "
          f"= {totals['utilization_12m288']*100:.2f}% -> {totals['verdict_12m288']}")
    print(f"  worst case @24.576 MHz: {totals['clocks_24m576']}/{totals['frame_clks_24m576']:.0f} clk "
          f"= {totals['utilization_24m576']*100:.2f}% -> {totals['verdict_24m576']}")
    print(f"  idle-frame (average-case) demand @12.288 MHz would be {idle} clk "
          f"= {idle/totals['frame_clks_12m288']*100:.2f}% -- recorded as the "
          f"forbidden average-case framing, never a verdict")
    return 0


if __name__ == "__main__":
    sys.exit(main())
