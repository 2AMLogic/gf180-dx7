"""Tests for the DX7 SysEx codec (issue #8 / P01).

Independent fixtures in tests/fixtures/ are assembled from raw bytes by
tools/make_test_fixtures.py without importing the codec under test; expected
field values below are hand-derived from that construction, so the
field-by-field checks do not depend on the codec agreeing with itself
(negative control for checksum-only validation).
"""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

_SRC = Path(__file__).resolve().parents[1] / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.sysex import (
    BadChecksum,
    BadFraming,
    BadLength,
    Bank,
    Operator,
    SysexError,
    UnsupportedFormat,
    Voice,
)

FIXTURES = Path(__file__).resolve().parent / "fixtures"


def load(name: str) -> bytes:
    return (FIXTURES / name).read_bytes()


def op_values(op: Operator) -> tuple:
    return tuple(getattr(op, f) for f in op.__dataclass_fields__)


def semantic(voice: Voice) -> tuple:
    return (tuple(op_values(o) for o in voice.operators),
            voice.pitch_eg_rate_1, voice.pitch_eg_rate_2,
            voice.pitch_eg_rate_3, voice.pitch_eg_rate_4,
            voice.pitch_eg_level_1, voice.pitch_eg_level_2,
            voice.pitch_eg_level_3, voice.pitch_eg_level_4,
            voice.algorithm, voice.feedback, voice.osc_key_sync,
            voice.lfo_speed, voice.lfo_delay, voice.lfo_pitch_mod_depth,
            voice.lfo_amp_mod_depth, voice.lfo_key_sync, voice.lfo_wave,
            voice.lfo_pitch_mod_sensitivity, voice.transpose,
            voice.name, voice.name_bytes)


class ValidVoiceTests(unittest.TestCase):
    """Independent fixture decodes expected fields, field by field."""

    def test_decodes_expected_fields(self) -> None:
        v = sysex.decode_voice(load("valid_voice.syx"))
        self.assertEqual(v.algorithm, 9)
        self.assertEqual(v.feedback, 5)
        self.assertEqual(v.osc_key_sync, 1)
        self.assertEqual(v.lfo_speed, 42)
        self.assertEqual(v.lfo_delay, 17)
        self.assertEqual(v.lfo_pitch_mod_depth, 88)
        self.assertEqual(v.lfo_amp_mod_depth, 66)
        self.assertEqual(v.lfo_key_sync, 1)
        self.assertEqual(v.lfo_wave, 3)
        self.assertEqual(v.lfo_pitch_mod_sensitivity, 6)
        self.assertEqual(v.transpose, 24)
        self.assertEqual((v.pitch_eg_rate_1, v.pitch_eg_rate_4), (60, 63))
        self.assertEqual((v.pitch_eg_level_1, v.pitch_eg_level_4), (70, 73))
        self.assertEqual(v.name, "TESTVOICE1")
        self.assertEqual(v.name_bytes, b"TESTVOICE1")
        self.assertEqual(v.raw_bytes, load("valid_voice.syx")[6:161])
        self.assertEqual(v.anomalies, [])
        op1 = v.operators[0]
        self.assertEqual(op1.rate_1, 41)
        self.assertEqual(op1.level_2, 11)
        self.assertEqual(op1.break_point, 21)
        self.assertEqual(op1.scale_left_depth, 31)
        self.assertEqual(op1.scale_right_depth, 61)
        self.assertEqual(op1.scale_left_curve, 1)
        self.assertEqual(op1.scale_right_curve, 2)
        self.assertEqual(op1.rate_scaling, 1)
        self.assertEqual(op1.amp_mod_sensitivity, 1)
        self.assertEqual(op1.key_velocity_sensitivity, 1)
        self.assertEqual(op1.output_level, 90)
        self.assertEqual(op1.osc_mode, 1)
        self.assertEqual(op1.osc_freq_coarse, 2)
        self.assertEqual(op1.osc_freq_fine, 71)
        self.assertEqual(op1.osc_detune, 13)
        op6 = v.operators[5]
        self.assertEqual(op6.output_level, 40)
        self.assertEqual(op6.osc_freq_coarse, 7)
        self.assertEqual(op6.osc_detune, 8)
        self.assertEqual(op6.osc_mode, 0)

    def test_operator_order_is_six_first_in_storage(self) -> None:
        v = sysex.decode_voice(load("valid_voice.syx"))
        self.assertEqual(v.operators[5].output_level, 40)
        self.assertEqual(v.operators[0].output_level, 90)

    def test_round_trip_is_byte_identical(self) -> None:
        raw = load("valid_voice.syx")
        v = sysex.decode_voice(raw)
        self.assertEqual(sysex.encode_voice(v), raw)
        again = sysex.decode_voice(sysex.encode_voice(v))
        self.assertEqual(again, v)


