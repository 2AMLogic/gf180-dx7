# CONTRACT-CORE-v1 — core interface and per-frame schedule contract (H03)

- **Issue:** [#25](https://github.com/2AMLogic/gf180-dx7/issues/25) · epic #2 ·
  planning ID H03
- **Frozen:** 2026-09-20 · **Status:** see
  [DR-0008](decision-records/0008-core-schedule-contract.md)
- **Machine-readable companion:**
  [`spec/core-contract-v1.json`](../spec/core-contract-v1.json) — kept in
  lockstep by `tests/test_h03_contract.py` and recomputed by
  `tools/h03_budget_check.py`.
- **Inputs (measured):** N08 frozen release (`reference/release/`,
  bundle `81bcc43c…`); H01 operator probe (1 clk/eval measured,
  `evidence/h01-operator-probe/`); H02 storage probe (38,781 mapped flops,
  2R+1W port structure, `evidence/h02-storage-probe/`); N01 §6/§7 schedule
  and storage facts; N03 envelope cadence facts; D01 clock/board constraints;
  D00 decisions (`docs/DECISIONS-v1.md`).
- **Claim advanced:** the RTL architecture is proven schedulable on paper —
  every per-frame consumer accounted at worst case — before H04–H07 write
  RTL against it.
- **Claim boundary:** this is a paper contract. No RTL exists yet, no
  synthesis/timing/fit claim is made or implied (H10/H11), no fidelity claim
  (RTL-vs-frozen-model exactness is the H-series bar), no audibility claim
  (listening records only, plan §7, DEC-001, DEC-024).

---

## HEADLINE FINDING (worst-case verdict)

> **12.288 MHz does NOT close the worst-case frame: the budget demands
> 17,260 clocks against 16,384 available (105.35%, shortfall 876 clocks).
> 24.576 MHz closes with 15,424 clocks of margin (52.93% worst-case
> utilization) and is the design point.**
>
> This is the outcome N01 §7 anticipated ("closes only if the shared core
> sustains ≤ 2.67 clk/op-eval all-in; no margin") — H01's measured
> 1 clk/eval kernel removed the arithmetic risk, but the line-by-line
> accounting of storage, envelopes, mixing, events and link work refutes
> the 12.288 MHz fallback. The idle-frame arithmetic (12,332 clocks =
> 75.3%) would "close" only by dropping the worst-burst event rows — that
> is precisely the average-case argument issue #25's negative control
> forbids. Polyphony is unchanged (DEC-003 is not a relaxation knob); no
> architecture change is required because the design point closes. The
> 24.576 MHz design-point adoption is the owner sign-off item (DR-0008,
> Proposed); ss-corner timing closure at 24.576 MHz remains unproven and
> is H10's measurement (D01 §2).

---

## 0. Scope, evidence rules, change control

This contract fixes, for all H04–H07 RTL work:

1. the **per-frame worst-case budget** (§2) — every consumer, line by line,
   at the worst admissible per-frame value;
2. the **storage port schedule** (§3) — how 2R+1W per eval slot resolves the
   H02 conflicts;
3. the **interface contract** (§4) — patch load/commit, events, queue and
   overflow, backpressure/status, reset/boot, latency, trace;
4. the **module boundaries** for RTL (§5), each with clocking, reset,
   handshake, and its conformance oracle;
5. the **STALE rules** (§6).

Evidence rules: costs are labeled `measured` (an artifact in this repo
establishes them), `structural` (a count fixed by the frame/polyphony
arithmetic or a one-register-write operation), or `derived_estimate` (an
upper-bound arithmetic estimate with its basis cited; H04–H06 must measure
them and re-run the budget tool). Worst case never average: the frame must
close for **every admissible frame**, including a frame containing a full
polyphony chord change — a schedule that closes only when events or envelope
transitions are rare is reported as failing, never as passing (issue #25
negative control; AGENTS.md).

Change control is §6 (STALE rules). Nothing in this contract weakens a
product goal: 16-note polyphony (DEC-003), all 32 algorithms (DEC-002),
48 kHz (DEC-009), dry mono I2S (DEC-008/DEC-012) are premises, not knobs.

## 1. Frame and clock targets

Frozen by NUM-002 and DEC-009: 48,000 samples/s in 64-sample frames =
**750 frames/s**; 16 notes × 6 operators = 96 operator evaluations per
sample = **6,144 operator evaluations per frame** =
**4,608,000 operator evaluations/s**.

| Clock | clk/sample | clk/frame | op-loop floor share | all-in clk/eval | verdict |
|---|---:|---:|---:|---:|---|
| 12.288 MHz (fallback) | 256 | 16,384 | 6,144/16,384 = 37.5% | 2.667 | **FAILS worst case (§2)** |
| 24.576 MHz (**design point**) | 512 | 32,768 | 6,144/32,768 = 18.75% | 5.333 | **closes, 52.93% utilization** |

The op-loop-only minimum clock is 4.608 MHz (one clock per evaluation with a
fully shared datapath — H01 measured exactly this throughput). 24.576 MHz is
the design point because (a) N01 §7 selected it with exactly this fallback
condition, (b) H01 measured the kernel at 1 clk/eval, leaving 1.67 clk/eval
of the 12.288 MHz frame for everything else, and (c) §2 shows everything else
needs 2.86 clk/eval worth of frame time under worst-case admissible input.

No timing claim is made for either clock: ss-corner closure at 24.576 MHz is
unproven (D01 §2; the sibling's joined chip implies a 14.0 MHz ss minimum on
*a different design* — calibration, not transfer). H10 measures timing in the
ORFS flow at the chosen corner.

## 2. Per-frame worst-case budget (line by line)

Basis: every voice live every frame (structural worst case), the worst
admissible event burst in the frame (16 note-ons + 16 note-offs + 16
controller events — admissible: 48 writes ≤ the 84–168 writes/frame the link
can deliver), and every envelope step carrying its worst-case segment
advance. Unit costs cite their evidence; `derived_estimate` values are
upper bounds that H04–H06 conformance must measure.

| # | Consumer | Unit clk (class) | × Count | Clk/frame | Basis |
|---:|---|---|---:|---:|---|
| 1 | Operator evals (mkiSin + phase advance + convert pipeline) | 1 (`measured`, H01) | 6,144 | **6,144** | H01 probe: sustained 1 eval/clk, 4,794/4,794 bit-exact |
| 2 | Operator-envelope steps, worst case incl. segment advance (levellut target, qrate, statics, `inc_ × sr_multiplier` constant multiply; env.cc:63-151) | 24 (`derived_estimate`; N01 §7 budgeted 8–12 ex-advance) | 96 | **2,304** | one step per op per frame (N03 cadence; NUM-005) |
| 3 | Frequency updates: Freqlut 1025-entry interpolated lookup (freqlut.cc:27-42) | 8 (`derived_estimate`) | 96 | **768** | per-op basepitch differs; pitch EG + LFO enter before the LUT (NUM-003) |
| 4 | Pitch-EG steps (pitchenv.cc:55-89) | 12 (`derived_estimate`) | 16 | **192** | one per note per frame (NUM-008) |
| 5 | LFO `getsample` + `getdelay` | 100 (`derived_estimate`; NUM-007: ~50–100) | 1 | **100** | once per frame, shared per timbre |
| 6 | Pitch-mod application per note (dx7note.cc:210-217) | 8 (`derived_estimate`) | 16 | **128** | log-domain add, all notes |
| 7 | Amp-mod application per operator gain (dx7note.cc:259-268) | 4 (`derived_estimate`) | 96 | **384** | N08 `ampmod-ams3` renders this path byte-exact |
| 8 | Control-tail state commits (96 env + 16 pitch-EG) | 1 (`structural`) | 112 | **112** | one register write per clock (H02 ports) |
| 9 | Note-on application: steal scan + per-op derived params (dx7note.cc:163-179) + basepitch + init | 224 (`derived_estimate`) | 16 | **3,584** | worst admissible burst: 16-note chord change (DEC-003 bound) |
| 10 | Note-off application: lookup + sustain branch (DEC-018) + reclaim predicate (NUM-012) | 48 (`derived_estimate`) | 16 | **768** | full chord release in one frame |
| 11 | Controller events (bend-scale commit, sustain, mod wheel, volume) | 32 (`derived_estimate`) | 16 | **512** | float-gated math stays host-side (NUM-008, DEC-010) |
| 12 | Patch-commit bookkeeping (generation swap + held-note tag sweep) | 64 (`derived_estimate`) | 1 | **64** | one commit per frame (DEC-019) |
| 13 | Per-note convert + unclamped ≥20-bit mix add (NUM-010) | 2 (`derived_estimate`; H01 stage 3 measures convert+clip in 1 clk, +1 for the mix add) | 1,024 | **2,048** | 16 notes × 64 samples; serialized — no free overlap assumed |
| 14 | I2S sample loads (BCLK/LRCLK are counter bits; shifting is a dedicated divider) | 1 (`structural`) | 64 | **64** | one registered load per sample |
| 15 | SPI write-queue drain into the shadow image | 1 (`structural`) | 84 @12.288 / 168 @24.576 | **84 / 168** | SCK ≤ f_core/4, 48-bit frame + 2-cycle gap = 194 clk/transaction |
| 16 | Frame controller tick + op-core pipeline fill | 4 (`structural`) | 1 | **4** | 1 tick + 3-stage H01 latency, once per frame |
| | **Total** | | | **17,260 @12.288 / 17,344 @24.576** | |

(Utilization check: `python3 tools/h03_budget_check.py` recomputes every row
and total from `spec/core-contract-v1.json` and the cited measured anchors.)

**Utilization and verdict:**

| Clock | Frame clocks | Budget demand | Utilization | Margin | Verdict |
|---|---:|---:|---:|---:|---|
| 12.288 MHz | 16,384 | 17,260 | **105.35%** | **−876** | **FAIL — headline finding** |
| 24.576 MHz | 32,768 | 17,344 | **52.93%** | **+15,424** | **PASS — design point** |

**The forbidden average-case framing, stated so it cannot sneak back:** an
idle frame (no events, no chord) costs 17,260 − 4,928 = 12,332 clocks =
75.3% of a 12.288 MHz frame — it closes. But a host *can* deliver the full
burst within one frame (48 events ≤ 84 admissible writes), and DEC-014
requires the schedule to hold for **every** admissible frame. Any schedule
argument that closes 12.288 MHz by assuming rare events, rare segment
advances, or partially-live voices is an average-case argument and is
rejected by this contract's negative control (enforced by
`tools/h03_budget_check.py`: the worst-burst rows are mandatory and the
recorded verdicts are checked).

**Family calibration (issue #25 acceptance):** the sibling parasynth
monosynth spends ~154/256 clk/frame (~60%) *on average* for one paraphonic
voice + drums, and its ladder filter measures 24 cycles/channel (8 channels =
192/256 = 75%, the practical-ceiling datapoint) — both quoted from D01 §2
and the issue's prior-art list as **their** designs' calibrations. They
demonstrate the family's realized utilizations and exactly why average-case
arguments fail: this contract's 12.288 MHz budget exceeds 100% on the
worst-case frame despite a ~75% average. No sibling number is transferred as
a budget.

**Sensitivity (what would change the verdict):** the 24.576 MHz margin
(15,424 clocks) tolerates every `derived_estimate` tripling. The 12.288 MHz
failure (−876) is not repaired by plausible unit-cost improvements either:
dropping the envelope advance entirely (row 2 → 12 clk) recovers 1,152 clocks
and still leaves the next estimate to defend. Only an architecture change
(e.g., a second operator datapath — an owner-level decision) or a polyphony
change (forbidden as a shortcut, DEC-003) could reopen 12.288 MHz.

## 3. Storage port schedule (H02 integration)

Structure (H02 verdict): **flop banks** — 96 operator banks × 325 bits +
16 note banks × 446 bits = 38,336 replicated runtime bits (N01 §6 rev 1.1),
plus the patch-shared store (§4.1). H02 measured the full image at 38,781
mapped flops / 4.115 mm² all-in on the probe structure and recorded the
bounded finding that this state cannot be all-register in a quarter slot —
an **owner architecture decision** that this contract assumes (flop
organization) but does not resolve; H10 owns fit.

Per eval slot the schedule needs **2R + 1W**: one 325-bit operator-bank read
and one 446-bit note-bank read (parallel), and one staged write. Conflict
resolutions (each named by H02 §4 or added here):

1. **Same-slot read-modify-write (feedback history)** — impossible on one
   write port; resolved by **staged delayed writeback**: a slot's new
   operator value rides a stage register and takes the W port at a later slot
   of the same bank (96-slot interleaving). The per-sample feedback history
   update rides the note-bank write in disjoint slots.
2. **1RW SRAM macros cannot serve 2R+1W per eval cycle** — resolved by the
   flop verdict (H02 §4.2/§5); banked 512 B macros would serialize the op
   loop.
3. **Eval-read vs env-write** — **slot disjointness**: the frame's eval pass
   owns read ports during eval slots; control-tail writes (96 env + 16
   pitch-EG commits) execute in the control-tail window *before* the eval
   pass and after the tick; frame-boundary commit writes occupy the tick
   cycle itself. No write and no control read ever shares an eval slot.
4. **No same-cycle dual write** — write traffic is bounded below the port
   supply: 6,144 op-bank + 1,024 note-bank + 112 control-tail + ≤48
   event-application writes = **7,328 worst-case writes** against 16,384
   (12.288 MHz) / 32,768 (24.576 MHz) available slots.

Write-bandwidth note: at 1 eval/clk the eval pass consumes 6,144 of the frame's
cycles; the W port schedules the staged writes in the cycles the eval pass
leaves free — supply exceeds demand ≥ 2.2× even at 12.288 MHz. This row of the
budget is feasible at both clocks; the frame fails at 12.288 MHz on **total
cycle demand** (§2), not on port bandwidth.

## 4. Interface contract

### 4.1 Patch load/commit (atomic; held notes keep the old patch)

Patch bytes arrive as SPI register writes (page SEC = 1) into a **shadow
image**, continuously drained at 1 write/clk — never live. The commit is
**atomic at a 64-sample frame boundary** (DEC-019): the shadow swaps into a
**4-entry committed-patch ring**, and each note binds the generation it
started on (2-bit tag per note). Held notes therefore keep sounding on the
patch they started with until note-off and release completion (DEC-019,
DEC-021); new note-ons use the newest generation.

**Declared bound:** a held note is exact for up to 3 subsequent commits; a
4th commit while a note is still held evicts its generation — a declared
DEC-019 conformance gap requiring an owner ruling (the alternative is
per-note replication of `rates_[4]`/`levels_[4]`, +5,376 bits, Dexed-exact).
This bound is recorded, not silently accepted; the reference itself never
exercises it (the pinned wrapper updates held notes' envelope params — the
N08 registry's `ev-patch-change-held` row is BLOCKED: no executable oracle
path — so comparisons never constrain this choice; D00 semantics do).

A full load (161-byte voice + performance settings ≈ ≤216 writes) absorbs
across ≤2 frames of drain capacity (≤168 writes/frame) and never blocks the
audio schedule. Load cost is link traffic, not frame-budget traffic.

### 4.2 Events: application timing, queue, overflow

Transport: **SPI slave**, 48-bit frames `{F, 6'b0, SEC, A[7:0], D[31:0]}`
adapted from gf180-parasynth `rtl-sketch/spi_ctl.v` (DR-0007; Apache-2.0,
provenance + requalification per DR-0002 — adapted pattern, no bytes copied
without record). **SCK ≤ f_core/4** (two-flop synchronizer sampling): 6.144
MHz at the 24.576 design point, 3.072 MHz at 12.288 MHz.

- **Application timing:** an event received during frame *N* is applied
  **before the first sample of frame *N+1*** (DEC-014 + NUM-011) — identical
  to the pinned oracle's `event_to_block_policy`, so comparison event
  alignment is trivial. Worst-case core commit latency = one frame.
- **Queue:** writes land in a bounded **skid FIFO (depth 8 × 49 bits)** and
  drain **continuously** (1 write/clk) into the shadow register image; the
  frame-boundary commit applies the shadow. Depth derivation (never guessed,
  DEC-014): worst admissible arrival = 1 write per 194 core clocks
  (48-bit frame + 2-cycle CS gap at SCK = f_core/4); the drain is 194× the
  arrival rate, so depth 8 is provably sufficient and overflow is impossible
  at the declared SCK bound.
- **Overflow policy (DEC-014):** explicit rejection — sticky `OVERFLOW` flag
  in the status word, cleared on read; the host retries. Silent drops and
  silent deferral to a later frame are forbidden. Since overflow cannot occur
  at the declared bound, the flag is a defect detector (like the parasynth
  `overrun` flag: "a defect if it ever sets").
- **Worst burst:** 16 note-ons + 16 note-offs + 16 controller events in one
  frame; budgeted in §2 rows 9–11 (4,864 clocks of application work).
- **Determinism:** every accepted event appears in the block-level
  event-application trace; N08's frozen traces (138 applied events) are the
  conformance reference after declared latency alignment. Steals and
  rejections surface in status/trace metadata (DEC-017), never random.
- **Performance controls:** note on/off, velocity, sustain (DEC-018), pitch
  bend, mod wheel, channel aftertouch, volume, all-sound-off (DEC-006). The
  chip receives final MIDI note numbers and frozen integer bend/amp-mod scale
  factors; **transpose/scaling/velocity are applied exactly once** — the
  patch's own parameters inside the core, host MIDI transposition host-side
  (DEC-023; the registry's transpose finding is pinned by N08
  `perf-transpose-12`).

### 4.3 Backpressure/status registers (DR-0007 pattern, adapted)

Status word on MISO (32 bits, loaded at CS_N fall), adapting the parasynth
DR-0007 §4 word (`{0x4D, VERSION, overrun, queue_nonempty, overflow, fresh,
frame[15:0]}`; first read after reset reads `0xD711_0001`-shaped
`{MAGIC, VERSION, FRESH=1, FRAME=1}`):

| Field | Width | Meaning |
|---|---:|---|
| MAGIC | 8 | `0xD7` |
| VERSION | 4 | `0x1` (this contract's map) |
| OVERRUN | 1 | schedule window missed (control tail or eval pass unfinished) — sticky defect detector, must never set |
| QUEUE | 1 | skid FIFO nonempty |
| OVERFLOW | 1 | a write was dropped — sticky defect detector (DEC-014 backpressure; host retries) |
| FRESH | 1 | no write accepted since reset |
| FRAME | 16 | frame counter |

`FRESH`/`FRAME` follow the sibling's brown-out protocol (ARCHITECTURE §7):
the host boots on `fresh = 1` and re-sends the image whenever `fresh` reads 1
later. `OVERRUN` gives H07 a pin-visible deadline signal: across the full
N08 suite it must never set.

### 4.4 Reset and boot protocol

`RST_N` (pad, active low) through a **two-flop synchronizer**; every block
uses **synchronous reset**; the frame counter and the link reset by the pad
only (parasynth `synth_top.v`/ARCHITECTURE §7 pattern). A `SOFT_RESET`
register write clears voice/env/phase state and the event shadow **without**
touching the link, the queue, or the frame counter (per-page soft-reset
pattern). Boot protocol: **reset → fresh → image** — after reset all state is
defined (DEC-011: explicit reset, explicit phase/LFO initialization per
DEC-020), the host waits for `FRESH = 1`, sends the patch image +
performance settings (DEC-022: separate state, separate defaults), and the
commit lands atomically at a frame boundary. Post-reset render repeatability
is tested, not assumed (DEC-011).

### 4.5 Latency definition (measured meaning for H07/H09)

**Event-sample → I2S-out latency** = the distance from an event's sample
position *p* in frame *N* to the SDATA bit of the first audio sample its
commit affects. Deterministic components: commit at the frame boundary
(first affected sample = frame *N+1* sample 0 — DEC-014/NUM-011) plus the
sample's serialization position. Declared bound: **≤ 2 frames = 128 samples
= 2.667 ms** worst case. Acceptance mapping for H07 (RTL bench) and H09
(hardware captures): measured `(first_affected_sdata_slot − event_sample)`
must land in `[(64 − p), (64 − p) + 65]` samples. Comparisons align events
per NUM-011 (byte-exact after declared scheduling/latency).

### 4.6 Trace access

- **State observability is load-bearing** (H01/H02 pattern): every state bit
  feeds a read mux or an XOR `state_obs` fold so synthesis cannot delete
  state; dropping any field drops the mapped flop count and fails the gate.
  The strip-observability negative controls stay live per module.
- **Block taps (optional):** a per-frame tap bus (frame counter, live_mask,
  keydown_mask, lfo_value, applied-event log) mirroring the R05 trace
  registry columns. Taps are read-only and **non-invasive**: traced and
  untraced renders must be byte-identical (TRACE-PLAN rule; the stub-oracle
  negative control is the template).
- **Conformance use:** N08 bundle traces (138 applied events) are frozen;
  RTL event-application traces must match them after the declared latency
  alignment.

### 4.7 No per-sample host dependency

The core advances audio-rate phase, envelopes, routing/feedback and mixing
autonomously (DEC-010). The host's per-frame obligations are bounded and
declared: it may deliver up to the SCK-bound write rate; nothing in the
audio path waits on the host. All float-gated reference math (bend scaling,
amp-mod sensitivity — NUM-008) runs host-side at event time; the chip
consumes frozen integer results. This declaration discharges DEC-010's
requirement that low-rate performance modulation be declared with its
worst-case link budget (§2 rows 9–12, §4.2).

## 5. Module boundaries for RTL (H04–H07)

Single clock domain, no derived clocks; BCLK (= clk/4) and LRCLK (= clk/256)
are frame-counter bits driven as registered outputs (parasynth pattern — no
CDC anywhere except the two-flop-synchronized SPI pins). Each module's
conformance oracle is the N08 frozen release (`tools/n08_verify_release.py`,
sha256-exact after declared latency alignment) restricted to the listed
vector subsets, plus the named probe controls.

| Module | Role | Clocking / reset | Handshake | Conformance vectors (N08 ids) + controls |
|---|---|---|---|---|
| `frame_controller` | cycle counter, tick/go, window enforcement (commit → control tail → eval pass → mix → I2S), OVERRUN | core clock; pad reset | tick/go pulses; busy collection at tick | `dir-base`, `ev-sum-15`, `ev-steal-16`; OVERRUN-never-sets over all 51 vectors; directed late-finish injection control |
| `op_core` | shared 96-state evaluation kernel (mkiSin + phase + convert), 1 eval/clk, 3-stage | core clock; sync (pad + soft scope) | `en`/`busy`/`out_valid` in acceptance order; `ld_phase` corners | H01 4,794-vector bit-exact equivalence retained; `dir-base`, `alg4-fb7`, `alg6-fb3`, `bd-rising-env`, `osc-fixed-carrier`; MUTATE_ADDRWIDTH control live |
| `state_banks` | 96×325 b + 16×446 b flop banks, 2R+1W staged writeback | core clock; sync; all state defined at reset | combinational bank reads per slot; writeback ack | H02 readback smoke + 38,781-flop gate + strip control; `dir-base`, `ev-sum-15`, `ev-steal-16`, `ev-repeated-note` |
| `env_unit` | 96 env steps + 16 pitch-EG steps per frame (N03 semantics) | core clock; control-tail window | `tail_start`/`tail_done` | `bd-fast-release`, `bd-slow-rate3`, `bd-delayed-attack`, `bd-nonzero-final`, `ev-sustain-pedal`, `ev-retrigger`; N03 sr_multiplier ±1 control |
| `pitch_mod_unit` | Freqlut, LFO, PM/AM application; DEC-023 single application | core clock; control-tail window; LFO init per DEC-020 | `params_valid` → freq/gain committed before `go` | `ev-pitch-bend-up/down/center`, `perf-transpose-12`, `lfo-pm-sens7`, `lfo-wave1`, `lfo-wave5`, `lfo-delay99`, `ampmod-ams3`, `peg-up`, `kbd-right-c3`, `bd-coarse0/31`, `bd-detune0/14`, `bd-fine99`, `bd-rs3`, `vel-kvs7-30`, `vel-kvs0-127`, `osc-keysync0` |
| `algorithm_router` | 32 algorithms, Mark I gate (16284), feedback incl. algo 4/6 serial (+2 shift) | core clock (wiring + compares) | per-eval in/out with op pipeline | `alg-05/16/31`, `alg4-fb0/3/7`, `alg6-fb0/3/7`; N08 `broken_feedback` mutation control |
| `mixer` | per-note convert + unclamped ≥20-bit mix (NUM-010 proof) | core clock; accumulator cleared per note-sample | note accumulator in; frame mix out | `ev-sum-15` (sums past per-note rails), `ev-all-sound-off`, `ev-all-notes-off`; `voice_sum_order_swap` mutation control |
| `spi_ctl` | SPI slave, 48-bit frames, skid queue, continuous drain, status word | core clock; pins through 2-flop syncs; pad reset only | `wr_valid`/`wr_addr`/`wr_data`; MISO status at CS_N fall | pin-to-acceptance + drain-order bench (adapted parasynth `verify_ctl` pattern); event traces reproduced in order (`dir-base`, `ev-repeated-note`, `perf-transpose-12`) |
| `i2s_tx` | I2S transmitter, BCLK/LRCLK from counter bits, 1 load/sample | core clock; pad reset | sample-load handshake | decoded-wire vs model bench (adapted sibling pattern); H07/H09 latency measurement |

Integration order follows the dependency structure: `op_core`/`state_banks`
(H01/H02 patterns) → `env_unit`/`pitch_mod_unit`/`algorithm_router` →
`mixer` → `spi_ctl`/`i2s_tx`/`frame_controller` chassis. Each module lands
with its strip-observability control and its N08 subset green before the
next depends on it.

## 6. STALE rules

- **Contract change:** any change to `docs/CONTRACT-CORE-v1.md` or
  `spec/core-contract-v1.json` **after H04–H07 implementation starts**
  requires a new decision record in `docs/decision-records/` and marks the
  affected modules' RTL evidence **STALE** (status vocabulary per AGENTS.md).
  No silent edits; a relaxed budget is never a substitute for a recorded
  ruling.
- **Artifact drift:** drift of any hash in `depends_on` (N08 bundle, numeric
  profile, H01/H02 evidence) fails `tools/h03_budget_check.py` and marks
  every H03-derived budget STALE until re-derived from the new artifacts
  (DR-0007's release STALE rules cascade here).
- **Clock change:** changing the design point or the fallback verdict
  requires a new DR (owner sign-off item). Polyphony (DEC-003) and reference
  budgets are never relaxation knobs; the stop/escalate rule of issue #25
  (architecture change / clock change / explicit polyphony revision request
  to owner) is discharged by this contract because the design point closes.
- **Negative control:** a budget that closes only by removing or shrinking
  worst-burst rows fails the checker (`required_row_ids` + recorded verdicts
  are enforced).

## 7. What this does NOT say

- No RTL exists; no module, integration, or end-to-end RTL result is claimed.
- No synthesis, place-and-route, timing closure, utilization, or fit claim —
  including for the 24.576 MHz design point (ss corner unproven, H10).
- No claim that the H02 area finding (16-note state > quarter slot) is
  resolved; this contract assumes the flop organization and the owner
  architecture decision remains open (H02 §6).
- No original-DX7 fidelity, preset-quality, or musical-usefulness claim;
  Dexed agreement is not original-DX7 fidelity (DEC-001, DEC-024, plan §7).
- The `derived_estimate` unit costs are upper-bound planning numbers, not
  measurements; H04–H06 conformance measures them and the budget tool is
  re-run against measured values.

## 8. Evidence and reproduction

```sh
python3 tools/h03_budget_check.py            # recompute the budget + hash checks
python3 -m unittest tests.test_h03_contract -v
```

| Artifact | Role |
|---|---|
| `spec/core-contract-v1.json` | machine-readable contract (rows, verdicts, queues, modules) |
| `evidence/h01-operator-probe/synth_report.json` | measured 1 clk/eval anchor (sha256-pinned in the JSON) |
| `evidence/h02-storage-probe/storage_synth_report.json` | measured storage/port anchor (sha256-pinned) |
| `reference/release/manifest.json` | frozen conformance oracle binding (bundle sha256-pinned) |
| `docs/decision-records/0008-core-schedule-contract.md` | adoption record (clock-target status Proposed) |

Statuses: budget arithmetic **PASS** (recomputed, worst case); 12.288 MHz
**FAIL** (headline finding, recorded); 24.576 MHz **PASS** on paper;
RTL, timing, fit, CI-without-artifacts: **NOT_RUN / out of scope**.
