#!/usr/bin/env python3
"""Generate the R02 smoke fixture: reference/fixtures/smoke/ (issue #7).

Constructs a synthetic DX7 single voice through the P01 codec
(src/gf180_dx7/sysex.py Voice/Operator/encode_voice) from a fully
deterministic parameter set chosen for a guaranteed non-silent render:

- algorithm 0 (panel algorithm 1: op 2 -> op 1, and op 6 -> op 5 -> op 4 ->
  op 3), feedback 3;
- carrier operators 1 and 3 at output level 99, modulator operators 2, 4,
  5, 6 at output level 90;
- fast attack (rate_1 = 99) to peak 99, sustained level 90, release
  (rate_4 = 42) to 0 so the note-off tail dies inside the 4 s render;
- no LFO/EG modulation of pitch or amplitude, no key scaling;

plus a matching events.txt: delayed note-on at sample 2400 (past the first
64-sample block), velocity 100, note-off at 96000 (2 s of a 4 s render).

All bytes are original (no third-party or real-world voice bytes). Regenerate
with:  python3 tools/make_smoke_fixture.py
"""

from __future__ import annotations

import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7.sysex import Operator, Voice, encode_voice  # noqa: E402

FIXTURE_DIR = REPO / "reference" / "fixtures" / "smoke"

VOICE_NAME = "R02SMOKE01"


def smoke_operator(k: int) -> Operator:
    """Deterministic operator k (1..6).

    Algorithm 0 topology: op 2 modulates carrier op 1; op 6 -> op 5 -> op 4
    modulate carrier op 3. Carriers run at output level 99, modulators at 90
    (a controlled modulation index). EG (DX7 convention: stage 1 = attack,
    stage 4 = release): fast attack (rate_1 = 99) to peak 99, decay to a
    sustained 90, and release (rate_4 = 42) to 0 so the note-off tail dies
    inside the 4 s render.
    """
    carrier = k in (1, 3)
    return Operator(
        rate_1=99, rate_2=71, rate_3=53, rate_4=42,
        level_1=99, level_2=95, level_3=90, level_4=0,
        break_point=99, scale_left_depth=0, scale_right_depth=0,
        scale_left_curve=0, scale_right_curve=0,
        rate_scaling=2, amp_mod_sensitivity=0, key_velocity_sensitivity=2,
        output_level=99 if carrier else 90,
        osc_mode=0, osc_freq_coarse=1, osc_freq_fine=0, osc_detune=7,
    )


def smoke_voice() -> Voice:
    return Voice(
        operators=[smoke_operator(k) for k in range(1, 7)],
        pitch_eg_rate_1=99, pitch_eg_rate_2=99,
        pitch_eg_rate_3=99, pitch_eg_rate_4=99,
        pitch_eg_level_1=50, pitch_eg_level_2=50,
        pitch_eg_level_3=50, pitch_eg_level_4=50,
        algorithm=0, feedback=3, osc_key_sync=0,
        lfo_speed=35, lfo_delay=0, lfo_pitch_mod_depth=0,
        lfo_amp_mod_depth=0, lfo_key_sync=1, lfo_wave=0,
        lfo_pitch_mod_sensitivity=0,
        transpose=24,
        name=VOICE_NAME, name_bytes=VOICE_NAME.encode("ascii"),
        channel_byte=0,
    )


EVENTS_TEXT = """\
# gf180-dx7 R02 smoke fixture events (issue #7)
# Positions are absolute sample offsets at 48000 Hz; the external oracle
# applies every event before the 64-sample block containing its position.
# 2400 lies past the first block (delayed onset); the note sustains until
# 96000 (2 s) inside a 4 s (192000-sample) render.
2400 note_on 60 100
96000 note_off 60
"""


def main() -> None:
    FIXTURE_DIR.mkdir(parents=True, exist_ok=True)
    voice = smoke_voice()
    data = encode_voice(voice)
    (FIXTURE_DIR / "voice.syx").write_bytes(data)

    (FIXTURE_DIR / "events.txt").write_text(EVENTS_TEXT, encoding="utf-8")

    import hashlib
    print(f"voice.syx  {len(data)} bytes  sha256="
          f"{hashlib.sha256(data).hexdigest()}")
    print(f"events.txt {len(EVENTS_TEXT)} bytes")


if __name__ == "__main__":
    main()
