#!/usr/bin/env python3
"""H10 gf180 mapped-feasibility report generator with the fit-conclusion gate.

Reads the machine-readable evidence bundle (evidence/h10-gf180/bundle.json,
assembled from committed evidence by tools/h10_collect.py), re-verifies
every evidence hash recorded there, and renders
docs/H10-GF180-FEASIBILITY.md.

THE FIT GATE (issue #32 acceptance item 5 and its negative control):

  A positive "fits <packaging>" conclusion may be emitted ONLY when the
  bundle carries an actually-run, hash-pinned stage chain for THAT
  packaging's fixed die:

      synth   PASS, `mapped: true` (real std cells from a synthesis run --
              an estimate/projection is refused), instance count > 0
      place   PASS and converged on the fixed die
      route   PASS, converged, and a MEASURED detailed-route DRC count of
              exactly 0 (a missing count is refused, never read as 0)
      sta     PASS with a measured setup WNS >= 0 at the headline corner
              (ss) on the target clock

  If any of those is missing, NOT_RUN, lacks verifiable evidence, or
  reports non-convergence / DRC / negative slack, the generator REFUSES:
  --require-fit exits 3; the default report prints
  "NO_VERDICT (fit claim refused: <reasons>)".

  "does not fit" is the area-alone path: permitted when the MEASURED
  mapped std-cell area exceeds the fixed core area even under the most
  favourable measured cell-use policy (the minimum over the bundle's
  policy areas). Cells cannot exceed 100 % of the core, so no placement
  run is needed to establish it; it still requires a mapped synth stage.

Statuses: PASS / FAIL / NOT_RUN / BLOCKED / NO_VERDICT / STALE only.

Exit codes: 0 report written; 3 fit required but refused; 4 bundle/hash
verification failed; 5 bundle schema/field error. Stdlib only.
"""

import argparse
import hashlib
import json
import os
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEADLINE_CORNER = "ss"   # family practice: ss_125C_4v50 decides feasibility
STATUSES = ("PASS", "FAIL", "NOT_RUN", "BLOCKED", "NO_VERDICT", "STALE")
KNOWN_VARIANTS = ("quarterslot", "twoslot", "natdie")
STAGE_NAMES = ("floorplan", "place", "cts", "route", "sta")


class BundleError(Exception):
    """The bundle fails schema verification."""


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def _num(x, what):
    if not isinstance(x, (int, float)) or isinstance(x, bool):
        raise BundleError(f"{what} must be a number, got {x!r}")
    return float(x)


def _is_int(x):
    return isinstance(x, int) and not isinstance(x, bool)


def _check_stage(stage, label, problems, root):
    """One stage dict: status legality + evidence hash verification."""
    if not isinstance(stage, dict):
        problems.append(f"{label}: not an object")
        return
    if stage.get("status") not in STATUSES:
        problems.append(
            f"{label}.status {stage.get('status')!r} not in {STATUSES} "
            "(a test that did not run must never be reported as a pass)")
    ev = stage.get("evidence")
    if stage.get("status") == "PASS" and not ev:
        problems.append(f"{label} is PASS without hash-pinned evidence")
    for rel, digest in (ev or {}).items():
        path = os.path.join(root, rel)
        if not os.path.isfile(path):
            problems.append(f"{label}.evidence: {rel} not found")
            continue
        actual = sha256_file(path)
        if actual != digest:
            problems.append(f"{label}.evidence: {rel} sha256 mismatch "
                            f"(recorded {digest[:12]}..., actual "
                            f"{actual[:12]}...)")


