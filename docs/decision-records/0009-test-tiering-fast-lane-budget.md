# DR-0009: Test tiering — `make test-fast` budget, restated with a measured profile

- **DR number:** 0009
- **Status:** Accepted
- **Date:** 2026-09-25
- **Deciders:** loom-builder (agent)
- **Related issues:** #87 (this restatement); originates from commit
  `77a9d22` ("tiering+governance: fast lane (DR-0009), check_reuse
  --union-merge, test tiering norm", 2026-09-21), which introduced
  `tools/test_fast.sh` and the "fast lane (seconds) / full suite (`make
  test`, wave gates)" norm cited by `CLAUDE.md`/`AGENTS.md` and by
  `docs/H07-CORE.md` / `docs/H08-PIN.md`, but never landed a decision-record
  file — this DR fills that gap using #87's measured evidence rather than
  changing the original intent.

## Context

`77a9d22` established the norm restated in `CLAUDE.md`/`AGENTS.md`: `make
test-fast` (seconds) is the default gate for builder/merge loops; the full
suite (`make test`, ~15 min, renders+synthesis) runs at judge approval and
wave gates. No decision record was ever filed for the number it cites
(`docs/decision-records/0009-*.md` did not exist), and the "seconds" budget
was never measured against a host that has Verilator installed.

Issue #87 reported `make test-fast` taking **9:22 wall / ~73 s CPU (12 % CPU
utilization)** on the original reporter's Apple Silicon macOS operator host —
far outside the "seconds" budget, with the low CPU share pointing at waiting
(subprocess/interpreter start-up, sandbox/endpoint-security latency), not
compute.

This DR reports a from-scratch profile taken on a different host — an AWS
Linux dispatch worker, not the macOS host the original observation was made
on — per #87's instruction to remeasure rather than assume the macOS numbers
transfer. Evidence lives in `evidence/dr-0009-fast-lane/` (host, Python
version, Verilator version, commit, and date recorded in each file):

1. **Warm run, Verilator present** (`profile-with-verilator-2026-09-25.txt`):
   386 tests, 11 skipped, **49.08 s wall, 45.26 s user + 0.61 s sys = 45.87 s
   CPU (92 % of wall)**. This host does **not** reproduce the low-CPU-share
   symptom from the macOS report — the time here is compute, not waiting.
   (A separate warm run without `/usr/bin/time` measured 35.9–38.4 s wall on
   this shared 8-vCPU host; wall time varies with the other sweeps this host
   runs concurrently, per this repo's host constraints. CPU time is the
   stable figure.)

2. **Per-test duration breakdown** (`pytest-durations-2026-09-25.txt`, `pytest
   -k <fast-lane expression> --durations=0`): **`tests/test_h08.py`'s
   `TestH08LiveControls` (`test_live_f0_and_freshctl` 31.10 s,
   `test_live_trunc47` 5.72 s, `test_live_ovf` 0.59 s) accounts for 37.41 s
   of the 43.72 s total run — 85.6 %.** Every other fast-lane test file
   combined is under 6.5 s. `test_live_f0_and_freshctl` alone (a 3,000-sample
   live re-run of the H08 pin bench through a Verilator `--timing` binary —
   `Verilator: $finish at 28ms; walltime 23.34s; speed 1.179 ms/s` measured
   directly against the built binary) is larger than the rest of the fast
   lane combined.

3. **Same run with Verilator absent from `PATH`** (a stand-in for what CI
   actually runs — see below —
   `profile-without-verilator-2026-09-25.txt`): 386 tests, 14 skipped (the 3
   `TestH08LiveControls` cases now skip via `shutil.which("verilator")`),
   **5.66 s wall, 4.93 s user + 0.51 s sys = 5.44 s CPU (96 %)**. This
   matches the "seconds" budget exactly.

4. **`TestH08LiveControls` is a deliberate design choice, not an accident.**
   `docs/H08-PIN.md` §"fast lane" states: "the injection/forensic vectors are
   additionally re-run live by `tests/test_h08.py` in the fast lane. The
   remaining ten matrix cases' completeness is re-exercised at the judge wave
   gate (`make test`, DR-0009), not assumed." The class docstring calls this
   the "H07 wall-heavy pattern" and accepts "~30 s cold, cached warm" as the
   cost of re-verifying three small injection vectors live on the executing
   host, catching toolchain drift that a purely-replayed evidence file
   cannot. Coverage is unchanged by this DR: these same three cases already
   run (unfiltered) inside `make test` regardless of the fast lane.

5. **CI's `test` job does not run `tools/test_fast.sh` at all.**
   `.github/workflows/ci.yml`'s `test` job runs `python -m unittest discover
   -s tests -v` (the full, unfiltered suite) on `blacksmith-4vcpu-ubuntu-2404`
   for Python 3.11/3.12/3.13; no step installs Verilator, so
   `TestH08LiveControls` skips there regardless of which lane runs. #87 listed
   CI's `test` job as one of the "exact inputs" for `test_fast.sh`; that
   premise does not hold today. This DR does not change CI wiring (out of
   scope for a fast-lane budget restatement); tracked separately (see
   Consequences).

6. **The failure control still holds.** A fast-lane test was deliberately
   broken (`self.fail(...)` injected into
   `tests/test_registry.py::TestCheckWrapper::test_check_registry_wrapper_exit0`,
   reverted immediately after the run) and `tools/test_fast.sh` still exited
   1 (`negative-control-2026-09-25.txt`). No coverage moved out of the fast
   lane by this DR — see Decision.

## Decision

1. **The "seconds" budget is restated as conditional on Verilator's
   presence on `PATH`**, not relaxed:
   - **Verilator absent** (every CI runner today; any dev host without it
     installed): `make test-fast` runs in **single-digit seconds** — measured
     5.66 s here. The original "seconds" language holds exactly as written.
   - **Verilator present** (an operator/dev host that opted into deeper local
     verification, e.g. this AWS worker, and apparently the macOS host in the
     #87 report): expect **tens of seconds, dominated by
     `TestH08LiveControls`'s live Verilator re-run** (measured 37–38 s of a
     43–50 s total run here) — genuine compute (92–96 % CPU on this host),
     not idle waiting. This is the documented cost of `docs/H08-PIN.md`'s
     live-reverification design, already accepted at `77a9d22` time; #87 is
     the first time it was measured and named.
2. **No test moves out of the fast lane.** `TestH08LiveControls` stays in
   `tools/test_fast.sh`'s `-k h08` selection; its coverage was already
   present in both lanes (it also runs, unfiltered, inside `make test`).
   Removing it from the fast lane would trade away the live-toolchain-drift
   check `docs/H08-PIN.md` explicitly wants there, which #87 forbids doing
   silently and this DR declines to do at all absent a stronger reason than
   a time budget.
3. **The macOS gap beyond compute time (9:22 wall / 73 s CPU / 12 % CPU) is
   not reproduced on this Linux host** (92–96 % CPU at the equivalent
   Verilator-present configuration) and is recorded, per #87's stop/escalate
   clause, as consistent with host-environmental subprocess-spawn or
   sandbox/endpoint-security latency specific to that machine — not fixed in
   this repository. If a future measurement on a macOS host reproduces the
   low-CPU-share symptom with `-X importtime`/`strace`-equivalent evidence
   pointing at a specific in-repo cause (e.g., an avoidable subprocess spawn
   pattern), that is new evidence and can motivate a follow-up fix; today's
   evidence does not localize one.

## Consequences

- `docs/H07-CORE.md` and `docs/H08-PIN.md`'s existing "DR-0009" citations now
  resolve to a real file with the budget they already assumed.
- Builder/merge loops keep using `make test-fast` as the default gate; on a
  host with Verilator installed, budget tens-of-seconds, not seconds, for
  that gate — this is not a regression, it was already true and unmeasured.
- CI's `test` job running the full suite instead of `test-fast` (Context
  item 5) is a separate, pre-existing gap between the documented tiering norm
  and CI's actual wiring; it is not weakened or strengthened by this DR and
  is left for a follow-up issue rather than folded into a budget
  restatement.
- Any change that alters `TestH08LiveControls`'s coverage (removing it from
  the fast lane, reducing its sample count, changing its Verilator
  invocation mode) needs its own DR noting exactly what coverage moved,
  per #87's failure control — this DR does not pre-authorize that.

## Evidence

- `evidence/dr-0009-fast-lane/profile-with-verilator-2026-09-25.txt` — warm
  `tools/test_fast.sh` run, Verilator present, `/usr/bin/time -v`.
- `evidence/dr-0009-fast-lane/profile-without-verilator-2026-09-25.txt` —
  same run with Verilator absent from `PATH` (CI-equivalent).
- `evidence/dr-0009-fast-lane/pytest-durations-2026-09-25.txt` — per-test
  wall-time profile (`pytest -k <fast-lane expression> --durations=0`)
  naming `TestH08LiveControls` as the dominant contributor.
- `evidence/dr-0009-fast-lane/negative-control-2026-09-25.txt` — a
  deliberately broken fast-lane test still yields `tools/test_fast.sh` exit
  code 1.
- `.github/workflows/ci.yml` (`test` job: full `unittest discover`, no
  Verilator install step).
- `docs/H08-PIN.md` (fast-lane live-reverification design intent) and
  `docs/H07-CORE.md` (prior "DR-0009" citations this file now resolves).
