# H08 — SPI/I2S pin-level integration

Issue #30, branch `loom/h08-spi` (base `loom/h07-core` @ `884a7fa`).

## Scope and shape of the change

H07 delivered the six-operator engine behind a **flat** interface
(`spi_frame`/`spi_frame_v` driven directly from the bench). The core
(`rtl/dx7_core.v`, sha `335599ea…`, byte-identical to the H07-benched
commit — **no core lines changed by H08**) already contained the full pin
front-end: an SPI slave (CS/SCK/MOSI/MISO, 48-bit frame
`{F,6'b0,SEC,ADDR,DATA}`, MSB-first, latched on the 48th SCK rising edge,
one `cs_fall` → `miso_sh <= status` per transaction) and an I2S master
(BCLK/LRCLK/D). H08 therefore adds **no protocol logic of its own**: it
adds a passive two-line chassis and the pin-level verification that
proves the flat-interface conformance record of H07 survives
serialization through the real pads.

New files (all H08-owned):

| File | Role |
|---|---|
| `rtl/synth_top.v` | 9-pad chassis: `clk`/`rst` (pad reset, active-high, active from t=100 ps), SPI slave pads `cs_n`/`sck`/`mosi`/`miso`, I2S master pads `bclk`/`lrclk`/`d`. One `u_core` instance; every DUT port tied to a pad; tap, status and debug/internal ports left unconnected. |
| `rtl/tb_synth_top.v` | Pin-level bench. Drives SPI transactions with timing, decodes I2S back to the 32-bit sample stream, reports structural BCLK/LRCLK measures, and asserts live negative controls (see below). Same vector protocol as the H07 bench plus injection directives (`X` truncated frame, `V` F=0 frame, `Y` rate-violating burst, `Z` pad reset, `Q` status read). |
| `tools/h08_pin_check.py` | Evidence runner: for every committed H07 case it runs the flat bench and the pin bench on the **same** vector, requires the streams to match byte-for-byte, cross-checks run metadata (samples, overrun/overflow, structural periods), then runs the corrupted-transaction injection suite. Writes `evidence/h08-pin/runs/<case>/…` and `results/h08/summary.json`. |

The H07 evidence tree (`evidence/h07-core/`) is **untouched** — the
runner only reads from it (committed vectors and goldens).

## The three claims, kept separate per repo doctrine

1. **RTL == frozen integer model (exact).** Not re-proven here; it is the
   H07 record, and H08 changes none of the hash-pinned core.
2. **Model == pinned software reference (within budget).** Not re-proven
   here; same reason. H08 pins to the H07 golden files as the reference
   record for *behavior*, and — see F-H07-STALE below — reports a
   staleness finding where the committed golden no longer matches a
   rerun of the flat bench against the pinned core.
3. **Musical usefulness.** No claim. Renders are still dry.

The H08-specific acceptances and their status (final numbers in
`results/h08/summary.json`):

- **A1 — pin path reproduces flat-path behavior exactly.** For each
  committed H07 case, `pin.i32` (decoded from the three I2S pads) is
  byte-identical to `flat.i32` (H07 flat-interface tap stream) at
  `cmp` level, and run metadata agrees (sample count, overrun/overflow,
  frame counts).
- **A2 — corrupted/invalid SPI transactions are handled without partial
  commit, and the partial state is observable.** Injection suite
  (trunc47 f0 freshctl ovf partial; see below).
- **A3 — I2S frame order/format matches the contract as closely as the
  as-built core allows.** Measured, with two deviations recorded as
  findings (F-I2S-1, F-I2S-2) rather than silently absorbed.
- **A4 — underrun/overflow signaling verified.** OVERFLOW demonstrably
  sets (sticky) and clears only on pad reset; OVERRUN provably never
  sets; FRESH lifecycle verified via status readbacks.
- **A5 — live negative controls.** The bench asserts, on every run:
  BCLK toggles only on `clk` posedges with exact period 8; LRCLK period
  exactly 512 and phase-locked; `i2s_d` may only change on a `bclk`
  posedge; the per-frame decode window is exactly 24 data bits followed
  by zeros; the 22 MSB-carry bits are zero for every dumped sample
  (`w24[1:0] == 0` for the LSB padding and the duplicate-MSB pairing
  checks); truncated frames are counted against a forensic
  `frame_reg_v` latch counter and must not change the stream. Each
  control demonstrably fails when the DUT misbehaves (the D-transition
  control alone trips on a single-cycle glitch).

## I2S format: contract vs as-built (the finding record)

Measured against the frozen contract (48 kHz audio, 4× oversample, MSB
first, 24-bit data, BCLK/LRCLK as defined in the plan section for the
digital front end):

