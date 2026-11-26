"""Exercise pinned-oracle discovery and rendering using a test executable."""

import hashlib
import json
import os
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
from oracle_harness import OracleRenderer, probe_oracle

STUB = """#!/usr/bin/env python3
import argparse
import struct
from pathlib import Path

parser = argparse.ArgumentParser()
for flag in ("voice", "events", "out"):
    parser.add_argument("--" + flag, type=Path, required=True)
parser.add_argument("--rate", type=int, required=True)
parser.add_argument("--seconds", type=float, required=True)
args = parser.parse_args()
with Path(__file__).with_name("calls").open("a") as log:
    print(args.out.parent, file=log)
events = args.events.read_text()
if events == "fail":
    parser.exit(3, "stub render failed")
if events != "no output":
    sample = struct.pack("<f", args.voice.read_bytes()[0] / 8)
    pcm = sample * int(args.rate * args.seconds)
    args.out.write_bytes(pcm[:-1] if events == "truncate" else pcm)
"""


class TestOracleHarness(unittest.TestCase):
    def setUp(self):
        root = Path(self.enterContext(tempfile.TemporaryDirectory()))
        self.enterContext(patch.dict(os.environ))
        os.environ.pop("DX7_ORACLE_ROOT", None)
        self.binary = root / "dx7-oracle"
        self.binary.write_text(STUB)
        self.binary.chmod(0o755)
        self.calls = root / "calls"
        self.identity = root / "identity.json"
        self.identity.write_text(
            json.dumps(
                {
                    "source": {"git_commit": "a" * 40, "license": "test"},
                    "binary": {
                        "path": str(self.binary),
                        "sha256": hashlib.sha256(self.binary.read_bytes()).hexdigest(),
                    },
                    "upstream": {"commit": "b" * 40},
                    "boundary": "external-oracle-only",
                    "toolchain_line": "test executable",
                }
            )
        )

    def test_pinned_render_cache_provenance_and_cleanup(self):
        binary, reason = probe_oracle(self.identity)
        self.assertEqual(binary, self.binary, reason)
        voice, events, seconds = b"\x04", "0 note_on 60 100\n", 0.5
        expected = struct.pack("<f", 0.5) * 4
        with OracleRenderer(binary, rate=8) as renderer:
            self.assertEqual(renderer.render_bytes(voice, events, seconds), expected)
            self.assertEqual(
                renderer.render_f32(voice, events, seconds).tolist(), [0.5] * 4
            )
            self.assertEqual(
                renderer.runs,
                [
                    {
                        "voice_sha256": hashlib.sha256(voice).hexdigest(),
                        "events_sha256": hashlib.sha256(events.encode()).hexdigest(),
                        "seconds": seconds,
                        "pcm_sha256": hashlib.sha256(expected).hexdigest(),
                    }
                ],
            )
            render_dir = Path(self.calls.read_text().strip())
            self.assertTrue(render_dir.is_dir())
            renderer.render_bytes(b"\x02", events, seconds)
            renderer.render_bytes(voice, "0 note_off 60\n", seconds)
            renderer.render_bytes(voice, events, 1.0)
            self.assertEqual(len(self.calls.read_text().splitlines()), 4)
        self.assertFalse(render_dir.exists())

    def test_root_override_still_requires_the_pin(self):
        override = self.binary.parent / "override"
        override.mkdir()
        moved = self.binary.rename(override / "dx7-oracle")
        os.environ["DX7_ORACLE_ROOT"] = str(override)
        self.assertEqual(probe_oracle(self.identity)[0], moved)
        moved.write_text(STUB + "# changed\n")
        path, reason = probe_oracle(self.identity)
        self.assertIsNone(path)
        self.assertRegex(reason, "NOT_RUN.*identity mismatch")
        self.assertFalse((override / "calls").exists())

    def test_missing_binary_is_not_run(self):
        self.binary.unlink()
        path, reason = probe_oracle(self.identity)
        self.assertIsNone(path)
        self.assertRegex(reason, "NOT_RUN.*not found")

    def test_invalid_identity_is_not_run(self):
        identity = json.loads(self.identity.read_text())
        del identity["toolchain_line"]
        self.identity.write_text(json.dumps(identity))
        path, reason = probe_oracle(self.identity)
        self.assertIsNone(path)
        self.assertRegex(reason, "NOT_RUN.*toolchain_line")

    def test_unexpected_probe_error_propagates(self):
        with (
            patch(
                "oracle_harness.check_oracle_binary",
                side_effect=RuntimeError("probe defect"),
            ),
            self.assertRaisesRegex(RuntimeError, "probe defect"),
        ):
            probe_oracle(self.identity)

    def test_bad_renders_fail_without_recording_or_reusing_old_pcm(self):
        renderer = OracleRenderer(self.binary)
        self.addCleanup(renderer.close)
        seconds = 0.001
        renderer.render_bytes(b"v", "good", seconds)
        for event, error in (
            ("no output", "output"),
            ("truncate", "191.*192"),
            ("fail", "stub render failed"),
        ):
            with (
                self.subTest(event=event),
                self.assertRaisesRegex(AssertionError, error),
            ):
                renderer.render_bytes(b"v", event, seconds)
        self.assertEqual(len(renderer.runs), 1)
        render_dir = Path(self.calls.read_text().splitlines()[0])
        renderer.close()
        self.assertFalse(render_dir.exists())


if __name__ == "__main__":
    unittest.main()
