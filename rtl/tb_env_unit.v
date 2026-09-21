// tb_env_unit.v -- H04 envelope-unit equivalence harness driver (issue #26).
//
// Drives rtl/env_unit.v with the directed command stream produced by
// tools/h04_compare.py from the FROZEN PYTHON MODEL (the golden:
// src/gf180_dx7/model/envelope.py) and dumps the committed slot state
// after every command for bit-exact comparison. This bench renders no
// verdict on numeric agreement -- the Python comparator is the judge; the
// bench proves the RTL ran the full stream, captured one dump per
// command, MEASURES the cycles every command (the H03 env_steps budget
// gate is the Python comparator's job), and self-checks the H03
// read-old/write-new hazard rule on the bank slot port before the stream.
//
// Command records (whitespace separated, '#' comments, blank lines):
//   P r0 r1 r2 r3 l0 l1 l2 l3 out rs   load derived param registers
//   I                                   init (level=0, down=1, advance(0))
//   K d                                 keydown(d): down-edge advance
//   U r0 r1 r2 r3 l0 l1 l2 l3 out rs    update: param refresh, then
//                                       advance(2) while down
//   S                                   getsample: one 64-sample frame step
//   A k                                 advance(k)
//   L lv tg inc st ix r d               direct slot load (state image)
// Output records (one per command, order preserved):
//   <level hex8> <target hex8> <inc hex8> <static hex8>
//   <ix hex1> <rising hex1> <down hex1>
//
// Hazard self-check (H03 section 3, read-old/write-new): load image A,
// drive an external slot write of image B, and require the combinational
// slot read to return A in the write cycle and B after the commit edge
// (back-to-back write->read returns the OLD value in the write cycle).
// Any violation prints FAIL before the stream runs.
//
// Usage (from the repository root):
//   iverilog -g2012 -o tb.vvp rtl/env_unit.v rtl/tb_env_unit.v
//   vvp tb.vvp +vectors=evidence/h04-env-rtl/vectors.txt \
//       +actual=/tmp/actual.txt
`default_nettype none

module tb_env_unit;

    localparam MAX_LINE = 512;  // must hold long +vectors=/+actual= paths
    localparam TIMEOUT_CYCLES = 40000000;

    localparam [2:0] CMD_PARAM   = 3'd0,
                     CMD_INIT    = 3'd1,
                     CMD_KEYDOWN = 3'd2,
                     CMD_UPDATE  = 3'd3,
                     CMD_STEP    = 3'd4,
                     CMD_ADV     = 3'd5,
                     CMD_LOAD    = 3'd6;

    reg        clk = 1'b0;
    reg        rst = 1'b1;
    reg        cmd_en = 1'b0;
    reg [2:0]  cmd = 3'd0;
    reg [132:0] cmd_data = 133'h0;
    reg        slot_wr_en = 1'b0;
    reg [132:0] slot_wr_data = 133'h0;

    wire        busy;
    wire        done;
    wire [132:0] slot_rd_data;
    wire [74:0]  param_rd;
    wire        state_obs;

    env_unit dut (
        .clk          (clk),
        .rst          (rst),
        .cmd_en       (cmd_en),
        .cmd          (cmd),
        .cmd_data     (cmd_data),
        .busy         (busy),
        .done         (done),
        .slot_wr_en   (slot_wr_en),
        .slot_wr_data (slot_wr_data),
        .slot_rd_data (slot_rd_data),
        .param_rd     (param_rd),
        .state_obs    (state_obs)
    );

    always #5 clk = ~clk;

    // vector stream + dump capture
    integer fvec, fact;
    reg [8*MAX_LINE:1] line;
    integer nread, nargs;
    integer ckind;
    reg [31:0] a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;

    integer cmds = 0;
    integer steps = 0;
    integer max_step_cycles = 0;
    integer max_cmd_cycles = 0;
    integer hazard_fail = 0;
    integer cyc;
    reg [132:0] img_a, img_b;

    // issue one command, wait for the done pulse, dump committed state
    task do_cmd(input [2:0] c, input [132:0] d, input is_step,
                input do_dump);
        begin
            @(negedge clk);
            while (busy) @(negedge clk);
            cmd_en = 1'b1;
            cmd = c;
            cmd_data = d;
            @(negedge clk);
            cmd_en = 1'b0;
            cyc = 1;
            while (done !== 1'b1) begin
                @(negedge clk);
                cyc = cyc + 1;
            end
            if (is_step) begin
                steps = steps + 1;
                if (cyc > max_step_cycles) max_step_cycles = cyc;
            end
            if (cyc > max_cmd_cycles) max_cmd_cycles = cyc;
            cmds = cmds + 1;
            if (do_dump)
                $fdisplay(fact, "%h %h %h %h %h %h %h",
                          slot_rd_data[132:101], slot_rd_data[100:69],
                          slot_rd_data[68:37], slot_rd_data[36:5],
                          slot_rd_data[4:2], slot_rd_data[1],
                          slot_rd_data[0]);
        end
    endtask

    integer i;
    reg [132:0] pload;
    initial begin
        if (!$value$plusargs("vectors=%s", line)) begin
            $display("FAIL tb_env_unit: missing +vectors=<file>");
            $finish;
        end
        fvec = $fopen(line, "r");
        if (fvec == 0) begin
            $display("FAIL tb_env_unit: cannot open vector file %0s", line);
            $finish;
        end
        if (!$value$plusargs("actual=%s", line)) begin
            $display("FAIL tb_env_unit: missing +actual=<file>");
            $finish;
        end
        fact = $fopen(line, "w");
        if (fact == 0) begin
            $display("FAIL tb_env_unit: cannot open actual file %0s", line);
            $finish;
        end

        // reset
        @(negedge clk);
        rst = 1;
        @(negedge clk);
        @(negedge clk);
        rst = 0;
        @(negedge clk);

        // ------------------------------------------------------------------
        // H03 hazard self-check: read-old / write-new on the bank slot.
        // Two distinct full-slot images; the write must not be visible
        // combinationally in its own cycle, and must commit at the edge.
        // ------------------------------------------------------------------
        img_a = {32'h0A0A0A0A, 32'h0B0B0B0B, 32'h0C0C0C0C, 32'h0D0D0D0D,
                 3'd2, 1'b1, 1'b0};
        img_b = {32'h1A0A0A0A, 32'h1B0B0B0B, 32'h1C0C0C0C, 32'h1D0D0D0D,
                 3'd3, 1'b0, 1'b1};
        do_cmd(CMD_LOAD, img_a, 1'b0, 1'b0);
        if (slot_rd_data !== img_a) begin
            $display("FAIL tb_env_unit (hazard: LOAD not committed for read)");
            hazard_fail = 1;
        end
        @(negedge clk);
        while (busy) @(negedge clk);
        slot_wr_en = 1'b1;
        slot_wr_data = img_b;
        // same cycle, BEFORE the commit edge: read must return OLD
        if (slot_rd_data !== img_a) begin
            $display("FAIL tb_env_unit (hazard: read-old violated)");
            hazard_fail = 1;
        end
        @(negedge clk);
        slot_wr_en = 1'b0;
        // after the commit edge: read must return NEW
        if (slot_rd_data !== img_b) begin
            $display("FAIL tb_env_unit (hazard: write-new violated)");
            hazard_fail = 1;
        end
        // back-to-back write->read: the just-committed value must be the
        // one a subsequent operation reads (restore image A for the stream)
        slot_wr_en = 1'b1;
        slot_wr_data = img_a;
        if (slot_rd_data !== img_b) begin
            $display("FAIL tb_env_unit (hazard: b2b write old-read violated)");
            hazard_fail = 1;
        end
        @(negedge clk);
        slot_wr_en = 1'b0;
        if (slot_rd_data !== img_a) begin
            $display("FAIL tb_env_unit (hazard: second write did not commit)");
            hazard_fail = 1;
        end
        if (hazard_fail) begin
            $fclose(fvec);
            $fclose(fact);
            $finish;
        end
        // restore the model-reset image (Envelope(): down_ = 1, rest 0) so
        // the stream starts from the same state as the golden driver
        do_cmd(CMD_LOAD, {128'h0, 3'b000, 1'b0, 1'b1}, 1'b0, 1'b0);

        // ------------------------------------------------------------------
        // directed command stream (the Python comparator judges agreement)
        // ------------------------------------------------------------------
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
            a1 = 32'h0; a2 = 32'h0; a3 = 32'h0; a4 = 32'h0; a5 = 32'h0;
            a6 = 32'h0; a7 = 32'h0; a8 = 32'h0; a9 = 32'h0; a10 = 32'h0;
            nargs = $sscanf(line, " %c %h %h %h %h %h %h %h %h %h %h",
                            ckind, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
            if (nargs < 1)
                continue;                       // unparsable: comparator's job
            case (ckind)
                "P": begin
                    pload = {{58{1'b0}}, a1[6:0], a2[6:0], a3[6:0], a4[6:0],
                             a5[6:0], a6[6:0], a7[6:0], a8[6:0],
                             a9[12:0], a10[5:0]};
                    do_cmd(CMD_PARAM, pload, 1'b0, 1'b1);
                end
                "I": do_cmd(CMD_INIT, 133'h0, 1'b0, 1'b1);
                "K": do_cmd(CMD_KEYDOWN, {132'h0, a1[0]}, 1'b0, 1'b1);
                "U": begin
                    pload = {{58{1'b0}}, a1[6:0], a2[6:0], a3[6:0], a4[6:0],
                             a5[6:0], a6[6:0], a7[6:0], a8[6:0],
                             a9[12:0], a10[5:0]};
                    do_cmd(CMD_UPDATE, pload, 1'b0, 1'b1);
                end
                "S": do_cmd(CMD_STEP, 133'h0, 1'b1, 1'b1);
                "A": do_cmd(CMD_ADV, {130'h0, a1[2:0]}, 1'b0, 1'b1);
                "L": begin
                    pload = {a1, a2, a3, a4, a5[2:0], a6[0], a7[0]};
                    do_cmd(CMD_LOAD, pload, 1'b0, 1'b1);
                end
                default: ;                      // unknown kind: comparator's job
            endcase
        end
        @(negedge clk);
        @(negedge clk);
        $fclose(fvec);
        $fclose(fact);

        if (cmds > 0)
            $display("PASS tb_env_unit (cmds=%0d steps=%0d max_step_cycles=%0d max_cmd_cycles=%0d hazard=ok)",
                     cmds, steps, max_step_cycles, max_cmd_cycles);
        else
            $display("FAIL tb_env_unit (no commands executed)");
        $finish;
    end

    // watchdog
    initial begin
        for (i = 0; i < TIMEOUT_CYCLES; i = i + 1)
            @(posedge clk);
        $display("FAIL tb_env_unit (timeout after %0d cycles)",
                 TIMEOUT_CYCLES);
        $finish;
    end

endmodule

`default_nettype wire
