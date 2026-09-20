# DECISIONS-v1 — frozen v1 product contract and fidelity profile

- **Contract version:** 1
- **Frozen:** 2026-09-19
- **Source of defaults:** `docs/dx7-chip-plan-v0.1-2026-09-20.md` sections 3 and 11
  (plan-proposed defaults), tightened by issue #4 (normative where it tightens the plan).
- **Machine-readable companion:** [`spec/contract-v1.json`](../spec/contract-v1.json)
  (kept in lockstep with this document by `tests/test_contract_v1.py`).
- **Adoption record:** [DR-0001](decision-records/0001-product-contract-v1.md) —
  Accepted (owner-directed backlog execution, 2026-09-19).

## 0. Status vocabulary and change control

- **Accepted (plan default)** — the plan's proposed default (section 3 or 11) is
  adopted as the v1 contract.
- **Accepted (owner-ruled)** — already ruled by the owner at repo level; recorded
  here for completeness, not re-decided.
- **Proposed (owner may veto)** — the plan flags this as taste/policy; the recorded
  default stands for planning purposes until the owner rules otherwise.

Every decision below carries a `DEC-NNN` id. **Changing any decision requires a new
decision record (DR) in `docs/decision-records/` and STALE marking of all dependents**
(backlog leaves, manifests, tests, and `spec/contract-v1.json` entries that cite the
changed id). No silent edits; a weakened product goal is never a substitute for a
recorded ruling.

Three claims stay separate and are never inferred from one another (plan section 7):
(1) the RTL matches the frozen integer model exactly; (2) the model reproduces the
pinned software reference within declared budgets; (3) the instrument sounds good.

## 1. Fidelity profile

### DEC-001 — Named fidelity profile `dx7-compatible-v1`

- **Status:** Accepted (plan default, section 3 "Fidelity profile")
- **Decision:** The v1 profile is **`dx7-compatible-v1`**: musically convincing DX7
  compatibility qualified against a **pinned Dexed Mark I** comparison target
  (exact commit, engine mode, sample rate, and runtime pinned by R01 before any
  render), frozen performance settings, known numeric deviations, and a separate
  frozen integer model for exact RTL verification. The pinned reference is never
  silently followed upstream; source drift is rejected. **Dexed agreement is not
  original-DX7 fidelity.**
- **Rationale:** Plan section 3 "Fidelity profile" and section 11 choice 1. A pinned
  executable target makes development reproducible; it does not make Dexed the final
  authority on Yamaha hardware (plan section 5).
- **Change control:** Changing the profile requires a new DR; STALE-mark R01, R02,
  R04, R07, N08, and every fidelity statement in README/AGENTS-facing documents.

## 2. Frozen contract table (every row of plan section 3)

### DEC-002 — Synthesis

- **Status:** Accepted (plan default, section 3 row "Synthesis")
- **Decision:** Six sine operators per note; **all 32 DX7 algorithms**, with
  documented feedback behavior including the self-feedback structures of
  **algorithms 4 and 6**.
- **Rationale:** Plan section 3 row "Synthesis"; the verification contract (section 7)
  names algorithms 4/6 as directed feedback cases.
- **Change control:** New DR required; STALE-mark N02/N04/N05, H04/H05, R06.

### DEC-003 — Polyphony and timbre

- **Status:** Accepted (plan default, section 3 row "Polyphony"; section 11 choice 3)
- **Decision:** **16 simultaneous notes, one active timbre.** One-note
  implementations are development milestones, not the product. Polyphony is never
  quietly reduced to make a task pass; an area-driven reduction would be an explicit
  product change requiring a new DR and owner sign-off.
