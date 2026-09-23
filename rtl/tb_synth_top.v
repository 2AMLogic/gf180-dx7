// H08: pin-level behavioral testbench for rtl/synth_top.v (9-pad chassis).
//
// This bench talks to the DUT ONLY through the nine top-level pins and never
// references an internal dut.* signal (see docs/H08-PIN.md, anti-circularity
// note). Structural expectations are from the H08 issue + frozen contract
// D00 (32:1 BCLK:LR, even-512-clk frames, mode-0 SPI); PACE decisions may
// observe the I2S pads (the DUT's own output waveform).
//
// Directions exercised:
//   * SPI: bit-banged mode-0 master (page/event writes + 32-bit status
//     reads). The boundary-compliant half-period is 2 clks (contract
//     SCK <= f/4; 194 clk/transaction, same as the H07 flat bench).
//   * I2S: passive capture of bclk/lrclk/d, decoded with the AS-BUILT
//     single-sample-per-DAC-sample framing (24-bit LJS in one lrclk high
//     half; the lrclk low half is all zeros; see synth_top.v header) and
//     appended little-endian to the actual dump.
//
// As-built timing (traced from the DUT I2S TX in rtl/dx7_core.v; BCLK
// period 8 clk — the DUT comment says clk/4, the as-built divider is
// clk/8, finding F-I2S-1):
//   * on the LR RISING edge at tick T the shift register loads
//     {sample[21:0], 2'b00} (24-bit LJS field, bits 23:22 are field
//     zeros); the BCLK fall at T+1 does not shift; the field shifts on
//     each BCLK fall from T+9;
//   * no BCLK rise exposes the MSB (s21): D carries it from T+1 to
//     T+8 only; it is captured one bench tick after the LR pad rise and
//     cross-checked against the first BCLK rise, which still shows it;
//   * BCLK rises 1..22 of the lr-HIGH half (T+5, T+13, ..., T+173)
//     carry s21..s0 (MSB first); rises 23..32 carry zeros; the whole
//     lr-LOW half is zeros (asserted in-bench);
//   * I2S D changes only on BCLK-fall ticks and at the LR load tick, is
//     stable while BCLK is high otherwise (asserted in-bench).
//
// Vector grammar (P/C/E/S/T/R/W identical semantics to tb_dx7_core.v;
// Q/X/Y/Z are H08 additions):
//  P <addr:2hex> <data:8hex>         SPI write, F=1 SEC=1 (page shadow)
//  E <block:dec> <addr:2hex> <data:8hex>
//                                    pace to absolute frame base+block,
//                                    SPI write F=1 SEC=0 (event)
//  C [block]                         optional pace (frame base+block), SPI
//                                    write F=1 SEC=0 0x42/0 (commit), record
//                                    base/commits from the pin frame counter
//  S [block]                         optional pace, SPI write F=1 SEC=0
//                                    0x0F (soft)
//  T <addr:2hex> <data:8hex>         tick-boundary write, F=1 SEC=0
//  R <n>                             wait n wire frames (lrclk periods)
//  W [n]                             run until the dump grows by n samples
//                                    (or reaches +expect), +1 flush frame
//  Q                                 32-bit status READ (MISO); meta line
//                                    "qread 0x<8hex>"; header 0xD71x is
//                                    asserted in-bench
//  X <sec:1hex> <addr:2hex> <data:8hex> <nbits:dec>
//                                    SPI transmit of only <nbits> bits of an
//                                    F=1 frame (truncation injection); CS
//                                    releases with no 48th rising edge
//  Y <n:dec> <addr:2hex> <data:8hex>
//                                    n SEC=1 page writes at SCK half-period
//                                    1 clks — a DELIBERATE > f/4 rate
//                                    violation (skid/event-queue overflow
//                                    injection, no events -> OVERRUN stays
//                                    0 and the PCM stays silent)
//  Z <n:dec>                         pad-level RST_N low pulse, n clks
//
// DONE is printed every run (injection or not). Any structural/protocol
// violation prints FAIL <reason> and $finish's without DONE. All meta
// signals are pin-derived (status from MISO, frames from the lrclk pad —
// never from dut.* internals).
`timescale 1ns / 1ps

module tb_synth_top;
    reg clk = 1'b0;
    reg rst_n = 1'b0;
    reg spi_cs_n = 1'b1;
    reg spi_sck = 1'b0;
    reg spi_mosi = 1'b0;
    wire spi_miso;
    wire i2s_bclk, i2s_lrclk, i2s_d;

    synth_top dut (
        .ref_clk(clk),
        .rst_n(rst_n),
        .spi_cs_n(spi_cs_n),
        .spi_sck(spi_sck),
        .spi_mosi(spi_mosi),
        .spi_miso(spi_miso),
        .i2s_bclk(i2s_bclk),
        .i2s_lrclk(i2s_lrclk),
        .i2s_d(i2s_d)
    );

    always #5 clk = ~clk;   // 10 ns period; timing is irrelevant to the check

    // --------------------------------------------------------------
    // vector file state (declared early: the decoder writes afd)
    // --------------------------------------------------------------
    integer vfd, afd, mfd;
    // 128-byte string width: the flat bench pinned this after absolutely-
    // pathed vectors right-fill fixed-width regs (narrower = silent
    // left-truncation of the path — the plusarg 'mangling' trap)
    reg [1023:0] vfile, afile, mfile;
    reg [255:0] line;
    integer rc, p0, pd, vdec;
    integer addr, data, block;
    reg [7:0]  tok;
    integer base_frame;
    integer commit_list[0:15];
    integer commit_n;
    integer expect_samples;
    reg [31:0] qword;

    // --------------------------------------------------------------
    // pin-truth edge detectors. All pulses fire one bench tick after
    // the pad edge tick, so captures read the settled post-edge pad
    // value (no same-tick NBA hazard):
    //   brise      : one tick after each BCLK pad rise;
    //   lr_rise_hi : one tick after the LR pad 0->1 edge;
    //   lr_fall_lo : one tick after the LR pad 1->0 edge.
    //
    // Timing ground truth (traced from the DUT I2S TX, rtl/dx7_core.v):
    //   * BCLK period = 8 clk, LRCLK period = 512 clk (32 BCLK cycles
    //     per LR half);
    //   * the LR pad rises at tick T: the shift register loads
    //     {sample[21:0], 2'b00} at T; the BCLK fall at T+1 does not
    //     shift; the field shifts on each BCLK fall from T+9;
    //   * D carries the MSB (s21) from T+1 to T+8 with no BCLK rise in
    //     that window; the lr-HIGH-half BCLK rises (T+5, T+13, ...,
    //     T+253) successively carry s21..s0 (rises 1..22) then zeros
    //     (rises 23..32); the lr-LOW half D is all zeros.
    // --------------------------------------------------------------
    reg bclk_d0 = 1'b0, bclk_d1 = 1'b0, lr_q_d = 1'b0;
    wire brise    = bclk_d0 & ~bclk_d1;    // bench tick after each bclk rise
    wire lr_rise_hi = i2s_lrclk & ~lr_q_d; // bench tick after the lr rising edge
    wire lr_fall_lo = ~i2s_lrclk & lr_q_d; // bench tick after the lr falling edge
    // one-posedge-delayed copies: at posedge P+1, (sig != sig_q) means the
    // signal transitioned at posedge P (the one the DAC latch samples).
    reg bclk_q = 1'b0;
    reg d_q = 1'b0;
    always @(posedge clk) begin
        if (!rst_n) begin
            bclk_d0 <= 1'b0;
            bclk_d1 <= 1'b0;
            lr_q_d  <= 1'b0;
            bclk_q  <= 1'b0;
            d_q     <= 1'b0;
        end else begin
            bclk_d0 <= i2s_bclk;
            bclk_d1 <= bclk_d0;
            lr_q_d  <= i2s_lrclk;
            bclk_q  <= i2s_bclk;
            d_q     <= i2s_d;
        end
    end

    // Measured as-built pin law (474692/474692 D transitions over the full
    // dir-base run): every i2s_d transition lands on the same posedge as an
    // i2s_bclk transition (the serialiser shifts on the same clk as the BCLK
    // output, so the data edge and the BCLK edge are clock-coincident and
    // zero-skew in silicon). A D transition WITHOUT a coincident BCLK edge
    // would indicate a mid-cell data drift that no DAC model tolerates.
    always @(posedge clk) begin
        if (rst_n && i2s_d !== d_q && i2s_bclk === bclk_q) begin
            $display("FAIL tb_synth_top (I2S D transition without BCLK edge)");
            $finish;
        end
    end

    // --------------------------------------------------------------
    // structural checks (tick-counter based):
    //   BCLK rise interval = 8 clk, LR rising-edge interval = 512 clk.
    // first_bit_delay := bench ticks from the lr-rise bench pulse to
    // the first high-half BCLK-rise bench pulse (as-built: 6; pad
    // edges are 5 clk apart).
    // --------------------------------------------------------------
    integer bclk_run = 0, bclk_count = 0, bclk_prev = -1;
    integer lr_run = 0, lr_count = 0, lr_prev = -1;
    reg [5:0] first_bit_delay = 6'd0;
    reg fb_recd = 1'b0;
    reg fb_wait = 1'b0;
    integer fb_ticks = 0;
    always @(posedge clk) begin
        if (!rst_n) begin
            bclk_run = 0;
            lr_run = 0;
            // re-arm the period monitors across a pad reset: the run
            // counters zero above, so the prev/count state must too, or
            // the first post-reset edge delta is computed across the
            // reset and trips the assert (observed: -33856).
            bclk_prev = -1;
            lr_prev = -1;
            bclk_count = 0;
            lr_count = 0;
        end else begin
            bclk_run = bclk_run + 1;
            lr_run = lr_run + 1;
            if (brise) begin
                if (bclk_count >= 2 && bclk_run - bclk_prev != 8) begin
                    $display("FAIL tb_synth_top (BCLK period %0d != 8 clk)",
                             bclk_run - bclk_prev);
                    $finish;
                end
                bclk_prev = bclk_run;
                bclk_count = bclk_count + 1;
                if (fb_wait) begin
                    fb_wait = 1'b0;
                    fb_recd = 1'b1;
                    first_bit_delay = (fb_ticks + 1) % 32;
                end
            end
            if (lr_rise_hi) begin
                if (lr_count >= 2 && lr_run - lr_prev != 512) begin
                    $display("FAIL tb_synth_top (LRCLK period %0d != 512 clk)",
                             lr_run - lr_prev);
                    $finish;
                end
                lr_prev = lr_run;
                lr_count = lr_count + 1;
                if (!fb_recd) begin
                    fb_wait = 1'b1;
                    fb_ticks = 0;
                end
            end
            if (fb_wait && !brise) begin
                fb_ticks = fb_ticks + 1;
            end
        end
    end

    // --------------------------------------------------------------
    // I2S decoder, as-built format (EMPIRICALLY VERIFIED against a
    // verbatim model of the dx7_core I2S TX):
    //   LRCLK = clk/256 (half_cnt 0..255); the new sample loads a 24-bit
    //   field {mix22, 2'b00} at the lr LOW->HIGH boundary posedge; BCLK is
    //   50% duty, period 8 clks; the register shifts once per 4-clk window
    //   (DUT `falls` events 1..24).
    //   Per capture posedge (2 after each window start, slot a = the a-th
    //   4-clk window of the lr-HIGH half): slots 0..1 carry field bit 23
    //   (load window + first-shift window, identical), slots 2..24 carry
    //   field bit (24-a), slots 25..63 are 0 (register empty after 24
    //   shifts, asserted). The captured word is exactly {mix22, 2'b00}:
    //   w24[23] = slot0; w24[j] = slot(24-j) for j = 0..22.
    //   The H07 golden tap is {1'b0, mix22} = mix << 1, so the decoded
    //   32-bit word written per decoded half is sign_ext32(word24) >>> 2
    //   (payload zeros removed) — exactly the H07 tap {0, cell[21:0]}.
    //   One word is written per LR fall; the samples counter ticks with the
    //   LR falls, matching the H07 flat-bench counter tick-for-tick.
    // --------------------------------------------------------------
    reg [63:0] slotw;
    integer cnt = 0;
    reg in_high = 1'b0;
    reg armed = 1'b0;
    reg msb = 1'b0;        // s23, captured one bench tick after lr rise
    integer samples = 0;
    integer frc = 0;   // forensic: DUT frame_reg_v latch count — read-only
                       // witness for the truncation checks (never an
                       // expectation source; stream-vs-control parity is
                       // the pin-provable property)
    always @(posedge clk)
        if (rst_n && dut.u_core.frame_reg_v) frc = frc + 1;
    always @(posedge clk) begin
        if (!rst_n) begin
            in_high <= 1'b0;
            armed <= 1'b0;
            cnt <= 0;
            slotw <= 64'd0;
            msb <= 1'b0;
        end else begin
            if (lr_fall_lo && armed && in_high) begin
                // lr-HIGH half complete: 256 posedges — the rise tick
                // (cnt 0) plus 255 mid-half posedges
                if (cnt != 255) begin
                    $display("FAIL tb_synth_top (lr-high half: %0d mid clks, want 255)", cnt);
                    $finish;
                end
                if (slotw[63:25] != 39'd0) begin
                    $display("FAIL tb_synth_top (I2S LJS tail windows 25..63 nonzero: cnt=%0d samples=%0d)", cnt, samples);
                    $finish;
                end
                if (slotw[0] !== slotw[1]) begin
                    $display("FAIL tb_synth_top (I2S LJS msb window dup violated: s0=%b s1=%b cnt=%0d samples=%0d)", slotw[0], slotw[1], cnt, samples);
                    $finish;
                end
                if (i2s_d !== 1'b0) begin
                    $display("FAIL tb_synth_top (I2S D not 0 at lr falling edge)");
                    $finish;
                end
                begin
                    reg [23:0] w24;
                    reg [31:0] m32, g32;
                    reg [7:0] c0, c1, c2, c3;
                    integer kk;
                    w24 = 24'd0;
                    w24[23] = slotw[0];
                    for (kk = 0; kk < 23; kk = kk + 1)
                        w24[kk] = slotw[24 - kk];   // slot a = bit (24-a), a=2..24
                    if (w24[1:0] != 2'd0) begin
                        $display("FAIL tb_synth_top (I2S LJS payload zeros violated: w24b=%0b cnt=%0d samples=%0d)",
                                 w24, cnt, samples);
                        $finish;
                    end
                    m32 = {{8{w24[23]}}, w24} >>> 2;   // sign-extended mix
                    g32 = m32;                           // tap = {0, cell[21:0]}
                    c0 = g32[7:0];
                    c1 = g32[15:8];
                    c2 = {{4{g32[21]}}, g32[21:16]};
                    c3 = {8{g32[21]}};
                    $fwrite(afd, "%c%c%c%c", c0, c1, c2, c3);
                    samples = samples + 1;
                end
                in_high <= 1'b0;
                cnt <= 0;
                slotw <= 64'd0;
            end else if (lr_rise_hi && armed && !in_high) begin
                // lr-LOW half complete: 255 mid posedges after the rise tick
                if (cnt != 255) begin
                    $display("FAIL tb_synth_top (lr-low half: %0d mid clks, want 255)", cnt);
                    $finish;
                end
                if (slotw != 64'd0) begin
                    $display("FAIL tb_synth_top (lr-low half not all zeros: %016hx %016hx)",
                             slotw[63:32], slotw[31:0]);
                    $finish;
                end
            end
            if (lr_rise_hi && !in_high) begin
                // entering lr-HIGH: D already carries the MSB (load window)
                msb <= i2s_d;
                in_high <= 1'b1;
                armed <= 1'b1;
                cnt <= 0;
                slotw <= 64'd0;
            end
            // mid-half sampling: one capture per 4-clk window at the
            // posedge where cnt%4==2 (D settles one posedge after each
            // window-boundary shift; 2 clks of hold before the next)
            if (armed && !lr_rise_hi && !lr_fall_lo) begin
                if (cnt % 4 == 2) begin
                    // full-register write (no variable part-select):
                    slotw <= (i2s_d ? (slotw | (64'h1 << (cnt >> 2))) : slotw);
                    if (cnt == 6 && i2s_d !== slotw[0]) begin
                        // MSB duplication law at capture level: slot1
                        // repeats slot0 (the 24-bit MSB); captured 2
                        // clks apart so both operands are stable.
                        $display("FAIL tb_synth_top (I2S slot1 != slot0 (MSB duplication): %b vs %b)",
                                 i2s_d, slotw[0]);
                        $finish;
                    end
                end
                cnt <= cnt + 1;
            end
        end
    end

    // --------------------------------------------------------------
    // SPI master, mode 0. 48-bit frame MSB first:
//   {F(1), 6'b000000, SEC(1), ADDR(8), DATA(32)};
//   MOSI sampled on SCK rising edges; status is clocked out MSB-first on
//   SCK falling edges, latched at CS falling edge.
// --------------------------------------------------------------
    function integer fbit;
        input integer bi;
        input f2;
        input sec2;
        input [7:0] a2;
        input [31:0] d2;
        begin
            if (bi == 0) fbit = f2;
            else if (bi <= 6) fbit = 1'b0;
            else if (bi == 7) fbit = sec2;
            else if (bi <= 15) fbit = a2[15 - (bi - 8)];
            else fbit = d2[47 - bi];
        end
    endfunction

    // All drives land on negedge offset (5+10k ps) so they never race the
    // DUT's posedge sampling; SCK half period 2 clks keeps 40 ps/bit =
    // 194 clk/tx, the H07 boundary-exact pace.
    task spi_write(input sec, input [7:0] a2, input [31:0] d2);
        integer bi;
        begin
            @(negedge clk);
            spi_cs_n = 1'b0;
            spi_sck = 1'b0;
            spi_mosi = 1'b0;
            #20;
            for (bi = 0; bi < 48; bi = bi + 1) begin
                spi_mosi = fbit(bi, 1'b1, sec, a2, d2);
                spi_sck = 1'b0; #20;
                spi_sck = 1'b1; #20;   // rise samples mosi (2-flop delayed)
            end
            spi_sck = 1'b0; #20;       // SCK ends LOW (protocol idle; a
            spi_cs_n = 1'b1;           // high idle SCK would inject a
            #20;                       // phantom shift on the next
        end                           // transaction's first SCK return)
    endtask

    // truncated frame: only <nbits> bits, same cadence; CS releases with
    // no 48th rising edge -> the DUT must not commit anything
    task spi_xfer_n(input integer sec, input [7:0] a2, input [31:0] d2,
                    input integer nbits);
        integer bi;
        begin
            @(negedge clk);
            spi_cs_n = 1'b0;
            spi_sck = 1'b0;
            spi_mosi = 1'b0;
            #20;
            for (bi = 0; bi < nbits; bi = bi + 1) begin
                spi_mosi = fbit(bi, 1'b1, sec, a2, d2);
                spi_sck = 1'b0; #20;
                spi_sck = 1'b1; #20;
            end
            spi_sck = 1'b0; #20;       // SCK ends LOW (see spi_write)
            spi_cs_n = 1'b1;
            #20;
        end
    endtask

    // full 48-bit frame with F=0: the decoder must ignore it entirely
    // (wr_wr = frame_reg_v & wr_f; no F -> not accepted, FRESH preserved)
    task spi_write_f0(input [7:0] a2, input [31:0] d2);
        integer bi;
        begin
            @(negedge clk);
            spi_cs_n = 1'b0;
            spi_sck = 1'b0;
            spi_mosi = 1'b0;
            #20;
            for (bi = 0; bi < 48; bi = bi + 1) begin
                spi_mosi = fbit(bi, 1'b0, 1'b1, a2, d2);
                spi_sck = 1'b0; #20;
                spi_sck = 1'b1; #20;
            end
            spi_sck = 1'b0; #20;       // SCK ends LOW (see spi_write)
            spi_cs_n = 1'b1;
            #20;
        end
    endtask

    // DELIBERATE rate violation: SCK 1 clk low / 1 clk high = 20 ps/bit
    // (98 clk/tx < the 194 boundary). Used to overfill the depth-168 event
    // queue inside one frame so OVERFLOW can demonstrably set.
    // SCK ends LOW before each CS release (spurious shift absorber).
    task spi_burst_fast(input integer n, input integer sec,
                        input [7:0] a2, input [31:0] d2);
        integer bi, it;
        begin
            for (it = 0; it < n; it = it + 1) begin
                @(negedge clk);
                spi_cs_n = 1'b0;
                spi_sck = 1'b0;
                spi_mosi = 1'b0;
                #20;
                for (bi = 0; bi < 48; bi = bi + 1) begin
                    spi_mosi = fbit(bi, 1'b1, sec, a2, d2);
                    spi_sck = 1'b0; #10;
                    spi_sck = 1'b1; #10;   // 1 posedge high (edge chain ok)
                end
                spi_sck = 1'b0; #10;       // SCK ends LOW (see spi_write)
                spi_cs_n = 1'b1;
                #20;
            end
        end
    endtask

    // 32-bit status read. DUT semantics (traced, as-built): miso_sh loads
    // status_real at the CS_N RISING edge (end of the previous transaction,
    // dx7_core cs_fall detector), so every read returns the word latched at
    // the end of the PREVIOUS transaction; the first read after reset
    // returns 0 (F-STA-1). The DUT shifts miso_sh one position per SCK
    // falling edge (+25 ps); sampling MISO mid SCK-HIGH (at +23 ps) lands
    // after the previous cycle's shift and before this cycle's shift, so
    // samples 0..31 are bits 31..0 of the loaded word (MSB first).
    task spi_read_status(output [31:0] w);
        integer bi;
        begin
            w = 32'd0;
            @(negedge clk);
            spi_cs_n = 1'b0;
            spi_sck = 1'b0;
            spi_mosi = 1'b0;
            #20;
            for (bi = 0; bi < 32; bi = bi + 1) begin
                spi_sck = 1'b1;
                #23;                      // SCK high 23 ps, low 17 ps
                w = {w[30:0], spi_miso};  // mid-high, pre-shift window
                spi_sck = 1'b0;
                #17;
            end
            spi_cs_n = 1'b1;
            #20;
        end
    endtask

    // --------------------------------------------------------------
    // pacing: pin-derived. lrclk rise k fires at clk ~256+512k from
    // reset; wire frame N spans rises 64N+1..64N+64. frame_now == N
    // from the frame's first rise; writers here always start within
    // the first ~500 clks of a frame, so the DUT frame a write drains
    // in matches the H07 flat bench (which exits wait_until at the
    // first frame-start posedge; this bench exits at the first LR rise
    // of the frame, 256 clks later — same apply frame, so all commit /
    // event frames stay equivalent).
    //
    // H07-exact exit ticks for the frame waits (derived purely from
    // the pin clock grid):
    //   * H07 wait_frames exits at the first negedge after the posedge
    //     whose post values are (sample_clk==511, sample_idx==62) — the
    //     63rd LR rise of the frame plus 255 clks;
    //   * H07 wait_tick_boundary exits at the first negedge after the
    //     posedge whose post values meet (sample_idx==63,
    //     sample_clk>=315) — the 64th LR rise plus 59 clks.
    // tick_c is the bench posedge index; tick63/tick64 are recorded on
    // the 63rd/64th rise of each lr-frame, with 1-tick entry pulses
    // p63/p64.
    // --------------------------------------------------------------
    integer lr_cnt = 0;
    integer frame_now = 0;
    integer tick_c = 0;
    integer tick63 = 0;
    integer tick64 = 0;
    reg p63 = 1'b0;
    reg p64 = 1'b0;
    always @(posedge clk) begin
        tick_c = tick_c + 1;
        if (!rst_n) begin
            p63 <= 1'b0;
            p64 <= 1'b0;
        end else begin
            p63 <= (lr_rise_hi && (lr_cnt % 64) == 62);
            p64 <= (lr_rise_hi && (lr_cnt % 64) == 63);
            if (lr_rise_hi) begin
                if ((lr_cnt % 64) == 62) tick63 = tick_c;
                if ((lr_cnt % 64) == 63) tick64 = tick_c;
                lr_cnt = lr_cnt + 1;
                frame_now = (lr_cnt == 1) ? 0 : (lr_cnt - 1) / 64;
            end
        end
    end

    task wait_until_frame(input integer target);
        begin
            @(negedge clk);
            while (frame_now < target)
                @(lr_rise_hi);
        end
    endtask

    // H07-exact: exit at the (63rd LR rise + 255 clks) negedge, n times
    task wait_frames(input integer n);
        integer i;
        begin
            for (i = 0; i < n; i = i + 1) begin
                @(posedge p63);
                wait (tick_c >= tick63 + 255);
                @(negedge clk);
            end
        end
    endtask

    // H07-exact tick boundary: 64th LR rise + 59 clks (first posedge
    // with post (sample_idx==63, sample_clk>=315))
    task wait_tick_boundary;
        begin
            @(posedge p64);
            wait (tick_c >= tick64 + 59);
            @(negedge clk);
        end
    endtask

    // run until `target` samples are dumped, plus one flush wire frame so
    // the final I2S half completes
    task wait_until_samples(input integer target);
        begin
            @(negedge clk);
            while (samples < target)
                @(negedge clk);
            wait_frames(1);
        end
    endtask

    // --------------------------------------------------------------
    // field parsers (mirrors tb_dx7_core.v: $fgets right-aligns the line)
    // --------------------------------------------------------------
    function integer hexdig(input [7:0] c);
        begin
            if (c >= "0" && c <= "9") hexdig = c - "0";
            else if (c >= "a" && c <= "f") hexdig = c - "a" + 10;
            else if (c >= "A" && c <= "F") hexdig = c - "A" + 10;
            else hexdig = 0;
        end
    endfunction

    function [7:0] ch(input [255:0] l, input integer pbase,
                      input integer j);
        ch = l[(pbase - j)*8 +: 8];
    endfunction

    function integer chex(input [255:0] l, input integer pbase,
                          input integer j, input integer n);
        integer k;
        begin
            chex = 0;
            for (k = 0; k < n; k = k + 1)
                chex = chex * 16 + hexdig(ch(l, pbase, j + k));
        end
    endfunction

    function integer dec_at(input [255:0] l, input integer pbase,
                            input integer j);
        integer k, v, c;
        begin
            v = 0; k = j;
            c = ch(l, pbase, k);
            while (c >= "0" && c <= "9") begin
                v = v*10 + (c - "0");
                k = k + 1;
                c = ch(l, pbase, k);
            end
            dec_at = v;
        end
    endfunction

    function integer ndig(input [255:0] l, input integer pbase,
                          input integer j);
        integer k, c;
        begin
            k = j;
            c = ch(l, pbase, k);
            while (c >= "0" && c <= "9") begin
                k = k + 1;
                c = ch(l, pbase, k);
            end
            ndig = k - j;
        end
    endfunction

    // --------------------------------------------------------------
    // main
    // --------------------------------------------------------------
    initial begin
        if (!$value$plusargs("vectors=%s", vfile)) begin
            $display("FAIL tb_synth_top (missing +vectors=)");
            $finish;
        end
        if (!$value$plusargs("actual=%s", afile)) begin
            $display("FAIL tb_synth_top (missing +actual=)");
            $finish;
        end
        if (!$value$plusargs("meta=%s", mfile)) begin
            $display("FAIL tb_synth_top (missing +meta=)");
            $finish;
        end
        vfd = $fopen(vfile, "r");
        if (vfd == 0) begin
            $display("FAIL tb_synth_top (cannot open %0s)", vfile);
            $finish;
        end
        afd = $fopen(afile, "wb");
        if (afd == 0) begin
            $display("FAIL tb_synth_top (cannot open actual file)");
            $finish;
        end
        mfd = $fopen(mfile, "w");
        samples = 0;
        base_frame = -1;
        commit_n = 0;
        expect_samples = 0;
        if (!$value$plusargs("expect=%d", expect_samples))
            expect_samples = 0;

        repeat (10) @(negedge clk);
        rst_n = 1'b1;
        repeat (10) @(negedge clk);

        while (1) begin
            rc = $fgets(line, vfd);
            if (rc == 0) begin
                break;
            end else begin
                // $fgets right-aligns the line: char j after the first
                // character sits at byte (p0 - j), p0 = first-char byte
                p0 = 31;
                while (p0 > 0 && line[p0*8 +: 8] == 8'd0)
                    p0 = p0 - 1;
                tok = line[p0*8 +: 8];
                if (tok == "P") begin
                    addr = chex(line, p0, 2, 2);
                    data = chex(line, p0, 5, 8);
                    spi_write(1'b1, addr[7:0], data[31:0]);
                end else if (tok == "C") begin
                    if (ndig(line, p0, 2) > 0)
                        wait_until_frame(base_frame + dec_at(line, p0, 2));
                    spi_write(1'b1, 8'h42, 32'h0);
                    base_frame = frame_now;
                    commit_list[commit_n] = base_frame;
                    commit_n = commit_n + 1;
                end else if (tok == "E") begin
                    pd = 2;
                    vdec = dec_at(line, p0, pd);
                    block = vdec;
                    pd = pd + ndig(line, p0, pd) + 1;
                    addr = chex(line, p0, pd, 2);
                    data = chex(line, p0, pd + 3, 8);
                    if (block > 0)
                        wait_until_frame(base_frame + block);
                    spi_write(1'b0, addr[7:0], data[31:0]);
                end else if (tok == "S") begin
                    if (ndig(line, p0, 2) > 0)
                        wait_until_frame(base_frame + dec_at(line, p0, 2));
                    spi_write(1'b0, 8'h0F, 32'h0);
                end else if (tok == "T") begin
                    addr = chex(line, p0, 2, 2);
                    data = chex(line, p0, 5, 8);
                    wait_tick_boundary();
                    spi_write(1'b0, addr[7:0], data[31:0]);
                end else if (tok == "R") begin
                    vdec = dec_at(line, p0, 2);
                    wait_frames(vdec);
                end else if (tok == "W") begin
                    if (ndig(line, p0, 2) > 0)
                        wait_until_samples(samples + dec_at(line, p0, 2));
                    else
                        wait_until_samples(expect_samples);
                end else if (tok == "Q") begin
                    spi_read_status(qword);
                    // as-built: a read returns the word latched at the end
                    // of the PREVIOUS transaction, so the first one after
                    // pad reset is 0x0000_0000 (miso_sh reset value); the
                    // protocol expectations are asserted by the python gate
                    if (qword[31:24] != 8'hD7 || qword[23:20] != 4'h1)
                        $display("WARN tb_synth_top (qread 0x%08x pre-protocol or pre-write)", qword);
                    $fwrite(mfd, "qread 0x%08x\n", qword);
                end else if (tok == "X") begin
                    // X <sec:1> <addr:2> <data:8> <nbits>: truncated F=1
                    // frame (bits 0..nbits-1 only, MSB first)
                    pd = 2;
                    vdec = dec_at(line, p0, pd);     // sec
                    pd = pd + 2;                     // skip field + space
                    addr = chex(line, p0, pd, 2);    // j = 4..5
                    data = chex(line, p0, pd + 3, 8); // j = 7..14
                    block = dec_at(line, p0, pd + 12); // nbits at j = 16
                    spi_xfer_n(vdec, addr[7:0], data[31:0], block);
                end else if (tok == "V") begin
                    // V <addr:2> <data:8>: full 48-bit F=0 frame
                    addr = chex(line, p0, 2, 2);
                    data = chex(line, p0, 5, 8);
                    spi_write_f0(addr[7:0], data[31:0]);
                end else if (tok == "Y") begin
                    // Y <sec:1> <n> <addr:2> <data:8>: n DELIBERATE
                    // rate-violating (20 ps/bit) full frames, SEC from arg
                    pd = 2;
                    vdec = chex(line, p0, pd, 1);      // sec
                    pd = pd + 2;
                    block = dec_at(line, p0, pd);        // n
                    pd = pd + ndig(line, p0, pd) + 1;
                    addr = chex(line, p0, pd, 2);
                    data = chex(line, p0, pd + 3, 8);
                    spi_burst_fast(block, vdec, addr[7:0], data[31:0]);
                end else if (tok == "Z") begin
                    vdec = dec_at(line, p0, 2);
                    rst_n = 1'b0;
                    spi_cs_n = 1'b1;
                    spi_sck = 1'b0;
                    spi_mosi = 1'b0;
                    repeat (vdec) @(negedge clk);
                    rst_n = 1'b1;
                    repeat (10) @(negedge clk);
                end else if (tok == "#") begin
                    // comment line
                end else begin
                    $display("FAIL tb_synth_top (unknown token %b in vector line)", tok);
                    $finish;
                end
            end
        end

        // ---- final status word (pin-truth), then meta ----
        // as-built: this is the word latched at the end of the previous
        // transaction (lenient here; python asserts the exact expectation)
        spi_read_status(qword);
        if (qword[31:24] != 8'hD7 || qword[23:20] != 4'h1)
            $display("WARN tb_synth_top (final qread 0x%08x)", qword);
        $fwrite(mfd, "qread 0x%08x\n", qword);
        // status word layout (traced, as-built): {D7,1,stun,q,ovf,fresh,ctr}
        $fwrite(mfd, "stun %0d\n", qword[19]);
        $fwrite(mfd, "queue %0d\n", qword[18]);
        $fwrite(mfd, "overflow %0d\n", qword[17]);
        $fwrite(mfd, "fresh %0d\n", qword[16]);
        // st_overrun is NOT in the status word; forensic hierarchical read
        // (reporting only) + the DUT's own invariant: it must never set.
        if (dut.u_core.st_overrun)
            $display("FAIL tb_synth_top (DUT st_overrun set)");
        $fwrite(mfd, "overrun %0d\n", dut.u_core.st_overrun);
        $fwrite(mfd, "frame %0d\n", qword[15:0]);
        $fwrite(mfd, "samples %0d\n", samples);
        $fwrite(mfd, "lr_edges %0d\n", lr_cnt);
        $fwrite(mfd, "frames_run %0d\n", frame_now);
        $fwrite(mfd, "base_frame %0d\n", base_frame);
        $fwrite(mfd, "commits");
        for (vdec = 0; vdec < commit_n; vdec = vdec + 1)
            $fwrite(mfd, " %0d", commit_list[vdec]);
        $fwrite(mfd, "\n");
        $fwrite(mfd, "bclk_period %0d\n", 8);    // asserted structurally in-bench
        $fwrite(mfd, "lr_period %0d\n", 512);    // asserted structurally in-bench
        $fwrite(mfd, "first_bit_delay %0d\n", first_bit_delay);
        $display("FORENSICS frame_latches=%0d", frc);
        $fclose(vfd);
        $fclose(afd);
        $fclose(mfd);
        $display("DONE samples=%0d", samples);
        $finish;
    end

    // watchdog — far beyond any contracted render
    initial begin
        #(64'd200_000_000_000);
        $display("FAIL tb_synth_top (watchdog timeout)");
        $finish;
    end

endmodule
`default_nettype wire