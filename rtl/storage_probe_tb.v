// storage_probe_tb.v -- iverilog smoke bench for the H02 storage probe.
//
// Writes every bank of the state image once (96 operator banks, 16 note
// banks, the patch store -- 113 writes) with an LFSR-derived pattern, reads
// every bank back through the probe's read ports, and compares full-width
// against the pattern. This proves the probe's write-bank/read-mux structure
// works; it is NOT a fidelity test against the frozen model (N-series scope)
// and it asserts nothing about area, timing or fit.
//
// Exit convention: exactly one "PASS storage_probe_tb" line on success,
// "FAIL storage_probe_tb: ..." lines on any mismatch. The Python suite
// (tests/test_storage_probe.py) compiles and runs this bench and checks the
// summary line.
`timescale 1ns/1ps
`default_nettype none
module storage_probe_tb;
    // mirrors the RTL defaults (tests cross-check RTL params vs the N01 JSON)
    parameter NUM_OPS   = 96;
    parameter NUM_NOTES = 16;
    parameter OP_W      = 325;
    parameter NOTE_W    = 446;
    parameter PATCH_W   = 444;

    reg clk = 0;
    reg        wr_en = 0;
    reg [1:0]  wr_cls = 0;
    reg [6:0]  wr_idx = 0;
    reg [NOTE_W-1:0] wr_data = 0;
    reg [6:0]  rd_op = 0;
    reg [3:0]  rd_note = 0;
    wire [OP_W-1:0]    op_rdata;
    wire [NOTE_W-1:0]  note_rdata;
    wire [PATCH_W-1:0] patch_rdata;
    wire               state_obs;

    storage_probe dut (
        .clk(clk), .wr_en(wr_en), .wr_cls(wr_cls), .wr_idx(wr_idx),
        .wr_data(wr_data), .rd_op(rd_op), .rd_note(rd_note),
        .op_rdata(op_rdata), .note_rdata(note_rdata),
        .patch_rdata(patch_rdata), .state_obs(state_obs));

    always #5 clk = ~clk;

    // 16-bit LFSR (x^16 + x^14 + x^13 + x^11 + 1): the write-pattern source.
    // fill_pattern steps it across the whole word, so every bank gets a
    // fresh deterministic pattern with no two adjacent banks alike.
    reg [15:0] lfsr = 16'hACE1;
    integer pi;
    task fill_pattern; begin
        for (pi = 0; pi < NOTE_W; pi = pi + 1) begin
            lfsr = {lfsr[14:0], lfsr[15] ^ lfsr[13] ^ lfsr[12] ^ lfsr[10]};
            wr_data[pi] = lfsr[8] ^ lfsr[3];
        end
    end endtask

    integer errors = 0;
    integer i;
    reg [OP_W-1:0]    exp_op   [0:NUM_OPS-1];
    reg [NOTE_W-1:0]  exp_note [0:NUM_NOTES-1];
    reg [PATCH_W-1:0] exp_patch;
    reg obs_seen0, obs_seen1;

    initial begin
        // ---- write every bank once --------------------------------------
        for (i = 0; i < NUM_OPS; i = i + 1) begin
            @(negedge clk);
            fill_pattern;
            wr_en = 1; wr_cls = 2'd0; wr_idx = i[6:0];
            exp_op[i] = wr_data[OP_W-1:0];
        end
        for (i = 0; i < NUM_NOTES; i = i + 1) begin
            @(negedge clk);
            fill_pattern;
            wr_en = 1; wr_cls = 2'd1; wr_idx = i[6:0];
            exp_note[i] = wr_data;
        end
        @(negedge clk);
        fill_pattern;
        wr_en = 1; wr_cls = 2'd2; wr_idx = 7'd0;
        exp_patch = wr_data[PATCH_W-1:0];
        @(negedge clk);
        wr_en = 0; wr_cls = 2'd3;           // idle the port
        repeat (2) @(negedge clk);

        // ---- read every bank back and compare ---------------------------
        for (i = 0; i < NUM_OPS; i = i + 1) begin
            rd_op = i[6:0];
            #1;
            if (op_rdata !== exp_op[i]) begin
                errors = errors + 1;
                $display("FAIL storage_probe_tb: op bank %0d readback mismatch", i);
            end
            @(negedge clk);
        end
        for (i = 0; i < NUM_NOTES; i = i + 1) begin
            rd_note = i[3:0];
            #1;
            if (note_rdata !== exp_note[i]) begin
                errors = errors + 1;
                $display("FAIL storage_probe_tb: note bank %0d readback mismatch", i);
            end
            @(negedge clk);
        end
        #1;
        if (patch_rdata !== exp_patch) begin
            errors = errors + 1;
            $display("FAIL storage_probe_tb: patch store readback mismatch");
        end

        // ---- observability is live: the registered reduction must be
        // 0/1 (not X) and must have toggled across the read walk ----------
        obs_seen0 = 1'b0; obs_seen1 = 1'b0;
        for (i = 0; i < 32; i = i + 1) begin
            @(posedge clk); #1;
            if (state_obs === 1'b0) obs_seen0 = 1'b1;
            if (state_obs === 1'b1) obs_seen1 = 1'b1;
            rd_op = (rd_op == 7'd95) ? 7'd0 : rd_op + 7'd1;
            rd_note = (rd_note == 4'd15) ? 4'd0 : rd_note + 4'd1;
        end
        if (!(obs_seen0 && obs_seen1)) begin
            errors = errors + 1;
            $display("FAIL storage_probe_tb: state_obs stuck (0 seen=%b, 1 seen=%b)",
                     obs_seen0, obs_seen1);
        end

        if (errors == 0)
            $display("PASS storage_probe_tb: 113 banks written and read back, obs live");
        else
            $display("FAIL storage_probe_tb: %0d error(s)", errors);
        $finish;
    end
endmodule
`default_nettype wire
