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
    int arrivals, rejections, pops, full_allocations;
    bit observing;
    always @(posedge clk) begin
        cycle++;
        if (observing && dut.skid_pop && !dut.dr_sec) begin
            if (dut.frame_ctr != arrival_frame ||
                (arrivals > 0 && cycle - last_arrival != 194))
                $fatal(1, "Unexpected SPI arrival frame or spacing");
            arrivals++;
            last_arrival = cycle;
            if (dut.evq_full) rejections++;
        end
        if (observing && dut.wc_pop_fire) begin
            if (dut.frame_ctr != arrival_frame + 16'd1 || dut.ev_a != dut.EA_NOTEON)
                $fatal(1, "Unexpected event or application frame");
            pops++;
        end
        if (observing && dut.fc_state == dut.F_ALLOC && !dut.free_found)
            full_allocations++;
    end

    initial begin
        if (!$value$plusargs("burst=%d", burst) ||
            !(burst inside {100, 168, 169})) $fatal(1, "Use +burst=100, 168 or 169");
        repeat (10) @(negedge clk);
        rst_n = 1;
        repeat (10) @(negedge clk);
        write_spi(1, 8'h42, 0);  // commit reset patch
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
        $display("arrivals=%0d rejected=%0d pops=%0d full_allocations=%0d overflow=%0d overrun=%0d",
                 arrivals, rejections, pops, full_allocations, overflow, overrun);
        if (arrivals != burst || rejections != int'(burst == 169) ||
            pops != burst - rejections || full_allocations != pops ||
            overflow !== (burst == 169) || overrun !== 0)
            $fatal(1, "DR-0008 review observation did not reproduce");
        $finish;
    end

    initial #2500000 $fatal(1, "Frame watchdog");
endmodule
