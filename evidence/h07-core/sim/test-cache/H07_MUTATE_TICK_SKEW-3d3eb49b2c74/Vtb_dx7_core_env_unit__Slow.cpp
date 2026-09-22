// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_dx7_core.h for the primary calling header

#include "Vtb_dx7_core__pch.h"

void Vtb_dx7_core_env_unit___ctor_var_reset(Vtb_dx7_core_env_unit* vlSelf);

Vtb_dx7_core_env_unit::Vtb_dx7_core_env_unit() = default;
Vtb_dx7_core_env_unit::~Vtb_dx7_core_env_unit() = default;

void Vtb_dx7_core_env_unit::ctor(Vtb_dx7_core__Syms* symsp, const char* namep) {
    vlSymsp = symsp;
    vlNamep = strdup(Verilated::catName(vlSymsp->name(), namep));
    // Reset structure values
    Vtb_dx7_core_env_unit___ctor_var_reset(this);
}

void Vtb_dx7_core_env_unit::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

void Vtb_dx7_core_env_unit::dtor() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
