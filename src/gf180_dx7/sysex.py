"""DX7 SysEx codec: lossless import/export of supported voice and bank messages.

Supported forms (D00 default ruling, issue #8):
  - DX7 single voice bulk dump, 163 bytes total:
        F0 43 <sub&channel> 00 01 1B <155 data bytes> <ck> F7
    where the byte count 155 is transmitted as a 7-bit hi/lo pair (0x01, 0x1B).
  - DX7 32-voice bank bulk dump, 4104 bytes total:
        F0 43 <sub&channel> 09 20 00 <4096 data bytes> <ck> F7
    where the byte count 4096 is transmitted as a 7-bit hi/lo pair (0x20, 0x00)
    and each of the 32 voices occupies 128 packed bytes.
  - Concatenations of the above via decode_stream(); messages are never merged.

Everything else (DX9, DX7II performance/additional-voice data, TX7 performance,
parameter changes, non-Yamaha manufacturers) is rejected as UnsupportedFormat,
never guessed from filenames.

Field map provenance: canonical DX7 VCED (155-byte unpacked voice) and VMEM
(128-byte packed voice) layouts per Dave Benson's "Unofficial DX7
Documentation" (dx7.txt, sections on voice/bank formats) as independently
implemented by DXconvert (DXconvert/dx7.py, vced2vmem/vmem2vced) and the Dexed
msfa parameter tables. Operators are stored 6,5,4,3,2,1 in both forms; the
packed form merges bitfields (curves, detune+rate-scaling, KVS+AMS,
coarse+osc-mode, feedback+key-sync, LFO sync/wave/sensitivity). The layout was
validated against real DX7 bank and voice dumps (names at unpacked offsets
145-154 / packed offsets 118-127, checksum agreement).

Decoded policies (documented per issue #8 acceptance):
  - Name policy: Voice.name_bytes holds the raw 10 bytes; Voice.name maps each
    byte outside the printable ASCII range 0x20-0x7E to a space (0x20). The
    raw bytes are never modified.
  - Reserved-bit / range policy: out-of-range values are decoded as-is and
    flagged in `anomalies`; nothing is ever clamped or repaired. In the packed
    bank form this also detects reserved bits set inside merged fields (e.g.
    rate-scaling bits 2+ set, detune > 14), because the split is exact:
    re-encoding always reproduces the original byte. encode_voice/encode_bank
    reject values that cannot be represented in the target form instead of
    silently truncating them.

Checksum: ck = (0x80 - (sum(data bytes) & 0x7F)) & 0x7F over the 155 or 4096
data bytes only (not the framing or the byte-count header).
"""

from __future__ import annotations

from dataclasses import dataclass, field

MANUFACTURER_YAMAHA = 0x43
FORMAT_VOICE = 0x00
FORMAT_BANK = 0x09

VOICE_DATA_LEN = 155
VOICE_COUNT_HI = 0x01
VOICE_COUNT_LO = 0x1B
VOICE_MSG_LEN = 163

VOICE_PACKED_LEN = 128
BANK_VOICES = 32
BANK_DATA_LEN = 4096
BANK_COUNT_HI = 0x20
BANK_COUNT_LO = 0x00
BANK_MSG_LEN = 4104

_SYSEX_START = 0xF0
_SYSEX_END = 0xF7


class SysexError(Exception):
    """Base class for classified SysEx codec errors."""

    def __init__(self, message: str, *, offset: int | None = None,
                 context: str | None = None) -> None:
        super().__init__(message)
        self.message = message
        self.offset = offset
        self.context = context

    def __str__(self) -> str:
        parts = [self.message]
        if self.offset is not None:
            parts.append(f"offset {self.offset}")
        if self.context is not None:
            parts.append(self.context)
        return " (" + ", ".join(parts) + ")" if len(parts) > 1 else self.message


class BadFraming(SysexError):
    """F0/F7 framing or 7-bit data-byte violation."""


class BadChecksum(SysexError):
    """Data checksum does not match the stored checksum byte."""


class BadLength(SysexError):
    """Declared byte count or actual message length is wrong."""


class UnsupportedFormat(SysexError):
    """A structurally framed message outside the supported DX7 forms."""


