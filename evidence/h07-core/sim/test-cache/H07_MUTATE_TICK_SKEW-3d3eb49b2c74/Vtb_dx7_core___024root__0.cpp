// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__0(Vtb_dx7_core___024root* vlSelf);
VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__1(Vtb_dx7_core___024root* vlSelf);
VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__2(Vtb_dx7_core___024root* vlSelf);

void Vtb_dx7_core___024root___eval_initial(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_initial\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    {
        // Inlined CFunc: _eval_initial__TOP
        VL_READMEM_N(true, 26, 2048, 0, "reference/tables/lfo_sin_table.hex"s
                     ,  &(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_sin_rom)
                     , 0, ~0ULL);
        VL_READMEM_N(true, 32, 1025, 0, "reference/tables/freqlut_table.hex"s
                     ,  &(vlSelfRef.tb_dx7_core__DOT__dut__DOT__freqlut_rom)
                     , 0, ~0ULL);
        VL_READMEM_N(true, 8, 192, 0, "reference/tables/alg_table.hex"s
                     ,  &(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_rom)
                     , 0, ~0ULL);
        VL_READMEM_N(true, 16, 1024, 0, "reference/tables/sinlog_table.hex"s
                     ,  &(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinlog_rom)
                     , 0, ~0ULL);
        VL_READMEM_N(true, 16, 1024, 0, "reference/tables/sinexp_table.hex"s
                     ,  &(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_kern__DOT__sinexp_rom)
                     , 0, ~0ULL);
    }
    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__2(vlSelf);
}

void Vtb_dx7_core___024root___eval_sample(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_sample\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_dx7_core___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtb_dx7_core___024root___eval_ico(Vtb_dx7_core___024root* vlSelf, CData/*0:0*/ firstIteration) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_ico\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[0U]) 
                                     | (IData)((IData)(firstIteration)));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_dx7_core___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
    return (0U);
}

void Vtb_dx7_core___024root___timing_ready(Vtb_dx7_core___024root* vlSelf);
void Vtb_dx7_core___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_dx7_core___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
bool Vtb_dx7_core___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);
void Vtb_dx7_core___024root___timing_resume(Vtb_dx7_core___024root* vlSelf);

bool Vtb_dx7_core___024root___eval_act(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_act\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    {
        // Inlined CFunc: _eval_triggers_vec__act
        vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                        ((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                          << 2U) 
                                                         | ((((~ (IData)(vlSelfRef.tb_dx7_core__DOT__clk)) 
                                                              & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_dx7_core__DOT__clk__0)) 
                                                             << 1U) 
                                                            | ((IData)(vlSelfRef.tb_dx7_core__DOT__clk) 
                                                               & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_dx7_core__DOT__clk__0)))))));
        vlSelfRef.__Vtrigprevexpr___TOP__tb_dx7_core__DOT__clk__0 
            = vlSelfRef.tb_dx7_core__DOT__clk;
    }
    Vtb_dx7_core___024root___timing_ready(vlSelf);
    Vtb_dx7_core___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VactTriggered, vlSelfRef.__VactTriggeredAcc);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_dx7_core___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtb_dx7_core___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_dx7_core___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        vlSelfRef.__VactTriggeredAcc.fill(0ULL);
        Vtb_dx7_core___024root___timing_resume(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb_dx7_core___024root___eval_inact(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_inact\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VinactExecute;
    // Body
    __VinactExecute = vlSelfRef.__VdlySched.awaitingZeroDelay();
    if (__VinactExecute) {
        VL_FATAL_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 47, "", "ZERODLY: Design Verilated with '--no-sched-zero-delay', but #0 delay executed at runtime");
    }
    return (__VinactExecute);
}

void Vtb_dx7_core___024root___eval_body__nba(Vtb_dx7_core___024root* vlSelf);
void Vtb_dx7_core___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out);

