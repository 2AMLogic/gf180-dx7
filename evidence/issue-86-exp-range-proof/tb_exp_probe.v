// Issue #86 cross-check testbench (driven by tools/exp_range_proof.py
// --mode xcheck).  Instantiates the real dx7_core and drives its exp()
// unit directly via hierarchical force on exp_amd/exp_ams/exp_start
// (exp_ams = 2^24 makes exp_x = exp_amd, so exp_y = the probed Y).
// Prints the unit's intermediate registers, the in-range bits of
// exp_t3/t4/t5, and exp_hsum as the simulator evaluates it (including the
// out-of-range reads), once exp_st reaches 10 (exp_hsum is then a function
// of the settled exp_xq/p2..p5).  Scratch probe only; not part of any
// product build.
`timescale 1ns/1ps
`default_nettype none
module tb_exp_probe;
    reg clk = 1'b0;
    reg rst_n = 1'b0;
    always #5 clk = ~clk;
    wire miso, bclk, lrclk, i2sd, mv, ov, of, so;
    wire [15:0] tf, tl;
    wire [22:0] tm;
    dx7_core dut (.clk(clk), .rst_n(rst_n), .spi_sck(1'b0), .spi_mosi(1'b0),
                  .spi_cs_n(1'b1), .spi_miso(miso), .i2s_bclk(bclk),
                  .i2s_lrclk(lrclk), .i2s_d(i2sd), .tap_frame(tf),
                  .tap_live(tl), .tap_mix(tm), .tap_mix_valid(mv),
                  .status_overrun(ov), .status_overflow(of), .state_obs(so));

    reg [24:0] ys [0:63];
    integer i, n;
    reg [31:0] lcg;
    initial begin
        ys[0] = 25'd0; ys[1] = 25'd1; ys[2] = 25'd8388608;
        ys[3] = 25'd16777215; ys[4] = 25'd16777216; ys[5] = 25'd11946402;
        ys[6] = 25'd12345678; ys[7] = 25'd1000000;
        lcg = 32'd86;
        for (i = 8; i < 64; i = i + 1) begin
            lcg = lcg * 32'd1664525 + 32'd1013904223;
            ys[i] = {1'b0, lcg[31:8]};
        end
        force dut.exp_start = 1'b0;
        force dut.exp_ams = 25'd16777216;
        repeat (8) @(posedge clk);
        rst_n = 1'b1;
        repeat (8) @(posedge clk);
        for (n = 0; n < 64; n = n + 1) begin
            force dut.exp_amd = ys[n];
            @(negedge clk);
            force dut.exp_start = 1'b1;
            @(negedge clk);
            force dut.exp_start = 1'b0;
            while (dut.exp_st != 4'd10) @(negedge clk);
            $display("Y=%0d xq=%0h p2=%0h p3=%0h p4=%0h p5=%0h t3=%0h t4=%0h t5=%0h hsum=%0h",
                     ys[n], dut.exp_xq, dut.exp_p2, dut.exp_p3, dut.exp_p4,
                     dut.exp_p5, dut.exp_t3, dut.exp_t4, dut.exp_t5,
                     dut.exp_hsum);
            while (dut.exp_st != 4'd0) @(negedge clk);
        end
        $finish;
    end
endmodule
