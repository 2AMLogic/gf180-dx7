// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h46231d0d_0;
extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h949cbf09_0;

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U]))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[0U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 1U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[1U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 2U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[2U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 3U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[3U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 4U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[4U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 5U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[5U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 6U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[6U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 7U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[7U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 8U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[8U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 9U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[9U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[10U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[11U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[12U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[13U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[14U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000000fU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[15U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000010U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[16U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000011U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[17U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000012U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[18U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000013U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[19U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000014U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[20U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000015U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[21U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000016U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[22U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000017U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[23U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000018U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[24U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x00000019U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[25U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000001aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[26U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000001bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[27U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000001cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[28U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000001dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[29U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                           >> 0x0000001eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[30U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                   (((~ (IData)(vlSelfRef.__PVT__state_q)) 
                                                     & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] 
                                                        >> 0x0000001fU))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[31U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U]))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[32U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 1U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[33U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 2U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[34U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 3U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[35U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 4U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[36U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 5U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[37U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 6U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[38U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 7U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[39U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 8U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[40U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 9U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[41U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[42U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[43U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[44U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[45U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[46U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __PVT__adv_rate;
    __PVT__adv_rate = 0;
    CData/*7:0*/ __PVT__srs_w;
    __PVT__srs_w = 0;
    IData/*31:0*/ __PVT__lv_jumped;
    __PVT__lv_jumped = 0;
    // Body
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                           >> 0x0000000fU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[47U]) 
                                                      << 2U))
                                                     : 0U)))));
}
