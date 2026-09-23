// gf180mcu_fd_sc_mcu7t5v0 techmap: cells_adders.v
//
// Mechanical rederivation of the pinned ORFS platform file
//   openroad/orfs:26Q3-296-gda37dce1c (image digest
//   sha256:ebc8142da6d65d1a1e9a528aa2cedcde356243465dd859af8d3ade51075f8cb2)
//   flow/platforms/gf180/cells_adders.v  (sha256 b310f12c9f6003b46c1f7e4d4fa98de1304af435efe31de12f41a5952a821cbf)
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
// (gf180-parasynth ada47b26, pnr/orfs/gf180_7t/cells_adders.v, sha256 686fc8100a3d5e69f07e573422611eee9ff509e3a44848bc6f8936f7bc203d98)
// is this file's body plus a 4-line header comment only -- observed,
// recorded, used for nothing.
//
// Verification (tools/verify_gf180_7t_techmaps.py, in the fast lane under
// -k h10_techmaps): every referenced gf180mcu cell exists in the
// gf180mcu_fd_sc_mcu7t5v0 tt liberty (PDK reference), and the file
// regenerates byte-identically from the image file under the stated
// transform.

(* techmap_celltype = "$fa" *)
module _tech_fa (A, B, C, X, Y);
  parameter WIDTH = 1;
  (* force_downto *)
    input [WIDTH-1 : 0] A, B, C;
  (* force_downto *)
    output [WIDTH-1 : 0] X, Y;
  
  parameter _TECHMAP_CONSTVAL_A_ = WIDTH'bx;
  parameter _TECHMAP_CONSTVAL_B_ = WIDTH'bx;
  parameter _TECHMAP_CONSTVAL_C_ = WIDTH'bx;
  
  genvar i;
  generate for (i = 0; i < WIDTH; i = i + 1) begin
      if (_TECHMAP_CONSTVAL_A_[i] === 1'b0 || _TECHMAP_CONSTVAL_B_[i] === 1'b0 || _TECHMAP_CONSTVAL_C_[i] === 1'b0) begin
        if (_TECHMAP_CONSTVAL_C_[i] === 1'b0) begin
          gf180mcu_fd_sc_mcu7t5v0__addh_1 halfadder_Cconst (
              .A(A[i]),
              .B(B[i]),
              .CO(X[i]), .S(Y[i])
            );
        end 
        else begin
          if (_TECHMAP_CONSTVAL_B_[i] === 1'b0) begin
            gf180mcu_fd_sc_mcu7t5v0__addh_1 halfadder_Bconst (
                .A(A[i]),
                .B(C[i]),
                .CO(X[i]), .S(Y[i])
              );
          end
          else begin
            gf180mcu_fd_sc_mcu7t5v0__addh_1 halfadder_Aconst (
                .A(B[i]),
                .B(C[i]),
                .CO(X[i]), .S(Y[i])
              );
          end
        end
      end
      else begin
        gf180mcu_fd_sc_mcu7t5v0__addf_1 fulladder (
            .A(A[i]), .B(B[i]), .CI(C[i]), .CO(X[i]), .S(Y[i])
          );
      end
    end endgenerate

endmodule
