# H07 evidence re-certification (2026-09-23)

The merged H07 evidence (results-verilog-accept*.json, results-iverilog-shadow.json)
was re-certified against `origin/main` after the H08 pin-level bench surfaced an
8-sample onset question that required independent confirmation.

## What ran

Full dev+stress conformance on the AWS repo-remote box, from a fresh sync of
`origin/main` (merge commit 2fabaf2 lineage), Verilator sweep + second clean run:

- `results-recert-accept.json` — **34/34 pass**
- `results-recert-run2.json` — **34/34 pass**, every per-case artifact sha256
  identical to the accept run (determinism)
- `rtl_fingerprint` = `25e7e1ebaf65…` — identical to the original committed
  evidence, confirming the merged RTL is exactly what the original evidence
  certified.

## Incident note (process learning)

During H08 bring-up an evidence-staleness alarm was raised by comparing the
recorded `rtl_sha256` (a 3-file concatenated fingerprint from
`rtl_fingerprint()`) against a single-file hash of `rtl/dx7_core.v`. That
comparison was invalid (unlike hashes); re-derivation cleared it. The recert
run above makes the confirmation independent of that analysis regardless.

Open item carried to H08 (#30): the pins-bench driver's single-segment
comparator invocation shows an 8-sample ≤2-LSB onset-edge difference vs the
golden on `dir-base` while the committed segment-per-commit comparator path
reports exact — the H08 harness adopts the H07 comparator invocation verbatim
to close this.
