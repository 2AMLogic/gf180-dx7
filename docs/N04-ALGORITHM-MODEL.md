# N04 — Integer algorithm/feedback model (issue #18)

- **Deliverable:** one integer model of all 32 algorithm graphs, carrier
  sums, and feedback history, validated with scripted operator sources plus
  reference renders (issue #18).
- **Contract:** `gf180-dx7-integer-v1`
  ([spec/numeric-profile-v1.json](../spec/numeric-profile-v1.json),
  DECISIONS-NUMERIC-v1 NUM-006, NUM-009, NUM-010, NUM-011); N01 frozen
  arithmetic; N02 operator kernel (`model/operator.py`, untouched); N03
  envelope (`model/envelope.py`); N05 pitch path (`model/pitch.py`).
- **Model:** `src/gf180_dx7/model/algorithm.py` (router + `render_note`) ·
  **Probes:** `tests/test_algorithm.py` · **Evidence:**
  `reference/evidence/n04-algorithm/algorithm-sweep.json`.
- **Result: EXACT — 42/42 rendered cases byte-identical against the pinned
  oracle (max_abs_error 0.0), covering all 32 algorithms, the algorithm 4/6
  serial feedback forms at feedback 0/3/7, feedback-off paths, gating /
  has_contents cases, a retrigger, and a release through the serial
  feedback loop.** Declared budget: exact; every integer operation is
  mirrored over byte-identical tables (N02), so any nonzero error would be
  a model defect, not a numeric budget.

## 1. Scope and licensing

The pinned reference engine is Mark I (R01: `DEXED_ENGINE_MARKI`). The
GPL-3.0 file `Source/EngineMkI.cpp` and the Apache-2.0
`Source/msfa/fm_core.cc` in the pinned Dexed tree (`b6cf2e9`, R01) are the
**semantic specification only**: the 32-row connection table values, decode
rules, gate constants, dispatch order, and feedback formulas were extracted
as facts and re-implemented in clean stdlib Python. Zero GPL bytes are
copied (NUM-001; `docs/REUSE-RULINGS.md` `dexed-enginemki`). The two msfa
interconnect buses are behavior facts (a 2×64 int32 state), not code.

## 2. The connection table (facts, with citations)

`fm_core.cc:29-62` packs each of the 32 rows as six op descriptors
(engine param order `params_[0..5]`; the unpacked patch block k is engine
param k, i.e. `Voice.operators[5-k]` through the P01 codec,
`sysex.py` encode order). Decode (`EngineMkI.cpp:309-313`):

```
inbus  = (flags >> 4) & 3      // 0 = no input bus, 1/2 = buf_[0]/buf_[1]
outbus = flags & 3             // 0 = note output, 1/2 = buf_[0]/buf_[1]
add    = flags & 0x04          // accumulate onto destination
fb op  = (flags & 0xc0) == 0xc0   // FB_IN | FB_OUT: exactly one per row
```

`tests/test_algorithm.py::TestConnectionTable` asserts the model's
`ALGORITHMS` table, a mechanically extracted transcription of the citation,
against a **hand-derived wiring table** (independent encoding of each op's
dataflow role); both encodings must agree, and exactly one feedback op per
algorithm is asserted. Process note (honesty record): the first draft of
this work transcribed ALG 23 as `{c1,14,14,14,01,04}`; the scripted suite's
divergence from the oracle localized the error to that one row (pinned:
`{c1,14,14,01,14,04}`), which is precisely the failure the double-encoding
guard exists to catch. Both encodings now match the pinned file
mechanically (checked entry-by-entry) and behaviorally (§5).

## 3. Routing semantics (facts, EngineMkI.cpp:295-363)

Per 64-sample frame, ops are processed strictly in param order 0..5, with
the algorithm 4/6 special cases advancing the loop index (below):

1. `gain1 = gain_out == 0 ? ENV_MAX-1 : gain_out` (:314) — zero-gain
   substitution; `gain2 = ENV_MAX - (level_in >> 14)` (:315);
   `gain_out = gain2` stored for the next frame (:316).
2. **Mark I gate** (NUM-009): the op renders only when
   `gain1 <= ENV_MAX-100 (16284) || gain2 <= 16284` (:296,318). A gated op
   still advances its phase (:361) and, if `add` is not set, marks its
   output bus empty (:358-360).
3. `if (!has_contents[outbus]) add = false` (:320-322) — accumulation onto
   a bus that nothing wrote this frame is forced to overwrite (the buses
   hold stale data from previous frames; they are never cleared).
4. `inbus == 0 || !has_contents[inbus]` (:324): the op renders without
   input — `compute_pure`, or `compute_fb` when `(flags & 0xc0) == 0xc0`
   and feedback is on (:326).
5. Otherwise `compute` reads the input bus per sample (:354).
6. Every destination write is `output[i] = y + adder[i]` with
   `adder = add ? destination : zeros` (:158,163,179,199); the note output
   buffer itself is zeroed per frame per note (wrapper audiobuf,
   PluginProcessor.cpp:247-249), so "carrier sum" is a plain int32
   accumulation in op order.
7. Frame-boundary skip: `param.phase += param.freq << LG_N` (:361,
   fm_core.cc:133) — for the op bound at loop entry, always, including
   gated ops and the fb-loop op.

**Carrier-sum order is not observable in the frozen integer domain:**
addition is associative/commutative modulo 2^32 and there is no
saturation, so reordering the final sum cannot change a byte. The
observable sum-order facts are the *bus sequencing* (later ops overwrite
earlier bus contents) and the `has_contents` add-suppression, both mirrored
above and probed by the scripted battery. This is why the negative control
for "swapped carrier-sum order" (§6) corrupts the accumulation to
last-writer-wins — the smallest observable corruption of the sum semantics
— instead of a pure reorder, which is provably a no-op.

## 4. Feedback semantics (facts)

- Depth mapping: `fb_shift = feedback != 0 ? 8 - feedback : 16`
  (dx7note.cc:197; `FEEDBACK_BITDEPTH = 8`, dx7note.cc:28). `fb_on` is
  `feedback_shift < 16` (EngineMkI.cpp:299): at feedback 0 every fb op
  takes the pure form and the two-element history is neither read nor
  written.
- History: two int32 elements per note (`fb_buf_[2]`, dx7note.h:77),
  read at frame start, written back at frame end inside the fb forms
  (EngineMkI.cpp:203-204,242-243,291-292). Per sample
  `scaled_fb = (y0 + y) >> (fb_shift + 1)` (:196,227,271) — arithmetic
  (floor) shift. Direct history depth is two samples: a one-frame impulse
  injected into the history appears in exactly the first two samples'
  feedback terms (`test_history_depth_two_direct_reads`), then only
  through recirculation (`test_recirculation_values`).
- One feedback loop per note (the single `(flags & 0xc0) == 0xc0` op); the
  history is per note and **not** reset by note-on init (no such line in
  `Dx7Note::init`, dx7note.cc:150-200; `oscSync`, dx7note.cc:398-403,
  resets phases/gain_out only).
- **Algorithm 4 / 6 inline serial forms** (EngineMkI.cpp:301-305,327-340):
  with feedback on, op0's flags are patched to `0xc4` (route to the note
  output) and ops 0..2 (ALGO 4, `compute_fb3`, :247-293) / ops 0..1
  (ALGO 6, `compute_fb2`, :208-244) render as an in-sample serial chain
  op0(fb) -> op1 [-> op2] -> output, with the shift `min(fb_shift+2, 16)`
  (:330,337) — 4× less feedback than the generic path. The consumed ops'
  phases advance exactly one frame via the explicit skips (:331-332,338)
  plus :361 for op0; the loop index skips them. The `compute_fb2/3` gain
  handling has pinned quirks mirrored exactly: the downstream ops' gains
  use the full-delta ramp `gain_out - gain1` without `>> LG_N`
  (:224,266-267 — normally zero), and `parms[1]/[2].gain_out` are written
  inside the forms (:218,258-259).
