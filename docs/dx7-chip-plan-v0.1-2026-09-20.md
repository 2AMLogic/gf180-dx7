# DX7 chip: product contract, references, and executable backlog

Version: 0.1  
Prepared: 2026-09-20T04:56:09Z  
Status: proposed architecture and issue briefs; research completed, no new synthesizer implementation or measurements performed.

## Recommendation

Create a sibling project, provisionally `gf180-dx7`. Use **TorchSynth's method of defining and qualifying an executable reference**, **Parasynth's experience building a continuous instrument and checking its external pins**, and **DX7-specific operator, envelope, and routing semantics**. Neither sibling's DSP is a suitable starting implementation.

The proposed first product is a 16-note, six-operator, 32-algorithm, real-time DX7-compatible sound engine with a carefully selected preset bank, import of ordinary DX7 voice banks, an external host controller, and digital audio output. Put the full preset archive in host-accessible flash or SD storage. Keep active parameters and synthesis state in the core.

Use a pinned **Dexed Mark I** configuration as the initial executable comparison target. Use Yamaha documentation, Ken Shirriff's chip analysis, MSFA research, and VDX7 to understand and challenge particular behaviors. Do not call the result a bit-perfect reproduction of an original DX7 unless a separate hardware-fidelity claim has actually been qualified.

The fastest useful milestone is: **load a real patch, play and release a live note, capture reproducible reference output, and listen**. Build the larger backlog around that working path.

## 1. What the supplied links establish

