# D01 — Physical constraint inventory

Issue: #5 (epic #1). Date: 2026-09-19. Planning ID: D01.

This document names the candidate board, core clock options, target
shuttle/padframe, cell library and memory options for the gf180-dx7 core, and
labels every item **confirmed**, **assumed** or **inaccessible**:

- **confirmed** — a source path plus a measurement, a committed evidence
  record, or a check re-run and recorded here. The claim states exactly what
  the source establishes and no more.
- **assumed** — a working hypothesis with no measurement behind it. Assumptions
  are listed so they are visible; they are not constraints and must not be
  treated as one.
- **inaccessible** — the access or artifact is missing. Missing access is
  reported as missing, never worked around and never replaced by an invented
  number.

Sibling-repo numbers are quoted from their committed documents and evidence
files (read and verified for this inventory, 2026-09-19); they are measurements
of *those* designs, not of the DX7 core. Where a number is a measurement, a
synthesis, a derivation or a projection, the source document's own labelling is
preserved.

Machine-readable subset: `spec/physical-constraints-v1.json`
(validated by `tests/test_physical_constraints.py`).

---

## 1. Shuttle and padframe

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| wafer.space gf180mcu quarter-slot MPW seat | confirmed | gf180-parasynth:docs/pnr-synth-top.md | Die 1314.88 × 1317.12 µm = **1.7319 mm² inside the default pad ring** (core 1.6734 mm²), used as the fixed-die input of the sibling's routed runs. It is a recorded constraint and budget premise — **this repo has not purchased a seat**. |
| TinyTapeout TTGF26c (gf180mcuD, wafer.space MPW) | confirmed as of 2026-09-16 | prep-tinytapeout:REPORT.md | Live shuttle-API check on 2026-09-16: deadline 2026-12-07 20:00 UTC, 160 tiles (158 available), sales `paused` at that date, delivery estimate early 2027. Re-check before relying; dates and availability drift. |
| TTGF26c tile geometry (padframe candidate) | confirmed | prep-tinytapeout:REPORT.md | GF180 tile dies (from `tile_sizes.yaml`): 1×1 = 346.64 × 160.72 µm; largest 4×4 = 1440.32 × 736.96 µm ≈ **1.06 mm²**; up to 16 tiles per design. Chip-level PDN forbids Metal5; `RT_MAX_LAYER` = Metal4; pins pre-placed on Metal4 top edge. |
| Which packaging this product targets | assumed | 2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md | Plan "Physical target" row: gf180mcu feasibility first, "no slot-fit assertion yet". Choosing TT tiles vs a direct quarter-slot seat is undecided (open question §10). |

The TT tile dies and the wafer.space direct quarter slot are two different
packagings of the same foundry MPW. Their fit questions are different
questions; neither has a DX7-core answer (§9).

## 2. Core clock candidates

The frame budget the clocks must serve (plan §4):
16 notes × 6 operators × 48,000 frames/s = **4,608,000 operator evaluations/s**.
At 24.576 MHz that is 512 clocks per 48 kHz frame, ≈ 5.33 clocks per operator
over all 96 operator states; at 12.288 MHz, ≈ 2.67 — before envelope work,
memory access, mixing, link handling and pipeline effects.

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| 12.288 MHz (81.38 ns) | confirmed as plan candidate + sibling-closure measurement | 2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md · gf180-parasynth:pnr/orfs/evidence/synth_top/joined-d1e5068/ | The sibling's joined gf180 chip routed at this clock: setup +10.173 ns at `ss_125C_4v50`, +43.592 ns at `tt_025C_5v00`, hold positive at all three corners. Implied min period 71.21 ns (14.0 MHz) at ss. **A different design; not a DX7-core result.** |
| 24.576 MHz (40.69 ns) | confirmed as plan candidate; ss closure **assumed, not established** | 2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md · gf180-parasynth:docs/pnr-synth-top.md | Plan candidate (512 clocks/frame). On the sibling design the same run implies 26.5 MHz at tt — the period would meet tt/ff but **miss the ss corner** (71.21 ns implied min period > 40.69 ns). Whether a DX7 core closes 24.576 MHz at ss is unknown until H10. |
| Frame-budget arithmetic | confirmed | 2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md | 4.608 M operator evaluations/s; 512 / 256 clocks per frame at 24.576 / 12.288 MHz. |
| TT voltage/corner difference | confirmed | prep-tinytapeout:REPORT.md | TT hardens gf180 7t cells at **3.3 V** (`VDD_PIN_VOLTAGE 3.3`, corner `tt_025C_3v30`); sibling 5 V timing numbers do not transfer to a TT tile environment. |

