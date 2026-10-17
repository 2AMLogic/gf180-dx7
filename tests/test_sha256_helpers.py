"""Known-answer tests for the shared SHA-256 helpers (issue #141)."""

import hashlib
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


if __name__ == "__main__":
    unittest.main()
