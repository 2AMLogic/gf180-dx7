# Decision record: Re-pin the R01 oracle toolchain to Ubuntu 24.04 clang 18 (Linux x86_64)

- **DR number:** 0014
- **Status:** Proposed
- **Date:** 2026-11-19
- **Deciders:** Builder implementation of the #200 decision (2026-11-18, fleet
  decision block, option (a), confidence 85%); manifest review happens in the
  change PR
- **Related issues:** #200, #6, #201
- **Change control:** DEC-001 (docs/DECISIONS-v1.md)

## Context

R01 (`reference/manifest.json`) pinned the oracle build toolchain as recorded
at manifest time (2026-09-20) on the operator's macOS pin machine:
`Apple clang version 21.0.0 (clang-2100.1.1.101)`, target
`arm64-apple-darwin25.5.0`, Xcode toolchain path, Python 3.14.7.
`tools/verify_reference.py` `verify_toolchain()` compares the exact first line
of `clang++ --version` against `toolchain.cxx_compiler.version_line` and
reports any other line as drift (exit 1) — a host-determined gate independent
of the Dexed clone.

The macOS hosts left the fleet on 2026-10-22. Measured on the Linux fleet
workers: `clang++ --version` reports `Ubuntu clang version 18.1.3 (1ubuntu1)`
(Ubuntu 24.04 amd64, package `clang-18 1:18.1.3-1ubuntu1`), so the verifier
exits 1 everywhere the fleet can run, `reference-pin` cannot be re-stamped to
PASS, and 13 of 15 capability nodes stay BLOCKED behind it. Each capability
check registration re-STALEs this node's coverage
(`EVALUATOR_INPUTS` in `src/gf180_dx7/capabilities.py`; observed with
#201/#205), so the fleet needs a re-stamp path it can actually run.

The #200 decision block (2026-11-18) selected option (a): re-pin the
toolchain to one the fleet can reproduce, as a reviewed manifest change under
DEC change control, keeping the toolchain assertion enforced.

## Decision

Re-pin `toolchain` in `reference/manifest.json` to the Linux fleet toolchain:

- `cxx_compiler.version_line`: `Ubuntu clang version 18.1.3 (1ubuntu1)`
  (was `Apple clang version 21.0.0 (clang-2100.1.1.101)`)
- `cxx_compiler.target`: `x86_64-pc-linux-gnu` (was
  `arm64-apple-darwin25.5.0`)
- `cxx_compiler.installed_dir`: `/usr/bin`, resolving to
  `/usr/lib/llvm-18/bin/clang` (was the Xcode toolchain path)
- `python_runtime_of_record.version`: `3.12.3` (was `3.14.7`)
- New `provisioning` block: Ubuntu 24.04 LTS (noble) amd64, package
  `clang-18 1:18.1.3-1ubuntu1` (version-bearing) plus the `clang` metapackage
  `1:18.0-59~exp2` (supplies `/usr/bin/clang++`), from the Ubuntu archive
  (noble/noble-updates, universe); install `sudo apt-get update && sudo
  apt-get install -y clang`, verify with `clang++ --version`.
- `drift_policy.command`: `python3 tools/verify_reference.py --dexed-root
  .oracle/dexed` (was the departed Mac path `/Users/joseph/dev/dexed`).

Unchanged: the Dexed commit pin `b6cf2e9190e216387ad96f977284ebbef765b447`,
Mark I engine mode, 48 kHz, every pinned source file hash, render settings,
and the exact version-line drift rule itself.

## Consequences

- The exact version-line drift check stays active and now refuses any line
  other than `Ubuntu clang version 18.1.3 (1ubuntu1)`; future re-pins still
  require a manifest change and review.
- **Prior-pin evidence is STALE for claims against the replacement pin until
  requalified.** The committed `reference-pin` PASS record (2026-09-20) and
  the `oracle-identity`/`renderer`/`comparator` evidence downstream of it
  were recorded under the Apple clang pin; oracle *builds* made under the new
  pin are a different compiler/OS/ABI until requalification says otherwise.
  (This change itself re-stamps only `reference-pin`, whose check is the
  verifier — commit, file hashes and toolchain line — not an oracle build.)
- **R02**: `reference/oracle-identity.json` still pins the historical
  Apple-clang-built oracle binary (sha256, path on the departed Mac). No
  replacement binary was built, and no comparison against a Linux-built
  oracle was performed in this change: such a build/comparison is **NOT_RUN**
  and remains out of scope per issue #200 (separate R02 work).
- **R04/R07/N08**: no comparator threshold, acceptance budget, or numeric
  profile value changes. Those statements reference the pinned reference by
  commit/engine/rate, which are untouched; only the build-environment
  attribution of the oracle binary changes.
- **Fidelity statements** are unchanged in substance: Dexed agreement is not
  original-DX7 fidelity; this DR changes where the oracle is compiled, not
  what it is.

## Durability

The recorded recipe does not reproduce the pin. `provisioning.install_commands`
install the unversioned `clang` metapackage, so they resolve to whatever
clang noble/noble-updates currently ships; the pin is the recorded
`cxx_compiler.version_line` and `provisioning.packages` values, and nothing in
the recipe holds the two together.

An archive revision bump of the clang-18 source package (e.g.
`1:18.1.3-1ubuntu2`, including security updates) changes the
`clang++ --version` first line, and a freshly provisioned worker following the
recipe is then expected to fail `tools/verify_reference.py` with
`toolchain drift: compiler version line is '<installed>', manifest pins
'<version_line>'` and exit 1. This is the drift rule behaving correctly — no
wrong result can pass; the cost is availability of the re-stamp path, not
correctness. Agreement with the pin holds only while the archive ships the
pinned revision; no long-term reproducibility is claimed.

The remedy is a reviewed re-pin — a manifest change under DEC-001 updating
`version_line` and `provisioning.packages` together, per the existing drift
rule — never an edit to `verify_toolchain()`. A durable provisioning fix
(version-pinned install commands, a snapshot source, or a container digest
pin) would be a separate decision; none is recorded here.

## Evidence

Measured 2026-11-19, loom-worker-3 (Ubuntu 24.04.4 LTS, x86_64), retained in
the PR for #200:

- `clang++ --version` → `Ubuntu clang version 18.1.3 (1ubuntu1)` /
  `Target: x86_64-pc-linux-gnu` / `Thread model: posix` /
  `InstalledDir: /usr/bin`; `/usr/bin/clang++` resolves to
  `/usr/lib/llvm-18/bin/clang`; `dpkg` records `clang-18 1:18.1.3-1ubuntu1`.
- Clean-environment demonstration: `docker run --rm ubuntu:24.04` with
  `apt-get update && apt-get install -y clang` installs `clang-18
  1:18.1.3-1ubuntu1` and `clang++ --version` reports the pinned first line
  verbatim.
- Positive run: `tools/fetch_dexed.sh` → `.oracle/dexed @ b6cf2e91…`; `python3
  tools/verify_reference.py` exit 0 (PASS).
- Controls: `python3 -m unittest -v tests.test_reference_manifest.TestPinnedTree`
  → 5 tests run, 0 skipped, exit 0 (mutated-byte and wrong-HEAD refusals run
  inside it); compiler-drift refusal demonstrated separately with `--manifest`
  on a temporary copy differing only in `version_line` → exit 1 naming the
  drift; missing clone → verifier exit 2 (NOT_RUN).
- Stamp: `python3 tools/compile_capabilities.py --stamp reference-pin` →
  `evidence/capabilities/reference-pin.json` `result=PASS`, `exit_code=0`,
  `pin-drift-refused` detected with `tests_run=5, skipped=0`;
  `--check` exit 0 afterwards.
