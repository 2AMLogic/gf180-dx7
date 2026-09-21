# DR-0006: N06 integration findings — wrapper-level LFO behavior, the pitch-path bend seam, and the parked-release predicate

- **DR number:** 0006
- **Status:** Proposed
- **Date:** 2026-09-20
- **Deciders:** loom-builder (agent), pending owner review
- **Related issues:** #22 (N06); refines NUM-007/NUM-011/NUM-012 readings,
  and N04/N05 method-level scope notes

## Context

N06 integrates the merged module models (N02 operator, N03 envelope, N04
algorithm/feedback, N05 pitch/mod) into one single-note voice and mirrors
the pinned render wrapper (external GPL oracle `main.cpp`, oracle commit
`0047c3f5`, `reference/oracle-identity.json`; R01-pinned Dexed tree
`b6cf2e91`). Composing the modules against the wrapper's actual block
loop exposed five findings. None was resolved by tuning model outputs to
reference outputs; all are recorded per property in
`reference/evidence/n06-integrated/dev32-subset.json` with the model
source in `src/gf180_dx7/model/integrated.py` and the derivation in
`docs/N06-INTEGRATED-MODEL.md`.

Measured facts:

- **F-1 (LFO cadence).** The wrapper samples the shared LFO
  (`getsample()` + `getdelay()`) once per 64-sample block from block 0,
  regardless of live voices (oracle `main.cpp:444-445`). N04's
  `render_note` had gated LFO stepping on note liveness; every committed
  N04 case starts its note at sample 0, so those renders were insensitive
  and N04's evidence stands unchanged.
- **F-2 (wrapper sine LFO is constant).** The wrapper's `State.init`
  (oracle `main.cpp:103-107`) calls `Freqlut::init`/`Lfo::init`/
  `PitchEnv::init`/`Env::init_sr` but never `Sin::init()`. msfa's
  `sintab` is a zero-initialized global (`sin.cc:14`, static storage), so
  `Sin::lookup` returns 0 for every phase and the wrapper's wave-4 LFO
  outputs the constant `1 << 23`. Oracle-trace evidence: `lfo_value ==
  8388608` on every block of a wave-4 render (dev32-01).
- **F-3 (pitch-path bend seam).** `NotePitch.frame_frequencies`
  (N05, `src/gf180_dx7/model/pitch.py`) drops `pitch_base` (bend +
  master tune) from the ratio-mode operator branch: it composes
  `pitch_mod` through `pitch_mod_per_frame` with the default (centered)
  bend and adds the real `pitch_base` only for fixed-mode ops. The pinned
  engine (`dx7note.cc:217,256-257,278-281`) adds `pitch_base` into
  `pitch_mod` before the operator loop, so ratio-mode operators DO take
  bend + master tune. N05's declared scope excluded bend renders ("full
  bend renders stay with N08"), and every committed N05/N02/N04 oracle
  comparison is bend-free, so no committed evidence is invalidated.
- **F-4 (uninitialized LFO phase, keysync 0).** msfa `Lfo` leaves
  `phase_` uninitialized (no constructor/reset initializer,
  `lfo.cc:26-49`); the wrapper resets it only via `lfo.keydown()` when
  the patch's LFO key sync is set. On keysync-0 patches the rendered LFO
  phase is C++-indeterminate stack residue (stable per binary; the R05
  finding, `reference/trace-registry.json`). dev32-04 and dev32-21
  (keysync 0 with an LFO path reaching the PCM) FAIL against the oracle
  on exactly that property; no derivable reference value exists.
- **F-5 (parked-release predicate).** NUM-012's reclaimable predicate
  reads `down_ == true ∧ ix_ == 3`; a *released* voice instead parks with
  `down_ == false` and `ix_` 3→4 (env.cc:74-97; N03 §7 already recorded
  the `ix_ == 4` parking). The exact-silence/output-neutrality argument
  is unchanged: `level_ == targetlevel_ == 16 << 16`, `ix_ >= 3`, and
  `gain_out > kLevelThresh` force the Mark I gate to skip the operator
  (EngineMkI.cpp:314-318) regardless of the key flag.

## Decision

1. `IntegratedVoice` (N06) is the composition of record for event-timed
   single-note renders. It mirrors the wrapper loop: LFO stepping from
   block 0 (F-1), pitch composition with `pitch_base` reaching ratio-mode
   operators, composed from N05's public pieces `pitch_mod_per_frame` +
   `_pitch_base` + `Freqlut` (F-3), and the F-5 predicate for the
   exact-silence continuation. `render_note` (N04) remains valid only for
   its committed note-live-from-block-0 scenarios and is superseded for
   event-timed cases.
2. The LFO ships behind `lfo_mode`: `"pinned_wrapper"` (default; the
   comparison target's observable behavior — wave 4 constant `1 << 23`,
   F-2) and `"documented"` (the NUM-007 regenerated msfa sine table).
   The frozen numeric profile is NOT changed; F-2 is recorded as a
   pinned-wrapper divergence, same treatment as the transpose finding
   (`perf-transpose-12`).
3. Model-vs-reference agreement on keysync-0 LFO-active renders (F-4) is
   **unprovable against the current reference** and must never be
   reported as a pass or absorbed into a numeric budget. The two
   committed FAIL rows stay FAIL with the reference-side localization
   until an owner ruling defines the reference behavior (or the wrapper
   is repinned with `Sin::init()`/an explicit LFO initializer, which
   would be a new oracle identity and re-qualification).
4. `pitch.py` bytes are unchanged (catalog pin intact). The F-3 fix lives
   in the N06 composition; if a later issue needs bend support through
   `frame_frequencies` directly, that method must be fixed and
   requalified in its own PR.

## Consequences

- Enables: end-to-end single-note oracle comparison at byte exactness
  (29/39 committed cases EXACT, including every registry single-note case
  and all three pitch-bend cases); honest N07-scope accounting; the
  documented-seam negative controls.
- Forbids: claiming integrated model-vs-reference agreement on
  keysync-0 LFO-active renders; using `render_note` for event-timed
  comparisons; "fixing" F-4 by adopting the observed stack value (that is
  tuning to outputs — barred).
- STALE if changed: `src/gf180_dx7/model/integrated.py`,
  `tools/n06_compare.py`, `tests/test_integrated.py`,
  `reference/evidence/n06-integrated/*`, `docs/N06-INTEGRATED-MODEL.md`.
  N04/N05 artifacts are annotated-scope, not invalidated: N04's
  `render_note` cases and all N05 evidence are bend-free and
  note-live-from-block-0, where the findings do not apply.

## Evidence

- `reference/evidence/n06-integrated/dev32-subset.json` (39 case rows:
  29 EXACT byte-identical, 8 N07_SCOPE, 2 FAIL with F-4 localization;
  per-row first-divergence block and input hashes)
- `reference/evidence/n06-integrated/negative-control.json` (envelope-seam
  stub control: 3/3 FAIL, localization names the stubbed seam)
- Oracle-trace capture for F-2/F-4 (`dx7-oracle --trace`,
  `lfo_value`/`lfo_delay` per block) reproduced by
  `tests/test_integrated.py` live checks where the pinned binary exists.
- Pinned sources: oracle `main.cpp:103-107,444-445`;
  `Source/msfa/lfo.cc:26-97`; `Source/msfa/sin.cc:14-31`;
  `Source/msfa/dx7note.cc:208-296`; `Source/msfa/env.cc:74-97`.
