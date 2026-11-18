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
- **#200**: Re-stamp the reference-pin capability node (STALE since tools/verify_reference.py changed) *(held)*

## Ready

Human-approved issues ready for implementation (`loom:issue`).

_None._

## In Progress

Issues currently being built (`loom:building`).

_None._

## PRs Awaiting Review

PRs waiting on Judge (`loom:review-requested`).

_None._

## Approved (Awaiting Merge)

PRs that passed review and are queued for Champion auto-merge (`loom:pr`).

_None._

## Proposed

Issues carrying `loom:curated`.

- **#2**: Epic: E2 — Verified core and physical feasibility *(curated)*
- **#3**: Epic: E3 — Curated instrument *(curated)*
- **#31**: H09: FPGA digital capture *(curated)* *(held)*
- **#33**: H11: Physical integration evidence *(curated)* *(held)*
- **#35**: U02: Reviewed 32-patch listening bank *(curated)* *(held)*
- **#36**: U03: Curated 128-patch bank manifest *(curated)* *(held)*
- **#101**: H08: re-synthesize the chassis on the DR-0011 refrozen core on the heavy host (gate full_vs_h07 is NOT_RUN) *(curated)* *(held)*
- **#103**: Pinned ORFS mapped census + H10 re-run on the DR-0012 core pin (blocked on #98 and on a >=12 GB synthesis host) *(curated)* *(held)*
- **#151**: Track the gap to T1 (klayout-tools design-evidence-tiers.md) *(curated)*

## Proposed (Architect / Hermit)

- **#161**: Consolidate triplicated parse_events in model/{algorithm,integrated,poly}.py *(hermit)* *(held)*
- **#208**: Consolidate the two Freqlut implementations: operator.py vs pitch.py *(hermit)*

## Epics

- **#2**: Epic: E2 — Verified core and physical feasibility
- **#3**: Epic: E3 — Curated instrument

## Backlog Balance

| Bucket | Count |
|------|-------|
| Operator merge-risk holds | 0 |
| Very important | 0 |
| Important | 3 |
| Ready (`loom:issue`, net of holds/building/merge-pending) | 0 |
| In Progress (`loom:building`) | 0 |
| PRs awaiting review | 0 |
| Approved PRs awaiting merge | 0 |
| Curated | 9 |
| Architect / Hermit proposals | 2 |
| Active epics | 2 |
<!-- guide:plan-body:end -->
