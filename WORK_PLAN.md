# Work Plan

This roadmap is generated from the current GitHub label state by the Loom Guide role.

<!-- guide:plan-body:start -->
## Operator Attention: Merge-Risk-Hold Pileup

Judge-approved PRs under a hold (`loom:blocked` or `loom:operator`) — only a human merge decision is missing.

- **#153**: feat: add klt signoff manifest and attest T1 items 9 and 10

## Very Important

`loom:very-important` (one counts per workspace); every stage takes these first.

_None._

## Important

`loom:important`; taken before unlabelled work.

- **#2**: Epic: E2 — Verified core and physical feasibility
- **#3**: Epic: E3 — Curated instrument
- **#31**: H09: FPGA digital capture
- **#32**: H10: gf180 mapped feasibility report
- **#33**: H11: Physical integration evidence
- **#35**: U02: Reviewed 32-patch listening bank
- **#36**: U03: Curated 128-patch bank manifest

## Ready

Human-approved issues ready for implementation (`loom:issue`).

_None._

## In Progress

Issues currently being built (`loom:building`).

- **#149**: Commit a klt signoff block manifest (kind: digital) and attest T1 items 9 and 10

## PRs Awaiting Review

PRs waiting on Judge (`loom:review-requested`).

_None._

## Approved (Awaiting Merge)

PRs that passed review and are queued for Champion auto-merge (`loom:pr`).

- **#153**: feat: add klt signoff manifest and attest T1 items 9 and 10

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
- **#141**: Consolidate 39 copies of sha256_file into one shared tools helper *(curated)*
- **#149**: Commit a klt signoff block manifest (kind: digital) and attest T1 items 9 and 10 *(curated)*
- **#151**: Track the gap to T1 (klayout-tools design-evidence-tiers.md) *(curated)*

## Proposed (Architect / Hermit)

- **#161**: Consolidate triplicated parse_events in model/{algorithm,integrated,poly}.py *(hermit)*

## Epics

- **#2**: Epic: E2 — Verified core and physical feasibility
- **#3**: Epic: E3 — Curated instrument

## Backlog Balance

| Bucket | Count |
|------|-------|
| Operator merge-risk holds | 1 |
| Very important | 0 |
| Important | 7 |
| Ready (`loom:issue`) | 0 |
| In Progress (`loom:building`) | 1 |
| PRs awaiting review | 0 |
| Approved PRs awaiting merge | 1 |
| Curated | 11 |
| Architect / Hermit proposals | 1 |
| Active epics | 2 |
<!-- guide:plan-body:end -->
