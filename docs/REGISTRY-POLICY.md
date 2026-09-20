# Registry policy — R06 directed compatibility cases (issue #13)

Status: normative for `spec/compatibility-registry-v1.json`, its validators
(`tools/registry_coverage.py`, `tools/check_registry.py`) and its executor
(`tools/registry_smoke.py`). The plan source is
`docs/dx7-chip-plan-v0.1-2026-09-20.md` section 7 ("Useful directed cases")
and the R06 row in section 8. The pinned oracle is defined in
`reference/oracle-identity.json` (R01/R02); the available trace taps in
`reference/trace-registry.json` (R05).

## 1. What the registry is

One machine-readable mapping from **every decodable voice parameter** (the
P01 canonical supported-field list, taken by reflection over the
`Voice`/`Operator` dataclasses in `src/gf180_dx7/sysex.py`) and **all 32
algorithms** to **directed cases**: patch inputs, event sequences, and a
property-level expectation. The registry is committed data; validators
enforce completeness and emit coverage holes as JSON, never prose.

Claim boundary: a registered case is a *directed input*, not a result.
Coverage (this registry) is reported separately from agreement (render
comparisons) and from musical usefulness (listening records). Nothing here
asserts original-hardware fidelity or preset quality.

## 2. Case taxonomy

| Section | Meaning |
|---|---|
| `fields[]` | one row per supported parameter (`kind` `op`/`common` from the P01 dataclasses), naming its `directed_cases` and `boundary_cases`. Codec bookkeeping members (`operators`, `channel_byte`, `anomalies`, `raw_bytes`) are excluded — the P01 test suite owns them. `name`/`name_bytes` are covered by the name-identity cases (a display-name change must never change audio). |
| `algorithms[]` | codec indices 0–31, each with ≥ 1 case; algorithms 4 and 6 carry feedback-specific cases at feedback 0, 3 (mid) and 7. |
| `event_interactions[]` | performance-event cases from plan section 7: repeated notes, retrigger, sustain, stealing (17th note), patch change while held, all-sound-off / all-notes-off, pitch-bend range, modwheel, aftertouch. |
| `boundary[]` | named input boundaries: ratio/fixed switch, coarse 0 and extremes, detune extremes, every LFO waveform (0–5) and both LFO/osc sync modes, LFO delay max, slow rates (rate = 1), fast rates (99), rising envelope segments, delayed attack, nonzero final level, pitch EG up/down, keyboard scaling curves 0–3, velocity sensitivity, rate scaling, maximum voice summation (16 notes). |
| `performance_controls[]` | the DEC-006 v1 control set with an explicit machine-readable status: `covered`, `covered-observed-inert`, or `blocked`. |

Every case row has: `id`, `patch` (selector `synthetic` with explicit
overrides against the committed `synthetic_base`, or `corpus` reusing a
committed canonical hash), `events` (oracle `events.txt` lines; positions
are absolute sample offsets at 48 kHz), `render_seconds`, and `expect`
(one primary `property` plus optional `also` predicates and a human-readable
description). **Expectations are property-level** — non-silence,
hash-distinctness, onset/sustain windows, applied-event counts, or an
explicitly `manual:` judgment — never numeric vectors. Numeric comparison
vectors arrive at N08, drawn from this registry.

## 3. Synthetic vs corpus

* `synthetic` cases build their patch from the registry's committed
  `synthetic_base` plus explicit overrides (`op.<field>` = all six
  operators, `opN.<field>` = operator N, `<common field>`, or the display
  `name`). The base is data, so every case's full patch is reconstructible
  from the registry alone; the P01 codec encodes it and the decode round
  trip must be anomaly-free.
* `corpus` cases reuse a committed canonical hash from the pinned archive
  catalog. **Voice bytes are never redistributed** (corpus rights policy);
  they resolve from the pinned archive at challenge gates. The smoke
  therefore records corpus cases as `NOT_RUN`, never as a pass or a skip
  that masquerades as coverage.

