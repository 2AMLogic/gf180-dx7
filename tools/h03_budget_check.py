"""H03 budget checker: recompute the core-contract worst-case budget, SPI
completion bounds, queue carry-over witnesses, host vs raw pricing, and probe
evidence from first principles, and fail on drift (issues #25, #339; DR-0008,
DR-0015).

Fails when:
  - the frame arithmetic (6,144 op evals/frame, 750 frames/s, both clock
    targets) disagrees with independent recomputation from the premises;
  - write_queue_drain uses whole-period floor(W / 194) = 84/168 instead of the
    arbitrary-phase half-open bound 1 + floor((W - 1) / 194) = 85/169;
  - note_on_application shrinks below the reachable occupied-polyphony raw-wire
    witness (98 @12.288 / 194 @24.576) or restores an all-mixes PASS at
    24.576 MHz (refuted already by the 100-note-on EE witness: 17,344 - 4,864 +
    100 * 224 = 34,880 clocks, margin -2,112);
  - any budget row's clocks disagree with unit x count, any required row is
    missing, or totals / utilizations / margins / verdicts disagree with
    recomputation (a correctly recorded negative planning margin with verdict
    FAIL passes; a fake PASS fails);
  - the historical 16/16/16 regression vector, raw-wire witnesses, unresolved
    paper conservation bound, or supplied-host pricing disagree with first-
    principles recomputation;
  - any pinned artifact hash (H01, H02, N08, numeric profile, H03 SPI admission
    probe) drifts, or probe frame conservation / STALE / NO_VERDICT / NOT_RUN
    dispositions are violated;
  - the frozen sr_multiplier disagrees with its cited formula (env.cc:47-49);
  - a DEC-id citation does not exist in spec/contract-v1.json.

Exit codes: 0 pass, 1 check failure, 2 could-not-run. Stdlib only.
"""

import json
import os
import re
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC_PATH = os.path.join(REPO_ROOT, "spec", "core-contract-v1.json")
CONTRACT_PATH = os.path.join(REPO_ROOT, "spec", "contract-v1.json")
PROFILE_PATH = os.path.join(REPO_ROOT, "spec", "numeric-profile-v1.json")

MEASURED_CLKS_PER_EVAL = 1  # H01 (evidence/h01-operator-probe)
MEASURED_H02_MAPPED_FLOPS = 38781  # H02 (38,336 replicated + 444 patch + 1 obs)
SR_MULTIPLIER_48K = 15414067  # floor((44100/48000) * 2^24), env.cc:47-49

CLOCKS = (("12m288", 12288000.0), ("24m576", 24576000.0))

_TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
from sha256_helpers import sha256_file


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
        "clocks": {
            name: {
                "hz": hz,
                "clks_per_frame": hz / frames,
                "clks_per_sample": hz / rate,
            }
            for name, hz in CLOCKS
        },
    }


def max_completions_arbitrary_phase(window_clks, period_clks):
    """Maximum completions in half-open [0, W) at minimum spacing P:
    1 + floor((W - 1) / P); a completion at offset W belongs to [W, 2W)."""
    return 1 + (int(window_clks) - 1) // int(period_clks)