## 3. FPGA board candidates

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| ULX3S (ECP5 LFE5U-25F, CABGA381) as the org's proven FPGA pattern | confirmed | gf180-parasynth:docs/fpga-build.md | Sibling routed on `nextpnr-ecp5 --25k`, ULX3S pinout, 25 MHz oscillator, PLL 725/59 MHz (+11 ppm) — with the real drum engine. Pattern transfers only with adaptation. |
| "ULX3S is sufficient for the DX7 core" | assumed (not established) | gf180-parasynth:docs/integration-area.md | DX7-core LUT/BRAM/DSP need is unmeasured (H09/H10). Scale reference only: sibling `synth_top` 5,885 LUT4, `drum_kit` 6,777 LUT4 — a six-operator × 16-note core is a different and unmeasured workload. |

## 4. Cell library

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| `gf180mcu_fd_sc_mcu7t5v0` (7-track, 5 metal, site `GF018hv5v_mcu_sc7`) | confirmed | gf180-parasynth:docs/pnr-synth-top.md | The library the working gf180 ORFS flow maps and routes (`TRACK_OPTION=7t`). 9-track costs **+26–27 % area** measured across blocks; use 7t. |
| Local PDK install on this machine | confirmed | local check 2026-09-19 · prep-tinytapeout:REPORT.md | Ciel install at `/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/gf180mcu/versions/54435919abffb937387ec956209f9cf5fd2dfbee/` (variants A–D; the same ciel commit `54435919ab…` the sibling docs cite), created 2026-09-16 (log `14-ciel-gf180mcu.log`). Full view set: liberty (all listed PVT corners incl. `tt_025C_5v00`), LEF, techlef, GDS, SPICE/CDL, behavioural Verilog. No `ciel`/`volare` CLI on PATH; `PDK_ROOT` unset — the install is at a nonstandard path and must be referenced by explicit path. |
| Local yosys can map against that liberty | confirmed | local check 2026-09-19 | Smoke check (§12): yosys `dfflibmap`+`abc` with `gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib` maps a trivial module; `dffq_1` liberty area 63.66 µm² reproduces the sibling's published per-cell table. A smoke check of tool access — **not** an H01/H02 result. |
| `DONT_USE_CELLS` policy difference | confirmed | gf180-parasynth:docs/pnr-synth-top.md · gf180-parasynth:docs/integration-area.md | ORFS `gf180` platform stock policy excludes `*_1` drive cells: **+29.1 %** (placeholder chip) / **+31.1 %** (joined chip) synthesis area vs `*_1` allowed; +33–40 % across blocks with local scripts. Our flow must declare its policy; numbers under different policies are not comparable. |
| No enable flop in the 7t library | confirmed | gf180-parasynth:docs/area-budget.md | An enable-written state bit costs `dffq_1` + `mux2_1` = 63.66 + 28.54 = **92.20 µm²** before any logic touches it. |

