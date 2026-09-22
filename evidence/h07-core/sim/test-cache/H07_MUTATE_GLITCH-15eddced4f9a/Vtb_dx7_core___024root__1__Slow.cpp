// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

bool Vtb_dx7_core___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_dx7_core___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_dx7_core___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vtb_dx7_core___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_dx7_core___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_dx7_core___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge tb_dx7_core.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge tb_dx7_core.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_dx7_core___024root___ctor_var_reset(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___ctor_var_reset\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_dx7_core__DOT__bit_i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5918942573894453202ull);
    vlSelf->tb_dx7_core__DOT__tx_frame = VL_SCOPED_RAND_RESET_Q(48, __VscopeHash, 17182993031629999618ull);
    vlSelf->tb_dx7_core__DOT__spi_xfer__Vstatic__f = VL_SCOPED_RAND_RESET_Q(48, __VscopeHash, 10497071727591045273ull);
    vlSelf->tb_dx7_core__DOT__afd = 0;
    VL_SCOPED_RAND_RESET_W(1024, vlSelf->tb_dx7_core__DOT__vfile, __VscopeHash, 11271554286329696738ull);
    VL_SCOPED_RAND_RESET_W(1024, vlSelf->tb_dx7_core__DOT__afile, __VscopeHash, 8234451770304673934ull);
    VL_SCOPED_RAND_RESET_W(1024, vlSelf->tb_dx7_core__DOT__mfile, __VscopeHash, 8698990563851352503ull);
    VL_SCOPED_RAND_RESET_W(256, vlSelf->tb_dx7_core__DOT__line, __VscopeHash, 3640020233453423261ull);
    vlSelf->tb_dx7_core__DOT__rc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9282973599611156690ull);
    vlSelf->tb_dx7_core__DOT__p0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3642476719439787479ull);
    vlSelf->tb_dx7_core__DOT__pd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13151080312190965860ull);
    vlSelf->tb_dx7_core__DOT__vdec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7226632397530304893ull);
    vlSelf->tb_dx7_core__DOT__addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3239899665428037558ull);
    vlSelf->tb_dx7_core__DOT__data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6454150589490847590ull);
    vlSelf->tb_dx7_core__DOT__block = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18055638158277872661ull);
    vlSelf->tb_dx7_core__DOT__tok = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3398315504045684765ull);
    vlSelf->tb_dx7_core__DOT__samples = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4047212978951022279ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__commit_list[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8593834782165558865ull);
    }
    vlSelf->tb_dx7_core__DOT__lr_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9352433637071203217ull);
    vlSelf->tb_dx7_core__DOT__wait_frames__Vstatic__n = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5079660158202574935ull);
    vlSelf->tb_dx7_core__DOT__wait_until__Vstatic__fr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7983371764516359483ull);
    vlSelf->tb_dx7_core__DOT__wait_until_samples__Vstatic__target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11840323916219007873ull);
    vlSelf->tb_dx7_core__DOT__c0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6650887247110901887ull);
    vlSelf->tb_dx7_core__DOT__c1 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10031731610163942008ull);
    vlSelf->tb_dx7_core__DOT__c2 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5469731943651156760ull);
    vlSelf->tb_dx7_core__DOT__c3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15005592959159029076ull);
    vlSelf->tb_dx7_core__DOT__tap_q = VL_SCOPED_RAND_RESET_I(23, __VscopeHash, 13868839182943764789ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__rst_ff1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14836578598039236605ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__rst_ff2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10336746709267726500ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sample_clk = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 15777993215113409841ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sample_idx = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 10194792121695111681ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__frame_ctr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17573717403397461971ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__frame_boundary = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5762445412390663873ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sck_s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17241857976238617280ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sck_s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 146724872508402563ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3949253935969133992ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mosi_s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15611816544366059046ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mosi_s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12968017172128364203ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cs_s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1521172890419079706ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cs_s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11558954316350160072ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cs_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11756833462773828676ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__st_overrun = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15639101802030234710ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__st_overflow = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7529538734170858479ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__st_fresh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2443135165947570828ull);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__skid[__Vi0] = VL_SCOPED_RAND_RESET_Q(49, __VscopeHash, 10492308015811245965ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__skid_wp = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14599177176262641413ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__skid_rp = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17276268188152764757ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__skid_cnt = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12923091515324381261ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shift_in = VL_SCOPED_RAND_RESET_Q(48, __VscopeHash, 17201229039628923896ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__bit_cnt = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 15547402757106458781ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__frame_reg = VL_SCOPED_RAND_RESET_Q(48, __VscopeHash, 14043013676432449782ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__frame_reg_v = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8579923147086807189ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__frame_captured = VL_SCOPED_RAND_RESET_Q(48, __VscopeHash, 7442651307995884062ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__wr_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11465535437026915632ull);
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_opitch[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14180404789911043033ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_fterm[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12968183375815391269ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_erates[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2595196253543252561ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_elevels[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9319732408714668864ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_scaleA[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6936129830094261700ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__shd_scaleB[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2538855946470383602ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_globA = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5675457786562446978ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_globB = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11484012350191591717ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_globC = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14791048682976396857ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_pegr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13173215949604092882ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_pegl = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2487484104896854240ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__shd_commit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3437831283277892304ull);
    for (int __Vi0 = 0; __Vi0 < 168; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__evq[__Vi0] = VL_SCOPED_RAND_RESET_Q(37, __VscopeHash, 8705346528379665769ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__evq_wp = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12906774400543369025ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__evq_rp = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15877785693437737566ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__evq_cnt = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 16923711058813506369ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__evq_out = VL_SCOPED_RAND_RESET_Q(37, __VscopeHash, 1270859132672832150ull);
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_opitch[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5777952752402992057ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_fterm[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8255673365358994ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_erates[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10923787849566814987ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_elevels[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15694935995817420085ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_scaleA[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12933634637167331081ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_scaleB[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14770467170986275451ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_globA = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1290797901677180198ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_globB = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4210124250492462238ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_globC = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3916273778946240906ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_pegr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 950661062950375669ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cmt_pegl = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13283992813387066312ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__bend_pb = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4974719539159286376ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__master_tune = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12221274676064375915ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__ctrl_pm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9490512828341114897ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__ctrl_am = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2364927057227703935ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__eg_mod = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16937679652737600240ull);
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__c_mode[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8572787292375050695ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__c_coarse[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8843292667927547184ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__c_fine[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4323042357764872523ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__c_detune[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7394539825736125252ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_live = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3336425775516387079ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_keydown = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14754286383184963816ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_sustained = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8286131879631396808ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_reclaimed = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 205303213034659397ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_used = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5569833992138294348ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_midi[__Vi0] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7532547380938736065ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_vel[__Vi0] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6727747630015320484ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_seq[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13364951016721827025ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_sil[__Vi0] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 17093639742818670540ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__n_silnow = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14500606774500662308ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__cur_note = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 534437805465964106ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__seq_ctr = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 3749965766398504565ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__sustain = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17310304635388293179ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__n_bp[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18418692715701134240ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_opmode[__Vi0] = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 11187106611411420530ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_alg[__Vi0] = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15897302853781853124ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_fbs[__Vi0] = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 17019724886574557412ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pmdep[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4305368196101356531ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pmsens[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 906508511370923536ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_amdep[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6195964452469688832ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__n_ams[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 10287065944633722446ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegr[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 12395938691039984312ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegl[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 2549048730625626890ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_peglvl[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2672553523245143687ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegtgt[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2853567120871084857ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_peginc[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 132533221117515117ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegix[__Vi0] = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12691755122642101655ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegris[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12084939557237063559ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_pegdwn[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12961238984555190245ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__stg_dd[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1504893694781136255ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__op_ph[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6886670780691368423ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__op_go[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10183959848823109572ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__op_fq[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 171921097326308481ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__op_li[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6683191032026158604ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_fb0[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3023917555101560395ull);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__n_fb1[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5376149208281805102ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__refresh_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16511792436911478323ull);
    VL_SCOPED_RAND_RESET_W(96, vlSelf->tb_dx7_core__DOT__dut__DOT__env_busy, __VscopeHash, 17933411936703966948ull);
    VL_SCOPED_RAND_RESET_W(96, vlSelf->tb_dx7_core__DOT__dut__DOT__env_cmd_en, __VscopeHash, 76070209932220214ull);
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__env_cmd[__Vi0] = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13880920607447555904ull);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(133, vlSelf->tb_dx7_core__DOT__dut__DOT__env_cmd_data[__Vi0], __VscopeHash, 17534301331933205109ull);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        VL_SCOPED_RAND_RESET_W(133, vlSelf->tb_dx7_core__DOT__dut__DOT__env_rd[__Vi0], __VscopeHash, 11878614698659968929ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_phase = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15809065104306801170ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_delta = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10431711782977081945ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_dstate = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4979489833217958743ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_dinc1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5950803474061064619ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_dinc2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15857463323790640387ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_rand = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5619004544861222296ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_val_q = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 978590924087790157ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_del_q = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 15652443849191120701ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_sr1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4768092442957769950ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_a0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17063570838367789441ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_a1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12920097925350490312ull);
    for (int __Vi0 = 0; __Vi0 < 2048; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__lfo_sin_rom[__Vi0] = VL_SCOPED_RAND_RESET_I(26, __VscopeHash, 123347855905192578ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_ph_n = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12709465899362814620ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_rand_nx = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 89141926658658204ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__l_dsum = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 4153782521225887323ull);
    for (int __Vi0 = 0; __Vi0 < 1025; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__freqlut_rom[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5491370064239060445ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12160631089440568219ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_amd = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 14188259826141923581ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_ams = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 2260155128512229481ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_st = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16741650050629441320ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_y = VL_SCOPED_RAND_RESET_I(26, __VscopeHash, 5677632196415482108ull);
    VL_SCOPED_RAND_RESET_W(80, vlSelf->tb_dx7_core__DOT__dut__DOT__exp_t1, __VscopeHash, 193392074783855391ull);
    VL_SCOPED_RAND_RESET_W(66, vlSelf->tb_dx7_core__DOT__dut__DOT__exp_arg, __VscopeHash, 9657505929998226898ull);
    VL_SCOPED_RAND_RESET_W(67, vlSelf->tb_dx7_core__DOT__dut__DOT__exp_zq, __VscopeHash, 16016986328638080782ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_k = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 17958040836667866260ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_T = VL_SCOPED_RAND_RESET_Q(62, __VscopeHash, 7436768523445136396ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_h = VL_SCOPED_RAND_RESET_Q(57, __VscopeHash, 4185972208946395688ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_res = VL_SCOPED_RAND_RESET_Q(63, __VscopeHash, 11592940084039697616ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_pt_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9450216975189853707ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_x = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 12638711525740830229ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__exp_hn = VL_SCOPED_RAND_RESET_Q(57, __VscopeHash, 13820033907282151754ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__fc_state = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 15346761729509242151ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__nn = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14030078619573910645ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__opk = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5946606023875721640ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__kind = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17897102406892438344ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__stg_note = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6619848737189861187ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__stg_vel = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7972221280547578402ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__stg_ctrl = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15504876215182505031ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__stg_val = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17437206048164198138ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__tgt = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6910718765053653364ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__scan = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14157896335876038934ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__pm_nobase_w = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7294461801960490611ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__pbase_w = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12947933996549365623ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__amd_w = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11954698121103645395ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_peg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2896480032722570758ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_pmdep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2905760733807306295ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_pmsens = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 344509047722397302ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_amdep = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 858613911766470145ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__fq_sel = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6728453021825176907ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__fq_fsh_s = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 7296501385117339835ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_cfg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12850645428235897355ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_fb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4978939628319130752ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_op = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16368747528989251282ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4315715678682103690ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_cfg_algorithm = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13035433310804974512ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_cfg_feedback = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5565211259971515408ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_cfg_fb0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7157069903555712736ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_cfg_fb1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1401215721016427894ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_phase = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14773336486788138111ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_freq = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8657229504930958925ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_op_idx = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3039398034744321545ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_level = VL_SCOPED_RAND_RESET_I(28, __VscopeHash, 10509333758249230838ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_ld_gain_out = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14521337715133292445ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__r_rd_addr = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 5353891271676982939ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_level = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8635258067189210640ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_li = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15677828546269046319ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_fa = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 10045717002541110695ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_fl = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 2222870570502342292ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_fsh = VL_SCOPED_RAND_RESET_I(9, __VscopeHash, 1333509147114081507ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mix_j = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 16338303644582529847ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__rb_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6146242262377502391ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_note = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 4914328127519385985ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_r0 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 26043520554116145ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_r1 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7639413582535700853ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_r2 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 14337219929326292944ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_r3 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 9875494008272310541ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_outlevel = VL_SCOPED_RAND_RESET_I(13, __VscopeHash, 8476606639540060022ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16448547843106559779ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5723800105321954353ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9612092755709450090ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13581595611752882834ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9267793585565430157ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13476827956847692213ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2941959740104429260ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17982491721570355969ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9458902436153165928ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16826655339318861210ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6178563413814084021ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f11 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11533857899158514661ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f12 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12489898757380286580ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9223618269805302138ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f14 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5258050352124887905ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__f15 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18050586020295750690ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__free_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6017098603539596232ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__note_silent = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9825093222322809175ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__in_rel = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16364533534687011586ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__age[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6839147319169695139ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__match_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10706071391799185914ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__match_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12027318083975706728ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__steal_idx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9038601890150064139ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__steal_found = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7023206186935688352ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_erates = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8604532065645189059ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_elevels = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5379264850064203033ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_scaleA = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17551119039956935221ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_scaleB = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8791288995934271199ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_rs_eff = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 4416801689356949073ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_fine = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 377236389657908400ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_detune = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2940919530868958093ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_bpf0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10381248293393125319ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__dv_bpr1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11751259995338788846ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__t_peg_act = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13079947734238985159ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__t_peg_up = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16691827876334926554ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__t_peg_dn = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1825041323738498642ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__t_peg_nxt = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8133974352622396390ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__t_peg_adv = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4007159720139809570ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_sl0 = VL_SCOPED_RAND_RESET_Q(40, __VscopeHash, 6668074255661791972ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_pmax = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16977197584655526613ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_am1b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 573503943561702039ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_am2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5649296524429539382ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_amax = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14242873217244027853ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__w_ambase = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17269794930336362275ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__ar_eval_y = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7529108389432192298ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__ar_rd_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17824922100201806366ull);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 64; ++__Vi1) {
            vlSelf->tb_dx7_core__DOT__dut__DOT__mixbuf[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 3902792203974507154ull);
        }
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__in_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11324143212374057601ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__out_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13617143168385775349ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__tgt_busy = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 5605653493624929102ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__scan_busy = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 11797455613063314689ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__bclk_div = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17415283839990640534ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__bclk_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10547727837498695014ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__lr_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1253775969895856958ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__half_cnt = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13744118122163964397ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__falls = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 7498247314898433179ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__i2s_sh = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 6724854007467608881ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__i2s_smp = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 3348831618626547409ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mix_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11563749250876362396ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__obs_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1675397125974681041ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mixbuf_obs0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8066616183125209932ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__mixbuf_obs1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17152541186995936545ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9273369238322450651ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1465893098057179135ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13819377673443877613ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15848343732216966753ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10478304452369916502ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__0__KET____DOT__s5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13403587346838794780ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10500764945980770751ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2485245816693871209ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3239692229973073367ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13670917174395554677ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13707508282399647907ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__1__KET____DOT__s5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11563229164011801711ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 471545147878579552ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11743214822908696014ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6419965947946347629ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 715122421583074775ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10819843624571402338ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__2__KET____DOT__s5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 961579467208421701ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8067135211404513029ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11752713771474075306ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18168976642471909111ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10489041750260404644ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2640680277392332672ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__3__KET____DOT__s5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 664193277542314236ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 243352613873440952ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5366154857536089105ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3745429316883486016ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1491314896565302392ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4653339878899663845ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__silgen__BRA__4__KET____DOT__s5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3405377596207831707ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15116201044862649417ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 6821778577761063464ull);
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2933361136406830928ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15018853162683243078ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[__Vi0] = VL_SCOPED_RAND_RESET_I(28, __VscopeHash, 15059842166639288822ull);
    }
    for (int __Vi0 = 0; __Vi0 < 6; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5356639229921062547ull);
    }
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11241865064599767916ull);
    }
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4065738040538268649ull);
    }
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17617683507216141594ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 802025567175785698ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17170080698111487048ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6412016026314066899ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5282122978158910296ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6928679234317211603ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8341406996917049693ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8952133207305510381ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17410774627975948089ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5327792102896846953ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7600035123828856799ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 16330581961351542173ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 8385438578713696954ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6500786242590775072ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9782415386666232230ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg1_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17193779127313773968ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg2_w = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15518701348641256305ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5809064755654500801ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7310336872073947275ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2093286688287575650ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7632047879378123651ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 11612368440532688265ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7467360669882889544ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17420522545082424885ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15405359266896768168ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15245826540332468974ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15072755414636768912ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8042353033082991189ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17929827967876156363ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5886498290759192117ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 17716155579461849323ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 9561255486365545313ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__obs_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4090001922109294787ull);
    for (int __Vi0 = 0; __Vi0 < 192; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_rom[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17197472224592532675ull);
    }
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14090426350809994632ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain1 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 290969389266270029ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain2 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 13502177620073904088ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gate_pass = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8923921401094722733ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_live = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5700793035301936857ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_dispatch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2633451744619801644ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15944351145971973843ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17321974824795773412ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 2677909610704127961ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2 = VL_SCOPED_RAND_RESET_I(15, __VscopeHash, 3296721418368257127ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1271594747077856423ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg2 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8039273986109522059ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__eval_mod_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7345454550747408764ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 5386711397975663120ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17568792063896438945ull);
    vlSelf->tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv2 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10311821369982446702ull);
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinlog_rom[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5935609567809449005ull);
    }
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinexp_rom[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18202066288324299220ull);
    }
    VL_ZERO_RESET_W(133, vlSelf->__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__s);
    vlSelf->__Vfunc_tb_dx7_core__DOT__dut__DOT__sil_op__188__go = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_0 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_1 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_2 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_23 = 0;
    vlSelf->__VdfgRegularize_hebeb780c_0_24 = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_st = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_y = 0;
    VL_ZERO_RESET_W(80, vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1);
    VL_ZERO_RESET_W(66, vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg);
    VL_ZERO_RESET_W(67, vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq);
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_k = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_T = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_h = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_res = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__tgt = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__kind = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__n_live = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__n_used = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__sustain = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__in_sel = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__bend_pb = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__master_tune = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globA = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globB = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globC = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegr = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegl = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__stg_vel = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__pm_nobase_w = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__pbase_w = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__amd_w = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_peg = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_level = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_li = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_fa = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_fl = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__w_fsh = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__fq_sel = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr = 0;
    vlSelf->__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v0 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v0 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v0 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v0 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v1 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v2 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v3 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v4 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v5 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v7 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v8 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v9 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v10 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v11 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_alg__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v97 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v98 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v99 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v100 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v101 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v17 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v17 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v18 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v19 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v20 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v21 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v21 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v22 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v23 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v24 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v25 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v25 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v26 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v27 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v28 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v29 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v29 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v30 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v31 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v32 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v34 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v35 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v36 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v38 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v39 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v40 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v41 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v41 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v42 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v43 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v44 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v45 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v45 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v46 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v47 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v48 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v49 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v49 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v50 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v51 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v52 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v53 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v53 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v54 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v55 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v56 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v57 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v57 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v58 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v59 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v60 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v61 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v61 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v62 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v63 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v64 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v65 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v65 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v66 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v67 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v68 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v69 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v69 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v70 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v71 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v72 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v73 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v73 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v74 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v75 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v76 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v77 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v77 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v78 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v79 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v12 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v13 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1153 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1154 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1155 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1156 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1157 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1158 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1159 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1160 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1161 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1162 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1163 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1164 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1165 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1166 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1167 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1168 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1169 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1170 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1171 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1172 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1173 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1174 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1175 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1176 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1177 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1178 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1179 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1180 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1181 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1182 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1183 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1184 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1185 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1186 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1187 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1188 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1189 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1190 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1191 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1192 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1193 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1194 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1195 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1196 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1197 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1198 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1199 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1200 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1201 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1202 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1203 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1204 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1205 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1206 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1207 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1208 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1209 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1210 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1211 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1212 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1213 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1214 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1215 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1216 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v14 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v15 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v17 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v18 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v19 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v20 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v21 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v22 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v23 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v24 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v25 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v26 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v27 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v28 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v29 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v30 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v31 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_seq__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_seq__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_seq__v16 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_midi__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_midi__v16 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v16 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_sil__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v96 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v193 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v193 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v194 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v195 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v196 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v197 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v198 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v199 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v199 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v200 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v201 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v202 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v203 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v204 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 = 0;
    vlSelf->__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v36 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v32 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v33 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v97 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v206 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v98 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v99 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v100 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v101 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v102 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v103 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v104 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v105 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v106 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v107 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v108 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v109 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v110 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v111 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v112 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v6 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v6 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v6 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v7 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v7 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v7 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v8 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v8 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v8 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v8 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v9 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v9 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v9 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v9 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v10 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v10 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v10 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v10 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v11 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v11 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v11 = 0;
    vlSelf->__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v11 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1217 = 0;
    vlSelf->__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1217 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1218 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1219 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1220 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1221 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1222 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1223 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1224 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1225 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1226 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1227 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1228 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1229 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1230 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1231 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1232 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1233 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1234 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1235 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1236 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1237 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1238 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1239 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1240 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1241 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1242 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1243 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1244 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1245 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1246 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1247 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1248 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1249 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1250 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1251 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1252 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1253 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1254 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1255 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1256 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1257 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1258 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1259 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1260 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1261 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1262 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1263 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1264 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1265 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1266 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1267 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1268 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1269 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1270 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1271 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1272 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1273 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1274 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1275 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1276 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1277 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1278 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1279 = 0;
    vlSelf->__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1280 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggeredAcc[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_dx7_core__DOT__clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
    vlSelf->__Vi = 0;
}