- **Algorithm 32** also takes `min(fb_shift+2, 16)` (:341-344); every
  other algorithm's fb op uses the plain shift (:345-348).
- With feedback 0, ALGO 4/6 render their generic table rows (no serial
  form; the `0xc4` patch only happens when `fb_on`), and the rows are
  self-consistent chains — there is no fb-off quirk in the pinned table.

## 5. Reference sweep (acceptance: feedback/routing match reference traces)

- **Voice:** one synthetic six-operator patch per case — rates/levels 99
  (envelopes park at a constant target from frame 1, N02 analytics), osc
  coarse 1 / fine 0 / detune 7 (neutral), all LFO/pitch-EG/mod bytes
  neutral so the pitch path is constant, `osc_key_sync` 1 (note-on resets
  phases and `gain_out` exactly like the wrapper, dx7note.cc:398-403),
  `note_on 60 100` at sample 0. Per-case overrides: algorithm byte,
  feedback byte, envelope-silent operators (TL 0 -> 16<<16 clamp floor ->
  gate-skipped, exercising has_contents fallbacks), events.
- **Runs:** the pinned oracle binary (SHA-256 re-verified per run) vs
  `model.render_note(...)`; compared dry with
  `src/gf180_dx7/compare.py` (exact-time-locked, no alignment). Event
  policy: every event with position inside a block is applied before that
  block (oracle main.cpp:385-407; NUM-011).
