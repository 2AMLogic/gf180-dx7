# Functional-verification evidence (T1 item 5, functional half) — issue #150

A committed `klt functional-verification` report for the bit-exact
RTL-vs-frozen-model suite on the DR-0012 core pin. It is machine-readable
evidence for **claim (1) only**: `rtl/dx7_core.v` + `rtl/env_unit.v` +
`rtl/alg_router.v` reproduce the frozen integer model sample for sample under
Icarus, over the declared window per case. It is **not** model-vs-reference
agreement, synthesis, STA, place-and-route, hardware playback, or sound.

Item 5 stays `unmet` until the STA half (#103, #32) lands; the signoff
manifest that will cite this report is #149.

## Re-run it (cold checkout)

```
make functional-verification            # check mode, exit 0 = PASS
make functional-verification-controls   # live negative controls
```

Needs `uv`, `iverilog`/`vvp` on `PATH`, and network access once, to build a
throwaway `.venv-fv` in the checkout (`klayout-tools==0.8.0`,
`cocotb==2.1.0`). No host tool is installed or changed. Both targets are
full/wave-gate lane only, not part of `make test-fast` (DR-0009).
**Runtime, measured on an 8-vCPU shared worker with 2 parallel playbacks:
33 min 38 s wall (65 min CPU) for the 23 graded cases**; each control is
about 3.5 min.

## Statuses (`tools/functional_verification.py`)

| status | exit | meaning |
| --- | --- | --- |
| PASS | 0 | re-run matches the committed report; every test passed |
| FAIL | 1 | a verdict, count, per-case output hash or recorded environment differs, or a test failed |
| NOT_RUN / BLOCKED | 2 | `klt`, `cocotb`, `iverilog` or the committed report is missing; never reported as a pass |
| STALE | 3 | the committed report names RTL hashes different from the tree |

Wall-clock fields and absolute paths are excluded from the comparison.
`--write` regenerates `evidence/t1-functional/report.json`.

## What is committed (`evidence/t1-functional/`)

- `request.json` — the klt request (top `dx7_core`, Icarus, pinned seed).
- `report.json` — the klt envelope with per-test verdicts (`tests[]`, one
  cocotb test per case) plus `provenance`: sha256 of `rtl/dx7_core.v`,
  `env_unit.v`, `alg_router.v` and `tb_dx7_core.v`, whether they equal the
  DR-0012 table, klt version, request hash, and per case the compared sample
  count, audible-sample count and sha256 of the captured PCM.
- `control-H07_MUTATE_GLITCH.json`, `control-H07_MUTATE_SUM_ORDER.json` —
  the same request against the mutant builds, restricted to `stress-sum16`;
  both report FAIL for it (first mismatch localized).

## How it is built

`tools/fv_cocotb_dx7_core.py` is a thin cocotb module: one test per case,
each calling `h07_compare.run_dev_case` / `run_stress_case` (the H07
harness's own vector build, golden and bit-exact comparator, factored out of
`main()` with no behavior change). `tb_dx7_core` takes its stimulus through
plusargs the cocotb `Runner` does not pass, so each test plays the vector
through the existing `tb_dx7_core.v` with `vvp` (same Icarus) and the
cocotb-attached `dx7_core` simulation is only the harness anchor.

## What is graded, and what is not

The graded set is 19 dev cases + the 4 stress cases = **23 tests**. Icarus
plays about 2.4 s per 64-sample frame and the remaining dev cases have
events out to block 1125 (tens of minutes each), so these 11 are **NOT_RUN
here** (listed in `provenance.not_graded`): `dev32-03`, `dev32-06`,
`dev32-30`, `ev-all-notes-off`, `ev-all-sound-off`, `ev-repeated-note`,
`ev-retrigger`, `ev-steal-16`, `ev-sum-15`, `ev-sustain-no-pedal`,
`ev-sustain-pedal`. Their earlier results are the Verilator/remote runs under
`evidence/h07-core/`; this report neither re-establishes nor weakens them.

Dev cases are compared over *last event block + 16* golden blocks (so the
window contains the note onset); the earlier 24-frame iverilog shadow prefix
covered only silence for dev cases whose first note-on is at block 37.
Coverage is separate from agreement: each graded dev case compares ~1.2k
audible samples; the pitch-bend/mod-wheel cases ~3.6k. **`stress-burst`
compares only silence** (`audible_samples` 0 — its notes begin and end
inside one window), so its PASS says nothing about audio.

## Bounded findings

- `H07_MUTATE_TICK_SKEW` is not detected by any graded case (measured:
  `stress-burst` and `stress-sum16` both still PASS under it). It drops only
  the last write of a 167-write single-window burst, which no graded case
  produces; `tests/test_h07.py`'s small-burst vector remains its only live
  control. No klt control is claimed for it.
- `H07_MUTATE_GLITCH` was measured PASS on `dir-base` at a 24-frame prefix
  (probably because no voice is sounding at frame 12; not investigated), so
  its control uses `stress-sum16`.
- No `klayout-tools` gap was hit for the cocotb path (item 5 of the issue).
  `klt 0.7.0` has no `-o/--outdir`; the tool runs klt on a scratch copy of
  the request instead.
