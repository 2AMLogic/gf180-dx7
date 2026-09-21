// env_unit.v -- H04 envelope/state unit (issue #26).
//
// The N03 integer operator envelope (docs/N03-ENVELOPE-MODEL.md) as
// synthesizable RTL, exact to the frozen model
// src/gf180_dx7/model/envelope.py (contract gf180-dx7-integer-v1). The
// pinned msfa env.cc/env.h are the semantic specification only (Apache-2.0
// semantic facts, re-implemented from scratch, NUM-001); no bytes are
// copied. State members are the N01 section-6 per-operator runtime fields:
// level_, targetlevel_, inc_, staticcount_ (32 b each), ix_ (3 b),
// rising_, down_ (1 b each). outlevel_/rate_scaling_ are per-note DERIVED
// inputs (dx7note.cc:163-179: key/velocity scaling is applied once by the
// note-on path, H07 scope) -- they enter here as committed param
// registers, exactly the W_OP_ENV_OUTLEVEL / W_OP_ENV_RATE_SC bank fields
// of H02's 325-bit operator slot (docs/H02-STORAGE-REPORT.md section 1).
//
// Semantics mirrored exactly (N03 section 1.3; env.cc:63-151):
//   advance(newix): ix_ = newix; for newix < 4:
//     target:  actual = (scaleoutlevel(levels[ix]) >> 1 << 6) + outlevel_
//              - 4256, clamped < 16 -> 16; targetlevel_ = actual << 16
//     rising:  rising_ = targetlevel_ > level_
//     qrate:   ((rates[ix] * 41) >> 6) + rate_scaling_, clamp <= 63
//     statics: if targetlevel_ == level_ or (ix == 0 and levels[ix] == 0):
//              staticrate = min(rates[ix] + rate_scaling_, 99);
//              staticcount_ = staticrate < 77 ? statics[staticrate]
//                             : 20 * (99 - staticrate);
//              if staticrate < 77 and ix == 0 and levels[ix] == 0:
//              staticcount_ = staticcount_ / 20   (floor);
//              staticcount_ = (staticcount_ * sr_multiplier) >> 24
//              else staticcount_ = 0
//     inc_:    inc_ = ((4 + (qrate & 3)) << (2 + LG_N + (qrate >> 2)))
//              * sr_multiplier >> 24   (both multiplies via int64)
//   getsample() -- one step per 64-sample frame (N03 cadence, env.h:39-40):
//     if staticcount_: staticcount_ -= 64; if <= 0: = 0, advance(ix_ + 1)
//     if ix_ < 3 or (ix_ < 4 and !down_):
//       hold while staticcount_; rising: jump floor 1716 << 16, then
//       level_ += (((17 << 24) - level_) >> 24) * inc_, clamp >= target
//       -> advance(ix_ + 1); falling: level_ -= inc_, clamp <= target
//       -> advance(ix_ + 1)
//     (a step can launch at most two advances: the static-expiry advance,
//     then the movement's clamp advance -- both are sequenced here)
//   keydown(d): if down_ != d: down_ = d, advance(d ? 0 : 3) --
//     attack/release (re)start FROM THE CURRENT LEVEL (retrigger path,
//     env.cc:102-107; no re-init -- the polyphonic re-init lives in the
//     note-on application, H07 scope).
//   init: level_ = 0, down_ = 1, advance(0).  update: param refresh; while
//   down_, the sustain target is recomputed from levels_[2] WITHOUT
//   outlevel and advance(2) then overwrites it with the outlevel target
//   (pinned quirk env.cc:166-167 -- the intermediate write is dead state,
//   final committed state identical, so this unit performs advance(2)
//   directly; the quirk is recorded here, not silent).
//
// sr_multiplier is the DERIVED constant (NUM-005 rev 1.1; env.cc:47-49):
// uint32 truncation of (44100.0 / 48000.0) * (1 << 24) = 15414067.2 ->
// 15,414,067. It is re-derived from the formula by tests/test_h04.py and
// cross-checked against spec/numeric-profile-v1.json
// envelope.sr_multiplier_48k -- never trusted as a bare literal.
// The statics //20 floor division is the constant-multiply identity
// floor(x/20) == (x * 838861) >> 24, exact for every reachable x
// (0..1,764,000, the statics table max); the identity is verified
// exhaustively by tests/test_h04.py.
//
// SCHEDULE (H03 contract, spec/core-contract-v1.json budget row
// env_steps): one step per operator per frame, worst case incl. a segment
// advance, budgeted at the derived_estimate 24-clock slot. This unit
// micro-sequences a step in bounded cycles -- capture, movement, and up
// to two 5-clock advance launches, worst case 13 clocks -- measured by
// the harness over the directed set incl. worst-rate updates;
// tools/h04_compare.py gates the measured bound against the contracted
// 24. Every advance (scaleoutlevel target, qrate, statics hold, the
// 15,414,067 constant multiply) runs inside the slot.
//
// HAZARD RULES (H03 section 3, read-old/write-new): the envelope slot is
// one H02-style bank slice. slot_rd_data is the COMMITTED state only
// (read-old); slot_wr_en commits at the clock edge (write-new); a
// back-to-back write -> read of the same slot returns the OLD value in
// the write cycle and the new value one cycle later -- asserted by the
// bench self-check. The step is a read-modify-write: it captures state
// once at entry and commits once at the end (the fb_buf delayed-commit
// lesson applied to any RMW: no same-cycle read-modify-write on one
// write port); external slot writes while busy are ignored (the step
// owns the slot; control-tail writes never share an eval slot, H03
// section 3 resolution 3).
//
// OBSERVABILITY IS LOAD-BEARING (H01/H02 pattern): the whole committed
// slot drives slot_rd_data, the param registers drive param_rd, and every
// state bit feeds the registered XOR fold state_obs, so synthesis cannot
// delete state. Negative controls (issue #26):
//   -DH04_MUTATE_STEP_SKEW  the movement commit is deferred one frame --
//                           the conformance harness MUST fail and localize.
//   -DH04_MUTATE_SRM        sr_multiplier - 1 -- MUST fail (N03 section 6:
//                           reachable inc_ coefficients flip).
//   -DH04_STRIP_OBSERVABILITY all outputs tied off; yosys deletes every
//                           state flop and the flop-count gate MUST FAIL.
//
// This file alone claims no P&R, no timing closure, no fit, and no
// original-DX7 fidelity: bit-exactness to the frozen model is the bar, and
// docs/H04-ENV-RTL.md records exactly what was measured.
`default_nettype none

module env_unit #(
    // derived at 48 kHz: floor((44100.0/48000.0) * 2**24) = 15414067
    // (NUM-005 rev 1.1; re-derived by test, never a bare literal)
    parameter SR_MULTIPLIER = 32'd15414067
)(
    input  wire         clk,
    input  wire         rst,          // synchronous, active high
    // command interface: one command accepted when !busy (control-tail
    // window handshake; tail_start/tail_done integration is H07 scope)
    input  wire         cmd_en,
    input  wire [2:0]   cmd,
    input  wire [132:0] cmd_data,     // command payload (packed below)
    output wire         busy,
    output wire         done,         // 1-clk pulse; command committed
    // H02-style bank slot ports (this unit owns one envelope slice):
    // external write = control-tail/event commit path (idle cycles only);
    // read = combinational view of the COMMITTED slot (read-old)
    input  wire         slot_wr_en,
    input  wire [132:0] slot_wr_data,
    output wire [132:0] slot_rd_data,
    output wire [74:0]  param_rd,
    // registered XOR reduction of every state bit (observability)
    output wire         state_obs
);

    // ------------------------------------------------------------------
    // Commands / payload packing
    //   CMD_PARAM  cmd_data[74:0] = {r0,r1,r2,r3, l0,l1,l2,l3 (7b each),
    //                                 outlevel[12:0], rate_scaling[5:0]}
    //   CMD_LOAD   cmd_data[132:0] = {level, target, inc, static (32b),
    //                                 ix[2:0], rising, down}
    //   CMD_KEYDOWN cmd_data[0] = d
    //   CMD_ADV    cmd_data[2:0] = newix
    //   CMD_INIT / CMD_UPDATE / CMD_STEP: no payload
    // ------------------------------------------------------------------
    localparam [2:0] CMD_PARAM   = 3'd0,
                     CMD_INIT    = 3'd1,
                     CMD_KEYDOWN = 3'd2,
                     CMD_UPDATE  = 3'd3,
                     CMD_STEP    = 3'd4,
                     CMD_ADV     = 3'd5,
                     CMD_LOAD    = 3'd6;

    // ------------------------------------------------------------------
    // Frozen envelope constants (env.cc; N03 section 1)
    // ------------------------------------------------------------------
    localparam [31:0] JUMP_FLOOR    = 32'h06B40000;  // 1716 << 16
    localparam [31:0] RISE_BASE     = 32'h11000000;  // 17 << 24
    localparam [15:0] TARGET_OFFSET = 16'd4256;
    localparam [15:0] LEVEL_FLOOR   = 16'd16;
    localparam [6:0]  STEP_N        = 7'd64;         // N = 1 << LG_N

`ifdef H04_MUTATE_SRM
    // NEGATIVE CONTROL: sr_multiplier - 1. Reachable inc_/static
    // coefficients flip (N03 section 6: rate 1/rs 1: 1175 -> 1176; rate
    // 74/rs 0: 3371827 -> 3371826); the conformance harness must fail on
    // this build. Never synthesize this build into any area claim.
    localparam [31:0] SRM_EFF = SR_MULTIPLIER - 32'd1;
