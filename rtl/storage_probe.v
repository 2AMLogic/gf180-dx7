// storage_probe.v -- H02 full-state storage feasibility probe (issue #21).
//
// One instance carries the ENTIRE projected 16-note state image from N01's
// storage classification (docs/DECISIONS-NUMERIC-v1.md section 6, machine
// copy spec/numeric-profile-v1.json "storage"): 96 operator-state banks of
// 325 bits, 16 note-state banks of 446 bits, and one 444-bit patch-shared
// store -- 38,336 replicated runtime bits + 444 patch bits = 38,780 flops of
// state, plus one registered observability flop. The field widths below are
// parameters mirroring the JSON field list in order; tests/test_storage_probe.py
// re-derives the JSON sums and cross-checks them against these parameters, so
// a silent width drift fails the suite.
//
// Organization: write-bank / read-mux. ONE write port (wr_cls selects the
// class: operator bank, note bank or patch) and the read structure the N01
// schedule needs at its assumed 1 evaluation/cycle access: every cycle the
// op-eval slot reads one 325-bit operator bank and the owning note's 446-bit
// bank (phase/freq/gain state; fb_buf_ and basepitch_ for the feedback and
// routing path), while the single write port carries that slot's writeback
// (phase) and the once-per-frame envelope commits. Patch is committed at
// D00 event time, not per slot. Bank decode is per-bank and constant-offset
// (generate blocks / unrolled loops, no address arithmetic): the probe
// measures storage, not a decoder's multiplier tree.
//
// OBSERVABILITY IS LOAD-BEARING: every state bit feeds its bank's read mux
// and the patch bus, and the mux outputs are XOR-folded into `state_obs`,
// one registered reduction bit -- so synthesis cannot optimize unused state
// away. The negative control proves it: compile with
// -DSTORAGE_PROBE_STRIP_OBSERVABILITY and every read/observability path is
// removed; yosys then deletes the state and tools/storage_probe_synth.py's
// flop-count gate FAILS (tests/test_storage_probe.py runs both directions).
//
// This is a storage probe only: it computes nothing, and functional fidelity
// to the frozen model is N-series scope. No fit/timing/slot claim survives
// from this file alone -- see docs/H02-STORAGE-REPORT.md for what it does
// and does not say.
`default_nettype none

module storage_probe #(
    // instances (spec/numeric-profile-v1.json storage.instances)
    parameter NUM_OPS   = 96,
    parameter NUM_NOTES = 16,
    // per-operator field widths, in spec/numeric-profile-v1.json
    // storage.per_operator_runtime_state_bits order (325 bits total)
    parameter W_OP_PHASE        = 32,  // FmOpParams.phase
    parameter W_OP_FREQ         = 32,  // FmOpParams.freq
    parameter W_OP_GAIN_OUT     = 32,  // FmOpParams.gain_out
    parameter W_OP_LEVEL_IN     = 32,  // FmOpParams.level_in
    parameter W_OP_ENV_LEVEL    = 32,  // Env.level_ (Q24)
    parameter W_OP_ENV_TARGET   = 32,  // Env.targetlevel_
    parameter W_OP_ENV_INC      = 32,  // Env.inc_
    parameter W_OP_ENV_STATIC   = 32,  // Env.staticcount_ (ACCURATE_ENVELOPE)
    parameter W_OP_ENV_OUTLEVEL = 32,  // effective outlevel_ (per-note derived)
    parameter W_OP_ENV_RATE_SC  = 32,  // effective rate_scaling_ (per-note derived)
    parameter W_OP_ENV_IX       = 3,   // Env.ix_
    parameter W_OP_ENV_RISING   = 1,   // Env.rising_
    parameter W_OP_ENV_DOWN     = 1,   // Env.down_
    // per-note field widths, storage.per_note_runtime_state_bits order
    // (446 bits total)
    parameter W_N_BASEPITCH_6   = 192, // basepitch_[6]
    parameter W_N_FB_BUF_2      = 64,  // fb_buf_[2] (NUM-006 history)
    parameter W_N_PENV_LEVEL    = 32,  // PitchEnv.level_
    parameter W_N_PENV_TARGET   = 32,  // PitchEnv.targetlevel_
    parameter W_N_PENV_INC      = 32,  // PitchEnv.inc_
    parameter W_N_PENV_IX       = 3,   // PitchEnv.ix_
    parameter W_N_PENV_RISING   = 1,   // PitchEnv.rising_
    parameter W_N_PENV_DOWN     = 1,   // PitchEnv.down_
    parameter W_N_NOTELOGFREQ   = 32,  // noteLogFreq
    parameter W_N_MIDI_NOTE     = 7,   // playingMidiNote
    parameter W_N_MIDI_CH       = 4,   // midiChannel
    parameter W_N_MPE_BEND      = 14,  // mpePitchBend
    parameter W_N_MPE_PRESSURE  = 7,   // mpePressure
    parameter W_N_MPE_TIMBRE    = 7,   // mpeTimbre
    parameter W_N_VOICE_FLAGS   = 3,   // keydown/sustained/live
    parameter W_N_STEAL_AGE     = 15,  // allocator bookkeeping estimate
    // patch-shared widths, storage.patch_shared_state_bits order
    // (444 bits total, one shared copy -- justified member-by-member in N01)
    parameter W_P_ENV_RATES     = 168, // Env.rates_[4] x 6 ops
    parameter W_P_ENV_LEVELS    = 168, // Env.levels_[4] x 6 ops
    parameter W_P_PENV_RATES    = 28,  // pitch EG rates_[4]
    parameter W_P_PENV_LEVELS   = 28,  // pitch EG levels_[4]
    parameter W_P_OPMODE_6      = 6,   // opMode[6]
    parameter W_P_AMPMODSENS_6  = 12,  // ampmodsens_[6]
    parameter W_P_ALGORITHM     = 5,   // algorithm_
    parameter W_P_FB_DEPTH      = 5,   // fb depth -> fb_shift_
    parameter W_P_MODDEPTH_SENS = 24,  // pitch/amp mod depths + sens
    // derived class widths
    parameter OP_W    = W_OP_PHASE + W_OP_FREQ + W_OP_GAIN_OUT + W_OP_LEVEL_IN
                      + W_OP_ENV_LEVEL + W_OP_ENV_TARGET + W_OP_ENV_INC
                      + W_OP_ENV_STATIC + W_OP_ENV_OUTLEVEL + W_OP_ENV_RATE_SC
                      + W_OP_ENV_IX + W_OP_ENV_RISING + W_OP_ENV_DOWN,
    parameter NOTE_W  = W_N_BASEPITCH_6 + W_N_FB_BUF_2 + W_N_PENV_LEVEL
                      + W_N_PENV_TARGET + W_N_PENV_INC + W_N_PENV_IX
                      + W_N_PENV_RISING + W_N_PENV_DOWN + W_N_NOTELOGFREQ
                      + W_N_MIDI_NOTE + W_N_MIDI_CH + W_N_MPE_BEND
                      + W_N_MPE_PRESSURE + W_N_MPE_TIMBRE + W_N_VOICE_FLAGS
                      + W_N_STEAL_AGE,
    parameter PATCH_W = W_P_ENV_RATES + W_P_ENV_LEVELS + W_P_PENV_RATES
                      + W_P_PENV_LEVELS + W_P_OPMODE_6 + W_P_AMPMODSENS_6
                      + W_P_ALGORITHM + W_P_FB_DEPTH + W_P_MODDEPTH_SENS,
    parameter OP_ALL_W   = NUM_OPS * OP_W,
    parameter NOTE_ALL_W = NUM_NOTES * NOTE_W,
    // bank index widths (don't-care above NUM_OPS-1 / NUM_NOTES-1)
    parameter OP_AW   = 7,
    parameter NOTE_AW = 4
)(
    input  wire               clk,
    // one write port (N01 access schedule: per-slot writeback + per-frame
    // envelope commits + event-time patch commits, one write per cycle)
    input  wire               wr_en,
    input  wire [1:0]         wr_cls,     // 0 op bank, 1 note bank, 2 patch
    input  wire [OP_AW-1:0]   wr_idx,     // bank within class (0 for patch)
    input  wire [NOTE_W-1:0]  wr_data,    // class-defined slice
    // read ports (assumed 1-eval/cycle access: each slot reads the slot's
    // operator bank and the owning note bank; patch read = full image)
    input  wire [OP_AW-1:0]   rd_op,
    input  wire [NOTE_AW-1:0] rd_note,
    output wire [OP_W-1:0]    op_rdata,
    output wire [NOTE_W-1:0]  note_rdata,
    output wire [PATCH_W-1:0] patch_rdata,
    // registered XOR-reduction of ALL state bits (every field observable)
    output wire               state_obs
);
    localparam [1:0] CLS_OP    = 2'd0;
    localparam [1:0] CLS_NOTE  = 2'd1;
    localparam [1:0] CLS_PATCH = 2'd2;

    // the full projected state image: 38,336 replicated runtime bits
    // (96 x 325 operator + 16 x 446 note) + 444 patch-shared bits
    reg [OP_ALL_W-1:0]    op_state;
    reg [NOTE_ALL_W-1:0]  note_state;
    reg [PATCH_W-1:0]     patch_state;

    // one write port: per-bank constant-offset decode (no address multiply)
    genvar g;
    generate
        for (g = 0; g < NUM_OPS; g = g + 1) begin : g_op_wr
            localparam [OP_AW-1:0] BANK = g;
            always @(posedge clk)
                if (wr_en && wr_cls == CLS_OP && wr_idx == BANK)
                    op_state[g*OP_W +: OP_W] <= wr_data[OP_W-1:0];
        end
        for (g = 0; g < NUM_NOTES; g = g + 1) begin : g_note_wr
            localparam [OP_AW-1:0] BANK = g;
            always @(posedge clk)
                if (wr_en && wr_cls == CLS_NOTE && wr_idx == BANK)
                    note_state[g*NOTE_W +: NOTE_W] <= wr_data;
        end
    endgenerate
    always @(posedge clk)
        if (wr_en && wr_cls == CLS_PATCH)
            patch_state <= wr_data[PATCH_W-1:0];

`ifdef STORAGE_PROBE_STRIP_OBSERVABILITY
    // negative control: no read data, no reduction -- the state has no
    // observer, so synthesis must (and does) remove every state flop.
    assign op_rdata    = {OP_W{1'b0}};
    assign note_rdata  = {NOTE_W{1'b0}};
    assign patch_rdata = {PATCH_W{1'b0}};
    assign state_obs   = 1'b0;
`else
    // async bank read muxes (unrolled, constant offsets)
    reg [OP_W-1:0]   op_rd;
    reg [NOTE_W-1:0] note_rd;
    integer j;
    always @* begin
        op_rd = {OP_W{1'b0}};
        for (j = 0; j < NUM_OPS; j = j + 1)
            if (rd_op == j[OP_AW-1:0])
                op_rd = op_state[j*OP_W +: OP_W];
        note_rd = {NOTE_W{1'b0}};
        for (j = 0; j < NUM_NOTES; j = j + 1)
            if (rd_note == j[NOTE_AW-1:0])
                note_rd = note_state[j*NOTE_W +: NOTE_W];
    end

    assign op_rdata    = op_rd;
    assign note_rdata  = note_rd;
    assign patch_rdata = patch_state;

    reg obs_q;
    wire obs_next = (^op_rd) ^ (^note_rd) ^ (^patch_state);
    always @(posedge clk) obs_q <= obs_next;
    assign state_obs = obs_q;
`endif

endmodule

`default_nettype wire
