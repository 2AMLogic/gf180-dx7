"""Mock core: a contract-faithful in-process model of the H03 core
interface (issue #37, U04; contract docs/CONTRACT-CORE-v1.md + 
spec/core-contract-v1.json).

Implements, over an in-process transport shaped like the SPI link
(48-bit frames, frame-serializable):

  * patch load/commit with atomicity (H03 section 4.1): SEC=1 writes
    drain continuously at 1 write/clk into a shadow image; COMMIT swaps
    the shadow into a 4-entry committed-patch ring at a 64-sample frame
    boundary; a load digest (P01 checksum over the 155 voice bytes) is
    verified at commit and a refused commit is recorded, never silent;
  * events at block boundaries (H03 section 4.2): an event received
    during frame N is applied before the first sample of frame N+1;
  * bounded skid FIFO (depth 8 x 49 bits) with explicit-rejection
    overflow policy (DEC-014): a dropped write sets the sticky OVERFLOW
    status flag and is recorded in the fault log - silent drops and
    silent deferral are forbidden;  * status reads (H03 section 4.3): 32-bit word {MAGIC 0xD7, VERSION
    0x1, OVERRUN, QUEUE, OVERFLOW, FRESH, FRAME[15:0]}; the first read
    after reset reads the 0xD711_0001-shaped word; sticky flags clear
    on read; FRESH = no write accepted since reset;
  * reset/boot protocol (H03 section 4.4): pad reset defines all state
    (frame counter and link reset by the pad only); SOFT_RESET clears
    voice/env/phase state and the event shadow without touching the
    link, the queue, or the frame counter; boot = reset -> fresh -> image;
  * generation ring with the declared DEC-019 held-note bound: a held
    note keeps its starting generation for up to 3 subsequent commits;
    a 4th commit while held evicts the binding and records metadata
    (declared conformance gap, surfaced per DEC-017, never silent).

FAILURE INJECTION (issue #37 acceptance): truncated transfer, bad
checksum, queue overflow, backpressure (drain stall / frozen queue),
overrun, and reset mid-transfer. Every injection leaves a record in the
fault log; nothing fails silently.

OBSERVABILITY (H03 section 4.6 style, read-only; U04 + U05): the
applied-event tap (applied_events), the committed-image accessors
(committed_image/committed_digest over the 4-entry ring), the fault log
(core.faults), the status word (read_status), and (U05, issue #38) the
drain tap (drain_tap) - the frame each accepted write left the skid FIFO,
used by the tools/demo fixture bench for the host prediction / CS-N pin /
drain frame three-way cross-check. Taps never affect behavior.

Timing model: each accepted transaction window costs 194 core clocks
(48-bit frame + 2-cycle CS gap at SCK = f_core/4; H03 section 2 row 15
unit basis), the skid FIFO drains at 1 write/clk, and frame boundaries
fall every clks_per_frame clocks (32,768 at the 24.576 MHz design
point, 16,384 at the 12.288 MHz fallback). Read transactions (F=1)
return the status word inside the same 194-clock window (declared by
U04; the contract fixes the write-transaction period only).

Claim boundary: this is a LINK-CONTRACT mock, not the FM engine. It
records applied events, generations and faults; it does not synthesize
audio. Qualifying the host against this mock establishes host-side
transaction conformance only - no RTL, board, fidelity, or audibility
claim is made or implied.
"""

from __future__ import annotations

import dataclasses
from dataclasses import dataclass, field

# H03 section 4.2/event_interface: transport shape and queue constants.
SEC_EVENTS = 0
SEC_PATCH = 1

CMD_NOTE_ON = 1
CMD_NOTE_OFF = 2
CMD_COMMIT = 3

ADDR_CMD = 0x00
ADDR_VELOCITY = 0x01
ADDR_CTRL = 0x02
ADDR_SOFT_RESET = 0x03
ADDR_BEND_PB = 0x28
ADDR_MASTER_TUNE = 0x29
ADDR_CTRL_PM = 0x2A
ADDR_CTRL_AM = 0x2B
ADDR_EG_MOD = 0x2C
ADDR_OP_DDELTA_BASE = 0x30  # 0x30..0x35, six per-op detune deltas