`else
    localparam [31:0] SRM_EFF = SR_MULTIPLIER;
`endif
    // floor(x/20) == (x * 838861) >> 24 for 0 <= x <= 1764000
    localparam [23:0] DIV20_M = 24'd838861;

    // ------------------------------------------------------------------
    // Committed state (the envelope slice of one H02 operator bank slot)
    // ------------------------------------------------------------------
    reg [31:0] level_q;      // level_   (Q24 doublings, stored << 16)
    reg [31:0] target_q;     // targetlevel_
    reg [31:0] inc_q;        // inc_
    reg [31:0] static_q;     // staticcount_ (ACCURATE_ENVELOPE)
    reg [2:0]  ix_q;         // ix_
    reg        rising_q;     // rising_
    reg        down_q;       // down_
    // per-note DERIVED param registers (committed by CMD_PARAM/CMD_UPDATE)
    reg [6:0]  rates_q [0:3];
    reg [6:0]  levels_q [0:3];
    reg [12:0] outlevel_q;
    reg [5:0]  rs_q;
    // observability reduction register
    reg        obs_q;

    // ------------------------------------------------------------------
    // Working registers
    // ------------------------------------------------------------------
    reg [3:0]  state_q;
    reg [3:0]  adv_ix;        // newix (0..5; >= 4 parks: ix-only commit)
    reg        adv_ret_mov;   // return-to-movement (static-expiry chain)
    reg [27:0] tgt_new;
    reg [6:0]  newlv_r;
    reg        rising_new;
    reg [5:0]  qr_r;
    reg        static_cond_r;
    reg [20:0] raw_static_r;
    reg [31:0] stat_new_r;
    reg [25:0] inc_pre_r;
    reg [31:0] st_after_r;    // decremented staticcount_
    reg        st_adv_r;      // static-expiry advance pending
    reg        kd_fire;       // down-edge detected at acceptance (pre-edge
                              // down_q; the flip commits at that edge)
    reg        done_q;

