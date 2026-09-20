"""Tests for the archive cataloger (issue #9 / P02).

Synthetic archives are built in temp dirs from codec-encoded messages. The
negative controls prove the tool refuses to emit a catalog with dropped
entries and that the reconciliation check catches tampering; the committed
alltheweb-catalog.json is re-reconciled against its own rows.
"""

from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path
from unittest.mock import patch

_REPO = Path(__file__).resolve().parents[1]
_SRC = _REPO / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

_spec = importlib.util.spec_from_file_location(
    "catalog_archive", _REPO / "tools" / "catalog_archive.py")
catalog_archive = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(catalog_archive)

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.sysex import Bank, Operator, Voice  # noqa: E402

SCHEMA = "gf180-dx7.archive-catalog/1"
REAL_SHA256 = ("c2c47bd1aa29ab07bdf5e6ec8a25faa1b10e06ea0bcf57f6a83d"
               "8363051ed6ef")
COMMITTED_CATALOG = (_REPO / "corpus" / "archive-catalogs"
                     / "alltheweb-catalog.json")


def mk_voice(name: bytes = b"ALPHA", feedback: int = 5,
             algorithm: int = 9) -> Voice:
    ops = [Operator(
        rate_1=41, rate_2=42, rate_3=43, rate_4=44, level_1=45, level_2=46,
        level_3=47, level_4=48, break_point=50, scale_left_depth=51,
        scale_right_depth=52, scale_left_curve=0, scale_right_curve=1,
        rate_scaling=2, amp_mod_sensitivity=1, key_velocity_sensitivity=3,
        output_level=70, osc_mode=0, osc_freq_coarse=2, osc_freq_fine=33,
        osc_detune=7) for _ in range(6)]
    return Voice(
        operators=ops, pitch_eg_rate_1=60, pitch_eg_rate_2=61,
        pitch_eg_rate_3=62, pitch_eg_rate_4=63, pitch_eg_level_1=70,
        pitch_eg_level_2=71, pitch_eg_level_3=72, pitch_eg_level_4=73,
        algorithm=algorithm, feedback=feedback, osc_key_sync=1, lfo_speed=42,
        lfo_delay=17, lfo_pitch_mod_depth=88, lfo_amp_mod_depth=66,
        lfo_key_sync=1, lfo_wave=3, lfo_pitch_mod_sensitivity=6, transpose=24,
        name=name.decode("ascii", "replace"), name_bytes=name, channel_byte=0)


def unsupported_msg() -> bytes:
    body = bytes(155)
    return (bytes((0xF0, 0x43, 0x00, 0x7E, 0x01, 0x1B)) + body
            + bytes((sysex.checksum(body), 0xF7)))


def rebuild_with_body(msg: bytes, body: bytes) -> bytes:
    return msg[:6] + body + bytes((sysex.checksum(body), 0xF7))


def write_zip(path: Path, members: list[tuple[str, bytes]]) -> None:
    with zipfile.ZipFile(path, "w", zipfile.ZIP_STORED) as zf:
        for name, data in members:
            zf.writestr(name, data)


def run_tool(zip_path: Path, out_path: Path,
             extra: list[str] | None = None) -> dict:
    argv = ["--zip", str(zip_path), "--out", str(out_path)] + (extra or [])
    rc = catalog_archive.main(argv)
    assert rc == 0
    return json.loads(out_path.read_text(encoding="utf-8"))