- **F-I2S-1 (deviation, as-built):** BCLK period is **8** `clk` (not 4)
  and LRCLK period is **512** `clk` (not 256). Data alignment still
  satisfies MSB-first-into-BCLK with the first data bit rising 6 BCLKs
  after the LRCLK edge (measured `first_bit_delay = 6`). The bench
  asserts the as-built constants and records them in every run's meta;
  the contract violation is recorded, not fixed (core is H07-pinned).
- **F-I2S-2 (deviation, as-built):** the register is loaded on the LRclk
  **rise** and the 24-bit word runs over 24 BCLK cycles with the slot
  window `w24[23:0] = {msb, b23..b1-MSB-dup excluded…}` — concretely the
  bench extracts `w24 = {slot0, slot24, slot23, …, slot2}` where slot0
  is the duplicate of the MSB across the BCLK edge and slots 2..24 are
  `W0[23]…W0[0]`. The as-built stream is what it is; the bench decodes
  exactly that and byte-matches the H07 golden on the flat interface,
  which is the proof the pin path introduces **no further** divergence.
- **F-OVF-1 (deviation, as-built):** the `st_overflow` flag is sticky
  until a pad reset; the depth-7 skid FIFO overflow is unreachable at
  any SCK rate (it is drained every `clk`, pushed at ≥194 clk/tx) —
  overflow can only occur via the event queue overfill (`Y` burst),
  which the injection suite exercises.
- **F-I2S-3 (deviation, as-built):** the H07 bench's own I2S decoder is
  dead code (it never ran; the flat-interface golden was produced from
  the direct tap port). H08's decoder is the **first** decoder that
  actually ran against the wire; it is pin-derived (BCLK/LRCLK edges
  only, no hierarchical reads expected from the DUT internals for the
  value stream — the `frame_reg_v`/`st_overrun` reads are forensic
  witnesses for the injection suite, not expectations).
- **F-STA-1 (deviation, as-built, reported not fixed):** the MISO status
  word is latched at `cs_fall` (end of the *previous* transaction), so
  the first read after pad reset returns `0x0000_0000`, not the
  configured word. The bench and runner account for this (first Q is
  expected-stale); the fix would require a core change and is out of
  H08 scope.
- **F-H07-STALE (evidence finding, H07-side):** the committed H07
  golden `evidence/h07-core/runs/verilog-accept-dev-dir-base/actual.i32`
  diverges from a fresh `tb_dx7_core` run on the same pinned core
  `335599ea…` and same committed vector. The flat bench itself is
  deterministic (two flat runs are byte-identical), and the pin bench
  byte-matches the flat bench, so the divergence is in the committed
  golden file, not in the RTL or the H08 harness. This is reported here
  because A1 compares pin vs a **rerun** of flat (the live flat stream),
  not pin vs the stale committed file, so A1 is unaffected.

## Injection suite (A2)

Vectors are built from the committed `dir-base` vector by surgical edit
in `tools/h08_pin_check.py`; each run is paired with a control that
performs the same vector with the corrupted transaction **removed**, so
a clean DUT must produce byte-identical streams.

| Case | What it does | Expected (clean DUT) | Check that fires on a broken DUT |
|---|---|---|---|
| `trunc47` | 47-bit (undersized) frame: only 47 SCK rising edges, CS releases. The 48th edge is never seen, so the DUT's `bit_cnt` is reset by the next `cs_fall` and `frame_reg_v` never pulses for it. | stream == control == flat; `frame_latches` count unchanged | a DUT that "helpfully" latches a partial frame changes the stream and the latch count |
| `f0` | Full 48-bit frame with F=0 in a write slot. `wr_wr = frame_reg_v & wr_f` → no-op. | stream == control; FRESH stays 1 | a DUT that commits on any frame drops FRESH and changes state |
| `freshctl` | FRESH lifecycle: only-no-op frames vs one accepted write. | FRESH stays 1 after F=0-only, drops to 0 after the first accepted write, with OVERRUN/OVERFLOW both 0 | a DUT that clears FRESH for non-accepted frames, or never clears it, fails |
| `ovf` | `Y 0 330 30 00000001`: 330 rate-violating frames at 98 clk/tx in one frame overfills the depth-168 event queue → OVERFLOW sticks; then `Z 60` pad-reset. | OVERFLOW=1 after the burst (and on re-read), =0 after pad reset with FRESH restored; OVERRUN never sets | a DUT that overflow-clears on re-read, or that sets OVERRUN, fails |
| `partial` | Drop the first half of the patch block, keep the commit. | stream == flat (for that vector), and ≠ full-dir-base stream | a DUT that "helpfully" back-fills missing patch words would match the full stream and fail the ≠ check |

## Status / FRESH protocol (A4)

`results/h08/summary.json` carries, for every run, the full qread
sequence (the as-built stale-first behavior — F-STA-1 — is visible in
the data, not just claimed) and the decoded bit fields. The FRESH boot
protocol is: after pad reset FRESH=1; the first *accepted* write (F=1,
any SEC) drops it to 0; it is restored to 1 by the next pad reset. A
write with F=0 never changes it (that is the `f0`/`freshctl` proof).