def checksum(data: bytes) -> int:
    """DX7 data checksum: (0x80 - sum(data)) mod 128."""
    return (0x80 - (sum(data) & 0x7F)) & 0x7F


@dataclass
class Operator:
    rate_1: int
    rate_2: int
    rate_3: int
    rate_4: int
    level_1: int
    level_2: int
    level_3: int
    level_4: int
    break_point: int
    scale_left_depth: int
    scale_right_depth: int
    scale_left_curve: int
    scale_right_curve: int
    rate_scaling: int
    amp_mod_sensitivity: int
    key_velocity_sensitivity: int
    output_level: int
    osc_mode: int
    osc_freq_coarse: int
    osc_freq_fine: int
    osc_detune: int


@dataclass
class Voice:
    """Canonical decoded DX7 voice; every parameter is a named field.

    operators[0] is operator 1 (bottom of the algorithm diagram), operators[5]
    is operator 6. raw_bytes preserves the source data bytes verbatim: the
    155 unpacked bytes for a single-voice message, or the 128 packed bytes
    when the voice came from a bank. channel_byte is the sub-status/channel
    header byte (0x00-0x0F) needed to reproduce the original framing.
    """

    operators: list[Operator]
    pitch_eg_rate_1: int
    pitch_eg_rate_2: int
    pitch_eg_rate_3: int
    pitch_eg_rate_4: int
    pitch_eg_level_1: int
    pitch_eg_level_2: int
    pitch_eg_level_3: int
    pitch_eg_level_4: int
    algorithm: int
    feedback: int
    osc_key_sync: int
    lfo_speed: int
    lfo_delay: int
    lfo_pitch_mod_depth: int
    lfo_amp_mod_depth: int
    lfo_key_sync: int
    lfo_wave: int
    lfo_pitch_mod_sensitivity: int
    transpose: int
    name: str = ""
    name_bytes: bytes = b""
    channel_byte: int = 0
    anomalies: list[str] = field(default_factory=list)
    raw_bytes: bytes = b""


@dataclass
class Bank:
    """Canonical decoded 32-voice bank."""

    voices: list[Voice]
    channel_byte: int = 0
    anomalies: list[str] = field(default_factory=list)
    raw_bytes: bytes = b""


# Unpacked (155-byte VCED) per-operator offsets, for operator chunk base 21*k.
_UNPACKED_OP_FIELDS = (
    ("rate_1", 0, 0, 99), ("rate_2", 1, 0, 99), ("rate_3", 2, 0, 99),
    ("rate_4", 3, 0, 99), ("level_1", 4, 0, 99), ("level_2", 5, 0, 99),
    ("level_3", 6, 0, 99), ("level_4", 7, 0, 99), ("break_point", 8, 0, 99),
    ("scale_left_depth", 9, 0, 99), ("scale_right_depth", 10, 0, 99),
    ("scale_left_curve", 11, 0, 3), ("scale_right_curve", 12, 0, 3),
    ("rate_scaling", 13, 0, 3), ("amp_mod_sensitivity", 14, 0, 3),
    ("key_velocity_sensitivity", 15, 0, 7), ("output_level", 16, 0, 99),
    ("osc_mode", 17, 0, 1), ("osc_freq_coarse", 18, 0, 31),
    ("osc_freq_fine", 19, 0, 99), ("osc_detune", 20, 0, 14),
)

_UNPACKED_COMMON_FIELDS = (
    ("pitch_eg_rate_1", 126, 0, 99), ("pitch_eg_rate_2", 127, 0, 99),
    ("pitch_eg_rate_3", 128, 0, 99), ("pitch_eg_rate_4", 129, 0, 99),
    ("pitch_eg_level_1", 130, 0, 99), ("pitch_eg_level_2", 131, 0, 99),
    ("pitch_eg_level_3", 132, 0, 99), ("pitch_eg_level_4", 133, 0, 99),
    ("algorithm", 134, 0, 31), ("feedback", 135, 0, 7),
    ("osc_key_sync", 136, 0, 1), ("lfo_speed", 137, 0, 99),
    ("lfo_delay", 138, 0, 99), ("lfo_pitch_mod_depth", 139, 0, 99),
    ("lfo_amp_mod_depth", 140, 0, 99), ("lfo_key_sync", 141, 0, 1),
    ("lfo_wave", 142, 0, 5), ("lfo_pitch_mod_sensitivity", 143, 0, 7),
    ("transpose", 144, 0, 48),
)


