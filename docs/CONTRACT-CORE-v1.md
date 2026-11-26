# CONTRACT-CORE-v1 — core interface and per-frame schedule contract (H03)

- **Issue:** [#25](https://github.com/2AMLogic/gf180-dx7/issues/25) · amended by
  [#339](https://github.com/2AMLogic/gf180-dx7/issues/339) · epic #2 · planning ID H03
- **Frozen:** 2026-09-20 · **Amended:** 2026-11-26 · **Status:** see
  [DR-0008](decision-records/0008-core-schedule-contract.md) and
  [DR-0015](decision-records/0015-h03-admission-budget-reconciliation.md)
- **Machine-readable companion:**
  [`spec/core-contract-v1.json`](../spec/core-contract-v1.json) — kept in
  lockstep by `tests/test_h03_contract.py` and recomputed by
  `tools/h03_budget_check.py`; item 6 view regenerated into
  [`spec/core-contract-v1.signoff.json`](../spec/core-contract-v1.signoff.json)
  by `tools/gen_item6_view.py`.
- **Inputs:** N08 frozen release (`reference/release/`, bundle `81bcc43c…`);
  H01 operator probe (1 clk/eval measured, `evidence/h01-operator-probe/`);
  H02 storage probe (38,781 mapped flops, 2R+1W port structure,
  `evidence/h02-storage-probe/`); N01 §6/§7 schedule and storage facts; N03
  envelope cadence facts; D01 clock/board constraints; D00 decisions
  (`docs/DECISIONS-v1.md`); as-built SPI/skid/event-queue/allocator RTL
  (`rtl/dx7_core.v`, `f33cecbd…`), supplied host (`src/gf180_dx7/host.py`,
  `4e7457a8…`), and SPI admission probe (`evidence/h03-event-admission/`).
- **Claim boundary:** planning verdicts, STALE dispositions, and measured RTL
  probe coverage are reported separately. All-input RTL deadline has
  **NO_VERDICT**, 12.288 MHz RTL execution is **NOT_RUN**, and no
  synthesis/timing/fit claim (H10/H11), original-DX7 fidelity claim, or
  audibility claim is made or implied (plan §7, DEC-001, DEC-013, DEC-024).

---

## HEADLINE FINDING (reconciled worst-case planning verdict, DR-0015)

> **12.288 MHz does NOT close the worst-case frame under any framing: the
> reachable raw-wire mix demands 34,349 clocks against 16,384 available
> (209.65%, margin −17,965 clocks), the supplied host sequence demands up to
> 16,765 clocks (margin −381), and the historical 16/16/16 vector demands
> 17,261 clocks at arbitrary phase (105.35%, shortfall 877; 17,260 / shortfall
> 876 under whole-period drain).**
>
> **24.576 MHz remains the selected design point (DR-0008): it closes the
> historical 16/16/16 vector with +15,423 clocks of margin (17,345 / 32,768 =
> 52.93%; 17,344 / +15,424 under whole-period drain) and closes supplied-host
> (`src/gf180_dx7/host.py`) traffic with +11,503 clocks of margin (21,265 /
> 32,768), but FAILS the all-admissible raw-wire planning budget under H03
> unit-cost estimates: 55,937 / 32,768 clocks (170.71%, margin −23,169) on the
> 194-strike reachable witness, and 34,880 clocks (margin −2,112) already on
> the 100-note-on EE witness (DR-0015).**
>
> The idle-frame arithmetic (12,333 clocks at arbitrary phase / 12,332
> whole-period = 75.27%) would "close" 12.288 MHz only by dropping the event
> rows — the forbidden average-case argument issue #25's negative control
> rejects. Polyphony (16 voices, DEC-003), six operators (DEC-002), and 48 kHz
> (DEC-009) remain fixed. Measured RTL probes on `rtl/dx7_core.v` at 24.576 MHz
> observe `OVERRUN = 0` across all 8 exercised boundary/burst cases (up to 194
> completed strikes in 8,257 active clocks), while the all-input RTL deadline
> remains **NO_VERDICT** and ss-corner physical timing closure belongs to H10
> (deferred under DR-0013).

---

## 0. Scope, evidence rules, change control

This contract fixes, for H04–H08 RTL and U04 host work:

1. the **per-frame worst-case budget** (§2) — every consumer, line by line,
   reconciled with arbitrary-phase SPI completions, queue carry-over, and
   separate raw-wire vs supplied-host pricing (DR-0015);
2. the **storage port schedule** (§3) — how 2R+1W per eval slot resolves the
   H02 conflicts;
3. the **interface contract** (§4) — patch load/commit, events, skid + event
   queue and overflow, backpressure/status, reset/boot, latency, trace;
4. the **module boundaries** for RTL (§5), each with clocking, reset,
   handshake, and its conformance oracle;
5. the **STALE rules and §6 evidence disposition** (§6).

Evidence rules: costs are labeled `measured` (an artifact in this repo
establishes them), `structural` (a count fixed by frame/polyphony/link
arithmetic or a one-register-write operation), or `derived_estimate` (an
upper-bound arithmetic estimate with its basis cited; H04–H06 must measure
them and re-run the budget tool). Worst case never average: planning requires
every admissible frame to close; where a reachable wire witness exceeds the
frame under the H03 unit costs, planning reports **FAIL** — never **PASS**
(issues #25, #339; AGENTS.md).

Change control is §6 (STALE rules). Nothing in this contract weakens a
product goal: 16-note polyphony (DEC-003), all 32 algorithms (DEC-002),
48 kHz (DEC-009), dry mono I2S (DEC-008/DEC-012) are premises, not knobs.
Any change to queue depth, admission, latency, host protocol, or overflow
semantics requires operator escalation.

## 1. Frame and clock targets

Frozen by NUM-002 and DEC-009: 48,000 samples/s in 64-sample frames =
**750 frames/s**; 16 notes × 6 operators = 96 operator evaluations per
sample = **6,144 operator evaluations per frame** =
**4,608,000 operator evaluations/s**.

| Clock | clk/sample | clk/frame | op-loop floor share | all-in clk/eval | planning disposition (§2) |
|---|---:|---:|---:|---:|---|
| 12.288 MHz (fallback) | 256 | 16,384 | 6,144/16,384 = 37.5% | 2.667 | **FAIL** (16/16/16, host, and raw wire) |
| 24.576 MHz (**design point**) | 512 | 32,768 | 6,144/32,768 = 18.75% | 5.333 | **FAIL** all-admissible raw wire; **PASS** 16/16/16 & supplied host |

The op-loop-only minimum clock is 4.608 MHz (1 clk/eval measured by H01).
24.576 MHz remains the selected design point (DR-0008); no physical timing
claim is made for either clock (H10 / DR-0013).

## 2. Per-frame worst-case budget (reconciled, DR-0015)

### 2.1 Arbitrary-phase SPI completion and drain bound

Each 48-bit SPI write at `SCK <= f_core/4` occupies `48 × 4 = 192` core clocks
plus a `2`-clock `CS_N` high gap = **194 core clocks** minimum spacing. For a
half-open frame `[0, W)` (where a completion at offset `W` belongs to the next
frame `[W, 2W)`), a transaction already in flight across the opening boundary
can complete at offset `0`, followed by completions at `194, 388, ..., 194 ×
floor((W − 1) / 194)`. The maximum number of SPI completions (and likewise
1-write/clk skid drains `skid_pop`) in `[0, W)` is therefore:

`C_max(W) = 1 + floor((W − 1) / 194)` → **85** for `W = 16,384` (`84 × 194 = 16,296 < 16,384 ≤ 16,490`) and **169** for `W = 32,768` (`168 × 194 = 32,592 < 32,768 ≤ 32,786`).

The original H03 floor divisions (`84` and `168`) counted whole periods only
and undercounted arbitrary-phase traffic by 1 completion per frame.

### 2.2 Skid stage, 168-entry event queue, carry-over, and allocator states

In `rtl/dx7_core.v`:
- **Skid FIFO (`skid[0:7]`):** `skid_push` (`wr_wr`) fires one clock after the
  48th synchronized `sck_rise`; `skid_pop = (skid_cnt != 0)` drains 1 write/clk
  into `SEC=1` shadow registers or the `SEC=0` event queue (`evq`). At `>= 194`
  clocks/write, `skid_cnt <= 1` on every reachable cycle (`skid_full` is
  unreachable; `skid_rej = 0`), carrying at most **1** write in flight across a
  frame boundary (`skid_push` at offset `W − 1` drains at offset `0` of the next
  frame).
- **Event queue (`evq[0:167]`, `EVQ_DEPTH = 168`):** `SEC=0` drains are admitted
  when `evq_cnt < 168`. When `evq_cnt == 168` (`evq_full == 1`), a `SEC=0` drain
  sets sticky `st_overflow <= 1` and is **rejected at `evq` even on a
  simultaneous `wc_pop_fire`** (`dx7_core.v:1445,1566,1580`), while that cycle's
  pop decrements `evq_cnt` to `167` and frees space for *later* cycles.
- **Drain-until-empty pop loop (`F_EVPOP`) and occupied allocator:** `F_COMMIT`
  runs at offset `0` (`SEC=1, A=0x42` bypasses `evq`, so `shd_commit = 1` can
  coincide with `evq_open = 168`), and `F_EVPOP` begins at offset `1`, popping
  until `evq_empty` (`evq_cnt == 0`). With all 16 voices occupied and sounding,
  each raw `EA_NOTEON` enters `F_ALLOC` (`!free_found`, `match_found ||
  steal_found`, `kind = 1`) and completes a strike in **26 core clocks** (under
  a completely silent patch where `sounding == 0 && in_rel == 0`, `F_ALLOC`
  exits in 2 clocks with 0 strikes). Because `26 < 194` and `224 > 48 > 32`,
  raw `EA_NOTEON`s uniquely maximize per-slot planning cost, and pops during
  `F_EVPOP` admit additional mid-frame arrivals before `evq` empties:

| Clock (`W`) | Frame `N−1` (`evq_open` → admits → pops → `evq_close`) | Frame `N` `skid_open` / `evq_open` | Frame `N` completions / `evq` admits / `evq` rej | Frame `N` pops (`note_on` / `note_off` / `ctrl`) | Frame `N` `evq_close` / `skid_close` |
|---|---|---:|---:|---:|---:|
| 12.288 MHz (`16,384`, clean prior) | `0 → 85 → 0 → 85` | `0 / 85` | `84 / 84 / 0` (`d_1 = 108`) | **`98` (`98 / 0 / 0`)** | `71 / 0` |
| 24.576 MHz (`32,768`, clean prior) | `0 → 168 → 0 → 168` | `0 / 168` | `169 / 169 / 0` (`d_1 = 2`) | **`194` (`194 / 0 / 0`)** | `143 / 0` |

**Unresolved paper conservation upper bound:** Under H03's `derived_estimate`
`224` clk/note-on (`> 194` clk/arrival), service would be slower than arrival
so `evq` would not empty mid-frame. Queue conservation (`Q_open + E_admitted`)
alone bounds queued note-ons in one frame to **`168 + 169 = 337`** at
24.576 MHz (`87,969` clocks, margin **`−55,201`**) and **`85 + 84 = 169`** from
a clean prior frame at 12.288 MHz (`50,253` clocks, margin **`−33,869`**; or
`168 + 85 = 253` from an already-full `evq`, `69,069` clocks, margin
**`−52,685`**).

### 2.3 Normative worst-case raw-wire line-by-line table

| # | Consumer | Unit clk (class) | × Count | Clk/frame | Basis |
|---:|---|---|---:|---:|---|
| 1 | Operator evals (`op_evals`) | 1 (`measured`, H01) | 6,144 | **6,144** | H01 probe: sustained 1 eval/clk, 4,794/4,794 bit-exact |
| 2 | Operator-envelope steps (`env_steps`) | 24 (`derived_estimate`) | 96 | **2,304** | one step per op per frame incl. advance (NUM-005) |
| 3 | Frequency updates (`freq_updates`) | 8 (`derived_estimate`) | 96 | **768** | 1025-entry interpolated Freqlut per op (NUM-003) |
| 4 | Pitch-EG steps (`pitch_eg_steps`) | 12 (`derived_estimate`) | 16 | **192** | one per note per frame (NUM-008) |
| 5 | LFO `getsample` + `getdelay` (`lfo_frame`) | 100 (`derived_estimate`) | 1 | **100** | once per frame, shared per timbre (NUM-007) |
| 6 | Pitch-mod application (`pitch_mod_application`) | 8 (`derived_estimate`) | 16 | **128** | log-domain add, all notes |
| 7 | Amp-mod application (`amp_mod_application`) | 4 (`derived_estimate`) | 96 | **384** | per-op gain modification once per frame |
| 8 | Control-tail state commits (`control_tail_commits`) | 1 (`structural`) | 112 | **112** | 96 env + 16 pitch-EG register writes |
| 9 | Note-on application (`note_on_application`) | 224 (`derived_estimate`) | 98 @12.288 / 194 @24.576 | **21,952 / 43,456** | largest RTL-reachable raw `EA_NOTEON` pops with 16 occupied voices (§2.2, DR-0015) |
| 10 | Note-off application (`note_off_application`) | 48 (`derived_estimate`) | 0 (16 in reg.; 91/183 host) | **0** | dominated by 224-clk `EA_NOTEON` in raw `evq` mix; priced in §2.4 |
| 11 | Controller events (`controller_events`) | 32 (`derived_estimate`) | 0 (16 in reg.) | **0** | dominated by 224-clk `EA_NOTEON` in raw `evq` mix; priced in §2.4 |
| 12 | Patch-commit bookkeeping (`patch_commit`) | 64 (`derived_estimate`) | 1 | **64** | atomic generation swap (`SEC=1` `0x42` bypasses `evq`, DEC-019) |
| 13 | Per-note convert + mix (`mix_convert`) | 2 (`derived_estimate`) | 1,024 | **2,048** | 16 notes × 64 samples, serialized (NUM-010) |
| 14 | I2S sample loads (`i2s_loads`) | 1 (`structural`) | 64 | **64** | one registered load per sample |
| 15 | SPI write-queue drain (`write_queue_drain`) | 1 (`structural`) | 85 @12.288 / 169 @24.576 | **85 / 169** | `1 + floor((W − 1) / 194)` arbitrary-phase half-open bound (§2.1) |
| 16 | Frame controller overhead (`frame_controller_overhead`) | 4 (`structural`) | 1 | **4** | 1 tick + 3-stage H01 latency, once per frame |
| | **Total (reachable raw-wire witness)** | | | **34,349 @12.288 / 55,937 @24.576** | |

### 2.4 Separate pricing: raw wire witnesses, supplied host (`host.py`), and 16/16/16 regression

The supplied host (`src/gf180_dx7/host.py`) reserves `STATUS_POLL_WINDOWS = 1`
out of `floor(W / 194)` (`write_cap = 83` @12.288 MHz, `167` @24.576 MHz) and
emits **8** writes per note-on (`1` velocity + `6` detune deltas + `1` command =
`28` planning clk/write) and **1** write per note-off (`48` clk/write) or
controller (`32` clk/write), giving **160** writes (`161` windows incl. poll)
for the historical 16/16/16 vector.

| Workload / witness | Clock | Frame clocks | Demand | Utilization | Signed margin | Planning verdict |
|---|---|---:|---:|---:|---:|---|
| Historical 16/16/16 (original 84-drain, 48 raw writes) | 12.288 MHz | 16,384 | 17,260 | 105.35% | **−876** | **FAIL** |
| Historical 16/16/16 (arbitrary-phase 85-drain, 48 raw writes; host 160 > 83 refused) | 12.288 MHz | 16,384 | 17,261 | 105.35% | **−877** | **FAIL** |
| Historical 16/16/16 (original 168-drain, 48 raw / 160 host writes) | 24.576 MHz | 32,768 | 17,344 | 52.93% | **+15,424** | **PASS** (vector only) |
| Historical 16/16/16 (arbitrary-phase 169-drain, 48 raw / 160 host writes) | 24.576 MHz | 32,768 | 17,345 | 52.93% | **+15,423** | **PASS** (vector only) |
| Supplied host (`host.py`) single-frame note-on mix (`10` on + `3` off = `83` writes) | 12.288 MHz | 16,384 | 14,781 | 90.22% | +1,603 | single-frame subset |
| Supplied host (`host.py`) single-frame max (`83` note-offs = `83` writes) | 12.288 MHz | 16,384 | 16,381 | 99.98% | +3 | single-frame subset |
| Supplied host (`host.py`) carry + replenish max (`91` note-offs, `evq_close = 75`) | 12.288 MHz | 16,384 | 16,765 | 102.33% | **−381** | **FAIL** |
| Supplied host (`host.py`) single-frame note-on mix (`20` on + `7` off = `167` writes) | 24.576 MHz | 32,768 | 17,297 | 52.79% | +15,471 | **PASS** (host only) |
| Supplied host (`host.py`) single-frame max (`167` note-offs = `167` writes) | 24.576 MHz | 32,768 | 20,497 | 62.55% | +12,271 | **PASS** (host only) |
| Supplied host (`host.py`) carry + replenish max (`183` note-offs; 334-bound = 28,513 / +4,255) | 24.576 MHz | 32,768 | 21,265 | 64.90% | **+11,503** | **PASS** (host only) |
| EE 100 raw note-ons + original non-event rows (`17,344 − 4,864 + 100 × 224`) | 24.576 MHz | 32,768 | 34,880 | 106.45% | **−2,112** | **FAIL** |
| Probe Frame 4 isolated 100 raw note-ons (`0` drains, `0` commit) | 24.576 MHz | 32,768 | 34,648 | 105.74% | **−1,880** | **FAIL** |
| Probe Frame 4 isolated 168 raw note-ons (`0` drains, `0` commit) | 24.576 MHz | 32,768 | 49,880 | 152.22% | **−17,112** | **FAIL** |
| Raw wire full opening queue (`85` / `168` note-ons + commit + drain) | 12.288 / 24.576 | 16,384 / 32,768 | 31,437 / 50,113 | 191.88% / 152.93% | **−15,053 / −17,345** | **FAIL** |
| **Raw wire carry + replenish reachable max (`98` / `194` note-ons, §2.3)** | **12.288 / 24.576** | **16,384 / 32,768** | **34,349 / 55,937** | **209.65% / 170.71%** | **−17,965 / −23,169** | **FAIL** |
| Raw wire unresolved paper conservation bound (`169` clean [`253` full] / `337` note-ons) | 12.288 / 24.576 | 16,384 / 32,768 | 50,253 / 87,969 | 306.72% / 268.46% | **−33,869 / −55,201** | **FAIL** |

**Family calibration (issue #25 acceptance):** the sibling parasynth monosynth
spends ~154/256 clk/frame (~60%) *on average* for one paraphonic voice + drums,
and its ladder filter measures 24 cycles/channel (8 channels = 192/256 = 75%,
practical ceiling datapoint) — quoted from D01 §2 as family calibration only,
never transferred as a budget.

## 3. Storage port schedule (H02 integration)

Structure (H02 verdict): **flop banks** — 96 operator banks × 325 bits +
16 note banks × 446 bits = 38,336 replicated runtime bits (N01 §6 rev 1.1),
plus the patch-shared store (§4.1). H02 measured the full image at 38,781
mapped flops / 4.115 mm² all-in on the probe structure; fit belongs to H10
(deferred under DR-0013).

Per eval slot the schedule needs **2R + 1W**: one 325-bit operator-bank read
and one 446-bit note-bank read (parallel), and one staged write. Conflict
resolutions:

1. **Same-slot read-modify-write (feedback history)** — resolved by **staged
   delayed writeback** (96-slot interleaving; note-bank writes in disjoint
   slots).
2. **1RW SRAM macros cannot serve 2R+1W per eval cycle** — resolved by the
   flop verdict (H02 §4.2/§5).
3. **Eval-read vs env-write** — **slot disjointness**: eval pass owns read
   ports during eval slots; control-tail writes (96 env + 16 pitch-EG) and
   event commits run in disjoint windows.
4. **No same-cycle dual write** — even at 194 strikes × 7 bank writes = 1,358
   event writes, `6,144 + 1,024 + 112 + 1,358 = 8,638` writes (7,328 on the
   historical 48-event vector) stay well below 16,384 / 32,768 available slots;
   the schedule fails on **planning cycle demand** (§2), not W-port bandwidth.

## 4. Interface contract

### 4.1 Patch load/commit (atomic; held notes keep the old patch)

Patch bytes arrive as SPI register writes (page SEC = 1) into a **shadow
image**, continuously drained at 1 write/clk — never live, and bypassing the
`SEC=0` event queue (`evq`). The commit is **atomic at a 64-sample frame
boundary** (DEC-019): the shadow swaps into the committed patch image (4-entry
ring in the contract; declared >3-commits bound recorded for owner ruling per
DEC-019). A full load (≤216 writes) absorbs across ≤2 frames at 24.576 MHz
(≤169 completions/frame).

### 4.2 Events: application timing, skid + event queue, overflow

Transport: **SPI slave**, 48-bit frames `{F, 6'b0, SEC, A[7:0], D[31:0]}`
adapted from gf180-parasynth `rtl-sketch/spi_ctl.v` (DR-0007; Apache-2.0,
provenance + requalification per DR-0002). **SCK ≤ f_core/4**: 6.144 MHz at
24.576 MHz, 3.072 MHz at 12.288 MHz.

- **Application timing:** events queued in `evq` are popped at `frame_tick` in
  `F_EVPOP` before sample 0 of the new frame (DEC-014 + NUM-011). Because
  `F_EVPOP` drains until `evq_empty`, events draining into `evq` while
  `F_EVPOP` is still active are applied in that same frame; events draining
  after `F_EVPOP` exits carry over into the next frame.
- **Two-stage buffering (`skid` + `evq`):** writes land in a **skid FIFO
  (depth 8 × 49 bits)** and drain at 1 write/clk. At `>= 194` core clocks per
  transaction, `skid_cnt <= 1` (skid overflow is unreachable at compliant SCK).
  `SEC=0` writes drain into the **168-entry event queue (`evq`, 37 bits)**.
  Because up to **169** completions can fall in `[0, 32,768)` at arbitrary
  phase (or carry over from the previous frame), a `SEC=0` write presented when
  `evq_cnt == 168` is rejected at `evq` — including on a simultaneous
  `wc_pop_fire` cycle — setting sticky `OVERFLOW` (DR-0015).
- **Overflow policy (DEC-014):** explicit rejection via sticky `OVERFLOW` flag
  (cleared on pad reset as built, documented in `docs/H08-PIN.md` F-OVF-1);
  silent drops and silent deferral are forbidden.
- **Determinism & performance controls:** every accepted event appears in the
  ordered application trace (DEC-017); note on/off, velocity, sustain
  (DEC-018), pitch bend, mod wheel, aftertouch, volume, all-sound-off
  (DEC-006), with single transpose/scaling application (DEC-023; N08
  `perf-transpose-12`).

### 4.3 Backpressure/status registers (DR-0007 pattern, adapted)

Status word on MISO (32 bits, loaded at CS_N fall): `{0xD7, 4'h1, OVERRUN,
QUEUE, OVERFLOW, FRESH, FRAME[15:0]}`:

| Field | Width | Meaning |
|---|---:|---|
| MAGIC | 8 | `0xD7` |
| VERSION | 4 | `0x1` (this contract's map) |
| OVERRUN | 1 | schedule window missed (`fc_state != F_IDLE` at `frame_tick`) — sticky defect detector, must never set |
| QUEUE | 1 | skid FIFO nonempty |
| OVERFLOW | 1 | a write was rejected (`skid_full` or `evq_full`) — sticky backpressure/defect detector (DEC-014) |
| FRESH | 1 | no write accepted since reset (`reset → fresh → image` boot protocol) |
| FRAME | 16 | frame counter |

### 4.4 Reset and boot protocol

`RST_N` (pad, active low) through a **two-flop synchronizer**; synchronous
reset; frame counter and link reset by the pad only. `SOFT_RESET` clears
voice/env/phase state without touching the link, queues, or frame counter.
Boot protocol: **reset → fresh → image** (DEC-011, DEC-020, DEC-022).

### 4.5 Latency definition (measured meaning for H07/H09)

**Event-sample → I2S-out latency** = distance from an event's sample position
*p* in frame *N* to the SDATA bit of the first audio sample its commit affects.
Declared bound: **≤ 2 frames = 128 samples = 2.667 ms** worst case (`[(64 − p),
(64 − p) + 65]` samples; NUM-011).

### 4.6 Trace access

- **State observability is load-bearing** (H01/H02 pattern; strip controls stay
  live per module).
- **Block taps (optional):** read-only and **non-invasive** (TRACE-PLAN rule).
- **Conformance use:** N08 bundle traces (138 applied events) are frozen.

### 4.7 No per-sample host dependency

The core advances audio-rate phase, envelopes, routing/feedback and mixing
autonomously (DEC-010). All float-gated reference math (bend scaling, amp-mod
sensitivity — NUM-008) runs host-side at event time; the chip consumes frozen
integer results.

## 5. Module boundaries for RTL (H04–H07)

Single clock domain, no derived clocks; BCLK and LRCLK are frame-counter bits
driven as registered outputs (no CDC except two-flop-synchronized SPI pins).

**I2S TX geometry (binding; amended by issue #78, operator ruling
2026-09-26).** The as-built geometry is the contract:

| Quantity | Binding value |
|---|---|
| BCLK period | **8** `clk` (3.072 MHz = 64 fs at the 24.576 MHz design point) |
| LRCLK period | **512** `clk` (one 48 kHz frame, DEC-009) |
| Payload | **22-bit**, MSB first, frame-aligned to the LRCLK rise, carried as `{mix[21:0], 2'b00}` in the LRCLK-high half only; the LRCLK-low half is all zeros |
| Alignment | 24-bit field loads at LRCLK-rise tick (BCLK falls on that tick); each bit cell is **4 `clk`** (`s21` in `[0, 8)`, `s20..s0` in 4-`clk` cells through `[88, 92)`). Binding reader: frame-aligned 22-bit recovery reader (`rtl/tb_synth_top.v`, `docs/H08-PIN.md`; issue #118 documents stale comments in `rtl/dx7_core.v`) |

| Module | Role | Clocking / reset | Handshake | Conformance vectors (N08 ids) + controls |
|---|---|---|---|---|
| `frame_controller` | cycle counter, tick/go, window enforcement (commit → control tail → eval pass → mix → I2S), OVERRUN | core clock; pad reset | tick/go pulses; busy collection at tick | `dir-base`, `ev-sum-15`, `ev-steal-16`; OVERRUN-never-sets over N08 suite; directed late-finish injection control |
| `op_core` | shared 96-state evaluation kernel (mkiSin + phase + convert), 1 eval/clk, 3-stage | core clock; sync (pad + soft scope) | `en`/`busy`/`out_valid` in acceptance order; `ld_phase` corners | H01 4,794-vector bit-exact equivalence retained; `dir-base`, `alg4-fb7`, `alg6-fb3`, `bd-rising-env`, `osc-fixed-carrier`; MUTATE_ADDRWIDTH control live |
| `state_banks` | 96×325 b + 16×446 b flop banks, 2R+1W staged writeback | core clock; sync; all state defined at reset | combinational bank reads per slot; writeback ack | H02 readback smoke + 38,781-flop gate + strip control; `dir-base`, `ev-sum-15`, `ev-steal-16`, `ev-repeated-note` |
| `env_unit` | 96 env steps + 16 pitch-EG steps per frame (N03 semantics) | core clock; control-tail window | `tail_start`/`tail_done` | `bd-fast-release`, `bd-slow-rate3`, `bd-delayed-attack`, `bd-nonzero-final`, `ev-sustain-pedal`, `ev-retrigger`; N03 sr_multiplier ±1 control |
| `pitch_mod_unit` | Freqlut, LFO, PM/AM application; DEC-023 single application | core clock; control-tail window; LFO init per DEC-020 | `params_valid` → freq/gain committed before `go` | `ev-pitch-bend-up/down/center`, `perf-transpose-12`, `lfo-pm-sens7`, `lfo-wave1`, `lfo-wave5`, `lfo-delay99`, `ampmod-ams3`, `peg-up`, `kbd-right-c3`, `bd-coarse0/31`, `bd-detune0/14`, `bd-fine99`, `bd-rs3`, `vel-kvs7-30`, `vel-kvs0-127`, `osc-keysync0` |
| `algorithm_router` | 32 algorithms, Mark I gate (16284), feedback incl. algo 4/6 serial (+2 shift) | core clock (wiring + compares) | per-eval in/out with op pipeline | `alg-05/16/31`, `alg4-fb0/3/7`, `alg6-fb0/3/7`; N08 `broken_feedback` mutation control |
| `mixer` | per-note convert + unclamped ≥20-bit mix (NUM-010 proof) | core clock; accumulator cleared per note-sample | note accumulator in; frame mix out | `ev-sum-15` (sums past per-note rails), `ev-all-sound-off`, `ev-all-notes-off`; `voice_sum_order_swap` mutation control |
| `spi_ctl` | SPI slave, 48-bit frames, skid queue, continuous drain, status word | core clock; pins through 2-flop syncs; pad reset only | `wr_valid`/`wr_addr`/`wr_data`; MISO status at CS_N fall | pin-to-acceptance + drain-order bench; event traces reproduced in order (`dir-base`, `ev-repeated-note`, `perf-transpose-12`) |
| `i2s_tx` | I2S transmitter, BCLK/LRCLK from counter bits, 1 load/sample | core clock; pad reset | sample-load handshake | decoded-wire vs model bench; H07/H09 latency measurement |

## 6. STALE rules and DR-0015 evidence disposition

- **Contract change:** any change to `docs/CONTRACT-CORE-v1.md` or
  `spec/core-contract-v1.json` **after H04–H07 implementation starts**
  requires a new decision record in `docs/decision-records/` and marks the
  affected modules' RTL evidence **STALE** (status vocabulary per AGENTS.md).
- **DR-0015 STALE disposition (#339):**
  - **H07 schedule/burst completeness (`docs/H07-CORE.md` §2,
    `evidence/h07-core/results-dr0012-p{1,2}.json`,
    `evidence/h07-core/stress/manifest.json` `stress-burst`) — `STALE`:**
    `stress-burst` exercises 167 host-encoded writes under the superseded 168
    whole-period bound and does not cover 169 arbitrary-phase completions or
    194 raw-wire `EA_NOTEON` strikes. H07's **RTL-vs-frozen-model PCM
    bit-exactness** across its 34 vectors is unaffected (`rtl/dx7_core.v`
    unchanged at `f33cecbd…`).
  - **H08 pin burst-bound citations (`docs/H08-PIN.md`, `tests/test_h08.py`) —
    `STALE`:** citations treating H03 §4.2's 168-period bound as unoverflowable
    under compliant SPI rate are STALE (a 169th `SEC=0` write into an unpopped
    168-entry `evq` sets `OVERFLOW` at `evq_full`); H08's 11-vector pin-vs-flat
    equivalence and corrupted-frame controls stand on unchanged RTL bytes.
  - **H01, H02, H04, H05, H06 slice evidence — unaffected** (`sha256` anchors
    verified; standalone slice timings remain slice bounds, not an integrated
    all-input WCET proof).
- **Artifact drift:** drift of any hash in `depends_on` (N08 bundle, numeric
  profile, H01/H02 evidence, H03 SPI admission probe) fails
  `tools/h03_budget_check.py` and marks every H03-derived budget STALE.
- **Clock / policy change:** changing the design point, fallback verdict, queue
  depth, admission, latency, or overflow semantics requires a new DR and
  operator sign-off.
- **Negative controls:** restoring whole-period `84/168` drain counts,
  stripping worst-burst rows, or restoring an all-mixes `PASS` at 24.576 MHz
  fails `tools/h03_budget_check.py`.

## 7. What this does NOT say

- It does NOT claim an all-input RTL deadline guarantee (`NO_VERDICT` at both
  clocks; 12.288 MHz RTL execution is `NOT_RUN`).
- No synthesis, place-and-route, timing closure, utilization, or fit claim —
  including for the 24.576 MHz design point (H10 / DR-0013).
- No claim that the H02 area finding (16-note state > quarter slot) is
  resolved.
- No original-DX7 fidelity, preset-quality, or musical-usefulness claim;
  Dexed agreement is not original-DX7 fidelity (DEC-001, DEC-024, plan §7).

## 8. Evidence and reproduction

```sh
python3 tools/h03_budget_check.py
python3 -m unittest tests.test_h03_contract -v
python3 tools/gen_item6_view.py --check
python3 evidence/h03-event-admission/run_probe.py --check
```

| Artifact | Role |
|---|---|
| `spec/core-contract-v1.json` | machine-readable contract (rows, regression, raw/host pricing, STALE disposition) |
| `spec/core-contract-v1.signoff.json` | generated signoff item 6 view (`tools/gen_item6_view.py`) |
| `docs/decision-records/0008-core-schedule-contract.md` | original adoption record and clock-target scope |
| `docs/decision-records/0015-h03-admission-budget-reconciliation.md` | §6 post-implementation admission/budget amendment |
| `evidence/h03-event-admission/tb_event_admission.sv` + `results.json` | Verilator 5.050 SPI admission/carry-over probe (8 exercised 24.576 MHz runs) |
| `evidence/h01-operator-probe/synth_report.json` | measured 1 clk/eval anchor (sha256-pinned) |
| `evidence/h02-storage-probe/storage_synth_report.json` | measured storage/port anchor (sha256-pinned) |
| `reference/release/manifest.json` | frozen conformance oracle binding (bundle sha256-pinned) |

Statuses: budget & conservation recomputation **PASS**; all-admissible raw-wire
planning **FAIL** at 12.288 MHz and **FAIL** at 24.576 MHz; historical 16/16/16
vector **FAIL** at 12.288 MHz / **PASS** at 24.576 MHz; supplied-host (`host.py`)
planning **FAIL** at 12.288 MHz / **PASS** at 24.576 MHz; exercised 24.576 MHz
SPI probes **PASS** (`OVERRUN = 0`, `OVERFLOW` at `evq` stage only when hit at
occupancy 168); all-input RTL deadline **NO_VERDICT**; 12.288 MHz RTL execution
**NOT_RUN**; H07/H08 worst-burst completeness claims **STALE**.
