#!/usr/bin/env python3
"""DX7 demo playback fixture: committed phrase set -> ordered control events.

U05 (issue #38). Adapted from gf180-parasynth ``fpga/fixtures.py`` (sibling
commit ``cbcc8b9e10e49c84f630550e2e145cc6da8a659c``, sha256
``10bfcb69917c34000def7ffe86dd2ad5854a6492a4a087c139cfc44cd52426f6``): the
sibling module builds and inspects a fixed playback sequence and its
``_coverage()`` counts the ACTUAL writes by tag. Provenance and ruling:
``docs/REUSE-RULINGS.md`` (U05 row) and ``docs/reuse/catalog.json``
(component ``u05-demo-fixture-bench``, ruling ``adapt``).

Honest differences from the sibling (no upstream bytes are copied beyond the
structure): the sibling builds a write stream for its own instrument model
(patch/kit/notes/drum-hit/knob state, numpy audio). Here the musical content
is the committed versioned phrase set ``spec/audition-phrases-v2.json`` (a
byte-identical superset of U01's ``spec/audition-phrases-v1.json`` — this
module EXTENDS the U01 phrase set, it does not fork it), and the events feed
the U04 production host (``src/gf180_dx7/host.py``) driving the U04 mock
core (``src/gf180_dx7/mock_core.py``). The mock is a link-contract model and
synthesizes no audio, so coverage is computed by this module from the
schedule (never from a claim); a fixture that would not deliver what it
claims is refused (bench exit 2) before a single write reaches the link.

DEMO SESSION LAYER (documented policy, NOT an H03 contract claim): the H03
core link carries generic CTRL writes; it has no pedal state and no
all-notes-off semantics. The demo session layer (what a host keyboard
firmware would do) maps:
  * CC64 >= 64  -> pedal down: subsequent key-ups are parked (not sent);
  * CC64 <  64  -> pedal up: parked note-offs are released in key-up order;
  * CC120 == 127 (all notes off) -> explicit note-offs for every currently
    held note, in ascending note order; the parking queue is superseded
    (the global release takes precedence).
These derived note-offs ARE part of the link schedule (each carries its own
pred frame) and are covered by the same three-way cross-check.
"""

from __future__ import annotations

import hashlib
import json
import sys
import zipfile
from dataclasses import dataclass
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
REPO = THIS_DIR.parents[1]
for _p in (str(THIS_DIR), str(REPO / "src"), str(REPO / "tools")):
    if _p not in sys.path:
        sys.path.insert(0, _p)

from catalog_archive import canonical_hash  # noqa: E402
from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7 import host as hostmod  # noqa: E402

SAMPLE_RATE = 48_000
FRAME_SAMPLES = 64
FIXTURE_SCHEMA = "gf180-dx7.demo-audition/1"
PHRASES_PATH_DEFAULT = REPO / "spec" / "audition-phrases-v2.json"

# CC64 pedal polarity / ANO value (MIDI; thresholds per the docstring above).
PEDAL_CC = 64
PEDAL_DOWN_VALUE = 64
ANO_CC = 120
ANO_VALUE = 127


class FixtureRefuse(Exception):
    """The fixture would not deliver what it claims; the bench refuses."""


@dataclass(frozen=True)
class LinkEvent:
    """One event fed to the production host (post session-layer mapping)."""

    kind: str            # note_on | note_off | controller | bend
    sample: int          # absolute song sample (48 kHz)
    phrase_id: str
    source: str          # the phrase-file event line that caused it
    note: int = -1
    velocity: int = -1
    cc: int = -1
    value: int = -1

    @property
    def frame(self) -> int:
        return self.sample // FRAME_SAMPLES


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(Path(path).read_bytes())


# ---------------------------------------------------------------------------
# Phrase set
# ---------------------------------------------------------------------------

def load_phrase_set(path: Path | None = None) -> list[dict]:
    """Load + validate a phrase file with the U01 validator (tools/audition):
    format, id rules, required U01 coverage. A committed v2 file passes the
    same validator because it is a superset of the U01 set."""
    import audition
    return audition.load_phrases(Path(path) if path else PHRASES_PATH_DEFAULT)


