# N06 — Integrated single-note fixed model (issue #22)

- **Issue:** [#22](https://github.com/2AMLogic/gf180-dx7/issues/22) · epic #2 ·
  planning ID N06
- **Contract:** `gf180-dx7-integer-v1`
  ([spec/numeric-profile-v1.json](../spec/numeric-profile-v1.json);
  DECISIONS-NUMERIC-v1 NUM-001..NUM-012)
- **Model:**
  [src/gf180_dx7/model/integrated.py](../src/gf180_dx7/model/integrated.py)
  (composes N02 `operator.py`, N03 `envelope.py`, N04 `algorithm.py`, N05
  `pitch.py`; all four merged modules are byte-unchanged — their catalog
  pins still verify)
- **Runner:** [tools/n06_compare.py](../tools/n06_compare.py) ·
  **Tests:** [tests/test_integrated.py](../tests/test_integrated.py) ·
  **Evidence:**
  [reference/evidence/n06-integrated/dev32-subset.json](../reference/evidence/n06-integrated/dev32-subset.json),
  [negative-control.json](../reference/evidence/n06-integrated/negative-control.json)
- **Finding record:** [DR-0006](decision-records/0006-n06-integration-findings.md)
- **Result: 29/39 committed cases EXACT (byte-identical, max_abs_error
  0.0) — 23/23 single-note registry cases and 6/11 dev32 entries; 8 cases
  N07_SCOPE (polyphony, mechanically out of the single-note scope); 2
  dev32 FAILs localized to a reference-side indeterminacy (uninitialized
  LFO phase on keysync-0 patches, R05 finding), not to model arithmetic
  and not absorbed into any budget.** Software-reference agreement only —
  not original-DX7 fidelity, not RTL agreement, not a musical claim
  (DEC-001, plan §7).

## 1. Scope and licensing

The comparison target is the pinned external GPL oracle
(`reference/oracle-identity.json`, binary sha256 re-verified before every
render). Its wrapper loop (`main.cpp`, oracle commit `0047c3f5`) and the
R01-pinned Dexed tree (`b6cf2e91`) were read as **semantic specifications
only**: block/event scheduling, controller defaults, voice-slot
bookkeeping, and the Dx7Note::compute order were mirrored as facts with
zero GPL bytes copied (NUM-001; `docs/REUSE-RULINGS.md`). All engine
arithmetic reuses the merged, previously validated module models — N02
(operator kernel + tables + output conversion), N03 (envelopes), N04
(router/feedback), N05 (pitch/EG/LFO/mod) — unchanged.

## 2. Composition order (one 64-sample frame, exact per-block sequence)

For every block `b` (samples `64·b … 64·b+63`), mirroring the pinned
wrapper's render loop:

1. **Events.** Apply, in file order, every event with
   `pos < (b+1)·64` (NUM-011; oracle main.cpp:385-407):
   `note_on` → `State::keydown` (velocity 0 = keyup), `note_off` →
   `State::keyup`, `cc` → controller switch (below), `pb` →
   `values_[kControllerPitch] = lsb | (msb << 7)`.
2. **refreshVoice consumption** (first block only; mirrors
   `Dx7Note::update` for any voice already live in block 0 — reachable
   only for `note_on pos < 64`, which forces `Env::update`'s segment-2
   quirk, env.cc:153-169, mirrored by `Envelope.update`).
3. **Single-note scope check.** More than one live voice not in the
   exact-silence state (§5) ⇒ `PolyphonyExceeded` (N07-scope, never a
   pass).
4. **LFO sample** — `lfo.getsample()` + `lfo.getdelay()` ONCE, live
   voices or not (oracle main.cpp:444-445; finding F-1). The LFO is the
   pinned wrapper's observable machine (`PinnedWrapperLfo`, finding F-2).
5. **Per live voice — `Dx7Note::compute`** (dx7note.cc:208-296):
   1. pitch composition: `pmd = pitchmoddepth_·lfo_delay` (uint32),
      `senslfo = pitchmodsens_·(lfo_val − 2^23)`,
      `pmod = max(|pmd·senslfo >> 39|, |ctrl_pitch_mod·senslfo >> 14|)`,
      `pitch_mod = pitchenv.getsample() + pmod·sign(senslfo)` — the pitch
      EG steps exactly once here;
   2. `pitch_base = pb + master_tune` with the NUM-008 float-gated bend
      scaling `((float)(pb << 11))·range / 12.0` (range up/down 3, step
      0), then `pitch_mod += pitch_base` — **ratio ops see bend + master
      tune** (finding F-5/DR-0006);
   3. amp-mod composition `amd_mod` (dx7note.cc:259-268; ctrl values are
      pinned 0 / eg 127);
   4. op loop in engine-param order 0..5: `freq = Freqlut(basepitch +
      (fixed ? pitch_base : pitch_mod))` (dx7note.cc:256-257, 278-281);
      `level = env.getsample()` (N03, once per frame per op);
      NUM-008 float-gated amp-mod application when `ampmodsens ≠ 0`
      (`exp` on a float argument, dx7note.cc:284-291); `level_in = level`;
   5. `AlgorithmRouter.render` (N04) — gate (`gain1/gain2` vs 16284),
      `has_contents`, feedback forms, per-frame phase skip.
6. **Output conversion per voice** (NUM-010): `>>4` → clip ±2^24 (the
   0x8000 negative-overflow quirk) → `>>9` → `/0x8000`; per-voice
   `clip_val` values mix as integers (the reference float sum is exactly
   the integer sum × 2^-15) and the block stores f32le.

Wrapper state mirrored: 16 voice slots with `currentNote` rotation and
first-non-keydown-slot allocation; `lfo.keydown()` per allocated note_on;
keyup matches the first keydown slot (channel match while mpeEnabled);
sustain pedal (cc 64 >63/< =63) defers `Dx7Note::keyup` to pedal-up;
cc 120 panic (all slots dead + `oscSync`); cc 123 all-notes-off; CC
1/2/4 are render-inert at the pinned FmMod defaults (range 0, all flags
false ⇒ `refresh()` pins pitch/amp mod 0, eg mod 127 — asserted); Dx7Note
state (phases/gain_out/fb_buf) is per-slot and survives slot reuse
(`Dx7Note::init` never resets them; only `oscSync` does, when patch byte
136 is set); one engine (bus) instance per render.

## 3. Event grammar (pinned oracle `events.txt`)

| Command | Args | Effect |
|---|---|---|
| `note_on` | pitch 0-127, velocity 1-127 | allocate slot, `Dx7Note::init`, `lfo.keydown()`, `oscSync` if data[136]; velocity 0 ⇒ keyup |
| `note_off` | pitch | first keydown slot: keydown=false; sustain ⇒ sustained=true, else `Dx7Note::keyup` |
| `cc` | ctrl, value | 1/2/4 → controller state + `refresh()` (render-inert at pinned defaults); 64 → sustain; 120 → panic; 123 → all-notes-off; others ignored |
| `pb` | lsb, msb | `values_[kControllerPitch] = lsb \| (msb << 7)` (14-bit, 0x2000 center) |

Events commit at 64-sample frame boundaries before the containing block
(NUM-011, D00 event semantics); positions are absolute sample offsets.
Covered by the registry `ev-*` cases and the unittest event suite.

## 4. Case suites (issue #22 acceptance)

Runner: `tools/n06_compare.py` (oracle identity re-hashed per run; exact
R04 comparator, dry, no alignment/normalization). Evidence:
`reference/evidence/n06-integrated/dev32-subset.json`.

### 4.1 dev32 subset (R03; ≥ 8 diverse entries required, 11 run)

Entries `[0, 1, 2, 3, 4, 6, 16, 21, 23, 27, 30]` of `corpus/dev32.json`
(voice bytes resolved from the pinned archive, canonical-hash checked;
guarded NOT_RUN when the archive is absent):

| Axis | Coverage |
|---|---|
| Algorithms | 0,1,2,3,4,6,16,21,23,27,30 — includes the directed 1/4/6/23; algorithms 4 and 6 additionally hold 6 registry cases (`alg4-fb*`, `alg6-fb*`), all EXACT |
| LFO waveforms | 0,1,2,3,4,5 (all six) |
| Fixed-frequency operators | entries 1, 2 (1 and 4 fixed ops) |
| Velocity | velocity-sensitive patches at velocity 100 (entries 0,1,2,16,23,…); the three `velocity_contrast` profile entries (3, 6, 30) are N07_SCOPE (below) |
| Pitch EG | entry 1 (present, active), entry 30 |

| dev32 verdicts | count | cases |
|---|---|---|
| EXACT (byte-identical) | 6 | 00, 01, 02, 16, 23, 27 |
| FAIL (reference-side, §6 F-4) | 2 | 04, 21 |
| N07_SCOPE (second live voice, §5) | 3 | 03, 06, 30 |

The mechanical guard, not a pre-judgment, placed 03/06/30: the model
raises `PolyphonyExceeded` at the first block where a previous voice has
not reached the exact-silence floor when a second note_on sounds.

### 4.2 Registry single-note cases (R06 short subset; ≥ 12 required, 23 run)

All 23 single-note-expressible short-subset cases are **EXACT
(byte-identical, max_abs_error 0.0)**: dir-base, osc-fixed-carrier,
bd-coarse0/31, bd-detune0/14, alg4-fb0/3/7, alg6-fb0/3/7, codec-name-a/b,
ev-sustain-no-pedal, ev-sustain-pedal, ev-all-sound-off, ev-all-notes-off,
ev-pitch-bend-up/down/center, ev-modwheel-127/0.

5 short-subset cases are **N07_SCOPE** (explicitly recorded, NOT_RUN,
never passes): ev-repeated-note, ev-retrigger (second live voice while a
predecessor still sounds), ev-sum-15, ev-steal-16, ev-steal-17
(multi-voice summation/stealing). Polyphony is issue #23/N07.

## 5. Single-note scope and the exact-silence continuation

The model renders the pinned wrapper exactly while at most one live voice
is non-silent. A predecessor voice that has reached the NUM-012
exact-silence state — all six envelopes `level_ == targetlevel_ ==
16 << 16`, `ix_ >= 3`, and `gain_out > kLevelThresh (16284)` — contributes
exactly zero forever (Mark I gate, EngineMkI.cpp:314-318), so sequential
notes stay byte-exact across the overlap (the N04 retrigger scenario).
Finding F-4 (DR-0006) amends the profile's predicate reading: a *released*
voice parks with `down_ == false` and `ix_` 3→4 (N03 §7 observed the same
parking), so `down_` is not part of the reclaimable predicate; the
output-neutrality argument is unchanged and no frozen number changes.

## 6. Deviations, findings, and honesty ledger

Per the issue's stop/escalate rule, integration exposed four seam/wrapper
findings; all are recorded in [DR-0006](decision-records/0006-n06-integration-findings.md)
and none was resolved by tuning to outputs.

| ID | Finding | Resolution |
|---|---|---|
| F-1 | The wrapper samples the shared LFO **every block from block 0**, live voices or not; N04's `render_note` gated LFO stepping on note liveness. N04's committed cases (all note-ons at sample 0) were insensitive, so N04 evidence stands; `render_note` is superseded by `IntegratedVoice` for event-timed renders. | mirrored (composition) |
| F-2 | The wrapper's `State.init` never calls `Sin::init()`; msfa's zero-initialized `sintab` makes the wave-4 LFO output the constant `1 << 23` (oracle-trace evidence: `lfo_value == 8388608` on every block of dev32-01). | mirrored via `PinnedWrapperLfo` (`lfo_mode="pinned_wrapper"`, the comparison default); the NUM-007 documented sine-table behavior remains available (`lfo_mode="documented"`) |
| F-3 | `NotePitch.frame_frequencies` (N05) drops `pitch_base` from the **ratio-mode** branch — its declared scope excluded bend renders ("full bend renders stay with N08"). dx7note.cc adds `pitch_base` into `pitch_mod` before the op loop, so ratio ops DO take bend + master tune. | `IntegratedVoice.compute_frame` composes the pinned order from N05's public pieces (`pitch_mod_per_frame` + `_pitch_base` + `Freqlut`); `pitch.py` bytes untouched; N05's method stays valid for bend-free renders (all committed N05/N02/N04 evidence is bend-free and unaffected) |
| F-4 | msfa `Lfo` leaves `phase_` uninitialized; with `lfo_key_sync = 0` nothing ever resets it, so keysync-0 renders carry C++-indeterminate stack residue (R05 finding; stable per binary, not a derivable semantic). dev32-04 and dev32-21 (keysync 0, LFO reaches PCM) **FAIL against that property** with first divergence + this localization. | reported, never budgeted: the model cannot match an undefined reference value by construction; documented semantics stay NUM-007. Model-vs-reference agreement on keysync-0 LFO-active renders is explicitly **unproved** (would require an owner ruling pinning a defined reference behavior) |
| F-5 | The profile's NUM-012 reclaimable predicate reads `down_ == true ∧ ix_ == 3`; a completed release parks at `down_ == false`, `ix_` 3→4 (N03 §7 noted the `ix_ == 4` parking). | model predicate uses `level_/targetlevel_/ix_/gain_out` only; output-neutrality argument unchanged; no frozen number changes |

Negative control (committed, `negative-control.json`): with the documented
`envelope` seam stubbed to a constant output, the case set
(dir-base, ev-pitch-bend-up, ev-all-sound-off) **fails 3/3** and every row
localizes to the stubbed seam (`control_fired: true`). The `lfo` stub seam
exists for the same purpose and is exercised by the unittest suite.

Measured budgets: EXACT (0.0) on all 29 exact rows — integration
introduced no numeric deviation beyond the two reference-side findings
above; there are no tolerated errors anywhere in this issue.

## 7. What remains unproved (honesty ledger)

- **Unproved:** model-vs-reference agreement on keysync-0 LFO-active
  renders (dev32-04, dev32-21) — the reference value is C++-indeterminate
  (F-4); needs an owner ruling (define the reference behavior) before it
  can even be attempted.
- **Out of scope (N07):** polyphony — repeated/overlapping notes,
  15/16-voice summation, stealing (8 recorded N07_SCOPE rows).
- **Not claimed:** RTL agreement (H-series), original-DX7 fidelity,
  preset quality, musical usefulness (listening records only, plan §7).
  Dexed agreement is not DX7 fidelity; the pinned wrapper's F-2 behavior
  is *not* claimed to match original hardware.
- Amp-mod sensitivity path: no committed case in this suite exercises
  `amp_mod_sensitivity ≠ 0` **with a non-inert LFO** reaching it (the
  NUM-008 NO_VERDICT stands for that combination; the code path is
  mirrored and unit-covered at composition level).

## 8. Status

- Model + runner + tests: **PASS** (30 unittest cases; committed evidence
  29 EXACT / 8 N07_SCOPE / 2 localized reference-side FAIL).
- Live oracle checks re-render on every test run where the pinned binary
  is present; guarded NOT_RUN otherwise (never a silent pass).
- CI without oracle/archive: dev32 rows NOT_RUN (guarded), pure sections
  still run.
