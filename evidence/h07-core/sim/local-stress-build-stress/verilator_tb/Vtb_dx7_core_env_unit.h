// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_dx7_core.h for the primary calling header

#ifndef VERILATED_VTB_DX7_CORE_ENV_UNIT_H_
#define VERILATED_VTB_DX7_CORE_ENV_UNIT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_dx7_core__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_dx7_core_env_unit final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ clk;
    CData/*0:0*/ rst;
    CData/*0:0*/ cmd_en;
    CData/*2:0*/ cmd;
    CData/*0:0*/ busy;
    CData/*0:0*/ done;
    CData/*0:0*/ slot_wr_en;
    CData/*0:0*/ __PVT__state_obs;
    CData/*2:0*/ __PVT__ix_q;
    CData/*0:0*/ __PVT__rising_q;
    CData/*0:0*/ __PVT__down_q;
    CData/*5:0*/ __PVT__rs_q;
    CData/*3:0*/ __PVT__state_q;
    CData/*3:0*/ __PVT__adv_ix;
    CData/*0:0*/ __PVT__adv_ret_mov;
    CData/*6:0*/ __PVT__newlv_r;
    CData/*0:0*/ __PVT__rising_new;
    CData/*5:0*/ __PVT__qr_r;
    CData/*0:0*/ __PVT__static_cond_r;
    CData/*0:0*/ __PVT__st_adv_r;
    CData/*0:0*/ __PVT__kd_fire;
    CData/*6:0*/ __PVT__adv_lv;
    CData/*7:0*/ __PVT__qsum;
    CData/*6:0*/ __PVT__sr_w;
    CData/*0:0*/ __PVT__rise_clamp;
    CData/*0:0*/ __PVT__fall_clamp;
    CData/*0:0*/ __PVT__mov_step;
    CData/*0:0*/ __PVT__mov_clamp;
    CData/*3:0*/ __PVT__nstate;
    SData/*12:0*/ __PVT__outlevel_q;
    SData/*15:0*/ __PVT__act16;
    VlWide<5>/*132:0*/ cmd_data;
    VlWide<5>/*132:0*/ slot_wr_data;
    VlWide<5>/*132:0*/ slot_rd_data;
    VlWide<3>/*74:0*/ __PVT__param_rd;
    IData/*31:0*/ __PVT__level_q;
    IData/*31:0*/ __PVT__target_q;
    IData/*31:0*/ __PVT__inc_q;
    IData/*31:0*/ __PVT__static_q;
    IData/*27:0*/ __PVT__tgt_new;
    IData/*20:0*/ __PVT__raw_static_r;
    IData/*31:0*/ __PVT__stat_new_r;
    IData/*25:0*/ __PVT__inc_pre_r;
    IData/*31:0*/ __PVT__st_after_r;
    IData/*20:0*/ __PVT__raw_base;
    QData/*41:0*/ __PVT__rise_sum;
    QData/*32:0*/ __PVT__fall_diff;
    VlUnpacked<CData/*6:0*/, 4> __PVT__rates_q;
    VlUnpacked<CData/*6:0*/, 4> __PVT__levels_q;

    // INTERNAL VARIABLES
    Vtb_dx7_core__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_dx7_core_env_unit();
    ~Vtb_dx7_core_env_unit();
    void ctor(Vtb_dx7_core__Syms* symsp, const char* namep);
    void dtor();
    VL_UNCOPYABLE(Vtb_dx7_core_env_unit);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
