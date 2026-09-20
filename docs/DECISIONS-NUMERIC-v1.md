# DECISIONS-NUMERIC-v1 — frozen arithmetic and scheduling profile

- **Profile version:** 1
- **Frozen:** 2026-09-20
- **Issue:** #15 (N01) · epic #2 · planning ID N01
- **Machine-readable companion:** [`spec/numeric-profile-v1.json`](../spec/numeric-profile-v1.json)
  (kept in lockstep by `tests/test_numeric_profile.py` and `tools/check_numeric_profile.py`)
- **Adoption record:** [DR-0003](decision-records/0003-numeric-profile-v1.md)
- **Upstream premise:** [DR-0001](decision-records/0001-product-contract-v1.md) (D00),
  [D01](PHYSICAL-CONSTRAINTS.md), R01 manifest (`reference/manifest.json`,
  Dexed commit `b6cf2e9190e216387ad96f977284ebbef765b447`), R02 oracle identity
  (`reference/oracle-identity.json`, oracle commit `0047c3f51b04f59e82c51e117e16fcb338e05c7e`),
  qualified R04/R05 trace machinery (`reference/evidence/compare/`).
- **Status vocabulary:** identical to D00 section 0 (`Accepted (plan default)`,
  `Accepted (owner-ruled)`, `Proposed (owner may veto)`), plus per-decision
  `error_estimate` and `cost_estimate` fields as required by issue #15's
  negative control.
- **Rev 1.1 (2026-09-20):** pre-merge corrections from the PR #52 review:
  NUM-005's `sr_multiplier` fixed to **15,414,067** (v1 froze 15,405,619 — an
  arithmetic error; the correct value is derived from `env.cc:47-49` and
  test-enforced, never restated as a literal), and the §6 storage estimate
  reclassified runtime vs patch state member-by-member (v1's "exact 32,416
  bits" was a mislabeled subset; the recount is 38,336 replicated runtime bits
  + 444 shared patch bits — bounded finding strengthened).

## 0. Scope, evidence rules, and change control

This record freezes the **arithmetic profile** every later integer model (N02+)
and RTL module is built to: widths, rounding, saturation/wrap, table
representation, envelope/control update timing, phase/sync, output scaling, and
the per-frame schedule budget. It does **not** implement anything (N02+ owns
models), does not claim RTL, synthesis, or fit (H10/H11), and does not claim
musical usefulness (listening records only, plan §7).

**Evidence rules used throughout:**

- Every claim about the pinned reference is backed by `file:line` in the pinned
  Dexed tree (`/Users/joseph/dev/dexed` at the R01 commit; file identities and
  hashes pinned by `reference/manifest.json`) or the pinned oracle tree
  (`reference/oracle-identity.json`). The machine-readable profile carries these
  citations with expected line content, and `tests/test_numeric_profile.py`
  re-verifies both hashes and line content against the pinned tree (guarded
  skip → `NOT_RUN`, never a silent pass, when a clone is absent).
- **Licensing:** the Mark I engine (`Source/EngineMkI.cpp`) is GPL-3.0-or-later
  and is **never copied** into this Apache-2.0 repository
  (`docs/REUSE-RULINGS.md: dexed-enginemki`). msfa files are Apache-2.0 and
  adoptable with provenance + requalification. What this record adopts is
  **numeric semantics as facts** (table sizes, formulas, update cadence,
  constants), re-implemented in this repository with our own table generator
  (the `gen_tables.py` regenerate-and-hash pattern); zero GPL bytes are copied.
- **Error estimates** distinguish:
  - `exact_by_construction` — the frozen model replicates the pinned reference's
    integer operations one-for-one; byte-level agreement is to be demonstrated
    downstream (N02 renders vs R02 oracle), not assumed here.
  - `derived: <number>` — an analytical bound computed and shown in this record.
  - `NO_VERDICT (measure at N02)` — float-gated reference paths (below) whose
    deviation is bounded only by measurement against the oracle.
- **Cost estimates** distinguish `family-measured` (a per-cell/per-bit anchor
  measured in a sibling gf180mcu flow: `gf180-parasynth:docs/area-budget.md`,
  quoted via D01 §4/§5: `dffq_1` 63.66 µm²/flop, replicated state ≈118 µm²/bit
  all-in, multipliers ~130–140 µm²/partial-product-bit (Booth ~90), ROM-as-logic
  ≈1.9–2.2 µm²/stored bit) from `derived` (this record's arithmetic using those
  anchors). No number here is a measurement of *this* core.
- **Change control:** the profile is frozen. Any change requires a new DR in
  `docs/decision-records/` and STALE marking of every dependent: N02–N08 models
  and tests, H03/H07/H08 budgets, R04/R05 comparison budgets, and
  `spec/numeric-profile-v1.json`. No silent edits; a relaxed budget is never a
  substitute for a recorded ruling (AGENTS.md).

## 1. The adoption strategy (read this first)

### NUM-001 — Adopt the pinned oracle's *integer* semantics as the frozen-model contract; regenerate tables; copy no bytes

- **Status:** **Proposed (owner sign-off requested)** — this is the one strategic
  call in the record; everything below is `Accepted (plan default)` or follows
  from it.
- **Context (verified by reading the pinned sources):** the briefing hypothesis
  was "the msfa engine is already integer, so adopting msfa semantics makes
  model-vs-reference budgets ≈ 0". Reading the pinned tree **confirms the
  premise and corrects the conclusion**:
  - The pinned reference engine is Mark I (R01 manifest `engine.engine_type =
    "marki"`; constructor `setEngineType(DEXED_ENGINE_MARKI)`,
    `Source/PluginProcessor.cpp:102`; binding
    `controllers.core = &engineMkI`, `Source/PluginProcessor.cpp:675-686`).
  - **EngineMkI is an integer engine.** Per sample it evaluates
    `mkiSin(int32_t phase, uint16_t env)` — a 10-bit log-sine table plus a
    10-bit exponential table in 16-bit entries, 14-bit envelope gain, int32
    accumulation (`Source/EngineMkI.cpp:124-150`, tables 51-62, generated by
    float formulas at init 80-92). There is no float in its per-sample path.
  - The msfa core (the *Modern* engine, **not** the pinned oracle) is also
    integer: 1024-entry linear-interpolated Q24 sine table with linear
    interpolation (`Source/msfa/sin.h:29-35,43-61`), Q24 exp2 table
    (`Source/msfa/exp2.h:27-45`), and a per-sample
    `((int64_t)y * (int64_t)gain) >> 24` multiply
    (`Source/msfa/fm_op_kernel.cc:62-64`).
  - Therefore the "adopt msfa ⇒ zero budget" claim **fails as stated**: msfa's
    tables and gain composition differ from Mark I's (24-bit interpolated
    multiply vs 14-bit log-domain add; different threshold rule, §NUM-009), so
    an msfa-faithful model has a *nonzero* model-vs-pinned-oracle budget.
  - What **is** true, and is adopted: both engines share one integer *frame* —
    64-sample blocks (`LG_N = 6`, `N = 64`, `Source/msfa/synth.h:29-30`), the
    same per-block envelope cadence (§NUM-005), the same 32-bit phase format
    (§NUM-003), the same 2-element feedback history (§NUM-006), the same linear
    intra-block gain ramp `dgain = (gain2 - gain1 + (N >> 1)) >> LG_N`
    (`Source/EngineMkI.cpp:155`, `Source/msfa/fm_op_kernel.cc:50`), the same
    per-block phase skip `phase += freq << LG_N`
    (`Source/msfa/fm_core.cc:133`, `Source/EngineMkI.cpp:361`), and the same
    output conversion `>>4 → clip ±(1<<24) → >>9 → /0x8000 → float`
    (`Source/PluginProcessor.cpp:261-263`). A frozen integer model can mirror
    the pinned Mark I semantics **exactly**, integer for integer, because the
    reference itself is integer end-to-end in the audio path.
