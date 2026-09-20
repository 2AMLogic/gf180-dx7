"""Tests for the R02 headless reference renderer (issue #7).

End-to-end and negative-control tests need the external GPL oracle (pinned in
reference/oracle-identity.json) and the R01 Dexed clone; they skip with the
missing path named when those are absent. Fixture/identity/guard checks run
everywhere (stdlib-only, no oracle).
"""

from __future__ import annotations

import array
import hashlib
import importlib.util
import json
import stat
import subprocess
import sys
import tempfile
import unittest
import wave
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
_SRC = REPO / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

from gf180_dx7 import sysex  # noqa: E402

TOOL = REPO / "tools" / "render_reference.py"
IDENTITY = REPO / "reference" / "oracle-identity.json"
FIXTURE_DIR = REPO / "reference" / "fixtures" / "smoke"
DEXED_ROOT = Path("/Users/joseph/dev/dexed")

# The oracle lives outside the repo; its path and pin come from the identity.
with open(IDENTITY, "r", encoding="utf-8") as fh:
    _IDENTITY = json.load(fh)
ORACLE = Path(_IDENTITY["binary"]["path"])
ORACLE_PIN = _IDENTITY["binary"]["sha256"]

ORACLE_ABSENT_MSG = (
    f"external oracle binary absent at {ORACLE} (pinned in "
    "reference/oracle-identity.json)")
CLONE_ABSENT_MSG = f"R01 dexed clone unavailable at {DEXED_ROOT}"

RENDER_SAMPLES = 192000
RENDER_RATE = 48000


def load_tool():
    spec = importlib.util.spec_from_file_location("render_reference", TOOL)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def run_harness(*extra: str) -> subprocess.CompletedProcess:
    return subprocess.run(
        [sys.executable, str(TOOL), *extra],
        capture_output=True, text=True, cwd=REPO)


def oracle_present() -> bool:
    return ORACLE.is_file()


def clone_present() -> bool:
    return DEXED_ROOT.is_dir()


class TestSmokeFixtureAndIdentity(unittest.TestCase):
    """No oracle needed: fixture decodes via P01; identity file is sound."""

    def test_fixture_voice_decodes_with_expected_params(self):
        data = (FIXTURE_DIR / "voice.syx").read_bytes()
        self.assertEqual(len(data), 163)
        voice = sysex.decode_voice(data)
        self.assertEqual(voice.anomalies, [])
        self.assertEqual(voice.algorithm, 0)
        self.assertEqual(voice.name, "R02SMOKE01")
        # Deterministic non-silent set: carriers (ops 1, 3) at 99, mods at 90.
        self.assertEqual(voice.operators[0].output_level, 99)
        self.assertEqual(voice.operators[2].output_level, 99)
        for idx in (1, 3, 4, 5):
            self.assertEqual(voice.operators[idx].output_level, 90)
        # EG: fast attack (stage 1), sustained 90, release to 0 (stage 4).
        for op in voice.operators:
            self.assertEqual(op.rate_1, 99)
            self.assertEqual(op.level_1, 99)
            self.assertEqual(op.level_3, 90)
            self.assertEqual(op.rate_4, 42)
            self.assertEqual(op.level_4, 0)
        # Round trip through the codec is byte-identical.
        self.assertEqual(sysex.encode_voice(voice), data)

    def test_fixture_events(self):
        mod = load_tool()
        events = mod.parse_events(
            (FIXTURE_DIR / "events.txt").read_text(encoding="utf-8"))
        self.assertEqual(events[0]["cmd"], "note_on")
        self.assertEqual(events[0]["pos"], 2400)
        self.assertEqual(events[0]["args"], [60, 100])
        self.assertEqual(events[0]["block"], 37)  # delayed onset: block > 0
        note_off = [e for e in events if e["cmd"] == "note_off"]
        self.assertEqual(len(note_off), 1)
        self.assertEqual(note_off[0]["pos"], 96000)
        self.assertTrue(mod.claims_active_note(events))

    def test_identity_file_pins_oracle(self):
        identity = json.loads(IDENTITY.read_text(encoding="utf-8"))
        self.assertRegex(identity["source"]["git_commit"], r"^[0-9a-f]{40}$")
        self.assertRegex(identity["binary"]["sha256"], r"^[0-9a-f]{64}$")
        self.assertEqual(identity["source"]["license"], "GPL-3.0-or-later")
        self.assertIn("external-oracle-only", identity["boundary"])
        self.assertEqual(identity["upstream"]["commit"],
                         "b6cf2e9190e216387ad96f977284ebbef765b447")
        self.assertIn("clang", identity["toolchain_line"])

    def test_silent_output_guard_function(self):
        """Direct negative control: a silent render with an active-note
        claim must be rejected even when no oracle is installed."""
        mod = load_tool()
        events = mod.parse_events("0 note_on 60 100\n")
        silent = array.array("f", [0.0] * 64)
        with self.assertRaises(mod.HarnessFail):
            mod.enforce_nonsilent_guard(silent, events)
        loud = array.array("f", [0.0, 0.5, -0.5] * 21)
        mod.enforce_nonsilent_guard(loud, events)  # must not raise