The linked [video](https://www.youtube.com/watch?v=fhfhKMtBRog) is indexed as a review of the M-Vave FM-1, titled “What's the Catch With a $65 Synth This Good? (FM-1)”. I could retrieve its indexed title/description, but not play the video or obtain a transcript. The [manufacturer's indexed product page](https://www.cuvave.com/product?id=fm-1) describes a six-operator, 32-algorithm pocket FM instrument; direct page retrieval timed out.

The [AliExpress listing](https://www.aliexpress.us/item/3256812599140430.html) was inaccessible. I have not verified that listing's exact variant, firmware, price, internals, or included sounds. Nothing here assumes that the FM-1 uses Dexed internally.

The FM-1 is useful as a product comparison: compact, playable, inexpensive access to FM sounds. It is not a numeric reference for reproducing a DX7. The reason to build this chip would be control of the implementation, a reusable verified core, and the silicon project itself; this research does not establish a cost advantage over a mass-produced module.

## 2. What to borrow from each existing repository

Repository snapshots inspected:

| Repository | Default-branch commit retrieved | Relevant current behavior |
|---|---|---|
| gf180-parasynth | `cbcc8b9e10e49c84f630550e2e145cc6da8a659c` | Live paraphonic subtractive voice plus drums, SPI control and I2S; model/RTL comparisons and physical-flow experience |
| gf180-torchsynth | `36bdc67faa1ff2509988b772efcca7a7eb4e7115` | Pinned TorchSynth Voice target; four-second clip renderer, explicit identities, traces, numeric contracts, capability graph and leaf issues |

These are repository-reported capabilities, not builds rerun during this review.

| Question | Recommendation | Reason |
|---|---|---|
| How to define the sound target? | TorchSynth approach | Pin an executable implementation and all settings; turn ambiguities into explicit decisions. |
| How should notes behave? | A new continuous, polyphonic profile | Parasynth has continuous instrument experience, but its shared paraphonic filter/envelope behavior is not a DX7 voice model. |
| How to choose arithmetic? | DX7-specific integer model, qualified against the chosen reference | Existing DX7 engines already contain useful integer behavior. A generic floating-point FM rewrite need not be an obligatory first stage. |
| What physical infrastructure might transfer? | Audit narrow Parasynth seams | I2S, SPI, clocking, pin-level verification and flow requests may be useful after adapting widths and timing. |
| What backlog pattern transfers? | TorchSynth leaf tasks and explicit dependencies | Each coding task can name one result, an oracle, and an exit condition. |
| What should not transfer? | Either sibling's voice DSP; TorchSynth's clip normalization | A live note does not know its future release time or future peak. DX7 envelopes are not ordinary ADSRs. |

TorchSynth explicitly documents why its current profile is noncausal: note duration is known in advance and final normalization can depend on a later sample. That is appropriate for its own target, but inappropriate for this instrument. See its [README](https://github.com/2AMLogic/gf180-torchsynth/blob/36bdc67faa1ff2509988b772efcca7a7eb4e7115/README.md).

Two especially valuable lessons are already written down in [TorchSynth's bring-up design](https://github.com/2AMLogic/gf180-torchsynth/blob/36bdc67faa1ff2509988b772efcca7a7eb4e7115/docs/BRINGUP-AND-EVIDENCE.md) and its [reuse audit](https://github.com/2AMLogic/gf180-torchsynth/blob/36bdc67faa1ff2509988b772efcca7a7eb4e7115/docs/REUSE-AUDIT.md): an issue's closure does not demonstrate a capability, and infrastructure from a sibling does not arrive prequalified for a new design.

Keep that discipline small. A working renderer, case manifest, comparator, and honest status file are enough to begin; a generalized verification platform is not a prerequisite to hearing a note.

## 3. Proposed product contract

These are recommendations for the new project's first contract, not established user decisions or measured capabilities.

| Item | Proposed v1 target |
|---|---|
| Synthesis | Six sine operators per note; all 32 DX7 algorithms; documented feedback behavior including algorithms 4 and 6 |
| Polyphony | 16 simultaneous notes, one active timbre; one-note implementations are development milestones |
| Presets | Initial 32-patch listening bank, expanding to a curated 128-patch shipping bank; larger archive import/search on host |
| Voice data | DX7 single-voice and 32-voice bank import/export with explicit support boundaries |
| Performance | Note on/off, velocity, sustain, pitch bend, modulation wheel, channel aftertouch, volume and all-sound-off; remaining original function modes tracked explicitly |
| Sound semantics | Operator rate/level envelopes, output level, velocity sensitivity, keyboard level/rate scaling, fixed/ratio frequency, coarse/fine/detune, transpose, oscillator sync, pitch envelope, LFO waveform/rate/delay/sync and sensitivities |
| Audio | Continuous dry mono synthesis, carried over an explicitly specified I2S format to an external DAC; optional duplication to both stereo slots |
| Working sample-rate default | 48 kHz, with the selected reference configured identically |
| Synthesis host boundary | Host performs file/MIDI/UI work and patch-to-core conversion; chip advances audio-rate phase, envelopes, routing/feedback and mixing autonomously |
| State | Explicit reset, phase/LFO initialization and event timing; repeatability tested rather than assumed |
| Effects | External or later; reference comparisons are dry |
| Physical target | gf180mcu feasibility, then the actual chosen shuttle/padframe/clock/memory constraints; no slot-fit assertion yet |

“Voice” is ambiguous in synthesizer discussions. In this plan, **patch** means a stored timbre and **note voice** means one independently sounding note. Supporting 22,000 patches does not require 22,000 concurrent voice engines.

### Fidelity profile

The first named profile should be something like `dx7-compatible-v1`: a pinned Dexed Mark I comparison target, frozen performance settings, known numeric deviations, and a separate fixed model for exact RTL verification.

An original-hardware profile is a different contract. It would additionally need original clock/sample timing (roughly 49.096 kHz), original envelope update behavior, arithmetic quirks, DAC companding/output filtering, and suitable hardware evidence. Simply changing the output sample rate or reducing PCM to 12 bits does not establish that claim.

Dexed's Mark I option is itself an approximation aimed at the original instrument. VDX7 also documents assumptions in its EGS and feedback behavior. Choosing one as the executable target makes development reproducible; it does not make that implementation the final authority on Yamaha hardware.

### Event and controller details that must be frozen

- Define when an event takes effect in sample/frame time, including events arriving during a frame and queue overflow.
- Define retrigger, repeated same-pitch notes, sustain release, voice stealing and patch change while notes are held.
- Keep patch data separate from performance/function settings. A voice `.syx` alone does not necessarily specify a complete performance.
- Preserve the patch's own transpose, scaling and velocity settings. Do not accidentally apply transpose twice or globally clamp MIDI velocity to the old keyboard's commonly observed output range.
- Define whether oscillator/LFO state is free-running or restarted. Comparing two different starting phases with a waveform subtraction can report a huge error without isolating a synthesis defect.
- Avoid arbitrary four-second note tails. Slow/delayed envelopes and nonzero final envelope levels need explicit treatment. A useful patch is not invalid just because it is quiet at one pitch or does not reach silence on a short timeout.

## 4. Recommended architecture and feasibility questions

| Boundary | Responsibilities |
|---|---|
| Laptop or embedded host | USB/DIN MIDI handling, SysEx validation, library import, favorites/categories, patch conversion, controller/function policy and optional effects |
| External flash or SD | Preset archive, names/metadata, user edits; patch data is read when selecting a sound |
| DX7 core | Active parameters, scheduled events, note/operator state, phase and envelope advancement, routing, feedback and voice mix |
| Audio interface | Known framing, clock relationship, buffering and external DAC |

Start with a host API accepting a resolved patch, explicit note/controller events and a deterministic time base. An external controller should not have to stream all operator amplitudes every sample to sustain a note. Whether low-rate performance modulation runs on the chip or host must be an explicit part of the contract and its worst-case link budget.

### Shared datapath

Ken Shirriff's [die analysis](https://www.righto.com/2021/11/reverse-engineering-yamaha-dx7.html) shows why a DX7 is a plausible digital chip target: the original cycles a shared datapath through 16 × 6 operator states. Log-sine and exponential lookup tables turn amplitude scaling into addition in the log domain. Study that organization before synthesizing 96 independent multiplier-based oscillators.

The new implementation can explore a time-multiplexed datapath with compact tables and a state store. It need not copy the original shift-register storage technology. It also should not copy a software emulator's expanded tables without measuring a more compact hardware representation.

At the proposed 48 kHz rate:

`16 notes × 6 operators × 48,000 frames/s = 4,608,000 operator evaluations/s`.

For illustration, 24.576 MHz provides 512 core clocks per sample frame, or about 5.33 clocks per operator before accounting for envelope work, memory accesses, mixing, link handling and pipeline constraints. That is a schedule question, not proof of timing closure. At 12.288 MHz the allowance is only about 2.67 clocks per operator.

Synthesize a representative operator **and the complete projected state/table storage** early. In a mature-node standard-cell flow, state mapped to flip-flops can dominate. FPGA block RAM/DSP availability does not prove the same resource is available in the chosen ASIC flow. Do not assume an SRAM or ROM macro is usable without the actual macro views, access timing and integration route.

### Preset capacity

A standard packed DX7 patch occupies 128 bytes within a 32-patch bank. A bank's 4,096 data bytes are wrapped by its SysEx framing. The [Dexed source documentation](https://github.com/asb2m10/dexed/blob/2e182b3db85c09083ab13c8b9b00565ce7d9ff85/README.md) discusses the 4,096/4,104-byte bank forms.

| Stored patches | Packed parameter bytes | Approximate binary size |
|---:|---:|---:|
| 32 | 4,096 | 4 KiB |
| 128 | 16,384 | 16 KiB |
| 22,000 | 2,816,000 | 2.69 MiB |

These are calculations for packed voice data only, excluding metadata, indexes, additional performance data and filesystem overhead. For 22,000 patches, the raw parameter bits total 22.528 Mbit. This is modest external storage but not something to casually synthesize into ordinary on-chip registers.

Recommended default: the instrument comes with its curated sounds in host/board storage and loads an active patch into the core. If literal single-die self-containment is essential, compare a tiny bootstrap bank and a real available ROM implementation as a separate physical experiment. Adding thousands of sounds should normally be a data update rather than a new chip revision.

## 5. Reference stack, ordered by usefulness

There is no single source that simultaneously defines usability, original-hardware accuracy, convenient software behavior and ASIC implementation.

| Reference | Use | Limit |
|---|---|---|
| [Yamaha DX7 operation manual](https://homepages.abdn.ac.uk/d.j.benson/pages/dx7/manuals/dx7-man.pdf), [MIDI implementation](https://homepages.abdn.ac.uk/d.j.benson/pages/dx7/manuals/dx7-midi.txt), and service documents linked from [Benson's index](https://homepages.abdn.ac.uk/d.j.benson/pages/html/dx7.html) | Intended controls, parameter meaning, algorithms, external behavior and original system structure | Not a complete bit-level implementation of the chips |
| [Dexed](https://github.com/asb2m10/dexed/tree/2e182b3db85c09083ab13c8b9b00565ce7d9ff85), explicitly its Mark I engine | Practical audible comparison, preset editing, interoperability, extractable numeric/trace reference | Different engines/settings produce different answers; Mark I is not a certification of original-hardware identity |
| [MSFA hardware notes](https://github.com/google/music-synthesizer-for-android/blob/f67d41d313b7dc85f6fb99e79e515cc9d208cfff/wiki/Dx7Hardware.wiki) and [envelope research](https://github.com/google/music-synthesizer-for-android/blob/f67d41d313b7dc85f6fb99e79e515cc9d208cfff/wiki/Dx7Envelope.wiki) | Understand measured mappings, envelope timing, gain and pitch representations; small code to inspect | Older hypotheses should be checked against later chip research; do not promote every conjecture to fact |
| [Ken Shirriff's OPS die analysis](https://www.righto.com/2021/11/reverse-engineering-yamaha-dx7.html) | Hardware datapath, table organization and the architectural basis for area-efficient arithmetic | Does not independently specify the complete EGS, firmware and analog system |
| [VDX7](https://github.com/chiaccona/VDX7/tree/c8540ac69a70d1db78378a805bc6c66fa80a748c), especially [implementation notes](https://github.com/chiaccona/VDX7/blob/c8540ac69a70d1db78378a805bc6c66fa80a748c/README.code.md) | Hardware-oriented cross-check; native-rate, CPU/EGS/OPS organization; locate differences from Dexed | Author documents unvalidated assumptions, including parts of envelope delay/rate scaling; software-expanded tables are not an ASIC layout recommendation |
| [ajxs's annotated firmware disassembly](https://github.com/ajxs/yamaha_dx7_rom_disassembly) and [technical analysis](https://ajxs.me/blog/Yamaha_DX7_Technical_Analysis.html) | Resolve firmware/controller/patch mapping and chip register questions | Does not imply that this project should emulate the whole original CPU or ship original firmware |
| [MiniDexed](https://github.com/probonopd/MiniDexed/tree/8e3fa301ddcd2c0e3b9c1023148df2302095b07b) and its Synth_Dexed dependency | Working host/MIDI/preset/embedded product patterns; quick listening prototype | A Dexed-family implementation is not an independent second oracle for the same DSP |
| [Hexter](https://github.com/theabolton/hexter) | Additional implementation lineage for investigating particular disagreements | Older DSSI interface; extra integration is justified only if it answers a concrete question |
| [Plogue chipsynth OPS7](https://www.plogue.com/products/chipsynth-ops7.html) | Optional external listening/behavior comparison for original-hardware fidelity | Closed source; vendor's digital-capture accuracy claims were not independently measured here |

### Source reuse is not the same as reference use

Dexed's project is GPLv3, but it preserves Apache-2.0 licensing for MSFA components. In the pinned source, `Source/EngineMkI.cpp` is specifically GPL-3.0-or-later while `Source/msfa/dx7note.cc` has an Apache-2.0 header. Do not call all of Dexed Apache-licensed because it contains MSFA. VDX7 declares GPL-3.0-or-later; Hexter declares GPL-2.0-or-later. Record provenance at the file/table level before adopting code into the new repository. These are observed source declarations, not a legal opinion about a particular redistribution.

An external comparison executable and code copied into a shipping design are different engineering decisions. Choose an explicit compatible source policy rather than accidentally inheriting one through a port.

### Existing FPGA work

[timholzhey/dx7-synthesizer](https://github.com/timholzhey/dx7-synthesizer) identifies itself as a software reference and LUT generator for a separate university-hosted FPGA project. Its GitHub repository is not itself the complete RTL core. The external RTL was not retrieved in this review.

[deadfatty/tangnano-dx7](https://github.com/deadfatty/tangnano-dx7/tree/2db415319abeef8315a95d1941b08c4a13f186c1) documents a Tang Nano 20K implementation with six operators, 32 algorithms and 16 notes. Its README also explicitly lists missing pitch EG and LFO features, patch-dependent level differences and no declared code license. It is useful evidence about architecture and failure cases to investigate. It is not a qualified, reusable, complete DX7 implementation for this project.

## 6. Sound library strategy

The exact “22K pack” has not been identified from the supplied information. Do not make its marketing count a requirement. Name and hash the chosen archive before treating it as an input.

Useful starting points are [BlackWinny's Dexed_cart compilation](https://asb2m10.github.io/dexed/), [Dave Benson's banks](https://homepages.abdn.ac.uk/d.j.benson/pages/html/dx7.html), and [Bobby Blues' collection](https://bobbyblues.recup.ch/yamaha_dx7/dx7_patches.html). BlackWinny's [collection notes](https://github.com/visualizersdotnl/Yamaha-DX7-patch-library/blob/master/Version%20and%20Readme.txt) explain the extensive duplication and malformed conversions in old archives. Bobby Blues explicitly warns that its broad archive is not deduplicated. These are discovery corpora, not ready-made rankings of the best sounds or blanket evidence of redistribution rights.

Maintain three separate sets:

1. **Compatibility corpus:** directed patches and diverse archived sounds that expose engine bugs.
2. **Curated instrument bank:** sounds selected for usefulness, character, velocity response, keyboard range and variety.
3. **Browsable archive:** additional user-imported sounds, preserving provenance and aliases.

Suggested initial listening categories: electric pianos/tines, basses, bells/mallets, organs, brass/reeds, plucked sounds, pads/strings, and unusual percussion/textures. Use recognizable factory timbres as comparison anchors where available; this memo does not claim to have auditioned or selected the final sounds.

For every candidate, preserve original bytes and source/bank/slot. Maintain a separate canonical sound hash over decoded sound parameters, excluding the display name and container framing. Keep aliases when two names describe the same parameter set. Use audio similarity to suggest near-duplicates for review, not to delete them automatically: phase, velocity, key range and controller behavior can distinguish apparently identical sounds.

A useful audition protocol includes low/mid/high notes, several velocities, short plucks, long holds, release, chords and applicable controllers. A quiet sound at middle C or at one velocity is not automatically broken. Dry comparisons expose synthesis; optional effects can then be evaluated as product taste.

Start with 32 favorites and expand to 128. The archive can contain tens of thousands without requiring the front panel to present an undifferentiated list. Favorites, categories and fast repeatable audition provide more value than a larger preset count.

## 7. Verification contract

Keep the following claims separate:

| Claim | Required evidence |
|---|---|
| Imports DX7 data | Valid/invalid format fixtures, correct decoded fields, round trips and declared unsupported forms |
| Implements the chosen software profile | Pinned reference inputs/settings and comparisons at named internal seams |
| RTL implements the fixed model | Exact integer sample/state comparisons after explicitly defined scheduling/latency |
| Behaves well as an instrument | Live-note and controller cases; selection/recall behavior; no unintended stuck notes, wraps or underruns |
| Sounds like original hardware | Controlled recordings/digital captures and a specific comparison procedure, distinct from software agreement |
| Is musically useful | A reviewed preset set and listening records; never inferred from passing numeric tests |
| Fits FPGA or ASIC | Resource/timing/physical reports for that exact implementation and constraints |

Trace at least decoded parameters, base/operator pitch increments, envelope stage/level, phase, operator output, routing/feedback state, voice mix and final PCM. Localize the first divergence before interpreting a final waveform score.

Reference vs reference repeatability can be exact under a defined environment. RTL vs frozen integer model must be exact. Fixed model vs the selected software reference can use justified error budgets when arithmetic differs. Hardware audio captures require different alignment/calibration. Do not normalize each render independently, arbitrarily time-warp it, or choose a favorable comparison engine per patch.

Useful directed cases include all algorithms, especially 4/6 feedback; operator permutation; ratio/fixed frequency; coarse=0; detune extremes; each scaling curve; velocity sensitivity; rising as well as falling envelope segments; slow rates; delayed attacks; nonzero final level; pitch EG; every LFO waveform/sync mode; repeated notes; sustain; stealing; all-sound-off; and maximum voice summation.

Use closed-form tests where their assumptions apply: a single sine, controlled gain changes, known phase increments and simple low-index PM sidebands. Do not pretend ideal sinusoidal theory certifies every quantized feedback network.

Maintain live negative controls: wrong operator order, broken feedback history, ordinary ADSR substituted for a DX envelope, fixed mode accidentally tracking the keyboard, inverted scaling, signed overflow, and a silent/stale-output stub. A control must demonstrably fail the relevant check.

Statuses should distinguish `PASS`, `FAIL`, `NOT_RUN`, `BLOCKED`, `NO_VERDICT`, and `STALE`. Report coverage separately from agreement. Keep a lightweight issue DAG for scheduling and evidence records keyed to source/input hashes; generate presentation views from those records instead of maintaining parallel hand-written claims.

Do not require an elaborate blind-holdout apparatus before the first renderer. Once choosing approximations, freeze development patches, independent challenge patches and acceptance thresholds; do not silently tune thresholds after inspecting challenge failures.

## 8. Executable backlog

IDs below are local planning identifiers, not existing GitHub issue numbers. All tasks are proposed/unrun. Dependencies refer to these IDs and must be translated to actual issue references if filed.

There are three epics: **E1 Reference and playable software**, **E2 Verified core and physical feasibility**, and **E3 Curated instrument**. Epics are trackers, never builder-sized work. Add only the next actionable leaves to a coding queue; keep later implementation details provisional until their dependencies establish the facts.

### E1 — Reference and playable software

| ID | One deliverable | Depends on | Acceptance and evidence |
|---|---|---|---|
| D00 | Product/profile decision record | — | Defines the table in section 3, claim boundaries, note/controller policy, baseline engine and non-goals. Unresolved choices have proposed defaults and a named decision owner. |
| D01 | Physical constraint inventory | — | Names candidate board, core clock, target shuttle/padframe and usable memory options, separating confirmed constraints from assumptions. Missing access is reported without inventing feasibility. |
| R01 | Pinned reference manifest | D00 | Records Dexed commit, Mark I selection, relevant source/table identities and licenses, runtime/toolchain, sample rate, block/event scheduling, reset, tuning, gain and performance settings. Rejects source drift. |
| R02 | Minimal headless reference renderer | R01 | Given one named fixture patch and event sequence, writes dry PCM and metadata; repeated fresh-process runs agree; event timing is recorded. First audible end-to-end path. |
| P01 | SysEx codec | D00 | Handles supported single/bank forms, checks framing/length/checksum/field ranges, preserves raw inputs, validates independent fixtures and valid-data round trips; malformed/unsupported data is classified. |
| P02 | Archive cataloger | P01 | Emits provenance, decoded parameters, stable IDs and duplicate aliases for a pinned input archive. Reports total/valid/unsupported/duplicate counts without silently deleting sources. |
| R03 | First 32-patch development manifest | P02, R02 | Includes musically recognizable cases and explicit feature coverage; each has events, render settings and source identity. Missing archive identity remains visible. |
| R04 | Comparator and apparatus qualification | R02 | Exact sample comparison plus property-specific diagnostics, validated with analytic fixtures and known-bad outputs; absent/silent/invalid evidence cannot become a pass. |
| R05 | Non-invasive reference trace adapter | R02 | Named phase, envelope, operator and feedback taps; instrumented final output matches the uninstrumented renderer. Any unavailable tap is explicitly documented. |
| R06 | Complete directed compatibility registry | P01, R05 | Maps every supported voice/performance field and all 32 algorithms to cases; includes boundary and event interactions, with a short fast subset. Coverage holes are machine-readable. |
| R07 | Targeted oracle-disagreement report | R03, R04 | Compare only a small diagnostic set with VDX7 or Hexter; explain actionable discrepancies without averaging engines or asserting which is physically correct without evidence. Nonblocking for the declared software profile. |

### E2 — Verified core and physical feasibility

| ID | One deliverable | Depends on | Acceptance and evidence |
|---|---|---|---|
| N01 | Numeric/scheduling decision record | R04, R05, D01 | Defines widths, rounding, saturation/wrap, table representation, envelope/control update timing, phase/sync and output scaling. Compares plausible arithmetic choices with error and storage/cycle estimates. |
| N02 | Integer phase/operator model | N01 | Single-operator phase/log-sine/level/exp/sign path matches its contract; complete table domains checked where tractable; valid analytic and reference probes pass. |
| N03 | Integer operator-envelope model | N01 | All rate/level stages, key transitions, output/velocity/rate/level scaling and delayed/slow cases tested against reference traces under declared tolerances. |
| N04 | Integer algorithm/feedback model | N02 | All 32 graphs, carrier sums and feedback histories validated using scripted operator sources plus reference examples; feedback 4/6 and operator-index mutations fail. |
| N05 | Integer pitch and modulation model | N01 | Fixed/ratio/coarse/fine/detune/transpose, pitch EG and LFO controls have named reference traces and field coverage; no unsupported parameter silently falls back. |
| H01 | Synthesizable operator probe | N02 | Representative operator is RTL-exact to the integer model; synthesis keeps observable useful outputs; table/logic area and delay reported with source/tool/PDK identity. |
| H02 | Full-state storage feasibility probe | N01, D01 | Projects all 96 operator states, active patch data, feedback and event buffers using actual available storage mappings. Reports ports/cycles, cell or macro area and uncertainties. |
| N06 | Integrated single-note fixed model | N03, N04, N05 | Patch plus live events produces audio; development cases and traces compared to reference; all deviations reported per property. |
| N07 | Polyphonic state/event manager | N06 | 16 independent note states, repeated-note/retrigger/sustain/stealing policies, held-note patch changes and overload tested; no accidental cross-voice state sharing. |
| N08 | Frozen fixed-model release and vectors | N07, R06 | Reproducible fixture bundle, arithmetic profile and derived error budgets; challenge run preserved; live mutations detected. Authorizes exact RTL work. |
| H03 | Core interface/schedule contract | N08, H01, H02 | Defines patch load/commit, events, backpressure/status, reset/latency, state access and trace points. Proves a bounded per-frame schedule including worst-case work, or records the blocker. |
| H04 | Envelope/state RTL | H03 | Exact to model for directed envelope and state transitions; verifies memory hazards and worst-rate updates. |
| H05 | Routing/feedback RTL | H03 | Exact for every algorithm and feedback setting, including schedule delays and accumulation signs; targeted mutations fail. |
| H06 | Pitch/modulation RTL or host binding | H03 | Implements precisely the chosen host/core split, with bounded update/event timing and exact boundary values; no per-sample host dependency introduced implicitly. |
| H07 | Integrated polyphonic PCM core | H04, H05, H06, H01 | Emits the full 16-note stream bit-for-bit against the fixed model at defined latency; reset, burst events, held notes and maximum summation pass. |
| H08 | SPI/I2S pin-level integration | H07 | Exercises actual serialized inputs and decodes actual output pins; checks incomplete/invalid writes, frame ordering, underrun/status and patch atomicity. |
| H09 | FPGA digital capture | H08, D01 | Board runs the same event vectors; captured digital PCM agrees after declared latency; clock, resources, firmware and bitstream recorded. |
| H10 | gf180 mapped feasibility report | H07, D01 | Real mapped timing/cell/storage results and the remaining die/pad/margin budget; 16-note requirement retained unless explicitly revised. |
| H11 | Physical integration evidence | H10 | Actual target wrapper, pads, power and clocks; route/timing/DRC/LVS/other required checks reported individually as run/passed/failed/unrun. No claim of silicon from a routed core. |

H01/H02 intentionally occur before full-core RTL. They can refute an unaffordable schedule or storage architecture while changing it is still cheap. H09 and H10 are separate claims; one does not prove the other.

### E3 — Curated instrument

| ID | One deliverable | Depends on | Acceptance and evidence |
|---|---|---|---|
| U01 | Software audition/recall tool | R02, P02 | Select, play a repeatable phrase, favorite and recall by stable sound identity; compare dry renders without changing gain per candidate. |
| U02 | Reviewed 32-patch listening bank | U01, R03 | Candidate list across sound families, structured listening records and provenance; distinguishes personal preference from technical failures. |
| U03 | Curated 128-patch bank manifest | U02 | Expands variety without redundant filler; stores authorship/source, audition notes, categories, performance settings and distribution status. It can be revised independently of RTL. |
| U04 | Embedded host against a mock core | P01, H03 | MIDI/library-to-core transaction path passes event timing, error handling, reset/recall and partial-transfer fixtures without requiring a board. |
| U05 | Complete instrument demonstration | U04, H09, U02 | Real keyboard → host → FPGA → DAC playback with verified patch recall and sustained playing; measured latency, missed-frame/stuck-note stress, reproducible recording and version identities. |

A mock host and a software audition tool can advance before silicon. Hardware availability blocks hardware evidence, not all useful development.

### First queue and decision order

The initial queue is D00 and D01, then R01. After R01, do R02 while building P01. The first reference audio should exist before expanding the project into broad frameworks. P02/R03 then establish real sounds; R04/R05 make them debuggable. N01 and the operator/storage probes reduce the largest chip risk early. Human listening can start at U01/U02 while the numeric work proceeds.

If a builder completes a task, it selects the next leaf whose prerequisites are truly satisfied. If the premise fails, it records a bounded finding and blocks only the affected dependency; it does not quietly weaken the acceptance rule.

## 9. Three fully specified starter briefs

### R02 — Render one real patch with pinned Dexed Mark I

**Claim advanced:** a reproducible executable sound target exists.

**Premise check:** R01 exists; the selected source hashes, engine mode and runtime match; a valid fixture patch is available with its source recorded. If a premise is false, stop this leaf and identify the missing input.

**Deliverable:** one command-line renderer and one smoke fixture. Input contains patch identity and timestamped note/controller events. Output contains raw dry PCM (and a WAV convenience copy), event metadata, source/config hashes, sample count and output hash. The wrapper must specify event-to-block behavior rather than assuming arbitrary DAW scheduling is irrelevant.

**Acceptance:** render the same input in two fresh processes; compare bytes. Render note-on, note-off and a delayed onset. Confirm a silent-output substitution is rejected. Check that enabling trace instrumentation later can be compared against this uninstrumented baseline. Listen to the output and record that listening happened, without treating it as proof of hardware fidelity.

**Non-goals:** GUI, full archive import, independent FM engine, RTL, ranking all patches, original-DX7 bit accuracy.

**Evidence:** exact command, compiler/runtime/config, input and output hashes, comparison result and limitation statement.

### P01 — Lossless supported SysEx import/export

**Claim advanced:** real DX7 voice data reaches the engine without field corruption.

**Premise check:** D00 identifies supported voice/bank forms and how unsupported extensions are handled. Do not guess DX7II performance compatibility from a filename.

**Deliverable:** codec with a canonical decoded patch representation; preserve original raw bytes separately. Validate message framing, length, checksum and field encodings. Clearly distinguish a single-voice message, a packed bank, a stream of messages, and a raw bank accepted through an explicitly selected import path.

**Acceptance:** independent valid fixtures decode expected fields; encode/decode preserves supported semantic fields; checksum/length/truncation/unsupported-format fixtures give specific errors; multiple messages do not merge accidentally. Original-name and reserved-bit policies are documented. A checksum-only assertion is insufficient to prove field mapping.

**Non-goals:** sound ranking, silent automatic repair, translating every Yamaha family, MIDI transport electronics.

**Evidence:** fixtures/provenance, decoded expected values, valid round trips and negative cases.

### H02 — Measure the projected state memory cost

**Claim advanced:** the storage organization has a plausible path to the physical target.

**Premise check:** D01 identifies the usable cell/memory libraries, and N01 identifies the state widths and required access schedule. If either is absent, return the exact missing assumption; do not substitute ideal FPGA RAM.

**Deliverable:** synthesizable storage probe and a reproducible report for all projected note/operator state, patch parameters, feedback state and queues. Keep each field observable so synthesis cannot remove unused state.

**Acceptance:** byte/bit accounting reconciles with the model; read/write ports and access conflicts meet the schedule or are identified; mapped cell/macro counts and timing are reported with tools, PDK and constraints. The report separates measured storage cost from estimated remaining DSP and pad/route overhead. A found area problem is a successful investigation result, not a license to change the 16-note product goal.

**Non-goals:** final top-level layout, unqualified SRAM generator integration, claiming complete-chip fit.

## 10. Issue template and completion rules

Suggested issue fields:

```text
Outcome: one independently verifiable result
Claim advanced: exact behavior/capability this establishes
Premise check: assertions checked before implementation
Inputs: pinned sources, contract version, fixture identities
Deliverable: specific files/interface/artifact
Acceptance: runnable check and precise pass condition
Negative control: targeted defect or invalid input the check must reject
Evidence: command, source/input/config hashes, raw result location
Non-goals: adjacent work excluded from this leaf
Depends on: concrete issue numbers after filing
Stop/escalate condition: failed premise or product decision required
```

Command names such as `reference-smoke`, `codec-check`, `operator-check`, `rtl-exact`, `pins-check`, `bank-audit` and `physical-report` can be standardized when their tools exist. They are proposed interface names, not commands available today.

Every PR should say what behavior changed, which acceptance case moved, how it was checked, and what remains unproved. Do not equate source presence, test count, generated report existence or a closed issue with sound fidelity or hardware readiness.

Run the cheap affected checks during iteration, broader relevant checks at integration, and expensive corpus/physical jobs at their defined gates. Repeat a costly run to resolve a specific remaining risk, not simply to produce another green count.

## 11. Highest-value product choices

These are the few choices likely to change substantial work, ranked by impact. The proposed defaults allow the research and software tasks to proceed without repeatedly asking the owner about routine implementation details.

| Rank | Choice | Proposed default | When a different answer matters |
|---:|---|---|---|
| 1 | Compatibility vs original-hardware reproduction | Musically convincing DX7 compatibility against a pinned Mark I software profile | Exact hardware emulation changes clocking, arithmetic, envelope scheduling and output-stage work. |
| 2 | Engine chip plus host vs literal one-die instrument | Engine chip, external controller and patch storage | Single-die storage/UI/MIDI/boot changes area and interfaces substantially. |
| 3 | Physical budget and required polyphony | 16 notes, one timbre; measure actual slot/clock/memory fit | Area pressure can force architecture changes; a reduction in polyphony is an explicit product change. |
| 4 | Which sounds are “good”? | Diverse 32 favorites first, 128 curated later | Personal preference changes the listening bank, but must not hide compatibility failures. |
| 5 | Vintage output color and effects | Dry core first; output coloration/effects separately | Changes presentation and possibly fidelity claims, without being necessary for initial patch correctness. |

Recommended immediate objective: **a pinned, audible, repeatable DX7 patch path plus a quantified operator/state-storage feasibility probe**. Those results establish both what the chip should sound like and whether the proposed physical architecture is worth pursuing.