bool Vtb_dx7_core___024root___eval_nba(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_nba\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_dx7_core___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_dx7_core___024root___eval_body__nba(vlSelf);
        Vtb_dx7_core___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

bool Vtb_dx7_core___024root___eval_obs(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_obs\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

bool Vtb_dx7_core___024root___eval_react(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_react\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

void Vtb_dx7_core___024root___eval_postponed(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_postponed\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(Vtb_dx7_core___024root* vlSelf, const char* __VeventDescription);

VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__0(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_dx7_core__DOT____VlemExpr_3;
    IData/*31:0*/ tb_dx7_core__DOT____VlemExpr_2;
    IData/*31:0*/ tb_dx7_core__DOT____VlemExpr_1;
    IData/*31:0*/ tb_dx7_core__DOT____VlemExpr_0;
    IData/*31:0*/ tb_dx7_core__DOT__vfd;
    tb_dx7_core__DOT__vfd = 0;
    IData/*31:0*/ tb_dx7_core__DOT__mfd;
    tb_dx7_core__DOT__mfd = 0;
    IData/*31:0*/ tb_dx7_core__DOT__base_frame;
    tb_dx7_core__DOT__base_frame = 0;
    IData/*31:0*/ tb_dx7_core__DOT__fi;
    tb_dx7_core__DOT__fi = 0;
    CData/*0:0*/ tb_dx7_core__DOT__eof;
    tb_dx7_core__DOT__eof = 0;
    IData/*31:0*/ tb_dx7_core__DOT__commit_n;
    tb_dx7_core__DOT__commit_n = 0;
    IData/*31:0*/ tb_dx7_core__DOT__expect_samples;
    tb_dx7_core__DOT__expect_samples = 0;
    IData/*31:0*/ tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0;
    tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1;
    tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__chex__0__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__chex__0__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__0__pbase;
    __Vfunc_tb_dx7_core__DOT__chex__0__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__0__j;
    __Vfunc_tb_dx7_core__DOT__chex__0__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__0__n;
    __Vfunc_tb_dx7_core__DOT__chex__0__n = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_1__hexdig;
    __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_1__hexdig = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__1__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__1__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__1__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__1__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__1__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__1__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__1__j;
    __Vfunc_tb_dx7_core__DOT__ch__1__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__2__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__hexdig__2__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__2__c;
    __Vfunc_tb_dx7_core__DOT__hexdig__2__c = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__chex__3__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__chex__3__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__3__pbase;
    __Vfunc_tb_dx7_core__DOT__chex__3__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__3__j;
    __Vfunc_tb_dx7_core__DOT__chex__3__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__3__n;
    __Vfunc_tb_dx7_core__DOT__chex__3__n = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_1__hexdig;
    __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_1__hexdig = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__4__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__4__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__4__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__4__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__4__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__4__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__4__j;
    __Vfunc_tb_dx7_core__DOT__ch__4__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__5__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__hexdig__5__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__5__c;
    __Vfunc_tb_dx7_core__DOT__hexdig__5__c = 0;
    CData/*0:0*/ __Vtask_tb_dx7_core__DOT__spi_write__6__sec;
    __Vtask_tb_dx7_core__DOT__spi_write__6__sec = 0;
    CData/*7:0*/ __Vtask_tb_dx7_core__DOT__spi_write__6__a;
    __Vtask_tb_dx7_core__DOT__spi_write__6__a = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__spi_write__6__d;
    __Vtask_tb_dx7_core__DOT__spi_write__6__d = 0;
    QData/*47:0*/ __Vtask_tb_dx7_core__DOT__spi_xfer__7__f;
    __Vtask_tb_dx7_core__DOT__spi_xfer__7__f = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ndig__8__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ndig__8__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__8__pbase;
    __Vfunc_tb_dx7_core__DOT__ndig__8__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__8__j;
    __Vfunc_tb_dx7_core__DOT__ndig__8__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__9__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__9__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__9__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__9__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__9__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__9__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__9__j;
    __Vfunc_tb_dx7_core__DOT__ch__9__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__10__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__10__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__10__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__10__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__10__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__10__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__10__j;
    __Vfunc_tb_dx7_core__DOT__ch__10__j = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__11__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__dec_at__11__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__11__pbase;
    __Vfunc_tb_dx7_core__DOT__dec_at__11__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__11__j;
    __Vfunc_tb_dx7_core__DOT__dec_at__11__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__12__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__12__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__12__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__12__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__12__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__12__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__12__j;
    __Vfunc_tb_dx7_core__DOT__ch__12__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__13__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__13__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__13__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__13__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__13__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__13__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__13__j;
    __Vfunc_tb_dx7_core__DOT__ch__13__j = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__14__fr;
    __Vtask_tb_dx7_core__DOT__wait_until__14__fr = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__14____VlefCall_0__frame_ctr_now;
    __Vtask_tb_dx7_core__DOT__wait_until__14____VlefCall_0__frame_ctr_now = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__frame_ctr_now__15__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__frame_ctr_now__15__Vfuncout = 0;
    CData/*0:0*/ __Vtask_tb_dx7_core__DOT__spi_write__16__sec;
    __Vtask_tb_dx7_core__DOT__spi_write__16__sec = 0;
    CData/*7:0*/ __Vtask_tb_dx7_core__DOT__spi_write__16__a;
    __Vtask_tb_dx7_core__DOT__spi_write__16__a = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__spi_write__16__d;
    __Vtask_tb_dx7_core__DOT__spi_write__16__d = 0;
    QData/*47:0*/ __Vtask_tb_dx7_core__DOT__spi_xfer__17__f;
    __Vtask_tb_dx7_core__DOT__spi_xfer__17__f = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dec_at__18__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__dec_at__18__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18__pbase;
    __Vfunc_tb_dx7_core__DOT__dec_at__18__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18__j;
    __Vfunc_tb_dx7_core__DOT__dec_at__18__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__19__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__19__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__19__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__19__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__19__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__19__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__19__j;
    __Vfunc_tb_dx7_core__DOT__ch__19__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__20__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__20__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__20__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__20__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__20__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__20__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__20__j;
    __Vfunc_tb_dx7_core__DOT__ch__20__j = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ndig__21__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ndig__21__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__21__pbase;
    __Vfunc_tb_dx7_core__DOT__ndig__21__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__21__j;
    __Vfunc_tb_dx7_core__DOT__ndig__21__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__22__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__22__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__22__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__22__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__22__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__22__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__22__j;
    __Vfunc_tb_dx7_core__DOT__ch__22__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__23__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__23__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__23__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__23__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__23__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__23__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__23__j;
    __Vfunc_tb_dx7_core__DOT__ch__23__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__chex__24__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__chex__24__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__24__pbase;
    __Vfunc_tb_dx7_core__DOT__chex__24__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__24__j;
    __Vfunc_tb_dx7_core__DOT__chex__24__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__24__n;
    __Vfunc_tb_dx7_core__DOT__chex__24__n = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_1__hexdig;
    __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_1__hexdig = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__25__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__25__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__25__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__25__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__25__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__25__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__25__j;
    __Vfunc_tb_dx7_core__DOT__ch__25__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__26__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__hexdig__26__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__26__c;
    __Vfunc_tb_dx7_core__DOT__hexdig__26__c = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__chex__27__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__chex__27__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__27__pbase;
    __Vfunc_tb_dx7_core__DOT__chex__27__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__27__j;
    __Vfunc_tb_dx7_core__DOT__chex__27__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__27__n;
    __Vfunc_tb_dx7_core__DOT__chex__27__n = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_1__hexdig;
    __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_1__hexdig = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__28__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__28__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__28__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__28__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__28__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__28__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__28__j;
    __Vfunc_tb_dx7_core__DOT__ch__28__j = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__29__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__hexdig__29__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__hexdig__29__c;
    __Vfunc_tb_dx7_core__DOT__hexdig__29__c = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__30__fr;
    __Vtask_tb_dx7_core__DOT__wait_until__30__fr = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__30____VlefCall_0__frame_ctr_now;
    __Vtask_tb_dx7_core__DOT__wait_until__30____VlefCall_0__frame_ctr_now = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__frame_ctr_now__31__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__frame_ctr_now__31__Vfuncout = 0;
    CData/*0:0*/ __Vtask_tb_dx7_core__DOT__spi_write__32__sec;
    __Vtask_tb_dx7_core__DOT__spi_write__32__sec = 0;
    CData/*7:0*/ __Vtask_tb_dx7_core__DOT__spi_write__32__a;
    __Vtask_tb_dx7_core__DOT__spi_write__32__a = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__spi_write__32__d;
    __Vtask_tb_dx7_core__DOT__spi_write__32__d = 0;
    QData/*47:0*/ __Vtask_tb_dx7_core__DOT__spi_xfer__33__f;
    __Vtask_tb_dx7_core__DOT__spi_xfer__33__f = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ndig__34__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ndig__34__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__34__pbase;
    __Vfunc_tb_dx7_core__DOT__ndig__34__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__34__j;
    __Vfunc_tb_dx7_core__DOT__ndig__34__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__35__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__35__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__35__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__35__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__35__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__35__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__35__j;
    __Vfunc_tb_dx7_core__DOT__ch__35__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__36__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__36__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__36__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__36__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__36__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__36__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__36__j;
    __Vfunc_tb_dx7_core__DOT__ch__36__j = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__37__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__dec_at__37__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__37__pbase;
    __Vfunc_tb_dx7_core__DOT__dec_at__37__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__37__j;
    __Vfunc_tb_dx7_core__DOT__dec_at__37__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__38__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__38__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__38__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__38__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__38__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__38__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__38__j;
    __Vfunc_tb_dx7_core__DOT__ch__38__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__39__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__39__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__39__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__39__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__39__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__39__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__39__j;
    __Vfunc_tb_dx7_core__DOT__ch__39__j = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__40__fr;
    __Vtask_tb_dx7_core__DOT__wait_until__40__fr = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until__40____VlefCall_0__frame_ctr_now;
    __Vtask_tb_dx7_core__DOT__wait_until__40____VlefCall_0__frame_ctr_now = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__frame_ctr_now__41__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__frame_ctr_now__41__Vfuncout = 0;
    CData/*0:0*/ __Vtask_tb_dx7_core__DOT__spi_write__42__sec;
    __Vtask_tb_dx7_core__DOT__spi_write__42__sec = 0;
    CData/*7:0*/ __Vtask_tb_dx7_core__DOT__spi_write__42__a;
    __Vtask_tb_dx7_core__DOT__spi_write__42__a = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__spi_write__42__d;
    __Vtask_tb_dx7_core__DOT__spi_write__42__d = 0;
    QData/*47:0*/ __Vtask_tb_dx7_core__DOT__spi_xfer__43__f;
    __Vtask_tb_dx7_core__DOT__spi_xfer__43__f = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dec_at__44__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__dec_at__44__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44__pbase;
    __Vfunc_tb_dx7_core__DOT__dec_at__44__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44__j;
    __Vfunc_tb_dx7_core__DOT__dec_at__44__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__45__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__45__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__45__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__45__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__45__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__45__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__45__j;
    __Vfunc_tb_dx7_core__DOT__ch__45__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__46__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__46__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__46__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__46__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__46__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__46__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__46__j;
    __Vfunc_tb_dx7_core__DOT__ch__46__j = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_frames__47__n;
    __Vtask_tb_dx7_core__DOT__wait_frames__47__n = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ndig__48__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ndig__48__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__48__pbase;
    __Vfunc_tb_dx7_core__DOT__ndig__48__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ndig__48__j;
    __Vfunc_tb_dx7_core__DOT__ndig__48__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__49__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__49__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__49__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__49__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__49__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__49__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__49__j;
    __Vfunc_tb_dx7_core__DOT__ch__49__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__50__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__50__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__50__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__50__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__50__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__50__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__50__j;
    __Vfunc_tb_dx7_core__DOT__ch__50__j = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__51__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__dec_at__51__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__51__pbase;
    __Vfunc_tb_dx7_core__DOT__dec_at__51__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__51__j;
    __Vfunc_tb_dx7_core__DOT__dec_at__51__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_1__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_1__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_0__ch;
    __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_0__ch = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__52__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__52__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__52__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__52__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__52__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__52__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__52__j;
    __Vfunc_tb_dx7_core__DOT__ch__52__j = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__ch__53__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__ch__53__Vfuncout = 0;
    VlWide<8>/*255:0*/ __Vfunc_tb_dx7_core__DOT__ch__53__l;
    VL_ZERO_W(256, __Vfunc_tb_dx7_core__DOT__ch__53__l);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__53__pbase;
    __Vfunc_tb_dx7_core__DOT__ch__53__pbase = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__ch__53__j;
    __Vfunc_tb_dx7_core__DOT__ch__53__j = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until_samples__54__target;
    __Vtask_tb_dx7_core__DOT__wait_until_samples__54__target = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_frames__55__n;
    __Vtask_tb_dx7_core__DOT__wait_frames__55__n = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_until_samples__56__target;
    __Vtask_tb_dx7_core__DOT__wait_until_samples__56__target = 0;
    IData/*31:0*/ __Vtask_tb_dx7_core__DOT__wait_frames__57__n;
    __Vtask_tb_dx7_core__DOT__wait_frames__57__n = 0;
    CData/*31:0*/ __VExpandSel_Lsb_1;
    CData/*31:0*/ __VExpandSel_Lsb_2;
    CData/*31:0*/ __VExpandSel_Lsb_3;
    CData/*31:0*/ __VExpandSel_Lsb_4;
    CData/*31:0*/ __VExpandSel_Lsb_5;
    CData/*31:0*/ __VExpandSel_Lsb_6;
    CData/*31:0*/ __VExpandSel_Lsb_7;
    CData/*31:0*/ __VExpandSel_Lsb_8;
    CData/*31:0*/ __VExpandSel_Lsb_9;
    CData/*31:0*/ __VExpandSel_Lsb_10;
    CData/*31:0*/ __VExpandSel_Lsb_11;
    CData/*31:0*/ __VExpandSel_Lsb_12;
    CData/*31:0*/ __VExpandSel_Lsb_13;
    CData/*31:0*/ __VExpandSel_Lsb_14;
    CData/*31:0*/ __VExpandSel_Lsb_15;
    CData/*31:0*/ __VExpandSel_Lsb_16;
    CData/*31:0*/ __VExpandSel_Lsb_17;
    CData/*31:0*/ __VExpandSel_Lsb_18;
    CData/*31:0*/ __VExpandSel_Lsb_19;
    CData/*31:0*/ __VExpandSel_Lsb_20;
    CData/*31:0*/ __VExpandSel_Lsb_21;
    CData/*31:0*/ __VExpandSel_Lsb_22;
    // Body
    tb_dx7_core__DOT____VlemExpr_0 = VL_VALUEPLUSARGS_INW(1024, "vectors=%s"s, 
                                                          vlSelfRef.tb_dx7_core__DOT__vfile);
    if (VL_UNLIKELY(((! tb_dx7_core__DOT____VlemExpr_0)))) {
        VL_WRITEF_NX("FAIL tb_dx7_core (missing +vectors=)\n",0);
        VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 161, "");
    }
    tb_dx7_core__DOT____VlemExpr_1 = VL_VALUEPLUSARGS_INW(1024, "actual=%s"s, 
                                                          vlSelfRef.tb_dx7_core__DOT__afile);
    if (VL_UNLIKELY(((! tb_dx7_core__DOT____VlemExpr_1)))) {
        VL_WRITEF_NX("FAIL tb_dx7_core (missing +actual=)\n",0);
        VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 165, "");
    }
    tb_dx7_core__DOT____VlemExpr_2 = VL_VALUEPLUSARGS_INW(1024, "meta=%s"s, 
                                                          vlSelfRef.tb_dx7_core__DOT__mfile);
    if (VL_UNLIKELY(((! tb_dx7_core__DOT____VlemExpr_2)))) {
        VL_WRITEF_NX("FAIL tb_dx7_core (missing +meta=)\n",0);
        VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 169, "");
    }
    tb_dx7_core__DOT__vfd = VL_FOPEN_NN(VL_CVT_PACK_STR_NW(32, vlSelfRef.tb_dx7_core__DOT__vfile)
                                        , "r"s);
    ;
    if (VL_UNLIKELY(((0U == tb_dx7_core__DOT__vfd)))) {
        VL_WRITEF_NX("FAIL tb_dx7_core (cannot open %0s)\n",1
                     , '#',1024,vlSelfRef.tb_dx7_core__DOT__vfile.data());
        VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 174, "");
    }
    vlSelfRef.tb_dx7_core__DOT__afd = VL_FOPEN_NN(VL_CVT_PACK_STR_NW(32, vlSelfRef.tb_dx7_core__DOT__afile)
                                                  , "wb"s);
    ;
    tb_dx7_core__DOT__mfd = VL_FOPEN_NN(VL_CVT_PACK_STR_NW(32, vlSelfRef.tb_dx7_core__DOT__mfile)
                                        , "w"s);
    ;
    vlSelfRef.tb_dx7_core__DOT__samples = 0U;
    tb_dx7_core__DOT__base_frame = 0xffffffffU;
    tb_dx7_core__DOT__commit_n = 0U;
    tb_dx7_core__DOT__expect_samples = 0U;
    tb_dx7_core__DOT____VlemExpr_3 = VL_VALUEPLUSARGS_INI(32, "expect=%d"s, 
                                                          tb_dx7_core__DOT__expect_samples);
    if ((! tb_dx7_core__DOT____VlemExpr_3)) {
        tb_dx7_core__DOT__expect_samples = 0U;
    }
    vlSelfRef.tb_dx7_core__DOT__lr_d = 1U;
    tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0x0000000aU;
    while (VL_LTS_III(32, 0U, tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                           "@(negedge tb_dx7_core.clk)");
        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(negedge tb_dx7_core.clk)", 
                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                             188);
        tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (tb_dx7_core__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_dx7_core__DOT__rst_n = 1U;
    tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0x0000000aU;
    while (VL_LTS_III(32, 0U, tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                           "@(negedge tb_dx7_core.clk)");
        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(negedge tb_dx7_core.clk)", 
                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                             190);
        tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (tb_dx7_core__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    tb_dx7_core__DOT__eof = 0U;
    while ((1U & (~ (IData)(tb_dx7_core__DOT__eof)))) {
        vlSelfRef.tb_dx7_core__DOT__rc = VL_FGETS_IXI(256, &(vlSelfRef.tb_dx7_core__DOT__line), tb_dx7_core__DOT__vfd);
        if ((0U == vlSelfRef.tb_dx7_core__DOT__rc)) {
            tb_dx7_core__DOT__eof = 1U;
        } else {
            vlSelfRef.tb_dx7_core__DOT__p0 = 0x0000001fU;
            while ((VL_LTS_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__p0) 
                    & (0U == (0x000000ffU & (((0U == 
                                               (0x0000001fU 
                                                & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0)))
                                               ? 0U
                                               : (vlSelfRef.tb_dx7_core__DOT__line
                                                  [
                                                  (((IData)(7U) 
                                                    + 
                                                    (0x000000ffU 
                                                     & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0))) 
                                                   >> 5U)] 
                                                  << 
                                                  ((IData)(0x00000020U) 
                                                   - 
                                                   (0x0000001fU 
                                                    & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0))))) 
                                             | (vlSelfRef.tb_dx7_core__DOT__line
                                                [(7U 
                                                  & (VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0) 
                                                     >> 5U))] 
                                                >> 
                                                (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0)))))))) {
                vlSelfRef.tb_dx7_core__DOT__p0 = (vlSelfRef.tb_dx7_core__DOT__p0 
                                                  - (IData)(1U));
            }
            vlSelfRef.tb_dx7_core__DOT__tok = (0x000000ffU 
                                               & (((0U 
                                                    == 
                                                    (0x0000001fU 
                                                     & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0)))
                                                    ? 0U
                                                    : 
                                                   (vlSelfRef.tb_dx7_core__DOT__line
                                                    [
                                                    (((IData)(7U) 
                                                      + 
                                                      (0x000000ffU 
                                                       & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0))) 
                                                     >> 5U)] 
                                                    << 
                                                    ((IData)(0x00000020U) 
                                                     - 
                                                     (0x0000001fU 
                                                      & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0))))) 
                                                  | (vlSelfRef.tb_dx7_core__DOT__line
                                                     [
                                                     (7U 
                                                      & (VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0) 
                                                         >> 5U))] 
                                                     >> 
                                                     (0x0000001fU 
                                                      & VL_MULS_III(32, (IData)(8U), vlSelfRef.tb_dx7_core__DOT__p0)))));
            if ((0x50U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                __Vfunc_tb_dx7_core__DOT__chex__0__n = 2U;
                __Vfunc_tb_dx7_core__DOT__chex__0__j = 2U;
                __Vfunc_tb_dx7_core__DOT__chex__0__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__chex__0__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__chex__0__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout = 0U;
                vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k, __Vfunc_tb_dx7_core__DOT__chex__0__n)) {
                    __Vfunc_tb_dx7_core__DOT__ch__1__j 
                        = (__Vfunc_tb_dx7_core__DOT__chex__0__j 
                           + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__1__pbase 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__1__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__0__l[7U];
                    __VExpandSel_Lsb_1 = (0x000000ffU 
                                          & VL_MULS_III(32, (IData)(8U), 
                                                        (__Vfunc_tb_dx7_core__DOT__ch__1__pbase 
                                                         - __Vfunc_tb_dx7_core__DOT__ch__1__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__1__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__1__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__1__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__1__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_1) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__1__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__1__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__1__l
                                             [(__VExpandSel_Lsb_1 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__1__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__1__j))))));
                    __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__1__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__hexdig__2__c 
                        = __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_0__ch;
                    __Vfunc_tb_dx7_core__DOT__hexdig__2__Vfuncout 
                        = (((0x30U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)) 
                            & (0x39U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)))
                            ? ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c) 
                               - (IData)(0x00000030U))
                            : (((0x61U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)) 
                                & (0x66U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)))
                                ? ((IData)(0x0000000aU) 
                                   + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c) 
                                      - (IData)(0x00000061U)))
                                : (((0x41U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)) 
                                    & (0x46U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c)))
                                    ? ((IData)(0x0000000aU) 
                                       + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__2__c) 
                                          - (IData)(0x00000041U)))
                                    : 0U)));
                    __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_1__hexdig 
                        = __Vfunc_tb_dx7_core__DOT__hexdig__2__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout 
                        = (VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout) 
                           + __Vfunc_tb_dx7_core__DOT__chex__0____VlefCall_1__hexdig);
                    vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                }
                vlSelfRef.tb_dx7_core__DOT__addr = __Vfunc_tb_dx7_core__DOT__chex__0__Vfuncout;
                __Vfunc_tb_dx7_core__DOT__chex__3__n = 8U;
                __Vfunc_tb_dx7_core__DOT__chex__3__j = 5U;
                __Vfunc_tb_dx7_core__DOT__chex__3__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__chex__3__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__chex__3__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout = 0U;
                vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k, __Vfunc_tb_dx7_core__DOT__chex__3__n)) {
                    __Vfunc_tb_dx7_core__DOT__ch__4__j 
                        = (__Vfunc_tb_dx7_core__DOT__chex__3__j 
                           + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__4__pbase 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__4__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__3__l[7U];
                    __VExpandSel_Lsb_2 = (0x000000ffU 
                                          & VL_MULS_III(32, (IData)(8U), 
                                                        (__Vfunc_tb_dx7_core__DOT__ch__4__pbase 
                                                         - __Vfunc_tb_dx7_core__DOT__ch__4__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__4__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__4__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__4__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__4__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_2) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__4__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__4__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__4__l
                                             [(__VExpandSel_Lsb_2 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__4__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__4__j))))));
                    __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__4__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__hexdig__5__c 
                        = __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_0__ch;
                    __Vfunc_tb_dx7_core__DOT__hexdig__5__Vfuncout 
                        = (((0x30U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)) 
                            & (0x39U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)))
                            ? ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c) 
                               - (IData)(0x00000030U))
                            : (((0x61U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)) 
                                & (0x66U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)))
                                ? ((IData)(0x0000000aU) 
                                   + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c) 
                                      - (IData)(0x00000061U)))
                                : (((0x41U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)) 
                                    & (0x46U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c)))
                                    ? ((IData)(0x0000000aU) 
                                       + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__5__c) 
                                          - (IData)(0x00000041U)))
                                    : 0U)));
                    __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_1__hexdig 
                        = __Vfunc_tb_dx7_core__DOT__hexdig__5__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout 
                        = (VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout) 
                           + __Vfunc_tb_dx7_core__DOT__chex__3____VlefCall_1__hexdig);
                    vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                }
                vlSelfRef.tb_dx7_core__DOT__data = __Vfunc_tb_dx7_core__DOT__chex__3__Vfuncout;
                __Vtask_tb_dx7_core__DOT__spi_write__6__d 
                    = vlSelfRef.tb_dx7_core__DOT__data;
                __Vtask_tb_dx7_core__DOT__spi_write__6__a 
                    = (0x000000ffU & vlSelfRef.tb_dx7_core__DOT__addr);
                __Vtask_tb_dx7_core__DOT__spi_write__6__sec = 1U;
                __Vtask_tb_dx7_core__DOT__spi_xfer__7__f 
                    = (0x0000800000000000ULL | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__6__sec)) 
                                                 << 0x00000028U) 
                                                | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__6__a)) 
                                                    << 0x00000020U) 
                                                   | (QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__6__d)))));
                vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f 
                    = __Vtask_tb_dx7_core__DOT__spi_xfer__7__f;
                vlSelfRef.tb_dx7_core__DOT__tx_frame 
                    = vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f;
                Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                   "@(negedge tb_dx7_core.clk)");
                co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                     nullptr, 
                                                                     "@(negedge tb_dx7_core.clk)", 
                                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                     86);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 0U;
                vlSelfRef.tb_dx7_core__DOT__bit_i = 0x0000002fU;
                while (VL_LTES_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__bit_i)) {
                    vlSelfRef.tb_dx7_core__DOT__spi_mosi 
                        = ((0x2fU >= (0x0000003fU & vlSelfRef.tb_dx7_core__DOT__bit_i)) 
                           && (1U & (IData)((vlSelfRef.tb_dx7_core__DOT__tx_frame 
                                             >> (0x0000003fU 
                                                 & vlSelfRef.tb_dx7_core__DOT__bit_i)))));
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         90);
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 1U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         91);
                    vlSelfRef.tb_dx7_core__DOT__bit_i 
                        = (vlSelfRef.tb_dx7_core__DOT__bit_i 
                           - (IData)(1U));
                }
                vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     94);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 1U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     96);
            } else if ((0x43U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                __Vfunc_tb_dx7_core__DOT__ndig__8__j = 2U;
                __Vfunc_tb_dx7_core__DOT__ndig__8__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__ndig__8__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__j;
                __Vfunc_tb_dx7_core__DOT__ch__9__j 
                    = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__9__pbase 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__9__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__9__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8__l[7U];
                __VExpandSel_Lsb_3 = (0x000000ffU & 
                                      VL_MULS_III(32, (IData)(8U), 
                                                  (__Vfunc_tb_dx7_core__DOT__ch__9__pbase 
                                                   - __Vfunc_tb_dx7_core__DOT__ch__9__j)));
                __Vfunc_tb_dx7_core__DOT__ch__9__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__9__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__9__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__9__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_3) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__9__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__9__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__9__l
                                         [(__VExpandSel_Lsb_3 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__9__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__9__j))))));
                __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__9__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__10__j 
                        = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__10__pbase 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__10__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8__l[7U];
                    __VExpandSel_Lsb_4 = (0x000000ffU 
                                          & VL_MULS_III(32, (IData)(8U), 
                                                        (__Vfunc_tb_dx7_core__DOT__ch__10__pbase 
                                                         - __Vfunc_tb_dx7_core__DOT__ch__10__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__10__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__10__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__10__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__10__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_4) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__10__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__10__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__10__l
                                             [(__VExpandSel_Lsb_4 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__10__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__10__j))))));
                    __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__10__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__ndig__8____VlefCall_1__ch;
                }
                vlSelfRef.tb_dx7_core__DOT____VlemCall_4__ndig 
                    = (vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                       - __Vfunc_tb_dx7_core__DOT__ndig__8__j);
                if (VL_LTS_III(32, 0U, vlSelfRef.tb_dx7_core__DOT____VlemCall_4__ndig)) {
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__j = 2U;
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__pbase 
                        = vlSelfRef.tb_dx7_core__DOT__p0;
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[0U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[0U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[1U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[1U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[2U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[2U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[3U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[3U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[4U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[4U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[5U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[5U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[6U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[6U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__11__l[7U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[7U];
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__j;
                    __Vfunc_tb_dx7_core__DOT__ch__12__j 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__12__pbase 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__12__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[7U];
                    __VExpandSel_Lsb_5 = (0x000000ffU 
                                          & VL_MULS_III(32, (IData)(8U), 
                                                        (__Vfunc_tb_dx7_core__DOT__ch__12__pbase 
                                                         - __Vfunc_tb_dx7_core__DOT__ch__12__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__12__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__12__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__12__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__12__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_5) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__12__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__12__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__12__l
                                             [(__VExpandSel_Lsb_5 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__12__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__12__j))))));
                    __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__12__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_0__ch;
                    while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c) 
                            & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c))) {
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v 
                            = (((IData)(0x0000000aU) 
                                * vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v) 
                               + (vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                                  - (IData)(0x00000030U)));
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                            = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k);
                        __Vfunc_tb_dx7_core__DOT__ch__13__j 
                            = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                        __Vfunc_tb_dx7_core__DOT__ch__13__pbase 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__pbase;
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[0U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[0U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[1U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[1U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[2U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[2U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[3U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[3U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[4U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[4U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[5U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[5U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[6U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[6U];
                        __Vfunc_tb_dx7_core__DOT__ch__13__l[7U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11__l[7U];
                        __VExpandSel_Lsb_6 = (0x000000ffU 
                                              & VL_MULS_III(32, (IData)(8U), 
                                                            (__Vfunc_tb_dx7_core__DOT__ch__13__pbase 
                                                             - __Vfunc_tb_dx7_core__DOT__ch__13__j)));
                        __Vfunc_tb_dx7_core__DOT__ch__13__Vfuncout 
                            = (0x000000ffU & (((0U 
                                                == 
                                                (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__13__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__13__j))))
                                                ? 0U
                                                : (__Vfunc_tb_dx7_core__DOT__ch__13__l
                                                   [
                                                   (((IData)(7U) 
                                                     + __VExpandSel_Lsb_6) 
                                                    >> 5U)] 
                                                   << 
                                                   ((IData)(0x00000020U) 
                                                    - 
                                                    (0x0000001fU 
                                                     & VL_MULS_III(32, (IData)(8U), 
                                                                   (__Vfunc_tb_dx7_core__DOT__ch__13__pbase 
                                                                    - __Vfunc_tb_dx7_core__DOT__ch__13__j)))))) 
                                              | (__Vfunc_tb_dx7_core__DOT__ch__13__l
                                                 [(__VExpandSel_Lsb_6 
                                                   >> 5U)] 
                                                 >> 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__13__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__13__j))))));
                        __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_1__ch 
                            = __Vfunc_tb_dx7_core__DOT__ch__13__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__11____VlefCall_1__ch;
                    }
                    vlSelfRef.tb_dx7_core__DOT____VlemCall_5__dec_at 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v;
                    __Vtask_tb_dx7_core__DOT__wait_until__14__fr 
                        = (tb_dx7_core__DOT__base_frame 
                           + vlSelfRef.tb_dx7_core__DOT____VlemCall_5__dec_at);
                    vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr 
                        = __Vtask_tb_dx7_core__DOT__wait_until__14__fr;
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         283);
                    while (true) {
                        __Vfunc_tb_dx7_core__DOT__frame_ctr_now__15__Vfuncout 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr;
                        __Vtask_tb_dx7_core__DOT__wait_until__14____VlefCall_0__frame_ctr_now 
                            = __Vfunc_tb_dx7_core__DOT__frame_ctr_now__15__Vfuncout;
                        if (!(VL_LTS_III(32, __Vtask_tb_dx7_core__DOT__wait_until__14____VlefCall_0__frame_ctr_now, vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr))) break;
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             285);
                    }
                }
                __Vtask_tb_dx7_core__DOT__spi_write__16__d = 0U;
                __Vtask_tb_dx7_core__DOT__spi_write__16__a = 0x42U;
                __Vtask_tb_dx7_core__DOT__spi_write__16__sec = 1U;
                __Vtask_tb_dx7_core__DOT__spi_xfer__17__f 
                    = (0x0000800000000000ULL | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__16__sec)) 
                                                 << 0x00000028U) 
                                                | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__16__a)) 
                                                    << 0x00000020U) 
                                                   | (QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__16__d)))));
                vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f 
                    = __Vtask_tb_dx7_core__DOT__spi_xfer__17__f;
                vlSelfRef.tb_dx7_core__DOT__tx_frame 
                    = vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f;
                Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                   "@(negedge tb_dx7_core.clk)");
                co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                     nullptr, 
                                                                     "@(negedge tb_dx7_core.clk)", 
                                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                     86);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 0U;
                vlSelfRef.tb_dx7_core__DOT__bit_i = 0x0000002fU;
                while (VL_LTES_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__bit_i)) {
                    vlSelfRef.tb_dx7_core__DOT__spi_mosi 
                        = ((0x2fU >= (0x0000003fU & vlSelfRef.tb_dx7_core__DOT__bit_i)) 
                           && (1U & (IData)((vlSelfRef.tb_dx7_core__DOT__tx_frame 
                                             >> (0x0000003fU 
                                                 & vlSelfRef.tb_dx7_core__DOT__bit_i)))));
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         90);
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 1U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         91);
                    vlSelfRef.tb_dx7_core__DOT__bit_i 
                        = (vlSelfRef.tb_dx7_core__DOT__bit_i 
                           - (IData)(1U));
                }
                vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     94);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 1U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     96);
                tb_dx7_core__DOT__base_frame = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr;
                vlSelfRef.tb_dx7_core__DOT__commit_list[(0x0000000fU 
                                                         & tb_dx7_core__DOT__commit_n)] 
                    = tb_dx7_core__DOT__base_frame;
                tb_dx7_core__DOT__commit_n = ((IData)(1U) 
                                              + tb_dx7_core__DOT__commit_n);
            } else if ((0x45U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                vlSelfRef.tb_dx7_core__DOT__pd = 2U;
                __Vfunc_tb_dx7_core__DOT__dec_at__18__j 
                    = vlSelfRef.tb_dx7_core__DOT__pd;
                __Vfunc_tb_dx7_core__DOT__dec_at__18__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__dec_at__18__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v = 0U;
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__j;
                __Vfunc_tb_dx7_core__DOT__ch__19__j 
                    = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__19__pbase 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__19__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__19__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[7U];
                __VExpandSel_Lsb_7 = (0x000000ffU & 
                                      VL_MULS_III(32, (IData)(8U), 
                                                  (__Vfunc_tb_dx7_core__DOT__ch__19__pbase 
                                                   - __Vfunc_tb_dx7_core__DOT__ch__19__j)));
                __Vfunc_tb_dx7_core__DOT__ch__19__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__19__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__19__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__19__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_7) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__19__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__19__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__19__l
                                         [(__VExpandSel_Lsb_7 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__19__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__19__j))))));
                __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__19__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v 
                        = (((IData)(0x0000000aU) * vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v) 
                           + (vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                              - (IData)(0x00000030U)));
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__20__j 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__20__pbase 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__20__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18__l[7U];
                    __VExpandSel_Lsb_8 = (0x000000ffU 
                                          & VL_MULS_III(32, (IData)(8U), 
                                                        (__Vfunc_tb_dx7_core__DOT__ch__20__pbase 
                                                         - __Vfunc_tb_dx7_core__DOT__ch__20__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__20__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__20__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__20__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__20__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_8) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__20__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__20__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__20__l
                                             [(__VExpandSel_Lsb_8 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__20__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__20__j))))));
                    __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__20__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__18____VlefCall_1__ch;
                }
                __Vfunc_tb_dx7_core__DOT__dec_at__18__Vfuncout 
                    = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v;
                vlSelfRef.tb_dx7_core__DOT__vdec = __Vfunc_tb_dx7_core__DOT__dec_at__18__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__block = vlSelfRef.tb_dx7_core__DOT__vdec;
                __Vfunc_tb_dx7_core__DOT__ndig__21__j 
                    = vlSelfRef.tb_dx7_core__DOT__pd;
                __Vfunc_tb_dx7_core__DOT__ndig__21__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__ndig__21__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__j;
                __Vfunc_tb_dx7_core__DOT__ch__22__j 
                    = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__22__pbase 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__22__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__22__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21__l[7U];
                __VExpandSel_Lsb_9 = (0x000000ffU & 
                                      VL_MULS_III(32, (IData)(8U), 
                                                  (__Vfunc_tb_dx7_core__DOT__ch__22__pbase 
                                                   - __Vfunc_tb_dx7_core__DOT__ch__22__j)));
                __Vfunc_tb_dx7_core__DOT__ch__22__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__22__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__22__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__22__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_9) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__22__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__22__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__22__l
                                         [(__VExpandSel_Lsb_9 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__22__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__22__j))))));
                __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__22__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__23__j 
                        = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__23__pbase 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__23__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21__l[7U];
                    __VExpandSel_Lsb_10 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__23__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__23__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__23__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__23__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__23__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__23__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_10) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__23__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__23__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__23__l
                                             [(__VExpandSel_Lsb_10 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__23__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__23__j))))));
                    __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__23__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__ndig__21____VlefCall_1__ch;
                }
                vlSelfRef.tb_dx7_core__DOT____VlemCall_6__ndig 
                    = (vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                       - __Vfunc_tb_dx7_core__DOT__ndig__21__j);
                vlSelfRef.tb_dx7_core__DOT__pd = ((IData)(1U) 
                                                  + 
                                                  (vlSelfRef.tb_dx7_core__DOT__pd 
                                                   + vlSelfRef.tb_dx7_core__DOT____VlemCall_6__ndig));
                __Vfunc_tb_dx7_core__DOT__chex__24__n = 2U;
                __Vfunc_tb_dx7_core__DOT__chex__24__j 
                    = vlSelfRef.tb_dx7_core__DOT__pd;
                __Vfunc_tb_dx7_core__DOT__chex__24__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__chex__24__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__chex__24__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout = 0U;
                vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k, __Vfunc_tb_dx7_core__DOT__chex__24__n)) {
                    __Vfunc_tb_dx7_core__DOT__ch__25__j 
                        = (__Vfunc_tb_dx7_core__DOT__chex__24__j 
                           + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__25__pbase 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__25__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__24__l[7U];
                    __VExpandSel_Lsb_11 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__25__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__25__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__25__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__25__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__25__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__25__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_11) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__25__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__25__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__25__l
                                             [(__VExpandSel_Lsb_11 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__25__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__25__j))))));
                    __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__25__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__hexdig__26__c 
                        = __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_0__ch;
                    __Vfunc_tb_dx7_core__DOT__hexdig__26__Vfuncout 
                        = (((0x30U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)) 
                            & (0x39U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)))
                            ? ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c) 
                               - (IData)(0x00000030U))
                            : (((0x61U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)) 
                                & (0x66U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)))
                                ? ((IData)(0x0000000aU) 
                                   + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c) 
                                      - (IData)(0x00000061U)))
                                : (((0x41U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)) 
                                    & (0x46U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c)))
                                    ? ((IData)(0x0000000aU) 
                                       + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__26__c) 
                                          - (IData)(0x00000041U)))
                                    : 0U)));
                    __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_1__hexdig 
                        = __Vfunc_tb_dx7_core__DOT__hexdig__26__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout 
                        = (VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout) 
                           + __Vfunc_tb_dx7_core__DOT__chex__24____VlefCall_1__hexdig);
                    vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                }
                vlSelfRef.tb_dx7_core__DOT__addr = __Vfunc_tb_dx7_core__DOT__chex__24__Vfuncout;
                __Vfunc_tb_dx7_core__DOT__chex__27__n = 8U;
                __Vfunc_tb_dx7_core__DOT__chex__27__j 
                    = ((IData)(3U) + vlSelfRef.tb_dx7_core__DOT__pd);
                __Vfunc_tb_dx7_core__DOT__chex__27__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__chex__27__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__chex__27__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout = 0U;
                vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k, __Vfunc_tb_dx7_core__DOT__chex__27__n)) {
                    __Vfunc_tb_dx7_core__DOT__ch__28__j 
                        = (__Vfunc_tb_dx7_core__DOT__chex__27__j 
                           + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__28__pbase 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__28__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__chex__27__l[7U];
                    __VExpandSel_Lsb_12 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__28__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__28__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__28__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__28__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__28__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__28__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_12) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__28__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__28__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__28__l
                                             [(__VExpandSel_Lsb_12 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__28__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__28__j))))));
                    __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__28__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__hexdig__29__c 
                        = __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_0__ch;
                    __Vfunc_tb_dx7_core__DOT__hexdig__29__Vfuncout 
                        = (((0x30U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)) 
                            & (0x39U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)))
                            ? ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c) 
                               - (IData)(0x00000030U))
                            : (((0x61U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)) 
                                & (0x66U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)))
                                ? ((IData)(0x0000000aU) 
                                   + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c) 
                                      - (IData)(0x00000061U)))
                                : (((0x41U <= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)) 
                                    & (0x46U >= (IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c)))
                                    ? ((IData)(0x0000000aU) 
                                       + ((IData)(__Vfunc_tb_dx7_core__DOT__hexdig__29__c) 
                                          - (IData)(0x00000041U)))
                                    : 0U)));
                    __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_1__hexdig 
                        = __Vfunc_tb_dx7_core__DOT__hexdig__29__Vfuncout;
                    __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout 
                        = (VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout) 
                           + __Vfunc_tb_dx7_core__DOT__chex__27____VlefCall_1__hexdig);
                    vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__chex__Vstatic__k);
                }
                vlSelfRef.tb_dx7_core__DOT__data = __Vfunc_tb_dx7_core__DOT__chex__27__Vfuncout;
                if (VL_LTS_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__block)) {
                    __Vtask_tb_dx7_core__DOT__wait_until__30__fr 
                        = (tb_dx7_core__DOT__base_frame 
                           + vlSelfRef.tb_dx7_core__DOT__block);
                    vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr 
                        = __Vtask_tb_dx7_core__DOT__wait_until__30__fr;
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         283);
                    while (true) {
                        __Vfunc_tb_dx7_core__DOT__frame_ctr_now__31__Vfuncout 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr;
                        __Vtask_tb_dx7_core__DOT__wait_until__30____VlefCall_0__frame_ctr_now 
                            = __Vfunc_tb_dx7_core__DOT__frame_ctr_now__31__Vfuncout;
                        if (!(VL_LTS_III(32, __Vtask_tb_dx7_core__DOT__wait_until__30____VlefCall_0__frame_ctr_now, vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr))) break;
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             285);
                    }
                }
                __Vtask_tb_dx7_core__DOT__spi_write__32__d 
                    = vlSelfRef.tb_dx7_core__DOT__data;
                __Vtask_tb_dx7_core__DOT__spi_write__32__a 
                    = (0x000000ffU & vlSelfRef.tb_dx7_core__DOT__addr);
                __Vtask_tb_dx7_core__DOT__spi_write__32__sec = 0U;
                __Vtask_tb_dx7_core__DOT__spi_xfer__33__f 
                    = (0x0000800000000000ULL | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__32__sec)) 
                                                 << 0x00000028U) 
                                                | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__32__a)) 
                                                    << 0x00000020U) 
                                                   | (QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__32__d)))));
                vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f 
                    = __Vtask_tb_dx7_core__DOT__spi_xfer__33__f;
                vlSelfRef.tb_dx7_core__DOT__tx_frame 
                    = vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f;
                Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                   "@(negedge tb_dx7_core.clk)");
                co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                     nullptr, 
                                                                     "@(negedge tb_dx7_core.clk)", 
                                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                     86);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 0U;
                vlSelfRef.tb_dx7_core__DOT__bit_i = 0x0000002fU;
                while (VL_LTES_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__bit_i)) {
                    vlSelfRef.tb_dx7_core__DOT__spi_mosi 
                        = ((0x2fU >= (0x0000003fU & vlSelfRef.tb_dx7_core__DOT__bit_i)) 
                           && (1U & (IData)((vlSelfRef.tb_dx7_core__DOT__tx_frame 
                                             >> (0x0000003fU 
                                                 & vlSelfRef.tb_dx7_core__DOT__bit_i)))));
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         90);
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 1U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         91);
                    vlSelfRef.tb_dx7_core__DOT__bit_i 
                        = (vlSelfRef.tb_dx7_core__DOT__bit_i 
                           - (IData)(1U));
                }
                vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     94);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 1U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     96);
            } else if ((0x53U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                __Vfunc_tb_dx7_core__DOT__ndig__34__j = 2U;
                __Vfunc_tb_dx7_core__DOT__ndig__34__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__ndig__34__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__j;
                __Vfunc_tb_dx7_core__DOT__ch__35__j 
                    = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__35__pbase 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__35__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__35__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34__l[7U];
                __VExpandSel_Lsb_13 = (0x000000ffU 
                                       & VL_MULS_III(32, (IData)(8U), 
                                                     (__Vfunc_tb_dx7_core__DOT__ch__35__pbase 
                                                      - __Vfunc_tb_dx7_core__DOT__ch__35__j)));
                __Vfunc_tb_dx7_core__DOT__ch__35__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__35__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__35__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__35__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_13) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__35__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__35__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__35__l
                                         [(__VExpandSel_Lsb_13 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__35__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__35__j))))));
                __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__35__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__36__j 
                        = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__36__pbase 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__36__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34__l[7U];
                    __VExpandSel_Lsb_14 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__36__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__36__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__36__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__36__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__36__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__36__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_14) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__36__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__36__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__36__l
                                             [(__VExpandSel_Lsb_14 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__36__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__36__j))))));
                    __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__36__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__ndig__34____VlefCall_1__ch;
                }
                vlSelfRef.tb_dx7_core__DOT____VlemCall_7__ndig 
                    = (vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                       - __Vfunc_tb_dx7_core__DOT__ndig__34__j);
                if (VL_LTS_III(32, 0U, vlSelfRef.tb_dx7_core__DOT____VlemCall_7__ndig)) {
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__j = 2U;
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__pbase 
                        = vlSelfRef.tb_dx7_core__DOT__p0;
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[0U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[0U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[1U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[1U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[2U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[2U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[3U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[3U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[4U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[4U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[5U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[5U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[6U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[6U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__37__l[7U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[7U];
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__j;
                    __Vfunc_tb_dx7_core__DOT__ch__38__j 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__38__pbase 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__38__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[7U];
                    __VExpandSel_Lsb_15 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__38__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__38__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__38__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__38__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__38__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__38__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_15) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__38__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__38__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__38__l
                                             [(__VExpandSel_Lsb_15 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__38__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__38__j))))));
                    __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__38__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_0__ch;
                    while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c) 
                            & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c))) {
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v 
                            = (((IData)(0x0000000aU) 
                                * vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v) 
                               + (vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                                  - (IData)(0x00000030U)));
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                            = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k);
                        __Vfunc_tb_dx7_core__DOT__ch__39__j 
                            = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                        __Vfunc_tb_dx7_core__DOT__ch__39__pbase 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__pbase;
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[0U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[0U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[1U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[1U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[2U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[2U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[3U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[3U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[4U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[4U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[5U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[5U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[6U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[6U];
                        __Vfunc_tb_dx7_core__DOT__ch__39__l[7U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37__l[7U];
                        __VExpandSel_Lsb_16 = (0x000000ffU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__39__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__39__j)));
                        __Vfunc_tb_dx7_core__DOT__ch__39__Vfuncout 
                            = (0x000000ffU & (((0U 
                                                == 
                                                (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__39__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__39__j))))
                                                ? 0U
                                                : (__Vfunc_tb_dx7_core__DOT__ch__39__l
                                                   [
                                                   (((IData)(7U) 
                                                     + __VExpandSel_Lsb_16) 
                                                    >> 5U)] 
                                                   << 
                                                   ((IData)(0x00000020U) 
                                                    - 
                                                    (0x0000001fU 
                                                     & VL_MULS_III(32, (IData)(8U), 
                                                                   (__Vfunc_tb_dx7_core__DOT__ch__39__pbase 
                                                                    - __Vfunc_tb_dx7_core__DOT__ch__39__j)))))) 
                                              | (__Vfunc_tb_dx7_core__DOT__ch__39__l
                                                 [(__VExpandSel_Lsb_16 
                                                   >> 5U)] 
                                                 >> 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__39__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__39__j))))));
                        __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_1__ch 
                            = __Vfunc_tb_dx7_core__DOT__ch__39__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__37____VlefCall_1__ch;
                    }
                    vlSelfRef.tb_dx7_core__DOT____VlemCall_8__dec_at 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v;
                    __Vtask_tb_dx7_core__DOT__wait_until__40__fr 
                        = (tb_dx7_core__DOT__base_frame 
                           + vlSelfRef.tb_dx7_core__DOT____VlemCall_8__dec_at);
                    vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr 
                        = __Vtask_tb_dx7_core__DOT__wait_until__40__fr;
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         283);
                    while (true) {
                        __Vfunc_tb_dx7_core__DOT__frame_ctr_now__41__Vfuncout 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr;
                        __Vtask_tb_dx7_core__DOT__wait_until__40____VlefCall_0__frame_ctr_now 
                            = __Vfunc_tb_dx7_core__DOT__frame_ctr_now__41__Vfuncout;
                        if (!(VL_LTS_III(32, __Vtask_tb_dx7_core__DOT__wait_until__40____VlefCall_0__frame_ctr_now, vlSelfRef.tb_dx7_core__DOT__wait_until__Vstatic__fr))) break;
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             285);
                    }
                }
                __Vtask_tb_dx7_core__DOT__spi_write__42__d = 0U;
                __Vtask_tb_dx7_core__DOT__spi_write__42__a = 0x0fU;
                __Vtask_tb_dx7_core__DOT__spi_write__42__sec = 0U;
                __Vtask_tb_dx7_core__DOT__spi_xfer__43__f 
                    = (0x0000800000000000ULL | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__42__sec)) 
                                                 << 0x00000028U) 
                                                | (((QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__42__a)) 
                                                    << 0x00000020U) 
                                                   | (QData)((IData)(__Vtask_tb_dx7_core__DOT__spi_write__42__d)))));
                vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f 
                    = __Vtask_tb_dx7_core__DOT__spi_xfer__43__f;
                vlSelfRef.tb_dx7_core__DOT__tx_frame 
                    = vlSelfRef.tb_dx7_core__DOT__spi_xfer__Vstatic__f;
                Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                   "@(negedge tb_dx7_core.clk)");
                co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                     nullptr, 
                                                                     "@(negedge tb_dx7_core.clk)", 
                                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                     86);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 0U;
                vlSelfRef.tb_dx7_core__DOT__bit_i = 0x0000002fU;
                while (VL_LTES_III(32, 0U, vlSelfRef.tb_dx7_core__DOT__bit_i)) {
                    vlSelfRef.tb_dx7_core__DOT__spi_mosi 
                        = ((0x2fU >= (0x0000003fU & vlSelfRef.tb_dx7_core__DOT__bit_i)) 
                           && (1U & (IData)((vlSelfRef.tb_dx7_core__DOT__tx_frame 
                                             >> (0x0000003fU 
                                                 & vlSelfRef.tb_dx7_core__DOT__bit_i)))));
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         90);
                    vlSelfRef.tb_dx7_core__DOT__spi_sck = 1U;
                    co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                         nullptr, 
                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                         91);
                    vlSelfRef.tb_dx7_core__DOT__bit_i 
                        = (vlSelfRef.tb_dx7_core__DOT__bit_i 
                           - (IData)(1U));
                }
                vlSelfRef.tb_dx7_core__DOT__spi_sck = 0U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     94);
                vlSelfRef.tb_dx7_core__DOT__spi_cs_n = 1U;
                co_await vlSelfRef.__VdlySched.delay(0x0000000000004e20ULL, 
                                                     nullptr, 
                                                     "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                     96);
            } else if ((0x52U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                __Vfunc_tb_dx7_core__DOT__dec_at__44__j = 2U;
                __Vfunc_tb_dx7_core__DOT__dec_at__44__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__dec_at__44__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v = 0U;
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__j;
                __Vfunc_tb_dx7_core__DOT__ch__45__j 
                    = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__45__pbase 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__45__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__45__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[7U];
                __VExpandSel_Lsb_17 = (0x000000ffU 
                                       & VL_MULS_III(32, (IData)(8U), 
                                                     (__Vfunc_tb_dx7_core__DOT__ch__45__pbase 
                                                      - __Vfunc_tb_dx7_core__DOT__ch__45__j)));
                __Vfunc_tb_dx7_core__DOT__ch__45__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__45__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__45__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__45__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_17) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__45__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__45__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__45__l
                                         [(__VExpandSel_Lsb_17 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__45__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__45__j))))));
                __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__45__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v 
                        = (((IData)(0x0000000aU) * vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v) 
                           + (vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                              - (IData)(0x00000030U)));
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__46__j 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__46__pbase 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__46__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44__l[7U];
                    __VExpandSel_Lsb_18 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__46__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__46__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__46__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__46__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__46__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__46__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_18) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__46__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__46__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__46__l
                                             [(__VExpandSel_Lsb_18 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__46__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__46__j))))));
                    __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__46__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__44____VlefCall_1__ch;
                }
                __Vfunc_tb_dx7_core__DOT__dec_at__44__Vfuncout 
                    = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v;
                vlSelfRef.tb_dx7_core__DOT__vdec = __Vfunc_tb_dx7_core__DOT__dec_at__44__Vfuncout;
                __Vtask_tb_dx7_core__DOT__wait_frames__47__n 
                    = vlSelfRef.tb_dx7_core__DOT__vdec;
                vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__n 
                    = __Vtask_tb_dx7_core__DOT__wait_frames__47__n;
                vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k, vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__n)) {
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         274);
                    while (((0x01ffU != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk)) 
                            | (0x3eU != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx)))) {
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             276);
                    }
                    vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k);
                }
            } else if ((0x57U == (IData)(vlSelfRef.tb_dx7_core__DOT__tok))) {
                __Vfunc_tb_dx7_core__DOT__ndig__48__j = 2U;
                __Vfunc_tb_dx7_core__DOT__ndig__48__pbase 
                    = vlSelfRef.tb_dx7_core__DOT__p0;
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[0U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[0U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[1U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[1U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[2U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[2U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[3U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[3U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[4U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[4U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[5U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[5U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[6U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[6U];
                __Vfunc_tb_dx7_core__DOT__ndig__48__l[7U] 
                    = vlSelfRef.tb_dx7_core__DOT__line[7U];
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__j;
                __Vfunc_tb_dx7_core__DOT__ch__49__j 
                    = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                __Vfunc_tb_dx7_core__DOT__ch__49__pbase 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__pbase;
                __Vfunc_tb_dx7_core__DOT__ch__49__l[0U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[0U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[1U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[1U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[2U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[2U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[3U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[3U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[4U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[4U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[5U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[5U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[6U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[6U];
                __Vfunc_tb_dx7_core__DOT__ch__49__l[7U] 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48__l[7U];
                __VExpandSel_Lsb_19 = (0x000000ffU 
                                       & VL_MULS_III(32, (IData)(8U), 
                                                     (__Vfunc_tb_dx7_core__DOT__ch__49__pbase 
                                                      - __Vfunc_tb_dx7_core__DOT__ch__49__j)));
                __Vfunc_tb_dx7_core__DOT__ch__49__Vfuncout 
                    = (0x000000ffU & (((0U == (0x0000001fU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__49__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__49__j))))
                                        ? 0U : (__Vfunc_tb_dx7_core__DOT__ch__49__l
                                                [(((IData)(7U) 
                                                   + __VExpandSel_Lsb_19) 
                                                  >> 5U)] 
                                                << 
                                                ((IData)(0x00000020U) 
                                                 - 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__49__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__49__j)))))) 
                                      | (__Vfunc_tb_dx7_core__DOT__ch__49__l
                                         [(__VExpandSel_Lsb_19 
                                           >> 5U)] 
                                         >> (0x0000001fU 
                                             & VL_MULS_III(32, (IData)(8U), 
                                                           (__Vfunc_tb_dx7_core__DOT__ch__49__pbase 
                                                            - __Vfunc_tb_dx7_core__DOT__ch__49__j))))));
                __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_0__ch 
                    = __Vfunc_tb_dx7_core__DOT__ch__49__Vfuncout;
                vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                    = __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_0__ch;
                while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c) 
                        & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c))) {
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k);
                    __Vfunc_tb_dx7_core__DOT__ch__50__j 
                        = vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__50__pbase 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__50__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48__l[7U];
                    __VExpandSel_Lsb_20 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__50__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__50__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__50__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__50__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__50__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__50__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_20) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__50__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__50__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__50__l
                                             [(__VExpandSel_Lsb_20 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__50__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__50__j))))));
                    __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_1__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__50__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__ndig__48____VlefCall_1__ch;
                }
                vlSelfRef.tb_dx7_core__DOT____VlemCall_9__ndig 
                    = (vlSelfRef.tb_dx7_core__DOT__ndig__Vstatic__k 
                       - __Vfunc_tb_dx7_core__DOT__ndig__48__j);
                if (VL_LTS_III(32, 0U, vlSelfRef.tb_dx7_core__DOT____VlemCall_9__ndig)) {
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__j = 2U;
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__pbase 
                        = vlSelfRef.tb_dx7_core__DOT__p0;
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[0U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[0U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[1U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[1U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[2U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[2U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[3U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[3U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[4U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[4U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[5U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[5U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[6U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[6U];
                    __Vfunc_tb_dx7_core__DOT__dec_at__51__l[7U] 
                        = vlSelfRef.tb_dx7_core__DOT__line[7U];
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__j;
                    __Vfunc_tb_dx7_core__DOT__ch__52__j 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                    __Vfunc_tb_dx7_core__DOT__ch__52__pbase 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__pbase;
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[0U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[0U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[1U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[1U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[2U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[2U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[3U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[3U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[4U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[4U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[5U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[5U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[6U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[6U];
                    __Vfunc_tb_dx7_core__DOT__ch__52__l[7U] 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[7U];
                    __VExpandSel_Lsb_21 = (0x000000ffU 
                                           & VL_MULS_III(32, (IData)(8U), 
                                                         (__Vfunc_tb_dx7_core__DOT__ch__52__pbase 
                                                          - __Vfunc_tb_dx7_core__DOT__ch__52__j)));
                    __Vfunc_tb_dx7_core__DOT__ch__52__Vfuncout 
                        = (0x000000ffU & (((0U == (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__52__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__52__j))))
                                            ? 0U : 
                                           (__Vfunc_tb_dx7_core__DOT__ch__52__l
                                            [(((IData)(7U) 
                                               + __VExpandSel_Lsb_21) 
                                              >> 5U)] 
                                            << ((IData)(0x00000020U) 
                                                - (0x0000001fU 
                                                   & VL_MULS_III(32, (IData)(8U), 
                                                                 (__Vfunc_tb_dx7_core__DOT__ch__52__pbase 
                                                                  - __Vfunc_tb_dx7_core__DOT__ch__52__j)))))) 
                                          | (__Vfunc_tb_dx7_core__DOT__ch__52__l
                                             [(__VExpandSel_Lsb_21 
                                               >> 5U)] 
                                             >> (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__52__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__52__j))))));
                    __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_0__ch 
                        = __Vfunc_tb_dx7_core__DOT__ch__52__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                        = __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_0__ch;
                    while (((0x00000030U <= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c) 
                            & (0x00000039U >= vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c))) {
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v 
                            = (((IData)(0x0000000aU) 
                                * vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v) 
                               + (vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                                  - (IData)(0x00000030U)));
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k 
                            = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k);
                        __Vfunc_tb_dx7_core__DOT__ch__53__j 
                            = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__k;
                        __Vfunc_tb_dx7_core__DOT__ch__53__pbase 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__pbase;
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[0U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[0U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[1U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[1U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[2U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[2U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[3U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[3U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[4U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[4U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[5U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[5U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[6U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[6U];
                        __Vfunc_tb_dx7_core__DOT__ch__53__l[7U] 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51__l[7U];
                        __VExpandSel_Lsb_22 = (0x000000ffU 
                                               & VL_MULS_III(32, (IData)(8U), 
                                                             (__Vfunc_tb_dx7_core__DOT__ch__53__pbase 
                                                              - __Vfunc_tb_dx7_core__DOT__ch__53__j)));
                        __Vfunc_tb_dx7_core__DOT__ch__53__Vfuncout 
                            = (0x000000ffU & (((0U 
                                                == 
                                                (0x0000001fU 
                                                 & VL_MULS_III(32, (IData)(8U), 
                                                               (__Vfunc_tb_dx7_core__DOT__ch__53__pbase 
                                                                - __Vfunc_tb_dx7_core__DOT__ch__53__j))))
                                                ? 0U
                                                : (__Vfunc_tb_dx7_core__DOT__ch__53__l
                                                   [
                                                   (((IData)(7U) 
                                                     + __VExpandSel_Lsb_22) 
                                                    >> 5U)] 
                                                   << 
                                                   ((IData)(0x00000020U) 
                                                    - 
                                                    (0x0000001fU 
                                                     & VL_MULS_III(32, (IData)(8U), 
                                                                   (__Vfunc_tb_dx7_core__DOT__ch__53__pbase 
                                                                    - __Vfunc_tb_dx7_core__DOT__ch__53__j)))))) 
                                              | (__Vfunc_tb_dx7_core__DOT__ch__53__l
                                                 [(__VExpandSel_Lsb_22 
                                                   >> 5U)] 
                                                 >> 
                                                 (0x0000001fU 
                                                  & VL_MULS_III(32, (IData)(8U), 
                                                                (__Vfunc_tb_dx7_core__DOT__ch__53__pbase 
                                                                 - __Vfunc_tb_dx7_core__DOT__ch__53__j))))));
                        __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_1__ch 
                            = __Vfunc_tb_dx7_core__DOT__ch__53__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__c 
                            = __Vfunc_tb_dx7_core__DOT__dec_at__51____VlefCall_1__ch;
                    }
                    vlSelfRef.tb_dx7_core__DOT____VlemCall_10__dec_at 
                        = vlSelfRef.tb_dx7_core__DOT__dec_at__Vstatic__v;
                    __Vtask_tb_dx7_core__DOT__wait_until_samples__54__target 
                        = (vlSelfRef.tb_dx7_core__DOT__samples 
                           + vlSelfRef.tb_dx7_core__DOT____VlemCall_10__dec_at);
                    vlSelfRef.tb_dx7_core__DOT__wait_until_samples__Vstatic__target 
                        = __Vtask_tb_dx7_core__DOT__wait_until_samples__54__target;
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         293);
                    while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__samples, vlSelfRef.tb_dx7_core__DOT__wait_until_samples__Vstatic__target)) {
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             295);
                    }
                    __Vtask_tb_dx7_core__DOT__wait_frames__55__n = 1U;
                    vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__n 
                        = __Vtask_tb_dx7_core__DOT__wait_frames__55__n;
                } else {
                    __Vtask_tb_dx7_core__DOT__wait_until_samples__56__target 
                        = tb_dx7_core__DOT__expect_samples;
                    vlSelfRef.tb_dx7_core__DOT__wait_until_samples__Vstatic__target 
                        = __Vtask_tb_dx7_core__DOT__wait_until_samples__56__target;
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         293);
                    while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__samples, vlSelfRef.tb_dx7_core__DOT__wait_until_samples__Vstatic__target)) {
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             295);
                    }
                    __Vtask_tb_dx7_core__DOT__wait_frames__57__n = 1U;
                    vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__n 
                        = __Vtask_tb_dx7_core__DOT__wait_frames__57__n;
                }
                vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k = 0U;
                while (VL_LTS_III(32, vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k, vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__n)) {
                    Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                       "@(negedge tb_dx7_core.clk)");
                    co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                         nullptr, 
                                                                         "@(negedge tb_dx7_core.clk)", 
                                                                         "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                         274);
                    while (((0x01ffU != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk)) 
                            | (0x3eU != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx)))) {
                        Vtb_dx7_core___024root____VbeforeTrig_h6456382b__0(vlSelf, 
                                                                           "@(negedge tb_dx7_core.clk)");
                        co_await vlSelfRef.__VtrigSched_h6456382b__0.trigger(0U, 
                                                                             nullptr, 
                                                                             "@(negedge tb_dx7_core.clk)", 
                                                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                                                             276);
                    }
                    vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k 
                        = ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__wait_frames__Vstatic__k);
                }
            }
        }
    }
    VL_FCLOSE_I(tb_dx7_core__DOT__vfd); VL_FCLOSE_I(vlSelfRef.tb_dx7_core__DOT__afd); VL_FWRITEF_NX(tb_dx7_core__DOT__mfd,"base_frame %0d\ncommits",1
                                                                                , '~',32,tb_dx7_core__DOT__base_frame);
    tb_dx7_core__DOT__fi = 0U;
    while (VL_LTS_III(32, tb_dx7_core__DOT__fi, tb_dx7_core__DOT__commit_n)) {
        VL_FWRITEF_NX(tb_dx7_core__DOT__mfd," %0d",1
                      , '~',32,vlSelfRef.tb_dx7_core__DOT__commit_list
                      [(0x0000000fU & tb_dx7_core__DOT__fi)]);
        tb_dx7_core__DOT__fi = ((IData)(1U) + tb_dx7_core__DOT__fi);
    }
    VL_FWRITEF_NX(tb_dx7_core__DOT__mfd,"\noverrun %0d\noverflow %0d\nframe %0d\nsamples %0d\nlive %0d\nstate_obs %0d\n",6
                  , '#',1,vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overrun
                  , '#',1,(IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow)
                  , '#',16,vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr
                  , '~',32,vlSelfRef.tb_dx7_core__DOT__samples
                  , '#',16,(IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live)
                  , '#',1,((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__obs_q) 
                           ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__obs_q)));
    VL_FCLOSE_I(tb_dx7_core__DOT__mfd); VL_WRITEF_NX("DONE tb_dx7_core base=%0d frames=%0d samples=%0d overrun=%0d overflow=%0d\n",5
                                                     , '~',32,tb_dx7_core__DOT__base_frame
                                                     , '#',16,(IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr)
                                                     , '~',32,vlSelfRef.tb_dx7_core__DOT__samples
                                                     , '#',1,(IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overrun)
                                                     , '#',1,vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow);
    VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 267, "");
    co_return;
}

VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__1(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    co_await vlSelfRef.__VdlySched.delay(0x0000b5e620f48000ULL, 
                                         nullptr, "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                         376);
    VL_WRITEF_NX("FAIL tb_dx7_core (watchdog timeout)\n",0);
    VL_FINISH_MT("/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 378, "");
    co_return;
}

VlCoroutine Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__2(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___eval_initial__TOP__Vtiming__2\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(0x0000000000001388ULL, 
                                             nullptr, 
                                             "/private/tmp/opencode/wt-h07/rtl/tb_dx7_core.v", 
                                             73);
        vlSelfRef.tb_dx7_core__DOT__clk = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__clk)));
    }
    co_return;
}

bool Vtb_dx7_core___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

bool Vtb_dx7_core___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtb_dx7_core___024root___nba_sequent__TOP__0(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___nba_sequent__TOP__0\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*7:0*/ tb_dx7_core__DOT__dut__DOT__u_router__DOT__row_index;
    tb_dx7_core__DOT__dut__DOT__u_router__DOT__row_index = 0;
    SData/*15:0*/ tb_dx7_core__DOT__dut__DOT__u_router__DOT__go_cur;
    tb_dx7_core__DOT__dut__DOT__u_router__DOT__go_cur = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__499__k;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__499__k = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__500__k;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__500__k = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__501__k;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__501__k = 0;
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__502__k;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__502__k = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__lr_d;
    __Vdly__tb_dx7_core__DOT__lr_d = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__samples;
    __Vdly__tb_dx7_core__DOT__samples = 0;
    IData/*31:0*/ __VdlyMask__tb_dx7_core__DOT__samples;
    __VdlyMask__tb_dx7_core__DOT__samples = 0;
    SData/*8:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk;
    __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk = 0;
    CData/*5:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx;
    __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx = 0;
    SData/*15:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr;
    __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr = 0;
    CData/*5:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt;
    __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt = 0;
    CData/*2:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp;
    __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp = 0;
    CData/*2:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp;
    __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp = 0;
    CData/*7:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp;
    __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp = 0;
    CData/*1:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div;
    __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q = 0;
    CData/*7:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt;
    __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt = 0;
    CData/*5:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__falls;
    __Vdly__tb_dx7_core__DOT__dut__DOT__falls = 0;
    IData/*23:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh;
    __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh = 0;
    CData/*5:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp;
    __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp = 0;
    CData/*4:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q = 0;
    CData/*4:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 = 0;
    IData/*16:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain = 0;
    SData/*9:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q = 0;
    SData/*15:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w = 0;
    SData/*15:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 = 0;
    IData/*31:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 = 0;
    CData/*2:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q = 0;
    CData/*6:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0;
    CData/*1:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q = 0;
    CData/*1:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q = 0;
    CData/*0:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q = 0;
    CData/*4:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q = 0;
    SData/*14:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q = 0;
    SData/*14:0*/ __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q = 0;
    QData/*48:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__skid__v0;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__skid__v0 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__skid__v0;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__skid__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__skid__v0;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__skid__v0 = 0;
    QData/*36:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__evq__v0;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__evq__v0 = 0;
    CData/*7:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__evq__v0;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__evq__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__evq__v0;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__evq__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v0;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v1;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v1 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 = 0;
    IData/*27:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 = 0;
    SData/*15:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v64;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v64 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v65;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v65 = 0;
    SData/*15:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 = 0;
    CData/*5:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 = 0;
    CData/*5:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 = 0;
    CData/*5:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 = 0;
    CData/*5:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 = 0;
    CData/*5:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 = 0;
    SData/*15:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8 = 0;
    SData/*15:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 0;
    SData/*15:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 0;
    CData/*0:0*/ __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10 = 0;
    IData/*31:0*/ __Vilp1;
    IData/*31:0*/ __Vilp2;
    IData/*31:0*/ __Vilp3;
    IData/*31:0*/ __Vilp4;
    CData/*31:0*/ __Vdeeptemp_ha0d6b95c__0;
    // Body
    __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div = vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div;
    __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q = vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr;
    __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk;
    __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt;
    __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp = vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_st 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_st;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_y 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_y;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[0U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[1U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_t1[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_t1[2U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[0U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[1U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_arg[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_arg[2U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_k 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_k;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_T 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_T;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_res 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_res;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[0U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[0U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[1U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[1U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_zq[2U] 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_zq[2U];
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_h 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_h;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp = vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__evq__v0 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt = vlSelfRef.tb_dx7_core__DOT__dut__DOT__bit_cnt;
    __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp = vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_wp;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__skid__v0 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__sustain 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__refresh_pending;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__in_sel 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__bend_pb 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__bend_pb;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__master_tune 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__master_tune;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globA 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globC 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegr 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegl 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_start;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pm_nobase_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__pm_nobase_w;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pbase_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__pbase_w;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__amd_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__amd_w;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_peg 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_peg;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_level 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_level;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_li 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_li;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fl 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fl;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fsh 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fsh;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fa 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fa;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fq_sel 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_sel;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_alg__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_vel 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_vel;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globB 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v97 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v98 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v99 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v100 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v101 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v36 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v17 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v18 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v19 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v20 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v21 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v22 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v23 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v24 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v25 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v26 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v27 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v28 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v29 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v30 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v31 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v32 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v34 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v35 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v36 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v37 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v38 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v39 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v40 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v41 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v42 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v43 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v44 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v45 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v46 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v47 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v48 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v49 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v50 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v51 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v52 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v53 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v54 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v55 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v56 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v57 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v58 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v59 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v60 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v61 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v62 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v63 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v64 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v65 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v66 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v67 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v68 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v69 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v70 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v71 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v72 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v73 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v74 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v75 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v76 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v77 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v78 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v79 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1217 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v16 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__seq_ctr;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_seq__v16 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_commit;
    __Vdly__tb_dx7_core__DOT__dut__DOT__falls = vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls;
    __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh = vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_sh;
    __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp = vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp;
    __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q = vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt = vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v1 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v2 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v3 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v4 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v5 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v6 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v7 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v8 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v9 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v10 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v11 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v12 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v13 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v14 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v15 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v16 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v17 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v18 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v19 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v20 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v21 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v22 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v23 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v24 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v25 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v26 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v27 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v28 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v29 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v30 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v31 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v32 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v33 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v199 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v206 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v193 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v97 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v98 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v99 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v100 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v101 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v102 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v103 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v104 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v105 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v106 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v107 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v108 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v109 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v110 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v111 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v112 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v0 = 0U;
    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v64 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v65 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 0U;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 0U;
    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q;
    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q 
        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v0 = 0U;
    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v1 = 0U;
    __Vdly__tb_dx7_core__DOT__lr_d = vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q;
    if (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q) 
         != (IData)(vlSelfRef.tb_dx7_core__DOT__lr_d))) {
        if ((1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q)))) {
            __Vdly__tb_dx7_core__DOT__samples = ((IData)(1U) 
                                                 + vlSelfRef.tb_dx7_core__DOT__samples);
            __VdlyMask__tb_dx7_core__DOT__samples = 0xffffffffU;
        }
    }
    if (VL_UNLIKELY((vlSelfRef.tb_dx7_core__DOT__dut__DOT__mix_load))) {
        vlSelfRef.tb_dx7_core__DOT__tap_q = (0x007fffffU 
                                             & (IData)(
                                                       ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mix_load)
                                                         ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__mixbuf
                                                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__out_sel]
                                                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp]
                                                         : 0ULL)));
        vlSelfRef.tb_dx7_core__DOT__c0 = (0x000000ffU 
                                          & vlSelfRef.tb_dx7_core__DOT__tap_q);
        vlSelfRef.tb_dx7_core__DOT__c1 = (0x000000ffU 
                                          & (vlSelfRef.tb_dx7_core__DOT__tap_q 
                                             >> 8U));
        vlSelfRef.tb_dx7_core__DOT__c2 = ((0x000000c0U 
                                           & ((- (IData)(
                                                         (1U 
                                                          & (vlSelfRef.tb_dx7_core__DOT__tap_q 
                                                             >> 0x15U)))) 
                                              << 6U)) 
                                          | (0x0000003fU 
                                             & (vlSelfRef.tb_dx7_core__DOT__tap_q 
                                                >> 0x10U)));
        vlSelfRef.tb_dx7_core__DOT__c3 = (0x000000ffU 
                                          & (- (IData)(
                                                       (1U 
                                                        & (vlSelfRef.tb_dx7_core__DOT__tap_q 
                                                           >> 0x15U)))));
        VL_FWRITEF_NX(vlSelfRef.tb_dx7_core__DOT__afd,"%c%c%c%c",4
                      , '#',8,vlSelfRef.tb_dx7_core__DOT__c0
                      , '#',8,(IData)(vlSelfRef.tb_dx7_core__DOT__c1)
                      , '#',8,vlSelfRef.tb_dx7_core__DOT__c2
                      , '#',8,(IData)(vlSelfRef.tb_dx7_core__DOT__c3));
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__mosi_s2 = vlSelfRef.tb_dx7_core__DOT__dut__DOT__mosi_s1;
    vlSelfRef.tb_dx7_core__DOT__lr_d = __Vdly__tb_dx7_core__DOT__lr_d;
    vlSelfRef.tb_dx7_core__DOT__samples = ((__Vdly__tb_dx7_core__DOT__samples 
                                            & __VdlyMask__tb_dx7_core__DOT__samples) 
                                           | (vlSelfRef.tb_dx7_core__DOT__samples 
                                              & (~ __VdlyMask__tb_dx7_core__DOT__samples)));
    __VdlyMask__tb_dx7_core__DOT__samples = 0U;
    __Vdeeptemp_ha0d6b95c__0 = (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note) 
                                ^ (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__seq_ctr) 
                                   ^ (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_phase) 
                                      ^ (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_delta) 
                                         ^ (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dstate) 
                                            ^ (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc1) 
                                               ^ (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc2) 
                                                  ^ 
                                                  (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand) 
                                                   ^ 
                                                   (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_val_q) 
                                                    ^ 
                                                    (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q) 
                                                     ^ 
                                                     (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bend_pb) 
                                                      ^ 
                                                      (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__master_tune) 
                                                       ^ 
                                                       (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_pm) 
                                                        ^ 
                                                        (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_am) 
                                                         ^ 
                                                         (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__eg_mod) 
                                                          ^ 
                                                          (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mixbuf_obs0) 
                                                           ^ 
                                                           (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mixbuf_obs1) 
                                                            ^ 
                                                            (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globA) 
                                                             ^ 
                                                             (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globB) 
                                                              ^ 
                                                              (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globC) 
                                                               ^ 
                                                               (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_pegr) 
                                                                ^ 
                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_pegl) 
                                                                 ^ 
                                                                 (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA) 
                                                                  ^ 
                                                                  (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB) 
                                                                   ^ 
                                                                   (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC) 
                                                                    ^ 
                                                                    (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr) 
                                                                     ^ 
                                                                     (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl) 
                                                                      ^ 
                                                                      (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state) 
                                                                       ^ 
                                                                       (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn) 
                                                                        ^ 
                                                                        (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                                                         ^ 
                                                                         (VL_REDXOR_2(vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind) 
                                                                          ^ 
                                                                          (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt) 
                                                                           ^ 
                                                                           (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan) 
                                                                            ^ 
                                                                            (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note) 
                                                                             ^ 
                                                                             (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_vel) 
                                                                              ^ 
                                                                              (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_ctrl) 
                                                                               ^ 
                                                                               (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_val) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__pm_nobase_w) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__pbase_w) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__amd_w) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_peg) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmdep) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmsens) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_amdep) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_level) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_li) 
                                                                                ^ 
                                                                                (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fa) 
                                                                                ^ 
                                                                                (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fl) 
                                                                                ^ 
                                                                                (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fsh) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_sel) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_algorithm) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_feedback) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb0) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb1) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq) 
                                                                                ^ 
                                                                                (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level) 
                                                                                ^ 
                                                                                (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mix_j) 
                                                                                ^ 
                                                                                (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__rb_cnt) 
                                                                                ^ 
                                                                                (VL_REDXOR_2(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_sh) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[0U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[1U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[2U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[3U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[4U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[5U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[6U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[7U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[8U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[9U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[10U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[11U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[12U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[13U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[14U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[15U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[0U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[1U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[2U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[3U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[4U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[5U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[6U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[7U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[8U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[9U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[10U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[11U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[12U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[13U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[14U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_seq[15U]) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[15U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[15U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[15U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[15U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 0x0000000dU)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 0x0000000eU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000fU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 0x0000000eU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000fU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[15U]))))) 
                                                                                ^ 
                                                                                ((((((((VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[14U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[14U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[14U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[14U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 0x0000000cU)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 0x0000000dU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000eU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 0x0000000dU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000eU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[14U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[13U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[13U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[13U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[13U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 0x0000000bU)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 0x0000000cU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000dU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 0x0000000cU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000dU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[13U])))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[12U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[12U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[12U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[12U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 0x0000000aU)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 0x0000000bU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000cU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 0x0000000bU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000cU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[12U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[11U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[11U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[11U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[11U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 9U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 0x0000000aU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000bU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 0x0000000aU)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000bU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[11U]))))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[10U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[10U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[10U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[10U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 8U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 9U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 0x0000000aU)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 9U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 0x0000000aU)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[10U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[9U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[9U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[9U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[9U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 7U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 8U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 9U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 8U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 9U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[9U]))))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[8U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[8U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[8U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[8U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 6U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 7U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 8U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 7U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 8U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[8U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[7U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[7U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[7U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[7U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 5U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 6U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 7U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 6U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 7U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[7U]))))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[6U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[6U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[6U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[6U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 4U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 5U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 6U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 5U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 6U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[6U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[5U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[5U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[5U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[5U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 3U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 4U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 5U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 4U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 5U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[5U]))))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[4U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[4U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[4U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[4U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 3U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 4U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 3U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 4U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[4U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[3U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[3U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[3U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[3U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                >> 1U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 2U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 3U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 2U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 3U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[3U]))))))) 
                                                                                ^ 
                                                                                (VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[2U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[2U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[2U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[2U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 2U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 2U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[2U]))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[1U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[1U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[1U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[1U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                << 1U)) 
                                                                                | ((2U 
                                                                                & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                                >> 1U)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                                >> 1U)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[1U]))))))) 
                                                                                ^ 
                                                                                VL_REDXOR_32(
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl[0U]) 
                                                                                ^ 
                                                                                ((0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegtgt[0U]) 
                                                                                ^ 
                                                                                (((0x0000f000U 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi[0U] 
                                                                                << 0x0000000cU)) 
                                                                                | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[0U]) 
                                                                                << 5U) 
                                                                                | ((((4U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed)))))) 
                                                                                ^ 
                                                                                (0x0000000fU 
                                                                                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peginc[0U]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__obs_q = (1U 
                                                   & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff2)) 
                                                      & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overrun) 
                                                         ^ 
                                                         ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow) 
                                                          ^ 
                                                          ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_fresh) 
                                                           ^ 
                                                           ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain) 
                                                            ^ 
                                                            ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__refresh_pending) 
                                                             ^ 
                                                             ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel) 
                                                              ^ 
                                                              ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__out_sel) 
                                                               ^ 
                                                               ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_commit) 
                                                                ^ 
                                                                ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_cfg) 
                                                                 ^ 
                                                                 ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_fb) 
                                                                  ^ 
                                                                  ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op) 
                                                                   ^ 
                                                                   ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_start) 
                                                                    ^ 
                                                                    ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_q) 
                                                                     ^ 
                                                                     ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q) 
                                                                      ^ 
                                                                      (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr) 
                                                                       ^ 
                                                                       (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk) 
                                                                        ^ 
                                                                        (VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx) 
                                                                         ^ 
                                                                         (VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt) 
                                                                          ^ 
                                                                          (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                                                           ^ 
                                                                           (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                                                            ^ 
                                                                            (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                                             ^ 
                                                                             (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                                                              ^ 
                                                                              (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed) 
                                                                               ^ 
                                                                               (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                                                                                ^ 
                                                                                (VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_silnow) 
                                                                                ^ __Vdeeptemp_ha0d6b95c__0)))))))))))))))))))))))))));
    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp = 0U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v0 = 1U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v0 = 1U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v0 = 1U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v0 = 1U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v0 = 1U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v0 = 1U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__falls = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q = 0x10U;
        __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v0 = 1U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg1_w = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg2_w = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q = 0x10U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q = 0U;
        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__obs_q = 0U;
        __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v1 = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shift_in = 0ULL;
        __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg = 0ULL;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg_v = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__mix_j = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_fresh = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow = 0U;
    } else {
        if ((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div))) {
            __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_q)));
            __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div = 0U;
        } else {
            __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div 
                = (3U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div)));
        }
        if ((0x01ffU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk))) {
            if ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx))) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr 
                    = (0x0000ffffU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr)));
                __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx = 0U;
            } else {
                __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx 
                    = (0x0000003fU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx)));
            }
            __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk = 0U;
        } else {
            __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk 
                = (0x000001ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk)));
        }
        if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__wr_wr) {
            if ((8U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt 
                    = (0x0000000fU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt)));
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__skid__v0 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__skid__v0 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_wp;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__skid__v0 = 1U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_wp)));
            }
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_fresh = 0U;
            if ((8U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt))) {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow = 1U;
            }
        }
        if ((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt))) {
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__skid_cnt 
                = (0x0000000fU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt) 
                                  - (IData)(1U)));
            __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp 
                = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp)));
            if (vlSelfRef.__VdfgRegularize_hebeb780c_0_1) {
                if ((0x42U != (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0))) {
                    if (((0x10U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x15U >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6 = 1U;
                        }
                    }
                    if (((0x18U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x1dU >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6 = 1U;
                        }
                    }
                    if (((0x20U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x25U >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v6 = 1U;
                        }
                    }
                    if (((0x28U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x2dU >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6 = 1U;
                        }
                    }
                    if (((0x30U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x35U >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6 = 1U;
                        }
                    }
                    if (((0x38U <= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)) 
                         & (0x3dU >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 
                                = vlSelfRef.__VdfgRegularize_hebeb780c_0_2;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 
                                = (7U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6 = 1U;
                        }
                    }
                }
                if ((0x42U == (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit = 1U;
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_1)))) {
                if ((0x00a8U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt))) {
                    if ((0xa7U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp))) {
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__evq__v0 
                            = (((QData)((IData)((0x0000001fU 
                                                 & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))) 
                                << 0x00000020U) | (QData)((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_2)));
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__evq__v0 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp;
                        __VdlySet__tb_dx7_core__DOT__dut__DOT__evq__v0 = 1U;
                    }
                    __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp 
                        = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp))
                            ? 0U : (0x000000ffU & ((IData)(1U) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                        = (0x000001ffU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt)));
                }
            }
        }
        if ((0xffU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt))) {
            if ((1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q)))) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q = 1U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__falls = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh 
                    = (0x00fffffcU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mixbuf
                                              [vlSelfRef.tb_dx7_core__DOT__dut__DOT__out_sel]
                                              [vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp]) 
                                      << 2U));
                __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp 
                    = ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp))
                        ? 0U : (0x0000003fU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp))));
            } else {
                __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__falls = 0U;
            }
        } else {
            __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt 
                = (0x000000ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt)));
            if ((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div))) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__falls 
                    = (0x0000003fU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls)));
                if (((1U <= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls)) 
                     & (0x18U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls)))) {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh 
                        = (0x00fffffeU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_sh 
                                          << 1U));
                }
            }
        }
        if ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_cfg) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_algorithm;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q 
                    = ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_feedback))
                        ? 0x00000010U : (0x0000001fU 
                                         & ((IData)(8U) 
                                            - (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_feedback))));
            }
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_fb) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb0;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb1;
            }
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op) {
                __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__499__k 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx;
                __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__500__k 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx;
                __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__501__k 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx;
                __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__502__k 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_2__pm 
                    = __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__499__k;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_3__pm 
                    = __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__500__k;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_4__pm 
                    = __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__501__k;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_5__pm 
                    = __Vfunc_tb_dx7_core__DOT__dut__DOT__u_router__DOT__pm__502__k;
                if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_2__pm))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_2__pm;
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6 = 1U;
                }
                if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_3__pm))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_3__pm;
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6 = 1U;
                }
                if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_4__pm))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_4__pm;
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6 = 1U;
                }
                if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_5__pm))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT____VlemCall_5__pm;
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6 = 1U;
                }
            }
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_start) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = 1U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q = 0U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v64 = 1U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 1U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v65 = 1U;
            }
        } else if ((1U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))) {
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain2;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7 = 1U;
            }
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q = 0U;
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gate_pass) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q 
                    = (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                        >> 2U) & ((0U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))
                                   ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q)
                                   : ((1U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))
                                       ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q)
                                       : (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q))));
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q 
                    = (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags));
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q 
                    = (3U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                             >> 4U));
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q 
                    = ((0U != (3U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                                     >> 4U))) & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_live));
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain1;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q 
                    = (0x000003ffU & VL_SHIFTRS_III(17,17,32, 
                                                    (0x0001ffffU 
                                                     & ((IData)(0x00000020U) 
                                                        + 
                                                        ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain2) 
                                                         - (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain1)))), 6U));
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
                    = ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))
                        ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q
                       [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q]
                        : 0U);
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 2U;
                if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_dispatch) {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 1U;
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q 
                        = (0x0000001fU & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_sel) 
                                           | ((0x1fU 
                                               == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q)) 
                                              | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_sel)))
                                           ? ((0x0eU 
                                               <= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q))
                                               ? 0x10U
                                               : ((IData)(2U) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q)))
                                           : (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q)));
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q;
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q;
                    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_sel) {
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 1U;
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_sel;
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1;
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U];
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg1_w 
                            = (0x0000ffffU & (VL_EXTENDS_II(16,15, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1)) 
                                              - VL_EXTENDS_II(16,15, 
                                                              (0x00007fffU 
                                                               & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1)))));
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg2;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg2_w 
                            = (0x0000ffffU & (VL_EXTENDS_II(16,15, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2)) 
                                              - VL_EXTENDS_II(16,15, 
                                                              (0x00007fffU 
                                                               & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg2)))));
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2;
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[2U];
                    } else {
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0U;
                        if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_sel) {
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 1U;
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1;
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U];
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg1_w 
                                = (0x0000ffffU & (VL_EXTENDS_II(16,15, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1)) 
                                                  - 
                                                  VL_EXTENDS_II(16,15, 
                                                                (0x00007fffU 
                                                                 & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1)))));
                        } else {
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0U;
                            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1;
                        }
                    }
                } else {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 0U;
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0U;
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0U;
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q = 0x10U;
                }
                if ((0U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))) {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = 1U;
                } else if ((1U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))) {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = 1U;
                } else {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = 1U;
                }
            } else {
                if ((1U & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                              >> 2U)))) {
                    if ((0U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))) {
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q = 0U;
                    } else if ((1U == (3U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))) {
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q = 0U;
                    } else {
                        __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q = 0U;
                    }
                }
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 5U;
            }
        } else if ((2U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
                   + ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))
                       ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q
                      [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q]
                       : 0U));
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w;
                if ((1U & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q)) 
                           & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q))))) {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
                }
            }
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv;
            if (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q) 
                 | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q))) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 3U;
            } else {
                if ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 
                        = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q)
                            ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y 
                               + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q
                               [(0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))])
                            : vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y);
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 
                        = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q));
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128 = 1U;
                } else if ((1U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 
                        = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q)
                            ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y 
                               + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q
                               [(0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))])
                            : vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y);
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 
                        = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q));
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64 = 1U;
                } else {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 
                        = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q)
                            ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y 
                               + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q
                               [(0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))])
                            : vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y);
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 
                        = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q));
                    __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64 = 1U;
                }
                if ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 5U;
                } else {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q 
                        = (0x0000007fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q)));
                }
            }
        } else if ((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
                   + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[1U]);
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv1;
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 4U;
            } else {
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 
                    = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q));
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129 = 1U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
                if ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 5U;
                } else {
                    __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q 
                        = (0x0000007fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q)));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 2U;
                }
            }
        } else if ((4U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
                   + vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[2U]);
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv2;
            __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 
                = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q));
            __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130 = 1U;
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_eval_y;
            if ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 5U;
            } else {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q 
                    = (0x0000007fU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q)));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 2U;
            }
        } else if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
            if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))) {
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 
                    = (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))
                         ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q]
                         : 0U) + (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))
                                    ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q
                                   [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q]
                                    : 0U) << 6U));
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7 = 1U;
            }
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q) {
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w;
                __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w;
            }
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q) {
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 1U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8 
                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U] 
                       + (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[1U] 
                          << 6U));
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 2U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 
                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[2U] 
                       + (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[2U] 
                          << 6U));
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 2U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9 = 1U;
            } else if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q) {
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 1U;
                __VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10 
                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U] 
                       + (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[1U] 
                          << 6U));
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10 = 1U;
            }
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q 
                = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q) 
                         + ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q)
                             ? 3U : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q)
                                      ? 2U : 1U))));
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q = 0U;
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q 
                = ((6U <= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q) 
                                 + ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q)
                                     ? 3U : ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q)
                                              ? 2U : 1U)))))
                    ? 6U : 1U);
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q = 0U;
            __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q = 0U;
        } else {
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q = 0U;
        }
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__obs_q 
            = (1U & (((((((((((((((((((((((((((((((
                                                   (((((((((((((((((((((((VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q) 
                                                                          ^ 
                                                                          VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q)) 
                                                                         ^ 
                                                                         VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[0U])) 
                                                                        ^ 
                                                                        VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U])) 
                                                                       ^ 
                                                                       VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[2U])) 
                                                                      ^ 
                                                                      VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[3U])) 
                                                                     ^ 
                                                                     VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[4U])) 
                                                                    ^ 
                                                                    VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[5U])) 
                                                                   ^ 
                                                                   VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[0U])) 
                                                                  ^ 
                                                                  VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[1U])) 
                                                                 ^ 
                                                                 VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[2U])) 
                                                                ^ 
                                                                VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[3U])) 
                                                               ^ 
                                                               VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[4U])) 
                                                              ^ 
                                                              VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[5U])) 
                                                             ^ 
                                                             VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[0U])) 
                                                            ^ 
                                                            VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[1U])) 
                                                           ^ 
                                                           VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[2U])) 
                                                          ^ 
                                                          VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[3U])) 
                                                         ^ 
                                                         VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[4U])) 
                                                        ^ 
                                                        VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[5U])) 
                                                       ^ 
                                                       VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[0U])) 
                                                      ^ 
                                                      VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[1U])) 
                                                     ^ 
                                                     VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[2U])) 
                                                    ^ 
                                                    VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[3U])) 
                                                   ^ 
                                                   VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[4U])) 
                                                  ^ 
                                                  VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[5U])) 
                                                 ^ 
                                                 VL_REDXOR_32(
                                                              (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[0U] 
                                                               ^ 
                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[1U] 
                                                                ^ 
                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[2U] 
                                                                 ^ 
                                                                 (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[3U] 
                                                                  ^ 
                                                                  (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[4U] 
                                                                   ^ 
                                                                   (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[5U] 
                                                                    ^ 
                                                                    (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[6U] 
                                                                     ^ 
                                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[7U] 
                                                                      ^ 
                                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[8U] 
                                                                       ^ 
                                                                       (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[9U] 
                                                                        ^ 
                                                                        (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[10U] 
                                                                         ^ 
                                                                         (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[11U] 
                                                                          ^ 
                                                                          (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[12U] 
                                                                           ^ 
                                                                           (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[13U] 
                                                                            ^ 
                                                                            (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[14U] 
                                                                             ^ 
                                                                             (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[15U] 
                                                                              ^ 
                                                                              (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[16U] 
                                                                               ^ 
                                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[17U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[18U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[19U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[20U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[21U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[22U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[23U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[24U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[25U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[26U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[27U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[28U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[29U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[30U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[31U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[32U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[33U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[34U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[35U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[36U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[37U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[38U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[39U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[40U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[41U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[42U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[43U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[44U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[45U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[46U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[47U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[48U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[49U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[50U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[51U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[52U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[53U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[54U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[55U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[56U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[57U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[58U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[59U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[60U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[61U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[62U] 
                                                                                ^ vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[63U]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) 
                                                ^ VL_REDXOR_32(
                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[0U] 
                                                                ^ 
                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[1U] 
                                                                 ^ 
                                                                 (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[2U] 
                                                                  ^ 
                                                                  (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[3U] 
                                                                   ^ 
                                                                   (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[4U] 
                                                                    ^ 
                                                                    (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[5U] 
                                                                     ^ 
                                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[6U] 
                                                                      ^ 
                                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[7U] 
                                                                       ^ 
                                                                       (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[8U] 
                                                                        ^ 
                                                                        (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[9U] 
                                                                         ^ 
                                                                         (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[10U] 
                                                                          ^ 
                                                                          (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[11U] 
                                                                           ^ 
                                                                           (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[12U] 
                                                                            ^ 
                                                                            (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[13U] 
                                                                             ^ 
                                                                             (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[14U] 
                                                                              ^ 
                                                                              (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[15U] 
                                                                               ^ 
                                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[16U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[17U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[18U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[19U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[20U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[21U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[22U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[23U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[24U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[25U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[26U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[27U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[28U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[29U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[30U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[31U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[32U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[33U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[34U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[35U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[36U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[37U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[38U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[39U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[40U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[41U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[42U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[43U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[44U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[45U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[46U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[47U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[48U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[49U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[50U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[51U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[52U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[53U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[54U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[55U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[56U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[57U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[58U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[59U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[60U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[61U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[62U] 
                                                                                ^ vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[63U]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) 
                                               ^ VL_REDXOR_32(
                                                              (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[0U] 
                                                               ^ 
                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[1U] 
                                                                ^ 
                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[2U] 
                                                                 ^ 
                                                                 (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[3U] 
                                                                  ^ 
                                                                  (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[4U] 
                                                                   ^ 
                                                                   (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[5U] 
                                                                    ^ 
                                                                    (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[6U] 
                                                                     ^ 
                                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[7U] 
                                                                      ^ 
                                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[8U] 
                                                                       ^ 
                                                                       (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[9U] 
                                                                        ^ 
                                                                        (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[10U] 
                                                                         ^ 
                                                                         (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[11U] 
                                                                          ^ 
                                                                          (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[12U] 
                                                                           ^ 
                                                                           (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[13U] 
                                                                            ^ 
                                                                            (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[14U] 
                                                                             ^ 
                                                                             (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[15U] 
                                                                              ^ 
                                                                              (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[16U] 
                                                                               ^ 
                                                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[17U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[18U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[19U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[20U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[21U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[22U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[23U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[24U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[25U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[26U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[27U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[28U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[29U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[30U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[31U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[32U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[33U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[34U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[35U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[36U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[37U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[38U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[39U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[40U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[41U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[42U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[43U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[44U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[45U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[46U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[47U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[48U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[49U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[50U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[51U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[52U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[53U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[54U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[55U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[56U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[57U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[58U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[59U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[60U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[61U] 
                                                                                ^ 
                                                                                (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[62U] 
                                                                                ^ vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[63U]))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) 
                                              ^ vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q) 
                                             ^ vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q) 
                                            ^ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q) 
                                               ^ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q) 
                                                  ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q)))) 
                                           ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w)) 
                                          ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w)) 
                                         ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph)) 
                                        ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1)) 
                                       ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2)) 
                                      ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain)) 
                                     ^ VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q)) 
                                    ^ VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w)) 
                                   ^ VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w)) 
                                  ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1)) 
                                 ^ VL_REDXOR_32(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2)) 
                                ^ VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q)) 
                               ^ VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q)) 
                              ^ VL_REDXOR_4(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q)) 
                             ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q)) 
                            ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q)) 
                           ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q)) 
                          ^ (VL_REDXOR_2(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q) 
                             ^ VL_REDXOR_2(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q))) 
                         ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q)) 
                        ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q)) 
                       ^ VL_REDXOR_8(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q)) 
                      ^ VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q)) 
                     ^ VL_REDXOR_16(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q)));
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg_v = 0U;
        if (((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_d)) 
             & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s2))) {
            __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt = 0U;
        } else if (((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s2)) 
                    & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_d)) 
                       & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_s2)))) {
            if ((0x2fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bit_cnt))) {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_captured;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg_v = 1U;
                __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt = 0U;
            } else {
                __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt 
                    = (0x0000003fU & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__bit_cnt)));
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__shift_in 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_captured;
            }
        }
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_rp = __Vdly__tb_dx7_core__DOT__dut__DOT__skid_rp;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_wp = __Vdly__tb_dx7_core__DOT__dut__DOT__evq_wp;
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__evq__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__evq__v0] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__evq__v0;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_wp = __Vdly__tb_dx7_core__DOT__dut__DOT__skid_wp;
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__skid__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__skid__v0] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__skid__v0;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc0_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__outbus_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__add_eff_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb0_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb1_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_ph;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph1;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__s_ph2;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y0_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y1_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid1;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__y_mid2;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__i_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_form_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus_form_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__eff_shift_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q;
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[0U] = 0U;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[5U] = 0U;
        __Vilp1 = 1U;
        while ((__Vilp1 <= 0x0000003fU)) {
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__Vilp1] = 0U;
            __Vilp1 = ((IData)(1U) + __Vilp1);
        }
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[5U] = 0U;
        __Vilp2 = 1U;
        while ((__Vilp2 <= 0x0000003fU)) {
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[__Vilp2] = 0U;
            __Vilp2 = ((IData)(1U) + __Vilp2);
        }
        __Vilp3 = 1U;
        while ((__Vilp3 <= 0x0000003fU)) {
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[__Vilp3] = 0U;
            __Vilp3 = ((IData)(1U) + __Vilp3);
        }
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[5U] = 0U;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__freq_q__v6;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v64) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[0U] = 0U;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v65) {
        __Vilp4 = 1U;
        while ((__Vilp4 <= 0x0000003fU)) {
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__Vilp4] = 0U;
            __Vilp4 = ((IData)(1U) + __Vilp4);
        }
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v128;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v129;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__nout_q__v130;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v6;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v7;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v8;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v9;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q__v6;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus1_q__v64;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__bus2_q__v64;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v6;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v7;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v8;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v9;
    }
    if (__VdlySet__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__phase_q__v10;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q__v10;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_q = __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_ctr 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__frame_ctr;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__falls = __Vdly__tb_dx7_core__DOT__dut__DOT__falls;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_sh = __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_sh;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__bclk_div 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__bclk_div;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__i2s_smp = __Vdly__tb_dx7_core__DOT__dut__DOT__i2s_smp;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__sample_clk;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__sample_idx;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt 
        = __Vdly__tb_dx7_core__DOT__dut__DOT__half_cnt;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q = __Vdly__tb_dx7_core__DOT__dut__DOT__lr_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__mosi_s1 = vlSelfRef.tb_dx7_core__DOT__spi_mosi;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv 
        = (0x0001ffffU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_gain 
                          + ((0x0001fc00U & ((- (IData)(
                                                        (1U 
                                                         & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q) 
                                                            >> 9U)))) 
                                             << 0x0000000aU)) 
                             | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__w_dgain_q))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv1 
        = (0x0000ffffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g1_w) 
                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg1_w)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain_adv2 
        = (0x0000ffffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__g2_w) 
                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__dg2_w)));
    tb_dx7_core__DOT__dut__DOT__u_router__DOT__row_index 
        = (0x000000ffU & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q) 
                           << 2U) + ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q) 
                                     + ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q) 
                                        << 1U))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og1 
        = (0x00007fffU & ((IData)(0x4000U) - (0x00003fffU 
                                              & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[1U] 
                                                 >> 0x0000000eU))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__og2 
        = (0x00007fffU & ((IData)(0x4000U) - (0x00003fffU 
                                              & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[2U] 
                                                 >> 0x0000000eU))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg1 
        = ((0U == (0x00007fffU & ((IData)(0x4000U) 
                                  - (0x00003fffU & 
                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[1U] 
                                      >> 0x0000000eU)))))
            ? 0x3fffU : (0x00007fffU & ((IData)(0x4000U) 
                                        - (0x00003fffU 
                                           & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[1U] 
                                              >> 0x0000000eU)))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__sg2 
        = ((0U == (0x00007fffU & ((IData)(0x4000U) 
                                  - (0x00003fffU & 
                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[2U] 
                                      >> 0x0000000eU)))))
            ? 0x3fffU : (0x00007fffU & ((IData)(0x4000U) 
                                        - (0x00003fffU 
                                           & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q[2U] 
                                              >> 0x0000000eU)))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain2 
        = (0x00007fffU & ((IData)(0x4000U) - (0x00003fffU 
                                              & ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__level_q
                                                  [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q] 
                                                  >> 0x0000000eU) 
                                                 & (- (IData)(
                                                              (5U 
                                                               >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))))))));
    tb_dx7_core__DOT__dut__DOT__u_router__DOT__go_cur 
        = ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))
            ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gaino_q
           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q]
            : 0U);
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__mix_load 
        = ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lr_q)) 
           & (0xffU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__half_cnt)));
    if ((1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff2)))) {
        if ((0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__skid_cnt))) {
            if ((1U & (~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_1)))) {
                if ((0x00a8U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt))) {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overflow = 1U;
                }
            }
        }
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__bit_cnt = __Vdly__tb_dx7_core__DOT__dut__DOT__bit_cnt;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags 
        = (((0x10U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q)) 
            & (((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q)) 
                | (5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q))) 
               & (0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__op_q))))
            ? 0xc4U : ((0xbfU >= (IData)(tb_dx7_core__DOT__dut__DOT__u_router__DOT__row_index))
                        ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_rom
                       [tb_dx7_core__DOT__dut__DOT__u_router__DOT__row_index]
                        : 0U));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain1 
        = ((0U == (IData)(tb_dx7_core__DOT__dut__DOT__u_router__DOT__go_cur))
            ? 0x00003fffU : (0x00007fffU & (IData)(tb_dx7_core__DOT__dut__DOT__u_router__DOT__go_cur)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_captured 
        = ((0x0000fffffffffffeULL & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__shift_in 
                                     << 1U)) | (QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__mosi_s2)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__wr_wr = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg_v) 
                                                   & (IData)(
                                                             (vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_reg 
                                                              >> 0x0000002fU)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_d = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s2;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_d = vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_s2;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_live 
        = ((0U != (3U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                         >> 4U))) & ((1U == (3U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                                                   >> 4U)))
                                      ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc1_q)
                                      : (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__hc2_q)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gate_pass 
        = ((0x3f9cU >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain1)) 
           | (0x3f9cU >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gain2)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_dispatch 
        = (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__gate_pass) 
            & (0xc0U == (0xc0U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags)))) 
           & ((0x10U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fbshift_q)) 
              & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__inbus_live)) 
                 | (0U == (3U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__flags) 
                                 >> 4U))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s2 = vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s1;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_s2 = vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_s1;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_sel 
        = ((3U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q)) 
           & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_dispatch));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser2_sel 
        = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__fb_dispatch) 
           & ((~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__ser3_sel)) 
              & (5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__alg_q))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cs_s1 = vlSelfRef.tb_dx7_core__DOT__spi_cs_n;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sck_s1 = vlSelfRef.tb_dx7_core__DOT__spi_sck;
}

extern const VlWide<8>/*255:0*/ Vtb_dx7_core__ConstPool__CONST_h831d05a1_0;
extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0;
extern const VlWide<128>/*4095:0*/ Vtb_dx7_core__ConstPool__CONST_h77b89011_0;

void Vtb_dx7_core___024root___nba_sequent__TOP__1(Vtb_dx7_core___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_dx7_core___024root___nba_sequent__TOP__1\n"); );
    Vtb_dx7_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<VlUnpacked<IData/*31:0*/, 6>, 16> tb_dx7_core__DOT__dut__DOT__n_dd;
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 6; ++__Vi1) {
            tb_dx7_core__DOT__dut__DOT__n_dd[__Vi0][__Vi1] = 0;
        }
    }
    CData/*2:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__pmsens_tab__447__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__pmsens_tab__447__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i = 0;
    CData/*1:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i = 0;
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s);
    IData/*31:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__Vfuncout = 0;
    VlWide<5>/*132:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s;
    VL_ZERO_W(133, __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s);
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__456__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__456__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__458__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__458__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__462__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__462__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__464__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__464__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__468__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__468__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__470__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__470__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__474__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__474__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__476__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__476__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__480__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__480__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__482__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__482__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__484__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__484__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__486__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__486__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__490__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__490__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__492__lvl;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__492__lvl = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__i = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494__r;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494__r = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494____VlefCall_0__peg_rate_lut;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494____VlefCall_0__peg_rate_lut = 0;
    CData/*7:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__Vfuncout;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__i;
    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__i = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_li__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_li__v96;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_li__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v0;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v1;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v1 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v2;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v2 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v3;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v3 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v4;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v4 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v5;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v5 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v6 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v7;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v7 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v8;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v8 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v9;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v9 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v10;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v10 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v11;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v11 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_bp__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_bp__v96;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_bp__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 0;
    CData/*0:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_opmode__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 0;
    CData/*2:0*/ __VdlyLsb__tb_dx7_core__DOT__dut__DOT__n_opmode__v16;
    __VdlyLsb__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_opmode__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 0;
    CData/*4:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_alg__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_alg__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_alg__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_alg__v16 = 0;
    CData/*4:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fbs__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fbs__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 = 0;
    CData/*7:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16 = 0;
    CData/*7:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_amdep__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_amdep__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_amdep__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_amdep__v16 = 0;
    CData/*7:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v96 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v97;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v97 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v97;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v97 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v98;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v98 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v98;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v98 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v99;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v99 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v99;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v99 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v100;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v100 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v100;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v100 = 0;
    IData/*24:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v101;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v101 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v101;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v101 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v96 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v97;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v97 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v97;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v97 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v98;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v98 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v98;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v98 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v99;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v99 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v99;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v99 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v96 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v97;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v97 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v97;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v97 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v98;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v98 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v98;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v98 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v99;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v99 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v99;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v99 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_ph__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_ph__v96;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb0__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb1__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_fq__v96;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_fq__v96;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_fq__v96;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 0;
    CData/*2:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegix__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v12;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v12 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v13;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v13 = 0;
    VlWide<5>/*132:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6;
    VL_ZERO_W(133, __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6);
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v14;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v14 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v15;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v15 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v16 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v17;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v17 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v18;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v18 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v19;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v19 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v33;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v33 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v20;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v20 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v21;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v21 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v22;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v22 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v23;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v23 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v24;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v24 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v25;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v25 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v34;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v34 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v26;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v26 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v27;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v27 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v28;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v28 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v29;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v29 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v30;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v30 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v31;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v31 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v35;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v35 = 0;
    CData/*6:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_vel__v16;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_vel__v16 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_vel__v16;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_vel__v16 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v192;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v192 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v192;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v192 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v193;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v193 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v193;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v193 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v194;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v194 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v194;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v194 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v195;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v195 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v195;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v195 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v196;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v196 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v196;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v196 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v197;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v197 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v197;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v197 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v193;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v193 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v194;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v194 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v195;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v195 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v196;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v196 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v197;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v197 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v198;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v198 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v33;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v33 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v33;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v33 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v199;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v199 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v200;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v200 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v201;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v201 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v202;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v202 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v203;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v203 = 0;
    CData/*3:0*/ __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v204;
    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v204 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v36;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v36 = 0;
    CData/*3:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v35;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v35 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v32;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v32 = 0;
    VlWide<5>/*132:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25;
    VL_ZERO_W(133, __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25);
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25 = 0;
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v33;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v33 = 0;
    VlWide<5>/*132:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26;
    VL_ZERO_W(133, __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26);
    CData/*6:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v0;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v0;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v1;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 = 0;
    CData/*2:0*/ __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v1;
    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v6 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v6;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v6 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v7;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v7 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v7;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v7 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v8;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v8 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v8;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v8 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v9;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v9 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v9;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v9 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v10;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v10 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v10;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v10 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v11;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v11 = 0;
    IData/*31:0*/ __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v11;
    __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v11 = 0;
    VlWide<3>/*95:0*/ __Vtemp_2;
    VlWide<3>/*95:0*/ __Vtemp_3;
    VlWide<3>/*95:0*/ __Vtemp_4;
    VlWide<5>/*159:0*/ __Vtemp_9;
    // Body
    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__rst_ff2) {
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__rb_cnt = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_silnow = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__sustain = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending = 1U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__in_sel = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__out_sel = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__bend_pb = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__master_tune = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_pm = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_am = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__eg_mod = 0x0000007fU;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_phase = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_delta = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dstate = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc1 = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc2 = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_val_q = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overrun = 0U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v0 = 1U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globA = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globB = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globC = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegr = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegl = 0U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v0 = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_vel = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_ctrl = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_val = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pm_nobase_w = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pbase_w = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__amd_w = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_peg = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_level = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_li = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fa = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fl = 0U;
        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fsh = 0U;
        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1 = 1U;
    } else {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[2U] = 0U;
        if (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_boundary) 
             & (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state)))) {
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__st_overrun = 1U;
        }
        if ((0x00000020U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if ((0x00000010U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
            } else if ((8U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
                } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
                    } else {
                        if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                            __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_li__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_li;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_li__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk;
                            __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_li__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_li__v96 = 1U;
                        }
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1dU;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fq_sel 
                            = (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))
                                 ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk]
                                 : 0U) + (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                           && (1U & 
                                               (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode
                                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn] 
                                                >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))
                                           ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__pbase_w
                                           : (vlSelfRef.tb_dx7_core__DOT__dut__DOT__pm_nobase_w 
                                              + vlSelfRef.tb_dx7_core__DOT__dut__DOT__pbase_w)));
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start = 0U;
                    if ((0x0bU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_st))) {
                        __Vtemp_2[0U] = ((IData)((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_pt_q))) 
                                         << 4U);
                        __Vtemp_2[1U] = (((IData)((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_pt_q))) 
                                          >> 0x0000001cU) 
                                         | ((IData)(
                                                    ((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_pt_q)) 
                                                     >> 0x00000020U)) 
                                            << 4U));
                        __Vtemp_2[2U] = ((IData)(((QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_pt_q)) 
                                                  >> 0x00000020U)) 
                                         >> 0x0000001cU);
                        __Vtemp_3[0U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_level;
                        __Vtemp_3[1U] = 0U;
                        __Vtemp_3[2U] = 0U;
                        VL_MUL_W(3, __Vtemp_4, __Vtemp_2, __Vtemp_3);
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_li 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_level 
                               - ((__Vtemp_4[1U] << 4U) 
                                  | (__Vtemp_4[0U] 
                                     >> 0x0000001cU)));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x2aU;
                    }
                } else if ((1U & (~ (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt_busy))))) {
                    if ((0x5fU >= (0x0000007fU & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v0 
                            = (0x0000007fU & ((IData)(6U) 
                                              * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v0 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0 
                            = (0x0000007fU & ((IData)(6U) 
                                              * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(6U) 
                                                                              * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))));
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 9U;
                    if ((0x5fU >= (0x0000007fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v1 
                            = (0x0000007fU & ((IData)(1U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v1 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1 
                            = (0x0000007fU & ((IData)(1U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(1U) 
                                                                              + 
                                                                              ((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(1U) 
                                        + ((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(1U) 
                                                       + 
                                                       ((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                    }
                    if ((0x5fU >= (0x0000007fU & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v2 
                            = (0x0000007fU & ((IData)(2U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v2 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2 
                            = (0x0000007fU & ((IData)(2U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(2U) 
                                                                              + 
                                                                              ((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(2U) 
                                        + ((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(2U) 
                                                       + 
                                                       ((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                    }
                    if ((0x5fU >= (0x0000007fU & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v3 
                            = (0x0000007fU & ((IData)(3U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v3 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3 
                            = (0x0000007fU & ((IData)(3U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(3U) 
                                                                              + 
                                                                              ((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(3U) 
                                        + ((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(3U) 
                                                       + 
                                                       ((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                    }
                    if ((0x5fU >= (0x0000007fU & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v4 
                            = (0x0000007fU & ((IData)(4U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v4 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4 
                            = (0x0000007fU & ((IData)(4U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(4U) 
                                                                              + 
                                                                              ((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(4U) 
                                        + ((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(4U) 
                                                       + 
                                                       ((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                    }
                    if ((0x5fU >= (0x0000007fU & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v5 
                            = (0x0000007fU & ((IData)(5U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v5 = 1U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5 
                            = (0x0000007fU & ((IData)(5U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & (((IData)(5U) 
                                                                              + 
                                                                              ((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & (((IData)(5U) 
                                        + ((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & ((IData)(5U) 
                                                       + 
                                                       ((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        if ((1U & (~ (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt_busy))))) {
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(6U) 
                                              * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v6 
                                    = (0x0000007fU 
                                       & ((IData)(6U) 
                                          * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v6 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(6U) 
                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))));
                            }
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 9U;
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v7 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v7 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(1U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(1U) 
                                                + ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(1U) 
                                                 + 
                                                 ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                            }
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(2U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v8 
                                    = (0x0000007fU 
                                       & ((IData)(2U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v8 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(2U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(2U) 
                                                + ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(2U) 
                                                 + 
                                                 ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                            }
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(3U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v9 
                                    = (0x0000007fU 
                                       & ((IData)(3U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v9 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(3U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(3U) 
                                                + ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(3U) 
                                                 + 
                                                 ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                            }
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(4U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v10 
                                    = (0x0000007fU 
                                       & ((IData)(4U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v10 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(4U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(4U) 
                                                + ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(4U) 
                                                 + 
                                                 ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                            }
                            if ((0x5fU >= (0x0000007fU 
                                           & ((IData)(5U) 
                                              + ((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v11 
                                    = (0x0000007fU 
                                       & ((IData)(5U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v11 = 1U;
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(5U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                    = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                       [(3U & (((IData)(5U) 
                                                + ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                               >> 5U))] 
                                       | ((IData)(1U) 
                                          << (0x0000001fU 
                                              & ((IData)(5U) 
                                                 + 
                                                 ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                            }
                        }
                    } else {
                        if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                            __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_bp__v96 
                                = ((1U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_23))
                                    ? ((7U < (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_detune))
                                        ? (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpf0 
                                           + ((IData)(0x00003491U) 
                                              * ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_detune) 
                                                 - (IData)(7U))))
                                        : vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpf0)
                                    : ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_fine))
                                        ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpr1
                                        : (((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))
                                             ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk]
                                             : 0U) 
                                           + vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_bpr1)));
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_bp__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk;
                            __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_bp__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v96 = 1U;
                            __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 
                                = (0U != (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_23));
                            __VdlyLsb__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_opmode__v16 = 1U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 5U;
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_alg__v16 
                        = (0x0000001fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                          >> 0x00000018U));
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_alg__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_alg__v16 = 1U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x26U;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 
                        = ((0U == (7U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                         >> 0x00000015U)))
                            ? 0x00000010U : (0x0000001fU 
                                             & ((IData)(8U) 
                                                - (7U 
                                                   & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                      >> 0x00000015U)))));
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16 = 1U;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16 
                        = (0x000000ffU & (((IData)(0x000000a5U) 
                                           * (0x000000ffU 
                                              & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA 
                                                 >> 0x00000010U))) 
                                          >> 6U));
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_amdep__v16 
                        = (0x000000ffU & (((IData)(0x000000a5U) 
                                           * (0x000000ffU 
                                              & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA 
                                                 >> 8U))) 
                                          >> 6U));
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_amdep__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__pmsens_tab__447__i 
                        = (7U & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_4__pmsens_tab 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h831d05a1_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__pmsens_tab__447__i))]);
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_4__pmsens_tab;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i 
                        = (3U & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_5__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__448__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v96 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_5__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v96 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v96 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i 
                        = (3U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
                                 >> 2U));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_6__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__449__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v97 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_6__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v97 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v97 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i 
                        = (3U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
                                 >> 4U));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_7__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__450__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v98 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_7__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v98 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v98 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i 
                        = (3U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
                                 >> 6U));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_8__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__451__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v99 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_8__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v99 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v99 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i 
                        = (3U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
                                 >> 8U));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_9__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__452__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v100 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_9__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v100 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v100 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i 
                        = (3U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
                                 >> 0x0000000aU));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_10__ampsens_tab 
                        = ((0U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i))
                            ? 0U : ((1U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i))
                                     ? 0x00424242U : 
                                    ((2U == (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__ampsens_tab__453__i))
                                      ? 0x006d6d6dU
                                      : 0x01000000U)));
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v101 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_10__ampsens_tab;
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v101 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v101 = 1U;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v96 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr);
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v96 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v97 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr 
                                          >> 8U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v97 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v98 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr 
                                          >> 0x00000010U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v98 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v99 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr 
                                          >> 0x00000018U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v99 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v96 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl);
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v96 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v97 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                                          >> 8U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v97 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v98 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                                          >> 0x10U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v98 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v99 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                                          >> 0x18U));
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v99 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                } else {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[0U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfffeU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (1U | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v16 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v17 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[0U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v17 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v18 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v19 = 1U;
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
                    if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[1U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfffdU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (2U | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v20 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v21 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[1U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v21 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v22 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v23 = 1U;
                    }
                    if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[2U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfffbU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (4U | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v24 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v25 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[2U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v25 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v26 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v27 = 1U;
                    }
                    if ((8U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((8U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[3U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfff7U & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (8U | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v28 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v29 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[3U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v29 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v30 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v31 = 1U;
                    }
                    if ((0x00000010U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000010U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[4U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xffefU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000010U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v32 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v33 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[4U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v33 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v34 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v35 = 1U;
                    }
                    if ((0x00000020U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000020U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[5U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xffdfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000020U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v36 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v37 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[5U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v37 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v38 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v39 = 1U;
                    }
                    if ((0x00000040U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000040U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[6U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xffbfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000040U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v40 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v41 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[6U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v41 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v42 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v43 = 1U;
                    }
                    if ((0x00000080U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000080U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[7U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xff7fU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000080U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v44 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v45 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[7U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v45 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v46 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v47 = 1U;
                    }
                    if ((0x00000100U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000100U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[8U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfeffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000100U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v48 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v49 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[8U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v49 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v50 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v51 = 1U;
                    }
                    if ((0x00000200U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000200U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[9U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfdffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000200U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v52 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v53 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[9U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v53 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v54 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v55 = 1U;
                    }
                    if ((0x00000400U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000400U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[10U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xfbffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000400U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v56 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v57 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[10U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v57 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v58 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v59 = 1U;
                    }
                    if ((0x00000800U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00000800U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[11U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xf7ffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00000800U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v60 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v61 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[11U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v61 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v62 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v63 = 1U;
                    }
                    if ((0x00001000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00001000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[12U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xefffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00001000U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v64 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v65 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[12U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v65 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v66 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v67 = 1U;
                    }
                    if ((0x00002000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00002000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[13U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xdfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00002000U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v68 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v69 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[13U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v69 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v70 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v71 = 1U;
                    }
                    if ((0x00004000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00004000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[14U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0xbfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00004000U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v72 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v73 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[14U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v73 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v74 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v75 = 1U;
                    }
                    if ((0x00008000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live))) {
                        if ((0x00008000U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent))) {
                            if ((0x40U <= (0x0000007fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[15U]))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                                    = (0x7fffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                                    = (0x00008000U 
                                       | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v76 = 1U;
                            } else {
                                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_sil__v77 
                                    = (0x0000007fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sil[15U])));
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v77 = 1U;
                            }
                        } else {
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v78 = 1U;
                        }
                    } else {
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v79 = 1U;
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((0x46U > (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr))) {
                        if ((5U >= (7U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr)))) {
                            __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_ph__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_ph__v96 
                                = (7U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr));
                            __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v96 = 1U;
                        }
                    } else if ((0x4cU > (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr))) {
                        if ((5U >= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr) 
                                          - (IData)(6U))))) {
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__op_go__v96 
                                = (0x0000ffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data);
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_go__v96 
                                = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr) 
                                         - (IData)(6U)));
                            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v96 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_go__v96 = 1U;
                        }
                    } else if ((0x4cU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr))) {
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb0__v16 = 1U;
                    } else {
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb1__v16 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_silnow 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__note_silent;
                        if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x24U;
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                = (0x0000000fU & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1fU;
                        }
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr 
                        = (0x0000007fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr)));
                } else {
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 
                        = (0x00000001ffffffffULL & 
                           (vlSelfRef.tb_dx7_core__DOT__dut__DOT__mixbuf
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel]
                            [(0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr))] 
                            + VL_EXTENDS_QI(33,17, 
                                            (VL_GTS_IQQ(33, 0x00000001ff000000ULL, 
                                                        (0x00000001ffffffffULL 
                                                         & VL_SHIFTRS_QQI(33,33,32, 
                                                                          (0x00000001ffffffffULL 
                                                                           & VL_EXTENDS_QI(33,32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data)), 4U)))
                                              ? 0x00008000U
                                              : (VL_LTES_IQQ(33, 0x0000000001000000ULL, 
                                                             (0x00000001ffffffffULL 
                                                              & VL_SHIFTRS_QQI(33,33,32, 
                                                                               (0x00000001ffffffffULL 
                                                                                & VL_EXTENDS_QI(33,32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data)), 4U)))
                                                  ? 0x00007fffU
                                                  : 
                                                 (0x0001ffffU 
                                                  & (IData)(
                                                            (0x00000001ffffffffULL 
                                                             & VL_SHIFTRS_QQI(33,33,32, 
                                                                              (0x00000001ffffffffULL 
                                                                               & VL_SHIFTRS_QQI(33,33,32, 
                                                                                (0x00000001ffffffffULL 
                                                                                & VL_EXTENDS_QI(33,32, vlSelfRef.tb_dx7_core__DOT__dut__DOT__ar_rd_data)), 4U)), 9U)))))))));
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 
                        = (0x0000003fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr));
                    vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1152 = 1U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr 
                        = (0x0000007fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr)));
                    if ((0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr))) {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__rb_cnt = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr = 0x40U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x23U;
                    }
                }
            } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_start = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_cfg = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_fb = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 0U;
                if ((6U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x22U;
                }
            } else if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_cfg = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_fb = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_start = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x21U;
                    } else {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx 
                            = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                     - (IData)(1U)));
                        if ((5U >= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                          - (IData)(1U))))) {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                        - (IData)(1U)))];
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                        - (IData)(1U)))];
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level 
                                = (0x0fffffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li
                                   [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                   [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                           - (IData)(1U)))]);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                        - (IData)(1U)))];
                        } else {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase = 0U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq = 0U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level = 0U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out = 0U;
                        }
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                            = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                    }
                } else {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 1U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx 
                        = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                 - (IData)(1U)));
                    if ((5U >= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                      - (IData)(1U))))) {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                    - (IData)(1U)))];
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                    - (IData)(1U)))];
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level 
                            = (0x0fffffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li
                               [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                               [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                       - (IData)(1U)))]);
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                    - (IData)(1U)))];
                    } else {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level = 0U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out = 0U;
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                        = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                }
            } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 1U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx 
                    = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                             - (IData)(1U)));
                if ((5U >= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                  - (IData)(1U))))) {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level 
                        = (0x0fffffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li
                           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                           [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                   - (IData)(1U)))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                } else {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out = 0U;
                }
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
            } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op = 1U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_op_idx 
                    = (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                             - (IData)(1U)));
                if ((5U >= (7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                  - (IData)(1U))))) {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level 
                        = (0x0fffffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li
                           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                           [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                   - (IData)(1U)))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_go
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                        [(7U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk) 
                                - (IData)(1U)))];
                } else {
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_phase = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_freq = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_level = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_gain_out = 0U;
                }
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
            } else {
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_cfg = 1U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_algorithm 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg
                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn];
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_ld_fb = 1U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 1U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_feedback 
                    = ((8U > vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs
                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn])
                        ? ((0x10U == vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn])
                            ? 0U : (0x0000001fU & ((IData)(8U) 
                                                   - (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs
                                                             [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]))))
                        : 0U);
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb0 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0
                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn];
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_cfg_fb1 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1
                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn];
            }
        } else if ((0x00000010U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if ((8U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                            if (((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                 & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                       >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x24U;
                            } else if ((1U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                              >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x20U;
                            } else {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                    = (0x0000000fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            }
                        } else {
                            if ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                                __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_fq__v96 
                                    = VL_SHIFTRS_III(32,32,9, 
                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__freqlut_rom
                                                      [vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fa] 
                                                      + (IData)(
                                                                (0x00000000ffffffffULL 
                                                                 & (VL_MULS_QQQ(46, 
                                                                                (0x00003fffffffffffULL 
                                                                                & VL_EXTENDS_QI(46,32, 
                                                                                (((0x0400U 
                                                                                >= (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_24))
                                                                                 ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__freqlut_rom
                                                                                [vlSelfRef.__VdfgRegularize_hebeb780c_0_24]
                                                                                 : 0U) 
                                                                                - vlSelfRef.tb_dx7_core__DOT__dut__DOT__freqlut_rom
                                                                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fa]))), (QData)((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fl))) 
                                                                    >> 0x0000000eU)))), (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fsh));
                                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_fq__v96 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk;
                                __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_fq__v96 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_fq__v96 = 1U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                                if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0U;
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1fU;
                                } else {
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                        = (0x0000000fU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x17U;
                                }
                            } else {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1cU;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fa 
                            = (0x000003ffU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_sel 
                                              >> 0x0eU));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fsh 
                            = ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_fsh_s) 
                               & (- (IData)((1U & (~ 
                                                   ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_fsh_s) 
                                                    >> 8U))))));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1eU;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fl 
                            = (0x00003fffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_sel);
                    } else {
                        if ((0x5fU >= (0x0000007fU 
                                       & (((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[0U] 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][0U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[1U] 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][1U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[2U] 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][2U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[3U] 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[4U] 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][4U];
                        } else {
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[0U] = 0U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[1U] = 0U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[2U] = 0U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[3U] = 0U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[4U] = 0U;
                        }
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__Vfuncout 
                            = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[4U] 
                                << 0x0000001bU) | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__s[3U] 
                                                   >> 5U));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_level 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__454__Vfuncout;
                        if ((0U != ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))
                                     ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams
                                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk]
                                     : 0U))) {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_amd 
                                = (0x01ffffffU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__amd_w);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__exp_ams 
                                = ((5U >= (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))
                                    ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams
                                   [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                                   [vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk]
                                    : 0U);
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__exp_start = 1U;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x29U;
                        } else {
                            if ((0x5fU >= (0x0000007fU 
                                           & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[0U] 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                    [(0x0000007fU & 
                                      (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][0U];
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[1U] 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                    [(0x0000007fU & 
                                      (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][1U];
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[2U] 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                    [(0x0000007fU & 
                                      (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][2U];
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[3U] 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                    [(0x0000007fU & 
                                      (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][3U];
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[4U] 
                                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                    [(0x0000007fU & 
                                      (((IData)(6U) 
                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][4U];
                            } else {
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[0U] = 0U;
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[1U] = 0U;
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[2U] = 0U;
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[3U] = 0U;
                                __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[4U] = 0U;
                            }
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__Vfuncout 
                                = ((__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[4U] 
                                    << 0x0000001bU) 
                                   | (__Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__s[3U] 
                                      >> 5U));
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_li 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__env_level__455__Vfuncout;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x2aU;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__amd_w 
                            = ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_ambase 
                                > vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_amax)
                                ? vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_ambase
                                : vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_amax);
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1cU;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pm_nobase_w 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_peg 
                               + ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_sl0 
                                                 >> 0x0000001fU)))
                                   ? (- vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmax)
                                   : vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmax));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1bU;
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_adv) {
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 
                            = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix
                                                           [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn])));
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v16 = 1U;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__456__lvl 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(3U & ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix
                                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]))];
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__i 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__456__lvl;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__Vfuncout 
                            = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                               [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__i))]);
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__457__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_28__peg_tab19 
                            = VL_MULS_III(32, (IData)(0x00080000U), 
                                          VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_28__peg_tab19;
                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v16 = 1U;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__458__lvl 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(3U & ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix
                                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]))];
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__i 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__458__lvl;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__Vfuncout 
                            = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                               [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__i))]);
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__459__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_29__peg_tab19 
                            = VL_MULS_III(32, (IData)(0x00080000U), 
                                          VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 
                            = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_29__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_nxt);
                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v16 = 1U;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460__r 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]
                            [(3U & ((IData)(1U) + vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix
                                    [vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn]))];
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__i 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460__r;
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__Vfuncout 
                            = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                               [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__i))]);
                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460____VlefCall_0__peg_rate_lut 
                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__461__Vfuncout;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_30__peg_rate_inc 
                            = ((IData)(0x0000041aU) 
                               * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__460____VlefCall_0__peg_rate_lut));
                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_30__peg_rate_inc;
                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v16 = 1U;
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1aU;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_peg 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__t_peg_nxt;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x19U;
                }
            } else if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        if (((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                             & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                   >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0U;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x1fU;
                        } else if ((1U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                          >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x18U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmdep 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan];
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_pmsens 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan];
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_amdep 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan];
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                = (0x0000000fU & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                        }
                    } else {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_phase 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_val_q 
                            = (0x01ffffffU & ((0U == 
                                               (7U 
                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                   >> 0x00000011U)))
                                               ? (0x00ffffffU 
                                                  & ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                      >> 0x0000001fU)
                                                      ? 
                                                     (~ 
                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                       >> 7U))
                                                      : 
                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                      >> 7U)))
                                               : ((1U 
                                                   == 
                                                   (7U 
                                                    & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                       >> 0x00000011U)))
                                                   ? 
                                                  (0x00ffffffU 
                                                   & (0x00800000U 
                                                      ^ 
                                                      (~ 
                                                       (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                        >> 8U))))
                                                   : 
                                                  ((2U 
                                                    == 
                                                    (7U 
                                                     & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                        >> 0x00000011U)))
                                                    ? 
                                                   (0x00ffffffU 
                                                    & (0x00800000U 
                                                       ^ 
                                                       (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                        >> 8U)))
                                                    : 
                                                   ((3U 
                                                     == 
                                                     (7U 
                                                      & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                         >> 0x00000011U)))
                                                     ? 
                                                    (0x01000000U 
                                                     & (- (IData)(
                                                                  (1U 
                                                                   & (~ 
                                                                      (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                                       >> 0x0000001fU))))))
                                                     : 
                                                    ((4U 
                                                      == 
                                                      (7U 
                                                       & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                          >> 0x00000011U)))
                                                      ? 
                                                     ((IData)(0x00800000U) 
                                                      + 
                                                      VL_SHIFTRS_III(25,32,32, 
                                                                     ((IData)(
                                                                              (0x000000ffffffffffULL 
                                                                               & VL_EXTENDS_QI(40,26, vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_sin_rom
                                                                                [
                                                                                (0x000007ffU 
                                                                                & ((IData)(1U) 
                                                                                + 
                                                                                (0x000007feU 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                                                >> 0x00000015U))))]))) 
                                                                      + (IData)(
                                                                                (0x000000ffffffffffULL 
                                                                                & VL_SHIFTRS_QQI(40,40,32, 
                                                                                (0x000000ffffffffffULL 
                                                                                & VL_MULS_QQQ(40, 
                                                                                (0x000000ffffffffffULL 
                                                                                & VL_EXTENDS_QI(40,26, vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_sin_rom
                                                                                [
                                                                                (0x000007feU 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                                                >> 0x00000015U))])), (QData)((IData)(
                                                                                (0x00003fffU 
                                                                                & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_ph_n 
                                                                                >> 8U)))))), 0x0000000eU)))), 1U))
                                                      : 
                                                     ((5U 
                                                       == 
                                                       (7U 
                                                        & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                                           >> 0x00000011U)))
                                                       ? 
                                                      (0x01ff0000U 
                                                       & (((IData)(1U) 
                                                           + 
                                                           (0x80U 
                                                            ^ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_rand_nx))) 
                                                          << 0x00000010U))
                                                       : 0x00800000U)))))));
                        if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_dsum 
                                           >> 0x20U)))) {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q = 0x01000000U;
                        } else {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dstate 
                                = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_dsum);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q 
                                = (0x00ffffffU & ((- (IData)(
                                                             (1U 
                                                              & (IData)(
                                                                        (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_dsum 
                                                                         >> 0x0000001fU))))) 
                                                  & (IData)(
                                                            (vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_dsum 
                                                             >> 7U))));
                        }
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pbase_w 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__bend_pb 
                               + vlSelfRef.tb_dx7_core__DOT__dut__DOT__master_tune);
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x17U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand 
                            = ((5U == (7U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
                                             >> 0x00000011U)))
                                ? (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_rand_nx)
                                : (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand));
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((0U == ((vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[0U] 
                                 | vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[1U]) 
                                | vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[2U]))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x16U;
                    }
                } else if ((1U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                  >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))) {
                    if ((1U & (~ ((0x5fU >= (0x0000007fU 
                                             & (((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))) 
                                  && (1U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy
                                            [(3U & 
                                              ((((IData)(6U) 
                                                 * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                               >> 5U))] 
                                            >> (0x0000001fU 
                                                & (((IData)(6U) 
                                                    * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))))))) {
                        if ((0x5fU >= (0x0000007fU 
                                       & (((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v12 
                                = (0x0000007fU & (((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v12 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & ((((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                                                + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & ((((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                            + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & (((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))));
                        }
                        if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                            if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x15U;
                            } else {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                    = (0x0000000fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        }
                    }
                } else if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x15U;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                        = (0x0000000fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                }
            } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x14U;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                            = (0x0000000fU & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x11U;
                    }
                } else if ((1U & (~ ((0x5fU >= (0x0000007fU 
                                                & (((IData)(6U) 
                                                    * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))) 
                                     && (1U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy
                                               [(3U 
                                                 & ((((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                     + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                    >> 5U))] 
                                               >> (0x0000001fU 
                                                   & (((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                      + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))))))) {
                    if ((0x5fU >= (0x0000007fU & (((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v13 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v13 = 1U;
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[0U] 
                            = (IData)((((QData)((IData)(
                                                        (0x0000007fU 
                                                         & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                            >> 0x00000010U)))) 
                                        << 0x0000001aU) 
                                       | (QData)((IData)(
                                                         ((0x03f80000U 
                                                           & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                              >> 5U)) 
                                                          | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_outlevel) 
                                                              << 6U) 
                                                             | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_rs_eff)))))));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U] 
                            = ((0xfffffffeU & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U]) 
                               | (IData)(((((QData)((IData)(
                                                            (0x0000007fU 
                                                             & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                                >> 0x00000010U)))) 
                                            << 0x0000001aU) 
                                           | (QData)((IData)(
                                                             ((0x03f80000U 
                                                               & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                                  >> 5U)) 
                                                              | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_outlevel) 
                                                                  << 6U) 
                                                                 | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_rs_eff)))))) 
                                          >> 0x00000020U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U] 
                            = ((0xffc00001U & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U]) 
                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r3) 
                                   << 0x0000000fU) 
                                  | (((0x00003f80U 
                                       & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                          << 7U)) | 
                                      (0x0000007fU 
                                       & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                          >> 8U))) 
                                     << 1U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U] 
                            = ((0x003fffffU & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U]) 
                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1) 
                                   << 0x0000001dU) 
                                  | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2) 
                                     << 0x00000016U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[2U] 
                            = ((0x003ffff0U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r0) 
                                               << 4U)) 
                               | ((0x003fffffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1) 
                                                  >> 3U)) 
                                  | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2) 
                                     >> 0x0000000aU)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[3U] = 0U;
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[4U] = 0U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & ((((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & ((((IData)(6U) 
                                         * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                        + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & (((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn)) 
                                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))));
                    }
                    if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x13U;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                            = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                    }
                }
            } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if (((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                     & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                           >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x14U;
                } else if ((1U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live) 
                                  >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x12U;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                        = (0x0000000fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                }
            } else {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_silnow = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__sustain = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending = 1U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_phase = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_delta = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dstate = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc1 = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc2 = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_val_q = 0U;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_del_q = 0U;
                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80 = 1U;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1153 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98 = 1U;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1154 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1155 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1156 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1157 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1158 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1159 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1160 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1161 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1162 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1163 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1164 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1165 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1166 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1167 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1168 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1169 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1170 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1171 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1172 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1173 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1174 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1175 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1176 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1177 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1178 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1179 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1180 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1181 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1182 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1183 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1184 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1185 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1186 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1187 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1188 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1189 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1190 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1191 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1192 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1193 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1194 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1195 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1196 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1197 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1198 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1199 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1200 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1201 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1202 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1203 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1204 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1205 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1206 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1207 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1208 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1209 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1210 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1211 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1212 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1213 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1214 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1215 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
                vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1216 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
            }
        } else if ((8U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                        if (((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                             & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                   >> 0x0fU)))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                        } else if ((1U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                          >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))) {
                            if ((1U & (~ (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan_busy))))) {
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                                    = ((~ ((IData)(1U) 
                                           << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                       & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                                if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain) {
                                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                                        = ((IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                           | (0x0000ffffU 
                                              & ((IData)(1U) 
                                                 << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                } else {
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v14 
                                            = (0x0000007fU 
                                               & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v14 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7 
                                            = (0x0000007fU 
                                               & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(6U) 
                                                     * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))));
                                    }
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v15 
                                            = (0x0000007fU 
                                               & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v15 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8 
                                            = (0x0000007fU 
                                               & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(1U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(1U) 
                                                     + 
                                                     ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(1U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                                    }
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v16 
                                            = (0x0000007fU 
                                               & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v16 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9 
                                            = (0x0000007fU 
                                               & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(2U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(2U) 
                                                     + 
                                                     ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(2U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                                    }
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(3U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v17 
                                            = (0x0000007fU 
                                               & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v17 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10 
                                            = (0x0000007fU 
                                               & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(3U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(3U) 
                                                     + 
                                                     ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(3U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                                    }
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v18 
                                            = (0x0000007fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v18 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11 
                                            = (0x0000007fU 
                                               & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(4U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(4U) 
                                                     + 
                                                     ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(4U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                                    }
                                    if ((0x5fU >= (0x0000007fU 
                                                   & ((IData)(5U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v19 
                                            = (0x0000007fU 
                                               & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v19 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12 
                                            = (0x0000007fU 
                                               & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12 = 1U;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                                & (((IData)(5U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                               [(3U 
                                                 & (((IData)(5U) 
                                                     + 
                                                     ((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                    >> 5U))] 
                                               | ((IData)(1U) 
                                                  << 
                                                  (0x0000001fU 
                                                   & ((IData)(5U) 
                                                      + 
                                                      ((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                                    }
                                    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn
                                        [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan]) {
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32 = 1U;
                                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__462__lvl 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__i 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__462__lvl;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__Vfuncout 
                                            = (0x000000ffU 
                                               & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                               [(0x07ffffffU 
                                                 & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__i))]);
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__463__Vfuncout;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_25__peg_tab19 
                                            = VL_MULS_III(32, (IData)(0x00080000U), 
                                                          VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_25__peg_tab19;
                                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v33 = 1U;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__464__lvl 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__i 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__464__lvl;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__Vfuncout 
                                            = (0x000000ffU 
                                               & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                               [(0x07ffffffU 
                                                 & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__i))]);
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__465__Vfuncout;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_26__peg_tab19 
                                            = VL_MULS_III(32, (IData)(0x00080000U), 
                                                          VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 
                                            = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_26__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl
                                                         [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan]);
                                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v33 = 1U;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466__r 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr
                                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__i 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466__r;
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__Vfuncout 
                                            = (0x000000ffU 
                                               & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                                               [(0x07ffffffU 
                                                 & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__i))]);
                                        __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466____VlefCall_0__peg_rate_lut 
                                            = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__467__Vfuncout;
                                        vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_27__peg_rate_inc 
                                            = ((IData)(0x0000041aU) 
                                               * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__466____VlefCall_0__peg_rate_lut));
                                        vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_27__peg_rate_inc;
                                        vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 
                                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v33 = 1U;
                                    }
                                }
                                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                    = (0x0000000fU 
                                       & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                                = (0x0000000fU & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if (((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                         & (~ (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                >> 0x0fU) & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                                >> 0x0fU)))))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    } else if ((1U & ((~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained) 
                                          >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                      | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                                         >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                            = (0x0000000fU & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                    } else if ((1U & (~ (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan_busy))))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = ((~ ((IData)(1U) << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                               & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(6U) 
                                          * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v20 
                                = (0x0000007fU & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v20 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13 
                                = (0x0000007fU & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(6U) 
                                            * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(6U) 
                                          * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(1U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v21 
                                = (0x0000007fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v21 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14 
                                = (0x0000007fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(1U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(1U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(1U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(2U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v22 
                                = (0x0000007fU & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v22 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15 
                                = (0x0000007fU & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(2U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(2U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(2U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(3U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v23 
                                = (0x0000007fU & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v23 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16 
                                = (0x0000007fU & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(3U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(3U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(3U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(4U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v24 
                                = (0x0000007fU & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v24 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17 
                                = (0x0000007fU & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(4U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(4U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(4U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(5U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v25 
                                = (0x0000007fU & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v25 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18 
                                = (0x0000007fU & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(5U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(5U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(5U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))))));
                        }
                        if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan]) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__468__lvl 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__468__lvl;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__i))]);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__469__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_22__peg_tab19 
                                = VL_MULS_III(32, (IData)(0x00080000U), 
                                              VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_22__peg_tab19;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v34 = 1U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__470__lvl 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__470__lvl;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__i))]);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__471__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_23__peg_tab19 
                                = VL_MULS_III(32, (IData)(0x00080000U), 
                                              VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 
                                = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_23__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl
                                             [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan]);
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v34 = 1U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472__r 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472__r;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__i))]);
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472____VlefCall_0__peg_rate_lut 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__473__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_24__peg_rate_inc 
                                = ((IData)(0x0000041aU) 
                                   * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__472____VlefCall_0__peg_rate_lut));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_24__peg_rate_inc;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v34 = 1U;
                        }
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                            = (0x0000000fU & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                    }
                } else if ((1U & (~ (0U != (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt_busy))))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                        = ((~ ((IData)(1U) << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                           & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = ((IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained) 
                               | (0x0000ffffU & ((IData)(1U) 
                                                 << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                    } else {
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(6U) 
                                          * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v26 
                                = (0x0000007fU & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v26 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19 
                                = (0x0000007fU & ((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(6U) 
                                            * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(6U) 
                                          * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(1U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v27 
                                = (0x0000007fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v27 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20 
                                = (0x0000007fU & ((IData)(1U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(1U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(1U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(1U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(2U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v28 
                                = (0x0000007fU & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v28 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21 
                                = (0x0000007fU & ((IData)(2U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(2U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(2U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(2U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(3U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v29 
                                = (0x0000007fU & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v29 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22 
                                = (0x0000007fU & ((IData)(3U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(3U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(3U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(3U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(4U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v30 
                                = (0x0000007fU & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v30 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23 
                                = (0x0000007fU & ((IData)(4U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(4U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(4U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(4U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                        }
                        if ((0x5fU >= (0x0000007fU 
                                       & ((IData)(5U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)))))) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v31 
                                = (0x0000007fU & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v31 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24 
                                = (0x0000007fU & ((IData)(5U) 
                                                  + 
                                                  ((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24 = 1U;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                              & (((IData)(5U) 
                                                                                + 
                                                                                ((IData)(6U) 
                                                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                                                                >> 5U))] 
                                = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                                   [(3U & (((IData)(5U) 
                                            + ((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                                           >> 5U))] 
                                   | ((IData)(1U) << 
                                      (0x0000001fU 
                                       & ((IData)(5U) 
                                          + ((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))));
                        }
                        if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt]) {
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34 = 1U;
                            __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__474__lvl 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__474__lvl;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__i))]);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__475__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_19__peg_tab19 
                                = VL_MULS_III(32, (IData)(0x00080000U), 
                                              VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_19__peg_tab19;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v35 = 1U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__476__lvl 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__476__lvl;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__i))]);
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__477__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_20__peg_tab19 
                                = VL_MULS_III(32, (IData)(0x00080000U), 
                                              VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 
                                = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_20__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl
                                             [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt]);
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v35 = 1U;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478__r 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr
                                [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt][3U];
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__i 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478__r;
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__Vfuncout 
                                = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                                   [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__i))]);
                            __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478____VlefCall_0__peg_rate_lut 
                                = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__479__Vfuncout;
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_21__peg_rate_inc 
                                = ((IData)(0x0000041aU) 
                                   * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__478____VlefCall_0__peg_rate_lut));
                            vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_21__peg_rate_inc;
                            vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 
                                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v35 = 1U;
                        }
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    if ((((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown) 
                          >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)) 
                         & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_midi
                            [vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan] 
                            == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note)))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x0cU;
                    } else if ((0x0fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan 
                            = (0x0000000fU & ((IData)(1U) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan)));
                    }
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr 
                        = (0x000000ffU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__seq_ctr)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_seq__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__seq_ctr;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_seq__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_seq__v16 = 1U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dstate = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_midi__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_midi__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v16 = 1U;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_vel__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_vel;
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_vel__v16 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v192 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[0U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v192 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v193 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[1U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v193 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v194 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[2U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v194 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v195 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[3U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v195 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v196 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[4U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v196 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v197 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[5U];
                    __VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v197 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                        = ((IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live) 
                           | (0x0000ffffU & ((IData)(1U) 
                                             << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                        = ((IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown) 
                           | (0x0000ffffU & ((IData)(1U) 
                                             << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                        = (((~ ((IData)(1U) << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                            & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained)) 
                           | (0x0000ffffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain) 
                                             << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_sil__v96 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v96 = 1U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed 
                        = ((~ ((IData)(1U) << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))) 
                           & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_reclaimed));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used 
                        = ((IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used) 
                           | (0x0000ffffU & ((IData)(1U) 
                                             << (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))));
                    if (((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind)) 
                         & (~ ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used) 
                               >> (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt))))) {
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v193 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v193 = 1U;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v193 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v194 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v194 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v195 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v195 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v196 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v196 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v197 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v197 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v198 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v198 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v33 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v33 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    }
                    if ((0x00100000U & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB)) {
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v199 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v199 = 1U;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v199 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v200 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v200 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v201 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v201 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v202 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v202 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v203 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v203 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        __VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v204 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                        vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_go__v204 
                            = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    }
                    if ((0x00010000U & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB)) {
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_phase = 0x7fffffffU;
                    }
                }
            } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind))) {
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__480__lvl 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                                          >> 0x18U));
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__480__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__481__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_11__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_11__peg_tab19;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peglvl__v32 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__482__lvl 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__482__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__483__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_12__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_12__peg_tab19;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v36 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__484__lvl 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__484__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__485__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_13__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__486__lvl 
                        = (0x0000007fU & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                                          >> 0x18U));
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__486__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__487__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_14__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 
                        = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_13__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_14__peg_tab19);
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v36 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488__r 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488__r;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__i))]);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488____VlefCall_0__peg_rate_lut 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__489__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_15__peg_rate_inc 
                        = ((IData)(0x0000041aU) * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__488____VlefCall_0__peg_rate_lut));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_15__peg_rate_inc;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v36 = 1U;
                } else {
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__490__lvl 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__490__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__491__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_16__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_16__peg_tab19;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegtgt__v37 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__492__lvl 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab19__492__lvl;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_hcedd08e0_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__i))]);
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_tab_lut__493__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_17__peg_tab19 
                        = VL_MULS_III(32, (IData)(0x00080000U), 
                                      VL_EXTENDS_II(32,8, (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__peg_tab19__Vstatic__t)));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 
                        = VL_GTS_III(32, vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_17__peg_tab19, vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_peglvl
                                     [vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt]);
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegris__v37 = 1U;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494__r 
                        = (0x0000007fU & vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__i 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494__r;
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__Vfuncout 
                        = (0x000000ffU & Vtb_dx7_core__ConstPool__CONST_h77b89011_0
                           [(0x07ffffffU & (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__i))]);
                    __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494____VlefCall_0__peg_rate_lut 
                        = __Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_lut__495__Vfuncout;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_18__peg_rate_inc 
                        = ((IData)(0x0000041aU) * (IData)(__Vfunc_tb_dx7_core__DOT__dut__DOT__peg_rate_inc__494____VlefCall_0__peg_rate_lut));
                    vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT____VlemCall_18__peg_rate_inc;
                    vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_peginc__v37 = 1U;
                }
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x0aU;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v36 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v36 = 1U;
                __VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v35 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt;
            } else {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
            }
        } else if ((4U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
                } else if ((1U & (~ ((0x5fU >= (0x0000007fU 
                                                & (((IData)(6U) 
                                                    * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))) 
                                     && (1U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy
                                               [(3U 
                                                 & ((((IData)(6U) 
                                                      * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                     + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                    >> 5U))] 
                                               >> (0x0000001fU 
                                                   & (((IData)(6U) 
                                                       * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                      + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))))))) {
                    if ((0x5fU >= (0x0000007fU & (((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v32 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v32 = 1U;
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[0U] 
                            = (0xfffffffeU & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[0U]);
                        if ((0x5fU >= (0x0000007fU 
                                       & (((IData)(6U) 
                                           * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                            __Vtemp_9[0U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][0U];
                            __Vtemp_9[1U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][1U];
                            __Vtemp_9[2U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][2U];
                            __Vtemp_9[3U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][3U];
                            __Vtemp_9[4U] = vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_rd
                                [(0x0000007fU & (((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))][4U];
                        } else {
                            __Vtemp_9[0U] = 0U;
                            __Vtemp_9[1U] = 0U;
                            __Vtemp_9[2U] = 0U;
                            __Vtemp_9[3U] = 0U;
                            __Vtemp_9[4U] = 0U;
                        }
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[0U] 
                            = ((1U & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[0U]) 
                               | (0xfffffffeU & __Vtemp_9[0U]));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[1U] 
                            = ((1U & __Vtemp_9[1U]) 
                               | (0xfffffffeU & __Vtemp_9[1U]));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[2U] 
                            = ((1U & __Vtemp_9[2U]) 
                               | (0xfffffffeU & __Vtemp_9[2U]));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[3U] 
                            = ((1U & __Vtemp_9[3U]) 
                               | (0xfffffffeU & __Vtemp_9[3U]));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[4U] 
                            = (0x0000001fU & ((1U & __Vtemp_9[4U]) 
                                              | (0x0000001eU 
                                                 & __Vtemp_9[4U])));
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & ((((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & ((((IData)(6U) 
                                         * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                        + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & (((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))));
                    }
                    if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x28U;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                            = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                    }
                }
            } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if ((1U & (~ ((0x5fU >= (0x0000007fU 
                                         & (((IData)(6U) 
                                             * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                            + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))) 
                              && (1U & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy
                                        [(3U & ((((IData)(6U) 
                                                  * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                 + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                >> 5U))] 
                                        >> (0x0000001fU 
                                            & (((IData)(6U) 
                                                * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))))))) {
                    if ((0x5fU >= (0x0000007fU & (((IData)(6U) 
                                                   * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                  + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))))) {
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v33 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v33 = 1U;
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[0U] 
                            = (IData)((((QData)((IData)(
                                                        (0x0000007fU 
                                                         & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                            >> 0x00000010U)))) 
                                        << 0x0000001aU) 
                                       | (QData)((IData)(
                                                         ((0x03f80000U 
                                                           & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                              >> 5U)) 
                                                          | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_outlevel) 
                                                              << 6U) 
                                                             | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_rs_eff)))))));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U] 
                            = ((0xfffffffeU & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U]) 
                               | (IData)(((((QData)((IData)(
                                                            (0x0000007fU 
                                                             & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                                >> 0x00000010U)))) 
                                            << 0x0000001aU) 
                                           | (QData)((IData)(
                                                             ((0x03f80000U 
                                                               & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                                                  >> 5U)) 
                                                              | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_outlevel) 
                                                                  << 6U) 
                                                                 | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_rs_eff)))))) 
                                          >> 0x00000020U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U] 
                            = ((0xffc00001U & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U]) 
                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r3) 
                                   << 0x0000000fU) 
                                  | (((0x00003f80U 
                                       & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                          << 7U)) | 
                                      (0x0000007fU 
                                       & (vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_elevels 
                                          >> 8U))) 
                                     << 1U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U] 
                            = ((0x003fffffU & __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U]) 
                               | (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1) 
                                   << 0x0000001dU) 
                                  | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2) 
                                     << 0x00000016U)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[2U] 
                            = ((0x003ffff0U & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r0) 
                                               << 4U)) 
                               | ((0x003fffffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r1) 
                                                  >> 3U)) 
                                  | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__dv_r2) 
                                     >> 0x0000000aU)));
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[3U] = 0U;
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[4U] = 0U;
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26 
                            = (0x0000007fU & (((IData)(6U) 
                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26 = 1U;
                        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en[(3U 
                                                                          & ((((IData)(6U) 
                                                                               * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                                              + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                                                             >> 5U))] 
                            = (vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_en
                               [(3U & ((((IData)(6U) 
                                         * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                        + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)) 
                                       >> 5U))] | ((IData)(1U) 
                                                   << 
                                                   (0x0000001fU 
                                                    & (((IData)(6U) 
                                                        * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt)) 
                                                       + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)))));
                    }
                    if ((5U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state 
                            = ((0U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind))
                                ? 0x27U : 6U);
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk 
                            = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk)));
                    }
                }
            } else {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0U;
            }
        } else if ((2U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
                if (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f1) 
                     | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f2) 
                        | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f3) 
                           | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f4) 
                              | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f5) 
                                 | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f6) 
                                    | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f7) 
                                       | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f8) 
                                          | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f9) 
                                             | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f10) 
                                                | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f11) 
                                                   | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f12) 
                                                      | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f13) 
                                                         | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f14) 
                                                            | ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f15) 
                                                               | (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__f0))))))))))))))))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__free_idx;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind = 0U;
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cur_note 
                        = (0x0000000fU & ((IData)(1U) 
                                          + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__free_idx)));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x25U;
                } else if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_found) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__match_idx;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind = 1U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x25U;
                } else if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_found) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt 
                        = vlSelfRef.tb_dx7_core__DOT__dut__DOT__steal_idx;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind = 1U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x25U;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                }
            } else if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_peek_empty_d) {
                if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__refresh_pending) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x11U;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk = 0U;
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x14U;
                }
            } else if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                      >> 0x00000024U)))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                    = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                        ? 0U : (0x000000ffU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                    = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                      - (IData)(1U)));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
            } else if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                      >> 0x00000023U)))) {
                if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                   >> 0x00000022U)))) {
                    if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                       >> 0x00000021U)))) {
                        if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                           >> 0x00000020U)))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                                = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                                    ? 0U : (0x000000ffU 
                                            & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                                = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                                  - (IData)(1U)));
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x10U;
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                                = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                                    ? 0U : (0x000000ffU 
                                            & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                                = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                                  - (IData)(1U)));
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__eg_mod 
                                = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                            = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                                ? 0U : (0x000000ffU 
                                        & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                            = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                              - (IData)(1U)));
                        if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                           >> 0x00000020U)))) {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_am 
                                = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                        } else {
                            vlSelfRef.tb_dx7_core__DOT__dut__DOT__ctrl_pm 
                                = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                        }
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                        = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                            ? 0U : (0x000000ffU & ((IData)(1U) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                        = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                          - (IData)(1U)));
                    if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                       >> 0x00000021U)))) {
                        if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                           >> 0x00000020U)))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__master_tune 
                                = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                        }
                    } else if ((5U >= (7U & ((IData)(
                                                     (vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                                      >> 0x00000020U)) 
                                             - (IData)(4U))))) {
                        __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 
                            = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                        __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 
                            = (7U & ((IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                              >> 0x00000020U)) 
                                     - (IData)(4U)));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v0 = 1U;
                    }
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                }
            } else if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                      >> 0x00000022U)))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                    = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                        ? 0U : (0x000000ffU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                    = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                      - (IData)(1U)));
                if ((5U >= (7U & ((IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                           >> 0x00000020U)) 
                                  - (IData)(4U))))) {
                    __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 
                        = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                    __VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 
                        = (7U & ((IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                          >> 0x00000020U)) 
                                 - (IData)(4U)));
                    vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v1 = 1U;
                }
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
            } else if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                      >> 0x00000021U)))) {
                if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                   >> 0x00000020U)))) {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                        = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                            ? 0U : (0x000000ffU & ((IData)(1U) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                        = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                          - (IData)(1U)));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__bend_pb 
                        = (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out);
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                } else {
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                        = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                            ? 0U : (0x000000ffU & ((IData)(1U) 
                                                   + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                    vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                        = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                          - (IData)(1U)));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_ctrl 
                        = (0x000000ffU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out));
                    vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_val 
                        = (0x000000ffU & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                                  >> 8U)));
                    if ((0x40U == (0x000000ffU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out)))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__sustain 
                            = (0x3fU < (0x000000ffU 
                                        & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                                   >> 8U))));
                        if ((0x3fU >= (0x000000ffU 
                                       & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                                  >> 8U))))) {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x0dU;
                        } else {
                            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                        }
                    } else if ((0x78U == (0x000000ffU 
                                          & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out)))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfffeU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfffeU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfffeU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v97 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v206 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfffdU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfffdU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfffdU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v98 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfffbU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfffbU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfffbU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v99 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfff7U & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfff7U & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfff7U & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v100 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xffefU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xffefU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xffefU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v101 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xffdfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xffdfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xffdfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v102 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xffbfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xffbfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xffbfU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v103 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xff7fU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xff7fU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xff7fU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v104 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfeffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfeffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfeffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v105 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfdffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfdffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfdffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v106 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xfbffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xfbffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xfbffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v107 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xf7ffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xf7ffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xf7ffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v108 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xefffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xefffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xefffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v109 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xdfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xdfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xdfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v110 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0xbfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0xbfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0xbfffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v111 = 1U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown 
                            = (0x7fffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained 
                            = (0x7fffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained));
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live 
                            = (0x7fffU & (IData)(vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live));
                        vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v112 = 1U;
                    } else if ((0x7bU == (0x000000ffU 
                                          & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out)))) {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x0fU;
                    } else {
                        vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
                    }
                }
            } else if ((1U & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                      >> 0x00000020U)))) {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                    = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                        ? 0U : (0x000000ffU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                    = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                      - (IData)(1U)));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note 
                    = (0x0000007fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan = 0U;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 0x0bU;
            } else {
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp 
                    = ((0xa7U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))
                        ? 0U : (0x000000ffU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp))));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_cnt 
                    = (0x000001ffU & ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_cnt) 
                                      - (IData)(1U)));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note 
                    = (0x0000007fU & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_vel 
                    = (0x0000007fU & (IData)((vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_out 
                                              >> 8U)));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 3U;
            }
        } else if ((1U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state))) {
            if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_commit) {
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[0U];
                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[0U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[0U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[0U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[0U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v6 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[0U];
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globA 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globA;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globB 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globB;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globC 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_globC;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegr 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_pegr;
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegl 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_pegl;
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_delta 
                    = ((IData)(0x00008333U) * ((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_sr1) 
                                               * ((0x00a0U 
                                                   > (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_sr1))
                                                   ? 0x0000000bU
                                                   : 
                                                  (0x0000ffffU 
                                                   & ((IData)(0x000bU) 
                                                      + 
                                                      (0x00000fffU 
                                                       & (((IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_sr1) 
                                                           - (IData)(0x00a0U)) 
                                                          >> 4U)))))));
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc1 
                    = (((IData)(0x00008333U) * (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a1)) 
                       | (- (IData)((0x63U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a0)))));
                vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_dinc2 
                    = ((- (IData)((0x63U == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a0)))) 
                       | ((0x0080U > (0xff80U & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a1)))
                           ? 0x00419980U : ((IData)(0x00008333U) 
                                            * (0xff80U 
                                               & (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__l_a1)))));
                vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit = 0U;
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[1U];
                vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7 = 1U;
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[1U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[1U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[1U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[1U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v7 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[1U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[2U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[2U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[2U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[2U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[2U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v8 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[2U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[3U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[3U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[3U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[3U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[3U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v9 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[3U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[4U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[4U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[4U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[4U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[4U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v10 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[4U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[5U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[5U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_erates__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[5U];
                __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[5U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleA__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[5U];
                vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_scaleB__v11 
                    = vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[5U];
            }
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 2U;
        } else if (vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_boundary) {
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__in_sel 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.tb_dx7_core__DOT__dut__DOT__out_sel 
                = vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel;
            vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state = 1U;
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1217 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__mixbuf__v1217 = 1U;
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1218 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1219 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1220 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1221 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1222 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1223 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1224 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1225 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1226 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1227 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1228 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1229 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1230 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1231 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1232 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1233 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1234 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1235 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1236 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1237 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1238 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1239 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1240 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1241 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1242 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1243 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1244 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1245 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1246 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1247 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1248 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1249 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1250 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1251 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1252 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1253 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1254 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1255 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1256 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1257 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1258 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1259 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1260 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1261 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1262 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1263 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1264 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1265 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1266 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1267 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1268 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1269 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1270 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1271 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1272 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1273 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1274 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1275 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1276 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1277 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1278 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1279 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
            vlSelfRef.__VdlyDim1__tb_dx7_core__DOT__dut__DOT__mixbuf__v1280 
                = (1U & (~ (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel)));
        }
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__frame_boundary 
        = ((0x01ffU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_clk)) 
           & (0x3fU == (IData)(vlSelfRef.tb_dx7_core__DOT__dut__DOT__sample_idx)));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_li = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_li;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__pbase_w = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pbase_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__pm_nobase_w 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__pm_nobase_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_level = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_level;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globA 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globA;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globC 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globC;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegr 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegr;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_pegl 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_pegl;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__in_sel = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__in_sel;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fl = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fl;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fsh = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fsh;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__amd_w = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__amd_w;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_peg = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_peg;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__bend_pb = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__bend_pb;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__master_tune 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__master_tune;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__sustain = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__sustain;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__kind = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__kind;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_used = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_used;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__refresh_pending 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__refresh_pending;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_commit 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__shd_commit;
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_opitch[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_opitch__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_fterm[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_fterm__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_erates__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_erates[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_erates__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_erates__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_elevels[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_elevels__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleA[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleA__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__shd_scaleB[vlSelfRef.__VdlyDim0__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6] 
            = vlSelfRef.__VdlyVal__tb_dx7_core__DOT__dut__DOT__shd_scaleB__v6;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__w_fa = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__w_fa;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__fq_sel = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fq_sel;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__tgt = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__tgt;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__scan = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__scan;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__lfo_rand 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__lfo_rand;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__evq_rp = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__evq_rp;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__r_rd_addr 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__r_rd_addr;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_keydown 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_keydown;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_sustained 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_sustained;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_vel = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_vel;
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v0] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v0;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__stg_dd__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_dd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__stg_dd__v1] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__stg_dd__v1;
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_globB 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__cmt_globB;
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v0][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v1][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v2][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v3][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v4][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5][0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v5][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[0U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[1U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[2U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[3U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6][4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v6[4U];
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v7][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v8][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v9][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v10][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v11][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v12][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v13][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v14][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v15][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v16][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v17][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v18][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v19][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v20][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v21][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v22][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v23][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v24][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[0U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[1U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[2U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[3U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25][4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v25[4U];
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[0U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[1U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[2U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[3U];
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd_data[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26][4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__env_cmd_data__v26[4U];
    }
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__u_router__DOT__state_q;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[0U] 
        = (((((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env.__PVT__state_q)) 
                  << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env.__PVT__state_q)) 
                            << 2U)) | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env.__PVT__state_q)) 
                                        << 1U) | (0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env.__PVT__state_q)))) 
               << 0x0000000cU) | (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env.__PVT__state_q)) 
                                     << 3U) | ((0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env.__PVT__state_q)) 
                                               << 2U)) 
                                   | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env.__PVT__state_q)) 
                                       << 1U) | (0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env.__PVT__state_q)))) 
                                  << 8U)) | ((((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env.__PVT__state_q)))) 
                                              << 4U) 
                                             | ((((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 3U) 
                                                 | ((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 2U)) 
                                                | (((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 1U) 
                                                   | (0U 
                                                      != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env.__PVT__state_q)))))) 
            << 0x00000010U) | (((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 0x0000000cU) | 
                                (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 8U)) | ((((((0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env.__PVT__state_q)) 
                                                << 3U) 
                                               | ((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 2U)) 
                                              | (((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 1U) 
                                                 | (0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env.__PVT__state_q)))) 
                                             << 4U) 
                                            | ((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env.__PVT__state_q)))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[1U] 
        = (((((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env.__PVT__state_q)) 
                  << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env.__PVT__state_q)) 
                            << 2U)) | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env.__PVT__state_q)) 
                                        << 1U) | (0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env.__PVT__state_q)))) 
               << 0x0000000cU) | (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env.__PVT__state_q)) 
                                     << 3U) | ((0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env.__PVT__state_q)) 
                                               << 2U)) 
                                   | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env.__PVT__state_q)) 
                                       << 1U) | (0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env.__PVT__state_q)))) 
                                  << 8U)) | ((((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env.__PVT__state_q)))) 
                                              << 4U) 
                                             | ((((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 3U) 
                                                 | ((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 2U)) 
                                                | (((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 1U) 
                                                   | (0U 
                                                      != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env.__PVT__state_q)))))) 
            << 0x00000010U) | (((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 0x0000000cU) | 
                                (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 8U)) | ((((((0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env.__PVT__state_q)) 
                                                << 3U) 
                                               | ((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 2U)) 
                                              | (((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 1U) 
                                                 | (0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env.__PVT__state_q)))) 
                                             << 4U) 
                                            | ((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env.__PVT__state_q)))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_busy[2U] 
        = (((((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env.__PVT__state_q)) 
                  << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env.__PVT__state_q)) 
                            << 2U)) | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env.__PVT__state_q)) 
                                        << 1U) | (0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env.__PVT__state_q)))) 
               << 0x0000000cU) | (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env.__PVT__state_q)) 
                                     << 3U) | ((0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env.__PVT__state_q)) 
                                               << 2U)) 
                                   | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env.__PVT__state_q)) 
                                       << 1U) | (0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env.__PVT__state_q)))) 
                                  << 8U)) | ((((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env.__PVT__state_q)))) 
                                              << 4U) 
                                             | ((((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 3U) 
                                                 | ((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 2U)) 
                                                | (((0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env.__PVT__state_q)) 
                                                    << 1U) 
                                                   | (0U 
                                                      != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env.__PVT__state_q)))))) 
            << 0x00000010U) | (((((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 0x0000000cU) | 
                                (((((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env.__PVT__state_q)) 
                                    << 3U) | ((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env.__PVT__state_q)) 
                                              << 2U)) 
                                  | (((0U != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env.__PVT__state_q)) 
                                      << 1U) | (0U 
                                                != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env.__PVT__state_q)))) 
                                 << 8U)) | ((((((0U 
                                                 != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env.__PVT__state_q)) 
                                                << 3U) 
                                               | ((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 2U)) 
                                              | (((0U 
                                                   != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env.__PVT__state_q)) 
                                                  << 1U) 
                                                 | (0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env.__PVT__state_q)))) 
                                             << 4U) 
                                            | ((((0U 
                                                  != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env.__PVT__state_q)) 
                                                 << 3U) 
                                                | ((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 2U)) 
                                               | (((0U 
                                                    != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env.__PVT__state_q)) 
                                                   << 1U) 
                                                  | (0U 
                                                     != (IData)(vlSymsp->TOP__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env.__PVT__state_q)))))));
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__stg_note 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__stg_note;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__fc_state 
        = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__fc_state;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__nn = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__nn;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__opk = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__opk;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_live = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__n_live;
    vlSelfRef.tb_dx7_core__DOT__dut__DOT__seq_ctr = vlSelfRef.__Vdly__tb_dx7_core__DOT__dut__DOT__seq_ctr;
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v0] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v1] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v2) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v2] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v3) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v3] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v4) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v4] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v5) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v5] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v6] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v7) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v7] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v8) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v8] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v9) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v9] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v10) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v10] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v11) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v11] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v12) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v12] = 4U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v13) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v13] = 3U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v14) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v14] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v15) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v15] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v16] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v17) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v17] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v18) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v18] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v19) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v19] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v20) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v20] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v21) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v21] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v22) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v22] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v23) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v23] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v24) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v24] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v25) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v25] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v26) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v26] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v27) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v27] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v28) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v28] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v29) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v29] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v30) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v30] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v31) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v31] = 2U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v32) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v32] = 6U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__env_cmd__v33) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__env_cmd[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__env_cmd__v33] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[0U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_alg__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_alg[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_alg__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_alg__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fbs[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fbs__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fbs__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmdep[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmdep__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[15U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fbs__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_amdep[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_amdep__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_amdep__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pmsens[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pmsens__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_opmode__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_opmode__v16] 
            = (((~ ((CData)(1U) << (IData)(__VdlyLsb__tb_dx7_core__DOT__dut__DOT__n_opmode__v16))) 
                & vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_opmode
                [__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_opmode__v16]) 
               | (0x3fU & ((CData)((IData)(__VdlyVal__tb_dx7_core__DOT__dut__DOT__n_opmode__v16)) 
                           << (IData)(__VdlyLsb__tb_dx7_core__DOT__dut__DOT__n_opmode__v16))));
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v96) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_bp__v96][__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_bp__v96] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_bp__v96;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_fq__v96) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_fq__v96][__VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_fq__v96] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_fq__v96;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_li__v96) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_li__v96][__VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_li__v96] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_li__v96;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb0__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb0__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_fb1__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_fb1__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_bp[15U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_fq[15U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_li[15U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v0) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v1) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[15U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[15U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[1U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[2U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[3U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[4U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[5U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[6U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[7U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[8U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[9U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[10U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[11U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[12U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[13U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[14U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[15U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[15U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v6) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v6;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v6;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__cmt_opitch__v7) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v7;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v8;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v9;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v10;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_fterm[5U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_fterm__v11;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v7;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v8;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v9;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v10;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__cmt_elevels[5U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__cmt_elevels__v11;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_vel[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_vel__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_vel__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[0U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[1U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[2U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[3U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[4U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[5U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[6U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[7U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[8U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[9U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[10U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[11U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[12U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[13U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[14U][5U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][0U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][1U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][2U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][3U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][4U] = 0U;
        tb_dx7_core__DOT__dut__DOT__n_dd[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_midi__v16) {
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v192][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v192;
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v193][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v193;
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v194][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v194;
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v195][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v195;
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v196][4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v196;
        tb_dx7_core__DOT__dut__DOT__n_dd[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_dd__v197][5U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_dd__v197;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v96) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v96][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v96;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v97) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v97][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v97;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v98][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v98;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v99) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v99][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v99;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v100) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v100][4U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v100;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_ams__v101) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v96][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v96;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v97][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v97;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v98][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v98;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegr[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegr__v99][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegr__v99;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v96][0U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v96;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v97][1U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v97;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v98][2U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v98;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegl[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_pegl__v99][3U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegl__v99;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_ams[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__n_ams__v101][5U] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_ams__v101;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[0U] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[1U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[2U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[3U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[4U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[5U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[6U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[7U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[8U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[9U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[10U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[11U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[12U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[13U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[14U] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[15U] = 1U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v16) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v16] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__n_pegix__v16;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[6U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[7U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[8U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[9U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[10U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[11U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[12U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[13U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[14U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[15U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v32) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v33] = 3U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v33) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v34] = 3U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v34) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v35] = 3U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_pegix__v36) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegdwn[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegdwn__v35] = 1U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_pegix[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_pegix__v36] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v96) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v96][__VdlyDim0__tb_dx7_core__DOT__dut__DOT__op_ph__v96] 
            = __VdlyVal__tb_dx7_core__DOT__dut__DOT__op_ph__v96;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v80) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_bp__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[3U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[4U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[5U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[6U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[7U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[8U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[9U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[10U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[11U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[12U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[13U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[14U][5U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[15U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v193) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb0[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb0__v33] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__n_fb1[__VdlyDim0__tb_dx7_core__DOT__dut__DOT__n_fb1__v33] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v193][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v194][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v195][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v196][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v197][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v198][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v199) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v199][0U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v200][1U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v201][2U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v202][3U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v203][4U] = 0U;
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[__VdlyDim1__tb_dx7_core__DOT__dut__DOT__op_ph__v204][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v97) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__op_ph__v206) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][1U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v98) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][2U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v99) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][3U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v100) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v101) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[0U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v102) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v103) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][1U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v104) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][2U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v105) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][3U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v106) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][4U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v107) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[1U][5U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v108) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][0U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v109) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][1U] = 0U;
    }
    if (vlSelfRef.__VdlySet__tb_dx7_core__DOT__dut__DOT__n_sil__v110) {
        vlSelfRef.tb_dx7_core__DOT__dut__DOT__op_ph[2U][2U] = 0U;
    }
}
