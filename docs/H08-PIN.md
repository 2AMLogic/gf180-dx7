# H08 — SPI/I2S pin-level integration

> **Supersession note (2026-09-24, DR-0011):** the frozen core this
> document pins (`335599ea…`) was re-freezed to the single-driver merged
> revision `34f93d2d…` (H10, see `docs/decision-records/0011-*`). The
> H08 evidence below remains the historical record of the chassis on the
> pre-refreeze core; the chassis files themselves are re-verified against
> the refrozen core by the H07 battery (bit-exact, unmodified chassis).

Issue #30, branch `loom/h08-spi` (rebases onto `origin/main`; core
lineage is the H07 record, PRs #73/#75/#76 — the hash-pinned core is the
same bytes H07 benched).

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
| `tools/h08_synth.py` | Chassis synthesis gate: yosys (0.69 + abc) over the chassis top (`synth_top`) against the named ciel `gf180mcu` 7t liberty, same script shape as `tools/h07_synth.py`, plus the `-DH07_STRIP_OBSERVABILITY` strip build as the resolution-proving negative control. Writes `evidence/h08-chassis/` (report + yosys log). No PnR, no timing closure, no fit claim (that is H10). |
| `tests/test_h08.py` | Evidence recompute (byte-level, recomputed *in the test*, not read from the summary) + live negative controls built on demand into the git-ignored `build/` tree. |
| `docs/reuse/catalog.json` | + `tools/h08_synth.py` and the re-pinned H08 report (local originals); the `synth-top-chassis` component carries the adapted `rtl/synth_top.v` hash and its requalification result. |

The H07 evidence tree (`evidence/h07-core/`) is **untouched** — the
runner only reads from it (committed vectors and goldens).

## The three claims, kept separate per repo doctrine

1. **RTL == frozen integer model (exact).** Not re-proven here; it is the
   H07 record, and H08 changes none of the hash-pinned core.
2. **Model == pinned software reference (within budget).** Not re-proven
   here; same reason. H08 pins to the H07 golden files as the reference
   record for *behavior*; batch-4 evidence shows the committed goldens
   byte-match fresh flat-bench reruns of the pinned core on all 11
   vectors (see the retracted F-H07-STALE note below for the early
   false-positive that this supersedes).
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
  control alone trips on a single-cycle glitch). The `trunc47` control
  also runs live in the fast lane (self-built bench, compact vector).
- **A6 — note-on latency stays inside the H03 §4.5 window `[(64-p),
  (64-p)+65]` (p = 32 → [32, 97] samples after the event).** Reported
  by carry, not re-measured: H08 changes no core logic (the event
  application timing is the H07 record), and the pin stream is
  byte-identical to the flat stream on all 11 committed vectors, so the
  first-voiced-sample index per event — the latency — is identical. The
  per-event H07 records (`evidence/h07-core/results-verilog-accept*.json`,
  `latency[].in_window = true` for every event on every case) therefore
  carry over to the pin path unmodified.
