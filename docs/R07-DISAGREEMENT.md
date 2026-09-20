# R07 — Targeted oracle-disagreement report (issue #14)

**Status:** investigation complete; six cases rendered, compared, and
localized. **No engine is adjudicated as physically correct.**

- Reference engine: `dx7-oracle` (pinned Dexed Mark I wrapper,
  `reference/oracle-identity.json`).
- Second engine: `hexter-oracle` (pinned hexter 1.1.1 DSSI engine wrapper,
  `reference/second-oracle-identity.json`; upstream
  `theabolton/hexter@1cf1bfea5962f7c9726e0cf809b762b3b2655225`,
  GPL-2.0-or-later, external-oracle-only).
- Evidence: `reference/evidence/r07/` (per-case inputs, comparison JSONs,
  render hashes, run logs; PCM gitignored and regenerable).
- Driver: `python3 tools/r07_compare.py` (regenerates everything
  deterministically on the pinned toolchain; identity re-verified per run).

## What this investigation is, and is not

Two independent software implementations of the DX7 voice architecture were
rendered on identical inputs (identical voice bytes re-encoded through the
P01 codec, identical event times at 64-sample block multiples, 48 kHz,
3.0 s, single note 60, velocity 100, note-off at 2.0 s) and compared with
the R04 exact time-locked comparator in exact mode. The exactness FAIL on
every case is the expected cross-engine result and is the *finding* — it is
localized, classified, and explained below as a set of HYPOTHESES.

**Non-claims (normative).** This report does not and cannot adjudicate
which implementation is physically correct: that would require original
DX7/TX7 hardware measurements (or a third independent source agreeing with
one of them), which this investigation does not have. Nothing here is a
claim about original-DX7 fidelity, about preset quality, or about the
instrument sounding good. Dexed agreement is not original-DX7 fidelity, and
neither is hexter agreement. No engines were averaged, no render was
normalized, aligned, or time-warped, and the project reference was not
switched (docs/COMPARE-CONTRACT.md).

## Per-case results

All renders are dry; primary rows come from
`src/gf180_dx7/compare.py` (candidate = hexter, reference = Dexed, so
error = hexter − Dexed). "Magnitude" is `rms_error` relative to the
reference signal RMS (signal_scale = within 6 dB of the signal; every case
landed in this class — the engines disagree at the scale of the waveform
itself, not at a rounding level).

| case | diverge from | rms_error | SNR (dB) | max err | mean bias | onset Δ | ZCR Dexed/hexter | env max rel Δ |
|---|---|---|---|---|---|---|---|---|
| fb-alg4 | 4800 | 0.164 | −2.67 | 0.291 @99113 | −8.6e-4 | −3 | 0.0105 / 0.0204 | 0.96 @4800 |
| fb-alg6 | 4800 | 0.096 | −1.54 | 0.185 @19826 | −2.4e-3 | −3 | 0.0208 / 0.0053 | 0.88 @4800 |
| detune-extreme | 4800 | 0.180 | −3.38 | 0.498 @100168 | +1.1e-4 | −30 | 0.0111 / 0.0027 | 0.98 @4800 |
| slow-envelope | 4800 | 0.0028 | −10.47 | 0.0055 @143463 | +2.6e-5 | −30 | 0.0105 / 0.0026 | 0.74 @4800 |
| fixed-frequency-high | 4800 | 0.168 | −2.96 | 0.458 @21055 | −1.6e-4 | −3 | 0.3459 / 0.3491 | 0.98 @4800 |
| corpus-shrpsynth | 4801 | 0.024 | −1.06 | 0.146 @5890 | −1.8e-3 | −17 | 0.0442 / 0.0288 | 1.00 @127200 |

Onset deltas of −3/−17/−30 samples are threshold-crossing artifacts of
already-diverging waveforms (the 1e-5 threshold is crossed at slightly
different times when waveform shape differs); both engines excite the note
at the same 4800 block, and every `first_divergent_sample` is the note-on
block or the sample immediately after — there is no pre-onset or
event-scheduling divergence.