def derive_reachable_noteon_pops(window_clks, period_clks, evq_depth, strike_clks):
    """Derive tight max occupied-polyphony raw-wire EA_NOTEON pops in [0, W)
    from a non-overrunning prior frame [-W, 0)."""
    w = int(window_clks)
    p = int(period_clks)
    qmax = min(int(evq_depth), 1 + (w - 3) // p)
    best = (0, 0, 0, 0)
    for q_open in range(qmax + 1):
        d1 = max(2 if q_open == evq_depth else 0, (-w + 2) + q_open * p)
        e_adm = 0 if d1 >= w else (1 + (w - 1 - d1) // p)
        m = 0
        if q_open > 0:
            while m + 1 <= e_adm and (d1 + p * m) <= strike_clks * (q_open + m):
                m += 1
        elif d1 == 0 and e_adm >= 1:
            m = 1
            while m + 1 <= e_adm and (d1 + p * m) <= strike_clks * m:
                m += 1
        pops = q_open + m
        q_close = q_open + e_adm - pops
        if pops > best[0] or (pops == best[0] and q_open > best[1]):
            best = (pops, q_open, e_adm, q_close)
    return {
        "pops": best[0],
        "evq_open": best[1],
        "evq_admitted": best[2],
        "evq_close": best[3],
    }


def row_clocks(row, key):
    """A row's clocks for clock `key` ('12m288' / '24m576')."""
    count_key = f"count_{key}"
    clocks_key = f"clocks_{key}"
    if count_key in row:
        if clocks_key not in row:
            return (
                None,
                f"row {row.get('id')}: {count_key} present without {clocks_key}",
            )
        if row[clocks_key] != row["unit_clks"] * row[count_key]:
            return None, (
                f"row {row.get('id')}: {clocks_key}={row[clocks_key]} != "
                f"unit {row['unit_clks']} x {count_key} {row[count_key]}"
            )
        return row[clocks_key], None
    if clocks_key in row:
        return None, f"row {row.get('id')}: {clocks_key} present without {count_key}"
    return row["unit_clks"] * row["count"], None


def row_count_for(row, key):
    return row[f"count_{key}"] if f"count_{key}" in row else row.get("count", 0)


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
                f"measured throughput is {MEASURED_CLKS_PER_EVAL} clk/eval"
            )
        if row.get("count") != (
            spec["premises"]["polyphony"]
            * spec["premises"]["operators_per_note"]
            * spec["premises"]["frame_samples"]
        ):
            fails.append("op_evals count disagrees with premises arithmetic")

    totals = {}
    for key, _ in CLOCKS:
        total = 0
        for row in rows:
            clocks, err = row_clocks(row, key)
            if err:
                fails.append(err)
                continue
            if (
                "count_12m288" not in row
                and "count_24m576" not in row
                and row.get("clocks") != row["unit_clks"] * row["count"]
            ):
                fails.append(
                    f"row {row.get('id')}: clocks={row.get('clocks')} != "
                    f"unit {row['unit_clks']} x count {row['count']}"
                )
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
                    f"totals.{name} = {got!r} but recomputation gives {want!r}"
                )
        got = rec_totals.get(f"verdict_{key}")
        if got != want_verdict:
            fails.append(
                f"totals.verdict_{key} = {got!r} but the worst-case "
                f"recomputation gives {want_verdict!r} "
                f"(margin {margin:+.0f} of {frame_clks:.0f} clocks)"
            )

    # Guard against restoring an all-mixes PASS at 24.576 MHz by shrinking the
    # admitted raw-wire note-on count back to 16/16/16 (issue #339 control 2).
    ev = spec.get("event_interface", {})
    period = ev.get("min_transaction_period_clks", 194)
    evq_depth = ev.get("queue_depth_evq", 168)
    strike_clks = ev.get("rtl_occupied_strike_clks", 26)
    if (
        "note_on_application" in by_id
        and "note_off_application" in by_id
        and "controller_events" in by_id
    ):
        non_ev_non_drain = sum(
            row_clocks(r, "24m576")[0] or 0
            for r in rows
            if r.get("id")
            not in {
                "note_on_application",
                "note_off_application",
                "controller_events",
                "write_queue_drain",
            }
        )
        unit_on = by_id["note_on_application"]["unit_clks"]
        if unit_on < 224:
            fails.append(f"note_on_application unit_clks = {unit_on} < 224")
        # EE 100-note-on counterexample under original whole-period drain (168):
        ee_witness_clks = non_ev_non_drain + (32768 // period) + 100 * unit_on
        ee_margin = 32768 - ee_witness_clks
        if rec_totals.get("verdict_24m576") == "PASS" and ee_margin < 0:
            fails.append(
                f"all-mixes PASS at 24.576 MHz is refuted by the 100-note-on "
                f"EE witness: {ee_witness_clks} clocks (margin {ee_margin:+d})"
            )
        w12 = derive_reachable_noteon_pops(16384, period, evq_depth, strike_clks)
        w24 = derive_reachable_noteon_pops(32768, period, evq_depth, strike_clks)
        got12 = row_count_for(by_id["note_on_application"], "12m288")
        got24 = row_count_for(by_id["note_on_application"], "24m576")
        if got12 < w12["pops"] or got24 < w24["pops"]:
            fails.append(
                f"note_on_application counts {got12}/{got24} undercount the "
                f"reachable raw-wire pops {w12['pops']}/{w24['pops']} "
                f"(EE 100-note-on witness alone needs {ee_witness_clks} clk, "
                f"margin {ee_margin:+d})"
            )

    headline = rec_totals.get("headline", "")
    if "12.288 MHz DOES NOT close" not in headline:
        fails.append(
            "totals.headline must state the 12.288 MHz worst-case failure "
            "verbatim (issue #25: the shortfall is the headline)"
        )
    if "FAILS all-admissible raw-wire planning" not in headline:
        fails.append(
            "totals.headline must state that 24.576 MHz FAILS all-admissible "
            "raw-wire planning under H03 unit costs (issue #339 / DR-0015)"
        )
    return facts


def check_workloads_and_pricing(spec, fails):
    """Independently recompute the 16/16/16 regression vector, raw-wire
    witnesses, paper conservation bound, and supplied-host pricing."""
    bw = spec.get("budget_worst_case", {})
    rows = {r.get("id"): r for r in bw.get("rows", []) if r.get("id")}
    required_ev = ("note_on_application", "note_off_application", "controller_events")
    if any(k not in rows for k in (*required_ev, "write_queue_drain", "patch_commit")):
        return
    u_on = rows["note_on_application"]["unit_clks"]
    u_off = rows["note_off_application"]["unit_clks"]
    u_cc = rows["controller_events"]["unit_clks"]
    u_commit = rows["patch_commit"]["unit_clks"]
    fixed_ex_commit = sum(
        row_clocks(r, "24m576")[0] or 0
        for rid, r in rows.items()
        if rid not in (*required_ev, "write_queue_drain", "patch_commit")
    )
    fixed_with_commit = fixed_ex_commit + u_commit

    # 1. Historical 16/16/16 regression vector
    reg = bw.get("historical_regression_vector")
    if not isinstance(reg, dict):
        fails.append("budget_worst_case.historical_regression_vector missing")
    else:
        hist_ev = 16 * u_on + 16 * u_off + 16 * u_cc
        if reg.get("raw_wire_writes") != 48:
            fails.append("historical_regression_vector.raw_wire_writes != 48")
        if reg.get("supplied_host_writes") != 160:
            fails.append("historical_regression_vector.supplied_host_writes != 160")
        for subkey, d12, d24 in (
            ("original_whole_period_drain", 84, 168),
            ("arbitrary_phase_drain", 85, 169),
        ):
            sub = reg.get(subkey, {})
            w12 = fixed_with_commit + d12 + hist_ev
            w24 = fixed_with_commit + d24 + hist_ev
            if (
                sub.get("clocks_12m288") != w12
                or sub.get("margin_12m288") != 16384 - w12
                or sub.get("verdict_12m288") != "FAIL"
                or sub.get("clocks_24m576") != w24
                or sub.get("margin_24m576") != 32768 - w24
                or sub.get("verdict_24m576") != "PASS"
            ):
                fails.append(
                    f"historical_regression_vector.{subkey} arithmetic mismatch"
                )

    # 2. Raw-wire witnesses & unresolved paper conservation bound
    raw = bw.get("raw_wire_witnesses")
    if not isinstance(raw, dict):
        fails.append("budget_worst_case.raw_wire_witnesses missing")
    else:
        ee = raw.get("ee_100_note_on_original_non_event", {})
        want_ee = fixed_with_commit + 168 + 100 * u_on
        if (
            ee.get("clocks_24m576") != want_ee
            or ee.get("margin_24m576") != 32768 - want_ee
            or ee.get("verdict_24m576") != "FAIL"
        ):
            fails.append(
                "raw_wire_witnesses.ee_100_note_on_original_non_event mismatch"
            )

        for name, cnt in (
            ("probe_100_note_on_application_frame", 100),
            ("probe_168_note_on_application_frame", 168),
        ):
            p = raw.get(name, {})
            want = fixed_ex_commit + cnt * u_on
            if (
                p.get("clocks_24m576") != want
                or p.get("margin_24m576") != 32768 - want
                or p.get("verdict_24m576") != "FAIL"
            ):
                fails.append(f"raw_wire_witnesses.{name} mismatch")

        ub = raw.get("unresolved_paper_conservation_upper_bound", {})
        if (
            ub.get("max_queued_note_ons_24m576") != 337
            or ub.get("clocks_24m576") != fixed_with_commit + 169 + 337 * u_on
            or ub.get("margin_24m576") != 32768 - (fixed_with_commit + 169 + 337 * u_on)
            or ub.get("max_queued_note_ons_12m288_clean_prior") != 169
            or ub.get("clocks_12m288_clean_prior")
            != fixed_with_commit + 85 + 169 * u_on
        ):
            fails.append(
                "raw_wire_witnesses.unresolved_paper_conservation_upper_bound mismatch"
            )

    # 3. Supplied host (src/gf180_dx7/host.py) pricing
    host = bw.get("supplied_host_pricing")
    if not isinstance(host, dict):
        fails.append("budget_worst_case.supplied_host_pricing missing")
    else:
        poll = host.get("status_poll_windows")
        if (
            host.get("note_on_writes") != 8
            or host.get("note_off_writes") != 1
            or host.get("controller_writes") != 1
            or poll != 1
        ):
            fails.append("supplied_host_pricing write constants disagree with host.py")
        if host.get("write_cap_12m288") != (16384 // 194) - poll:
            fails.append("supplied_host_pricing.write_cap_12m288 mismatch")
        if host.get("write_cap_24m576") != (32768 // 194) - poll:
            fails.append("supplied_host_pricing.write_cap_24m576 mismatch")
        sf = host.get("single_frame_cost_maximizing", {})
        if (
            sf.get("clocks_12m288") != fixed_with_commit + 85 + 83 * u_off
            or sf.get("clocks_24m576") != fixed_with_commit + 169 + 167 * u_off
        ):
            fails.append("supplied_host_pricing.single_frame_cost_maximizing mismatch")
        cr = host.get("carry_and_replenish_cost_maximizing", {})
        if (
            cr.get("clocks_12m288") != fixed_with_commit + 85 + 91 * u_off
            or cr.get("margin_12m288") != 16384 - (fixed_with_commit + 85 + 91 * u_off)
            or cr.get("clocks_24m576") != fixed_with_commit + 169 + 183 * u_off
            or cr.get("margin_24m576")
            != 32768 - (fixed_with_commit + 169 + 183 * u_off)
            or host.get("verdict_12m288") != "FAIL"
            or host.get("verdict_24m576") != "PASS"
        ):
            fails.append(
                "supplied_host_pricing.carry_and_replenish_cost_maximizing mismatch"
            )


def idle_frame_clocks(spec):
    """Clocks the frame would demand with no event burst (the forbidden
    average-case framing; reported for the record, never a verdict)."""
    burst_ids = (
        "note_on_application",
        "note_off_application",
        "controller_events",
        "patch_commit",
    )
    total = spec["budget_worst_case"]["totals"]["clocks_12m288"]
    rows = {r.get("id"): r for r in spec["budget_worst_case"]["rows"]}
    burst = sum(
        (row_clocks(rows[rid], "12m288")[0] or 0) for rid in burst_ids if rid in rows
    )
    return total - burst


def check_dependencies(spec, fails):
    deps = spec.get("depends_on", {})
    probe_deps = deps.get("h03_admission_probe", {})
    artifacts = (
        ("h01_synth_report", deps.get("h01_evidence", {}).get("synth_report")),
        ("h01_compare_report", deps.get("h01_evidence", {}).get("compare_report")),
        (
            "h02_storage_report",
            deps.get("h02_evidence", {}).get("storage_synth_report"),
        ),
        ("h03_rtl_core", probe_deps.get("rtl_core")),
        ("h03_host_module", probe_deps.get("host_module")),
        ("h03_probe_testbench", probe_deps.get("testbench")),
        ("h03_probe_results", probe_deps.get("probe_results")),
    )
    for name, artifact in artifacts:
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
                f"{str(artifact.get('sha256'))[:12]}… to {actual[:12]}… — every "
                f"H03-derived budget is STALE until re-derived"
            )

    h01 = os.path.join(REPO_ROOT, "evidence", "h01-operator-probe", "synth_report.json")
    if os.path.isfile(h01):
        data, err = load_json(h01)
        if err:
            fails.append(f"h01 synth_report unreadable: {err}")
        else:
            throughput = data.get("cycles_per_operator", {}).get(
                "throughput_clks_per_eval"
            )
            if throughput != MEASURED_CLKS_PER_EVAL:
                fails.append(
                    f"H01 synth_report records throughput {throughput!r} "
                    f"clk/eval, not the contracted {MEASURED_CLKS_PER_EVAL}"
                )
    h02 = os.path.join(
        REPO_ROOT, "evidence", "h02-storage-probe", "storage_synth_report.json"
    )
    if os.path.isfile(h02):
        data, err = load_json(h02)
        if err:
            fails.append(f"h02 storage report unreadable: {err}")
        else:
            flops = data.get("gates", {}).get("full", {}).get("mapped_flops")
            if flops != MEASURED_H02_MAPPED_FLOPS:
                fails.append(
                    f"H02 report records {flops!r} mapped flops, not the "
                    f"contracted {MEASURED_H02_MAPPED_FLOPS}"
                )
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
                    f"recorded {str(want)[:12]}…"
                )
    profile_sha = deps.get("numeric_profile_sha256")
    if profile_sha and os.path.isfile(PROFILE_PATH):
        actual = sha256_file(PROFILE_PATH)
        if actual != profile_sha:
            fails.append(
                f"numeric profile sha256 drifted ({actual[:12]}… != "
                f"{profile_sha[:12]}…): all budget premises STALE"
            )

    probe_art = probe_deps.get("probe_results", {})
    probe_path = os.path.join(REPO_ROOT, probe_art.get("path", ""))
    if probe_art and os.path.isfile(probe_path):
        pdata, err = load_json(probe_path)
        if err:
            fails.append(f"h03 probe results unreadable: {err}")
        else:
            check_probe_and_disposition(spec, pdata, fails)


def check_probe_and_disposition(spec, pdata, fails):
    """Verify probe frame conservation, source hashes, and §6 dispositions."""
    for rel, digest in pdata.get("source_sha256", {}).items():
        full = os.path.join(REPO_ROOT, rel)
        if not os.path.isfile(full) or sha256_file(full) != digest:
            fails.append(f"probe results source_sha256 drifted for {rel}")
    for run in pdata.get("exercised_runs", []):
        for fr in run.get("frames", []):
            drains = fr["drains_sec0"] + fr["drains_sec1"]
            if (
                fr["skid_open"] + (fr["completions"] - fr["skid_rej"]) - drains
                != fr["skid_close"]
            ):
                fails.append(
                    f"probe {run['id']} frame {fr['frame']}: skid conservation failed"
                )
            if fr["drains_sec0"] != fr["evq_admit"] + fr["evq_rej"]:
                fails.append(
                    f"probe {run['id']} frame {fr['frame']}: SEC=0 split failed"
                )
            pops = fr["pops_noteon"] + fr["pops_other"]
            if fr["evq_open"] + fr["evq_admit"] - pops != fr["evq_close"]:
                fails.append(
                    f"probe {run['id']} frame {fr['frame']}: EVQ conservation failed"
                )
    disp = spec.get("evidence_disposition", {})
    rtl_m = disp.get("rtl_measured", {})
    if rtl_m.get("24m576_all_admissible_deadline") != "NO_VERDICT":
        fails.append(
            "evidence_disposition: 24m576_all_admissible_deadline must be NO_VERDICT"
        )
    if rtl_m.get("12m288_rtl_deadline_and_overflow") != "NOT_RUN":
        fails.append(
            "evidence_disposition: 12m288_rtl_deadline_and_overflow must be NOT_RUN"
        )
    stale_list = disp.get("stale_evidence", [])
    if not stale_list or any(s.get("status") != "STALE" for s in stale_list):
        fails.append(
            "evidence_disposition.stale_evidence must record STALE affected items"
        )


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
    want = int((44100.0 / rate) * (1 << 24))
    if want != SR_MULTIPLIER_48K:
        fails.append(
            f"checker's own sr_multiplier re-derivation {want} disagrees with "
            f"the frozen {SR_MULTIPLIER_48K}"
        )
    got = (profile.get("envelope", {}) or {}).get("sr_multiplier_48k")
    if got != want:
        fails.append(
            f"numeric profile sr_multiplier_48k = {got!r} but "
            f"floor((44100.0/{rate}) * 2^24) = {want} (env.cc:47-49)"
        )


def check_link_derivation(spec, fails):
    """Queue depth and arbitrary-phase drain counts must follow from link arithmetic."""
    ev = spec.get("event_interface", {})
    depth = ev.get("queue_depth_skid")
    evq_depth = ev.get("queue_depth_evq")
    period = ev.get("min_transaction_period_clks")
    drain_rate = ev.get("drain_rate_writes_per_clk")
    if depth is None or evq_depth is None:
        fails.append(
            "event_interface.queue_depth_skid / queue_depth_evq missing (DEC-014)"
        )
        return
    if period is None or drain_rate is None:
        fails.append(
            "event_interface drain derivation numbers missing "
            "(min_transaction_period_clks / drain_rate_writes_per_clk)"
        )
        return
    if period < drain_rate:
        fails.append(
            "arrival period faster than the drain rate: skid depth derivation invalid"
        )
    if depth < 8:
        fails.append("skid queue depth below the derived bound (8 entries)")
    if evq_depth != 168:
        fails.append(
            f"event_interface.queue_depth_evq = {evq_depth!r} != 168 (rtl/dx7_core.v)"
        )

    want12 = max_completions_arbitrary_phase(16384, period)
    want24 = max_completions_arbitrary_phase(32768, period)
    rows = {r.get("id"): r for r in spec.get("budget_worst_case", {}).get("rows", [])}
    drain = rows.get("write_queue_drain", {})
    if drain.get("count_12m288") != want12 or drain.get("count_24m576") != want24:
        fails.append(
            f"write_queue_drain counts {drain.get('count_12m288')}/"
            f"{drain.get('count_24m576')} disagree with arbitrary-phase "
            f"1 + floor((frame - 1)/{period}) = {want12}/{want24}"
        )
    lat = spec.get("latency", {})
    if lat.get("commit_bound_frames") != 1:
        fails.append("latency.commit_bound_frames must be 1 (DEC-014/NUM-011)")
    if lat.get("worst_case_to_sdata_samples") != 128:
        fails.append("latency.worst_case_to_sdata_samples must be 128 (<= 2 frames)")


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
    for section in ("patch_interface", "event_interface", "status_word", "reset_boot"):
        where.append((section, spec.get(section, {}).get("dec_ids", [])))
    for mod in spec.get("modules", []):
        where.append((f"module {mod.get('name')}", mod.get("dec_ids", [])))
    for label, ids in where:
        for dec in ids:
            if dec not in known:
                fails.append(
                    f"{label}: cited {dec} does not exist in spec/contract-v1.json"
                )


def check_spec(spec, repo_root=REPO_ROOT):
    """Check an in-memory spec dict (importable for negative controls)."""
    fails = []
    for section in (
        "premises",
        "clock_targets",
        "budget_worst_case",
        "storage_ports",
        "patch_interface",
        "event_interface",
        "status_word",
        "reset_boot",
        "latency",
        "modules",
        "evidence_disposition",
        "stale_rules",
        "depends_on",
    ):
        if section not in spec:
            fails.append(f"required section missing: {section}")
    if fails:
        return fails
    try:
        facts = check_budget(spec, fails)
        check_workloads_and_pricing(spec, fails)
        targets = spec.get("clock_targets", {})
        for role, key in (("design_point", "24m576"), ("fallback", "12m288")):
            t = targets.get(role, {})
            f = facts["clocks"][key]
            if abs(t.get("clks_per_frame", -1) - f["clks_per_frame"]) > 1e-6:
                fails.append(
                    f"clock_targets.{role}.clks_per_frame = "
                    f"{t.get('clks_per_frame')!r} != {f['clks_per_frame']:.0f}"
                )
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
    hist = spec["budget_worst_case"]["historical_regression_vector"][
        "arbitrary_phase_drain"
    ]
    host = spec["budget_worst_case"]["supplied_host_pricing"]
    idle = idle_frame_clocks(spec)
    print("h03 core-contract check PASS")
    print(
        f"  raw-wire worst case @12.288 MHz: {totals['clocks_12m288']}/"
        f"{totals['frame_clks_12m288']:.0f} clk = "
        f"{totals['utilization_12m288'] * 100:.2f}% "
        f"(margin {totals['margin_12m288']:+d}) -> {totals['verdict_12m288']}"
    )
    print(
        f"  raw-wire worst case @24.576 MHz: {totals['clocks_24m576']}/"
        f"{totals['frame_clks_24m576']:.0f} clk = "
        f"{totals['utilization_24m576'] * 100:.2f}% "
        f"(margin {totals['margin_24m576']:+d}) -> {totals['verdict_24m576']}"
    )
    print(
        f"  historical 16/16/16 (arbitrary-phase drain): "
        f"12.288 MHz {hist['clocks_12m288']} clk ({hist['margin_12m288']:+d}) -> {hist['verdict_12m288']}; "
        f"24.576 MHz {hist['clocks_24m576']} clk ({hist['margin_24m576']:+d}) -> {hist['verdict_24m576']}"
    )
    print(
        f"  supplied host (host.py): 12.288 MHz -> {host['verdict_12m288']}; "
        f"24.576 MHz -> {host['verdict_24m576']}"
    )
    print(
        f"  idle-frame (average-case) demand @12.288 MHz would be {idle} clk "
        f"= {idle / totals['frame_clks_12m288'] * 100:.2f}% -- recorded as the "
        f"forbidden average-case framing, never a verdict"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
