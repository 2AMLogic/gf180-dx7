# U02 listening kit — one sitting (issue #35)

Generated: 2026-09-22T16:55:26Z  
Manifest: `corpus/u02-candidates/candidates.json` (gf180-dx7.u02-candidates/1, 24 candidates = 8 families x 3)  
Catalog snapshot: `corpus/archive-catalogs/alltheweb-catalog.json` sha256 `9bfe25ca706f15f6…`  
Archive pin: `DX7_AllTheWeb.zip` sha256 `c2c47bd1aa29ab07…` (local-only; not redistributed)

## What this kit is (and is not)

- Candidates across the plan-section-6 families were shortlisted by documented deterministic heuristics over decoded parameters (`candidates.json` → `selection.heuristics`). **Heuristics select; humans decide.** No taste judgment was made by the tool or its author — your listening records are the point of this kit.
- Excluded: the dev32 development patches (32 hashes) and already-audited favorites (1 hash(es)).
- Identity is the P02 canonical hash everywhere; names are display sugar. Per-candidate provenance (archive path / bank slot / hash) is in `candidates.json`.
- All renders are **dry** oracle renders (pinned Dexed Mark I binary, hash verified): peaks are recorded, never altered. No normalization, no time-warp, no engine switching per patch.
- Oracle renders prove selection and reproduction only. Dexed agreement is not original-DX7 fidelity, and nothing here establishes that a patch sounds good — that is exactly what this sitting produces.

## One-sitting protocol (~45–60 min)

0. **Generate the audio** (idempotent, a few minutes — 288 oracle renders):
   ```sh
   python3 tools/u02_candidates.py render-kit
   ```
   The pinned oracle identity is verified before the batch; missing zip/oracle exits 2 naming the file.
1. **Listen family by family, candidates in table order.** For each candidate play the 12 phrase renders in this order:
   `mid-60-vel100` (anchor) → `low-36-vel100` → `high-84-vel100` → `mid-60-vel32` / `mid-60-vel80` / `mid-60-vel127` (velocity response) → `short-pluck-60` / `long-hold-60` / `release-tail-60` (envelope) → `chord-3note-60-64-67` → `modwheel-sweep-60` → `pitch-bend-60`.
   Each render dir has one subdirectory per phrase; listen to `render.wav`.
2. **What to listen for, per family:**
   - **EPs/tines** — bell-like tine attack over a rounded body; strike transient vs sustain balance; velocity response should open the tine, not just get louder.
   - **basses** — fundamental weight at MIDI 36; attack definition; whether the low register stays tuned and non-muddy.
   - **bells/mallets** — strike transient, inharmonic shimmer, how long the decay rings at high register; release tail behaviour.
   - **organs** — steady sustain without wobble; drawbar-style timbre steps across registers; attack should be immediate.
   - **brass/reeds** — breath/reed grit at the attack, sustain stability under mod-wheel, velocity dynamics.
   - **plucks** — attack snap, decay speed, whether the pluck repeats cleanly at low velocities.
   - **pads/strings** — slow attack bloom, sustain evenness, detune width/ beating; watch for phasey or muddy chords.
   - **percussion/textures** — fixed-frequency sweeps/noise-like behaviour, attack transients, whether textures evolve under mod-wheel/bend.
3. **Record a verdict per candidate in the table below** (`character` / `velocity response` / `keyboard range` / `verdict` / `notes`). U02 acceptance requires every entry to carry listening notes from the phrase set and a verdict — **selected** or **rejected with reason**. Candidate count is 24; U02's bank is 32 patches, so also note near-misses worth pulling from the catalog later.
4. **Technical failures are compatibility bugs, not taste.** Stuck notes, wrong envelope behavior, silence where a note should sound — file an issue against the owning E1/E2 issue and reference it in `notes`; never absorb it into “don't like it”.
5. **Promote keepers as you go** (records the favorite + per-phrase hashes for later recall):
   ```sh
   python3 tools/audition.py favorite --hash <64-hex> --note "..."
   python3 tools/audition.py recall   --hash <64-hex>   # later: must reproduce byte-identically
   ```
6. **Transfer the completed table** into the bank manifest + listening records and close #35 with the evidence links. This kit does **not** close #35 — the listening itself is the human gate; this kit only makes it turnkey.

## Candidate table

