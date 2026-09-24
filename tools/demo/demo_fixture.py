#!/usr/bin/env python3
"""U05 demo fixture bench: the committed playback fixture, driven through
the production host -> mock-core control path, with a three-way
cross-check at every frame. Exit codes: 0 pass, 1 fail, 2 refuse/not-run.

U05 (issue #38). Adapted from gf180-parasynth ``fpga/verify_fixture.py``
(sibling commit ``cbcc8b9e10e49c84f630550e2e145cc6da8a659c``, sha256
``d8f400c1c9fb5f3b9524fdce7984439beca6e62e5dc96ebe8beaa679d80b2c22``): the
sibling benches its committed playback fixture against a mock link and a
numpy audio simulation with ``--wrong <mode>`` / ``--expect-fail``, and
refuses (exit 2) a fixture that is silent or under-delivers relative to
the claims it is checked against. Provenance and ruling:
``docs/REUSE-RULINGS.md`` (U05 row) and ``docs/reuse/catalog.json``
(component ``u05-demo-fixture-bench``, ruling ``adapt``).

Honest differences from the sibling: the sibling compares an H03 stream
against a numpy audio simulation and checks a latency budget; this design's
control path is the U04 production host (``src/gf180_dx7/host.py``) driving
the U04 mock core (``src/gf180_dx7/mock_core.py``), which synthesizes no
audio. The three-way cross-check here is therefore
  (1) host prediction  - the (sec, addr, data) stream the production host
                         scheduled, in order (host.log), plus its
                         per-frame send-frame prediction derived from the
                         committed write-cost model (TXN_PERIOD_CLKS,
                         clks_per_frame, per-event write counts) - never
                         read back from the core;
  (2) CS-N pin         - the capture taken as each write crosses the link
                         (TappedCore.pin): the frame at transfer entry;
  (3) drain frame      - the frame in which each accepted write left the
                         skid FIFO into core state (mock drain tap, H03
                         section 4.6 style, added by this PR).
All three must agree, one write per accepted transaction, in order, and a
musical event's applied tap must land exactly one frame after its last
write drained (H03 4.2: applied before the first sample of frame N+1).
A failing or under-delivering fixture is REFUSED (exit 2) before a single
write reaches the link; the ported corruption modes (``--wrong``) plus the
U05 underrun negative control must each be caught by the named gates. The
mock keeps the clean contract in every run - corruption is injected on the
stream, the timing, the host, or the link, never by loosening a gate.
"""

from __future__ import annotations

import argparse
import json
import os
import sys
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
REPO = THIS_DIR.parents[1]
for _p in (str(THIS_DIR), str(REPO / "src"), str(REPO / "tools")):
    if _p not in sys.path:
        sys.path.insert(0, _p)

import fixtures_dx7 as F  # noqa: E402
from gf180_dx7 import host as hostmod  # noqa: E402
from gf180_dx7 import mock_core as M  # noqa: E402
from gf180_dx7 import sysex  # noqa: E402

TXN = M.TXN_PERIOD_CLKS
DD_BASE = M.ADDR_OP_DDELTA_BASE
DD_TOP = M.ADDR_OP_DDELTA_BASE + 5

CORRUPTION_MODES = [
    "none", "drop-deltas", "drop-restore", "late-window", "drop-bend",
    "drop-bend-step", "burst", "throttle", "drop-note-off",
]

# --wrong <mode> with --expect-fail: the NAMED gates that must go the
# expected way for the negative control to count (a control that fails the
# wrong gate, or fails when it should pass, is itself a failure).
EXPECTED_GATES = {
    "drop-deltas": [
        ("three_way_stream_agreement", False,
         "6 per-op detune-delta writes missing from pin and drain legs"),
    ],
    "drop-restore": [
        ("three_way_stream_agreement", False,
         "drain leg restarted at the pad reset; host log keeps the full "
         "stream"),
        ("commit_audit", False,
         "boot commit lost at reset; the no-recall host delivered nothing"),
    ],
    "late-window": [
        ("frame_landing_prediction", False,
         "the late batch lands off its predicted link frame"),
    ],
    "drop-bend": [
        ("three_way_stream_agreement", False,
         "all bend writes missing from pin and drain legs"),
    ],
    "drop-bend-step": [
        ("three_way_stream_agreement", False,
         "one bend step missing from pin and drain legs"),
    ],
    "burst": [
        ("frame_landing_prediction", False,
         "the collapsed pair shares one link frame"),
    ],
    "throttle": [
        ("no_contract_defects", True,
         "explicit DEC-014 queue-overflow rejections logged (underrun "
         "handling engaged)"),
        ("three_way_stream_agreement", True,
         "no silent loss: every host-accepted write drained exactly once, "
         "retried after each explicit rejection"),
    ],
    "drop-note-off": [
        ("no_stuck_notes", False,
         "the dropped final note-off leaves a note held at session end"),
    ],
}


