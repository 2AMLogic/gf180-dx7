# Oracle protocol — pinned Dexed Mark I reference (R01/R02)

Status: the **interface** is defined here. The executable wrapper that
implements it is **defined by R02**. Until R02 lands, no render exists and no
"matches Dexed" claim is made.

**Licensing boundary (normative).** The oracle executable is built from Dexed
sources pinned in `reference/manifest.json`; `Source/EngineMkI.cpp` and
`Source/msfa/tuning.{cc,h}` are GPL-3.0-or-later (or carry no in-file header
and inherit the GPL-3.0-or-later Dexed project license). **Oracle code is GPL
and lives OUTSIDE this repository.** This repository stores only this protocol
document, the pinned hashes, and render evidence. GPL-3.0-or-later files are
never copied into this Apache-2.0 repository (docs/REUSE-RULINGS.md:
`dexed-enginemki`; AGENTS.md licensing/source policy).

## 1. What the oracle is

A headless command-line renderer linked against the pinned Dexed engine
sources, configured to behave like `DexedAudioProcessor` at the pinned
settings with:

- engine type `DEXED_ENGINE_MARKI` (value 1, `Source/PluginProcessor.h`),
- sample rate 48000 Hz,
- controller and fx defaults as pinned in `reference/manifest.json`
  (`settings` array; each entry names its source path),
- standard tuning (A4 = 440), no MTS master,
- no UI, no host, no sysex echo, no normalization: **dry output only**.

The wrapper must replicate the `DexedAudioProcessor::processBlock` render path
(Source/PluginProcessor.cpp): per-note `Dx7Note::compute` into an int32
buffer, `>>4`, clip to ±(1<<24), `>>9`, `/0x8000` float conversion clamped to
[-1, 1], voice summation, block/event scheduling via `getNextEvent` semantics,
`MAX_ACTIVE_NOTES = 16` voice handling, and the LFO shared per timbre. The
exact wrapper implementation is owned by R02; this document fixes only the
I/O contract and the config echo.

## 2. Inputs

The oracle is invoked with a voice, an event list, and an output prefix:

```
oracle --voice <voice-input> --events <events.json> --out <prefix> [flags owned by R02]
```

### 2.1 Voice input (`--voice`)

Exactly one of:

- **`.syx` file** — a DX7 single-voice SysEx dump as accepted by this
  repository's P01 codec (`src/gf180_dx7/sysex.py`). The wrapper unpacks it to
  Dexed's internal 161-byte voice state (`uint8_t data[161]`,
  Source/PluginProcessor.h). The `.syx` → `data[161]` mapping (which is Dexed
  cart-load code, GPL) is **wrapper-defined in R02**.
- **`voice.bin`** — the raw 161-byte Dexed internal voice state `data[161]`
  (`uint8_t data[161]`, Source/PluginProcessor.h) for cases where the mapping
  is done out of band. Verified layout: `data[0..154]` are the 155 packed DX7
  single-voice bytes exactly as a `.syx` body is copied by
  `memcpy(data, rawdata, 155)` (Source/PluginData.cpp
  `updateProgramFromSysex`); `data[155]` is the packed operator-switch byte
  (Source/PluginParam.cpp `setDxValue`); `data[156..160]` are Dexed-internal
  state.

The voice file is hashed (SHA-256) and echoed in the output metadata.

### 2.2 Event list (`--events`)

JSON, UTF-8:

```json
{
  "events": [
    { "type": "note_on",  "note": 60, "velocity": 100, "delay_samples": 0 },
    { "type": "note_off", "note": 60,                 "delay_samples": 48000 }
  ],
  "render_samples": 96000
}
```

- `delay_samples` is the **absolute sample offset from the first output
  sample** at which the event is applied; events must be listed in
  non-decreasing `delay_samples` order.
- `type` is `note_on` (requires `note`, `velocity` in 1..127) or `note_off`
  (requires `note`).
- `render_samples` is the total number of output samples; renderings always
  run to exactly this length (release tails included by choosing a long
  enough value).
- Controller events, if any are defined, are **wrapper-defined in R02** and
  must be echoed in the metadata.

The events file is hashed (SHA-256) and echoed in the output metadata.

## 3. Outputs

- **`<prefix>.pcm`** — raw headerless PCM, mono, **f32le**, 48000 Hz, exactly
  `render_samples` frames. Mono because Dexed renders one mono channel and
  copies it (Source/PluginProcessor.cpp end of `processBlock`).
- **`<prefix>.json`** — metadata that **echoes the exact configuration**:

```json
{
  "oracle_protocol": "reference/oracle-protocol.md",
  "oracle_wrapper_version": "<R02-defined>",
  "upstream": { "repo_url": "...", "commit": "<40-hex>" },
  "engine_type": "marki",
  "engine_type_enum": { "name": "DEXED_ENGINE_MARKI", "value": 1 },
  "sample_rate": 48000,
  "render_samples": 96000,
  "format": { "codec": "pcm_f32le", "channels": 1 },
  "voice": { "kind": "syx|voice.bin", "sha256": "<64-hex>" },
  "events": { "sha256": "<64-hex>", "count": 2 },
  "pinned_settings": { "<echo of every settings entry>": "..." },
  "toolchain": { "<echo of manifest toolchain used to build the wrapper>": "..." }
}
```

The metadata must be sufficient to re-run the render bit-identically without
consulting anything else.

## 4. Verification contract

- The oracle binary is built only from a source tree that passes
  `python3 tools/verify_reference.py --dexed-root <root>` (exit 0).
- Renders are compared **dry**: no per-render normalization, no time warping,
  no engine substitution (plan section 7).
- A render without its `.json` config echo, or with an echo that disagrees
  with `reference/manifest.json`, is `NO_VERDICT` evidence, not a pass.
- Dexed agreement established this way is software-reference agreement only;
  it is **not** original-DX7 fidelity and never implies the instrument sounds
  good (plan section 7; DEC-001).