def _common_values(u: bytes, anomalies: list[str]) -> dict[str, int]:
    values = {}
    for name, off, lo, hi in _UNPACKED_COMMON_FIELDS:
        v = u[off]
        values[name] = v
        if not lo <= v <= hi:
            anomalies.append(f"common {name}={v} outside {lo}-{hi}")
    return values


def _voice_from_values(op_values: list[dict[str, int]],
                       common: dict[str, int], name_bytes: bytes,
                       channel_byte: int, anomalies: list[str],
                       raw: bytes) -> Voice:
    name = "".join(chr(b) if 0x20 <= b <= 0x7E else " " for b in name_bytes)
    ops = []
    for chunk in range(6):
        o = op_values[5 - chunk]  # stored order is operators 6..1
        ops.append(Operator(**o))
    return Voice(
        operators=ops,
        pitch_eg_rate_1=common["pitch_eg_rate_1"],
        pitch_eg_rate_2=common["pitch_eg_rate_2"],
        pitch_eg_rate_3=common["pitch_eg_rate_3"],
        pitch_eg_rate_4=common["pitch_eg_rate_4"],
        pitch_eg_level_1=common["pitch_eg_level_1"],
        pitch_eg_level_2=common["pitch_eg_level_2"],
        pitch_eg_level_3=common["pitch_eg_level_3"],
        pitch_eg_level_4=common["pitch_eg_level_4"],
        algorithm=common["algorithm"],
        feedback=common["feedback"],
        osc_key_sync=common["osc_key_sync"],
        lfo_speed=common["lfo_speed"],
        lfo_delay=common["lfo_delay"],
        lfo_pitch_mod_depth=common["lfo_pitch_mod_depth"],
        lfo_amp_mod_depth=common["lfo_amp_mod_depth"],
        lfo_key_sync=common["lfo_key_sync"],
        lfo_wave=common["lfo_wave"],
        lfo_pitch_mod_sensitivity=common["lfo_pitch_mod_sensitivity"],
        transpose=common["transpose"],
        name=name,
        name_bytes=bytes(name_bytes),
        channel_byte=channel_byte,
        anomalies=anomalies,
        raw_bytes=bytes(raw),
    )


def _decode_unpacked(u: bytes, channel_byte: int) -> Voice:
    anomalies: list[str] = []
    op_values: list[dict[str, int]] = []
    for chunk in range(6):
        base = 21 * chunk
        o = {}
        for name, off, lo, hi in _UNPACKED_OP_FIELDS:
            v = u[base + off]
            o[name] = v
            if not lo <= v <= hi:
                op_num = 6 - chunk
                anomalies.append(
                    f"operator {op_num} {name}={v} outside {lo}-{hi}")
        op_values.append(o)
    common = _common_values(u, anomalies)
    return _voice_from_values(op_values, common, u[145:155], channel_byte,
                              anomalies, u)