PATCH_VOICE_BYTES = 155     # P01 unpacked voice data bytes, A = 0..154
PATCH_FTERM_BASE = 155      # A = 155..160, six host-frozen fine terms
ADDR_PM_DEPTH = 161
ADDR_PM_SENS = 162
ADDR_AM_DEPTH = 163
ADDR_LOAD_DIGEST = 0xFF     # P01 checksum over the 155 voice bytes

TXN_PERIOD_CLKS = 194       # 48-bit frame + 2-cycle CS gap at SCK = f_core/4
SKID_DEPTH = 8              # 8 entries x 49 bits (48-bit frame + valid)
FRAME_SAMPLES = 64
FRAMES_PER_SECOND = 750

STATUS_MAGIC = 0xD7
STATUS_VERSION = 0x1
STATUS_FIRST_READ = 0xD7110001  # {MAGIC, VERSION, FRESH=1, FRAME=1}

CLOCKS = {
    # name -> (hz, clks_per_frame); spec/core-contract-v1.json clock_targets
    "clk_24m576": (24_576_000, 32_768),
    "clk_12m288": (12_288_000, 16_384),
}


class MockCoreError(Exception):
    """Base class for mock-core contract errors."""


@dataclass(frozen=True)
class Frame48:
    """One 48-bit link frame {F, 6'b0, SEC, A[7:0], D[31:0]} (H03 4.2)."""

    sec: int
    addr: int
    data: int
    is_read: bool = False

    def __post_init__(self) -> None:
        for name, value, bits in (("sec", self.sec, 1), ("addr", self.addr, 8),
                                  ("data", self.data, 32)):
            if not 0 <= value < (1 << bits):
                raise MockCoreError(
                    f"frame field {name}={value} outside {bits} bits")

    def encode(self) -> bytes:
        """Serialize to the 6-byte (48-bit) wire form, MSB first."""
        word = ((1 if self.is_read else 0) << 47) | (self.sec << 40) \
            | (self.addr << 32) | self.data
        return word.to_bytes(6, "big")

    @staticmethod
    def decode(raw: bytes) -> "Frame48":
        if len(raw) != 6:
            raise MockCoreError(
                f"frame must be exactly 48 bits (6 bytes), got {len(raw)}")
        word = int.from_bytes(raw, "big")
        return Frame48(sec=(word >> 40) & 1, addr=(word >> 32) & 0xFF,
                       data=word & 0xFFFFFFFF, is_read=bool(word >> 47))


@dataclass
class TransactionResult:
    """Outcome of one transaction window as the transport delivered it."""

    accepted: bool
    bits_delivered: int
    detail: str = ""


@dataclass
class FaultRecord:
    """One recorded failure; the mock never fails silently."""

    kind: str
    detail: str
    frame: int
    txn_index: int


@dataclass
class EventRecord:
    """One applied event / commit in the block-level tap (H03 4.6)."""

    frame: int
    kind: str
    detail: str


@dataclass(frozen=True)
class DrainRecord:
    """One accepted write drained from the skid FIFO into the core (U05,
    issue #38). Read-only H03 section 4.6-style observability: the frame in
    which the write left the FIFO and entered shadow/event state. The FIFO
    never reorders, and every accepted write drains exactly once (explicitly
    rejected writes are refused before queueing and recorded as faults), so
    the drain tap correlates one-for-one, in order, with every accepted
    write - the drain side of the host prediction / CS-N pin / drain frame
    three-way cross-check.
    """

    frame: int
    sec: int
    addr: int
    data: int
    seq: int


@dataclass
class FailureInjection:
    """Failure-injection hooks (issue #37); every use records a fault."""

    truncate_next_bits: int | None = None
    corrupt_checksum: bool = False
    corrupt_byte: tuple[int, int] | None = None       # (addr, wrong value)
    stall_drain_clks: int = 0                          # one-shot backpressure
    freeze_queue: bool = False                         # drop all arrivals
    overrun_once: bool = False
    reset_after_writes: int | None = None              # one-shot pad reset


@dataclass
class _HeldBinding:
    generation: int
    commits_since: int = 0


