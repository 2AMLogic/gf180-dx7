# N08 — Frozen fixed-model release `release-vectors-v1` (issue #24)

- **Issue:** [#24](https://github.com/2AMLogic/gf180-dx7/issues/24) · epic #2 ·
  planning ID N08
- **Preregistration:**
  [spec/release-vectors-v1.json](../spec/release-vectors-v1.json) ·
  **Freeze DR:**
  [DR-0007](decision-records/0007-release-freeze-v1.md)
- **Bundle:** `reference/release/` —
  `manifest.json` (bundle sha256
  `81bcc43cd49a29f35b21370733bb9888dd5641862501b441136e122485f54f8c`) +
  `vectors/<case>.f32` (51 files, 15,456,000 bytes)
- **Tools:** [tools/n08_build_release.py](../tools/n08_build_release.py) ·
  [tools/n08_verify_release.py](../tools/n08_verify_release.py) ·
  [tools/n08_mutations.py](../tools/n08_mutations.py) ·
  [tools/n08_challenge.py](../tools/n08_challenge.py)
- **Tests:** [tests/test_n08_release.py](../tests/test_n08_release.py)
  (25 cases, green) · **Records:** `reference/release/challenge-results.json`,
  `reference/release/mutation-results.json` (both bound to the bundle hash)
- **Result: the frozen fixed-model release is committed and verified —
  51 frozen vectors (30 dev + 21 challenge), 21/21 challenge cases
  AGREE_EXACT at the frozen thresholds (zero bounded findings), 5/5
  mutations detected by the verifier, tamper and STALE negative controls
  live. This release authorizes exact RTL work (H03+).** It advances the
  RTL-oracle claim only — not original-DX7 fidelity, not RTL agreement
  itself, not a musical claim (DEC-001, DEC-024, plan §7).

## 1. What is in the release

| Item | Content |
|---|---|
| Model identity | sha256 of the 10 frozen modules (sysex + model/*, N06+N07 tree, byte-unchanged); renderer = D00 `PolyManager` around the integrated single-note composition |
| Arithmetic profile pointer | `spec/numeric-profile-v1.json` by sha256; per-property error budgets DERIVED from its `decision_index` + float-gated spots (recorded, never tuned) |
| Vectors | 51 dry f32le renders: 30 dev (23 N06-exact single-note + 4 N07 wrapper-valid poly + dev32-03/06/30) + 21 preregistered challenge |
| Traces | per-case block-level event-application logs (138 events) + poly `alloc_log` entries |
| Thresholds | vector conformance = sha256-exact (claim 1); model-vs-oracle = byte-identical f32le (claim 2, challenge only); `thresholds_frozen_before_challenge: true` |
| Exclusions | lfo-keysync0 (F-4 indeterminate reference), ev-steal-17 (documented DEC-017 divergence), 3 registry-BLOCKED cases, 3 corpus-selector cases — each with its reason |
| Bundle hash | covers case records, thresholds, budgets, model identity, vector shas; provenance (`built_at_commit`) deliberately excluded so rebuilds stay byte-identical |

Build determinism: the full bundle was built twice at freeze time —
byte-identical (`diff -r` clean, same bundle hash). The unittest suite
additionally builds a subset twice and compares every byte.

## 2. Verification commands

```sh
# H-series conformance gate (exit 0/1/2; full integrity + full re-render):
python3 tools/n08_verify_release.py

# integrity only (fast), or a re-render subset:
python3 tools/n08_verify_release.py --no-rerender
python3 tools/n08_verify_release.py --only dir-base,ev-sum-15

# rebuild from the frozen tree (must be byte-identical):
python3 tools/n08_build_release.py

# frozen-threshold challenge run (requires the pinned oracle):
python3 tools/n08_challenge.py

# mutation battery (requires nothing but the bundle):
python3 tools/n08_mutations.py
```

Exit codes for the verifier: `0` intact (NOT_RUN rows reported loudly,
never as passes), `1` tamper/drift/threshold failure **naming the case**,
`2` bundle missing. Any model-module, numeric-profile, or preregistration
change fails the verifier and makes **all H-series conformance STALE**
(DR-0007).

## 3. Challenge run (frozen thresholds)

Preregistered before any challenge output was inspected; thresholds and
results are committed together and any threshold change is detectably a
preregistration edit (bundle hash + spec binding break). Verdicts:

| Verdict | Count |
|---|---|
| AGREE_EXACT (byte-identical, max_abs_error 0.0) | **21 / 21** |
| FAIL (bounded finding at frozen thresholds) | 0 |
| MODEL_MISMATCH / NOT_RUN | 0 |

Notable rows: `ampmod-ams3` — the amp-mod-sensitivity path with a
non-inert LFO, the exact combination N06 declared NO_VERDICT — is
**byte-exact** against the pinned oracle; `osc-keysync0` reproduces the
registry's observed-identical pairing with `osc-keysync1` (second note on
a fresh rotation slot); `perf-transpose-12` reproduces the pinned-wrapper
transpose finding. Had any row failed, it would be recorded as a bounded
finding here with no retuning and no case swap — none did.

## 4. Mutation coverage (live negative controls)

Each mutation is applied to the frozen model at runtime; the release
verification must CATCH it (re-rendered sha256 diverges; verifier exits 1
naming the case). Committed results: `reference/release/mutation-results.json`.

| Mutation | Defect class | Seam | Detected on |
|---|---|---|---|
| `param_shuffle` | wrong operator→envelope mapping | envelope param derivation | dir-base, ev-repeated-note |
| `wrong_gain` | +6 dB datapath gain (`>>8` vs `>>9`) | output conversion | dir-base, ev-pitch-bend-up |
| `broken_feedback` | feedback history delayed one block | `AlgorithmRouter.render` fb_buf | dir-base, alg4-fb7 |
| `silent_stub` | dead/stale module renders silence | per-voice frame compute | dir-base, ev-sustain-pedal |
| `voice_sum_order_swap` | mix pipeline order swapped (sum-then-clip vs frozen clip-then-sum) | poly block mix | ev-sum-15 (the only preregistered case whose voices actually sum past the per-voice clip rails) |

**5/5 detected.** The tamper negative control is exercised live in the
unittest suite: one flipped byte in a copied vector fails verification
naming the case; a drifted model-module hash fails the STALE rule.

## 5. Known bounded findings and exclusions (with reasons)

- **`lfo-keysync0` excluded** — reference-side C++-indeterminate LFO phase
  (N06 finding F-4 / R05); no derivable oracle value exists, so the case is
  not freezable as oracle-comparable. Needs an owner ruling first.
- **`ev-steal-17` excluded** — documented DEC-017 (model steals) vs wrapper
  (drops the 17th note) divergence; no equality budget may be declared.
- **`ev-aftertouch`, `ev-volume-cc7`, `ev-patch-change-held` excluded** —
  registry BLOCKED (no executable oracle path; the D00 `patch` event is
  model grammar only).
- **Corpus-selector registry cases excluded from the challenge** —
  archive-gated voice bytes; the challenge stays reproducible everywhere.
  Their axes are covered by dev rows.
- **dev32-03/06/30 are archive-gated for re-rendering** — committed
  vectors are environment-independent; re-rendering those three rows
  requires the pinned archive (guarded NOT_RUN otherwise, never a pass).
- **No polyphonic challenge case** — the registry offers none beyond the
  dev set; poly coverage = dev set + N07 evidence. Recorded gap.
- **Inherited unproved:** keysync-0 LFO-active model-vs-reference agreement
  (F-4). **Not claimed:** RTL agreement (that is what H03+ must now
  establish against this release), original-DX7 fidelity, preset quality,
  musical usefulness (listening records only, plan §7).

## 6. Status

- Build, verify, challenge, mutations: **PASS** (see §2–4); tests 25/25.
- A release **tag** should be cut at merge by the owner (issue evidence
  names a release tag; tagging a pre-merge commit from a worktree branch
  would pin the wrong tree).
- The full-bundle rebuild determinism and full-bundle verify were run at
  freeze time and are recorded here; CI re-runs the unittest suite
  (subset build determinism + integrity + live controls) without needing
  the oracle or the archive.