- **Per-algorithm agreement (all EXACT, byte-identical, max_abs_error
  0.0; hashes per case in the evidence JSON):**

  | Case group | Cases | Result |
  |---|---|---|
  | Algorithms 1..32, feedback 7, 2 s | 32 | 32/32 EXACT |
  | ALG 4 (serial fb3) fb 0 / fb 3 | 2 | 2/2 EXACT |
  | ALG 6 (serial fb2) fb 0 / fb 3 | 2 | 2/2 EXACT |
  | ALG 6 fb 7 with note_off release at 1 s | 1 | 1/1 EXACT |
  | ALG 8 / ALG 32 with feedback 0 (fb-off generic path) | 2 | 2/2 EXACT |
  | ALG 1 gate op0 / ALG 16 gate op2 (has_contents) | 2 | 2/2 EXACT |
  | ALG 32 fb 7 retrigger (note_off/note_on, fresh Dx7Note slot,
    shared engine buses, release-to-floor silence NUM-012) | 1 | 1/1 EXACT |

  Per-algorithm sweep detail (feedback 7, 2 s, dry f32le; `EXACT` =
  byte-identical, max_abs_error 0.0):

  | ALG | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
  |---|---|---|---|---|---|---|---|---|
  | result | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT |

  | ALG | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
  |---|---|---|---|---|---|---|---|---|
  | result | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT |

  | ALG | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 |
  |---|---|---|---|---|---|---|---|---|
  | result | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT |

  | ALG | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 |
  |---|---|---|---|---|---|---|---|---|
  | result | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT | EXACT |

  No deviations were accepted or declared: a per-case deviation row would
  name its measured error and localization (none exists).

  Retrigger note: the wrapper allocates the next voice slot per keydown
  (PluginProcessor.cpp:482-490), so the second note starts from
  constructor state (phase/gain_out zeros, heap-fresh fb_buf) while the
  engine's buses persist; `render_note` mirrors exactly that single-note
  scenario (polyphonic slot allocation is H03's).

- Status: **PASS** (42/42 EXACT). These are software-reference agreements
  only — not original-DX7 fidelity, not RTL agreement, not a musical claim.

## 6. Scripted-source validation and negative controls

Scripted battery (runs everywhere, no oracle): the router is driven with a
seam that replaces `mkiSin` by known modulation-sensitive sequences
`y = base[op] + t*step[op] + 3*mod_term`; the test records the full call
sequence `(op, t, mod_term)` plus master outputs, final history, and per-op
phase bookkeeping, and compares against the hand-derived wiring evaluator
for **all 32 algorithms x {feedback 0, feedback 7} x 3 frames**, plus six
gating/has_contents scenarios (empty-input-bus fallback, add-forced-off on
empty output bus, gated fb op with the ALG 4/6 `0xc4` patch semantics).

Negative controls — each demonstrably fails the check it targets
(`TestNegativeControls`):

1. **Operator-index permutation** (issue #18's control): a permuted params
   view makes the router gate and state-load the wrong operators; the
   scripted battery fails broadly (assert >= 28 of 32 algorithms fail;
   observed 32/32 with distinct per-position envelope levels).
2. **One-sample-broken feedback history** (reads delayed by one whole
   64-sample block via a proxy buffer): fails all 8 probed feedback cases
   (assert exact).
3. **Carrier-sum corruption** (last-writer-wins, the observable corruption
   of sum-order semantics per §3): fails all 9 probed multi-carrier
   algorithms (assert exact).

The battery is first asserted to pass on the clean router, so every mutant
failure is attributable to its injected defect.

## 7. What remains unproved (honesty ledger)

- This verifies **model vs pinned software reference** only. It is not RTL
  agreement (H05), not original-DX7 fidelity, and says nothing about
  musical usefulness (listening records only).
- Dexed's effective graphs are Dexed's; a canonical Yamaha-chart
  comparison of all 32 graphs (and the Dexed-specific fb-shift mapping)
  against a second oracle (VDX7/Hexter) is **not** settled here. The
  existing cross-engine findings (docs/R07-DISAGREEMENT.md: feedback
  mapping/history differ between Dexed and hexter) already show the
  engines disagree on feedback semantics; per the issue's stop/escalate
  rule, any chart-vs-Dexed disagreement stays an R07 case, and this model
  follows the pinned profile (Dexed Mark I) exactly.
- The amp-modulation sensitivity branch inside `NoteState.compute_frame`
  (dx7note.cc:284-291, float-gated NUM-008 spot) is mirrored structurally
  but never exercised by an N04 case (all voices carry amp_mod_sensitivity
  0); it stays NO_VERDICT and is N06's to measure.
- `render_note` covers the single non-overlapping note scenario
  (constructor-fresh per-note state + one shared engine instance);
  multi-note polyphony, voice stealing, and mono-mode transfer are out of
  N04 scope (H03/H06).
- Envelope, pitch, and LFO sub-paths are exercised here only through their
  N03/N05-validated surface; their own budgets and caveats carry over
  unchanged.
