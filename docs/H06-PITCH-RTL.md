# H06 — Pitch/modulation RTL under the H03 host/core split (issue #28)

Issue: #28 (epic #2) · Date: 2026-09-21 · Planning ID: H06
Depends on: H03 (issue #25, contract), N08 (issue #24, frozen release),
N05 (issue #19, pitch model) · RTL: `rtl/pitch_mod.v` · Bench:
`rtl/tb_pitch_mod.v` · Conformance: `tools/h06_compare.py` · Synthesis:
`tools/h06_synth.py` · Tests: `tests/test_h06.py` · Evidence:
`evidence/h06-pitch-rtl/`

**Claim advanced:** the pitch/modulation path and its host-boundary
semantics are pinned in RTL exactly as contracted: the unit is
**bit-exact to the frozen N05 integer model** on a directed 4,332-frame
suite (plus 1,138+ controlled-mutation failures proving resolution),
consumes **only per-event frozen 32-bit register writes** from the host
(no per-sample host dependency, DEC-010), applies events at the H03
frame boundary, and its worst-case host link traffic fits the 24.576 MHz
design-point budget. It survives gf180mcu 7t synthesis with every output
observable at a mapped **2,459 flops / 58,904 cells / 1.195 mm²**
(1,195,376.4 µm², sequential 156,541.9 µm²).

**Claim boundary:** this is the *pitch/mod unit probe* — one note slot,
no frame controller, no op core, no storage banks, no polyphony
scheduler (H07 integrates; the multi-note event stream serialization is
H07's). RTL-vs-frozen-model exactness is the bar here; model-vs-
pinned-reference agreement is N05's measured result, and neither implies
original-DX7 fidelity or musical usefulness (DEC-001, DEC-024, plan §7).

---

## 1. What was built

`rtl/pitch_mod.v` — one per-note pitch/modulation datapath, RTL-exact to
`src/gf180_dx7/model/pitch.py` (contract `gf180-dx7-integer-v1`,
docs/N05-PITCH-MODEL.md), composed in the pinned integrated order of
`src/gf180_dx7/model/integrated.py::compute_frame` (seam finding F-3,
DR-0006): **ratio-mode operators take `basepitch + i32(pitch_mod_nobase
+ pitch_base)`; fixed-mode operators take `basepitch + pitch_base`
only** — pitch EG, LFO PM and controller PM reach ratio ops; bend + master
tune reach everything.

Per-frame arithmetic (one frame = one control-tail pass over the unit;
each step mirrors the cited model file):

| Stage | Model semantics | Registers |
|---|---|---|
| Command apply | note-on: `PitchEnv.set` + `advance(0)` (pitchenv.cc:45-53), LFO `keydown()` (lfo.cc:92-97: sync restarts `phase_ = 2^31-1`, `delaystate_ = 0` always); note-off: `keydown(false)` → `advance(3)`; commit: shadow→active swap (see §2) | `note_logfreq_q`, `opmode_q`, PEG state, LFO phase/dstate |
| Note-on derivation | `l' = midinote_to_logfreq(note + transpose − 24)` (tuning.cc); per-op basepitch: ratio `i32(i32(l'+delta) + COARSEMUL[coarse&31])` (+ host-frozen fine term when fine ≠ 0); fixed `(4458616·((coarse&3)·100+fine))>>3` (+13457·(detune−7) iff detune>7) (dx7note.cc:30-64) | `basepitch_q[0..5]` |
| LFO step | one `getsample()` (six waveforms incl. the regenerated SIN_DELTA table for sine and the S&H LCG `rand_ = (rand·179+17) & 0xFF`) + one `getdelay()` delay ramp with the 2^31 branch and 2^32 saturation (lfo.cc:51-90) | `lfo_phase`, `lfo_dstate`, `lfo_rand`, `lfo_val_q`, `lfo_del_q` |
| Pitch-EG step | one `getsample()`: additive `level_ ± inc_` against `PITCHENV_TAB[l]<<19`, `inc_ = PITCHENV_RATE[r]·1050`, wrap-then-signed-compare, clamp+advance (pitchenv.cc:55-93) | `peg_lvl`, `peg_tgt`, `peg_inc`, `peg_ix`, `peg_rising`, `peg_down`, `peg_out_q` |
| PM/AM composition | `pmd = PM_DEPTH·lfo_delay` (u32); `senslfo = i32(PM_SENS·(lfo_val−2^23))`; `pmod_1 = |i32((pmd·senslfo)>>39)|`; `pmod_2 = |i32((ctrl_pm·senslfo)>>14)|`; `pm_nobase = i32(peg + max·sign)`; `pitch_base = i32(bend_pb + master_tune)`; AM depth composition `amd_mod` (dx7note.cc:210-217, 259-268) | `pm_nobase_q`, `pitch_base_q`, `amd_q` |
| Freqlut ×6 | linear interpolation over the low 14 bits, `y = y0 + i32((y1−y0)·lowbits>>14)`, final `y >> (20 − hibits)`; ratio input `i32(bp + i32(pm_nobase + pitch_base))`, fixed input `i32(bp + pitch_base)` (freqlut.cc:46-55) | `faddr_q`, `flsb_q`, `fsh_q`, `freq_q[0..5]` |

FSM: `IDLE → [COMMIT] → [NN → NNB×6 | NO] → LFO → PEG → PM → (FA→FB)×6
→ OUT → FIN` — **worst-case 25 clocks per frame measured** (bench
`maxcycles`, §4), well inside the H03 `derived_estimate` rows this unit
covers (12 + 100 + 8 + 48 = 168 clk/note/frame; §6).

ROMs: `$readmemh` from `reference/tables/freqlut_table.hex` — the SAME
pinned bytes the model verifies by SHA-256 in
`reference/tables/manifest.json` (freqlut `6d3c5970…`, generator
`tools/gen_tables.py --check`) — and `reference/tables/lfo_sin_table.hex`,
the regenerated msfa Sin table (SIN_DELTA form, sin.cc:31-56 + sin.h)
the model rebuilds with `build_sintab()`; the harness re-verifies both
byte-for-byte on every run. Both ROMs dissolve to combinational logic in
synthesis (no BRAM in this flow; §6).

Declared state: 1,559 register bits + 86,048 ROM-init bits (two arrays);
2,459 mapped flops (yosys exploits reset-value constants and ROM-content
redundancy; behavior is the contract and it is proven bit-exact, §4).
Observability is load-bearing: every state bit feeds the output ports or
the registered XOR `state_obs` fold (H01/H02 pattern).

## 2. The host/core split (docs/CONTRACT-CORE-v1.md, H03 sections 4.2/4.7/5)

The core advances the audio-rate pitch path autonomously: pitch-EG
stepping, LFO sampling, PM/AM depth composition, transpose application
and the Freqlut lookups all live in the unit, stepped once per
64-sample frame from the frame controller's **payload-less** control-tail
tick. The host sends **only per-event frozen 32-bit register writes** —
one write per contracted register, per-event only (DEC-010; NUM-008
`event_time_pitch_math`: "osc_freq detune/fine … host/control-side float
math off the audio frame budget").

| Writes | Register(s) | Content | Class |
|---|---|---|---|
| 0x00 | CMD | `{note[15:8], cmd[7:0]}`; 1 note-on, 2 note-off, 3 commit | event |
| 0x10-0x15 | OP_CFG | `{detune, fine, coarse, mode}` raw patch bytes | patch, shadowed |
| 0x18-0x1D | OP_FTERM | fine term, host-frozen int32 (dx7note.cc:50-53 float gate) | patch, shadowed |
| 0x20/0x21 | PEG_RATES/LEVELS | `{r3,r2,r1,r0}` / `{l3,l2,l1,l0}` bytes | patch, shadowed |
| 0x22 | LFO_PARAMS | `{wave, sync, delay, rate}` | patch, shadowed |
| 0x23/0x24 | PM_DEPTH/PM_SENS | frozen `(byte·165)>>6` / `PITCHMODSENSTAB[byte&7]` | patch, shadowed |
| 0x25 | TRANSPOSE | patch transpose byte | patch, shadowed |
| 0x26 | AM_DEPTH | frozen `(byte·165)>>6` | patch, shadowed |
| 0x28-0x2C | BEND_PB/MASTER_TUNE/CTRL_PM/CTRL_AM/EG_MOD | performance integers; BEND_PB is the frozen scaled bend (NUM-008 float gate); EG_MOD resets to 127 (pinned `Controllers::refresh` default) | perf, direct (DEC-022) |
| 0x30-0x35 | OP_DDELTA | per-op detune delta, host-frozen int32 | note-on payload |

Timing: patch-shadowed registers swap into the active image **only** on
CMD_COMMIT (atomic at the frame tick, DEC-019); performance settings are
separate state applied at event time (DEC-022); commands pending at a
tick apply **commit first, then the note event** — an event received
during frame N is applied before the first sample of frame N+1 (DEC-014,
NUM-011). A pending commit + note-on is supported (the note reads the
committed image). Reset state is explicit (DEC-011, DEC-020): LFO
phase/rand/delaystate zeroed (the model's declared init), PEG at
`PitchEnv()` init, all parameters zero, bend centered, EG_MOD 127.

**Boundary-value exactness of the detune split.** The float-gated detune
step (dx7note.cc:46-47) is computed host-side: `delta = detuned − l'`
with `detuned = int(l' + ratio·l'·(detune−7))` in pinned float64. The RTL
computes `l'` itself — the **patch transpose is applied exactly once,
inside the core** (contract 4.2, DEC-023) — and reconstructs
`i32(l' + delta)`; both sides derive `l'` from the same contracted
documented-mode shift, so the reconstruction is exact (mod-2^32
arithmetic; verified exhaustively by construction and by the suite's
69 shifted-note-on cases + 20,000-case model check at authoring time).
The fixed-mode formula is pure integer and fully core-side. The host's
delta computation *mirrors* the documented shift to produce its frozen
integers (DEC-010 patch-to-core conversion); the pitch-shifting
application happens exactly once, in the core.

**Transpose ruling.** The contract fixes the ruling (H03 §4.2: "the
patch's own parameters inside the core, host MIDI transposition
host-side"; DEC-023), so the RTL implements the **documented** mode,
`shift = transpose − 24`, applied once at note-on. The pinned-oracle
wrapper's omission of that shift (registry finding `perf-transpose-12`)
is a **wrapper finding, not RTL scope**: N08 `perf-transpose-12` remains
the wrapper-level conformance reference and is deliberately not
reproduced here; the suite covers 69 shifted-note-on frames against the
documented-mode golden instead.

## 3. Link budget (issue #28 acceptance)

Writes per event: note-on = 6 detune deltas + 1 command = **7**;
note-off = 1; controller event = 1. The worst admissible burst
(16 note-ons + 16 note-offs + 16 controller events, contract §2) costs
**144 writes ≤ 168** = the 24.576 MHz design-point capacity
(floor(32768/194)); the committed vector stream's worst inter-frame
window measures **91 writes** ≤ 168. A full patch commit costs
**21 writes ≤ 216** (the declared §4.1 load cost). Bounded observation:
this burst would **not** fit the 12.288 MHz fallback's 84-write
capacity — recorded here because honesty beats averages; it changes
nothing: 12.288 MHz already FAILS the worst-case frame budget (H03
headline finding), and the stop/escalate condition of issue #25/#28 does
not trigger because the design point respects the budget. Changing the
split remains an H03 decision-record event; no contract file was edited.

## 4. Equivalence to the frozen model (bit-exact, no tolerance)

`tools/h06_compare.py` generates the directed vector set from the
PYTHON model (the golden — the harness plays the HOST role and streams
the contracted per-event writes; the model judges every frame), runs
`rtl/tb_pitch_mod.v` under iverilog, and compares the six 32-bit phase
increments + pitch-EG level + LFO sample/delay + AM composition
**bit-exact** with first-mismatch localization (frame index, scenario
tag, note, transpose, LFO params, bend, PEG rates/levels, per-op
basepitch, per-field diff).

**Result: PASS — 4,332 frames (4,328 note-active), 0 mismatches; 12,645
contracted writes; worst control-tail latency 25 clk/frame** (issue
requires ≥ 4,000 evals). Coverage (reported separately from agreement):

| Dimension | Measured |
|---|---:|
| Frames / note-active frames | 4,332 / 4,328 |
| LFO waveform frames (waves 0-5) | 1,892 / 560 / 460 / 470 / 490 / 460 |
| Fixed-mode-op frames | 1,138 |
| coarse 0 / coarse 31 frames | 528 / 508 |
| detune 0 / detune 14 frames | 564 / 594 |
| Keysync note-ons (phase restarts) | 421 |
| Documented-transpose shifted note-ons | 69 |
| Bend events (frozen-integer scaling) | 57 |
| Frames with **zero** host traffic | 3,424 |
| Negative logfreq lookups / distinct freqlut indexes | 169 / 917 of 1,024 |
| Distinct final shift amounts (hibits values) | 21 |
| Distinct scenario blocks | 74 (static matrix, transpose, bend, 8 PEG shapes, 48+18 LFO runs, ctrl-PM, AM, 700 seeded-random frames, burst) |

Scenario highlights: `osc-fixed-carrier`-style fixed/keyboard-invariance
directed cases; `bd-coarse0/31`, `bd-detune0/14`, `bd-fine99` corners;
`peg-up/peg-down` fast/slow/rate3/nonzero-final/retrigger/repeated-note;
`lfo-wave0..5 × speed × delay99 × keysync`; `lfo-pm-sens7` depth
composition; documented nonzero-wheel-range controller-PM path (the
pinned-default CC1 inertness, registry `ev-modwheel-127`, is a host-side
range-0 fact and needs no RTL case); AM depth composition corners;
reset-state frames before any traffic (DEC-011 repeatability); and a
burst window with 8 note-on/off pairs + a note-on inside one inter-frame
gap (91 writes, last command wins, ≤ 1 frame latency). Freqlut's
`hibits > 20` domain is excluded by construction: beyond it the frozen
model's own lookup raises (freqlut.cc:44-45 note); the RTL's shift clamp
is unreachable inside the modeled domain. `abs()` on INT_MIN is
unreachable (model deviation note 2). Golden vector generation is
deterministic (fixed seed; byte-identical regeneration test-enforced).

Tool identity (sim): Icarus Verilog 13.0 (stable) (`iverilog -g2012`),
vvp; golden = `src/gf180_dx7/model/pitch.py` on CPython 3.x with the
pinned freqlut LUT bytes loaded via `tables.load_pinned`; hashes of RTL,
vectors and expected are in `evidence/h06-pitch-rtl/compare_report.json`.

## 5. Negative controls (all live; each demonstrably fails its check)

| Control | Mechanism | Result |
|---|---|---|
| 1-LSB increment mutation (issue-named boundary-value control) | `-DMUTATE_INCREMENT_LSB`: op-0 increment +1 LSB every frame | MET — 4,332 mismatches |
| Fixed mode tracks keyboard (N05 named control) | `-DMUTATE_FIXED_TRACKS_KEYBOARD`: fixed basepitch gains `l'` | MET — 1,138 mismatches |
| LFO sync ignored (N05 named control) | `-DMUTATE_LFO_SYNC_IGNORED`: keydown keeps `phase_` | MET — 3,939 mismatches |
| Per-sample host dependency (split violation) | `-DH06_PER_SAMPLE_HOST`: an illegal per-sample host stream port gates the increment commit; the harness never streams → increments stall | MET — 4,332 mismatches |
| Strip observability (synthesis) | `-DH06_STRIP_OBSERVABILITY`: all outputs tied off → yosys deletes every state flop → the flop gate FAILS | MET — 0-flop strip build |

Plus the **static interface guard** (`tests/test_h06.py`): the default
build's input ports must equal the contracted per-event set
{clk, rst, cfg_wr, cfg_addr, cfg_wdata, frame_tick} exactly; the same
guard FAILS the `-DH06_PER_SAMPLE_HOST` build's port list
(host_stream_valid), proving resolution. A guard that passed anything
would be no control.

## 6. Mapped result (measured, real cells) and control-tail cost

Flow (per issue #28): `read_verilog; hierarchy -check -top pitch_mod;
synth -top pitch_mod; dfflibmap -liberty <7t>; abc -liberty <7t>;
stat -liberty <7t>`.

| Item | Value | Class |
|---|---|---|
| Yosys | 0.69+post, git sha1 `143eb14f9cc…`, native macOS (same install H01/H02 measured) | measured identity |
| Liberty | `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`, ciel commit `54435919ab…` | measured identity |
| Mapped flops | **2,459** (declared 1,559 state bits + 86,048 ROM-init bits; > 192 floor gate) | measured |
| Total mapped cells | **58,904** | measured |
| Chip area | **1,195,376.4 µm² = 1.195 mm²** all-in (sequential 156,541.9 µm²) | measured |
| ROM representation | **logic** — freqlut 1025×32 and LFO-sin 2048×26 dissolve to combinational gates (MEMORY_MAP; no BRAM) | measured |
| Constraints | none (no SDC, no clock — probe flow) | declared |

The mapped cost is dominated by the two ROMs-as-logic and the seven
constant/variable multiplies the pinned semantics require (pmd·senslfo,
ctrl·senslfo, the AM products, the freqlut interpolation); it is one
per-note unit's probe datapoint, not a chip-fit claim. Control-tail
cost: the unit measures **25 clk/frame worst case** (≈ 16,875 clk/s at
750 frames/s) against the 168 clk H03 budgeted for this unit's
`derived_estimate` rows — recorded as the measured input for H04/H07's
integration; the budget tool is NOT re-run and no contract file is
edited (STALE rules, H03 §6).

## 7. What this does NOT say

- **No place-and-route, no timing closure, no utilization, no
  routability, no fit claim of any kind** — no clock constraint was
  applied; every nanosecond number is unmeasured and clock closure at
  the 24.576 MHz design point remains H10's measurement (D01 §2).
- **No full-chip or shared-core claim** — this is one note slot: the
  16-note schedule, per-frame slot allocation, storage-bank integration
  and multi-note event serialization are H07's; the unit's 25 clk/frame
  is a component measurement, not a schedule proof.
- **No post-synthesis netlist re-simulation** — the equivalence proof is
  on the pre-synthesis RTL; gate-level LEC is recorded as future work
  (H01 §5 pattern).
- **No original-DX7 fidelity, preset-quality, or musical-usefulness
  claim** — bit-exactness to the frozen model is the bar here;
  model-vs-pinned-reference is N05's result; listening records are the
  only audibility evidence (AGENTS.md; the three claims stay separate).
- CI re-runs without iverilog/vvp/yosys/the local ciel install are
  guarded skips (NOT_RUN, naming the missing path), never silent passes.

## 8. Uncertainties

1. The mapped flop count (2,459) and area are yosys-version-dependent
   (constant/ROM redundancy exploitation); the acceptance gate is the
   principled > 192 floor plus area > 0 plus the strip control at 0.
2. The 25 clk/frame figure covers this unit only; H07's integration adds
   arbitration, storage access and the event frontend.
3. The host-side frozen integers (detune deltas, fine terms, bend scale)
   assume the host performs pinned float64/float32 math exactly as
   NUM-008 declares; the harness reproduces it by construction. A host
   that approximates these would break exactness at the seam — that is a
   U04/host-conformance concern, recorded here as the boundary contract.
4. Fixed-mode `coarse&3`, ratio `coarse&31`, PEG index and LFO-waveform
   domains are enforced by the patch-byte domains (0..99/0..127); the
   RTL masks indexes but out-of-domain bytes are outside the modeled
   configuration.
5. `abs()` on INT_MIN and `hibits > 20` remain the declared unreachable
   corners (mirroring the model's own notes); the RTL's behavior there
   is defined but unverified by vectors.

## 9. Evidence and reproduction

| Artifact | Role |
|---|---|
| `evidence/h06-pitch-rtl/compare_report.json` | equivalence + 4 negative-control gates, coverage stats, tool identities (sha256s inside) |
| `evidence/h06-pitch-rtl/vectors.txt` / `expected.txt` | committed golden vector set (deterministic regeneration is test-enforced) |
| `evidence/h06-pitch-rtl/actual.txt` / `actual_*.txt` | RTL outputs, real build and the four mutant builds |
| `evidence/h06-pitch-rtl/yosys_full.log` | full mapped transcript (stat inside) |
| `evidence/h06-pitch-rtl/yosys_strip.log` | strip-mode transcript — state removed |
| `evidence/h06-pitch-rtl/synth_report.json` | gates, runs, declared-vs-mapped accounting, tool/liberty/RTL identities |
| `reference/tables/lfo_sin_table.hex` | regenerated LFO sine table (SIN_DELTA), byte-verified against `build_sintab()` on every run, byte-pinned in `docs/reuse/catalog.json` |

Reproduce:

```sh
python3 tools/h06_compare.py          # golden gen + iverilog sim + bit-exact compare + 4 mutant controls
python3 tools/h06_synth.py            # yosys full + strip + gates + report
python3 -m unittest tests.test_h06 -v # all gates, guards, controls, determinism
```

Statuses: equivalence **PASS** (bit-exact, 4,332 frames, 0 mismatches);
negative controls **MET** (1-LSB 4,332 / fixed-keyboard 1,138 /
LFO-sync 3,939 / per-sample-stream 4,332 mismatches; strip gate fails as
required); synthesis full gate **PASS** (2,459 flops, 1.195 mm², all
outputs observable); interface guard **PASS** with demonstrated
resolution; link budget **PASS** at the 24.576 MHz design point
(144 ≤ 168; 12.288 MHz shortfall recorded, consistent with the H03 FAIL
verdict); P&R, timing closure, post-synthesis netlist re-simulation,
16-note integration, CI-without-artifacts: **NOT_RUN / out of scope by
design**.