## 5. Memory options

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| Register-based state cost anchors | confirmed | gf180-parasynth:docs/area-budget.md · gf180-parasynth:docs/pnr-synth-top.md | `dffq_1` 63.66 µm²/flop; replicated state bit ≈ **118 µm²/bit** all-in (ladder-channel measurement); FIFO storage 120 µm²/bit; a measured real register file (`drum_regs`) came out at 318,300 µm² / 2,276 bits = **139.9 µm²/bit** — per-bit rules can understate once decode/enable overhead lands. H02 must project with these anchors and declare its uncertainty. |
| Foundry SRAM macros `gf180mcu_fd_ip_sram` | confirmed | 2AMLogic/gf180-sram:spec/kb-scale-integration.md · local check 2026-09-19 | Exactly four hardened macros: 64×8 / 128×8 / 256×8 / 512×8. Ceiling **512 B per macro** (`sram512x8m8wm1`, footprint 0.2094 mm² from LEF `SIZE`). Liberty views are present in the local ciel install (`libs.ref/gf180mcu_fd_ip_sram/lib/`) — usable for small memories with real timing. |
| Custom 256×32 macro (2AMLogic/gf180-sram) | confirmed (and confirmed-incomplete) | 2AMLogic/gf180-sram:README.md | DRC-clean, LVS-clean, 27-corner PVT signoff passed, LEF abstract committed — **no Liberty timing views generated; integrated nowhere**. Not usable for timing-closed design today. |
| KB-scale on-chip RAM (>512 B) | inaccessible | 2AMLogic/gf180-sram:spec/kb-scale-integration.md | No usable large SRAM macro with timing views exists on this PDK today. Recorded paths: tile foundry 512×8 macros (derived; bank-select/mux cost **not modelled**), DFFRAM (gf180mcuD target configured but **not signoff-clean** — unverified tier), OpenRAM (**no gf180mcu SRAM support**, ROM only). Reported as missing; not worked around. |
| ROM-as-logic cost | confirmed | gf180-parasynth:docs/area-budget.md | Case-table ROMs synthesize to logic at ≈ **1.9–2.2 µm²/stored bit** (sine 2.15, note table 1.88) — cheaper per bit than a flop; relevant to DX7 sine/exp table representation. |
| What the DX7 core's state actually uses | not decided here | 2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md | H02 decides against this constraint set; this inventory only maps the options and their evidentiary status. |

