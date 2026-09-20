# N02 — Integer phase/operator model (issue #16)

- **Deliverable:** one integer model of the single-operator path (phase
  accumulation, log-sine lookup, level scaling, exponentiation, sign),
  matching the N01 contract; complete table-domain checks.
- **Contract:** `gf180-dx7-integer-v1`
  ([spec/numeric-profile-v1.json](../spec/numeric-profile-v1.json),
  [DECISIONS-NUMERIC-v1](DECISIONS-NUMERIC-v1.md) NUM-001..NUM-004,
  NUM-010); frozen N01 arithmetic profile.
- **Model:** `src/gf180_dx7/model/operator.py` ·
  **Tables:** `src/gf180_dx7/model/tables.py` + `tools/gen_tables.py` →
  `reference/tables/` · **Probes:** `tests/test_operator.py` ·
  **Evidence:** `reference/evidence/n02-operator/reference-probe.json`.
- **Result: reference probe EXACT — byte-identical PCM, max_abs_error
  0.0** over a 192,000-sample dry render (declared budget: exact; see §7).

## 1. Scope and licensing

The pinned reference engine is Mark I (R01: `DEXED_ENGINE_MARKI`), whose
per-sample operator path is integer end-to-end. The GPL-3.0 file
`Source/EngineMkI.cpp` in the pinned Dexed tree (commit `b6cf2e9`, R01) is
used as a **semantic specification only**: constants, operation order,
shifts, and table-generation formulas were extracted as facts and
re-implemented in clean stdlib Python in this Apache-2.0 repository. Zero
GPL or MSFA bytes are copied (NUM-001; `docs/REUSE-RULINGS.md`
`dexed-enginemki`). The tables are **regenerated** by this repository's own
generator and pinned by SHA-256 — the gf180-parasynth
regenerate-and-hash discipline.

## 2. Operator semantics (facts, with citations)

Per sample (`EngineMkI.cpp:124-150`), with `SINLOG_BITDEPTH = 10`
(`:54`), `NEGATIVE_BIT = 0x8000` (`:51`):

```
phi       = (phase >> (22 - 10)) & 0xFFFF          // :125, uint16 truncation
expVal    = sinLog(phi) + env   (mod 2^16)         // :125
isSigned  = expVal & 0x8000                         // :128
expVal   &= 0x7FFF                                  // :129
result    = 4096 + sinExpTable[(expVal & 0x3FF) ^ 0x3FF]   // :131-132
result  >>= expVal >> 10                           // :135
out       = isSigned ? (-result - 1) << 13 : result << 13  // :146-149
```

Quarter-wave fold `sinLog(phi)` (`EngineMkI.cpp:64-78`): `index = phi &
1023`, quadrant = `phi & 3072`; quadrants 0/1 read the table directly /
mirrored (`index ^ 1023`), quadrants 2/3 repeat 0/1 with the sign bit
(`| 0x8000`). There is **no interpolation** — the reference itself
truncates to the 10-bit index (NUM-004).

Level scaling **is** the log-domain add of a 14-bit env gain
(`ENV_BITDEPTH = 14`, `:52`; `ENV_MAX = 1<<14 = 16384`, `:62`) followed by
the anti-log table above — multiply-free by construction (NUM-004).

**Phase convention (measured clarification):** the register is a 32-bit
wrapping accumulator (`int32_t phase`, `fm_op_kernel.h:20-25`; natural
wrap `EngineMkI.cpp:164,180,200`), but `mkiSin` consumes phase bits
12..27 only, so **one sine cycle = 2^24 phase units**. Consistency
check: `Freqlut::lookup(noteLogFreq(60)) = 91444`
(`freqlut.cc:46-55`; `noteLogFreq(60) = 50857777 + (2^24/12)·60`,
`tuning.cc:13-21`) gives `91444 · 48000 / 2^24 = 261.63 Hz` — and the
oracle render of the §6 probe shows a 183-sample period (= 262.3 Hz),
confirmed by the model's byte-identical render. The N01 profile's
"one cycle = 2^32" phrasing (NUM-003) describes the register width; the
cycle definition above is the operative semantics. No numeric deviation
is involved — this clarification is recorded here, and the frozen profile
is unchanged (change control: N01 §0).

## 3. Frame semantics (facts)

