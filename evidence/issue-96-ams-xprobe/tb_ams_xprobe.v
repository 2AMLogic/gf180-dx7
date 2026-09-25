// Issue #96 AMS-path X-propagation probe (driven by tools/ams_xprobe.py).
//
// Instantiates the REAL dx7_core, drives its REAL SPI port with a real
// patch page + commit + note-on (frames pre-serialised by the Python
// driver into +frames=<memh>, one 48-bit SPI word per line), then runs a
// bounded number of wire frames while watching the AM/LFO exp() datapath
// for 4-state unknowns:
//
//   exp_hsum  -- the Taylor sum that reads exp_t3[85:30]/exp_t4[85:30]/
//                exp_t5[81:26] out of range (dx7_core.v:953-961)
//   exp_pt    -- the exp() unit's registered output (dx7_core.v:1010)
//   exp_li    -- the AM-attenuated level (dx7_core.v:1051)
//   op_li     -- the per-operator level handed to alg_router
//   tap_mix   -- the mixer output the conformance dump records
//
// It reports, per signal, the first wire frame at which any bit is x/z,
// plus how many dumped samples (tap_mix_valid) carried an unknown.  The
// conformance testbench (rtl/tb_dx7_core.v) writes tap_mix with $fwrite
// "%c", which renders an x byte as NUL -- so an unknown sample is
// indistinguishable from a real 0 in actual.i32.  This probe looks at the
// 4-state value directly, before that lossy step.
//
// Plusargs:
//   +frames=<memh>   SPI frames to send before the note-on (patch + commit)
//   +events=<memh>   SPI frames to send as the note-on burst
//   +runframes=<n>   wire frames to run after the note-on (default 12)
//
// Scratch probe only; not part of any product build.
`timescale 1ns/1ps
`default_nettype none
module tb_ams_xprobe;
    reg clk = 1'b0;
    reg rst_n = 1'b0;
    reg spi_sck = 1'b0;
    reg spi_mosi = 1'b0;
    reg spi_cs_n = 1'b1;
    wire spi_miso, i2s_bclk, i2s_lrclk, i2s_d;
    wire [15:0] tap_frame, tap_live;
    wire [22:0] tap_mix;
    wire tap_mix_valid, status_overrun, status_overflow, state_obs;

    dx7_core dut (
        .clk(clk), .rst_n(rst_n),
        .spi_sck(spi_sck), .spi_mosi(spi_mosi), .spi_cs_n(spi_cs_n),
        .spi_miso(spi_miso),
        .i2s_bclk(i2s_bclk), .i2s_lrclk(i2s_lrclk), .i2s_d(i2s_d),
        .tap_frame(tap_frame), .tap_live(tap_live), .tap_mix(tap_mix),
        .tap_mix_valid(tap_mix_valid),
        .status_overrun(status_overrun), .status_overflow(status_overflow),
        .state_obs(state_obs)
    );

    always #5 clk = ~clk;   // 10 ns period (same as rtl/tb_dx7_core.v)

    // ---- SPI master (identical protocol to rtl/tb_dx7_core.v) ----------
    integer bit_i;
    reg [47:0] tx_frame;
    task spi_xfer(input [47:0] f);
        begin
            tx_frame = f;
            @(negedge clk);
            spi_cs_n = 1'b0;
            for (bit_i = 47; bit_i >= 0; bit_i = bit_i - 1) begin
                spi_mosi = tx_frame[bit_i];
                spi_sck = 1'b0; #20;
                spi_sck = 1'b1; #20;
            end
            spi_sck = 1'b0;
            #20;
            spi_cs_n = 1'b1;
            #20;
        end
    endtask

    // ---- X monitors ----------------------------------------------------
    // first_* : wire frame at which the signal first showed an x/z bit
    //           (-1 = never).  n_*_samples counts dumped samples carrying
    //           an unknown.
    integer first_hsum, first_pt, first_li, first_wli, first_mix;
    integer n_mix_samples, n_mix_unknown, n_opli_unknown;
    integer run_frames;
    integer note_frame;

    function integer isx56(input [56:0] v);
        isx56 = (^v === 1'bx) ? 1 : 0;
    endfunction
    function integer isx32(input [31:0] v);
        isx32 = (^v === 1'bx) ? 1 : 0;
    endfunction
    function integer isx23(input [22:0] v);
        isx23 = (^v === 1'bx) ? 1 : 0;
    endfunction

    always @(posedge clk) if (rst_n) begin
        if (first_hsum < 0 && isx56(dut.exp_hsum))
            first_hsum = dut.frame_ctr;
        if (first_pt < 0 && isx32(dut.exp_pt))
            first_pt = dut.frame_ctr;
        if (first_li < 0 && isx32(dut.exp_li))
            first_li = dut.frame_ctr;
        if (first_wli < 0 && isx32(dut.w_li))
            first_wli = dut.frame_ctr;
        if (tap_mix_valid === 1'b1) begin
            n_mix_samples = n_mix_samples + 1;
            if (isx23(tap_mix)) begin
                n_mix_unknown = n_mix_unknown + 1;
                if (first_mix < 0) first_mix = dut.frame_ctr;
            end
        end
    end

    // ---- stimulus ------------------------------------------------------
    reg [47:0] pframe [0:511];
    reg [47:0] eframe [0:63];
    reg [1023:0] pfile, efile;
    integer i, np, ne;

    task run_wire_frames(input integer n);
        integer k;
        begin
            for (k = 0; k < n; k = k + 1) begin
                @(negedge clk);
                while (dut.sample_clk != 9'd511 || dut.sample_idx != 6'd62)
                    @(negedge clk);
            end
        end
    endtask

    initial begin
        first_hsum = -1; first_pt = -1; first_li = -1;
        first_wli = -1; first_mix = -1;
        n_mix_samples = 0; n_mix_unknown = 0; n_opli_unknown = 0;
        for (i = 0; i < 512; i = i + 1) pframe[i] = 48'h0;
        for (i = 0; i < 64; i = i + 1) eframe[i] = 48'h0;
        if (!$value$plusargs("frames=%s", pfile)) begin
            $display("PROBE-ERROR missing +frames="); $finish;
        end
        if (!$value$plusargs("events=%s", efile)) begin
            $display("PROBE-ERROR missing +events="); $finish;
        end
        if (!$value$plusargs("runframes=%d", run_frames)) run_frames = 12;
        $readmemh(pfile, pframe);
        $readmemh(efile, eframe);
        np = 0;
        while (np < 512 && pframe[np] !== 48'h0) np = np + 1;
        ne = 0;
        while (ne < 64 && eframe[ne] !== 48'h0) ne = ne + 1;

        repeat (10) @(negedge clk);
        rst_n = 1'b1;
        repeat (10) @(negedge clk);

        for (i = 0; i < np; i = i + 1) spi_xfer(pframe[i]);
        run_wire_frames(2);
        note_frame = dut.frame_ctr;
        for (i = 0; i < ne; i = i + 1) spi_xfer(eframe[i]);
        run_wire_frames(run_frames);

        for (i = 0; i < 6; i = i + 1)
            if (isx32(dut.op_li[0][i]))
                n_opli_unknown = n_opli_unknown + 1;

        $display("PROBE frames=%0d events=%0d note_frame=%0d end_frame=%0d",
                 np, ne, note_frame, dut.frame_ctr);
        $display("PROBE first_x_frame hsum=%0d pt=%0d li=%0d wli=%0d mix=%0d",
                 first_hsum, first_pt, first_li, first_wli, first_mix);
        $display("PROBE opli_slot0_unknown_ops=%0d", n_opli_unknown);
        $display("PROBE samples=%0d unknown_samples=%0d live=%0d overrun=%0d overflow=%0d",
                 n_mix_samples, n_mix_unknown, tap_live, status_overrun,
                 status_overflow);
        $display("PROBE-DONE");
        $finish;
    end

    // watchdog: a deadlocked run must not hang the driver
    initial begin
        #(64'd20_000_000_000);
        $display("PROBE-WATCHDOG timeout");
        $finish;
    end
endmodule
`default_nettype wire
