# DR-0013: Fidelity first, area second — the 16-note core stays the reference target; H10's physical rows are deferred

- **DR number:** 0013
- **Status:** Accepted
- **Date:** 2026-09-29 (owner ruling 2026-09-26)
- **Deciders:** Repository owner (Joseph Turian), ruling on #85 on
  2026-09-26; recorded as this DR on 2026-09-29, with the operator's
  concurrence
- **Related issues:** #85 (the escalation this answers), #32 (H10), PR #84
  (the evidence record)

## Context

1. **The measured gap.** H10 (#32) mapped the refrozen 16-note core
   (DR-0011) through the pinned ORFS flow, with the evidence in PR #84:
   `docs/H10-GF180-FEASIBILITY.md` and `evidence/h10-gf180/`. The most
   favourable measured policy gives **24.00 mm²** of cells: **14.3×** the
   D01 quarter-slot core (1.673 mm²) and **7.1×** the two-slot core
   (3.382 mm²). `env_unit` ×96 is **70.9 %** of that area. The
   synthesis-stage setup slack at 24.576 MHz is −92.9 ns, on the
   voice-allocation search path. That is a pre-repair figure, not a closure
   verdict.
2. **The escalation.** H10's stop/escalate clause sent the
   architecture/requirements question to the owner as #85. The issue listed
   five directions, none of them synthesised: (1) time-multiplex the
   envelope units, (2) SRAM-backed voice/operator state, (3) pipeline the
   voice-allocation search, (4) change the physical target, (5) a
   polyphony ruling.
3. **The ruling.** On 2026-09-26 the owner ruled: *achieve pure DX7 clone
   fidelity and functional perfection first, then optimize the physical
   size/area as a subsequent phase.* That means proving 100 % functional
   equivalence against the golden reference model first, then addressing
   area (polyphony reduction, time-multiplexing, hard SRAM macros) in a
   dedicated optimization pass.
4. **A labelling hazard this record removes.** The ruling was posted as
   "Option 1 (Best)". In #85's own list, option 1 is *time-multiplexing the
   envelope units*, an area optimization and the opposite of what the
   ruling says. The ruling's **words** are unambiguous; its **label** is
   not. This record follows the words.

## Decision

1. **Fidelity first.** The current full-fidelity, 16-note core remains the
   reference design. Work continues on functional equivalence to the golden
   model. No functional accuracy, polyphony or sound fidelity is traded for
   area in this phase.
2. **None of #85's directions 1–5 is adopted now.** In particular, this is
   **not** a choice of #85 option 1 (time-multiplexing), whatever the
   ruling's label said, and it is **not** a polyphony revision (#85
   option 5). DR-0001's 16-note goal stands unchanged.
3. **H10's remaining physical rows are deferred, not blocked.** Place, CTS,
   route and post-route timing need a die the cells can occupy, so they wait
   for the optimization phase and are re-scoped against the direction chosen
   then. "Deferred" means an owner-chosen sequencing, not an impediment
   waiting on someone, so it should not show up as blocked work.
4. **Measuring area is not forbidden.** Synthesis and area censuses may
   still run and be recorded as information (for example to watch the
   trend while the core changes for fidelity reasons). They are not gates,
   and they are not a licence to start an architecture change.
5. **Opening the optimization phase takes a new DR.** When the owner judges
   golden functionality locked, a later DR picks among #85's directions (or
   others) and supersedes Decision 3 above. Until that DR exists, #85's
   stop/escalate rule still holds: **no architecture change or requirement
   revision for area's sake.**

## Consequences

- **Enables:** fidelity work on the unconstrained core without an area
  budget distorting it; one golden reference to measure any later
  optimization against.
- **Forbids:** starting any of #85's five directions, or any area-motivated
  restructuring, before the optimization-phase DR.
- **Known cost:** no layout, CTS or routing evidence exists until the
  optimization phase, so no claim of tapeout readiness can be made.
  **Revisit trigger:** if a shuttle or tapeout date is adopted for this
  design, that date is a reason to supersede this DR early (for example
  with a bounded, flagged area spike in parallel).
- **Becomes STALE when:** the owner declares golden functionality locked
  (then the optimization-phase DR follows), or a tapeout date is set.

## Evidence

- PR #84 — `docs/H10-GF180-FEASIBILITY.md`, `evidence/h10-gf180/`
  (`bundle.json`, `orfs/`): the mapped-area and timing figures quoted in
  Context 1.
- #85 — the escalation, the five directions, and the owner's ruling of
  2026-09-26 (two comments: "Operator Input" and "Operator Decision
  Ruling").
- DR-0001 — the 16-note polyphony goal this record leaves unchanged.
- DR-0011 — the refrozen core that H10 measured.
