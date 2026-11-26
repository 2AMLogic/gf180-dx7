// H03 event-admission and frame-boundary SPI probe (issue #339; DR-0008 / DR-0015).
// Drives 48-bit SPI frames through dx7_core's pins at the contracted minimum
// spacing (4 core clocks/bit * 48 bits + 2-clock CS_N high gap = 194 core
// clocks per transaction) with all 16 voices occupied, and checks per-frame
// conservation across the skid FIFO, 168-entry event queue, and voice allocator.
`timescale 1ns/1ps
module tb_event_admission;
    bit clk = 0;
    bit rst_n = 0;
    bit sck = 0;
    bit mosi = 0;
    bit cs_n = 1;
    wire overrun, overflow;
    wire [15:0] live;
    always #5 clk = ~clk;

    dx7_core dut (
        .clk(clk),
        .rst_n(rst_n),
        .spi_sck(sck),
        .spi_mosi(mosi),
        .spi_cs_n(cs_n),
        .spi_miso(),
        .i2s_bclk(),
        .i2s_lrclk(),
        .i2s_d(),
        .tap_frame(),
        .tap_live(live),
        .tap_mix(),
        .tap_mix_valid(),
        .status_overrun(overrun),
        .status_overflow(overflow),
        .state_obs()
    );

    // Called on negedge clk. Each bit occupies 4 core clocks (192 clocks for
    // 48 bits) followed by a 2-clock CS_N high gap = 194 core clocks total.
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

    wire [14:0] frame_off = {dut.sample_idx, dut.sample_clk}; // 0..32767

    int f_skid_open     [0:15];
    int f_evq_open      [0:15];
    int f_completions   [0:15]; // wr_wr (skid_push)
    int f_skid_rejects  [0:15]; // skid_push && skid_full
    int f_drains_sec0   [0:15]; // skid_pop && !dr_sec
    int f_drains_sec1   [0:15]; // skid_pop && dr_sec
    int f_evq_admits    [0:15]; // skid_pop && !dr_sec && !evq_full
    int f_evq_rejects   [0:15]; // skid_pop && !dr_sec && evq_full
    int f_sim_pop_rej   [0:15]; // skid_pop && !dr_sec && evq_full && wc_pop_fire
    int f_pops_noteon   [0:15];
    int f_pops_other    [0:15];
    int f_occ_attempts  [0:15];
    int f_strikes       [0:15];
    int f_commits       [0:15];
    int f_busy_clks     [0:15]; // cycles with fc_state != F_IDLE
    int f_evpop_last    [0:15]; // last frame_off in F_EVPOP
    int f_first_push    [0:15];
    int f_last_push     [0:15];
    int f_first_drain   [0:15];
    int f_last_drain    [0:15];
    int f_skid_close    [0:15];
    int f_evq_close     [0:15];
    int f_overflow_end  [0:15];
    int f_overrun_end   [0:15];

    int cycle = 0;
    int last_push_cycle = -1;
    int last_drain_cycle = -1;
    bit check_spacing = 0;

    initial begin
        for (int k = 0; k < 16; k++) begin
            f_first_push[k]  = -1;
            f_last_push[k]   = -1;
            f_first_drain[k] = -1;
            f_last_drain[k]  = -1;
            f_evpop_last[k]  = -1;
        end
    end

    always @(posedge clk) begin
        cycle <= cycle + 1;
        if (!dut.rst && dut.frame_ctr < 16'd16) begin
            int fn;
            int off;
            fn = int'(dut.frame_ctr);
            off = int'(frame_off);

            if (off == 0) begin
                f_skid_open[fn] = int'(dut.skid_cnt);
                f_evq_open[fn]  = int'(dut.evq_cnt);
            end

            if (dut.skid_push) begin
                if (check_spacing && last_push_cycle >= 0 &&
                    (cycle - last_push_cycle) != 194)
                    $fatal(1, "Expected 194-clk spacing on skid_push, got %0d",
                           cycle - last_push_cycle);
                last_push_cycle = cycle;
                f_completions[fn]++;
                if (f_first_push[fn] < 0) f_first_push[fn] = off;
                f_last_push[fn] = off;
                if (dut.skid_full)
                    f_skid_rejects[fn]++;
            end

            if (dut.skid_pop) begin
                if (check_spacing && last_drain_cycle >= 0 &&
                    (cycle - last_drain_cycle) != 194)
                    $fatal(1, "Expected 194-clk spacing on skid_pop, got %0d",
                           cycle - last_drain_cycle);
                last_drain_cycle = cycle;
                if (f_first_drain[fn] < 0) f_first_drain[fn] = off;
                f_last_drain[fn] = off;
                if (dut.dr_sec) begin
                    f_drains_sec1[fn]++;
                end else begin
                    f_drains_sec0[fn]++;
                    if (dut.evq_full) begin
                        f_evq_rejects[fn]++;
                        if (dut.wc_pop_fire)
                            f_sim_pop_rej[fn]++;
                    end else begin
                        f_evq_admits[fn]++;
                    end
                end
            end

            if (dut.fc_commit && dut.shd_commit)
                f_commits[fn]++;

            if (dut.fc_state == dut.F_EVPOP)
                f_evpop_last[fn] = off;

            if (dut.wc_pop_fire) begin
                if (dut.ev_a == dut.EA_NOTEON)
                    f_pops_noteon[fn]++;
                else
                    f_pops_other[fn]++;
            end

            if (dut.fc_state == dut.F_ALLOC && !dut.free_found)
                f_occ_attempts[fn]++;

            if (dut.fc_state == dut.F_NN_BOOK)
                f_strikes[fn]++;

            if (dut.fc_state != dut.F_IDLE)
                f_busy_clks[fn]++;

            if (off == 32767) begin
                int next_skid;
                int next_evq;
                next_skid = int'(dut.skid_cnt)
                          + ((dut.skid_push && !dut.skid_full) ? 1 : 0)
                          - (dut.skid_pop ? 1 : 0);
                next_evq  = int'(dut.evq_cnt)
                          + ((dut.skid_pop && !dut.dr_sec && !dut.evq_full) ? 1 : 0)
                          - (dut.wc_pop_fire ? 1 : 0);
                f_skid_close[fn] = next_skid;
                f_evq_close[fn]  = next_evq;
                f_overflow_end[fn] = int'(overflow
                    || (dut.skid_push && dut.skid_full)
                    || (dut.skid_pop && !dut.dr_sec && dut.evq_full));
                f_overrun_end[fn]  = int'(overrun || (dut.fc_state != dut.F_IDLE));
            end
        end
    end

    task automatic print_and_check_frame(input int fn);
        int drains_tot;
        int pops_tot;
        drains_tot = f_drains_sec0[fn] + f_drains_sec1[fn];
        pops_tot   = f_pops_noteon[fn] + f_pops_other[fn];
        if (f_skid_open[fn] + (f_completions[fn] - f_skid_rejects[fn]) - drains_tot
            != f_skid_close[fn])
            $fatal(1, "Skid conservation failed in frame %0d", fn);
        if (f_drains_sec0[fn] != f_evq_admits[fn] + f_evq_rejects[fn])
            $fatal(1, "SEC=0 drain split failed in frame %0d", fn);
        if (f_evq_open[fn] + f_evq_admits[fn] - pops_tot != f_evq_close[fn])
            $fatal(1, "EVQ conservation failed in frame %0d", fn);

        $display("FRAME %0d skid_open=%0d completions=%0d skid_rej=%0d drains_sec0=%0d drains_sec1=%0d skid_close=%0d evq_open=%0d evq_admit=%0d evq_rej=%0d sim_pop_rej=%0d pops_noteon=%0d pops_other=%0d evq_close=%0d occ_attempts=%0d strikes=%0d commits=%0d busy_clks=%0d first_push=%0d last_push=%0d first_drain=%0d last_drain=%0d evpop_last=%0d overflow=%0d overrun=%0d",
                 fn, f_skid_open[fn], f_completions[fn], f_skid_rejects[fn],
                 f_drains_sec0[fn], f_drains_sec1[fn], f_skid_close[fn],
                 f_evq_open[fn], f_evq_admits[fn], f_evq_rejects[fn],
                 f_sim_pop_rej[fn], f_pops_noteon[fn], f_pops_other[fn],
                 f_evq_close[fn], f_occ_attempts[fn], f_strikes[fn],
                 f_commits[fn], f_busy_clks[fn], f_first_push[fn],
                 f_last_push[fn], f_first_drain[fn], f_last_drain[fn],
                 f_evpop_last[fn], f_overflow_end[fn], f_overrun_end[fn]);
    endtask

    task automatic wait_frame_off(input int target_frame, input int target_off);
        while (!(int'(dut.frame_ctr) == target_frame && int'(frame_off) == target_off))
            @(negedge clk);
    endtask

    string mode;
    int burst;
    bit reset_patch;

    initial begin
        if (!$value$plusargs("mode=%s", mode))
            mode = "burst";
        reset_patch = $test$plusargs("reset_patch");

        repeat (10) @(negedge clk);
        rst_n = 1;
        repeat (10) @(negedge clk);

        if (!reset_patch) begin
            write_spi(1, 8'h20, 32'h63636363);  // op 6 (carrier in alg 1) rates
            write_spi(1, 8'h28, 32'h00636363);  // sustain at level 99
            write_spi(1, 8'h38, 32'd99);        // output level 99
        end
        write_spi(1, 8'h42, 0);                 // commit patch
        for (int note = 40; note < 56; note++)
            write_spi(0, 0, 32'h6400 + note);   // occupy all 16 voices

        if (mode == "burst") begin
            if (!$value$plusargs("burst=%d", burst) ||
                !(burst inside {100, 168, 169}))
                $fatal(1, "Use +burst=100, 168 or 169");
            wait_frame_off(2, 32671); // sample_idx=63, sample_clk=415
            if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
                $fatal(1, "Invalid setup before burst");
            check_spacing = 1;
            last_push_cycle = -1;
            last_drain_cycle = -1;
            for (int i = 0; i < burst; i++)
                write_spi(0, 0, 32'h6400 + ((60 + i) % 128));
            check_spacing = 0;
            wait_frame_off(5, 16);
            print_and_check_frame(3);
            print_and_check_frame(4);
            if (f_completions[3] != burst ||
                f_drains_sec0[3] != burst ||
                f_skid_rejects[3] != 0 ||
                f_evq_rejects[3] != int'(burst == 169) ||
                f_pops_noteon[4] != burst - int'(burst == 169) ||
                f_occ_attempts[4] != f_pops_noteon[4] ||
                f_strikes[4] != (reset_patch ? 0 : f_pops_noteon[4]) ||
                f_overflow_end[4] != int'(burst == 169) ||
                f_overrun_end[4] != 0)
                $fatal(1, "Burst probe assertion failed");
        end else if (mode == "phase_max") begin
            wait_frame_off(2, 32575);
            if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
                $fatal(1, "Invalid setup before phase_max");
            check_spacing = 1;
            last_push_cycle = -1;
            last_drain_cycle = -1;
            for (int i = 0; i < 170; i++)
                write_spi(0, 0, 32'h6400 + ((60 + i) % 128));
            check_spacing = 0;
            wait_frame_off(6, 16);
            print_and_check_frame(2);
            print_and_check_frame(3);
            print_and_check_frame(4);
            print_and_check_frame(5);
            if (f_completions[3] != 169 ||
                f_first_push[3] != 0 || f_last_push[3] != 32592 ||
                f_evq_admits[3] != 168 || f_evq_rejects[3] != 1 ||
                f_skid_rejects[3] != 0 || f_evq_close[3] != 168 ||
                f_completions[4] != 1 || f_first_push[4] != 18 ||
                f_evq_admits[4] != 1 || f_pops_noteon[4] != 169 ||
                f_strikes[4] != 169 || f_overflow_end[4] != 1 ||
                f_overrun_end[4] != 0)
                $fatal(1, "phase_max probe assertion failed");
        end else if (mode == "skid_straddle") begin
            wait_frame_off(2, 32574);
            if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
                $fatal(1, "Invalid setup before skid_straddle");
            check_spacing = 1;
            last_push_cycle = -1;
            last_drain_cycle = -1;
            for (int i = 0; i < 169; i++)
                write_spi(0, 0, 32'h6400 + ((60 + i) % 128));
            check_spacing = 0;
            wait_frame_off(5, 16);
            print_and_check_frame(2);
            print_and_check_frame(3);
            print_and_check_frame(4);
            if (f_completions[2] != 1 || f_first_push[2] != 32767 ||
                f_skid_close[2] != 1 || f_skid_open[3] != 1 ||
                f_completions[3] != 168 || f_drains_sec0[3] != 169 ||
                f_first_drain[3] != 0 || f_last_drain[3] != 32592 ||
                f_evq_admits[3] != 169 || f_evq_rejects[3] != 0 ||
                f_pops_noteon[3] != 1 || f_strikes[3] != 1 ||
                f_evq_close[3] != 168 || f_pops_noteon[4] != 168 ||
                f_strikes[4] != 168 || f_overflow_end[4] != 0 ||
                f_overrun_end[4] != 0)
                $fatal(1, "skid_straddle probe assertion failed");
        end else if (mode == "sim_pop_reject") begin
            wait_frame_off(2, 32751);
            if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
                $fatal(1, "Invalid setup before sim_pop_reject");
            check_spacing = 1;
            last_push_cycle = -1;
            last_drain_cycle = -1;
            for (int i = 0; i < 170; i++)
                write_spi(0, 0, 32'h6400 + ((60 + i) % 128));
            check_spacing = 0;
            wait_frame_off(5, 16);
            print_and_check_frame(3);
            print_and_check_frame(4);
            if (f_completions[3] != 168 ||
                f_first_push[3] != 176 || f_last_push[3] != 32574 ||
                f_evq_close[3] != 168 || f_overflow_end[3] != 0 ||
                f_evq_open[4] != 168 || f_completions[4] != 2 ||
                f_first_push[4] != 0 || f_last_push[4] != 194 ||
                f_first_drain[4] != 1 || f_last_drain[4] != 195 ||
                f_evq_rejects[4] != 1 || f_sim_pop_rej[4] != 1 ||
                f_evq_admits[4] != 1 || f_pops_noteon[4] != 169 ||
                f_strikes[4] != 169 || f_overflow_end[4] != 1 ||
                f_overrun_end[4] != 0)
                $fatal(1, "sim_pop_reject probe assertion failed");
        end else if (mode == "carry_replenish") begin
            wait_frame_off(2, 32752);
            if (live !== 16'hffff || dut.evq_cnt !== 0 || overrun || overflow)
                $fatal(1, "Invalid setup before carry_replenish");
            check_spacing = 1;
            last_push_cycle = -1;
            last_drain_cycle = -1;
            for (int i = 0; i < 337; i++)
                write_spi(0, 0, 32'h6400 + ((60 + i) % 128));
            check_spacing = 0;
            wait_frame_off(6, 16);
            print_and_check_frame(3);
            print_and_check_frame(4);
            print_and_check_frame(5);
            if (f_completions[3] != 168 || f_evq_admits[3] != 168 ||
                f_evq_close[3] != 168 || f_overflow_end[3] != 0 ||
                f_evq_open[4] != 168 || f_completions[4] != 169 ||
                f_evq_admits[4] != 169 || f_evq_rejects[4] != 0 ||
                f_first_push[4] != 1 || f_last_push[4] != 32593 ||
                f_first_drain[4] != 2 || f_last_drain[4] != 32594 ||
                f_pops_noteon[4] != 194 || f_strikes[4] != 194 ||
                f_evq_close[4] != 143 || f_evpop_last[4] != 5045 ||
                f_overflow_end[4] != 0 || f_overrun_end[4] != 0 ||
                f_evq_open[5] != 143 || f_pops_noteon[5] != 143 ||
                f_strikes[5] != 143 || f_evq_close[5] != 0 ||
                f_overflow_end[5] != 0 || f_overrun_end[5] != 0)
                $fatal(1, "carry_replenish probe assertion failed");
        end else begin
            $fatal(1, "Unknown +mode=%s", mode);
        end
        $finish;
    end

    initial #5000000 $fatal(1, "Frame watchdog");
endmodule