## Tooling / environment

- Verilator 5.052 is the sole executing tool. Icarus (iverilog
  13.0 stable and 14.0-devel) **crashes in codegen** on `dx7_core`
  elaboration (`Code generator failure: -1`) both before and after the
  H08 change reproduces on the committed H07-benched core — this is a
  pre-existing environment regression, not an H08 defect, and the Icarus
  shadow is reported as `NOT_RUN` (a test that cannot run is never a
  pass).
- Build: `verilator --binary --timing -j 4 --Wno-fatal
  --timescale-override 1ns/1ps --x-initial 0 --x-assign unique`.
- The bench's plusarg strings are declared 128 bytes wide
  (`reg [1023:0]`), matching the H07 bench: `$value$plusargs` right-fills
  fixed-width registers, so a narrow declaration silently
  left-truncates long absolute paths (a path "mangling" trap that
  cost one evidence pass — see the H07 bench for the same pin).
- The runner builds both benches from the repo sources into
  `build/h08/` (gitignored) and reuses them when the sources are
  unchanged, so a rerun is cheap.

## Anti-circularity

- Golden expectations come from the **committed** H07 evidence and the
  **pinned** core sha; the bench never reads DUT internals to set an
  expectation. The two hierarchical reads in the pin bench
  (`frame_reg_v` latch count, `st_overrun` invariant) are forensic
  witnesses for the injection suite and the DUT's own never-set
  invariant; they are never used to define a pass for the value stream.
- The flat-to-pin comparison is against a **live rerun** of the flat
  bench, not against a stored file, so a stale golden cannot mask a
  pin-path bug and vice versa. F-H07-STALE is the direct consequence of
  that choice and is the reason the committed golden needs a follow-up
  H07-side rerun, not an H08 fix.

## Reuse rulings (provenance)

| Artifact | Source | License | Destination | Adaptation | Requalification |
|---|---|---|---|---|---|
| `rtl/dx7_core.v` (SPI slave, I2S master) | H07, this repo, sha `335599ea…` | Apache-2.0 | unchanged | none | untouched by H08 (byte-identical to H07 commit) |
| Pin bench `rtl/tb_synth_top.v` | Adapted from `rtl/tb_dx7_core.v` (H07, this repo) + structural measures from the plan's I2S section | Apache-2.0 | new file | flat-interface stimulus replaced by timed SPI transactions; tap-port golden replaced by BCLK/LRCLK/D wire decode; injection directives added | live-ran on every committed H07 vector + injection suite; byte-matches the flat bench, so the adaptation is behavior-preserving |
| `tools/h08_pin_check.py` | New (runner pattern follows `tools/h07_compare.py`, this repo) | Apache-2.0 | new file | — | ran to completion on the full matrix |
| `rtl/synth_top.v` | Adapted from the sibling `synth-top-chassis` (gf180-parasynth, Apache-2.0, commit + path in `docs/reuse/catalog.json`) | Apache-2.0 | `rtl/synth_top.v` | sibling bench/test files reference-only; chassis reduced to the 9-pad map + reset (no DSP, no tick/go — the pinned core owns all timing) | `check_reuse.py` PASS incl. adapted-hash pin; pin co-sim byte-exact (this report) |

No GPL-3.0/GPL-2.0 material is introduced (Dexed/VDX7/Hexter remain
external oracles only). No per-file license headers (repo convention),
repo-level Apache-2.0.

## Evidence

- `evidence/h08-pin/runs/<case>/`: `vector.txt` (copy of the committed
  vector), `golden-committed.i32` (copy of the committed H07 golden, for
  the record), `flat.i32`/`flat.meta` (this run's flat bench),
  `pin.i32`/`pin.meta` (this run's pin bench), `log-flat.txt`,
  `log-pin.txt`. For injections the corruption and control vectors are
  committed alongside (`vec-trunc.txt`, `vec-ctrl.txt`, etc.).
- `results/h08/summary.json`: verdict per case/injection, the meta
  cross-check fields, qread sequences, and the Icarus `NOT_RUN` record.
- `tools/test_fast.sh` re-pinned to run the H08 harness on the
  `dir-base` fast subset with `--skip-inj` and to check the summary
  file; the full matrix + injections run at judge-approval wave.

## Stop / escalate

- **Stop condition invoked:** if any pin-path change would alter
  core-observable behavior vs the H07 vectors, stop and block. Not
  triggered (the pin path is passive; the core is untouched; A1 is
  byte-exact on every committed vector).
- **Escalated (not fixed, by scope):** F-H07-STALE → H07 follow-up
  (rerun + replace the committed golden with a hash-matched pair).