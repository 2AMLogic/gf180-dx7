// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_dx7_core__pch.h"

//============================================================
// Constructors

Vtb_dx7_core::Vtb_dx7_core(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_dx7_core__Syms(contextp(), _vcname__, this)}
    , m_evalLoop{*this, /*convergeLimit:*/ 10000}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env}
    , __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env{vlSymsp->TOP.__PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_dx7_core::Vtb_dx7_core(const char* _vcname__)
    : Vtb_dx7_core(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_dx7_core::~Vtb_dx7_core() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_dx7_core___024root___eval_debug_assertions(Vtb_dx7_core___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_static(Vtb_dx7_core___024root* vlSelf);
void Vtb_dx7_core___024root___eval_initial(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD bool Vtb_dx7_core___024root___eval_stl(Vtb_dx7_core___024root* vlSelf, CData/*0:0*/ firstIteration);
void Vtb_dx7_core___024root___eval_sample(Vtb_dx7_core___024root* vlSelf);
bool Vtb_dx7_core___024root___eval_ico(Vtb_dx7_core___024root* vlSelf, CData/*0:0*/ firstIteration);
bool Vtb_dx7_core___024root___eval_act(Vtb_dx7_core___024root* vlSelf);
bool Vtb_dx7_core___024root___eval_inact(Vtb_dx7_core___024root* vlSelf);
bool Vtb_dx7_core___024root___eval_nba(Vtb_dx7_core___024root* vlSelf);
bool Vtb_dx7_core___024root___eval_obs(Vtb_dx7_core___024root* vlSelf);
bool Vtb_dx7_core___024root___eval_react(Vtb_dx7_core___024root* vlSelf);
void Vtb_dx7_core___024root___eval_postponed(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_final(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__stl(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__ico(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__act(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__nba(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__obs(Vtb_dx7_core___024root* vlSelf);
VL_ATTR_COLD void Vtb_dx7_core___024root___eval_dump_triggers__react(Vtb_dx7_core___024root* vlSelf);

void Vtb_dx7_core::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_dx7_core::eval_step\n"); );
    m_evalLoop.eval();
}

void Vtb_dx7_core::evalBegin() {
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_dx7_core___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
}

void Vtb_dx7_core::evalEnd() {
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
    vlSymsp->TOP.__VdlySched.cleanupForevered();
}

void Vtb_dx7_core::evalStatic() {
    Vtb_dx7_core___024root___eval_static(&(vlSymsp->TOP));
}

void Vtb_dx7_core::evalInitial() {
    Vtb_dx7_core___024root___eval_initial(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalStl(bool firstIteration) {
    return Vtb_dx7_core___024root___eval_stl(&(vlSymsp->TOP), firstIteration);
}

void Vtb_dx7_core::evalSample() {
    Vtb_dx7_core___024root___eval_sample(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalIco(bool firstIteration) {
    return Vtb_dx7_core___024root___eval_ico(&(vlSymsp->TOP), firstIteration);
}

bool Vtb_dx7_core::evalAct() {
    return Vtb_dx7_core___024root___eval_act(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalInact() {
    return Vtb_dx7_core___024root___eval_inact(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalNba() {
    return Vtb_dx7_core___024root___eval_nba(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalObs() {
    return Vtb_dx7_core___024root___eval_obs(&(vlSymsp->TOP));
}

bool Vtb_dx7_core::evalReact() {
    return Vtb_dx7_core___024root___eval_react(&(vlSymsp->TOP));
}

void Vtb_dx7_core::evalPostponed() {
    Vtb_dx7_core___024root___eval_postponed(&(vlSymsp->TOP));
}

void Vtb_dx7_core::evalFinal() {
    Vtb_dx7_core___024root___eval_final(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersStl() {
    Vtb_dx7_core___024root___eval_dump_triggers__stl(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersIco() {
    Vtb_dx7_core___024root___eval_dump_triggers__ico(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersAct() {
    Vtb_dx7_core___024root___eval_dump_triggers__act(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersNba() {
    Vtb_dx7_core___024root___eval_dump_triggers__nba(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersObs() {
    Vtb_dx7_core___024root___eval_dump_triggers__obs(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vtb_dx7_core::dumpTriggersReact() {
    Vtb_dx7_core___024root___eval_dump_triggers__react(&(vlSymsp->TOP));
}

//============================================================
// Events and timing
bool Vtb_dx7_core::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty() && !contextp()->gotFinish(); }

uint64_t Vtb_dx7_core::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_dx7_core::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

VL_ATTR_COLD void Vtb_dx7_core::final() {
    contextp()->executingFinal(true);
    evalFinal();
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_dx7_core::hierName() const { return vlSymsp->name(); }
const char* Vtb_dx7_core::modelName() const { return "Vtb_dx7_core"; }
unsigned Vtb_dx7_core::threads() const { return 1; }
void Vtb_dx7_core::prepareClone() const { contextp()->prepareClone(); }
void Vtb_dx7_core::atClone() const {
    contextp()->threadPoolpOnClone();
}
