# U04 — Embedded host against a mock core (issue #37)

Issue: #37 (epic #3) · Date: 2026-09-21 · Planning ID: U04
Depends on: H03 (issue #25, contract), P01 (issue #8, codec) · Host:
`src/gf180_dx7/host.py` · Mock core: `src/gf180_dx7/mock_core.py` ·
Tests: `tests/test_host_mock.py` (32 tests)

**Claim advanced:** one embedded host is validated against a mock core
that implements the H03 interface contract — the MIDI/library-to-core
transaction path passes event timing, error handling, reset/recall and
partial-transfer fixtures **without requiring a board**.

**Claim boundary:** this qualifies the HOST SIDE against an in-process
mock. The mock is a link-contract model, not the FM engine and not RTL:
no board bring-up (H08/H09), no RTL claim (H04–H07), no synthesis or
timing claim, no original-DX7 fidelity claim, no audibility claim
(AGENTS.md; the three claims stay separate). Agreement numbers below are
host-vs-mock contract conformance only.

---

## 1. What was built

### 1.1 Mock core (`src/gf180_dx7/mock_core.py`)

A contract-faithful model of the H03 core link over an in-process,
frame-serializable transport (48-bit frames `{F, 6'b0, SEC, A[7:0],
D[31:0]}`, H03 §4.2):

- **Patch load/commit with atomicity (§4.1):** SEC=1 writes drain
  continuously at 1 write/clk into a shadow image; a load digest (P01
  checksum over the 155 voice bytes) is verified at the commit; COMMIT
  swaps the shadow into a **4-entry committed-patch ring** at a
  64-sample frame boundary; a bare COMMIT (no loaded image) and a
  digest mismatch are **refused and recorded** — the committed image is
  never partially replaced.
- **Events at block boundaries (§4.2):** an event received during frame
  N is applied before the first sample of frame N+1; a pending commit
  applies before the same boundary's note events (H06 §2 ordering).
- **Queue and overflow (§4.2/DEC-014):** bounded skid FIFO, depth 8 ×
  49 bits; a write arriving at depth 8 is **explicitly rejected** —
  sticky `OVERFLOW` status flag plus a fault record. Silent drops and
  silent deferral are forbidden: a write still undrained at a frame
  boundary records a `deferred_write` defect (unreachable at the
  declared SCK bound; defect detector, like OVERRUN).
- **Status word (§4.3):** `{MAGIC 0xD7, VERSION 0x1, OVERRUN, QUEUE,
  OVERFLOW, FRESH, FRAME[15:0]}`; the first read after reset reads the
  `0xD711_0001`-shaped word; sticky flags clear on read; FRESH = no
  write accepted since reset.
- **Reset/boot (§4.4):** pad reset redefines all state (frame counter
  and link reset by the pad only; the fault log survives as
  observability); SOFT_RESET clears voice/env/phase state and the event
  shadow without touching the link, the queue, or the frame counter.
- **Generation ring (DEC-019/DEC-021):** each note-on binds the
  generation live at its application boundary; a held note keeps its
  generation across commits; a 4th commit while held records
  `generation_evicted` metadata (the declared DEC-019 conformance gap,
  surfaced per DEC-017, never silent).
- **Timing:** each transaction window costs 194 core clocks (48-bit
  frame + 2-cycle CS gap at SCK = f_core/4; §2 row 15 unit basis);
  frame boundaries every 32,768 clocks (24.576 MHz design point) or
  16,384 (12.288 MHz fallback).
- **Failure injection (issue #37 acceptance):** truncated transfer,
  corrupt byte in flight, directed bad checksum, queue overflow
  (freeze), drain stall (backpressure), directed OVERRUN, and pad reset
  after N accepted writes. Every injection leaves a fault record;
  nothing fails silently.

### 1.2 Embedded host (`src/gf180_dx7/host.py`)

The SPI-master host: MIDI-event → core-transaction scheduling per H03,
patch loads from P01 Voice/bank bytes, link-budget accounting, error
handling, and the reset/recall protocol.

- **Scheduling:** events carry 48 kHz sample positions; the scheduler
  maps them to frames (`sample // 64`), packs each frame's writes plus
  one status poll into the frame window (each window 194 clocks), and
  **refuses an over-capacity frame with an explicit, enumerated
  `BackpressureRefused`** — the DEC-014 no-silent-deferral rule.
- **Error handling:** truncated transfer → `TruncatedTransfer` (abort,
  no COMMIT); overflow sticky → read, clear, **retry** (DEC-014), then
  `QueueOverflow`; OVERRUN → `ContractDefect` (must never set);
  FRESH=1 or a FRAME-counter regression → `ResetObserved` (FRAME is
  pad-reset-only per §4.4, so regression is unambiguous reset evidence
  even when the fresh window was consumed by the aborted load's own
  writes); commit refused → `PatchLoadRefused`; P01 codec errors →
  `MalformedPatch` with the P01 error as `__cause__`, **nothing placed
  on the link**.
- **Reset/recall:** boot = reset → fresh=1 → image (§4.4); recall
  re-delivers the last successfully committed image **byte-identically**
  (hash-comparable transaction log), armed by FRESH=1 or a FRAME
  regression.
- **Frozen integers:** per-op detune deltas, fine terms, PM/AM depths
  mirror the pinned formulas of `src/gf180_dx7/model/pitch.py` (N05;
  H06 §2 documents this host/core split). The patch's own transpose is
  applied once, in the core (documented mode, DEC-023).

## 2. Transaction map (U04, adapted from the H06 §2 register conventions)

Frame: `{F, 6'b0, SEC, A[7:0], D[31:0]}`. F=0 register write, F=1
status read (word on MISO; U04 accounts reads at the same 194-clock
window — declared; the contract fixes the write-transaction period).

| SEC | A | Register | D[31:0] | Class | Writes |
|---|---|---|---|---|---:|
| 0 | 0x00 | CMD | `{note[15:0], cmd[7:0], 8'0}` — 1 note-on, 2 note-off, 3 commit | event | 1 |
| 0 | 0x01 | VELOCITY | `{velocity[7:0], 24'0}` | note-on payload | 1 |
| 0 | 0x02 | CTRL | `{cc[7:0], value[7:0], 16'0}` (sustain CC64, mod CC1, volume CC7, …) | event | 1 |
| 0 | 0x03 | SOFT_RESET | any write | event | 1 |
| 0 | 0x28–0x2C | BEND_PB / MASTER_TUNE / CTRL_PM / CTRL_AM / EG_MOD | frozen int32 (defaults 0,0,0,0,127 — pinned `Controllers::refresh`) | perf, direct (DEC-022) | 5/boot |
| 0 | 0x30–0x35 | OP_DDELTA | per-op frozen int32 detune delta (dx7note.cc:46-47 float gate, host-side) | note-on payload | 6 |
| 1 | 0x00–0x9A | VOICE_BYTES | the 155 unpacked P01 voice data bytes | patch, shadowed | 155 |
| 1 | 0x9B–0xA0 | OP_FTERM | six frozen fine terms (dx7note.cc:50-53) | patch, shadowed | 6 |
| 1 | 0xA1–0xA3 | PM_DEPTH / PM_SENS / AM_DEPTH | `(byte·165)>>6` / `PITCHMODSENSTAB[byte&7]` / `(byte·165)>>6` | patch, shadowed | 3 |
| 1 | 0xFF | LOAD_DIGEST | P01 checksum over the 155 voice bytes | patch, shadowed | 1 |

Per-event write costs: **note-on 8** (1 velocity + 6 deltas + 1 CMD),
**note-off 1**, **controller 1**, **bend 1**. Boot image = 155 + 6 + 3 +
1 digest (shadowed) + 5 perf (direct) + 1 commit = **171 writes**.

## 3. Link-budget accounting (issue #37 acceptance)

Contract basis: H03 §2 row 15 (SCK ≤ f_core/4; 48-bit frame + 2-cycle CS
gap = 194 core clk/transaction; capacity floor(32,768/194) = **168** at
the 24.576 MHz design point, floor(16,384/194) = **84** at 12.288 MHz)
and H03 §4.1 (full load ≤ 216 writes, absorbed across ≤ 2 frame windows
at the design point). Core-side application costs (H03 §2 row 9: note-on
application 224 clk × 16 in the worst burst; rows 10–12) are the core's
frame budget, not link traffic — the link carries the writes below.

| Measurement | Value | Verdict |
|---|---|---|
| Worst admissible burst (16 on + 16 off + 16 ctrl) | 160 writes + 1 status poll = 161 windows | ≤ 168 @ 24.576 MHz — **fits one frame** |
| Same burst at the 12.288 MHz fallback | 161 > 84 | **explicitly refused** (`BackpressureRefused`, DEC-014; consistent with the H03 headline FAIL) |
| Full boot image | 171 writes ≤ 216 declared | **PASS** (H03 §4.1) |
| Boot absorption | 2 frames @ 24.576 MHz (168−1 poll = 167 writes/frame) | **≤ 2 frames declared — PASS** |
| Boot absorption @ 12.288 MHz | 83 writes/frame → 3 frames | **recorded finding** — the 2-frame absorption claim holds at the design point only; the fallback already fails the H03 frame budget, so nothing is weakened |
| Boot link time @ 24.576 MHz | 171 × 194 = 33,174 clk = **1.350 ms** | measured arithmetic |
| Burst link time @ 24.576 MHz | 160 × 194 = 31,040 clk = **1.263 ms** | inside one 1.333 ms frame |
| Boot link time @ 12.288 MHz | 2.700 ms (burst 2.526 ms) | measured arithmetic |
| Per-write window | 194 / 24.576 MHz = **7.894 µs** | vs parasynth 24.34 µs/write (below) |

**Family anchor (issue #37; not a budget transfer):** the sibling
gf180-parasynth boot image measures **182 writes ≈ 4.43 ms at 2 MHz
SPI** (~24.3 µs/write; `fpga/spi_host.py` per-event write-cost
accounting datapoint). Our contracted link runs SCK = f_core/4 =
6.144 MHz, so the same-scale boot image costs **171 writes ≈ 1.350 ms**
— same order of magnitude, faster per write. As in H03: the sibling
number calibrates the family's boot-image scale (~10² writes,
single-digit milliseconds); **no sibling number is transferred as a
budget**.

Accounting is **measured, not just declared**: the mock's transaction
counter reflects exactly the modeled windows (boot = 171 writes + 1
fresh-wait read + 2 per-frame polls; burst = 160 + 1;
`test_measured_traffic_matches_the_budget_model`).

## 4. Failure-injection matrix and results

| # | Injection | Mechanism | Host-visible behavior | Result |
|---|---|---|---|---|
| 1 | Truncated transfer | next transaction delivers 24/48 bits | `TruncatedTransfer`; load aborted; **no COMMIT sent**; committed image unchanged; `truncated_transfer` fault recorded | MET |
| 2 | Bad checksum (byte corrupted in flight) | `corrupt_byte` flips patch byte 10 | load digest mismatch at commit → `commit_refused`; host raises `PatchLoadRefused`; clean retry commits (generation 2) | MET |
| 3 | Bad checksum (directed) | `corrupt_checksum` forces digest mismatch | same as #2 | MET |
| 4 | Bare COMMIT, no loaded image | COMMIT with empty shadow | digest gate refuses; committed image untouched; fault recorded | MET (extra control) |
| 5 | Queue overflow | `freeze_queue`: every arrival dropped | sticky OVERFLOW observed in status; host reads, clears, retries twice, then `QueueOverflow`; drop recorded in the fault log; re-send applies — **no silent loss** | MET |
| 6 | Backpressure drain stall | drain frozen 4,000 clks mid-burst | FIFO fills to contracted depth 8; 9th write explicitly rejected; after the stall every intended event applies (12/12, in order) | MET |
| 7 | OVERRUN (defect detector) | `overrun_once` | host raises `ContractDefect` on the poll; sticky clears on read (must never set in real operation) | MET |
| 8 | Reset mid-transfer | pad reset after 200 accepted writes | FRAME counter regression 3→1 detected (pad-reset-only evidence, H03 §4.4) → `ResetObserved`; interrupted image commits nothing (generation 0); **recall re-delivers the last good image byte-identically** (transaction-log hash equal) | MET |
| 9 | Event overload at the fallback clock | 160-write burst scheduled at 12.288 MHz | `BackpressureRefused` with the excess enumerated **before anything is sent** — never a silent one-frame deferral (DEC-014) | MET |

## 5. Negative controls (live; each demonstrably fails the check it targets)

| Control | Mechanism | Result |
|---|---|---|
| Naive loader claims success on a partial transfer (`NaiveLoader`) | swallows the truncation, skips commit verification, returns success for patch B | the core's digest gate keeps the image stale (state protected), and the **host-claim verification check FAILS the stub's claim** (`checksum(claimed) != committed_digest(claimed_generation)`) | MET |
| Silent-loss host (`DeafHost`) | ignores overflow rejection, never reads status, never retries | writes are silently lost; the no-loss check FAILS it (applied events < intended, with `queue_overflow` recorded by the mock) | MET |
| Stale-state rule (issue negative control) | a truncated patch transfer must not commit; stale state must be detectable | committed generation, image and digest all unchanged and ≠ the intended patch; host claim still verifies as the OLD patch | MET |

## 6. Status word and frame-boundary checks

- First read after reset: `0xD711_0001` (test-asserted).
- FRESH clears on the first accepted write; boot proceeds only on
  FRESH=1 (recall exempt: armed by already-detected reset evidence).
- OVERRUN never sets across all clean traffic in the suite (boot, chord
  burst, patch reload, note-off).
- Every event received in frame N applies in frame N+1 (asserted per
  record against the §4.6 applied-event tap; the tap is read-only,
  in-process — the block-tap observability of H03 §4.6).
- Frame mapping `sample // 64` over 750 frames/s verified (samples 0,
  63, 64, 47,999, 48,000 → frames 0, 0, 1, 749, 750).

## 7. What this does NOT say

- **No board claim** — the mock is in-process; physical SPI/I2S, DAC
  and storage bring-up are H08/H09.
- **No RTL claim** — the mock models the H03 link contract, not any
  Verilog module; RTL-vs-model exactness remains the H-series bar.
- **No synthesis, timing, fit, fidelity, or audibility claim** — none
  of these are established by host-vs-mock conformance (AGENTS.md).
- The 12.288 MHz fallback results (burst refusal, 3-frame boot
  absorption) are recorded findings consistent with the H03 headline
  FAIL; no contract file was edited (STALE rules, H03 §6).

## 8. Reuse and provenance

Locally authored stdlib-only Python following the pinned sibling
*patterns* (no upstream bytes copied): the mock-core genre of
gf180-torchsynth `spec/protocol/` (FRAMING/SESSION/PATCH-LOAD/
MOCK-HARNESS/TRANSPORTS; catalog `protocol-specs`, ruling adapt) and
gf180-parasynth `fpga/spi_host.py` schedulers + `link_budget.py` +
ARCHITECTURE §7 boot protocol (catalog `spi-host-driver`, `link-budget`,
`spi-reg-control`; rulings adapt/adopt — `spread()`-style write
scheduling, per-event write-cost accounting, reset → fresh → image, and
the address-collision lesson). Apache-2.0 siblings; provenance recorded
in `docs/reuse/catalog.json`. All four new files are byte-pinned in the
catalog's `local_originals`.

## 9. Evidence and reproduction

```sh
python3 -m unittest tests.test_host_mock -v   # 32 tests
python3 -m unittest discover -s tests -v      # full suite
bash tools/check_all.sh                       # repo checks
```

| Artifact | Role |
|---|---|
| `src/gf180_dx7/mock_core.py` | contract-faithful mock core + failure injection |
| `src/gf180_dx7/host.py` | embedded-host library (scheduling, load/commit, reset/recall, budget) |
| `tests/test_host_mock.py` | the acceptance suite (32 tests incl. 3 live negative-control stubs) |
| `docs/reuse/catalog.json` | byte-pins of the four new files (`local_originals`) |

Statuses: host-vs-mock conformance **PASS** (32/32); event-timing,
reset/recall, partial-transfer, malformed-SysEx, write-cost acceptance
items **PASS** as individually asserted above; negative controls
**MET**; board, RTL, synthesis, fidelity, audibility: **NOT_RUN / out
of scope by design**.