def _decode_packed(p: bytes, channel_byte: int,
                   where: str) -> Voice:
    anomalies: list[str] = []
    op_values: list[dict[str, int]] = []
    for chunk in range(6):
        base = 17 * chunk
        op_num = 6 - chunk
        o: dict[str, int] = {}
        for name, off, lo, hi in _UNPACKED_OP_FIELDS[:11]:
            v = p[base + off]
            o[name] = v
            if not lo <= v <= hi:
                anomalies.append(
                    f"{where} operator {op_num} {name}={v} outside {lo}-{hi}")
        merged_curves = p[base + 11]
        merged_det_rs = p[base + 12]
        merged_kvs_ams = p[base + 13]
        output_level = p[base + 14]
        merged_coarse_mode = p[base + 15]
        fine = p[base + 16]
        vals = {
            "scale_left_curve": merged_curves & 0x03,
            "scale_right_curve": merged_curves >> 2,
            "osc_detune": merged_det_rs >> 3,
            "rate_scaling": merged_det_rs & 0x07,
            "key_velocity_sensitivity": merged_kvs_ams >> 2,
            "amp_mod_sensitivity": merged_kvs_ams & 0x03,
            "output_level": output_level,
            "osc_freq_coarse": merged_coarse_mode >> 1,
            "osc_mode": merged_coarse_mode & 0x01,
            "osc_freq_fine": fine,
        }
        for name, lo, hi, bits in (
                ("scale_left_curve", 0, 3, 2), ("scale_right_curve", 0, 3, 5),
                ("osc_detune", 0, 14, 4), ("rate_scaling", 0, 3, 3),
                ("key_velocity_sensitivity", 0, 7, 5),
                ("amp_mod_sensitivity", 0, 3, 2),
                ("output_level", 0, 99, 7),
                ("osc_freq_coarse", 0, 31, 6), ("osc_mode", 0, 1, 1),
                ("osc_freq_fine", 0, 99, 7)):
            v = vals[name]
            o[name] = v
            if not lo <= v <= hi:
                anomalies.append(
                    f"{where} operator {op_num} {name}={v} outside {lo}-{hi}"
                    f" (reserved bits set in packed field, {bits}-bit slot)")
        op_values.append(o)
    merged_fb_ks = p[111]
    lfo_merged = p[116]
    common = {
        "pitch_eg_rate_1": p[102], "pitch_eg_rate_2": p[103],
        "pitch_eg_rate_3": p[104], "pitch_eg_rate_4": p[105],
        "pitch_eg_level_1": p[106], "pitch_eg_level_2": p[107],
        "pitch_eg_level_3": p[108], "pitch_eg_level_4": p[109],
        "algorithm": p[110],
        "feedback": merged_fb_ks & 0x07,
        "osc_key_sync": merged_fb_ks >> 3,
        "lfo_speed": p[112], "lfo_delay": p[113],
        "lfo_pitch_mod_depth": p[114], "lfo_amp_mod_depth": p[115],
        "lfo_key_sync": lfo_merged & 0x01,
        "lfo_wave": (lfo_merged >> 1) & 0x07,
        "lfo_pitch_mod_sensitivity": lfo_merged >> 4,
        "transpose": p[117],
    }
    for name, lo, hi in (("pitch_eg_rate_1", 0, 99), ("pitch_eg_rate_2", 0, 99),
                         ("pitch_eg_rate_3", 0, 99), ("pitch_eg_rate_4", 0, 99),
                         ("pitch_eg_level_1", 0, 99), ("pitch_eg_level_2", 0, 99),
                         ("pitch_eg_level_3", 0, 99), ("pitch_eg_level_4", 0, 99),
                         ("algorithm", 0, 31), ("feedback", 0, 7),
                         ("osc_key_sync", 0, 1), ("lfo_speed", 0, 99),
                         ("lfo_delay", 0, 99), ("lfo_pitch_mod_depth", 0, 99),
                         ("lfo_amp_mod_depth", 0, 99), ("lfo_key_sync", 0, 1),
                         ("lfo_wave", 0, 5), ("lfo_pitch_mod_sensitivity", 0, 7),
                         ("transpose", 0, 48)):
        v = common[name]
        if not lo <= v <= hi:
            anomalies.append(
                f"{where} common {name}={v} outside {lo}-{hi}"
                " (reserved bits set in packed field)")
    return _voice_from_values(op_values, common, p[118:128], channel_byte,
                              anomalies, p)


def _validate_framing(data: bytes) -> None:
    if len(data) < 8:
        raise BadFraming(
            f"message too short ({len(data)} bytes) for a DX7 SysEx message",
            offset=0, context="framing")
    if data[0] != _SYSEX_START:
        raise BadFraming(
            f"expected SysEx start 0xF0, found 0x{data[0]:02X}",
            offset=0, context="framing")
    if data[-1] != _SYSEX_END:
        raise BadFraming(
            f"expected SysEx end 0xF7, found 0x{data[-1]:02X}",
            offset=len(data) - 1, context="framing")
    for i in range(1, len(data) - 1):
        if data[i] > 0x7F:
            raise BadFraming(
                f"data byte 0x{data[i]:02X} has high bit set inside SysEx",
                offset=i, context="framing")
    if data[1] != MANUFACTURER_YAMAHA:
        raise UnsupportedFormat(
            f"manufacturer ID 0x{data[1]:02X} is not Yamaha (0x43)",
            offset=1, context="manufacturer")
    if data[2] & 0x70:
        raise UnsupportedFormat(
            f"sub-status {data[2] >> 4} is not a bulk dump (0)",
            offset=2, context="sub-status")