# ---------------------------------------------------------------------------
# Observation + failure-injection classes (thin; read-only or directed)
# ---------------------------------------------------------------------------

class TappedCore(M.MockCore):
    """MockCore + the CS-N pin (capture at link-crossing, core side) +
    the directed stream failures used by ``--wrong``. The pin is read-only
    evidence; the directed drops fake an accepted result exactly like a
    wire dropping the transaction (the host's log keeps believing)."""

    def __init__(self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.pins: list[tuple[int, int, int, int]] = []
        self.drop_dd_left = 0          # next N per-op detune-delta writes
        self.drop_bend: str | int | None = None  # "all" | 0-based index
        self.drop_single_left = 0      # next N single-write CMD events
        self._bend_seen = 0

    def transfer(self, raw) -> M.TransactionResult:
        frame = M.Frame48.decode(raw)
        if not frame.is_read:
            if self.drop_dd_left > 0 and frame.sec == M.SEC_EVENTS and \
                    DD_BASE <= frame.addr <= DD_TOP:
                self.drop_dd_left -= 1
                return M.TransactionResult(True, 48, "directed drop")
            if frame.sec == M.SEC_EVENTS and frame.addr == M.ADDR_BEND_PB:
                if self.drop_bend == "all" or \
                        (isinstance(self.drop_bend, int)
                         and self._bend_seen == self.drop_bend):
                    self._bend_seen += 1
                    if isinstance(self.drop_bend, int):
                        self.drop_bend = None
                    return M.TransactionResult(True, 48, "directed drop")
                self._bend_seen += 1
            if self.drop_single_left > 0 and frame.sec == M.SEC_EVENTS and \
                    frame.addr == M.ADDR_CMD:
                self.drop_single_left -= 1
                return M.TransactionResult(True, 48, "directed drop")
            self.pins.append((self.frame_now, frame.sec, frame.addr,
                              frame.data))
        return super().transfer(raw)

    def late_tick(self, frames: int) -> None:
        """Directed timing fault: the core advances K frames before the
        next batch crosses the link (a late window)."""
        self.tick(frames * self.clks_per_frame)


class NoRecallHost(hostmod.EmbeddedHost):
    """Directed failure: recall() claims success but delivers nothing.
    The core stays reset (fresh, empty) and the play continues on it."""

    def __init__(self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.recall_undelivered = False

    def recall(self):
        self.recall_undelivered = True
        return {"recall": True, "writes": 0,
                "note": "directed failure: recall not delivered"}


class ThrottledCore(TappedCore):
    """Directed failure - the U05 underrun negative control: a link that
    undershoots the drain budget. Once armed, each accepted write arms a
    one-shot drain stall (three transaction windows, sized so the host's
    immediate DEC-014 retries always recover - a recovery-guaranteed
    magnitude): the skid FIFO fills to its bound and H03 DEC-014 must
    reject explicitly (overflow status, fault logged); the host reads
    status, sees the explicit rejection, and retries - never a silent
    drop or deferral. The run completes only because the rejection is
    loud and the retry recovers; the gates prove no write was silently
    lost and the rejection count resolves the check (a link that never
    rejects, even while the same stalls would accumulate, cannot pass).
    Scope note: at the recovery-guaranteed magnitude the per-burst landing
    gate is unchanged (stalls reabsorb within the frame); a stall long
    enough to drift landings would exhaust the mock host's two immediate
    retries - recorded as a host finding (retry backoff is out of H03
    scope and of U05 scope)."""

    def __init__(self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.armed = False

    def transfer(self, raw) -> M.TransactionResult:
        res = super().transfer(raw)
        if res.accepted and not M.Frame48.decode(raw).is_read \
                and self.armed:
            self.injection.stall_drain_clks = 3 * TXN
        return res


# ---------------------------------------------------------------------------
# The drive: one playthrough burst (song frame) per link frame, via the
# host's production primitives (the same send/poll/flush sequence
# schedule_burst uses per frame; equivalence with schedule_burst itself is
# asserted by the fast tests on a shared schedule).
# ---------------------------------------------------------------------------

def _host_frames(host, evs) -> tuple[list, list[str]]:
    frames: list = []
    tags: list[str] = []
    for ev in evs:
        h = dict(kind=ev.kind, sample=ev.sample)
        if ev.kind == "note_on":
            h["note"], h["velocity"] = ev.note, ev.velocity
        elif ev.kind == "note_off":
            h["note"] = ev.note
        elif ev.kind == "controller":
            h["cc"], h["value"] = ev.cc, ev.value
        elif ev.kind == "bend":
            h["value"] = ev.value
        f, t = host._event_frames(h)
        frames.extend(f)
        tags.append(t)
    return frames, tags


def _burst_info(play: dict) -> list[dict]:
    by_frame: dict[int, list] = {}
    for ev in play["events"]:
        by_frame.setdefault(ev.frame, []).append(ev)
    info = []
    for sf in sorted(by_frame):
        evs = by_frame[sf]
        info.append({
            "song_frame": sf,
            "evs": evs,
            "has_note_on": any(e.kind == "note_on" for e in evs),
            "has_bend": any(e.kind == "bend" for e in evs),
            "has_note_off": any(e.kind == "note_off" for e in evs),
        })
    return info


def drive_playthrough(host, core, play: dict, mode: str) -> dict:
    """Drive the playthrough burst by burst. Returns per-burst accounting:
    [{burst, song_frame, send_frame_pred, tags, n_writes}]."""
    bursts = _burst_info(play)
    n = len(bursts)
    first_bend = next((i for i, b in enumerate(bursts) if b["has_bend"]),
                      None)
    last_note_on = max((i for i, b in enumerate(bursts) if b["has_note_on"]),
                       default=None)
    last_note_off = max((i for i, b in enumerate(bursts) if b["has_note_off"]),
                        default=None)
    burst_collapse = n - 2 if mode == "burst" and n >= 2 else None

    accounting = []
    for i, b in enumerate(bursts):
        if mode == "late-window" and i == last_note_on:
            core.late_tick(2)
        elif mode == "drop-deltas" and i == last_note_on:
            core.drop_dd_left = 6      # this burst is a lone note_on
        elif mode == "drop-bend" and i == first_bend:
            core.drop_bend = "all"
        elif mode == "drop-bend-step" and i == first_bend:
            core.drop_bend = 2         # the 3rd bend write of the play
        elif mode == "drop-note-off" and i == last_note_off:
            core.drop_single_left = 1
        frames, tags = _host_frames(host, b["evs"])
        send_pred = core.clk // core.clks_per_frame + 1
        for f in frames:
            host._send(f)
        if i != burst_collapse:
            host.poll()
            core.flush_frame()
        accounting.append({
            "burst": i, "song_frame": b["song_frame"],
            "send_frame_pred": send_pred, "tags": tags,
            "n_writes": len(frames),
        })
    host.poll()
    core.flush_frame()      # session end: drain the tail
    return {"accounting": accounting}


def drive_with_reset(host, core, play: dict) -> dict:
    """drop-restore drive: play until the directed pad reset fires
    (detected by the host's own contract mapping on the next status poll);
    the no-recall host swallows it and the play continues on the fresh
    (committed-image-less) core."""
    info: dict = {}
    for b in _burst_info(play):
        frames, _tags = _host_frames(host, b["evs"])
        for f in frames:
            try:
                host._send(f)
            except hostmod.HostError as e:
                info.setdefault("send_errors", []).append(str(e))
        try:
            host.poll()
        except hostmod.ResetObserved as e:
            info["reset_observed_at_song_frame"] = b["song_frame"]
            info["reset_observed"] = str(e)
            host.recall()
            info["recall_undelivered"] = host.recall_undelivered
        core.flush_frame()
    host.poll()
    core.flush_frame()
    return info


def expected_send_frames(play: dict, boot_land_frame: int) -> list[int]:
    """Pure prediction from committed constants: each playthrough burst is
    one link frame (worst burst 8 writes x 194 clk + 194 poll < 32768),
    the first landing at boot_land_frame."""
    n_bursts = len({e.frame for e in play["events"]})
    return [boot_land_frame + i for i in range(n_bursts)]


# ---------------------------------------------------------------------------
# The run
# ---------------------------------------------------------------------------

def run_bench(fixture_path, profile: str, mode: str, expect_fail: bool,
              expect_refuse: bool = False,
              patch_override: str | None = None,
              report_path=None) -> int:
    fixture_path = Path(fixture_path)
    if not fixture_path.is_absolute():
        fixture_path = REPO / fixture_path

    report = {
        "schema": "gf180-dx7.demo-bench-report/1",
        "fixture": str(fixture_path.relative_to(REPO))
        if fixture_path.is_relative_to(REPO) else str(fixture_path),
        "profile": profile, "mode": mode, "expect_fail": expect_fail,
        "expect_refuse": expect_refuse,
    }
    if mode == "none" and expect_fail:
        report["status"] = "REFUSE"
        report["reason"] = ("expect-fail requires a named corruption mode: "
                            "a clean run has no named gates to verify "
                            "against (ap.error guard in main(), enforced "
                            "here for programmatic callers)")
        return _finish(report, report_path, expect_fail, expect_refuse)

    # --- refuse an imperfect fixture BEFORE any write reaches the link ----
    try:
        fx, phrases = F.load_fixture(fixture_path)
        F.check_superset_of_v1(REPO / "spec" / "audition-phrases-v1.json",
                               phrases)
        play = F.assemble_playthrough(fx, phrases, profile)
        cov = F.coverage_from_schedule(play["events"])
        problems = F.check_want(cov, fx, profile)
    except Exception as e:  # noqa: BLE001
        report["status"] = "REFUSE"
        report["reason"] = f"{type(e).__name__}: {e}"
        return _finish(report, report_path, expect_fail, expect_refuse)
    if problems:
        report["status"] = "REFUSE"
        report["coverage_problems"] = problems
        report["coverage"] = cov
        return _finish(report, report_path, expect_fail, expect_refuse)
    # fixture integrity: the counts pinned in the fixture file must equal
    # the counts computed from the schedule (a fixture that lies about
    # its own expected counts is a broken fixture)
    pinned = (fx.get("expected_counts") or {}).get(profile) or {}
    mismatch = {k: (pinned.get(k), play["counts"][k])
                for k in play["counts"]
                if pinned.get(k) is not None and pinned[k] != play["counts"][k]}
    missing = [k for k in play["counts"] if k not in pinned]
    if mismatch or missing:
        report["status"] = "REFUSE"
        report["reason"] = (f"expected_counts pinned in the fixture do not "
                            f"match the computed schedule counts: "
                            f"mismatch={mismatch} missing={missing}")
        return _finish(report, report_path, expect_fail, expect_refuse)
    report["coverage"] = cov
    report["counts_expected"] = play["counts"]

    # --- patch identity ----------------------------------------------------
    patch_info = dict(fx["patch"])
    try:
        if patch_override and patch_override.startswith("dev32:"):
            target = patch_override.split(":", 1)[1]
            zip_env = os.environ.get("DX7_ALLTHEWEB_ZIP")
            if not zip_env or not Path(zip_env).exists():
                report["status"] = "NOT_RUN"
                report["patch_override"] = patch_override
                report["reason"] = ("dev32 variant requires the pinned "
                                    "AllTheWeb archive "
                                    "(env DX7_ALLTHEWEB_ZIP); environment "
                                    "gate not met - a gated run is never "
                                    "reported as a pass")
                return _finish(report, report_path, expect_fail,
                               expect_refuse)
            try:
                voice = F.load_dev32_voice(target, Path(zip_env))
            except (PermissionError, OSError) as e:
                report["status"] = "NOT_RUN"
                report["patch_override"] = patch_override
                report["reason"] = (f"AllTheWeb archive present but "
                                    f"unreadable ({e}); environment gate "
                                    f"not met - never reported as a pass")
                return _finish(report, report_path, expect_fail,
                               expect_refuse)
            patch_info = {"source": f"dev32:{target}",
                          "canonical_hash": target, "name": voice.name,
                          "env_gate": "DX7_ALLTHEWEB_ZIP present"}
        else:
            voice = F.resolve_patch(fx)
    except Exception as e:  # noqa: BLE001
        report["status"] = "REFUSE"
        report["reason"] = f"patch: {type(e).__name__}: {e}"
        return _finish(report, report_path, expect_fail, expect_refuse)
    report["patch"] = patch_info
    expected_digest = sysex.checksum(voice.raw_bytes)
    report["expected_digest"] = expected_digest

    # --- core + host --------------------------------------------------------
    if mode == "throttle":
        core: M.MockCore = ThrottledCore()
    else:
        core = TappedCore()
    if mode == "drop-restore":
        host = NoRecallHost(core, hostmod.DESIGN_POINT)
    else:
        host = hostmod.EmbeddedHost(core, hostmod.DESIGN_POINT)

    try:
        boot_report = host.boot(voice)
    except Exception as e:  # noqa: BLE001
        report["status"] = "FAIL"
        report["boot_error"] = f"{type(e).__name__}: {e}"
        return _finish(report, report_path, expect_fail, expect_refuse)
    report["boot"] = {
        "writes": boot_report["writes"],
        "flush_cycles": boot_report["frames"],
        "land_frame": core.clk // core.clks_per_frame + 1,
        "generation": boot_report.get("generation"),
    }
    run_info: dict = {"d0": len(core.drain_tap())}
    d0_drains = run_info.pop("d0")

    if mode == "drop-restore":
        # directed pad reset after the first playthrough burst's writes
        first_burst_writes = len(_host_frames(host, _burst_info(play)[0]["evs"])[0])
        core.injection.reset_after_writes = \
            boot_report["writes"] + first_burst_writes
        run_info = drive_with_reset(host, core, play)
    else:
        if mode == "throttle":
            core.armed = True
        run_info = drive_playthrough(host, core, play, mode)
    run_info["drains_at_play_start"] = d0_drains
    run_info["drains_at_play_end"] = len(core.drain_tap())

    # --- second commit (the fixture's recall check) -------------------------
    commit_report = None
    if play["second_commit_after"]:
        try:
            commit_report = host.load_patch(voice)
        except Exception as e:  # noqa: BLE001
            run_info["second_commit_error"] = (f"{type(e).__name__}: {e}")

    # --- gates ---------------------------------------------------------------
    gates = _evaluate_gates(host, core, play, mode, run_info,
                            commit_report, expected_digest)
    report["gates"] = gates
    report["run_info"] = run_info
    report["faults"] = [{"kind": f.kind, "detail": f.detail, "frame": f.frame}
                        for f in core.faults]
    report["counts_observed"] = {
        "pin_writes": len(core.pins),
        "pin_writes_collapse_dec014": _collapse_pins(core.pins).__len__(),
        "drain_writes": len(core.drain_tap()),
        "host_log_writes": len(host.log),
        "applied_events": _applied_counts(core),
    }
    if expect_fail:
        bad = []
        for gate, wanted, why in EXPECTED_GATES[mode]:
            got = gates[gate]["pass"]
            if got != wanted:
                bad.append(f"gate {gate} pass={got}, expected {wanted} "
                           f"({why})")
        report["status"] = "FAIL" if bad else "PASS"
        report["expect_fail_verification"] = {
            "passed": not bad, "problems": bad,
            "expected": {g: w for g, w, _ in EXPECTED_GATES[mode]},
        }
    else:
        failures = [g for g, v in gates.items() if not v["pass"]]
        report["status"] = "FAIL" if failures else "PASS"
    return _finish(report, report_path, expect_fail, expect_refuse)


def _collapse_pins(pins: list[tuple]) -> list[tuple]:
    """Collapse DEC-014 retry duplicates: a rejected write is re-sent, so
    the pin leg shows two records for one accepted write (same content,
    consecutive). Consecutive same-content records collapse to one."""
    out: list[tuple] = []
    for p in pins:
        if out and out[-1][1:] == p[1:]:
            continue
        out.append(p)
    return out


def _applied_counts(core) -> dict[str, int]:
    out: dict[str, int] = {}
    for e in core.applied_events():
        out[e.kind] = out.get(e.kind, 0) + 1
    return out


def _evaluate_gates(host, core, play: dict, mode: str, run_info: dict,
                    commit_report, expected_digest: int) -> dict:
    gates: dict[str, dict] = {}
    pins = list(core.pins)
    pins_c = _collapse_pins(pins)
    drains = list(core.drain_tap())
    # the playthrough's slice of the drain leg (boot and the second commit
    # are image traffic; the event gates reason about play traffic only)
    d0 = run_info.get("drains_at_play_start")
    d1 = run_info.get("drains_at_play_end")
    play_drains = (drains[d0:d1]
                   if d0 is not None and d1 is not None and d1 >= d0
                   else [])
    log_keys = [(f.sec, f.addr, f.data) for f in host.log]
    expected_counts = play["counts"]

    # G1: three-way stream agreement (order + content), 1:1 by acceptance
    g1 = []
    if len(log_keys) != len(drains):
        g1.append(f"host accepted {len(log_keys)} writes, core drained "
                  f"{len(drains)}")
    for i in range(min(len(log_keys), len(drains))):
        if log_keys[i] != (drains[i].sec, drains[i].addr, drains[i].data):
            g1.append(f"write {i}: host {log_keys[i]} != drain "
                      f"({drains[i].sec:#x}/{drains[i].addr:#x}/"
                      f"{drains[i].data:#x})")
            break
    if len(pins_c) != len(drains):
        g1.append(f"pin leg {len(pins_c)} (after DEC-014 collapse) != "
                  f"drain leg {len(drains)}; raw pins {len(pins)}")
    for i in range(min(len(pins_c), len(drains))):
        pc, d = pins_c[i], drains[i]
        if (pc[1], pc[2], pc[3]) != (d.sec, d.addr, d.data):
            g1.append(f"write {i}: pin ({pc[1]:#x}/{pc[2]:#x}) != drain "
                      f"({d.sec:#x}/{d.addr:#x})")
            break
    if len(log_keys) != expected_counts["total_write_windows"]:
        g1.append(f"host log {len(log_keys)} != fixture expected "
                  f"{expected_counts['total_write_windows']}")
    gates["three_way_stream_agreement"] = {
        "pass": not g1 and len(drains) == expected_counts[
            "total_write_windows"],
        "detail": g1 or f"host prediction == CS-N pin == drain frame, 1:1 "
        f"for all {len(drains)} accepted writes",
    }

    # G2: per-burst landing frames == the contract-derived prediction
    acc = run_info.get("accounting")
    boot_land = (acc[0]["send_frame_pred"] if acc else None)
    g2 = []
    if not acc or boot_land is None:
        # drop-restore drive: no per-burst accounting (pad reset mid-play)
        g2.append("no per-burst accounting (pad reset mid-play): "
                  "link-frame prediction is undefined across the reset")
    else:
        pred = expected_send_frames(play, boot_land)
        if len(acc) != len(pred):
            g2.append(f"burst accounting {len(acc)} != predicted "
                      f"{len(pred)}")
        offsets = []
        total = 0
        for a in acc:
            offsets.append(total)
            total += a["n_writes"]
        if total != len(play_drains):
            g2.append(f"accounting write total {total} != play-drain count "
                      f"{len(play_drains)} (attribution impossible)")
        else:
            for i, a in enumerate(acc):
                d0, d1 = offsets[i], offsets[i] + a["n_writes"]
                if i >= len(pred):
                    break
                frames = {play_drains[j].frame
                          for j in range(d0, d1)}
                if len(frames) > 1:
                    g2.append(f"burst {i} drained across frames "
                              f"{sorted(frames)} (predicted {pred[i]})")
                elif play_drains[d0].frame != pred[i]:
                    g2.append(f"burst {i} (song frame {a['song_frame']}) "
                              f"landed at frame {play_drains[d0].frame}, "
                              f"predicted {pred[i]} "
                              f"({play_drains[d0].frame - pred[i]} late)")
    gates["frame_landing_prediction"] = {
        "pass": not g2,
        "detail": g2 or f"all {len(acc)} bursts landed exactly on their "
        f"predicted link frames",
    }

    # G3: applied timing - each note event's applied tap lands exactly one
    # frame after its last write drained (H03 4.2)
    g3 = []
    applied = list(core.applied_events())
    note_records = [e for e in applied if e.kind in ("note_on", "note_off")]
    play_cmds = [d for d in play_drains
                 if d.addr == M.ADDR_CMD and
                 ((d.data >> 16) & 0xFFFF) != M.CMD_COMMIT]
    note_seq = [e for e in play["events"] if e.kind in ("note_on",
                                                        "note_off")]
    if len(note_records) != len(note_seq):
        g3.append(f"applied note events {len(note_records)} != scheduled "
                  f"{len(note_seq)}")
    else:
        cmd_i = 0
        for rec, ev in zip(note_records, note_seq):
            if cmd_i >= len(play_cmds):
                g3.append(f"run short at event {ev.source!r}")
                break
            # each note event carries exactly one CMD write - and it is the
            # event's LAST write (vel + 6 DDs precede the CMD)
            last_drain = play_cmds[cmd_i]
            cmd_i += 1
            if rec.frame != last_drain.frame + 1:
                g3.append(f"event {ev.source!r} applied frame {rec.frame} "
                          f"!= last-write drain {last_drain.frame} + 1")
                break
    gates["applied_timing_h03_4_2"] = {
        "pass": not g3,
        "detail": g3 or "every note event applied exactly one frame after "
        "its last write drained (frame N+1 rule)",
    }

    # G4: commit audit - expected commit count, digest-verified generations
    g4 = []
    commits = [e for e in applied if e.kind == "commit"]
    refusals = [e for e in applied if e.kind == "commit_refused"]
    if len(commits) != expected_counts["commits_applied_expected"]:
        g4.append(f"commits applied {len(commits)} != expected "
                  f"{expected_counts['commits_applied_expected']}")
    if refusals:
        g4.append(f"commit refusals logged: {len(refusals)}")
    gen_expected = expected_counts["generations_expected"]
    if core.generation != gen_expected:
        g4.append(f"core generation {core.generation} != expected "
                  f"{gen_expected}")
    try:
        if core.committed_digest() != expected_digest:
            g4.append(f"committed digest {core.committed_digest():#x} != "
                      f"expected {expected_digest:#x}")
    except (IndexError, KeyError):
        g4.append("no committed image in the ring")
    if commit_report is not None and \
            commit_report.get("digest") != expected_digest:
        g4.append("second-commit digest mismatch")
    gates["commit_audit"] = {
        "pass": not g4,
        "detail": g4 or f"{len(commits)} digest-verified commit(s), "
        f"generation {core.generation}",
    }

    # G5: no contract defects (OVERRUN must never set; faults audited)
    g5 = []
    if core.sticky_overrun:
        g5.append("OVERRUN sticky: schedule deadline missed (defect)")
    fault_kinds: dict[str, int] = {}
    for f in core.faults:
        fault_kinds[f.kind] = fault_kinds.get(f.kind, 0) + 1
    if mode == "throttle":
        if fault_kinds.get("queue_overflow", 0) < 1:
            g5.append("throttle run: no explicit queue-overflow rejection "
                      "logged - the underrun path did not engage (the "
                      "check has no resolution)")
        bad_kinds = [k for k in fault_kinds
                     if k not in ("queue_overflow", "drain_stall",
                                  "deferred_write")]
        if bad_kinds:
            g5.append(f"unexpected fault kinds: {sorted(bad_kinds)}")
        detail = ("underrun handling engaged: "
                  f"{fault_kinds.get('queue_overflow', 0)} explicit "
                  "DEC-014 rejection(s), "
                  f"{fault_kinds.get('drain_stall', 0)} drain stall(s), "
                  f"{fault_kinds.get('deferred_write', 0)} "
                  "deferred-boundary record(s); the host retried every "
                  "rejection - see three_way_stream_agreement for the no-"
                  "silent-loss proof")
    elif mode == "drop-restore":
        bad_kinds = [k for k in fault_kinds
                     if k not in ("reset_mid_transfer",)]
        if bad_kinds:
            g5.append(f"unexpected fault kinds: {sorted(bad_kinds)}")
        detail = ("directed pad reset recorded "
                  f"({fault_kinds.get('reset_mid_transfer', 0)}x); "
                  "OVERRUN never set")
    else:
        if fault_kinds:
            g5.append(f"unexpected faults: {fault_kinds}")
        detail = "no faults; OVERRUN never set"
    gates["no_contract_defects"] = {
        "pass": not g5, "detail": detail, "fault_counts": fault_kinds}

    # G6: session end - no stuck notes, no 16-bit frame counter wrap
    g6 = []
    if core.held:
        g6.append(f"notes still held at session end (stuck notes): "
                  f"{sorted(core.held)}")
    if core.frame_count >= 0x10000:
        g6.append("16-bit frame counter wrapped")
    gates["no_stuck_notes"] = {
        "pass": not g6,
        "detail": g6 or f"{len(core.held)} notes held at end; frame "
        f"counter {core.frame_count} < 2^16",
    }

    # G7: applied/event counts match the committed fixture's expected
    # counts (anti-circular: expected comes from the fixture, observed from
    # the core taps)
    g7 = []
    observed = _applied_counts(core)
    dsec = [d for d in play_drains if d.sec == M.SEC_EVENTS]
    bench = {
        "note_on": observed.get("note_on", 0),
        "note_off": observed.get("note_off", 0),
        "controller": observed.get("controller", 0),
        "bend": len([d for d in dsec if d.addr == M.ADDR_BEND_PB]),
        "event_writes": len(dsec),
        "commits_applied": observed.get("commit", 0),
        "generations": core.generation,
    }
    for k in ("note_on", "note_off", "controller"):
        if bench[k] != expected_counts[k]:
            g7.append(f"{k}: observed {bench[k]} != expected "
                      f"{expected_counts[k]}")
    if bench["event_writes"] != expected_counts["event_writes"]:
        g7.append(f"event write windows: observed {bench['event_writes']} "
                  f"!= expected {expected_counts['event_writes']}")
    gates["expected_event_counts"] = {
        "pass": not g7, "detail": g7 or
        (f"observed note_on={bench['note_on']} note_off={bench['note_off']} "
         f"cc={bench['controller']} bend={bench['bend']} "
         f"commits={bench['commits_applied']}"),
        "bench_counts": bench,
    }
    return gates


def _finish(report: dict, report_path, expect_fail: bool,
            expect_refuse: bool) -> int:
    if report_path is not None:
        report_path = Path(report_path)
        if not report_path.is_absolute():
            report_path = REPO / report_path
        report_path.parent.mkdir(parents=True, exist_ok=True)
        try:
            report["written"] = (report_path.resolve()
                                 .relative_to(REPO.resolve())
                                 .as_posix())
        except ValueError:
            report["written"] = report_path.name
        report_path.write_text(json.dumps(report, indent=2) + "\n")
    status = report["status"]
    print(json.dumps({"status": status,
                      "gates": {k: v["pass"] for k, v in
                                report.get("gates", {}).items()}},
                     indent=2))
    if status == "REFUSE" or status == "NOT_RUN":
        # a refuse/not-run is the correct answer only when it was expected
        if expect_refuse:
            return 0
        return 2
    if status == "PASS":
        return 0
    return 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--fixture", default="spec/demo-audition-v1.json")
    ap.add_argument("--profile", default="compact", choices=["compact",
                                                             "full"])
    ap.add_argument("--wrong", default="none", choices=CORRUPTION_MODES,
                    help="corruption mode (negative control); the model "
                         "keeps the clean contract, the stream/timing/"
                         "host/link is corrupted")
    ap.add_argument("--expect-fail", action="store_true",
                    help="assert the named corruption IS caught by its "
                         "named gates")
    ap.add_argument("--expect-refuse", action="store_true",
                    help="assert the fixture is REFUSED (coverage/"
                         "identity shortfall) before any write")
    ap.add_argument("--patch", default=None,
                    help="patch override: dev32:<canonical_hash> "
                         "(environment-gated on DX7_ALLTHEWEB_ZIP)")
    ap.add_argument("--report", default=None,
                    help="write the full report JSON here")
    args = ap.parse_args()
    if args.wrong == "none" and args.expect_fail:
        ap.error("--expect-fail requires --wrong <mode>")
    return run_bench(args.fixture, args.profile, args.wrong, args.expect_fail,
                     expect_refuse=args.expect_refuse,
                     patch_override=args.patch, report_path=args.report)


if __name__ == "__main__":
    sys.exit(main())