def check_superset_of_v1(v1_path: Path, v2: list[dict]) -> None:
    """v2 (validated dicts) must contain every U01 phrase with unchanged
    events (canonical form) and duration."""
    v1 = json.loads(Path(v1_path).read_text())
    v2_by_id = {p["id"]: p for p in v2}
    for row in v1["phrases"]:
        pid = row["id"]
        if pid not in v2_by_id:
            raise FixtureRefuse(f"phrase set v2 dropped U01 phrase {pid!r}")
        canon_v1 = [(e["pos"], e["cmd"], tuple(e["args"])) 
              for e in _canonical_events(row["events"])]
        canon_v2 = [(e["pos"], e["cmd"], tuple(e["args"])) 
              for e in v2_by_id[pid]["events"]]
        if canon_v1 != canon_v2 or round(v2_by_id[pid]["duration_s"], 6) != \
                round(float(row["duration_s"]), 6):
            raise FixtureRefuse(
                f"phrase set v2 changed U01 phrase {pid!r} events/duration")


def _canonical_events(raw_events) -> list[dict]:
    """Normalize raw phrase-file event strings to the {pos, cmd, args}
    canonical form used by the U01 validator (without the coverage rules)."""
    out = []
    for line in raw_events:
        parts = line.split()
        out.append({"pos": int(parts[0]), "cmd": parts[1],
                    "args": [int(v) for v in parts[2:]]})
    return out


def event_source(ev: dict) -> str:
    """Reconstruct the phrase-file line for a validated event dict (trace
    string carried by LinkEvent.source)."""
    return str(ev["pos"]) + " " + ev["cmd"] + " " + " ".join(
        str(a) for a in ev["args"])


# ---------------------------------------------------------------------------
# Patch identity
# ---------------------------------------------------------------------------

def resolve_patch(fx: dict) -> "sysex.Voice":
    """Resolve the fixture's patch to a P01 Voice, verifying every pinned
    identity (source file sha256, slot, canonical hash, name)."""
    pid: dict = fx["patch"]
    src = REPO / pid["source"]
    if not src.exists():
        raise FixtureRefuse(f"patch source {pid['source']} missing")
    if sha256_file(src) != pid["source_sha256"]:
        raise FixtureRefuse(
            f"patch source {pid['source']} sha256 drift: "
            f"{sha256_file(src)} != pinned {pid['source_sha256']}")
    data = src.read_bytes()
    try:
        obj = sysex.decode_bank(data)
        if not 0 <= pid["bank_slot"] < len(obj.voices):
            raise FixtureRefuse("bank_slot out of range")
        voice = obj.voices[pid["bank_slot"]]
    except sysex.SysexError:
        voice = sysex.decode_voice(data)
        if pid["bank_slot"] != 0:
            raise FixtureRefuse("single-voice file requires bank_slot 0")
    voice.raw_bytes = sysex.encode_voice(voice)[6:6 + sysex.VOICE_DATA_LEN]
    chash = canonical_hash(voice)
    if chash != pid["canonical_hash"]:
        raise FixtureRefuse(
            f"patch canonical hash {chash} != pinned {pid['canonical_hash']}")
    if voice.name != pid["name"]:
        raise FixtureRefuse(
            f"patch name {voice.name!r} != pinned {pid['name']!r}")
    return voice


def load_dev32_voice(target_hash: str, zip_path: Path) -> "sysex.Voice":
    """Resolve one dev32 patch (corpus/dev32.json entry by canonical hash)
    from the pinned AllTheWeb zip. Environment-gated (the zip is not
    committed); the wave-gate tests use this for the 'real patch' variant.
    A skipped environment gate is never reported as a pass."""
    dev32 = json.loads((REPO / "corpus" / "dev32.json").read_text())
    entries = [e for e in dev32["entries"]
               if e["canonical_hash"] == target_hash]
    if len(entries) != 1:
        raise FixtureRefuse(
            f"dev32 canonical hash {target_hash[:12]}... resolves "
            f"{len(entries)} entries, expected 1")
    entry = entries[0]
    with zipfile.ZipFile(zip_path) as zf:
        data = zf.read(entry["path"])
    obj = sysex.decode_message(data)
    if isinstance(obj, sysex.Bank):
        voice = obj.voices[entry["slot"]]
    else:
        voice = obj
    voice.raw_bytes = sysex.encode_voice(voice)[6:6 + sysex.VOICE_DATA_LEN]
    if canonical_hash(voice) != target_hash:
        raise FixtureRefuse("extracted dev32 voice hash drift")
    return voice


# ---------------------------------------------------------------------------
# Demo session layer (pedal + all-notes-off; document above)
# ---------------------------------------------------------------------------

