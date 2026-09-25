// gf180mcu_fd_sc_mcu7t5v0 techmap: cells_latch.v
//
// Mechanical rederivation of the pinned ORFS platform file
//   openroad/orfs:26Q3-296-gda37dce1c (image digest
//   sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2)
//   flow/platforms/gf180/cells_latch.v  (sha256 7e0cfebde5bab67c6ea7f2334c6ae786ef8f475cd18027fce53f8c26ed975626)
// under the single token substitution
//   gf180mcu_fd_sc_mcu9t5v0  ->  gf180mcu_fd_sc_mcu7t5v0
// (the stock platform file hard-codes 9t cell names; on TRACK_OPTION=7t those
// names are not cells of the 7t library and yosys leaves undriven nets).
//
// Provenance: derived directly from the pinned image's platform source -- not
// re-imported from any sibling checkout. The prior catalog ruling
// (orfs-gf180-7t-techmaps: reject) blocked the sibling copy on attribution
// grounds; DR-0010 records the trace and the cell-by-cell verification
// against the 7t liberty, and re-rules the component. For transparency: a
// functionally identical sibling byte stream
// (gf180-parasynth ada47b26, pnr/orfs/gf180_7t/cells_latch.v, sha256 bd21a88f1ce3ad8087cba16dab01f8cb5b34ce5c8f0dae8b7a7b7fb8daec68d5)
// is this file's body plus a 4-line header comment only -- observed,
// recorded, used for nothing.
//
// License: OpenROAD-flow-scripts, Copyright (c) 2018-2023, The Regents of the
// University of California, BSD 3-Clause -- the ORFS repository license for
// its build/run scripts (image /OpenROAD-flow-scripts/LICENSE_BUILD_RUN_SCRIPTS,
// sha256 fae0db7a4c00c3125f037e5818e8cd0c8aa5c67fff2e8c558312068e9f2d1592,
// full text alongside as LICENSE-ORFS-BSD-3-Clause). The image's
// flow/platforms/gf180/ directory carries no platform-specific license file;
// that gap is recorded in DR-0010 for owner review.
//
// Verification (tools/verify_gf180_7t_techmaps.py, in the fast lane under
// -k h10_techmaps): every referenced gf180mcu cell exists in the
// gf180mcu_fd_sc_mcu7t5v0 tt liberty (PDK reference), and the file
// regenerates byte-identically from the image file under the stated
// transform.
module $_DLATCH_P_(input E, input D, output Q);
    gf180mcu_fd_sc_mcu7t5v0__latq_1 _TECHMAP_REPLACE_ (
        .D(D),
        .E(E),
        .Q(Q)
        );
endmodule

module $_DLATCH_N_(input E, input D, output Q);
    gf180mcu_fd_sc_mcu7t5v0__latsnq_1 _TECHMAP_REPLACE_ (
        .D(D),
        .E(E),
        .Q(Q)
        );
endmodule
