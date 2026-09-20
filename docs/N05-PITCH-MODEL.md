# N05 — Integer pitch and modulation model (issue #19)

- **Model:** `src/gf180_dx7/model/pitch.py` (stdlib-only, clean
  re-implementation; numeric semantics adopted as **facts** from the pinned
  Apache-2.0 msfa files by citation — zero copied bytes; the GPL-3.0
  `Source/EngineMkI.cpp` wrapper is never copied or imported)
- **Tests:** `tests/test_pitch.py` (66 tests; `python3 -m unittest discover
  -s tests` OK; oracle-render tests guarded-skip to NOT_RUN when the pinned
  oracle binary is absent or hash-drifted)
- **Status:** PASS on all executed cases (2026-09-20, pinned Dexed
  `b6cf2e9190e216387ad96f977284ebbef765b447`, pinned oracle binary
  sha256 `6e17a540…8ed42`, 48 kHz). One pre-existing unrelated skip in the
  repo suite (audition recall guard) is unchanged by this issue.
- **Claim boundary:** model-vs-reference agreement at the *pitch/modulation
  seam only*. This is software-reference agreement, not original-DX7
  fidelity, and never a musical-quality claim (DEC-001, plan §7). RTL-vs-
  frozen-model exactness is H-series work and is not claimed here.

## 1. What the model pins

`NotePitch` consumes the 155-byte unpacked voice body (identical to Dexed's
`data[0..154]`, `Source/PluginData.cpp:199`) plus a midinote, and produces
per-operator 32-bit phase increments per 64-sample frame, mirroring
`Dx7Note::compute` (`Source/msfa/dx7note.cc:208-296`).

| Semantics | Pinned behavior | Citation (pinned tree) |
|---|---|---|
| Standard tuning | `logfreq = 50857777 + (1<<24)//12 · midinote` | `Source/msfa/tuning.cc:14-19` |
| Freqlut LUT | `y = (1<<44)/rate`; 1024 doublings in float64, `floor(y+0.5)` | `Source/msfa/freqlut.cc:35-42` |
| Freqlut lookup | linear interp over low 14 bits; `y >> (20 - hibits)` | `Source/msfa/freqlut.cc:46-55` |
| Ratio mode (`osc_mode 0`) | keyboard-tracking: `noteLogFreq` + detune + coarse + fine | `Source/msfa/dx7note.cc:42-53` |
| Detune | `logfreq += 0.0209·exp(-0.396·l/2^24)/7 · l · (detune-7)` (float64 → int32 trunc) | `Source/msfa/dx7note.cc:46-47` |
| Coarse | `coarsemul[coarse & 31]` (32-entry Q24 log2 table; coarse 0 = −1 octave, coarse 1 = neutral) | `Source/msfa/dx7note.cc:30-36, 49` |
| Fine | `+ floor(24204406.323123·ln(1+0.01·fine)+0.5)` when fine ≠ 0 | `Source/msfa/dx7note.cc:50-53` |
| Fixed mode (`osc_mode ≠ 0`) | **keyboard-independent**: `(4458616·((coarse&3)·100+fine))>>3`, `+13457·(detune-7)` only for detune > 7 ⇒ f = 10^((100·coarse+fine)/100) Hz | `Source/msfa/dx7note.cc:58-61` |
| Pitch EG | per-frame `level_ ± inc_` against `pitchenv_tab[l]<<19` targets, `inc_ = pitchenv_rate[r]·unit_`, `unit_ = 1050` at 48 kHz; segments 0-2 run while keydown (held level = `levels_[2]`), `levels_[3]` is the release target | `Source/msfa/pitchenv.cc:22-24, 26-43, 45-93`; `Source/msfa/env.cc:74-97` (same segment rule) |
| LFO | 6 waveforms; `unit_ = 33587` at 48 kHz; rate shaping `sr`-multiplier; delay ramp `delayinc_/delayinc2_` with saturation; keydown resets `phase_ = (1U<<31)-1` only when sync, always resets `delaystate_` | `Source/msfa/lfo.cc:26-97` |
| LFO sine branch | msfa `Sin::lookup` (SIN_DELTA form; rotation-recursion table) | `Source/msfa/sin.cc:31-56`, `Source/msfa/sin.h:29-35, 43-61`, `Source/msfa/lfo.cc:67` |
| LFO PM composition | `pmd = pitchmoddepth_·lfo_delay` (uint32 Q32); `senslfo = pitchmodsens_·(lfo_val − 2^23)`; `pmod_1 = |pmd·senslfo >> 39|`; `pmod_2 = |ctrl_pitch_mod·senslfo >> 14|`; `pitch_mod = pitchenv.getsample() + max(pmod_1,pmod_2)·sign(senslfo)` | `Source/msfa/dx7note.cc:210-217` |
| Sensitivities | `pitchmoddepth_ = (patch[139]·165)>>6`; `pitchmodsens_ = {0,10,20,33,55,92,153,255}[patch[143]&7]` | `Source/msfa/dx7note.cc:128-130, 198-199` |
| Pitch bend | float-gated spot (NUM-008 item 1): `(float)(pb<<11)·range/12.0 → int32`; pinned defaults range up/dn = 3, step 0, wheel at 0x2000 | `Source/msfa/dx7note.cc:219-233`; `reference/manifest.json` pitch_bend_range |
| Routing of mod | ratio ops: `Freqlut(basepitch + pitch_mod)`; fixed ops: `Freqlut(basepitch + pitch_base)` — **pitch EG and LFO PM do not reach fixed-mode operators**; `pitch_base` = bend + master tune only | `Source/msfa/dx7note.cc:256-257, 278-281` |
| Frame cadence | one LFO `getsample()`+`getdelay()` and one pitch-EG step per 64-sample frame; LFO `keydown()` at note-on before the block renders | `Source/PluginProcessor.cpp:244-245`; oracle `main.cpp:187-189, 410-411` |
| AM path (interface for N06) | `amp_mod_per_frame()` returns `amd_mod` (Q24) per `dx7note.cc:259-268`; the in-operator application (`exp` sensitivity, `dx7note.cc:284-291`) stays with N06 | `Source/msfa/dx7note.cc:259-268` |