- **A7 — the synthesized chassis meets the resource target (chassis
  overhead vs the core evidenced, not estimated).** A passive pin map
  adds *zero* state and *zero* logic: `tools/h08_synth.py`
  synthesizes the chassis top with H07's pass shape (read; hierarchy;
  synth; dfflibmap; abc; stat -- liberty file sha-identical to H07's,
  `330aa943...`; but local yosys is 0.69+post vs H07's 0.57+72, so the
  numeric comparison is a cross-version bound, while the zero-addition
  property is tool-independent) and gates on (a) mapped flops above
  H02's
  38,781 floor (state present, not optimized away), (b) mapped flops
  and cell total **not exceeding** H07's core full build (138,490
  flops / 36,803 cells; any shortfall is reported as *connection-state
  pruning of the unconnected tap/status/debug outputs* — the H08 top
  exposes fewer ports than the H07 core-top build — never as overhead),
  and (c) the committed wrapper source being structurally a pin map
  (one core instance, no procedural logic, all nine pads mapped). The
  strip build (`-DH07_STRIP_OBSERVABILITY`) must collapse the flop
  count — the negative control proving this measurement has
  resolution. Mapped area/cell numbers only: **no PnR, no timing
  closure, no fit, no board claim** (H10).

  **Status on this host (2026-09-23): the yosys+abc run is NOT_RUN, not
  a pass.** The shared host was ~100x CPU-oversubscribed (load ~100 on
  10 cores, 64 users, parallel H10 wave); the job accumulated ~5.5
  CPU-minutes in ~61 wall-minutes (~9% of one core) and was still
  inside the internal `synth` pass, so the full+strip pipeline was
  infeasible in session budget and was killed to release host CPU.
  Machine-readable record with host diagnostics and the exact re-check
  command: `evidence/h08-chassis/NOT_RUN-note.json`. What IS directly
  established this session (and stands regardless of the run): (i) the
  structural no-logic wrapper check PASS with live negative controls —
  injecting a `reg` or an `always` block makes the checker FAIL (run
  and recorded in the note); (ii) the local 7t liberty file is
  sha-identical to H07's (`330aa943...`); (iii) all four RTL files are
  sha-pinned (core trio via the H07 pins, the wrapper via this PR's
  catalog pin). From (i)+(ii)+(iii) the zero-addition property follows
  as a documented inference with its warrants — never reported as a
  measured gate result. The full gate is re-checkable with one command:
  `python3 tools/h08_synth.py` (and `--from-logs` once committed logs
  exist).

  **Gate result (2026-09-23, wave-gate re-check on `repo-remote`):**
  the re-check command ran on `repo-remote-gf180-dx7`
  (i-0c0a073ffb9db41d5, m5.2xlarge, 8 vCPU) with **yosys 0.57+72
  (git sha1 fa02d71f6)** — the same build H07's committed report used,
  so no cross-version qualification survives — and the pinned 7t
  liberty (`330aa943...`, byte-identical, recorded in the report), over
  the RTL synced byte-identical by `git archive` (file sha256 verified
  on the box). `evidence/h08-chassis/synth_report.json` carries the
  full+strip Yosys logs with their sha256s. **Full build = PASS:**
  138,490 mapped flops, 36,803 mapped cells, 1,158,154.592 um² chip
  area — exactly the committed H07 core full build (0 flops culled, 0
  cells culled, 0 overhead: the pin map adds nothing, as required). The
  **strip negative control = MET**: with
  `-DH07_STRIP_OBSERVABILITY` the mapped flop count falls to 0, proving
  the measurement has resolution. Wrapper structural check PASS (one
  core instance, no procedural logic, all nine pads mapped). Chassis
  overhead vs the H07 core is now **measured at zero, not inferred.**

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
  until a **pad reset** — the contract's DEC-014 "cleared on read"
  policy is not the as-built behavior (the `ovf` injection's qread
  map shows `OVERFLOW=1` on two consecutive reads, then the pad-reset
  canary, then `OVERFLOW=0` with FRESH restored). The depth-7 skid
  FIFO overflow is unreachable at any SCK rate (it is drained every
  `clk`, pushed at ≥194 clk/tx) — overflow can only occur via the event
  queue overfill (`Y` burst), which the injection suite exercises.
- **F-I2S-3 (deviation, as-built):** the H07 bench's own I2S decoder is
  dead code (it never ran; the flat-interface golden was produced from
  the direct tap port). H08's decoder is the **first** decoder that
  actually ran against the wire; it is pin-derived (BCLK/LRCLK edges
  only, no hierarchical reads expected from the DUT internals for the
  value stream — the `frame_reg_v`/`st_overrun` reads are forensic
  witnesses for the injection suite, not expectations).
- **F-STA-1 (deviation, as-built, reported not fixed):** status reads
  are stable-by-one: a Q transaction returns the word latched at the
  **previous** transaction's end (the DUT reloads `miso_sh` from
  `status_real` at transaction end — `dx7_core.v` MISO FSM, the reload
  arm is its named `cs_fall` condition). Additionally, the FSM's reset
  arm zeros `miso_sh`, so the first read after **any** (pad) reset is
  `0x00000000` (a canary, not a status word) and the real post-reset
  word arrives one read later. Empirically fitted on three full runs
  (batch 2/3 + standalone ovf) and enforced by the runner's qread index
  map; the fix would require a core change and is out of H08 scope.
- **F-H07-STALE (RETRACTED):** early batches (1–2) of this work showed
  the flat bench's stream diverging from the committed dir-base golden
  (first diff at byte 10033; one length mismatch). Investigation after
  the harness stabilized: the committed golden files are unchanged
  (sha256 `2df385e2…`, committed with H07 PR #73, mtimes intact, git
  clean), and batch 4 — a single coherent run with the final harness —
  shows the committed golden byte-matches both the flat bench rerun and
  the pin bench stream on **all 11 vectors**. The early divergence was
  an artifact of the in-development harness (stale/mid-write `flat.i32`
  files from superseded builds), not an H07 discrepancy. No H07
  escalation is required; the note is retained for audit trail.

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
| `ovf` | `Y 0 330 30 00000001`: 330 rate-violating frames at 98 clk/tx in one frame overfills the depth-168 event queue → OVERFLOW sticks; then `Z 60` pad-reset. | qread map (7 reads): [0] canary 0, [1] post-reset fresh=1, [2] post-burst **OVERFLOW=1**, [3] sticky **OVERFLOW=1**, [4] post-pad-reset **canary 0x00000000** (F-STA-1), [5] **FRESH=1, OVERFLOW=0**, [6] end-of-loop; OVERRUN (forensic) never sets | a DUT that overflow-clears on re-read, that does not re-arm FRESH at pad reset, or that sets OVERRUN, fails |
| `partial` | Drop the first half of the patch block, keep the commit. | stream == flat (for that vector), and ≠ full-dir-base stream | a DUT that "helpfully" back-fills missing patch words would match the full stream and fail the ≠ check |

