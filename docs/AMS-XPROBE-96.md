# iverilog x on the LFO-AMS path: the hazard is live, and the recorded shadow masked it (issue #96)

**Verdict: FAIL, and issue #96's stop/escalate condition applies.** The
out-of-range `exp_t3`/`exp_t4`/`exp_t5` reads at `rtl/dx7_core.v:953-961`
are not a dormant, theoretical hazard for Icarus Verilog. On the frozen
DR-0011 core they make `exp_hsum` unknown, and on any patch with a nonzero
per-operator LFO amp-mod sensitivity that unknown reaches `exp_pt`, the
per-operator level `op_li`, and the mixer output `tap_mix` — 383 of 639
dumped samples in a 9-frame probe. Two H07 corpus cases (`dev32-06`,
`dev32-30`) have such a patch. Re-compared through **the repository's own
conformance harness** over a window that reaches past the note-on, both
now report a bit-exactness **FAIL** under iverilog 13.0 (stable) — every
compared golden sample of the live note rendered as 0 — while the same
window passes under Verilator and an AMS = 0 sibling case passes under
iverilog. In the committed canonical shadow both render as silence where
Verilator renders full audio (113,735 and 115,073 diverging samples over
the 120,063-sample overlap), and `dev32-06` was nevertheless recorded
**`pass: true`**.

So the answer to #96's stop/escalate question is yes: the corpus *was*
silently passing an AMS ≠ 0 case despite x-corruption. Section 4 gives the
masking mechanism. It is two independent lossy steps, neither of which
involves the comparator being wrong about the samples it looked at.

- Scope: `rtl/dx7_core.v` at the DR-0011 pin
  `34f93d2d391412fc8d8495653c1a00f2860beabf9d6fb0deaffccdbd53159f58`
  (re-hashed in every artifact below). **No RTL is changed by this work.**
- Claim class: claim (1) of `CLAUDE.md` — whether the RTL matches the
  frozen integer model, and specifically what the recorded iverilog
  evidence does and does not establish about that. Nothing here concerns
  the software reference (claim 2) or sound quality (claim 3).
- Relationship to #86 / #98: #86 proved the out-of-range bits are 0 in
  exact arithmetic and recorded that iverilog returns x for them at the
  exp() unit's own boundary. This document carries that x forward into the
  whole core and into the recorded conformance evidence. The RTL width fix
  is **not** done here — it is #98's single-change re-freeze.
- Evidence: `evidence/issue-96-ams-xprobe/` (reports) and
  `evidence/h07-core/runs/issue96-window-*` (the compared dumps). Tools:
  `tools/ams_xprobe.py`, `tools/ams_window_compare.py`,
  `tools/h07_ams_audit.py`. Fast-lane test: `tests/test_ams_audit.py`.

## Summary of checks