class MockCore:
    """Contract-faithful mock of the H03 core link interface."""

    def __init__(self, clock: str = "clk_24m576") -> None:
        if clock not in CLOCKS:
            raise MockCoreError(
                f"unknown clock {clock!r}; expected one of {sorted(CLOCKS)}")
        self.clock_name = clock
        self.clock_hz, self.clks_per_frame = CLOCKS[clock]
        self.injection = FailureInjection()
        self.txn_index = 0
        self._reset_state()

    # -- reset paths (H03 section 4.4) ------------------------------------

    def _reset_state(self, keep_faults: bool = False) -> None:
        prior_faults = self.faults if keep_faults else []
        self.clk = 0
        self.queue: list[Frame48] = []  # bounded skid FIFO
        self.stall_until = -1
        self.shadow: dict[int, int] = {}
        self.direct: dict[int, int] = {}
        self.digest: int | None = None
        self.pending_events: list[tuple[int, Frame48]] = []
        self.ring: list[dict[int, int]] = [{}]   # generation 0: reset default
        self.generation = 0
        self.held: dict[int, _HeldBinding] = {}
        self.fresh = True
        self.sticky_overrun = False
        self.sticky_overflow = False
        self.accepted_since_reset = 0
        self.applied: list[EventRecord] = []
        self.drains: list[DrainRecord] = []
        self.faults = prior_faults  # evidence survives a reset
        self._pending_velocity: int | None = None
        self._pending_ddelta: dict[int, int] = {}

    def pad_reset(self) -> None:
        """RST_N pad: everything defined again; link/queue/frame reset.
        The fault log is observability, not core state: it survives."""
        self._reset_state(keep_faults=True)

    def soft_reset(self) -> None:
        """SOFT_RESET register write: voice/env/phase + event shadow only;
        never touches the link, the queue, or the frame counter."""
        self.shadow = {}
        self.digest = None
        self.pending_events = []
        self.ring = [{}]
        self.generation = 0
        self.held = {}
        self._pending_velocity = None
        self._pending_ddelta = {}
        self.applied.append(EventRecord(self.frame_count, "soft_reset", ""))

    # -- time ---------------------------------------------------------------

    @property
    def frame_now(self) -> int:
        return self.clk // self.clks_per_frame + 1

    @property
    def frame_count(self) -> int:
        """The 16-bit FRAME counter of the status word (free-running,
        pad-reset only)."""
        return self.frame_now

    def tick(self, clks: int) -> None:
        """Advance the core clock: continuous 1 write/clk drain; apply the
        frame-boundary commit and pending events at each boundary."""
        target = self.clk + clks
        while self.clk < target:
            boundary = self.frame_count * self.clks_per_frame
            step = min(target - self.clk, boundary - self.clk)
            if self.clk >= self.stall_until:
                unstalled = step
            elif self.clk + step <= self.stall_until:
                unstalled = 0
            else:
                unstalled = step - (self.stall_until - self.clk)
            for _ in range(unstalled):
                if not self.queue:
                    break
                self._drain_one()
            self.clk += step
            if self.clk >= boundary:
                self._frame_boundary()

    def flush_frame(self) -> None:
        """Advance to the next frame boundary (idle link clocks)."""
        self.tick(self.frame_count * self.clks_per_frame - self.clk)

    def _drain_one(self) -> None:
        frame = self.queue.pop(0)
        if not frame.is_read:  # writes only; status reads drain as no-ops
            self.drains.append(DrainRecord(
                self.frame_now, frame.sec, frame.addr, frame.data,
                len(self.drains)))
        self._apply_write(frame)

    def _record_fault(self, kind: str, detail: str) -> None:
        self.faults.append(FaultRecord(kind, detail, self.frame_now,
                                       self.txn_index))

    # -- transport ----------------------------------------------------------

    def transfer(self, raw: bytes) -> TransactionResult:
        """One transaction window (194 core clocks). Returns what the
        transport delivered; a truncated frame is rejected and recorded."""
        self.txn_index += 1
        if self.injection.truncate_next_bits is not None:
            bits = self.injection.truncate_next_bits
            self.injection.truncate_next_bits = None
            self._record_fault("truncated_transfer",
                               f"{bits} of 48 bits delivered")
            self.tick(TXN_PERIOD_CLKS)
            return TransactionResult(False, bits, "truncated transfer")
        try:
            frame = Frame48.decode(raw)
        except MockCoreError as exc:
            self._record_fault("malformed_frame", str(exc))
            self.tick(TXN_PERIOD_CLKS)
            return TransactionResult(False, len(raw) * 8, str(exc))
        if frame.is_read:
            self.tick(TXN_PERIOD_CLKS)
            return TransactionResult(True, 48, "status read")
        if self.injection.freeze_queue or len(self.queue) >= SKID_DEPTH:
            # DEC-014 explicit rejection: sticky OVERFLOW, recorded, never
            # a silent drop. Unreachable at the declared SCK bound; the
            # flag is a defect detector and here a directed injection.
            self.sticky_overflow = True
            self._record_fault(
                "queue_overflow",
                f"write {frame.sec:#x}/{frame.addr:#x} dropped, "
                f"depth {len(self.queue)} >= {SKID_DEPTH}")
            self.tick(TXN_PERIOD_CLKS)
            return TransactionResult(False, 48, "queue overflow")
        if self.injection.stall_drain_clks:
            # One-shot backpressure stall: the shadow drain stops for the
            # declared window (an adversarial/defect condition; at the
            # declared SCK bound the drain outpaces arrival 194x).
            self.stall_until = self.clk + self.injection.stall_drain_clks
            self.injection.stall_drain_clks = 0
            self._record_fault("drain_stall",
                               f"drain stalled {self.stall_until - self.clk}"
                               " clks")
        self.queue.append(frame)
        self.fresh = False
        self.accepted_since_reset += 1
        if self.injection.reset_after_writes is not None and \
                self.accepted_since_reset >= self.injection.reset_after_writes:
            self.injection.reset_after_writes = None
            self._record_fault("reset_mid_transfer",
                               f"pad reset after {self.accepted_since_reset} "
                               "accepted writes")
            self.pad_reset()
            return TransactionResult(True, 48, "reset mid-transfer")
        self.tick(TXN_PERIOD_CLKS)
        return TransactionResult(True, 48, "accepted")

    def read_status(self) -> int:
        """The 32-bit MISO status word (H03 section 4.3); sticky flags
        clear on read."""
        overrun = self.sticky_overrun
        overflow = self.sticky_overflow
        self.sticky_overrun = False
        self.sticky_overflow = False
        word = (STATUS_MAGIC << 24) | (STATUS_VERSION << 20) \
            | (overrun << 19) | ((1 if self.queue else 0) << 18) \
            | (overflow << 17) | ((1 if self.fresh else 0) << 16) \
            | (self.frame_count & 0xFFFF)
        if self.injection.overrun_once:
            self.injection.overrun_once = False
            self._record_fault("overrun_injected", "directed OVERRUN stick")
            word |= 1 << 19
        return word

    # -- write application --------------------------------------------------

    def _apply_write(self, frame: Frame48) -> None:
        if self.injection.corrupt_byte is not None and \
                frame.sec == SEC_PATCH and \
                frame.addr == self.injection.corrupt_byte[0]:
            addr, wrong = self.injection.corrupt_byte
            self.injection.corrupt_byte = None
            self._record_fault("corrupt_byte",
                               f"patch byte at {addr:#x} corrupted in flight")
            frame = Frame48(sec=frame.sec, addr=addr, data=wrong)
        if frame.sec == SEC_PATCH:
            if frame.addr == ADDR_LOAD_DIGEST:
                self.digest = frame.data & 0x7F
            else:
                self.shadow[frame.addr] = frame.data & 0xFFFFFFFF
            return
        if frame.addr == ADDR_SOFT_RESET:
            self.soft_reset()
            return
        if frame.addr in (ADDR_CMD, ADDR_VELOCITY, ADDR_CTRL) or \
                ADDR_OP_DDELTA_BASE <= frame.addr <= ADDR_OP_DDELTA_BASE + 5:
            self.pending_events.append((self.frame_now, frame))
            return
        self.direct[frame.addr] = frame.data & 0xFFFFFFFF

    def _frame_boundary(self) -> None:
        """Atomic frame-boundary application (H03 4.1/4.2): a pending
        commit applies first (H06 section 2: commit, then the note event),
        then the pending events in arrival order, all before the first
        sample of frame N+1."""
        if self.queue:
            # A write still undrained at the boundary would apply one frame
            # late - a silent deferral, forbidden by DEC-014. Unreachable
            # at the declared SCK bound; a defect detector like OVERFLOW.
            self._record_fault(
                "deferred_write",
                f"{len(self.queue)} write(s) undrained at the boundary")
        for _, frame in self.pending_events:
            if frame.addr == ADDR_CMD and \
                    (frame.data >> 16) & 0xFFFF == CMD_COMMIT:
                self._commit()
                break
        for arrival_frame, frame in self.pending_events:
            if frame.addr == ADDR_CMD:
                cmd = (frame.data >> 16) & 0xFFFF
                note = frame.data & 0xFF
                if cmd == CMD_NOTE_ON:
                    self._apply_note_on(arrival_frame, note)
                elif cmd == CMD_NOTE_OFF:
                    self.held.pop(note, None)
                    self.applied.append(EventRecord(
                        self.frame_count, "note_off", f"note {note}"))
                elif cmd == CMD_COMMIT:
                    continue
                else:
                    self._record_fault("unknown_cmd", f"cmd {cmd}")
            elif frame.addr == ADDR_VELOCITY:
                self._pending_velocity = frame.data & 0x7F
            elif frame.addr == ADDR_CTRL:
                cc = (frame.data >> 16) & 0xFF
                value = frame.data & 0xFF
                self.applied.append(EventRecord(
                    self.frame_count, "controller", f"cc {cc} = {value}"))
            elif ADDR_OP_DDELTA_BASE <= frame.addr <= ADDR_OP_DDELTA_BASE + 5:
                op = frame.addr - ADDR_OP_DDELTA_BASE
                self._pending_ddelta[op] = frame.data & 0xFFFFFFFF
        self.pending_events = []

    def _apply_note_on(self, arrival_frame: int, note: int) -> None:
        velocity = self._pending_velocity
        self._pending_velocity = None
        deltas = self._pending_ddelta
        self._pending_ddelta = {}
        self.held[note] = _HeldBinding(self.generation)
        self.applied.append(EventRecord(
            self.frame_count, "note_on",
            f"note {note} vel {velocity} gen {self.generation} "
            f"deltas {sorted(deltas.items())}"))

    def _commit(self) -> None:
        """Atomic shadow -> ring swap (H03 section 4.1), digest-verified."""
        voice_bytes = bytes(self.shadow.get(i, 0)
                            for i in range(PATCH_VOICE_BYTES))
        computed = (0x80 - (sum(voice_bytes) & 0x7F)) & 0x7F
        if self.digest is None or computed != self.digest:
            self._record_fault(
                "commit_refused",
                f"load digest {self.digest} != computed {computed} "
                "(truncated/bad-checksum load); committed image unchanged")
            self.applied.append(EventRecord(self.frame_count,
                                            "commit_refused", ""))
            return
        if self.injection.corrupt_checksum:
            self.injection.corrupt_checksum = False
            self._record_fault(
                "commit_refused",
                "directed bad-checksum injection; committed image unchanged")
            self.applied.append(EventRecord(self.frame_count,
                                            "commit_refused", ""))
            return
        image = dict(self.shadow)
        self.ring.append(image)
        if len(self.ring) > 4:
            self.ring.pop(0)
        self.generation += 1
        self.shadow = {}
        self.digest = None
        for note, binding in list(self.held.items()):
            binding.commits_since += 1
            if binding.commits_since > 3:
                # Declared DEC-019 conformance gap (4-entry ring): a 4th
                # commit while held evicts the generation. Surfaced here,
                # never silent (DEC-017).
                self.applied.append(EventRecord(
                    self.frame_count, "generation_evicted",
                    f"note {note} held across >3 commits"))
                del self.held[note]
        self.applied.append(EventRecord(self.frame_count, "commit",
                                        f"generation {self.generation}"))

    # -- observability taps (H03 section 4.6; read-only) --------------------

    def applied_events(self) -> tuple[EventRecord, ...]:
        return tuple(self.applied)

    def drain_tap(self) -> tuple[DrainRecord, ...]:
        """Drain tap (U05, issue #38; read-only evidence, H03 4.6 style).

        One record per accepted write, in FIFO order: the frame in which the
        write drained into the core. Survives pad resets (evidence, like the
        fault log and ``applied``). One append per drained write - no effect
        on behavior.
        """
        return tuple(self.drains)

    def committed_image(self, generation: int | None = None) -> dict[int, int]:
        gen = self.generation if generation is None else generation
        return dict(self.ring[gen])

    def committed_digest(self, generation: int | None = None) -> int:
        image = self.committed_image(generation)
        voice = bytes(image.get(i, 0) for i in range(PATCH_VOICE_BYTES))
        return (0x80 - (sum(voice) & 0x7F)) & 0x7F

    def queue_depth_now(self) -> int:
        return len(self.queue)