## Per-case hypotheses (labeled HYPOTHESES, with seam + citation)

**fb-alg4 / fb-alg6 — seam: feedback coefficient mapping + feedback history
depth.** Steady-state error is constant in time (fb-alg4: err_rms 0.169
every 0.5 s window after attack; fb-alg6: 0.099), i.e. both engines reach a
stable feedback regime but at different gain/character, rather than
diverging without bound. The mapped coefficient differs by construction:
Dexed msfa uses `fb_shift_ = 8 − feedback` (`dx7note.cc:196-197`,
`FEEDBACK_BITDEPTH = 8` at `dx7note.cc:28`) with a two-sample averaged
history `(y0 + y) >> (fb_shift + 1)` inside the 64-sample kernels
(`fm_op_kernel.cc compute_fb`, entered from `fm_core.cc:114-118`); hexter
uses `aux_feedbk = feedback/(2π)·0.18` with an explicit upstream note that
per-algorithm feedback scaling is missing (`dx7_voice.c:1390`, `-FIX-
feedback_scaling[voice->algorithm]`) and a one-sample history
(`dx7_voice_render.c:128-131`). The topology sensitivity supports this:
fb-alg6 (three carriers, feedback into a different routing) shows a ~33 %
steady level deficit in hexter (window RMS 0.0835 vs 0.0555) and a much
darker spectrum (ZCR 0.0208 vs 0.0053), while fb-alg4 differs mostly in
spectral brightness (ZCR 0.0105 vs 0.0204, level within 3 %). If confirmed
by a future N-model-vs-hexter probe at the operator level, the actionable
outcome is: our frozen integer model must pick ONE feedback mapping and
history depth and declare it; Dexed's mapping is already the R01 reference,
and the finding documents that hexter cannot be treated as an
interchangeable oracle for feedback-heavy material.

**detune-extreme — seam: detune-to-frequency conversion at range extremes.**
The two algorithm-1 carriers (raw detune 0 vs 14, center 7) produce a beat
whose rate visibly differs: window RMS in the two engines walks out of and
back into phase (Dexed 0.084→0.134→0.117 vs hexter
0.153→0.114→0.036→0.117), max error grows to 0.498 near note-off
(phase-walk accumulation), and ZCR differs 4× (0.0111 vs 0.0027).
Dexed converts detune through its pitch tables (`dx7note.cc`
pitchdetune/`Freefract` path); hexter through its own frequency arithmetic
(`dx7_voice.c` recalculate-freq path). Both are "detuned by ±7", but the
implied cents differ → different beat rate. Actionable: the detune curve is
a declared-model seam (N01 budgets should include detune-curve tolerance
explicitly, since corpus voices use detune heavily).

**slow-envelope — seam: envelope rate/level coefficient fit at rate = 1.**
Smallest absolute disagreement of the set (rms_error 0.0028, max 0.0055)
but a large *relative* one: during the near-flat rate-1 attack hexter sits
~2–3× above Dexed (0.5 s window RMS 0.002 vs 0.0005, relative gap ≈ 0.7
persisting across the whole hold). Dexed's msfa `env.cc` interpolates a
keyframed table per 64-sample block; hexter derives frame durations from
its rise/decay duration tables with pre/post compensation
(`dx7_voice.c dx7_op_eg_set_rate`, `dx7_voice_eg_rate_*` tables in
`dx7_voice_data.c`). At rate 1 both are extrapolating the low-slew end of
their respective coefficient fits, and they disagree by a small constant
factor — a tuning-class difference, not a structural one. Actionable: the
N01 model-vs-reference error budget must state envelope-coefficient
tolerance at rate extremes; this case is the calibration point.