## Status / FRESH protocol (A4)

`results/h08/summary.json` carries, for every run, the full qread
sequence (the as-built stale-first behavior — F-STA-1 — is visible in
the data, not just claimed) and the decoded bit fields. The FRESH boot
protocol is: after pad reset FRESH=1; the first *accepted* write (F=1,
any SEC) drops it to 0; it is restored to 1 by the next pad reset. A
write with F=0 never changes it (that is the `f0`/`freshctl` proof).
Protocol note (validated in H08 probes): the FRESH flag tracks
`wr_wr` (latched frame with F=1) only — any accepted F=1 frame
(patch, commit, or event-slot) unfreshes it, and only a pad reset
restores it (the `ovf` qread map shows fresh=0 after the accepted
burst and fresh=1 after `Z`). Event frames (SEC=0 opcodes) are
actuated on the event path **independently of the F bit**: `E`
note events voice audio with F=0 (a static patch + commit alone
voices nothing — an H08 compact probe lands the first voiced sample
at index 2508 with the dir-base P+C+E block, matching the committed
stream), and the same event path is the one the `ovf` burst
overfills.

## Transport spec (DR-0007 / contract §4.2–4.3) mapping

The transport H08 verifies is the contract's DR-0007-adapted SPI/I2S
front end (gf180-parasynth DR-0007 is the transport spec; its register
semantics are sibling-specific and were **not** imported — catalog
note on the `synth-top-chassis` component):

- **Frame:** 48-bit `{F, 6'b0, SEC, ADDR[7:0], DATA[31:0]}`, MSB-first,
  latched on the 48th SCK rising edge (`cs_n` falling loads the status
  word onto MISO for the next read). Mode-0 shape (SCK idle-low,
  sample on the rising edge), matching the spec-0007 transport; the
  bench drives it timing-true so the phase law is exercised, not
  assumed.
- **SCK bound:** contract §4.2 says SCK ≤ f_core/4 (6.144 MHz at the
  24.576 MHz design point), i.e. ≥4 core clocks per SCK bit. The bench's
  normal transactions run at exactly the compliant edge (1 write per
  194 core clocks: 48 bits × 4 + 2-cycle CS gap); the `Y` burst is the
  deliberate rate violation that demonstrates the explicit-rejection
  policy instead of a silent drop.
- **Event application timing (DEC-014):** an event received during frame
  *N* applies before the first sample of frame *N+1* — worst-case core
  commit latency one frame. Directly observed through the pads: the
  compact H08 probe (static patch + commit + the dir-base `E` block
  only) lands the first voiced sample at pin-derived index 2508, and
  the same event path is the one the `Y` burst overfills.
- **Status word (contract §4.3, adapted from the sibling §4 word
  `{0x4D, VERSION, overrun, queue_nonempty, overflow, fresh, frame}`):**
  `{MAGIC=0xD7, VERSION=0x1, OVERRUN, QUEUE, OVERFLOW, FRESH,
  FRAME[15:0]}`, 32 bits on MISO, loaded at CS_N fall. The bench's `Q`
  readbacks decode exactly this map (field positions asserted in
  `tests/test_h08.py`). Two as-built deviations from the contractual
  assumptions are recorded, not absorbed: F-STA-1 (reads are
  stale-by-one — the contract's *first read after reset returns a
  `0xD711_0001`-shaped word* is not the as-built behavior; the canary
  `0x00000000` comes first) and F-OVF-1 (OVERFLOW does not clear on
  read; pad reset only). A fix for either is a core change and is out
  of H08 scope (the core is hash-pinned).
- **FRESH/FRAME brown-out protocol (sibling ARCHITECTURE §7):** boot on
  `fresh = 1`, re-send the image whenever `fresh` later reads 1. The
  `freshctl`/`ovf` injections verify the lifecycle end-to-end through
  the pad (see the Status/FRESH section).

## Tooling / environment