| # | family | name | hash | why shortlisted | renders | character | velocity response | keyboard range | verdict | notes |
|---|--------|------|------|-----------------|---------|-----------|-------------------|---------------|---------|-------|
| 0 | EPs/tines | voices#3 | `6002d1564074` | exactly two live carriers (algorithm 1); moderate feedback 2; decaying sustain (loudest carrier 99 -> 0); medium attack (loudest carrier rate1 53) | `corpus/u02-candidates/renders/6002d1564074-voices-3/` |  |  |  |  |  |
| 1 | EPs/tines | UltraBasso | `18d9f4cc17cc` | exactly two live carriers (algorithm 4); moderate feedback 2; decaying sustain (loudest carrier 99 -> 0); medium attack (loudest carrier rate1 49) | `corpus/u02-candidates/renders/18d9f4cc17cc-UltraBasso/` |  |  |  |  |  |
| 2 | EPs/tines | GIZMO..... | `6e3287174ffb` | exactly two live carriers (algorithm 4); moderate feedback 3; decaying sustain (loudest carrier 98 -> 0); medium attack (loudest carrier rate1 44) | `corpus/u02-candidates/renders/6e3287174ffb-GIZMO/` |  |  |  |  |  |
| 3 | basses | DUBBS BASS | `25e891d215f2` | single dominant carrier (loudest level >= 60); fast attack (all live carriers rate1 >= 60); low output tilt: modulator mass 301 >= carrier mass 99; decayed sustain (loudest carrier level4 0 <= 45) | `corpus/u02-candidates/renders/25e891d215f2-DUBBS-BASS/` |  |  |  |  |  |
| 4 | basses | GUNSHOT | `d3a8b5667f0c` | single dominant carrier (loudest level >= 60); fast attack (all live carriers rate1 >= 60); low output tilt: modulator mass 423 >= carrier mass 99; decayed sustain (loudest carrier level4 0 <= 45) | `corpus/u02-candidates/renders/d3a8b5667f0c-GUNSHOT/` |  |  |  |  |  |
| 5 | basses | EXPLOSION | `516191b9dacb` | single dominant carrier (loudest level >= 60); fast attack (all live carriers rate1 >= 60); low output tilt: modulator mass 423 >= carrier mass 70; decayed sustain (loudest carrier level4 0 <= 45) | `corpus/u02-candidates/renders/516191b9dacb-EXPLOSION/` |  |  |  |  |  |
| 6 | bells/mallets | SUS ROADS | `09300a239289` | high modulation index (max modulator level 91); inharmonic partials (non-unison ratio/detune); struck attack (all live carriers rate1 >= 60); long decay to silence (min carrier level4 0 <= 15) | `corpus/u02-candidates/renders/09300a239289-SUS-ROADS/` |  |  |  |  |  |
| 7 | bells/mallets | ZIMBALONE | `ff8eae172231` | high modulation index (max modulator level 99); inharmonic partials (non-unison ratio/detune); struck attack (all live carriers rate1 >= 60); long decay to silence (min carrier level4 0 <= 15) | `corpus/u02-candidates/renders/ff8eae172231-ZIMBALONE/` |  |  |  |  |  |
| 8 | bells/mallets | THUNDER-R | `269b629d92c1` | high modulation index (max modulator level 99); inharmonic partials (non-unison ratio/detune); struck attack (all live carriers rate1 >= 60); long decay to silence (min carrier level4 0 <= 15) | `corpus/u02-candidates/renders/269b629d92c1-THUNDER-R/` |  |  |  |  |  |
| 9 | organs | Downstairs | `c4d246ec7877` | sustained carriers (min level4 96); clean tone (feedback 1 <= 1) | `corpus/u02-candidates/renders/c4d246ec7877-Downstairs/` |  |  |  |  |  |
| 10 | organs | MajecFlute | `3a6e9efb4e47` | sustained carriers (min level4 80); clean tone (feedback 0 <= 1) | `corpus/u02-candidates/renders/3a6e9efb4e47-MajecFlute/` |  |  |  |  |  |
| 11 | organs | Akano.Yoru | `73b49d47568f` | sustained carriers (min level4 99); drawbar stack (3 live carriers); no decay sag (level3 == level4 on all carriers); clean tone (feedback 0 <= 1) | `corpus/u02-candidates/renders/73b49d47568f-Akano-Yoru/` |  |  |  |  |  |
| 12 | brass/reeds | MOSQUITO | `f821a61b8b7a` | sustained carriers (min level4 85); 1-2 carrier voicing (1); medium attack (max carrier rate1 48); velocity dynamics present | `corpus/u02-candidates/renders/f821a61b8b7a-MOSQUITO/` |  |  |  |  |  |
| 13 | brass/reeds | FALLSTAIRS | `b31f6215dcc7` | sustained carriers (min level4 96); 1-2 carrier voicing (2); velocity dynamics present; deep modulation (max modulator level 96 >= 60) | `corpus/u02-candidates/renders/b31f6215dcc7-FALLSTAIRS/` |  |  |  |  |  |
| 14 | brass/reeds | LFO.Pulse | `0dff9feb46b8` | sustained carriers (min level4 96); 1-2 carrier voicing (2); feedback growl (4); velocity dynamics present | `corpus/u02-candidates/renders/0dff9feb46b8-LFO-Pulse/` |  |  |  |  |  |
| 15 | plucks | TOUCHSYNTH | `c32a0cd1a1c6` | percussive attack (all live carriers rate1 >= 70); steep decay (loudest carrier 99 -> 0); high modulation index (max modulator level 99 >= 70); velocity dynamics present | `corpus/u02-candidates/renders/c32a0cd1a1c6-TOUCHSYNTH/` |  |  |  |  |  |
| 16 | plucks | CLAV 7 | `280c2bebe099` | percussive attack (all live carriers rate1 >= 70); steep decay (loudest carrier 99 -> 0); high modulation index (max modulator level 95 >= 70); velocity dynamics present | `corpus/u02-candidates/renders/280c2bebe099-CLAV-7/` |  |  |  |  |  |
| 17 | plucks | PIANO C3 | `979cbade04e0` | percussive attack (all live carriers rate1 >= 70); steep decay (loudest carrier 99 -> 0); high modulation index (max modulator level 86 >= 70); velocity dynamics present | `corpus/u02-candidates/renders/979cbade04e0-PIANO-C3/` |  |  |  |  |  |
| 18 | pads/strings | OUTERLIMIT | `ac157b810277` | slow attack (max carrier rate1 35); sustained carriers (min level4 99 >= 55); wide voicing (3 live carriers); vibrato wired (LFO pitch-mod depth 99 >= 15) | `corpus/u02-candidates/renders/ac157b810277-OUTERLIMIT/` |  |  |  |  |  |
| 19 | pads/strings | STORM    2 | `0cf036d5c5b7` | slow attack (max carrier rate1 22); sustained carriers (min level4 99 >= 55); detuned carriers (width/beating); vibrato wired (LFO pitch-mod depth 99 >= 15) | `corpus/u02-candidates/renders/0cf036d5c5b7-STORM-2/` |  |  |  |  |  |
| 20 | pads/strings | WhosTardis | `768debf14d59` | slow attack (max carrier rate1 26); sustained carriers (min level4 99 >= 55); wide voicing (3 live carriers); vibrato wired (LFO pitch-mod depth 99 >= 15) | `corpus/u02-candidates/renders/768debf14d59-WhosTardis/` |  |  |  |  |  |
| 21 | percussion/textures | MOOG PIANO | `d6f570e72e7e` | fixed-frequency operators (2); extreme feedback (7); high modulation index (max modulator level 99 >= 85); fast strike (all live carriers rate1 >= 60) | `corpus/u02-candidates/renders/d6f570e72e7e-MOOG-PIANO/` |  |  |  |  |  |
| 22 | percussion/textures | SUNSCREAM | `00b9712f8ac7` | fixed-frequency operators (2); extreme feedback (7); high modulation index (max modulator level 99 >= 85); fast strike (all live carriers rate1 >= 60) | `corpus/u02-candidates/renders/00b9712f8ac7-SUNSCREAM/` |  |  |  |  |  |
| 23 | percussion/textures | SUNSCREAM | `d314d74e19b1` | fixed-frequency operators (2); extreme feedback (7); high modulation index (max modulator level 99 >= 85); fast strike (all live carriers rate1 >= 60) | `corpus/u02-candidates/renders/d314d74e19b1-SUNSCREAM/` |  |  |  |  |  |

