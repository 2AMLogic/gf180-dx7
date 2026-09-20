// tb_operator_probe.v -- H01 equivalence harness driver (issue #20).
//
// Drives rtl/operator_probe.v with the directed vector set produced by
// tools/h01_compare.py from the FROZEN PYTHON MODEL (the golden:
// src/gf180_dx7/model/operator.py) and dumps the RTL's actual outputs for
// bit-exact comparison. This bench itself renders no verdict on numeric
// agreement -- the Python comparator is the judge; the bench only proves
// the RTL ran the full vector stream and captured one output per eval.
//
// Vector records (whitespace separated, '#' comments, blank lines ignored):
//   L <phase hex8>            load the phase register (asserted while en=0)
//   E <freq hex8> <env hex4>  one evaluation: out = mkiSin(phase, env),
//                             then phase += freq (mod 2^32)
// Output records (one per eval, acceptance order):
//   <out_raw hex8> <clip_val hex5>   (17-bit two's-complement clip domain)
//
// Usage (from the repository root so the default ROM paths resolve):
//   iverilog -g2012 -o tb.vvp rtl/operator_probe.v rtl/tb_operator_probe.v
//   vvp tb.vvp +vectors=evidence/h01-operator-probe/vectors.txt \
//       +actual=/tmp/actual.txt
`default_nettype none

module tb_operator_probe;

    localparam MAX_LINE = 256;  // must hold long +vectors=/+actual= paths
    localparam TIMEOUT_CYCLES = 200000;

    reg         clk = 1'b0;
    reg         rst = 1'b1;
    reg         en  = 1'b0;
    reg [31:0]  freq = 32'h0;
    reg [15:0]  env = 16'h0;
    reg         ld_phase = 1'b0;
    reg [31:0]  phase_in = 32'h0;

    wire [31:0] out_raw;
    wire [16:0] clip_val;
    wire        out_valid;
    wire        busy;
    wire        state_obs;

    operator_probe dut (
        .clk       (clk),
        .rst       (rst),
        .en        (en),
        .freq      (freq),
        .env       (env),
        .ld_phase  (ld_phase),
        .phase_in  (phase_in),
        .out_raw   (out_raw),
        .clip_val  (clip_val),
        .out_valid (out_valid),
        .busy      (busy),
        .state_obs (state_obs)
    );

    always #5 clk = ~clk;   // 10 ns period; timing is irrelevant to the check

    // vector stream + actual-output capture
    integer fvec, fact;
    reg [8*MAX_LINE:1] line;
    integer nread, nargs;
    integer ckind;          // receives %c
    reg [31:0] arg1, arg2;

    integer evals = 0;
    integer loads = 0;
    integer captured = 0;

    // Capture at negedge: out_valid/out_raw/clip_val are registered outputs
    // (stable between posedges), so this reads post-edge values without
    // racing the DUT's non-blocking updates.
    always @(negedge clk) begin
        if (out_valid) begin
            $fdisplay(fact, "%h %h", out_raw, clip_val);
            captured = captured + 1;
        end
    end

    task drain_pipeline;
        begin
            while (busy) @(posedge clk);
        end
    endtask

    integer i;
    initial begin
        if (!$value$plusargs("vectors=%s", line)) begin
            $display("FAIL tb_operator_probe: missing +vectors=<file>");
            $finish;
        end
        fvec = $fopen(line, "r");
        if (fvec == 0) begin
            $display("FAIL tb_operator_probe: cannot open vector file %0s",
                     line);
            $finish;
        end
        if (!$value$plusargs("actual=%s", line)) begin
            $display("FAIL tb_operator_probe: missing +actual=<file>");
            $finish;
        end
        fact = $fopen(line, "w");
        if (fact == 0) begin
            $display("FAIL tb_operator_probe: cannot open actual file %0s",
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
            if (nargs < 2)
                continue;                       // unparsable: comparator's job
            if (ckind == "L") begin
                @(negedge clk);
                en = 1'b0;
                drain_pipeline();
                @(negedge clk);
                ld_phase = 1'b1;
                phase_in = arg1;
                @(negedge clk);
                ld_phase = 1'b0;
                loads = loads + 1;
            end else if (ckind == "E") begin
                @(negedge clk);
                en   = 1'b1;
                freq = arg1;
                env  = arg2[15:0];
                evals = evals + 1;
            end
        end
        @(negedge clk);
        en = 1'b0;
        drain_pipeline();
        @(negedge clk);
        @(negedge clk);
        $fclose(fvec);
        $fclose(fact);

        if (captured == evals && evals > 0)
            $display("PASS tb_operator_probe (evals=%0d loads=%0d captured=%0d)",
                     evals, loads, captured);
        else
            $display("FAIL tb_operator_probe (evals=%0d loads=%0d captured=%0d)",
                     evals, loads, captured);
        $finish;
    end

    // watchdog
    initial begin
        for (i = 0; i < TIMEOUT_CYCLES; i = i + 1)
            @(posedge clk);
        $display("FAIL tb_operator_probe (timeout after %0d cycles)",
                 TIMEOUT_CYCLES);
        $finish;
    end

endmodule

`default_nettype wire