- Verilator 5.052 is the sole **executing** tool. Icarus: the earlier
  session record claimed iverilog (13.0 stable and 14.0-devel) crashed
  in codegen on `dx7_core` elaboration; **that did not reproduce on
  this host (2026-09-23 re-check)** — `iverilog -g2012 -o x.vvp
  rtl/dx7_core.v rtl/env_unit.v rtl/alg_router.v rtl/synth_top.v
  rtl/tb_synth_top.v` completes at rc=0 to a valid ~7.2 MB `vvp`
  binary, with only the known sized hex-constant warnings from the core.
  Icarus therefore serves as a second **elaboration** check: PASS (the
  command above is the re-check; it is fast and does not render audio).
  It is not viable as the *shadow execution* tool for the 16-case
  matrix: measured `vvp` throughput — a 2,688-sample (42-frame) prefix
  of one case was still incomplete after 25 minutes (killed; the same
  prefix runs in seconds under Verilator), so the full matrix under vvp
  would take days, not hours. The Icarus shadow is therefore still
  reported as `NOT_RUN` with a reason in `results/h08/summary.json` (a
  test that cannot run is never a pass), and the reason text was
  updated to the measured, reproducible shape: elaboration PASS,
  execution infeasible in budget. The prior "codegen crash" note is
  retained here as the superseded observation with its re-check record.
- Fast lane (`tools/test_fast.sh`): H08 found the script's
  `discover | tail` pipeline (POSIX sh, no pipefail) masked unittest
  failures — the gate exited 0 on a red suite. It now captures
  discover's rc explicitly and fails with it. On this machine the
  audition tamper test needs the pinned archive zip in `~/Downloads`,
  where macOS TCC denies content reads (EPERM even to `shasum`); the
  test now records that as a reason-carrying SKIP (NOT_RUN) instead of
  an ERROR — an unreadable pinned asset is never a pass and never an
  environment-shaped error. Every other fast-lane test is green.
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
  pin-path bug and vice versa. Batch-4 additionally confirms the
  committed H07 goldens are byte-identical to fresh flat reruns on all
  11 vectors, so the rerun and the committed record agree.

## Reuse rulings (provenance)

| Artifact | Source | License | Destination | Adaptation | Requalification |
|---|---|---|---|---|---|
| `rtl/dx7_core.v` (SPI slave, I2S master) | H07, this repo, sha `335599ea…` | Apache-2.0 | unchanged | none | untouched by H08 (byte-identical to H07 commit) |
| Pin bench `rtl/tb_synth_top.v` | Adapted from `rtl/tb_dx7_core.v` (H07, this repo) + structural measures from the plan's I2S section | Apache-2.0 | new file | flat-interface stimulus replaced by timed SPI transactions; tap-port golden replaced by BCLK/LRCLK/D wire decode; injection directives added | live-ran on every committed H07 vector + injection suite; byte-matches the flat bench, so the adaptation is behavior-preserving |
| `tools/h08_pin_check.py` | New (runner pattern follows `tools/h07_compare.py`, this repo) | Apache-2.0 | new file | — | ran to completion on the full matrix |
| `tools/h08_synth.py` | New (pattern follows `tools/h07_synth.py`, this repo; same ciel gf180mcu 7t liberty and script shape) | Apache-2.0 | new file | — | ran to completion on `synth_top` (full + strip negative control); evidence in `evidence/h08-chassis/` |
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
  cross-check fields, qread sequences, and the Icarus record (elaboration
  PASS + shadow `NOT_RUN` with reason).
- `evidence/h08-chassis/`: `NOT_RUN-note.json` (this session: the
  yosys+abc gate run did not complete under ~100x host oversubscription;
  the note carries the host diagnostics, the liberty/RTL identity
  checks, the passing structural check with its negative controls, and
  the exact re-check command). A completed gate run drops into the same
  directory as `synth_report.json` + `yosys_full.log` /
  `yosys_strip.log` and is re-checkable via
  `tools/h08_synth.py --from-logs`. **No PnR, no timing, no fit, no
  board claim (H10).**
- **Freshness re-verification (2026-09-23, this host, identical
  toolchain):** the committed batch was produced on this host by the
  first two H08 sessions (2026-09-23 morning). The full longest matrix
  case (`dir-base`, 72,319 samples) was re-run end to end today in both
  bench modes and reproduces the committed `flat.i32` **and** `pin.i32`
  byte-identically (zero diff, so the committed batch stands as the
  record); the injection/forensic vectors are additionally re-run live
  by `tests/test_h08.py` in the fast lane. The remaining ten matrix
  cases' completeness is re-exercised at the judge wave gate
  (`make test`, DR-0009), not assumed — a test that was not re-run is a
  committed batch on the same host/toolchain, and it is reported as
  such, not as freshly re-generated.
- `tools/test_fast.sh` re-pinned to run the H08 harness on the
  `dir-base` fast subset with `--skip-inj` and to check the summary
  file; the full matrix + injections run at judge-approval wave.

## Stop / escalate

- **Stop condition invoked:** if any pin-path change would alter
  core-observable behavior vs the H07 vectors, stop and block. Not
  triggered (the pin path is passive; the core is untouched; A1 is
  byte-exact on every committed vector).
- **Escalated (not fixed, by scope):** none. (The early
  F-H07-STALE escalation is retracted — see the findings list; it was
  an in-development harness artifact, not an H07 defect.)