## Coverage

| family | selected | strict pool | relaxed |
|--------|----------|-------------|---------|
| EPs/tines | 3 | 1736 | no |
| basses | 3 | 1103 | no |
| bells/mallets | 3 | 5601 | no |
| organs | 3 | 6 | no |
| brass/reeds | 3 | 13 | no |
| plucks | 3 | 8426 | no |
| pads/strings | 3 | 45 | no |
| percussion/textures | 3 | 10366 | no |

## Provenance

- Selection tool: `tools/u02_candidates.py` (sha256 `ba7b0aca2303d5d0…`), deterministic, no randomness.
- Catalog: `corpus/archive-catalogs/alltheweb-catalog.json` schema gf180-dx7.archive-catalog/1, snapshot sha256 `9bfe25ca706f15f6…`; universe 161925 clean instances / 15209 distinct hashes, 15176 eligible after exclusions.
- Archive: `DX7_AllTheWeb.zip` sha256 `c2c47bd1aa29ab07…` — voice bytes are never redistributed; renders are derived audio of archived patches (catalog rights statement applies).
- Phrase set: `spec/audition-phrases-v1.json` (U01, fixed; the tool refuses improvised phrase sets).
- Oracle: pinned in `reference/oracle-identity.json`; every render re-verifies the binary hash. External GPL oracle only — never copied into this repository.
- **Dry-render reminder:** renders are dry; peaks recorded, gain untouched. Compare candidates without normalization tricks.
