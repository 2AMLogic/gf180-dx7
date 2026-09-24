#!/usr/bin/env python3
"""U05 stress battery (issue #38): the committed musical fixture is not the
whole story - the control path must survive playing styles, not just the
audition. Scenarios (all software, mock link - the board variants of the
same exercises are the operator rows pending #31 H09 capture):

  fast_play                    dense 16-note-limit note traffic
  sustain_pedal                pedal-held notes then pedal release
  patch_changes_while_held     patches committed while notes are held
                               (the recall row, software leg)
  all_sound_off                every note plus pedal plus ANO at once

Required negative control: throttled_link - the deliberate underrun. The
contract under test is H03 DEC-014: a link that cannot drain must REJECT
EXPLICITLY (overflow status + logged fault), the host reads status and
retries, and no write is silently lost. The run is only a PASS if
(a) at least one explicit rejection is logged (the check has resolution -
an unthrottled run logs zero and would fail this assert) and (b) host
log, pin, and drain legs still agree one write per accepted transaction
(no silent loss).

Exit: 0 all scenarios pass, 1 any failure, 2 refuse (bad fixture/patch).
Reports: JSON, schema gf180-dx7.stress-report/1 (committed under
evidence/u05-demo/).
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
REPO = THIS_DIR.parents[1]
for _p in (str(THIS_DIR), str(REPO / "src"), str(REPO / "tools")):
    if _p not in sys.path:
        sys.path.insert(0, _p)

import demo_fixture as B  # noqa: E402
import fixtures_dx7 as F  # noqa: E402
from gf180_dx7 import host as hostmod  # noqa: E402
from gf180_dx7 import mock_core as M  # noqa: E402
from gf180_dx7 import sysex  # noqa: E402

SCENARIOS = ["fast_play", "sustain_pedal", "patch_changes_while_held",
             "all_sound_off", "throttled_link"]

REPORT_SCHEMA = "gf180-dx7.stress-report/1"


def _mk(name: str, kind: str, sample: int, **kw) -> F.LinkEvent:
    return F.LinkEvent(kind, sample, phrase_id=f"stress:{name}",
                       source=f"{name}@{sample}", **kw)


def build_events(name: str) -> list[F.LinkEvent]:
    """The scenario schedule (LinkEvent stream)."""
    evs: list[F.LinkEvent] = []
    if name == "fast_play":
        # 16-note-limit dense traffic: 64 on/off pairs over ~2 s, four
        # notes ever concurrent, velocities spread, no two on closer
        # than 15 ms (well inside a song frame)
        notes = [48, 55, 60, 64, 67, 71, 72, 76]
        base = 0
        i = 0
        while i < 64:
            for k, note in enumerate(notes):
                if i >= 64:
                    break
                pos = base + k * 720          # 15 ms spacing
                vel = 40 + (i * 7) % 88
                evs.append(_mk(name, "note_on", pos, note=note, velocity=vel))
                evs.append(_mk(name, "note_off", pos + 2400, note=note))   # 50 ms
                i += 1
            base += 12000                      # 250 ms per chord wave
        return evs
    if name == "sustain_pedal":
        # pedal down - six-note arpeggio - keys up while pedal holds
        # (notes keep sounding) - pedal up (notes die)
        pos = 4800
        evs.append(_mk(name, "controller", pos, cc=64, value=127))
        arp = [60, 64, 67, 71, 72, 76]
        for note in arp:
            evs.append(_mk(name, "note_on", pos, note=note, velocity=96))
            evs.append(_mk(name, "note_off", pos + 4800, note=note))
            pos += 9600                         # 200 ms per note
        evs.append(_mk(name, "controller", pos + 4800, cc=64, value=0))
        return evs
    if name == "patch_changes_while_held":
        # note A held - a second image committed WHILE A IS HELD (the
        # recall row, software leg) - note B - everything off
        pos = 4800
        evs.append(_mk(name, "note_on", pos, note=60, velocity=100))
        evs.append(_mk(name, "pause_before_commit", pos + 4800))
        evs.append(_mk(name, "note_on", pos + 9600, note=64, velocity=80))
        evs.append(_mk(name, "note_off", pos + 24000, note=60))
        evs.append(_mk(name, "note_off", pos + 28800, note=64))
        return evs
    if name == "all_sound_off":
        # eight notes + pedal + mod wheel, then everything off at once:
        # ANO (CC120) first, then each note-off
        pos = 4800
        evs.append(_mk(name, "controller", pos, cc=1, value=64))
        for k, note in enumerate([48, 55, 60, 64, 67, 71, 72, 76]):
            evs.append(_mk(name, "note_on", pos, note=note, velocity=96))
        end = pos + 24000
        evs.append(_mk(name, "controller", end, cc=120, value=127))
        for note in [48, 55, 60, 64, 67, 71, 72, 76]:
            evs.append(_mk(name, "note_off", end, note=note))
        return evs
    if name == "throttled_link":
        # the fast_play schedule replayed through a throttled link -
        # same events, corrupted transport (see run_scenario)
        return build_events("fast_play")
    raise ValueError(f"unknown scenario {name!r}")


def drive(host, core, evs: list[F.LinkEvent], block: int = 1) -> dict:
    """Send the event list through the host's production primitives.
    block: number of events sent between poll+flush cycles - the default 1
    is the fixture bench's one-song-frame-per-burst drive; the throttled
    scenario uses 2 so one frozen-drain window spans 9+ writes (a note-on
    pair), more than the skid FIFO depth, which is what forces the DEC-014
    rejections."""
    accounting = []
    i = 0
    while i < len(evs):
        chunk = evs[i:i + block]
        in_block = [f for ev in chunk
                    if ev.kind != "pause_before_commit"
                    for f in _host_frames_of(host, ev)[0]]
        if in_block:
            send_pred = core.clk // core.clks_per_frame + 1
            for f in in_block:
                host._send(f)
            host.poll()
            core.flush_frame()
            accounting.append({
                "sample": chunk[0].sample,
                "kinds": [e.kind for e in chunk
                          if e.kind != "pause_before_commit"],
                "send_frame_pred": send_pred,
                "n_writes": len(in_block)})
        i += block
    host.poll()
    core.flush_frame()
    return {"accounting": accounting}


def _host_frames_of(host, ev: F.LinkEvent):
    h = dict(kind=ev.kind, sample=ev.sample)
    if ev.kind == "note_on":
        h["note"], h["velocity"] = ev.note, ev.velocity
    elif ev.kind == "note_off":
        h["note"] = ev.note
    elif ev.kind == "controller":
        h["cc"], h["value"] = ev.cc, ev.value
    elif ev.kind == "bend":
        h["value"] = ev.value
    else:
        raise ValueError(f"unknown event kind {ev.kind!r}")
    return host._event_frames(h)


def check_core(host, core, allow_faults: set[str],
               require_overflow: bool = False) -> list[str]:
    """Shared end-of-scenario checks; returns problems (empty = pass)."""
    problems = []
    drains = list(core.drain_tap())
    log = [(f.sec, f.addr, f.data) for f in host.log]
    if len(log) != len(drains):
        problems.append(f"host accepted {len(log)} writes, core drained "
                        f"{len(drains)} (silent loss or phantom write)")
    else:
        for i in range(len(log)):
            if log[i] != (drains[i].sec, drains[i].addr, drains[i].data):
                problems.append(f"write {i}: host {log[i]} != drain "
                                f"({drains[i].sec:#x}/{drains[i].addr:#x}/"
                                f"{drains[i].data:#x})")
                break
    pins_c = B._collapse_pins(list(core.pins))
    if len(pins_c) != len(drains):
        problems.append(f"pin leg {len(pins_c)} (after DEC-014 "
                        f"collapse) != drain leg {len(drains)}")
    kinds: dict[str, int] = {}
    for fr in core.faults:
        kinds[fr.kind] = kinds.get(fr.kind, 0) + 1
    bad = {k: v for k, v in kinds.items() if k not in allow_faults}
    if bad:
        problems.append(f"disallowed fault kinds: {bad}")
    if core.sticky_overrun:
        problems.append("OVERRUN sticky set: schedule deadline missed")
    if require_overflow and kinds.get("queue_overflow", 0) < 1:
        problems.append("throttled run logged NO explicit queue-overflow "
                        "rejection: the underrun check has no resolution "
                        "(it cannot have caught the corruption it was "
                        "asked to catch)")
    if core.held:
        problems.append(f"notes still held at scenario end (stuck notes): "
                        f"{sorted(core.held)}")
    return problems


def run_scenario(name: str, voice) -> dict:
    throttled = name == "throttled_link"
    core: M.MockCore = B.ThrottledCore() if throttled else B.TappedCore()
    host = hostmod.EmbeddedHost(core, hostmod.DESIGN_POINT)
    evs = build_events(name)
    boot = host.boot(voice)
    if throttled:
        core.armed = True   # the deliberate underrun: from here on, every
        # accepted write re-arms a 3-transaction drain stall
    block = 2 if throttled else 1
    if name == "patch_changes_while_held":
        marker = next(i for i, e in enumerate(evs)
                      if e.kind == "pause_before_commit")
        run_info = drive(host, core, evs[:marker])
        host.load_patch(voice)          # commit WHILE note A is held
        tail = drive(host, core, evs[marker + 1:])
        run_info = {"accounting": run_info["accounting"]
                    + tail["accounting"]}
    else:
        run_info = drive(host, core, evs, block=block)
    allow = ({"queue_overflow", "drain_stall", "deferred_write"}
             if throttled else set())
    problems = check_core(host, core, allow,
                          require_overflow=throttled)
    kinds = {}
    for fr in core.faults:
        kinds[fr.kind] = kinds.get(fr.kind, 0) + 1
    applied = {}
    for e in core.applied_events():
        applied[e.kind] = applied.get(e.kind, 0) + 1
    return {
        "scenario": name,
        "status": "PASS" if not problems else "FAIL",
        "problems": problems,
        "events_sent": len([e for e in evs if e.kind
                            != "pause_before_commit"]),
        "image_commits": applied.get("commit", 0),
        "applied_events": applied,
        "fault_counts": kinds,
        "rejections": kinds.get("queue_overflow", 0),
        "notes_held_at_end": sorted(core.held),
        "n_writes": len(list(core.drain_tap())),
        "accounting": run_info["accounting"][:8],
        "accounting_total_bursts": len(run_info["accounting"]),
        "throttled": throttled,
        "boot": boot,
    }


def run_all(voice) -> dict:
    report = {"schema": REPORT_SCHEMA, "scenarios": {}}
    all_pass = True
    for name in SCENARIOS:
        r = run_scenario(name, voice)
        report["scenarios"][name] = r
        all_pass = all_pass and r["status"] == "PASS"
    report["status"] = "PASS" if all_pass else "FAIL"
    return report


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--scenario", default="all", choices=SCENARIOS + ["all"])
    ap.add_argument("--patch", default="default",
                    help="default (the fixture patch) or dev32:<hash> "
                         "(environment-gated on DX7_ALLTHEWEB_ZIP)")
    ap.add_argument("--report", default=None)
    args = ap.parse_args()

    if args.patch.startswith("dev32:"):
        import os
        zip_env = os.environ.get("DX7_ALLTHEWEB_ZIP")
        if not zip_env or not Path(zip_env).exists():
            print(json.dumps({"status": "NOT_RUN",
                              "reason": "dev32 patch requires "
                              "DX7_ALLTHEWEB_ZIP (environment gate); "
                              "a gated run is never reported as a pass"}))
            return 2
        voice = F.load_dev32_voice(args.patch.split(":", 1)[1],
                                   Path(zip_env))
    else:
        fx, _phrases = F.load_fixture(REPO / "spec" / "demo-audition-v1.json")
        voice = F.resolve_patch(fx)

    if args.scenario == "all":
        report = run_all(voice)
    else:
        r = run_scenario(args.scenario, voice)
        report = {"schema": REPORT_SCHEMA, "scenarios":
                  {args.scenario: r},
                  "status": "PASS" if r["status"] == "PASS" else "FAIL"}
    if args.report:
        rp = Path(args.report)
        if not rp.is_absolute():
            rp = REPO / rp
        rp.parent.mkdir(parents=True, exist_ok=True)
        rp.write_text(json.dumps(report, indent=2) + "\n")
    # keep stdout as the machine-readable summary
    summary = {
        "status": report["status"],
        "scenarios": {k: {"status": v["status"],
                          "rejections": v["rejections"],
                          "problems": v["problems"]}
                      for k, v in report["scenarios"].items()},
    }
    print(json.dumps(summary, indent=2))
    return 0 if report["status"] == "PASS" else (
        2 if report["status"] == "NOT_RUN" else 1)


if __name__ == "__main__":
    sys.exit(main())