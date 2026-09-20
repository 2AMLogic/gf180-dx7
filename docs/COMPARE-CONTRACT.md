# Comparator contract (R04, issue #11)

Status: **normative** for every comparison this repository performs on
reference-path renders. Implementation: `src/gf180_dx7/compare.py`
(library) + `tools/compare_renders.py` (CLI) + `tools/compare_evidence.py`
(reproducibility evidence). Qualification evidence lives under
`reference/evidence/compare/`.

## 1. Scope: exactness only

The comparator's contract is **EXACTNESS for the reference path**:
verdict **PASS** iff the two streams are byte-identical, or — in the
default exact mode — numerically identical (for finite IEEE-754
binary32 values this differs from byte equality only by the sign of
zero). Error-budgeted comparisons (frozen-integer-model vs software
reference) are a different claim class and arrive later; they must not
be declared with this tool's PASS.

A PASS here asserts sample agreement between two renders and nothing
else. It is **not** original-DX7 fidelity, not a musical-quality claim,
and not an FPGA/gf180mcu claim (AGENTS.md; DEC-001).

## 2. The no-alignment hard rule

PRIMARY rows are time-locked: index 0 is the declared common time
origin. The comparator **never** aligns, trims, crops, pads, time-warps,
resamples, or normalizes either stream, and never switches a per-patch
reference. Comparison renders are dry (plan section 7;
`reference/oracle-protocol.md` section 4). Concretely:

- A length mismatch is a **failure class** (`LENGTH_MISMATCH`), not
  something to work around. No common prefix is scored, no synthetic
  tail is appended.
- No per-render gain fitting or DC removal is applied before comparing.
- Sample-rate handling: the rate is a **label** echoed into the report;
  a wrong label is caught at the **metadata level** by
  `tools/compare_evidence.py` cross-checking `render.json` against
  `render.json` (`sample_rate` disagreement = FAIL, localized to the
  field), never by transforming samples.

## 3. Failure classes — invalid inputs never compare silently

| Class | Meaning | Result |
| --- | --- | --- |
| `EMPTY_INPUT` | one or both streams contain zero samples | FAIL (empty evidence is never a perfect match) |
| `LENGTH_MISMATCH` | decodable streams with unequal sample counts | FAIL, `sample_count_delta` recorded |
| `NON_FINITE` | NaN/±inf in either stream | FAIL, first non-finite index recorded per stream |
| `NotRunnable` (raised) | byte length not a multiple of 4 (not decodable as f32le), invalid rate label | CLI exit 2 = NOT_RUN |

Apparatus failures — missing/unreadable files, undecodable byte
lengths — produce exit 2 (`NOT_RUN`), **never a pass** (0/1/2 = pass /
fail / did-not-run, per docs/REUSE-RULINGS.md `verification-rules`
doctrine).

## 4. Primary rows and numeric policy

Let `e[i] = b[i] - a[i]` (candidate minus reference). Primary
(time-locked) rows:

- `first_divergent_sample`, `mismatch_count`, `exact_equal`
- `max_abs_error`, `max_abs_error_index` (earliest tie wins)
- `mean_bias` (mean signed error), `rms_error`
- `snr_db` = `20·(log10 rms_a − log10 rms_error)`, epsilon-guarded by
  case analysis: silent degenerate cases are `null` with an explicit
  `snr_reason` (`undefined_both_silent`, `zero_error_infinite_snr`,
  `silent_reference_negative_infinity`) — never a fabricated finite dB
  value (the scorecard-rejection lesson, REUSE-RULINGS.md).
- per-stream `peak_abs`, `peak_index`, `dc_offset`, `rms`.

Accumulation follows the cited `scaled-fsum-binary64-v1` convention
(scale by max |v|, `math.fsum`, restore units). Primary equality uses
**no epsilon and no tolerance**; the first divergence is localized by
sample index before any aggregate score is interpreted.

## 5. Secondary diagnostics — triage only, never a verdict

`Report.secondary` (clearly labeled `SECONDARY` in every report):

- **onset-time delta**: first sample with |value| > 1e-5, per stream,
  and the delta.
- **envelope-shape proxy**: short-time RMS curve over non-overlapping
  50 ms windows and the max relative window-RMS difference.
- **spectral-character proxy**: zero-crossing-rate difference.

These aid debugging only. PASS is computed exclusively from time-locked
sample identity; no secondary value can create, rescue, or upgrade a
verdict (tested: a gain-offset pair with identical onset delta and
zero-crossing rate still FAILs).

## 6. Exit-code convention

`tools/compare_renders.py` and `tools/compare_evidence.py`:
exit 0 = PASS, exit 1 = FAIL (samples differ or a failure class),
exit 2 = NOT_RUN (apparatus/environment). Statuses follow the repo
vocabulary (PASS/FAIL/NOT_RUN/BLOCKED/NO_VERDICT/STALE); coverage is
reported separately from agreement.

## 7. Reproducibility evidence (`tools/compare_evidence.py`)

Compares committed render evidence (`reference/evidence/<fixture>/`)
against a **fresh** re-render by the R02 harness (untouched; owned by
#7): metadata cross-check (`fixture`, `sample_rate`, output
samples/bytes/sha256/format, voice/events hashes, oracle binary pin,
determinism flag) followed by the exact comparator on the two
`render.f32` files. A PASS record proves reproducibility end-to-end for
that fixture under the pinned oracle; a render without its metadata
echo, or with an echo that disagrees, is never a pass.

## 8. Adaptation provenance

The measurement **discipline** is adapted from the 2AMLogic/gf180-torchsynth
paired-metrics component — `src/torchsynth_voice/paired_metrics.py` +
`spec/PAIRED-METRICS.md`, pinned at
`36bdc67faa1ff2509988b772efcca7a7eb4e7115` (Apache-2.0; catalog row
`paired-comparator` in docs/REUSE-RULINGS.md and
docs/reuse/catalog.json). It is re-implemented stdlib-only in
`src/gf180_dx7/compare.py` with **no upstream bytes imported** (DR-0002:
provenance before import; hash equality alone never qualifies). What
was kept: time-locked paired comparison, no-alignment doctrine,
first-divergence localization, earliest-tie max-abs-error, scaled-fsum
accumulation, silent-case SNR refusal table, empty-input-is-invalid
rule. What was retargeted: f32le PCM input boundary (the oracle
protocol's render format), single shared sample-rate label with
metadata-level rate checking, exactness-only verdict (no rubric
machinery), stdlib-only implementation (no NumPy/Torch), and the
DX7/gf180mcu evidence conventions. Requalification: 
`tests/test_compare.py` + `tools/qualify_compare.py`, with the retained
report at `reference/evidence/compare/qualification-r04.json`.

The apparatus-is-its-own-failure-surface doctrine (gf180-parasynth
`model/audio_measure.py`, per the R04 issue brief) is honored by the
mutation battery, the apparatus negative controls (missing file, odd
byte count → exit 2), and the retained qualification report.

## 9. Non-goals

Perceptual metrics, corpus-wide gate runs, RTL comparison (H-series),
error-budget tolerances, hardware audio captures (they need their own
alignment/calibration procedure), and per-patch reference switching.
