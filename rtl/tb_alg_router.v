// tb_alg_router.v -- H05 conformance harness driver (issue #27).
//
// Drives rtl/alg_router.v with the directed run program produced by
// tools/h05_compare.py from the FROZEN PYTHON MODEL (the golden:
// src/gf180_dx7/model/algorithm.py) and streams the RTL's eval records
// and per-frame results for bit-exact comparison. This bench itself
// renders no verdict on numeric agreement -- the Python comparator is
// the judge; the bench only proves the RTL ran the full program and
// captured one record per eval / per output word.
//
// The operator kernel is the scripted-source seam (the model's sin_fn):
// each eval the DUT requests is answered with
//     y = w32(BASE[op] + t * STEP[op] + WMUL * mod)
// where t is the run-global eval counter and mod the DUT-presented
// modulation term -- the SAME function the golden side substitutes into
// AlgorithmRouter.render, so a routing defect moves the (op, phase_in,
// gain, mod) record stream and/or the frame outputs.
//
// Program records (whitespace separated, '//' comments, blanks ignored):
//   A <alg hex2> <fb hex1>   new run: reset the DUT, load cfg
//   P <idx> <phase h8> <freq h8> <level h8> <gain h4>  load op params
//   F <fb0 h8> <fb1 h8>      load the committed fb history
//   S                        render one 64-sample frame
// Stream records emitted (same order the golden generator emits):
//   A <alg hex1> <fb hex1>                     on each A record
//   E <op h1> <phase_in h8> <gain h4> <mod h8> per eval (negedge-sampled)
//   O <data h8>   x64                          note output per sample
//   Q <data h8>   x14                          6 phases, 6 gain_out, fb
//
// Usage (from the repository root so the default ROM path resolves):
//   iverilog -g2012 -o tb.vvp rtl/alg_router.v rtl/tb_alg_router.v
//   vvp tb.vvp +program=evidence/h05-routing-rtl/program.txt \
//       +actual=/tmp/actual.txt
`default_nettype none

module tb_alg_router;

    localparam MAX_LINE = 256;  // must hold long +program=/+actual= paths
    localparam TIMEOUT_CYCLES = 40000000;

    // scripted source constants (mirror tools/h05_compare.py)
    function [31:0] sbase (input [2:0] k);
        begin
            case (k)
                3'd0: sbase = 32'd110;
                3'd1: sbase = 32'd220;
                3'd2: sbase = 32'd330;
                3'd3: sbase = 32'd440;
                3'd4: sbase = 32'd550;
                default: sbase = 32'd660;
            endcase
        end
    endfunction
    function [31:0] sstep (input [2:0] k);
        begin
            case (k)
                3'd0: sstep = 32'd3;
                3'd1: sstep = 32'd10;
                3'd2: sstep = 32'd17;
                3'd3: sstep = 32'd24;
                3'd4: sstep = 32'd31;
                default: sstep = 32'd38;
            endcase
        end
    endfunction

    reg         clk = 1'b0;
    reg         rst = 1'b1;
    reg         ld_cfg = 1'b0;
    reg  [4:0]  cfg_algorithm = 5'd0;
    reg  [4:0]  cfg_feedback = 5'd0;
    reg         ld_fb = 1'b0;
    reg  [31:0] cfg_fb0 = 32'h0, cfg_fb1 = 32'h0;
    reg         ld_op = 1'b0;
    reg  [2:0]  ld_op_idx = 3'd0;
    reg  [31:0] ld_phase = 32'h0, ld_freq = 32'h0;
    reg  [27:0] ld_level = 28'h0;
    reg  [15:0] ld_gain_out = 16'h0;
    reg         start = 1'b0;
    wire        eval_req;
    wire [2:0]  eval_op;
    wire [31:0] eval_phase_in, eval_mod;
    wire [15:0] eval_gain;
    reg  [31:0] eval_y;
    wire        frame_done;
    reg  [6:0]  rd_addr = 7'd0;
    wire [31:0] rd_data;
    wire        busy;

    alg_router dut (
        .clk         (clk),
        .rst         (rst),
        .ld_cfg      (ld_cfg),
        .cfg_algorithm (cfg_algorithm),
        .cfg_feedback (cfg_feedback),
        .ld_fb       (ld_fb),
        .cfg_fb0     (cfg_fb0),
        .cfg_fb1     (cfg_fb1),
        .ld_op       (ld_op),
        .ld_op_idx   (ld_op_idx),
        .ld_phase    (ld_phase),
        .ld_freq     (ld_freq),
        .ld_level    (ld_level),
        .ld_gain_out (ld_gain_out),
        .start       (start),
        .eval_req    (eval_req),
        .eval_op     (eval_op),
        .eval_phase_in (eval_phase_in),
        .eval_gain   (eval_gain),
        .eval_mod    (eval_mod),
        .eval_y      (eval_y),
        .frame_done  (frame_done),
        .rd_addr     (rd_addr),
        .rd_data     (rd_data),
        .busy        (busy),
        .state_obs   ()
    );

    always #5 clk = ~clk;   // 10 ns period; timing is irrelevant here

    // ------------------------------------------------------------------
    // Scripted operator kernel: combinational, run-global eval counter.
    // t_q holds the count of evals accepted so far in this run (pre-edge
    // value during the request cycle, exactly the golden's len(calls)).
    // ------------------------------------------------------------------
    integer t_q = 0;
    wire signed [33:0] ysum = $signed({2'b0, sbase(eval_op)})
                              + $signed(t_q * sstep(eval_op))
                              + 3 * $signed(eval_mod);
    always @* eval_y = ysum[31:0];
    always @(posedge clk) begin
        if (rst) t_q <= 0;
        else if (eval_req) t_q <= t_q + 1;
    end

    // stream capture + program file
    integer fprog, fact;
    reg [8*MAX_LINE:1] line;
    integer nread, nargs;
    integer ckind;
    reg [31:0] a1, a2, a3, a4, a5;
    integer runs = 0;
    integer frames = 0;
    integer dumped = 0;
    integer i, r;

    // eval records are sampled at negedge: the DUT's eval_* outputs are
    // combinational from registered state (stable between posedges), and
    // t_q/eval_y are the pre-edge values the DUT consumes.
    always @(negedge clk) begin
        if (eval_req && fact != 0) begin
            $fdisplay(fact, "E %x %08x %04x %08x",
                      eval_op, eval_phase_in, eval_gain, eval_mod);
            dumped = dumped + 1;
        end
    end

    task do_reset;
        begin
            @(negedge clk);
            rst = 1;
            @(negedge clk);
            @(negedge clk);
            rst = 0;
        end
    endtask

    task render_frame;
        begin
            @(negedge clk);
            start = 1'b1;
            @(negedge clk);
            start = 1'b0;
            // frame_done is a registered one-cycle pulse
            @(posedge clk);
            while (frame_done !== 1'b1) @(posedge clk);
            // note output (64) then state readout (14), combinational
            // mux; #1 lets the read mux settle before sampling (we are
            // between posedges; no DUT state changes here)
            for (r = 0; r < 64; r = r + 1) begin
                rd_addr = r[6:0];
                #1;
                $fdisplay(fact, "O %08x", rd_data);
            end
            for (r = 64; r < 78; r = r + 1) begin
                rd_addr = r[6:0];
                #1;
                $fdisplay(fact, "Q %08x", rd_data);
            end
            rd_addr = 7'd0;
            frames = frames + 1;
        end
    endtask

    integer j;
    initial begin
        if (!$value$plusargs("program=%s", line)) begin
            $display("FAIL tb_alg_router: missing +program=<file>");
            $finish;
        end
        fprog = $fopen(line, "r");
        if (fprog == 0) begin
            $display("FAIL tb_alg_router: cannot open program file %0s",
                     line);
            $finish;
        end
        if (!$value$plusargs("actual=%s", line)) begin
            $display("FAIL tb_alg_router: missing +actual=<file>");
            $finish;
        end
        fact = $fopen(line, "w");
        if (fact == 0) begin
            $display("FAIL tb_alg_router: cannot open actual file %0s",
                     line);
            $finish;
        end

        do_reset();

        while (!$feof(fprog)) begin
            nread = $fgets(line, fprog);
            if (nread <= 0)
                continue;
            if (line[8*MAX_LINE -: 8] == "/" ||
                line[8*MAX_LINE -: 8] == "#")
                continue;                       // comment line
            if (line[8*MAX_LINE -: 8] == "\n" ||
                line[8*MAX_LINE -: 8] == "\r" ||
                line[8*MAX_LINE -: 8] == " " ||
                line[8*MAX_LINE -: 8] == "\t")
                continue;                       // blank
            nargs = $sscanf(line, " %c %h %h %h %h %h",
                            ckind, a1, a2, a3, a4, a5);
            if (nargs < 1)
                continue;
            if (ckind == "A") begin
                // new run: reset (fresh buses/history, like a fresh
                // AlgorithmRouter; t_q clears with rst), then load
                // algorithm + feedback
                do_reset();
                @(negedge clk);
                ld_cfg = 1'b1;
                cfg_algorithm = a1[4:0];
                cfg_feedback = a2[4:0];
                @(negedge clk);
                ld_cfg = 1'b0;
                runs = runs + 1;
                $fdisplay(fact, "A %0x %0x", a1[4:0], a2[4:0]);
            end else if (ckind == "P") begin
                @(negedge clk);
                ld_op = 1'b1;
                ld_op_idx = a1[2:0];
                ld_phase = a2;
                ld_freq = a3;
                ld_level = a4[27:0];
                ld_gain_out = a5[15:0];
                @(negedge clk);
                ld_op = 1'b0;
            end else if (ckind == "F") begin
                @(negedge clk);
                ld_fb = 1'b1;
                cfg_fb0 = a1;
                cfg_fb1 = a2;
                @(negedge clk);
                ld_fb = 1'b0;
            end else if (ckind == "S") begin
                render_frame();
            end
        end
        @(negedge clk);
        $fclose(fprog);
        $fclose(fact);

        if (runs > 0 && frames >= runs)
            $display("PASS tb_alg_router (runs=%0d frames=%0d evals=%0d)",
                     runs, frames, dumped);
        else
            $display("FAIL tb_alg_router (runs=%0d frames=%0d evals=%0d)",
                     runs, frames, dumped);
        $finish;
    end

    // watchdog
    initial begin
        for (j = 0; j < TIMEOUT_CYCLES; j = j + 1)
            @(posedge clk);
        $display("FAIL tb_alg_router (timeout after %0d cycles)",
                 TIMEOUT_CYCLES);
        $finish;
    end

endmodule

`default_nettype wire
