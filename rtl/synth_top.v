// ============================================================================
// rtl/synth_top.v — H08 (issue #30): pin-level top / passive chassis
//
// Product-level top module: exposes the external REFERENCE/CLOCK pins, the
// four SPI-slave data pins, and the three I2S-master output pins, then pins
// them 1:1 to the H07-verified `dx7_core` module (rtl/dx7_core.v, unchanged
// in H08; its file hash is pinned by tests/test_h07.py against the frozen
// H07 evidence and may not be touched in H08).
//
// Provenance (Apache-2.0, 2AMLogic gf180-parasynth sibling — see
// docs/reuse/catalog.json, component "synth-top-chassis", rulings in
// docs/REUSE-RULINGS.md): adapted from the parasynth synth_top passive-
// chassis pattern (chip = external pins + one core; the chassis is a pin
// map, verified at pin level). Divergence from the source: the parasynth
// chassis places its SPI controller, I2S transmitter, and reset sync
// OUTSIDE the core behind its own pins; here those blocks ALREADY EXIST
// INSIDE rtl/dx7_core.v (landed in H07; hash-pinned), so this chassis is
// deliberately pin-net-only: NO clock generation, NO reset synchronization,
// NO glue logic. Moving clock/reset sync into the chassis would change
// core-observable behavior and is out of scope for H08 (the issue's stop
// condition); the chassis stays passive.
//
// AS-BUILT interface contract (verified at the pads by rtl/tb_synth_top.v,
// evidence under evidence/h08-pin/):
//   * ref_clk 24.576 MHz (clock-free-running design; the external host runs
//     off this clock), rst_n active-low pad reset.
//   * SPI slave, mode 0 (CPOL=0/CPHA=0), 48-bit MSB-first
//     {F, 6'b0, SEC, A[7:0], D[31:0]}; SCK sampled on rising edges while
//     CS_N is low (two-flop sync inside the core); a frame is accepted at
//     the 48th rising edge; the MISO status word is loaded on the CS_N
//     falling edge and shifts out on SCK falling edges (master samples on
//     SCK rising edges, bit31 first; full protocol in docs/H08-PIN.md §4).
//     Host SCK bound: <= f_ref/4 (194 clk/transaction worst case).
//   * I2S MASTER output to an external DAC, as built and pinned here:
//       - single clock domain; BCLK period = 8 ref_clk (3.072 MHz),
//         LRCLK period = 512 ref_clk (96 kHz). NOTE: the H07 core header
//         and CONTRACT-CORE §5 preamble state "BCLK (= clk/4)"; the
//         as-built divider is f_ref/8 — a 32:1 BCLK:LRCLK ratio, a valid
//         32-bit-I2S frame rate at the contracted 96 kHz LRCLK. Recorded
//         as H08 finding F-I2S-1 (docs/H08-PIN.md); the core is NOT
//         modified in H08 (hash-pinned by tests/test_h07.py).
//       - one register load per sample (H03 §5: "BCLK from counter bits,
//         one load per sample"); the loaded 22-bit sample serializes
//         MSB-first, LEFT-JUSTIFIED in a 24-bit field ({sample, 2'b00}),
//         into the LRHIGH half-slot only (32 BCLK per half-slot); the
//         LRLOW half-slot carries an explicit zero field (DEC-008 stereo
//         duplication is OPTIONAL and is not used — the external DAC reads
//         one slot; both slots are well defined, the stream is a valid
//         32-bit-I2S frame).
//       - sample value = the previous wire frame's mixed output ([21:0]
//         two's complement), wire-frame latency N -> DAC samples on N+2
//         (H07 §5.3).
//   * No other pins. No sideband. No JTAG. (P-0002: single clock domain,
//     clock-free-running; the DAC and the host are the two external
//     consumers.)
//
// Verification: rtl/tb_synth_top.v drives ONLY the nine pads below and
// observes only the three output pads; expectations come from the frozen
// model / H07 evidence, never from DUT-internal state (anti-circularity).
// ============================================================================

`timescale 1ns / 1ps

module synth_top (
    // reference clock + reset (H03 §4.1 / CONTRACT-CORE §4: host runs off
    // this clock; the DUT is active-low)
    input  wire ref_clk,
    input  wire rst_n,

    // SPI slave data pins (contract 4.2, mode 0, 48-bit frames)
    input  wire spi_cs_n,
    input  wire spi_sck,
    input  wire spi_mosi,
    output wire spi_miso,

    // I2S master output (contract 5: BCLK/LRCLK/D for an external DAC)
    output wire i2s_bclk,
    output wire i2s_lrclk,
    output wire i2s_d
);

    // -------------------------------------------------------------------------
    // One core, pin-mapped. `dx7_core` already instantiates the SPI slave
    // (frame counter, skid FIFO, event queue, status word) and the I2S
    // serializer internally; every pad above is wired 1:1.
    // -------------------------------------------------------------------------
    dx7_core u_core (
        .clk        (ref_clk),
        .rst_n      (rst_n),
        .spi_cs_n   (spi_cs_n),
        .spi_sck    (spi_sck),
        .spi_mosi   (spi_mosi),
        .spi_miso   (spi_miso),
        .i2s_bclk   (i2s_bclk),
        .i2s_lrclk  (i2s_lrclk),
        .i2s_d      (i2s_d)
    );

endmodule