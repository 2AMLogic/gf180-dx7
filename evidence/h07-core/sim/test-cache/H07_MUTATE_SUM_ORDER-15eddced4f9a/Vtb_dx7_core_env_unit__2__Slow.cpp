// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

VL_ATTR_COLD void Vtb_dx7_core_env_unit___ctor_var_reset(Vtb_dx7_core_env_unit* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vtb_dx7_core_env_unit___ctor_var_reset\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->cmd_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5847278361250794325ull);
    vlSelf->cmd = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4759173320218931530ull);
    VL_SCOPED_RAND_RESET_W(133, vlSelf->cmd_data, __VscopeHash, 9522006951793221091ull);
    vlSelf->busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6386567572483775230ull);
    vlSelf->done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10296494685231209730ull);
    vlSelf->slot_wr_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17800879074072650338ull);
    VL_SCOPED_RAND_RESET_W(133, vlSelf->slot_wr_data, __VscopeHash, 11445311823075955708ull);
    VL_SCOPED_RAND_RESET_W(133, vlSelf->slot_rd_data, __VscopeHash, 3512901641799682541ull);
    VL_SCOPED_RAND_RESET_W(75, vlSelf->__PVT__param_rd, __VscopeHash, 8757219087169577129ull);
    vlSelf->__PVT__state_obs = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13093738810982917966ull);
    vlSelf->__PVT__level_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3571006781652411078ull);
    vlSelf->__PVT__target_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17421037048355496041ull);
    vlSelf->__PVT__inc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 631965769484134767ull);
    vlSelf->__PVT__static_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14347639376898029052ull);
    vlSelf->__PVT__ix_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4229320969043445574ull);
    vlSelf->__PVT__rising_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2883365890438271330ull);
    vlSelf->__PVT__down_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 974971754275984164ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->__PVT__rates_q[__Vi0] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 16788227444102595623ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->__PVT__levels_q[__Vi0] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 1419703664617499906ull);
    }
    vlSelf->__PVT__outlevel_q = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 12471924412081751973ull);
    vlSelf->__PVT__rs_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 10840542217231084921ull);
    vlSelf->__PVT__state_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14578357007859236226ull);
    vlSelf->__PVT__adv_ix = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14644317755297790410ull);
    vlSelf->__PVT__adv_ret_mov = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1042940870786368439ull);
    vlSelf->__PVT__tgt_new = VL_SCOPED_RAND_RESET_I(28, __VscopeHash, 14888714049923402444ull);
    vlSelf->__PVT__newlv_r = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 717417509857154937ull);
    vlSelf->__PVT__rising_new = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17470253594508284696ull);
    vlSelf->__PVT__qr_r = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 6811016690049542339ull);
    vlSelf->__PVT__static_cond_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6622221548434069808ull);
    vlSelf->__PVT__raw_static_r = VL_SCOPED_RAND_RESET_I(21, __VscopeHash, 15978050677163150235ull);
    vlSelf->__PVT__stat_new_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13160805953659507887ull);
    vlSelf->__PVT__inc_pre_r = VL_SCOPED_RAND_RESET_I(26, __VscopeHash, 16387362852036982961ull);
    vlSelf->__PVT__st_after_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14959091198872999543ull);
    vlSelf->__PVT__st_adv_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8975589410915416378ull);
    vlSelf->__PVT__kd_fire = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6693715046910583429ull);
    vlSelf->__PVT__adv_lv = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 4074463213145866858ull);
    vlSelf->__PVT__act16 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14395798112939822718ull);
    vlSelf->__PVT__qsum = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16335810068866503488ull);
    vlSelf->__PVT__sr_w = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7224465195296817973ull);
    vlSelf->__PVT__raw_base = VL_SCOPED_RAND_RESET_I(21, __VscopeHash, 11616717528581500159ull);
    vlSelf->__PVT__rise_sum = VL_SCOPED_RAND_RESET_Q(42, __VscopeHash, 11407579935804820765ull);
    vlSelf->__PVT__rise_clamp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13282563280697297321ull);
    vlSelf->__PVT__fall_diff = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 15995403657042926343ull);
    vlSelf->__PVT__fall_clamp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3093618022160968119ull);
    vlSelf->__PVT__mov_step = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1283178403879981153ull);
    vlSelf->__PVT__mov_clamp = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17255496833000151010ull);
    vlSelf->__PVT__nstate = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14473519320944010295ull);
}
