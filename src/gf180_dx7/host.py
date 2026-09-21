"""Embedded host library: MIDI-event -> core-transaction scheduling under
the H03 contract (issue #37, U04; docs/CONTRACT-CORE-v1.md).

The host is the SPI master of the H03 link. It converts MIDI/library
activity into the contracted 48-bit transactions, schedules them against
the per-frame link budget, loads patches from P01 Voice/bank bytes with
atomic commits, applies the reset/boot/recall protocol, and enforces the
contract's error rules (explicit rejection, never silent loss):

  * Event timing (H03 section 4.2): an event received during frame N is
    applied before the first sample of frame N+1; the scheduler packs a
    frame's transactions into the frame window (each transaction = 194
    core clocks, H03 section 2 row 15) and REFUSES an over-capacity
    frame with an explicit, enumerated backpressure error - silent
    deferral to a later frame is forbidden (DEC-014).
  * Patch load/commit (H03 section 4.1): the 155 P01 voice bytes + the
    host-frozen derived integers go to the SEC=1 shadow image; a load
    digest (P01 checksum) is verified by the core at the frame-boundary
    commit; the host verifies acceptance via the section 4.6 applied-
    event tap before declaring success. A truncated or bad-checksum
    load therefore leaves the committed image unchanged AND fails
    loudly - the stale-state negative control of issue #37.
  * Link budget (H03 section 2 rows 9-15, section 4.1/4.2): per-event
    write costs are counted (note-on 8, note-off 1, controller 1, bend
    1; worst admissible burst 160 writes vs the 168-write 24.576 MHz
    capacity, floor(32768/194)); a full boot image costs 171 writes <=
    the declared 216 bound and absorbs across 2 frame windows at the
    design point.
  * Reset/boot/recall (H03 section 4.4): boot waits for FRESH=1, sends
    the patch image + performance settings, and the commit lands
    atomically at a frame boundary; FRESH observed later (brown-out,
    pad reset) aborts the load and recall re-delivers the last
    successfully committed image byte-identically.
  * Patch-change-while-held (H03 section 4.1, DEC-019/DEC-021): the
    host books each note-on against the generation live at its
    application boundary; the mock's 4-entry ring keeps held notes
    exact for up to 3 subsequent commits (the declared conformance gap
    is surfaced as generation_evicted metadata, never silent).

The frozen integers (per-op detune deltas, fine terms, PM/AM depths)
mirror the pinned formulas of src/gf180_dx7/model/pitch.py (N05, the
frozen integer model; H06 section 2 documents the host/core split).

Claim boundary: this qualifies the HOST SIDE against a mock core. No
RTL, board, synthesis, original-DX7 fidelity, or musical-usefulness
claim is made or implied (AGENTS.md).
"""

from __future__ import annotations

import hashlib
import math

from . import mock_core as mc
from . import sysex
from .mock_core import (CLOCKS, FRAME_SAMPLES, PATCH_VOICE_BYTES,
                        TXN_PERIOD_CLKS, Frame48)
from .model.pitch import PITCHMODSENSTAB, i32, midinote_to_logfreq

# -- link budget constants (H03 sections 2/4.1/4.2; cited in docstrings) ---

EVENTS_PER_SEC = 48_000
DESIGN_POINT = "clk_24m576"
FALLBACK = "clk_12m288"

# Capacity per frame window = floor(clks_per_frame / 194) (H03 section 2
# row 15: 84 @12.288 MHz, 168 @24.576 MHz). U04 reserves one window per
# frame for the status poll (declared; the contract fixes write windows).
STATUS_POLL_WINDOWS = 1

