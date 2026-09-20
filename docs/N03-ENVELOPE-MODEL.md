# N03 — Integer operator-envelope model

- **Issue:** [#17](https://github.com/2AMLogic/gf180-dx7/issues/17) · epic #2 ·
  planning ID N03
- **Contract:** `gf180-dx7-integer-v1`
  ([spec/numeric-profile-v1.json](../spec/numeric-profile-v1.json), NUM-005)
- **Model:** [src/gf180_dx7/model/envelope.py](../src/gf180_dx7/model/envelope.py)
  · **Tests:**
  [tests/test_envelope.py](../tests/test_envelope.py) · **Evidence:**
  [reference/evidence/n03-envelope/directed-results.json](../reference/evidence/n03-envelope/directed-results.json)
- **Licensing:** semantic facts only, re-implemented from scratch; zero GPL
  bytes (`Source/msfa/env.*` and `dx7note.cc` are Apache-2.0-msfa material
  used as the semantic specification; `Source/EngineMkI.cpp` facts are cited
  read-only, `external-oracle-only`, docs/REUSE-RULINGS.md). The executable
  comparison target is the external pinned oracle (R02). All claims below are
  **software-reference agreement only** — not original-DX7 fidelity, not
  musical usefulness (plan section 7, DEC-001).

## 1. Semantics as facts (pinned tree, R01)

Pinned Dexed tree: `/Users/joseph/dev/dexed` @
`b6cf2e9190e216387ad96f977284ebbef765b447` (verified clean at N03 time);
oracle `0047c3f51b04f59e82c51e117e16fcb338e05c7e`, binary sha256
`6e17a540…ed42` (re-hashed before every render by the suite).

### 1.1 State (N01 §6 classification mirrored member-for-member)

`Env` members (`Source/msfa/env.h:60-76`): `rates_[4]`, `levels_[4]`,
`outlevel_`, `rate_scaling_`, `level_` (Q24 doublings, stored `<<16`,
env.h:64-67), `targetlevel_`, `rising_`, `ix_`, `inc_`,
`staticcount_` (`ACCURATE_ENVELOPE` defined, env.h:25), `down_`. N01 §6
classifies `level_`, `targetlevel_`, `inc_`, `staticcount_` as 32-bit
per-operator runtime state, `ix_`/`rising_`/`down_` as 3+1+1 bits, and the
**effective** `outlevel_` / `rate_scaling_` as per-note DERIVED runtime
state (they differ per note even under one timbre). The model class carries
these names verbatim.

### 1.2 Per-note derivation at note-on (`Source/msfa/dx7note.cc:163-179`)

Per operator: `rates[i] = patch[off+i]`, `levels[i] = patch[off+4+i]`;
`outlevel = Env::scaleoutlevel(patch[off+16])` (env.cc:109-111:
`>= 20 ? 28+ol : levellut[ol]`, levellut env.cc:28-30) `+ ScaleLevel(...)`
(dx7note.cc:118-126, exp_scale_data dx7note.cc:96-99, ScaleCurve
dx7note.cc:101-116: curves 0/3 linear, 1/2 exponential, curve < 2 negated),
clamp 127, `<< 5`, `+ ScaleVelocity(velocity, patch[off+15])`
(dx7note.cc:66-80: `velocity_data[vel>>1] − 239`,
`((sens·v + 7) >> 3) << 4`), clamp 0; `rate_scaling = ScaleRate(midinote,
patch[off+13])` (dx7note.cc:82-94: `x = min(31, max(0, midinote/3 − 7))`,
`(sens·x) >> 3`).

### 1.3 Segment machine (`Source/msfa/env.cc:63-151`)

One step per frame (see 1.5). In `advance(newix)` (env.cc:113-151), for
`ix_ < 4`:

- target: `actuallevel = (scaleoutlevel(levels_[ix_]) >> 1 << 6) + outlevel_
  − 4256`, clamp `< 16 → 16` (env.cc:117-119), `targetlevel_ = actuallevel
  << 16` (env.cc:121);
- direction: `rising_ = targetlevel_ > level_` (env.cc:122);
- rate: `qrate = (rates_[ix_]·41) >> 6 + rate_scaling_`, clamp ≤ 63 (env.cc:125-127,
  no lower clamp is applied);
- `ACCURATE_ENVELOPE` static hold (env.cc:129-145): if `targetlevel_ ==
  level_` or (`ix_ == 0` and `levels_[0] == 0`), then
  `staticrate = min(rates_[ix_] + rate_scaling_, 99)`,
  `staticcount_ = statics[staticrate]` for `staticrate < 77` else
  `20·(99 − staticrate)` (statics table env.cc:33-44, 77 entries),
  `÷ 20` when `staticrate < 77` and (`ix_ == 0` and level 0), then
  `staticcount_ = (staticcount_·sr_multiplier) >> 24`; else 0;
- step: `inc_ = (4 + (qrate & 3)) << (2 + LG_N + (qrate >> 2))`, then
  `inc_ = (inc_·sr_multiplier) >> 24` (env.cc:147-149), both via int64.

`getsample()` (env.cc:63-100), once per frame:

1. if `staticcount_`: `staticcount_ -= N`; on `≤ 0` → 0 and
   `advance(ix_ + 1)`;
2. if `ix_ < 3 || (ix_ < 4 && !down_)`: hold while `staticcount_`;
   rising: jump-start floor `level_ < 1716 << 16 → 1716 << 16` (env.cc:79-82),
   then `level_ += (((17 << 24) − level_) >> 24) · inc_` (env.cc:83),
   clamp to target and `advance(ix_+1)` on `≥` (env.cc:85-88); falling:
   `level_ -= inc_`, clamp on `≤` (env.cc:90-95);
3. return `level_`.

Key transitions (env.cc:102-107): `keydown(d)` with `down_ != d` sets
`down_ = d` and `advance(d ? 0 : 3)` — attack/release start **from the
current level** (the model exposes this as the retrigger path; the
polyphonic voice allocator instead re-inits each note-on via
`Dx7Note::init` → `Env::init`, level_ = 0, dx7note.cc:147-179/env.cc:51-61,
so oracle-rendered retriggers always restart from silence — the Env-level
retrigger is what Dexed mono mode exercises via `transfer`,
dx7note.cc:385-392, env.cc:175-191, and it is unit-tested directly).

`Env::update` (env.cc:153-169, parameter refresh while held): copies the new
rates/levels/outlevel/rate_scaling; while `down_`, sets an outlevel-less
sustain target from `levels_[2]` and calls `advance(2)` — which then
**overwrites** that target using the new `outlevel_` (pinned quirk,
env.cc:166-167; mirrored exactly and unit-tested).

### 1.4 sr_multiplier (env.cc:47-49)

`sr_multiplier = (44100.0 / sampleRate) · (1 << 24)` truncated to `uint32_t`:
at 48 kHz, `⌊0.91875 · 2^24⌋ = 15,414,067` (NUM-005 rev 1.1; v1's 15,405,619
was an arithmetic error). The frozen value is always **re-derived from the
formula by test** and cross-checked against
`spec/numeric-profile-v1.json: envelope.sr_multiplier_48k`, never trusted as
a literal.

### 1.5 Frame cadence and the gain side

The reference advances each operator envelope **once per 64-sample block**:
`Env::getsample()` is called from `Dx7Note::compute`
(dx7note.cc:283), which runs once per live note per block
(PluginProcessor.cpp:232 `i += N`, :256; LG_N = 6, N = 64, synth.h:29-30).
Events commit at block boundaries before the block renders
(PluginProcessor.cpp:236-238; oracle `event_to_block_policy`,
reference/oracle-identity.json; NUM-011). Within a block the envelope does
not step; the 14-bit operator gain is derived per frame as
`gain1 = gain_out == 0 ? ENV_MAX−1 : gain_out` (osc key sync resets
`gain_out` to 0 at note-on, dx7note.cc oscSync), `gain2 = ENV_MAX −
(level_in >> (28 − ENV_BITDEPTH))` (EngineMkI.cpp:314-315, ENV_BITDEPTH 14,
ENV_MAX 1<<14, EngineMkI.cpp:51-62) and ramped **linearly per sample** with
`dgain = (gain2 − gain1 + (N >> 1)) >> LG_N` (EngineMkI.cpp:155).

## 2. How the envelope becomes PCM (measurement model)

Mark I `mkiSin` (EngineMkI.cpp:124-150): `expVal = sinLog(phase >> 12
folded) + env`; amplitude law `|out| ≈ 2^(−expVal/1024)` (anti-log via
`sinExpTable`, shift `expVal >> 10`). For the gain range reachable from
envelope levels (env ≤ 16368), `expVal < 0x8000` always, so the waveform is
a wide **positive hump train** (the sign bit never sets). Consequences used
by the detector:

- block signal power ∝ `2^(−2·gain/1024)` with `gain = ENV_MAX −
  (level >> 14)`; a level increase of 2^24 (one doubling) attenuates by
  exactly 6.02 dB. The residual table-fraction error is ≤ 2^−11 ≈ 0.006 dB;
- **release-to-floor is exact digital silence**: at `level_ = 16 << 16`
  (clamp floor), `gain2 = 16368 > kLevelThresh = ENV_MAX − 100 = 16284`
  (EngineMkI.cpp:296,318); once both `gain1` and `gain2` exceed the
  threshold the Mark I gate skips the operator for whole frames — the PCM is
  exactly 0, giving a bit-exact silence-boundary probe;
- a silenced operator (output_level 0, levels 0) parks at the clamp floor
  and is gated identically, so an **algorithm-32 (index 31) voice with
  operators 1–5 at output level 0 renders exactly one audible envelope**
  (fm_core.cc algorithms[31] `{0xc4,0x04,…}`: with feedback 0 every op takes
  the no-input carrier path onto the main bus). All directed voices use this
  construction, `osc_key_sync = 1` (deterministic `gain_out = 0` at
  note-on), LFO/pitch-EG/amp-mod all zero, so the PCM tracks one operator
  envelope with no modulation side effects.

## 3. Directed cases

Single-carrier synthetic voices built with the P01 codec
(`sysex.encode_voice`), rendered by the pinned oracle
(`--rate 48000`, `--seconds` per case), compared against
`Envelope.simulate` fed with the per-note derived params of the case's
carrier. Notes 96/108 (C7/C8) keep ≥ 2.8/5.6 waveform periods inside every
64-sample block, which bounds the raw per-block RMS ripple (section 4).

| Case | Coverage | Probes |
|---|---|---|
| `base` | attack, decay (L1→L2→L3), sustain, release to **nonzero L4**, no tail cut | onset; 3 decay values; sustain; 3 release values; tail |
| `attack_fast` | rising segment, rate 99 (1-frame attack + statics[60] hold at L1) | onset; 3 sustain-window values |
| `attack_slow` | **delayed attack** (rate 20, ~1500 frames), hold, decay, release | onset; 3 attack values; decay value; 3 release values |
| `attack_rate1` | **slow rate = 1** (unfinished 3.2 s rise; slope probe, no arbitrary tail cut) | onset; 4 attack values; 2-point slope Δ probe |
| `sustain_hold` | long sustain, release, **nonzero final level** explicit | onset; 2 sustain values; 3 release values; tail |
| `release_middecay` | **release from the current level** mid-decay (L4 = 0) | onset; 4 release values; **exact-silence block** |
| `sustain_pedal` | cc64 hold: note_off does not release; cc64 off releases from the sustained level | onset; 2 held values; 2 release values |
| `velocity` | kvs = 3, velocity 20 vs 127 → plateau ratio from derived outlevels | ratio probe |
| `velocity_off` | kvs = 0: velocity must not enter the render at all | byte-identity of the two renders |
| `keylevel` | break point 40, right depth 25 exp(+), notes 96 vs 108 → plateau ratio | ratio probe |
| `keyrate_low/high` | rs = 3, rate 70: attack crossing time per note | onset; steep time probe; plateau value |

## 4. Detector (documented measurement methodology)

Per block k (64 samples): raw RMS `r[k]`, peak `p[k]`. All value comparisons
are **relative to a declared anchor window** (a plateau, or the last window
for the never-finishing rate-1 case): the unknown absolute scale constant
(mkiSin table scaling) cancels.

- **Predicted block power**: exact 64-sample sum of `2^(−2·g_i/1024)` over
  the intra-frame linear gain ramp (`gain1` = previous frame's `gain2`,
  `gain2` from the model level; EngineMkI.cpp:155,314-315). Both the
  predicted and the measured side are reduced with the **same centered
  ±8-block mean-power window**, so slope bias inside the window cancels.
- **Value probe**: `10·log10(windowed measured power / anchor)` vs the same
  for predicted power. Budget ±0.6 dB.
- **Time probe** (steep crossings only): first raw-RMS crossing of the
  amplitude implied by a model level Q-value vs the model's crossing frame.
  Budget ±4 blocks. A **slope guard** asserts ≥ 1.2 dB/block at the crossing
  (case-design error otherwise raises instead of silently loosening).
- **Onset probe**: first block with peak > 1e-6 (pre-onset PCM is exact
  digital silence). Budget ±1 block.
- **Exact-silence probe**: first frame where `gain1` and `gain2` both exceed
  16284 must be the first all-zero block, with the previous block audible.
  Budget 0 (bit-exact by the gated-skip argument, section 2).
- **Ratio probes** (velocity/keylevel): difference of anchor medians between
  two renders vs the model's `2^(−Δgain/1024)` prediction. Budget ±0.4 dB.
- **Byte-identity probe** (velocity off): SHA-256 equality of the two PCM
  byte strings.

### Tolerances and justification

| Probe | Budget | Error sources (measured on N03 renders) |
|---|---|---|
| value | ±0.6 dB | per-block RMS hump-window ripple ±0.75 dB at 5.6 periods/block reduced by the ±8-block window to ≈ ±0.2 dB; intra-window slope bias cancels by window parity; table fraction ≤ 0.006 dB; `level >> 14` truncation ≤ 0.007 dB; anchor median quantization ≈ ±0.1 dB. Empirical worst residual over all committed probes: ≤ 0.5 dB |
| time | ±4 blocks | raw (unwindowed) RMS ripple ±0.75 dB ÷ slope-guard minimum 1.2 dB/block ≈ ±0.7 block; block-peak vs frame-step alignment ±1; margin |
| onset | ±1 block | pre-onset digital silence is exact |
| ratio | ±0.4 dB | plateau ripple after median ±0.2 dB; table fraction ±0.01 dB |
| silence | 0 | exact (gate skip, section 2) |

## 5. Independent re-derivation

`tests/test_envelope.py` contains a **second implementation**
(`independent_trace`/`independent_advance`) written from section 1's cited
lines only: full per-frame trace equality with the model over a 108-combo
parameter battery (rates × levels × outlevels × rate-scaling with key
down/up events), plus exact crossing-time probes and the rate-0 static-hold
coefficient case. The frozen `sr_multiplier` is re-derived from the env.cc
formula and cross-checked against the profile JSON.

## 6. Negative controls (each demonstrably fails its check)

| Control | Mechanism | Result (committed evidence, `controls[]`) |
|---|---|---|
| Plain 4-stage linear ADSR | `LinearAdsr` interpolates **gain-linearly** between the same stage-boundary frames (times match, law differs) and drives the same probe machinery against the oracle PCM | FAIL_DETECTED, divergent stages named: `decay`, `release` (attack matches at the probed block because the DX jump-floor start crosses the gain-linear ramp there — the exponential-segment law diverges on the falling legs). An oracle-independent pure variant asserts mid-segment divergence > 2^20 Q24 |
| sr_multiplier ±1 | Mutated constant must (a) fail the formula re-derivation, (b) flip reachable coefficients so the independent derivation disagrees on exact frames | FAIL_DETECTED: M+1 flips the `inc_` coefficient in battery entry 3 (rate 1/rs 1: 1175 → 1176, diverging from frame 0) and entry 4 (rate-99 and rate-40 coefficients at rs 0); M−1 flips the `inc_` coefficient at rate 74/rs 0 (entry 2: 3371827 → 3371826). The rate-0 `statics[0]//20` count (81033) does **not** flip — it is identical for M−1/M/M+1 (entry 1 exercises that path without diverging). ±1 is otherwise absorbed by the `>> 24` truncation; the committed scan (section 7) records this as the control's sensitivity boundary |
| Swapped rate/level arrays | Model consumes `levels` as `rates` and vice versa; same probe machinery against the oracle PCM | FAIL_DETECTED, divergent stages named: `decay`, `release`; pure variant shows > 500 diverging frames |

## 7. Deviations, observations, and what is NOT claimed

- **Oracle-level retrigger**: a polyphonic note-on always re-inits the
  envelope (level 0), so "retrigger mid-decay" is exercised at the Env level
  (unit test, env.cc:102-107 semantics) plus two oracle-observable
  current-level resumes: key-up mid-decay and sustained-keyup (cc64). Mono
  mode's `transfer` path (which needs the current-level retrigger) is unit
  tested but not oracle-rendered (the oracle exposes no mono-mode events).
- **NUM-012 predicate observation** (bounded finding, not blocking N03):
  `spec/numeric-profile-v1.json: note_tail.reclaimable_predicate` reads
  `ix_ == 3`, but a completed release parks at `ix_ == 4` (advance(4) after
  the final clamp, env.cc:94/113); `ix_ == 3 ∧ down_` is the sustained
  state. N01 owns the predicate; N03 only mirrors env.cc exactly.
- **sr_multiplier ±1 sensitivity boundary**: ±1 is below the resolution of
  the `>> 24` coefficient truncation for most of the sweep space. The
  committed scan (`test_sr_multiplier_scan_statistics` in
  `tests/test_envelope.py`) sweeps rate 0..99 × rate_scaling {0,1,2,3} (the
  100×4 grid, 400 pairs), mapping each pair to its q-rate coefficient
  `q = min((41·rate) >> (6 + rate_scaling), 63)`, plus the 77-entry statics
  table in raw and attack-hold (`//20`) variants: 122 M+1 coefficient flips
  (113 `inc_` + 7 raw statics + 2 attack-hold) and 19 M−1 flips (all `inc_`;
  no statics entry flips in either variant). Widening the sweep to
  rate_scaling 0..63 (100×64 grid) gives 188 `inc_` M+1 / 19 M−1 flips.
  The control therefore asserts the formula check plus the flip locations
  above; a looser timing-tolerance control would be vacuous.
- **Not claimed**: RTL equivalence (H-stage), pitch EG (N05), routing (N04),
  synthesis/place-and-route/signoff, original-DX7 fidelity, preset quality.
  Oracle agreement is software-reference agreement only. The listening
  verdict for musical usefulness remains NOT_RUN (human audition, plan §7).

## 8. Status

- Model + directed suite: **PASS** (33/33 tests; 12 directed cases, 51 value/
  time/onset/ratio/silence probes, 15 oracle renders — see the committed
  evidence JSON for per-probe rows).
- Negative controls: **FAIL_DETECTED** (all three fire, stages named).
- CI without the pinned oracle binary: directed cases guarded-skip as
  **NOT_RUN** (never a silent pass); pure sections still run.
