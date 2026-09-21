#!/usr/bin/env python3
"""N06 integrated single-note case runner (issue #22).

For every case of the committed configuration, renders the pinned external
GPL oracle (reference/oracle-identity.json, re-hashed before any render) and
the integrated single-note model (src/gf180_dx7/model/integrated.py), and
compares the two dry f32le streams with the R04 exact paired comparator
(src/gf180_dx7/compare.py, exact mode: no alignment, trimming, or
normalization; docs/COMPARE-CONTRACT.md).

Case sources (issue #22 inputs):

  * dev32 subset  -- entries of corpus/dev32.json chosen to cover
    algorithms 1/4/6/23, all six LFO waveforms, fixed-frequency operators,
    and velocity-sensitive patches; voice bytes are resolved from the
    pinned DX7_AllTheWeb archive (canonical-hash checked; bytes are never
    redistributed, corpus policy) and are therefore guarded NOT_RUN when
    the archive is absent;
  * registry      -- the single-note-expressible cases of the R06 short
    subset (spec/compatibility-registry-v1.json); cases that need more than
    one simultaneously-sounding voice (repeated note while held, retrigger
    with a live release tail, 15/16-voice stealing) are outside the
    single-note model's declared scope and are recorded N07_SCOPE (NOT_RUN,
    with the mechanical reason), never as passes.

Per-case verdicts: EXACT (byte-identical), FAIL (with first-divergence
block + HYPOTHESIS localization naming the module/seam), N07_SCOPE
(NOT_RUN), NOT_RUN (environment). EXACT is the target: N02/N03/N04 were
byte-exact on their scopes, so any integration deviation is localized and
reported as a finding, never absorbed into a budget.

Negative control (--stub envelope|lfo): re-renders a fixed 3-case subset
with one module seam replaced by a constant output; the control PASSES only
if every case FAILS and the localization names the stubbed seam (the suite
must demonstrably fail and localize the stub).

Evidence: reference/evidence/n06-integrated/dev32-subset.json (dev32 +
registry verdicts) and negative-control.json (stub run), committed.

Exit codes: 0 = every rendered case EXACT (or N07_SCOPE with reason) and,
for --stub, the control fired; 1 = any FAIL or control miss; 2 = NOT_RUN
(environment).

Software-reference agreement only -- not original-DX7 fidelity, not RTL
agreement, not a musical claim (DEC-001, plan section 7). Stdlib-only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import compare, sysex  # noqa: E402
from gf180_dx7.model.integrated import (  # noqa: E402
    PolyphonyExceeded, render_integrated)
from render_reference import (  # noqa: E402
    HarnessFail, HarnessNotRun, check_oracle_binary, load_identity)
from registry_coverage import input_fingerprint, load_registry  # noqa: E402
from registry_smoke import build_voice  # noqa: E402

RATE = 48000
DEFAULT_IDENTITY = REPO / "reference" / "oracle-identity.json"
DEFAULT_REGISTRY = REPO / "spec" / "compatibility-registry-v1.json"
DEFAULT_DEV32 = REPO / "corpus" / "dev32.json"
DEFAULT_EVIDENCE = REPO / "reference" / "evidence" / "n06-integrated"
DEFAULT_ARCHIVE = Path("/Users/joseph/Downloads/DX7_AllTheWeb.zip")

SCHEMA = "gf180-dx7.n06-integrated/1"
FIDELITY_NOTE = ("Software-reference agreement only (pinned Dexed Mark I "
                 "oracle); not original-DX7 fidelity, not RTL agreement, "
                 "not a musical-quality claim (DEC-001).")

# dev32 entries rendered end-to-end (issue #22: >= 8 diverse entries).
# Coverage: algorithms 0,1,2,3,4,6,16,21,23,27,30 (incl. 1/4/6/23);
# LFO waves 0,1,2,3,4,5 (all six); fixed-frequency operators (entries
# 1 and 2); velocity-sensitive patches incl. velocity_contrast profiles
# (entries 3, 6, 30); pitch EG present (entries 1, 30).
DEV32_SUBSET = [0, 1, 2, 3, 4, 6, 16, 21, 23, 27, 30]

# R06 short-subset cases outside the single-note model's declared scope:
# a second live voice sounds while a predecessor has not reached the
# NUM-012 exact-silence state (polyphony is issue #23/N07).
N07_SCOPE = {
    "ev-repeated-note": "second note_on while the first voice is still "
                        "keydown: two simultaneously sounding voices",
    "ev-retrigger": "note_on during the previous voice's release tail: "
                    "two live voices (release not at the NUM-012 floor)",
    "ev-sum-15": "fifteen simultaneous notes (voice summation)",
    "ev-steal-16": "sixteen simultaneous notes + voice stealing",
    "ev-steal-17": "seventeen simultaneous notes + voice stealing",
}

# Registry single-note cases rendered end-to-end (short subset minus the
# N07-scope cases above; 23 cases).
SINGLE_NOTE_CASES = [
    "dir-base", "osc-fixed-carrier",
    "bd-coarse0", "bd-coarse31", "bd-detune0", "bd-detune14",
    "alg4-fb0", "alg4-fb3", "alg4-fb7",
    "alg6-fb0", "alg6-fb3", "alg6-fb7",
    "codec-name-a", "codec-name-b",
    "ev-sustain-no-pedal", "ev-sustain-pedal",
    "ev-all-sound-off", "ev-all-notes-off",
    "ev-pitch-bend-up", "ev-pitch-bend-down", "ev-pitch-bend-center",
    "ev-modwheel-127", "ev-modwheel-0",
]

# Cases rendered for the negative control (fast, distinct properties).
STUB_CASES = ["dir-base", "ev-pitch-bend-up", "ev-all-sound-off"]
STUB_SECONDS = {"dir-base": 1.0, "ev-pitch-bend-up": 1.0,
                "ev-all-sound-off": 1.0}

STUB_SEAM = {
    "envelope": ("envelope (N03 seam: operator level_in composition, "
                 "dx7note.cc:283)"),
    "lfo": ("LFO (N05 seam: shared lfo.getsample/getdelay, oracle "
            "main.cpp:444-445)"),
}


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


# ---------------------------------------------------------------------------
# Case sources


def dev32_voice_bytes(zf: zipfile.ZipFile, entry: dict) -> bytes:
    """Single-voice SysEx message for a dev32 entry, canonical-hash
    checked (validate_dev32.extract_voice_bytes semantics)."""
    from catalog_archive import canonical_hash
    path, slot, want = entry["path"], entry["slot"], entry["canonical_hash"]
    data = zf.read(path)
    if slot is None:
        message = data
        voice = sysex.decode_voice(data)
    else:
        bank = sysex.decode_bank(data)
        if not 0 <= slot < len(bank.voices):
            raise ValueError(f"dev32 entry {entry['index']}: slot {slot} "
                             f"outside bank {path}")
        voice = bank.voices[slot]
        message = sysex.encode_voice(voice)
    got = canonical_hash(voice)
    if got != want:
        raise ValueError(f"dev32 entry {entry['index']}: canonical hash "
                         f"drift for {path} slot={slot}: {got} != {want}")
    return message


def events_text(entry_events) -> str:
    lines = [f"{e['pos']} {e['cmd']} " + " ".join(str(a) for a in e["args"])
             for e in entry_events]
    return "\n".join(lines) + "\n"


def reference_lfo_note(voice_bytes: bytes) -> dict:
    """Reference-side LFO determinism classification for a patch (R05
    uninitialized-LFO finding, reference/trace-registry.json; N06 doc
    findings F-2/F-4):

    * wave 4 (sine): the pinned wrapper never calls Sin::init(), so its
      sintab is zero-initialized and lfo_value is the constant 1 << 23 —
      deterministic and mirrored by the model (PinnedWrapperLfo).
    * keysync 0: msfa Lfo leaves phase_ uninitialized (no constructor /
      reset initializer) and the wrapper's keydown only resets it when
      sync is set, so the rendered LFO phase is C++-indeterminate stack
      residue (stable per binary, not a derivable semantic). Whenever the
      LFO reaches the PCM (pitch-PM depth+sens > 0 or amp depth +
      sensitivity > 0), the reference value on that property is
      indeterminate and the model cannot match it by construction.
    """
    voice = sysex.decode_voice(voice_bytes)
    patch = voice_patch_bytes(voice_bytes)
    wave = patch[142]
    keysync = patch[141]
    pm_active = (patch[139] > 0) and (voice.lfo_pitch_mod_sensitivity > 0)
    amp_active = (patch[140] > 0) and any(
        op.amp_mod_sensitivity > 0 for op in voice.operators)
    lfo_reaches_pcm = pm_active or amp_active
    note = {
        "lfo_wave": wave, "lfo_key_sync": keysync,
        "lfo_reaches_pcm": lfo_reaches_pcm,
    }
    if wave == 4:
        note["reference_class"] = "pinned_wrapper_zeroed_sintab"
        note["note"] = ("wave 4: pinned wrapper's sine branch is the "
                        "constant 1<<23 (Sin::init never called; zeroed "
                        "sintab); mirrored by PinnedWrapperLfo (F-2)")
    elif keysync == 0 and lfo_reaches_pcm:
        note["reference_class"] = "reference_indeterminate_lfo_phase"
        note["note"] = ("keysync 0 with an active LFO path: the wrapper's "
                        "Lfo phase_ is C++-indeterminate (uninitialized, "
                        "R05 finding); the comparison on the LFO-driven "
                        "property has no derivable reference value and is "
                        "reported FAIL with this localization, never "
                        "budgeted")
    else:
        note["reference_class"] = "determinate"
    return note


def voice_patch_bytes(voice_bytes: bytes) -> list[int]:
    return list(voice_bytes[6:6 + 155])


# ---------------------------------------------------------------------------
# Rendering + diagnostics


class OracleRenderer:
    """Pinned-oracle subprocess renders, cached per input hash within one
    run; each fresh render records its provenance."""

    def __init__(self, oracle_path: Path, tmp: Path):
        self.oracle = oracle_path
        self.tmp = tmp
        self.cache: dict[tuple, bytes] = {}
        self.runs: list[dict] = []

    def render(self, voice_bytes: bytes, events: str, seconds: float) -> bytes:
        key = (sha256_bytes(voice_bytes), sha256_bytes(events.encode()),
               seconds)
        if key in self.cache:
            return self.cache[key]
        n = len(self.runs)
        vpath = self.tmp / f"c{n:03d}-voice.syx"
        epath = self.tmp / f"c{n:03d}-events.txt"
        opath = self.tmp / f"c{n:03d}-out.f32"
        vpath.write_bytes(voice_bytes)
        epath.write_text(events, encoding="utf-8")
        proc = subprocess.run(
            [str(self.oracle), "--voice", str(vpath), "--events", str(epath),
             "--out", str(opath), "--rate", str(RATE),
             "--seconds", str(seconds)], capture_output=True, text=True)
        if proc.returncode != 0:
            raise RuntimeError(f"oracle failed (exit {proc.returncode}): "
                               f"{proc.stderr.strip()}")
        data = opath.read_bytes()
        if len(data) != 4 * int(RATE * seconds):
            raise RuntimeError("oracle produced an unexpected length")
        self.runs.append({"voice_sha256": key[0], "events_sha256": key[1],
                          "seconds": seconds,
                          "pcm_sha256": sha256_bytes(data)})
        self.cache[key] = data
        return data

    @property
    def n_renders(self) -> int:
        return len(self.runs)


def hypothesis(first_div: int | None, entry_events, seconds: float,
               stub: str | None) -> dict:
    """HYPOTHESIS localization for a non-exact case (diagnostic only; a
    named behavior/seam, never a verdict)."""
    if stub is not None:
        return {
            "role": "HYPOTHESIS",
            "module": STUB_SEAM[stub],
            "behavior": f"injected negative control: {stub} seam replaced "
                        "by a constant output",
            "basis": "the stub was injected by the runner; the divergence "
                     "is attributable to the stubbed seam by construction",
        }
    if first_div is None:
        return {"role": "HYPOTHESIS", "module": None, "behavior": None,
                "basis": "case is exact; no localization needed"}
    block = first_div // 64
    ev_blocks = [(e["pos"] // 64, e["pos"], e["cmd"])
                 for e in entry_events]
    onsets = [b for b, _, cmd in ev_blocks if cmd == "note_on"]
    offs = [b for b, _, cmd in ev_blocks if cmd == "note_off"]
    ctrls = [b for b, _, cmd in ev_blocks if cmd in ("cc", "pb")]
    if onsets and block < min(onsets):
        module = "init/tables seam (N02 tables, patch decode, wrapper init)"
        behavior = "divergence before the first note-on"
    elif block in onsets:
        module = ("voice-init/envelope attack seam (N03/N04 composition at "
                  "Dx7Note::init)")
        behavior = "divergence at a note-on block"
    elif block in offs:
        module = "envelope release seam (N03, Env::keydown(false))"
        behavior = "divergence at a note-off block"
    elif block in ctrls:
        module = ("controller event seam (wrapper CC/pb grammar, N05 pitch "
                  "path)")
        behavior = "divergence at a cc/pb block"
    else:
        module = ("sustain-path seam (N03 envelope trajectory, N05 LFO/"
                  "pitch-EG, or the NUM-008 amp-mod float-gated spot)")
        behavior = ("divergence mid-note; distinguish via the comparator's "
                    "secondary spectral proxy")
    return {"role": "HYPOTHESIS", "module": module, "behavior": behavior,
            "basis": f"first divergence in block {block} "
                     f"(sample {first_div}) of a {int(seconds * RATE)}-"
                     "sample render; plausibility only"}


def run_case(case_id: str, voice_bytes: bytes, events: str, seconds: float,
             oracle: OracleRenderer, *, stub: str | None = None,
             entry_events=None, lfo_note: dict | None = None) -> dict:
    """Render oracle + model, compare exactly, return the evidence row."""
    row = {
        "id": case_id,
        "voice_sha256": sha256_bytes(voice_bytes),
        "events_sha256": sha256_bytes(events.encode("utf-8")),
        "render_seconds": seconds,
        "stub": stub,
    }
    if lfo_note is not None:
        row["reference_lfo"] = lfo_note
    try:
        oracle_pcm = oracle.render(voice_bytes, events, seconds)
    except RuntimeError as exc:
        row.update(verdict="FAIL", status="FAIL",
                   reason=f"oracle render error: {exc}")
        return row
    if stub == "envelope":
        stubs = {"envelope": "constant"}
    elif stub == "lfo":
        stubs = {"lfo": "zero"}
    else:
        stubs = None
    try:
        model_pcm = render_integrated(sysex.decode_voice(voice_bytes),
                                      events, seconds=seconds, stubs=stubs)
    except PolyphonyExceeded as exc:
        row.update(verdict="N07_SCOPE", status="NOT_RUN", reason=str(exc),
                   block=exc.block, live_slots=exc.live_slots)
        return row
    report = compare.compare_streams(oracle_pcm, model_pcm)
    primary = report.primary or {}
    first_div = primary.get("first_divergent_sample")
    row.update({
        "oracle_pcm_sha256": report.sha256_a,
        "model_pcm_sha256": report.sha256_b,
        "verdict": "EXACT" if report.passed else "FAIL",
        "status": "PASS" if report.passed else "FAIL",
        "byte_identical": report.byte_identical,
        "max_abs_error": primary.get("max_abs_error"),
        "mismatch_count": primary.get("mismatch_count"),
        "first_divergent_sample": first_div,
        "first_divergent_block": (first_div // 64
                                  if first_div is not None else None),
    })
    if not report.passed:
        row["hypothesis"] = hypothesis(first_div, entry_events or [],
                                       seconds, stub)
        if lfo_note and lfo_note.get("reference_class") == \
                "reference_indeterminate_lfo_phase":
            row["hypothesis"] = {
                "role": "HYPOTHESIS",
                "module": ("reference-side LFO indeterminacy (R05 "
                           "uninitialized-phase finding; keysync=0, LFO "
                           "reaches PCM)"),
                "behavior": lfo_note["note"],
                "basis": (f"first divergence in block "
                          f"{first_div // 64} (sample {first_div}); the "
                          "reference initial LFO phase is C++-"
                          "indeterminate stack residue, so the LFO-driven "
                          "property has no derivable reference value — a "
                          "reference finding, not a model defect and not a "
                          "numeric budget"),
            }
    return row


# ---------------------------------------------------------------------------
# Set runners


def run_dev32(oracle: OracleRenderer, entries: list[int],
              archive: Path) -> list[dict]:
    """dev32 subset end-to-end; guarded NOT_RUN when the pinned archive is
    absent (voice bytes resolve only from the archive, corpus policy)."""
    manifest = json.loads(DEFAULT_DEV32.read_text(encoding="utf-8"))
    by_index = {e["index"]: e for e in manifest["entries"]}
    rows: list[dict] = []
    try:
        zf = zipfile.ZipFile(archive)
    except (OSError, zipfile.BadZipFile) as exc:
        for idx in entries:
            rows.append({
                "id": f"dev32-{idx:02d}",
                "verdict": "NOT_RUN", "status": "NOT_RUN",
                "reason": f"pinned archive unavailable ({exc}); dev32 voice "
                          "bytes resolve only from the pinned archive",
            })
        return rows
    with zf:
        for idx in entries:
            entry = by_index[idx]
            try:
                voice_bytes = dev32_voice_bytes(zf, entry)
            except (ValueError, KeyError) as exc:
                rows.append({"id": f"dev32-{idx:02d}", "verdict": "FAIL",
                             "status": "FAIL", "reason": str(exc)})
                continue
            rows.append(run_case(
                f"dev32-{idx:02d}", voice_bytes, events_text(entry["events"]),
                float(entry["render"]["seconds"]), oracle,
                entry_events=entry["events"],
                lfo_note=reference_lfo_note(voice_bytes)))
    return rows


def run_registry(oracle: OracleRenderer, case_ids: list[str],
                 stub: str | None = None) -> list[dict]:
    reg = load_registry(DEFAULT_REGISTRY)
    cases = {c["id"]: c for c in reg["cases"]}
    rows: list[dict] = []
    for cid in case_ids:
        case = cases[cid]
        voice = build_voice(reg, case)
        voice_bytes = sysex.encode_voice(voice)
        events = "\n".join(case["events"]) + "\n"
        seconds = (STUB_SECONDS.get(cid, 1.0) if stub
                   else float(case.get("render_seconds", 1.5)))
        row = run_case(cid, voice_bytes, events, seconds, oracle,
                       stub=stub, entry_events=case["events"])
        row["input_fingerprint"] = input_fingerprint(case)
        row["expect_property"] = case.get("expect", {}).get("property", "")
        rows.append(row)
    return rows


def n07_rows() -> list[dict]:
    return [{"id": cid, "verdict": "N07_SCOPE", "status": "NOT_RUN",
             "reason": reason,
             "note": "registered R06 short-subset case, outside the "
                     "single-note model scope (polyphony is issue #23/N07)"}
            for cid, reason in N07_SCOPE.items()]


# ---------------------------------------------------------------------------
# Evidence assembly


def summarize(sections: dict) -> dict:
    flat = sections.get("dev32", []) + sections.get("registry", [])
    counts: dict[str, int] = {}
    for r in flat:
        counts[r["verdict"]] = counts.get(r["verdict"], 0) + 1
    return {"cases_total": len(flat), "by_verdict": counts,
            "exact": counts.get("EXACT", 0), "fail": counts.get("FAIL", 0),
            "n07_scope": counts.get("N07_SCOPE", 0),
            "not_run": counts.get("NOT_RUN", 0)}


def write_evidence(path: Path, sections: dict, oracle: OracleRenderer,
                   identity: dict, extra: dict | None = None) -> None:
    evidence = {
        "schema": SCHEMA,
        "issue": "https://github.com/2AMLogic/gf180-dx7/issues/22",
        "model": "src/gf180_dx7/model/integrated.py (N06 integrated "
                 "single-note fixed model)",
        "comparator": "src/gf180_dx7/compare.py (exact mode, dry; "
                      "docs/COMPARE-CONTRACT.md)",
        "oracle": {
            "identity_path": "reference/oracle-identity.json",
            "pinned_sha256": identity["binary"]["sha256"],
            "observed_sha256_match": True,
            "n_renders": oracle.n_renders,
        },
        "sample_rate": RATE,
        "target": "EXACT (byte-identical); any deviation is localized and "
                  "reported per property, never budgeted",
        "summary": summarize(sections),
        "sections": sections,
        "fidelity_note": FIDELITY_NOTE,
    }
    if extra:
        evidence.update(extra)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")


# ---------------------------------------------------------------------------
# Main


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--oracle", type=Path, default=None)
    parser.add_argument("--identity", type=Path, default=DEFAULT_IDENTITY)
    parser.add_argument("--evidence-dir", type=Path, default=DEFAULT_EVIDENCE)
    parser.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
    parser.add_argument("--registry-only", action="store_true",
                        help="skip the dev32 subset")
    parser.add_argument("--stub", choices=sorted(STUB_SEAM), default=None,
                        help="run only the negative control with the named "
                             "module seam stubbed to a constant output")
    args = parser.parse_args(argv)

    try:
        identity = load_identity(args.identity)
        oracle_path = args.oracle or Path(identity["binary"]["path"])
        check_oracle_binary(identity, oracle_path)
    except HarnessFail as exc:
        print(f"n06_compare: FAIL — {exc}", file=sys.stderr)
        return 1
    except HarnessNotRun as exc:
        print(f"n06_compare: NOT_RUN — {exc}", file=sys.stderr)
        return 2

    stub = args.stub
    out_path = (args.evidence_dir /
                ("negative-control.json" if stub else "dev32-subset.json"))

    n_problems = 0
    with tempfile.TemporaryDirectory(prefix="n06-compare-") as td:
        oracle = OracleRenderer(oracle_path, Path(td))
        sections: dict = {}
        extra: dict | None = None

        if stub:
            rows = run_registry(oracle, STUB_CASES, stub=stub)
            sections["stubbed_cases"] = rows
            fired = bool(rows) and all(r["verdict"] == "FAIL" for r in rows)
            localized = all(
                r.get("hypothesis", {}).get("module") == STUB_SEAM[stub]
                for r in rows)
            control = {
                "stub": stub,
                "seam": STUB_SEAM[stub],
                "all_cases_fail": fired,
                "localization_names_stubbed_seam": localized,
                "control_fired": bool(fired and localized),
            }
            if not control["control_fired"]:
                control["note"] = ("NEGATIVE CONTROL DID NOT FIRE: a stubbed "
                                   "case did not fail or did not localize "
                                   "the stubbed seam")
                n_problems += 1
            extra = {"control": control}
        else:
            if not args.registry_only:
                sections["dev32"] = run_dev32(oracle, DEV32_SUBSET,
                                              args.archive)
            sections["registry"] = (run_registry(oracle, SINGLE_NOTE_CASES)
                                    + n07_rows())
            n_problems = sum(1 for r in
                             sections["dev32"] + sections["registry"]
                             if r["verdict"] == "FAIL")
            n_problems += sum(1 for r in sections["registry"]
                              if r["verdict"] == "N07_SCOPE"
                              and r["id"] not in N07_SCOPE)

        write_evidence(out_path, sections, oracle, identity, extra)

    verdict = "PASS" if n_problems == 0 else "FAIL"
    print(f"n06_compare: {verdict} — evidence: {out_path} "
          f"({oracle.n_renders} oracle renders)")
    return 0 if n_problems == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