def verify_bundle(bundle, root):
    """Schema + hash verification. Returns a list of problems (empty = ok)."""
    problems = []
    for key in ("design", "target_clock_mhz", "clock_period_ns",
                "packaging_budgets", "synth", "stages", "family_context"):
        if key not in bundle:
            problems.append(f"bundle missing key: {key}")
    if problems:
        return problems
    if bundle["synth"] is not None:
        _check_stage(bundle["synth"], "synth", problems, root)
    if bundle.get("orfs_synth") is not None:
        _check_stage(bundle["orfs_synth"], "orfs_synth", problems, root)
    for name, st in (bundle.get("synth_sta") or {}).items():
        _check_stage(st, f"synth_sta.{name}", problems, root)
    for variant, vstages in (bundle["stages"] or {}).items():
        if variant not in KNOWN_VARIANTS:
            problems.append(f"stages: unknown variant {variant!r}")
            continue
        for name in STAGE_NAMES:
            st = (vstages or {}).get(name)
            if st is None:
                continue  # absent stage -> NOT_RUN (the fit gate handles it)
            _check_stage(st, f"stages.{variant}.{name}", problems, root)
    return problems


def _stage_ok(stage, root):
    """True iff stage PASSED with present, hash-matching evidence."""
    if not isinstance(stage, dict) or stage.get("status") != "PASS":
        return False
    ev = stage.get("evidence") or {}
    if not ev:
        return False
    return all(os.path.isfile(os.path.join(root, rel))
               and sha256_file(os.path.join(root, rel)) == dig
               for rel, dig in ev.items())


def _synth_is_mapped(synth):
    """A synth stage counts only if it is a real mapped result."""
    if not isinstance(synth, dict) or synth.get("status") != "PASS":
        return False, "synthesis stage did not pass -- nothing is mapped"
    if synth.get("mapped") is not True:
        return False, ("synthesis stage is not marked as a mapped real-cell "
                       "result (an estimate or projection is refused)")
    n = synth.get("instance_count")
    if not _is_int(n) or n <= 0:
        return False, f"synthesis instance count {n!r} is not a positive int"
    return True, ""


def most_favourable_area(synth):
    """The smallest measured mapped area over the recorded policies."""
    areas = [a for a in (synth.get("policy_areas_um2") or {}).values()
             if isinstance(a, (int, float)) and not isinstance(a, bool)]
    base = _num(synth.get("mapped_stdcell_area_um2"),
                "synth.mapped_stdcell_area_um2")
    return min(areas + [base])