| # | Check | Tool | Status |
|---|---|---|---|
| 1 | Corpus-wide AMS census, all 34 cases incl. the 4 registry-sourced stress voices (#96 acceptance path (a)) | `h07_ams_audit.py` | **PASS** (audit complete): exactly `dev32-06`, `dev32-30` have AMS ≠ 0 (OP6 = 3); all 4 stress voices are AMS 0 |
| 2 | Whole-core X propagation on a real AMS ≠ 0 patch, real SPI stimulus, iverilog (#96 acceptance path (b)) | `ams_xprobe.py` | **FAIL** (hazard confirmed live): x reaches `exp_pt`, `op_li`, `tap_mix`; 383/639 samples unknown |
| 2a | Control — same patch, AMS zeroed (removes the gate at `dx7_core.v:2139`) | `ams_xprobe.py --zero-ams` | **PASS** (clean): 0 unknown samples |
| 2b | Control — same patch, scratch RTL with the three wires widened (removes the x source) | `ams_xprobe.py --widened` | **PASS** (clean): `exp_hsum` never unknown, 0 unknown samples |
| 2c | Control — same patch, same probe, Verilator | `ams_xprobe.py --sim verilator` | **PASS** (clean), with the 2-state caveat in §3 |
| 3 | Conformance compare of `dev32-30` / `dev32-06` over a window that CONTAINS the note, iverilog (#96 path (b), through the real harness) | `ams_window_compare.py` | **FAIL** (bit-exactness): 0 of 2,856 / 2,864 nonzero golden samples reproduced; first mismatch in block 75 |
| 3a | Control — same window, same case, Verilator | `ams_window_compare.py --tool verilator` | **PASS**: bit-exact, 2,856/2,856 |
| 3b | Control — AMS = 0 sibling `dev32-03`, same window, same truncation, iverilog | `ams_window_compare.py --case dev32-03` | **PASS**: bit-exact, 2,870/2,870 |
| 4 | Compared-window coverage of the recorded iverilog canonical shadow | `h07_ams_audit.py` | **FINDING**: 30 of 34 compared windows are golden-silence; 3 have audio; 1 never compared |
| 5 | Committed iverilog vs Verilator dump divergence (`h07_compare.py` requires this to be reported) | `h07_ams_audit.py` | **FINDING**: `dev32-06` and `dev32-30` differ on ~95 % of the overlap; `dev32-03` (AMS 0) is bit-identical |
| 6 | Does `dev32-30` deadlock under a *stable* iverilog 13.0, as the DR-0011 re-run recorded? | `ams_window_compare.py` / `h07_compare.py` | **Partly answered**: the 126-block windowed vector completes normally; the *full-length* vector is **NOT_RUN** — see §6 |

## 1. Which corpus patches can reach the exp() unit at all (path (a))

`dx7_core.v:2139` starts the exp() unit only when
`n_ams[nn][opk] != 25'd0`, and `ampsens_tab()` (`dx7_core.v:821-828`) maps
the 2-bit `amp_mod_sensitivity` field 0 → 0 and 1/2/3 → nonzero. So
"no operator of this patch has AMS ≠ 0" is exactly "this case never
evaluates exp()", with no runtime qualifier — the LFO amp-mod *depth* does
not enter the gate.

`tools/h07_ams_audit.py` decodes the voice bytes of all 34 corpus cases:
the 30 dev cases through the same `h07_compare.case_voice_bytes()` path the
harness uses (manifest-hash-checked), and the 4 stress cases through
`h07_compare._stress_voice()` — the registry voices that are **not** in
`evidence/h07-core/cases.json` and were therefore outside the audit #96
inherited.

Result (`h07-ams-coverage-audit.json`):

- `dev32-06` — `amp_mod_sensitivity` OP1..OP6 = `[0,0,0,0,0,3]`
- `dev32-30` ("SOFT TOUCH") — `[0,0,0,0,0,3]`
- every other operator of every other case, including both voices of
  `stress-patchcommit` — 0

Path (a) is therefore **complete**, and its answer is the opposite of the
dormancy it was meant to confirm: two corpus cases do exercise the path.

## 2. The x reaches the audio path (path (b), direct 4-state observation)

`evidence/issue-96-ams-xprobe/tb_ams_xprobe.v` instantiates the real
`dx7_core`, drives its real SPI port with the real `dev32-30` patch page,
a commit, and a note-on (frames built by the same
`h07_compare.page_writes()` / `note_on_writes()` helpers the conformance
harness uses), then watches `exp_hsum`, `exp_pt`, `exp_li`, `w_li`,
`op_li[0][*]` and `tap_mix` for 4-state unknowns over 8 wire frames after
the strike. It is not a hierarchical-force rig: the stimulus is the
contracted host interface.

| Run | `exp_hsum` first x | `exp_pt` | `exp_li` | `w_li` | `tap_mix` | unknown samples | `op_li` slot-0 ops unknown |
|---|---|---|---|---|---|---|---|
| iverilog, as-frozen RTL, AMS = 3 | frame 0 | frame 3 | frame 3 | frame 3 | frame 4 | **383 / 639** | 1 (OP6) |
| iverilog, as-frozen RTL, **AMS = 0** control | frame 0 | never | never | never | never | 0 / 639 | 0 |
| iverilog, **widened** scratch RTL, AMS = 3 | never | never | never | never | never | 0 / 639 | 0 |
| **Verilator**, as-frozen RTL, AMS = 3 | never | never | never | never | never | 0 / 639 | 0 |

All four runs render the same 639 samples, allocate the same one voice, and
strike at the same wire frame, so no control is vacuously clean
(`tests/test_ams_audit.py::CommittedProbeEvidence::test_controls_are_not_vacuously_clean`
keeps that live).

Read together the three controls make the attribution, each by removing
exactly one link of the chain:

- **the gate** — zeroing AMS keeps the x source (`exp_hsum` is
  combinational, so its out-of-range bits are unknown from frame 0 whether
  or not the unit is ever started) and the propagation stops dead. The
  unknown is admitted to the datapath by `n_ams != 0`, nothing else.
- **the source** — widening `exp_t3`/`exp_t4` to `[85:0]` and `exp_t5` to
  `[81:0]` in a scratch copy removes the unknown at `exp_hsum` itself and
  with it every downstream unknown. (Scratch only. Doing this to the frozen
  RTL is #98's re-freeze, not this work.)
- **the simulator** — Verilator shows no unknown anywhere (see the caveat
  in §3), so this is an iverilog-vs-Verilator divergence, not an
  RTL-vs-model defect visible to both.

Note the ordering in the positive run: `exp_hsum` (frame 0, always) →
`exp_pt` (frame 3, the first strike-driven evaluation) → level → mix. The
16 unknown bits of `exp_hsum` enter `exp_rp = exp_T * {2'b0, exp_hsum}`,
which makes all of `exp_res` unknown under 4-state multiplication, so
`exp_pt_q` is fully unknown — not merely the bits the reads touched.

## 3. The conformance harness itself reports the failure, once the window contains the note

The probe above reads a 4-state value directly. That is the sharpest
instrument, but it is a bespoke one; the claim that matters for this
repository is what the **frozen conformance harness** says. So
`tools/ams_window_compare.py` runs `tools/h07_compare.py`'s own
`build_dev_vector()` / `build_sim()` / `play_sim()` / `compare_case()` on
an unmodified corpus case, against the untouched frozen golden vector,
over a 120-block window — the note-on is at block 75, so 45 blocks of the
window are live audio. Nothing about the comparison is relaxed: the same
bit-exact compare, the same golden, the same latency convention.

The only change is the length: the event-trace rows after the window
(blocks 750/1125/1875) are dropped and the tail shortened, which is what
makes the run ≈ 6 minutes instead of ≈ 2 hours on one core.

| Case | AMS (OP1..OP6) | Simulator | compared | golden nonzero | rendered nonzero | bit-exact | first mismatch |
|---|---|---|---|---|---|---|---|
| `dev32-30` | `[0,0,0,0,0,3]` | iverilog 13.0 (stable) | 7,680 | 2,856 | **0** | **no** | index 4946 = block 75 sample 18, golden 1, actual 0 |
| `dev32-06` | `[0,0,0,0,0,3]` | iverilog 13.0 (stable) | 7,680 | 2,864 | **0** | **no** | index 4944 = block 75 sample 16, golden 2, actual 0 |
| `dev32-30` | `[0,0,0,0,0,3]` | Verilator 5.050 | 7,680 | 2,856 | 2,856 | yes | — |
| `dev32-03` | `[0,0,0,0,0,0]` | iverilog 13.0 (stable) | 7,680 | 2,870 | 2,870 | yes | — |

Both iverilog runs report `state_obs x`; both passing runs report
`state_obs 1`.

Three things follow, each from a specific row rather than from the set:

- **The gap is real in the harness, not only under a probe.** The two
  nonzero-AMS cases fail a bit-exact conformance compare the moment the
  compared window contains a live note. `dev32-06` is the case the
  committed shadow recorded as `pass: true`.
- **The truncation is sound, and that is checked rather than asserted.**
  `dev32-03` has a *byte-identical event trace* to the other two and is
  truncated identically; it reproduces the frozen golden prefix exactly.
  Had dropping the later events perturbed the compared prefix, this
  control would have failed too. (Causality says it cannot; the control is
  what makes that more than an argument.)
- **It is simulator-specific.** The same case, same window, same vector
  passes under Verilator.

**Caveat on the Verilator rows (§2 and this section).** Verilator is a
2-state simulator: it cannot represent `x` at all, so "Verilator shows no
unknown" is true by construction and is **not** independent evidence that
the reads are benign. What the Verilator rows *do* establish is the thing
that matters here — that Verilator renders the **correct audio**
(bit-exact against the frozen golden over the same window) where iverilog
renders zeros. The independent evidence that the unknown is caused by the
out-of-range reads is the **widened-RTL control**, which is run under
iverilog, in 4-state, with everything else held fixed.

## 4. Why the recorded shadow did not see it (the masking mechanism)

Two lossy steps compose. Neither is a comparator bug.

**Step 1 — the dump destroys the x.** `rtl/tb_dx7_core.v:387-392` writes
each sample with `$fwrite(afd, "%c%c%c%c", …)`. iverilog renders an x byte
under `%c` as NUL (verified directly: an 8-bit x written with `%c`
produces `0x00`). So an unknown sample lands in `actual.i32` as the integer
0 and is compared as a legitimate 0. The whole `dev32-30` iverilog dump is
120,063 samples of exact zero; the metadata line `state_obs x` is the only
surviving trace of the unknown, and nothing reads it.

**Step 2 — the compared window is silence.** The canonical shadow is run
as a bounded prefix slice (`--tool iverilog --frames 24`, per
`docs/H07-CORE.md` §4). `--frames` caps the **compare window**, not the
vector: the run still plays to block 1875. Every dev case's first event is
at block 37 or later (the `dev32-*` cases at block 75), so the compared
golden samples — blocks 0..23 — are pre-note-on silence. Auditing the
committed `results-iverilog-shadow.json` this way:

| Compared window | Cases |
|---|---|
| golden-silence (every compared golden sample is 0) | **30** of 34 |
| golden has audio | 3 (`stress-sum16`, `stress-reset`, `stress-patchcommit`) |
| never compared (recorded `CouldNotRun`) | 1 (`dev32-30`) |

Over a golden-silence window an x-corrupted render, dumped as NUL, compares
**equal**. That is how `dev32-06` — a nonzero-AMS case whose full render
disagrees with Verilator on 113,735 samples, and which fails outright once
the window reaches the note (§3) — is recorded `pass: true` with
`checked: 1536` and an empty mismatch list.

The same audit over the full-length Verilator acceptance run reports one
golden-silence window (`stress-burst`, whose golden really is silent) and
33 windows with audio, so the metric is measuring something real rather
than flagging everything.

## 5. The committed dumps already disagree

`tools/h07_compare.py`'s own docstring: "Any Verilator/iverilog artifact
divergence on the same vector is reported as a finding, never silently
accepted." The artifacts under `evidence/h07-core/runs/` (both committed at
`2fabaf2`, the run recorded as 34/34 PASS) were never compared to each
other — only each against the golden, and only inside the compared window.
Comparing them directly:

| Case | AMS | overlap | diverging samples | first diverging index | iverilog nonzero | Verilator nonzero | iverilog `state_obs` |
|---|---|---|---|---|---|---|---|
| `dev32-03` | 0 | 120,063 | **0** | – | 115,055 | 173,770 | `1` |
| `dev32-06` | 3 (OP6) | 120,063 | **113,735** | 4,944 | 416 | 137,659 | `x` |
| `dev32-30` | 3 (OP6) | 120,063 | **115,073** | 4,946 | 0 | 187,169 | `x` |

The three cases have identical event traces (note-on at block 75 → dump
index `(0 + 75 + 2) × 64 = 4928`) and differ only in the patch. Divergence
starts at the first audible sample after the strike and continues; the
AMS = 0 case of the same shape is bit-identical across the two simulators
for its whole overlap. The first diverging indices in this table — 4,946
for `dev32-30`, 4,944 for `dev32-06` — are the same indices §3's fresh
runs independently report as the first mismatch *against the golden*.

**Consequence for the recorded claim.** `docs/H07-CORE.md` §4 said the
iverilog shadow was "34/34 PASS — **zero Verilator/iverilog divergence**
(every case bit-exact under both)". Both halves are wrong, independently:
the committed file has 33 passes and one `CouldNotRun`, and two cases are
not bit-exact under both. The corrected statement is in that file now; what
the iverilog evidence actually establishes is listed in section 7.

## 6. What the `dev32-30` `CouldNotRun` is (and is not)

`evidence/h07-core/results-iverilog-shadow.json` records `dev32-30` as
`{"error": "simulation failed:  ", "pass": false}` with empty captured
output. Issue #96 asked whether that failure is the x. It is **not** —
at least not directly. Commit `178280d`'s message records what happened:
the case "deadlocked in the box's iverilog 13.0 devel build (vvp 99 % CPU
> 2 h, zero output growth; killed by PID per box hard-rules)". An
externally killed process is exactly a nonzero return code with no stdout.
Treating that as evidence of the x would be wrong.

The honest split:

- The recorded `CouldNotRun` is a **tool/host outcome on an iverilog devel
  build**, as its own commit message says. Under a **stable** iverilog
  13.0 the same case, same patch, same vector grammar does **not** hang:
  §3's 126-block windowed vector ran to `DONE` and produced a complete
  dump. Whether the **full-length** vector (to block 1875, ≈ 2 h on one
  core) also completes under the stable build is **NOT_RUN** here — a
  full-length attempt was started on this host and did not finish inside
  this work's window; before it was stopped it had progressed steadily,
  with an all-zero dump prefix byte-identical in shape to the committed
  iverilog artifact
  (`dev32-30-stable-iverilog-rerun-partial.json`, recorded as NOT_RUN).
- The **x-corruption is established independently of that failure**, by
  §2's probe, by §3's harness-native compare, and by `dev32-06` — a case
  that completed, was recorded as passing, and is silent under iverilog.

So the stop/escalate condition of #96 is met through `dev32-06`, not
through `dev32-30`'s crash.

## 7. What the iverilog evidence does and does not establish

- **Does**: the three stress cases with audio in their compared window
  (`stress-sum16`, `stress-reset`, `stress-patchcommit`) are bit-exact
  under iverilog over those windows. All of them are AMS = 0.
- **Does**: `dev32-03` (AMS = 0) is bit-exact under iverilog over a
  120-block window containing 45 blocks of live audio (§3) — so the dev
  set's audio is not *uniformly* unverified under iverilog any more, for
  AMS = 0.
- **Does not**: anything about the AM/exp() path, other than that it is
  broken under iverilog. No compared sample in the *recorded shadow* comes
  from a live note on a nonzero-AMS patch.
- **Does not**: anything about the dev set's audio beyond block 120, under
  iverilog, for any case.
- **Unchanged**: the Verilator acceptance evidence
  (`results-verilog-accept.json`, 34/34, full-length windows, two
  artifact-identical runs). Nothing here touches it, and the AMS ≠ 0 cases
  pass it with audio in the window.

## 8. Failure controls (all demonstrated)

| Control | Must | Observed |
|---|---|---|
| AMS zeroed on the same patch | stop the propagation, keep the source | `exp_hsum` still x at frame 0; 0 unknown samples downstream |
| Widened scratch RTL | remove the source and the propagation | `exp_hsum` never x; 0 unknown samples |
| Verilator, same probe | render the correct audio where iverilog renders zeros | bit-exact over the same 120-block window |
| Same stimulus in every probe control | render the same samples/voice/strike frame | 639 samples, `live=1`, strike frame 1 in all four runs |
| AMS = 0 sibling case (`dev32-03`), same window truncation, iverilog | must be bit-exact — otherwise the truncation, not the AM path, is what the FAIL measures | 7,680/7,680 bit-exact, 2,870 nonzero samples reproduced |
| Coverage metric on a non-vacuous run | must not flag everything | Verilator acceptance run: 33/34 windows have audio |
| AMS = 0 sibling case (`dev32-03`), committed artifacts | must be bit-identical across simulators | 0 diverging samples in 120,063 |

## 9. Reproduce

```bash
# path (a) + the coverage/divergence audit (no simulator, ~1 s)
python3 tools/h07_ams_audit.py \
    --out evidence/issue-96-ams-xprobe/h07-ams-coverage-audit.json

# path (b) through the conformance harness (~6 min per iverilog run)
python3 tools/ams_window_compare.py --case dev32-30 --tool iverilog
python3 tools/ams_window_compare.py --case dev32-06 --tool iverilog
python3 tools/ams_window_compare.py --case dev32-30 --tool verilator   # control
python3 tools/ams_window_compare.py --case dev32-03 --tool iverilog    # control

# path (b) as a direct 4-state observation: the probe and its three controls
python3 tools/ams_xprobe.py --case dev32-30 --sim iverilog   --runframes 8
python3 tools/ams_xprobe.py --case dev32-30 --sim iverilog   --runframes 8 --zero-ams
python3 tools/ams_xprobe.py --case dev32-30 --sim iverilog   --runframes 8 --widened
python3 tools/ams_xprobe.py --case dev32-30 --sim verilator  --runframes 8

# fast-lane re-derivation of all of the above from committed artifacts
python3 -m unittest tests.test_ams_audit -v
# optional live re-run of the probe inside the suite
GF180_ISSUE96_LIVE=1 python3 -m unittest tests.test_ams_audit -v
```

Tool identities and RTL hashes for the recorded runs:
`evidence/issue-96-ams-xprobe/tool-versions-2026-09-25.txt`.

## 10. What remains unproved

- **No fix is made here.** Widening the wires is #98's change and needs the
  DR-0009/DR-0011 re-freeze battery; until that lands the frozen core keeps
  this behaviour under iverilog.
- **The dev set has never been compared over its full length under
  iverilog.** §3 closes the gap for three cases over blocks 0..119; blocks
  120..1875 of every dev case remain uncompared under iverilog, and a
  full-length dev case is ≈ 2 h on one core here, so that belongs to a wave
  gate, not a builder loop. Until then the iverilog tier's dev-set verdicts
  are **STALE for coverage** beyond block 120, not merely bounded.
- **Whether a stable iverilog 13.0 completes the full-length `dev32-30`
  vector** is NOT_RUN (§6); only the 126-block windowed vector is known to
  complete.
- **Nothing here says the mapped netlist behaves like either simulator** —
  that is #86's FAIL and #98's escalation.
- **Nothing here concerns how the instrument sounds.** An all-zero render
  is a numeric finding about a simulator's 4-state semantics, not a
  listening result.
