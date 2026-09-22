// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U]))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[64U]) 
                                                      << 2U))
                                                     : 0U)))));
}

extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h46231d0d_0;
extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h949cbf09_0;

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[65U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 1U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[66U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 2U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((8U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[67U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 3U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000010U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[68U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 4U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000020U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[69U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 5U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000040U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[70U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 6U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000080U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[71U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 7U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000100U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[72U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 8U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000200U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[73U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 9U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000400U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[74U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00000800U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[75U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00001000U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[76U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__1(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__PVT__nstate = (0x0000000fU & ((8U & (IData)(vlSelfRef.__PVT__state_q))
                                               ? ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  (7U 
                                                   & (- (IData)((IData)(
                                                                        ((0U 
                                                                          == 
                                                                          (3U 
                                                                           & (IData)(vlSelfRef.__PVT__state_q))) 
                                                                         & (IData)(vlSelfRef.__PVT__adv_ret_mov))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0cU
                                                     : 0x0bU)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 0x0aU
                                                     : 9U)))
                                               : ((4U 
                                                   & (IData)(vlSelfRef.__PVT__state_q))
                                                   ? 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__mov_clamp))))
                                                     : 
                                                    ((IData)(vlSelfRef.__PVT__st_adv_r)
                                                      ? 8U
                                                      : 7U))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__state_q))
                                                     ? 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__down_q))))
                                                     : 
                                                    (8U 
                                                     & (- (IData)((IData)(vlSelfRef.__PVT__kd_fire))))))
                                                   : 
                                                  ((2U 
                                                    & (IData)(vlSelfRef.__PVT__state_q))
                                                    ? 
                                                   (8U 
                                                    & (- (IData)(
                                                                 (1U 
                                                                  & (IData)(vlSelfRef.__PVT__state_q)))))
                                                    : 
                                                   ((1U 
                                                     & ((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U]) 
                                                      << 2U))
                                                     : 0U)))));
}

void Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___nba_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    IData/*31:0*/ __Vdly__level_q;
    __Vdly__level_q = 0;
    IData/*31:0*/ __Vdly__target_q;
    __Vdly__target_q = 0;
    IData/*31:0*/ __Vdly__static_q;
    __Vdly__static_q = 0;
    CData/*2:0*/ __Vdly__ix_q;
    __Vdly__ix_q = 0;
    CData/*0:0*/ __Vdly__rising_q;
    __Vdly__rising_q = 0;
    CData/*0:0*/ __Vdly__down_q;
    __Vdly__down_q = 0;
    CData/*3:0*/ __Vdly__adv_ix;
    __Vdly__adv_ix = 0;
    IData/*31:0*/ __Vdly__st_after_r;
    __Vdly__st_after_r = 0;
    CData/*0:0*/ __VdlySet__rates_q__v0;
    __VdlySet__rates_q__v0 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v1;
    __VdlySet__rates_q__v1 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v4;
    __VdlyVal__rates_q__v4 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v4;
    __VdlySet__rates_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v4;
    __VdlyVal__levels_q__v4 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v5;
    __VdlyVal__rates_q__v5 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v5;
    __VdlySet__rates_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v5;
    __VdlyVal__levels_q__v5 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v6;
    __VdlyVal__rates_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v6;
    __VdlyVal__levels_q__v6 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v7;
    __VdlyVal__rates_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v7;
    __VdlyVal__levels_q__v7 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v8;
    __VdlyVal__rates_q__v8 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v8;
    __VdlySet__rates_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v8;
    __VdlyVal__levels_q__v8 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v9;
    __VdlyVal__rates_q__v9 = 0;
    CData/*0:0*/ __VdlySet__rates_q__v9;
    __VdlySet__rates_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v9;
    __VdlyVal__levels_q__v9 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v10;
    __VdlyVal__rates_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v10;
    __VdlyVal__levels_q__v10 = 0;
    CData/*6:0*/ __VdlyVal__rates_q__v11;
    __VdlyVal__rates_q__v11 = 0;
    CData/*6:0*/ __VdlyVal__levels_q__v11;
    __VdlyVal__levels_q__v11 = 0;
    // Body
    __Vdly__st_after_r = vlSelfRef.__PVT__st_after_r;
    __Vdly__adv_ix = vlSelfRef.__PVT__adv_ix;
    __VdlySet__rates_q__v0 = 0U;
    __VdlySet__rates_q__v1 = 0U;
    __VdlySet__rates_q__v4 = 0U;
    __VdlySet__rates_q__v5 = 0U;
    __VdlySet__rates_q__v8 = 0U;
    __VdlySet__rates_q__v9 = 0U;
    __Vdly__rising_q = vlSelfRef.__PVT__rising_q;
    __Vdly__static_q = vlSelfRef.__PVT__static_q;
    __Vdly__ix_q = vlSelfRef.__PVT__ix_q;
    __Vdly__down_q = vlSelfRef.__PVT__down_q;
    __Vdly__target_q = vlSelfRef.__PVT__target_q;
    __Vdly__level_q = vlSelfRef.__PVT__level_q;
    if (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__level_q = 0U;
        __Vdly__target_q = 0U;
        vlSelfRef.__PVT__inc_q = 0U;
        __Vdly__static_q = 0U;
        __Vdly__ix_q = 0U;
        __Vdly__rising_q = 0U;
        __Vdly__down_q = 0U;
        __VdlySet__rates_q__v0 = 1U;
        vlSelfRef.__PVT__outlevel_q = 0U;
        vlSelfRef.__PVT__rs_q = 0U;
        __Vdly__adv_ix = 0U;
        vlSelfRef.__PVT__adv_ret_mov = 0U;
        __Vdly__st_after_r = 0U;
        vlSelfRef.__PVT__st_adv_r = 0U;
        vlSelfRef.__PVT__kd_fire = 0U;
        __VdlySet__rates_q__v1 = 1U;
        vlSelfRef.__PVT__state_q = 0U;
    } else {
        if ((8U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                              >> 1U)))) {
                    if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                        if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                            __Vdly__rising_q = vlSelfRef.__PVT__rising_new;
                            __Vdly__target_q = vlSelfRef.__PVT__tgt_new;
                            __Vdly__static_q = vlSelfRef.__PVT__stat_new_r;
                            vlSelfRef.__PVT__inc_q 
                                = (IData)(((0x0000000000eb3333ULL 
                                            * (QData)((IData)(vlSelfRef.__PVT__inc_pre_r))) 
                                           >> 0x00000018U));
                        } else {
                            __Vdly__ix_q = (7U & (IData)(vlSelfRef.__PVT__adv_ix));
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    vlSelfRef.__PVT__stat_new_r = (0x1fffffffU 
                                                   & (IData)(
                                                             ((0x0000000000eb3333ULL 
                                                               * (QData)((IData)(vlSelfRef.__PVT__raw_static_r))) 
                                                              >> 0x00000018U)));
                    vlSelfRef.__PVT__inc_pre_r = (0x03ffffffU 
                                                  & (((IData)(4U) 
                                                      + 
                                                      (3U 
                                                       & (IData)(vlSelfRef.__PVT__qr_r))) 
                                                     << 
                                                     (0x0000001fU 
                                                      & ((IData)(8U) 
                                                         + 
                                                         (0x0000000fU 
                                                          & ((IData)(vlSelfRef.__PVT__qr_r) 
                                                             >> 2U))))));
                } else {
                    vlSelfRef.__PVT__raw_static_r = 
                        (0x001fffffU & ((((0x4dU > (IData)(vlSelfRef.__PVT__sr_w)) 
                                          & ((0U == 
                                              (7U & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                             & (0U 
                                                == (IData)(vlSelfRef.__PVT__newlv_r))))
                                          ? (IData)(
                                                    (0x00000000001fffffULL 
                                                     & ((0x00000000000ccccdULL 
                                                         * (QData)((IData)(vlSelfRef.__PVT__raw_base))) 
                                                        >> 0x00000018U)))
                                          : vlSelfRef.__PVT__raw_base) 
                                        & (- (IData)((IData)(vlSelfRef.__PVT__static_cond_r)))));
                }
            } else if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                vlSelfRef.__PVT__rising_new = ((QData)((IData)(vlSelfRef.__PVT__tgt_new)) 
                                               > (QData)((IData)(vlSelfRef.__PVT__level_q)));
                vlSelfRef.__PVT__qr_r = (0x0000003fU 
                                         & ((IData)(vlSelfRef.__PVT__qsum) 
                                            | (- (IData)(
                                                         (0x3fU 
                                                          < (IData)(vlSelfRef.__PVT__qsum))))));
                vlSelfRef.__PVT__static_cond_r = ((vlSelfRef.__PVT__tgt_new 
                                                   == vlSelfRef.__PVT__level_q) 
                                                  | ((0U 
                                                      == 
                                                      (7U 
                                                       & (IData)(vlSelfRef.__PVT__adv_ix))) 
                                                     & (0U 
                                                        == (IData)(vlSelfRef.__PVT__newlv_r))));
            } else {
                vlSelfRef.__PVT__tgt_new = (VL_GTS_III(16, 0x0010U, (IData)(vlSelfRef.__PVT__act16))
                                             ? 0x00100000U
                                             : (0x0fff0000U 
                                                & ((IData)(vlSelfRef.__PVT__act16) 
                                                   << 0x00000010U)));
                vlSelfRef.__PVT__newlv_r = vlSelfRef.__PVT__adv_lv;
            }
        } else if ((4U & (IData)(vlSelfRef.__PVT__state_q))) {
            if ((2U & (IData)(vlSelfRef.__PVT__state_q))) {
                if ((1U & (IData)(vlSelfRef.__PVT__state_q))) {
                    if (vlSelfRef.__PVT__mov_step) {
                        __Vdly__level_q = ((IData)(vlSelfRef.__PVT__rising_q)
                                            ? ((IData)(vlSelfRef.__PVT__rise_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__rise_sum))
                                            : ((IData)(vlSelfRef.__PVT__fall_clamp)
                                                ? vlSelfRef.__PVT__target_q
                                                : (IData)(vlSelfRef.__PVT__fall_diff)));
                        if (vlSelfRef.__PVT__mov_clamp) {
                            __Vdly__adv_ix = (0x0000000fU 
                                              & ((IData)(1U) 
                                                 + (IData)(vlSelfRef.__PVT__ix_q)));
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        }
                    }
                } else {
                    __Vdly__static_q = vlSelfRef.__PVT__st_after_r;
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__state_q) 
                             >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.__PVT__state_q)))) {
                if ((0x00002000U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U])) {
                    if ((2U != vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                        vlSelfRef.__PVT__kd_fire = 0U;
                    }
                    if ((4U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                        if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                            if ((1U & (~ vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U]))) {
                                __Vdly__level_q = (
                                                   (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][4U] 
                                                    << 0x0000001bU) 
                                                   | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][3U] 
                                                      >> 5U));
                                __Vdly__target_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][3U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                        >> 5U));
                                vlSelfRef.__PVT__inc_q 
                                    = ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                        << 0x0000001bU) 
                                       | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                          >> 5U));
                                __Vdly__static_q = 
                                    ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                      << 0x0000001bU) 
                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                        >> 5U));
                                __Vdly__ix_q = (7U 
                                                & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                                   >> 2U));
                                __Vdly__rising_q = 
                                    (1U & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                           >> 1U));
                                __Vdly__down_q = (1U 
                                                  & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U]);
                            }
                        } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                            __Vdly__adv_ix = (7U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U]);
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__st_adv_r 
                                = ((0U != vlSelfRef.__PVT__static_q) 
                                   & (0x00000040U >= vlSelfRef.__PVT__static_q));
                            __Vdly__st_after_r = ((0U 
                                                   == vlSelfRef.__PVT__static_q)
                                                   ? 0U
                                                   : 
                                                  ((0x00000040U 
                                                    >= vlSelfRef.__PVT__static_q)
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.__PVT__static_q 
                                                    - (IData)(0x00000040U))));
                            if (((0U != vlSelfRef.__PVT__static_q) 
                                 & (0x00000040U >= vlSelfRef.__PVT__static_q))) {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelfRef.__PVT__ix_q)));
                                vlSelfRef.__PVT__adv_ret_mov = 1U;
                            } else {
                                __Vdly__adv_ix = (0x0000000fU 
                                                  & (IData)(vlSelfRef.__PVT__adv_ix));
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                        }
                    } else if ((2U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                        if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                            __VdlyVal__rates_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                                  >> 4U));
                            __VdlySet__rates_q__v4 = 1U;
                            __VdlyVal__levels_q__v4 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                  >> 8U));
                            vlSelfRef.__PVT__outlevel_q 
                                = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                                  >> 6U));
                            vlSelfRef.__PVT__rs_q = 
                                (0x0000003fU & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U]);
                            if (vlSelfRef.__PVT__down_q) {
                                __Vdly__adv_ix = 2U;
                                vlSelfRef.__PVT__adv_ret_mov = 0U;
                            }
                            __VdlyVal__rates_q__v5 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                                   << 3U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                     >> 0x0000001dU)));
                            __VdlySet__rates_q__v5 = 1U;
                            __VdlyVal__levels_q__v5 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                  >> 1U));
                            __VdlyVal__rates_q__v6 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                  >> 0x00000016U));
                            __VdlyVal__levels_q__v6 
                                = (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                   << 6U) 
                                                  | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                                     >> 0x0000001aU)));
                            __VdlyVal__rates_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                  >> 0x0000000fU));
                            __VdlyVal__levels_q__v7 
                                = (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                                  >> 0x00000013U));
                        } else if (((IData)(vlSelfRef.__PVT__down_q) 
                                    != (1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U]))) {
                            vlSelfRef.__PVT__kd_fire = 1U;
                            if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U])) {
                                __Vdly__down_q = 1U;
                                __Vdly__adv_ix = 0U;
                            } else {
                                __Vdly__down_q = 0U;
                                __Vdly__adv_ix = 3U;
                            }
                            vlSelfRef.__PVT__adv_ret_mov = 0U;
                        } else {
                            vlSelfRef.__PVT__kd_fire = 0U;
                        }
                    } else if ((1U & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U])) {
                        __Vdly__level_q = 0U;
                        __Vdly__down_q = 1U;
                        __Vdly__adv_ix = 0U;
                        vlSelfRef.__PVT__adv_ret_mov = 0U;
                    } else {
                        __VdlyVal__rates_q__v8 = (0x0000007fU 
                                                  & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                                     >> 4U));
                        __VdlySet__rates_q__v8 = 1U;
                        __VdlyVal__levels_q__v8 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                      >> 8U));
                        vlSelfRef.__PVT__outlevel_q 
                            = (0x00001fffU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                              >> 6U));
                        vlSelfRef.__PVT__rs_q = (0x0000003fU 
                                                 & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U]);
                        __VdlyVal__rates_q__v9 = (0x0000007fU 
                                                  & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][2U] 
                                                      << 3U) 
                                                     | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                        >> 0x0000001dU)));
                        __VdlySet__rates_q__v9 = 1U;
                        __VdlyVal__levels_q__v9 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                      >> 1U));
                        __VdlyVal__rates_q__v10 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                      >> 0x00000016U));
                        __VdlyVal__levels_q__v10 = 
                            (0x0000007fU & ((vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                             << 6U) 
                                            | (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                               >> 0x0000001aU)));
                        __VdlyVal__rates_q__v11 = (0x0000007fU 
                                                   & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][1U] 
                                                      >> 0x0000000fU));
                        __VdlyVal__levels_q__v11 = 
                            (0x0000007fU & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_data[77U][0U] 
                                            >> 0x00000013U));
                    }
                }
            }
        }
        vlSelfRef.__PVT__state_q = vlSelfRef.__PVT__nstate;
    }
    vlSelfRef.__PVT__st_after_r = __Vdly__st_after_r;
    vlSelfRef.__PVT__adv_ix = __Vdly__adv_ix;
    if (__VdlySet__rates_q__v0) {
        vlSelfRef.__PVT__levels_q[0U] = 0U;
        vlSelfRef.__PVT__rates_q[0U] = 0U;
    }
    if (__VdlySet__rates_q__v1) {
        vlSelfRef.__PVT__levels_q[1U] = 0U;
        vlSelfRef.__PVT__levels_q[2U] = 0U;
        vlSelfRef.__PVT__levels_q[3U] = 0U;
        vlSelfRef.__PVT__rates_q[1U] = 0U;
        vlSelfRef.__PVT__rates_q[2U] = 0U;
        vlSelfRef.__PVT__rates_q[3U] = 0U;
    }
    if (__VdlySet__rates_q__v4) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v4;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v4;
    }
    if (__VdlySet__rates_q__v5) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v5;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v6;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v7;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v5;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v6;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v7;
    }
    if (__VdlySet__rates_q__v8) {
        vlSelfRef.__PVT__levels_q[0U] = __VdlyVal__levels_q__v8;
        vlSelfRef.__PVT__rates_q[0U] = __VdlyVal__rates_q__v8;
    }
    if (__VdlySet__rates_q__v9) {
        vlSelfRef.__PVT__levels_q[1U] = __VdlyVal__levels_q__v9;
        vlSelfRef.__PVT__levels_q[2U] = __VdlyVal__levels_q__v10;
        vlSelfRef.__PVT__levels_q[3U] = __VdlyVal__levels_q__v11;
        vlSelfRef.__PVT__rates_q[1U] = __VdlyVal__rates_q__v9;
        vlSelfRef.__PVT__rates_q[2U] = __VdlyVal__rates_q__v10;
        vlSelfRef.__PVT__rates_q[3U] = __VdlyVal__rates_q__v11;
    }
    vlSelfRef.__PVT__rising_q = __Vdly__rising_q;
    vlSelfRef.__PVT__static_q = __Vdly__static_q;
    vlSelfRef.__PVT__ix_q = __Vdly__ix_q;
    vlSelfRef.__PVT__down_q = __Vdly__down_q;
    vlSelfRef.__PVT__target_q = __Vdly__target_q;
    vlSelfRef.__PVT__level_q = __Vdly__level_q;
    if ((4U > (7U & (IData)(vlSelfRef.__PVT__adv_ix)))) {
        vlSelfRef.__PVT__adv_lv = vlSelfRef.__PVT__levels_q
            [(3U & (IData)(vlSelfRef.__PVT__adv_ix))];
        __PVT__adv_rate = vlSelfRef.__PVT__rates_q[
            (3U & (IData)(vlSelfRef.__PVT__adv_ix))];
    } else {
        vlSelfRef.__PVT__adv_lv = 0U;
        __PVT__adv_rate = 0U;
    }
    vlSelfRef.__PVT__mov_step = (((3U > (IData)(vlSelfRef.__PVT__ix_q)) 
                                  | ((~ (IData)(vlSelfRef.__PVT__down_q)) 
                                     & (4U > (IData)(vlSelfRef.__PVT__ix_q)))) 
                                 & (0U == vlSelfRef.__PVT__static_q));
    vlSelfRef.__PVT__fall_diff = (0x00000001ffffffffULL 
                                  & ((QData)((IData)(vlSelfRef.__PVT__level_q)) 
                                     - (QData)((IData)(vlSelfRef.__PVT__inc_q))));
    __PVT__lv_jumped = ((0x06b40000U > vlSelfRef.__PVT__level_q)
                         ? 0x06b40000U : vlSelfRef.__PVT__level_q);
    vlSelfRef.__PVT__act16 = (0x0000ffffU & (((IData)(vlSelfRef.__PVT__outlevel_q) 
                                              + (0x00000fc0U 
                                                 & (((0x14U 
                                                      <= (IData)(vlSelfRef.__PVT__adv_lv))
                                                      ? 
                                                     (0x0000003fU 
                                                      & (((IData)(0x1cU) 
                                                          + (IData)(vlSelfRef.__PVT__adv_lv)) 
                                                         >> 1U))
                                                      : 
                                                     (((0U 
                                                        == 
                                                        (0x0000001fU 
                                                         & ((IData)(1U) 
                                                            + 
                                                            ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                             << 5U))))
                                                        ? 0U
                                                        : 
                                                       (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                        [
                                                        (((IData)(6U) 
                                                          + 
                                                          ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                           << 5U)) 
                                                         >> 5U)] 
                                                        << 
                                                        ((IData)(0x00000020U) 
                                                         - 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                      | (Vtb_dx7_core__ConstPool__CONST_h46231d0d_0
                                                         [
                                                         (((IData)(1U) 
                                                           + 
                                                           ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                            << 5U)) 
                                                          >> 5U)] 
                                                         >> 
                                                         (0x0000001fU 
                                                          & ((IData)(1U) 
                                                             + 
                                                             ((IData)(vlSelfRef.__PVT__adv_lv) 
                                                              << 5U)))))) 
                                                    << 6U))) 
                                             - (IData)(0x10a0U)));
    vlSelfRef.__PVT__qsum = (0x000000ffU & ((IData)(vlSelfRef.__PVT__rs_q) 
                                            + (0x0000003fU 
                                               & (((IData)(0x0029U) 
                                                   * (IData)(__PVT__adv_rate)) 
                                                  >> 6U))));
    __PVT__srs_w = (0x000000ffU & ((IData)(__PVT__adv_rate) 
                                   + (IData)(vlSelfRef.__PVT__rs_q)));
    vlSelfRef.__PVT__fall_clamp = (IData)(((vlSelfRef.__PVT__fall_diff 
                                            >> 0x00000020U) 
                                           | ((QData)((IData)(vlSelfRef.__PVT__fall_diff)) 
                                              <= (QData)((IData)(vlSelfRef.__PVT__target_q)))));
    vlSelfRef.__PVT__rise_sum = (0x000003ffffffffffULL 
                                 & (VL_MULS_QQQ(42, 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,10, 
                                                                 (0x000003ffU 
                                                                  & (IData)(
                                                                            (0x00000003ffffffffULL 
                                                                             & VL_SHIFTRS_QQI(34,34,32, 
                                                                                (0x00000003ffffffffULL 
                                                                                & (0x0000000011000000ULL 
                                                                                - (QData)((IData)(__PVT__lv_jumped)))), 0x00000018U)))))), 
                                                (0x000003ffffffffffULL 
                                                 & VL_EXTENDS_QI(42,32, vlSelfRef.__PVT__inc_q))) 
                                    + (QData)((IData)(__PVT__lv_jumped))));
    vlSelfRef.__PVT__sr_w = ((0x63U < (IData)(__PVT__srs_w))
                              ? 0x00000063U : (0x0000007fU 
                                               & (IData)(__PVT__srs_w)));
    vlSelfRef.__PVT__rise_clamp = VL_GTES_IQQ(46, (0x00003fffffffffffULL 
                                                   & VL_EXTENDS_QQ(46,42, vlSelfRef.__PVT__rise_sum)), (QData)((IData)(vlSelfRef.__PVT__target_q)));
    vlSelfRef.__PVT__raw_base = (0x001fffffU & ((0x4dU 
                                                 > (IData)(vlSelfRef.__PVT__sr_w))
                                                 ? Vtb_dx7_core__ConstPool__CONST_h949cbf09_0
                                                [(0x07ffffffU 
                                                  & (IData)(vlSelfRef.__PVT__sr_w))]
                                                 : 
                                                ((IData)(0x00000014U) 
                                                 * 
                                                 ((IData)(0x00000063U) 
                                                  - (IData)(vlSelfRef.__PVT__sr_w)))));
    vlSelfRef.__PVT__mov_clamp = (((IData)(vlSelfRef.__PVT__rising_q)
                                    ? (IData)(vlSelfRef.__PVT__rise_clamp)
                                    : (IData)(vlSelfRef.__PVT__fall_clamp)) 
                                  & (IData)(vlSelfRef.__PVT__mov_step));
}