- **Decision:**
  1. The frozen model contract is **`gf180-dx7-integer-v1`**: the pinned
     oracle's Mark I numeric semantics, organized in Dexed/msfa's 64-sample
     block structure, with every decision below.
  2. Numeric constants, table sizes, and formulas are adopted **as facts**;
     tables are **regenerated by this repository's own generator** and pinned by
     SHA-256 of the generated bytes at N02 (provisional float64 regeneration
     and determinism margins: §5 below and the profile's `provisional_tables`).
  3. msfa source files remain `reference-pin; adoptable only with provenance +
     requalification` (R01 manifest roles); EngineMkI remains
     `external-oracle-only; never copied`. If any msfa file is later imported,
     it follows DR-0002 governance.
- **Alternatives considered:**
  - *Fresh float FM model* (rejected): would need float32 semantics in RTL or
    declared per-op rounding budgets against the oracle across all 6 ops × 64
    samples × 32 algorithms; adds a 24×24-class multiplier (~0.075–0.081 mm²
    derived from family-measured 130–140 µm²/pp-bit) plus ~0.28 mm² of
    24-bit interpolated tables (vs ~0.07 mm² for Mark I's two 16-bit tables,
    §6) — and still lands *further* from the pinned oracle than exact integer
    mirroring. Cost: large error budgets + more area; plan §2 row "How to
    choose arithmetic?" already rules this out ("A generic floating-point FM
    rewrite need not be an obligatory first stage") and §4 ("log-sine and
    exponential lookup tables turn amplitude scaling into addition in the log
    domain").
  - *Adopt msfa/Modern semantics instead of Mark I* (rejected as v1 contract;
    kept as documented alternative): error vs pinned oracle = NO_VERDICT until
    measured but structurally nonzero (different sine/gain composition, §NUM-004);
    cost = larger tables + a multiplier (above). Would only become attractive
    with an owner-level decision to re-pin the reference engine to Modern.
- **Error estimate:** `exact_by_construction` for every decision below that
  mirrors a cited line; the explicitly listed float-gated paths (§NUM-008) are
  `derived` ≤ bounds shown there, or `NO_VERDICT (measure at N02)`.
- **Consequence / freeze:** N02+ implement against this profile, not against
  upstream Dexed; drift of the pinned tree is rejected by R01's verifier.
  Changing NUM-001 requires a new DR and STALE-marks every N02+ artifact.

## 2. Frame, timing, and phase

### NUM-002 — Block structure and control cadence: N = 64, control decisions once per block

- **Status:** Accepted (plan default; matches reference exactly).
- **Context:** Dexed renders in 64-sample blocks: `LG_N = 6`, `N = 64`
  (`Source/msfa/synth.h:29-30`); `processBlock` iterates `i += N`
  (`Source/PluginProcessor.cpp:232`); per block: LFO sampled once
  (`Source/PluginProcessor.cpp:244-245`), each live note computed once
  (`Source/PluginProcessor.cpp:256`, `Source/msfa/dx7note.cc:208,295`), each
  operator envelope advanced once per block (`env.h:39-40` "result is
  subsampled for every N samples"; `Source/msfa/dx7note.cc:283`), and the
  oracle writes one trace line per 64-sample block
  (`reference/oracle-identity.json` trace_extension.granularity). The oracle
  render loop is the same 64-block loop (`main.cpp:385`).
- **Decision:** the chip renders 64-sample frames; LFO, pitch envelope, pitch
  modulation, amp modulation, operator-envelope *segment decisions*, and
  frequency lookups update **once per frame**; only the per-sample inner loop
  (phase advance, intra-block gain ramp, sine evaluation, accumulation) runs at
  sample rate.
- **Alternatives:** per-sample control updates — error vs reference = structural
  (different envelope trajectories every segment), cost = 64× control-path work
  per second for zero fidelity gain (it would *diverge* from the reference).
  Rejected.
- **Error estimate:** `exact_by_construction` (mirrors the reference cadence).
- **Cost estimate:** `derived` — control path ≈ 96 envelope steps + 96 frequency
  lookups + 17 LFO/pitch-env steps per frame ≈ low-10³ clocks/frame (§7);
  ≤10% of the 12.288 MHz frame budget.
- **Freeze:** changing N or cadence ⇒ new DR; STALE-mark N02–N08, R02 traces,
  and the schedule table (§7).

### NUM-003 — Phase accumulator: 32-bit wrapping, one cycle = 2³²; increments from a Q24-octave log-frequency LUT

- **Status:** Accepted (plan default; matches reference exactly).
- **Context:** operator phase and per-sample increment are `int32_t`
  (`FmOpParams`, `Source/msfa/fm_op_kernel.h:20-25`); per sample `phase += freq`
  with natural 2³² wraparound (`Source/EngineMkI.cpp:164,180,200,233,238,277,
  282`; `Source/msfa/fm_op_kernel.cc:65,73`); per frame the carried state skips
  a full block `param.phase += param.freq << LG_N`
  (`Source/msfa/fm_core.cc:133`, `Source/EngineMkI.cpp:361`). Frequency format:
  log-frequency in Q24 octaves (A4 base `50857777 = (1<<24)·log₂440 − 69/12·2²⁴`,
  R01 manifest `settings[tuning]`); `Freqlut::lookup` converts Q24 logfreq →
  32-bit per-sample phase increment via a 1025-entry int32 LUT generated at
  48 kHz (`Source/msfa/freqlut.cc:27-42`), linear interpolation, output scaled
  by `y >> (MAX_LOGFREQ_INT - hibits)` (`Source/msfa/freqlut.cc:46-55`).
  Pitch EG and modulation add in log domain *before* the LUT
  (`Source/msfa/dx7note.cc:279-281`).
- **Decision:** 32-bit phase registers, natural wrap (no saturation), one cycle
  = 2³²; 32-bit per-sample increment `freq`; carried state advanced by
  `freq << 6` per frame; log-frequency arithmetic and the 1025-entry Freqlut
  regenerated and byte-pinned at N02.
- **Alternatives:** smaller phase (e.g. 24-bit) — error: truncation of the
  increment's low bits changes low-frequency detune behavior; derived budget:
  a 2⁸ coarser phase gives ≥1.4° phase error growth per cycle at A4 — audible
  drift vs reference in beat-sensitive patches; cost saving ≈ 96×8 bits ≈
  0.09 mm² registers (derived, family-measured per-bit anchor). Rejected —
  exactness is the contract.
- **Error estimate:** `exact_by_construction` for the accumulator; the
  regenerated LUT's entry drift vs the C-generated one is bounded `derived
  ≤ ~2⁻¹³ relative` per §5 (generation-error analysis) ⇒ ≤1 phase-increment LSB
  on rare pitches; to be counted exactly by N02's model-vs-oracle renders.
- **Cost estimate:** `derived` — 96 × 32-bit phase = 3,072 bits of the state
  store (§6); per-eval addressing work is 2 adds (phase+input, phase+freq),
  inside the 2–3 clk/op-eval target (§7).

### NUM-004 — Operator evaluation: Mark I log-domain `mkiSin` (10-bit log-sine + 10-bit sin-exp, 14-bit envelope gain) — multiply-free

- **Status:** Accepted (follows NUM-001; mirrors the pinned oracle exactly).
- **Context:** the pinned oracle's per-sample operator output is
  `Source/EngineMkI.cpp:124-150`:
  1. table index = `phase >> (22 - 10)` — top 10 bits of the 24-bit
     quarter-cycle index (`EngineMkI.cpp:125`);
  2. quarter-wave fold + sign bit via `sinLog()` (`EngineMkI.cpp:64-78`);
  3. log-domain gain add: `expVal = sinLog(...) + env` with 14-bit envelope gain
     (`ENV_BITDEPTH = 14`, `EngineMkI.cpp:52`; `ENV_MAX = 1<<14`, line 62);
  4. anti-log: `result = 4096 + sinExpTable[(expVal & 0x3FF) ^ 0x3FF]`
     (`EngineMkI.cpp:132`), then `result >>= expVal >> 10` (`EngineMkI.cpp:135`);
  5. output `±result << 13`, negative side `(-result - 1) << 13`
     (`EngineMkI.cpp:146-149`).
  Tables: `sinLogTable[1024]`, `sinExpTable[1024]`, both `uint16_t`
  (`EngineMkI.cpp:54-60`), generated at init by
  `round(-1024·log2(sin((0.5+i)/1024 · π/2)))` (`EngineMkI.cpp:83-86`) and
  `round((2^(i/1024) − 1)·4096)` (`EngineMkI.cpp:88-92`). No interpolation —
  the reference itself truncates to the 10-bit index.
- **Decision:** the frozen model evaluates operators with exactly this
  composition and these two 1024×16-bit tables (regenerated + pinned at N02).
  Because gain scaling is a **log-domain addition**, the operator needs **no
  multiplier** — this is both the fidelity decision and the area decision
  (plan §4: "Study that organization before synthesizing 96 independent
  multiplier-based oscillators").
- **Alternatives:**
  - *msfa/Modern operator* (`Sin::lookup` + `(y·gain)>>24`,
    `Source/msfa/fm_op_kernel.cc:62-64`): 1024-entry Q24 table with linear
    interpolation stored as 2048 int32 delta-form (`Source/msfa/sin.h:29-35`,
    `sin.cc:23-57`), Q24 exp2 table 2048 int32 (`Source/msfa/exp2.h:27-45`);
    error vs pinned oracle: `NO_VERDICT (measure at N02)` — structurally
    nonzero (interpolated 24-bit amplitude vs 10-bit log composition; even the
    gain convention differs); cost `derived`: 131,072 table bits ≈ 0.282 mm²
    ROM-as-logic + a 24×24 multiplier ≈ 0.075–0.081 mm² (family-measured
    130–140 µm²/pp-bit; Booth ~90 ⇒ ~0.052 mm²) vs 32,768 table bits ≈
    0.070 mm² and no multiplier for Mark I. Rejected for v1 (NUM-001).
  - *Interpolated Mark I tables* (10.2, 10.3-bit index): error vs oracle:
    nonzero by construction (the oracle truncates); cost: +1 comparison mux +
    second ROM port per lookup, +1–2 clk/eval. Rejected — deviation from the
    reference is the one thing the budget must not buy.
- **Error estimate:** `exact_by_construction` (same tables, same integer ops).
- **Cost estimate:** `derived` with family-measured per-bit anchors: two
  16 Kbit ROMs ≈ 0.070 mm² total (§6); target 2–3 clk/op-eval on a shared
  datapath with both ROM banks read in parallel (§7).

## 3. Envelopes, LFO, pitch

### NUM-005 — Operator envelopes: 4-rate/4-level segments, Q24-log level, once-per-frame stepping, linear intra-frame gain ramp

- **Status:** Accepted (plan default; matches reference exactly).
- **Context:** `Env` keeps `rates_[4]`, `levels_[4]`, `outlevel_`,
  `rate_scaling_`, Q24-doubling `level_` (stored `<<16`), `targetlevel_`,
  `rising_`, `ix_`, `inc_`, `staticcount_` (`Source/msfa/env.h:60-76`;
  `ACCURATE_ENVELOPE` defined, `env.h:25`). One step per frame
  (`env.h:39-40`; called from `dx7note.cc:283` once per note compute):
  rising legs multiply toward target with the 1716 jump-start floor
  (`level_ += (((17 << 24) - level_) >> 24) · inc_`, jump target
  `1716 << 16` — `Source/msfa/env.cc:78-89`), falling legs subtract
  (`level_ -= inc_`, `env.cc:90-96`). Segment targets:
  `actuallevel = (scaleoutlevel(level)>>1 <<6) + outlevel_ − 4256`, clamp `<16 → 16`
  (`env.cc:113-121`, `levellut` `env.cc:28-30`, `scaleoutlevel`
  `env.cc:109-111`). Rates: `qrate = (rate·41)>>6 + rate_scaling`, clamp 63
   (`env.cc:125-127`); step `inc_ = (4 + (qrate&3)) << (2 + 6 + (qrate>>2))`
   scaled by `sr_multiplier = (44100/sample_rate)·2²⁴`
   (`env.cc:147-149`, `env.cc:47-49`; at 48 kHz, `(44100/48000)·2²⁴ =
   15,414,067.2` ⇒ **15,414,067** after `uint32_t` truncation).
  `ACCURATE_ENVELOPE` static-hold counters decrement by N per frame
  (`env.cc:64-71`, `statics[]` table `env.cc:33-44`).
  The 14-bit *gain* applied per sample is derived per frame as
  `gain2 = ENV_MAX − (level_in >> 14)` and ramped linearly per sample with
  `dgain = (gain2 − gain1 + 32) >> 6` (`Source/EngineMkI.cpp:315,155,161`).
  This is a DX exponential-segment envelope with per-sample linear gain
  interpolation — **not** an ADSR (AGENTS.md negative-control list; D00 DEC-015).
- **Decision:** replicate the model above integer-for-integer at 48 kHz
  (sr_multiplier frozen to the derived 48 kHz value
  ⌊(44100.0/48000.0)·2²⁴⌋ = **15,414,067**; v1's 15,405,619 was an arithmetic
  error — corrected on PR #52 review, and the frozen value is checked against
  the formula by test, not restated as a bare literal).
- **Alternatives:** true per-sample envelope integration — error: structural
  divergence from reference trajectories; cost: 64× envelope rate for no
  fidelity gain. *Coarser envelopes (per-2-frames)* — error: every segment
  boundary quantized to 2 frames; visible on fast attacks; saves < 1% of frame
  budget (control path is ≤10% total, §7). Both rejected.
- **Error estimate:** `exact_by_construction`.
- **Cost estimate:** `derived` — 96 envelope steps/frame × ~8–12 clk ≈ 0.8–1.2k
  clk/frame (< 8% of 16,384 at 12.288 MHz); envelope runtime state 96 × 229
  bits (§6).

### NUM-006 — Feedback path: two-element int32 history, `(y0+y) >> (shift+1)`, 8-bit feedback depth, algorithms 4/6 serial expansion

- **Status:** Accepted (plan default; matches reference exactly; D00 DEC-002
  names algorithms 4/6 as directed cases).
- **Context:** `FEEDBACK_BITDEPTH = 8` (`Source/msfa/dx7note.cc:28`);
  `fb_shift_ = feedback != 0 ? 8 − feedback : 16`
  (`Source/msfa/dx7note.cc:197,366`); per sample
  `scaled_fb = (y0 + y) >> (fb_shift + 1)` from the two carried int32
  histories (`Source/EngineMkI.cpp:196`, buffer `fb_buf_[2]`,
  `Source/msfa/dx7note.h:77`), stored back at frame end
  (`EngineMkI.cpp:203-204,242-243,291-292`). Mark I renders the 3-operator
  serial feedback of algorithm 4 and the 2-operator serial feedback of
  algorithm 6 inline (`compute_fb3`/`compute_fb2`,
  `EngineMkI.cpp:247-293,208-244`) with feedback shift `(feedback_shift+2)`
  capped at 16 (`EngineMkI.cpp:330,337,343`), i.e. 4× less feedback than the
  generic path, and advances the skipped operators' phase by
  `freq << LG_N` (`EngineMkI.cpp:331-339`).
- **Decision:** replicate exactly: 2 × 32-bit history per note, arithmetic
  shift `>> (shift+1)`, the 8-bit depth mapping, the algorithm 4/6 inline
  serial forms with `+2` shift, and the phase-skip bookkeeping.
- **Alternatives:** a generic feedback loop (msfa Modern style, single-op
  `compute_fb` only, `Source/msfa/fm_core.cc:114-118`) — error vs oracle:
  nonzero on algorithms 4/6 (different topology *and* shift) — the exact
  algorithms D00 DEC-002 names; cost: fewer special cases (~negligible
  datapath saving). Rejected.
- **Error estimate:** `exact_by_construction`.
- **Cost estimate:** `derived` — 16 notes × 64 bits history = 1,024 bits (§6);
  one add + one shifter shared per eval; within the 2–3 clk/op-eval budget
  (§7).

### NUM-007 — LFO: six waveforms, one `getsample()` + one `getdelay()` per frame, sine via the msfa sine table

- **Status:** Accepted (plan default; matches reference exactly).
- **Context:** waveforms triangle/saw-down/saw-up/square/sine/S&H
  (`Source/msfa/lfo.cc:54-74`); 32-bit phase accumulator with
  `unit_ = (int32)(N·25190424/sample_rate + 0.5)` (⇒ 33,587 at 48 kHz;
  `lfo.cc:26-29`), rate shaping `lfo.cc:31-49`; delay ramp
  `lfo.cc:78-90`; keydown sync resets phase to `(1U<<31)−1` and delay state
  (`lfo.cc:92-97`). Dexed samples it **once per 64-block**
  (`Source/PluginProcessor.cpp:244-245`), and the LFO's sine branch uses the
  msfa `Sin::lookup` table (`lfo.cc:67`) — so the frozen model carries that
  one table *in addition to* the two Mark I tables.
- **Decision:** replicate exactly at 48 kHz (`unit_` frozen to 33,587;
  `delta_`, delay increments all in exact integer arithmetic).
- **Alternatives:** per-sample LFO (error: structural divergence — the
  reference LFO genuinely steps per frame; cost: 64× LFO rate). Rejected.
- **Error estimate:** `exact_by_construction` (sine branch = regenerated
  msfa table, pinned at N02).
- **Cost estimate:** `derived` — negligible: ~50–100 clk/frame; 32-bit
  phase + 32-bit delay state once per chip (not per note).

### NUM-008 — Pitch EG and per-frame pitch path; the reference's float-gated spots are declared, bounded, and measured at N02

- **Status:** Accepted for the integer contract; float-gated deviations
  recorded with derived bounds (NO_VERDICT until N02 measures them).
- **Context:** pitch EG steps once per frame with
  `unit_ = N·2²⁴/(21.3·sample_rate) + 0.5` (⇒ 1050 at 48 kHz,
  `Source/msfa/pitchenv.cc:22-24`), `pitchenv_rate`/`pitchenv_tab` tables and
  additive `level_ ± inc_` legs (`pitchenv.cc:55-89`); applied in log domain
  (`Source/msfa/dx7note.cc:217`). Modulation inputs are integer per frame:
  pitch mod `dx7note.cc:210-217`, amp mod `dx7note.cc:259-268`, both Q24-ish
  shifts. Three reference spots use floating point **outside the per-sample
  path** (per frame or per event):
  1. pitch-bend scaling `(float)(pb << 11) × range / 12.0`
     (`Source/msfa/dx7note.cc:225-227`, MPE variant `dx7note.cc:237`);
  2. amp-mod sensitivity `exp(((float)sensamp)/262144·0.07 + 12.2)`
     (`Source/msfa/dx7note.cc:288`) — per frame, only when amp-mod
     sensitivity ≠ 0;
  3. note-setup pitch math (`osc_freq` detune/fine, `dx7note.cc:46-52`;
     LFO `unit_` init; `Freqlut::init`; envelope `sr_multiplier`) — per event,
     not per sample.
- **Decision:** the frozen model is integer everywhere per-sample/per-frame
  except these three spots, where the model reproduces the reference's float
  formulas **in the model** (Python float64/C float32 semantics documented
  case-by-case at N02) and RTL consumes their frozen integer *results* (bend
  scale factors, amp-mod LUT) committed per event/frame by the control path.
- **Alternatives:** re-derive these in fixed point — error `derived`: for (1),
  float32 relative error ≤ 2⁻²⁴ on |pb| ≤ 2²⁴ ⇒ ≤ ±1 logfreq count before the
  Freqlut's own 14-bit interpolation granularity; dF/F ≤ ln2·2⁻²² ≈ 1.7·10⁻⁷
  (≈ 0.0001 Hz at 440 Hz — inaudible, but *countable* in byte comparisons);
  for (2), a fixed-point exp re-derivation risks per-D count differences
  compounding through `2^x` — bounded only by measurement. Rejected for exact
  paths; chosen where integer results are committed once per event/frame.
- **Error estimate:** (1) `derived ≤ ±1 logfreq count (≤1.7·10⁻⁷ relative)`
  when a bend is active; (2) `NO_VERDICT (measure at N02)` — patch-gated
  (only amp-mod-sensitive patches); (3) `derived ≤ ±2 lut entries` at the top
  of the Freqlut from generation-time double accumulation (§5) ⇒ ≤1
  phase-increment LSB on rare pitches; N02 counts actual divergences.
- **Cost estimate:** `derived` — pitch EG ≈ 16 steps/frame (negligible);
  event-time float math runs on the host/control side, off the audio frame
  budget (D00 DEC-010 boundary).

## 4. Routing, gating, output

### NUM-009 — Algorithm routing, level gating, and the Mark I threshold rule

- **Status:** Accepted (plan default; matches reference exactly).
- **Context:** routing is a 32-entry table of 6 packed op descriptors
  (in-bus/out-bus/add/fb flags — `Source/msfa/fm_core.cc:29-62` and the
  equivalent table in `Source/EngineMkI.h`; decode at
  `EngineMkI.cpp:307-313`). Mark I gating: per frame,
  `gain1 = gain_out == 0 ? ENV_MAX−1 : gain_out`,
  `gain2 = ENV_MAX − (level_in >> 14)` (`EngineMkI.cpp:314-315`), and the
  operator renders only when `gain1 <= kLevelThresh || gain2 <= kLevelThresh`
  with `kLevelThresh = ENV_MAX−100 = 16284` (`EngineMkI.cpp:296,318`) —
  i.e. near-max attenuation gates the op off. (The msfa Modern engine's
  rule differs in kind and constant: `Exp2::lookup`-domain gain, threshold
  1120, `>=` test — `Source/msfa/fm_core.cc:93-132`.) `has_contents`
  bookkeeping suppresses adds onto empty buses (`EngineMkI.cpp:298,320-360`).
- **Decision:** replicate the Mark I table, gain chain, threshold constant
  16284, `<=` comparison, zero-gain substitution `ENV_MAX−1`, and
  `has_contents` behavior exactly.
- **Alternatives:** msfa Modern gating — error vs oracle: nonzero (different
  threshold/domain: ops near silence gate at different frames); cost:
  needs the exp2 table + multiplier anyway (NUM-004). Rejected.
- **Error estimate:** `exact_by_construction`.
- **Cost estimate:** `derived` — routing is wiring (no per-eval cost); the
  gate check is 2 compares inside the op-eval budget (§7).

### NUM-010 — Output conversion and mix: `>>4` → clip ±2²⁴ → `>>9` → 16-bit; integer mix is *bit-exact* vs the reference's float mix; comparison format f32le

- **Status:** Accepted (plan default; mirrors reference; D00 DEC-008/DEC-012
  dry-mono transport).
- **Context:** per note per sample the reference does
  `val = audiobuf >> 4`; `clip_val = val < −(1<<24) ? 0x8000 : val >= (1<<24)
  ? 0x7fff : val >> 9`; `f = clip_val / 0x8000`, clamp to ±1, and
  **float-accumulates per-note f into the mix**
  (`Source/PluginProcessor.cpp:258-268`; oracle mirror
  `main.cpp:455-465`), writing raw float32 (f32le) PCM
  (`main.cpp:481-482`; R01 manifest `render_scaling`). Note the frozen quirk:
  negative overflow maps to `0x8000 = +32768` ⇒ f = **+1.0**.
- **Decision:**
  1. The chip's per-note output is `clip_val` (17 values: −32768…32767 and
     the quirk value +32768 on negative overflow, all representable as
     int32 by the same comparisons).
  2. The comparison/product mix accumulates `clip_val` in a ≥20-bit integer
     sum. **Proof of exactness (derived):** each f is an integer multiple of
     2⁻¹⁵ with |f| ≤ 1 (exactly representable in float32); the frame has ≤16
     live notes so |Σf| ≤ 16, i.e. Σf is an integer multiple of 2⁻¹⁵ of
     magnitude < 2²⁴·2⁻¹⁵ ⇒ every partial sum is exactly representable in
     float32 (≤ 20 mantissa bits), so the reference's sequential float sum
     is exact and equals the integer sum converted by ×2⁻¹⁵. No −0.0 can
     arise (no term is −0; x + (−x) = +0.0 in round-to-nearest). The model
     therefore mixes in integers and remains **bit-exact vs the reference
     float path** (`exact_by_construction`), to be confirmed by N02
     byte-difference runs.
  3. Comparison renders stay **dry f32le** as pinned (R01; DEC-012). Product
     I2S framing (16-bit MSB alignment, optional duplication) is H08's
     contract; this profile fixes only that the chip hands off `clip_val`-
     domain samples.
- **Alternatives:** float mixing in the core — cost: float units in ASIC
  standard cells for zero fidelity gain (the mix is provably exact in
  integers); *wider* per-note output (skip `>>4/>>9`) — error: diverges from
  the pinned render_scaling (structural); saves nothing. Both rejected.
- **Error estimate:** `exact_by_construction` (proof above; N02 confirms
  byte-identity on real renders).
- **Cost estimate:** `derived` — 1,024 conversions/frame (16 notes × 64) ×
  2–4 clk ≈ 2–4k clk, pipelined alongside the op loop (§7); zero multipliers.

### NUM-011 — Event application: commit at 64-sample frame boundaries (the reference's own policy), queue/backpressure per DEC-014

- **Status:** Accepted for the commit-cadence value; **Proposed (owner may
  veto)** for reading DEC-014's "frame" as the 64-sample core frame.
- **Context:** D00 DEC-014 freezes *next-frame-boundary* application, worst
  case one frame, bounded queue, explicit overflow rejection
  (`spec/contract-v1.json: event_application = "next_frame_boundary"`). The
  reference's actual policy: Dexed drains events at each 64-block boundary
  (`Source/PluginProcessor.cpp:236-238`), and the pinned oracle applies
  *every event with position inside a block before rendering that block*
  (`main.cpp:385-407`; policy string in `reference/oracle-identity.json`
  event_to_block_policy; traces carry the applied-block log).
- **Decision:** the core commits events at **64-sample frame boundaries**
  (1.333 ms at 48 kHz): an event received during frame *N* is applied before
  the first sample of frame *N+1*; worst-case core event-to-audio latency is
  one frame = 64 samples = 1.333 ms plus declared transport latency. This is
  exactly the pinned oracle's policy, so comparison event alignment is
  trivial. DEC-014's bounded-queue and explicit-rejection/backpressure rules,
  its never-silent-drop rule, and its transport-budget split (H03) are
  unchanged. **Flagged for owner sign-off:** DEC-014's parenthetical
  "(20.833 µs at 48 kHz)" read "frame" as one sample; this record fixes the
  core's frame as the 64-sample render block. If the owner rules that
  events must commit per sample, N02 adds a per-sample event shifter (cost
  `derived`: one small event FIFO + per-sample commit logic; no op-loop
  impact) — the comparison policy would then deviate from the oracle by up to
  63 samples and comparisons must shift-align events explicitly.
- **Alternatives:** per-sample commit — error: none audible, but breaks
  trace-for-trace alignment with every committed reference artifact (R02/R05)
  and costs the shifter above. Rejected for v1 (recorded, revisit on owner
  ruling).
- **Error estimate:** `exact_by_construction` vs the oracle's event policy.
- **Cost estimate:** `derived` — one bounded event queue + frame-boundary
  commit; queue depth is H03's number (D00 DEC-014), never guessed here.

### NUM-012 — Note tails (DEC-021 floor): exact release-to-floor predicate, 64-frame hold, output-neutral reclamation

- **Status:** Accepted (plan default; DEC-021 delegates the numbers to this
  record; `spec/contract-v1.json: silence_floor_owner = "N01"`).
- **Context:** the reference never reclaims voices (Dexed keeps
  `MAX_ACTIVE_NOTES` slots; notes stay live until stolen), but envelope-gated
  silence is exact: with the release target at the clamp floor,
  `level_ = 16 << 16` (`Source/msfa/env.cc:118-119`), the frame gain becomes
  `gain2 = ENV_MAX − 16 = 16368 > kLevelThresh = 16284`, and once `gain_out`
  follows, the Mark I gate skips the operator
  (`EngineMkI.cpp:314-318`) — the voice contributes exactly nothing to the
  mix. Envelopes parked in final release never advance again
  (`env.cc:74`: `ix_==3 && down_` → no step). D00 DEC-021 forbids arbitrary
  fixed tails and requires a documented floor + hold.
- **Decision:** a note voice is **reclaimable** iff, for all six operators,
  `down_ = true ∧ ix_ = 3 ∧ level_ = 16<<16` (the exact clamp-floor state)
  — held continuously for **64 consecutive frames** (85.33 ms) — and is
  reclaimed at the next frame boundary. Patches with louder release targets
  never satisfy the predicate (they keep sounding until stolen per D00
  DEC-016/DEC-017) — a valid patch, not an error. Retrigger (DEC-015) resets
  envelopes and breaks the hold. The reclamation is a chip-side state change
  only; it is **sample-exact output-neutral** vs the reference render (the
  gated-skip argument above), so R04/R05 comparisons need no allowance for it.
- **Alternatives:** a float output-magnitude threshold (|mix| < ε for k
  frames) — error: not reference-derivable, and per-note output in the mix
  is not directly observable; cost: same. An N-frames-only rule (no level
  predicate) — violates DEC-021's no-arbitrary-tails rule. Both rejected.
- **Error estimate:** `exact_by_construction` (integer predicate; audio
  provably identical).
- **Cost estimate:** `derived` — 3 flags + 3-bit ix + level comparison per
  operator, one 6-input AND + 7-bit hold counter per note ≈ negligible.

## 5. Table regeneration policy and determinism (applies to NUM-003/004/007)

The reference computes all tables at runtime from float formulas. This
repository's generator (N02) recomputes them with the exact operation order of
the cited lines and pins the generated bytes by SHA-256. Determinism of the
16-bit tables is established **by argument and checked by test**:

- `sinLogTable[i] = round(−1024·log2(float32(sin((0.5+i)/1024·π/2))))`
  (`EngineMkI.cpp:84-85`): the quantity −1024·log2(x) stays ≥ 1.8·10⁻⁴ away
  from every half-integer rounding boundary across all 1024 entries
  (computed for this record; asserted by
  `tests/test_numeric_profile.py::test_provisional_tables`), while any
  conforming double libm agrees to ≲10⁻¹² on these arguments ⇒ the rounded
  16-bit entries are platform-independent.
- `sinExpTable[i] = round((2^(i/1024) − 1)·4096)` (`EngineMkI.cpp:89-92`):
  minimum boundary distance 3.7·10⁻⁵ ⇒ same conclusion.
- Freqlut's 1025-entry LUT (`freqlut.cc:35-42`) accumulates 1024 doublings in
  double: cumulative error up to ~10⁻¹³ relative ≈ a few LSBs at the top
  entries ⇒ **not** claimed byte-stable; the frozen model pins *its own*
  generated bytes and declares the ≤1 phase-increment-LSB budget
  (§NUM-003/NUM-008), with actual divergence counted by N02.

Provisional float64 regenerations (non-normative until N02's requalification;
recorded in `spec/numeric-profile-v1.json: provisional_tables`):
`sinLogTable` sha256 `990c19e90732efe712a19ba4272f97c7c9d884ac0e4e19450a1067165d8aa8a8`,
`sinExpTable` sha256 `e515a71ae736d92dcb3fd36973dea486c96d1521f1a0bab1f917be2c4ec07794`
(1024 × uint16 LE each; first entry 10597, last 0; first 0, last 4090).

## 6. Storage estimate for the 16-note state (issue #15 negative control)

**Rev 1.1 (judge findings on PR #52):** v1's table claimed an "exact 16-note
state = 32,416 bits" but was a **mislabeled subset**: it omitted
`FmOpParams.freq` and `level_in` and gave no classification of which members
are per-note state at all. This section counts member-by-member, classifying
every member as **runtime state** (mutable per note/op while audio renders —
must exist 16×) or **patch params** (loaded per patch, identical across all
voices of the single-timbre product — may live in one shared patch register
file). The claim is relabeled accordingly.

**Classification policy.** Runtime = mutable per note or per op-of-a-note as
audio renders. Patch = written at patch-load/edit events only and identical
for all 16 voices — the reference copies the *same* patch bytes into every
note (`dx7note.cc:163-200`), so shared registers committed at D00 event time
are behavior-identical under the one-timbre premise. Widths: pinned reference
type width (32 bits for `int`/`int32_t` accumulators, counters, and the
derived env params); content width for small bounded fields, stated per
field. Trimming below these widths remains a later, separately-budgeted
decision.

### Per-operator runtime state (× 96) — `FmOpParams` (`fm_op_kernel.h:20-25`), `Env` (`env.h:60-76`), per-note derived env params (`dx7note.cc:163-179`)

| Member | Bits | Class | Why |
|---|---:|---|---|
| `FmOpParams.phase` | 32 | runtime | per-sample accumulator, advances per note |
| `FmOpParams.freq` | 32 | runtime | per-note per-frame increment, carried across the frame boundary (`fm_core.cc:133`); **omitted in v1** |
| `FmOpParams.gain_out` | 32 | runtime | previous frame env gain → next frame's `gain1` (`EngineMkI.cpp:314`) |
| `FmOpParams.level_in` | 32 | runtime | current-frame env gain into the op render (`EngineMkI.cpp:315`); **omitted in v1** |
| `Env.level_` | 32 | runtime | Q24-doubling envelope level (`env.h:67`) |
| `Env.targetlevel_` | 32 | runtime | segment target (`env.cc:121`) |
| `Env.inc_` | 32 | runtime | `sr_multiplier`-scaled segment step (`env.cc:147-149`) |
| `Env.staticcount_` | 32 | runtime | `ACCURATE_ENVELOPE` hold counter (`env.h:73`, `env.cc:137-141`) |
| effective `outlevel_` | 32 | runtime (per-note derived) | `scaleoutlevel` + key-level scaling (midinote) + velocity scaling (`dx7note.cc:169-177`) — differs per note even under one timbre |
| effective `rate_scaling_` | 32 | runtime (per-note derived) | keyboard rate scaling `ScaleRate(midinote, sens)` (`dx7note.cc:178`) |
| `Env.ix_` | 3 | runtime | segment index 0..4 |
| `Env.rising_` | 1 | runtime | direction flag |
| `Env.down_` | 1 | runtime | key-state flag |
| **per op** | **325** | | |

### Per-note runtime state (× 16) — `Dx7Note` (`dx7note.h:72-96`), `PitchEnv` (`pitchenv.h:34-45`), `ProcessorVoice` (`PluginProcessor.h:40-53`)

| Member | Bits | Class | Why |
|---|---:|---|---|
| `basepitch_[6]` | 192 | runtime | per-note pitch (midinote-dependent, `dx7note.cc:185-187`) |
| `fb_buf_[2]` | 64 | runtime | feedback history (NUM-006) |
| pitch EG `level_`/`targetlevel_`/`inc_` | 96 | runtime | Q24-octave pitch envelope state |
| pitch EG `ix_`/`rising_`/`down_` | 5 | runtime | 3 + 1 + 1 |
| `noteLogFreq` | 32 | runtime | note-on log frequency incl. tuning (`dx7note.cc:154-161`) |
| `playingMidiNote` | 7 | runtime | 0..127 |
| `midiChannel` | 4 | runtime | 0..15 |
| `mpePitchBend`/`mpePressure`/`mpeTimbre` | 28 | runtime | 14 + 7 + 7 (`dx7note.h:68-70`) |
| keydown/sustained/live flags | 3 | runtime | voice management (`PluginProcessor.h:44-46`) |
| steal rotation/age | 15 | runtime | allocator bookkeeping estimate (v1's "18" = 3 flags + 15, split explicitly here) |
| **per note** | **446** | | |

### Patch-shared state (one copy — **excluded from replication**, justified)

| Member | Bits | Why shared |
|---|---:|---|
| `Env.rates_[4]` ×6 ops | 168 | raw 0..99 rates (7 bits each, 4 × 6 ops); the same patch bytes go into every note (`dx7note.cc:165-167`) — per-note divergence enters only via the derived params counted above |
| `Env.levels_[4]` ×6 ops | 168 | same |
| pitch EG `rates_[4]`/`levels_[4]` | 56 | `dx7note.cc:190-194` |
| `opMode[6]` | 6 | `patch[off+17]` (`dx7note.cc:181`) |
| `ampmodsens_[6]` | 12 | `ampmodsenstab[patch[off+14]&3]` (`dx7note.cc:188`) |
| `algorithm_` | 5 | `patch[134]` |
| feedback depth → `fb_shift_` | 5 | derived once per patch (`dx7note.cc:196-197`) |
| `pitchmoddepth_`/`pitchmodsens_`/`ampmoddepth_` | 24 | derived from patch bytes once (`dx7note.cc:198-200`) |
| **shared total** | **444** | |

**Excluded as not-state:** `currentPatch` (pointer into the shared patch
buffer); `tuning_state_`/`mtsFreq`/`mtsClient` (single fixed tuning in the
pinned configuration, R01 `settings[tuning]`; MTS absent ⇒ `mtsFreq = 0`,
`dx7note.cc:154-161`); raw velocity (consumed once at note-on into the
effective outlevel, `dx7note.cc:176`); `VoiceStatus` (`dx7note.h:34-38`, UI
snapshot only).

**Claim (relabeled honestly):** core replicated runtime state =
96 × 325 + 16 × 446 = 31,200 + 7,136 = **38,336 bits ≈ 4.68 KiB**
(recomputed by test from the classification tables above); static patch
params excluded from replication (444 shared bits, justified
member-by-member). Cost lines:

- All-register: 38,336 bits × **118 µm²/bit** (family-measured replicated-state
  anchor, D01 §5) = **4.52 mm² derived** (4.58 mm² including the shared patch
  registers) — **2.70× the 1.6734 mm² quarter-slot core** (confirmed die
  geometry, D01 §1). *Bounded finding:* the 16-note runtime state cannot be
  all-DFF in a quarter slot — **strengthened** vs v1's 3.83 mm², which was a
  subset.
- SRAM tiling: ⌈4,792 B / 512 B⌉ = 10 foundry macros (D01 §5; ceiling
  512 B/macro) = 10 × 0.2094 mm² = **2.09 mm² derived LEF footprint** (the
  55.5 B of shared patch bytes do not change the macro count);
  bank-select/mux cost **not modelled** (D01 records this gap) and the macros
  have **no Liberty timing views** (D01 §5, confirmed-incomplete) — usable
  for area projection only.
- Tables (Mark I): 2 × 16,384 bits = 32,768 bits × **1.9–2.2 µm²/bit**
  (family-measured ROM-as-logic anchor, D01 §5) = **0.062–0.072 mm² derived**.
  Modern-engine comparison (NUM-004): 131,072 bits ≈ 0.25–0.29 mm² **plus**
  a ≥24×24 multiplier ≈ 0.075–0.081 mm² (family-measured 130–140 µm²/pp-bit;
  Booth ~90 ⇒ ~0.052 mm²) — the multiply-free Mark I core is the smaller and
  the reference-faithful choice.

H02 owns the register-vs-SRAM-hybrid decision against these anchors; nothing
here asserts slot fit (D00 DEC-013, D01 §9).

## 7. The 96-operator schedule, computed carefully (issue #15 headline tension)

All arithmetic recomputed independently by `tests/test_numeric_profile.py`.
Time bases first, to kill the confusion in earlier notes (D01 §2 divides per
**sample**; earlier drafts divided per **block** — both are shown):

```
sample rate        = 48,000 samples/s          (DEC-009)
frames (64-sample) = 48,000 / 64 = 750 frames/s
op-evals/s         = 16 notes × 6 ops × 48,000 = 4,608,000   (= 96 per sample)
op-evals/frame     = 96 × 64 = 6,144
```

| Clock | clk/sample | clk/frame | op-eval floor (6,144/frame) | all-in clk/op-eval | op-loop-only min clock |
|---|---:|---:|---:|---:|---:|
| 12.288 MHz | 256 | 16,384 | **6,144/16,384 = 37.5%** | **2.667** | 6,144×750 = **4.608 MHz** |
| 24.576 MHz | 512 | 32,768 | **18.75%** | **5.333** | 4.608 MHz |

(The "213 clocks/frame is impossible" line from early notes divided 12.288 MHz
by a block rate 64× too high; the correct frame rate is 750/s ⇒ 16,384
clocks/frame. The correct *sample*-frame view is D01 §2's 256/512 clk with 96
evals/sample ⇒ 2.67/5.33 clk per eval. Both views agree below.)

**Confronting the floor honestly:** a time-multiplexed operator needs ≥1 clock
per op-eval even with a fully shared datapath, so the absolute minimum clock
for the op loop alone is 4.608 MHz — 12.288 MHz is 2.67× that floor, 24.576 MHz
is 5.33×. The real question is the all-in budget:

| Consumer (derived, order-of-magnitude) | clk/frame @12.288 | share of 16,384 |
|---|---:|---:|
| Operator loop at 2 clk/eval (dual-bank ROM, shared add/shift) | 12,288 | 75% |
| Operator loop at 3 clk/eval | 18,432 | **112% — over budget** |
| Envelopes + Freqlut + pitch/LFO (96+96+17 steps) | ~1,300–1,600 | ~9% |
| Output conversion + mix (1,024 × 2–4 clk, pipelined) | ~2,000–4,000 | ~12–24% |
| I2S/DAC, event queue, pipeline margin | ~800+ | ~5% |

**Resolution of the 12.288-vs-24.576 tension:** at 12.288 MHz the frame closes
only if the shared operator core sustains **≤2.67 clk/op-eval all-in** —
achievable on paper with the multiply-free Mark I core (§NUM-004: two parallel
16-bit ROM reads + adds/shifts, 1,024-entry tables need no interpolation pass)
but with essentially no margin. At 24.576 MHz the same core at 4–5 clk/eval
uses ~55–70% of the frame with real margin for memory and control. **Design
point: 24.576 MHz; keep 12.288 MHz as the fallback only if the operator core
measures ≤2 clk/eval.** Calibration: the sibling monosynth spends ~154/256
clk/sample on one paraphonic voice + drums (issue #15 prior art; D01 §2) — a
different workload, quoted as the family's realized-utilization datapoint, not
a transferable budget. D01 §2 records that 24.576 MHz ss-corner closure is
**unproven** (sibling implies 14.0 MHz at ss on its design): H01/H10 must
measure both clocks before any fit/timing claim. If neither clock closes with
the full 16-note schedule, the stop rule is issue #15's: present architecture
alternatives to the owner — polyphony (DEC-003) and reference budgets are not
relaxation knobs.

## 8. Decision index

| ID | Decision | Status | Error estimate | Cost estimate |
|---|---|---|---|---|
| NUM-001 | Adopt pinned-oracle integer semantics; regenerate tables; copy no bytes | **Proposed (owner sign-off)** | exact_by_construction per §2–4 | §6/§7 |
| NUM-002 | N=64 frames; per-frame control cadence | Accepted | exact_by_construction | ≤10% frame @12.288 (derived) |
| NUM-003 | 32-bit phase, 2³²/cycle; Q24 logfreq + 1025-entry Freqlut | Accepted | exact; lut ≤1 incr LSB (derived) | 3,072 bits phase (derived) |
| NUM-004 | Mark I mkiSin: 10-bit log-sin + 10-bit sin-exp, 14-bit env, multiply-free | Accepted | exact_by_construction | 0.070 mm² tables, 2–3 clk/eval (derived) |
| NUM-005 | DX envelope: 4×4, Q24-log level, per-frame step, linear intra-frame gain | Accepted | exact_by_construction | 96×229 bits env runtime state; <8% frame (derived) |
| NUM-006 | Feedback: 2×int32 history, >> (shift+1), 8-bit depth, algo 4/6 serial (+2 shift) | Accepted | exact_by_construction | 1,024 bits (derived) |
| NUM-007 | LFO: 6 waveforms, per-frame sample, sine via msfa table (unit 33,587) | Accepted | exact_by_construction | negligible (derived) |
| NUM-008 | Pitch EG (unit 1050) + declared float-gated spots with bounds | Accepted / bounds NO_VERDICT→N02 | ≤1 logfreq count (bend); others per §3 | host-side event-time (derived) |
| NUM-009 | Routing + Mark I gate (thresh 16284, `<=`, ENV_MAX−1 substitution) | Accepted | exact_by_construction | 2 compares/eval (derived) |
| NUM-010 | >>4 → clip ±2²⁴ → >>9 → 16-bit; integer mix bit-exact vs float mix; f32le dry | Accepted | exact_by_construction (proof §4) | 2–4k clk/frame (derived) |
| NUM-011 | Events commit at 64-sample frame boundaries (DEC-014 queue rules unchanged) | Accepted / **Proposed (owner may veto)** on frame reading | exact vs oracle policy | event FIFO (H03 depth) |
| NUM-012 | Silence floor: 6-op release-to-16<<16 predicate, 64-frame hold, neutral reclaim | Accepted | exact_by_construction | negligible (derived) |