### Phase-increment units (reading NUM-003 precisely)

The frozen profile freezes a 32-bit wrapping phase accumulator (NUM-003).
The **sine period is 2^24 phase LSBs**, not 2^32: Mark I's `mkiSin` folds the
quarter wave over `(phase >> 12) & 1023` with quadrant bits 22-23
(`Source/EngineMkI.cpp:64-78, 125`), and msfa's `Sin::lookup` spans the same
2^24-unit period (`Source/msfa/sin.h:45-53` over a 1024-entry full-period
table, `sin.cc:43` antiperiod at 512 entries). Freqlut is built to match:
`lookup` yields `2^(24 + logfreq_octaves)/rate` per sample. Empirical anchor
(committed R02 smoke render): midinote 60 measures 784.87 Hz crossing rate /
3 = **261.62 Hz = C4**. All frequency assertions in the test suite use
`f = increment · 48000 / 2^24`.

## 2. PINNED-CONFIG divergences (explicit switch, registry §5)

Both findings are encoded in `spec/compatibility-registry-v1.json` and
`docs/REGISTRY-POLICY.md` §5; the model implements **both** behaviors behind
`NotePitch(transpose_mode=...)` / explicit controller arguments, and the
tests assert the divergence is real and nonzero.

1. **Transpose (finding `perf-transpose-12`).** Upstream Dexed applies
   `pitch += tuningTranspositionShift()` at keydown — `data[144] - 24`
   semitones under standard tuning (`Source/PluginProcessor.cpp:458,
   566-580`). The pinned oracle wrapper's `keydown` omits that shift
   (oracle `main.cpp:168-194`), so transpose overrides render
   byte-identically to the base (registry case
   `identical_hash_vs:dir-base`). Model switch:
   - `transpose_mode="documented"` — implements the DOCUMENTED upstream
     semantics (transpose − 24);
   - `transpose_mode="pinned_wrapper"` — the pinned wrapper's observed
     behavior (transpose ignored).
   Measured divergence (test
   `TestTransposeDivergence.test_divergence_is_real_and_nonzero`): for
   transpose = 36, documented − wrapper basepitch = **16,777,212 =
   exactly 12·(1<<24)//12 logfreq** (12 semitones; increment ratio 2.0).
   The wrapper behavior is additionally confirmed on fresh renders
   (transpose 24 vs 36 renders measure the same frequency,
   `TestRenderedStaticPitch.test_transpose_does_not_shift_render_pinned_wrapper`).
   DEC-023 still requires the product to preserve transpose, which the
   `"documented"` mode provides.

