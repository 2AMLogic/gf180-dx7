# sta-synth.tcl -- SYNTHESIS-STAGE static timing of a mapped gf180mcu 7t netlist at the three 5.0 V
# corners (H10, issue #32). Adapted from 2AMLogic/gf180-parasynth pnr/orfs/sta-corners.tcl @ cbcc8b9e
# (Apache-2.0): the corner setup, period lookup and per-corner "=== CORNER" block are upstream
# verbatim (tools/h10_collect.py parses that line format); netlist input, ideal clock, no
# extraction and the report set are ours. Provenance: docs/reuse/catalog.json entry
# orfs-dx7core-design-and-synth-sta.
#
# What this stage is and is not:
#   * the netlist is the unplaced, unsized synthesis result: NO wire parasitics (no placement exists),
#     IDEAL clock (no clock tree exists), no repair_timing / resizing;
#   * it is the "synth" row of the per-stage timing table, labelled as such -- it is not post-route
#     timing, and a positive slack here establishes nothing about the routed design;
#   * a NEGATIVE slack here is a measured property of the mapped logic depth at that corner, before
#     any flow repair; the flow's resizer may or may not recover it (that is the place/CTS/route rows).
#
# Inputs (environment): NETLIST, TOP, SDC, LIB_TT, LIB_SS, LIB_FF. Run by sta-synth.sh inside the
# pinned ORFS image (its standalone OpenSTA build, `sta`).
set netlist $::env(NETLIST)
define_corners tt ss ff
read_liberty -corner tt $::env(LIB_TT)
read_liberty -corner ss $::env(LIB_SS)
read_liberty -corner ff $::env(LIB_FF)
read_verilog $netlist
link_design $::env(TOP)
read_sdc $::env(SDC)

set clk [lindex [all_clocks] 0]
set period [get_property -object_type clock $clk period]
puts "=== STA_SYNTH design=$::env(TOP) stage=synth clock_period_ns=$period parasitics=none clock=ideal lib=7t5v0 tt_025C_5v00/ss_125C_4v50/ff_n40C_5v50"
foreach c {tt ss ff} {
  set ws_max [worst_slack -corner $c -max]
  set ws_min [worst_slack -corner $c -min]
  set tns_max [total_negative_slack -corner $c -max]
  set tns_min [total_negative_slack -corner $c -min]
  # implied minimum period: period - WNS (an arithmetic consequence of this stage's WNS, not a closure run)
  set minp [expr $period - $ws_max]
  puts [format "=== CORNER %s setup_wns_ns=%+.3f setup_tns_ns=%.3f hold_wns_ns=%+.3f hold_tns_ns=%.3f implied_min_period_ns=%.2f implied_fmax_mhz=%.1f" \
    $c $ws_max $tns_max $ws_min $tns_min $minp [expr 1000.0/$minp]]
}
foreach c {ss tt ff} {
  puts "=== WORST SETUP PATH corner $c"
  report_checks -corner $c -path_delay max -group_path_count 1 -fields {slew cap fanout}
}
puts "=== WORST HOLD PATH corner ff"
report_checks -corner ff -path_delay min -group_path_count 1
puts "=== DONE"
