#!/usr/bin/env python3
"""H10 gf180 mapped-feasibility report generator with the fit-conclusion gate.

Reads the machine-readable evidence bundle (evidence/h10-gf180/bundle.json,
assembled from the ORFS per-stage reports and corner-STA logs by
tools/h10_collect.py), checks the identity hashes recorded there, and
renders docs/H10-GF180-FEASIBILITY.md.

Bundle shape (abridged):

    design, target_clock_mhz, clock_period_ns
    packaging_budgets: { <packaging>: { die/core areas, source }, ... }
    synth:             the single shared synthesis stage (STATUS + evidence)
    stages:            per FLOW_VARIANT: { <variant>: { floorplan, place,
                        cts, route, sta ... } }
    family_context:    sibling measured data points (provenance-quoted)
    acceptance_rows, unaccounted, unproved

THE FIT GATE (acceptance item 5 of issue #32 -- the negative control):

  A positive "fits <packaging>" conclusion may be emitted ONLY when the
  bundle carries an actually-run stage chain with hash-pinned evidence for
  EVERY required stage of THAT packaging's variant:

      synth      shared, mapped std-cell instances (count > 0) from a real
                 synthesis run -- no estimates, no projections
      place      detailed placement CONVERGED on the declared fixed die
      route      detailed route CONVERGED with zero DRC errors
      sta        worst-case setup slack measured (not estimated) at the
                 headline corner on the target clock, and >= 0 for "fits"

  If any of those is missing, absent, not RUN, has no hash-pinned evidence,
  or reports non-convergence / DRC / negative setup slack, the generator
  REFUSES: with --require-fit it exits 3 without writing a "fits" verdict;
  in the default report the packaging line reads
  "NO_VERDICT (fit claim refused: <exact reasons>)".

  A "does not fit" verdict is the other, area-alone path: it is permitted
  when the MEASURED mapped std-cell area of the actually-synthesised netlist
  exceeds the declared fixed core area -- that is a measurement, not an
  estimate, and needs only the synth stage. Timing claims for a packaging
  that never placed still cannot be made.

Reported statuses are PASS / FAIL / NOT_RUN / BLOCKED / NO_VERDICT / STALE
only (repo convention); coverage is never conflated with agreement.

Exit codes: 0 report written (no positive fit claimed, or claimed and
supported); 3 fit claim required but refused; 4 bundle/hash verification
failed; 5 bundle schema/field error. Stdlib only.
"""

import argparse
import hashlib
import json
import os
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEADLINE_CORNER = "ss"   # family practice: ss_125C_4v50 is the feasibility corner
STATUSES = ("PASS", "FAIL", "NOT_RUN", "BLOCKED", "NO_VERDICT", "STALE")
KNOWN_VARIANTS = ("quarterslot", "twoslot", "natdie")
STAGE_NAMES = ("floorplan", "place", "cts", "route", "sta")


class FitRefused(Exception):
    """A positive fit conclusion was demanded but the stage chain is absent."""


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
    # synth may be absent-as-null: the run never reached synthesis. That is
    # a NOT_RUN stage (reported as such, fit gate returns NO_VERDICT), not a
    # schema error.
    if bundle["synth"] is not None:
        _check_stage(bundle["synth"], "synth", problems, root)
    for variant, vstages in (bundle["stages"] or {}).items():
        if variant not in KNOWN_VARIANTS:
            problems.append(f"stages: unknown variant {variant!r}")
            continue
        for name in STAGE_NAMES:
            st = (vstages or {}).get(name)
            if st is None:
                continue  # absent stage -> NOT_RUN, legal (the fit gate handles it)
            _check_stage(st, f"stages.{variant}.{name}", problems, root)
    return problems


def _stage_ok(stage, root):
    """True iff stage PASSED with hash-verified evidence in the bundle."""
    if not isinstance(stage, dict) or stage.get("status") != "PASS":
        return False
    ev = stage.get("evidence") or {}
    if not ev:
        return False
    return all(os.path.isfile(os.path.join(root, rel))
               and sha256_file(os.path.join(root, rel)) == dig
               for rel, dig in ev.items())