class SessionPolicy:
    """The documented demo session layer. Pure schedule bookkeeping: no
    core state, no H03 contract claim."""

    def __init__(self) -> None:
        self.pedal_down = False
        # parked (keyup sample, note, source line)
        self.held_releases: list[tuple[int, int, str]] = []
        self.notes_held: set[int] = set()

    def note_on(self, note: int) -> None:
        self.notes_held.add(note)

    def note_off(self, note: int, sample: int, line: str, phrase_id: str,
                 out: list[LinkEvent]) -> None:
        self.notes_held.discard(note)
        if self.pedal_down:
            self.held_releases.append((sample, note, line))
            return
        out.append(LinkEvent("note_off", sample, phrase_id, line, note=note))

    def pedal_up(self, sample: int, line: str, phrase_id: str,
                 out: list[LinkEvent]) -> None:
        self.pedal_down = False
        held = sorted(self.held_releases, key=lambda t: (t[0], t[1]))
        self.held_releases = []
        for _keyup_sample, note, _keyup_line in held:
            out.append(LinkEvent("note_off", sample, phrase_id, line,
                                 note=note))

    def all_notes_off(self, sample: int, line: str, phrase_id: str,
                      out: list[LinkEvent]) -> None:
        release = sorted(self.notes_held)
        self.notes_held.clear()
        self.held_releases = []  # superseded by the global release
        for note in release:
            out.append(LinkEvent("note_off", sample, phrase_id, line,
                                 note=note))


def phrase_to_events(phrase: dict, cursor: int,
                     policy: SessionPolicy) -> list[LinkEvent]:
    """Map one phrase (oracle event lines) to LINK events at absolute song
    sample = cursor + event pos, applying the session layer. Returns link
    events in send order (derived note-offs ride at the position of the
    pedal-up / ANO controller that triggered them)."""
    out: list[LinkEvent] = []
    for ev in phrase["events"]:  # validated dicts: {pos, cmd, args}
        pos = ev["pos"]
        cmd = ev["cmd"]
        args = ev["args"]
        src = event_source(ev)
        abs_pos = cursor + pos
        if cmd == "note_on":
            note, vel = args
            out.append(LinkEvent("note_on", abs_pos, phrase["id"], src,
                                 note=note, velocity=vel))
            policy.note_on(note)
        elif cmd == "note_off":
            note = args[0]
            policy.note_off(note, abs_pos, src, phrase["id"], out)
        elif cmd == "cc":
            cc, value = args
            out.append(LinkEvent("controller", abs_pos, phrase["id"], src,
                                 cc=cc, value=value))
            if cc == PEDAL_CC and value >= PEDAL_DOWN_VALUE:
                policy.pedal_down = True
            elif cc == PEDAL_CC and value < PEDAL_DOWN_VALUE:
                policy.pedal_up(abs_pos, src, phrase["id"], out)
            elif cc == ANO_CC and value == ANO_VALUE:
                policy.all_notes_off(abs_pos, src, phrase["id"], out)
        elif cmd == "pb":
            lo, hi = args  # U01 pb args are (lo, hi); 14-bit bend int
            out.append(LinkEvent("bend", abs_pos, phrase["id"], src,
                                 value=(hi << 7) | lo))
        else:  # pragma: no cover - phrase validation rejects unknown cmds
            raise FixtureRefuse(f"unknown event cmd {cmd!r}")
    # keep the out list chronological (derived releases ride at their
    # trigger controller's position, which is >= the parked key-up positions)
    out.sort(key=lambda e: (e.sample, 0 if e.kind == "controller" else 1,
                            e.note))
    return out


# ---------------------------------------------------------------------------
# Playthrough assembly
# ---------------------------------------------------------------------------

