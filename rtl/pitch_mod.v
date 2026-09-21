// pitch_mod.v -- H06 pitch/modulation RTL unit (issue #28).
//
// One per-note pitch/modulation datapath, RTL-exact to the frozen N05
// integer model (src/gf180_dx7/model/pitch.py, contract
// gf180-dx7-integer-v1; docs/N05-PITCH-MODEL.md), composed in the pinned
// integrated order of src/gf180_dx7/model/integrated.py::compute_frame
// (seam finding F-3, DR-0006): ratio-mode operators take
// basepitch + i32(pitch_mod_nobase + pitch_base); fixed-mode operators
// take basepitch + pitch_base only.
//
// HOST/CORE SPLIT (docs/CONTRACT-CORE-v1.md sections 4.2/4.7/5;
// DEC-010/DEC-014/DEC-019/DEC-022/DEC-023; NUM-008
// "event_time_pitch_math"): the core advances the audio-rate pitch path
// autonomously -- pitch-EG stepping, LFO sampling (six waveforms, delay
// ramp, keysync), PM/AM depth composition, transpose application and the
// Freqlut lookups all live here, stepped once per 64-sample frame from
// the frame controller's control tail. The host sends ONLY per-event
// frozen integers through the cfg write port (one write per contracted
// register): patch-derived params at patch-commit time and the
// float-gated event-time values (per-op detune deltas, fine terms, bend
// scale, controller levels) that NUM-008 assigns to host/control-side
// math off the audio frame budget. There is NO per-sample host input;
// nothing in the audio path waits on the host (DEC-010, contract 4.7).
//
// Transpose ruling: the patch's own transpose byte is applied EXACTLY
// ONCE, inside the core, at note-on (contract 4.2 + DEC-023: "the
// patch's own parameters inside the core, host MIDI transposition
// host-side"), in the DOCUMENTED upstream mode: shift = transpose - 24
// semitones (PluginProcessor.cpp:458/:566-580). The pinned-oracle
// wrapper's omission of that shift (registry finding perf-transpose-12)
// is a wrapper finding, not RTL scope; N08 perf-transpose-12 remains the
// wrapper-level conformance reference and is not reproduced here.
//
// ROMs: $readmemh from reference/tables/freqlut_table.hex -- the SAME
// pinned bytes the model verifies by SHA-256 in
// reference/tables/manifest.json (freqlut 6d3c5970..., generator
// tools/gen_tables.py --check) -- and reference/tables/lfo_sin_table.hex,
// the regenerated msfa Sin table (SIN_DELTA form; sin.cc:31-56, sin.h),
// which the model rebuilds with build_sintab() and the compare harness
// re-verifies byte-for-byte on every run.
//
// Per-frame arithmetic (one frame = one control-tail pass; citations):
//   LFO   lfo.cc:51-97   one getsample() + one getdelay(); keydown()
//                        (sync restarts phase_ = 2^31-1; delaystate_ = 0
//                        always) at note-on.
//   PEG   pitchenv.cc:45-93  one getsample(): additive level_ +/- inc_
//                        against PITCHENV_TAB[l]<<19 targets,
//                        inc_ = PITCHENV_RATE[r]*1050; segments 0-2 run
//                        while keydown, segment 3 on release; set() +
//                        advance(0) at note-on.
//   PM    dx7note.cc:210-217 (+ integrated F-3 composition)
//                        pmd = PM_DEPTH*lfo_delay (u32); senslfo =
//                        i32(PM_SENS*(lfo_val-2^23)); pmod_1 =
//                        |i32((pmd*senslfo)>>39)|; pmod_2 =
//                        |i32((ctrl_pm*senslfo)>>14)|; pm_nobase =
//                        i32(peg + max(pmod_1,pmod_2)*sign(senslfo)).
//                        pitch_base = i32(bend_pb + master_tune), where
//                        bend_pb is the host-frozen scaled bend integer
//                        (NUM-008 float-gated spot).
//   AM    dx7note.cc:259-268  amd_mod depth composition (the in-operator
//                        application stays with the operator path, N06).
//   FREQ  freqlut.cc:46-55  linear interpolation over the low 14 bits,
//                        y >> (20 - hibits); ratio input =
//                        i32(basepitch + i32(pm_nobase + pitch_base));
//                        fixed input = i32(basepitch + pitch_base).
//   basepitch (at note-on) dx7note.cc:38-64: ratio =
//                        i32(i32(l' + delta) + COARSEMUL[coarse&31])
//                        (+ fine_term when fine != 0); fixed = pure
//                        integer formula; l' =
//                        midinote_to_logfreq(note + transpose - 24).
//                        delta/fine_term are the host-frozen integers
//                        for the float-gated detune/fine steps.
//
// Freqlut shift amounts with hibits > 20 (a left/negative shift) are the
// declared C-UB/unreachable domain (freqlut.cc:44-45 note, mirrored in
// the model); the clamp is unreachable at audio parameters. abs() on
// INT_MIN is likewise unreachable (model deviation note 2).
//
// NEGATIVE CONTROLS (each must demonstrably fail its check):
//   -DMUTATE_INCREMENT_LSB         op-0 phase increment +1 LSB every
//                                  frame; the exact compare MUST fail.
//   -DMUTATE_FIXED_TRACKS_KEYBOARD fixed-mode basepitch gains the
//                                  keyboard logfreq; fixed-mode
//                                  invariance frames MUST fail (N05
//                                  named control).
//   -DMUTATE_LFO_SYNC_IGNORED      LFO keydown keeps phase_; keysync
//                                  frames MUST fail (N05 named control).
//   -DH06_PER_SAMPLE_HOST          adds an illegal per-sample host
//                                  streaming port and gates the
//                                  increment commit on it; increments
//                                  stall without the stream and the
//                                  compare MUST fail (split violation).
//   -DH06_STRIP_OBSERVABILITY      all outputs tied off; yosys deletes
//                                  every state flop and the flop gate
//                                  MUST FAIL (tools/h06_synth.py).
//
// This file alone claims no P&R, no timing closure, no fit, and no
// original-DX7 fidelity: bit-exactness to the frozen model is the bar,
// and docs/H06-PITCH-RTL.md records exactly what was measured.
`default_nettype none

`ifndef H06_FREQLUT_HEX
`define H06_FREQLUT_HEX "reference/tables/freqlut_table.hex"
`endif
`ifndef H06_LFO_SIN_HEX
`define H06_LFO_SIN_HEX "reference/tables/lfo_sin_table.hex"
`endif

module pitch_mod (
    input  wire        clk,
    input  wire        rst,           // synchronous, active high
    // Contracted event/config write port (contract section 5 wr_* shape):
    // one 32-bit write per link write, per-event only. NEVER a per-sample
    // stream (DEC-010; contract 4.7; guarded by tests/test_h06.py).
    input  wire        cfg_wr,
    input  wire [7:0]  cfg_addr,
    input  wire [31:0] cfg_wdata,
    // Frame-controller control-tail tick: schedule event, no payload.
    input  wire        frame_tick,
    // Frame results, stable for the whole 64-sample frame (consumed by the
    // op core): six 32-bit phase increments + boundary observability.
    output wire [31:0] freq0,
    output wire [31:0] freq1,
    output wire [31:0] freq2,
    output wire [31:0] freq3,
    output wire [31:0] freq4,
    output wire [31:0] freq5,
    output wire [31:0] peg_level,     // post-step pitch-EG level (int32)
    output wire [24:0] lfo_val,       // this frame's LFO sample
    output wire [24:0] lfo_delay,     // this frame's LFO delay ramp
    output wire [31:0] amd_mod,       // AM depth composition (Q24, N06 input)
    output wire        out_valid,
    output wire        busy,
    output wire        state_obs      // registered XOR fold of all state
`ifdef H06_PER_SAMPLE_HOST
    // NEGATIVE CONTROL ONLY (see header): an illegal per-sample host
    // streaming input. The contracted interface has no such port.
    , input  wire      host_stream_valid
