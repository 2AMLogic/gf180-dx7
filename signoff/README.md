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
| 5 Full corner verification vs a ratified spec | uncited | envelope not produced yet: #150 (T1 gap tracker #151); also gated on #103 |
| 6 Statistical claims carry Monte Carlo evidence | uncited | see "Item 6" below |
| 7 Post-layout verification | uncited | needs layout and mapped netlist: #33, #103 |
| 8 Characterization report | uncited | needs a characterization record (waits on #32/#33) |
| 11 Power delivery (structural) | uncited | needs place-and-route and layout: #32, #33 |

Item 1 stays uncited until #103 produces the mapped netlist; no
native-envelope or placeholder citation is made for it.

## Item 6

No row of `spec/core-contract-v1.json` (DR-0001, DR-0007) is statistical:
the contract and both decision records contain no Monte Carlo, distribution
or tolerance rows, and the core contract is integer and bit-exact. The
expected next step is the bound `not_applicable` statement the checklist
allows. That is a follow-up and is not made here, so item 6 is uncited.

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

`klt` is pinned at `2AMLogic/klayout-tools`
`7ee09483b74867a47a05eb353015c2cf5ca33ef3` (`klt 0.7.1.dev1743+g7ee09483b748`).
PyPI 0.7.0 does not have `--attest`. `design-evidence-tiers.md` is
`docs/design-evidence-tiers.md` from that commit, byte for byte
(sha256 `a26b9026…efdc7`).

## Regenerating

From the repo root, with the pinned klt and iverilog and Verilator on PATH:

```bash
klt signoff --attest 9  --inventory signoff/inventory-item9.json  -o signoff/evidence/item9.json
klt signoff --attest 10 --inventory signoff/inventory-item10.json -o signoff/evidence/item10.json
# set each manifest content_hash to the envelope's provenance.input.content_hash
klt signoff --manifest signoff/manifest.json \
  --tiers-doc signoff/design-evidence-tiers.md --format json > signoff/signoff.json
```

## CI

The `signoff-fresh` job (pull requests) runs `--attest --check` for items 9
and 10 and `--manifest --check` against `signoff.json`; each exits 3 on
drift. Negative controls run when this was added: adding a nonexistent path
to the item 10 inventory, and editing `README.md` after attestation, each
made `--attest 10 --check` report `drifted`.