- N = 64, LG_N = 6 (`msfa/synth.h:29-30`).
- Intra-frame gain ramp (`EngineMkI.cpp:155,160-162`):
  `dgain = (gain2 - gain1 + (N >> 1)) >> LG_N`; `gain` starts at `gain1`
  and advances **before** each sample (sample i uses `gain1 + i·dgain`,
  i = 1..64); the shift is C arithmetic (floor for negatives).
- Frame forms: `compute` (adds per-sample `input[i]` to the phase,
  `:162`), `compute_pure` (`:178`), `compute_fb` (single-operator
  feedback: `scaled_fb = (y0 + y) >> (fb_shift + 1)`, `y0 = y; y =
  mkiSin(phase + scaled_fb, gain)`, history stored back at frame end,
  `:194-204`; two-element int32 history `msfa/dx7note.h:77`).
- Frame-end phase skip: `param.phase += param.freq << LG_N`
  (`EngineMkI.cpp:361`, `msfa/fm_core.cc:133`); the frame functions'
  local phase and this line agree (both = phase0 + 64·freq). The skip
  runs even for gated operators (`:318,361`).
- Gate (quoted for the probe): `kLevelThresh = ENV_MAX - 100 = 16284`
  (`:296`); an operator renders only when
  `gain1 <= 16284 || gain2 <= 16284` (`:314-318`), with the zero-gain
  substitution `gain1 = gain_out == 0 ? ENV_MAX-1 : gain_out` (`:314`).

## 4. Tables — regenerated, pinned, deterministic

Formulas (facts) and our generator's operation order:

| Table | Formula | Citation | sha256 (uint16/int32 LE) |
|---|---|---|---|
| `sinlog_table` | `round(-1024·log2(float32(sin(((0.5+i)/1024)·π/2))))` | `EngineMkI.cpp:83-86` | `990c19e90732efe712a19ba4272f97c7c9d884ac0e4e19450a1067165d8aa8a8` |
| `sinexp_table` | `round(float32((pow(2, float(i)/1024) − 1)·4096))` | `EngineMkI.cpp:88-92` | `e515a71ae736d92dcb3fd36973dea486c96d1521f1a0bab1f917be2c4ec07794` |
| `freqlut_table` | `y = 2^44/48000; lut[i] = floor(y+0.5); y *= 2^(1/1024)` (1025 × int32) | `freqlut.cc:35-42` | `6d3c597008e85c67cfa1165ef4038a3c1c9c56a60a4b441b7486296a897dc64d` |

- Operation order mirrored exactly: double-precision `sin`/`log2`/`pow`
  with the single C++ `float` narrowing at `x1` (struct float32
  round-trip in Python), then C `round()` = half-away-from-zero (both
  domains nonnegative). For `pow(2, float)`, C++ `<cmath>` resolves the
  double overload ([cmath.syn] additional-overload rule).
- **Measured determinism (this host, Apple clang 21.0.0 — the oracle's
  pinned toolchain):** a throwaway probe computing the formulas via the
  float-returning and double-returning overload variants produced
  byte-identical tables, and both hash to the values above — equal to the
  N01 `provisional_tables` records in
  `spec/numeric-profile-v1.json`. Boundary-margin argument (N01 §5):
  min half-boundary distance 1.8e-4 (sinlog) / 3.7e-5 (sinexp) against
  ~1e-12 double libm spread. The first 16-bit tables are
  platform-independent by argument **and** measurement.
- **Freqlut is NOT claimed byte-stable across libms** (N01 §5): the
  frozen model pins its own generated bytes; the reference probe below
  counts the actual divergence — it is zero on the pinned host.
- Artifacts: `reference/tables/<name>.hex` + `manifest.json`
  (sha256/entries/bits/formula_citation per table), regenerated by
  `tools/gen_tables.py` (`--check` verifies byte-identity; committed).
- The `Sin::lookup` sine table (msfa, for the LFO's sine branch,
  NUM-007) is **out of N02 scope** (no LFO in the single-operator probe);
  it is regenerated+pinned with the LFO work (N06-style follow-up) and
  its absence here gates nothing in this issue.

## 5. Output conversion (facts, NUM-010)

Per note per sample (`PluginProcessor.cpp:258-268`; oracle mirror
`main.cpp:455-465`): `val = block >> 4`;
`clip_val = val < -(1<<24) ? 0x8000 : val >= (1<<24) ? 0x7fff : val >> 9`;
`f = clip_val / 0x8000` (float32; negative overflow maps to **+1.0** —
the pinned quirk). `src/gf180_dx7/model/operator.py` exposes both the
`clip_val` product-transport domain and the f32le comparison encoding.

