// alg_router.v -- H05 routing/feedback RTL (issue #27).
//
// The 32-algorithm Mark I router, RTL-exact to the frozen N04 integer
// model (src/gf180_dx7/model/algorithm.py::AlgorithmRouter.render,
// contract gf180-dx7-integer-v1; docs/N04-ALGORITHM-MODEL.md sections
// 2-4; H03 contract module `algorithm_router`). The pinned GPL engine
// file (EngineMkI.cpp) is the semantic specification only; no bytes are
// copied (NUM-001).
//
// The 32-row connection table ROM is $readmemh-loaded from
// reference/tables/alg_table.hex -- GENERATED from the model's
// ALGORITHMS table by tools/h05_compare.py --gen-tables and sha-pinned
// in reference/tables/alg_manifest.json (parasynth gen_tables
// discipline), so the Python model and this RTL share ONE table
// artifact. tools/h05_compare.py re-verifies the pin before every
// simulation; a mutated hex never silently becomes the shared bytes.
//
// Per-frame semantics mirrored one-for-one (EngineMkI.cpp:295-363 via
// the N04 model):
//   - ops processed strictly in engine param order 0..5, one eval per
//     clock (the H03/H01 contracted eval-slot ordering);
//   - decode inbus/outbus/add/fb-loop from the table row (with the
//     algorithm 4/6 op0 -> 0xC4 patch when feedback is on);
//   - Mark I gate: gain1 = gain_out==0 ? ENV_MAX-1 : gain_out,
//     gain2 = ENV_MAX - (level_in >> 14), render iff
//     gain1 <= 16284 || gain2 <= 16284; gain_out committed either way;
//   - has_contents bookkeeping ([1,0,0] at frame start, add suppressed
//     onto an empty destination, empty input bus falls back to pure);
//   - carrier sum: out[i] = y + out[i] when add_eff else y, in op order
//     (bus adder = the bus's own current slot, note adder = the frame
//     output buffer);
//   - feedback: 2-element history read at the fb form's dispatch into
//     stage registers, per sample scaled_fb = (y0+y) >>> (shift+1)
//     (32-bit pre-wrap, arithmetic shift), history committed at the fb
//     form's end slot -- the H03 section-3.1 staged delayed writeback
//     (the read-modify-write never shares a slot);
//   - algorithm 4/6 inline serial chains (op0(fb) -> op1 [-> op2] ->
//     output, out[i] = y overwrite, pinned full-delta downstream gains)
//     and the algorithm 32 min(fb_shift+2, 16) rule;
//   - every operator's committed phase advances exactly one frame per
//     render (freq << 6), including gated and serial-consumed ops.
//
// The operator kernel itself is the eval seam: each cycle with eval_req
// the router presents (eval_op, eval_phase_in = phase + mod, eval_gain,
// eval_mod) and consumes eval_y (int32) combinationally -- the same seam
// the model calls sin_fn(op, i, phase_plus, gain, mod). Conformance
// (tools/h05_compare.py) drives the seam with the SAME scripted source
// on both sides and compares bit-exactly; in integration the seam is the
// H01 operator probe (1 eval/clk, bit-exact there).
//
// Bus/note buffers hold the N04 facts: the two interconnect buses are
// NEVER cleared (stale contents persist across frames; add onto a bus
// nothing wrote this frame is suppressed via has_contents), while the
// note output buffer is cleared at frame start (caller-zeroed audiobuf).
//
// OBSERVABILITY IS LOAD-BEARING: every state bit feeds the readout mux
// or the registered XOR reduction state_obs, so synthesis cannot remove
// state. Negative controls (issue #27), each MUST fail the check it
// targets:
//   -DH05_MUTATE_OP_ORDER   operator STATE addressing permuted 0..5 ->
//                           5..0 (table row order and eval labels
//                           unchanged): wrong operators gated, loaded,
//                           ramped and advanced; conformance must fail
//                           on >= 28 of 32 algorithms.
//   -DH05_INVERT_FB_SIGN    feedback term sign inverted; every
//                           feedback>0 run must fail.
//   -DH05_DELAY_FB_COMMIT   fb history commit promoted one block late:
//                           frame N reads frame N-2 results; every
//                           feedback>0 run must fail.
//   -DH05_STRIP_OBSERVABILITY all outputs tied off; yosys deletes every
//                           state flop and the flop-count gate MUST FAIL.
// Mutant builds are probe-only: never synthesize them into any area
// claim.
//
// This file alone claims no place-and-route, no timing closure, no fit,
// and no original-DX7 fidelity: bit-exactness to the frozen model is the
// bar, and docs/H05-ROUTING-RTL.md records exactly what was measured.
`default_nettype none

`ifndef H05_ALG_HEX
`define H05_ALG_HEX "reference/tables/alg_table.hex"
`endif