# U04 transaction map (H06 section 2 register conventions, adapted):
NOTE_ON_WRITES = 8    # 1 velocity + 6 per-op detune deltas + 1 CMD
NOTE_OFF_WRITES = 1   # CMD only
CONTROLLER_WRITES = 1  # CTRL {cc, value}
BEND_WRITES = 1       # BEND_PB frozen int32
# Boot image: 155 voice bytes + 6 fine terms + PM/AM depths (3) + digest
# = 165 shadow writes; + 5 direct performance settings (DEC-022) + 1 CMD
# commit = 171 <= 216 declared (H03 section 4.1).
BOOT_IMAGE_WRITES = PATCH_VOICE_BYTES + 6 + 3 + 1 + 5 + 1
LOAD_COST_BOUND = 216          # H03 section 4.1 declared bound
WORST_BURST_WRITES = (16 * NOTE_ON_WRITES + 16 * NOTE_OFF_WRITES
                      + 16 * CONTROLLER_WRITES)


class HostError(Exception):
    """Base class for host-side contract errors (contract errors, not
    transport quirks: the host never drops or defers silently)."""


class TruncatedTransfer(HostError):
    """A transaction window delivered fewer than 48 bits."""


class QueueOverflow(HostError):
    """The core rejected a write (DEC-014); the host retries explicitly."""


class BackpressureRefused(HostError):
    """A frame's scheduled traffic exceeds the link capacity; the excess
    is enumerated in `events`, never silently deferred."""


class PatchLoadRefused(HostError):
    """The core refused the commit (digest mismatch / truncation); the
    committed image is unchanged."""


class ResetObserved(HostError):
    """FRESH=1 observed mid-operation (pad reset / brown-out); recall
    protocol required (H03 section 4.4)."""


class ContractDefect(HostError):
    """OVERRUN or a deferred-write defect: must never set (H03 4.3)."""


class MalformedPatch(HostError):
    """P01 codec rejected the SysEx; the P01 error is the __cause__."""


def parse_status(word: int) -> dict:
    """Split the 32-bit status word (H03 section 4.3)."""
    if word >> 24 != 0xD7:
        raise HostError(f"status magic {word >> 24:#x} != 0xD7")
    return {
        "raw": word,
        "version": (word >> 20) & 0xF,
        "overrun": bool(word >> 19 & 1),
        "queue_nonempty": bool(word >> 18 & 1),
        "overflow": bool(word >> 17 & 1),
        "fresh": bool(word >> 16 & 1),
        "frame": word & 0xFFFF,
    }


def frame_of_sample(sample: int) -> int:
    """H03 section 1: 48 kHz in 64-sample frames; frame N holds samples
    64N..64N+63 (frame numbering matches the core's FRAME counter)."""
    return sample // FRAME_SAMPLES


def capacity(clock: str) -> int:
    """Write windows per frame at `clock` (H03 section 2 row 15)."""
    _, cpf = CLOCKS[clock]
    return cpf // TXN_PERIOD_CLKS


def link_seconds(writes: int, clock: str) -> float:
    """Link time for `writes` transaction windows at `clock`."""
    return writes * TXN_PERIOD_CLKS / CLOCKS[clock][0]


def detune_delta(logfreq: int, detune: int) -> int:
    """Host-frozen per-op detune delta: the dx7note.cc:46-47 float gate
    (double math, truncation toward zero), mirrored from
    model/pitch.py::osc_freq (H06 section 2 OP_DDELTA row)."""
    detune_ratio = 0.0209 * math.exp(
        -0.396 * (float(logfreq) / (1 << 24))) / 7
    detuned = i32(int(logfreq + detune_ratio * logfreq * (detune - 7)))
    return i32(detuned - logfreq)


def fine_term(fine: int) -> int:
    """Host-frozen fine term (dx7note.cc:50-53; H06 OP_FTERM row)."""
    if fine == 0:
        return 0
    return i32(int(math.floor(
        24204406.323123 * math.log(1 + 0.01 * fine) + 0.5)))