- **Rationale:** Plan section 3 row "Polyphony"; section 11 choice 3 ("a reduction in
  polyphony is an explicit product change").
- **Change control:** New DR required; STALE-mark N07, H07, D01, H02, H10 and the
  worst-case schedule figures (16 × 6 × 48,000 operator evaluations/s).

### DEC-004 — Presets — **Proposed (owner may veto) for curated bank contents**

- **Status:** Accepted for structure (plan default, section 3 row "Presets");
  **Proposed (owner may veto)** for the exact curated contents.
- **Decision:** An initial **32-patch listening bank** expanding to a curated
  **128-patch shipping bank**; larger archive import/search happens on the host.
  Bank *contents* are a taste choice: the categories of plan section 6 (electric
  pianos/tines, basses, bells/mallets, organs, brass/reeds, plucked sounds,
  pads/strings, unusual percussion/textures) are the recorded default starting
  distribution, subject to owner veto. Preset quality is never claimed from numeric
  tests; listening records are required (plan section 7).
- **Rationale:** Plan section 3 row "Presets", section 6, and section 11 choice 4.
- **Change control:** Contents changes need a new DR (or owner ruling recorded);
  STALE-mark R03, U02, U03.

### DEC-005 — Voice data

- **Status:** Accepted (plan default, section 3 row "Voice data")
- **Decision:** DX7 **single-voice and 32-voice bank import/export** with explicit
  support boundaries. DX7II/performance formats are supported only when explicitly
  declared; a filename never implies compatibility (P01 premise).
- **Rationale:** Plan section 3 row "Voice data" and the P01 brief (section 9).
- **Change control:** New DR required; STALE-mark P01, P02, R06.

### DEC-006 — Performance controls

- **Status:** Accepted (plan default, section 3 row "Performance")
- **Decision:** v1 responds to: **note on/off, velocity, sustain, pitch bend,
  modulation wheel, channel aftertouch, volume, and all-sound-off.** Remaining
  original function modes are **tracked explicitly** (machine-readable coverage
  holes in R06), never silently unsupported.
- **Rationale:** Plan section 3 row "Performance".
- **Change control:** New DR required; STALE-mark N05, N07, H06, R06.

### DEC-007 — Sound semantics

- **Status:** Accepted (plan default, section 3 row "Sound semantics")
- **Decision:** Full DX7 voice-parameter semantics: operator rate/level envelopes,
  output level, velocity sensitivity, keyboard level/rate scaling, fixed/ratio
  frequency, coarse/fine/detune, transpose, oscillator sync, pitch envelope, LFO
  waveform/rate/delay/sync and sensitivities. No supported parameter silently falls
  back to a default (N05 acceptance).
- **Rationale:** Plan section 3 row "Sound semantics".
- **Change control:** New DR required; STALE-mark N02–N05, R05, R06.

### DEC-008 — Audio

- **Status:** Accepted (plan default, section 3 row "Audio")
- **Decision:** **Continuous dry mono synthesis**, carried over an explicitly
  specified **I2S format to an external DAC**; optional duplication to both stereo
  slots. Rendering for comparisons is always dry (see DEC-012).
- **Rationale:** Plan section 3 row "Audio".
- **Change control:** New DR required; STALE-mark H07, H08, H09, R02.

### DEC-009 — Working sample rate: 48 kHz

- **Status:** Accepted (plan default, section 3 row "Working sample-rate default")
- **Decision:** All reference rendering, the frozen integer model, and the product
  audio path default to **48,000 Hz**. The selected reference (pinned Dexed Mark I)
  is configured **identically**; any residual rate conversion would be explicit and
  declared in R01, never implicit.
- **Rationale:** Plan section 3 row "Working sample-rate default" ("48 kHz, with the
  selected reference configured identically"); a single declared rate bounds the
  per-frame schedule (16 × 6 × 48,000 operator evaluations/s) and keeps
  reference-vs-reference repeatability exact under a defined environment.
- **Change control:** New DR required; STALE-mark R01, R02, N01, H03, H07, H08 and
  `spec/contract-v1.json`.

### DEC-010 — Synthesis host boundary

- **Status:** Accepted (plan default, section 3 row "Synthesis host boundary")
- **Decision:** The **host** performs file/MIDI/UI work and patch-to-core conversion;
  the **chip** advances audio-rate phase, envelopes, routing/feedback and mixing
  autonomously. No per-sample host dependency is introduced implicitly (H06
  acceptance); whether low-rate performance modulation runs on chip or host is
  declared with its worst-case link budget in H03.
- **Rationale:** Plan section 3 row "Synthesis host boundary" and section 4; an
  external controller must not stream operator amplitudes every sample.
- **Change control:** New DR required; STALE-mark H03, H06, U04.

### DEC-011 — State

- **Status:** Accepted (plan default, section 3 row "State")
- **Decision:** Explicit reset, explicit phase/LFO initialization, and explicit event
  timing. Repeatability is **tested, not assumed** (repeated fresh-process renders
  must agree byte-for-byte, R02).
- **Rationale:** Plan section 3 row "State".
- **Change control:** New DR required; STALE-mark N07, H03, H04, R02.

### DEC-012 — Effects policy — **Proposed (owner may veto)**

- **Status:** **Proposed (owner may veto)** for the product policy; the dry-render
  rule for comparisons is a hard rule of the verification contract either way.
- **Decision:** Effects are **external or later**; the core is dry in v1. All
  reference comparisons are **dry**: renders are never normalized per render, never
  time-warped, and the reference engine is never switched per patch (plan section 7;
  AGENTS.md).
- **Rationale:** Plan section 3 row "Effects" and section 11 choice 5 ("dry core
  first; output coloration/effects separately"). Output coloration changes
  presentation and possibly fidelity claims without being needed for patch
  correctness.
- **Change control:** New DR required; STALE-mark R02, R04, U01 and any comparison
  budgets derived from dry renders.

### DEC-013 — Physical target

- **Status:** Accepted (plan default, section 3 row "Physical target")
- **Decision:** **gf180mcu** feasibility first, then the actual chosen
  shuttle/padframe/clock/memory constraints. **No slot-fit assertion is made** until
  mapped, measured evidence exists (H10/H11); FPGA results do not prove ASIC results
  (plan section 4).
- **Rationale:** Plan section 3 row "Physical target".
- **Change control:** New DR required; STALE-mark D01, H09–H11.

## 3. Event and controller decisions (plan section 3, "Event and controller details that must be frozen")

### DEC-014 — Event application timing: next frame boundary, bounded latency, explicit overflow

- **Status:** Accepted (proposed default; plan leaves this open and requires it frozen)
- **Decision:** An event (note or controller) **takes effect at the next audio frame
  boundary** after the core receives it. An event arriving during frame *N* is applied
  before the first sample of frame *N+1*; worst-case core event-to-audio latency is
  **one frame (20.833 µs at 48 kHz)** plus declared transport/queueing latency.
  Incoming events are held in a **bounded per-frame queue**; its exact depth is set by
  the interface contract (H03) sized for the worst-case realistic event burst, never
  guessed. **Overflow policy:** an event that cannot be enqueued is **rejected with an
  explicit error/status to the host** (backpressure; the host retries). Silent drops
  and silent deferral to a later frame are both forbidden.
- **Rationale:** Plan section 3 bullet 1 ("Define when an event takes effect in
  sample/frame time, including events arriving during a frame and queue overflow")
  and section 4 ("a deterministic time base").
- **Change control:** New DR required; STALE-mark R02 (event metadata), N07, H03.

### DEC-015 — Retrigger: re-trigger envelopes from current level

- **Status:** Accepted (proposed default)
- **Decision:** A note-on for an already-sounding note voice **re-triggers that
  voice's envelopes from their current levels** (DX envelope legs per the pinned
  reference), reinitializes per-note phase per DEC-020, and reinitializes all other
  per-note state exactly as the frozen integer model specifies. No stale envelope
  level leaks into the new attack beyond the documented from-current-level start.
- **Rationale:** Plan section 3 bullet 2; a DX envelope is not an ADSR, so
  "restart from zero" would be a different (wrong) instrument.
- **Change control:** New DR required; STALE-mark N03, N07, R06.

### DEC-016 — Same-pitch repeated notes: steal the oldest matching voice

- **Status:** Accepted (proposed default)
- **Decision:** When a note-on arrives for a pitch that already sounds and no free
  voice exists, the engine **steals the oldest matching voice** (longest-sounding at
  that pitch) and retriggers it per DEC-015. A same-pitch note never allocates a new
  voice while a matching one can be reused.
- **Rationale:** Plan section 3 bullet 2 ("repeated same-pitch notes"); bounded,
  predictable behavior for repeated-key playing.
- **Change control:** New DR required; STALE-mark N07, R06.

### DEC-017 — Voice-stealing order — **Proposed (owner may veto)**

- **Status:** **Proposed (owner may veto)** for the exact order; the existence of a
  deterministic, documented, tested order is not optional.
- **Decision:** When polyphony (16) is exhausted and no same-pitch voice can be
  reused, allocation follows a **deterministic documented order**; recorded default:
  prefer voices already in release, then steal the **oldest sounding voice**
  (earliest still-sounding note-on). Stealing is surfaced in the event/trace
  metadata; it is never random.
- **Rationale:** Plan section 3 bullet 2 ("voice stealing"); N07 tests stealing as a
  directed case.
- **Change control:** New DR (or owner ruling) required; STALE-mark N07, R06.

### DEC-018 — Sustain pedal: hold until pedal release (MIDI semantics)

- **Status:** Accepted (proposed default)
- **Decision:** With sustain (CC 64) active, a received note-off places the voice in
  **sustain-hold**: the voice keeps sounding per its envelope until **pedal release**,
  then enters normal release. Pedal release with no held note-offs changes nothing.
  All-sound-off (DEC-021 forced kill) and explicit reset override sustain.
- **Rationale:** Plan section 3 bullet 2 ("sustain release") and standard MIDI
  channel semantics; sustain must not truncate or retrigger.
- **Change control:** New DR required; STALE-mark N07, R06.

### DEC-019 — Patch change while notes are held: old patch until note-off

- **Status:** Accepted (proposed default)
- **Decision:** A patch selection **commits atomically at a frame boundary**.
  Already-sounding notes **keep sounding on the patch they started with** until each
  receives note-off (and completes release per DEC-021); new note-ons use the new
  patch. No mid-note hot parameter swap.
- **Rationale:** Plan section 3 bullet 2 ("patch change while notes are held");
  H03/H08 test patch atomicity.
- **Change control:** New DR required; STALE-mark N07, H03, U04.

### DEC-020 — Oscillator/LFO phase: free-running except where the sync parameter restarts

- **Status:** Accepted (proposed default)
- **Decision:** Phase state is **free-running** with respect to unrelated events: the
  global LFO is not restarted by note events, and per-note operator phase behaves as
  frozen by the integer model. The **patch's own oscillator-sync parameter is the only
  in-patch restart control**, with exact restart semantics pinned by R01 against the
  reference. Starting phase is therefore a declared property of every comparison, so
  waveform subtraction cannot report a huge phase-mismatch error as if it were a
  synthesis defect.
- **Rationale:** Plan section 3 bullet 5 ("Define whether oscillator/LFO state is
  free-running or restarted. Comparing two different starting phases … can report a
  huge error without isolating a synthesis defect").
- **Change control:** New DR required; STALE-mark N02, N05, R04, R05.

### DEC-021 — Note tails: envelope to a documented floor; forced kill only via all-sound-off

- **Status:** Accepted (proposed default)
- **Decision:** **No arbitrary fixed-length note tails** (explicitly no four-second
  cutoff; plan section 2 "What should not transfer"). A voice sounds until its
  envelope release decays below a **documented silence floor**, held for a documented
  number of frames, after which the voice is reclaimed. The exact floor and hold
  numbers are frozen by the numeric decision record (N01), not assumed here. The
  **only forced kill is all-sound-off** (and explicit reset). A quiet patch, a slow
  delayed attack, or a nonzero final/sustain level is a valid patch, not an error.
- **Rationale:** Plan section 3 bullet 6 ("Avoid arbitrary four-second note tails.
  Slow/delayed envelopes and nonzero final envelope levels need explicit treatment").
- **Change control:** New DR required; STALE-mark N01, N03, N07, R02 (render-length
  policy), U01.

### DEC-022 — Patch data is separate from performance/function settings

- **Status:** Accepted (plan requires this frozen; plan section 3 bullet 3)
- **Decision:** Voice patch data and performance/function settings (volume, bend
  range, controller sensitivity assignments, and similar) are **separate state with
  separate defaults**. A voice `.syx` alone does **not** constitute a complete
  performance. The v1 default performance settings are frozen in R01 and never
  smuggled into patch bytes.
- **Rationale:** Plan section 3 bullet 3 ("A voice `.syx` alone does not necessarily
  specify a complete performance").
- **Change control:** New DR required; STALE-mark P01, R01, N05.

### DEC-023 — Preserve the patch's own transpose, scaling and velocity settings

- **Status:** Accepted (plan requires this frozen; plan section 3 bullet 4)
- **Decision:** The patch's own transpose, keyboard level/rate scaling, and velocity
  sensitivity parameters are **preserved and applied exactly once**. Transpose is
  never double-applied (the host stage and core stage each apply only what their
  declared contract says). MIDI velocity is **never globally clamped** to an old
  keyboard's commonly observed output range; the velocity mapping is declared in R01.
- **Rationale:** Plan section 3 bullet 4; P01/R06 test these as directed cases.
- **Change control:** New DR required; STALE-mark P01, N05, R06.

## 4. Explicit exclusions and non-goals

### DEC-024 — No original-hardware fidelity claim

- **Status:** Accepted (plan section 3 "Fidelity profile"; normative in AGENTS.md)
- **Decision:** v1 makes **no original-hardware fidelity claim**. Stated plainly:
  **"Dexed agreement is not original-DX7 fidelity."** The instrument is not claimed
  to be a bit-perfect reproduction of an original DX7 unless a separate
  hardware-fidelity claim has actually been qualified. An original-hardware profile
  is a **different contract**: it would additionally require original clock/sample
  timing (roughly 49.096 kHz), original envelope update behavior, arithmetic quirks,
  DAC companding/output filtering, and suitable hardware evidence. Simply changing
  the output sample rate or reducing PCM to 12 bits does not establish that claim.
- **Also excluded from v1 (non-goals):**
  - FPGA or gf180mcu synthesis, place-and-route, signoff, or hardware-playback
    claims without committed evidence records establishing exactly that claim.
  - Effects or output coloration in the core (DEC-012); comparisons are dry.
  - Multi-timbre operation (DEC-003 fixes one active timbre).
  - Preset-quality or musical-usefulness claims from numeric tests; listening
    records are the only evidence for those.
  - Arithmetic widths and rounding (deferred to N01) and the physical constraint
    inventory (deferred to D01) — out of scope for this document (issue #4
    non-goals).
  - Per-file license headers; the license is repo-level Apache-2.0 (DEC-025).
- **Rationale:** Plan section 3 "Fidelity profile" and section 7 claim table; issue
  #4 acceptance ("Non-goals and claim boundaries … stated verbatim").
- **Change control:** Relaxing any exclusion requires a new DR plus the evidence
  record that establishes the new claim; STALE-mark all fidelity statements and the
  verification contract.

## 5. Licensing and source policy

### DEC-025 — Repo-level Apache-2.0; GPL engines are external oracles only

- **Status:** **Accepted (owner-ruled)** — already decided at repo level
  (LICENSE, AGENTS.md); recorded here, not re-decided.
- **Decision:**
  - This repository is **Apache-2.0 at repo level, with no per-file license
    headers** (org convention; banner comments are fine).
  - **GPL-3.0-or-later code** (Dexed `Source/EngineMkI.cpp`, VDX7) and
    **GPL-2.0-or-later code** (Hexter) are **external comparison oracles only and are
    never copied into this repository**.
  - **Apache-2.0 material** (Dexed `msfa/`, any 2AMLogic sibling component) may be
    adopted only with a **provenance record** (upstream commit, path, hashes,
    license, destination, adaptations) **plus requalification**; hash equality alone
    never qualifies (TorchSynth DR-0005 import-governance pattern; see
    `docs/reuse/README.md`).
- **Rationale:** AGENTS.md licensing/source policy; plan section 5 ("Source reuse is
  not the same as reference use"). An external comparison executable and code copied
  into a shipping design are different engineering decisions.
- **Change control:** Changing license policy is an owner decision, not a builder
  decision; any relaxation requires a new DR and STALE-marking of the reuse
  governance (`docs/reuse/`, A01 checks).

## 6. Plan section 11 highest-value choices — recorded defaults

| Rank | Choice (plan section 11) | Recorded default | Decision ids |
|---:|---|---|---|
| 1 | Compatibility vs original-hardware reproduction | Musically convincing DX7 compatibility against the pinned Dexed Mark I profile; no hardware-fidelity claim | DEC-001, DEC-024 |
| 2 | Engine chip plus host vs literal one-die instrument | Engine chip with external host/controller and patch storage in host-accessible storage; curated banks are data updates, not chip revisions | DEC-010, DEC-004, DEC-008 |
| 3 | Physical budget and required polyphony | 16 notes, one timbre; actual slot/clock/memory fit is measured, never asserted | DEC-003, DEC-013 |
| 4 | Which sounds are "good"? | Diverse 32 favorites first, 128 curated later; preference never hides compatibility failures | DEC-004 |
| 5 | Vintage output color and effects | Dry core first; coloration/effects external or later; comparisons dry | DEC-012, DEC-008 |

## 7. Verification-contract obligations inherited from plan section 7

This contract inherits, without restating in full, the plan's claim/evidence table:
import correctness fixtures; pinned-reference comparisons at named seams; exact
RTL-vs-frozen-model comparisons after declared scheduling/latency; live instrument
behavior; hardware captures under their own alignment/calibration procedure; musical
usefulness via listening records only; physical fit via mapped reports for that exact
implementation. Statuses are reported as `PASS`, `FAIL`, `NOT_RUN`, `BLOCKED`,
`NO_VERDICT`, or `STALE`; coverage is reported separately from agreement; negative
controls (wrong operator order, broken feedback history, ADSR substituted for a DX
envelope, silent/stale stubs) stay live and must demonstrably fail the check they
target. The automated negative control for **this** document is
`tests/test_contract_v1.py`: a contract draft omitting sample rate, polyphony, the
reference engine, or the effects policy fails.
