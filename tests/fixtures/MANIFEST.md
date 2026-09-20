# tests/fixtures — P01 SysEx codec fixtures (issue #8)

All files are **synthetic**: assembled from raw bytes by
`tools/make_test_fixtures.py` (commit generated them; regenerate by running
that script). The generator does **not** import `gf180_dx7.sysex`, so the
fixtures are an independent oracle for the codec's decode checks. No
third-party or real-world bytes are committed; real downloaded files are used
only as uncommitted spot checks (see below).

Field-map reference used for construction (provenance): canonical DX7 VCED
155-byte unpacked and VMEM 128-byte packed voice layouts per Dave Benson's
"Unofficial DX7 documentation" as implemented by DXconvert
(`DXconvert/dx7.py`, `vced2vmem`/`vmem2vced`) and Dexed `msfa` parameter
tables. Validated against real DX7 dumps (names, checksums).

| file | bytes | content |
|---|---|---|
| valid_voice.syx | 163 | single voice, `F0 43 00 00 01 1B` + 155 data + ck + `F7`; algorithm 9, feedback 5, LFO speed 42, name `TESTVOICE1`, six distinct operator sets |
| valid_bank.syx | 4104 | 32-voice bank, `F0 43 00 09 20 00` + 4096 data + ck + `F7`; voice *i* named `BANKVCE0ii`, algorithm *i*, feedback *i* mod 8 |
| bad_checksum_voice.syx | 163 | valid_voice data with checksum byte +1 |
| truncated_voice.syx | 153 | header declares 155 data bytes, only 145 present (checksum recomputed) |
| truncated_bank_31_voices.syx | 3976 | header declares 4096 data bytes, only 31 voices present (checksum recomputed) |
| bad_framing_voice.syx | 163 | valid voice with `F0` replaced by `0xFA` |
| unsupported_dx7ii_performance.syx | 10 | `F0 43 00 08 00 02 <2 data> ck F7` (DX7II performance format) |
| stream_voice_bank_voice.syx | 4430 | valid_voice + valid_bank + valid_voice concatenated |

sha256 at generation time (Python 3.12, 2026-09-19):

```
360567f7f3275b0a85c46f9eff138f8251725f416bf098a2ee6132e47d30966f  bad_checksum_voice.syx
0b93c73f9d659e0289ea04dda067d41e2fae1dfa57f22b38e5c2a4ae24ac701a  bad_framing_voice.syx
a3d61501e01f582fd5011b56947120658044a2d063ca91c2796a6bd0db6f6a7f  stream_voice_bank_voice.syx
59dad56a5cc67eca0e0d94abc9d613654e3da20d521c527183d416161d38ea37  truncated_bank_31_voices.syx
97acf84f5b60f7658f621f3db88d4f146abbe81b4f91656b21de90cadac6ac2f  truncated_voice.syx
c8f4a9198b7b062782d8098ca2f0ce9640a0106ee6b8277306896b477c510d7d  unsupported_dx7ii_performance.syx
d28921d47c89bb774df6928f55b6037b80423746a91ddf2e63779132a9045861  valid_bank.syx
d0b8879d57bfbbe9081b64e9f5cd03000ac24439ff2c74ad9cc72916e9bbb0a7  valid_voice.syx
```

## Uncommitted real-file spot check (evidence only)

Source: `DX7_AllTheWeb.zip` (Aminet collection), extracted locally, never
committed. Two files were probed:

- `DX7_AllTheWeb/Aminet/10.syx`, 4104 bytes,
  sha256 `0564f109cacfab72d92b388ca955737542fbae869febe84e5ba79b3c73bf2ea9`:
  bank decodes 32 voices (e.g. `SIZLECYMB.`, `LAYR VOICE`, `WURLI/TREM`),
  checksum PASS, encode round trip byte-identical; 9 voices carry
  out-of-range/reserved-bit anomalies (e.g. rate_scaling 4-7 in packed
  fields, rate_1=127), decoded as-is and flagged, never clamped.
- `DX7_AllTheWeb/Bridge Music Recording Studio/Sysex/Set 26 A-Z/Misc G-L/Glashrp1.syx`,
  163 bytes, sha256
  `c8a7861cb34aff46bfda069d8ea3b367e2437486fd17b22cc8213bd0fc503245`:
  single voice `GLASHARP 1`, algorithm 15, checksum PASS, round trip
  byte-identical.

These confirm the framing/field-map choice (`01 1B` count split, name offsets
145-154 unpacked / 118-127 packed) against real DX7 data.