`ifdef H04_MUTATE_STEP_SKEW
    // NEGATIVE CONTROL: one-frame update skew. The movement commit is
    // parked here and applied at the NEXT step's capture, so the visible
    // level trajectory lags the model by exactly one frame.
    reg        skew_v;
    reg [31:0] skew_level;
`endif

    // FSM states
    localparam [3:0] S_IDLE     = 4'd0,
                     S_PARAM    = 4'd1,
                     S_LOAD     = 4'd2,
                     S_INIT     = 4'd3,
                     S_KD       = 4'd4,
                     S_UPD      = 4'd5,
                     S_STEP_CAP = 4'd6,
                     S_STEP_MOV = 4'd7,
                     S_ADV_TGT  = 4'd8,
                     S_ADV_QR   = 4'd9,
                     S_ADV_STA  = 4'd10,
                     S_ADV_MUL1 = 4'd11,
                     S_ADV_MUL2 = 4'd12;


    // ------------------------------------------------------------------
    // Advance combinational path (shared by INIT/KEYDOWN/UPDATE/ADV/STEP)
    // ------------------------------------------------------------------
    wire [2:0]  aix     = adv_ix[2:0];
    wire        aix_lt4 = (aix < 3'd4);
    wire [6:0]  adv_rate = aix_lt4 ? rates_q[aix[1:0]] : 7'h00;
    wire [6:0]  adv_lv   = aix_lt4 ? levels_q[aix[1:0]] : 7'h00;

    // scaleoutlevel (env.cc:109-111): >= 20 ? 28 + ol : levellut[ol]
    reg [6:0] lut_v;
    always @(*) begin
        if (adv_lv >= 7'd20)
            lut_v = adv_lv + 7'd28;
        else
            case (adv_lv)
7'd00: lut_v = 7'd00;
                7'd01: lut_v = 7'd05;
                7'd02: lut_v = 7'd09;
                7'd03: lut_v = 7'd13;
                7'd04: lut_v = 7'd17;
                7'd05: lut_v = 7'd20;
                7'd06: lut_v = 7'd23;
                7'd07: lut_v = 7'd25;
                7'd08: lut_v = 7'd27;
                7'd09: lut_v = 7'd29;
                7'd10: lut_v = 7'd31;
                7'd11: lut_v = 7'd33;
                7'd12: lut_v = 7'd35;
                7'd13: lut_v = 7'd37;
                7'd14: lut_v = 7'd39;
                7'd15: lut_v = 7'd41;
                7'd16: lut_v = 7'd42;
                7'd17: lut_v = 7'd43;
                7'd18: lut_v = 7'd45;
                7'd19: lut_v = 7'd46;
                default: lut_v = 7'd00;
            endcase
    end

    // target: ((scaleoutlevel >> 1) << 6) + outlevel_ - 4256, clamp <16->16
    wire [12:0] scale_part  = {lut_v[6:1], 6'b000000};  // (lut>>1)<<6
    wire signed [15:0] act16 = $signed({3'b000, scale_part})
                             + $signed({3'b000, outlevel_q})
                             - $signed(TARGET_OFFSET);
    wire signed [15:0] act_cl = (act16 < $signed(LEVEL_FLOOR))
                              ? $signed(LEVEL_FLOOR) : act16;
    wire [27:0] tgt_w = {act_cl[12:0], 16'b0000000000000000};

    // rising: targetlevel_ > level_ (both nonnegative in every reachable
    // state; unsigned compare == the model's integer comparison)
    wire rising_w = ({4'b0000, tgt_new} > {4'b0000, level_q});

    // qrate: ((rate * 41) >> 6) + rate_scaling_, clamp <= 63 (no lower
    // clamp is applied, env.cc:125-127)
    wire [11:0] qprod = adv_rate * 7'd41;          // 99*41 = 4059
    wire [7:0]  q7    = {2'b00, qprod[11:6]};      // >> 6
    wire [7:0]  qsum  = q7 + {2'b00, rs_q};
    wire [5:0]  qr_w  = (qsum > 8'd63) ? 6'd63 : qsum[5:0];

    // static-rate: min(rate + rate_scaling_, 99)
    wire [7:0] srs_w = {1'b0, adv_rate} + {2'b00, rs_q};
    wire [6:0] sr_w  = (srs_w > 8'd99) ? 7'd99 : srs_w[6:0];

    // statics hold lengths (env.cc:33-44), 77 entries x 21 b
    reg [20:0] sta_v;
    always @(*) begin
        case (sr_w)
7'd00: sta_v = 21'd1764000;
                7'd01: sta_v = 21'd1764000;
                7'd02: sta_v = 21'd1411200;
                7'd03: sta_v = 21'd1411200;
                7'd04: sta_v = 21'd1190700;
                7'd05: sta_v = 21'd1014300;
                7'd06: sta_v = 21'd0992250;
                7'd07: sta_v = 21'd0882000;
                7'd08: sta_v = 21'd0705600;
                7'd09: sta_v = 21'd0705600;
                7'd10: sta_v = 21'd0584325;
                7'd11: sta_v = 21'd0507150;
                7'd12: sta_v = 21'd0502740;
                7'd13: sta_v = 21'd0441000;
                7'd14: sta_v = 21'd0418950;
                7'd15: sta_v = 21'd0352800;
                7'd16: sta_v = 21'd0308700;
                7'd17: sta_v = 21'd0286650;
                7'd18: sta_v = 21'd0253575;
                7'd19: sta_v = 21'd0220500;
                7'd20: sta_v = 21'd0220500;
                7'd21: sta_v = 21'd0176400;
                7'd22: sta_v = 21'd0145530;
                7'd23: sta_v = 21'd0145530;
                7'd24: sta_v = 21'd0125685;
                7'd25: sta_v = 21'd0110250;
                7'd26: sta_v = 21'd0110250;
                7'd27: sta_v = 21'd0088200;
                7'd28: sta_v = 21'd0088200;
                7'd29: sta_v = 21'd0074970;
                7'd30: sta_v = 21'd0061740;
                7'd31: sta_v = 21'd0061740;
                7'd32: sta_v = 21'd0055125;
                7'd33: sta_v = 21'd0048510;
                7'd34: sta_v = 21'd0044100;
                7'd35: sta_v = 21'd0037485;
                7'd36: sta_v = 21'd0031311;
                7'd37: sta_v = 21'd0030870;
                7'd38: sta_v = 21'd0027562;
                7'd39: sta_v = 21'd0027562;
                7'd40: sta_v = 21'd0022050;
                7'd41: sta_v = 21'd0018522;
                7'd42: sta_v = 21'd0017640;
                7'd43: sta_v = 21'd0015435;
                7'd44: sta_v = 21'd0014112;
                7'd45: sta_v = 21'd0013230;
                7'd46: sta_v = 21'd0011025;
                7'd47: sta_v = 21'd0009261;
                7'd48: sta_v = 21'd0009261;
                7'd49: sta_v = 21'd0007717;
                7'd50: sta_v = 21'd0006615;
                7'd51: sta_v = 21'd0006615;
                7'd52: sta_v = 21'd0005512;
                7'd53: sta_v = 21'd0005512;
                7'd54: sta_v = 21'd0004410;
                7'd55: sta_v = 21'd0003969;
                7'd56: sta_v = 21'd0003969;
                7'd57: sta_v = 21'd0003439;
                7'd58: sta_v = 21'd0002866;
                7'd59: sta_v = 21'd0002690;
                7'd60: sta_v = 21'd0002249;
                7'd61: sta_v = 21'd0001984;
                7'd62: sta_v = 21'd0001896;
                7'd63: sta_v = 21'd0001808;
                7'd64: sta_v = 21'd0001411;
                7'd65: sta_v = 21'd0001367;
                7'd66: sta_v = 21'd0001234;
                7'd67: sta_v = 21'd0001146;
                7'd68: sta_v = 21'd0000926;
                7'd69: sta_v = 21'd0000837;
                7'd70: sta_v = 21'd0000837;
                7'd71: sta_v = 21'd0000705;
                7'd72: sta_v = 21'd0000573;
                7'd73: sta_v = 21'd0000573;
                7'd74: sta_v = 21'd0000529;
                7'd75: sta_v = 21'd0000441;
                7'd76: sta_v = 21'd0000441;
            default: sta_v = 21'd0;
        endcase
    end

    // raw static count for the static condition
    wire [20:0] raw_ge77 = (99 - sr_w) * 5'd20;    // sr 77..99 -> 440..0
    wire [20:0] raw_base = (sr_w < 7'd77) ? sta_v : raw_ge77;
    wire        div20    = (sr_w < 7'd77) & (aix == 3'd0) & (newlv_r == 7'd0);
    wire [44:0] raw_prod = raw_base * DIV20_M;     // 21b x 24b -> 41b used
    wire [20:0] raw_w    = static_cond_r
                         ? (div20 ? raw_prod[44:24] : raw_base)
                         : 21'd0;

    // staticcount_ = (raw * sr_multiplier) >> 24   (int64 in the model)
    wire [63:0] stat_prod = raw_static_r * SRM_EFF;  // <= 45 b used
    wire [31:0] stat_w    = stat_prod[52:24];        // >> 24, fits 29 b

    // inc_: ((4 + (qrate & 3)) << (2 + LG_N + (qrate >> 2)))
    //       * sr_multiplier >> 24
    wire [25:0] inc_base = 26'd4 + {24'b0, qr_r[1:0]};
    wire [25:0] inc_pre  = inc_base << (5'd8 + qr_r[5:2]);  // shift 8..23
    wire [63:0] inc_prod = {38'b0, inc_pre_r} * SRM_EFF;    // <= 58 b used
    wire [31:0] inc_w    = inc_prod[55:24];                 // fits 26 b

    // ------------------------------------------------------------------
    // Step (getsample) combinational path
    // ------------------------------------------------------------------
    // movement gate: ix_ < 3 or (ix_ < 4 and !down_)   [env.cc:73]
    wire mov_ok = (ix_q < 3'd3) | ((ix_q < 3'd4) & ~down_q);
    // rising leg: jump floor, then level += ((RISE_BASE - level) >> 24)*inc
    // (arithmetic >> == the model's floor shift, sign-safe for any loaded
    // state; reachable states keep the quotient in 1..10)
    wire [31:0] lv_jumped = (level_q < JUMP_FLOOR) ? JUMP_FLOOR : level_q;
    wire signed [33:0] base_diff = $signed({2'b00, RISE_BASE})
                                 - $signed({2'b00, lv_jumped});
    wire signed [9:0]  quot10    = base_diff >>> 24;
    wire signed [41:0] rise_prod = quot10 * $signed(inc_q);
    wire signed [41:0] rise_sum  = $signed({10'b0, lv_jumped}) + rise_prod;
    wire        rise_clamp = rise_sum >= $signed({14'b0, target_q});
    wire [31:0] rise_next  = rise_clamp ? target_q : rise_sum[31:0];
    // falling leg: level -= inc, clamp <= target (borrow => below target)
    wire [32:0] fall_diff  = {1'b0, level_q} - {1'b0, inc_q};
    wire        fall_clamp = fall_diff[32]
                           | ({1'b0, fall_diff[31:0]} <= {1'b0, target_q});
    wire [31:0] fall_next  = fall_clamp ? target_q : fall_diff[31:0];

    // movement actually steps (else hold / park)
    wire mov_step = mov_ok & (static_q == 32'd0);
    // movement clamp: launch the second advance of the step
    wire mov_clamp = mov_step & (rising_q ? rise_clamp : fall_clamp);

    // ------------------------------------------------------------------
    // Next-state (committed state only; the single read point per path)
    // ------------------------------------------------------------------
    reg [3:0] nstate;
    always @(*) begin
        case (state_q)
            S_IDLE: begin
                if (cmd_en) begin
                    case (cmd)
                        CMD_PARAM:   nstate = S_PARAM;
                        CMD_LOAD:    nstate = S_LOAD;
                        CMD_INIT:    nstate = S_INIT;
                        CMD_KEYDOWN: nstate = S_KD;
                        CMD_UPDATE:  nstate = S_UPD;
                        CMD_STEP:    nstate = S_STEP_CAP;
                        CMD_ADV:     nstate = S_ADV_TGT;
                        default:     nstate = S_IDLE;
                    endcase
                end else
                    nstate = S_IDLE;
            end
            S_PARAM, S_LOAD: nstate = S_IDLE;
            S_INIT:     nstate = S_ADV_TGT;
            S_KD:       nstate = kd_fire ? S_ADV_TGT : S_IDLE;
            S_UPD:      nstate = down_q ? S_ADV_TGT : S_IDLE;
            S_STEP_CAP: nstate = st_adv_r ? S_ADV_TGT : S_STEP_MOV;
            S_STEP_MOV: nstate = mov_clamp ? S_ADV_TGT : S_IDLE;
            S_ADV_TGT:  nstate = S_ADV_QR;
            S_ADV_QR:   nstate = S_ADV_STA;
            S_ADV_STA:  nstate = S_ADV_MUL1;
            S_ADV_MUL1: nstate = S_ADV_MUL2;
            S_ADV_MUL2: nstate = adv_ret_mov ? S_STEP_MOV : S_IDLE;
            default:    nstate = S_IDLE;
        endcase
    end

    // ------------------------------------------------------------------
    // Sequential logic: every command captures once, commits once
    // (write-new at a single clock edge; reads are of committed state)
    // ------------------------------------------------------------------
    always @(posedge clk) begin
        if (rst) begin
            state_q    <= S_IDLE;
            done_q     <= 1'b0;
            level_q    <= 32'h0;
            target_q   <= 32'h0;
            inc_q      <= 32'h0;
            static_q   <= 32'h0;
            ix_q       <= 3'h0;
            rising_q   <= 1'b0;
            down_q     <= 1'b0;
            rates_q[0] <= 7'h0;
            rates_q[1] <= 7'h0;
            rates_q[2] <= 7'h0;
            rates_q[3] <= 7'h0;
            levels_q[0] <= 7'h0;
            levels_q[1] <= 7'h0;
            levels_q[2] <= 7'h0;
            levels_q[3] <= 7'h0;
            outlevel_q <= 13'h0;
            rs_q       <= 6'h0;
            adv_ix     <= 4'h0;
            adv_ret_mov <= 1'b0;
            st_after_r <= 32'h0;
            st_adv_r   <= 1'b0;
            kd_fire    <= 1'b0;
`ifdef H04_MUTATE_STEP_SKEW
            skew_v     <= 1'b0;
            skew_level <= 32'h0;
`endif
        end else begin
            done_q <= (nstate == S_IDLE) && (state_q != S_IDLE);

            case (state_q)
                S_IDLE: begin
                    if (cmd_en) begin
                        if (cmd != CMD_KEYDOWN)
                            kd_fire <= 1'b0;
                        case (cmd)
                            CMD_PARAM: begin
                                rates_q[0] <= cmd_data[74:68];
                                rates_q[1] <= cmd_data[67:61];
                                rates_q[2] <= cmd_data[60:54];
                                rates_q[3] <= cmd_data[53:47];
                                levels_q[0] <= cmd_data[46:40];
                                levels_q[1] <= cmd_data[39:33];
                                levels_q[2] <= cmd_data[32:26];
                                levels_q[3] <= cmd_data[25:19];
                                outlevel_q <= cmd_data[18:6];
                                rs_q       <= cmd_data[5:0];
                            end
                            CMD_LOAD: begin
                                level_q  <= cmd_data[132:101];
                                target_q <= cmd_data[100:69];
                                inc_q    <= cmd_data[68:37];
                                static_q <= cmd_data[36:5];
                                ix_q     <= cmd_data[4:2];
                                rising_q <= cmd_data[1];
                                down_q   <= cmd_data[0];
                            end
                            CMD_INIT: begin
                                // Env::init: level_ = 0, down_ = 1, then
                                // advance(0) -- rising_ computed vs level 0
                                level_q <= 32'h0;
                                down_q  <= 1'b1;
                                adv_ix  <= 4'd0;
                                adv_ret_mov <= 1'b0;
                            end
                            CMD_KEYDOWN: begin
                                // env.cc:102-107: on a down_ edge, start
                                // segment 0 (down) or 3 (up) from the
                                // CURRENT level; no re-init here. The
                                // edge test uses the pre-edge down_q and
                                // is captured here -- down_q commits at
                                // this same edge.
                                kd_fire <= (down_q != cmd_data[0]);
                                if (down_q != cmd_data[0]) begin
                                    down_q <= cmd_data[0];
                                    adv_ix <= cmd_data[0] ? 4'd0 : 4'd3;
                                    adv_ret_mov <= 1'b0;
                                end
                            end
                            CMD_UPDATE: begin
                                // param refresh; while down_ the sustain
                                // target recompute (levels_[2], no
                                // outlevel, env.cc:166-167) is dead state
                                // -- advance(2) overwrites it (pinned
                                // quirk; final committed state identical)
                                rates_q[0] <= cmd_data[74:68];
                                rates_q[1] <= cmd_data[67:61];
                                rates_q[2] <= cmd_data[60:54];
                                rates_q[3] <= cmd_data[53:47];
                                levels_q[0] <= cmd_data[46:40];
                                levels_q[1] <= cmd_data[39:33];
                                levels_q[2] <= cmd_data[32:26];
                                levels_q[3] <= cmd_data[25:19];
                                outlevel_q <= cmd_data[18:6];
                                rs_q       <= cmd_data[5:0];
                                if (down_q) begin
                                    adv_ix <= 4'd2;
                                    adv_ret_mov <= 1'b0;
                                end
                            end
                            CMD_STEP: begin
                                // the step's single capture (read-old) +
                                // static-branch eval; decrement commits
                                // at this edge (write-new)
                                st_adv_r <= (static_q != 32'd0)
                                         && (static_q <= {25'b0, STEP_N});
                                st_after_r <= (static_q == 32'd0)
                                            ? 32'd0
                                            : ((static_q <= {25'b0, STEP_N})
                                               ? 32'd0
                                               : static_q - {25'b0, STEP_N});
                                adv_ix <= (static_q != 32'd0)
                                       && (static_q <= {25'b0, STEP_N})
                                        ? {1'b0, ix_q} + 4'd1 : adv_ix;
                                adv_ret_mov <= (static_q != 32'd0)
                                            && (static_q <= {25'b0, STEP_N});
`ifdef H04_MUTATE_STEP_SKEW
                                // apply the previous step's deferred
                                // movement one frame late (control)
                                if (skew_v) begin
                                    level_q <= skew_level;
                                    skew_v  <= 1'b0;
                                end
`endif
                            end
                            CMD_ADV: begin
                                adv_ix <= {1'b0, cmd_data[2:0]};
                                adv_ret_mov <= 1'b0;
                            end
                            default: ;
                        endcase
                    end
                end
                S_STEP_CAP: begin
                    static_q <= st_after_r;
                end
                S_STEP_MOV: begin
                    if (mov_step) begin
`ifdef H04_MUTATE_STEP_SKEW
                        skew_v     <= 1'b1;
                        skew_level <= rising_q ? rise_next : fall_next;
`else
                        level_q <= rising_q ? rise_next : fall_next;
`endif
                        if (mov_clamp) begin
                            adv_ix <= {1'b0, ix_q} + 4'd1;
                            adv_ret_mov <= 1'b0;
                        end
                    end
                end
                S_ADV_TGT: begin
                    tgt_new <= tgt_w;
                    newlv_r <= adv_lv;
                end
                S_ADV_QR: begin
                    rising_new    <= rising_w;
                    qr_r          <= qr_w;
                    static_cond_r <= (tgt_new == level_q)
                                   | ((aix == 3'd0) & (newlv_r == 7'd0));
                end
                S_ADV_STA: begin
                    raw_static_r <= raw_w;
                end
                S_ADV_MUL1: begin
                    stat_new_r <= stat_w;
                    inc_pre_r  <= inc_pre;
                end
                S_ADV_MUL2: begin
                    adv_ret_mov <= 1'b0;
                    if (aix_lt4) begin
                        ix_q     <= aix;
                        rising_q <= rising_new;
                        target_q <= tgt_new;
                        static_q <= stat_new_r;
                        inc_q    <= inc_w;
                    end else begin
                        ix_q <= aix;   // park: ix_-only commit (env.cc:113)
                    end
                end
                default: ;
            endcase

            state_q <= nstate;
        end
    end

    // external slot write (control-tail/event commit path): committed
    // state only, idle cycles only (the step owns the slot while busy)
    always @(posedge clk) begin
        if (!rst && slot_wr_en && !busy) begin
            level_q  <= slot_wr_data[132:101];
            target_q <= slot_wr_data[100:69];
            inc_q    <= slot_wr_data[68:37];
            static_q <= slot_wr_data[36:5];
            ix_q     <= slot_wr_data[4:2];
            rising_q <= slot_wr_data[1];
            down_q   <= slot_wr_data[0];
        end
    end

`ifdef H04_STRIP_OBSERVABILITY
    // NEGATIVE CONTROL: no output observable, no reduction -- synthesis
    // must (and does) delete every state flop (busy/done included); the
    // flop-count gate then FAILS. This build exists only to prove the
    // gate has resolution; it is never simulated.
    assign slot_rd_data = 133'h0;
    assign param_rd     = 75'h0;
    assign state_obs    = 1'b0;
    assign busy         = 1'b0;
    assign done         = 1'b0;
`else
    assign busy = (state_q != S_IDLE);
    assign done = done_q;
    assign slot_rd_data = {level_q, target_q, inc_q, static_q,
                           ix_q, rising_q, down_q};
    assign param_rd = {rates_q[3], rates_q[2], rates_q[1], rates_q[0],
                       levels_q[3], levels_q[2], levels_q[1], levels_q[0],
                       outlevel_q, rs_q};

    wire obs_next = (^level_q) ^ (^target_q) ^ (^inc_q) ^ (^static_q)
                  ^ (^ix_q) ^ rising_q ^ down_q
                  ^ (^{rates_q[0], rates_q[1], rates_q[2], rates_q[3]})
                  ^ (^{levels_q[0], levels_q[1], levels_q[2], levels_q[3]})
                  ^ (^outlevel_q) ^ (^rs_q);
    always @(posedge clk) begin
        if (rst)
            obs_q <= 1'b0;
        else
            obs_q <= obs_next;
    end
    assign state_obs = obs_q;
`endif

endmodule

`default_nettype wire