def fit_gate(bundle, packaging, root=REPO_ROOT):
    """The negative control. Returns (verdict, reasons); verdict is one of
    "fits", "does not fit", "NO_VERDICT"."""
    pk = (bundle.get("packaging_budgets") or {}).get(packaging)
    if not pk:
        return "NO_VERDICT", [f"no declared fixed-die budget for {packaging!r}"]
    synth = bundle.get("synth") or {}
    ok, why = _synth_is_mapped(synth)
    if not ok:
        return "NO_VERDICT", [why]
    if not _stage_ok(synth, root):
        return "NO_VERDICT", ["synth stage evidence missing or drifted"]
    core_area = _num(pk.get("core_area_um2"), f"{packaging}.core_area_um2")
    best = most_favourable_area(synth)
    if best > core_area:
        reasons = [f"measured mapped std-cell area {best:,.0f} um^2 (the "
                   f"most favourable measured policy/recipe) exceeds the "
                   f"fixed core area {core_area:,.0f} um^2 = "
                   f"{100.0 * best / core_area:.1f} % cell utilisation "
                   "before any placement overhead; established from mapped "
                   "cells alone (no place/route stage needed or claimed)"]
        fp = ((bundle.get("stages") or {}).get(packaging) or {}) \
            .get("floorplan") or {}
        if _stage_ok({**fp, "status": "PASS"}, root) and \
                isinstance(fp.get("utilisation_pct"), (int, float)):
            reasons.append(f"corroborated by the pinned ORFS floorplan on "
                           f"this die: measured utilisation "
                           f"{fp['utilisation_pct']:.1f} %")
        return "does not fit", reasons
    reasons = []
    vst = (bundle.get("stages") or {}).get(packaging) or {}
    if not _stage_ok(vst.get("place"), root):
        reasons.append(f"place stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    elif vst["place"].get("converged") is not True:
        reasons.append("placement did not CONVERGE on the fixed die")
    if not _stage_ok(vst.get("route"), root):
        reasons.append(f"route stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    else:
        rt = vst["route"]
        if rt.get("converged") is not True:
            reasons.append("detailed route did not converge")
        drc = rt.get("drc_errors")
        if not _is_int(drc):
            reasons.append(f"route DRC count not measured (drc_errors="
                           f"{drc!r}); a missing count is never read as 0")
        elif drc != 0:
            reasons.append(f"route has {drc} detailed-route DRC errors")
    if not _stage_ok(vst.get("sta"), root):
        reasons.append(f"sta stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    else:
        c = (vst["sta"].get("corners") or {}).get(HEADLINE_CORNER) or {}
        wns = c.get("setup_wns_ns")
        if not isinstance(wns, (int, float)) or isinstance(wns, bool):
            reasons.append(f"no measured setup slack at headline corner "
                           f"{HEADLINE_CORNER!r}")
        elif wns < 0:
            reasons.append(f"setup slack {wns:.3f} ns is NEGATIVE at the "
                           f"headline corner {HEADLINE_CORNER!r} on the "
                           "target clock -- timing does not close")
    if reasons:
        return "NO_VERDICT", reasons
    return "fits", []


# ------------------------------------------------------------- render ---
def _mm2(x):
    return f"{x / 1e6:,.3f} mm²" if isinstance(x, (int, float)) else "n/a"


def _um2(x):
    return f"{x:,.0f} µm²" if isinstance(x, (int, float)) else "n/a"


def _num_or(x, fmt, default="n/a"):
    return format(x, fmt) if isinstance(x, (int, float)) and \
        not isinstance(x, bool) else default


def render_report(bundle, verdicts):
    b = bundle
    L = []
    add = L.append
    s = b.get("synth") or {}
    h = b.get("policy_synth") or {}
    o = b.get("orfs_synth") or {}
    stages = b.get("stages") or {}
    period = b["clock_period_ns"]
    ran = sorted(f"{v}.{k}" for v, vst in stages.items() for k in vst)
    add("# H10 — gf180mcu mapped feasibility report (16-note DX7 core)")
    add("")
    add("Issue #32 (H10). Generated by `tools/h10_report.py` from "
        "`evidence/h10-gf180/bundle.json`; every evidence hash is "
        "re-verified at generation (drift exits 4 and writes nothing). "
        f"Design `{b['design']}` (refrozen core, DR-0011). Target clock "
        f"**{b['target_clock_mhz']} MHz ({period} ns)**, the DR-0008 "
        "design point.")
    add("")
    sup = b.get("superseded_by")
    if sup:
        add(f"> **{sup.get('status', 'STALE')} — superseded by "
            f"`{sup.get('decision_record')}` (issue #{sup.get('issue')}).** "
            f"{sup.get('reason')} Core pin measured here: "
            f"`{str(sup.get('core_pin_measured'))[:8]}…`; current frozen pin: "
            f"`{str(sup.get('core_pin_current'))[:8]}…`.")
        add("")
    add("> **Scope.** Mapped synthesis (real gf180mcu 7t cells, pinned ORFS "
        "flow + a two-policy yosys A/B), synthesis-stage static timing, and ORFS "
        "floorplan on the fixed dies"
        + (f" (stages run: {', '.join(ran)})" if ran else "")
        + ". No placement, CTS or routing ran. No silicon, board, FPGA, "
        "signoff, original-hardware or musical claim.")
    add("")
    # ---- 0. headline
    add("## 0. Headline findings")
    add("")
    if isinstance(s.get("mapped_stdcell_area_um2"), (int, float)):
        best = most_favourable_area(s)
        q = b["packaging_budgets"].get("quarterslot", {})
        qs = verdicts.get("quarterslot", ("NO_VERDICT", []))[0]
        ts = verdicts.get("twoslot", ("NO_VERDICT", []))[0]
        add(f"1. **Area.** The mapped 16-note core is "
            f"**{_mm2(s['mapped_stdcell_area_um2'])}** of standard cells in "
            f"the pinned flow ({s.get('instance_count'):,} instances); the "
            f"smallest area measured under any policy/recipe is "
            f"**{_mm2(best)}**. The quarter-slot core is "
            f"{_mm2(q.get('core_area_um2'))}: the cells alone are "
            f"**{best / q['core_area_um2']:.1f}×** it. Quarter slot: "
            f"**{qs}**; two slots: **{ts}**.")
    else:
        add("1. No mapped synthesis result is present: **NO_VERDICT**.")
    ssta = b.get("synth_sta") or {}
    prim = ssta.get("orfs_natdie") or next(iter(ssta.values()), {})
    ss = (prim.get("corners") or {}).get("ss")
    if ss:
        add(f"2. **Timing.** At the synthesis stage — ideal clock and zero "
            f"wire load (optimistic), but before any timing repair "
            f"(resizing/buffering can shorten paths) — the worst setup "
            f"slack at ss is "
            f"**{ss['setup_wns_ns']:+.3f} ns** on the {period} ns clock "
            f"(implied minimum period {ss['implied_min_period_ns']:.1f} ns "
            f"≈ {ss['implied_fmax_mhz']} MHz). That is also longer than the "
            "12.288 MHz fallback period (81.38 ns). This is a measured "
            "property of the mapped logic depth, not a closure verdict: "
            "the resizer, placement and routing all move it (section 2).")
    add("3. **Escalation.** Both findings go to the owner (issue #32 "
        "stop/escalate). The 16-note goal and the 24.576 MHz design point "
        "are **not** weakened here; which architecture/requirement changes "
        "is an owner decision (section 4).")
    add("")
    # ---- 1. mapped results
    add("## 1. Mapped results (measured, not estimated)")
    add("")
    add("| run | recipe / policy | std-cell area | instances | flip-flops "
        "| sequential area | status |")
    add("|---|---|---:|---:|---:|---:|---|")
    if o.get("status"):
        add(f"| pinned ORFS flow (primary) | {o.get('policy', 'n/a')} | "
            f"{_um2(o.get('mapped_stdcell_area_um2'))} | "
            f"{_num_or(o.get('instance_count'), ',')} | "
            f"{_num_or(o.get('flop_count'), ',')} | "
            f"{_um2(o.get('seq_area_um2'))} | **{o['status']}** |")
    for p, a in sorted((h.get("policy_areas_um2") or {}).items()):
        add(f"| yosys (policy A/B) | `{p}` | {_um2(a)} | "
            f"{_num_or((h.get('policy_instances') or {}).get(p), ',')} | "
            f"{_num_or((h.get('policy_flops') or {}).get(p), ',')} | n/a | "
            f"**{h.get('status')}** |")
    if not h.get("policy_areas_um2"):
        add(f"| yosys (policy A/B) | both policies | n/a | n/a | n/a | n/a | "
            f"**{h.get('status', 'NOT_RUN')}** |")
    add("")
    add(f"- Pinned flow: {o.get('method', 'n/a')}; yosys `check`: "
        f"{o.get('check', 'n/a')}.")
    for name, r in sorted(((o.get("run") or {}).get("results_not_committed")
                           or {}).items()):
        add(f"  - `{name}`: sha256 `{r['sha256']}`, {r['bytes']:,} B (not "
            "committed; regenerate with `asic/orfs/run-orfs.sh`)")
    add(f"- Policy A/B: {h.get('method', 'n/a')}.")
    for p, n in sorted((h.get("netlists") or {}).items()):
        add(f"  - `{p}` netlist: sha256 `{n['sha256']}`, {n['bytes']:,} B "
            "(not committed; regenerate with `tools/h10_synth.py`)")
    add("- All areas are liberty cell areas at `tt_025C_5v00`. The two "
        "recipes differ (the ORFS script runs `extract_fa` and maps adders "
        "to `addf_1`/`addh_1` through its techmap even under the `*_1` "
        "exclusion); compare policies within one recipe only.")
    add("")
    mods = h.get("module_local_area_um2") or {}
    subs = h.get("top_submodule_instances") or {}
    tot = (h.get("policy_areas_um2") or {}).get("orfs_stock")
    if mods and isinstance(tot, (int, float)):
        add("Area attribution (policy A/B run, `orfs_stock`, hierarchy kept: "
            "module-local area × instances in the top):")
        add("")
        add("| module | local area | instances | total | share |")
        add("|---|---:|---:|---:|---:|")
        top = b["design"]
        rows = [(top, mods.get(top), 1)] + [
            (m, mods.get(m), n) for m, n in sorted(subs.items())]
        for m, a, n in rows:
            if not isinstance(a, (int, float)):
                continue
            add(f"| `{m}` | {_um2(a)} | {n} | {_um2(a * n)} | "
                f"{100.0 * a * n / tot:.1f} % |")
        add("")
    add("| die | floorplan | place | CTS | route | post-route STA |")
    add("|---|---|---|---|---|---|")
    for variant in KNOWN_VARIANTS:
        vst = stages.get(variant) or {}
        cells = [f"**{vst[n]['status']}**" if vst.get(n) else "NOT_RUN"
                 for n in STAGE_NAMES]
        add(f"| {variant} | " + " | ".join(cells) + " |")
    add("")
    for variant in KNOWN_VARIANTS:
        fp = (stages.get(variant) or {}).get("floorplan")
        if fp and fp.get("note"):
            add(f"- {variant} floorplan: {fp['note']} "
                f"({_um2(fp.get('instance_area_um2'))} of cells, core "
                f"{_um2(fp.get('core_area_um2'))}, ORFS "
                "`floorplan__design__instance__utilization`).")
    add("")
    # ---- 2. timing
    add("## 2. Worst-case timing vs the target clock, per stage")
    add("")
    add(f"Target clock {period} ns. Every stage that ran is listed; stages "
        "that did not run are NOT_RUN. Only post-route timing could decide "
        "closure, and it does not exist.")
    add("")
    add("| stage | netlist / die | corner | setup WNS (ns) | setup TNS (ns) "
        "| hold WNS (ns) | conditions | status |")
    add("|---|---|---|---:|---:|---:|---|---|")
    for name, st in sorted(ssta.items()):
        for c in ("ss", "tt", "ff"):
            cv = (st.get("corners") or {}).get(c)
            if not cv:
                continue
            # The STA *run* completing is not the timing check passing:
            # report the setup check against the target clock, and only
            # fall back to the run status when the run itself did not pass.
            if st.get("status") != "PASS":
                verdict = f"**{st.get('status')}** (STA run)"
            elif cv["setup_wns_ns"] < 0:
                verdict = "**FAIL** (setup vs target; STA ran)"
            else:
                verdict = "**PASS** (setup vs target; STA ran)"
            add(f"| synthesis | `{name}` | {c} | {cv['setup_wns_ns']:+.3f} | "
                f"{cv['setup_tns_ns']:.3f} | {cv['hold_wns_ns']:+.3f} | "
                f"ideal clock, no wires, no resizing | {verdict} |")
    for variant in KNOWN_VARIANTS:
        fp = (stages.get(variant) or {}).get("floorplan")
        if not fp:
            continue
        add(f"| floorplan | {variant} | tt | "
            f"{_num_or(fp.get('setup_wns_ns'), '+.3f')} | "
            f"{_num_or(fp.get('setup_tns_ns'), '.3f')} | "
            f"{_num_or(fp.get('hold_wns_ns'), '+.3f')} | wire-load models, "
            "after the floorplan resizer (flow CORNER=TC) | "
            f"**{fp['status']}** (stage ran; die cannot hold the cells) |")
    add("| place / CTS / route / post-route | any | ss/tt/ff | — | — | — | "
        "— | **NOT_RUN** |")
    add("")
    if not ssta:
        add("Synthesis-stage STA: **NOT_RUN**.")
        add("")
    wp = prim.get("worst_path_ss") if prim else None
    if wp:
        add(f"Worst ss setup path at the synthesis stage: `{wp}`.")
        add("")
    # ---- 3. budget
    add("## 3. Die / pad / margin budget, per packaging")
    add("")
    for pk, pv in b["packaging_budgets"].items():
        verdict, reasons = verdicts.get(pk, ("NO_VERDICT", ["unjudged"]))
        rem = None
        if isinstance(s.get("mapped_stdcell_area_um2"), (int, float)):
            rem = pv["core_area_um2"] - most_favourable_area(s)
        add(f"- **{pk}**: die {pv['die_um_w']} × {pv['die_um_h']} µm "
            f"({_um2(pv.get('die_area_um2'))}), core "
            f"{_um2(pv['core_area_um2'])} — {pv.get('source')}. Core "
            f"remaining after mapped cells (most favourable measured area): "
            f"**{_um2(rem)}**. Verdict: **{verdict}**"
            + ("" if verdict in ("fits", "does not fit") else
               " (fit claim refused)"))
        for r in reasons:
            add(f"  - {r}")
    add("")
    add("**Explicitly unaccounted for** (none of these is subtracted above; "
        "each only makes the margin worse):")
    add("")
    for u in b.get("unaccounted", []):
        add(f"- {u}")
    add("")
    # ---- 4. 16-note
    add("## 4. The 16-note requirement")
    add("")
    add("The synthesised unit is the committed 16-note core: `dx7_core` "
        "instantiates `env_unit` 96× (16 notes × 6 operators, "
        "`rtl/dx7_core.v` genvar `ge < 96`). No decision record revises the "
        "16-note goal; DR-0001 forbids silently weakening it. The area and "
        "timing findings are reported against the retained goal and "
        "escalated. Options such as time-multiplexing envelope state, "
        "SRAM-backed state, pipelining the voice-allocation search, a "
        "larger die, or a polyphony ruling are owner decisions; none was "
        "synthesised here, so none is claimed to fit.")
    add("")
    # ---- 5. family
    add("## 5. Context vs the family's measured data points")
    add("")
    fx = b.get("family_context") or {}
    sj = fx.get("sibling_joined") or {}
    sp = fx.get("sibling_placeholder") or {}
    if sj:
        add(f"- Sibling joined chip ({sj['design']}): {sj['instances']:,} "
            f"instances / {_mm2(sj['cell_area_um2'])} cells = "
            f"{sj['of_quarterslot_pct']} % of the quarter-slot core (does "
            f"not place); on the two-slot die {sj['twoslot_util_pct']} % "
            f"utilisation, {sj['drc_errors']} detailed-route DRC, ss setup "
            f"{sj['ss_setup_wns_ns']:+.3f} ns at {sj['clock_mhz']} MHz "
            f"({sj['clock_period_ns']} ns; implied ss fmax "
            f"{sj['ss_implied_fmax_mhz']} MHz). Policy: {sj['policy']}. "
            f"Provenance: {sj['provenance']}.")
    if sp:
        add(f"- Sibling placeholder chip: {sp['instances']:,} instances / "
            f"{_mm2(sp['cell_area_um2'])}, routed in the quarter slot at "
            f"{sp['quarterslot_routed_util_pct']} % ({sp['provenance']}).")
    if sj and isinstance(s.get("mapped_stdcell_area_um2"), (int, float)):
        add(f"- This core vs the sibling joined chip, same flow image and "
            f"stock policy: **{s['mapped_stdcell_area_um2'] / sj['cell_area_um2']:.1f}×"
            f"** the cell area, **{s['instance_count'] / sj['instances']:.1f}×"
            "** the instances — and the joined chip already did not fit a "
            "quarter slot. The sibling's clock (12.288 MHz) is half this "
            "core's target; its ss closure does not transfer.")
    pol = fx.get("dont_use_policy") or {}
    eff = h.get("dont_use_effect") or {}
    sp_pct = pol.get("sibling_pct") or ["?", "?"]
    add(f"- **DONT_USE_CELLS policy.** The pinned image's gf180 platform "
        f"excludes drive-1 cells ({pol.get('policy_source', 'n/a')}); the "
        "sibling's klt/local-script flows allowed them. Sibling measured "
        f"effect: +{sp_pct[0]} % / +{sp_pct[1]} % "
        f"({pol.get('sibling_detail', '')}; {pol.get('provenance', '')}).")
    if eff:
        add(f"  - **This core, measured** (tools/h10_synth.py; {eff.get('basis')}): "
            f"`orfs_stock` {_um2(eff['orfs_stock_um2'])} vs `x1_allowed` "
            f"{_um2(eff['x1_allowed_um2'])} = **{eff['pct']:+.1f} %**. Even "
            "the permissive policy leaves the core far above both dies, so "
            "the policy choice changes no verdict here.")
    else:
        add("  - This core's own policy A/B: **NOT_RUN**.")
    add("")
    # ---- 6. other findings
    findings = b.get("findings") or []
    if findings:
        add("## 6. Other findings from this run")
        add("")
        for fnd in findings:
            add(f"- {fnd}")
        add("")
    # ---- 7. acceptance
    add("## 7. Acceptance items of issue #32")
    add("")
    add("| item | status | evidence |")
    add("|---|---|---|")
    for row in b.get("acceptance_rows", []):
        add(f"| {row[0]} | **{row[1]}** | {row[2]} |")
    add("")
    add("## 8. What remains unproved")
    add("")
    for u in b.get("unproved", []):
        add(f"- {u}")
    add("")
    add("---")
    add("*Regenerate: `python3 tools/h10_collect.py && python3 "
        "tools/h10_report.py`. The fit gate refuses any positive fit "
        "without a full RUN-evidenced chain (live tests: "
        "`tests/test_h10_report.py`).*")
    return "\n".join(L) + "\n"


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--bundle", default=os.path.join(
        REPO_ROOT, "evidence", "h10-gf180", "bundle.json"))
    ap.add_argument("--out", default=os.path.join(
        REPO_ROOT, "docs", "H10-GF180-FEASIBILITY.md"))
    ap.add_argument("--root", default=None,
                    help="root for resolving evidence paths (tests)")
    ap.add_argument("--require-fit", action="store_true",
                    help="exit 3 unless every packaging's fit gate passes")
    ap.add_argument("--check", action="store_true",
                    help="verify only: exit 1 if --out differs from what "
                         "would be generated (STALE)")
    args = ap.parse_args(argv)
    with open(args.bundle) as f:
        bundle = json.load(f)
    root = args.root or REPO_ROOT
    problems = verify_bundle(bundle, root)
    if problems:
        for p in problems:
            print(f"BUNDLE-CHECK FAIL: {p}", file=sys.stderr)
        return 4
    try:
        verdicts = {pk: fit_gate(bundle, pk, root)
                    for pk in bundle["packaging_budgets"]}
        text = render_report(bundle, verdicts)
    except (BundleError, KeyError, TypeError) as e:
        print(f"BUNDLE-SCHEMA FAIL: {e!r}", file=sys.stderr)
        return 5
    if args.require_fit:
        refused = {pk: r for pk, (v, r) in verdicts.items() if v != "fits"}
        if refused:
            for pk, reasons in refused.items():
                print(f"FIT REFUSED for {pk}: " + "; ".join(reasons),
                      file=sys.stderr)
            return 3
    if args.check:
        cur = open(args.out).read() if os.path.isfile(args.out) else ""
        if cur != text:
            print(f"STALE: {args.out} differs from the bundle's rendering",
                  file=sys.stderr)
            return 1
        print(f"FRESH: {args.out}")
        return 0
    with open(args.out, "w") as f:
        f.write(text)
    for pk, (v, r) in verdicts.items():
        print(f"{pk}: {v}" + (f" -- {r[0]}" if r else ""))
    print(f"report: {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