class ValidBankTests(unittest.TestCase):
    def test_decodes_32_voices(self) -> None:
        raw = load("valid_bank.syx")
        bank = sysex.decode_bank(raw)
        self.assertEqual(len(bank.voices), 32)
        self.assertIsInstance(bank, Bank)
        self.assertEqual(bank.raw_bytes, raw)
        self.assertEqual(bank.voices[0].name, "BANKVCE000")
        self.assertEqual(bank.voices[31].name, "BANKVCE031")
        self.assertEqual(bank.voices[0].algorithm, 0)
        self.assertEqual(bank.voices[25].algorithm, 25)
        self.assertEqual(bank.voices[25].feedback, 1)
        self.assertEqual(bank.voices[25].operators[2].osc_freq_coarse,
                         1 + (25 + 2) % 31)
        self.assertEqual(bank.voices[25].operators[2].output_level,
                         (70 + 25) % 100)
        for i, voice in enumerate(bank.voices):
            self.assertEqual(voice.raw_bytes, raw[6 + 128 * i:6 + 128 * (i + 1)])
            self.assertEqual(len(voice.raw_bytes), 128)
            self.assertEqual(voice.anomalies, [])

    def test_round_trip_is_byte_identical(self) -> None:
        raw = load("valid_bank.syx")
        bank = sysex.decode_bank(raw)
        self.assertEqual(sysex.encode_bank(bank), raw)
        self.assertEqual(sysex.decode_bank(sysex.encode_bank(bank)), bank)

    def test_bank_voice_exports_as_single_voice(self) -> None:
        bank = sysex.decode_bank(load("valid_bank.syx"))
        for index in (0, 17, 31):
            voice = bank.voices[index]
            msg = sysex.encode_voice(voice)
            self.assertEqual(len(msg), 163)
            single = sysex.decode_voice(msg)
            self.assertEqual(semantic(single), semantic(voice))
            self.assertEqual(single.raw_bytes, msg[6:161])