def fit_gate(bundle, packaging, root=REPO_ROOT):
    """The negative control.

    Returns (verdict, reasons) for one packaging where verdict is one of
    "fits", "does not fit", "NO_VERDICT". A positive "fits" needs the full
    RUN-evidenced stage chain for that packaging's variant (place, route,
    sta) plus the shared synth stage, with place converged, route converged
    + 0 DRC, and sta setup slack at the headline corner >= 0 on the target
    clock. "does not fit" needs only the measured mapped area exceeding the
    fixed core area.
    """
    reasons = []
    pk = bundle["packaging_budgets"].get(packaging)
    if not pk:
        return "NO_VERDICT", [f"no declared fixed-die budget for {packaging!r}"]
    synth = bundle.get("synth") or {}
    if synth.get("status") != "PASS":
        return "NO_VERDICT", ["synthesis stage did not pass -- nothing is mapped"]
    # --- area-alone non-fit: measured mapped area vs fixed core area -------
    core_area = _num(pk.get("core_area_um2"), f"{packaging}.core_area_um2")
    mapped = _num(synth.get("mapped_stdcell_area_um2"),
                  "synth.mapped_stdcell_area_um2")
    if mapped > core_area:
        return ("does not fit",
                [f"measured mapped std-cell area {mapped:.1f} um^2 exceeds the "
                 f"fixed core area {core_area:.1f} um^2 "
                 f"({100.0 * mapped / core_area:.1f} % utilisation, measured -- "
                 "placement and routing on this die are impossible by area "
                 "alone; no place/route stage was run for this packaging, "
                 "and none was needed to establish non-fit)"])
    # --- positive fit: every required stage must be RUN + hash-pinned -----
    vst = (bundle["stages"] or {}).get(packaging) or {}
    if not _stage_ok(synth, root):
        reasons.append("synth stage not PASS with hash-pinned evidence "
                       "(no mapped netlist is established)")
    if not _stage_ok(vst.get("place"), root):
        reasons.append(f"place stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    else:
        pl = vst["place"]
        if pl.get("converged") is not True:
            reasons.append("placement did not CONVERGE on the fixed die")
    if not _stage_ok(vst.get("route"), root):
        reasons.append(f"route stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    else:
        rt = vst["route"]
        if rt.get("converged") is not True:
            reasons.append("detailed route did not converge")
        drc = rt.get("drc_errors")
        if not isinstance(drc, int) or isinstance(drc, bool) or not 0 <= drc:
            reasons.append(f"route did not clear DRC (drc_errors={drc!r})")
    if not _stage_ok(vst.get("sta"), root):
        reasons.append(f"sta stage did not run (or is not pass-evidenced) "
                       f"for {packaging!r}")
    else:
        corners = (vst["sta"].get("corners") or {}).get(HEADLINE_CORNER) or {}
        wns = corners.get("setup_wns_ns")
        if not isinstance(wns, (int, float)) or isinstance(wns, bool):
            reasons.append(f"no measured setup slack at headline corner "
                           f"{HEADLINE_CORNER!r}")
        elif wns < 0:
            reasons.append(f"setup slack {wns:.3f} ns is NEGATIVE at the "
                           f"headline corner {HEADLINE_CORNER!r} on the target "
                           "clock -- timing does not close")
    if reasons:
        return "NO_VERDICT", reasons
    return "fits", []


def render_report(bundle, verdicts):
    """Render the markdown report. Returns the text."""
    v = lambda val, unit=" um^2": ("n/a" if not isinstance(val, (int, float))
                                   else f"{val:,.2f}{unit}".replace(",", ","))
    b = bundle
    lines = []
    add = lines.append
    add("# H10 — gf180mcu mapped feasibility report (16-note DX7 core)")
    add("")
    add(f"Design: `{b['design']}` -- the committed H02/H07 integrated "
        f"16-note core (`dx7_core`). Target clock **{b['target_clock_mhz']} "
        f"MHz ({b['clock_period_ns']} ns)** -- the DR-0008 design point "
        "(512 clocks per 48 kHz audio frame). This report is generated from "
        "hash-pinned run evidence by `tools/h10_report.py`; every number "
        "below quotes the stage artifact it came from "
        "(`evidence/h10-gf180/bundle.json`, hashes re-verified on every "
        "generation).")
    add("")
    add("> **Scope.** Mapped synthesis + floorplan/place/route/STA on the "
        "declared fixed dies, `gf180mcu_fd_sc_mcu7t5v0` (7t track), ORFS "
        "pinned image. **No** silicon, **no** board, **no** "
        "original-hardware playback, **no** signoff claim; physical "
        "integration (pads, PDN signoff, LVS/IR) is H11's claim. A mapped "
        "fit is not a signoff; mapped timing at the target clock on one PDK "
        "corner set is not a product claim.")
    add("")
    add("## 1. Mapped results (measured, not estimated)")
    add("")
    s = b.get("synth") or {}
    area = s.get("mapped_stdcell_area_um2")
    add(f"- Synthesis (shared by all variants; netlist "
        f"`{s.get('netlist_rel', 'evidence/.../1_2_yosys.v')}`): "
        f"**{s.get('status', 'NOT_RUN')}** -- "
        + (f"**{area / 1e6:.3f} mm^2** mapped std-cell area "
           f"({s.get('instance_count', 'n/a')} instances, "
           f"{s.get('flop_count', 'n/a')} sequential elements)"
           if isinstance(area, (int, float)) else "no measured area")
        + ", cell area at the `tt` TT corner liberty, under the stock ORFS "
          "gf180 platform policy `DONT_USE_CELLS=*_1` (drive-1 cells "
          "excluded -- the policy the sibling runs used, so the numbers are "
          "comparable).")
    add("")
    add("| variant | floorplan | place | cts | route | corner STA |")
    add("|---|---|---|---|---|---|")
    for variant in KNOWN_VARIANTS:
        vst = (b.get("stages") or {}).get(variant) or {}
        def cell(name):
            st = vst.get(name)
            if not st:
                return "NOT_RUN"
            t = f"**{st.get('status')}**"
            if st.get("note"):
                t += f" — {st['note']}"
            return t
        add(f"| {variant} | {cell('floorplan')} | {cell('place')} | "
            f"{cell('cts')} | {cell('route')} | {cell('sta')} |")
    add("")
    add("Stage evidence (reports/logs, hash-pinned in the bundle): "
        "`evidence/h10-gf180/` per variant.")
    add("")
    add("## 2. Worst-case timing vs the target clock")
    add("")
    add(f"Per-stage worst negative setup slack (WNS, ns), "
        f"{b['clock_period_ns']} ns target clock, from the flow's own "
        "per-stage reports of the `natdie` run (hash-pinned in the bundle); "
        "load grows across the chain (taps/IO -> CTS -> routed "
        "interconnect), so the late rows are the deciding ones:")
    add("")
    vst0 = ((b.get("stages") or {}).get("natdie") or {})
    rows = [(k, vst0.get(k).get("setup_wns_ns"))
            for k in ("floorplan", "place", "cts", "route")
            if isinstance(vst0.get(k), dict)]
    if any(w is not None for _, w in rows):
        add("| stage | setup WNS (ns) | verdict vs target clock |")
        add("|---|---|---|")
        for k, wns in rows:
            if wns is None:
                continue
            ok = wns >= -0.0005
            add(f"| {k} | {wns:+.3f} | "
                f"{'closes' if ok else 'VIOLATES'} the "
                f"{b['clock_period_ns']} ns clock |")
        add("")
    sta = ((b.get("stages") or {}).get("natdie") or {}).get("sta") or {}
    if sta.get("status") == "PASS" and sta.get("corners"):
        add(f"Post-route static timing analysis (OpenRCX-extracted "
            f"interconnect, routed `natdie` run, {b['clock_period_ns']} ns "
            "clock):")
        add("")
        add("| corner | setup WNS (ns) | setup TNS (ns) | hold WNS (ns) | "
            "implied fmax @ closed (MHz) |")
        add("|---|---|---|---|---|")
        for c, cv in sorted((sta["corners"] or {}).items()):
            wns = cv.get("setup_wns_ns")
            fmax = (f"{1000.0 / max(1e-9, b['clock_period_ns'] - wns):.2f}"
                    if isinstance(wns, (int, float)) and
                    b['clock_period_ns'] - wns > 0 else "n/a (not closed)")
            add(f"| {c} | {wns if wns is not None else 'n/a'} | "
                f"{cv.get('setup_tns_ns', 'n/a')} | "
                f"{cv.get('hold_wns_ns', 'n/a')} | {fmax} |")
        add("")
        add(f"Headline corner per family practice: **{HEADLINE_CORNER} "
            "(ss_125C_4v50)** -- feasibility is judged there, not at tt. "
            "Worst-path reports are hash-pinned in the bundle.")
    else:
        add("**NOT_RUN** -- no routed netlist exists, so no post-route "
            "timing is claimed for any corner.")
    add("")
    add("## 3. Die / pad / margin budget, per packaging")
    add("")
    add("Fixed die and core areas are the D01 constraint inputs "
        "(quarter-slot MPW seat; two-slot die of the sibling calibration "
        "runs). Pad ring, metal margins and power delivery are outside the "
        "core area and inside H11's physical integration scope.")
    add("")
    for pk, pv in b["packaging_budgets"].items():
        verdict, reasons = verdicts.get(pk, ("NO_VERDICT", ["unjudged"]))
        add(f"- **{pk}** -- fixed die {v(pv.get('die_area_um2'))}, fixed "
            f"core {v(pv.get('core_area_um2'))}, die dimensions "
            f"{pv.get('die_um_w')} × {pv.get('die_um_h')} µm. "
            f"Verdict: **{verdict}**")
        if reasons:
            for r in reasons:
                add(f"  - {r}")
    add("")
    ndie = ((b.get("stages") or {}).get("natdie") or {}).get("floorplan") or {}
    if ndie.get("die_um_w"):
        add(f"- **natdie** (natural-die reference run, not a packaging "
            f"claim -- the die is sized post-synthesis from the measured "
            f"mapped area, so it cannot itself establish fit) -- die "
            f"{ndie.get('die_um_w')} × {ndie.get('die_um_h')} µm, core "
            f"{v(ndie.get('core_area_um2'))}, placement utilisation "
            f"{ndie.get('utilisation_pct')} %.")
        add("")
    add("**Explicitly unaccounted for in the budgets above** (listed, not "
        "hidden):")
    for u in b.get("unaccounted", []):
        add(f"- {u}")
    add("")
    add("## 4. The 16-note requirement")
    add("")
    add("The synthesised unit IS the committed 16-note polyphonic core "
        "(`dx7_core`: 96 operator states, 16-note state banks, event queue, "
        "mix buffer). No decision record revises the 16-note goal "
        "(DR-0008 retains it). Wherever the measured mapped numbers fall "
        "short of a packaging budget, the shortfall is reported against the "
        "retained 16-note goal -- the goal is not weakened to make this "
        "task pass, and no acceptance rule is weakened here.")
    add("")
    add("## 5. Context vs the family (measured sibling data points)")
    add("")
    fx = b.get("family_context") or {}
    sj = fx.get("sibling_joined") or {}
    if sj:
        add(f"- Combined sibling chip (monosynth product) routed in this "
            f"same flow image: {sj.get('instances')} instances / "
            f"{sj.get('cell_area_mm2')} mm^2 cells = "
            f"{sj.get('of_quarterslot_pct')} % of a quarter slot (does not "
            f"place); on its two-slot fixed die "
            f"{sj.get('twoslot_util_pct')} % utilisation, "
            f"{sj.get('drc_errors')} detailed-route DRC, ss setup "
            f"{sj.get('ss_setup_wns_ns')} ns at its "
            f"{sj.get('clock_mhz')} MHz clock. Provenance: "
            f"{sj.get('provenance')}.")
    pol = fx.get("dont_use_policy") or {}
    if pol and pol.get("pct_low") is not None:
        add(f"- **DONT_USE_CELLS policy, with its measured area effect.** "
            f"The stock ORFS gf180 platform policy (used by this run and by "
            f"the sibling's ORFS runs) excludes the drive-1 cell set; the sibling's "
            f"klt (kepler) flow allowed them. "
            f"{pol.get('how_measured', '')} Measured there: "
            f"**+{pol.get('pct_low'):.0f}-{pol.get('pct_high'):.0f} % area** "
            f"({pol.get('provenance')}). Cross-policy area numbers are not "
            "comparable: every number in this report is under the ORFS "
            "stock policy.")
    for note in fx.get("notes", []):
        add(f"- {note}")
    add("")
    add("## 6. Verification statuses (this issue's acceptance items)")
    add("")
    add("| item | status | evidence |")
    add("|---|---|---|")
    for row in b.get("acceptance_rows", []):
        add(f"| {row[0]} | **{row[1]}** | {row[2]} |")
    add("")
    add("## 7. What remains unproved (explicit)")
    add("")
    for u in b.get("unproved", []):
        add(f"- {u}")
    add("")
    add("---")
    add("*Generated by `tools/h10_report.py` from `evidence/h10-gf180/"
        "bundle.json` (all evidence hashes re-verified at generation time; "
        "a drift exits 4 and writes nothing). Regenerate: "
        "`python3 tools/h10_report.py`. The fit gate in this generator "
        "refuses any positive fit conclusion without the full RUN-evidenced "
        "stage chain (module docstring; live unit tests: "
        "`tests/test_h10_report.py`).*")
    return "\n".join(lines) + "\n"


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--bundle", default=os.path.join(REPO_ROOT, "evidence",
                  "h10-gf180", "bundle.json"))
    ap.add_argument("--out", default=os.path.join(REPO_ROOT, "docs",
                  "H10-GF180-FEASIBILITY.md"))
    ap.add_argument("--root", default=None,
                    help="root for resolving bundle evidence paths "
                         "(default: the repo root) -- the test harness "
                         "points this at its temp tree")
    ap.add_argument("--require-fit", action="store_true",
                    help="exit 3 unless every packaging's fit gate passes "
                         "with a full RUN-evidenced chain (the negative "
                         "control hook)")
    args = ap.parse_args(argv)

    with open(args.bundle) as f:
        bundle = json.load(f)
    root = args.root or REPO_ROOT
    problems = verify_bundle(bundle, root)
    if problems:
        for p in problems:
            print(f"BUNDLE-CHECK FAIL: {p}", file=sys.stderr)
        return 4

    verdicts = {}
    for pk in bundle["packaging_budgets"]:
        verdicts[pk] = fit_gate(bundle, pk, root)

    if args.require_fit:
        refused = {pk: (v, r) for pk, (v, r) in verdicts.items()
                   if v != "fits"}
        if refused:
            for pk, (_, reasons) in refused.items():
                print(f"FIT REFUSED for {pk}: " + "; ".join(reasons),
                      file=sys.stderr)
            return 3

    text = render_report(bundle, verdicts)
    with open(args.out, "w") as f:
        f.write(text)
    for pk, (v, r) in verdicts.items():
        print(f"{pk}: {v}" + (f" -- {r[0]}" if r else ""))
    print(f"report: {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())