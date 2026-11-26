"""H03 core-contract tests (issues #25, #339; DR-0008 / DR-0015).

Covers, per issue #25 and issue #339 acceptance:
  - independent first-principles recomputation of:
      * half-open frame SPI completions 1 + floor((W - 1) / 194) -> 85 and 169
        (a completion at offset W belongs to the next frame);
      * worst reachable raw-wire event-application mix across adjacent frames
        (opening occupancy, admissions, pops by event type, closing occupancy,
        simultaneous pop+push rejection at full occupancy, skid stage in-flight);
      * separate pricing of raw wire commands vs supplied host (host.py: 8
        writes per note-on, 1 per note-off/controller -> 160 writes for 16/16/16);
      * historical 16/16/16 regression vector under both original whole-period
        (17,260 / 17,344) and corrected arbitrary-phase (17,261 / 17,345) drain;
      * unresolved paper conservation upper bound (when service > 194 clk);
      * STALE evidence disposition (§6) and measured RTL deadline/overflow vs
        NOT_RUN / NO_VERDICT coverage over evidence/h03-event-admission/results.json;
  - every interface decision cites a DEC id in spec/contract-v1.json;
  - tools/h03_budget_check.py passes live (accepting negative planning margins);
  - negative controls:
      1. mutated op_evals unit (2 clk/eval) fails the checker;
      2. burst-stripped average-case budget fails the checker;
      3. restored whole-period drain bound (84 / 168) fails the checker;
      4. restored all-mixes PASS at 24.576 MHz (both bare verdict flip and
         restored 16/16/16 rows + PASS, refuted by the 100-note-on EE witness
         17,344 - 4,864 + 100 * 224 = 34,880, margin -2,112) fails the checker;
      5. fake PASS verdict at 12.288 MHz fails the checker;
      6. drifted evidence hash (H01 and H03 admission probe) fails the checker;
  - lockstep with docs/CONTRACT-CORE-v1.md, DR-0015, and N08 vectors.

Stdlib only (runs in make test-fast under DR-0009).
"""

import copy
import hashlib
import json
import os
import subprocess
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC_PATH = os.path.join(REPO, "spec", "core-contract-v1.json")
CONTRACT_PATH = os.path.join(REPO, "spec", "contract-v1.json")
DOC_PATH = os.path.join(REPO, "docs", "CONTRACT-CORE-v1.md")
DR0015_PATH = os.path.join(
    REPO, "docs", "decision-records", "0015-h03-admission-budget-reconciliation.md"
)
PROBE_RESULTS_PATH = os.path.join(
    REPO, "evidence", "h03-event-admission", "results.json"
)
TOOL = os.path.join(REPO, "tools", "h03_budget_check.py")
VECTORS_DIR = os.path.join(REPO, "reference", "release", "vectors")

sys_path = os.path.join(REPO, "tools")
if sys_path not in __import__("sys").path:
    __import__("sys").path.insert(0, sys_path)
import h03_budget_check