module alg_router (
    input  wire        clk,
    input  wire        rst,          // synchronous, active high
    // program load (accepted while !busy)
    input  wire        ld_cfg,       // algorithm + feedback
    input  wire [4:0]  cfg_algorithm,// 0..31
    input  wire [4:0]  cfg_feedback, // 0..7
    input  wire        ld_fb,        // committed fb history (note bank)
    input  wire [31:0] cfg_fb0,
    input  wire [31:0] cfg_fb1,
    input  wire        ld_op,        // one operator's per-frame params
    input  wire [2:0]  ld_op_idx,
    input  wire [31:0] ld_phase,
    input  wire [31:0] ld_freq,
    input  wire [27:0] ld_level,     // level_in: gain2 = 2^14 - (lvl >> 14)
    input  wire [15:0] ld_gain_out,
    // frame run
    input  wire        start,        // render one 64-sample frame
    // eval stream (combinational same-cycle response)
    output wire        eval_req,
    output wire [2:0]  eval_op,      // engine param index of the eval
    output wire [31:0] eval_phase_in,// phase + mod (mod 2^32)
    output wire [15:0] eval_gain,    // gain, uint16 domain (mkiSin env)
    output wire [31:0] eval_mod,     // the mod term (0 / bus / fb / chain)
    input  wire [31:0] eval_y,       // op result (int32)
    // completion + readout (combinational; 0..63 note out, 64..69 phase,
    // 70..75 gain_out, 76/77 committed fb history)
    output wire        frame_done,
    input  wire [6:0]  rd_addr,
    output wire [31:0] rd_data,
    output wire        busy,
    output wire        state_obs
);

    localparam [14:0] ENV_MAX  = 15'd16384;  // EngineMkI.cpp:62 (N02)
    localparam [14:0] GAIN_SUB = 15'd16383;  // ENV_MAX - 1 (:314)
    localparam [14:0] K_THRESH = 15'd16284;  // ENV_MAX - 100 (:296, NUM-009)

    // FSM states
    localparam S_IDLE = 3'd0, S_DISPATCH = 3'd1, S_EVAL = 3'd2,
               S_SER_B = 3'd3, S_SER_C = 3'd4, S_OPEND = 3'd5,
               S_FEND = 3'd6;

    // ------------------------------------------------------------------
    // State
    // ------------------------------------------------------------------
    reg [4:0]  alg_q;
    reg [4:0]  fbshift_q;        // feedback != 0 ? 8 - fb : 16 (dx7note:197)
    reg [31:0] phase_q [0:5];
    reg [31:0] freq_q  [0:5];
    reg [27:0] level_q [0:5];
    reg [15:0] gaino_q [0:5];
    reg [31:0] bus1_q [0:63];    // fm_core.h buf_[0] -- NEVER cleared
    reg [31:0] bus2_q [0:63];    // fm_core.h buf_[1] -- NEVER cleared
    reg [31:0] nout_q [0:63];    // note output buffer, cleared per frame
    reg [31:0] fb0_q, fb1_q;     // committed 2-element history (dx7note.h:77)
    reg        hc0_q, hc1_q, hc2_q;   // has_contents (:298: 1,0,0 per frame)
    // form working state (the H03 stage registers between RMW slots)
    reg [31:0] y0_w, y1_w;       // fb working pair (history stage)
    reg [31:0] w_ph;             // consumed-op-0 local phase
    reg [31:0] s_ph1, s_ph2;     // serial ops 1/2 local phases
    reg [16:0] w_gain;           // consumed-op gain (in-form ramp)
    reg signed [9:0] w_dgain_q;  // consumed-op gain ramp (dispatch latch)
    reg [15:0] g1_w, g2_w;       // serial ops 1/2 gains
    reg signed [15:0] dg1_w, dg2_w;  // serial ops 1/2 full-delta forms
    reg [31:0] y_mid1, y_mid2;   // serial chain staging
    reg [2:0]  op_q;
    reg [6:0]  i_q;              // sample index 0..64
    reg [2:0]  state_q;
    reg        fb_form_q;        // current form carries the fb history
    reg        ser2_q, ser3_q;   // inline serial form flags (ALG 6 / 4)
    reg [1:0]  inbus_q, outbus_q;
    reg        bus_form_q;       // inbus selected AND live (else pure)
    reg        add_eff_q;
    reg [4:0]  eff_shift_q;      // fb shift for this form (incl. +2 rules)
    reg [14:0] og1_q, og2_q;     // serial ops 1/2 gain_out commits (:218/:258)
