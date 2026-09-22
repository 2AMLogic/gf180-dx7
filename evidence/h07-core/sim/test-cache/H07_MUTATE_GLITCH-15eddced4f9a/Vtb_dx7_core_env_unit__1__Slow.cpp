// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h46231d0d_0;
extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h949cbf09_0;

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000010U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[48U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000011U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[49U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000012U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[50U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000013U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[51U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000014U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[52U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000015U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[53U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000016U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[54U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000017U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[55U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000018U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[56U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x00000019U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[57U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x0000001aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[58U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x0000001bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[59U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x0000001cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[60U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x0000001dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[61U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env__0\n"); );
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
                                                           >> 0x0000001eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[62U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env__0\n"); );
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
                                                     & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] 
                                                        >> 0x0000001fU))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[63U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env__0\n"); );
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
                                                        & vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U]))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[64U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 1U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[65U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 2U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[66U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 3U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[67U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 4U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[68U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 5U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[69U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 6U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[70U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 7U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[71U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 8U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[72U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 9U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[73U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[74U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[75U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[76U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[77U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[78U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000000fU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[79U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000010U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[80U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000011U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[81U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000012U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[82U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000013U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[83U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000014U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[84U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000015U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[85U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000016U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[86U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000017U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[87U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000018U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[88U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x00000019U)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[89U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000001aU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[90U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000001bU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[91U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000001cU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[92U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000001dU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[93U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env__0\n"); );
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
                                                        & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                           >> 0x0000001eU)))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[94U]) 
                                                      << 2U))
                                                     : 0U)))));
}

VL_ATTR_COLD void Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env__0(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___stl_sequent__TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env__0\n"); );
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
                                                     & (vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] 
                                                        >> 0x0000001fU))
                                                     ? 
                                                    (0x02865431U 
                                                     >> 
                                                     ((IData)(vlSymsp->TOP.tb_dx7_core__DOT__dut__DOT__env_cmd[95U]) 
                                                      << 2U))
                                                     : 0U)))));
}