## 4. Verdict semantics (smoke evidence)

`reference/evidence/registry-smoke.json` records one row per executed or
registered non-executable case:

| Verdict | Meaning |
|---|---|
| `PASS` | every machine predicate held against a pinned-oracle render. |
| `FAIL` | any machine predicate failed — a real finding about the pinned reference, never silently weakened; fix the case design only with a recorded justification. |
| `NO_VERDICT` (`manual: true`, `method: manual-listening`) | the expectation is a human listening judgment (`manual:...` properties such as "audible pitch differs"). Machine preconditions may be listed under `also` and must pass; the judgment itself is **never guessed from a hash**. |
| `BLOCKED` | no executable path exists on the pinned oracle (e.g. aftertouch, volume CC 7, mid-render patch change). The directed case and its reason stay registered per the issue's stop/escalate rule instead of inventing a substitute. |
| `NOT_RUN` | corpus-selector cases awaiting archive resolution. |
| `STALE` | an evidence row whose `input_fingerprint` no longer matches the case (see §6). |

Statuses follow the repo convention: coverage is reported separately from
agreement; a test that did not run is never a pass.

## 5. Observed-reference findings encoded as expectations

Three cases encode **observed pinned-oracle behavior** that differs from
naive DX7 expectations, each recorded as a bounded finding instead of a
silent weakening:

* `perf-transpose-12` — the pinned wrapper's keydown omits the
  `data[144]` transpose shift that upstream Dexed applies; transpose
  overrides render byte-identically to the base (finding recorded against
  the wrapper; DEC-023 still requires the model to preserve transpose).
* `ev-modwheel-127` — the pinned controller-modifier defaults
  (`mod_wheel_default_modifiers: range=0`) leave CC 1 inert; the render is
  byte-identical to CC 1 = 0.
* `ev-steal-17` — the 16-voice allocator drops the 17th note (no stealing);
  the 17-note render is byte-identical to the 16-note render (N07 must
  reproduce or consciously revise this allocation policy).

The LFO caveat: `lfo_value`/`lfo_delay` are instance-dependent across
oracle processes (R05 finding), so LFO-active renders support **one-sided**
difference claims only; identity claims (`identical_hash_vs`) are only
paired between LFO-inert renders. The smoke renders each case once and
makes no cross-run determinism claim (R02 owns determinism).

## 6. STALE fingerprints and the holdout seal

Each case carries a deterministic `input_fingerprint` (SHA-256 over its
patch, events, render config and expectation; computed by
`tools/registry_coverage.py:input_fingerprint` and recorded in every
evidence row). **Changing a case's covered inputs changes the fingerprint;
evidence rows with a stale fingerprint are STALE** and must be re-executed
before they count again.

`holdout` records the seal: `challenge_subset` stays **empty until
acceptance thresholds are frozen at N08**. Until the seal, challenge runs
must not draw from holdout cases and the validator rejects a non-empty
challenge subset while `sealed` is false. This follows the torchsynth
case-registry prior art (preregistered cases; result discovery cannot
shrink the denominator; sealed holdout until thresholds freeze).

## 7. Short subset and gates

`short_subset` marks cases for frequent runs; the smoke must stay well
under 30 s (it currently renders 28 cases in ≈ 1 s). Hash-comparison
partners of short-subset cases must themselves be in the subset (validated).
`tools/check_registry.py` (wired into `tools/check_all.sh`) gates commits on
zero coverage holes; `tools/registry_coverage.py --selftest-negative` proves
resolution: dropping an algorithm's cases, dropping a field's cases, or
emptying the short subset must each fail validation with the hole named.

## 8. Relation to N08 and the H-series

N08 (frozen fixed-model release and vectors) draws its challenge cases from
this registry: challenge selection must respect the holdout seal, and any
registry edit after selection re-opens the seal consciously. H-series RTL
conformance compares the RTL against the frozen model on registry cases at
the same seams; registry coverage is necessary but never sufficient —
agreement budgets and listening records live in their own evidence records.