def decode_voice(data: bytes) -> Voice:
    """Decode a DX7 single-voice bulk dump (163 bytes) into a Voice."""
    data = bytes(data)
    _validate_framing(data)
    fmt = data[3]
    if fmt != FORMAT_VOICE:
        if fmt == FORMAT_BANK:
            raise UnsupportedFormat(
                "format 0x09 is a 32-voice bank; use decode_bank()",
                offset=3, context="format")
        raise UnsupportedFormat(
            f"format 0x{fmt:02X} is not a DX7 single voice (0x00)",
            offset=3, context="format")
    if (data[4], data[5]) != (VOICE_COUNT_HI, VOICE_COUNT_LO):
        declared = (data[4] << 7) | data[5]
        raise BadLength(
            f"declared byte count {declared} != 155 for a single voice",
            offset=4, context="byte count")
    if len(data) != VOICE_MSG_LEN:
        raise BadLength(
            f"message length {len(data)} != {VOICE_MSG_LEN} for a single voice",
            offset=0, context="length")
    body = data[6:6 + VOICE_DATA_LEN]
    stored = data[6 + VOICE_DATA_LEN]
    if checksum(body) != stored:
        raise BadChecksum(
            f"checksum byte 0x{stored:02X} != computed 0x{checksum(body):02X}",
            offset=6 + VOICE_DATA_LEN, context="checksum")
    return _decode_unpacked(body, data[2])


def decode_bank(data: bytes) -> Bank:
    """Decode a DX7 32-voice bank bulk dump (4104 bytes) into a Bank."""
    data = bytes(data)
    _validate_framing(data)
    fmt = data[3]
    if fmt != FORMAT_BANK:
        if fmt == FORMAT_VOICE:
            raise UnsupportedFormat(
                "format 0x00 is a single voice; use decode_voice()",
                offset=3, context="format")
        raise UnsupportedFormat(
            f"format 0x{fmt:02X} is not a DX7 32-voice bank (0x09)",
            offset=3, context="format")
    if (data[4], data[5]) != (BANK_COUNT_HI, BANK_COUNT_LO):
        declared = (data[4] << 7) | data[5]
        raise BadLength(
            f"declared byte count {declared} != 4096 for a bank",
            offset=4, context="byte count")
    if len(data) != BANK_MSG_LEN:
        raise BadLength(
            f"message length {len(data)} != {BANK_MSG_LEN} for a bank "
            f"(holds {len(data) - 8} of 4096 data bytes)",
            offset=0, context="length")
    body = data[6:6 + BANK_DATA_LEN]
    stored = data[6 + BANK_DATA_LEN]
    if checksum(body) != stored:
        raise BadChecksum(
            f"checksum byte 0x{stored:02X} != computed 0x{checksum(body):02X}",
            offset=6 + BANK_DATA_LEN, context="checksum")
    anomalies: list[str] = []
    voices = []
    for i in range(BANK_VOICES):
        chunk = body[i * VOICE_PACKED_LEN:(i + 1) * VOICE_PACKED_LEN]
        voices.append(_decode_packed(chunk, data[2], f"bank voice {i}"))
    return Bank(voices=voices, channel_byte=data[2], anomalies=anomalies,
                raw_bytes=data)


def split_stream(data: bytes) -> list[bytes]:
    """Split concatenated SysEx messages; adjacent messages are never merged.

    Every byte must belong to exactly one F0..F7 message; stray bytes between
    messages raise BadFraming. Since every DX7 data field is a 7-bit value,
    0xF7 can only occur as a message terminator.
    """
    data = bytes(data)
    messages: list[bytes] = []
    i = 0
    n = len(data)
    while i < n:
        if data[i] != _SYSEX_START:
            raise BadFraming(
                f"expected SysEx start 0xF0, found 0x{data[i]:02X}",
                offset=i, context="stream")
        end = data.find(_SYSEX_END, i + 1)
        if end == -1:
            raise BadFraming(
                "stream ends without SysEx end 0xF7",
                offset=n - 1, context="stream")
        messages.append(data[i:end + 1])
        i = end + 1
    return messages