class ClassifiedErrorTests(unittest.TestCase):
    def test_bad_checksum(self) -> None:
        raw = load("bad_checksum_voice.syx")
        with self.assertRaises(BadChecksum) as ctx:
            sysex.decode_voice(raw)
        self.assertEqual(ctx.exception.offset, 161)

    def test_truncated_voice_is_bad_length(self) -> None:
        raw = load("truncated_voice.syx")
        self.assertEqual(len(raw), 153)
        with self.assertRaises(BadLength) as ctx:
            sysex.decode_voice(raw)
        self.assertIn("153", str(ctx.exception))
        self.assertIn("163", str(ctx.exception))

    def test_bank_truncated_to_31_voices_is_bad_length(self) -> None:
        raw = load("truncated_bank_31_voices.syx")
        with self.assertRaises(BadLength):
            sysex.decode_bank(raw)

    def test_missing_f7_is_bad_framing(self) -> None:
        raw = load("valid_voice.syx")[:-1]
        with self.assertRaises(BadFraming):
            sysex.decode_voice(raw)

    def test_missing_f0_is_bad_framing(self) -> None:
        raw = bytes((0x43,)) + load("valid_voice.syx")[1:]
        with self.assertRaises(BadFraming) as ctx:
            sysex.decode_voice(raw)
        self.assertEqual(ctx.exception.offset, 0)

    def test_replaced_f0_is_bad_framing(self) -> None:
        with self.assertRaises(BadFraming):
            sysex.decode_voice(load("bad_framing_voice.syx"))

    def test_high_bit_data_byte_is_bad_framing(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[20] |= 0x80
        with self.assertRaises(BadFraming) as ctx:
            sysex.decode_voice(bytes(raw))
        self.assertEqual(ctx.exception.offset, 20)

    def test_dx7ii_performance_is_unsupported(self) -> None:
        with self.assertRaises(UnsupportedFormat) as ctx:
            sysex.decode_voice(load("unsupported_dx7ii_performance.syx"))
        self.assertIn("0x08", str(ctx.exception))

    def test_non_yamaha_manufacturer_is_unsupported(self) -> None:
        raw = bytes((0xF0, 0x7D, 0x00, 0x00, 0x01, 0x1B, 0x00, 0x7F, 0xF7))
        with self.assertRaises(UnsupportedFormat):
            sysex.decode_voice(raw)

    def test_nonzero_substatus_is_unsupported(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[2] = 0x20
        with self.assertRaises(UnsupportedFormat):
            sysex.decode_voice(bytes(raw))

    def test_wrong_declared_count_is_bad_length(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[5] = 0x1A
        with self.assertRaises(BadLength):
            sysex.decode_voice(bytes(raw))

    def test_decoders_reject_each_others_format(self) -> None:
        bank = load("valid_bank.syx")
        voice = load("valid_voice.syx")
        with self.assertRaises(UnsupportedFormat):
            sysex.decode_voice(bank)
        with self.assertRaises(UnsupportedFormat):
            sysex.decode_bank(voice)


class StreamTests(unittest.TestCase):
    def test_voice_bank_voice_yields_three_objects(self) -> None:
        raw = load("stream_voice_bank_voice.syx")
        objects = sysex.decode_stream(raw)
        self.assertEqual(len(objects), 3)
        voice0, bank, voice1 = objects
        self.assertIsInstance(voice0, Voice)
        self.assertIsInstance(bank, Bank)
        self.assertIsInstance(voice1, Voice)
        self.assertEqual(len(bank.voices), 32)
        self.assertEqual(voice0.name, "TESTVOICE1")
        self.assertEqual(voice1.name, "TESTVOICE1")
        self.assertEqual(len(bank.raw_bytes), 4104)

    def test_adjacent_voice_messages_do_not_merge(self) -> None:
        raw = load("valid_voice.syx")
        stream = raw + raw
        self.assertEqual(len(sysex.decode_stream(stream)), 2)
        messages = sysex.split_stream(stream)
        self.assertEqual([len(m) for m in messages], [163, 163])
        self.assertEqual(messages[0], raw)
        self.assertEqual(messages[1], raw)

    def test_stray_bytes_between_messages_raise(self) -> None:
        raw = load("valid_voice.syx")
        with self.assertRaises(BadFraming) as ctx:
            sysex.decode_stream(raw + b"\x00\x01" + raw)
        self.assertEqual(ctx.exception.offset, 163)

    def test_empty_stream_yields_no_objects(self) -> None:
        self.assertEqual(sysex.decode_stream(b""), [])


class NegativeControlTests(unittest.TestCase):
    """A checksum pass alone proves nothing: field values are asserted, and
    every single-bit mutation must be caught or visibly change the decode."""

    def test_every_single_bit_flip_is_caught_or_changes_decode(self) -> None:
        raw = load("valid_voice.syx")
        baseline = semantic(sysex.decode_voice(raw))
        data_start, data_end = 6, 6 + 155
        for byte_index in range(data_start, data_end):
            for bit in range(8):
                mutated = bytearray(raw)
                mutated[byte_index] ^= 1 << bit
                mutated = bytes(mutated)
                try:
                    voice = sysex.decode_voice(mutated)
                except (BadChecksum, BadFraming):
                    continue
                self.assertNotEqual(
                    semantic(voice), baseline,
                    f"silent corruption at byte {byte_index} bit {bit}")

    def test_operator_order_swap_changes_decode(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        chunk0 = raw[6:27]
        raw[6:27] = raw[27:48]
        raw[27:48] = chunk0
        voice = sysex.decode_voice(bytes(raw))
        self.assertEqual(
            (voice.operators[5].rate_1, voice.operators[5].output_level),
            (45, 50))
        self.assertEqual(
            (voice.operators[4].rate_1, voice.operators[4].output_level),
            (46, 40))
        self.assertNotEqual(
            semantic(voice),
            semantic(sysex.decode_voice(load("valid_voice.syx"))))

    def test_checksum_only_assertion_is_insufficient(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        original_algorithm = raw[6 + 134]
        raw[6 + 134] = 20
        raw[-2] = sysex.checksum(bytes(raw[6:-2]))
        voice = sysex.decode_voice(bytes(raw))
        self.assertEqual(voice.algorithm, 20)
        self.assertNotEqual(original_algorithm, 20)


class PolicyTests(unittest.TestCase):
    def test_out_of_range_values_flagged_not_clamped(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[6 + 134] = 40
        raw[6 + 3] = 120
        raw[-2] = sysex.checksum(bytes(raw[6:-2]))
        voice = sysex.decode_voice(bytes(raw))
        self.assertEqual(voice.algorithm, 40)
        self.assertEqual(voice.operators[5].rate_4, 120)
        self.assertTrue(any("algorithm=40 outside 0-31" in a
                            for a in voice.anomalies))
        self.assertTrue(any("operator 6 rate_4=120 outside 0-99" in a
                            for a in voice.anomalies))
        self.assertEqual(sysex.encode_voice(voice), bytes(raw))

    def test_packed_reserved_bits_flagged_and_lossless(self) -> None:
        bank_raw = bytearray(load("valid_bank.syx"))
        chunk_start = 6 + 5 * 128
        bank_raw[chunk_start + 12] = (10 << 3) | 0b101
        bank_raw[-2] = sysex.checksum(bytes(bank_raw[6:-2]))
        bank = sysex.decode_bank(bytes(bank_raw))
        op6 = bank.voices[5].operators[5]
        self.assertEqual(op6.osc_detune, 10)
        self.assertEqual(op6.rate_scaling, 5)
        self.assertTrue(any("rate_scaling=5 outside 0-3" in a
                            for a in bank.voices[5].anomalies))
        self.assertEqual(sysex.encode_bank(bank), bytes(bank_raw))

    def test_name_policy_nonprintable_becomes_space(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[6 + 145:6 + 155] = b"AB\x00C\x7fDEFGH"
        raw[-2] = sysex.checksum(bytes(raw[6:-2]))
        voice = sysex.decode_voice(bytes(raw))
        self.assertEqual(voice.name_bytes, b"AB\x00C\x7fDEFGH")
        self.assertEqual(voice.name, "AB C DEFGH")
        self.assertEqual(sysex.encode_voice(voice), bytes(raw))

    def test_channel_byte_preserved(self) -> None:
        raw = bytearray(load("valid_voice.syx"))
        raw[2] = 0x05
        raw[-2] = sysex.checksum(bytes(raw[6:-2]))
        voice = sysex.decode_voice(bytes(raw))
        self.assertEqual(voice.channel_byte, 0x05)
        self.assertEqual(sysex.encode_voice(voice), bytes(raw))

    def test_encode_rejects_unrepresentable_values(self) -> None:
        voice = sysex.decode_voice(load("valid_voice.syx"))
        voice.operators[0].osc_freq_coarse = 200
        with self.assertRaises(SysexError):
            sysex.encode_voice(voice)
        self.assertEqual(voice.operators[0].osc_freq_coarse, 200)

    def test_encode_rejects_wrong_voice_count(self) -> None:
        bank = sysex.decode_bank(load("valid_bank.syx"))
        with self.assertRaises(SysexError):
            sysex.encode_bank(Bank(voices=bank.voices[:31]))

    def test_checksum_function(self) -> None:
        self.assertEqual(sysex.checksum(bytes([0x63, 0x63])), (0x80 - 0xC6) & 0x7F)
        self.assertEqual(sysex.checksum(b""), 0)


if __name__ == "__main__":
    unittest.main()