## 6. Physical flow

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| ORFS pinned image, gf180mcuD | confirmed | gf180-parasynth:docs/pnr-synth-top.md | `openroad/orfs:26Q3-296-gda37dce1c` @ `sha256:ebc8142d…`. The working gf180 pattern: platform `gf180`, `TRACK_OPTION=7t`, `CORNER=TC`. Sibling results: joined chip routes on a fixed 3.4650 mm² two-slot die at **60.1 % utilisation, 0 detailed-route DRC, ss setup +10.173 ns**; the same chip is **118.3 %** of the quarter slot and does not place. |
| Fixed-die methodology | confirmed | gf180-parasynth:docs/pnr-synth-top.md | Set `DIE_AREA`/`CORE_AREA` to fixed rectangles; never set `CORE_UTILIZATION` — utilisation is the measured quantity, not an input. |
| ORFS image present on this machine | confirmed | local check 2026-09-19 · prep-asic:REPORT.md | `docker image ls -a` shows the pinned image (ID `02d4efa1ace4`, 4,639,381,198 bytes, RepoDigest `sha256:ebc8142da6d6…` = the recorded pin) with a dangling tag (`openroad/orfs:<none>`). Runs linux/amd64 under Docker Desktop emulation; proven on this machine (sky130, three byte-identical routed GDS) and in the sibling gf180 runs (same macOS/Docker environment per that document's header). |
| klt (2AMLogic/klayout-tools) cross-check flow | confirmed pattern (sky130) / **never run on gf180** | prep-asic:REPORT.md · gf180-polysynth:asic/README.md | klt RTL→GDS→DRC/LVS/STA/ERC proven end-to-end on this machine for **sky130** only. The gf180 `klt place-and-route` configuration (polysynth `asic/`) **has never been executed anywhere**; its PDN `power` block and strap pitches (Metal1 5.04 µm followpins, Metal4 44.8 µm, Metal5 89.6 µm — 9t strategy) are recorded lessons, not verified results. |
| PDN power-block lesson | confirmed as recorded lesson | gf180-polysynth:asic/README.md | A routed layout without a `power` block fails `klt drc --deck gf180mcu` with **19 `nwell.space.1`** violations (unfilled row gaps); the same design with the block routes clean. Any flow request we write carries a power block. |
| ORFS gf180 sign-off gaps | confirmed | gf180-parasynth:docs/pnr-synth-top.md | No KLayout sign-off DRC deck in the ORFS `gf180` platform (router DRC only, "not a sign-off DRC run"); **no LVS**; IR analysis aborts at TC; power figures are ORFS estimates with default switching activity. |
| GitHub Actions CI (ubuntu) for flows | assumed | local check 2026-09-19 | No evidence record exists of a CI run installing the PDK/ORFS in *this* repo. Ubuntu runners plausibly install ciel/ORFS at run time (multi-GB images; minutes of setup) — **assumed until exercised**. |

## 7. EDA access on this machine (measured 2026-09-19)

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| yosys (native) | confirmed | local check 2026-09-19 | 0.69+post, git sha1 `143eb14f9cc…` (`/opt/homebrew/bin/yosys`). |
| iverilog (native) | confirmed | local check 2026-09-19 | Icarus Verilog 13.0 stable. |
| gf180mcuD standard-cell liberty + views | confirmed (present, nonstandard path) | local check 2026-09-19 | See §4 "Local PDK install". Smoke-checked usable by yosys. |
| OpenROAD binary (native) | inaccessible | local check 2026-09-19 | Not on PATH; no local install. Available only inside the ORFS Docker image. |
| Docker Desktop | confirmed | local check 2026-09-19 · prep-tinytapeout:REPORT.md | Running, 29.7.2. Recorded risk: the Docker **VM** disk (59 GB) had 5.0 GB free on 2026-09-16 and a 1.47 GB image pull failed `no space left on device` then; the **host** disk has ~2.0 Ti free today. Image pulls need VM-disk headroom. |
| Generic yosys structural / mapped probes | confirmed possible | local check 2026-09-19 | Liberty-based mapping works today (§4 smoke check). **Correction to the task briefing**: the briefing said standard-cell liberty was absent locally, which would have made mapped probes (H01/H02 mapped, H10) BLOCKED on PDK access. Measured reality: the ciel install exists at a nonstandard path and maps — mapped probes are **not blocked at the liberty level**. What remains genuinely never-run: any gf180 `klt` place-and-route, and any P&R of a DX7 core (H10/H11 work). CI-based flow execution remains assumed (§6). |

## 8. DAC / audio interface

| Item | Status | Source | Measurement / claim |
|---|---|---|---|
| I2S to an external DAC | confirmed pattern | gf180-parasynth:docs/ARCHITECTURE.md · gf180-parasynth:docs/pnr-synth-top.md | Sibling's `i2s_tx` (49 flops, 3,334.5 µm² flop area) is routed in both chips; DAC off-chip. Plan: audio carried "over an explicitly specified I2S format to an external DAC". |
| PCM5102A-class DAC | confirmed as the sibling's format-compatibility intent; part choice for this product assumed | gf180-parasynth:spec/NUMERIC-CONTRACT.md | The sibling's I2S bench "decodes as a PCM5102A does"; its DAC side names PCM5102A (SCK tied low), MAX98357A, TLV320DAC3100 as candidates. No part is chosen for gf180-dx7; N01/H08 own that decision. |

## 9. What this does NOT say

- **No die-fit claim for the DX7 core.** The only placed-and-routed chips on
  this PDK are the *sibling's* designs. Whether a six-operator × 16-note DX7
  core fits a quarter slot, a TT tile group, or neither is exactly what H10
  exists to measure. The 118.3 %-does-not-fit and 60.1 %-routes numbers above
  are sibling measurements, quoted as *calibration*, not as our budget.
- No claim that sibling area/timing numbers transfer to the DX7 core; only the
  per-cell/per-bit anchors (§4, §5) are flow-level constants of this library.
- No FPGA capture, no bitstream, no hardware playback (H09's business).
- No silicon, fabrication, shuttle purchase, or signoff claim of any kind.
- No verdict on preset quality, musical usefulness, or original-DX7 fidelity —
  outside this inventory's scope entirely.
- "confirmed as of 2026-09-16" shuttle facts are a snapshot; deadlines and
  availability drift and must be re-checked at decision time.
- The TT-tile capacity question for a DX7 core (16 tiles max, ≈1.06 mm²) is
  unmeasured; nothing here says the core does or does not fit it.

## 10. Open questions

1. Shuttle/packaging: TinyTapeout TTGF26c tiles vs a direct wafer.space
   quarter-slot seat vs later. Decision depends on H10's measured core size and
   on whether a 3.3 V TT environment is acceptable.
2. Target corner: 5 V (`tt_025C_5v00`, sibling precedent) vs TT's 3.3 V
   (`tt_025C_3v30`) for feasibility probing — timing conclusions differ.
3. `DONT_USE_CELLS` policy for our flow (area +29–40 % hangs on it); must be
   pinned before any H01/H02 number is quoted.
4. DX7 core state storage: registers vs foundry SRAM macros vs tiling — H02,
   against §5's anchors and the 512 B macro ceiling.
5. CI: exercise a GitHub Actions run that installs ciel + ORFS and re-runs the
   §4 smoke check, to convert §6's "assumed" into a record.
6. Docker VM disk headroom for image pulls (recorded failure on 2026-09-16).
7. Custom gf180-sram macro Liberty generation (upstream work, not this repo's).

## 11. Sources

| Cite key (used in `spec/physical-constraints-v1.json`) | Location |
|---|---|
| `2AMLogic/gf180-dx7:docs/dx7-chip-plan-v0.1-2026-09-20.md` | this repo, `docs/` |
| `gf180-parasynth:docs/pnr-synth-top.md` | `/Users/joseph/dev/gf180-parasynth/docs/pnr-synth-top.md` |
| `gf180-parasynth:docs/area-budget.md` | `/Users/joseph/dev/gf180-parasynth/docs/area-budget.md` |
| `gf180-parasynth:docs/integration-area.md` | `/Users/joseph/dev/gf180-parasynth/docs/integration-area.md` |
| `gf180-parasynth:docs/fpga-build.md` | `/Users/joseph/dev/gf180-parasynth/docs/fpga-build.md` |
| `gf180-parasynth:docs/ARCHITECTURE.md` | `/Users/joseph/dev/gf180-parasynth/docs/ARCHITECTURE.md` |
| `gf180-parasynth:spec/NUMERIC-CONTRACT.md` | `/Users/joseph/dev/gf180-parasynth/spec/NUMERIC-CONTRACT.md` |
| `gf180-parasynth:pnr/orfs/evidence/synth_top/joined-d1e5068/` | `/Users/joseph/dev/gf180-parasynth/pnr/orfs/evidence/synth_top/joined-d1e5068/` (committed metrics: 60.1 %, +10.173 ns, 118.285 % no-fit) |
| `gf180-polysynth:asic/README.md` | `/Users/joseph/dev/2amlogic/gf180-polysynth/asic/README.md` |
| `2AMLogic/gf180-sram:README.md` | https://github.com/2AMLogic/gf180-sram (shallow clone inspected 2026-09-19) |
| `2AMLogic/gf180-sram:spec/kb-scale-integration.md` | same repo, `spec/kb-scale-integration.md` |
| `prep-tinytapeout:REPORT.md` | `/Users/joseph/dev/2amlogic/prep/tinytapeout/REPORT.md` (2026-09-16) |
| `prep-asic:REPORT.md` | `/Users/joseph/dev/2amlogic/prep/asic/REPORT.md` (2026-09-16/17) |
| `local check 2026-09-19` | commands and outputs reproduced in §12 of this document |

## 12. Reproduce (the local checks behind `local check 2026-09-19`)

```sh
# Tools
yosys -V                 # Yosys 0.69+post (git sha1 143eb14f…)
iverilog -V | head -1    # Icarus Verilog version 13.0 (stable)

# Liberty smoke check (tooling access, not an H01/H02 probe)
CIEL=/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/gf180mcu/versions/54435919abffb937387ec956209f9cf5fd2dfbee
LIB=$CIEL/gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib
yosys -p "read_verilog smoke.v; synth -top smoke; \
  dfflibmap -liberty $LIB; abc -liberty $LIB; \
  stat -liberty $LIB"
# dfflibmap reports dffq_1 area 63.66 — matches gf180-parasynth:docs/area-budget.md §0

# ORFS image presence
docker image ls -a --format '{{.ID}} {{.Repository}} {{.Size}}' | grep orfs
# 02d4efa1ace4 openroad/orfs:<none> 4.64GB  (RepoDigest sha256:ebc8142da6d6… = the pinned digest)
```
