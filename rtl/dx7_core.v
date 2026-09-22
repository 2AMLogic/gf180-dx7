// dx7_core.v -- H07 integrated 16-note polyphonic PCM core (issue #29).
//
// The complete gf180-dx7 core: SPI slave command link (48-bit frames per
// docs/CONTRACT-CORE-v1.md section 4.2), skid FIFO + continuous drain,
// patch shadow/commit, ordered event queue, voice allocation (D00 DEC-015
// through DEC-021), 96 operator-envelope slices, a shared per-note pitch
// path (pitch-EG, chip-global LFO, PM/AM composition, Freqlut), the H05
// algorithm router around the H01 Mark I operator kernel, the NUM-010
// unclamped mixer, and the I2S master output.  RTL-exact to the FROZEN
// N06+N07 integer model (src/gf180_dx7/model/{integrated,poly}.py,
// contract gf180-dx7-integer-v1): every output sample is the exact integer
// mix the model produces, at the H03 declared latency.
//
// COMPOSITION OF THE LANDED MODULES (and the documented deviations):
//   * rtl/alg_router.v (H05) is instantiated as-is: per-note renders are
//     time-multiplexed through its ld_cfg/ld_fb/ld_op/start/readout
//     interface; its persistent interconnect buses are shared across notes
//     exactly like the model's single AlgorithmRouter instance (N07
//     isolation by has_contents).
//   * the operator eval seam: H05 documents (docs/H05-ROUTING-RTL.md
//     uncertainty 2) that the router consumes eval_y combinationally in
//     the same cycle as eval_req.  The H01 kernel is therefore reused
//     WITHOUT its 3-stage registers: h07_op_kernel below is the H01
//     stage-1/2/3 combinational logic (same pinned ROM bytes, same
//     fold/add/shift chain) lifted from rtl/operator_probe.v, keeping the
//     measured 1 eval/clk frame cost.  operator_probe.v itself is
//     unchanged and its H01 evidence stands.
//   * rtl/pitch_mod.v (H06) is the single-note PROBE of the pitch path.
//     It cannot be instanced 16x: the frozen model's LFO is chip-global
//     (one shared LFO keyed down on EVERY strike and reconfigured by patch
//     selection, DEC-019/DEC-020) while pitch-EG state, basepitch and the
//     patch-derived depths are per-note and bound to the patch generation
//     a note was struck on.  The core therefore integrates the per-note
//     pitch path as shared time-multiplexed logic over the note banks;
//     every frozen constant/table/formula is identical to
//     rtl/pitch_mod.v and src/gf180_dx7/model/pitch.py.
//   * rtl/env_unit.v (H04) is instantiated 96x (one envelope slice per
//     note operator), driven through its command interface.  The H07
//     note-on derivation compensates env_unit's 6-bit rate_scaling field
//     exactly (docs/H07-CORE.md section 4): for scale_rate results >= 64
//     the glue sends rs' = 63 with per-segment rates r' = min(r+rs, 99)-
//     63, which reproduces the model's qrate/staticrate arithmetic
//     identically on every path env_unit exposes.
//   * TRANSPOSE: the frozen N06/N07 model binds NotePitch with
//     transpose_mode="pinned_wrapper" (transpose ignored; registry finding
//     perf-transpose-12), and N08 is the H07 conformance oracle, so the
//     core derives l' = midinote_to_logfreq(midinote) with NO transpose
//     shift.  The H06 unit-level documented-mode ruling is recorded in
//     docs/H07-CORE.md as the reconciliation of the two contracts.
//
// FRAME SCHEDULE (as-built; the H03 section-2 budget is the planning
// basis and the measured utilization is reported in docs/H07-CORE.md):
//   clk = 24.576 MHz design point, 512 clk/sample, 64 samples/frame,
//   32768 clk/frame.  Free-running sample counters derive BCLK = clk/4
//   and LRCLK = clk/256.  Compute of frame N runs inside the wire window
//   of frame N into a double-buffered output; the I2S streams the previous
//   frame.  An event received during wire frame N commits at the tick,
//   affects first the compute of N+1 and reaches SDATA in wire frame N+2
//   -- inside the H03 latency window [(64-p), (64-p)+65] samples.
//
//   tick: patch commit (atomic, DEC-019), then the queued events in
//         arrival order (note_on/note_off/cc/pb + per-note frozen detune
//         deltas + performance integers, DEC-014/NUM-011); voice
//         allocation with DEC-016 same-pitch reuse, DEC-017 steal
//         (release preference, then oldest) and DEC-015 retrigger;
//   control tail: per-frame envelope steps (parallel across the 96
//         env_unit slices), shared LFO step, then per live note: pitch-EG
//         step, PM/AM composition, per-op amp-mod application (the
//         NUM-008 float-gated exp via a high-precision fixed-point
//         evaluator) and six Freqlut lookups;
//   eval pass: per live note in slot order: router load, one 64-sample
//         render (1 eval/clk), per-sample >>4 / clip / >>9 conversion
//         into the unclamped >=20-bit mix accumulator (NUM-010), state
//         readback (phase/gain_out/fb history), NUM-012 exact-silence
//         census; reclaim pass at the frame end.
//
// I2S OUTPUT: BCLK = clk/4, LRCLK = clk/256, data delayed 1 BCLK after
// each LRCLK edge, left-justified MSB first, the same sample duplicated
// on both slots (DEC-008).  DEVIATION (recorded finding, issue #29): the
// data field is 24 b carrying the full 21-bit unclamped mix (NUM-010
// freezes the PCM as the >=20-bit integer sum; the required ev-sum-15
// vector peaks at +43181 = 1.3176..., which no 16-bit field can carry
// bit-exactly).  A 16-bit DAC slice is an H08 owner decision; the
// conformance transport here is the exact mix value.
//
// OBSERVABILITY IS LOAD-BEARING (H01/H02/H03 4.6 pattern): all state is
// visible through the block taps or the registered XOR fold state_obs.
// NEGATIVE CONTROLS (each must demonstrably fail its check; issue #29):
//   -DH07_MUTATE_GLITCH      one-sample glitch injected in the mix path
//                            (frame 5 sample 17 gets +1); conformance
//                            must fail and the comparator must localize
//                            the sample.
//   -DH07_MUTATE_SUM_ORDER   mix pipeline order swapped: the clip is
//                            applied to the SUM (sum-then-clip) instead of
//                            per voice (frozen clip-then-sum) -- the N08
//                            voice_sum_order_swap mutation, which ev-sum-15
//                            detects.
//   -DH07_MUTATE_TICK_SKEW   stale schedule: event application pops the
//                            queue with a one-cycle-stale read pointer, so
//                            the last write of a burst window is lost --
//                            event-timing cases must fail.
//   -DH07_STRIP_OBSERVABILITY all outputs tied off; yosys deletes every
//                            state flop and the flop gate MUST FAIL.
// Mutant builds are conformance-only: never synthesize them into any
// area claim.
//
// This file claims no P&R, no timing closure, no fit, no board bring-up
// and no original-DX7 fidelity: bit-exactness to the frozen model is the
// bar, and docs/H07-CORE.md records exactly what was measured.
`default_nettype none

`ifndef H07_SINLOG_HEX
`define H07_SINLOG_HEX "reference/tables/sinlog_table.hex"
`endif
`ifndef H07_SINEXP_HEX
`define H07_SINEXP_HEX "reference/tables/sinexp_table.hex"
`endif
`ifndef H07_ALG_HEX
`define H07_ALG_HEX "reference/tables/alg_table.hex"
`endif
`ifndef H07_FREQLUT_HEX
`define H07_FREQLUT_HEX "reference/tables/freqlut_table.hex"
`endif

// ---------------------------------------------------------------------------
// h07_op_kernel: the H01 Mark I operator arithmetic as a combinational
// eval seam (see header; adapted from rtl/operator_probe.v stage 1-3
// logic, same pinned ROM bytes, same frozen arithmetic).
// ---------------------------------------------------------------------------
module h07_op_kernel (
    input  wire [31:0] phase,   // phase + mod (router eval_phase_in)
    input  wire [15:0] env,     // 14-bit frame gain, uint16 domain
    output wire [31:0] out_raw  // mkiSin int32 result
);
    reg [15:0] sinlog_rom [0:1023];
    reg [15:0] sinexp_rom [0:1023];
    initial begin
        $readmemh(`H07_SINLOG_HEX, sinlog_rom);
        $readmemh(`H07_SINEXP_HEX, sinexp_rom);
    end

    wire [15:0] phi       = phase[27:12];
    wire [9:0]  sl_addr   = phi[9:0] ^ {10{phi[10]}};
    wire [15:0] sl_data   = sinlog_rom[sl_addr];
    wire [15:0] sl_signed = sl_data | (phi[11] ? 16'h8000 : 16'h0000);

    wire [15:0] exp_val16 = sl_signed + env;
    wire        ex_sign   = exp_val16[15];
    wire [14:0] exp_val15 = exp_val16[14:0];
    wire [9:0]  ex_addr   = ~exp_val15[9:0];
    wire [4:0]  ex_shift  = exp_val15[14:10];
    wire [15:0] ex_data   = sinexp_rom[ex_addr];

    wire [12:0] result13  = 13'd4096 + {1'b0, ex_data};
    wire [12:0] result_sh = result13 >> ex_shift;
    wire [31:0] res32     = {19'b0, result_sh};
    wire [31:0] out_pos   = res32 << 13;
    wire [31:0] out_neg   = (~res32) << 13;
    assign out_raw = ex_sign ? out_neg : out_pos;
endmodule

// ---------------------------------------------------------------------------
// dx7_core
// ---------------------------------------------------------------------------
module dx7_core (
    input  wire        clk,
    input  wire        rst_n,        // pad reset, active low (two-flop sync)
    // SPI slave (mode 0, SCK <= clk/4)
    input  wire        spi_sck,
    input  wire        spi_mosi,
    input  wire        spi_cs_n,
    output wire        spi_miso,
    // I2S master output
    output wire        i2s_bclk,
    output wire        i2s_lrclk,
    output wire        i2s_d,
    // block taps (read-only, non-invasive; load-bearing observability)
    output wire [15:0] tap_frame,
    output wire [15:0] tap_live,
    output wire [22:0] tap_mix,      // sample at the I2S load boundary
    output wire        tap_mix_valid,
    output wire        status_overrun,
    output wire        status_overflow,
    output wire        state_obs
);

    // =====================================================================
    // Reset synchronizer (H03 4.4: RST_N through two flops)
    // =====================================================================
    reg rst_ff1, rst_ff2;
    always @(posedge clk) begin
        if (!rst_n) begin
            rst_ff1 <= 1'b1;
            rst_ff2 <= 1'b1;
        end else begin
            rst_ff1 <= 1'b0;
            rst_ff2 <= rst_ff1;
        end
    end
    wire rst = rst_ff2;   // synchronous, active high, core-wide

    // =====================================================================
    // Free-running frame/sample timing
    // =====================================================================
    reg [8:0]  sample_clk;    // 0..511 within a sample
    reg [5:0]  sample_idx;    // 0..63 within a frame
    reg [15:0] frame_ctr;
    wire       frame_boundary = (sample_clk == 9'd511) &&
                                (sample_idx == 6'd63);
    always @(posedge clk) begin
        if (rst) begin
            sample_clk <= 9'd0;
            sample_idx <= 6'd0;
            frame_ctr  <= 16'd0;
        end else if (sample_clk == 9'd511) begin
            sample_clk <= 9'd0;
            if (sample_idx == 6'd63) begin
                sample_idx <= 6'd0;
                frame_ctr  <= frame_ctr + 16'd1;
            end else
                sample_idx <= sample_idx + 6'd1;
        end else
            sample_clk <= sample_clk + 9'd1;
    end
    wire frame_tick = frame_boundary;   // the compute frame starts here

    // =====================================================================
    // SPI slave: two-flop syncs, 48-bit frames (H03 4.2/4.3)
    // =====================================================================
    reg sck_s1, sck_s2, sck_d;
    reg mosi_s1, mosi_s2;
    reg cs_s1, cs_s2, cs_d;
    always @(posedge clk) begin
        sck_s1  <= spi_sck;   sck_s2  <= sck_s1;  sck_d <= sck_s2;
        mosi_s1 <= spi_mosi;  mosi_s2 <= mosi_s1;
        cs_s1   <= spi_cs_n;  cs_s2   <= cs_s1;   cs_d  <= cs_s2;
    end
    wire sck_rise = sck_s2 & ~sck_d;
    wire sck_fall = ~sck_s2 & sck_d;
    wire cs_fall  = cs_s2 & ~cs_d;

    reg        st_overrun;    // sticky defect detector: must never set
    reg        st_overflow;   // sticky defect detector (DEC-014)
    reg        st_fresh;      // no write accepted since (pad) reset

    // skid FIFO (depth 8 x 49 b: 48-bit frame + valid, H03 4.2)
    reg [48:0] skid [0:7];
    reg [2:0]  skid_wp, skid_rp;
    reg [3:0]  skid_cnt;
    wire       skid_nonempty = (skid_cnt != 4'd0);
    wire       skid_full     = (skid_cnt == 4'd8);
    wire       skid_pop;

    wire [31:0] status_real = {8'hD7, 4'h1, st_overrun, skid_nonempty,
                               st_overflow, st_fresh, frame_ctr};

    reg [31:0] miso_sh;
    always @(posedge clk) begin
        if (rst)
            miso_sh <= 32'h0;
        else if (cs_fall)
            miso_sh <= status_real;
        else if (sck_fall && !cs_s2)
            miso_sh <= {miso_sh[30:0], 1'b0};
    end
    assign spi_miso = miso_sh[31];

    reg [47:0] shift_in;
    reg [5:0]  bit_cnt;
    reg [47:0] frame_reg;
    reg        frame_reg_v;
    wire [47:0] frame_captured = {shift_in[46:0], mosi_s2};
    always @(posedge clk) begin
        if (rst) begin
            shift_in    <= 48'd0;
            bit_cnt     <= 6'd0;
            frame_reg   <= 48'd0;
            frame_reg_v <= 1'b0;
        end else begin
            frame_reg_v <= 1'b0;
            if (cs_fall) begin
                bit_cnt <= 6'd0;
            end else if (!cs_s2 && sck_rise) begin
                if (bit_cnt == 6'd47) begin
                    frame_reg   <= frame_captured;
                    frame_reg_v <= 1'b1;
                    bit_cnt     <= 6'd0;
                end else begin
                    shift_in <= frame_captured;
                    bit_cnt  <= bit_cnt + 6'd1;
                end
            end
        end
    end

    // decoded write fields: {F, 6'b0, SEC, A[7:0], D[31:0]}
    wire        wr_f   = frame_reg[47];
    wire        wr_sec = frame_reg[40];
    wire [7:0]  wr_a   = frame_reg[39:32];
    wire [31:0] wr_d   = frame_reg[31:0];
    wire        wr_wr  = frame_reg_v & wr_f;      // F=1: write
    wire        skid_push = wr_wr;

    always @(posedge clk) begin
        if (rst) begin
            skid_wp <= 3'd0; skid_rp <= 3'd0; skid_cnt <= 4'd0;
            st_overflow <= 1'b0;
            st_fresh    <= 1'b1;
        end else begin
            if (wr_wr)
                st_fresh <= 1'b0;   // any accepted write unfreshes
            if (skid_push) begin
                if (skid_full)
                    st_overflow <= 1'b1;   // explicit rejection (DEC-014)
                else begin
                    skid[skid_wp] <= frame_reg;
                    skid_wp  <= skid_wp + 3'd1;
                    skid_cnt <= skid_cnt + 4'd1;
                end
            end
            if (skid_pop) begin
                skid_rp  <= skid_rp + 3'd1;
                skid_cnt <= skid_cnt - 4'd1;
            end
        end
    end
    wire [48:0] skid_out = skid[skid_rp];

    // =====================================================================
    // Continuous drain: skid -> patch shadow (SEC=1) / event queue (SEC=0)
    // =====================================================================
    assign skid_pop = (skid_cnt != 4'd0);

    // ---- patch shadow image (SEC=1) --------------------------------------
    // 0x10..0x15 OP_PITCH[k]  {detune, fine, coarse, mode}
    // 0x18..0x1D OP_FTERM[k]  int32 (host-frozen fine term, dx7note.cc:50-53)
    // 0x20..0x25 OP_ENV_RATES[k] {r3,r2,r1,r0}   (param k's env block)
    // 0x28..0x2D OP_ENV_LEVELS[k] {l3,l2,l1,l0}
    // 0x30..0x35 OP_SCALE_A[k] {ldep[7:0],rdep[7:0],lcur[1:0],rcur[1:0],
    //                           kvs[2:0],bp[7:0]}
    // 0x38..0x3D OP_SCALE_B[k] {rs[7:0], outlevel[7:0]}
    // 0x40 GLOBAL_A {transpose[7:0], pm_sens_src[2:0], am_depth[7:0],
    //                pm_depth[7:0]}
    // 0x41 GLOBAL_B {wave[2:0], sync, delay[7:0], rate[7:0], okeysync,
    //                feedback[2:0], algorithm[4:0]}
    // 0x43 GLOBAL_C {ams[0],ams[1],ams[2],ams[3],ams[4],ams[5]} (2 b each)
    // 0x42 COMMIT (any write commits at the next frame tick, DEC-019)
    reg [31:0] shd_opitch  [0:5];
    reg [31:0] shd_fterm   [0:5];
    reg [31:0] shd_erates  [0:5];
    reg [31:0] shd_elevels [0:5];
    reg [31:0] shd_scaleA  [0:5];
    reg [31:0] shd_scaleB  [0:5];
    reg [31:0] shd_globA, shd_globB, shd_globC;
    reg [31:0] shd_pegr, shd_pegl;
    reg        shd_commit;
    integer si;

    // ---- event queue (SEC=0): ordered, applied at the tick (DEC-014) -----
    // depth 168 = the worst admissible per-frame write count at SCK =
    // clk/4 (floor(32768/194)); overflow is a defect at this bound.
    localparam EVQ_DEPTH = 168;
    reg [36:0] evq [0:EVQ_DEPTH-1];   // {addr[4:0], data[31:0]}
    reg [7:0]  evq_wp, evq_rp;
    reg [8:0]  evq_cnt;
    wire       evq_full  = (evq_cnt == EVQ_DEPTH);
    wire       evq_empty = (evq_cnt == 9'd0);

    wire        dr_sec  = skid_out[40];
    wire [7:0]  dr_a    = skid_out[39:32];
    wire [31:0] dr_d    = skid_out[31:0];
    always @(posedge clk) begin
        if (rst) begin
            for (si = 0; si < 6; si = si + 1) begin
                shd_opitch[si]  <= 32'd0;  shd_fterm[si]  <= 32'd0;
                shd_erates[si]  <= 32'd0;  shd_elevels[si] <= 32'd0;
                shd_scaleA[si]  <= 32'd0;  shd_scaleB[si] <= 32'd0;
            end
            shd_globA <= 32'd0;  shd_globB <= 32'd0;  shd_globC <= 32'd0;
            shd_pegr <= 32'd0;   shd_pegl <= 32'd0;
            shd_commit <= 1'b0;
            evq_wp <= 8'd0; evq_rp <= 8'd0; evq_cnt <= 9'd0;
        end else if (skid_pop) begin
            if (dr_sec) begin
                if (dr_a == 8'h42) begin
                    shd_commit <= 1'b1;
                end else begin
                    case (dr_a)
                        8'h40: shd_globA <= dr_d;
                        8'h41: shd_globB <= dr_d;
                        8'h43: shd_globC <= dr_d;
                        8'h44: shd_pegr   <= dr_d;
                        8'h45: shd_pegl   <= dr_d;
                        default: ;
                    endcase
                    if (dr_a >= 8'h10 && dr_a <= 8'h15)
                        shd_opitch[dr_a[2:0]] <= dr_d;
                    if (dr_a >= 8'h18 && dr_a <= 8'h1D)
                        shd_fterm[dr_a[2:0]] <= dr_d;
                    if (dr_a >= 8'h20 && dr_a <= 8'h25)
                        shd_erates[dr_a[2:0]] <= dr_d;
                    if (dr_a >= 8'h28 && dr_a <= 8'h2D)
                        shd_elevels[dr_a[2:0]] <= dr_d;
                    if (dr_a >= 8'h30 && dr_a <= 8'h35)
                        shd_scaleA[dr_a[2:0]] <= dr_d;
                    if (dr_a >= 8'h38 && dr_a <= 8'h3D)
                        shd_scaleB[dr_a[2:0]] <= dr_d;
                end
            end else begin
                if (evq_full)
                    st_overflow <= 1'b1;
                else begin
                    evq[evq_wp] <= {dr_a[4:0], dr_d};
                    evq_wp  <= (evq_wp == EVQ_DEPTH-1) ? 8'd0 : evq_wp + 8'd1;
                    evq_cnt <= evq_cnt + 9'd1;
                end
            end
        end
    end
`ifdef H07_MUTATE_TICK_SKEW
    // NEGATIVE CONTROL: stale schedule. Event application observes the
    // queue state one cycle late, so the final write of a burst window is
    // dropped. Event-timing conformance must fail on this build.
    reg        evq_peek_empty_d;
    always @(posedge clk) evq_peek_empty_d <= evq_empty | skid_pop;
    wire       evq_peek_empty = evq_peek_empty_d;
`else
    wire       evq_peek_empty = evq_empty;
`endif
    wire [36:0] evq_out = evq[evq_rp];
    wire [4:0]  ev_a    = evq_out[36:32];
    wire [31:0] ev_d    = evq_out[31:0];

    // event addresses (page 0)
    localparam [4:0] EA_NOTEON = 5'h00, EA_NOTEOFF = 5'h01, EA_CC = 5'h02,
                     EA_PB = 5'h03, EA_DD0 = 5'h04, EA_MTUNE = 5'h0B,
                     EA_CTRLPM = 5'h0C, EA_CTRLAM = 5'h0D, EA_EGMOD = 5'h0E,
                     EA_SOFT = 5'h0F;

    // =====================================================================
    // Committed patch image + shared performance state
    // =====================================================================
    reg [31:0] cmt_opitch  [0:5];
    reg [31:0] cmt_fterm   [0:5];
    reg [31:0] cmt_erates  [0:5];
    reg [31:0] cmt_elevels [0:5];
    reg [31:0] cmt_scaleA  [0:5];
    reg [31:0] cmt_scaleB  [0:5];
    reg [31:0] cmt_globA, cmt_globB, cmt_globC;
    reg [31:0] cmt_pegr, cmt_pegl;
    wire [31:0] shd_pegr_w = cmt_pegr;
    wire [31:0] shd_pegl_w = cmt_pegl;
    wire [31:0] cmt_pegr_w = cmt_pegr;
    wire [31:0] cmt_pegl_w = cmt_pegl;
    // performance state (DEC-022: separate, applied at event time)
    reg signed [31:0] bend_pb;
    reg signed [31:0] master_tune, ctrl_pm, ctrl_am;
    reg        [31:0] eg_mod;

    // committed-image field decode
    wire [7:0] c_mode   [0:5];
    wire [7:0] c_coarse [0:5];
    wire [7:0] c_fine   [0:5];
    wire [7:0] c_detune [0:5];
    genvar gk;
    generate
        for (gk = 0; gk < 6; gk = gk + 1) begin : c_decode
            assign c_mode[gk]   = cmt_opitch[gk][7:0];
            assign c_coarse[gk] = cmt_opitch[gk][15:8];
            assign c_fine[gk]   = cmt_opitch[gk][23:16];
            assign c_detune[gk] = cmt_opitch[gk][31:24];
        end
    endgenerate
    wire [2:0] c_pmsens_idx = cmt_globA[2:0];    // byte0 & 7 (patch[143])
    // forward declaration for the frame-controller commit cycle
    wire        fc_commit;
    wire [7:0] c_amdep_src  = cmt_globA[15:8];
    wire [7:0] c_pmdep_src  = cmt_globA[23:16];
    wire [7:0] c_transpose  = cmt_globA[31:24];  // stored; NOT applied
                                                 // (pinned_wrapper finding)
    wire [7:0]  c_lfo_delay = cmt_globB[15:8];
    wire [7:0]  c_lfo_rate  = cmt_globB[7:0];
    wire        c_lfo_sync  = cmt_globB[16];
    wire [2:0]  c_lfo_wave  = cmt_globB[19:17];
    wire        c_okeysync  = cmt_globB[20];
    wire [2:0]  c_feedback  = cmt_globB[23:21];
    wire [4:0]  c_algorithm = cmt_globB[28:24];
    wire [1:0] c_ams0 = cmt_globC[1:0];
    wire [1:0] c_ams1 = cmt_globC[3:2];
    wire [1:0] c_ams2 = cmt_globC[5:4];
    wire [1:0] c_ams3 = cmt_globC[7:6];
    wire [1:0] c_ams4 = cmt_globC[9:8];
    wire [1:0] c_ams5 = cmt_globC[11:10];
    // =====================================================================
    // Per-note state (the H02 note-bank slice, flop-based)
    // =====================================================================
    reg  [15:0] n_live, n_keydown, n_sustained, n_reclaimed, n_used;
    reg  [6:0]  n_midi    [0:15];
    reg  [6:0]  n_vel     [0:15];
    reg  [7:0]  n_seq     [0:15];
    reg  [6:0]  n_sil     [0:15];    // NUM-012 consecutive silence frames
    reg  [15:0] n_silnow;            // this frame's exact-silence census
    reg  [3:0]  cur_note;            // allocation rotation
    reg  [7:0]  seq_ctr;             // newest sequence (age order)
    reg         sustain;
    // bound patch-derived params (DEC-019: fixed at the striking note-on)
    reg [31:0] n_bp      [0:15][0:5];
    reg [5:0]  n_opmode  [0:15];
    reg [4:0]  n_alg     [0:15];
    reg [4:0]  n_fbs     [0:15];
    reg [7:0]  n_pmdep   [0:15];
    reg [7:0]  n_pmsens  [0:15];
    reg [7:0]  n_amdep   [0:15];
    reg [24:0] n_ams     [0:15][0:5];
    reg [6:0]  n_pegr    [0:15][0:3];
    reg [6:0]  n_pegl    [0:15][0:3];
    // pitch-EG runtime state (pitchenv.cc)
    reg [31:0] n_peglvl  [0:15];
    reg [31:0] n_pegtgt  [0:15];
    reg [31:0] n_peginc  [0:15];
    reg [2:0]  n_pegix   [0:15];
    reg        n_pegris  [0:15];
    reg        n_pegdwn  [0:15];
    // per-note frozen detune deltas (note-on payload, NUM-008)
    reg [31:0] n_dd      [0:15][0:5];
    reg [31:0] stg_dd    [0:5];

    // per-op render state (phase/gain_out carried; freq/level_in per frame)
    reg [31:0] op_ph  [0:15][0:5];
    reg [15:0] op_go  [0:15][0:5];
    reg [31:0] op_fq  [0:15][0:5];
    reg [31:0] op_li  [0:15][0:5];   // signed level_in
    reg [31:0] n_fb0  [0:15];
    reg [31:0] n_fb1  [0:15];
    reg        refresh_pending;      // wrapper refreshVoice (first block)

    // =====================================================================
    // env_unit slices x96 (H04 command interface)
    // =====================================================================
    wire [95:0]  env_busy, env_done;
    reg  [95:0]  env_cmd_en;
    reg  [2:0]   env_cmd   [0:95];
    reg  [132:0] env_cmd_data [0:95];
    wire [132:0] env_rd [0:95];
    genvar ge;
    generate
        for (ge = 0; ge < 96; ge = ge + 1) begin : envs
            env_unit u_env (
                .clk(clk), .rst(rst),
                .cmd_en(env_cmd_en[ge]),
                .cmd(env_cmd[ge]),
                .cmd_data(env_cmd_data[ge]),
                .busy(env_busy[ge]),
                .done(env_done[ge]),
                .slot_wr_en(1'b0),
                .slot_wr_data(133'd0),
                .slot_rd_data(env_rd[ge]),
                .param_rd(),
                .state_obs()
            );
        end
    endgenerate
    // CMD opcodes (must match rtl/env_unit.v)
    localparam [2:0] EC_PARAM = 3'd0, EC_INIT = 3'd1, EC_KEYDOWN = 3'd2,
                     EC_UPDATE = 3'd3, EC_STEP = 3'd4, EC_ADV = 3'd5,
                     EC_LOAD = 3'd6;

    // env_unit state readback decode: {level, target, inc, static,
    //                                  ix[2:0], rising, down}
    function [31:0] env_level(input [132:0] s); env_level = s[132:101]; endfunction
    function [31:0] env_target(input [132:0] s); env_target = s[100:69]; endfunction
    function [2:0]  env_ix(input [132:0] s); env_ix = s[4:2]; endfunction
    function        env_down(input [132:0] s); env_down = s[0]; endfunction

    // =====================================================================
    // Shared LFO (chip-global; lfo.cc semantics identical to pitch_mod.v)
    // =====================================================================
    reg [31:0] lfo_phase, lfo_delta, lfo_dstate, lfo_dinc1, lfo_dinc2;
    reg [7:0]  lfo_rand;
    reg [24:0] lfo_val_q, lfo_del_q;

    localparam [31:0] LFO_UNIT = 32'd33587;   // lfo.cc:26-29 @48k
    // lfo.cc:31-49 param decode; during the commit cycle decode the
    // incoming (shadow) params, otherwise the committed ones
    wire [7:0]  l_dec_rate  = fc_commit ? shd_globB[7:0]   : c_lfo_rate;
    wire [7:0]  l_dec_delay = fc_commit ? shd_globB[15:8]  : c_lfo_delay;
    wire        l_dec_sync  = fc_commit ? shd_globB[16] : c_lfo_sync;
    wire [2:0]  l_dec_wave  = fc_commit ? shd_globB[19:17] : c_lfo_wave;
    wire [15:0] l_sr1   = (l_dec_rate == 8'd0) ? 16'd1
                                               : ((165 * l_dec_rate) >> 6);
    wire [15:0] l_srfac = (l_sr1 < 16'd160) ? 16'd11
                          : (16'd11 + ((l_sr1 - 16'd160) >> 4));
    wire [31:0] l_delta_w = LFO_UNIT * (l_sr1 * l_srfac);
    wire [7:0]  l_a0    = 8'd99 - l_dec_delay;
    wire [15:0] l_a1    = (16 + (l_a0 & 8'd15)) << (1 + (l_a0 >> 4));
    wire [15:0] l_a2    = ((l_a1 & 16'hFF80) < 16'h80) ? 16'h80
                                                       : (l_a1 & 16'hFF80);
    wire [31:0] l_dinc1_w = (l_a0 == 8'd99) ? 32'hFFFFFFFF : (LFO_UNIT * l_a1);
    wire [31:0] l_dinc2_w = (l_a0 == 8'd99) ? 32'hFFFFFFFF : (LFO_UNIT * l_a2);

    // LFO step combinational (post-advance phase), identical to
    // pitch_mod.v EXCEPT wave 4, which mirrors the frozen model's
    // PinnedWrapperLfo (integrated.py:109-130): the pinned wrapper's
    // State.init never calls Sin::init(), so its LFO sine waveform
    // outputs the constant 1 << 23 (finding 8). The phase register still
    // advances exactly like the reference.
    wire [31:0] l_ph_n    = lfo_phase + lfo_delta;
    wire [23:0] l_tri     = l_ph_n[31] ? ~l_ph_n[30:7] : l_ph_n[30:7];
    wire [23:0] l_sawdn   = ((~l_ph_n) ^ 32'h80000000) >> 8;
    wire [23:0] l_sawup   = (l_ph_n ^ 32'h80000000) >> 8;
    wire [24:0] l_square  = l_ph_n[31] ? 25'd0 : 25'd16777216;
    wire        l_wrap    = (l_ph_n < lfo_delta);
    wire [7:0]  l_rs_hit  = (lfo_rand * 8'd179) + 8'd17;
    wire [7:0]  l_rand_nx = l_wrap ? l_rs_hit : lfo_rand;
    wire [8:0]  l_shplus  = {1'b0, (l_rand_nx ^ 8'h80)} + 9'd1;
    wire [24:0] l_sh_v    = l_shplus << 16;
    wire [24:0] l_wave_v  =
        (c_lfo_wave == 3'd0) ? {1'b0, l_tri}   :
        (c_lfo_wave == 3'd1) ? {1'b0, l_sawdn} :
        (c_lfo_wave == 3'd2) ? {1'b0, l_sawup} :
        (c_lfo_wave == 3'd3) ? l_square        :
        (c_lfo_wave == 3'd4) ? 25'd8388608     :
        (c_lfo_wave == 3'd5) ? l_sh_v          : 25'd8388608;
    wire [31:0] l_dsel    = lfo_dstate[31] ? lfo_dinc2 : lfo_dinc1;
    wire [32:0] l_dsum    = {1'b0, lfo_dstate} + {1'b0, l_dsel};
    wire [24:0] l_dly_sat = 25'd16777216;
    wire [24:0] l_dly_nrm = l_dsum[31] ? {1'b0, l_dsum[30:7]} : 25'd0;

    // =====================================================================
    // Freqlut ROM + pitch constants (freqlut.cc / tuning.cc / pitchenv.cc;
    // identical to pitch_mod.v and the frozen model)
    // =====================================================================
    reg [31:0] freqlut_rom [0:1024];
    initial begin
        $readmemh(`H07_FREQLUT_HEX, freqlut_rom);
    end
    localparam signed [31:0] A4_BASE      = 32'sd50857777;
    localparam signed [31:0] SEMI_LOGFREQ = 32'sd1398101;
    localparam        [31:0] PEG_UNIT     = 32'd1050;

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

    function [7:0] peg_rate_lut(input [6:0] i);
        case (i)
            7'd0:  peg_rate_lut = 8'd1;    7'd1:  peg_rate_lut = 8'd2;
            7'd2:  peg_rate_lut = 8'd3;    7'd3:  peg_rate_lut = 8'd3;
            7'd4:  peg_rate_lut = 8'd4;    7'd5:  peg_rate_lut = 8'd4;
            7'd6:  peg_rate_lut = 8'd5;    7'd7:  peg_rate_lut = 8'd5;
            7'd8:  peg_rate_lut = 8'd6;    7'd9:  peg_rate_lut = 8'd6;
            7'd10: peg_rate_lut = 8'd7;    7'd11: peg_rate_lut = 8'd7;
            7'd12: peg_rate_lut = 8'd8;    7'd13: peg_rate_lut = 8'd8;
            7'd14: peg_rate_lut = 8'd9;    7'd15: peg_rate_lut = 8'd9;
            7'd16: peg_rate_lut = 8'd10;   7'd17: peg_rate_lut = 8'd10;
            7'd18: peg_rate_lut = 8'd11;   7'd19: peg_rate_lut = 8'd11;
            7'd20: peg_rate_lut = 8'd12;   7'd21: peg_rate_lut = 8'd12;
            7'd22: peg_rate_lut = 8'd13;   7'd23: peg_rate_lut = 8'd13;
            7'd24: peg_rate_lut = 8'd14;   7'd25: peg_rate_lut = 8'd14;
            7'd26: peg_rate_lut = 8'd15;   7'd27: peg_rate_lut = 8'd16;
            7'd28: peg_rate_lut = 8'd16;   7'd29: peg_rate_lut = 8'd17;
            7'd30: peg_rate_lut = 8'd18;   7'd31: peg_rate_lut = 8'd18;
            7'd32: peg_rate_lut = 8'd19;   7'd33: peg_rate_lut = 8'd20;
            7'd34: peg_rate_lut = 8'd21;   7'd35: peg_rate_lut = 8'd22;
            7'd36: peg_rate_lut = 8'd23;   7'd37: peg_rate_lut = 8'd24;
            7'd38: peg_rate_lut = 8'd25;   7'd39: peg_rate_lut = 8'd26;
            7'd40: peg_rate_lut = 8'd27;   7'd41: peg_rate_lut = 8'd28;
            7'd42: peg_rate_lut = 8'd30;   7'd43: peg_rate_lut = 8'd31;
            7'd44: peg_rate_lut = 8'd33;   7'd45: peg_rate_lut = 8'd34;
            7'd46: peg_rate_lut = 8'd36;   7'd47: peg_rate_lut = 8'd37;
            7'd48: peg_rate_lut = 8'd38;   7'd49: peg_rate_lut = 8'd39;
            7'd50: peg_rate_lut = 8'd41;   7'd51: peg_rate_lut = 8'd42;
            7'd52: peg_rate_lut = 8'd44;   7'd53: peg_rate_lut = 8'd46;
            7'd54: peg_rate_lut = 8'd47;   7'd55: peg_rate_lut = 8'd49;
            7'd56: peg_rate_lut = 8'd51;   7'd57: peg_rate_lut = 8'd53;
            7'd58: peg_rate_lut = 8'd54;   7'd59: peg_rate_lut = 8'd56;
            7'd60: peg_rate_lut = 8'd58;   7'd61: peg_rate_lut = 8'd60;
            7'd62: peg_rate_lut = 8'd62;   7'd63: peg_rate_lut = 8'd64;
            7'd64: peg_rate_lut = 8'd66;   7'd65: peg_rate_lut = 8'd68;
            7'd66: peg_rate_lut = 8'd70;   7'd67: peg_rate_lut = 8'd72;
            7'd68: peg_rate_lut = 8'd74;   7'd69: peg_rate_lut = 8'd76;
            7'd70: peg_rate_lut = 8'd79;   7'd71: peg_rate_lut = 8'd82;
            7'd72: peg_rate_lut = 8'd85;   7'd73: peg_rate_lut = 8'd88;
            7'd74: peg_rate_lut = 8'd91;   7'd75: peg_rate_lut = 8'd94;
            7'd76: peg_rate_lut = 8'd98;   7'd77: peg_rate_lut = 8'd102;
            7'd78: peg_rate_lut = 8'd106;  7'd79: peg_rate_lut = 8'd110;
            7'd80: peg_rate_lut = 8'd115;  7'd81: peg_rate_lut = 8'd120;
            7'd82: peg_rate_lut = 8'd125;  7'd83: peg_rate_lut = 8'd130;
            7'd84: peg_rate_lut = 8'd135;  7'd85: peg_rate_lut = 8'd141;
            7'd86: peg_rate_lut = 8'd147;  7'd87: peg_rate_lut = 8'd153;
            7'd88: peg_rate_lut = 8'd159;  7'd89: peg_rate_lut = 8'd165;
            7'd90: peg_rate_lut = 8'd171;  7'd91: peg_rate_lut = 8'd178;
            7'd92: peg_rate_lut = 8'd185;  7'd93: peg_rate_lut = 8'd193;
            7'd94: peg_rate_lut = 8'd202;  7'd95: peg_rate_lut = 8'd211;
            7'd96: peg_rate_lut = 8'd232;  7'd97: peg_rate_lut = 8'd243;
            7'd98: peg_rate_lut = 8'd254;  default: peg_rate_lut = 8'd255;
        endcase
    endfunction

    function signed [7:0] peg_tab_lut(input [6:0] i);
        case (i)
            7'd0:    peg_tab_lut = -8'sd128; 7'd1:  peg_tab_lut = -8'sd116;
            7'd2:    peg_tab_lut = -8'sd104; 7'd3:  peg_tab_lut = -8'sd95;
            7'd4:    peg_tab_lut = -8'sd85;  7'd5:  peg_tab_lut = -8'sd76;
            7'd6:    peg_tab_lut = -8'sd68;  7'd7:  peg_tab_lut = -8'sd61;
            7'd8:    peg_tab_lut = -8'sd56;  7'd9:  peg_tab_lut = -8'sd52;
            7'd10:   peg_tab_lut = -8'sd49;  7'd11: peg_tab_lut = -8'sd46;
            7'd12:   peg_tab_lut = -8'sd43;  7'd13: peg_tab_lut = -8'sd41;
            7'd14:   peg_tab_lut = -8'sd39;  7'd15: peg_tab_lut = -8'sd37;
            7'd16:   peg_tab_lut = -8'sd35;  7'd17: peg_tab_lut = -8'sd33;
            7'd18:   peg_tab_lut = -8'sd32;  7'd19: peg_tab_lut = -8'sd31;
            7'd20:   peg_tab_lut = -8'sd30;  7'd21: peg_tab_lut = -8'sd29;
            7'd22:   peg_tab_lut = -8'sd28;  7'd23: peg_tab_lut = -8'sd27;
            7'd24:   peg_tab_lut = -8'sd26;  7'd25: peg_tab_lut = -8'sd25;
            7'd26:   peg_tab_lut = -8'sd24;  7'd27: peg_tab_lut = -8'sd23;
            7'd28:   peg_tab_lut = -8'sd22;  7'd29: peg_tab_lut = -8'sd21;
            7'd30:   peg_tab_lut = -8'sd20;  7'd31: peg_tab_lut = -8'sd19;
            7'd32:   peg_tab_lut = -8'sd18;  7'd33: peg_tab_lut = -8'sd17;
            7'd34:   peg_tab_lut = -8'sd16;  7'd35: peg_tab_lut = -8'sd15;
            7'd36:   peg_tab_lut = -8'sd14;  7'd37: peg_tab_lut = -8'sd13;
            7'd38:   peg_tab_lut = -8'sd12;  7'd39: peg_tab_lut = -8'sd11;
            7'd40:   peg_tab_lut = -8'sd10;  7'd41: peg_tab_lut = -8'sd9;
            7'd42:   peg_tab_lut = -8'sd8;   7'd43: peg_tab_lut = -8'sd7;
            7'd44:   peg_tab_lut = -8'sd6;   7'd45: peg_tab_lut = -8'sd5;
            7'd46:   peg_tab_lut = -8'sd4;   7'd47: peg_tab_lut = -8'sd3;
            7'd48:   peg_tab_lut = -8'sd2;   7'd49: peg_tab_lut = -8'sd1;
            7'd50:   peg_tab_lut = 8'sd0;    7'd51: peg_tab_lut = 8'sd1;
            7'd52:   peg_tab_lut = 8'sd2;    7'd53: peg_tab_lut = 8'sd3;
            7'd54:   peg_tab_lut = 8'sd4;    7'd55: peg_tab_lut = 8'sd5;
            7'd56:   peg_tab_lut = 8'sd6;    7'd57: peg_tab_lut = 8'sd7;
            7'd58:   peg_tab_lut = 8'sd8;    7'd59: peg_tab_lut = 8'sd9;
            7'd60:   peg_tab_lut = 8'sd10;   7'd61: peg_tab_lut = 8'sd11;
            7'd62:   peg_tab_lut = 8'sd12;   7'd63: peg_tab_lut = 8'sd13;
            7'd64:   peg_tab_lut = 8'sd14;   7'd65: peg_tab_lut = 8'sd15;
            7'd66:   peg_tab_lut = 8'sd16;   7'd67: peg_tab_lut = 8'sd17;
            7'd68:   peg_tab_lut = 8'sd18;   7'd69: peg_tab_lut = 8'sd19;
            7'd70:   peg_tab_lut = 8'sd20;   7'd71: peg_tab_lut = 8'sd21;
            7'd72:   peg_tab_lut = 8'sd22;   7'd73: peg_tab_lut = 8'sd23;
            7'd74:   peg_tab_lut = 8'sd24;   7'd75: peg_tab_lut = 8'sd25;
            7'd76:   peg_tab_lut = 8'sd26;   7'd77: peg_tab_lut = 8'sd27;
            7'd78:   peg_tab_lut = 8'sd28;   7'd79: peg_tab_lut = 8'sd29;
            7'd80:   peg_tab_lut = 8'sd30;   7'd81: peg_tab_lut = 8'sd31;
            7'd82:   peg_tab_lut = 8'sd32;   7'd83: peg_tab_lut = 8'sd33;
            7'd84:   peg_tab_lut = 8'sd34;   7'd85: peg_tab_lut = 8'sd35;
            7'd86:   peg_tab_lut = 8'sd38;   7'd87: peg_tab_lut = 8'sd40;
            7'd88:   peg_tab_lut = 8'sd43;   7'd89: peg_tab_lut = 8'sd46;
            7'd90:   peg_tab_lut = 8'sd49;   7'd91: peg_tab_lut = 8'sd53;
            7'd92:   peg_tab_lut = 8'sd58;   7'd93: peg_tab_lut = 8'sd65;
            7'd94:   peg_tab_lut = 8'sd73;   7'd95: peg_tab_lut = 8'sd82;
            7'd96:   peg_tab_lut = 8'sd92;   7'd97: peg_tab_lut = 8'sd103;
            7'd98:   peg_tab_lut = 8'sd115;  default: peg_tab_lut = 8'sd127;
        endcase
    endfunction

    function signed [31:0] peg_tab19(input [6:0] lvl);
        reg signed [7:0] t;
        begin
            t = peg_tab_lut(lvl);
            peg_tab19 = t * 32'sd524288;        // t << 19, sign-extended
        end
    endfunction

    function [31:0] peg_rate_inc(input [6:0] r);
        peg_rate_inc = PEG_UNIT * {24'b0, peg_rate_lut(r)};
    endfunction

    // =====================================================================
    // Note-on derivations (dx7note.cc:66-179; the envelope.py integer
    // model, applied at the striking note-on)
    // =====================================================================
    function [6:0] levellut(input [6:0] ol);
        case (ol)
            7'd0:  levellut = 7'd0;   7'd1:  levellut = 7'd5;
            7'd2:  levellut = 7'd9;   7'd3:  levellut = 7'd13;
            7'd4:  levellut = 7'd17;  7'd5:  levellut = 7'd20;
            7'd6:  levellut = 7'd23;  7'd7:  levellut = 7'd25;
            7'd8:  levellut = 7'd27;  7'd9:  levellut = 7'd29;
            7'd10: levellut = 7'd31;  7'd11: levellut = 7'd33;
            7'd12: levellut = 7'd35;  7'd13: levellut = 7'd37;
            7'd14: levellut = 7'd39;  7'd15: levellut = 7'd41;
            7'd16: levellut = 7'd42;  7'd17: levellut = 7'd43;
            7'd18: levellut = 7'd45;  7'd19: levellut = 7'd46;
            default: levellut = ol + 7'd28;   // >= 20: 28 + ol
        endcase
    endfunction

    function [7:0] vel_data(input [5:0] i);
        case (i)
            6'd0:  vel_data = 8'd0;   6'd1:  vel_data = 8'd70;
            6'd2:  vel_data = 8'd86;  6'd3:  vel_data = 8'd97;
            6'd4:  vel_data = 8'd106; 6'd5:  vel_data = 8'd114;
            6'd6:  vel_data = 8'd121; 6'd7:  vel_data = 8'd126;
            6'd8:  vel_data = 8'd132; 6'd9:  vel_data = 8'd138;
            6'd10: vel_data = 8'd142; 6'd11: vel_data = 8'd148;
            6'd12: vel_data = 8'd152; 6'd13: vel_data = 8'd156;
            6'd14: vel_data = 8'd160; 6'd15: vel_data = 8'd163;
            6'd16: vel_data = 8'd166; 6'd17: vel_data = 8'd170;
            6'd18: vel_data = 8'd173; 6'd19: vel_data = 8'd174;
            6'd20: vel_data = 8'd178; 6'd21: vel_data = 8'd181;
            6'd22: vel_data = 8'd184; 6'd23: vel_data = 8'd186;
            6'd24: vel_data = 8'd189; 6'd25: vel_data = 8'd190;
            6'd26: vel_data = 8'd194; 6'd27: vel_data = 8'd196;
            6'd28: vel_data = 8'd198; 6'd29: vel_data = 8'd200;
            6'd30: vel_data = 8'd202; 6'd31: vel_data = 8'd205;
            6'd32: vel_data = 8'd206; 6'd33: vel_data = 8'd209;
            6'd34: vel_data = 8'd211; 6'd35: vel_data = 8'd214;
            6'd36: vel_data = 8'd216; 6'd37: vel_data = 8'd218;
            6'd38: vel_data = 8'd220; 6'd39: vel_data = 8'd222;
            6'd40: vel_data = 8'd224; 6'd41: vel_data = 8'd225;
            6'd42: vel_data = 8'd227; 6'd43: vel_data = 8'd229;
            6'd44: vel_data = 8'd230; 6'd45: vel_data = 8'd232;
            6'd46: vel_data = 8'd233; 6'd47: vel_data = 8'd235;
            6'd48: vel_data = 8'd237; 6'd49: vel_data = 8'd238;
            6'd50: vel_data = 8'd240; 6'd51: vel_data = 8'd241;
            6'd52: vel_data = 8'd242; 6'd53: vel_data = 8'd243;
            6'd54: vel_data = 8'd244; 6'd55: vel_data = 8'd246;
            6'd56: vel_data = 8'd246; 6'd57: vel_data = 8'd248;
            6'd58: vel_data = 8'd249; 6'd59: vel_data = 8'd250;
            6'd60: vel_data = 8'd251; 6'd61: vel_data = 8'd252;
            6'd62: vel_data = 8'd253; default: vel_data = 8'd254;
        endcase
    endfunction

    function [7:0] exp_scale_data(input [5:0] i);
        case (i)
            6'd0:  exp_scale_data = 8'd0;   6'd1:  exp_scale_data = 8'd1;
            6'd2:  exp_scale_data = 8'd2;   6'd3:  exp_scale_data = 8'd3;
            6'd4:  exp_scale_data = 8'd4;   6'd5:  exp_scale_data = 8'd5;
            6'd6:  exp_scale_data = 8'd6;   6'd7:  exp_scale_data = 8'd7;
            6'd8:  exp_scale_data = 8'd8;   6'd9:  exp_scale_data = 8'd9;
            6'd10: exp_scale_data = 8'd11;  6'd11: exp_scale_data = 8'd14;
            6'd12: exp_scale_data = 8'd16;  6'd13: exp_scale_data = 8'd19;
            6'd14: exp_scale_data = 8'd23;  6'd15: exp_scale_data = 8'd27;
            6'd16: exp_scale_data = 8'd33;  6'd17: exp_scale_data = 8'd39;
            6'd18: exp_scale_data = 8'd47;  6'd19: exp_scale_data = 8'd56;
            6'd20: exp_scale_data = 8'd66;  6'd21: exp_scale_data = 8'd80;
            6'd22: exp_scale_data = 8'd94;  6'd23: exp_scale_data = 8'd110;
            6'd24: exp_scale_data = 8'd126; 6'd25: exp_scale_data = 8'd142;
            6'd26: exp_scale_data = 8'd158; 6'd27: exp_scale_data = 8'd174;
            6'd28: exp_scale_data = 8'd190; 6'd29: exp_scale_data = 8'd206;
            6'd30: exp_scale_data = 8'd222; 6'd31: exp_scale_data = 8'd238;
            default: exp_scale_data = 8'd250;
        endcase
    endfunction

    // dx7note.cc:128-130 PITCHMODSENSTAB
    function [7:0] pmsens_tab(input [2:0] i);
        case (i)
            3'd0: pmsens_tab = 8'd0;   3'd1: pmsens_tab = 8'd10;
            3'd2: pmsens_tab = 8'd20;  3'd3: pmsens_tab = 8'd33;
            3'd4: pmsens_tab = 8'd55;  3'd5: pmsens_tab = 8'd92;
            3'd6: pmsens_tab = 8'd153; default: pmsens_tab = 8'd255;
        endcase
    endfunction

    // dx7note.cc:135-137 AMP_MOD_SENS_TAB (Q24 sensitivity per op)
    function [24:0] ampsens_tab(input [1:0] i);
        case (i)
            2'd0: ampsens_tab = 25'd0;
            2'd1: ampsens_tab = 25'd4342338;
            2'd2: ampsens_tab = 25'd7171437;
            default: ampsens_tab = 25'd16777216;
        endcase
    endfunction

    // =====================================================================
    // NUM-008 amp-mod exp evaluator (dx7note.cc:288 float gate).
    // pt = int(exp(f32(x/262144)*0.07 + 12.2)), x = (amd*ams)>>24.
    // Domain declaration (docs/H07-CORE.md section 5): 0 <= x <= 2^24
    // (amd <= 2^24 and ams <= 2^24), so f32(x/2^18) is exact and y = x is
    // the exact U7.18 argument.  Constants are the exact float64 dyadic
    // values of 0.07, 12.2 and log2(e); 2^frac = table(i/64) * poly
    // (degree 5), relative error <= ~3e-15, so int(exp(arg)) matches the
    // model wherever exp(arg) is farther than ~1e-5 from an integer --
    // measured over the full conformance suite (and auditable domain-wide
    // via tools/h07_compare.py --audit-exp).
    // =====================================================================
    // exp unit: pt = int(exp(0.07*y/2^18 + 12.2)) for the pinned U7.18 y.
    // Fixed-point datapath (python-verified over the domain, 0 diff vs
    // the model's int(math.exp(f64 arg)) at 100k random + boundary
    // points): z = arg*log2(e); pt = 2^k * 2^(i/64) * 2^(rem/64) with
    // the last factor a degree-5 Taylor in rem*ln2/64 (<= 0.0109,
    // truncation error ~1e-16, far inside the min exp-to-integer
    // boundary distance 1.8e-7 measured over the domain).
    // =====================================================================
    localparam [55:0] EXP_M07  = 56'h11EB851EB851EC;  // 0.07 * 2^56
    localparam [53:0] EXP_M122 = 54'h18666666666666;  // 12.2 * 2^49
    localparam [52:0] EXP_MLG2 = 53'h171547652B82FE;  // log2(e) * 2^52
    localparam [55:0] EXP_LN2  = 56'hB17217F7D1CF79;  // ln(2) * 2^56

    function [61:0] powtab(input [5:0] i);   // 2^(i/64) in Q2.60
        case (i)
            6'd0:  powtab = 62'd1152921504606846976;
            6'd1:  powtab = 62'd1165475996665667840;
            6'd2:  powtab = 62'd1178167198179750912;
            6'd3:  powtab = 62'd1190996597817460736;
            6'd4:  powtab = 62'd1203965700457697024;
            6'd5:  powtab = 62'd1217076027366416896;
            6'd6:  powtab = 62'd1230329116375077120;
            6'd7:  powtab = 62'd1243726522061021184;
            6'd8:  powtab = 62'd1257269815929830144;
            6'd9:  powtab = 62'd1270960586599658752;
            6'd10: powtab = 62'd1284800439987579392;
            6'd11: powtab = 62'd1298790999497954816;
            6'd12: powtab = 62'd1312933906212861952;
            6'd13: powtab = 62'd1327230819084590336;
            6'd14: powtab = 62'd1341683415130235136;
            6'd15: powtab = 62'd1356293389628410624;
            6'd16: powtab = 62'd1371062456318104832;
            6'd17: powtab = 62'd1385992347599699712;
            6'd18: powtab = 62'd1401084814738180864;
            6'd19: powtab = 62'd1416341628068558592;
            6'd20: powtab = 62'd1431764577203528192;
            6'd21: powtab = 62'd1447355471243389440;
            6'd22: powtab = 62'd1463116138988252416;
            6'd23: powtab = 62'd1479048429152555008;
            6'd24: powtab = 62'd1495154210581915392;
            6'd25: powtab = 62'd1511435372472346368;
            6'd26: powtab = 62'd1527893824591856128;
            6'd27: powtab = 62'd1544531497504463360;
            6'd28: powtab = 62'd1561350342796650752;
            6'd29: powtab = 62'd1578352333306284032;
            6'd30: powtab = 62'd1595539463354025216;
            6'd31: powtab = 62'd1612913748977263872;
            6'd32: powtab = 62'd1630477228166597888;
            6'd33: powtab = 62'd1648231961104887552;
            6'd34: powtab = 62'd1666180030408913920;
            6'd35: powtab = 62'd1684323541373667584;
            6'd36: powtab = 62'd1702664622219298560;
            6'd37: powtab = 62'd1721205424340754944;
            6'd38: powtab = 62'd1739948122560140032;
            6'd39: powtab = 62'd1758894915381816576;
            6'd40: powtab = 62'd1778048025250290432;
            6'd41: powtab = 62'd1797409698810902016;
            6'd42: powtab = 62'd1816982207173354752;
            6'd43: powtab = 62'd1836767846178115584;
            6'd44: powtab = 62'd1856768936665714688;
            6'd45: powtab = 62'd1876987824748979456;
            6'd46: powtab = 62'd1897426882088230144;
            6'd47: powtab = 62'd1918088506169474816;
            6'd48: powtab = 62'd1938975120585633024;
            6'd49: powtab = 62'd1960089175320820480;
            6'd50: powtab = 62'd1981433147037730560;
            6'd51: powtab = 62'd2003009539368145408;
            6'd52: powtab = 62'd2024820883206609920;
            6'd53: powtab = 62'd2046869737007303424;
            6'd54: powtab = 62'd2069158687084144896;
            6'd55: powtab = 62'd2091690347914165248;
            6'd56: powtab = 62'd2114467362444183296;
            6'd57: powtab = 62'd2137492402400822528;
            6'd58: powtab = 62'd2160768168603901696;
            6'd59: powtab = 62'd2184297391283238400;
            6'd60: powtab = 62'd2208082830398904832;
            6'd61: powtab = 62'd2232127275964966656;
            6'd62: powtab = 62'd2256433548376752128;
            default: powtab = 62'd2281004498741680128;
        endcase
    endfunction

    // exp unit: start/done handshake, serialized (<= 96 uses per frame)
    reg         exp_start;
    wire        exp_done;
    reg  [24:0] exp_amd;        // Q24 AM depth
    reg  [24:0] exp_ams;        // Q24 sensitivity
    wire [31:0] exp_pt;
    reg  [3:0]  exp_st;
    reg  [25:0] exp_y;          // U7.18 argument (exact, <= 2^24)
    reg  [79:0] exp_t1;         // y * 0.07 * 2^56
    reg  [65:0] exp_arg;        // arg * 2^60 (Q7.60, <= 16.68)
    reg  [66:0] exp_zq;         // arg*log2(e) * 2^60
    reg  [5:0]  exp_k;          // floor(log2 exp(arg)) in [17, 24]
    reg  [61:0] exp_T;          // 2^(i/64) Q2.60
    reg  [55:0] exp_xq;         // rem*ln2/64 in Q0.56
    reg  [45:0] exp_p2;         // x^2 in Q0.56
    reg  [43:0] exp_p3;         // x^3 in Q0.56
    reg  [42:0] exp_p4;         // x^4 in Q0.56
    reg  [40:0] exp_p5;         // x^5 in Q0.56
    reg  [61:0] exp_res;        // 2^frac Q2.60
    reg  [31:0] exp_pt_q;
    wire [57:0] exp_xmul = exp_amd * exp_ams;          // <= 2^49
    wire [24:0] exp_x    = exp_xmul[48:24];            // >> 24 (<= 2^24)
    wire [79:0] exp_p1   = exp_y * EXP_M07;            // y*0.07*2^56
    wire [119:0] exp_zp  = exp_arg * {12'b0, EXP_MLG2};
    wire [109:0] exp_xp  = exp_zq[53:0] * EXP_LN2;     // rem*ln2, Q0.110
    wire [111:0] exp_p2w = exp_xq * exp_xq;            // x^2, Q0.112
    wire [99:0]  exp_p3w = exp_p2 * exp_xq;            // x^3, Q0.112
    wire [87:0]  exp_p4w = exp_p3 * exp_xq;            // x^4, Q0.112
    wire [81:0]  exp_p5w = exp_p4 * exp_xq;            // x^5, Q0.112
    wire [70:0]  exp_t3  = exp_p3 * 30'h2AAAAAAA;      // p3/6,  Q0.62
    wire [70:0]  exp_t4  = exp_p4 * 28'h0AAAAAAA;      // p4/24, Q0.60
    wire [65:0]  exp_t5  = exp_p5 * 26'h02222222;      // p5/120, Q0.58
    wire [56:0]  exp_hsum = (57'd1 << 56)
        + {1'b0, exp_xq}
        + {13'b0, exp_p2[44:1]}         // x^2/2, Q0.56
        + {13'b0, exp_t3[85:30]}        // x^3/6, Q0.56
        + {13'b0, exp_t4[85:30]}        // x^4/24, Q0.56
        + {13'b0, exp_t5[81:26]};       // x^5/120, Q0.56
    wire [117:0] exp_rp  = exp_T * {2'b0, exp_hsum};

    always @(posedge clk) begin
        if (rst) begin
            exp_st <= 4'd0; exp_pt_q <= 32'd0;
            exp_y <= 26'd0; exp_t1 <= 80'd0; exp_arg <= 66'd0;
            exp_zq <= 67'd0; exp_k <= 6'd0; exp_T <= 62'd0;
            exp_xq <= 56'd0; exp_p2 <= 45'd0; exp_p3 <= 39'd0;
            exp_p4 <= 43'd0; exp_p5 <= 41'd0;
            exp_res <= 63'd0;
        end else begin
            case (exp_st)
                4'd0: if (exp_start) begin
                    exp_y <= (exp_x > 25'd16777216) ? 25'd16777216 : exp_x;
                    exp_st <= 4'd1;
                end
                4'd1: begin exp_t1 <= exp_p1; exp_st <= 4'd2; end
                4'd2: begin
                    // arg*2^60 = (y*0.07*2^56) >> 14 + 12.2*2^60
                    //            (the U7.18 <<18 of y folded: 18-14 = 4)
                    exp_arg <= {1'b0, exp_t1[78:14]}
                             + {11'b0, EXP_M122, 11'b0};
                    exp_st <= 4'd3;
                end
                4'd3: begin exp_zq <= exp_zp[118:52]; exp_st <= 4'd4; end
                4'd4: begin
                    exp_k  <= exp_zq[65:60];
                    exp_T  <= powtab(exp_zq[59:54]);
                    exp_xq <= exp_xp[109:60];       // rem*ln2/64, Q0.56
                    exp_st <= 4'd5;
                end
                4'd5: begin exp_p2 <= exp_p2w[100:56]; exp_st <= 4'd6; end
                4'd6: begin exp_p3 <= exp_p3w[99:56]; exp_st <= 4'd7; end
                4'd7: begin exp_p4 <= exp_p4w[87:56]; exp_st <= 4'd8; end
                4'd8: begin exp_p5 <= exp_p5w[81:56]; exp_st <= 4'd9; end
                4'd9: begin exp_res <= exp_rp[117:56]; exp_st <= 4'd10; end
                4'd10: begin
                    // pt = 2^frac * 2^k: res(Q2.60) >> (60-k)
                    exp_pt_q <= exp_res >> (6'd60 - exp_k);
                    exp_st   <= 4'd11;
                end
                default: exp_st <= 4'd0;
            endcase
        end
    end
    assign exp_done = (exp_st == 4'd11);
    assign exp_pt = exp_pt_q;

    // =====================================================================
    // Frame controller: the H03 schedule (tick / events / tail / eval)
    // =====================================================================
    localparam [5:0]
        F_IDLE=0, F_COMMIT=1, F_EVPOP=2, F_ALLOC=3, F_NN_BASE=4,
        F_NN_ENVP=5, F_NN_ENVK=6, F_NN_ENVK2=7, F_NN_ENVK3=8, F_NN_PEG=9,
        F_NN_BOOK=10, F_OFFFIND=11, F_OFFAPPLY=12, F_PEDUP=13, F_ASO=14,
        F_ANO=15, F_SOFT=16, F_REFSEL=17, F_REFOP=18, F_REFOP2=19,
        F_ENVGO=20, F_ENVW=21, F_LFO=22, F_TAILSEL=23, F_TAILPEG=24,
        F_TAILPEG2=25, F_TAILPM=26, F_TAILAM=27, F_TAILFQ1=28,
        F_TAILFQ2=29, F_TAILFQ3=30, F_EVALSEL=31, F_EVALLD=32,
        F_EVALRUN=33, F_EVALMIX=34, F_EVALRB=35, F_RECLAIM=36,
        F_NN_BP=37, F_NN_BPW=38, F_NN_ENVI=39, F_NN_ENVK4=40,
        F_TAILEXP=41, F_TAILLI=42;

    reg [5:0]  fc_state;
    reg [3:0]  nn;             // active note (scan / target)
    reg [2:0]  opk;            // active operator
    reg [1:0]  kind;           // strike kind: 0 free, 1 retrigger
    reg [6:0]  stg_note, stg_vel;
    reg [7:0]  stg_ctrl, stg_val;
    reg [3:0]  tgt;            // allocated / matched slot
    reg [3:0]  scan;
    reg [132:0] stg_envstate;  // captured slot state (retrigger LOAD)
    reg [31:0] pm_nobase_w, pbase_w, amd_w, w_peg;
    reg [7:0]  w_pmdep, w_pmsens, w_amdep;
    reg [31:0] fq_sel;
    wire signed [8:0] fq_fsh_s = 9'sd20 - $signed(fq_sel[31:24]);
    wire [8:0] fq_fsh_c = fq_fsh_s[8] ? 9'd0 : fq_fsh_s[8:0];
    // router driving registers
    reg        r_ld_cfg, r_ld_fb, r_ld_op, r_start;
    reg [4:0]  r_cfg_algorithm, r_cfg_feedback;
    reg [31:0] r_cfg_fb0, r_cfg_fb1, r_ld_phase, r_ld_freq;
    reg [2:0]  r_ld_op_idx;
    reg [27:0] r_ld_level;
    reg [15:0] r_ld_gain_out;
    reg [6:0]  r_rd_addr;
    reg [31:0] w_level, w_li;
    // level -= ((level * (pt << 4)) >> 28) & 0xffffffff  (dx7note.cc:291)
    wire [67:0] exp_prod = w_level * {4'b0, exp_pt, 4'b0};
    wire [31:0] exp_sub  = exp_prod[67:28] & 32'hFFFFFFFF;
    wire [31:0] exp_li   = w_level - exp_sub;
    reg [9:0]  w_fa;
    reg [13:0] w_fl;
    reg [8:0]  w_fsh;
    reg [5:0]  mix_j;
    reg [2:0]  rb_cnt;
    reg [15:0] sched_start, sched_used;
    // derivation outputs (combinational; inputs dv_*)
    wire [6:0] dv_note = (fc_state == F_REFOP) ? n_midi[nn] : stg_note;
    wire [6:0] dv_vel  = (fc_state == F_REFOP) ? n_vel[nn]  : stg_vel;
    wire [2:0] dv_op   = opk;
    wire [6:0] dv_r0, dv_r1, dv_r2, dv_r3;
    wire [6:0] dv_l0, dv_l1, dv_l2, dv_l3;
    wire [12:0] dv_outlevel;
    wire [8:0]  dv_rs;
    wire [31:0] dv_bp;
    wire [5:0]  dv_opmode;
    reg  [95:0] env_cmd_en_nxt;

    // ---------------- allocation (combinational, DEC-015..DEC-017) -------
    // rotated free-slot scan (model: from current_note, !live || reclaimed)
    wire [3:0] rot0 = cur_note;
    wire f0 = (!n_live[rot0]              || n_reclaimed[rot0]);
    wire f1 = (!n_live[rot0+4'd1]         || n_reclaimed[rot0+4'd1]);
    wire f2 = (!n_live[rot0+4'd2]         || n_reclaimed[rot0+4'd2]);
    wire f3 = (!n_live[rot0+4'd3]         || n_reclaimed[rot0+4'd3]);
    wire f4 = (!n_live[rot0+4'd4]         || n_reclaimed[rot0+4'd4]);
    wire f5 = (!n_live[rot0+4'd5]         || n_reclaimed[rot0+4'd5]);
    wire f6 = (!n_live[rot0+4'd6]         || n_reclaimed[rot0+4'd6]);
    wire f7 = (!n_live[rot0+4'd7]         || n_reclaimed[rot0+4'd7]);
    wire f8 = (!n_live[rot0+4'd8]         || n_reclaimed[rot0+4'd8]);
    wire f9 = (!n_live[rot0+4'd9]         || n_reclaimed[rot0+4'd9]);
    wire f10= (!n_live[rot0+4'd10]        || n_reclaimed[rot0+4'd10]);
    wire f11= (!n_live[rot0+4'd11]        || n_reclaimed[rot0+4'd11]);
    wire f12= (!n_live[rot0+4'd12]        || n_reclaimed[rot0+4'd12]);
    wire f13= (!n_live[rot0+4'd13]        || n_reclaimed[rot0+4'd13]);
    wire f14= (!n_live[rot0+4'd14]        || n_reclaimed[rot0+4'd14]);
    wire f15= (!n_live[rot0+4'd15]        || n_reclaimed[rot0+4'd15]);
    reg [3:0] free_idx; reg free_found;
    always @* begin
        free_found = 1'b1;
        if (f0)       free_idx = rot0;
        else if (f1)  free_idx = rot0 + 4'd1;
        else if (f2)  free_idx = rot0 + 4'd2;
        else if (f3)  free_idx = rot0 + 4'd3;
        else if (f4)  free_idx = rot0 + 4'd4;
        else if (f5)  free_idx = rot0 + 4'd5;
        else if (f6)  free_idx = rot0 + 4'd6;
        else if (f7)  free_idx = rot0 + 4'd7;
        else if (f8)  free_idx = rot0 + 4'd8;
        else if (f9)  free_idx = rot0 + 4'd9;
        else if (f10) free_idx = rot0 + 4'd10;
        else if (f11) free_idx = rot0 + 4'd11;
        else if (f12) free_idx = rot0 + 4'd12;
        else if (f13) free_idx = rot0 + 4'd13;
        else if (f14) free_idx = rot0 + 4'd14;
        else if (f15) free_idx = rot0 + 4'd15;
        else begin free_idx = 4'd0; free_found = 1'b0; end
    end
    // exact-silence predicate per note (combinational; allocation + census)
    function sil_op(input [132:0] s, input [15:0] go);
        sil_op = (env_level(s) == 32'h00100000)
               & (env_target(s) == 32'h00100000)
               & (env_ix(s) >= 3'd3)
               & (go > 16'd16284);
    endfunction
    genvar gn, go2;
    wire [15:0] note_silent;
    generate
        for (gn = 0; gn < 16; gn = gn + 1) begin : silgen
            wire s0 = sil_op(env_rd[gn*6+0], op_go[gn][0]);
            wire s1 = sil_op(env_rd[gn*6+1], op_go[gn][1]);
            wire s2 = sil_op(env_rd[gn*6+2], op_go[gn][2]);
            wire s3 = sil_op(env_rd[gn*6+3], op_go[gn][3]);
            wire s4 = sil_op(env_rd[gn*6+4], op_go[gn][4]);
            wire s5 = sil_op(env_rd[gn*6+5], op_go[gn][5]);
            assign note_silent[gn] = n_live[gn] & s0 & s1 & s2 & s3 & s4 & s5;
        end
    endgenerate
    // sounding / in_release pools (DEC-017)
    wire [15:0] sounding  = n_live & ~note_silent;
    wire [15:0] in_rel    = n_live & ~n_keydown & ~n_sustained;
    // age (wrap-aware; smaller = older)
    wire [7:0] age [0:15];
    genvar ga;
    generate
        for (ga = 0; ga < 16; ga = ga + 1) begin : agegen
            assign age[ga] = seq_ctr - n_seq[ga];
        end
    endgenerate
    // oldest matching pitch among sounding (DEC-016)
    reg [3:0] match_idx; reg match_found;
    // steal victim pool (DEC-017): release preference, then oldest
    wire any_rel = |in_rel;
    reg [3:0] steal_idx; reg steal_found;
    integer ai;
    always @* begin
        match_found = 1'b0; match_idx = 4'd0;
        for (ai = 0; ai < 16; ai = ai + 1) begin
            if (sounding[ai] && (n_midi[ai] == stg_note)
                && (!match_found || (age[ai] < age[match_idx]))) begin
                match_found = 1'b1; match_idx = ai[3:0];
            end
        end
        steal_found = 1'b0; steal_idx = 4'd0;
        for (ai = 0; ai < 16; ai = ai + 1) begin
            if ((any_rel ? in_rel[ai] : sounding[ai])
                && (!steal_found || (age[ai] < age[steal_idx]))) begin
                steal_found = 1'b1; steal_idx = ai[3:0];
            end
        end
    end

    // =====================================================================
    // Note-on per-op derivation (dx7note.cc:163-179 + 30-64; envelope.py)
    // Combinational; inputs dv_note/dv_vel/dv_op (+ staged deltas).
    // =====================================================================
    wire [2:0] dv_eb = 3'd5 - dv_op;   // env block = param 5-k
    wire [31:0] dv_erates  = cmt_erates[dv_eb];
    wire [31:0] dv_elevels = cmt_elevels[dv_eb];
    wire [31:0] dv_scaleA  = cmt_scaleA[dv_eb];
    wire [31:0] dv_scaleB  = cmt_scaleB[dv_eb];
    wire [7:0] dv_r_raw0 = dv_erates[7:0];
    wire [7:0] dv_r_raw1 = dv_erates[15:8];
    wire [7:0] dv_r_raw2 = dv_erates[23:16];
    wire [7:0] dv_r_raw3 = dv_erates[31:24];
    wire [6:0] dv_rate0  = dv_r_raw0[6:0];
    wire [6:0] dv_rate1  = dv_r_raw1[6:0];
    wire [6:0] dv_rate2  = dv_r_raw2[6:0];
    wire [6:0] dv_rate3  = dv_r_raw3[6:0];
    wire [6:0] dv_lvl0   = dv_elevels[6:0];
    wire [6:0] dv_lvl1   = dv_elevels[14:8];
    wire [6:0] dv_lvl2   = dv_elevels[22:16];
    wire [6:0] dv_lvl3   = dv_elevels[30:24];
    wire [7:0] dv_bp_byte  = dv_scaleA[7:0];
    wire [2:0] dv_kvs      = dv_scaleA[10:8];
    wire [1:0] dv_rcur     = dv_scaleA[12:11];
    wire [1:0] dv_lcur     = dv_scaleA[14:13];
    wire [7:0] dv_rdep     = dv_scaleA[22:15];
    wire [7:0] dv_ldep     = dv_scaleA[30:23];
    wire [7:0] dv_out_byte = dv_scaleB[7:0];
    wire [7:0] dv_rs_byte  = dv_scaleB[15:8];
    // scale_level (dx7note.cc:118-126)
    wire signed [8:0] dv_offset = $signed({1'b0, dv_note})
                                - $signed({1'b0, dv_bp_byte}) - 9'sd17;
    wire        dv_pos    = (dv_offset >= 9'sd0);
    wire [8:0]  dv_gpre   = dv_pos ? ($unsigned(dv_offset) + 9'd1)
                                   : (9'd1 - $unsigned(dv_offset));
    wire [8:0]  dv_gdiv   = dv_gpre / 9'd3;           // <= 42
    wire [5:0]  dv_g      = (dv_gdiv > 6'd32) ? 6'd32 : dv_gdiv[5:0];
    wire [7:0]  dv_depth  = dv_pos ? dv_rdep : dv_ldep;
    wire [1:0]  dv_curve  = dv_pos ? dv_rcur : dv_lcur;
    wire [20:0] dv_linp   = dv_g * dv_depth * 21'd329;   // <= 1,042,752
    wire [22:0] dv_expp   = exp_scale_data(dv_g) * dv_depth * 16'd329;
    // scale_curve (dx7note.cc:101-116): curves 0/3 linear, 1/2 exponential;
    // curve bit 1 is the SIGN (0: negative, 1: positive) in the model --
    // 0: -lin, 1: -exp, 2: +exp, 3: +lin.
    wire signed [21:0] dv_lin_s = $signed({5'b0, dv_linp[20:12]});
    wire signed [23:0] dv_exp_s = $signed({7'b0, dv_expp[22:15]});
    wire signed [15:0] dv_scaleS =
        (dv_curve == 2'd0 || dv_curve == 2'd3)
        ? (dv_curve[1] ? $signed({9'b0, dv_lin_s}) : -$signed({9'b0, dv_lin_s}))
        : (dv_curve[1] ? $signed({9'b0, dv_exp_s}) : -$signed({9'b0, dv_exp_s}));
    wire signed [13:0] dv_ols_pre = $signed({6'b0, levellut(dv_out_byte[6:0])})
                                  + dv_scaleS;
    wire signed [13:0] dv_ols = (dv_ols_pre > 14'sd127) ? 14'sd127
                                                        : dv_ols_pre;
    wire signed [8:0] dv_velval = $signed({1'b0,
        vel_data(dv_vel[6:1])}) - 9'sd239;
    // scale_velocity (dx7note.cc:75-80): ((sens*velval + 7) >> 3) << 4 --
    // the << 4 is part of the model's velocity scaling (microstep units);
    // BOTH operands must be signed (negative velval is in-domain)
    wire signed [8:0] dv_kvs_s = $signed({1'b0, dv_kvs});
    wire signed [19:0] dv_velsc = (((dv_kvs_s * dv_velval)
                                    + 16'sd7) >>> 3) <<< 4;
    wire signed [15:0] dv_olsum = $signed({1'b0, dv_ols[12:0], 5'b0})
                                + dv_velsc;
    assign dv_outlevel = (dv_olsum < 16'sd0) ? 13'd0 : dv_olsum[12:0];
    // scale_rate (dx7note.cc:82-94)
    wire [6:0] dv_srx = (dv_note >= 7'd21) ? ((dv_note / 7'd3) - 7'd7) : 7'd0;
    wire [6:0] dv_srx_c = (dv_srx > 7'd31) ? 7'd31 : dv_srx;
    assign dv_rs = (dv_rs_byte * dv_srx_c) >> 3;       // <= 382 (9 b)
    // rs compensation for env_unit's 6-bit field (H07-CORE.md section 4):
    // rs' = min(rs, 63); r'_i = min(r_i + rs, 99) - 63 when rs >= 64.
    wire       dv_rs_big  = (dv_rs >= 9'd64);
    wire [5:0] dv_rs_eff  = dv_rs_big ? 6'd63 : dv_rs[5:0];
    wire [6:0] dv_rsub0   = dv_rs_big
        ? ((({2'b0, dv_rate0} + {2'b0, dv_rs}) > 10'd99)
           ? 7'd99 : ({2'b0, dv_rate0} + {2'b0, dv_rs})) - 7'd63
        : dv_rate0;
    wire [6:0] dv_rsub1   = dv_rs_big
        ? ((({2'b0, dv_rate1} + {2'b0, dv_rs}) > 10'd99)
           ? 7'd99 : ({2'b0, dv_rate1} + {2'b0, dv_rs})) - 7'd63
        : dv_rate1;
    wire [6:0] dv_rsub2   = dv_rs_big
        ? ((({2'b0, dv_rate2} + {2'b0, dv_rs}) > 10'd99)
           ? 7'd99 : ({2'b0, dv_rate2} + {2'b0, dv_rs})) - 7'd63
        : dv_rate2;
    wire [6:0] dv_rsub3   = dv_rs_big
        ? ((({2'b0, dv_rate3} + {2'b0, dv_rs}) > 10'd99)
           ? 7'd99 : ({2'b0, dv_rate3} + {2'b0, dv_rs})) - 7'd63
        : dv_rate3;
    assign {dv_r3, dv_r2, dv_r1, dv_r0} = {dv_rsub3, dv_rsub2, dv_rsub1,
                                           dv_rsub0};
    assign {dv_l3, dv_l2, dv_l1, dv_l0} = {dv_lvl3, dv_lvl2, dv_lvl1,
                                           dv_lvl0};
    // basepitch (dx7note.cc:30-64; pinned_wrapper transpose finding)
    wire signed [31:0] dv_lprime = A4_BASE + SEMI_LOGFREQ
                                        * $signed({25'b0, dv_note});
    wire [4:0] dv_coarse = c_coarse[dv_op][4:0];
    wire [7:0] dv_fine   = c_fine[dv_op];
    wire [7:0] dv_detune = c_detune[dv_op];
    wire [31:0] dv_bpf0  = (32'd4458616
        * (({30'b0, dv_coarse[1:0]} * 32'd100)
           + {24'b0, dv_fine})) >> 3;
    wire [31:0] dv_bpf   = (dv_detune > 8'd7)
        ? dv_bpf0 + 32'd13457 * (dv_detune - 8'd7) : dv_bpf0;
    wire signed [31:0] dv_bpr0 = dv_lprime + $signed(stg_dd[dv_op]);
    wire signed [31:0] dv_bpr1 = dv_bpr0 + coarsemul_lut(dv_coarse);
    wire signed [31:0] dv_bpr  = (dv_fine != 8'd0)
        ? dv_bpr1 + $signed(cmt_fterm[dv_op]) : dv_bpr1;
    assign dv_bp = c_mode[dv_op][0] ? dv_bpf : dv_bpr[31:0];
    wire dv_mode = (c_mode[dv_op] != 8'd0);

    // =====================================================================
    // Freqlut + PM/AM combinational stage (pitch_mod.v semantics, shared)
    // =====================================================================
    wire [31:0] fq_y0 = freqlut_rom[w_fa];
    wire [31:0] fq_y1 = freqlut_rom[w_fa + 11'd1];
    wire signed [31:0] fq_diff = $signed(fq_y1) - $signed(fq_y0);
    wire signed [45:0] fq_prod = fq_diff * $signed({32'b0, w_fl});
    wire [31:0] fq_fy = fq_y0 + fq_prod[45:14];
    wire [31:0] fq_val = $signed(fq_fy) >>> w_fsh;
    // pitch-EG step (pitchenv.cc:55-89) for note nn
    wire        t_peg_act = (n_pegix[nn] < 3'd3)
                              || ((n_pegix[nn] < 3'd4) && !n_pegdwn[nn]);
    wire [31:0] t_peg_up  = n_peglvl[nn] + n_peginc[nn];
    wire [31:0] t_peg_dn  = n_peglvl[nn] - n_peginc[nn];
    wire        t_up_hit  = $signed(t_peg_up) >= $signed(n_pegtgt[nn]);
    wire        t_dn_hit  = $signed(t_peg_dn) <= $signed(n_pegtgt[nn]);
    wire [31:0] t_peg_nxt = !t_peg_act ? n_peglvl[nn] :
                  (n_pegris[nn] && t_up_hit) ? n_pegtgt[nn] :
                  (n_pegris[nn])             ? t_peg_up :
                  (t_dn_hit)                 ? n_pegtgt[nn] : t_peg_dn;
    wire        t_peg_adv = t_peg_act
                  && (n_pegris[nn] ? t_up_hit : t_dn_hit);
    // PM composition (dx7note.cc:210-217) for the latched note
    wire [31:0] w_pmdd   = {24'b0, w_pmdep} * lfo_del_q;
    wire signed [31:0] w_slfo = $signed({7'b0, lfo_val_q}) - 32'sd8388608;
    wire signed [39:0] w_sl0  = $signed({32'b0, w_pmsens}) * w_slfo;
    wire signed [31:0] w_senslfo = w_sl0[31:0];
    wire signed [64:0] w_prod1 = $signed({1'b0, w_pmdd}) * $signed(w_senslfo);
    wire signed [63:0] w_sh1   = w_prod1 >>> 39;
    wire [31:0] w_pm1  = w_sh1[31:0];
    wire [31:0] w_pm1a = w_pm1[31] ? (~w_pm1 + 32'd1) : w_pm1;
    wire signed [63:0] w_prod2 = $signed(ctrl_pm) * $signed(w_senslfo);
    wire signed [63:0] w_sh2   = w_prod2 >>> 14;
    wire [31:0] w_pm2  = w_sh2[31:0];
    wire [31:0] w_pm2a = w_pm2[31] ? (~w_pm2 + 32'd1) : w_pm2;
    wire [31:0] w_pmax = ($signed(w_pm1a) > $signed(w_pm2a)) ? w_pm1a : w_pm2a;
    wire [31:0] w_pmsign = w_senslfo[31] ? (32'd0 - w_pmax) : w_pmax;
    wire [31:0] w_pmnobase = w_peg + w_pmsign;
    // AM composition (dx7note.cc:259-268)
    wire [31:0] w_lv    = 32'h01000000 - {7'b0, lfo_val_q};
    wire [39:0] w_am1p  = {32'b0, w_amdep} * {7'b0, lfo_del_q};
    wire [31:0] w_am1   = w_am1p[39:8];
    wire [55:0] w_am1bp = {24'b0, w_am1} * {8'b0, w_lv};
    wire [31:0] w_am1b  = w_am1bp[55:24];
    wire signed [64:0] w_am2p = $signed(ctrl_am)
                              * $signed({1'b0, w_lv});
    wire [31:0] w_am2   = w_am2p[31:0] >>> 7;
    wire [31:0] w_amax  = (w_am1b > w_am2) ? w_am1b : w_am2;
    wire [31:0] w_a3    = (eg_mod + 32'd1) << 17;
    wire [31:0] w_ambase = 32'h01000000 - w_a3;
    wire [31:0] w_amd   = (w_ambase > w_amax) ? w_ambase : w_amax;

    // =====================================================================
    // The H05 algorithm router (single shared instance, model-identical)
    // + the combinational op-kernel eval seam
    // =====================================================================
    wire        ar_ld_cfg, ar_ld_fb, ar_ld_op, ar_start, ar_eval_req,
                ar_frame_done, ar_busy;
    wire [4:0]  ar_cfg_algorithm;
    wire [4:0]  ar_cfg_feedback;
    wire [31:0] ar_cfg_fb0, ar_cfg_fb1;
    wire [2:0]  ar_ld_op_idx;
    wire [31:0] ar_ld_phase, ar_ld_freq, ar_ld_mod;
    wire [27:0] ar_ld_level;
    wire [15:0] ar_ld_gain_out;
    wire [2:0]  ar_eval_op;
    wire [31:0] ar_eval_phase_in, ar_eval_mod;
    wire [15:0] ar_eval_gain;
    wire [31:0] ar_eval_y;
    wire [6:0]  ar_rd_addr;
    wire [31:0] ar_rd_data;
    wire        ar_state_obs;

    h07_op_kernel u_kern (
        .phase(ar_eval_phase_in),
        .env(ar_eval_gain),
        .out_raw(ar_eval_y)
    );

    assign ar_ld_cfg = r_ld_cfg;
    assign ar_cfg_algorithm = r_cfg_algorithm;
    assign ar_cfg_feedback = r_cfg_feedback;
    assign ar_ld_fb = r_ld_fb;
    assign ar_cfg_fb0 = r_cfg_fb0;
    assign ar_cfg_fb1 = r_cfg_fb1;
    assign ar_ld_op = r_ld_op;
    assign ar_ld_op_idx = r_ld_op_idx;
    assign ar_ld_phase = r_ld_phase;
    assign ar_ld_freq = r_ld_freq;
    assign ar_ld_level = r_ld_level;
    assign ar_ld_gain_out = r_ld_gain_out;
    assign ar_start = r_start;
    assign ar_rd_addr = r_rd_addr;

    alg_router u_router (
        .clk(clk), .rst(rst),
        .ld_cfg(ar_ld_cfg), .cfg_algorithm(ar_cfg_algorithm),
        .cfg_feedback(ar_cfg_feedback),
        .ld_fb(ar_ld_fb), .cfg_fb0(ar_cfg_fb0), .cfg_fb1(ar_cfg_fb1),
        .ld_op(ar_ld_op), .ld_op_idx(ar_ld_op_idx), .ld_phase(ar_ld_phase),
        .ld_freq(ar_ld_freq), .ld_level(ar_ld_level),
        .ld_gain_out(ar_ld_gain_out),
        .start(ar_start),
        .eval_req(ar_eval_req), .eval_op(ar_eval_op),
        .eval_phase_in(ar_eval_phase_in), .eval_gain(ar_eval_gain),
        .eval_mod(ar_eval_mod), .eval_y(ar_eval_y),
        .frame_done(ar_frame_done), .rd_addr(ar_rd_addr),
        .rd_data(ar_rd_data), .busy(ar_busy), .state_obs(ar_state_obs)
    );

    // =====================================================================
    // Mixer: per-note >>4 / clip(+-2^24, +32768 quirk) / >>9 conversion
    // into the unclamped mix accumulator (NUM-010), double buffered.
    // =====================================================================
    reg signed [32:0] mixbuf [0:1][0:63];
    reg        in_sel, out_sel;
    wire signed [32:0] cv_val = $signed(ar_rd_data) >>> 4;
    wire        cv_lo = $signed(cv_val) < -$signed(33'sd16777216);
    wire        cv_hi = $signed(cv_val) >= $signed(33'sd16777216);
    wire signed [16:0] cv_c17 = cv_lo ? 17'sd32768
                           : cv_hi ? 17'sd32767
                           : ($signed(cv_val) >>> 9);
    wire signed [32:0] cv_add = cv_c17;
`ifdef H07_MUTATE_SUM_ORDER
    wire signed [32:0] cv_mix = cv_val;   // sum-then-clip mutant
`else
    wire signed [32:0] cv_mix = cv_add;
`endif
    // one-sample glitch control (frame 12, sample 17, +1 LSB)
`ifdef H07_MUTATE_GLITCH
    wire signed [32:0] cv_mix_g =
        (frame_ctr == 16'd12 && ar_rd_addr == 7'd17)
        ? cv_mix + 33'sd1 : cv_mix;
`else
    wire signed [32:0] cv_mix_g = cv_mix;
`endif

    // =====================================================================
    // Master sequential FSM
    // =====================================================================
    assign fc_commit = (fc_state == F_COMMIT);

    // env command handshake: a command is accepted only while the target
    // unit is idle (one-command interface), so every issue point waits
    wire [5:0] tgt_busy  = env_busy[tgt*6 +: 6];
    wire [5:0] nn_busy   = env_busy[nn*6 +: 6];
    wire [5:0] scan_busy = env_busy[scan*6 +: 6];

    integer fi, fj;

    always @(posedge clk) begin
        if (rst) begin
            fc_state <= F_IDLE;
            nn <= 4'd0; opk <= 3'd0; tgt <= 4'd0; scan <= 4'd0;
            kind <= 2'd0; mix_j <= 6'd0; rb_cnt <= 3'd0;
            n_live <= 16'd0; n_keydown <= 16'd0; n_sustained <= 16'd0;
            n_reclaimed <= 16'd0; n_used <= 16'd0; n_silnow <= 16'd0;
            cur_note <= 4'd0; seq_ctr <= 8'd0; sustain <= 1'b0;
            refresh_pending <= 1'b1;
            in_sel <= 1'b0; out_sel <= 1'b0;
            bend_pb <= 32'sd0; master_tune <= 32'sd0;
            ctrl_pm <= 32'sd0; ctrl_am <= 32'sd0; eg_mod <= 32'd127;
            lfo_phase <= 32'd0; lfo_delta <= 32'd0; lfo_dstate <= 32'd0;
            lfo_dinc1 <= 32'd0; lfo_dinc2 <= 32'd0; lfo_rand <= 8'd0;
            lfo_val_q <= 25'd0; lfo_del_q <= 25'd0;
            st_overrun <= 1'b0;
            for (fi = 0; fi < 6; fi = fi + 1) begin
                cmt_opitch[fi]  <= 32'd0;  cmt_fterm[fi]   <= 32'd0;
                cmt_erates[fi]  <= 32'd0;  cmt_elevels[fi] <= 32'd0;
                cmt_scaleA[fi]  <= 32'd0;  cmt_scaleB[fi]  <= 32'd0;
            end
            cmt_globA <= 32'd0; cmt_globB <= 32'd0; cmt_globC <= 32'd0;
            cmt_pegr <= 32'd0;  cmt_pegl <= 32'd0;
            for (fi = 0; fi < 16; fi = fi + 1) begin
                n_midi[fi] <= 7'd0;  n_vel[fi] <= 7'd0;  n_seq[fi] <= 8'd0;
                n_sil[fi] <= 7'd0;
                n_opmode[fi] <= 6'd0; n_alg[fi] <= 5'd0; n_fbs[fi] <= 5'd0;
                n_pmdep[fi] <= 8'd0; n_pmsens[fi] <= 8'd0;
                n_amdep[fi] <= 8'd0;
                n_peglvl[fi] <= 32'd0; n_pegtgt[fi] <= 32'd0;
                n_peginc[fi] <= 32'd0; n_pegix[fi] <= 3'd0;
                n_pegris[fi] <= 1'b0; n_pegdwn[fi] <= 1'b1;
                n_fb0[fi] <= 32'd0; n_fb1[fi] <= 32'd0;
                for (fj = 0; fj < 6; fj = fj + 1) begin
                    n_bp[fi][fj] <= 32'd0;
                    n_pegr[fi][fj] <= 7'd0;
                    n_pegl[fi][fj] <= 7'd0;
                    n_ams[fi][fj] <= 25'd0;
                    n_dd[fi][fj] <= 32'd0;
                    op_ph[fi][fj] <= 32'd0;
                    op_go[fi][fj] <= 16'd0;
                    op_fq[fi][fj] <= 32'd0;
                    op_li[fi][fj] <= 32'd0;
                end
                for (fj = 0; fj < 64; fj = fj + 1)
                    mixbuf[fi[0]][fj] <= 33'sd0;
            end
            for (fj = 0; fj < 64; fj = fj + 1) begin
                mixbuf[0][fj] <= 33'sd0;
                mixbuf[1][fj] <= 33'sd0;
            end
            env_cmd_en <= 96'd0;
            exp_start <= 1'b0;
            stg_note <= 7'd0; stg_vel <= 7'd0; stg_ctrl <= 8'd0;
            stg_val <= 8'd0; stg_envstate <= 133'd0;
            pm_nobase_w <= 32'd0; pbase_w <= 32'd0; amd_w <= 32'd0;
            w_peg <= 32'd0; w_level <= 32'd0; w_li <= 32'd0;
            w_fa <= 10'd0; w_fl <= 14'd0; w_fsh <= 9'd0;
            sched_start <= 16'd0; sched_used <= 16'd0;
        end else begin
            env_cmd_en <= 96'd0;          // default: no command this cycle
            if (frame_tick && (fc_state != F_IDLE))
                st_overrun <= 1'b1;       // defect detector: must never set

            case (fc_state)
            // ------------------------------------------------------- idle
            F_IDLE: begin
                if (frame_tick) begin
                    sched_start <= frame_ctr;
                    in_sel <= ~in_sel;
                    out_sel <= in_sel;
                    for (fj = 0; fj < 64; fj = fj + 1)
                        mixbuf[~in_sel][fj] <= 33'sd0;
                    fc_state <= F_COMMIT;
                end
            end
            // ------------------------------------------------------ commit
            F_COMMIT: begin
                if (shd_commit) begin
                    for (fi = 0; fi < 6; fi = fi + 1) begin
                        cmt_opitch[fi]  <= shd_opitch[fi];
                        cmt_fterm[fi]   <= shd_fterm[fi];
                        cmt_erates[fi]  <= shd_erates[fi];
                        cmt_elevels[fi] <= shd_elevels[fi];
                        cmt_scaleA[fi]  <= shd_scaleA[fi];
                        cmt_scaleB[fi]  <= shd_scaleB[fi];
                    end
                    cmt_globA <= shd_globA;
                    cmt_globB <= shd_globB;
                    cmt_globC <= shd_globC;
                    cmt_pegr  <= shd_pegr;
                    cmt_pegl  <= shd_pegl;
                    lfo_delta <= l_delta_w;    // DEC-019/DEC-020: no phase
                    lfo_dinc1 <= l_dinc1_w;    // reset, params from the
                    lfo_dinc2 <= l_dinc2_w;    // incoming image
                    shd_commit <= 1'b0;
                end
                fc_state <= F_EVPOP;
            end
            // ---------------------------------------------- event queue pop
            F_EVPOP: begin
                if (!evq_peek_empty) begin
                    case (ev_a)
                        EA_NOTEON: begin
                            stg_note <= ev_d[6:0];
                            stg_vel  <= ev_d[14:8];
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_ALLOC;
                        end
                        EA_NOTEOFF: begin
                            stg_note <= ev_d[6:0];
                            scan <= 4'd0;
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_OFFFIND;
                        end
                        EA_CC: begin
                            stg_ctrl <= ev_d[7:0];
                            stg_val  <= ev_d[15:8];
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            case (ev_d[7:0])
                                8'd64: begin
                                    sustain <= (ev_d[15:8] > 8'd63);
                                    if (!(ev_d[15:8] > 8'd63)) begin
                                        scan <= 4'd0;
                                        fc_state <= F_PEDUP;
                                    end else
                                        fc_state <= F_EVPOP;
                                end
                                8'd120: begin
                                    for (fi = 0; fi < 16; fi = fi + 1) begin
                                        n_keydown[fi] <= 1'b0;
                                        n_sustained[fi] <= 1'b0;
                                        n_live[fi] <= 1'b0;
                                        n_sil[fi] <= 7'd0;
                                        for (fj = 0; fj < 6; fj = fj + 1) begin
                                            op_ph[fi][fj] <= 32'd0;
                                            op_go[fi][fj] <= 16'd0;
                                        end
                                    end
                                    fc_state <= F_EVPOP;
                                end
                                8'd123: begin
                                    // all-notes-off: walk the banks, key_up
                                    // per keydown slot (sustain aware,
                                    // DEC-018); commands need idle units
                                    scan <= 4'd0;
                                    fc_state <= F_ANO;
                                end
                                default: fc_state <= F_EVPOP;
                            endcase
                        end
                        EA_PB: begin
                            bend_pb <= $signed(ev_d);
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_DD0, EA_DD0+5'd1, EA_DD0+5'd2, EA_DD0+5'd3,
                        EA_DD0+5'd4, EA_DD0+5'd5: begin
                            stg_dd[ev_a - 5'd4] <= ev_d;   // op = addr-DD0
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_MTUNE: begin
                            master_tune <= $signed(ev_d);
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_CTRLPM: begin
                            ctrl_pm <= $signed(ev_d);
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_CTRLAM: begin
                            ctrl_am <= $signed(ev_d);
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_EGMOD: begin
                            eg_mod <= ev_d;
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                        EA_SOFT: begin
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_SOFT;
                        end
                        default: begin
                            evq_rp <= (evq_rp == EVQ_DEPTH-1) ? 8'd0
                                                              : evq_rp + 8'd1;
                            evq_cnt <= evq_cnt - 9'd1;
                            fc_state <= F_EVPOP;
                        end
                    endcase
                end else begin
                    // queue drained: control tail
                    if (refresh_pending) begin
                        scan <= 4'd0;
                        nn <= 4'd0;
                        fc_state <= F_REFSEL;
                    end else begin
                        scan <= 4'd0;
                        opk <= 3'd0;
                        fc_state <= F_ENVGO;
                    end
                end
            end
            // ---------------------------------------------------- allocation
            F_ALLOC: begin
                if (free_found) begin
                    tgt <= free_idx;
                    kind <= 2'd0;
                    cur_note <= free_idx + 4'd1;    // wrapper rotation
                    fc_state <= F_NN_BP;
                end else if (match_found) begin
                    tgt <= match_idx;
                    kind <= 2'd1;
                    fc_state <= F_NN_BP;
                end else if (steal_found) begin
                    tgt <= steal_idx;
                    kind <= 2'd1;
                    fc_state <= F_NN_BP;
                end else
                    fc_state <= F_EVPOP;   // no slot at all (cannot happen)
            end
            // ------------------------- strike: bind patch + basepitch x6
            F_NN_BP: begin
                // bind the striking patch's derived params (DEC-019)
                n_alg[tgt]     <= c_algorithm;
                n_fbs[tgt]     <= (c_feedback == 3'd0) ? 5'd16
                                                       : (5'd8 - {2'b0,
                                                            c_feedback});
                // (depth*165)>>6 with the product computed at full width
                // (a sized 9'd165 operand would truncate the multiply)
                n_pmdep[tgt]   <= 8'((c_pmdep_src * 17'd165) >> 6);
                n_amdep[tgt]   <= 8'((c_amdep_src * 17'd165) >> 6);
                n_pmsens[tgt]  <= pmsens_tab(c_pmsens_idx);
                n_ams[tgt][0]  <= ampsens_tab(c_ams0);
                n_ams[tgt][1]  <= ampsens_tab(c_ams1);
                n_ams[tgt][2]  <= ampsens_tab(c_ams2);
                n_ams[tgt][3]  <= ampsens_tab(c_ams3);
                n_ams[tgt][4]  <= ampsens_tab(c_ams4);
                n_ams[tgt][5]  <= ampsens_tab(c_ams5);
                n_pegr[tgt][0] <= shd_pegr_w[7:0];
                n_pegr[tgt][1] <= shd_pegr_w[15:8];
                n_pegr[tgt][2] <= shd_pegr_w[23:16];
                n_pegr[tgt][3] <= shd_pegr_w[31:24];
                n_pegl[tgt][0] <= shd_pegl_w[6:0];
                n_pegl[tgt][1] <= shd_pegl_w[14:8];
                n_pegl[tgt][2] <= shd_pegl_w[22:16];
                n_pegl[tgt][3] <= shd_pegl_w[30:24];
                nn   <= tgt;
                opk  <= 3'd0;
                fc_state <= F_NN_BPW;
            end
            F_NN_BPW: begin
                n_bp[tgt][opk] <= dv_bp;
                n_opmode[tgt][opk] <= {5'b0, dv_mode};
                if (opk == 3'd5) begin
                    opk <= 3'd0;
                    fc_state <= F_NN_ENVP;
                end else
                    opk <= opk + 3'd1;
            end
            // ------------------------------------- strike: env params x6
            F_NN_ENVP: begin
                if (env_busy[tgt*6+opk]) begin
                    // wait: the unit may still be executing a prior command
                end else begin
                    env_cmd[tgt*6+opk]  <= EC_PARAM;
                    env_cmd_data[tgt*6+opk] <=
                        {dv_r0, dv_r1, dv_r2, dv_r3,
                         dv_l0, dv_l1, dv_l2, dv_l3,
                         dv_outlevel, dv_rs_eff};
                    env_cmd_en[tgt*6+opk] <= 1'b1;
                    if (opk == 3'd5) begin
                        opk <= 3'd0;
                        fc_state <= (kind == 2'd0) ? F_NN_ENVI : F_NN_ENVK;
                    end else
                        opk <= opk + 3'd1;
                end
            end
            // ------------------------- free slot strike: Env::init x6
            F_NN_ENVI: begin
                if (|tgt_busy) begin
                    // wait for the param writes to land
                end else begin
                    for (fi = 0; fi < 6; fi = fi + 1) begin
                        env_cmd[tgt*6+fi]  <= EC_INIT;
                        env_cmd_en[tgt*6+fi] <= 1'b1;
                    end
                    fc_state <= F_NN_PEG;
                end
            end
            // ----------------- retrigger strike: forced down edge x6
            F_NN_ENVK: begin
                if (env_busy[tgt*6+opk]) begin
                    // wait
                end else begin
                    env_cmd[tgt*6+opk]  <= EC_LOAD;
                    // keep committed level/target/inc/static/ix/rising,
                    // force down_ = 0 (DEC-015 edge precondition)
                    env_cmd_data[tgt*6+opk] <=
                        {env_rd[tgt*6+opk][132:1], 1'b0};
                    env_cmd_en[tgt*6+opk] <= 1'b1;
                    if (opk == 3'd5) begin
                        opk <= 3'd0;
                        fc_state <= F_NN_ENVK4;
                    end else
                        opk <= opk + 3'd1;
                end
            end
            F_NN_ENVK4: begin
                if (|tgt_busy) begin
                    // wait for the LOAD writes to land
                end else begin
                    for (fi = 0; fi < 6; fi = fi + 1) begin
                        env_cmd[tgt*6+fi]  <= EC_KEYDOWN;
                        env_cmd_data[tgt*6+fi] <= 133'd1;  // keydown(True)
                        env_cmd_en[tgt*6+fi] <= 1'b1;
                    end
                    fc_state <= F_NN_PEG;
                end
            end
            // ------------------------------------------ strike: pitch EG
            F_NN_PEG: begin
                if (kind == 2'd0) begin
                    // PitchEnv.set: level = TAB[l3] << 19, then advance(0)
                    n_peglvl[tgt] <= peg_tab19(cmt_pegl_w[30:24]);
                    n_pegtgt[tgt] <= peg_tab19(cmt_pegl_w[6:0]);
                    n_pegris[tgt] <= (peg_tab19(cmt_pegl_w[6:0])
                                      > peg_tab19(cmt_pegl_w[30:24]));
                    n_peginc[tgt] <= peg_rate_inc(cmt_pegr_w[6:0]);
                end else begin
                    // DEC-015: re-leg segment 0 from the CURRENT level
                    n_pegtgt[tgt] <= peg_tab19(cmt_pegl_w[6:0]);
                    n_pegris[tgt] <= (peg_tab19(cmt_pegl_w[6:0])
                                      > $signed(n_peglvl[tgt]));
                    n_peginc[tgt] <= peg_rate_inc(cmt_pegr_w[6:0]);
                end
                n_pegix[tgt]  <= 3'd0;
                n_pegdwn[tgt] <= 1'b1;
                fc_state <= F_NN_BOOK;
            end
            // ------------------------- strike: bookkeeping + LFO keydown
            F_NN_BOOK: begin
                seq_ctr <= seq_ctr + 8'd1;
                n_seq[tgt]     <= seq_ctr;
                n_midi[tgt]    <= stg_note;
                n_vel[tgt]     <= stg_vel;
                n_dd[tgt][0]   <= stg_dd[0];
                n_dd[tgt][1]   <= stg_dd[1];
                n_dd[tgt][2]   <= stg_dd[2];
                n_dd[tgt][3]   <= stg_dd[3];
                n_dd[tgt][4]   <= stg_dd[4];
                n_dd[tgt][5]   <= stg_dd[5];
                n_live[tgt]    <= 1'b1;
                n_keydown[tgt] <= 1'b1;
                n_sustained[tgt] <= sustain;
                n_sil[tgt]     <= 7'd0;
                n_reclaimed[tgt] <= 1'b0;
                n_used[tgt]    <= 1'b1;
                if (kind == 2'd0 && !n_used[tgt]) begin
                    // fresh slot: constructor state (phases/gain_out/fb = 0)
                    for (fi = 0; fi < 6; fi = fi + 1) begin
                        op_ph[tgt][fi] <= 32'd0;
                        op_go[tgt][fi] <= 16'd0;
                    end
                    n_fb0[tgt] <= 32'd0;
                    n_fb1[tgt] <= 32'd0;
                end
                if (c_okeysync) begin
                    // Dx7Note::oscSync on every struck slot (patch[136])
                    for (fi = 0; fi < 6; fi = fi + 1) begin
                        op_ph[tgt][fi] <= 32'd0;
                        op_go[tgt][fi] <= 16'd0;
                    end
                end
                // the SHARED lfo.keydown(): every strike, sync or not
                lfo_dstate <= 32'd0;
                if (c_lfo_sync)
                    lfo_phase <= 32'h7FFFFFFF;
                fc_state <= F_EVPOP;
            end
            // ---------------------------------------------------- note off
            F_OFFFIND: begin
                if (n_keydown[scan] && (n_midi[scan] == stg_note)) begin
                    tgt <= scan;
                    fc_state <= F_OFFAPPLY;
                end else if (scan == 4'd15) begin
                    fc_state <= F_EVPOP;
                end else begin
                    scan <= scan + 4'd1;
                end
            end
            F_OFFAPPLY: begin
                if (|tgt_busy) begin
                    // wait: the slot's units may still be executing this
                    // tick's strike/update commands
                end else begin
                    n_keydown[tgt] <= 1'b0;
                    if (sustain) begin
                        n_sustained[tgt] <= 1'b1;   // DEC-018: deferred keyup
                    end else begin
                        for (fi = 0; fi < 6; fi = fi + 1) begin
                            env_cmd[tgt*6+fi]  <= EC_KEYDOWN;
                            env_cmd_data[tgt*6+fi] <= 133'd0;  // keydown(False)
                            env_cmd_en[tgt*6+fi] <= 1'b1;
                        end
                        // PitchEnv.keydown(False): advance(3) on the edge
                        if (n_pegdwn[tgt]) begin
                            n_pegdwn[tgt] <= 1'b0;
                            n_pegix[tgt]  <= 3'd3;
                            n_pegtgt[tgt] <= peg_tab19(n_pegl[tgt][3]);
                            n_pegris[tgt] <= (peg_tab19(n_pegl[tgt][3])
                                              > $signed(n_peglvl[tgt]));
                            n_peginc[tgt] <= peg_rate_inc(n_pegr[tgt][3]);
                        end
                    end
                    fc_state <= F_EVPOP;
                end
            end
            // ---------------------------------------- sustain pedal release
            F_PEDUP: begin
                if (scan == 4'd15 && !(n_sustained[15] && !n_keydown[15])) begin
                    fc_state <= F_EVPOP;
                end else if (!n_sustained[scan] || n_keydown[scan]) begin
                    scan <= scan + 4'd1;
                end else if (|scan_busy) begin
                    // wait: this tick's commands may still be in flight
                end else begin
                    n_sustained[scan] <= 1'b0;
                    for (fj = 0; fj < 6; fj = fj + 1) begin
                        env_cmd[scan*6+fj]  <= EC_KEYDOWN;
                        env_cmd_data[scan*6+fj] <= 133'd0;
                        env_cmd_en[scan*6+fj] <= 1'b1;
                    end
                    if (n_pegdwn[scan]) begin
                        n_pegdwn[scan] <= 1'b0;
                        n_pegix[scan]  <= 3'd3;
                        n_pegtgt[scan] <= peg_tab19(n_pegl[scan][3]);
                        n_pegris[scan] <= (peg_tab19(n_pegl[scan][3])
                                           > $signed(n_peglvl[scan]));
                        n_peginc[scan] <= peg_rate_inc(n_pegr[scan][3]);
                    end
                    scan <= scan + 4'd1;
                end
            end
            // ---------------------------------------- all-notes-off (CC123)
            F_ANO: begin
                if (scan == 4'd15 && !n_keydown[15]) begin
                    fc_state <= F_EVPOP;
                end else if (!n_keydown[scan]) begin
                    scan <= scan + 4'd1;
                end else if (|scan_busy) begin
                    // wait: this tick's commands may still be in flight
                end else begin
                    n_keydown[scan] <= 1'b0;
                    if (sustain) begin
                        n_sustained[scan] <= 1'b1;
                    end else begin
                        for (fj = 0; fj < 6; fj = fj + 1) begin
                            env_cmd[scan*6+fj]  <= EC_KEYDOWN;
                            env_cmd_data[scan*6+fj] <= 133'd0;
                            env_cmd_en[scan*6+fj] <= 1'b1;
                        end
                        if (n_pegdwn[scan]) begin
                            n_pegdwn[scan] <= 1'b0;
                            n_pegix[scan]  <= 3'd3;
                            n_pegtgt[scan] <= peg_tab19(n_pegl[scan][3]);
                            n_pegris[scan] <= (peg_tab19(n_pegl[scan][3])
                                               > $signed(n_peglvl[scan]));
                            n_peginc[scan] <= peg_rate_inc(n_pegr[scan][3]);
                        end
                    end
                    scan <= scan + 4'd1;
                end
            end
            // --------------------------------------------------- soft reset
            F_SOFT: begin
                n_live <= 16'd0; n_keydown <= 16'd0; n_sustained <= 16'd0;
                n_reclaimed <= 16'd0; n_used <= 16'd0; n_silnow <= 16'd0;
                cur_note <= 4'd0; seq_ctr <= 8'd0; sustain <= 1'b0;
                refresh_pending <= 1'b1;
                lfo_phase <= 32'd0; lfo_delta <= 32'd0; lfo_dstate <= 32'd0;
                lfo_dinc1 <= 32'd0; lfo_dinc2 <= 32'd0; lfo_rand <= 8'd0;
                lfo_val_q <= 25'd0; lfo_del_q <= 25'd0;
                for (fi = 0; fi < 16; fi = fi + 1) begin
                    n_sil[fi] <= 7'd0;
                    n_peglvl[fi] <= 32'd0; n_pegtgt[fi] <= 32'd0;
                    n_peginc[fi] <= 32'd0; n_pegix[fi] <= 3'd0;
                    n_pegris[fi] <= 1'b0; n_pegdwn[fi] <= 1'b1;
                    n_fb0[fi] <= 32'd0; n_fb1[fi] <= 32'd0;
                    for (fj = 0; fj < 6; fj = fj + 1) begin
                        n_bp[fi][fj] <= 32'd0; n_dd[fi][fj] <= 32'd0;
                        op_ph[fi][fj] <= 32'd0; op_go[fi][fj] <= 16'd0;
                        op_fq[fi][fj] <= 32'd0; op_li[fi][fj] <= 32'd0;
                    end
                end
                for (fj = 0; fj < 64; fj = fj + 1) begin
                    mixbuf[in_sel][fj] <= 33'sd0;  // compute buffer only:
                end                                // the previous frame's
                fc_state <= F_EVPOP;               // output must still
            end                                    // stream (H03 4.5)
            // ------------------------- first-block refresh (wrapper quirk)
            F_REFSEL: begin
                if (scan == 4'd15 && !n_live[scan]) begin
                    refresh_pending <= 1'b0;
                    scan <= 4'd0;
                    opk <= 3'd0;
                    fc_state <= F_ENVGO;
                end else if (n_live[scan]) begin
                    nn <= scan;
                    opk <= 3'd0;
                    fc_state <= F_REFOP;
                end else begin
                    scan <= scan + 4'd1;
                end
            end
            F_REFOP: begin
                if (env_busy[nn*6+opk]) begin
                    // wait for the unit's earlier command to land
                end else begin
                    env_cmd[nn*6+opk]  <= EC_UPDATE;
                    env_cmd_data[nn*6+opk] <=
                        {dv_r0, dv_r1, dv_r2, dv_r3,
                         dv_l0, dv_l1, dv_l2, dv_l3,
                         dv_outlevel, dv_rs_eff};
                    env_cmd_en[nn*6+opk] <= 1'b1;
                    if (opk == 3'd5) begin
                        opk <= 3'd0;
                        fc_state <= F_REFOP2;
                    end else
                        opk <= opk + 3'd1;
                end
            end
            F_REFOP2: begin
                if (scan == 4'd15) begin
                    refresh_pending <= 1'b0;
                    scan <= 4'd0;
                    opk <= 3'd0;
                    fc_state <= F_ENVGO;
                end else begin
                    scan <= scan + 4'd1;
                    fc_state <= F_REFSEL;
                end
            end
            // ------------------------------------ parallel envelope stepping
            // (walked per unit: a command is accepted only while idle, so
            // each unit is stepped as soon as its previous command landed)
            F_ENVGO: begin
                if (!n_live[scan]) begin
                    if (scan == 4'd15) begin
                        fc_state <= F_ENVW;
                    end else begin
                        scan <= scan + 4'd1;
                    end
                end else if (env_busy[scan*6+opk]) begin
                    // wait for this unit's earlier command to land
                end else begin
                    env_cmd[scan*6+opk]  <= EC_STEP;
                    env_cmd_en[scan*6+opk] <= 1'b1;
                    if (opk == 3'd5) begin
                        opk <= 3'd0;
                        if (scan == 4'd15) begin
                            fc_state <= F_ENVW;
                        end else begin
                            scan <= scan + 4'd1;
                        end
                    end else begin
                        opk <= opk + 3'd1;
                    end
                end
            end
            F_ENVW: begin
                if (env_busy == 96'd0)
                    fc_state <= F_LFO;
            end
            // ------------------------------------------------ shared LFO step
            F_LFO: begin
                lfo_phase <= l_ph_n;
                lfo_val_q <= l_wave_v;
                lfo_rand  <= (c_lfo_wave == 3'd5) ? l_rand_nx : lfo_rand;
                if (l_dsum[32])
                    lfo_del_q <= l_dly_sat;
                else begin
                    lfo_dstate <= l_dsum[31:0];
                    lfo_del_q  <= l_dly_nrm;
                end
                pbase_w <= bend_pb + master_tune;
                nn <= 4'd0;
                scan <= 4'd0;
                fc_state <= F_TAILSEL;
            end
            // ---------------------------------------- per-note control tail
            F_TAILSEL: begin
                if (scan == 4'd15 && !n_live[scan]) begin
                    nn <= 4'd0; scan <= 4'd0;
                    fc_state <= F_EVALSEL;
                end else if (n_live[scan]) begin
                    nn <= scan;
                    w_pmdep  <= n_pmdep[scan];
                    w_pmsens <= n_pmsens[scan];
                    w_amdep  <= n_amdep[scan];
                    fc_state <= F_TAILPEG;
                end else begin
                    scan <= scan + 4'd1;
                end
            end
            F_TAILPEG: begin
                w_peg <= t_peg_nxt;
                fc_state <= F_TAILPEG2;
            end
            F_TAILPEG2: begin
                if (t_peg_adv) begin
                    n_pegix[nn]  <= n_pegix[nn] + 3'd1;
                    n_pegtgt[nn] <= peg_tab19(n_pegl[nn][n_pegix[nn] + 3'd1]);
                    n_pegris[nn] <= (peg_tab19(n_pegl[nn][n_pegix[nn] + 3'd1])
                                     > $signed(t_peg_nxt));
                    n_peginc[nn] <= peg_rate_inc(n_pegr[nn][n_pegix[nn]
                                                                 + 3'd1]);
                end
                fc_state <= F_TAILPM;
            end
            F_TAILPM: begin
                pm_nobase_w <= w_pmnobase;
                fc_state <= F_TAILAM;
            end
            F_TAILAM: begin
                amd_w <= w_amd;
                opk <= 3'd0;
                fc_state <= F_TAILFQ1;
            end
            F_TAILFQ1: begin
                w_level <= env_level(env_rd[nn*6+opk]);
                if (n_ams[nn][opk] != 25'd0) begin
                    exp_amd  <= amd_w[24:0];
                    exp_ams  <= n_ams[nn][opk];
                    exp_start <= 1'b1;
                    fc_state <= F_TAILEXP;
                end else begin
                    w_li <= env_level(env_rd[nn*6+opk]);
                    fc_state <= F_TAILLI;
                end
            end
            F_TAILEXP: begin
                exp_start <= 1'b0;
                if (exp_done) begin
                    w_li <= exp_li;
                    fc_state <= F_TAILLI;
                end
            end
            F_TAILLI: begin
                op_li[nn][opk] <= w_li;
                // Freqlut index/shift capture (freqlut.cc:46-55)
                fq_sel <= n_bp[nn][opk]
                        + (n_opmode[nn][opk] ? pbase_w
                                             : (pm_nobase_w + pbase_w));
                fc_state <= F_TAILFQ2;
            end
            F_TAILFQ2: begin
                w_fa  <= fq_sel[23:14];
                w_fl  <= fq_sel[13:0];
                w_fsh <= fq_fsh_c;
                fc_state <= F_TAILFQ3;
            end
            F_TAILFQ3: begin
                op_fq[nn][opk] <= fq_val;
                if (opk == 3'd5) begin
                    if (scan == 4'd15) begin
                        nn <= 4'd0; scan <= 4'd0;
                        fc_state <= F_EVALSEL;
                    end else begin
                        scan <= scan + 4'd1;
                        fc_state <= F_TAILSEL;
                    end
                end else begin
                    opk <= opk + 3'd1;
                    fc_state <= F_TAILFQ1;
                end
            end
            // ------------------------------------------------------ eval pass
            F_EVALSEL: begin
                if (scan == 4'd15 && !n_live[scan]) begin
                    fc_state <= F_RECLAIM;
                end else if (n_live[scan]) begin
                    nn <= scan;
                    opk <= 3'd0;
                    fc_state <= F_EVALLD;
                end else begin
                    scan <= scan + 4'd1;
                end
            end
            F_EVALLD: begin
                case (opk)
                    3'd0: begin
                        r_ld_cfg <= 1'b1;
                        r_cfg_algorithm <= n_alg[nn];
                        r_cfg_feedback  <= n_fbs[nn] < 5'd8
                                           ? (n_fbs[nn] == 5'd16 ? 5'd0
                                                                : 5'd8
                                                                  - n_fbs[nn])
                                           : 5'd0;
                        r_ld_fb <= 1'b1;
                        r_cfg_fb0 <= n_fb0[nn];
                        r_cfg_fb1 <= n_fb1[nn];
                        opk <= 3'd1;
                    end
                    3'd1, 3'd2, 3'd3, 3'd4, 3'd5, 3'd6: begin
                        r_ld_op       <= 1'b1;
                        r_ld_op_idx   <= opk - 3'd1;
                        r_ld_phase    <= op_ph[nn][opk-3'd1];
                        r_ld_freq     <= op_fq[nn][opk-3'd1];
                        r_ld_level    <= op_li[nn][opk-3'd1][27:0];
                        r_ld_gain_out <= op_go[nn][opk-3'd1];
                        opk <= opk + 3'd1;
                    end
                    default: begin
                        r_ld_cfg <= 1'b0; r_ld_fb <= 1'b0; r_ld_op <= 1'b0;
                        r_start <= 1'b1;
                        fc_state <= F_EVALRUN;
                    end
                endcase
            end
            F_EVALRUN: begin
                r_start <= 1'b0;
                r_ld_cfg <= 1'b0; r_ld_fb <= 1'b0; r_ld_op <= 1'b0;
                if (ar_frame_done) begin
                    r_rd_addr <= 7'd0;
                    fc_state <= F_EVALMIX;
                end
            end
            F_EVALMIX: begin
                mixbuf[in_sel][r_rd_addr[5:0]]
                    <= mixbuf[in_sel][r_rd_addr[5:0]] + cv_mix_g;
                r_rd_addr <= r_rd_addr + 7'd1;
                if (r_rd_addr == 7'd63) begin
                    rb_cnt <= 3'd0;
                    r_rd_addr <= 7'd64;
                    fc_state <= F_EVALRB;
                end
            end
            F_EVALRB: begin
                // rd 64..69 phase, 70..75 gain_out (op = rb order:
                // rd-64 / rd-70 is the op index), 76/77 fb
                if (r_rd_addr < 7'd70)
                    op_ph[nn][r_rd_addr - 7'd64] <= ar_rd_data;
                else if (r_rd_addr < 7'd76)
                    op_go[nn][r_rd_addr - 7'd70] <= ar_rd_data[15:0];
                else if (r_rd_addr == 7'd76)
                    n_fb0[nn] <= ar_rd_data;
                else begin
                    n_fb1[nn] <= ar_rd_data;
                    n_silnow  <= note_silent;
                    if (scan == 4'd15)
                        fc_state <= F_RECLAIM;
                    else begin
                        scan <= scan + 4'd1;
                        fc_state <= F_EVALSEL;
                    end
                end
                r_rd_addr <= r_rd_addr + 7'd1;
            end
            // ----------------------------------------------- NUM-012 reclaim
            F_RECLAIM: begin
                for (fi = 0; fi < 16; fi = fi + 1) begin
                    if (n_live[fi]) begin
                        if (note_silent[fi]) begin
                            if (n_sil[fi] + 7'd1 >= 7'd64) begin
                                n_live[fi] <= 1'b0;
                                n_reclaimed[fi] <= 1'b1;
                                n_sil[fi] <= 7'd0;
                            end else
                                n_sil[fi] <= n_sil[fi] + 7'd1;
                        end else
                            n_sil[fi] <= 7'd0;
                    end else
                        n_sil[fi] <= 7'd0;
                end
                fc_state <= F_IDLE;
            end
            default: fc_state <= F_IDLE;
            endcase
        end
    end

    // =====================================================================
    // I2S master transmitter: BCLK = clk/4, LRCLK = clk/256, data delayed
    // 1 BCLK after each LRCLK edge, 24-bit left-justified field (the full
    // unclamped mix; see the header deviation note), duplicated on both
    // slots (DEC-008).  The wire streams out_sel (the previous compute
    // frame); sample s = mixbuf[out_sel][s].
    // =====================================================================
    reg [1:0]  bclk_div;
    reg        bclk_q, lr_q;
    reg [7:0]  half_cnt;      // 0..255 clks per LR half
    reg [5:0]  falls;         // BCLK falling edges within this half
    reg [23:0] i2s_sh;
    reg [5:0]  i2s_smp;
    wire       bclk_fall = (bclk_div == 2'd3);
    always @(posedge clk) begin
        if (rst) begin
            bclk_div <= 2'd0; bclk_q <= 1'b0; lr_q <= 1'b0;
            half_cnt <= 8'd0; falls <= 6'd0;
            i2s_sh <= 24'd0; i2s_smp <= 6'd0;
        end else begin
            if (bclk_div == 2'd3) begin
                bclk_div <= 2'd0;
                bclk_q <= ~bclk_q;
            end else
                bclk_div <= bclk_div + 2'd1;
            if (half_cnt == 8'd255) begin
                half_cnt <= 8'd0;
                lr_q <= ~lr_q;
                falls <= 6'd0;
                if (!lr_q) begin   // entering the LEFT half: load sample s
                    i2s_sh <= {mixbuf[out_sel][i2s_smp][21:0], 2'b00};
                    if (i2s_smp == 6'd63)
                        i2s_smp <= 6'd0;
                    else
                        i2s_smp <= i2s_smp + 6'd1;
                end
            end else begin
                half_cnt <= half_cnt + 8'd1;
                if (bclk_fall) begin
                    falls <= falls + 6'd1;
                    if (falls >= 6'd1 && falls <= 6'd24)
                        i2s_sh <= {i2s_sh[22:0], 1'b0};
                end
            end
        end
    end
    assign i2s_bclk  = bclk_q;
    assign i2s_lrclk = lr_q;
    assign i2s_d     = i2s_sh[23];
    wire mix_load = (half_cnt == 8'd255) && !lr_q;
    // tap: the exact sample the wire carries in this slot (the value being
    // loaded at the mix_load edge, seen one cycle early in the clk domain)
    wire [5:0] tap_smp = i2s_smp;
    assign tap_mix         = mix_load ? {1'b0, mixbuf[out_sel][tap_smp]}
                             : 23'd0;
    assign tap_mix_valid   = mix_load;

    // =====================================================================
    // Taps + observability (H03 4.6: read-only, non-invasive; load-bearing)
    // =====================================================================
    assign tap_frame       = frame_ctr;
    assign tap_live        = n_live;
    assign status_overrun  = st_overrun;
    assign status_overflow = st_overflow;

`ifdef H07_STRIP_OBSERVABILITY
    // NEGATIVE CONTROL: no output observable, no reduction -- synthesis
    // must (and does) delete every state flop; the flop gate then FAILS.
    assign tap_frame = 16'h0;
    assign tap_live = 16'h0;
    assign tap_mix = 23'h0;
    assign tap_mix_valid = 1'b0;
    assign spi_miso = 1'b0;
    assign i2s_bclk = 1'b0;
    assign i2s_lrclk = 1'b0;
    assign i2s_d = 1'b0;
    assign status_overrun = 1'b0;
    assign status_overflow = 1'b0;
    assign state_obs = 1'b0;
`else
    reg obs_q;
    wire [255:0] nb_obs;
    genvar gnb;
    generate
        for (gnb = 0; gnb < 16; gnb = gnb + 1) begin : nbgen
            assign nb_obs[gnb*16 +: 16] =
                n_peglvl[gnb][3:0] ^ n_pegtgt[gnb][3:0]
                ^ n_peginc[gnb][3:0] ^ {n_pegris[gnb], n_pegdwn[gnb],
                                        n_pegix[gnb], n_sil[gnb][6:0],
                                        n_midi[gnb], n_vel[gnb], n_used[gnb],
                                        n_keydown[gnb], n_live[gnb],
                                        n_sustained[gnb], n_reclaimed[gnb]};
        end
    endgenerate
    reg [31:0] mixbuf_obs0, mixbuf_obs1;
    integer ob;
    always @* begin
        mixbuf_obs0 = 32'h0;
        mixbuf_obs1 = 32'h0;
        for (ob = 0; ob < 64; ob = ob + 1) begin
            mixbuf_obs0 = mixbuf_obs0 ^ mixbuf[0][ob];
            mixbuf_obs1 = mixbuf_obs1 ^ mixbuf[1][ob];
        end
    end
    wire obs_next =
        (^frame_ctr) ^ (^sample_clk) ^ (^sample_idx)
        ^ (^{st_overrun, st_overflow, st_fresh})
        ^ (^skid_cnt) ^ (^evq_cnt)
        ^ (^n_live) ^ (^n_keydown) ^ (^n_sustained) ^ (^n_reclaimed)
        ^ (^n_used) ^ (^n_silnow) ^ (^{cur_note, seq_ctr}) ^ sustain
        ^ refresh_pending ^ in_sel ^ out_sel ^ (^lfo_phase) ^ (^lfo_delta)
        ^ (^lfo_dstate) ^ (^lfo_dinc1) ^ (^lfo_dinc2) ^ (^lfo_rand)
        ^ (^lfo_val_q) ^ (^lfo_del_q)
        ^ (^bend_pb) ^ (^master_tune) ^ (^ctrl_pm) ^ (^ctrl_am) ^ (^eg_mod)
        ^ (^mixbuf_obs0) ^ (^mixbuf_obs1)
        ^ (^shd_globA) ^ (^shd_globB) ^ (^shd_globC)
        ^ (^shd_pegr) ^ (^shd_pegl) ^ shd_commit
        ^ (^cmt_globA) ^ (^cmt_globB) ^ (^cmt_globC)
        ^ (^cmt_pegr) ^ (^cmt_pegl)
        ^ (^fc_state) ^ (^nn) ^ (^opk) ^ (^kind) ^ (^tgt) ^ (^scan)
        ^ (^{stg_note, stg_vel}) ^ (^{stg_ctrl, stg_val})
        ^ (^pm_nobase_w) ^ (^pbase_w) ^ (^amd_w) ^ (^w_peg)
        ^ (^{w_pmdep, w_pmsens, w_amdep})
        ^ (^w_level) ^ (^w_li) ^ (^w_fa) ^ (^w_fl) ^ (^w_fsh)
        ^ (^fq_sel) ^ (^{r_ld_cfg, r_ld_fb, r_ld_op, r_start})
        ^ (^r_cfg_algorithm) ^ (^r_cfg_feedback)
        ^ (^r_cfg_fb0) ^ (^r_cfg_fb1) ^ (^r_ld_phase) ^ (^r_ld_freq)
        ^ (^{r_ld_op_idx, r_ld_level, r_ld_gain_out})
        ^ (^r_rd_addr) ^ (^mix_j) ^ (^rb_cnt)
        ^ (^bclk_div) ^ bclk_q ^ lr_q ^ (^half_cnt) ^ (^falls)
        ^ (^i2s_sh) ^ (^i2s_smp)
        ^ (^{n_midi[0], n_midi[1], n_midi[2], n_midi[3]})
        ^ (^{n_midi[4], n_midi[5], n_midi[6], n_midi[7]})
        ^ (^{n_midi[8], n_midi[9], n_midi[10], n_midi[11]})
        ^ (^{n_midi[12], n_midi[13], n_midi[14], n_midi[15]})
        ^ (^{n_seq[0], n_seq[1], n_seq[2], n_seq[3]})
        ^ (^{n_seq[4], n_seq[5], n_seq[6], n_seq[7]})
        ^ (^{n_seq[8], n_seq[9], n_seq[10], n_seq[11]})
        ^ (^{n_seq[12], n_seq[13], n_seq[14], n_seq[15]})
        ^ (^nb_obs);
    always @(posedge clk) begin
        if (rst)
            obs_q <= 1'b0;
        else
            obs_q <= obs_next;
    end
    assign state_obs = obs_q ^ ar_state_obs;
`endif

    // note-bank array observability: the per-note arrays are folded into
    // obs_next above via nb_obs (declared before the FSM)
endmodule
`default_nettype wire