`ifdef H05_DELAY_FB_COMMIT
    reg [31:0] pend0_q, pend1_q; // mutant: one-block-delayed commit stage
    reg        pend_v_q;
`endif
    reg        obs_q;

    // ------------------------------------------------------------------
    // ROM: the generated, sha-pinned connection table (32 x 6 bytes)
    // ------------------------------------------------------------------
    reg [7:0] alg_rom [0:191];
    initial begin
        $readmemh(`H05_ALG_HEX, alg_rom);
    end

    wire [7:0] row_index = {alg_q, 2'b00} + {alg_q, 1'b0} + {5'b0, op_q};
    wire [7:0] flags_raw = alg_rom[row_index];
    wire       fb_on     = (fbshift_q != 5'd16);        // :299
    wire       alg46     = (alg_q == 5'd3) || (alg_q == 5'd5);
    // :301-305: with feedback on, ALGO 4/6 op0 is patched to 0xC4
    wire [7:0] flags = (alg46 && fb_on && (op_q == 3'd0))
                         ? 8'hc4 : flags_raw;

    // --------------------------------------------------------------
    // Operator STATE addressing. The golden build addresses state with
    // the table position; the MUTATE_OP_ORDER control permutes it
    // (wrong operators gated, loaded, ramped and advanced -- the model
    // control's "permuted params view"). Eval labels stay positional
    // (the model's sin_fn receives the table position), so a permuted
    // build shows up as wrong values for every op, not renumbered ops.
    // --------------------------------------------------------------
`ifdef H05_MUTATE_OP_ORDER
    function [2:0] pm (input [2:0] k);
        begin
            case (k)
                3'd0: pm = 3'd5;
                3'd1: pm = 3'd4;
                3'd2: pm = 3'd3;
                3'd3: pm = 3'd2;
                3'd4: pm = 3'd1;
                default: pm = 3'd0;
            endcase
        end
    endfunction
`else
    function [2:0] pm (input [2:0] k);
        begin
            pm = k;
        end
    endfunction
`endif

    // ------------------------------------------------------------------
    // Dispatch decode + gate (combinational; EngineMkI.cpp:309-322)
    // ------------------------------------------------------------------
    wire [2:0]  cur_s   = pm(op_q);
    wire [1:0]  inbus   = flags[5:4];
    wire [1:0]  outbus  = flags[1:0];
    wire        add     = flags[2];
    wire        isfb    = (flags[7:6] == 2'b11);
    wire [15:0] go_cur  = gaino_q[cur_s];
    wire [14:0] gain1   = (go_cur == 16'd0) ? GAIN_SUB : go_cur[14:0];
    wire [14:0] gain2   = ENV_MAX - {1'b0, level_q[cur_s][27:14]};
    wire        gate_pass = (gain1 <= K_THRESH) || (gain2 <= K_THRESH);
    // signed (gain2 - gain1 + 32) >>> 6 (floor); values in [-256, 256]
    wire signed [16:0] gdiff = $signed({2'b00, gain2})
                               - $signed({2'b00, gain1});
    wire signed [16:0] dgsum = gdiff + 17'sd32;
    wire signed [9:0]  dgain = dgsum >>> 6;
    wire        inbus_live = (inbus != 2'd0) &&
                             ((inbus == 2'd1) ? hc1_q : hc2_q);
    wire        outbus_live = (outbus == 2'd0) ? hc0_q
                            : ((outbus == 2'd1) ? hc1_q : hc2_q);
    // fb dispatch (:326) + special forms (:327-348)
    wire        fb_dispatch = gate_pass && isfb && fb_on &&
                              ((inbus == 2'd0) || !inbus_live);
    wire        ser3_sel = fb_dispatch && (alg_q == 5'd3);
    wire        ser2_sel = fb_dispatch && (alg_q == 5'd5) && !ser3_sel;
    wire        plain_fb = fb_dispatch && !ser3_sel && !ser2_sel;
    wire [4:0]  plus2    = (fbshift_q >= 5'd14) ? 5'd16
                                                : fbshift_q + 5'd2;
    wire [4:0]  fb_shift_sel = (ser3_sel || ser2_sel || alg_q == 5'd31)
                               ? plus2 : fbshift_q;   // :330/:337/:343
    wire [14:0] og1 = ENV_MAX - {1'b0, level_q[pm(3'd1)][27:14]};  // :218
    wire [14:0] og2 = ENV_MAX - {1'b0, level_q[pm(3'd2)][27:14]};  // :258
    wire [15:0] sg1 = (og1 == 15'd0) ? {1'b0, GAIN_SUB} : {1'b0, og1};
    wire [15:0] sg2 = (og2 == 15'd0) ? {1'b0, GAIN_SUB} : {1'b0, og2};

    // ------------------------------------------------------------------
    // Eval presentation (combinational; the sin_fn seam arguments)
    // ------------------------------------------------------------------
    wire        in_eval = (state_q == S_EVAL) || (state_q == S_SER_B) ||
                          (state_q == S_SER_C);
    wire [31:0] fb_sum32 = y0_w + y1_w;               // i32 wrap first
    wire signed [31:0] scaled_fb = $signed(fb_sum32)
                                   >>> (eff_shift_q + 5'd1);  // :196/:227/:271
`ifdef H05_INVERT_FB_SIGN
    // NEGATIVE CONTROL: feedback term sign inverted. Every feedback>0
    // run must fail conformance; feedback-0 runs carry no term.
    wire signed [31:0] fb_mod = -scaled_fb;
`else
    wire signed [31:0] fb_mod = scaled_fb;
`endif
    wire [31:0] bus_mod = (inbus_q == 2'd1) ? bus1_q[i_q[5:0]]
                                            : bus2_q[i_q[5:0]];
    // eval 0 of a serial form runs in S_EVAL; labels are positional.
    wire [2:0]  eval_op_r = (state_q == S_SER_B) ? 3'd1
                          : (state_q == S_SER_C) ? 3'd2
                          : (ser2_q || ser3_q) ? 3'd0
                          : op_q;
    wire [31:0] eval_mod_r = (state_q == S_SER_B) ? y_mid1
                           : (state_q == S_SER_C) ? y_mid2
                           : fb_form_q ? fb_mod
                           : bus_form_q ? bus_mod
                           : 32'h0;   // pure: no input bus or empty bus
    wire [16:0] gain_adv  = w_gain + {{7{w_dgain_q[9]}}, w_dgain_q};
    wire [15:0] gain_adv1 = g1_w + dg1_w;   // :236 (full-delta form)
    wire [15:0] gain_adv2 = g2_w + dg2_w;   // :285
    wire [15:0] eval_gain_r = (state_q == S_SER_B) ? gain_adv1
                            : (state_q == S_SER_C) ? gain_adv2
                            : gain_adv[15:0];
    wire [31:0] eval_ph_r = (state_q == S_SER_B) ? s_ph1 + eval_mod_r
                          : (state_q == S_SER_C) ? s_ph2 + eval_mod_r
                          : w_ph + eval_mod_r;


    // ------------------------------------------------------------------
    // Observability: combinational reduction of the wide arrays, folded
    // with every other state bit into the registered state_obs.
    // ------------------------------------------------------------------
    integer b;
    reg [31:0] bus1_obs, bus2_obs, nout_obs;
    always @* begin
        bus1_obs = 32'h0;
        bus2_obs = 32'h0;
        nout_obs = 32'h0;
        for (b = 0; b < 64; b = b + 1) begin
            bus1_obs = bus1_obs ^ bus1_q[b];
            bus2_obs = bus2_obs ^ bus2_q[b];
            nout_obs = nout_obs ^ nout_q[b];
        end
    end

    // ------------------------------------------------------------------
    // Readout (combinational mux; 0..63 note out, 64..69 phase,
    // 70..75 gain_out, 76/77 committed fb history)
    // ------------------------------------------------------------------
    reg [31:0] rd_data_r;
    // 70..75 -> gain_out 0..5: rd_addr[2:0] is 6,7,0,1,2,3; +2 (mod 8)
    // rotates to 0..5. (64..69 phases: rd_addr[2:0] is already 0..5.)
    wire [2:0] rd_gain_idx = rd_addr[2:0] + 3'd2;
    always @* begin
        if (rd_addr < 7'd64)
            rd_data_r = nout_q[rd_addr[5:0]];
        else if (rd_addr < 7'd70)
            rd_data_r = phase_q[rd_addr[2:0]];
        else if (rd_addr < 7'd76)
            rd_data_r = {16'h0, gaino_q[rd_gain_idx]};
        else if (rd_addr == 7'd76)
            rd_data_r = fb0_q;
        else if (rd_addr == 7'd77)
            rd_data_r = fb1_q;
        else
            rd_data_r = 32'h0;
    end

`ifdef H05_STRIP_OBSERVABILITY
    // NEGATIVE CONTROL: no output observable, no reduction -- synthesis
    // must (and does) delete every state flop; the flop-count gate then
    // FAILS. This build exists only to prove the gate has resolution.
    assign eval_req      = 1'b0;
    assign eval_op       = 3'b0;
    assign eval_phase_in = 32'h0;
    assign eval_gain     = 16'h0;
    assign eval_mod      = 32'h0;
    assign rd_data       = 32'h0;
    assign frame_done    = 1'b0;
    assign busy          = 1'b0;
    assign state_obs     = 1'b0;
`else
    assign eval_req      = in_eval;
    assign eval_op       = eval_op_r;
    assign eval_phase_in = eval_ph_r;
    assign eval_gain     = eval_gain_r;
    assign eval_mod      = eval_mod_r;
    assign rd_data       = rd_data_r;
    assign frame_done    = (state_q == S_FEND);
    assign busy          = (state_q != S_IDLE);
    assign state_obs     = obs_q;
`endif

    // ------------------------------------------------------------------
    // Sequential logic
    // ------------------------------------------------------------------
    integer k;
    always @(posedge clk) begin
        if (rst) begin
            alg_q <= 5'd0;
            fbshift_q <= 5'd16;
            for (k = 0; k < 6; k = k + 1) begin
                phase_q[k]  <= 32'h0;
                freq_q[k]   <= 32'h0;
                level_q[k]  <= 28'h0;
                gaino_q[k]  <= 16'h0;
            end
            for (k = 0; k < 64; k = k + 1) begin
                bus1_q[k] <= 32'h0;
                bus2_q[k] <= 32'h0;
                nout_q[k] <= 32'h0;
            end
            fb0_q <= 32'h0;
            fb1_q <= 32'h0;
            hc0_q <= 1'b0;
            hc1_q <= 1'b0;
            hc2_q <= 1'b0;
            y0_w <= 32'h0;  y1_w <= 32'h0;
            w_ph <= 32'h0;  s_ph1 <= 32'h0;  s_ph2 <= 32'h0;
            w_gain <= 17'h0;  w_dgain_q <= 10'sd0;
            g1_w <= 16'h0;  g2_w <= 16'h0;
            dg1_w <= 16'sd0;  dg2_w <= 16'sd0;
            y_mid1 <= 32'h0;  y_mid2 <= 32'h0;
            op_q <= 3'd0;  i_q <= 7'd0;  state_q <= S_IDLE;
            fb_form_q <= 1'b0;  ser2_q <= 1'b0;  ser3_q <= 1'b0;
            inbus_q <= 2'd0;  outbus_q <= 2'd0;  add_eff_q <= 1'b0;
            bus_form_q <= 1'b0;
            eff_shift_q <= 5'd16;  og1_q <= 15'h0;  og2_q <= 15'h0;
`ifdef H05_DELAY_FB_COMMIT
            pend0_q <= 32'h0;  pend1_q <= 32'h0;  pend_v_q <= 1'b0;
`endif
            obs_q <= 1'b0;
        end else begin
            // ----------------------------------------------------------
            // program load + frame start (IDLE only)
            // ----------------------------------------------------------
            if (state_q == S_IDLE) begin
                if (ld_cfg) begin
                    alg_q <= cfg_algorithm;
                    fbshift_q <= (cfg_feedback == 5'd0)
                                 ? 5'd16 : 5'd8 - cfg_feedback;
                end
                if (ld_fb) begin
                    fb0_q <= cfg_fb0;   // committed history store
                    fb1_q <= cfg_fb1;
                end
                if (ld_op) begin
                    phase_q[pm(ld_op_idx)] <= ld_phase;
                    freq_q[pm(ld_op_idx)]  <= ld_freq;
                    level_q[pm(ld_op_idx)] <= ld_level;
                    gaino_q[pm(ld_op_idx)] <= ld_gain_out;
                end
                if (start) begin
                    // frame init: :298 hc = [1,0,0]; audiobuf zeroed per
                    // frame per note (PluginProcessor.cpp:247-249)
                    hc0_q <= 1'b1;
                    hc1_q <= 1'b0;
                    hc2_q <= 1'b0;
                    op_q  <= 3'd0;
                    i_q   <= 7'd0;
                    for (k = 0; k < 64; k = k + 1)
                        nout_q[k] <= 32'h0;
                    state_q <= S_DISPATCH;
                end
            end else if (state_q == S_DISPATCH) begin
                // gain_out commit is unconditional (:316)
                gaino_q[cur_s] <= {1'b0, gain2};
                i_q <= 7'd0;
                if (!gate_pass) begin
                    // :358-360: a gated op marks an add-less destination
                    // empty; an add destination keeps its contents (the
                    // ALG 4/6 patched 0xC4 op0 has add set).
                    if (!add) begin
                        if (outbus == 2'd0)        hc0_q <= 1'b0;
                        else if (outbus == 2'd1)   hc1_q <= 1'b0;
                        else                       hc2_q <= 1'b0;
                    end
                    fb_form_q <= 1'b0; ser2_q <= 1'b0; ser3_q <= 1'b0;
                    state_q <= S_OPEND;
                end else begin
                    // :320-322: add onto an empty destination is forced
                    // off (buses hold stale data, never cleared)
                    add_eff_q <= add && outbus_live;
                    outbus_q  <= outbus;
                    inbus_q   <= inbus;
                    // :324 the input bus is read only when live; an empty
                    // bus falls back to the pure form (mod = 0)
                    bus_form_q <= (inbus != 2'd0) && inbus_live;
                    w_gain    <= {1'b0, gain1};   // :277/:259 gain = gain1
                    w_dgain_q <= dgain;           // latch the ramp: the
                    // gain_out commit below changes gain1/gain2 decode
                    // for later cycles, so the in-form ramp must be a
                    // dispatch-time register (the model snapshots the
                    // same values into the frame form)
                    w_ph      <= phase_q[cur_s];
                    // :357 (rendered ops mark their destination; later
                    // ops' dispatches see it -- same order as the model)
                    if (outbus == 2'd0)        hc0_q <= 1'b1;
                    else if (outbus == 2'd1)   hc1_q <= 1'b1;
                    else                       hc2_q <= 1'b1;
                    if (fb_dispatch) begin
                        fb_form_q   <= 1'b1;
                        eff_shift_q <= fb_shift_sel;
                        // history READ rides the dispatch slot into the
                        // stage registers (H03 section 3.1); the commit
                        // takes the W port at this form's end slot.
                        y0_w <= fb0_q;
                        y1_w <= fb1_q;
                        ser3_q <= ser3_sel;
                        ser2_q <= ser2_sel;
                        og1_q <= og1;
                        if (ser3_sel) begin
                            s_ph1 <= phase_q[pm(3'd1)];
                            g1_w  <= sg1[15:0];
                            // :224/:266 pinned full-delta form (no >> 6)
                            dg1_w <= $signed(og1) - $signed(sg1[14:0]);
                            s_ph2 <= phase_q[pm(3'd2)];
                            g2_w  <= sg2[15:0];
                            dg2_w <= $signed(og2) - $signed(sg2[14:0]);
                            og2_q <= og2;
                        end else if (ser2_sel) begin
                            s_ph1 <= phase_q[pm(3'd1)];
                            g1_w  <= sg1[15:0];
                            dg1_w <= $signed(og1) - $signed(sg1[14:0]);
                        end
                        state_q <= S_EVAL;
                    end else begin
                        fb_form_q <= 1'b0; ser2_q <= 1'b0; ser3_q <= 1'b0;
                        eff_shift_q <= 5'd16;
                        state_q <= S_EVAL;
                    end
                end
            end else if (state_q == S_EVAL) begin
                if (fb_form_q) begin
                    y0_w <= y1_w;      // y0 = y (:198/:231/:275)
                    if (!ser2_q && !ser3_q)
                        y1_w <= eval_y;    // plain fb: y = eval (:200)
                end
                w_gain <= gain_adv;
                w_ph   <= w_ph + freq_q[cur_s];
                if (ser2_q || ser3_q) begin
                    // serial eval0 feeds the in-sample chain (:232/:276);
                    // only the chain's final op writes the destination
                    y_mid1 <= eval_y;
                    state_q <= S_SER_B;
                end else begin
                    // out[i] = y + adder[i]; adder = add_eff ? dest : 0
                    if (outbus_q == 2'd0)
                        nout_q[i_q[5:0]] <= add_eff_q
                                            ? eval_y + nout_q[i_q[5:0]]
                                            : eval_y;
                    else if (outbus_q == 2'd1)
                        bus1_q[i_q[5:0]] <= add_eff_q
                                            ? eval_y + bus1_q[i_q[5:0]]
                                            : eval_y;
                    else
                        bus2_q[i_q[5:0]] <= add_eff_q
                                            ? eval_y + bus2_q[i_q[5:0]]
                                            : eval_y;
                    if (i_q == 7'd63) state_q <= S_OPEND;
                    else              i_q <= i_q + 7'd1;
                end
            end else if (state_q == S_SER_B) begin
                g1_w  <= gain_adv1;
                s_ph1 <= s_ph1 + freq_q[pm(3'd1)];
                if (ser3_q) begin
                    y_mid2 <= eval_y;
                    state_q <= S_SER_C;
                end else begin
                    // ALGO 6: op1 writes the note output, overwrite :240
                    nout_q[i_q[5:0]] <= eval_y;
                    y1_w <= eval_y;                    // :237-243
                    if (i_q == 7'd63) state_q <= S_OPEND;
                    else begin
                        i_q <= i_q + 7'd1;
                        state_q <= S_EVAL;   // next sample's eval0
                    end
                end
            end else if (state_q == S_SER_C) begin
                // ALGO 4: op2 writes the note output, overwrite :289
                g2_w  <= gain_adv2;
                s_ph2 <= s_ph2 + freq_q[pm(3'd2)];
                nout_q[i_q[5:0]] <= eval_y;
                y1_w <= eval_y;                        // :286-292
                if (i_q == 7'd63) state_q <= S_OPEND;
                else begin
                    i_q <= i_q + 7'd1;
                    state_q <= S_EVAL;   // next sample's eval0
                end
            end else if (state_q == S_OPEND) begin
                // :361 one-frame phase skip for the dispatching op --
                // always, including gated and serial-consumed ops
                phase_q[cur_s] <= phase_q[cur_s]
                                  + (freq_q[cur_s] <<< 6);
                if (fb_form_q) begin
                    // delayed-commit RMW rule (H03 section 3.1): the fb
                    // history stage registers take the W port here, at
                    // the form's end slot (:203-204/:242-243/:291-292) --
                    // for every fb form, including the ALG 4/6 serials
`ifdef H05_DELAY_FB_COMMIT
                    // NEGATIVE CONTROL: the commit is promoted one block
                    // late; frame N reads frame N-2 results. Must fail
                    // every feedback>0 run.
                    if (pend_v_q) begin
                        fb0_q <= pend0_q;
                        fb1_q <= pend1_q;
                    end
                    pend0_q <= y0_w;
                    pend1_q <= y1_w;
                    pend_v_q <= 1'b1;
`else
                    fb0_q <= y0_w;
                    fb1_q <= y1_w;
`endif
                end
                if (ser3_q) begin
                    // :331-332 explicit skips + :258-259 gain_out commits
                    phase_q[pm(3'd1)] <= phase_q[pm(3'd1)]
                                         + (freq_q[pm(3'd1)] <<< 6);
                    phase_q[pm(3'd2)] <= phase_q[pm(3'd2)]
                                         + (freq_q[pm(3'd2)] <<< 6);
                    gaino_q[pm(3'd1)] <= {1'b0, og1_q};
                    gaino_q[pm(3'd2)] <= {1'b0, og2_q};
                end else if (ser2_q) begin
                    // :338 explicit skip + :218 gain_out commit
                    phase_q[pm(3'd1)] <= phase_q[pm(3'd1)]
                                         + (freq_q[pm(3'd1)] <<< 6);
                    gaino_q[pm(3'd1)] <= {1'b0, og1_q};
                end
                fb_form_q <= 1'b0; ser2_q <= 1'b0; ser3_q <= 1'b0;
                op_q <= op_q + (ser3_q ? 3'd3 : ser2_q ? 3'd2 : 3'd1);
                state_q <= (op_q + (ser3_q ? 3'd3 : ser2_q ? 3'd2 : 3'd1)
                            >= 3'd6) ? S_FEND : S_DISPATCH;
            end else begin // S_FEND
                state_q <= S_IDLE;
            end

            // observability reduction: every state bit feeds state_obs
            obs_q <= (^alg_q) ^ (^fbshift_q)
                     ^ (^phase_q[0]) ^ (^phase_q[1]) ^ (^phase_q[2])
                     ^ (^phase_q[3]) ^ (^phase_q[4]) ^ (^phase_q[5])
                     ^ (^freq_q[0]) ^ (^freq_q[1]) ^ (^freq_q[2])
                     ^ (^freq_q[3]) ^ (^freq_q[4]) ^ (^freq_q[5])
                     ^ (^level_q[0]) ^ (^level_q[1]) ^ (^level_q[2])
                     ^ (^level_q[3]) ^ (^level_q[4]) ^ (^level_q[5])
                     ^ (^gaino_q[0]) ^ (^gaino_q[1]) ^ (^gaino_q[2])
                     ^ (^gaino_q[3]) ^ (^gaino_q[4]) ^ (^gaino_q[5])
                     ^ (^bus1_obs) ^ (^bus2_obs) ^ (^nout_obs)
                     ^ fb0_q[0] ^ fb1_q[0] ^ (^{hc0_q, hc1_q, hc2_q})
                     ^ (^y0_w) ^ (^y1_w) ^ (^w_ph) ^ (^s_ph1) ^ (^s_ph2)
                     ^ (^w_gain) ^ (^w_dgain_q) ^ (^g1_w) ^ (^g2_w)
                     ^ (^y_mid1) ^ (^y_mid2)
                     ^ (^op_q) ^ (^i_q) ^ (^state_q)
                     ^ fb_form_q ^ ser2_q ^ ser3_q
                     ^ (^{inbus_q, outbus_q}) ^ add_eff_q ^ bus_form_q
                     ^ (^eff_shift_q) ^ (^og1_q) ^ (^og2_q);
        end
    end

endmodule

`default_nettype wire