def load(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def sha256_path(path):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


def max_completions_half_open(window_clks, period_clks):
    """Maximum completions in half-open [0, W) at arbitrary phase:
    offsets 0, P, ..., P * floor((W - 1) / P); offset W is in [W, 2W)."""
    assert window_clks >= 1 and period_clks >= 1
    return 1 + (window_clks - 1) // period_clks


def rtl_max_noteon_pops_from_clean_prior(
    window_clks, period_clks, evq_depth, strike_clks
):
    """Independent derivation of max single-frame occupied-note-on pops in
    dx7_core.v when the prior frame finished F_EVPOP (no prior overrun).

    In Frame N-1 ([-W, 0)), F_EVPOP exits no earlier than offset -W + 1, so
    drains accumulating into Frame N's opening occupancy Q_open lie in
    [-W + 2, -1], giving Q_open <= min(evq_depth, 1 + (W - 3) // P).
    In Frame N ([0, W)):
      - if Q_open == evq_depth, offsets 0 (F_COMMIT) and 1 (first F_EVPOP) see
        evq_full == 1 (even on simultaneous wc_pop_fire at offset 1), so the
        first admitted drain in Frame N is at offset d1 >= max(2, -W + 2 + Q_open * P);
      - if Q_open < evq_depth, d1 >= max(0, -W + 2 + Q_open * P).
    With m >= 0 mid-pop admissions before F_EVPOP sees evq_empty at 1 + S*(Q_open+m),
    the m-th drain at d1 + P*(m - 1) must satisfy d1 + P*(m - 1) <= S*(Q_open + m - 1).
    """
    max_q_open = min(evq_depth, 1 + (window_clks - 3) // period_clks)
    best = (0, 0, 0, 0)  # (pops, q_open, e_admitted, q_close)
    for q_open in range(max_q_open + 1):
        min_d1_phase = (-window_clks + 2) + q_open * period_clks
        d1 = max(2 if q_open == evq_depth else 0, min_d1_phase)
        if d1 >= window_clks:
            e_adm = 0
        else:
            e_adm = 1 + (window_clks - 1 - d1) // period_clks
        m = 0
        if q_open > 0:
            while m + 1 <= e_adm:
                dm_next = d1 + period_clks * m
                if dm_next <= strike_clks * (q_open + m):
                    m += 1
                else:
                    break
        elif d1 == 0 and e_adm >= 1:
            # Drain at offset 0 (during F_COMMIT) lands in evq before F_EVPOP at offset 1.
            m = 1
            while m + 1 <= e_adm:
                dm_next = d1 + period_clks * m
                if dm_next <= strike_clks * m:
                    m += 1
                else:
                    break
        pops = q_open + m
        q_close = q_open + e_adm - pops
        assert 0 <= q_close <= evq_depth
        if pops > best[0] or (pops == best[0] and q_open > best[1]):
            best = (pops, q_open, e_adm, q_close)
    return {
        "pops": best[0],
        "evq_open": best[1],
        "evq_admitted": best[2],
        "evq_close": best[3],
    }


def recompute(spec):
    """Independent recomputation from first principles (test-local)."""
    p = spec["premises"]
    evals_frame = p["polyphony"] * p["operators_per_note"] * p["frame_samples"]
    frames = p["sample_rate_hz"] // p["frame_samples"]
    frame_clks = {
        "12m288": 12288000 // frames,
        "24m576": 24576000 // frames,
    }
    totals = {}
    for key in ("12m288", "24m576"):
        total = 0
        for r in spec["budget_worst_case"]["rows"]:
            cnt = r[f"count_{key}"] if f"count_{key}" in r else r["count"]
            clk = r.get(f"clocks_{key}", r.get("clocks"))
            assert clk == r["unit_clks"] * cnt, f"row {r['id']} {key} arith"
            total += r["unit_clks"] * cnt
        totals[key] = total
    return {
        "evals_frame": evals_frame,
        "evals_second": evals_frame * frames,
        "frames": frames,
        "frame_clks": frame_clks,
        "totals": totals,
        "util": {k: totals[k] / frame_clks[k] for k in totals},
        "margin": {k: frame_clks[k] - totals[k] for k in totals},
    }


class TestBudgetArithmetic(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.spec = load(SPEC_PATH)
        cls.d00 = load(CONTRACT_PATH)

    def test_frame_facts(self):
        rc = recompute(self.spec)
        self.assertEqual(rc["evals_frame"], 6144)
        self.assertEqual(rc["evals_second"], 4608000)
        self.assertEqual(rc["frames"], 750)
        self.assertEqual(rc["frame_clks"]["12m288"], 16384)
        self.assertEqual(rc["frame_clks"]["24m576"], 32768)

    def test_arbitrary_phase_spi_completion_bounds(self):
        period = self.spec["event_interface"]["min_transaction_period_clks"]
        self.assertEqual(period, 194)
        self.assertEqual(max_completions_half_open(16384, period), 85)
        self.assertEqual(max_completions_half_open(32768, period), 169)
        # Offset W belongs to [W, 2W), never [0, W): 168 * 194 = 32592 < 32768 < 32786.
        self.assertEqual(168 * period, 32592)
        self.assertGreaterEqual(169 * period, 32768)
        self.assertEqual(84 * period, 16296)
        self.assertGreaterEqual(85 * period, 16384)

        rows = {r["id"]: r for r in self.spec["budget_worst_case"]["rows"]}
        drain = rows["write_queue_drain"]
        self.assertEqual(drain["count_12m288"], 85)
        self.assertEqual(drain["count_24m576"], 169)
        self.assertEqual(drain["clocks_12m288"], 85)
        self.assertEqual(drain["clocks_24m576"], 169)

    def test_reachable_queue_and_allocator_worst_mix(self):
        ev = self.spec["event_interface"]
        self.assertEqual(ev["queue_depth_evq"], 168)
        self.assertEqual(ev["rtl_occupied_strike_clks"], 26)
        self.assertTrue(ev["simultaneous_pop_on_full_evq_rejected"])
        self.assertEqual(ev["skid_max_in_flight_compliant"], 1)

        w12 = rtl_max_noteon_pops_from_clean_prior(16384, 194, 168, 26)
        w24 = rtl_max_noteon_pops_from_clean_prior(32768, 194, 168, 26)
        self.assertEqual(
            w12, {"pops": 98, "evq_open": 85, "evq_admitted": 84, "evq_close": 71}
        )
        self.assertEqual(
            w24, {"pops": 194, "evq_open": 168, "evq_admitted": 169, "evq_close": 143}
        )

        rows = {r["id"]: r for r in self.spec["budget_worst_case"]["rows"]}
        self.assertEqual(rows["note_on_application"]["unit_clks"], 224)
        self.assertEqual(rows["note_off_application"]["unit_clks"], 48)
        self.assertEqual(rows["controller_events"]["unit_clks"], 32)
        self.assertEqual(rows["note_on_application"]["count_12m288"], 98)
        self.assertEqual(rows["note_on_application"]["count_24m576"], 194)
        self.assertEqual(rows["note_off_application"]["count"], 0)
        self.assertEqual(rows["controller_events"]["count"], 0)

    def test_row_arithmetic_totals_and_planning_verdicts(self):
        rc = recompute(self.spec)
        t = self.spec["budget_worst_case"]["totals"]
        self.assertEqual(t["clocks_12m288"], rc["totals"]["12m288"])
        self.assertEqual(t["clocks_24m576"], rc["totals"]["24m576"])
        self.assertEqual(t["clocks_12m288"], 34349)
        self.assertEqual(t["clocks_24m576"], 55937)
        self.assertAlmostEqual(t["utilization_12m288"], 34349 / 16384)
        self.assertAlmostEqual(t["utilization_24m576"], 55937 / 32768)
        self.assertEqual(t["margin_12m288"], -17965)
        self.assertEqual(t["margin_24m576"], -23169)
        self.assertEqual(t["verdict_12m288"], "FAIL")
        self.assertEqual(t["verdict_24m576"], "FAIL")

    def test_historical_16_16_16_regression_preserved(self):
        rows = {r["id"]: r for r in self.spec["budget_worst_case"]["rows"]}
        event_ids = {"note_on_application", "note_off_application", "controller_events"}
        non_ev_non_drain = sum(
            r["clocks"]
            for r in rows.values()
            if r["id"] not in event_ids and r["id"] != "write_queue_drain"
        )
        self.assertEqual(non_ev_non_drain, 12312)
        hist_ev = (
            16 * rows["note_on_application"]["unit_clks"]
            + 16 * rows["note_off_application"]["unit_clks"]
            + 16 * rows["controller_events"]["unit_clks"]
        )
        self.assertEqual(hist_ev, 4864)

        reg = self.spec["budget_worst_case"]["historical_regression_vector"]
        self.assertEqual(reg["raw_wire_writes"], 48)
        self.assertEqual(reg["supplied_host_writes"], 160)

        orig = reg["original_whole_period_drain"]
        self.assertEqual(orig["clocks_12m288"], non_ev_non_drain + 84 + hist_ev)
        self.assertEqual(orig["clocks_24m576"], non_ev_non_drain + 168 + hist_ev)
        self.assertEqual(orig["clocks_12m288"], 17260)
        self.assertEqual(orig["clocks_24m576"], 17344)
        self.assertEqual(orig["margin_12m288"], -876)
        self.assertEqual(orig["margin_24m576"], 15424)
        self.assertEqual(orig["verdict_12m288"], "FAIL")
        self.assertEqual(orig["verdict_24m576"], "PASS")

        arb = reg["arbitrary_phase_drain"]
        self.assertEqual(arb["clocks_12m288"], non_ev_non_drain + 85 + hist_ev)
        self.assertEqual(arb["clocks_24m576"], non_ev_non_drain + 169 + hist_ev)
        self.assertEqual(arb["clocks_12m288"], 17261)
        self.assertEqual(arb["clocks_24m576"], 17345)
        self.assertEqual(arb["margin_12m288"], -877)
        self.assertEqual(arb["margin_24m576"], 15423)
        self.assertEqual(arb["verdict_12m288"], "FAIL")
        self.assertEqual(arb["verdict_24m576"], "PASS")

    def test_raw_wire_vs_supplied_host_pricing_and_unresolved_bound(self):
        bw = self.spec["budget_worst_case"]
        raw = bw["raw_wire_witnesses"]
        # EE 100-note-on witness with original non-event rows: 17344 - 4864 + 22400 = 34880 (-2112)
        ee = raw["ee_100_note_on_original_non_event"]
        self.assertEqual(ee["clocks_24m576"], 17344 - 4864 + 100 * 224)
        self.assertEqual(ee["clocks_24m576"], 34880)
        self.assertEqual(ee["margin_24m576"], -2112)
        self.assertEqual(ee["verdict_24m576"], "FAIL")

        # Probe Frame 4 isolated application witnesses (0 drains, 0 patch commit):
        self.assertEqual(
            raw["probe_100_note_on_application_frame"]["clocks_24m576"], 34648
        )
        self.assertEqual(
            raw["probe_100_note_on_application_frame"]["margin_24m576"], -1880
        )
        self.assertEqual(
            raw["probe_168_note_on_application_frame"]["clocks_24m576"], 49880
        )
        self.assertEqual(
            raw["probe_168_note_on_application_frame"]["margin_24m576"], -17112
        )

        # Unresolved paper queue-conservation upper bound (224 > 194 clk):
        ub = raw["unresolved_paper_conservation_upper_bound"]
        self.assertEqual(ub["max_queued_note_ons_24m576"], 337)
        self.assertEqual(ub["clocks_24m576"], 12312 + 169 + 337 * 224)
        self.assertEqual(ub["margin_24m576"], 32768 - 87969)
        self.assertEqual(ub["max_queued_note_ons_12m288_clean_prior"], 169)
        self.assertEqual(ub["clocks_12m288_clean_prior"], 12312 + 85 + 169 * 224)
        self.assertEqual(ub["margin_12m288_clean_prior"], 16384 - 50253)

        # Supplied host (src/gf180_dx7/host.py): 8 writes/note-on, 1/note-off, 1/controller, 1 poll
        host = bw["supplied_host_pricing"]
        self.assertEqual(host["note_on_writes"], 8)
        self.assertEqual(host["note_off_writes"], 1)
        self.assertEqual(host["controller_writes"], 1)
        self.assertEqual(host["status_poll_windows"], 1)
        self.assertEqual(host["write_cap_12m288"], 83)
        self.assertEqual(host["write_cap_24m576"], 167)
        self.assertEqual(
            16 * host["note_on_writes"]
            + 16 * host["note_off_writes"]
            + 16 * host["controller_writes"],
            160,
        )
        self.assertFalse(host["historical_16_16_16_admitted_12m288"])
        self.assertTrue(host["historical_16_16_16_admitted_24m576"])
        # Single-frame host note-off burst (48 clk/write > 224/8 = 28 clk/write):
        self.assertEqual(
            host["single_frame_cost_maximizing"]["clocks_12m288"], 12312 + 85 + 83 * 48
        )
        self.assertEqual(host["single_frame_cost_maximizing"]["margin_12m288"], 3)
        self.assertEqual(
            host["single_frame_cost_maximizing"]["clocks_24m576"],
            12312 + 169 + 167 * 48,
        )
        self.assertEqual(host["single_frame_cost_maximizing"]["margin_24m576"], 12271)
        # Two-frame carry-over + mid-pop replenishment of host note-offs:
        self.assertEqual(
            host["carry_and_replenish_cost_maximizing"]["clocks_12m288"],
            12312 + 85 + 91 * 48,
        )
        self.assertEqual(
            host["carry_and_replenish_cost_maximizing"]["margin_12m288"], -381
        )
        self.assertEqual(
            host["carry_and_replenish_cost_maximizing"]["clocks_24m576"],
            12312 + 169 + 183 * 48,
        )
        self.assertEqual(
            host["carry_and_replenish_cost_maximizing"]["margin_24m576"], 11503
        )
        self.assertEqual(host["verdict_12m288"], "FAIL")
        self.assertEqual(host["verdict_24m576"], "PASS")

    def test_measured_anchor_h01(self):
        rows = {r["id"]: r for r in self.spec["budget_worst_case"]["rows"]}
        self.assertEqual(rows["op_evals"]["unit_class"], "measured")
        self.assertEqual(rows["op_evals"]["unit_clks"], 1)
        report = load(
            os.path.join(REPO, "evidence", "h01-operator-probe", "synth_report.json")
        )
        self.assertEqual(report["cycles_per_operator"]["throughput_clks_per_eval"], 1)


class TestAdmissionProbeEvidenceAndStaleDisposition(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.spec = load(SPEC_PATH)
        cls.probe = load(PROBE_RESULTS_PATH)

    def test_probe_source_hashes_and_conservation(self):
        for rel, digest in self.probe["source_sha256"].items():
            self.assertEqual(sha256_path(os.path.join(REPO, rel)), digest, rel)

        runs = {r["id"]: r for r in self.probe["exercised_runs"]}
        for required_id in (
            "burst_100_sustained",
            "burst_168_sustained",
            "burst_169_sustained",
            "burst_100_reset_patch",
            "phase_max_170",
            "skid_straddle_169",
            "sim_pop_reject_170",
            "carry_replenish_337",
        ):
            self.assertIn(required_id, runs)
            run = runs[required_id]
            self.assertEqual(run["final_sticky_overrun"], 0)
            for fr in run["frames"]:
                # Skid conservation
                drains = fr["drains_sec0"] + fr["drains_sec1"]
                self.assertEqual(
                    fr["skid_open"] + (fr["completions"] - fr["skid_rej"]) - drains,
                    fr["skid_close"],
                )
                # SEC=0 split
                self.assertEqual(fr["drains_sec0"], fr["evq_admit"] + fr["evq_rej"])
                # EVQ conservation
                pops = fr["pops_noteon"] + fr["pops_other"]
                self.assertEqual(
                    fr["evq_open"] + fr["evq_admit"] - pops, fr["evq_close"]
                )
                self.assertEqual(fr["skid_rej"], 0)
                if fr["evq_rej"] > 0:
                    self.assertEqual(fr["rejection_stage"], "event_queue")

        # Specific boundary & rejection checks:
        b169 = {f["frame"]: f for f in runs["burst_169_sustained"]["frames"]}
        self.assertEqual(b169[3]["evq_rej"], 1)
        self.assertEqual(b169[3]["rejection_stage"], "event_queue")
        self.assertEqual(b169[4]["strikes"], 168)
        self.assertEqual(runs["burst_169_sustained"]["final_sticky_overflow"], 1)

        pmax = {f["frame"]: f for f in runs["phase_max_170"]["frames"]}
        self.assertEqual((pmax[3]["first_push"], pmax[3]["last_push"]), (0, 32592))
        self.assertEqual(pmax[3]["completions"], 169)
        self.assertEqual(pmax[4]["first_push"], 18)

        sstr = {f["frame"]: f for f in runs["skid_straddle_169"]["frames"]}
        self.assertEqual(sstr[2]["first_push"], 32767)
        self.assertEqual(sstr[2]["skid_close"], 1)
        self.assertEqual(sstr[3]["skid_open"], 1)
        self.assertEqual(sstr[3]["drains_sec0"], 169)

        simr = {f["frame"]: f for f in runs["sim_pop_reject_170"]["frames"]}
        self.assertEqual(simr[4]["first_push"], 0)  # offset 32768 of Frame 3 -> Frame 4
        self.assertEqual(simr[4]["sim_pop_rej"], 1)
        self.assertEqual(simr[4]["strikes"], 169)

        crep = {f["frame"]: f for f in runs["carry_replenish_337"]["frames"]}
        self.assertEqual(crep[4]["evq_open"], 168)
        self.assertEqual(crep[4]["evq_admit"], 169)
        self.assertEqual(crep[4]["strikes"], 194)
        self.assertEqual(crep[4]["evq_close"], 143)
        self.assertEqual(runs["carry_replenish_337"]["final_sticky_overflow"], 0)

    def test_stale_and_not_run_dispositions_recorded(self):
        disp = self.spec["evidence_disposition"]
        self.assertEqual(
            disp["amendment_record"],
            "docs/decision-records/0015-h03-admission-budget-reconciliation.md",
        )
        self.assertTrue(os.path.isfile(DR0015_PATH))
        self.assertEqual(
            disp["rtl_measured"]["24m576_all_admissible_deadline"], "NO_VERDICT"
        )
        self.assertEqual(
            disp["rtl_measured"]["12m288_rtl_deadline_and_overflow"], "NOT_RUN"
        )
        stale = {s["id"]: s for s in disp["stale_evidence"]}
        self.assertIn("h07_schedule_and_burst", stale)
        self.assertIn("h08_pin_burst_bound", stale)
        for item in stale.values():
            self.assertEqual(item["status"], "STALE")

        not_run = {c["id"]: c for c in self.probe["not_run_cases"]}
        self.assertIn("clk_12m288_rtl_probe", not_run)
        for item in not_run.values():
            self.assertEqual(item["status"], "NOT_RUN")


class TestInterfaceCitations(unittest.TestCase):
    def test_every_cited_dec_id_exists(self):
        spec = load(SPEC_PATH)
        d00 = load(CONTRACT_PATH)
        known = set(d00["decisions"])
        cites = []
        for row in spec["budget_worst_case"]["rows"]:
            cites += [(row["id"], d) for d in row.get("dec_ids", [])]
        for section in (
            "patch_interface",
            "event_interface",
            "status_word",
            "reset_boot",
        ):
            cites += [(section, d) for d in spec[section].get("dec_ids", [])]
        for mod in spec["modules"]:
            cites += [(mod["name"], d) for d in mod.get("dec_ids", [])]
        self.assertTrue(cites)
        for label, dec in cites:
            self.assertIn(dec, known, f"{label} cites unknown {dec}")

    def test_no_per_sample_host_dependency(self):
        spec = load(SPEC_PATH)
        self.assertIn("DEC-010", spec["event_interface"]["dec_ids"])
        with open(DOC_PATH, "r", encoding="utf-8") as f:
            doc = f.read()
        self.assertIn("No per-sample host dependency", doc)


class TestQueueLatencyConstants(unittest.TestCase):
    def test_queue_and_latency_constants_present(self):
        spec = load(SPEC_PATH)
        ev = spec["event_interface"]
        self.assertEqual(ev["queue_depth_skid"], 8)
        self.assertEqual(ev["queue_entry_bits"], 49)
        self.assertEqual(ev["queue_depth_evq"], 168)
        self.assertEqual(ev["drain_rate_writes_per_clk"], 1)
        self.assertEqual(ev["min_transaction_period_clks"], 194)
        lat = spec["latency"]
        self.assertEqual(lat["commit_bound_frames"], 1)
        self.assertEqual(lat["worst_case_to_sdata_samples"], 128)


class TestToolAndControls(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.spec = load(SPEC_PATH)

    def test_budget_check_tool_passes_with_negative_planning_margin(self):
        proc = subprocess.run(
            ["python3", TOOL],
            capture_output=True,
            text=True,
            cwd=REPO,
            timeout=300,
            check=False,
        )
        self.assertEqual(
            proc.returncode, 0, f"tool failed:\n{proc.stdout}\n{proc.stderr}"
        )
        self.assertIn("PASS", proc.stdout)
        self.assertIn("FAIL", proc.stdout)  # both planning verdicts are FAIL

    def test_negative_control_mutated_unit_fails(self):
        bad = copy.deepcopy(self.spec)
        for row in bad["budget_worst_case"]["rows"]:
            if row["id"] == "op_evals":
                row["unit_clks"] = 2
                row["clocks"] = 2 * row["count"]
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(any("op_evals" in f for f in fails), fails)

    def test_negative_control_average_case_budget_fails(self):
        bad = copy.deepcopy(self.spec)
        burst = {"note_on_application", "note_off_application", "controller_events"}
        bad["budget_worst_case"]["rows"] = [
            r for r in bad["budget_worst_case"]["rows"] if r["id"] not in burst
        ]
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(
            any("required worst-case budget row missing" in f for f in fails), fails
        )

    def test_negative_control_restored_whole_period_drain_bound_fails(self):
        """Restoring floor(W / 194) = 84 / 168 must be rejected."""
        bad = copy.deepcopy(self.spec)
        for row in bad["budget_worst_case"]["rows"]:
            if row["id"] == "write_queue_drain":
                row["count_12m288"] = 84
                row["clocks_12m288"] = 84
                row["count_24m576"] = 168
                row["clocks_24m576"] = 168
        bad["budget_worst_case"]["totals"]["clocks_12m288"] -= 1
        bad["budget_worst_case"]["totals"]["margin_12m288"] += 1
        bad["budget_worst_case"]["totals"]["utilization_12m288"] = (
            bad["budget_worst_case"]["totals"]["clocks_12m288"] / 16384.0
        )
        bad["budget_worst_case"]["totals"]["clocks_24m576"] -= 1
        bad["budget_worst_case"]["totals"]["margin_24m576"] += 1
        bad["budget_worst_case"]["totals"]["utilization_24m576"] = (
            bad["budget_worst_case"]["totals"]["clocks_24m576"] / 32768.0
        )
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails, "whole-period 84/168 drain bound must fail")
        self.assertTrue(
            any("write_queue_drain" in f and "85/169" in f for f in fails), fails
        )

    def test_negative_control_restored_all_mixes_pass_24m576_fails(self):
        """Restoring an all-mixes PASS at 24.576 MHz must fail both when flipping
        verdict_24m576 alone and when restoring 16/16/16 counts + PASS."""
        # Case A: flip verdict_24m576 directly
        bad_verdict = copy.deepcopy(self.spec)
        bad_verdict["budget_worst_case"]["totals"]["verdict_24m576"] = "PASS"
        fails_a = h03_budget_check.check_spec(bad_verdict)
        self.assertTrue(fails_a)
        self.assertTrue(any("verdict_24m576" in f for f in fails_a), fails_a)

        # Case B: restore historical 16/16/16 rows and recompute totals as PASS
        bad_rows = copy.deepcopy(self.spec)
        for row in bad_rows["budget_worst_case"]["rows"]:
            if row["id"] == "note_on_application":
                row.pop("count_12m288", None)
                row.pop("clocks_12m288", None)
                row.pop("count_24m576", None)
                row.pop("clocks_24m576", None)
                row["count"] = 16
                row["clocks"] = 16 * 224
            elif row["id"] == "note_off_application":
                row["count"] = 16
                row["clocks"] = 16 * 48
            elif row["id"] == "controller_events":
                row["count"] = 16
                row["clocks"] = 16 * 32
        bad_rows["budget_worst_case"]["totals"].update(
            {
                "clocks_12m288": 17261,
                "utilization_12m288": 17261 / 16384.0,
                "margin_12m288": 16384 - 17261,
                "verdict_12m288": "FAIL",
                "clocks_24m576": 17345,
                "utilization_24m576": 17345 / 32768.0,
                "margin_24m576": 32768 - 17345,
                "verdict_24m576": "PASS",
            }
        )
        fails_b = h03_budget_check.check_spec(bad_rows)
        self.assertTrue(fails_b, "restored 16/16/16 all-mixes PASS must fail")
        self.assertTrue(
            any(
                "34880" in f or "194" in f or "note_on_application" in f
                for f in fails_b
            ),
            fails_b,
        )

    def test_negative_control_fake_pass_verdict_12m288_fails(self):
        bad = copy.deepcopy(self.spec)
        bad["budget_worst_case"]["totals"]["verdict_12m288"] = "PASS"
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(any("verdict_12m288" in f for f in fails), fails)

    def test_negative_control_hash_drift_fails(self):
        bad = copy.deepcopy(self.spec)
        bad["depends_on"]["h01_evidence"]["synth_report"]["sha256"] = "0" * 64
        fails = h03_budget_check.check_spec(bad)
        self.assertTrue(fails)
        self.assertTrue(any("drifted" in f for f in fails), fails)

        bad_probe = copy.deepcopy(self.spec)
        bad_probe["depends_on"]["h03_admission_probe"]["probe_results"]["sha256"] = (
            "0" * 64
        )
        fails_p = h03_budget_check.check_spec(bad_probe)
        self.assertTrue(fails_p)
        self.assertTrue(any("drifted" in f for f in fails_p), fails_p)


class TestDocLockstep(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.spec = load(SPEC_PATH)
        with open(DOC_PATH, "r", encoding="utf-8") as f:
            cls.doc = f.read()

    def test_headline_and_amended_figures_present(self):
        rc = recompute(self.spec)
        self.assertIn("12.288 MHz does NOT close", self.doc)
        self.assertIn(f"{rc['totals']['12m288']:,}", self.doc)
        self.assertIn(f"{rc['totals']['24m576']:,}", self.doc)
        self.assertIn("17,260", self.doc)
        self.assertIn("17,344", self.doc)
        self.assertIn("17,261", self.doc)
        self.assertIn("17,345", self.doc)
        self.assertIn("34,880", self.doc)
        self.assertIn("169", self.doc)
        self.assertIn("85", self.doc)
        self.assertIn("DR-0015", self.doc)
        self.assertIn("STALE", self.doc)
        self.assertIn("NO_VERDICT", self.doc)
        self.assertIn("NOT_RUN", self.doc)

    def test_modules_and_n08_conformance_cases_exist(self):
        bundled = {
            name[:-4] for name in os.listdir(VECTORS_DIR) if name.endswith(".f32")
        }
        self.assertGreaterEqual(len(bundled), 51)
        for mod in self.spec["modules"]:
            self.assertIn(mod["name"], self.doc)
            for case in mod.get("conformance_vectors", []):
                self.assertIn(case, bundled)


if __name__ == "__main__":
    unittest.main()