@unittest.skipUnless(oracle_present(), ORACLE_ABSENT_MSG)
@unittest.skipUnless(clone_present(), CLONE_ABSENT_MSG)
class TestHarnessEndToEnd(unittest.TestCase):
    """Full harness runs against the real oracle."""

    def render_to(self, evidence_dir: Path) -> subprocess.CompletedProcess:
        return run_harness("--fixture", "smoke",
                           "--evidence-dir", str(evidence_dir))

    def test_two_full_runs_byte_identical(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            ev1, ev2 = Path(tmp) / "run1", Path(tmp) / "run2"
            for ev in (ev1, ev2):
                proc = self.render_to(ev)
                self.assertEqual(proc.returncode, 0,
                                 proc.stdout + proc.stderr)
            f32_1 = ev1 / "smoke" / "render.f32"
            f32_2 = ev2 / "smoke" / "render.f32"
            self.assertEqual(sha256_file(f32_1), sha256_file(f32_2),
                             "render.f32 differs between two full harness "
                             "runs")
            meta1 = json.loads((ev1 / "smoke" / "render.json")
                               .read_text(encoding="utf-8"))
            meta2 = json.loads((ev2 / "smoke" / "render.json")
                               .read_text(encoding="utf-8"))
            self.assertEqual(meta1["output"]["sha256"],
                             meta2["output"]["sha256"])
            self.assertTrue(meta1["determinism"]["byte_identical"])
            # (also committed-evidence cross-check when present)
            committed = REPO / "reference" / "evidence" / "smoke" / "render.f32"
            if committed.is_file():
                self.assertEqual(sha256_file(committed), sha256_file(f32_1),
                                 "fresh render disagrees with committed "
                                 "evidence (oracle drift?)")

    def test_wav_convenience_copy_header_valid(self):
        with tempfile.TemporaryDirectory() as tmp:
            ev = Path(tmp) / "ev"
            self.assertEqual(self.render_to(ev).returncode, 0)
            wav_path = ev / "smoke" / "render.wav"
            self.assertTrue(wav_path.is_file())
            with wave.open(str(wav_path), "rb") as wf:
                self.assertEqual(wf.getnchannels(), 1)
                self.assertEqual(wf.getsampwidth(), 2)  # 16-bit PCM copy
                self.assertEqual(wf.getframerate(), RENDER_RATE)
                self.assertEqual(wf.getnframes(), RENDER_SAMPLES)

    def test_metadata_contains_command_hashes_and_listening_status(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            ev = Path(tmp) / "ev"
            self.assertEqual(self.render_to(ev).returncode, 0)
            meta = json.loads((ev / "smoke" / "render.json")
                              .read_text(encoding="utf-8"))
            self.assertEqual(meta["oracle_command"][0], str(ORACLE))
            self.assertIn("--voice", meta["oracle_command"])
            self.assertIn("--events", meta["oracle_command"])
            self.assertEqual(meta["inputs"]["voice"]["sha256"],
                             sha256_file(FIXTURE_DIR / "voice.syx"))
            self.assertEqual(meta["inputs"]["events"]["sha256"],
                             sha256_file(FIXTURE_DIR / "events.txt"))
            self.assertEqual(meta["output"]["sha256"],
                             sha256_file(ev / "smoke" / "render.f32"))
            self.assertEqual(meta["output"]["samples"], RENDER_SAMPLES)
            self.assertTrue(meta["identity_verified"])
            self.assertEqual(meta["oracle_identity"]["binary"]["sha256"],
                             ORACLE_PIN)
            self.assertEqual(meta["listening"],
                             "NOT_RUN: pending human audition")
            self.assertFalse(meta["silent_output_guard"]["triggered"])
            self.assertGreater(meta["stats"]["peak_abs"], 1e-6)
            self.assertEqual(
                meta["event_to_block_policy"],
                json.loads(IDENTITY.read_text(encoding="utf-8"))
                ["event_to_block_policy"])
            self.assertTrue(meta["repo_git_head"])

    def test_missing_fixture_is_not_run(self):
        with tempfile.TemporaryDirectory() as tmp:
            proc = run_harness("--fixture", "no-such-fixture",
                               "--evidence-dir", str(Path(tmp) / "ev"))
            self.assertEqual(proc.returncode, 2)
            self.assertIn("NOT_RUN", proc.stderr)


@unittest.skipUnless(oracle_present(), ORACLE_ABSENT_MSG)
@unittest.skipUnless(clone_present(), CLONE_ABSENT_MSG)
class TestNegativeControls(unittest.TestCase):
    """The smoke check must reject silent substitution and binary tampering."""

    def make_silent_stub(self, root: Path) -> Path:
        """A fake oracle: accepts the CLI, emits a silent f32 of correct
        size. Never committed; lives only in this temp dir."""
        stub = root / "silent_oracle.py"
        stub.write_text(
            "#!/usr/bin/env python3\n"
            "import struct, sys\n"
            "args = sys.argv[1:]\n"
            "def get(flag, default=None):\n"
            "    return args[args.index(flag) + 1] if flag in args "
            "else default\n"
            "rate = int(get('--rate', '48000'))\n"
            "seconds = float(get('--seconds', '4'))\n"
            "n = int(rate * seconds)\n"
            "with open(get('--out'), 'wb') as fh:\n"
            "    fh.write(struct.pack('<%df' % n, *([0.0] * n)))\n",
            encoding="utf-8")
        stub.chmod(stub.stat().st_mode | stat.S_IEXEC)
        return stub

    def identity_pinning(self, root: Path, binary: Path) -> Path:
        identity = json.loads(IDENTITY.read_text(encoding="utf-8"))
        identity["binary"]["path"] = str(binary)
        identity["binary"]["sha256"] = sha256_file(binary)
        path = root / "identity.json"
        path.write_text(json.dumps(identity), encoding="utf-8")
        return path

    def test_silent_substitution_rejected_and_no_evidence(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            stub = self.make_silent_stub(root)
            identity = self.identity_pinning(root, stub)
            ev = root / "evidence"
            proc = run_harness("--fixture", "smoke", "--oracle", str(stub),
                               "--identity", str(identity),
                               "--evidence-dir", str(ev))
            self.assertEqual(proc.returncode, 1,
                             "silent stub must FAIL:\n"
                             + proc.stdout + proc.stderr)
            self.assertIn("silent-output guard", proc.stderr)
            self.assertFalse((ev / "smoke" / "render.f32").exists(),
                             "evidence written despite silent guard")
            self.assertFalse((ev / "smoke" / "render.json").exists())

    def test_tampered_oracle_binary_aborts_identity_verification(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            tampered = root / "tampered-oracle"
            data = bytearray(ORACLE.read_bytes())
            data[-1] ^= 0x01  # one byte changed
            tampered.write_bytes(bytes(data))
            ev = root / "evidence"
            proc = run_harness("--fixture", "smoke", "--oracle",
                               str(tampered), "--evidence-dir", str(ev))
            self.assertEqual(proc.returncode, 1,
                             "tampered oracle must FAIL:\n"
                             + proc.stdout + proc.stderr)
            self.assertIn("identity mismatch", proc.stderr)
            self.assertIn(tampered.name, proc.stderr)
            self.assertFalse((ev / "smoke" / "render.f32").exists())


if __name__ == "__main__":
    unittest.main()
