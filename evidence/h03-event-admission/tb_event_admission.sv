// DR-0008 review probe. Drive raw NOTEON commands through the SPI pins.
// Counters tap the queue and allocator to distinguish arrivals from admissions.
`timescale 1ns/1ps
module tb_event_admission;
    bit clk, rst_n, sck, mosi;
    bit cs_n = 1;
    wire overrun, overflow;
    wire [15:0] live;
    always #5 clk = ~clk;

    dx7_core dut (.clk(clk), .rst_n(rst_n), .spi_sck(sck), .spi_mosi(mosi),
                  .spi_cs_n(cs_n), .spi_miso(), .i2s_bclk(), .i2s_lrclk(),
                  .i2s_d(), .tap_frame(), .tap_live(live), .tap_mix(),
                  .tap_mix_valid(), .status_overrun(overrun),
                  .status_overflow(overflow), .state_obs());

    // Called on a falling edge. Each bit occupies four core clocks.
    task automatic write_spi(input logic page, input logic [7:0] addr,
                             input logic [31:0] data);
        logic [47:0] word_out;
        word_out = {1'b1, 6'b0, page, addr, data};
        cs_n = 0;
        for (int i = 47; i >= 0; i--) begin
            mosi = word_out[i];
            repeat (2) @(negedge clk);
            sck = 1;
            repeat (2) @(negedge clk);
            sck = 0;
        end
        cs_n = 1;
        repeat (2) @(negedge clk);
    endtask

    logic [15:0] arrival_frame;
    int burst, cycle, last_arrival;
    int drains, rejections, pops, occupied_attempts, strikes;
    bit observing, reset_patch;
    always @(posedge clk) begin
        cycle++;
        if (observing && dut.skid_pop) begin
            if (dut.dr_sec || dut.frame_ctr != arrival_frame ||
                (drains > 0 && cycle - last_arrival != 194))
                $fatal(1, "Unexpected SPI arrival frame or spacing");
            drains++;
            last_arrival = cycle;
            if (dut.evq_full) rejections++;
        end
        if (observing && dut.fc_commit && dut.shd_commit)
            $fatal(1, "Unexpected patch commit during the burst observation");
        if (observing && dut.wc_pop_fire) begin
            if (dut.frame_ctr != arrival_frame + 16'd1 || dut.ev_a != dut.EA_NOTEON)
                $fatal(1, "Unexpected event or application frame");
            pops++;
        end
        if (observing && dut.fc_state == dut.F_ALLOC && !dut.free_found)
            occupied_attempts++;
        // F_NN_BOOK records the struck pitch and advances the sequence number.
        if (observing && dut.fc_state == dut.F_NN_BOOK) begin
            if (dut.frame_ctr != arrival_frame + 16'd1)
                $fatal(1, "Strike outside the application frame");
            strikes++;
        end
    end

    initial begin
        if (!$value$plusargs("burst=%d", burst) ||
            !(burst inside {100, 168, 169})) $fatal(1, "Use +burst=100, 168 or 169");
        repeat (10) @(negedge clk);
        rst_n = 1;
        repeat (10) @(negedge clk);
        // Keep one operator out of exact silence so the full-voice allocator
        // has a sounding victim. +reset_patch repeats the old silent setup.
        reset_patch = $test$plusargs("reset_patch");
        if (!reset_patch) begin
            write_spi(1, 8'h20, 32'h63636363);  // envelope rates
            write_spi(1, 8'h28, 32'h00636363);  // sustain at level 99
            write_spi(1, 8'h38, 32'd99);        // output level
        end
        write_spi(1, 8'h42, 0);
        for (int note = 40; note < 56; note++)
            write_spi(0, 0, 'h6400 + note);

        // Start with 16 occupied voices and an empty event queue. The first
        // transaction straddles the boundary and completes in frame 3.
        while (!(dut.frame_ctr == 2 && dut.sample_idx == 63 && dut.sample_clk == 415))
            @(negedge clk);
        if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
            $fatal(1, "Invalid setup");
        arrival_frame = dut.frame_ctr + 16'd1;
        observing = 1;
        for (int i = 0; i < burst; i++)
            write_spi(0, 0, 'h6400 + ((60 + i) % 128));
        while (dut.frame_ctr != arrival_frame + 16'd2) @(negedge clk);
        $display("frame=%0d drains=%0d queue_rejections=%0d pops=0 commits=0",
                 arrival_frame, drains, rejections);
        $display("frame=%0d drains=0 pops=%0d occupied_attempts=%0d strikes=%0d commits=0 overflow=%0d overrun=%0d",
                 arrival_frame + 1, pops, occupied_attempts, strikes, overflow, overrun);
        if (drains != burst || rejections != int'(burst == 169) ||
            pops != burst - rejections || occupied_attempts != pops ||
            strikes != (reset_patch ? 0 : pops) ||
            overflow !== (burst == 169) || overrun !== 0)
            $fatal(1, "DR-0008 review observation did not reproduce");
        $finish;
    end

    initial #2500000 $fatal(1, "Frame watchdog");
endmodule