def decode_message(data: bytes) -> Voice | Bank:
    """Decode one framed message, dispatching on the format byte."""
    data = bytes(data)
    if len(data) >= 4 and data[0] == _SYSEX_START and data[3] == FORMAT_BANK:
        return decode_bank(data)
    return decode_voice(data)


def decode_stream(data: bytes) -> list[Voice | Bank]:
    """Decode a concatenation of messages into objects, in order."""
    return [decode_message(seg) for seg in split_stream(data)]


def _require_7bit(value: int, where: str) -> int:
    if not 0 <= value <= 0x7F:
        raise SysexError(
            f"{where}={value} cannot be represented as a 7-bit byte",
            context="encode")
    return value


def _operator_bytes_unpacked(op: Operator, num: int) -> list[int]:
    fields = (
        ("rate_1", op.rate_1), ("rate_2", op.rate_2), ("rate_3", op.rate_3),
        ("rate_4", op.rate_4), ("level_1", op.level_1),
        ("level_2", op.level_2), ("level_3", op.level_3),
        ("level_4", op.level_4), ("break_point", op.break_point),
        ("scale_left_depth", op.scale_left_depth),
        ("scale_right_depth", op.scale_right_depth),
        ("scale_left_curve", op.scale_left_curve),
        ("scale_right_curve", op.scale_right_curve),
        ("rate_scaling", op.rate_scaling),
        ("amp_mod_sensitivity", op.amp_mod_sensitivity),
        ("key_velocity_sensitivity", op.key_velocity_sensitivity),
        ("output_level", op.output_level), ("osc_mode", op.osc_mode),
        ("osc_freq_coarse", op.osc_freq_coarse),
        ("osc_freq_fine", op.osc_freq_fine),
        ("osc_detune", op.osc_detune))
    return [_require_7bit(v, f"operator {num} {name}") for name, v in fields]


def _encode_common_unpacked(v: Voice) -> list[int]:
    fields = (
        ("pitch_eg_rate_1", v.pitch_eg_rate_1),
        ("pitch_eg_rate_2", v.pitch_eg_rate_2),
        ("pitch_eg_rate_3", v.pitch_eg_rate_3),
        ("pitch_eg_rate_4", v.pitch_eg_rate_4),
        ("pitch_eg_level_1", v.pitch_eg_level_1),
        ("pitch_eg_level_2", v.pitch_eg_level_2),
        ("pitch_eg_level_3", v.pitch_eg_level_3),
        ("pitch_eg_level_4", v.pitch_eg_level_4),
        ("algorithm", v.algorithm), ("feedback", v.feedback),
        ("osc_key_sync", v.osc_key_sync), ("lfo_speed", v.lfo_speed),
        ("lfo_delay", v.lfo_delay),
        ("lfo_pitch_mod_depth", v.lfo_pitch_mod_depth),
        ("lfo_amp_mod_depth", v.lfo_amp_mod_depth),
        ("lfo_key_sync", v.lfo_key_sync), ("lfo_wave", v.lfo_wave),
        ("lfo_pitch_mod_sensitivity", v.lfo_pitch_mod_sensitivity),
        ("transpose", v.transpose))
    return [_require_7bit(val, f"common {name}") for name, val in fields]


def _message(body: bytes, channel_byte: int, fmt: int,
             count_hi: int, count_lo: int) -> bytes:
    if not 0 <= channel_byte <= 0x0F:
        raise SysexError(
            f"channel_byte={channel_byte} is not a sub-status 0 channel nibble",
            context="encode")
    return (bytes((_SYSEX_START, MANUFACTURER_YAMAHA, channel_byte, fmt,
                   count_hi, count_lo)) + bytes(body)
            + bytes((checksum(body), _SYSEX_END)))


