# Work Plan

This roadmap is generated from the current GitHub label state by the Loom Guide role.

<!-- guide:plan-body:start -->
## Operator Attention: Merge-Risk-Hold Pileup

Judge-approved PRs under a hold (`loom:blocked` or `loom:operator`) — only a human merge decision is missing.

_None._

## Very Important

`loom:very-important` (one counts per workspace); every stage takes these first.

_None._

## Important

`loom:important`; taken before unlabelled work.

- **#2**: Epic: E2 — Verified core and physical feasibility
- **#3**: Epic: E3 — Curated instrument
- **#200**: Re-stamp the reference-pin capability node (STALE since tools/verify_reference.py changed)

## Ready

Human-approved issues ready for implementation (`loom:issue`).

_None._

## In Progress

Issues currently being built (`loom:building`).

- **#206**: Capability evidence records cannot distinguish a full check run from a guarded skip
- **#222**: N01 checker: five prose strings restate checked numbers the checker never re-derives (#212's class, fully enumerated)
- **#226**: FV lane re-breaks on every klayout-tools pin bump: provenance.klt_version is compared verbatim and nothing gates the bump

## PRs Awaiting Review

PRs waiting on Judge (`loom:review-requested`).

- **#228**: feat: capture guarded-skip output and skip counts in capability evidence
- **#229**: fix: N01 checker pins prose restatements of recomputed values; guard closes the class
- **#230**: check: gate FV_PINS bumps against committed FV evidence provenance

## Approved (Awaiting Merge)

PRs that passed review and are queued for Champion auto-merge (`loom:pr`).

_None._

## Proposed

Issues carrying `loom:curated`.

- **#2**: Epic: E2 — Verified core and physical feasibility *(curated)*
- **#3**: Epic: E3 — Curated instrument *(curated)*
- **#31**: H09: FPGA digital capture *(curated)*
- **#33**: H11: Physical integration evidence *(curated)*
- **#35**: U02: Reviewed 32-patch listening bank *(curated)*
- **#36**: U03: Curated 128-patch bank manifest *(curated)*
- **#101**: H08: re-synthesize the chassis on the DR-0011 refrozen core on the heavy host (gate full_vs_h07 is NOT_RUN) *(curated)*
- **#103**: Pinned ORFS mapped census + H10 re-run on the DR-0012 core pin (blocked on #98 and on a >=12 GB synthesis host) *(curated)*
- **#151**: Track the gap to T1 (klayout-tools design-evidence-tiers.md) *(curated)*
- **#206**: Capability evidence records cannot distinguish a full check run from a guarded skip *(curated)*
- **#222**: N01 checker: five prose strings restate checked numbers the checker never re-derives (#212's class, fully enumerated) *(curated)*
- **#226**: FV lane re-breaks on every klayout-tools pin bump: provenance.klt_version is compared verbatim and nothing gates the bump *(curated)*

## Proposed (Architect / Hermit)

- **#161**: Consolidate triplicated parse_events in model/{algorithm,integrated,poly}.py *(hermit)*
- **#208**: Consolidate the two Freqlut implementations: operator.py vs pitch.py *(hermit)*
- **#219**: Remove tools/sysex_probe.py: unreferenced P01 probe CLI *(hermit)*

## Epics

- **#2**: Epic: E2 — Verified core and physical feasibility
- **#3**: Epic: E3 — Curated instrument

## Backlog Balance

| Bucket | Count |
|------|-------|
| Operator merge-risk holds | 0 |
| Very important | 0 |
| Important | 3 |
| Ready (`loom:issue`) | 0 |
| In Progress (`loom:building`) | 3 |
| PRs awaiting review | 3 |
| Approved PRs awaiting merge | 0 |
| Curated | 12 |
| Architect / Hermit proposals | 3 |
| Active epics | 2 |
<!-- guide:plan-body:end -->
