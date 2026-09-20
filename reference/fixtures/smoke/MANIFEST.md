# reference/fixtures/smoke — R02 render fixture (issue #7)

Synthetic fixture for the headless reference renderer. Both files are
**original bytes** generated in this repository: no third-party, real-world,
or downloaded DX7 voice data is committed anywhere in this fixture.

## Contents

| file | bytes | content |
|---|---|---|
| voice.syx | 163 | DX7 single-voice bulk dump `F0 43 00 00 01 1B` + 155 data + checksum + `F7`, encoded by the P01 codec (`gf180_dx7.sysex.encode_voice`) |
| events.txt | 358 | note-on at sample 2400 (delayed onset, past the first 64-sample block), velocity 100, pitch 60; note-off at 96000 (2 s) inside a 4 s (192000-sample @ 48 kHz) render |

## Construction (deterministic)

Generator: `tools/make_smoke_fixture.py` (committed). Regenerate with:

```sh
python3 tools/make_smoke_fixture.py
```

The voice is built through the P01 codec from a fixed parameter set —
algorithm 0 (panel algorithm 1: op 2 → op 1 and op 6 → op 5 → op 4 → op 3),
feedback 3, carrier operators 1 and 3 at output level 99 with modulators
2, 4, 5, 6 at 90, fast attack (rate_1 = 99) to peak 99, sustained level 90,
release (rate_4 = 42) to 0, no
pitch/amp modulation — chosen so the oracle render is guaranteed
non-silent, which the renderer's silent-output guard relies on. Generation
is byte-deterministic: same generator, same bytes.

voice.syx sha256 (generation and committed bytes are byte-identical;
hash corrected per judge review on PR #47):
`076ef44094de8bfe6933d52b04cc2b232504de8494c5edb629273cbe25d2864d`

## Provenance and licensing

- Field map: the P01 codec's own canonical DX7 VCED layout (see
  `src/gf180_dx7/sysex.py` module docstring); the codec validates framing and
  the DX7 checksum on decode.
- Parameter values are original synthetic choices made for this issue; they
  imitate no commercial preset and encode no third-party intellectual
  property.
- License of this fixture: the repository license (Apache-2.0), like the rest
  of the repo.

## Role in verification

`tools/render_reference.py` renders this fixture through the external GPL
oracle pinned in `reference/oracle-identity.json`. The render evidence lives
in `reference/evidence/smoke/`. Oracle agreement is software-reference
agreement only; it is not original-DX7 fidelity and says nothing about
musical quality (reference/oracle-protocol.md section 4).
