"""Known-answer tests for the shared SHA-256 helpers (issue #141)."""

import hashlib
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
if str(REPO / "tools") not in sys.path:
    sys.path.insert(0, str(REPO / "tools"))

import sha256_helpers  # noqa: E402


class Sha256HelpersTest(unittest.TestCase):
    def test_empty_known_answer(self):
        self.assertEqual(
            sha256_helpers.sha256_bytes(b""),
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")

    def test_file_matches_bytes_across_chunk_boundary(self):
        data = bytes(range(256)) * (sha256_helpers.CHUNK_SIZE // 256 + 3)
        self.assertGreater(len(data), sha256_helpers.CHUNK_SIZE)
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "blob.bin"
            p.write_bytes(data)
            self.assertEqual(sha256_helpers.sha256_file(p),
                             hashlib.sha256(data).hexdigest())
            self.assertEqual(sha256_helpers.sha256_file(str(p)),
                             sha256_helpers.sha256_bytes(data))


# Scripts whose private sha256_file copy was replaced by the shared helper
# (issue #141, audited catalog-only subset). Each must keep `sha256_file`
# importable from the script itself and resolve to the shared helper.
MIGRATED = (
    "tests/test_h07.py",
    "tests/test_u02_kit.py",
    "tools/ams_window_compare.py",
    "tools/check_reuse.py",
    "tools/demo/fixtures_dx7.py",
    "tools/h01_compare.py",
    "tools/h01_synth.py",
    "tools/h03_budget_check.py",
    "tools/h04_compare.py",
    "tools/h04_synth.py",
    "tools/h05_compare.py",
    "tools/h05_synth.py",
    "tools/h06_compare.py",
    "tools/h06_synth.py",
    "tools/h07_synth.py",
    "tools/h08_synth.py",
    "tools/h10_synth.py",
    "tools/r07_compare.py",
    "tools/storage_probe_synth.py",
    "tools/validate_bank128.py",
)

# Loads the script by path in an isolated interpreter (no PYTHONPATH, no cwd
# entry; tools/ added the way the existing suites do for sibling imports),
# then hashes a file larger than one chunk through the script's own
# `sha256_file`. Prints "<same-object> <digest>".
_PROBE = """
import importlib.util, sys
path, blob, tools = sys.argv[1], sys.argv[2], sys.argv[3]
sys.path.insert(0, tools)
spec = importlib.util.spec_from_file_location("probe_mod", path)
mod = importlib.util.module_from_spec(spec)
sys.modules["probe_mod"] = mod
spec.loader.exec_module(mod)
import sha256_helpers
print(mod.sha256_file is sha256_helpers.sha256_file, mod.sha256_file(blob))
"""


class MigratedCallersTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls._td = tempfile.TemporaryDirectory()
        cls.blob = Path(cls._td.name) / "multi_chunk.bin"
        cls.data = bytes(range(256)) * (sha256_helpers.CHUNK_SIZE // 256 + 5)
        cls.blob.write_bytes(cls.data)
        cls.expected = hashlib.sha256(cls.data).hexdigest()

    @classmethod
    def tearDownClass(cls):
        cls._td.cleanup()

    def test_each_migrated_script_reexports_shared_helper(self):
        for rel in MIGRATED:
            with self.subTest(script=rel):
                proc = subprocess.run(
                    [sys.executable, "-I", "-c", _PROBE,
                     str(REPO / rel), str(self.blob),
                     str(REPO / "tools")],
                    capture_output=True, text=True, cwd=self._td.name)
                self.assertEqual(proc.returncode, 0, proc.stderr[-800:])
                same, digest = proc.stdout.split()
                self.assertEqual(same, "True")
                self.assertEqual(digest, self.expected)

    def test_direct_script_invocation_still_resolves_helper(self):
        proc = subprocess.run(
            [sys.executable, "-I", str(REPO / "tools" / "check_reuse.py"),
             "--help"],
            capture_output=True, text=True, cwd=self._td.name)
        self.assertEqual(proc.returncode, 0, proc.stderr[-800:])

    def test_empty_and_exact_one_chunk_files(self):
        for size in (0, sha256_helpers.CHUNK_SIZE):
            with self.subTest(size=size):
                p = Path(self._td.name) / f"edge_{size}.bin"
                data = b"\xa5" * size
                p.write_bytes(data)
                self.assertEqual(sha256_helpers.sha256_file(p),
                                 hashlib.sha256(data).hexdigest())


def _first_chunk_only(path):
    """Deliberately wrong helper: digests only the first chunk."""
    with open(path, "rb") as fh:
        return hashlib.sha256(fh.read(sha256_helpers.CHUNK_SIZE)).hexdigest()


class FirstChunkFailureControlTest(unittest.TestCase):
    """A first-chunk-only helper must fail a caller's hash comparison."""

    def setUp(self):
        import check_reuse
        self.check_reuse = check_reuse
        self.td = tempfile.TemporaryDirectory()
        root = Path(self.td.name)
        data = b"\x5a" * (sha256_helpers.CHUNK_SIZE + 4096)
        (root / "big.bin").write_bytes(data)
        self.root = str(root)
        self.entry = {
            "component": "control",
            "upstream": {"path": ["up/big.bin"]},
            "sha256": {"up/big.bin": hashlib.sha256(data).hexdigest()},
        }

    def tearDown(self):
        self.td.cleanup()

    def test_correct_helper_passes_caller_comparison(self):
        self.check_reuse.check_one(
            self.root, self.entry, "adopt", {}, "big.bin", "control")

    def test_first_chunk_only_helper_fails_caller_comparison(self):
        original = self.check_reuse.sha256_file
        self.check_reuse.sha256_file = _first_chunk_only
        try:
            with self.assertRaises(self.check_reuse.CheckFailure):
                self.check_reuse.check_one(
                    self.root, self.entry, "adopt", {}, "big.bin", "control")
        finally:
            self.check_reuse.sha256_file = original


if __name__ == "__main__":
    unittest.main()