def encode_voice(v: Voice) -> bytes:
    """Encode a Voice as a single-voice bulk dump (163 bytes)."""
    if len(v.operators) != 6:
        raise SysexError(
            f"voice has {len(v.operators)} operators, expected 6",
            context="encode_voice")
    if len(v.name_bytes) > 10:
        raise SysexError("name_bytes longer than 10 bytes",
                         context="encode_voice")
    body: list[int] = []
    for chunk in range(6):
        body.extend(_operator_bytes_unpacked(v.operators[5 - chunk], 6 - chunk))
    body.extend(_encode_common_unpacked(v))
    body.extend(_require_7bit(b, "name byte") for b in v.name_bytes)
    body.extend([0x20] * (VOICE_DATA_LEN - len(body)))
    return _message(bytes(body), v.channel_byte, FORMAT_VOICE,
                    VOICE_COUNT_HI, VOICE_COUNT_LO)


def _pack_operator(op: Operator, num: int) -> list[int]:
    base = _operator_bytes_unpacked(op, num)
    slots = (
        (op.scale_left_curve, 0x03, "operator %d scale_left_curve" % num),
        (op.scale_right_curve, 0x1F, "operator %d scale_right_curve" % num),
        (op.osc_detune, 0x0F, "operator %d osc_detune" % num),
        (op.rate_scaling, 0x07, "operator %d rate_scaling" % num),
        (op.key_velocity_sensitivity, 0x1F,
         "operator %d key_velocity_sensitivity" % num),
        (op.amp_mod_sensitivity, 0x03, "operator %d amp_mod_sensitivity" % num),
        (op.osc_freq_coarse, 0x3F, "operator %d osc_freq_coarse" % num),
        (op.osc_mode, 0x01, "operator %d osc_mode" % num))
    for value, mask, where in slots:
        if value & ~mask:
            raise SysexError(
                f"{where}={value} does not fit its packed bit field",
                context="encode_bank")
    return (base[0:11]
            + [(op.scale_right_curve << 2) | op.scale_left_curve]
            + [(op.osc_detune << 3) | op.rate_scaling]
            + [(op.key_velocity_sensitivity << 2) | op.amp_mod_sensitivity]
            + [base[16]]
            + [(op.osc_freq_coarse << 1) | op.osc_mode]
            + [base[19]])


def _pack_common(v: Voice) -> list[int]:
    common = _encode_common_unpacked(v)
    if v.feedback & ~0x0F:
        raise SysexError(
            f"common feedback={v.feedback} does not fit its packed bit field",
            context="encode_bank")
    if v.osc_key_sync & ~0x0F:
        raise SysexError(
            f"common osc_key_sync={v.osc_key_sync} does not fit its packed"
            " bit field", context="encode_bank")
    if v.lfo_key_sync & ~0x01 or v.lfo_wave & ~0x07 \
            or v.lfo_pitch_mod_sensitivity & ~0x07:
        raise SysexError(
            "common LFO sync/wave/sensitivity do not fit their packed"
            " bit fields", context="encode_bank")
    packed = common[0:8] + [common[8]]
    packed.append((v.osc_key_sync << 3) | v.feedback)
    packed.extend(common[11:15])
    packed.append((v.lfo_pitch_mod_sensitivity << 4) | (v.lfo_wave << 1)
                  | v.lfo_key_sync)
    packed.append(common[18])
    return packed


def _pack_voice(v: Voice) -> bytes:
    if len(v.operators) != 6:
        raise SysexError(
            f"voice has {len(v.operators)} operators, expected 6",
            context="encode_bank")
    body: list[int] = []
    for chunk in range(6):
        body.extend(_pack_operator(v.operators[5 - chunk], 6 - chunk))
    body.extend(_pack_common(v))
    if len(v.name_bytes) > 10:
        raise SysexError("name_bytes longer than 10 bytes",
                         context="encode_bank")
    name = bytes(v.name_bytes) + b"\x20" * (10 - len(v.name_bytes))
    body.extend(name)
    return bytes(body)


def encode_bank(b: Bank) -> bytes:
    """Encode a Bank of exactly 32 voices as a bank bulk dump (4104 bytes)."""
    if len(b.voices) != BANK_VOICES:
        raise SysexError(
            f"bank has {len(b.voices)} voices, expected {BANK_VOICES}",
            context="encode_bank")
    body = b"".join(_pack_voice(v) for v in b.voices)
    return _message(body, b.channel_byte, FORMAT_BANK,
                    BANK_COUNT_HI, BANK_COUNT_LO)