class ToolAvailabilityTests(unittest.TestCase):
    def test_missing_archive_blocks(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            missing = Path(tmp) / "nope.zip"
            with self.assertRaises(SystemExit):
                catalog_archive.build_catalog(
                    missing, "x", "", "local", "rights", None, "cmd")


class SyntheticArchiveTests(unittest.TestCase):
    """3 voices (1 alias pair), 1 bank, 1 bad-checksum, 1 unsupported,
    1 txt, 1 directory: counts reconcile, nothing dropped."""

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        tmp = Path(self.tmp.name)
        self.zip_path = tmp / "mini.zip"
        v_alpha = sysex.encode_voice(mk_voice(name=b"ALPHA"))
        v_bravo = sysex.encode_voice(mk_voice(name=b"BRAVO", feedback=6))
        v_charlie = sysex.encode_voice(mk_voice(name=b"CHARLIE"))
        bank = sysex.encode_bank(
            Bank(voices=[mk_voice(name=b"BANKVWXYZ", feedback=7)] * 32,
                 channel_byte=0))
        bad = bytearray(v_alpha)
        bad[10] ^= 0x01
        self.members = [
            ("banks/v1.syx", v_alpha),
            ("banks/v2.syx", v_bravo),
            ("banks/v3.syx", v_charlie),
            ("banks/bank.syx", bank),
            ("banks/bad.syx", bytes(bad)),
            ("banks/unsup.syx", unsupported_msg()),
            ("notes.txt", b"DX7 archive notes\n"),
            ("docs/", b""),
        ]
        write_zip(self.zip_path, self.members)
        self.out = tmp / "catalog.json"
        self.cat = run_tool(self.zip_path, self.out)

    def test_counts_reconcile(self) -> None:
        cat = self.cat
        catalog_archive._check_reconciliation(cat)
        self.assertEqual(cat["schema"], SCHEMA)
        self.assertEqual(cat["counts"]["total_entries"], 8)
        self.assertEqual(cat["counts"]["by_class"], {
            "voice": 3, "bank": 1, "stream": 0, "unsupported": 1,
            "malformed": 1, "other": 2})
        self.assertEqual(cat["counts"]["malformed_by_reason"],
                         {"bad_checksum": 1})
        self.assertEqual(cat["counts"]["unsupported_by_reason"],
                         {"unsupported_format": 1})
        self.assertEqual(cat["counts"]["voice_instances"],
                         {"standalone": 3, "in_banks": 32, "in_streams": 0})
        self.assertEqual(cat["counts"]["voices_total"], 35)
        self.assertEqual(cat["counts"]["files"], 7)
        self.assertEqual(cat["counts"]["directories"], 1)
        self.assertEqual(cat["archive"]["entry_count"], 8)
        self.assertFalse(cat["run"]["truncated"])

    def test_every_entry_present_in_order(self) -> None:
        with zipfile.ZipFile(self.zip_path) as zf:
            self.assertEqual([e["p"] for e in self.cat["entries"]],
                             zf.namelist())

    def test_alias_pair_detected_exactly_once(self) -> None:
        cat = self.cat
        self.assertEqual(cat["duplicates"]["alias_groups"], 1)
        self.assertEqual(
            cat["duplicates"]["voice_instances_in_alias_groups"], 2)
        group = cat["duplicates"]["largest_alias_groups"][0]
        self.assertEqual(group["instances"], 2)
        self.assertEqual(group["names"], 2)
        self.assertEqual(group["aliases"], 1)
        self.assertEqual(sorted(group["name_list"]),
                         ["ALPHA".ljust(10), "CHARLIE".ljust(10)])
        self.assertEqual(len(cat["hashes"]),
                         cat["counts"]["distinct_canonical_hashes"])
        self.assertEqual(len(cat["hashes"]), 3)
        alpha_hash = cat["entries"][0]["h"]
        self.assertEqual(alpha_hash, cat["entries"][2]["h"])
        self.assertEqual(group["canonical"], alpha_hash)
        self.assertNotEqual(alpha_hash, cat["entries"][1]["h"])

    def test_malformed_classified_with_reason(self) -> None:
        bad_entry = self.cat["entries"][4]
        self.assertEqual(bad_entry["c"], "malformed")
        self.assertEqual(bad_entry["v"], "rejected")
        self.assertEqual(bad_entry["r"], "bad_checksum")


class NegativeControlTests(unittest.TestCase):
    """Any silent drop must fail reconciliation; the tool must refuse to
    write a dropping catalog (issue #9 negative control)."""

    def make_zip(self, tmp: str) -> tuple[Path, list[bytes]]:
        zip_path = Path(tmp) / "nc.zip"
        voices = [sysex.encode_voice(mk_voice(name=b"V%d" % i))
                  for i in range(3)]
        members = [(f"v{i}.syx", v) for i, v in enumerate(voices)]
        members.append(("readme.txt", b"hi\n"))
        write_zip(zip_path, members)
        return zip_path, voices

    def test_dropped_entry_fails_reconciliation(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            zip_path, _ = self.make_zip(tmp)
            out = Path(tmp) / "cat.json"
            cat = run_tool(zip_path, out)
            catalog_archive._check_reconciliation(cat)
            for tamper in (
                    lambda c: c["entries"].pop(1),
                    lambda c: c["counts"].__setitem__("total_entries", 3),
                    lambda c: c["counts"]["by_class"].__setitem__("voice", 2),
                    lambda c: c["entries"][0].__setitem__("h", "0" * 64),
                    lambda c: c["counts"].__setitem__("files", 2)):
                broken = copy.deepcopy(cat)
                tamper(broken)
                with self.assertRaises(ValueError, msg=tamper):
                    catalog_archive._check_reconciliation(broken)

    def test_tool_refuses_to_emit_dropping_catalog(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            zip_path, _ = self.make_zip(tmp)
            out = Path(tmp) / "cat.json"

            class Dropper(catalog_archive.Cataloger):
                def __init__(self) -> None:
                    super().__init__()
                    self.n = 0

                def classify(self, zf, info) -> None:
                    if info.filename.endswith(".syx"):
                        self.n += 1
                        if self.n % 2 == 0:
                            return
                    super().classify(zf, info)

            with patch.object(catalog_archive, "Cataloger", Dropper):
                with self.assertRaises(SystemExit):
                    catalog_archive.build_catalog(
                        zip_path, "x", "", "local", "rights", None, "cmd")
            self.assertFalse(out.exists())

    def test_max_files_truncation_is_recorded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            zip_path, _ = self.make_zip(tmp)
            out = Path(tmp) / "cat3.json"
            cat = run_tool(zip_path, out, ["--max-files", "3"])
            catalog_archive._check_reconciliation(cat)
            self.assertTrue(cat["run"]["truncated"])
            self.assertEqual(cat["run"]["max_files"], 3)
            self.assertEqual(cat["counts"]["total_entries"], 3)
            self.assertEqual(len(cat["entries"]), 3)
            self.assertEqual(cat["archive"]["entry_count"], 4)
            self.assertEqual(sum(cat["counts"]["by_class"].values()), 3)
            with zipfile.ZipFile(zip_path) as zf:
                self.assertEqual([e["p"] for e in cat["entries"]],
                                 zf.namelist()[:3])

    def test_n_copies_under_n_names_report_n_minus_1_aliases(self) -> None:
        n = 4
        with tempfile.TemporaryDirectory() as tmp:
            zip_path = Path(tmp) / "copies.zip"
            members = [("copy%d.syx" % i,
                        sysex.encode_voice(mk_voice(name=b"NAME%d" % i)))
                       for i in range(n)]
            write_zip(zip_path, members)
            cat = run_tool(zip_path, Path(tmp) / "cat.json")
            catalog_archive._check_reconciliation(cat)
            self.assertEqual(cat["counts"]["distinct_canonical_hashes"], 1)
            self.assertEqual(cat["duplicates"]["alias_groups"], 1)
            group = cat["duplicates"]["largest_alias_groups"][0]
            self.assertEqual(group["instances"], n)
            self.assertEqual(group["names"], n)
            self.assertEqual(group["aliases"], n - 1)
            self.assertEqual(group["canonical"], cat["hashes"][0])
            self.assertEqual(sorted(group["name_list"]),
                             [("NAME%d" % i).ljust(10) for i in range(n)])


class CanonicalHashTests(unittest.TestCase):
    def test_name_change_keeps_hash(self) -> None:
        h1 = catalog_archive.canonical_hash(mk_voice(name=b"ALPHA"))
        h2 = catalog_archive.canonical_hash(mk_voice(name=b"ZEBRAXYZ09"))
        self.assertEqual(h1, h2)

    def test_param_change_changes_hash(self) -> None:
        base = catalog_archive.canonical_hash(mk_voice())
        for kwargs in ({"feedback": 6}, {"algorithm": 10}):
            other = catalog_archive.canonical_hash(mk_voice(**kwargs))
            self.assertNotEqual(base, other, kwargs)

    def test_every_non_name_byte_is_hashed(self) -> None:
        base_msg = sysex.encode_voice(mk_voice(name=b"NAMETAGXX"))
        base = catalog_archive.canonical_hash(sysex.decode_voice(base_msg))
        for i in range(155):
            body = bytearray(base_msg[6:161])
            body[i] = (body[i] + 1) % 128
            mutated = sysex.decode_voice(rebuild_with_body(base_msg, body))
            mutated_hash = catalog_archive.canonical_hash(mutated)
            if 145 <= i <= 154:
                self.assertEqual(mutated_hash, base, f"name byte {i}")
            else:
                self.assertNotEqual(mutated_hash, base, f"param byte {i}")

    def test_name_is_canonical_body_zeroed(self) -> None:
        v = mk_voice(name=b"ALPHA")
        msg = sysex.encode_voice(v)
        body = msg[6:161]
        expected = hashlib.sha256(body[:145] + b"\x00" * 10).hexdigest()
        self.assertEqual(catalog_archive.canonical_hash(v), expected)

    def test_bank_voice_hashes_like_standalone(self) -> None:
        twin = mk_voice(name=b"SAMEPATCH")
        bank = sysex.encode_bank(Bank(voices=[twin] * 32, channel_byte=0))
        from_bank = sysex.decode_bank(bank).voices[17]
        self.assertEqual(catalog_archive.canonical_hash(from_bank),
                         catalog_archive.canonical_hash(twin))

    def test_cross_form_alias_group(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            zip_path = Path(tmp) / "forms.zip"
            twin = mk_voice(name=b"SAMEPATCH")
            members = [
                ("single.syx", sysex.encode_voice(twin)),
                ("bank.syx", sysex.encode_bank(
                    Bank(voices=[twin] * 32, channel_byte=0))),
            ]
            write_zip(zip_path, members)
            cat = run_tool(zip_path, Path(tmp) / "cat.json")
            catalog_archive._check_reconciliation(cat)
            self.assertEqual(cat["duplicates"]["alias_groups"], 0)
            self.assertEqual(
                cat["duplicates"]["groups_with_multiple_instances"], 1)
            self.assertEqual(cat["duplicates"]["largest_alias_groups"], [])
            self.assertEqual(cat["counts"]["distinct_canonical_hashes"], 1)
            self.assertEqual(
                cat["counts"]["voice_instances"],
                {"standalone": 1, "in_banks": 32, "in_streams": 0})


class StreamTests(unittest.TestCase):
    def test_voice_bank_voice_stream_expanded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            zip_path = Path(tmp) / "stream.zip"
            a = sysex.encode_voice(mk_voice(name=b"ALPHA"))
            b = sysex.encode_voice(mk_voice(name=b"OMEGA"))
            bank = sysex.encode_bank(
                Bank(voices=[mk_voice(name=b"SAMEPATCH")] * 32,
                     channel_byte=0))
            write_zip(zip_path, [("stream.syx", a + bank + b)])
            cat = run_tool(zip_path, Path(tmp) / "cat.json")
            catalog_archive._check_reconciliation(cat)
            entry = cat["entries"][0]
            self.assertEqual(entry["c"], "stream")
            self.assertEqual(entry["v"], "decoded")
            self.assertEqual(cat["counts"]["voice_instances"]["in_streams"],
                             34)
            self.assertEqual(cat["counts"]["banks_in_streams"], 1)
            self.assertEqual(len(entry["w"]), 34 * 5)
            self.assertEqual(entry["w"][1], -1)
            self.assertEqual(entry["w"][4], 0)
            self.assertEqual(entry["w"][1 + 5 * 33], -1)
            group = cat["duplicates"]["largest_alias_groups"][0]
            self.assertEqual(group["instances"], 34)
            self.assertEqual(sorted(group["name_list"]),
                             ["ALPHA".ljust(10), "OMEGA".ljust(10),
                              "SAMEPATCH".ljust(10)])


class CommittedCatalogTests(unittest.TestCase):
    """The committed real-run catalog must reconcile against its own rows."""

    @classmethod
    def setUpClass(cls) -> None:
        if not COMMITTED_CATALOG.exists():
            raise unittest.SkipTest("committed catalog not present")
        cls.cat = json.loads(COMMITTED_CATALOG.read_text(encoding="utf-8"))

    def test_reconciles(self) -> None:
        catalog_archive._check_reconciliation(self.cat)

    def test_pinned_archive_identity(self) -> None:
        arch = self.cat["archive"]
        self.assertEqual(arch["sha256"], REAL_SHA256)
        self.assertEqual(arch["entry_count"], 13124)
        self.assertEqual(arch["entry_count"],
                         self.cat["counts"]["total_entries"])
        self.assertFalse(self.cat["run"]["truncated"])
        self.assertIsNone(self.cat["run"]["max_files"])

    def test_provenance_and_rights(self) -> None:
        prov = self.cat["provenance"]
        self.assertEqual(prov["collection"], "DX7_AllTheWeb")
        self.assertEqual(prov["compiler"], "Dave Benson")
        self.assertEqual(
            prov["rights"],
            "redistribution rights not established; catalog metadata only,"
            " no audio or bank bytes redistributed")

    def test_no_bytes_embedded(self) -> None:
        allowed = {"p", "s", "c", "v", "r", "rd", "h", "n", "a", "w"}
        keys: set[str] = set()
        for entry in self.cat["entries"]:
            keys.update(entry)
            self.assertIsInstance(entry["p"], str)
            self.assertIsInstance(entry["s"], int)
            self.assertIn(entry["c"],
                          {"voice", "bank", "stream", "unsupported",
                           "malformed", "other"})
            self.assertIn(entry["v"], {"decoded", "rejected", "skipped"})
        self.assertLessEqual(keys, allowed)


if __name__ == "__main__":
    unittest.main()