## 6. Reference probe (acceptance: "match pinned traces within budgets")

- **Voice (via the P01 codec):** algorithm 32 (byte 31) — all six
  operators are output carriers; ops 1-5 at total level 0 ⇒ effective
  outlevel 0 ⇒ frame gain `16384 − (16<<16 >> 14) = 16320 > 16284` ⇒
  envelope-gated, contributing exactly zero from frame 1 (analytically
  and empirically silent); op 6 (TL 99) is the only audible, unmodulated
  operator. Rates 99 + equal levels park every envelope at its target
  from the first frame; pitch-EG levels 50 (`pitchenv_tab[50] = 0`) and
  neutral LFO/mod/scale bytes make the pitch path constant.
- **Probe-only constant-gain path:** frame 1 `gain1 = ENV_MAX−1`
  (zero-gain substitution), later frames `gain1 = gain2 = 1024` (from the
  frozen target formula `env.cc:113-121`, `dx7note.cc:169-179`,
  `EngineMkI.cpp:314-316`) ⇒ `dgain = 0`. This minimal path exists only
  to drive the operator with reference-exact gains; **N03 remains the
  owner of the general envelope** and nothing here weakens it.
- **Run:** pinned oracle binary (sha256 verified against
  `reference/oracle-identity.json`) renders `note_on 60 100` at sample 0,
  4 s dry f32le @ 48 kHz; the model renders the same patch+events; the
  streams are compared with `src/gf180_dx7/compare.py`
  (exact-time-locked, no alignment/normalization).
- **Evidence:** `reference/evidence/n02-operator/reference-probe.json`
  (input/output SHA-256s, metrics, budget, status PASS);
  `tests/test_operator.py::TestReferenceProbe` re-renders live and fails
  if the artifact goes stale.

## 7. Budget statement

**Declared budget: exact (max_abs_error = 0.0, byte-identical).**
Justification: the operator path contains no float steps and no free
numeric choices; with tables byte-identical to the pinned oracle's
runtime tables (§4) and every integer operation mirrored (§2-§3, §5),
any nonzero error indicates a model defect, not an acceptable numeric
budget. **Measured: exact — byte-identical PCM, max_abs_error 0.0 over
192,000 samples** (`reference/evidence/n02-operator/reference-probe.json`).

**Deviations: none.** One semantic clarification (cycle = 2^24 phase
units, §2) is recorded; it changes no bytes and no frozen rule.

## 8. Probe suite and negative controls (all live, all pass)

`python3 -m unittest tests.test_operator -v` — 24 tests:

- **Analytic:** full-domain sinlog/sinexp/freqlut checks (every entry
  against independently re-coded formulas); quadrant mirror symmetry;
  non-increasing first quadrant; monotone amplitude under monotone env
  gain (closed form); known-value spot checks computed from the
  documented formula in the test; phase wraparound equivalence;
  sign-reconstruction asymmetry `neg = -pos - 8192`; frame gain ramp and
  phase-carry identities; Freqlut known values incl. the 261.63 Hz
  cross-check.
- **Negative controls (each demonstrably fails its target):**
  1. exp-table index off-by-one mutant → probe suite fails;
  2. inverted sign-bit mutant → probe suite fails;
  3. one flipped hex digit in a pinned table → `load_pinned` raises
     `TableDrift` (SHA-256 manifest check);
  4. end-to-end: a one-entry sinlog mutation changes the model render so
     the byte-identical reference comparison fails — the reference probe
     detects table drift.

## 9. What remains unproved (honesty ledger)

- This verifies the **model vs pinned software reference** only. It is
  not RTL agreement (H01), not original-DX7 fidelity, and says nothing
  about musical usefulness (listening records only).
- Multi-operator routing, feedback topology/policy (N04), envelopes
  (N03), pitch/mod (N05), and the LFO sine table (NUM-007) are out of
  scope; the reference probe's constant-gain path does not pre-verify
  them.
- Freqlut byte-stability across *other* libms is not proven (nor
  claimed); the ≤1 phase-increment-LSB budget (NUM-003/NUM-008) stands
  with the measured zero on the pinned host.
