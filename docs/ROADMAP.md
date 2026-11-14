# Roadmap

gf180-dx7 is a DX7-compatible six-operator FM engine for the gf180mcu open
PDK, built so that the RTL matches a frozen integer model exactly.

Status as of 2026-11-13, `main` @ `7a31594`.

This is the hand-written plan; `WORK_PLAN.md` and the README's issue-DAG and
capability blocks are generated views.

## Where it stands

**Issue DAG (`spec/issue-dag-v1.json`): 31 of 36 tasks closed.** Open: H09
(#31), H10 (#32), H11 (#33), U02 (#35), U03 (#36), under epics E2 (#2) and
E3 (#3). Done and evidenced in the tree:

- Reference stack, codec, catalog, comparator, traces, registry (E1;
  `reference/`, `corpus/`, `docs/COMPARE-CONTRACT.md`).
- Integer models N01 to N08 and the frozen release (`docs/N08-RELEASE.md`,
  `spec/release-vectors-v1.json`, DR-0007).
- RTL H01 to H08: module benches, the integrated core and the pin-level
  chassis (`rtl/`, `docs/H07-CORE.md`, `docs/H08-PIN.md`). The core was
  refrozen twice (DR-0011, DR-0012); DR-0012 is the current pin.
- A `klt functional-verification` report for the bit-exact RTL-vs-model
  suite on the DR-0012 pin (`evidence/t1-functional/`, #150).
- Host mock and end-to-end demo tooling (U04, U05; `docs/U05-DEMO.md`).
- H10 mapped synthesis on the DR-0011 core (`docs/H10-GF180-FEASIBILITY.md`):
  the cells alone are 14.3x the quarter-slot core, and synthesis-stage setup
  slack is negative. The report is marked STALE since DR-0012; its figures
  are lower bounds for the current pin.

**Design-evidence checklist (`signoff/signoff.json`, klt 0.8.0): not T1,
2 of 11 items met.** Tracker: #151.

| Item | State | Waits on |
| --- | --- | --- |
| 1 Design sources | unmet | mapped netlist on the DR-0012 pin: #103 |
| 2 Layout, 3 DRC, 4 LVS | unmet | #33 |
| 5 Full corner verification | unmet | functional half exists (#150, not yet cited); STA half needs #103, #32 |
| 6 Statistical claims | unmet | a bound `not_applicable` statement (Now 4) |
| 7 Post-layout verification | unmet | #33, #103 |
| 8 Characterization, 11 Power delivery | unmet | #32, #33 |
| 9 Testbenches, 10 Repo hygiene | met | `signoff/evidence/item9.json`, `item10.json` |

**Capability DAG (`docs/CAPABILITIES.md`): 1 PASS, 1 STALE, 13 BLOCKED.**
This understates the work above for two mechanical reasons: `reference-pin`
went STALE when `tools/verify_reference.py` changed (#170), which blocks
every node downstream of it; and eight nodes (`numeric-profile`,
`fixed-model`, `rtl-core`, `rtl-exact`, `registry`, `fpga`, `gf180`,
`instrument`) still declare a `planned:` check with nothing registered in
`src/gf180_dx7/capabilities.py`.

**Governing decision.** DR-0013 (fidelity first, area second): the 16-note
core stays the reference design, and H10's place, CTS and route rows wait
for an owner-opened optimization-phase decision record.

## Now

Every open issue is held; these have no issue yet and can start today.

1. **Re-stamp `reference-pin`.** Run `tools/fetch_dexed.sh`, then
   `python3 tools/compile_capabilities.py --stamp reference-pin` with its
   `pin-drift-refused` control. Done when `docs/CAPABILITIES.md` shows
   `reference-pin: PASS` and `compile_capabilities.py --check` exits 0.
2. **Register a check for the `numeric-profile` node.**
   `tools/check_numeric_profile.py` exists but is not in `CHECKS`. Done when
   the node names a registered check, covered inputs and a negative control,
   and `tests/test_capabilities.py` passes (BLOCKED on prerequisites is fine).
3. **Bring the signoff record up to date on item 5.** `signoff/README.md`
   still says the functional envelope is "not produced yet". Done when that
   row is correct and, if klt 0.8.0 accepts a functional-only citation,
   `signoff/manifest.json` cites `evidence/t1-functional/report.json` and
   `signoff.json` is regenerated (item 5 stays unmet without the STA half).
4. **Item 6: the bound `not_applicable` statement.** `signoff/README.md`
   names it as the expected follow-up: no row of `spec/core-contract-v1.json`
   is statistical. Done when `klt signoff` reports item 6 `not_applicable`
   from a committed, hash-bound statement covering every spec row; stop and
   report if klt requires a ratification record the repo lacks.
5. **Correct the README Status section.** It says "Planning" and "35
   tasks"; the DAG block says 31 of 36 closed. Done when it matches the
   generated blocks and item 10 is re-attested (the edit drifts it).

## Next

- **Re-stamp the nodes downstream of `reference-pin`** (`oracle-identity`,
  `renderer`, `comparator`, `traces`) once Now 1 lands. Done when each reads
  PASS, or a guarded NOT_RUN where the pinned oracle binary is absent.
- **Re-stamp `corpus-dev32`** (STALE on `tools/validate_dev32.py`). Needs the
  pinned archive; `tools/fetch_alltheweb.sh` takes its URL from the caller.
- **Register checks for `fixed-model`, `rtl-core`, `rtl-exact` and
  `registry`**, one PR per node, after Now 2 sets the pattern. Candidates
  already in the tree: `tools/n08_verify_release.py`, the `h0x_compare.py`
  benches, `tools/functional_verification.py`, `tools/check_registry.py`.
  Done per node when the spec names a registered command with covered
  inputs and executed negative controls.
- **#103 then #101 and the H10 re-run**, once a host with at least 12 GB for
  the pinned ORFS image is available. Done when `tools/exp_netlist_census.py`
  reports a nonzero exp() datapath on the DR-0012 pin, the STALE banner
  leaves `docs/H10-GF180-FEASIBILITY.md`, and `gates.full_vs_h07` reads PASS
  or FAIL rather than NOT_RUN. This also supplies checklist item 1.
- **#161 (shared `parse_events`)** rides with the next `model/` change, so H07 evidence is re-qualified once.

## Later

- **Optimization phase (H10 physical rows, then H11 #33).** Question first:
  is golden functionality declared locked, and which of #85's five
  directions does the new decision record choose? Checklist items 2 to 4,
  7, 8 and 11 follow from it.
- **`fpga` and `gf180` capability checks.** Question first: what evidence
  format do H09 and the re-run H10 commit? U03 (#36) follows U02's verdicts.

## Blocked and waiting

Nothing in the tracker is dispatchable today: every open issue carries
`loom:blocked` or is a tracker, epic or dashboard.

- **#31 H09 FPGA capture:** needs a ULX3S-class board on a named, reachable
  bench host. Simulation is not a substitute.
- **#32 H10, #33 H11:** deferred by DR-0013 until the optimization-phase
  decision record.
- **#103, #101:** need the heavy synthesis host; a 7.75 GB run was killed in
  `techmap` (`evidence/issue-98-exp-refreeze/`).
- **#35 U02, then #36 U03:** need a human listener to run the protocol in
  `corpus/u02-candidates/LISTENING-KIT.md` and commit the records.
- **#141:** maintainer review and the real-dev32 archive run remain. **#161:** deferred as above.

## Not planned

- Area-motivated architecture or polyphony changes before DR-0013 is superseded.
- Copying code from the GPL comparison oracles (Dexed, VDX7, Hexter).
- Tapeout or shuttle submission; H11 leaves that to a separate owner decision.