`endif
);

    // Constants (frozen at 48 kHz; pitch.py / lfo.cc / pitchenv.cc)
    localparam [31:0] A4_BASE      = 32'd50857777;  // tuning.cc:14-19
    localparam [31:0] SEMI_LOGFREQ = 32'd1398101;   // (1<<24)//12
    localparam [31:0] LFO_UNIT     = 32'd33587;     // lfo.cc:26-29 @48k
    localparam [31:0] PEG_UNIT     = 32'd1050;      // pitchenv.cc:22-24 @48k

    // Command opcodes (cfg 0x00 data[7:0]); data[15:8] = note for NOTE_ON.
    localparam [7:0] CMD_NOTE_ON  = 8'd1;
    localparam [7:0] CMD_NOTE_OFF = 8'd2;
    localparam [7:0] CMD_COMMIT   = 8'd3;

    // Contracted register map (docs/H06-PITCH-RTL.md section 2):
    //   0x00 CMD             {note[15:8], cmd[7:0]}
    //   0x10..0x15 OP_CFG    {detune, fine, coarse, mode}   (patch, shadow)
    //   0x18..0x1D OP_FTERM  fine term, host-frozen int32   (patch, shadow)
    //   0x20 PEG_RATES       {8'b0, r3, r2, r1, r0}         (patch, shadow)
    //   0x21 PEG_LEVELS      {8'b0, l3, l2, l1, l0}         (patch, shadow)
    //   0x22 LFO_PARAMS      {wave, sync, delay, rate}      (patch, shadow)
    //   0x23 PM_DEPTH        host-frozen int32              (patch, shadow)
    //   0x24 PM_SENS         host-frozen int32              (patch, shadow)
    //   0x25 TRANSPOSE       {24'b0, transpose}             (patch, shadow)
    //   0x26 AM_DEPTH        host-frozen int32              (patch, shadow)
    //   0x28 BEND_PB         host-frozen scaled bend int32  (perf, direct)
    //   0x29 MASTER_TUNE     int32                          (perf, direct)
    //   0x2A CTRL_PM         int32                          (perf, direct)
    //   0x2B CTRL_AM         int32                          (perf, direct)
    //   0x2C EG_MOD          int32, reset 127 (pinned default) (perf, direct)
    //   0x30..0x35 OP_DDELTA per-op detune delta, int32     (note-on payload)
    // Patch-shadowed registers swap into the active image only on CMD_COMMIT
    // (atomic at the frame tick, DEC-019); performance settings are separate
    // state applied at event time (DEC-022); note-on payloads are consumed
    // by CMD_NOTE_ON at the tick (an event received during frame N applies
    // before the first sample of frame N+1, DEC-014).

    // ------------------------------------------------------------------
    // State
    // ------------------------------------------------------------------
    reg [31:0] shd_opcfg  [0:5];
    reg [31:0] shd_fterm  [0:5];
    reg [31:0] shd_pegr, shd_pegl, shd_lfop, shd_pmdep, shd_pmsens;
    reg [7:0]  shd_trans;
    reg [31:0] shd_amdep;
    reg [31:0] act_opcfg  [0:5];
    reg [31:0] act_fterm  [0:5];
    reg [31:0] act_pegr, act_pegl, act_lfop, act_pmdep, act_pmsens;
    reg [7:0]  act_trans;
    reg [31:0] act_amdep;
    // performance state (DEC-022: separate, applied at event time)
    reg [31:0] bend_pb, master_tune, ctrl_pm, ctrl_am, eg_mod;
    // note-on payload
    reg [31:0] pend_delta [0:5];
    reg        pend_commit;    // CMD_COMMIT pending (applied first at tick)
    reg [1:0]  pend_note;      // 0 none, 1 note-on, 2 note-off (then commit)
    reg [7:0]  cmd_note;
    // derived at note-on
    reg [31:0] note_logfreq_q;     // l' (transpose applied here, DEC-023)
    reg [31:0] basepitch_q [0:5];
    reg [5:0]  opmode_q;           // frozen at note-on (model op_mode)
    // LFO state (lfo.cc; zeroed at reset per the model's declared init)
    reg [31:0] lfo_phase, lfo_delta, lfo_dstate, lfo_dinc1, lfo_dinc2;
    reg [7:0]  lfo_rand;
    reg [24:0] lfo_val_q, lfo_del_q;
    // pitch-EG state (pitchenv.cc; PitchEnv() init at reset)
    reg [31:0] peg_lvl, peg_tgt, peg_inc;
    reg        peg_rising, peg_down;
    reg [2:0]  peg_ix;
    reg [31:0] peg_out_q;
    // per-frame composition
    reg [31:0] pm_nobase_q, pitch_base_q, amd_q;
    // freqlut stage
    reg [9:0]  faddr_q;
    reg [13:0] flsb_q;
    reg [8:0]  fsh_q;              // 20 - hibits (>= 0 in domain)
    reg [31:0] freq_q [0:5];
    // FSM
    reg [3:0]  phase;
    reg [2:0]  opix;
    reg        out_valid_q;
    reg        obs_next;
    reg        obs_q;

    localparam S_IDLE = 4'd0, S_COMMIT = 4'd1, S_NN = 4'd2, S_NNB = 4'd3,
               S_NO = 4'd4, S_LFO = 4'd5, S_PEG = 4'd6, S_PM = 4'd7,
               S_FA = 4'd8, S_FB = 4'd9, S_OUT = 4'd10, S_FIN = 4'd11;

    // ------------------------------------------------------------------
    // ROMs: pinned freqlut table bytes (manifest-hashed) + regenerated
    // LFO sine table (SIN_DELTA form: dy at even, y0 at odd indices)
    // ------------------------------------------------------------------
    reg [31:0] freqlut_rom [0:1024];
    reg [25:0] sin_rom     [0:2047];
    initial begin
        $readmemh(`H06_FREQLUT_HEX, freqlut_rom);
        $readmemh(`H06_LFO_SIN_HEX, sin_rom);
    end

    // dx7note.cc:30-36 coarsemul table (Q24 log2 multipliers, coarse 0..31).
    // 32-bit signed return: a narrower signed return would be zero-extended
    // (not sign-extended) inside the unsigned addition context below.
    function signed [31:0] coarsemul_lut(input [4:0] i);
        case (i)
            5'd0:  coarsemul_lut = -32'sd16777216;
            5'd1:  coarsemul_lut = 32'sd0;
            5'd2:  coarsemul_lut = 32'sd16777216;
            5'd3:  coarsemul_lut = 32'sd26591258;
            5'd4:  coarsemul_lut = 32'sd33554432;
            5'd5:  coarsemul_lut = 32'sd38955489;
            5'd6:  coarsemul_lut = 32'sd43368474;
            5'd7:  coarsemul_lut = 32'sd47099600;
            5'd8:  coarsemul_lut = 32'sd50331648;
            5'd9:  coarsemul_lut = 32'sd53182516;
            5'd10: coarsemul_lut = 32'sd55732705;
            5'd11: coarsemul_lut = 32'sd58039632;
            5'd12: coarsemul_lut = 32'sd60145690;
            5'd13: coarsemul_lut = 32'sd62083076;
            5'd14: coarsemul_lut = 32'sd63876816;
            5'd15: coarsemul_lut = 32'sd65546747;
            5'd16: coarsemul_lut = 32'sd67108864;
            5'd17: coarsemul_lut = 32'sd68576247;
            5'd18: coarsemul_lut = 32'sd69959732;
            5'd19: coarsemul_lut = 32'sd71268397;
            5'd20: coarsemul_lut = 32'sd72509921;
            5'd21: coarsemul_lut = 32'sd73690858;
            5'd22: coarsemul_lut = 32'sd74816848;
            5'd23: coarsemul_lut = 32'sd75892776;
            5'd24: coarsemul_lut = 32'sd76922906;
            5'd25: coarsemul_lut = 32'sd77910978;
            5'd26: coarsemul_lut = 32'sd78860292;
            5'd27: coarsemul_lut = 32'sd79773775;
            5'd28: coarsemul_lut = 32'sd80654032;
            5'd29: coarsemul_lut = 32'sd81503396;
            5'd30: coarsemul_lut = 32'sd82323963;
            default: coarsemul_lut = 32'sd83117622;
        endcase
    endfunction

    // pitchenv.cc:26-33 rate table, indices 0..99
    function [7:0] pitchenv_rate_lut(input [6:0] i);
        case (i)
            7'd0:  pitchenv_rate_lut = 8'd1;
            7'd1:  pitchenv_rate_lut = 8'd2;
            7'd2:  pitchenv_rate_lut = 8'd3;
            7'd3:  pitchenv_rate_lut = 8'd3;
            7'd4:  pitchenv_rate_lut = 8'd4;
            7'd5:  pitchenv_rate_lut = 8'd4;
            7'd6:  pitchenv_rate_lut = 8'd5;
            7'd7:  pitchenv_rate_lut = 8'd5;
            7'd8:  pitchenv_rate_lut = 8'd6;
            7'd9:  pitchenv_rate_lut = 8'd6;
            7'd10: pitchenv_rate_lut = 8'd7;
            7'd11: pitchenv_rate_lut = 8'd7;
            7'd12: pitchenv_rate_lut = 8'd8;
            7'd13: pitchenv_rate_lut = 8'd8;
            7'd14: pitchenv_rate_lut = 8'd9;
            7'd15: pitchenv_rate_lut = 8'd9;
            7'd16: pitchenv_rate_lut = 8'd10;
            7'd17: pitchenv_rate_lut = 8'd10;
            7'd18: pitchenv_rate_lut = 8'd11;
            7'd19: pitchenv_rate_lut = 8'd11;
            7'd20: pitchenv_rate_lut = 8'd12;
            7'd21: pitchenv_rate_lut = 8'd12;
            7'd22: pitchenv_rate_lut = 8'd13;
            7'd23: pitchenv_rate_lut = 8'd13;
            7'd24: pitchenv_rate_lut = 8'd14;
            7'd25: pitchenv_rate_lut = 8'd14;
            7'd26: pitchenv_rate_lut = 8'd15;
            7'd27: pitchenv_rate_lut = 8'd16;
            7'd28: pitchenv_rate_lut = 8'd16;
            7'd29: pitchenv_rate_lut = 8'd17;
            7'd30: pitchenv_rate_lut = 8'd18;
            7'd31: pitchenv_rate_lut = 8'd18;
            7'd32: pitchenv_rate_lut = 8'd19;
            7'd33: pitchenv_rate_lut = 8'd20;
            7'd34: pitchenv_rate_lut = 8'd21;
            7'd35: pitchenv_rate_lut = 8'd22;
            7'd36: pitchenv_rate_lut = 8'd23;
            7'd37: pitchenv_rate_lut = 8'd24;
            7'd38: pitchenv_rate_lut = 8'd25;
            7'd39: pitchenv_rate_lut = 8'd26;
            7'd40: pitchenv_rate_lut = 8'd27;
            7'd41: pitchenv_rate_lut = 8'd28;
            7'd42: pitchenv_rate_lut = 8'd30;
            7'd43: pitchenv_rate_lut = 8'd31;
            7'd44: pitchenv_rate_lut = 8'd33;
            7'd45: pitchenv_rate_lut = 8'd34;
            7'd46: pitchenv_rate_lut = 8'd36;
            7'd47: pitchenv_rate_lut = 8'd37;
            7'd48: pitchenv_rate_lut = 8'd38;
            7'd49: pitchenv_rate_lut = 8'd39;
            7'd50: pitchenv_rate_lut = 8'd41;
            7'd51: pitchenv_rate_lut = 8'd42;
            7'd52: pitchenv_rate_lut = 8'd44;
            7'd53: pitchenv_rate_lut = 8'd46;
            7'd54: pitchenv_rate_lut = 8'd47;
            7'd55: pitchenv_rate_lut = 8'd49;
            7'd56: pitchenv_rate_lut = 8'd51;
            7'd57: pitchenv_rate_lut = 8'd53;
            7'd58: pitchenv_rate_lut = 8'd54;
            7'd59: pitchenv_rate_lut = 8'd56;
            7'd60: pitchenv_rate_lut = 8'd58;
            7'd61: pitchenv_rate_lut = 8'd60;
            7'd62: pitchenv_rate_lut = 8'd62;
            7'd63: pitchenv_rate_lut = 8'd64;
            7'd64: pitchenv_rate_lut = 8'd66;
            7'd65: pitchenv_rate_lut = 8'd68;
            7'd66: pitchenv_rate_lut = 8'd70;
            7'd67: pitchenv_rate_lut = 8'd72;
            7'd68: pitchenv_rate_lut = 8'd74;
            7'd69: pitchenv_rate_lut = 8'd76;
            7'd70: pitchenv_rate_lut = 8'd79;
            7'd71: pitchenv_rate_lut = 8'd82;
            7'd72: pitchenv_rate_lut = 8'd85;
            7'd73: pitchenv_rate_lut = 8'd88;
            7'd74: pitchenv_rate_lut = 8'd91;
            7'd75: pitchenv_rate_lut = 8'd94;
            7'd76: pitchenv_rate_lut = 8'd98;
            7'd77: pitchenv_rate_lut = 8'd102;
            7'd78: pitchenv_rate_lut = 8'd106;
            7'd79: pitchenv_rate_lut = 8'd110;
            7'd80: pitchenv_rate_lut = 8'd115;
            7'd81: pitchenv_rate_lut = 8'd120;
            7'd82: pitchenv_rate_lut = 8'd125;
            7'd83: pitchenv_rate_lut = 8'd130;
            7'd84: pitchenv_rate_lut = 8'd135;
            7'd85: pitchenv_rate_lut = 8'd141;
            7'd86: pitchenv_rate_lut = 8'd147;
            7'd87: pitchenv_rate_lut = 8'd153;
            7'd88: pitchenv_rate_lut = 8'd159;
            7'd89: pitchenv_rate_lut = 8'd165;
            7'd90: pitchenv_rate_lut = 8'd171;
            7'd91: pitchenv_rate_lut = 8'd178;
            7'd92: pitchenv_rate_lut = 8'd185;
            7'd93: pitchenv_rate_lut = 8'd193;
            7'd94: pitchenv_rate_lut = 8'd202;
            7'd95: pitchenv_rate_lut = 8'd211;
            7'd96: pitchenv_rate_lut = 8'd232;
            7'd97: pitchenv_rate_lut = 8'd243;
            7'd98: pitchenv_rate_lut = 8'd254;
            default: pitchenv_rate_lut = 8'd255;
        endcase
    endfunction

    // pitchenv.cc:35-43 level table, indices 0..127 (signed 8-bit)
    function signed [7:0] pitchenv_tab_lut(input [6:0] i);
        case (i)
            7'd0:    pitchenv_tab_lut = -8'sd128;
            7'd1:    pitchenv_tab_lut = -8'sd116;
            7'd2:    pitchenv_tab_lut = -8'sd104;
            7'd3:    pitchenv_tab_lut = -8'sd95;
            7'd4:    pitchenv_tab_lut = -8'sd85;
            7'd5:    pitchenv_tab_lut = -8'sd76;
            7'd6:    pitchenv_tab_lut = -8'sd68;
            7'd7:    pitchenv_tab_lut = -8'sd61;
            7'd8:    pitchenv_tab_lut = -8'sd56;
            7'd9:    pitchenv_tab_lut = -8'sd52;
            7'd10:   pitchenv_tab_lut = -8'sd49;
            7'd11:   pitchenv_tab_lut = -8'sd46;
            7'd12:   pitchenv_tab_lut = -8'sd43;
            7'd13:   pitchenv_tab_lut = -8'sd41;
            7'd14:   pitchenv_tab_lut = -8'sd39;
            7'd15:   pitchenv_tab_lut = -8'sd37;
            7'd16:   pitchenv_tab_lut = -8'sd35;
            7'd17:   pitchenv_tab_lut = -8'sd33;
            7'd18:   pitchenv_tab_lut = -8'sd32;
            7'd19:   pitchenv_tab_lut = -8'sd31;
            7'd20:   pitchenv_tab_lut = -8'sd30;
            7'd21:   pitchenv_tab_lut = -8'sd29;
            7'd22:   pitchenv_tab_lut = -8'sd28;
            7'd23:   pitchenv_tab_lut = -8'sd27;
            7'd24:   pitchenv_tab_lut = -8'sd26;
            7'd25:   pitchenv_tab_lut = -8'sd25;
            7'd26:   pitchenv_tab_lut = -8'sd24;
            7'd27:   pitchenv_tab_lut = -8'sd23;
            7'd28:   pitchenv_tab_lut = -8'sd22;
            7'd29:   pitchenv_tab_lut = -8'sd21;
            7'd30:   pitchenv_tab_lut = -8'sd20;
            7'd31:   pitchenv_tab_lut = -8'sd19;
            7'd32:   pitchenv_tab_lut = -8'sd18;
            7'd33:   pitchenv_tab_lut = -8'sd17;
            7'd34:   pitchenv_tab_lut = -8'sd16;
            7'd35:   pitchenv_tab_lut = -8'sd15;
            7'd36:   pitchenv_tab_lut = -8'sd14;
            7'd37:   pitchenv_tab_lut = -8'sd13;
            7'd38:   pitchenv_tab_lut = -8'sd12;
            7'd39:   pitchenv_tab_lut = -8'sd11;
            7'd40:   pitchenv_tab_lut = -8'sd10;
            7'd41:   pitchenv_tab_lut = -8'sd9;
            7'd42:   pitchenv_tab_lut = -8'sd8;
            7'd43:   pitchenv_tab_lut = -8'sd7;
            7'd44:   pitchenv_tab_lut = -8'sd6;
            7'd45:   pitchenv_tab_lut = -8'sd5;
            7'd46:   pitchenv_tab_lut = -8'sd4;
            7'd47:   pitchenv_tab_lut = -8'sd3;
            7'd48:   pitchenv_tab_lut = -8'sd2;
            7'd49:   pitchenv_tab_lut = -8'sd1;
            7'd50:   pitchenv_tab_lut = 8'sd0;
            7'd51:   pitchenv_tab_lut = 8'sd1;
            7'd52:   pitchenv_tab_lut = 8'sd2;
            7'd53:   pitchenv_tab_lut = 8'sd3;
            7'd54:   pitchenv_tab_lut = 8'sd4;
            7'd55:   pitchenv_tab_lut = 8'sd5;
            7'd56:   pitchenv_tab_lut = 8'sd6;
            7'd57:   pitchenv_tab_lut = 8'sd7;
            7'd58:   pitchenv_tab_lut = 8'sd8;
            7'd59:   pitchenv_tab_lut = 8'sd9;
            7'd60:   pitchenv_tab_lut = 8'sd10;
            7'd61:   pitchenv_tab_lut = 8'sd11;
            7'd62:   pitchenv_tab_lut = 8'sd12;
            7'd63:   pitchenv_tab_lut = 8'sd13;
            7'd64:   pitchenv_tab_lut = 8'sd14;
            7'd65:   pitchenv_tab_lut = 8'sd15;
            7'd66:   pitchenv_tab_lut = 8'sd16;
            7'd67:   pitchenv_tab_lut = 8'sd17;
            7'd68:   pitchenv_tab_lut = 8'sd18;
            7'd69:   pitchenv_tab_lut = 8'sd19;
            7'd70:   pitchenv_tab_lut = 8'sd20;
            7'd71:   pitchenv_tab_lut = 8'sd21;
            7'd72:   pitchenv_tab_lut = 8'sd22;
            7'd73:   pitchenv_tab_lut = 8'sd23;
            7'd74:   pitchenv_tab_lut = 8'sd24;
            7'd75:   pitchenv_tab_lut = 8'sd25;
            7'd76:   pitchenv_tab_lut = 8'sd26;
            7'd77:   pitchenv_tab_lut = 8'sd27;
            7'd78:   pitchenv_tab_lut = 8'sd28;
            7'd79:   pitchenv_tab_lut = 8'sd29;
            7'd80:   pitchenv_tab_lut = 8'sd30;
            7'd81:   pitchenv_tab_lut = 8'sd31;
            7'd82:   pitchenv_tab_lut = 8'sd32;
            7'd83:   pitchenv_tab_lut = 8'sd33;
            7'd84:   pitchenv_tab_lut = 8'sd34;
            7'd85:   pitchenv_tab_lut = 8'sd35;
            7'd86:   pitchenv_tab_lut = 8'sd38;
            7'd87:   pitchenv_tab_lut = 8'sd40;
            7'd88:   pitchenv_tab_lut = 8'sd43;
            7'd89:   pitchenv_tab_lut = 8'sd46;
            7'd90:   pitchenv_tab_lut = 8'sd49;
            7'd91:   pitchenv_tab_lut = 8'sd53;
            7'd92:   pitchenv_tab_lut = 8'sd58;
            7'd93:   pitchenv_tab_lut = 8'sd65;
            7'd94:   pitchenv_tab_lut = 8'sd73;
            7'd95:   pitchenv_tab_lut = 8'sd82;
            7'd96:   pitchenv_tab_lut = 8'sd92;
            7'd97:   pitchenv_tab_lut = 8'sd103;
            7'd98:   pitchenv_tab_lut = 8'sd115;
            default: pitchenv_tab_lut = 8'sd127;
        endcase
    endfunction

    function [7:0] bsel(input [31:0] w, input [1:0] i);
        case (i)
            2'd0:    bsel = w[7:0];
            2'd1:    bsel = w[15:8];
            2'd2:    bsel = w[23:16];
            default: bsel = w[31:24];
        endcase
    endfunction

    // PITCHENV_TAB[l] << 19 with explicit sign extension (pitchenv.cc:81-89)
    function signed [31:0] peg_tab19(input [6:0] lvl);
        reg signed [7:0] t;
        begin
            t = pitchenv_tab_lut(lvl);
            peg_tab19 = t * 32'sd524288;        // t << 19, sign-extended
        end
    endfunction

    // PITCHENV_RATE[r] * unit_ (pitchenv.cc:81-89)
    function [31:0] peg_rate_inc(input [7:0] r);
        peg_rate_inc = PEG_UNIT * {24'b0, pitchenv_rate_lut(r[6:0])};
    endfunction

    // ------------------------------------------------------------------
    // LFO param decode wires (lfo.cc:31-49): pure functions of the active
    // params; recomputed at commit. All multiplies stay in-domain, so the
    // truncated-step Verilog arithmetic equals the model's single-trunc
    // integer semantics (mod-2^32 equality of sum/shift chains).
    // ------------------------------------------------------------------
    // during S_COMMIT the active image is being swapped in; decode the
    // incoming (shadow) params in that state, the active ones otherwise
    wire [31:0] lfop_src = (phase == S_COMMIT) ? shd_lfop : act_lfop;
    wire [7:0]  w_rate  = lfop_src[7:0];
    wire [7:0]  w_delay = lfop_src[15:8];
    wire [2:0]  w_wave  = lfop_src[26:24];
    wire        w_sync  = (lfop_src[23:16] != 8'd0);
    wire [15:0] sr1     = (w_rate == 8'd0) ? 16'd1
                                        : ((165 * w_rate) >> 6);
    wire [15:0] srfac   = (sr1 < 16'd160) ? 16'd11
                                          : (16'd11 + ((sr1 - 16'd160) >> 4));
    wire [31:0] w_delta = LFO_UNIT * (sr1 * srfac);
    wire [7:0]  a0      = 8'd99 - w_delay;
    wire [15:0] a1      = (16 + (a0 & 8'd15)) << (1 + (a0 >> 4));
    wire [15:0] a2      = ((a1 & 16'hFF80) < 16'h80) ? 16'h80
                                                     : (a1 & 16'hFF80);
    wire [31:0] w_dinc1 = (a0 == 8'd99) ? 32'hFFFFFFFF : (LFO_UNIT * a1);
    wire [31:0] w_dinc2 = (a0 == 8'd99) ? 32'hFFFFFFFF : (LFO_UNIT * a2);

    // ------------------------------------------------------------------
    // LFO step combinational (lfo.cc:51-90), from the post-advance phase
    // ------------------------------------------------------------------
    wire [31:0] ph_n      = lfo_phase + lfo_delta;
    wire [23:0] tri_v     = ph_n[31] ? ~ph_n[30:7] : ph_n[30:7];
    wire [23:0] sawdn_v   = ((~ph_n) ^ 32'h80000000) >> 8;
    wire [23:0] sawup_v   = (ph_n ^ 32'h80000000) >> 8;
    wire [24:0] square_v  = ph_n[31] ? 25'd0 : 25'd16777216;
    // sine branch: Sin::lookup(phase_ >> 8), SIN_DELTA table
    wire [23:0] p8        = ph_n[31:8];
    wire [10:0] spi       = {p8[23:14], 1'b0};   // (p8>>13) & 2046, even
    wire [13:0] slowbits  = p8[13:0];
    wire signed [25:0] s_dy  = $signed(sin_rom[spi]);
    wire signed [25:0] s_y0  = $signed(sin_rom[spi + 11'd1]);
    wire signed [39:0] s_prod = s_dy * $signed({26'b0, slowbits});
    wire signed [31:0] s_val  = s_y0 + $signed(s_prod >>> 14);
    wire [24:0] sine_v = 32'sd8388608 + (s_val >>> 1);  // i32 wrap; >= 0
    // sample & hold: LCG advance on the wrap step (lfo.cc:69-74)
    wire        wrap_step = (ph_n < lfo_delta);
    wire [7:0]  rs_hit    = (lfo_rand * 8'd179) + 8'd17;
    wire [7:0]  rand_nx   = wrap_step ? rs_hit : lfo_rand;
    wire [8:0]  shplus    = {1'b0, (rand_nx ^ 8'h80)} + 9'd1;
    wire [24:0] sh_v      = shplus << 16;
    wire [24:0] wave_v =
        (w_wave == 3'd0) ? {1'b0, tri_v}   :
        (w_wave == 3'd1) ? {1'b0, sawdn_v} :
        (w_wave == 3'd2) ? {1'b0, sawup_v} :
        (w_wave == 3'd3) ? square_v        :
        (w_wave == 3'd4) ? sine_v          :
        (w_wave == 3'd5) ? sh_v            : 25'd8388608;
    // delay ramp (lfo.cc:78-90, uint32/uint64 arithmetic)
    wire [31:0] dsel    = lfo_dstate[31] ? lfo_dinc2 : lfo_dinc1;
    wire [32:0] dsum    = {1'b0, lfo_dstate} + {1'b0, dsel};
    wire [24:0] dly_sat = 25'd16777216;          // 1<<24, state NOT committed
    wire [24:0] dly_nrm = dsum[31] ? {1'b0, dsum[30:7]} : 25'd0;

    // ------------------------------------------------------------------
    // Pitch-EG step combinational (pitchenv.cc:55-89)
    // ------------------------------------------------------------------
    wire        peg_act = (peg_ix < 3'd3) || ((peg_ix < 3'd4) && !peg_down);
    wire [31:0] peg_up  = peg_lvl + peg_inc;     // i32 wrap
    wire [31:0] peg_dn  = peg_lvl - peg_inc;
    wire        up_hit  = $signed(peg_up) >= $signed(peg_tgt);
    wire        dn_hit  = $signed(peg_dn) <= $signed(peg_tgt);
    wire [31:0] peg_nxt = !peg_act                 ? peg_lvl  :
                          (peg_rising && up_hit) ? peg_tgt  :
                          (peg_rising)           ? peg_up   :
                          (dn_hit)               ? peg_tgt  : peg_dn;
    wire        peg_adv = peg_act && (peg_rising ? up_hit : dn_hit);

    // ------------------------------------------------------------------
    // PM / AM composition combinational (dx7note.cc:210-217, 259-268)
    // ------------------------------------------------------------------
    wire [31:0] pmd32 = act_pmdep * {7'b0, lfo_del_q};          // u32 trunc
    wire signed [31:0] slfo    = $signed({7'b0, lfo_val_q}) - 32'sd8388608;
    wire signed [31:0] senslfo = $signed(act_pmsens) * slfo;    // i32 wrap
    wire signed [63:0] prod1   = $signed({1'b0, pmd32}) * senslfo;
    wire signed [63:0] sh1     = prod1 >>> 39;
    wire [31:0] pm1  = sh1[31:0];
    wire [31:0] pm1a = pm1[31] ? (~pm1 + 32'd1) : pm1;          // abs
    wire signed [63:0] prod2   = $signed(ctrl_pm) * senslfo;
    wire signed [63:0] sh2     = prod2 >>> 14;
    wire [31:0] pm2  = sh2[31:0];
    wire [31:0] pm2a = pm2[31] ? (~pm2 + 32'd1) : pm2;          // abs
    wire [31:0] pmax   = ($signed(pm1a) > $signed(pm2a)) ? pm1a : pm2a;
    wire [31:0] pmsign = senslfo[31] ? (32'd0 - pmax) : pmax;
    wire [31:0] pbase  = bend_pb + master_tune;                 // i32 wrap
    // AM depth composition (dx7note.cc:259-268)
    wire [31:0] lv_r   = 32'h01000000 - {7'b0, lfo_val_q};
    wire [63:0] am1p   = {32'b0, act_amdep} * {32'b0, lfo_del_q};
    wire [31:0] am1    = am1p[39:8];                            // (..)>>8, u32
    wire [63:0] am1bp  = {32'b0, am1} * {32'b0, lv_r};
    wire [31:0] am1b   = am1bp[55:24];                          // (..)>>24, u32
    wire signed [63:0] am2p = $signed(ctrl_am) * $signed({1'b0, lv_r});
    wire [31:0] am2    = (am2p >>> 7);                          // (..)>>7, u32
    wire [31:0] amax   = (am1b > am2) ? am1b : am2;
    wire [31:0] a3     = (eg_mod + 32'd1) << 17;
    wire [31:0] ambase = 32'h01000000 - a3;
    wire [31:0] amd_n  = (ambase > amax) ? ambase : amax;

    // ------------------------------------------------------------------
    // Freqlut stage combinational (freqlut.cc:46-55)
    // ------------------------------------------------------------------
    wire [31:0] fmode_sel = basepitch_q[opix] +
        (opmode_q[opix] ? pitch_base_q
                        : (pm_nobase_q + pitch_base_q));        // i32 wraps
    wire signed [8:0] fsh_s = 9'sd20 - $signed(fmode_sel[31:24]);
    wire [8:0] fsh_n = fsh_s[8] ? 9'd0 : fsh_s;  // hibits > 20 unreachable
    wire [31:0] fy0 = freqlut_rom[faddr_q];
    wire [31:0] fy1 = freqlut_rom[faddr_q + 11'd1];  // 11-bit: index 1024
    wire signed [31:0] fdiff = $signed(fy1) - $signed(fy0);
    wire signed [63:0] fprod = fdiff * $signed({50'b0, flsb_q});
    wire [31:0] fy = fy0 + fprod[45:14];                        // i32 wrap
    wire [31:0] fval_raw = $signed(fy) >>> fsh_q;

    // ------------------------------------------------------------------
    // Basepitch latch combinational (at note-on; dx7note.cc:38-64)
    // ------------------------------------------------------------------
    wire signed [31:0] note_eff = $signed({24'b0, cmd_note}) +
                                  $signed({24'b0, act_trans}) - 32'sd24;
    wire signed [31:0] lprime   = $signed(A4_BASE) +
                                  $signed(SEMI_LOGFREQ) * note_eff;
    wire [31:0] bp_fixed0 = (32'd4458616 *
        ((act_opcfg[opix][9:8] * 16'd100) + act_opcfg[opix][23:16])) >> 3;
    wire [31:0] bp_fixed1 = (act_opcfg[opix][31:24] > 8'd7)
        ? bp_fixed0 + 32'd13457 * (act_opcfg[opix][31:24] - 8'd7)
        : bp_fixed0;
`ifdef MUTATE_FIXED_TRACKS_KEYBOARD
    // NEGATIVE CONTROL: fixed mode tracks the keyboard (N05 named control).
    wire [31:0] bp_fixed = lprime + bp_fixed1;
`else
    wire [31:0] bp_fixed = bp_fixed1;
`endif
    wire [31:0] bp_ratio0 = note_logfreq_q + pend_delta[opix];  // i32 wrap
    wire [31:0] bp_ratio1 = bp_ratio0 + coarsemul_lut(act_opcfg[opix][12:8]);
    wire [31:0] bp_ratio  = act_opcfg[opix][23:16]
                                ? (bp_ratio1 + act_fterm[opix])
                                : bp_ratio1;

    // ------------------------------------------------------------------
    // Sequential logic
    // ------------------------------------------------------------------
    integer i;
    always @(posedge clk) begin
        if (rst) begin
            for (i = 0; i < 6; i = i + 1) begin
                shd_opcfg[i]  <= 32'd0;  shd_fterm[i]  <= 32'd0;
                act_opcfg[i]  <= 32'd0;  act_fterm[i]  <= 32'd0;
                pend_delta[i] <= 32'd0;  basepitch_q[i] <= 32'd0;
                freq_q[i]     <= 32'd0;
            end
            shd_pegr <= 32'd0;  shd_pegl <= 32'd0;   shd_lfop <= 32'd0;
            shd_pmdep <= 32'd0; shd_pmsens <= 32'd0; shd_trans <= 8'd0;
            shd_amdep <= 32'd0;
            act_pegr <= 32'd0;  act_pegl <= 32'd0;   act_lfop <= 32'd0;
            act_pmdep <= 32'd0; act_pmsens <= 32'd0; act_trans <= 8'd0;
            act_amdep <= 32'd0;
            bend_pb <= 32'd0;   master_tune <= 32'd0;
            ctrl_pm <= 32'd0;   ctrl_am <= 32'd0;
            eg_mod <= 32'd127;  // pinned Controllers::refresh() default
            pend_commit <= 1'b0; pend_note <= 2'd0; cmd_note <= 8'd0;
            note_logfreq_q <= 32'd0; opmode_q <= 6'd0;
            lfo_phase <= 32'd0;  lfo_delta <= 32'd0;  lfo_dstate <= 32'd0;
            lfo_dinc1 <= 32'd0;  lfo_dinc2 <= 32'd0;  lfo_rand <= 8'd0;
            lfo_val_q <= 25'd0;  lfo_del_q <= 25'd0;
            peg_lvl <= 32'd0;    peg_tgt <= 32'd0;    peg_inc <= 32'd0;
            peg_rising <= 1'b0;  peg_down <= 1'b1;    peg_ix <= 3'd0;
            peg_out_q <= 32'd0;
            pm_nobase_q <= 32'd0; pitch_base_q <= 32'd0; amd_q <= 32'd0;
            faddr_q <= 10'd0; flsb_q <= 14'd0; fsh_q <= 9'd0;
            out_valid_q <= 1'b0;
            phase <= S_IDLE; opix <= 3'd0; obs_q <= 1'b0;
        end else begin
            // contracted write port: shadow / payload / performance
            if (cfg_wr) begin
                case (cfg_addr)
                    8'h00: begin
                        case (cfg_wdata[7:0])
                            CMD_NOTE_ON: begin
                                pend_note  <= 2'd1;
                                cmd_note   <= cfg_wdata[15:8];
                            end
                            CMD_NOTE_OFF: pend_note <= 2'd2;
                            CMD_COMMIT:   pend_commit <= 1'b1;
                            default: begin
                                pend_note   <= 2'd0;
                                pend_commit <= 1'b0;
                            end
                        endcase
                    end
                    8'h20: shd_pegr   <= cfg_wdata;
                    8'h21: shd_pegl   <= cfg_wdata;
                    8'h22: shd_lfop   <= cfg_wdata;
                    8'h23: shd_pmdep  <= cfg_wdata;
                    8'h24: shd_pmsens <= cfg_wdata;
                    8'h25: shd_trans  <= cfg_wdata[7:0];
                    8'h26: shd_amdep  <= cfg_wdata;
                    8'h28: bend_pb    <= cfg_wdata;
                    8'h29: master_tune <= cfg_wdata;
                    8'h2A: ctrl_pm    <= cfg_wdata;
                    8'h2B: ctrl_am    <= cfg_wdata;
                    8'h2C: eg_mod     <= cfg_wdata;
                    default: ;
                endcase
                if (cfg_addr >= 8'h10 && cfg_addr <= 8'h15)
                    shd_opcfg[cfg_addr[2:0]] <= cfg_wdata;
                if (cfg_addr >= 8'h18 && cfg_addr <= 8'h1D)
                    shd_fterm[cfg_addr[2:0]] <= cfg_wdata;
                if (cfg_addr >= 8'h30 && cfg_addr <= 8'h35)
                    pend_delta[cfg_addr[2:0]] <= cfg_wdata;
            end

            case (phase)
                S_IDLE: if (frame_tick) begin
                    // dispatch: a pending commit runs FIRST in its own
                    // state so the note event reads the committed image
                    if (pend_commit)
                        phase <= S_COMMIT;
                    else if (pend_note == 2'd1)
                        phase <= S_NN;
                    else if (pend_note == 2'd2)
                        phase <= S_NO;
                    else
                        phase <= S_LFO;
                end
                S_COMMIT: begin
                    // atomic patch commit (DEC-019): shadow -> active
                    for (i = 0; i < 6; i = i + 1) begin
                        act_opcfg[i] <= shd_opcfg[i];
                        act_fterm[i] <= shd_fterm[i];
                    end
                    act_pegr   <= shd_pegr;   act_pegl <= shd_pegl;
                    act_lfop   <= shd_lfop;   act_pmdep <= shd_pmdep;
                    act_pmsens <= shd_pmsens; act_trans <= shd_trans;
                    act_amdep  <= shd_amdep;
                    lfo_delta  <= w_delta;   // lfo.cc:31-49 params decoded
                    lfo_dinc1  <= w_dinc1;   // from the incoming image;
                    lfo_dinc2  <= w_dinc2;   // phase/rand/dstate are NOT
                    pend_commit <= 1'b0;     // restarted (DEC-020)
                    phase <= (pend_note == 2'd1) ? S_NN :
                             (pend_note == 2'd2) ? S_NO : S_LFO;
                end
                S_NN: begin // note-on: PitchEnv.set + LFO keydown
                    note_logfreq_q <= lprime;
                    opmode_q <= {act_opcfg[5][7:0] != 8'd0,
                                 act_opcfg[4][7:0] != 8'd0,
                                 act_opcfg[3][7:0] != 8'd0,
                                 act_opcfg[2][7:0] != 8'd0,
                                 act_opcfg[1][7:0] != 8'd0,
                                 act_opcfg[0][7:0] != 8'd0};
                    peg_lvl <= peg_tab19(bsel(act_pegl, 2'd3));
                    peg_tgt <= peg_tab19(bsel(act_pegl, 2'd0));
                    peg_rising <= peg_tab19(bsel(act_pegl, 2'd0)) >
                                  peg_tab19(bsel(act_pegl, 2'd3));
                    peg_inc <= peg_rate_inc(bsel(act_pegr, 2'd0));
                    peg_ix <= 3'd0;
                    peg_down <= 1'b1;
`ifdef MUTATE_LFO_SYNC_IGNORED
                    // NEGATIVE CONTROL: keydown keeps phase_
                    lfo_phase <= lfo_phase;
`else
                    lfo_phase <= w_sync ? 32'h7FFFFFFF : lfo_phase;
`endif
                    lfo_dstate <= 32'd0;
                    pend_note <= 2'd0;
                    opix <= 3'd0;
                    phase <= S_NNB;
                end
                S_NNB: begin  // basepitch per op (dx7note.cc:38-64)
                    basepitch_q[opix] <= opmode_q[opix] ? bp_fixed
                                                        : bp_ratio;
                    if (opix == 3'd5) begin
                        opix  <= 3'd0;
                        phase <= S_LFO;
                    end else
                        opix <= opix + 3'd1;
                end
                S_NO: begin // note-off: keydown(false), advance(3)
                    if (peg_down) begin
                        peg_down <= 1'b0;
                        peg_ix <= 3'd3;
                        peg_tgt <= peg_tab19(bsel(act_pegl, 2'd3));
                        peg_rising <= peg_tab19(bsel(act_pegl, 2'd3)) >
                                      $signed(peg_lvl);
                        peg_inc <= peg_rate_inc(bsel(act_pegr, 2'd3));
                    end
                    pend_note <= 2'd0;
                    phase <= S_LFO;
                end
                S_LFO: begin  // one getsample + one getdelay per frame
                    lfo_phase <= ph_n;
                    lfo_rand  <= (w_wave == 3'd5) ? rand_nx : lfo_rand;
                    lfo_val_q <= wave_v;
                    if (dsum[32]) begin
                        lfo_del_q <= dly_sat;
                    end else begin
                        lfo_dstate <= dsum[31:0];
                        lfo_del_q  <= dly_nrm;
                    end
                    phase <= S_PEG;
                end
                S_PEG: begin  // one pitchenv getsample per frame
                    peg_out_q <= peg_nxt;
                    peg_lvl   <= peg_nxt;
                    if (peg_adv) begin
                        peg_ix <= peg_ix + 3'd1;
                        peg_tgt <= peg_tab19(bsel(act_pegl, peg_ix + 3'd1));
                        peg_rising <= peg_tab19(bsel(act_pegl, peg_ix + 3'd1)) >
                                      $signed(peg_nxt);
                        peg_inc <= peg_rate_inc(bsel(act_pegr, peg_ix + 3'd1));
                    end
                    phase <= S_PM;
                end
                S_PM: begin  // dx7note.cc:210-217 + AM composition
                    pm_nobase_q  <= peg_out_q + pmsign;
                    pitch_base_q <= pbase;
                    amd_q        <= amd_n;
                    opix         <= 3'd0;
                    phase        <= S_FA;
                end
                S_FA: begin  // index/shift capture for the current op
                    faddr_q <= fmode_sel[23:14];
                    flsb_q  <= fmode_sel[13:0];
                    fsh_q   <= fsh_n;
                    phase   <= S_FB;
                end
                S_FB: begin  // interpolate + final shift, latch increment
`ifdef H06_PER_SAMPLE_HOST
                    // NEGATIVE CONTROL: illegal per-sample host gating
                    if (host_stream_valid)
`endif
`ifdef MUTATE_INCREMENT_LSB
                    // NEGATIVE CONTROL: op-0 increment +1 LSB every frame
                    freq_q[opix] <= (opix == 3'd0) ? (fval_raw + 32'd1)
                                                   : fval_raw;
`else
                    freq_q[opix] <= fval_raw;
`endif
                    if (opix == 3'd5) begin
                        opix  <= 3'd0;
                        phase <= S_OUT;
                    end else begin
                        opix  <= opix + 3'd1;
                        phase <= S_FA;
                    end
                end
                S_OUT: begin
                    out_valid_q <= 1'b1;
                    phase <= S_FIN;
                end
                S_FIN: begin
                    out_valid_q <= 1'b0;
                    phase <= S_IDLE;
                end
                default: phase <= S_IDLE;
            endcase

            // observability is load-bearing: registered XOR fold over every
            // state bit (H01/H02 pattern); stripping it drops mapped flops.
            obs_next = 1'b0;
            for (i = 0; i < 6; i = i + 1) begin
                obs_next = obs_next ^ (^shd_opcfg[i]) ^ (^shd_fterm[i])
                         ^ (^act_opcfg[i]) ^ (^act_fterm[i])
                         ^ (^pend_delta[i]) ^ (^basepitch_q[i])
                         ^ (^freq_q[i]);
            end
            obs_next = obs_next
                     ^ (^shd_pegr) ^ (^shd_pegl) ^ (^shd_lfop)
                     ^ (^shd_pmdep) ^ (^shd_pmsens) ^ (^shd_amdep)
                     ^ (^shd_trans)
                     ^ (^act_pegr) ^ (^act_pegl) ^ (^act_lfop)
                     ^ (^act_pmdep) ^ (^act_pmsens) ^ (^act_amdep)
                     ^ (^act_trans)
                     ^ (^bend_pb) ^ (^master_tune) ^ (^ctrl_pm) ^ (^ctrl_am)
                     ^ (^eg_mod)
                     ^ pend_commit ^ (^pend_note) ^ (^cmd_note)
                     ^ (^note_logfreq_q) ^ (^opmode_q)
                     ^ (^lfo_phase) ^ (^lfo_delta) ^ (^lfo_dstate)
                     ^ (^lfo_dinc1) ^ (^lfo_dinc2) ^ (^lfo_rand)
                     ^ (^lfo_val_q) ^ (^lfo_del_q)
                     ^ (^peg_lvl) ^ (^peg_tgt) ^ (^peg_inc)
                     ^ peg_rising ^ peg_down ^ (^peg_ix) ^ (^peg_out_q)
                     ^ (^pm_nobase_q) ^ (^pitch_base_q) ^ (^amd_q)
                     ^ (^faddr_q) ^ (^flsb_q) ^ (^fsh_q)
                     ^ (^phase) ^ (^opix) ^ out_valid_q;
            obs_q <= obs_next;
        end
    end
`ifdef H06_STRIP_OBSERVABILITY
    // NEGATIVE CONTROL: no output observable, no reduction -- synthesis
    // must (and does) delete every state flop; the flop-count gate then
    // FAILS. This build exists only to prove the gate has resolution.
    assign freq0 = 32'h0;
    assign freq1 = 32'h0;
    assign freq2 = 32'h0;
    assign freq3 = 32'h0;
    assign freq4 = 32'h0;
    assign freq5 = 32'h0;
    assign peg_level = 32'h0;
    assign lfo_val = 25'h0;
    assign lfo_delay = 25'h0;
    assign amd_mod = 32'h0;
    assign out_valid = 1'b0;
    assign busy = 1'b0;
    assign state_obs = 1'b0;
`else
    assign freq0 = freq_q[0];
    assign freq1 = freq_q[1];
    assign freq2 = freq_q[2];
    assign freq3 = freq_q[3];
    assign freq4 = freq_q[4];
    assign freq5 = freq_q[5];
    assign peg_level = peg_out_q;
    assign lfo_val = lfo_val_q;
    assign lfo_delay = lfo_del_q;
    assign amd_mod = amd_q;
    assign out_valid = out_valid_q;
    assign busy = (phase != S_IDLE);
    assign state_obs = obs_q;
`endif

endmodule

`default_nettype wire