def assemble_playthrough(fx: dict, phrases: list[dict],
                         profile: str) -> dict:
    """Build the full ordered schedule for one profile.

    Returns a dict:
      events: list[LinkEvent] in exact send order (phrase-by-phrase,
              chronological within phrase),
      phrase_starts: {phrase_id: first sample of that phrase},
      second_commit_after: phrase id (or None) after which the fixture
              commits the same patch again mid-playthrough (the recall
              check),
      counts: exact expected event/write counts (a pure function of the
              schedule), computed here and pinned in the fixture JSON,
      profile: the profile name.

    Core-frame mapping (the bench uses this): the bench boots the core,
    which absorbs ``n_boot_frames`` frame windows (167+4 writes at the
    24.576 MHz design point), so song frame f (0-based) lands at core frame
    3 + f; the mid-playthrough commit absorbs 2 more frame windows and the
    bench accounts for it at the phrase boundary (musical samples do not
    move — the two extra link frames are idle gap time; phrases carry
    >= 50 ms inter-event gaps).
    """
    order = (fx.get("playthrough_order") or {}).get(profile)
    if not order:
        raise FixtureRefuse(f"profile {profile!r} has no playthrough_order")
    by_id = {p["id"]: p for p in phrases}
    unknown = [pid for pid in order if pid not in by_id]
    if unknown:
        raise FixtureRefuse(
            f"playthrough references unknown phrase ids: {unknown}")
    second_commit = (fx.get("second_commit_after") or {}).get(profile)
    if second_commit and second_commit not in by_id:
        raise FixtureRefuse(
            f"second_commit_after {second_commit!r} is not a phrase id")
    policy = SessionPolicy()
    events: list[LinkEvent] = []
    cursor = 0
    phrase_starts: dict[str, int] = {}
    for pid in order:
        phrase = by_id[pid]
        phrase_starts[pid] = cursor
        events.extend(phrase_to_events(phrase, cursor, policy))
        cursor += int(round(phrase["duration_s"] * SAMPLE_RATE))
    if policy.pedal_down or policy.held_releases:
        raise FixtureRefuse(
            "playthrough ended with the pedal down / parked key-ups: the "
            "session layer must return to idle (the fixture would leave "
            "stuck notes in the core)")
    if policy.notes_held:
        raise FixtureRefuse(
            f"playthrough still holding {sorted(policy.notes_held)}: "
            "stuck notes at the fixture end")
    counts = _expected_counts(fx, events, profile)
    return {"events": events, "phrase_starts": phrase_starts,
            "second_commit_after": second_commit,
            "counts": counts, "profile": profile}


