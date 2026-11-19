# Signoff: machine-graded T1 state (items 9 and 10)

`signoff/signoff.json` is the verdict of record for this block against the
[klayout-tools T1 checklist](design-evidence-tiers.md). It is the output of

```bash
klt signoff --manifest signoff/manifest.json \
  --tiers-doc signoff/design-evidence-tiers.md --format json > signoff/signoff.json
```

Exit `3` ("rendered, not T1") is expected: **the block is not T1**
(`tier: null`, 2 of 11 items met). This record says nothing about
RTL-vs-model exactness, model-vs-Dexed agreement, or whether the instrument
sounds good; those are separate claims with their own evidence.

## Status by item

| item | status | reason |
| --- | --- | --- |
| 9 Testbenches shipped | met | `evidence/item9.json`, from `klt signoff --attest 9` over `inventory-item9.json` |
| 10 Repo hygiene | met | `evidence/item10.json`, from `klt signoff --attest 10` over `inventory-item10.json` |
| 1 Design sources | uncited | needs the mapped netlist on the DR-0012 pin: #103 |
| 2 Layout | uncited | no layout exists yet: #33 |
| 3 DRC clean | uncited | no layout exists yet: #33 |
| 4 LVS clean | uncited | no layout exists yet: #33 |
| 5 Full corner verification vs a ratified spec | `unmet` / `partial_coverage` | functional half cited: `evidence/t1-functional/report.json`, graded `met` as a citation; klt reports `missing_kinds: ["sta"]`, so the STA half still waits on #103 and #32 (T1 gap tracker #151). Not full functional coverage — see "Item 5" below |
| 6 Statistical claims carry Monte Carlo evidence | `unmet` / `no_evidence` | coverage statement committed, not a grade; manifest step waits on DR-0008 ratification (#264) — see "Item 6" below |
| 7 Post-layout verification | uncited | needs layout and mapped netlist: #33, #103 |
| 8 Characterization report | uncited | needs a characterization record (waits on #32/#33) |
| 11 Power delivery (structural) | uncited | needs place-and-route and layout: #32, #33 |

Item 1 stays uncited until #103 produces the mapped netlist; no
native-envelope or placeholder citation is made for it.

The statuses and reasons above are klt's own tokens where a row has been
brought up to date; the remaining `uncited` rows predate that convention and
are corrected separately. `klt signoff` itself emits
`no_evidence`, not `uncited`.

## Item 5

The manifest cites the committed `klt functional-verification` report
(`evidence/t1-functional/report.json`, described in
[`docs/FUNCTIONAL-VERIFICATION.md`](../docs/FUNCTIONAL-VERIFICATION.md)).
klt grades that citation `met` and the item `unmet` with
`reason: "partial_coverage"` and `detail.missing_kinds: ["sta"]`: item 5 wants
multi-corner STA **and** a bit-exact functional suite, and only the functional
half exists. **Item 5 is not met, and this citation is not a timing claim** —
no STA has been run; that half waits on #103 (mapped netlist) and #32
(place-and-route). `tier` stays `null` and `t1_met_count` stays `2`.

The citation is also **not full functional coverage**. The cited report records
`status: "pass"` over 23 graded cases (`passed_count: 23`, `failed_count: 0`),
and its `provenance.not_graded` block records a further 11 cases as
`NOT_RUN` (`dev32-03`, `dev32-06`, `dev32-30` and the eight `ev-*` cases),
because Icarus plays ~2.4 s per 64-sample frame and their events run past
block 75. A `NOT_RUN` case is never a pass. Coverage is reported separately
from agreement, and the report establishes claim (1) only — RTL versus the
frozen integer model — not model-vs-reference agreement, synthesis, STA,
place-and-route, gf180mcu signoff, hardware playback, or sound.

## Item 6

Item 6 is `unmet` with `reason: "no_evidence"`. No row of
`spec/core-contract-v1.json` `budget_worst_case.rows` is statistical: the
contract is integer and bit-exact. That is now recorded row by row in
[`item6-not-applicable-statement.json`](item6-not-applicable-statement.json),
one entry (`id`, `class`, `reason`) for each of the 16 rows, and
`tests/test_item6_statement.py` fails if a spec row is missing from it, if it
names a row the spec does not have, or on a duplicate id or empty field.

This is a **coverage statement, not a `not_applicable` grade**. The pinned
grader (`klayout-tools==0.8.0`) accepts a `not_applicable` outcome only when
the cited spec document carries a top-level `"status": "ratified"` and a
top-level `rows` list; `spec/core-contract-v1.json` has neither, and its
adoption record is DR-0008 (`Status: Proposed`, owner sign-off requested).
Nothing here asserts that ratification. The manifest declaration, the
`signoff.json` regeneration and the byte-drift negative control are a
follow-up gated on DR-0008 ratification (#264); until then
`signoff/manifest.json` and `signoff/signoff.json` are unchanged.

## What items 9 and 10 do and do not establish

- Item 9: the six `rtl/tb_*.v` benches exist, and `cold-start-benches.sh`
  runs all six from a clean checkout and exits 0. A missing simulator is a
  failure (BLOCKED), never a skip. Each bench runs through its conformance
  or live-control pass only; mutant sweeps stay in `make test`. The
  inventory's `pins` record the versions attested on 2026-10-17 (iverilog
  13.0, Verilator 5.050, Yosys 0.67+post, ciel PDK revision as in DR-0010,
  ORFS image digest). klt checks only that pins are non-empty; CI builds
  iverilog `v13_0` and Verilator `v5.050` from their release tags (cached),
  because the Ubuntu 24.04 apt iverilog 12.0 cannot parse
  `rtl/tb_operator_probe.v`. The Yosys, PDK and ORFS pins are not exercised
  by CI.
- Item 10: README, LICENSE and the CI workflow exist, and the byte-compile
  and `tools/check_all.sh` steps that CI's `test` job runs exit 0 locally.
  It does not attest that any remote CI run succeeded.

## Pinned grader

`klt` is pinned at PyPI `klayout-tools==0.8.0` (tag `v0.8.0`, commit
`c572046be644`; `klt --version` prints `klt 0.8.0`). `--attest` is available
in this release. `design-evidence-tiers.md` is `docs/design-evidence-tiers.md`
from tag `v0.8.0`, byte for byte (sha256 `a26b9026…efdc7`, unchanged from the
previous pin).

## Regenerating

From the repo root, with the pinned klt and iverilog and Verilator on PATH:

```bash
klt signoff --attest 9  --inventory signoff/inventory-item9.json  -o signoff/evidence/item9.json
klt signoff --attest 10 --inventory signoff/inventory-item10.json -o signoff/evidence/item10.json
# set each ATTESTED envelope's manifest content_hash (items 9 and 10 only) to
# that envelope's provenance.input.content_hash
klt signoff --manifest signoff/manifest.json \
  --tiers-doc signoff/design-evidence-tiers.md --format json > signoff/signoff.json
```

The `content_hash` pin applies **only** to the `--attest` envelopes above. A
native `klt functional-verification` report carries no `provenance.input`
block, so pinning a `content_hash` on item 5's entry makes klt grade the
citation `unmet` / `unverifiable_provenance`
(`content_hash_pin: "unverifiable"`) instead of `met`. Item 5's manifest entry
is therefore `{"file": "evidence/t1-functional/report.json"}` with no
`content_hash` key; do not add one back.

Before pushing, `python3 tools/check_signoff_hashes.py` (also run by
`tools/check_all.sh` and the fast lane) cheaply re-checks only the per-file
`content_hash` entries of items 9 and 10 against the working tree, so editing
one of those files without regenerating its envelope fails locally (issue
#260). It is a hygiene/testbench hash-drift guard only — it is not a
`signoff-fresh` pass and does not replace that job.

## CI

The `signoff-fresh` job (pull requests) runs `--attest --check` for items 9
and 10 and `--manifest --check` against `signoff.json`; each exits 3 on
drift. Negative controls run when this was added: adding a nonexistent path
to the item 10 inventory, and editing `README.md` after attestation, each
made `--attest 10 --check` report `drifted`.
