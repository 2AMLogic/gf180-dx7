// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VTB_DX7_CORE_H_
#define VERILATED_VTB_DX7_CORE_H_  // guard

#include "verilated.h"

class Vtb_dx7_core__Syms;
class Vtb_dx7_core___024root;
class Vtb_dx7_core_env_unit;


// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) Vtb_dx7_core VL_NOT_FINAL : public VerilatedModel {
    friend class Vtb_dx7_core__Syms;
  private:
    // Symbol table holding complete model state (owned by this class)
    Vtb_dx7_core__Syms* const vlSymsp;
    // Evaluation loop
    VerilatedEvalLoop m_evalLoop;

  public:

    // CONSTEXPR CAPABILITIES
    // Verilated with --trace?
    static constexpr bool traceCapable = false;

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__0__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__1__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__2__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__3__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__4__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__5__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__6__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__7__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__8__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__9__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__10__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__11__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__12__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__13__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__14__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__15__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__16__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__17__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__18__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__19__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__20__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__21__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__22__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__23__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__24__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__25__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__26__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__27__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__28__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__29__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__30__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__31__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__32__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__33__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__34__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__35__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__36__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__37__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__38__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__39__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__40__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__41__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__42__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__43__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__44__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__45__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__46__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__47__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__48__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__49__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__50__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__51__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__52__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__53__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__54__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__55__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__56__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__57__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__58__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__59__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__60__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__61__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__62__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__63__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__64__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__65__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__66__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__67__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__68__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__69__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__70__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__71__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__72__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__73__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__74__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__75__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__76__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__77__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__78__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__79__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__80__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__81__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__82__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__83__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__84__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__85__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__86__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__87__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__88__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__89__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__90__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__91__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__92__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__93__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__94__KET____DOT__u_env;
    Vtb_dx7_core_env_unit* const __PVT__tb_dx7_core__DOT__dut__DOT__envs__BRA__95__KET____DOT__u_env;

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vtb_dx7_core___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vtb_dx7_core(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vtb_dx7_core(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vtb_dx7_core();
  private:
    VL_UNCOPYABLE(Vtb_dx7_core);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedTraceBaseC* tfp, int levels, int options = 0) { contextp()->trace(tfp, levels, options); }
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
  private:

    // Internal functions - the model's evaluation entry points
    void evalBegin() override final;
    void evalEnd() override final;
    void evalStatic() override final;
    void evalInitial() override final;
    bool evalStl(bool firstIteration) override final;
    void evalSample() override final;
    bool evalIco(bool firstIteration) override final;
    bool evalAct() override final;
    bool evalInact() override final;
    bool evalNba() override final;
    bool evalObs() override final;
    bool evalReact() override final;
    void evalPostponed() override final;
    void evalFinal() override final;
    void dumpTriggersStl() override final;
    void dumpTriggersIco() override final;
    void dumpTriggersAct() override final;
    void dumpTriggersNba() override final;
    void dumpTriggersObs() override final;
    void dumpTriggersReact() override final;

    // Internal functions - trace registration
    void traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options);
};

#endif  // guard
