# DR-0010: gf180 7t techmaps — rederivation from the pinned ORFS image and provenance re-ruling

- **DR number:** 0010
- **Status:** Proposed (builder's record; the re-ruling of the previously
  `reject`ed catalog entry requires owner review)
- **Date:** 2026-09-23
- **Deciders:** loom-builder (agent), H10 (#32)
- **Related issues:** #32 (H10)

## Context

Issue #39 / the reuse catalog (`docs/reuse/catalog.json`) records the
component `orfs-gf180-7t-techmaps` (the sibling's
`pnr/orfs/gf180_7t/cells_adders.v`, `cells_latch.v`, `cells_latch.v`,
`synth_unsigned.tcl` from `gf180-parasynth`) as **`reject`**: "the ORFS
platform hard-codes 9t cell names in its techmaps; the 7t copies'
attribution is unresolved (torchsynth audit P38/P39) and the cells are not
re-verified against the gf180mcu PDK. May be re-ruled adopt/adapt only
after tracing original ORFS provenance and cell-by-cell re-verification
against the PDK — a new decision record."

H10 needs a working `TRACK_OPTION=7t` ORFS synthesis for the gf180mcu
mapped feasibility run. The pinned flow image
`openroad/orfs:26Q3-296-gda37dce1c` (image manifest digest
`sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2`,
the same image the sibling routed with — see its `pnr/orfs/README.md`)
ships the platform source:

- `flow/platforms/gf180/cells_adders.v` —
  `sha256 b310f12c9f6003b46c1f7e4d4fa98de1304af435efe31de12f41a5952a821cbf`
- `flow/platforms/gf180/cells_latch.v` —
  `sha256 7e0cfebde5bab67c6ea7f2334c6ae786ef8f475cd18027fce53f8c26ed975626`

Both hard-code `gf180mcu_fd_sc_mcu9t5v0__*` cell names; on
`TRACK_OPTION=7t` those names are not cells of the 7t library and the
stock flow leaves undriven adder/latch nets (the sibling hit the same and
made 7t copies).

## Decision

The 7t techmaps are **derived in this repository from the pinned image's
own platform files** — not re-imported from any sibling checkout — by the
single documented token substitution

```
gf180mcu_fd_sc_mcu9t5v0  ->  gf180mcu_fd_sc_mcu7t5v0
```

and committed at `asic/orfs/gf180_7t/cells_adders.v` and
`asic/orfs/gf180_7t/cells_latch.v`, each carrying a provenance header
recording the image digest, the two source sha256 values, and the
substitution. The sibling's `synth_unsigned.tcl` is **not** imported: this
image's stock `synth_preamble.tcl` reads all `VERILOG_FILES` with
`read_verilog -defer -sv`, so the unsigned-cast workaround is unneeded and
the stock synthesis pipeline runs unmodified.

Re-verification performed (the attribution + cell-by-cell gate the `reject`
set):

1. **Attribution traced**: the derivation source is the pinned image's
   platform source (hashes above), a component of the ORFS project the
   image is built from; no sibling bytes enter the committed files.
   Observation recorded for transparency: the sibling's 7t files at
   `gf180-parasynth` @ `ada47b26aa87fd247808051803d9a4b5b2090359`
   (`cells_adders.v` `sha256 686fc8100a3d...`, `cells_latch.v`
   `sha256 bd21a88f1ce3...`) are **this derivation's exact output plus a
   4-line header comment only** — i.e. the sibling made the same mechanical
   conversion; the byte streams agree, so no information was lost by
   deriving independently.
2. **Cell-by-cell check against the PDK reference**: the 7t tt liberty
   (`gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, `sha256
   330aa9439255e3870ad7adfb987cd4d7bcfa7b6f9fb0cd0a712400e9bd80e38a`, the
   family-pinned PDK version 54435919) contains every
   `gf180mcu_fd_sc_mcu7t5v0__*` cell referenced by the two techmaps.
3. **Round-trip pin**: `tools/verify_gf180_7t_techmaps.py` re-derives each
   committed file from an image platform directory and fails on any byte
   drift; it also refuses a liberty that parses to zero cells (no vacuous
   passes). Both checks enter the fast lane via `tests/test_h10_techmaps.py`
   (the hermetic negative controls: missing cell, drifted transform) plus a
   real-PDK check when the pinned PDK is mounted.

Catalog effect: a new entry
`orfs-gf180-7t-techmaps-rederived` (ruling `adapt`, destination
`asic/orfs/gf180_7t/`) supersedes the operational conclusion of
`orfs-gf180-7t-techmaps`; the old entry is retained verbatim as the
historical record of the rejection.

## Consequences

- ORFS `TRACK_OPTION=7t` runs use `ADDER_MAP_FILE` / `LATCH_MAP_FILE`
  pointing at the rederived files (selected in `asic/orfs/dx7core/config.mk`);
  the stock 9t platform files remain untouched inside the image.
- Any future ORFS image bump must re-run the transform check against the
  new image's platform files (the tool takes `--image-dir`); a drifted
  image breaks the fast lane, which is the intended behaviour.
- Superseded when a revised ORFS ships a working 7t platform techmap
  natively (delete these files, use the platform ones, re-run the mapped
  feasibility evidence — every number in that case is STALE for policy
  comparison, not just stale).

## Evidence

- Committed files: `asic/orfs/gf180_7t/cells_adders.v`,
  `asic/orfs/gf180_7t/cells_latch.v` (provenance headers in-file).
- Verifier + test: `tools/verify_gf180_7t_techmaps.py`,
  `tests/test_h10_techmaps.py` (fast lane, `tools/test_fast.sh`).
- The mapped run itself: `evidence/h10-gf180/` and
  `docs/H10-GF180-FEASIBILITY.md` (H10).