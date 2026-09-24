# Reuse rulings — sibling infrastructure audit (A01, issue #39)

Date: 2026-09-19 · Catalog: [docs/reuse/catalog.json](reuse/catalog.json) ·
Governance: [DR-0002](decision-records/0002-reuse-governance.md) ·
Negative control: `tools/check_reuse.py` (via `tools/check_all.sh`)

Method adopted from the gf180-torchsynth reuse audit (DR-0005, commit
`36bdc67faa1ff2509988b772efcca7a7eb4e7115`): every candidate gets an explicit
ruling; provenance is recorded **before** any import; hashes pin bytes, never
qualification; each adopted component needs a requalification check plus a
negative control. Rulings enum: `adopt`, `adapt`, `reject`,
`reference-only`, `external-oracle-only`.

**Pin and hash method.** Component pins: gf180-torchsynth
`36bdc67faa1ff2509988b772efcca7a7eb4e7115` (fixedpoint library:
`6532ec08eee7c79fd642f95d77bc086507a05f79`, where it first appears),
gf180-parasynth `cbcc8b9e10e49c84f630550e2e145cc6da8a659c` (origin/main tip),
gf180-polysynth `b60e424ec570d5ef988c0b2c05dc71ba5eed1825`. Every SHA-256 in
the catalog was computed as
`git -C <sibling-checkout> show <commit>:<path> | shasum -a 256` — bytes at
the pinned commit, not a moving working tree. Re-verification at audit time:
`GF180_SIBLING_ROOTS=<roots> python3 tools/check_reuse.py --verify-upstream`
→ 67/67 pinned files matched. Licenses: all three sibling LICENSE files hash
to `d47483eb…` (Apache-2.0, same org text); klayout-tools LICENSE is MIT
(`721f4ce1…`, matching the torchsynth audit's K08).

## Rulings

| Component (catalog key) | Ruling | Upstream @ pin | Planned destination | Reason in one line |
| --- | --- | --- | --- | --- |
| reuse-governance-audit | adapt | torchsynth @ 36bdc67 | docs/reuse/, DR-0002 | Audit + 70-file catalog + DR-0005 adopted as method; re-expressed locally, no bytes imported. |
| trace-capture | adopt | torchsynth @ 36bdc67 | src/gf180_dx7/trace_capture.py | stdlib-only non-perturbing capture; retarget seams to the pinned reference (R01/R04). |
| paired-comparator | adopt | torchsynth @ 36bdc67 | src/gf180_dx7/paired_metrics.py | No-alignment paired comparison matches the dry-render comparison rule. |
| case-registry | adopt | torchsynth @ 36bdc67 | src/gf180_dx7/case_registry.py | Statuses + coverage-separate-from-agreement pattern our reporting rules already require. |
| fixedpoint-library | adopt | torchsynth @ 6532ec08 | src/gf180_dx7/fixedpoint/ | Generic fixed-point primitives (formats/rounding/LUT/phase); not present at 36bdc67, pinned at first-appearance commit. |
| reference-manifest-pattern | adapt | torchsynth @ 36bdc67 | spec/reference/upstream.json | Manifest shape only; R01 fills the Dexed pin (commit, engine mode, sample rate, runtime). |
| protocol-specs | adapt | torchsynth @ 36bdc67 | spec/protocol/ | Framing/patch-load/session docs retargeted to DX7 patches at 48 kHz. |
| capability-dag-compiler | adapt | torchsynth @ 36bdc67 | src/gf180_dx7/capabilities.py, tools/compile_capabilities.py | Re-ruled at C01 (issue #45, DR-0004): pattern adopted as locally authored stdlib code with no upstream bytes; the required fixes below are implemented and negative-controlled. |
| spi-reg-control | adapt | parasynth @ cbcc8b9e | rtl/spi_ctl.v, tb/verify_ctl.py | Mode-0 48-bit transport + queue + drain kept; sibling register pages replaced by a DX7 protocol. |
| i2s-transmitter | adapt | parasynth @ cbcc8b9e | rtl/i2s_tx.v | 16-bit serializer pattern; width/strobe/latency decisions re-derived; decode bench is the proof. |
| synth-top-chassis | adapt | parasynth @ cbcc8b9e | rtl/synth_top.v, tb/ | Single-clock 256-cycle frame chassis (tick/go, overrun); DSP instances replaced by the FM engine. |
| negative-control-stub-pattern | adapt | parasynth @ cbcc8b9e | rtl/stubs/ | Keep a deliberately-broken prior revision per component as a standing bench control. |
| ladder-filter-core | **reject** | parasynth @ cbcc8b9e | — | Sibling DSP (ladder core): no FM/DX use; module names are not semantic proof. The N-channel bit-exactness *method* may be re-expressed locally. |
| scorecard | **reject** | parasynth @ cbcc8b9e | — | Inherits the NaN-after-finite-pass defect (below); re-derive the judge locally with finiteness gates. |
| orfs-run-flow | adapt | parasynth @ cbcc8b9e | asic/orfs/run-orfs.sh | Working gf180mcu ORFS driver; import trigger is real ASIC work; stage evidence required for any claim. |
| orfs-gf180-7t-techmaps | **reject** | parasynth @ cbcc8b9e | — | ORFS-derived, attribution unresolved (torchsynth audit P38/P39) and cells unverified against the PDK; re-rule only after tracing + re-verification. |
| orfs-evidence-records | reference-only | parasynth @ cbcc8b9e | — | Historical design-specific routed reports; evidence-contents checklist, not evidence for us. |
| ulx3s-top-wrapper | adapt | parasynth @ cbcc8b9e | fpga/rtl/ulx3s_top.v | Wiring precedent only; as-is board-wrapper import forbidden — pins, reset, clocks re-derived after a board decision. |
| ulx3s-board-constraints | **reject** | parasynth @ cbcc8b9e | — | Board constraints as-is: vendor-derived pin attribution unresolved. |
| fpga-srccheck-makefile | adapt | parasynth @ cbcc8b9e | fpga/Makefile | Cheap iverilog lint-first pattern; retarget file lists. |
| spi-host-driver | adapt | parasynth @ cbcc8b9e | tools/spi_host.py | Host-side 48-bit transaction scheduler; retarget to the DX7 control protocol. |
| link-budget | adopt | parasynth @ cbcc8b9e | tools/link_budget.py | Board-link arithmetic helper with known-answer controls. |
| pll-search | adapt | parasynth @ cbcc8b9e | fpga/scripts/pll_search.py | Divider enumeration; fix the sibling's false "crystal dominates" claim (its 44.1 kHz candidate measured +64 ppm); no tolerance accepted. |
| area-scraper | adapt | parasynth @ cbcc8b9e | tools/area/synth_area.py | Yosys area-report parser behind the sibling's measured gf180mcu calibrations. |
| shared-substrate-dr | adapt | polysynth @ b60e424e | DR-0002 (this repo) | One-master/no-submodule governance, adopted with torchsynth DR-0005 into DR-0002; adapted hash recorded in the catalog. |
| uart-receiver | adapt | polysynth @ b60e424e | rtl/uart_rx.v | Byte receiver only, and only if UART is selected; no voice-parser semantics. |
| rom-table-generator | adapt | polysynth @ b60e424e | rtl/gen_tables.py | Generate-RTL-tables-from-frozen-model-bytes pattern with content hashing. |
| numeric-contract | adapt | polysynth @ b60e424e | spec/NUMERIC-CONTRACT.md | Frozen-literals contract pattern; content is ours (DX7 FM), nothing numeric transfers. |
| pdk-free-runner | adapt | polysynth @ b60e424e | tb/run_tb.py | cocotb runner with the 0/1/2 exit convention required by the A01 acceptance. |
| gf180-sram-macro | reference-only | 2AMLogic/gf180-sram (not pinned) | — | No Liberty views, no local checkout; nothing importable, so nothing pinned. |
| klayout-tools-klt | reference-only | klayout-tools @ d5893304 | — | Existing org-shared ASIC interface (`klt`, MIT); use as a pinned package when ASIC work starts; never fork internals. |
| dexed-msfa-tables | reference-only | asb2m10/dexed (pin = R01) | — | Apache-2.0 per org policy and adoptable **with** provenance, but not before R01 pins commit + files. |
| dexed-enginemki | external-oracle-only | asb2m10/dexed | — | GPL-3.0-or-later comparison oracle; **never copied into this Apache-2.0 repository**. |
| vdx7 | external-oracle-only | external (pin = R01) | — | GPL-3.0-or-later oracle; never copied in. |
| hexter | external-oracle-only | external (pin = R01) | — | GPL-2.0-or-later oracle; never copied in. |
| u05-demo-fixture-bench | adapt | parasynth @ cbcc8b9e | tools/demo/fixtures_dx7.py, tools/demo/demo_fixture.py | Fixture assembly + coverage-counted-from-schedule + refuse-the-under-delivering-fixture negative kept; re-expressed around the U01 phrase set and the H03 link bench (numpy audio, patch/kit state, drum/knob machinery removed). |

## Negative findings inherited from the torchsynth audit

These findings are adoption gates, not folklore; both were probes run by the
torchsynth audit (2026-09-18) against the sibling sources:

1. **Scorecard passes a required NaN after a finite metric.** The parasynth
   scorecard (`tools/scorecard.py`, ruling **reject**) returned `pass` when a
   required metric was NaN following finite ones; schema checks are shallow
   and one worst tolerance-normalized distance mixes properties. Consequence:
   our judge must gate finiteness, keep per-property units/tolerances, and
   report PASS/FAIL/NOT_RUN/BLOCKED/NO_VERDICT/STALE with coverage separate
   from agreement (AGENTS.md reporting rule).
2. **Capability DAG accepts any existing file as passed evidence.** The
   torchsynth capability-DAG compiler (`tools/compile_capabilities.py`,
   ruling **reference-only** — note it is engine-agnostic) accepted an
   arbitrary existing `evidence_file` as passed. Consequence: file existence,
   tags, or ancestry never count as qualification; any future adoption
   requires content-hash verification, schema validation, and a negative
   control that must demonstrably fail.

## What is *not* claimed

- Except for the already-landed `shared-substrate-dr` (DR-0002) and the
  C01 `capability-dag-compiler` adaptation (DR-0004, requalified by
  `tests/test_capabilities.py` + `tools/check_reuse.py` in its import PR),
  every `adopt`/`adapt` requalification above is NOT_RUN until its import PR
  runs it.
- Hash equality establishes byte identity only. Nothing here claims Dexed,
  original-DX7, or sibling fidelity; nothing here claims synthesis, P&R,
  signoff, FPGA playback, or preset quality.
- gf180-sram is reference-only: with no Liberty views it cannot be qualified
  for any use.

## Evidence

- Catalog with per-file SHA-256: [docs/reuse/catalog.json](reuse/catalog.json)
  (35 components; every hash computed from pinned commits; verified 67/67
  against local sibling checkouts via
  `python3 tools/check_reuse.py --verify-upstream`).
- Negative control: `python3 -m unittest discover -s tests -v` (6 tests:
  unrecorded file fails, drifted hash fails, compliant tree passes, missing
  catalog → 2, unhashed adapt destination fails, reject entry cannot own a
  destination) and `bash tools/check_all.sh` → PASS.
- Upstream governance sources: torchsynth DR-0005 + `docs/REUSE-AUDIT.md` +
  `docs/reuse-audit-sources.json` @ `36bdc67`; polysynth DR-0003 @ `b60e424e`.