class EmbeddedHost:
    """SPI-master host for the H03 core link."""

    def __init__(self, core, clock: str = DESIGN_POINT) -> None:
        self.core = core
        self.clock = clock
        self.log: list[Frame48] = []
        self.committed_voice: sysex.Voice | None = None
        self.committed_generation = 0
        self._good_image: list[Frame48] | None = None
        self._last_frame: int | None = None

    # -- low-level link ------------------------------------------------------

    def _send(self, frame: Frame48, *, retries: int = 2) -> None:
        """One write window with DEC-014 retry: an overflow rejection is
        read (a full status window), cleared, and the write re-sent
        explicitly - never a silent re-drop."""
        raw = frame.encode()
        for attempt in range(retries + 1):
            result = self.core.transfer(raw)
            if result.accepted:
                self.log.append(frame)
                return
            if result.bits_delivered < 48:
                raise TruncatedTransfer(
                    f"transaction delivered {result.bits_delivered}/48 bits")
            status = self._read_status()
            if status["overflow"]:
                if attempt == retries:
                    raise QueueOverflow(
                        f"core rejected write {frame.sec:#x}/{frame.addr:#x} "
                        f"after {retries + 1} attempts")
                continue
            raise TruncatedTransfer(f"write rejected: {result.detail}")
        raise QueueOverflow("unreachable")

    def _read_status(self) -> dict:
        """One status-read window: the F=1 transaction returns the word
        on MISO (in-process: the mock's read_status fetch). A FRAME
        counter regression is unambiguous pad-reset evidence (H03 4.4:
        the counter is reset by the pad only and is monotonic within a
        power session; 16-bit wraparound excluded)."""
        result = self.core.transfer(Frame48(0, 0, 0, is_read=True).encode())
        if result.bits_delivered < 48:
            raise TruncatedTransfer("status read truncated")
        status = parse_status(self.core.read_status())
        frame = status["frame"]
        if self._last_frame is not None and \
                frame < self._last_frame and \
                self._last_frame - frame < 0x8000:
            raise ResetObserved(
                f"FRAME counter regressed {self._last_frame} -> {frame}: "
                "pad reset observed (H03 section 4.4); recall required")
        self._last_frame = frame
        return status

    def poll(self) -> dict:
        """Mid-operation status check; contract error mapping. FRESH=1
        here means the core lost state (pad reset / brown-out)."""
        status = self._read_status()
        if status["overrun"]:
            raise ContractDefect("OVERRUN set: schedule deadline missed; "
                                 "must never set (H03 section 4.3)")
        if status["fresh"]:
            raise ResetObserved(
                f"FRESH=1 at frame {status['frame']}: core state undefined; "
                "recall protocol required (H03 section 4.4)")
        return status

    # -- scheduling ----------------------------------------------------------

    def schedule_burst(self, events, start_frame: int | None = None):
        """Schedule MIDI events (with `sample` positions) into frame
        windows and drive the link. Returns per-frame accounting. A frame
        whose writes cannot fit the window raises BackpressureRefused
        with the excess enumerated - the DEC-014 no-silent-deferral rule."""
        per_frame: dict[int, list] = {}
        for ev in events:
            per_frame.setdefault(frame_of_sample(ev["sample"]), []).append(ev)
        first = min(per_frame) if per_frame else 0
        if start_frame is not None:
            first = start_frame
        write_cap = capacity(self.clock) - STATUS_POLL_WINDOWS
        accounting = []
        for frame_index in sorted(per_frame):
            writes: list[Frame48] = []
            tags = []
            for ev in per_frame[frame_index]:
                frames, tag = self._event_frames(ev)
                writes.extend(frames)
                tags.append(tag)
            used = len(writes) + STATUS_POLL_WINDOWS
            if used > write_cap + STATUS_POLL_WINDOWS:
                excess = writes[write_cap:]
                raise BackpressureRefused(
                    f"frame {frame_index} needs {used} windows > capacity "
                    f"{write_cap + STATUS_POLL_WINDOWS} at {self.clock}; "
                    f"{len(excess)} write(s) would defer silently "
                    f"(DEC-014): {tags}")
            for w in writes:
                self._send(w)
            status = self.poll()
            self.core.flush_frame()
            accounting.append({
                "frame": frame_index, "writes": len(writes), "status": status,
                "applied_frame": self.core.frame_now, "tags": tags,
            })
        return accounting

    def _event_frames(self, ev):
        kind = ev["kind"]
        if kind == "note_on":
            frames, tag = self._note_on_frames(ev["note"], ev["velocity"])
        elif kind == "note_off":
            frames = [Frame48(mc.SEC_EVENTS, mc.ADDR_CMD,
                              (mc.CMD_NOTE_OFF << 16) | ev["note"])]
            tag = f"note_off {ev['note']}"
        elif kind == "controller":
            frames = [Frame48(mc.SEC_EVENTS, mc.ADDR_CTRL,
                              (ev["cc"] << 16) | ev["value"])]
            tag = f"cc {ev['cc']}={ev['value']}"
        elif kind == "bend":
            frames = [Frame48(mc.SEC_EVENTS, mc.ADDR_BEND_PB,
                              ev["value"] & 0xFFFF)]
            tag = f"bend {ev['value']}"
        else:
            raise HostError(f"unknown event kind {kind!r}")
        return frames, tag

    def _note_on_frames(self, midinote: int, velocity: int):
        """Note-on transaction set (H06 section 2, U04 map): 1 velocity +
        6 per-op frozen detune deltas + 1 CMD; the deltas mirror the
        documented-mode shift (transpose applied once, in the core,
        DEC-023) using the committed patch's operator bytes."""
        voice = self.committed_voice
        if voice is None:
            raise HostError("no committed patch; boot the core first")
        shift = voice.transpose - 24  # documented mode (H06 section 2)
        l_prime = midinote_to_logfreq(midinote + shift)
        frames = [Frame48(mc.SEC_EVENTS, mc.ADDR_VELOCITY,
                          velocity & 0x7F)]
        for k, op in enumerate(voice.operators):  # operators[0] = op 1
            frames.append(Frame48(mc.SEC_EVENTS,
                                  mc.ADDR_OP_DDELTA_BASE + k,
                                  detune_delta(l_prime, op.osc_detune)
                                  & 0xFFFFFFFF))
        frames.append(Frame48(mc.SEC_EVENTS, mc.ADDR_CMD,
                              (mc.CMD_NOTE_ON << 16) | midinote))
        return frames, f"note_on {midinote} vel {velocity}"

    # -- patch load / boot / recall ------------------------------------------

    def patch_image_frames(self, voice: sysex.Voice) -> list[Frame48]:
        """Boot-image transactions for one P01 Voice (U04 map, H03
        section 4.1 load cost): 155 shadowed voice bytes (A=0..154) + 6
        shadowed fine terms + PM_DEPTH/PM_SENS/AM_DEPTH + the load
        digest, then 5 direct performance settings (DEC-022) and the
        CMD_COMMIT."""
        if len(voice.raw_bytes) != sysex.VOICE_DATA_LEN:
            raise MalformedPatch(
                f"voice raw_bytes is {len(voice.raw_bytes)} bytes, "
                f"expected the unpacked {sysex.VOICE_DATA_LEN}")
        frames = [Frame48(mc.SEC_PATCH, i, b)
                  for i, b in enumerate(voice.raw_bytes)]
        for k, op in enumerate(voice.operators):
            frames.append(Frame48(mc.SEC_PATCH, mc.PATCH_FTERM_BASE + k,
                                  fine_term(op.osc_freq_fine)))
        frames.append(Frame48(mc.SEC_PATCH, mc.ADDR_PM_DEPTH,
                              (voice.lfo_pitch_mod_depth * 165) >> 6))
        frames.append(Frame48(mc.SEC_PATCH, mc.ADDR_PM_SENS,
                              PITCHMODSENSTAB[
                                  voice.lfo_pitch_mod_sensitivity & 7]))
        frames.append(Frame48(mc.SEC_PATCH, mc.ADDR_AM_DEPTH,
                              (voice.lfo_amp_mod_depth * 165) >> 6))
        digest = sysex.checksum(voice.raw_bytes)
        frames.append(Frame48(mc.SEC_PATCH, mc.ADDR_LOAD_DIGEST, digest))
        # Direct performance settings, declared defaults (H06 section 2:
        # bend centered, master tune 0, ctrl PM/AM 0, EG_MOD 127 pinned
        # Controllers::refresh default; DEC-022 separate state).
        frames.extend([
            Frame48(mc.SEC_EVENTS, mc.ADDR_BEND_PB, 0),
            Frame48(mc.SEC_EVENTS, mc.ADDR_MASTER_TUNE, 0),
            Frame48(mc.SEC_EVENTS, mc.ADDR_CTRL_PM, 0),
            Frame48(mc.SEC_EVENTS, mc.ADDR_CTRL_AM, 0),
            Frame48(mc.SEC_EVENTS, mc.ADDR_EG_MOD, 127),
        ])
        frames.append(Frame48(mc.SEC_EVENTS, mc.ADDR_CMD,
                              mc.CMD_COMMIT << 16))
        return frames

    def load_patch(self, voice: sysex.Voice) -> dict:
        """Full patch load + atomic commit (H03 section 4.1). Raises
        PatchLoadRefused if the core refuses the commit; ResetObserved if
        the core resets mid-load."""
        frames = self.patch_image_frames(voice)
        return self._deliver_image(frames, voice)

    def load_patch_bytes(self, data: bytes, bank_slot: int | None = None):
        """P01 decode then load; P01 error classes propagate as contract
        errors (MalformedPatch with the codec error as __cause__)."""
        try:
            decoded = sysex.decode_message(data)
        except sysex.SysexError as exc:
            raise MalformedPatch(str(exc)) from exc
        if isinstance(decoded, sysex.Bank):
            if bank_slot is None:
                raise MalformedPatch(
                    "bank dump selected; bank_slot 0..31 required")
            if not 0 <= bank_slot < len(decoded.voices):
                raise MalformedPatch(f"bank_slot {bank_slot} outside 0..31")
            voice = decoded.voices[bank_slot]
            voice.raw_bytes = sysex.encode_voice(voice)[6:6 + sysex.VOICE_DATA_LEN]
        else:
            voice = decoded
        return self.load_patch(voice)

    def _deliver_image(self, frames: list[Frame48], voice) -> dict:
        write_cap = capacity(self.clock) - STATUS_POLL_WINDOWS
        sent = 0
        frames_used = 0
        while sent < len(frames):
            batch = frames[sent:sent + write_cap]
            for frame in batch:
                self._send(frame)
            sent += len(batch)
            self.poll()
            self.core.flush_frame()
            frames_used += 1
        report = self._verify_commit(voice)
        report["writes"] = len(frames)
        report["frames"] = frames_used
        report["image_sha256"] = hashlib.sha256(
            b"".join(f.encode() for f in frames)).hexdigest()
        return report

    def _verify_commit(self, voice) -> dict:
        """Contract-shaped acceptance check: the section 4.6 applied-event
        tap must show the commit; a refusal or silence fails loudly."""
        applied = self.core.applied_events()
        commits = [e for e in applied if e.kind == "commit"]
        refusals = [e for e in applied if e.kind == "commit_refused"]
        if not commits or (refusals and refusals[-1].frame >
                           (commits[-1].frame if commits else 0)):
            raise PatchLoadRefused(
                "core refused the patch commit (truncated/bad-checksum "
                "load); committed image unchanged - stale state detected")
        gen = self.core.generation
        self.committed_voice = voice
        self.committed_generation = gen
        self._good_image = self.patch_image_frames(voice)
        return {"generation": gen,
                "digest": self.core.committed_digest(gen)}

    def boot(self, voice: sysex.Voice | None = None, *,
             recall: bool = False) -> dict:
        """Reset -> fresh -> image (H03 section 4.4). With recall=True the
        last successfully committed image is re-delivered byte-identically
        (brown-out protocol; the recall segment is hash-comparable)."""
        if recall:
            if self._good_image is None:
                raise HostError("nothing committed to recall")
            frames = list(self._good_image)
            voice = self.committed_voice
        else:
            if voice is None:
                raise HostError("boot requires a Voice")
            frames = self.patch_image_frames(voice)
        deadline = 4  # frames to observe FRESH=1 after reset
        self._last_frame = None  # the pad reset is expected here
        if recall:
            # Recall is armed by an already-detected reset (FRESH=1 or a
            # FRAME regression - both pad-reset-only evidence, H03 4.4);
            # the fresh window may have been consumed by the aborted
            # load's own accepted writes, so it is not re-required.
            report = self._deliver_image(frames, voice)
            report["recall"] = True
            report["writes"] = len(frames)
            return report
        for _ in range(deadline):
            status = self._read_status()
            if status["fresh"]:
                break
        else:
            raise HostError("FRESH never observed; reset the core first "
                            "(boot protocol: reset -> fresh -> image)")
        report = self._deliver_image(frames, voice)
        report["recall"] = recall
        report["writes"] = len(frames)
        return report

    def recall(self) -> dict:
        """Re-deliver the last good image after an observed reset."""
        return self.boot(recall=True)

    # -- accounting ----------------------------------------------------------

    def log_digest(self) -> str:
        return hashlib.sha256(
            b"".join(f.encode() for f in self.log)).hexdigest()

    def budget_report(self) -> dict:
        """Link-budget accounting (H03 section 2 rows 9-15; issue #37
        acceptance 'write-cost accounting'), including the parasynth
        family anchor (182 writes ~ 4.43 ms at 2 MHz SPI; quoted as a
        family anchor, never transferred as a budget)."""
        burst = WORST_BURST_WRITES
        cap_design = capacity(DESIGN_POINT)
        cap_fallback = capacity(FALLBACK)
        return {
            "note_on_writes": NOTE_ON_WRITES,
            "note_off_writes": NOTE_OFF_WRITES,
            "controller_writes": CONTROLLER_WRITES,
            "worst_burst_writes": burst,
            "capacity_24m576": cap_design,
            "capacity_12m288": cap_fallback,
            "burst_fits_24m576": burst + STATUS_POLL_WINDOWS <= cap_design,
            "burst_fits_12m288": burst + STATUS_POLL_WINDOWS <= cap_fallback,
            "boot_image_writes": BOOT_IMAGE_WRITES,
            "load_cost_bound": LOAD_COST_BOUND,
            "boot_frames_24m576": -(-BOOT_IMAGE_WRITES //
                                    (cap_design - STATUS_POLL_WINDOWS)),
            "boot_frames_12m288": -(-BOOT_IMAGE_WRITES //
                                    (cap_fallback - STATUS_POLL_WINDOWS)),
            "boot_seconds_24m576": link_seconds(BOOT_IMAGE_WRITES,
                                                DESIGN_POINT),
            "boot_seconds_12m288": link_seconds(BOOT_IMAGE_WRITES, FALLBACK),
            "burst_seconds_24m576": link_seconds(burst, DESIGN_POINT),
            "burst_seconds_12m288": link_seconds(burst, FALLBACK),
            "parasynth_anchor": {
                "writes": 182, "seconds_at_2mhz": 4.43e-3,
                "role": "family anchor (gf180-parasynth boot image, "
                        "spi_host.py datapoint); not a budget for this "
                        "design",
            },
        }