2. **CC1 inert at pinned controller defaults (finding `ev-modwheel-127`).**
   The pinned `FmMod` wheel default is `range = 0`
   (`Source/msfa/controllers.h:43-48`; manifest
   `mod_wheel_default_modifiers`), so `Controllers::applyMod` computes
   `total = cc · 0.01·0 = 0` (`controllers.h:66-77`) and CC 1 can never
   move pitch — the committed smoke shows `ev-modwheel-127` byte-identical
   to `ev-modwheel-0` (PASS in `reference/evidence/registry-smoke.json`).
   The model reproduces the inertness at the defaults
   (`TestControllerCC1.test_cc1_inert_at_pinned_defaults`) and the
   DOCUMENTED response when the wheel range is nonzero
   (`test_cc1_live_with_documented_nonzero_range`: range 99 turns
   CC 1 = 127 into `ctrl pitch_mod = 125`, which moves the frame
   increments). CC1 inertness is a pinned-*defaults* finding, not a model
   limitation.

## 3. Oracle-side estimator methodology

All rendered claims come from dry f32le renders of the pinned oracle
(identity re-hash gated; guarded NOT_RUN skip when absent). Probe patches
are single-audible-operator (algorithm 32, one `output_level` = 99, the
other five at 0, which gate to exact silence — verified: rendered
frequencies match the single operator's pitch exactly).

- **Estimator (interval method):** rising zero crossings with sub-sample
  linear interpolation; each consecutive pair yields one per-period
  frequency estimate `f = 48000/(Δsamples)`, timestamped at the interval
  center. Per-period estimates localize time well enough to track sweeps
  and vibrato without window-smearing bias; the crossing pair spacing also
  supplies a duration weight used only for the time-averaged vibrato
  center.
- **Coarse estimator (LSQ):** least-squares fit of crossing index vs time
  over long windows, used for low-rate sustained segments and
  octave-distinctness checks.
- **Model counterpart:** per-frame log-frequency deviation replayed with
  the same frame cadence the wrapper uses (LFO sampled from block 0;
  `lfo.keydown()` at the note-on block — `ONSET_FRAME = 37` for the 2400-
  sample onset — then per-frame `frame_frequencies()`).

### Tolerances (declared) and measured results

| Check | Tolerance | Measured (worst case) |
|---|---|---|
| Static pitch: rendered f vs model `lookup(basepitch)·48000/2^24` | 2% rel (`TRAJ_REL_TOL`) | ≤ 0.02% (C4 probe: 261.642 vs 261.626 Hz) |
| Ratio-mode keyboard tracking on renders (60 vs 72 semitones) | 0.1% | ≤ 0.002% |
| Fixed-mode render vs 10^(coarse) Hz; keyboard-invariance | 0.2% / 0.1% | ≈ 0% (1000.0 Hz) |
| Pitch-EG trajectory (rising `peg-up`, falling `peg-down` shapes), per interval vs model frame | 2% rel (`TRAJ_REL_TOL`) | ≤ ~0.5% (window lag) |
| Pitch-EG held endpoint (rising +10<<19 = +0.3125 oct → 321.66 Hz; falling −10<<19 = −0.3125 oct → 213.06 Hz) | 2% rel (`ENDPOINT_REL_TOL`) | 1.0% / 1.1% |
| Vibrato extent (octaves, either side) vs model replay | 0.15 oct (`EXTENT_TOL_OCT`) | 0.069 oct (depth 99/sens 7: measured [−0.919, +0.975] vs model [−0.988, +0.991]) |
| Vibrato center (time-weighted) | 1% (`CENTER_REL_TOL`) | ≤ 1% (depth 99 worst case) |
| Depth-0 control (no vibrato) | extent ≤ 0.005 oct (`VIB_CONTROL_OCT`) | ≤ 0.0004 oct |
| Freqlut ratio math (octave doubling, semitone steps) | 2e-4 rel (`LUT_REL_TOL`) | ≤ 2e-4 (LUT interpolation granularity) |

Extent tolerance headroom: the interval estimator lags during fast sweeps
(one period of time localization), which is why deep-vibrato extremes read
~0.07 oct inside the model's; the declared 0.15 oct covers that estimator
bias, not model error. LFO-active renders support **one-sided**
difference claims only (R05 instance-dependence finding,
`reference/trace-registry.json` note); extents and time-weighted centers
are phase-independent, so the comparisons above are phase-safe.

## 4. Acceptance coverage (issue #19) and R06 cross-reference

| Issue acceptance item | Named trace/case | Where verified |
|---|---|---|
| Every supported pitch/mod parameter has a named case | coverage table below + `tools/registry_coverage.py` (0 holes) | R06 registry |
| Fixed mode does NOT track keyboard; ratio mode does | `osc-fixed-carrier`, `dir-base` | `TestRatioVsFixed.test_fixed_mode_ignores_keyboard`, `test_ratio_mode_tracks_keyboard`; rendered: `TestRenderedStaticPitch.test_fixed_mode_render_ignores_keyboard`, `test_ratio_mode_render_tracks_keyboard` |
| Coarse = 0 | `bd-coarse0` | `TestCoarseAndDetune.test_coarse_zero_is_minus_one_octave` |
| Coarse extremes (+wrap `&31`) | `bd-coarse31` | `test_coarse_two_doubles`, `test_coarse_three_triples`, `test_coarse_mask_31_wrap` |
| Detune extremes | `bd-detune0`, `bd-detune14` | `test_detune_extremes_distinct_and_monotone`, `test_detune_identity_linear_in_detune`, `test_detune_matches_published_ratio` |
| Transpose interactions | `perf-transpose-12` (registered finding; fresh-render measurement in `TestRenderedStaticPitch`) | `TestTransposeDivergence` (documented vs pinned-wrapper, divergence = 12 semitones, NONZERO) |
| Pitch EG up/down | `peg-up`, `peg-down` | `TestRenderedPitchEnv.test_rising_pitch_eg`, `test_falling_pitch_eg`, `test_rising_and_falling_renders_distinct`; analytic: `TestPitchEnvAnalytic` |
| LFO waveforms 0-5 | `lfo-wave0` … `lfo-wave5` | `TestLfoWaveforms` (formula transcription + shape probes: triangle legs, saw complement, square two-valued duty, sine antiperiod, S&H LCG) |
| LFO speed modes | `lfo-speed0`, `lfo-speed99` | `test_rate_shaping_deltas`, `test_unit_48k` |
| LFO delay (max / immediate) | `lfo-delay99` | `test_delay_param_99_ramps_late`, `test_delay_param_zero_full_immediately` |
| LFO sync modes | `lfo-keysync0`, `lfo-keysync1` | `TestLfoSync` (+ `osc-keysync0/1` stay with N02/N08) |
| LFO PM depth/sens | `lfo-pm-depth0/99`, `lfo-pm-sens0/7` | `TestRenderedLfoPM` (extent vs model replay) |
| Mod-wheel | `ev-modwheel-127` | `TestControllerCC1` + committed smoke evidence |
| Pitch bend | `ev-pitch-bend-up/down/center` | bend path mirrored per NUM-008 item 1 (`_pitch_base`); full bend renders stay with N08 (float-gated spot, declared budget) |

**No unsupported parameter silently falls back:** the model raises on
unknown `transpose_mode`, unknown patch-override keys, and patches shorter
than 145 bytes; every field above is implemented, none approximated.

## 5. Negative controls (live, must fail with the case named)

| Control | Mutation | Target check |
|---|---|---|
| Fixed mode tracking keyboard (issue #19 named control) | `_osc_freq_fixed_tracks_keyboard` adds `noteLogFreq` in fixed mode | `check_fixed_mode_ignores_keyboard` raises "fixed-mode tracks keyboard (case: coarse=… fine=… detune=…)" — `test_negative_fixed_mode_tracking_keyboard_fails` |
| Detune-extreme wrap | `_osc_freq_detune_wrapbug` misreads the unsigned 0..14 field as a signed 4-bit field | `check_detune_extreme_identity` raises "detune-extreme wrap identity broken (midinote …)" — `test_negative_detune_extreme_wrap_fails` |
| LFO sync ignored | `_LfoSyncIgnored.keydown()` keeps `phase_` | `check_lfo_keysync_restarts_phase` raises "LFO sync ignored: keydown did not restart the phase (case: lfo keysync trajectory)" — `test_negative_lfo_sync_ignored_fails` |

All three are exercised on every run and demonstrably fail the check they
target (a mutation that does NOT change the outcome is itself reported as
"negative control did not mutate").

## 6. Deviations and bounded findings

1. **NUM-003 prose vs sine period** (§1 above): "one cycle = 2^32"
   describes the 32-bit accumulator's wrap width; the sine period is 2^24
   phase LSBs. Anchored empirically against the pinned smoke render and by
   Freqlut/tuning-base consistency. Recorded here for the N02/N08
   consumers of the frequency mapping; the frozen accumulator semantics
   are unchanged.
2. **`abs()` on `INT_MIN`** (`dx7note.cc:213,215`): UB in C, defined in the
   model. Unreachable at real parameter values (would require
   `pmod == −2^31`).
3. **Freqlut regeneration:** the model rebuilds the LUT in Python float64
   following `freqlut.cc:35-42` operation order; per NUM-003/NUM-008 the
   regenerated LUT carries a ≤ 1 phase-increment-LSB budget on rare
   pitches, counted at N02. The pinned-oracle rendered comparisons above
   are insensitive to it (≈ 2.5e-8 relative at A4).
4. **LFO uninitialized state:** upstream leaves `phase_`/`randstate_`/
   `delaystate_` uninitialized (`reference/trace-registry.json` R05
   finding); the model zeroes them and relies on `reset()`/`keydown()`.
   With key-sync patches (all rendered probes) the wrapper's own
   `lfo.keydown()` makes the rendered LFO deterministic, and all rendered
   claims are phase-independent (one-sided discipline) regardless.
5. **MPE branch:** `mpePitchBend = 8192` at note init (`dx7note.cc:203`)
   makes the MPE bend term zero in the pinned configuration; the model
   documents and relies on that (`NotePitch._pitch_base`).
6. **Pre-existing branch drift repaired in passing:** two C01 files
   (`src/gf180_dx7/capabilities.py`, `tools/compile_capabilities.py`) had
   landed without `local_originals` byte-pins, failing
   `tools/check_reuse.py` on the base branch; they are pinned
   retroactively in this PR (adds drift detection, weakens nothing), and
   `README.md`'s generated capability view was regenerated (one trailing
   blank line) so `tests/test_capabilities.py` agrees with the committed
   view.

## 7. Evidence retention

- Model + tests: `src/gf180_dx7/model/pitch.py`, `tests/test_pitch.py`
  (byte-pinned in `docs/reuse/catalog.json` `local_originals`).
- Committed oracle evidence cross-referenced:
  `reference/evidence/registry-smoke.json` (dir-base, ev-modwheel-0/127,
  bd-coarse0/31, bd-detune0/14, lfo-* cases, peg-up/peg-down);
  `reference/evidence/smoke/render.f32` (the 2^24-phase-unit anchor).
- Fresh rendered measurements: produced on every test run from the pinned
  oracle binary (12 ms per render; full N05 suite < 1 s), guarded to
  NOT_RUN where the binary is absent. Per-case verdicts: PASS (66/66, no
  skips inside `tests/test_pitch.py`).