def _expected_counts(fx: dict, events: list[LinkEvent],
                     profile: str) -> dict:
    batch = hostmod.capacity(hostmod.DESIGN_POINT) - hostmod.STATUS_POLL_WINDOWS
    n_boot_frames = -(-hostmod.BOOT_IMAGE_WRITES // batch)
    has_recall_commit = bool(
        (fx.get("second_commit_after") or {}).get(profile))
    per_kind = {"note_on": 0, "note_off": 0, "controller": 0, "bend": 0}
    for ev in events:
        per_kind[ev.kind] += 1
    event_writes = (per_kind["note_on"] * hostmod.NOTE_ON_WRITES
                    + per_kind["note_off"] * hostmod.NOTE_OFF_WRITES
                    + per_kind["controller"] * hostmod.CONTROLLER_WRITES
                    + per_kind["bend"] * hostmod.BEND_WRITES)
    n_images = 2 if has_recall_commit else 1
    return {
        "note_on": per_kind["note_on"],
        "note_off": per_kind["note_off"],
        "controller": per_kind["controller"],
        "bend": per_kind["bend"],
        "event_writes": event_writes,
        "image_writes": hostmod.BOOT_IMAGE_WRITES * n_images,
        "commits_applied_expected": 1 + (1 if has_recall_commit else 0),
        "generations_expected": 1 + (1 if has_recall_commit else 0),
        "total_write_windows": event_writes
        + hostmod.BOOT_IMAGE_WRITES * n_images,
        "n_boot_frames_at_design": n_boot_frames,
    }


# ---------------------------------------------------------------------------
# Coverage (sibling _coverage() pattern: count the schedule, never a claim)
# ---------------------------------------------------------------------------

def _final_held(events: list[LinkEvent]) -> set[int]:
    held: set[int] = set()
    for e in events:
        if e.kind == "note_on":
            held.add(e.note)
        elif e.kind == "note_off":
            held.discard(e.note)
    return held


def coverage_from_schedule(events: list[LinkEvent]) -> dict:
    """Coverage computed FROM THE SCHEDULE (sibling ``_coverage()`` pattern).
    Never inferred from a claim; the bench refuses a fixture whose schedule
    does not deliver its declared coverage."""
    on_notes = [e for e in events if e.kind == "note_on"]
    velocities = sorted({e.velocity for e in on_notes})
    held: set[int] = set()
    max_width = 0
    last_off_sample: dict[int, int] = {}
    for e in events:
        if e.kind == "note_on":
            held.add(e.note)
            max_width = max(max_width, len(held))
        elif e.kind == "note_off":
            held.discard(e.note)
            last_off_sample[e.note] = e.sample
    pedals = [e for e in events
              if e.kind == "controller" and e.cc == PEDAL_CC]
    ano = [e for e in events if e.kind == "controller"
           and e.cc == ANO_CC and e.value == ANO_VALUE]
    bends = [e.value for e in events if e.kind == "bend"]
    cc_values = sorted({(e.cc, e.value) for e in events
                        if e.kind == "controller"
                        and e.cc not in (PEDAL_CC, ANO_CC)})
    # release tails: a note fully released with >= 0.4 s of keyboard
    # silence before the next key event on that note (or the end)
    tail_notes = 0
    for note in sorted(last_off_sample):
        if note in _final_held(events):
            continue
        later = [e for e in events
                 if e.kind in ("note_on", "note_off") and e.note == note
                 and e.sample > last_off_sample[note]]
        if not later or later[0].sample - last_off_sample[note] \
                >= int(0.4 * SAMPLE_RATE):
            tail_notes += 1
    return {
        "distinct_notes": sorted({e.note for e in on_notes}),
        "distinct_velocities": velocities,
        "max_chord_width": max_width,
        "pedal_down_up": (any(e.value >= PEDAL_DOWN_VALUE for e in pedals)
                          and any(e.value < PEDAL_DOWN_VALUE for e in pedals)),
        "all_notes_off_events": len(ano),
        "bend_steps": len(bends),
        "bend_values": bends,
        "controller_cc_values": [list(t) for t in cc_values],
        "release_tail_notes": tail_notes,
        "note_on_total": len(on_notes),
        "final_held": sorted(_final_held(events)),
    }


def check_want(coverage: dict, fx: dict, profile: str) -> list[str]:
    """Problems (empty list = ok) if the schedule does not deliver the
    coverage the fixture claims for this profile. The bench refuses on any
    problem (exit 2) — the live negative control 'a fixture whose silence
    or undershoot must make the harness REFUSE'."""
    want = (fx.get("coverage_wants") or {}).get(profile)
    if want is None:
        return [f"no coverage_wants declared for profile {profile!r}"]
    problems = []
    if len(coverage["distinct_notes"]) < want.get("min_distinct_notes", 1):
        problems.append(f"notes {len(coverage['distinct_notes'])} distinct "
                        f"< {want['min_distinct_notes']} (a one-note "
                        f"audition is not a demonstration)")
    if len(coverage["distinct_velocities"]) < want["min_distinct_velocities"]:
        problems.append(f"velocity layers: {len(coverage['distinct_velocities'])}"
                        f" distinct < {want['min_distinct_velocities']}")
    missing_vel = set(want.get("required_velocities", [])) \
        - set(coverage["distinct_velocities"])
    if missing_vel:
        problems.append(f"required velocities missing: {sorted(missing_vel)}")
    if coverage["max_chord_width"] < want["min_chord_width"]:
        problems.append(f"chord width {coverage['max_chord_width']} "
                        f"< {want['min_chord_width']}")
    if coverage["release_tail_notes"] < want["min_release_tails"]:
        problems.append(f"release tails {coverage['release_tail_notes']} "
                        f"< {want['min_release_tails']}")
    if want.get("require_pedal") and not coverage["pedal_down_up"]:
        problems.append("sustain pedal down+up not present")
    if want.get("require_all_notes_off") \
            and coverage["all_notes_off_events"] < 1:
        problems.append("all-notes-off (CC120) not present")
    if coverage["bend_steps"] < want["min_bend_steps"]:
        problems.append(f"bend steps {coverage['bend_steps']} "
                        f"< {want['min_bend_steps']}")
    for cc in want.get("required_cc", []):
        if cc not in {t[0] for t in coverage["controller_cc_values"]}:
            problems.append(f"controller CC{cc} not present")
    if coverage["final_held"]:
        problems.append(f"notes still held at the end: "
                        f"{coverage['final_held']}")
    return problems


def load_fixture(path: Path,
                 phrases: list[dict] | None = None) -> tuple[dict, list[dict]]:
    fx = json.loads(Path(path).read_text())
    if fx.get("schema") != FIXTURE_SCHEMA:
        raise FixtureRefuse(
            f"fixture schema {fx.get('schema')!r} != {FIXTURE_SCHEMA}")
    if fx.get("sample_rate") != SAMPLE_RATE:
        raise FixtureRefuse("fixture sample_rate must be 48000")
    if phrases is None:
        phrases = load_phrase_set(Path(REPO / fx["phrases_ref"]["path"]))
    return fx, phrases


def main() -> int:  # pragma: no cover - manual inspection helper
    import argparse
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--fixture", default="spec/demo-audition-v1.json")
    ap.add_argument("--profile", default="compact")
    args = ap.parse_args()
    fx_path = Path(args.fixture)
    fx, phrases = load_fixture(fx_path if fx_path.is_absolute()
                               else REPO / fx_path)
    play = assemble_playthrough(fx, phrases, args.profile)
    cov = coverage_from_schedule(play["events"])
    print(json.dumps({"profile": args.profile, "counts": play["counts"],
                      "coverage": cov,
                      "problems": check_want(cov, fx, args.profile)},
                     indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())