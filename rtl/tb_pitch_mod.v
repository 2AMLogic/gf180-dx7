// tb_pitch_mod.v -- H06 conformance harness driver (issue #28).
//
// Drives rtl/pitch_mod.v with the directed vector set produced by
// tools/h06_compare.py from the FROZEN PYTHON MODEL (the golden:
// src/gf180_dx7/model/pitch.py composed per the integrated pinned order)
// and dumps the RTL's actual per-frame outputs for bit-exact comparison.
// The bench renders no verdict on numeric agreement -- the Python
// comparator is the judge; the bench only proves the RTL ran the full
// vector stream and captured one output row per frame tick.
//
// Vector records (whitespace separated, '#' comments, blank lines):
//   W <addr hex2> <data hex8>   one contracted cfg write (per-event; the
//                               comparator's register map defines semantics)
//   F                           one frame tick; after out_valid, the six
//                               phase increments + boundary values latch
// Output records (one per F, in order):
//   <freq0..freq5 hex8> <peg hex8> <lfo_val hex6> <lfo_delay hex6>
//   <amd hex8>
//
// The bench also reports the worst-case control-tail latency measured in
// clocks from frame_tick to out_valid (the H03 derived-estimate check
// consumes that measurement).
//
// Usage (from the repository root so the default ROM paths resolve):
//   iverilog -g2012 -o tb.vvp rtl/pitch_mod.v rtl/tb_pitch_mod.v
//   vvp tb.vvp +vectors=<file> +actual=<file>
`default_nettype none

module tb_pitch_mod;

    localparam MAX_LINE = 512;  // must hold long +vectors=/+actual= paths
    localparam TIMEOUT_CYCLES = 40000000;

    reg         clk = 1'b0;
    reg         rst = 1'b1;
    reg         cfg_wr = 1'b0;
    reg [7:0]   cfg_addr = 8'h0;
    reg [31:0]  cfg_wdata = 32'h0;
    reg         frame_tick = 1'b0;

    wire [31:0] freq0, freq1, freq2, freq3, freq4, freq5;
    wire [31:0] peg_level;
    wire [24:0] lfo_val, lfo_delay;
    wire [31:0] amd_mod;
    wire        out_valid, busy, state_obs;

`ifdef H06_PER_SAMPLE_HOST
    // negative-control build: the illegal per-sample host stream the
    // harness refuses to provide (increments stall without it)
    reg         host_stream_valid = 1'b0;
`endif

    pitch_mod dut (
        .clk        (clk),
        .rst        (rst),
        .cfg_wr     (cfg_wr),
        .cfg_addr   (cfg_addr),
        .cfg_wdata  (cfg_wdata),
        .frame_tick (frame_tick),
        .freq0      (freq0),
        .freq1      (freq1),
        .freq2      (freq2),
        .freq3      (freq3),
        .freq4      (freq4),
        .freq5      (freq5),
        .peg_level  (peg_level),
        .lfo_val    (lfo_val),
        .lfo_delay  (lfo_delay),
        .amd_mod    (amd_mod),
        .out_valid  (out_valid),
        .busy       (busy),
        .state_obs  (state_obs)
`ifdef H06_PER_SAMPLE_HOST
        ,
        .host_stream_valid (host_stream_valid)
`endif
    );

    always #5 clk = ~clk;   // 10 ns period; timing is irrelevant to the check

    integer fvec, fact;
    reg [8*MAX_LINE:1] line;
    integer nread, nargs;
    integer ckind;
    reg [31:0] arg1, arg2;

    integer frames = 0;
    integer writes = 0;
    integer captured = 0;
    integer cycles_this = 0;
    integer max_cycles = 0;

    // capture at negedge: outputs are registered (stable between posedges)
    always @(negedge clk) begin
        if (out_valid) begin
            $fdisplay(fact, "%h %h %h %h %h %h %h %h %h %h",
                      freq0, freq1, freq2, freq3, freq4, freq5,
                      peg_level, lfo_val, lfo_delay, amd_mod);
            captured = captured + 1;
            if (cycles_this > max_cycles)
                max_cycles = cycles_this;
        end
    end

    // count clocks from tick to out_valid (control-tail latency datapoint)
    always @(posedge clk) begin
        if (!busy && frame_tick)
            cycles_this = 0;
        else if (busy || frame_tick)
            cycles_this = cycles_this + 1;
    end

    task do_write;
        input [7:0]  addr;
        input [31:0] data;
        begin
            @(negedge clk);
            cfg_wr = 1'b1;
            cfg_addr = addr;
            cfg_wdata = data;
            @(negedge clk);
            cfg_wr = 1'b0;
            writes = writes + 1;
        end
    endtask

    task do_frame;
        begin
            while (busy) @(negedge clk);
            @(negedge clk);
            frame_tick = 1'b1;
            @(negedge clk);
            frame_tick = 1'b0;
            cycles_this = 1;
            while (!out_valid) @(negedge clk);
            frames = frames + 1;
        end
    endtask

    integer i;
    initial begin
        if (!$value$plusargs("vectors=%s", line)) begin
            $display("FAIL tb_pitch_mod: missing +vectors=<file>");
            $finish;
        end
        fvec = $fopen(line, "r");
        if (fvec == 0) begin
            $display("FAIL tb_pitch_mod: cannot open vector file %0s",
                     line);
            $finish;
        end
        if (!$value$plusargs("actual=%s", line)) begin
            $display("FAIL tb_pitch_mod: missing +actual=<file>");
            $finish;
        end
        fact = $fopen(line, "w");
        if (fact == 0) begin
            $display("FAIL tb_pitch_mod: cannot open actual file %0s",
                     line);
            $finish;
        end

        // reset
        @(negedge clk);
        rst = 1;
        @(negedge clk);
        @(negedge clk);
        rst = 0;

        while (!$feof(fvec)) begin
            nread = $fgets(line, fvec);
            if (nread <= 0)
                continue;
            if (line[8*MAX_LINE -: 8] == "#")
                continue;                       // comment
            if (line[8*MAX_LINE -: 8] == "\n" ||
                line[8*MAX_LINE -: 8] == "\r" ||
                line[8*MAX_LINE -: 8] == " " ||
                line[8*MAX_LINE -: 8] == "\t")
                continue;                       // blank
            nargs = $sscanf(line, " %c %h %h", ckind, arg1, arg2);
            if (nargs < 1)
                continue;                       // unparsable: comparator's job
            if (ckind == "W") begin
                if (nargs < 3)
                    continue;                   // malformed: comparator's job
                do_write(arg1[7:0], arg2);
            end else if (ckind == "F")
                do_frame();
        end
        @(negedge clk);
        $fclose(fvec);
        $fclose(fact);

        if (captured == frames && frames > 0)
            $display(
        "PASS tb_pitch_mod (frames=%0d writes=%0d captured=%0d maxcycles=%0d)",
                frames, writes, captured, max_cycles);
        else
            $display(
        "FAIL tb_pitch_mod (frames=%0d writes=%0d captured=%0d maxcycles=%0d)",
                frames, writes, captured, max_cycles);
        $finish;
    end

    // watchdog
    initial begin
        for (i = 0; i < TIMEOUT_CYCLES; i = i + 1)
            @(posedge clk);
        $display("FAIL tb_pitch_mod (timeout after %0d cycles)",
                 TIMEOUT_CYCLES);
        $finish;
    end

endmodule

`default_nettype wire
