#!/usr/bin/env python3
"""Generate synthetic DX7 SysEx fixtures for tests/fixtures/ (issue #8).

Fixtures are assembled from raw bytes directly from the canonical DX7 field
map (Benson / DXconvert vced2vmem-vmem2vced layout); the codec under test is
NOT imported, so the fixtures are an independent oracle for decode checks.
No third-party (real-world) bytes are committed; see tests/fixtures/MANIFEST.md.
"""

from __future__ import annotations

import hashlib
from pathlib import Path

FIXTURES = Path(__file__).resolve().parents[1] / "tests" / "fixtures"


def ck(body: bytes) -> int:
    return (0x80 - (sum(body) & 0x7F)) & 0x7F


def frame(fmt: int, count_hi: int, count_lo: int, body: bytes,
          channel: int = 0x00, corrupt_checksum: int = 0) -> bytes:
    return (bytes((0xF0, 0x43, channel, fmt, count_hi, count_lo)) + body
            + bytes(((ck(body) + corrupt_checksum) & 0x7F, 0xF7)))


def unpacked_voice_bytes(op_params: list[list[int]],
                         common: list[int], name: bytes) -> bytes:
    """155 unpacked bytes: six 21-byte operator chunks (ops 6..1), 19 common,
    10 name."""
    body = []
    for chunk in range(6):
        body.extend(op_params[5 - chunk])
    body.extend(common)
    assert len(body) == 145 and len(name) == 10
    body.extend(name)
    return bytes(body)


def packed_voice_bytes(op_params: list[list[int]],
                       common: list[int], name: bytes) -> bytes:
    """128 packed bytes: six 17-byte chunks (ops 6..1), 10 common, 10 name.

    Merged fields: [11]=(rc<<2)|lc, [12]=(detune<<3)|rs, [13]=(kvs<<2)|ams,
    [15]=(coarse<<1)|mode, common[9]=(oks<<3)|fb,
    common[14]=(sens<<4)|wave<<1|sync.
    """
    body = []
    for chunk in range(6):
        p = op_params[5 - chunk]
        r1, r2, r3, r4, l1, l2, l3, l4, bp, ld, rd, lc, rc, rs, ams, kvs, \
            ol, mode, coarse, fine, det = p
        body.extend((r1, r2, r3, r4, l1, l2, l3, l4, bp, ld, rd))
        body.append((rc << 2) | lc)
        body.append((det << 3) | rs)
        body.append((kvs << 2) | ams)
        body.append(ol)
        body.append((coarse << 1) | mode)
        body.append(fine)
    pegr1, pegr2, pegr3, pegr4, pegl1, pegl2, pegl3, pegl4, alg, fb, oks, \
        lspd, ldly, lpmd, lamd, lsync, lwave, lsens, transpose = common
    body.extend((pegr1, pegr2, pegr3, pegr4, pegl1, pegl2, pegl3, pegl4))
    body.append(alg)
    body.append((oks << 3) | fb)
    body.extend((lspd, ldly, lpmd, lamd))
    body.append((lsens << 4) | (lwave << 1) | lsync)
    body.append(transpose)
    assert len(body) == 118 and len(name) == 10
    body.extend(name)
    return bytes(body)


def op_set(k: int) -> list[int]:
    """Distinct in-range 21 parameters for operator k (1..6)."""
    return [
        40 + k, 41 + k, 42 + k, 43 + k,          # rates
        10 * k, 10 * k + 1, 10 * k + 2, 10 * k + 3,  # levels
        20 + k,                                   # break point
        30 + k, 60 + k,                           # left/right depth
        k % 4, (k + 1) % 4,                       # left/right curve
        k % 4, k % 4, k % 8,                      # rs, ams, kvs
        90 - 10 * (k - 1),                        # output level
        k % 2,                                    # osc mode
        1 + k,                                    # coarse
        70 + k,                                   # fine
        14 - k,                                   # detune
    ]


COMMON = [60, 61, 62, 63, 70, 71, 72, 73,  # pitch EG
          9, 5, 1,                          # algorithm, feedback, key sync
          42, 17, 88, 66,                   # LFO speed/delay/PMD/AMD
          1, 3, 6, 24]                      # LFO sync/wave/sens, transpose
VOICE_NAME = b"TESTVOICE1"


def main() -> None:
    FIXTURES.mkdir(parents=True, exist_ok=True)
    ops = [op_set(k) for k in range(1, 7)]

    voice_body = unpacked_voice_bytes(ops, COMMON, VOICE_NAME)
    voice = frame(0x00, 0x01, 0x1B, voice_body)
    (FIXTURES / "valid_voice.syx").write_bytes(voice)

    chunks = []
    for i in range(32):
        name = f"BANKVCE{i:03d}".encode()
        bops = [row.copy() for row in ops]
        for j, row in enumerate(bops):
            row[16] = (row[16] + i) % 100       # output level varies
            row[18] = 1 + (i + j) % 31          # coarse varies
        bcommon = COMMON.copy()
        bcommon[8] = i                          # algorithm 0..31
        bcommon[9] = i % 8                      # feedback
        chunks.append(packed_voice_bytes(bops, bcommon, name))
    bank_body = b"".join(chunks)
    bank = frame(0x09, 0x20, 0x00, bank_body)
    (FIXTURES / "valid_bank.syx").write_bytes(bank)

    bad_ck = frame(0x00, 0x01, 0x1B, voice_body, corrupt_checksum=1)
    (FIXTURES / "bad_checksum_voice.syx").write_bytes(bad_ck)

    short_body = voice_body[:145]
    truncated = frame(0x00, 0x01, 0x1B, short_body)
    (FIXTURES / "truncated_voice.syx").write_bytes(truncated)

    short_bank = frame(0x09, 0x20, 0x00, bank_body[:31 * 128])
    (FIXTURES / "truncated_bank_31_voices.syx").write_bytes(short_bank)

    bad_framing = bytes((0xFA,)) + voice[1:]
    (FIXTURES / "bad_framing_voice.syx").write_bytes(bad_framing)

    perf_body = bytes((0x7F, 0x40))
    perf = frame(0x08, 0x00, 0x02, perf_body)
    (FIXTURES / "unsupported_dx7ii_performance.syx").write_bytes(perf)

    stream = voice + bank + voice
    (FIXTURES / "stream_voice_bank_voice.syx").write_bytes(stream)

    for p in sorted(FIXTURES.glob("*.syx")):
        data = p.read_bytes()
        print(f"{p.name:40s} {len(data):5d} bytes  sha256="
              f"{hashlib.sha256(data).hexdigest()}")


if __name__ == "__main__":
    main()
