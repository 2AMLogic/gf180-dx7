// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

extern const VlWide<32>/*1023:0*/ Vtb_dx7_core__ConstPool__CONST_hcd701f6b_0;
extern const VlWide<64>/*2047:0*/ Vtb_dx7_core__ConstPool__CONST_hcf843e9c_0;
extern const VlWide<64>/*2047:0*/ Vtb_dx7_core__ConstPool__CONST_h0848605e_0;

void Vtb_dx7_core___024root___nba_sequent__TOP__3(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___nba_sequent__TOP__3\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ tb_dx7_core__DOT__dut__DOT__l_dec_rate;
    tb_dx7_core__DOT__dut__DOT__l_dec_rate = 0;
    SData/*8:0*/ tb_dx7_core__DOT__dut__DOT__dv_rs;
    tb_dx7_core__DOT__dut__DOT__dv_rs = 0;
    SData/*15:0*/ tb_dx7_core__DOT__dut__DOT__sounding;
    tb_dx7_core__DOT__dut__DOT__sounding = 0;
    SData/*8:0*/ tb_dx7_core__DOT__dut__DOT__dv_offset;
    tb_dx7_core__DOT__dut__DOT__dv_offset = 0;
    SData/*8:0*/ tb_dx7_core__DOT__dut__DOT__dv_gdiv;
    tb_dx7_core__DOT__dut__DOT__dv_gdiv = 0;
    CData/*5:0*/ tb_dx7_core__DOT__dut__DOT__dv_g;
    tb_dx7_core__DOT__dut__DOT__dv_g = 0;
    CData/*7:0*/ tb_dx7_core__DOT__dut__DOT__dv_depth;
    tb_dx7_core__DOT__dut__DOT__dv_depth = 0;
    CData/*1:0*/ tb_dx7_core__DOT__dut__DOT__dv_curve;
    tb_dx7_core__DOT__dut__DOT__dv_curve = 0;
    SData/*13:0*/ tb_dx7_core__DOT__dut__DOT__dv_ols_pre;
    tb_dx7_core__DOT__dut__DOT__dv_ols_pre = 0;
    SData/*15:0*/ tb_dx7_core__DOT__dut__DOT__dv_olsum;
    tb_dx7_core__DOT__dut__DOT__dv_olsum = 0;
    CData/*6:0*/ tb_dx7_core__DOT__dut__DOT__dv_srx;
    tb_dx7_core__DOT__dut__DOT__dv_srx = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__t_up_hit;
    tb_dx7_core__DOT__dut__DOT__t_up_hit = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__t_dn_hit;
    tb_dx7_core__DOT__dut__DOT__t_dn_hit = 0;
    IData/*31:0*/ tb_dx7_core__DOT__dut__DOT__w_pm1a;
    tb_dx7_core__DOT__dut__DOT__w_pm1a = 0;
    QData/*63:0*/ tb_dx7_core__DOT__dut__DOT__w_sh2;
    tb_dx7_core__DOT__dut__DOT__w_sh2 = 0;
    IData/*31:0*/ tb_dx7_core__DOT__dut__DOT__w_pm2a;
    tb_dx7_core__DOT__dut__DOT__w_pm2a = 0;
    IData/*31:0*/ tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in;
    tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s5 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s0;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s0 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s1;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s1 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s2;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s2 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s3;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s3 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s4;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s4 = 0;
    CData/*0:0*/ tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s5;
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s5 = 0;
    SData/*15:0*/ tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16;
    tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16 = 0;
    IData/*31:0*/ tb_dx7_core__DOT__dut__DOT__u_kern__DOT__res32;
    tb_dx7_core__DOT__dut__DOT__u_kern__DOT__res32 = 0;
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__Vfuncout = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s);
    CData/*0:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s);
    SData/*15:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__go;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__go = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_2__env_ix;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_2__env_ix = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_1__env_target;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_1__env_target = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_0__env_level;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_0__env_level = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s);
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s);
    IData/*23:0*/ __VdfgRegularize_hebeb780c_0_20;
    __VdfgRegularize_hebeb780c_0_20 = 0;
    QData/*32:0*/ __VdfgRegularize_hebeb780c_0_21;
    __VdfgRegularize_hebeb780c_0_21 = 0;
    CData/*7:0*/ __VdfgRegularize_hebeb780c_0_22;
    __VdfgRegularize_hebeb780c_0_22 = 0;
    VlWide<3>/*64:0*/ __VdfgRegularize_hebeb780c_0_25;
    VL_ZERO_W(65, __VdfgRegularize_hebeb780c_0_25);
    SData/*10:0*/ __VdfgRegularize_hebeb780c_0_124;
    __VdfgRegularize_hebeb780c_0_124 = 0;
    SData/*10:0*/ __VdfgRegularize_hebeb780c_0_125;
    __VdfgRegularize_hebeb780c_0_125 = 0;
    SData/*10:0*/ __VdfgRegularize_hebeb780c_0_126;
    __VdfgRegularize_hebeb780c_0_126 = 0;
    SData/*10:0*/ __VdfgRegularize_hebeb780c_0_127;
    __VdfgRegularize_hebeb780c_0_127 = 0;
    VlWide<3>/*95:0*/ __Vtemp_4;
    VlWide<3>/*95:0*/ __Vtemp_5;
    VlWide<3>/*95:0*/ __Vtemp_7;
    VlWide<3>/*95:0*/ __Vtemp_8;
    VlWide<3>/*95:0*/ __Vtemp_9;
    VlWide<3>/*95:0*/ __Vtemp_10;
    VlWide<3>/*95:0*/ __Vtemp_11;
    // Body
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_0__env_level 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__189__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[0U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[1U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[2U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[3U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[4U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__190__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[0U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[1U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[2U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[3U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[4U] 
        = vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__191__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(vlSelfRef.__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[5U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[31U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[31U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[31U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[31U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[31U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__193__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__194__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__195__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__192__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[5U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[32U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[32U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[32U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[32U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[32U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__197__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__198__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__199__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__196__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[5U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[33U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[33U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[33U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[33U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[33U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__201__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__202__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__203__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__200__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[5U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[34U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[34U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[34U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[34U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[34U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__205__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__206__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__207__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__204__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[5U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[35U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[35U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[35U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[35U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[35U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__209__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__210__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__211__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__208__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[36U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[36U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[36U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[36U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[36U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__213__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__214__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__215__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__212__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[37U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[37U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[37U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[37U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[37U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__217__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__218__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__219__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__216__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[38U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[38U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[38U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[38U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[38U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__221__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__222__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__223__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__220__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[39U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[39U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[39U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[39U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[39U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__225__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__226__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__227__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__224__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[40U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[40U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[40U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[40U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[40U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__229__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__230__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__231__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__228__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[6U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[41U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[41U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[41U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[41U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[41U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__233__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__234__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__235__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__232__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[42U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[42U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[42U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[42U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[42U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__237__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__238__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__239__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__236__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[43U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[43U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[43U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[43U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[43U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__241__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__242__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__243__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__240__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[44U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[44U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[44U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[44U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[44U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__245__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__246__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__247__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__244__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[45U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[45U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[45U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[45U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[45U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__249__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__250__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__251__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__248__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[46U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[46U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[46U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[46U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[46U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__253__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__254__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__255__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__252__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[7U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[47U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[47U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[47U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[47U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[47U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__257__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__258__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__259__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__256__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[48U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[48U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[48U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[48U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[48U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__261__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__262__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__263__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__260__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[49U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[49U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[49U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[49U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[49U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__265__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__266__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__267__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__264__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[50U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[50U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[50U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[50U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[50U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__269__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__270__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__271__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__268__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[51U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[51U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[51U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[51U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[51U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__273__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__274__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__275__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__272__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[52U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[52U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[52U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[52U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[52U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__277__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__278__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__279__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__276__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[8U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[53U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[53U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[53U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[53U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[53U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__281__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__282__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__283__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__280__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[54U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[54U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[54U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[54U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[54U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__285__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__286__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__287__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__284__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[55U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[55U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[55U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[55U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[55U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__289__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__290__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__291__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__288__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[56U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[56U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[56U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[56U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[56U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__293__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__294__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__295__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__292__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[57U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[57U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[57U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[57U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[57U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__297__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__298__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__299__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__296__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[58U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[58U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[58U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[58U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[58U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__301__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__302__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__303__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__300__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[9U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[59U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[59U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[59U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[59U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[59U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__305__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__306__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__307__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__304__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[60U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[60U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[60U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[60U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[60U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__309__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__310__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__311__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__308__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[61U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[61U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[61U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[61U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[61U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__313__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__314__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__315__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__312__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[62U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[62U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[62U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[62U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[62U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__317__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__318__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__319__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__316__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[63U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[63U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[63U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[63U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[63U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__321__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__322__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__323__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__320__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[64U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[64U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[64U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[64U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[64U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__325__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__326__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__327__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__324__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[10U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[65U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[65U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[65U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[65U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[65U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__329__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__330__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__331__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__328__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[66U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[66U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[66U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[66U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[66U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__333__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__334__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__335__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__332__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[67U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[67U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[67U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[67U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[67U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__337__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__338__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__339__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__336__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[68U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[68U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[68U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[68U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[68U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__341__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__342__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__343__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__340__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[69U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[69U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[69U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[69U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[69U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__345__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__346__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__347__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__344__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[70U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[70U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[70U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[70U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[70U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__349__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__350__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__351__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__348__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[11U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[71U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[71U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[71U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[71U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[71U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__353__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__354__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__355__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__352__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[72U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[72U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[72U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[72U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[72U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__357__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__358__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__359__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__356__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[73U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[73U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[73U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[73U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[73U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__361__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__362__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__363__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__360__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[74U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[74U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[74U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[74U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[74U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__365__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__366__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__367__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__364__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[75U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[75U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[75U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[75U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[75U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__369__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__370__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__371__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__368__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[76U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[76U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[76U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[76U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[76U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__373__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__374__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__375__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__372__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[12U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[77U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[77U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[77U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[77U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[77U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__377__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__378__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__379__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__376__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[78U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[78U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[78U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[78U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[78U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__381__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__382__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__383__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__380__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[79U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[79U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[79U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[79U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[79U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__385__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__386__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__387__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__384__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[80U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[80U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[80U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[80U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[80U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__389__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__390__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__391__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__388__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[81U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[81U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[81U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[81U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[81U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__393__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__394__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__395__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__392__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[82U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[82U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[82U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[82U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[82U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__397__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__398__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__399__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__396__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[13U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[83U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[83U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[83U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[83U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[83U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__401__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__402__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__403__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__400__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[84U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[84U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[84U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[84U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[84U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__405__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__406__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__407__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__404__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[85U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[85U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[85U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[85U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[85U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__409__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__410__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__411__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__408__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[86U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[86U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[86U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[86U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[86U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__413__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__414__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__415__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__412__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[87U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[87U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[87U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[87U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[87U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__417__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__418__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__419__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__416__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[88U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[88U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[88U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[88U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[88U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__421__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__422__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__423__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__420__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[14U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[89U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[89U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[89U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[89U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[89U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__425__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__426__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__427__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__424__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[90U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[90U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[90U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[90U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[90U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__429__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__430__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__431__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s0 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__428__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[91U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[91U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[91U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[91U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[91U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__433__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__434__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__435__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s1 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__432__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[92U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[92U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[92U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[92U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[92U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__437__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__438__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__439__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s2 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__436__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[93U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[93U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[93U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[93U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[93U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__441__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__442__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__443__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s3 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__440__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[94U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[94U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[94U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[94U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[94U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__445__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__446__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__447__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s4 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__444__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__go 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go[15U][5U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[95U][0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[95U][1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[95U][2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[3U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[95U][3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[4U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd[95U][4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[4U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__s[3U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_0__env_level 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__449__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__Vfuncout 
        = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[3U] 
            << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__s[2U] 
                               >> 5U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_1__env_target 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_target__450__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[0U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[0U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[1U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[1U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[2U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[2U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[3U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[3U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[4U] 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__s[4U];
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__Vfuncout 
        = (7U & (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__s[0U] 
                 >> 2U));
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_2__env_ix 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_ix__451__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__Vfuncout 
        = ((((0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_0__env_level) 
             & (0x00100000U == __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_1__env_target)) 
            & (3U <= (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448____VlefCall_2__env_ix))) 
           & (0x3f9cU < (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__go)));
    tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s5 
        = __Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__448__Vfuncout;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_rand_nx 
        = (0x000000ffU & ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                           < vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_delta)
                           ? ((IData)(0x11U) + ((IData)(0xb3U) 
                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand)))
                           : (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_amax = 
        ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_am1b 
          > vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_am2)
          ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_am1b
          : vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_am2);
    tb_dx7_core__DOT__dut__DOT__w_sh2 = VL_SHIFTRS_QQI(64,64,32, 
                                                       VL_MULS_QQQ(64, 
                                                                   VL_EXTENDS_QI(64,32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_pm), 
                                                                   VL_EXTENDS_QI(64,32, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_sl0))), 0x0000000eU);
    VL_EXTENDS_WQ(65,33, __Vtemp_4, (QData)((IData)(
                                                    (vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q 
                                                     * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmdep)))));
    __Vtemp_5[0U] = __Vtemp_4[0U];
    __Vtemp_5[1U] = __Vtemp_4[1U];
    __Vtemp_5[2U] = (1U & __Vtemp_4[2U]);
    VL_EXTENDS_WI(65,32, __Vtemp_7, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_sl0));
    __Vtemp_8[0U] = __Vtemp_7[0U];
    __Vtemp_8[1U] = __Vtemp_7[1U];
    __Vtemp_8[2U] = (1U & __Vtemp_7[2U]);
    VL_MULS_WWW(65, __Vtemp_9, __Vtemp_5, __Vtemp_8);
    __Vtemp_10[0U] = __Vtemp_9[0U];
    __Vtemp_10[1U] = __Vtemp_9[1U];
    __Vtemp_10[2U] = (1U & __Vtemp_9[2U]);
    VL_SHIFTRS_WWI(65,65,32, __Vtemp_11, __Vtemp_10, 0x00000027U);
    __VdfgRegularize_hebeb780c_0_25[0U] = __Vtemp_11[0U];
    __VdfgRegularize_hebeb780c_0_25[1U] = __Vtemp_11[1U];
    __VdfgRegularize_hebeb780c_0_25[2U] = (1U & __Vtemp_11[2U]);
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt;
    tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in = 
        ((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))
          ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
             + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eval_mod_r)
          : ((4U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))
              ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
                 + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eval_mod_r)
              : (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
                 + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eval_mod_r)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__free_idx 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f0)
                           ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note)
                           : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f1)
                               ? ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                               : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f2)
                                   ? ((IData)(2U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                   : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f3)
                                       ? ((IData)(3U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                       : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f4)
                                           ? ((IData)(4U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                           : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f5)
                                               ? ((IData)(5U) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                               : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f6)
                                                   ? 
                                                  ((IData)(6U) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                   : 
                                                  ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f7)
                                                    ? 
                                                   ((IData)(7U) 
                                                    + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                    : 
                                                   ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f8)
                                                     ? 
                                                    ((IData)(8U) 
                                                     + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                     : 
                                                    ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f9)
                                                      ? 
                                                     ((IData)(9U) 
                                                      + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                      : 
                                                     ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f10)
                                                       ? 
                                                      ((IData)(0x0aU) 
                                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                       : 
                                                      ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f11)
                                                        ? 
                                                       ((IData)(0x0bU) 
                                                        + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                        : 
                                                       ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f12)
                                                         ? 
                                                        ((IData)(0x0cU) 
                                                         + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                         : 
                                                        ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f13)
                                                          ? 
                                                         ((IData)(0x0dU) 
                                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                          : 
                                                         ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f14)
                                                           ? 
                                                          ((IData)(0x0eU) 
                                                           + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note))
                                                           : 
                                                          (((IData)(0x0fU) 
                                                            + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note)) 
                                                           & (- (IData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f15))))))))))))))))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_start 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_y = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_y;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[0U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[0U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[1U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[1U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[2U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[2U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[0U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[0U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[1U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[1U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[2U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[2U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_k = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_k;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_T = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_T;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_res = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_res;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_st = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_st;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_x = (0x01ffffffU 
                                                   & (IData)(
                                                             (0x00000003ffffffffULL 
                                                              & (((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_ams)) 
                                                                  * (QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_amd))) 
                                                                 >> 0x00000018U))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[0U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[0U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[1U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[1U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[2U] 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[2U];
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_h = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_h;
    vlSelfRef.__VdfgRegularize_hebeb780c_0_2 = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid
                                                       [vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp]);
    vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = (0x000000ffU 
                                                & (IData)(
                                                          (vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid
                                                           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp] 
                                                           >> 0x00000020U)));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_1 = (1U 
                                                & (IData)(
                                                          (vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid
                                                           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp] 
                                                           >> 0x00000028U)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt;
    tb_dx7_core__DOT__dut__DOT__t_up_hit = VL_GTES_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_up, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt
                                                       [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]);
    tb_dx7_core__DOT__dut__DOT__t_dn_hit = VL_LTES_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_dn, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt
                                                       [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]);
    tb_dx7_core__DOT__dut__DOT__dv_srx = (0x0000007fU 
                                          & ((VL_DIV_III(7, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_note), (IData)(3U)) 
                                              - (IData)(7U)) 
                                             & (- (IData)(
                                                          (0x15U 
                                                           <= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_note))))));
    tb_dx7_core__DOT__dut__DOT__dv_offset = (0x000001ffU 
                                             & ((VL_EXTENDS_II(9,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_note)) 
                                                 - 
                                                 (0x000000ffU 
                                                  & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA)) 
                                                - (IData)(0x0011U)));
    if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
        vlSelfRef.__VdfgRegularize_hebeb780c_0_23 = vlSelfRef.tb_dx7_core__DOT__dut__DOT__c_mode
            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_detune 
            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__c_detune
            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_fine 
            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__c_fine
            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk];
        __VdfgRegularize_hebeb780c_0_22 = vlSelfRef.tb_dx7_core__DOT__dut__DOT__c_coarse
            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk];
    } else {
        vlSelfRef.__VdfgRegularize_hebeb780c_0_23 = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_detune = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_fine = 0U;
        __VdfgRegularize_hebeb780c_0_22 = 0U;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent 
        = (((((((0x0003fffeU & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                 >> 0x0000000eU) & 
                                (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s0) 
                                  & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s1) 
                                     & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s2) 
                                        & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s3) 
                                           & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s4) 
                                              & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__15__KET____DOT__s5)))))) 
                                 << 1U))) | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                              >> 0x0000000eU) 
                                             & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s0) 
                                                & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s1) 
                                                   & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s2) 
                                                      & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s3) 
                                                         & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s4) 
                                                            & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__14__KET____DOT__s5)))))))) 
               << 6U) | (((0x000ffffeU & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                           >> 0x0000000cU) 
                                          & (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s0) 
                                              & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s1) 
                                                 & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s2) 
                                                    & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s3) 
                                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s4) 
                                                          & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__13__KET____DOT__s5)))))) 
                                             << 1U))) 
                          | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                              >> 0x0000000cU) & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s0) 
                                                 & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s1) 
                                                    & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s2) 
                                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s3) 
                                                          & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s4) 
                                                             & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__12__KET____DOT__s5)))))))) 
                         << 4U)) | ((((0x003ffffeU 
                                       & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                           >> 0x0000000aU) 
                                          & (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s0) 
                                              & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s1) 
                                                 & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s2) 
                                                    & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s3) 
                                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s4) 
                                                          & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__11__KET____DOT__s5)))))) 
                                             << 1U))) 
                                      | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                          >> 0x0000000aU) 
                                         & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s0) 
                                            & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s1) 
                                               & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s2) 
                                                  & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s3) 
                                                     & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s4) 
                                                        & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__10__KET____DOT__s5)))))))) 
                                     << 2U) | ((0x00fffffeU 
                                                & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                    >> 8U) 
                                                   & (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s0) 
                                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s1) 
                                                          & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s2) 
                                                             & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s3) 
                                                                & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s4) 
                                                                   & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__9__KET____DOT__s5)))))) 
                                                      << 1U))) 
                                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                   >> 8U) 
                                                  & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s0) 
                                                     & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s1) 
                                                        & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s2) 
                                                           & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s3) 
                                                              & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s4) 
                                                                 & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__8__KET____DOT__s5)))))))))) 
            << 8U) | (((((0x03fffffeU & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                          >> 6U) & 
                                         (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s0) 
                                           & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s1) 
                                              & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s2) 
                                                 & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s3) 
                                                    & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s4) 
                                                       & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__7__KET____DOT__s5)))))) 
                                          << 1U))) 
                         | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                             >> 6U) & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s0) 
                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s1) 
                                          & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s2) 
                                             & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s3) 
                                                & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s4) 
                                                   & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__6__KET____DOT__s5)))))))) 
                        << 6U) | (((0x0ffffffeU & (
                                                   ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                    >> 4U) 
                                                   & (((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s0) 
                                                       & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s1) 
                                                          & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s2) 
                                                             & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s3) 
                                                                & ((IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s4) 
                                                                   & (IData)(tb_dx7_core__DOT__dut__DOT__silgen__BRA__5__KET____DOT__s5)))))) 
                                                      << 1U))) 
                                   | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                       >> 4U) & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s0) 
                                                 & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s1) 
                                                    & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s2) 
                                                       & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s3) 
                                                          & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s4) 
                                                             & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s5)))))))) 
                                  << 4U)) | ((((0x3ffffffeU 
                                                & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                    >> 2U) 
                                                   & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s0) 
                                                       & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s1) 
                                                          & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s2) 
                                                             & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s3) 
                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s4) 
                                                                   & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s5)))))) 
                                                      << 1U))) 
                                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                   >> 2U) 
                                                  & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s0) 
                                                     & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s1) 
                                                        & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s2) 
                                                           & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s3) 
                                                              & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s4) 
                                                                 & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s5)))))))) 
                                              << 2U) 
                                             | ((0xfffffffeU 
                                                 & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                    & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s0) 
                                                        & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s1) 
                                                           & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s2) 
                                                              & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s3) 
                                                                 & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s4) 
                                                                    & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s5)))))) 
                                                       << 1U))) 
                                                | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                   & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s0) 
                                                      & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s1) 
                                                         & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s2) 
                                                            & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s3) 
                                                               & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s4) 
                                                                  & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s5)))))))))));
    tb_dx7_core__DOT__dut__DOT__w_pm2a = ((1U & (IData)(
                                                        (tb_dx7_core__DOT__dut__DOT__w_sh2 
                                                         >> 0x0000001fU)))
                                           ? ((IData)(1U) 
                                              + (~ (IData)(tb_dx7_core__DOT__dut__DOT__w_sh2)))
                                           : (IData)(tb_dx7_core__DOT__dut__DOT__w_sh2));
    tb_dx7_core__DOT__dut__DOT__w_pm1a = ((__VdfgRegularize_hebeb780c_0_25[0U] 
                                           >> 0x0000001fU)
                                           ? ((IData)(1U) 
                                              + (~ __VdfgRegularize_hebeb780c_0_25[0U]))
                                           : __VdfgRegularize_hebeb780c_0_25[0U]);
    tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16 
        = (0x0000ffffU & (((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))
                            ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv1)
                            : ((4U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))
                                ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv2)
                                : vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv)) 
                          + (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinlog_rom
                             [(0x000003ffU & ((- (IData)(
                                                         (1U 
                                                          & (tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in 
                                                             >> 0x00000016U)))) 
                                              ^ (tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in 
                                                 >> 0x0000000cU)))] 
                             | (0x8000U & (- (IData)(
                                                     (1U 
                                                      & (tb_dx7_core__DOT__dut__DOT__ar_eval_phase_in 
                                                         >> 0x00000017U))))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_hn = 
        (1ULL & ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_h 
                  * (0x00fffffffffffffcULL & (((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[1U])) 
                                               << 0x00000022U) 
                                              | ((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[0U])) 
                                                 << 2U)))) 
                 >> 0x00000038U));
    tb_dx7_core__DOT__dut__DOT__l_dec_rate = (0x000000ffU 
                                              & ((1U 
                                                  == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))
                                                  ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globB
                                                  : vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a0 = (0x000000ffU 
                                                  & ((IData)(0x63U) 
                                                     - 
                                                     ((1U 
                                                       == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))
                                                       ? 
                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globB 
                                                       >> 8U)
                                                       : 
                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                       >> 8U))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff2 = 
        ((1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__rst_n))) 
         || (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff1));
    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_act) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_adv 
            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegris
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                ? (IData)(tb_dx7_core__DOT__dut__DOT__t_up_hit)
                : (IData)(tb_dx7_core__DOT__dut__DOT__t_dn_hit));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_nxt 
            = (((IData)(tb_dx7_core__DOT__dut__DOT__t_up_hit) 
                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegris
                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn])
                ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                : (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegris
                   [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                    ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_up
                    : ((IData)(tb_dx7_core__DOT__dut__DOT__t_dn_hit)
                        ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt
                       [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        : vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_dn)));
    } else {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_adv = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_nxt 
            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl
            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn];
    }
    tb_dx7_core__DOT__dut__DOT__dv_rs = (0x0000003fU 
                                         & (((0x000000ffU 
                                              & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB 
                                                 >> 8U)) 
                                             * ((0x1fU 
                                                 < (IData)(tb_dx7_core__DOT__dut__DOT__dv_srx))
                                                 ? 0x001fU
                                                 : (IData)(tb_dx7_core__DOT__dut__DOT__dv_srx))) 
                                            >> 3U));
    if (VL_LTES_III(9, 0U, (IData)(tb_dx7_core__DOT__dut__DOT__dv_offset))) {
        tb_dx7_core__DOT__dut__DOT__dv_curve = (3U 
                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA 
                                                   >> 0x0000000bU));
        tb_dx7_core__DOT__dut__DOT__dv_depth = (0x000000ffU 
                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA 
                                                   >> 0x0000000fU));
    } else {
        tb_dx7_core__DOT__dut__DOT__dv_curve = (3U 
                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA 
                                                   >> 0x0000000dU));
        tb_dx7_core__DOT__dut__DOT__dv_depth = (0x000000ffU 
                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA 
                                                   >> 0x00000017U));
    }
    tb_dx7_core__DOT__dut__DOT__dv_gdiv = (0x000001ffU 
                                           & VL_DIV_III(9, 
                                                        (0x000001ffU 
                                                         & (VL_LTES_III(9, 0U, (IData)(tb_dx7_core__DOT__dut__DOT__dv_offset))
                                                             ? 
                                                            ((IData)(1U) 
                                                             + (IData)(tb_dx7_core__DOT__dut__DOT__dv_offset))
                                                             : 
                                                            ((IData)(1U) 
                                                             - (IData)(tb_dx7_core__DOT__dut__DOT__dv_offset)))), (IData)(3U)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpf0 = 
        (((IData)(0x00440878U) * (((IData)(0x00000064U) 
                                   * (3U & (IData)(__VdfgRegularize_hebeb780c_0_22))) 
                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_fine))) 
         >> 3U);
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpr1 = 
        ((IData)(0x03080731U) + (Vtb_dx7_core__ConstPool__CONST_hcd701f6b_0
                                 [(0x0000001fU & (IData)(__VdfgRegularize_hebeb780c_0_22))] 
                                 + (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))
                                      ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd
                                     [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk]
                                      : 0U) + VL_MULS_III(32, (IData)(0x00155555U), (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_note)))));
    tb_dx7_core__DOT__dut__DOT__sounding = ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent)) 
                                            & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmax = 
        (VL_GTS_III(32, tb_dx7_core__DOT__dut__DOT__w_pm1a, tb_dx7_core__DOT__dut__DOT__w_pm2a)
          ? tb_dx7_core__DOT__dut__DOT__w_pm1a : tb_dx7_core__DOT__dut__DOT__w_pm2a);
    tb_dx7_core__DOT__dut__DOT__u_kern__DOT__res32 
        = ((0x00001fffU & ((IData)(0x1000U) + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinexp_rom
                           [(0x000003ffU & (~ (IData)(tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16)))])) 
           >> (0x0000001fU & ((IData)(tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16) 
                              >> 0x0000000aU)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_sr1 = (
                                                   (0U 
                                                    == (IData)(tb_dx7_core__DOT__dut__DOT__l_dec_rate))
                                                    ? 1U
                                                    : 
                                                   (0x0000ffffU 
                                                    & (((IData)(0x000000a5U) 
                                                        * (IData)(tb_dx7_core__DOT__dut__DOT__l_dec_rate)) 
                                                       >> 6U)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a1 = (0x0000ffffU 
                                                  & VL_SHIFTL_III(16,16,32, 
                                                                  ((IData)(0x0010U) 
                                                                   + 
                                                                   (0x0fU 
                                                                    & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a0))), 
                                                                  ((IData)(1U) 
                                                                   + 
                                                                   (0x0000000fU 
                                                                    & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a0) 
                                                                       >> 4U)))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_rs_eff 
        = (0x0000003fU & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_rs) 
                          | (- (IData)((0x0040U <= (IData)(tb_dx7_core__DOT__dut__DOT__dv_rs))))));
    __VdfgRegularize_hebeb780c_0_124 = (0x000007ffU 
                                        & ((0x0000007fU 
                                            & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                                               >> 8U)) 
                                           + (IData)(tb_dx7_core__DOT__dut__DOT__dv_rs)));
    __VdfgRegularize_hebeb780c_0_125 = (0x000007ffU 
                                        & ((0x0000007fU 
                                            & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                                               >> 0x00000010U)) 
                                           + (IData)(tb_dx7_core__DOT__dut__DOT__dv_rs)));
    __VdfgRegularize_hebeb780c_0_126 = (0x000007ffU 
                                        & ((0x0000007fU 
                                            & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                                               >> 0x00000018U)) 
                                           + (IData)(tb_dx7_core__DOT__dut__DOT__dv_rs)));
    __VdfgRegularize_hebeb780c_0_127 = (0x000007ffU 
                                        & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_rs) 
                                           + (0x0000007fU 
                                              & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates)));
    tb_dx7_core__DOT__dut__DOT__dv_g = ((0x0020U < (IData)(tb_dx7_core__DOT__dut__DOT__dv_gdiv))
                                         ? 0x00000020U
                                         : (0x0000003fU 
                                            & (IData)(tb_dx7_core__DOT__dut__DOT__dv_gdiv)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 0U;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0U;
    if ((1U & ((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel)
                : (IData)(tb_dx7_core__DOT__dut__DOT__sounding)))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 1U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 1U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[1U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 1U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 2U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 2U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[2U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 2U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 3U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 3U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[3U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 3U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 4U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 4U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[4U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 4U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 5U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 5U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[5U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 5U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 6U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 6U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[6U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 6U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 7U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 7U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[7U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 7U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 8U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 8U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[8U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 8U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 9U) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                              >> 9U)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                         | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[9U] 
                                            < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 9U;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0aU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0aU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[10U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0aU;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0bU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0bU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[11U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0bU;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0cU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0cU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[12U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0cU;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0dU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0dU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[13U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0dU;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0eU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0eU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[14U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0eU;
    }
    if ((1U & (((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel))
                 ? ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_rel) 
                    >> 0x0fU) : ((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
                                 >> 0x0fU)) & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found)) 
                                               | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[15U] 
                                                  < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx]))))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx = 0x0fU;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 0U;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0U;
    if (((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
         & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[0U] 
            == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note)))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 1U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[1U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[1U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 1U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 2U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[2U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[2U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 2U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 3U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[3U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[3U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 3U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 4U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[4U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[4U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 4U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 5U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[5U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[5U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 5U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 6U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[6U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[6U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 6U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 7U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[7U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[7U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 7U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 8U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[8U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[8U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 8U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 9U) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[9U] 
                     == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[9U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 9U;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0aU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[10U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[10U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0aU;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0bU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[11U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[11U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0bU;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0cU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[12U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[12U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0cU;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0dU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[13U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[13U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0dU;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0eU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[14U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[14U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0eU;
    }
    if (((((IData)(tb_dx7_core__DOT__dut__DOT__sounding) 
           >> 0x0fU) & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[15U] 
                        == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note))) 
         & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found)) 
            | (vlSelfRef.tb_dx7_core__DOT__dut__DOT__age[15U] 
               < vlSelfRef.tb_dx7_core__DOT__dut__DOT__age
               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx])))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx = 0x0fU;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y 
        = ((0x00008000U & (IData)(tb_dx7_core__DOT__dut__DOT__u_kern__DOT__exp_val16))
            ? ((~ tb_dx7_core__DOT__dut__DOT__u_kern__DOT__res32) 
               << 0x0000000dU) : (tb_dx7_core__DOT__dut__DOT__u_kern__DOT__res32 
                                  << 0x0000000dU));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff1 = 
        (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__rst_n)));
    if ((0x0040U <= (IData)(tb_dx7_core__DOT__dut__DOT__dv_rs))) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1 
            = (0x0000007fU & (((0x0063U < (IData)(__VdfgRegularize_hebeb780c_0_124))
                                ? 0x0063U : (IData)(__VdfgRegularize_hebeb780c_0_124)) 
                              - (IData)(0x3fU)));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2 
            = (0x0000007fU & (((0x0063U < (IData)(__VdfgRegularize_hebeb780c_0_125))
                                ? 0x0063U : (IData)(__VdfgRegularize_hebeb780c_0_125)) 
                              - (IData)(0x3fU)));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r3 
            = (0x0000007fU & (((0x0063U < (IData)(__VdfgRegularize_hebeb780c_0_126))
                                ? 0x0063U : (IData)(__VdfgRegularize_hebeb780c_0_126)) 
                              - (IData)(0x3fU)));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r0 
            = (0x0000007fU & (((0x0063U < (IData)(__VdfgRegularize_hebeb780c_0_127))
                                ? 0x0063U : (IData)(__VdfgRegularize_hebeb780c_0_127)) 
                              - (IData)(0x3fU)));
    } else {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1 
            = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                              >> 8U));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2 
            = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                              >> 0x00000010U));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r3 
            = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates 
                              >> 0x00000018U));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r0 
            = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_erates);
    }
    __VdfgRegularize_hebeb780c_0_21 = (0x00000001ffffffffULL 
                                       & VL_EXTENDS_QI(33,31, 
                                                       (0x003fffffU 
                                                        & VL_EXTENDS_II(22,14, 
                                                                        (0x000001ffU 
                                                                         & (((IData)(0x00000149U) 
                                                                             * 
                                                                             (0x001fffffU 
                                                                              & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_depth) 
                                                                                * (IData)(tb_dx7_core__DOT__dut__DOT__dv_g)))) 
                                                                            >> 0x0000000cU))))));
    __VdfgRegularize_hebeb780c_0_20 = (0x00ffffffU 
                                       & VL_EXTENDS_II(24,15, 
                                                       (0x000000ffU 
                                                        & (((IData)(0x00000149U) 
                                                            * 
                                                            (0x007fffffU 
                                                             & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_depth) 
                                                                * 
                                                                (0x000000ffU 
                                                                 & Vtb_dx7_core__ConstPool__CONST_hcf843e9c_0
                                                                 [
                                                                 (0x07ffffffU 
                                                                  & (IData)(tb_dx7_core__DOT__dut__DOT__dv_g))])))) 
                                                           >> 0x0000000fU))));
    tb_dx7_core__DOT__dut__DOT__dv_ols_pre = (0x00003fffU 
                                              & ((0x0000ffffU 
                                                  & VL_EXTENDS_II(16,13, 
                                                                  (0x0000007fU 
                                                                   & ((0x00000040U 
                                                                       & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                       ? 
                                                                      ((IData)(0x1cU) 
                                                                       + vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                       : 
                                                                      ((0x00000020U 
                                                                        & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                        ? 
                                                                       ((IData)(0x1cU) 
                                                                        + vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                        : 
                                                                       ((0x00000010U 
                                                                         & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                         ? 
                                                                        ((8U 
                                                                          & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                          ? 
                                                                         ((IData)(0x1cU) 
                                                                          + vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                          : 
                                                                         ((4U 
                                                                           & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                           ? 
                                                                          ((IData)(0x1cU) 
                                                                           + vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                           : 
                                                                          ((2U 
                                                                            & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                            ? 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x2eU
                                                                             : 0x2dU)
                                                                            : 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x2bU
                                                                             : 0x2aU))))
                                                                         : 
                                                                        ((8U 
                                                                          & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                          ? 
                                                                         ((4U 
                                                                           & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                           ? 
                                                                          ((2U 
                                                                            & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                            ? 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x29U
                                                                             : 0x27U)
                                                                            : 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x25U
                                                                             : 0x23U))
                                                                           : 
                                                                          ((2U 
                                                                            & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                            ? 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x21U
                                                                             : 0x1fU)
                                                                            : 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x1dU
                                                                             : 0x1bU)))
                                                                          : 
                                                                         ((4U 
                                                                           & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                           ? 
                                                                          ((2U 
                                                                            & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                            ? 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x19U
                                                                             : 0x17U)
                                                                            : 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x14U
                                                                             : 0x11U))
                                                                           : 
                                                                          ((2U 
                                                                            & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                            ? 
                                                                           ((1U 
                                                                             & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB)
                                                                             ? 0x0dU
                                                                             : 9U)
                                                                            : 
                                                                           (5U 
                                                                            & (- (IData)(
                                                                                (1U 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleB))))))))))))) 
                                                 + 
                                                 (((0U 
                                                    == (IData)(tb_dx7_core__DOT__dut__DOT__dv_curve)) 
                                                   | (3U 
                                                      == (IData)(tb_dx7_core__DOT__dut__DOT__dv_curve)))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(tb_dx7_core__DOT__dut__DOT__dv_curve))
                                                    ? (IData)(__VdfgRegularize_hebeb780c_0_21)
                                                    : (IData)(
                                                              (0x00000001ffffffffULL 
                                                               & (- __VdfgRegularize_hebeb780c_0_21))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(tb_dx7_core__DOT__dut__DOT__dv_curve))
                                                    ? __VdfgRegularize_hebeb780c_0_20
                                                    : (IData)(
                                                              (0x00000001ffffffffULL 
                                                               & (- (QData)((IData)(__VdfgRegularize_hebeb780c_0_20)))))))));
    tb_dx7_core__DOT__dut__DOT__dv_olsum = (0x0000ffffU 
                                            & ((0x000fffffU 
                                                & VL_EXTENDS_II(20,19, 
                                                                (VL_LTS_III(14, 0x007fU, (IData)(tb_dx7_core__DOT__dut__DOT__dv_ols_pre))
                                                                  ? 0x00000fe0U
                                                                  : 
                                                                 (0x0003ffe0U 
                                                                  & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_ols_pre) 
                                                                     << 5U))))) 
                                               + (0x0000fff0U 
                                                  & (VL_SHIFTRS_III(20,20,32, 
                                                                    (0x000fffffU 
                                                                     & ((IData)(7U) 
                                                                        + 
                                                                        VL_MULS_III(20, 
                                                                                (0x000fffffU 
                                                                                & VL_EXTENDS_II(20,4, 
                                                                                (7U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_scaleA 
                                                                                >> 8U)))), 
                                                                                (0x000fffffU 
                                                                                & VL_EXTENDS_II(20,9, 
                                                                                (0x000001ffU 
                                                                                & ((0x000000ffU 
                                                                                & Vtb_dx7_core__ConstPool__CONST_h0848605e_0
                                                                                [
                                                                                (0x0000003fU 
                                                                                & ((0x12U 
                                                                                == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))
                                                                                 ? 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel
                                                                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn] 
                                                                                >> 1U)
                                                                                 : 
                                                                                ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_vel) 
                                                                                >> 1U)))]) 
                                                                                - (IData)(0x00efU)))))))), 3U) 
                                                     << 4U))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_outlevel 
        = (0x00001fffU & ((IData)(tb_dx7_core__DOT__dut__DOT__dv_olsum) 
                          & (- (IData)(VL_LTES_III(16, 0U, (IData)(tb_dx7_core__DOT__dut__DOT__dv_olsum))))));
}
