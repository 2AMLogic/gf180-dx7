# C01 adoption log — capability DAG (issue #45, DR-0004)

Retained run evidence for the adoption of the capability-DAG pattern
(reference: gf180-torchsynth @ `36bdc67faa1ff2509988b772efcca7a7eb4e7115`,
Apache-2.0, catalog ruling `capability-dag-compiler` = `adapt`, no upstream
bytes). Recorded 2026-09-20 in worktree `wt-c01-capabilities`, branch
`loom/c01-capabilities`.

## Compiler output (view regeneration after the final stamp pass)

```
$ python3 tools/compile_capabilities.py
Capability states: BLOCKED=6, NO_VERDICT=2, PASS=7
Generated capability views.
```

Node states at adoption: PASS = contract, reference-pin, oracle-identity,
renderer, traces, comparator, corpus-dev32. NO_VERDICT = registry,
numeric-profile (planned nodes whose prerequisites are all PASS). BLOCKED =
fixed-model, rtl-core, rtl-exact, fpga, gf180, instrument (planned nodes with
a prerequisite that is not PASS — fixed-model waits on numeric-profile, which
has no registered check yet; the DAG shows exactly where evidence stops).
STALE = 0, FAIL = 0, NOT_RUN = 0.

## `--strict` run log (future release gate — honestly red today)

```
$ python3 tools/compile_capabilities.py --strict; echo "strict exit: $?"
ERROR: --strict requires every node PASS; not PASS: fixed-model, fpga, gf180, instrument, numeric-profile, registry, rtl-core, rtl-exact
Capability states: BLOCKED=6, NO_VERDICT=2, PASS=7
strict exit: 1
```

`--strict` is deliberately NOT wired into CI until these nodes are evidenced.

## Verification runs

- `python3 -m unittest discover -s tests` — OK (includes the 30
  `tests/test_capabilities.py` cases: acceptance (a)–(f), stamping, refusals,
  BLOCKED propagation, registry boundary).
- `bash tools/check_all.sh` — `reuse check PASS: 35 catalog components, 24
  byte-pinned originals, governed destinations clean`; verify_reference PASS;
  render_selfcheck PASS; "all repo checks passed".
- `python3 tools/compile_capabilities.py --check` — "Generated-view agreement
  checked." (CI staleness guard; runs on every pull request).
- `python3 tools/render_issue_dag.py --check` — "README issue-DAG block is
  fresh." (block refreshed for U01 #34 closing after this branch was cut).

## Bounded finding: stale committed smoke evidence on main

As found, `python3 tools/compare_evidence.py` FAILED on main:
`oracle_identity.binary.sha256: committed 'dc4c1b26322caa4ec5086c7a28bd86c9b0566c7c0ac03bf4c95ce178b077d39f' != fresh '6e17a540fd6be55190824539deeb256a25e80675e2424638d3c1a8718388ed42'`
— the committed smoke render predated the oracle binary rebuild pinned by the
R05 update of `reference/oracle-identity.json`. The pinned harness
(`tools/render_reference.py`) was re-run to refresh
`reference/evidence/smoke/`: `render.f32`
(`1bebaf668e82ae5885750566af22ca47cbb4ed9403e6fc121b349c5fc0bd8e9a`) and
`render.wav` (`d963de69e13fe9498bd054684e2b3b09ec8939c0961fc477d9a2d95bb6a21cb4`)
are byte-identical to the committed evidence; only the recorded oracle
metadata changed. After the refresh, `tools/compare_evidence.py` reports:
"fresh re-render byte-matches committed evidence under the pinned oracle;
comparator: PASS (byte_identical)".

## Hashes (SHA-256, adoption-time bytes)

- `src/gf180_dx7/capabilities.py` (adapted bytes, pinned in catalog):
  recorded in `docs/reuse/catalog.json` `capability-dag-compiler.adapted_sha256`
- `tools/compile_capabilities.py` (adapted bytes, pinned in catalog):
  recorded in `docs/reuse/catalog.json` `capability-dag-compiler.adapted_sha256`
- `tests/test_capabilities.py`, `spec/capabilities-v1.json`: pinned under
  `local_originals`
- Evidence records: `evidence/capabilities/<node>.json` (each record carries
  its own coverage hashes; the records are the hashes' authority, so this log
  does not duplicate them)

## Not claimed

Nothing here asserts FPGA or gf180mcu synthesis, place-and-route, signoff, or
hardware playback, original-DX7 fidelity, or preset quality. Dexed agreement
is not original-DX7 fidelity. The eight non-PASS nodes state exactly what is
not yet evidenced.
