# Functional-verification evidence (T1 item 5, functional half) — issue #150

A committed `klt functional-verification` report for the bit-exact
RTL-vs-frozen-model suite on the DR-0012 core pin. It is machine-readable
evidence for **claim (1) only**: `rtl/dx7_core.v` + `rtl/env_unit.v` +
`rtl/alg_router.v` reproduce the frozen integer model sample for sample under
Icarus, over the declared window per case. It is **not** model-vs-reference
agreement, synthesis, STA, place-and-route, hardware playback, or sound.

Item 5 stays `unmet` until the STA half (#103, #32) lands. #149 landed
`signoff/manifest.json` citing items 9 and 10 only — it never cited this
report; #202 added the item 5 citation. With it, klt grades the citation
`met` and item 5 `unmet` / `partial_coverage` with
`detail.missing_kinds: ["sta"]`, and `t1_met_count` stays `2`. See
[`signoff/README.md`](../signoff/README.md) § "Item 5".

## Capability registration (`rtl-exact`, issue #276)

`python3 tools/functional_verification.py` in check mode is the registered
`rtl-exact-v1` capability check (`src/gf180_dx7/capabilities.py`). It is
read-only: it never rewrites `evidence/t1-functional/report.json`, whose
sha256 is bound into the stamp as the baseline. Because 11 dev cases are
never graded in this lane (below), a run where every selected window agrees
is a **guarded skip**, not a pass: the check exits 0, prints the token
`GUARDED_SKIP`, and `python3 tools/compile_capabilities.py --stamp
rtl-exact` records **NOT_RUN** (`skip_policy: not_run`). Verdict
composition, per case:

| situation | reads |
| --- | --- |
| 23 selected windows bit-exact, 11 dev cases ungraded | NOT_RUN (guarded skip; exit 0 + token) |
| a selected case failed at the comparator | FAIL, with the first divergence (dump index, golden, actual) |
| a selected case did not execute (absent, skipped, or the driver's "could not run") | NOT_RUN, naming the case — zero simulator skips cannot establish coverage |
| `klt`, cocotb or `iverilog` missing | NOT_RUN (exit 2 + token) |
| the committed report names other RTL than the tree | STALE (exit 3; recorded FAIL, diagnosis kept in the stamp tail) |

A measured mismatch outranks unexecuted or ungraded work; a stale pin
outranks both. The check prints its per-case coverage record last — one
token per selected case (`id:verdict:checked`, the first divergence on
FAIL), the ungraded ids and their reason — so the stamp's 2,000-character
output tail in `evidence/capabilities/rtl-exact.json` retains all of it
(`tests/test_capabilities.py` recovers it with `parse_coverage`). The two
controls below are the node's required negative controls; each counts as
detected only when the selected case **fails at the sample comparator**
with a measured divergence — a cocotb `failed` alone also covers setup
failures, and a surviving mutant or a missing case is never a detection.

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
| PASS | 0 | re-run matches the committed report; every selected test passed (still a guarded skip for the capability stamp while dev cases remain ungraded — see above) |
| FAIL | 1 | a verdict, count, per-case output hash or recorded environment differs, or a selected test failed at the comparator (first divergence named) |
| NOT_RUN / BLOCKED | 2 | `klt`, `cocotb`, `iverilog` or the committed report is missing, or a selected case did not execute; never reported as a pass |
| STALE | 3 | the committed report names RTL hashes different from the tree |

Wall-clock fields and absolute paths are excluded from the comparison.
`--write` regenerates `evidence/t1-functional/report.json`.

## Bumping the pinned toolchain

`Makefile`'s `FV_PINS` and the committed evidence must move together: the
re-run compares `provenance` (including `klt_version`, the stripped stdout
of `klt --version`) and the recorded environment (including
`cocotb_version`) verbatim, so a bump without a regeneration render would
turn this lane FAIL-on-provenance at the next wave gate. The recurrence
guard `tools/check_fv_pins.py` (issue #226) makes that visible at bump
time, in the fast lane: it fails `make check` / `bash tools/check_all.sh`
and `make test-fast` (`tests/test_check_fv_pins.py`) naming the file, the
pinned version and the recorded version. When it fails:

1. bump `FV_PINS` in the `Makefile`;
2. regenerate: `rm -rf .venv-fv` (the make venv rule does not re-run while
   `.venv-fv/bin/klt` exists, so the old-pin venv would silently re-record
   the old versions), then `make functional-verification` (recreates the
   venv from the new `FV_PINS`; that run shows the provenance mismatch),
   then `python3 tools/functional_verification.py --venv .venv-fv --write`
   for the evidence, and the same with `--control H07_MUTATE_GLITCH` and
   with `--control H07_MUTATE_SUM_ORDER` for the two controls (`--write`
   is a flag of `tools/functional_verification.py`, not of the make
   targets);
3. update `docs/reuse/catalog.json` if any byte-pinned `tools/` file
   changed.

## What is committed (`evidence/t1-functional/`)

- `request.json` — the klt request (top `dx7_core`, Icarus, pinned seed).
- `report.json` — the klt envelope with per-test verdicts (`tests[]`, one
  cocotb test per case) plus `provenance`: sha256 of `rtl/dx7_core.v`,
  `env_unit.v`, `alg_router.v` and `tb_dx7_core.v`, whether they equal the
  DR-0012 table, klt version, request hash, and per case the compared sample
  count, audible-sample count and sha256 of the captured PCM.
- `control-H07_MUTATE_GLITCH.json`, `control-H07_MUTATE_SUM_ORDER.json` —
  the same request against the mutant builds, restricted to `stress-sum16`;
  both report FAIL for it at the sample comparator (first mismatch
  localized: dump index 849, 23300 vs 23316 for the glitch build; index
  397, 2 vs 1024 for the sum-order build).

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

## Verilator C++ compiler override (H07 / H08 live tests, issue #343)

Scope: the H07 negative controls (`tests/test_h07.py`), the H08 live controls
(`tests/test_h08.py`) and `tools/h08_pin_check.py`, which all build a bench
with `verilator --binary`. The installed Verilator's `verilated.mk` selects the
C++ compiler; on a host where that pairing is unusable (e.g. clang-only flags
with `c++` resolving to g++) pick a compatible compiler with the existing GNU
Make override, which reaches Verilator through the environment:

```
make test CXX=clang++
verilator --binary --timing -MAKEFLAGS "CXX=clang++" ...   # direct invocation
```

Compatible installations keep the platform default. After a failed cold bench
build both harnesses build an independent timed module with the same
Verilator/compiler settings (`tests/vlt_toolchain.py`): if that also fails the
live case is **NOT_RUN** with the compiler diagnostic; if it succeeds the
project failure is a **FAIL** with the original diagnostic; an inconclusive
probe stays a FAIL. A missing Verilator is NOT_RUN. This does not change what
the live controls assert.

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
  the request instead. (Observed on `klt 0.7.0`, the version the original
  report was produced with; not re-checked on the `0.8.0` pin as of
  2026-11-15 — the scratch-copy workaround stands either way.)