**fixed-frequency-high — seam: fixed-frequency mode conversion.**
Post-attack window RMS agrees to 0.1 % (0.125 vs 0.125) yet error is
signal-scale (0.168) with the SMALLEST ZCR difference of the set
(0.3459 vs 0.3491) — the engines produce nearly the same energy and nearly
the same frequency, but de-cohered waveforms. Fixed coarse=15 fine=99 sits
at the top of the fixed range where Dexed's frequency tables
(`dx7note.cc`/`freqlut.cc`) and hexter's `fixed_freq_multiplier = tuning/440`
formula (`dx7_voice.c:815`) converge to slightly different Hz. Actionable:
fixed-mode frequency conversion needs an explicit tolerance row in the N01
budget; corpus voices contain fixed-mode operators (dev32 goal
`fixed_frequency_op`).

**corpus-shrpsynth — anchor case (real R03 voice, dev32 entry 0).**
Amplitude envelope tracks well (mid-body window RMS relative difference
≤ 5 %; both decay to near-silence after note-off), onset within 17 samples,
but waveform-scale error dominates (SNR −1.06 dB against a quiet voice,
max err 0.146 just after onset) and hexter's spectral character is darker
(ZCR 0.0442 vs 0.0288). Reading: for a real voice with feedback 1 the
compounded disagreement is dominated by waveform-phase/spectral-structure
differences (modulation-index scaling, sine-table phase conventions),
not by envelope shape. This case anchors the budget discussion below.

## What the findings would mean for N01 / model budgets

These are BOUNDED FINDINGS, nonblocking for the declared software profile
(the project reference remains the R01/R02-pinned Dexed Mark I oracle;
nothing switches per patch). Full analysis in
`docs/decision-records/0005-oracle-disagreement-findings.md`. In summary:

1. Model-vs-reference error budgets (N-series) must be declared PER SEAM,
   not as one global number: feedback mapping (feedback 0 vs >0 behave
   categorically differently), envelope coefficient fit at rate extremes,
   detune curve at range extremes, and fixed-mode frequency conversion are
   four independent tolerance rows.
2. The second oracle is NOT interchangeable with the reference for
   regression use: cross-engine disagreement is signal-scale on every seam
   probed. It is usable for *localization* (this report) and as an
   independent sanity direction (sign of error, presence/absence of
   features), never for exactness.
3. Corpus renders (R03 conventions) inherit these seams; dev32 contains
   high-feedback and fixed-frequency voices, so corpus-level agreements
   must quote the seam budgets, not an average.

## Follow-up actions (recorded; none blocking R07)

- File/track an N-series issue defining per-seam error-budget rows
  (feedback, envelope extremes, detune curve, fixed-mode conversion) —
  from finding F1–F4 of DR-0005.
- Optional (no commitment): a hexter-side build with
  `HEXTER_USE_FLOATING_POINT` to check whether the feedback/algebra
  differences are quantization-scale or structural, using the same driver
  with a second identity file. Only if an N-series issue needs it.
- If any future discrepancy invalidates a D00/N01 decision (not a mere
  tuning difference), stop and escalate per the issue's stop/escalate
  rule. Nothing in this report reached that bar: all six findings are
  tuning/implementation-convention differences between software engines,
  none adjudicable without hardware evidence.

## Negative controls

- Review rejects any "implementation X is wrong" conclusion without a
  third source or hardware evidence: this report makes none, and
  `tools/r07_compare.py validate_case_report()` mechanically rejects
  case reports lacking the non-adjudication statement, the
  `status=HYPOTHESIS` labeling, or the seam naming (tested in
  `tests/test_r07.py`).
- Cross-engine exactness PASS is structurally flagged: the validator
  treats a PASS verdict on a cross-engine case as a defect (it would
  indicate engine mixing or identical engines).
- The corpus case refuses to run on archive or canonical-hash drift
  (tested with a tampered-voice negative control in `tests/test_r07.py`).
