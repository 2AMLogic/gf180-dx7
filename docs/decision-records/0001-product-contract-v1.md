# Decision record: Product contract v1 and fidelity profile `dx7-compatible-v1`

- **DR number:** 0001
- **Status:** Accepted (owner-directed backlog execution, 2026-09-19)
- **Date:** 2026-09-19
- **Deciders:** Repository owner (Joseph Turian), by owner-directed backlog
  execution of issue #4; defaults drafted by Loom Builder from the plan's
  proposed defaults
- **Related issues:** #4 (D00), #1 (E1 epic)

## Context

The plan (`docs/dx7-chip-plan-v0.1-2026-09-20.md`) defines a proposed product
contract (section 3), a verification contract (section 7), completion rules
(section 10), and five highest-value product choices (section 11) — all marked
as recommendations, not established user decisions. Issue #4 (D00) requires one
decision record that instantiates the section 3 contract, freezes the
event/controller details the plan leaves open, records deliberate exclusions,
and names the baseline engine, so that dependent backlog leaves (R01, P01, N07,
H03, …) have an explicit contract to inherit. No new measurements were performed
for this record; it adopts plan-proposed defaults and does not itself establish
any fidelity claim.

## Decision

Adopt `docs/DECISIONS-v1.md` as the frozen v1 product contract, with its
machine-readable companion `spec/contract-v1.json` (lockstep enforced by
`tests/test_contract_v1.py`). The named fidelity profile is
**`dx7-compatible-v1`**: a **pinned Dexed Mark I** configuration is the initial
executable comparison target — exact commit, engine mode, sample rate (48 kHz),
and runtime pinned by R01 before any render; upstream drift is rejected.

Reference-stack roles recorded by this decision:

- **Dexed Mark I** — initial executable comparison oracle (external; its
  `Source/EngineMkI.cpp` is GPL-3.0-or-later and is never copied into this
  Apache-2.0 repository). Dexed agreement is not original-DX7 fidelity.
- **MSFA** (`Source/msfa/`, Apache-2.0) — adoptable research/code with a
  provenance record plus requalification; hash equality alone never qualifies.
- **VDX7** (GPL-3.0-or-later) — hardware-oriented cross-check oracle; used only
  for targeted oracle-disagreement diagnostics (R07), never copied in.
- **Hexter** (GPL-2.0-or-later) — additional external lineage oracle for
  investigating concrete disagreements, never copied in.

Event/controller defaults frozen by the contract include: events apply at the
next frame boundary with a one-frame bound and explicit backpressure on queue
overflow (DEC-014); retrigger from current envelope level (DEC-015); same-pitch
repeats steal the oldest matching voice (DEC-016); sustain holds voices until
pedal release (DEC-018); held notes keep sounding on their old patch until
note-off (DEC-019); oscillator/LFO phase is free-running except where the patch
sync parameter restarts (DEC-020); no arbitrary fixed-length tails — voices
release per envelope to a documented floor, forced kill only via all-sound-off
(DEC-021).

## Consequences

- Backlog leaves that depend on the contract (R01, R02, P01, P02, N01, N07,
  H03, U01–U04) now inherit explicit decisions instead of open questions.
- Changing any `DEC-NNN` requires a new decision record and STALE marking of
  the dependents listed under that id in `docs/DECISIONS-v1.md`.
- The contract forbids: original-hardware fidelity claims for v1; effects in
  the core; per-render normalization, time-warping, or per-patch switching of
  the reference engine; arbitrary fixed-length note tails; silently weakening
  the 16-note polyphony goal.
- Three decisions are **Proposed (owner may veto)**: curated bank contents
  (DEC-004), effects policy (DEC-012), exact voice-stealing order (DEC-017).
  Until ruled, their recorded defaults stand and remain listed under "What
  remains unproved" in the adopting PR.
- Negative control: `tests/test_contract_v1.py` fails if the contract omits
  sample rate, polyphony, the reference engine, or the effects policy, or if
  the JSON and `DECISIONS-v1.md` disagree on DEC ids.

## Evidence

- `docs/dx7-chip-plan-v0.1-2026-09-20.md` sections 3, 7, 10, 11 (normative for
  this record).
- Issue #4 (D00) brief and acceptance checklist.
- `docs/DECISIONS-v1.md`, `spec/contract-v1.json`, `tests/test_contract_v1.py`.
- Licensing policy pre-ruled at repo level: LICENSE and AGENTS.md; reuse
  governance in `docs/reuse/README.md` (TorchSynth DR-0005 pattern).
- No measurements, renders, or synthesis results are claimed by this record.
