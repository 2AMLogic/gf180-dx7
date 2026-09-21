# N07 — Polyphonic state/event manager (issue #23)

- **Issue:** [#23](https://github.com/2AMLogic/gf180-dx7/issues/23) · epic #2 ·
  planning ID N07
- **Contract:** `gf180-dx7-integer-v1`
  ([spec/numeric-profile-v1.json](../spec/numeric-profile-v1.json);
  DECISIONS-NUMERIC-v1 NUM-001..NUM-012)
- **Policy:** [docs/DECISIONS-v1.md](DECISIONS-v1.md) §3
  (DEC-014..DEC-023 — the D00 event policy is the model semantics)
- **Model:** [src/gf180_dx7/model/poly.py](../src/gf180_dx7/model/poly.py)
  (16 `PolyVoiceSlot`s around the N06 composition; `integrated.py` and all
  merged module models byte-unchanged)
- **Runner:** [tools/n07_compare.py](../tools/n07_compare.py) ·
  **Tests:** [tests/test_poly.py](../tests/test_poly.py) ·
  **Evidence:**
  [reference/evidence/n07-poly/evidence.json](../reference/evidence/n07-poly/evidence.json)
- **Result: 7/7 wrapper-valid cross-check cases EXACT (byte-identical,
  max_abs_error 0.0) — the 4 N06-punted registry cases that the wrapper can
  express without stealing (ev-repeated-note, ev-retrigger, ev-sum-15,
  ev-steal-16) and 3 dev32 `velocity_contrast` entries (primary: dev32-03);
  1 DOCUMENTED POLICY DIFFERENCE (ev-steal-17) measured and classified,
  never budgeted. D00 policy battery, isolation, and both negative controls
  PASS in the unittest suite (39 cases).** Software-reference agreement
  only — not original-DX7 fidelity, not RTL agreement, not a musical claim
  (DEC-001, plan §7).

## 1. Scope and the two semantic layers

One manager, sixteen independent voice slots, one active timbre (DEC-003).
Two layers are kept deliberately separate and never conflated:

1. **Model semantics = D00 policy** (docs/DECISIONS-v1.md §3). The manager
   implements the frozen product contract: allocation, retrigger, sustain,
   patch binding, stealing, reclamation, all-sound-off.
2. **The pinned wrapper is a comparison oracle with narrower semantics.**
   Its allocator drops the 17th note (no stealing — registry finding
   `alloc-17th-drop`, [docs/REGISTRY-POLICY.md](REGISTRY-POLICY.md) §5,
   re-confirmed live in the committed evidence), and its `keyup` matches
   the first keydown slot by CHANNEL while `mpeEnabled` (flipping to
   pitch-matching only after a same-channel overlap flips the MPE flag —
   the "MPE-flip" semantic, render-inert at the pinned controller
   defaults). Comparison against the oracle is therefore **valid only for
   ≤16-voice cases that trigger no stealing and where the two note-off
   matchings coincide**. Engine arithmetic itself mirrors the wrapper
   exactly (proven at N06 and re-proven here on multi-voice renders).

Engine computation is not duplicated: each slot owns a `NoteState` (N02–N05
facts) and the per-frame `Dx7Note::compute` composition is
`IntegratedVoice.compute_frame` applied per slot; the block loop commits
events at 64-sample frame boundaries (DEC-014/NUM-011), samples the shared
LFO once unconditionally (finding F-1), and mixes per-voice `clip_val`
values in an exact integer sum (NUM-010).

## 2. Policy mapping to D00

| D00 | Implementation | Notes |
|---|---|---|
| DEC-014 | events commit at frame boundaries; queue = the block event list; nothing silently dropped | NUM-011 cadence |
| DEC-015 | `PolyVoiceSlot.retrigger`: every operator envelope AND the pitch EG re-leg from their **current level** (forced key-down edge: `down_ = False` then `keydown(True)`, i.e. env.cc segment-0 from `level_`); derived params recomputed for the new midinote/velocity; phases and fb history carried | "reinitializes per-note phase per DEC-020" = the frozen model's reuse path keeps them; a patch with osc-sync SET still zeroes them per the pinned `oscSync` (dx7note.cc:398-403) — the patch's own restart control |
| DEC-016 | free voice first; **only when no free voice exists**, a note-on for an already-sounding pitch reuses the **oldest matching** voice (retrigger per DEC-015) | see the reading note below |
| DEC-017 | steal order: (1) voices in release (key-up received, not sustain-held), (2) oldest sounding voice (earliest still-present note-on sequence); deterministic, surfaced in `alloc_log` entries (`kind`, `victim_slot`, `victim_midi_note`, `victim_seq`); never random | `steal_policy="newest"` exists only as the negative-control mutator |
| DEC-018 | sustain (CC 64 >63/<=63): note-off under the pedal → sustain-hold (keeps sounding); pedal-up → normal release; pedal-up with nothing held changes nothing; CC 120 overrides | exact wrapper semantics coincide with DEC-018 |
| DEC-019 | `patch <n>` event (this model's grammar extension; the oracle cannot express mid-render program change — registry `ev-patch-change-held` is BLOCKED there) commits atomically at the frame boundary; sounding voices keep the patch they were struck with (release tails included); new note-ons bind the current patch; the chip-global LFO reconfigures from the new patch's LFO bytes without a phase reset | patch is bound per strike; verified by additive renders against per-patch solo renders |
| DEC-020 | LFO free-running except the patch's key-sync (`lfo.keydown()` per strike, as the frozen N06 comparison model does); per-note phases free-running | |
| DEC-021/NUM-012 | no arbitrary tails: a voice whose six envelopes sit in the exact-silence state (level_ = targetlevel_ = 16<<16, ix_ ≥ 3, gain_out > 16284; N06 finding F-5 reading) for **64 consecutive frames** is reclaimed at the frame boundary (output-neutral); the only forced kill is CC 120 | reclaimed slots re-enter service with NUM-012-frozen phase/fb state (`alloc_log` kind `reuse`) |
| DEC-023 | transpose/scaling/velocity follow the frozen N06 composition unchanged (patch transpose bytes ignored per the pinned-wrapper finding `perf-transpose-12`, exactly like every committed N02–N06 case) | |

**DEC-016 reading (recorded decision, not improvised policy).** DEC-016's
two clauses — *"when a note-on arrives for a pitch that already sounds and
no free voice exists, steal the oldest matching voice"* and *"a same-pitch
note never allocates a new voice while a matching one can be reused"* —
admit two readings. The implemented reading makes clause 1 the rule (reuse
requires exhausted polyphony; with a free voice a repeated same-pitch note
stacks a second voice) and clause 2 its rationale: under exhaustion the
matching voice is the preferred steal target. Grounds: (a) the alternative
(always reuse the matching voice) would diverge from the pinned wrapper on
`ev-repeated-note`, `ev-retrigger` and the dev32 sequential-note cases
with no DEC mandate, contradicting their wrapper-valid treatment; (b) it
preserves DEC-015 as the mechanics of reuse. The alternative reading is
recorded here and would require a DR to adopt. No other policy hole was
found; the stop/escalate condition was not triggered.

## 3. Isolation methodology (no cross-voice state leakage)

Structural argument: one `AlgorithmRouter` instance is shared by all
sixteen slots, exactly like the pinned wrapper's single engine
(`controllers.core = &engineMkI`). The router's two interconnect buses are
persistent members, but `has_contents` is **render-local** and starts
`false` on both buses, and a bus is only read when a same-frame earlier
operator of the same voice rendered to it — so every bus word a voice reads
it wrote itself that frame. Cross-voice bus leakage is structurally
impossible, and the empirical checks below would catch it if that argument
were wrong.

Empirical checks (tests/test_poly.py, `TestOverloadDeterminismAndIsolation`):

1. **Exact integer additivity at the NUM-010 clip domain**: render A alone,
   B alone, A+B — the mixed clip stream must equal `clip_A + clip_B`
   sample-exactly (integer arithmetic; sum order is irrelevant, so A's
   contribution is bit-identical pre-clip).
2. **Bit-identical prefix discipline**: before B's onset, A+B is
   byte-identical (f32le) to the A-alone render.
3. Per-voice `clip_val` clipping is the **only** saturation: clipping never
   couples voices (it happens per voice before the mix).

**Measured pinned mixing semantics (finding N07-F1).** The pinned wrapper
stores the summed mix **unclamped**: an oracle ev-sum-15 render
(oracle_pcm_sha256 `aa3f0ebd0df0139bba2d2c454445487fca570f037a3ae5794bd8d4048cb2824a`)
peaks at 1.317626953125 (> ±1.0; corrected per judge review on #63 — the
previously recorded 1.0985 did not reproduce). The model therefore converts
the integer mix as
`sum × 2⁻¹⁵` raw (the 0x8000 quirk is +32768 in the integer domain and maps
to +1.0 per voice exactly as `c / 32768.0`). The N06 single-note path never
exercised this (one voice ⇒ |f| ≤ 1) and is unchanged. This is the
"clipping couples voices" corner anticipated by the issue brief — resolved
by measurement: it does not couple voices, but the sum itself is not
clamped, and the model reproduces that.

## 4. Wrapper-valid cross-checks (committed evidence)

Runner `tools/n07_compare.py` re-verifies the oracle binary hash before any
render; comparator is the exact R04 paired comparator (dry, no alignment or
normalization). All rows EXACT, byte-identical, max_abs_error 0.0:

| Case | Why wrapper-valid | Verdict |
|---|---|---|
| ev-repeated-note | 2 live voices, 14 free; no stealing; no note-off | EXACT |
| ev-retrigger | 2 live voices (release tail + rekey); no stealing; single keydown at each note-off (matchings coincide) | EXACT |
| ev-sum-15 | 15 live voices; no stealing; note-offs in slot=pitch order | EXACT |
| ev-steal-16 | 16 live voices — the boundary; the wrapper still allocates every note | EXACT |
| dev32-03 (primary), dev32-06, dev32-30 | `velocity_contrast`: the second strike lands during the first voice's release tail (2 live voices) | EXACT |

## 5. The documented divergence: 17th note (D00 vs the wrapper)

`ev-steal-17` is rendered on both sides and recorded as a **policy
difference, not a defect and not a budgeted equality** (evidence section
`policy_divergence`):

- **Wrapper:** drops the 17th note; its render is byte-identical to the
  16-note render (`alloc-17th-drop` re-confirmed live:
  `wrapper_alloc_17th_drop_confirmed: true`).
- **Model (D00 DEC-017):** steals deterministically — the oldest held voice
  (slot 0, pitch 60) is re-struck with pitch 76 (`alloc_log` steal entry
  retained in the evidence).
- **Measurement:** first divergence at sample 3968 (block 62 — the 17th
  note's block), max_abs_error 0.345, 92004 mismatching samples of 96000.

No equality budget is declared on this case, and none may be inferred from
it. Reverting to wrapper behavior here would weaken the DEC-003/DEC-017
product policy and require a DR.

## 6. Negative controls (live)

| Control | Seam | Must fail | Check |
|---|---|---|---|
| Cross-voice state sharing | `negative_control_state_share=(0, 1)` aliases two slots onto one `NoteState` | isolation additivity | `test_state_share_seam_breaks_isolation` (and `test_clean_manager_passes_the_same_check` proves the check fires only under the seam) |
| Stealing-policy mutator | `steal_policy="newest"` steals the newest instead of the DEC-017 oldest | the 17th-note battery assertion, raising a message naming **DEC-017 / case ev-steal-17** | `test_steal_newest_mutator_fails_battery_naming_case` + `test_steal_newest_mutator_named_for_what_it_breaks` |

## 7. What remains unproved (honesty ledger)

- **Unproved:** wrapper agreement for arbitrary >16-note sequences other
  than the recorded divergence, and for any case where the wrapper's
  channel/MPE-flip note-off matching would pick a different voice than
  pitch matching — such cases are outside the wrapper-valid boundary by
  construction (the D00 policy is the product semantics there).
- **Not claimed:** RTL agreement (H04/H07 own the manager in RTL),
  original-DX7 fidelity, preset quality, musical usefulness (listening
  records only, plan §7). Dexed agreement is not DX7 fidelity.
- **Inherited unproved:** model-vs-reference agreement on keysync-0
  LFO-active renders (N06 finding F-4); none of the committed N07 cases
  exercises that combination.
- Reclamation (NUM-012) is asserted at state level and its output-neutrality
  is inherited from the N06 gate argument; no dedicated oracle render of a
  mid-render reclamation exists in this evidence (the wrapper never
  reclaims; comparison there is structurally impossible).

## 8. Status

- Model + runner + tests: **PASS** (39 unittest cases; committed evidence
  7 EXACT / 1 documented policy difference).
- Live oracle checks re-render on every test run where the pinned binary is
  present; guarded NOT_RUN otherwise (never a silent pass).
- CI without oracle/archive: dev32 rows NOT_RUN (guarded), pure sections
  still run.
