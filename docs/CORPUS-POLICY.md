# Development-corpus policy (R03, issue #10)

Scope: how `tools/select_dev32.py` builds `corpus/dev32.json` (the first
32-patch DEVELOPMENT manifest) from the committed P02 catalog, how
`tools/validate_dev32.py` validates and render-proofs it, and what this
corpus deliberately is not.

## What this corpus is (and is not)

This is a **compatibility corpus**: real archived patches chosen to expose
engine behavior, with explicit feature coverage and full source identity.
It is **not** a favorites list. Taste, character, velocity feel and
musical usefulness are curated later in U02/U03 against listening records;
numeric tests never establish that a patch sounds good (plan section 7).
"Musically recognizable" at R03 means only that every entry is a real
archive patch with its real display name and provenance recorded — factory
anchors are deliberately *not* hand-picked here; that is U02's curation
job.

## Input pinning

Selection reads two pinned inputs:

- the **committed catalog** `corpus/archive-catalogs/alltheweb-catalog.json`
  (P02); its file SHA-256 is recorded in the manifest
  (`selection.catalog_snapshot_sha256`). Validation recomputes the hash and
  rejects the manifest as STALE on any drift.
- the **archive zip** (read-only), SHA-256-checked against
  `catalog.archive.sha256` before any decode. Voice bytes are never
  redistributed; the zip supplies decoded feature values only.

If the zip is absent, selection exits 2 (NOT_RUN) naming the path — the
skip-with-reason pattern; `--validate` never needs the zip.

## Candidate universe

A candidate is a **decoded, anomaly-free voice instance** in the committed
catalog: standalone `voice` entries and `bank` slot rows with
`anomaly_count == 0` (161,925 instances, 15,209 distinct canonical hashes
in this catalog). Stream-class instances are excluded from selection for
extraction simplicity (27 files / 1,728 instances vs 307k bank instances);
this is a documented scope decision, not a coverage hole — the bank slots
alone cover every feature goal.

Features are functions of the canonical parameters, so one decode per
distinct canonical hash suffices; the catalog's per-instance rows supply
path/slot/hash/name/anomaly data without decoding.

## Feature definitions

| feature | definition (decode-only) |
|---|---|
| `pitch_eg_present` | any pitch-EG level ≠ 50. Level 50 is the zero-deviation center of the pitch-EG table (MSFA `pitchenv_tab[50] == 0`, Dexed `Source/msfa/pitchenv.cc`, Apache-2.0 — cited as external reference, code not copied). Counting any nonzero level would mislabel flat-centered voices. |
| `lfo_wave_W` | decoded LFO waveform == W (0–3 are goals per issue #10; 4–5 exist in the archive and are reported if present) |
| `fixed_frequency_op` | at least one operator `osc_mode == 1` (fixed frequency) |
| `high_feedback` | feedback ≥ 4 (0–7 range) |
| `low_feedback` | feedback ≤ 0 (i.e. 0) |
| `velocity_sensitive` | at least one operator `key_velocity_sensitivity > 0` |

## Selection (deterministic, documented)

Stages run in this order; ties break by sort key
`(zip path, bank slot with standalone voices first, catalog order)`.
No randomness, no taste, no filesystem-order dependence.

1. **Stage A — algorithm coverage:** for algorithm 0..31 ascending, the
   lexicographically-first candidate with that algorithm and an unused
   canonical hash. All 32 algorithms are covered in this catalog, so this
   fills 32 slots; if an algorithm had no clean candidate it would be
   recorded in `coverage.algorithms_missing` / `algorithm_holes`
   (machine-readable) and escalation is required by the issue's
   stop/escalate condition.
2. **Stage B — feature goals:** goals in the fixed order
   `pitch_eg_present, lfo_wave_0..3, fixed_frequency_op, high_feedback,
   low_feedback, velocity_sensitive`. A goal already covered by the
   selected set is skipped. Otherwise the lexicographically-first unused
   candidate satisfying the goal is appended (if capacity remains) or
   swaps into its own algorithm's slot, keeping that algorithm covered
   (the swap is recorded in `selection.stages.stage_b_swaps`). An unsatisfiable
   goal is recorded in `coverage.goal_holes` (machine-readable), never
   silently dropped and never allowed to weaken the coverage rule.
3. **Stage C — fill:** any remaining slots take the lexicographically-first
   unused candidates (tagged `fill`). Not needed in this catalog.

Uniqueness invariant: 32 entries, 32 distinct canonical hashes, preserved
by construction (replacement releases the old hash).

## Why these goals

Plan section 7 ("Useful directed cases") requires all algorithms —
especially 4 and 6 feedback — pitch EG, every LFO waveform, fixed as well
as ratio frequency, and velocity sensitivity among the directed cases.
Stage A guarantees algorithm breadth (including 4 and 6); the goals force
the remaining dimensions to be explicit rather than incidental, so model
comparisons and challenge runs (R04+, N06+) inherit a corpus that already
exercises them. The catalog cannot express events/controller coverage
(sustain, bend, all-sound-off) — that is R06's registry, not R03's.

## Events and render convention

Every entry carries explicit, renderer-executable events (oracle
`events.txt` line format: `<pos> <cmd> <args...>`, positions in absolute
samples) and render settings (48000 Hz; 4.0 s default, 6.0 s for slow
patches). Profiles, applied in precedence order `slow_hold` >
`velocity_contrast` > `default`:

| profile | trigger | events | render |
|---|---|---|---|
| `slow_hold` | any operator envelope rate ≤ 25 | note_on v100 @ 4800; note_off @ 144000 (3 s hold) | 6.0 s |
| `velocity_contrast` | velocity-sensitive | strikes at velocity 100 then 30 | 4.0 s |
| `default` | otherwise | note_on v100 @ 4800; note_off @ 96000 (2 s hold) | 4.0 s |

Slow releases are not forced to decay inside the window: a patch that is
still sounding at note-off is not invalid (plan section 3). Renders are
dry; no per-render normalization.

## Validation contract

`select_dev32.py --validate` (zip-free) checks: schema; exactly 32 entries
with unique canonical hashes; every hash resolves in the committed catalog
at the entry's own (path, slot); names match the catalog; selected/decoded
feature fields present; the coverage table matches an independent
recomputation from per-entry fields; every event/render set is explicit,
parseable and inside the render window; catalog snapshot and archive
identity match (drift ⇒ STALE). Negative controls (kept live in
`tests/test_dev32.py`): a foreign source hash, a duplicate hash, and
missing events/render settings/unrenderable events must each fail
validation — a control must demonstrably fail the check it targets.

## Render-proof

`tools/validate_dev32.py --render-sample 3` validates, then extracts the
first 3 manifest entries from the pinned zip (bank slots re-encoded to
single-voice SysEx via the P01 codec; canonical hash re-checked), renders
each with the external GPL oracle pinned in
`reference/oracle-identity.json` (binary hash verified before use,
`--oracle` overrides the executed file, never the expected hash), enforces
the non-silent guard, and records output hashes/peaks in
`corpus/dev32-renderproof.json` (committed). This is a **renderability
proof only** — not a comparison, agreement, or musical-quality claim; the
comparison protocol is R02/R04. Full-32 rendering is a later gate and is
explicitly recorded as not run.

## Exit codes

`0` PASS, `1` FAIL (validation failure, drift, silent render), `2`
NOT_RUN (environment: missing catalog/manifest/zip/oracle). A run that did
not happen is never reported as a pass.
