# dx7core: 24.576 MHz core clock = 40.692 ns (DR-0008 design point: 512 clocks per
# 48 kHz frame). I/O delays 20 % of the period, same convention as the sibling
# synth_top constraint (sibling ran the 12.288 MHz fallback at 81.38 ns; this run's
# clock is the 24.576 MHz fallback-free design point).
set clk_period 40.692
create_clock -name clk -period $clk_period [get_ports clk]
set_input_delay  [expr $clk_period * 0.2] -clock clk [all_inputs -no_clocks]
set_output_delay [expr $clk_period * 0.2] -clock clk [all_outputs]