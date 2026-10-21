"""tools/pdk_resolve.py: sha256-pinned PDK discovery, hermetic (fake trees).

The negative controls are live: a PDK whose liberty has the wrong bytes must
be refused, and an empty search must raise naming every path tried.
"""

import hashlib
import os
import sys
import tempfile
import unittest
from unittest import mock

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "tools"))

import pdk_resolve  # noqa: E402

GOOD = b"fake liberty bytes (the pin is patched to their digest)"


def make_tree(root, data):
    lib = os.path.join(root, pdk_resolve.SC_LIB_REL)
    os.makedirs(os.path.dirname(lib))
    with open(lib, "wb") as fh:
        fh.write(data)


class TestPdkResolve(unittest.TestCase):
    def setUp(self):
        pdk_resolve._sha256.cache_clear()
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.addCleanup(pdk_resolve._sha256.cache_clear)
        pin = mock.patch.object(pdk_resolve, "LIB_SHA256",
                                hashlib.sha256(GOOD).hexdigest())
        pin.start()
        self.addCleanup(pin.stop)

    def _only(self, *roots):
        p = mock.patch.object(pdk_resolve, "candidate_roots",
                              return_value=list(roots))
        p.start()
        self.addCleanup(p.stop)

    def test_right_sha_resolves(self):
        root = os.path.join(self.tmp.name, "good")
        make_tree(root, GOOD)
        self._only(os.path.join(self.tmp.name, "absent"), root)
        self.assertEqual(pdk_resolve.resolve_pdk_root(), root)
        self.assertEqual(pdk_resolve.find_pdk_root(), root)
        self.assertIsNone(pdk_resolve.missing_reason())

    def test_wrong_sha_is_refused_and_next_candidate_wins(self):
        bad = os.path.join(self.tmp.name, "bad")
        good = os.path.join(self.tmp.name, "good")
        make_tree(bad, b"different liberty")
        make_tree(good, GOOD)
        self._only(bad, good)
        self.assertEqual(pdk_resolve.resolve_pdk_root(), good)

    def test_wrong_sha_only_fails_naming_paths_and_digest(self):
        bad = os.path.join(self.tmp.name, "bad")
        missing = os.path.join(self.tmp.name, "missing")
        make_tree(bad, b"different liberty")
        self._only(bad, missing)
        with self.assertRaises(pdk_resolve.PdkNotFound) as cm:
            pdk_resolve.resolve_pdk_root()
        msg = str(cm.exception)
        self.assertIn(bad, msg)
        self.assertIn(missing, msg)
        self.assertIn(pdk_resolve.LIB_SHA256, msg)
        self.assertIsNone(pdk_resolve.find_pdk_root())
        self.assertIn("tried", pdk_resolve.missing_reason())

    def test_none_found_raises(self):
        self._only(os.path.join(self.tmp.name, "nothing"))
        with self.assertRaises(pdk_resolve.PdkNotFound):
            pdk_resolve.resolve_pdk_root()

    def test_pdk_root_env_is_first_candidate(self):
        with mock.patch.dict(os.environ, {"PDK_ROOT": "/x/pdk"}):
            self.assertEqual(pdk_resolve.candidate_roots()[0], "/x/pdk")
        self.assertIn(os.path.expanduser("~/.ciel"),
                      pdk_resolve.candidate_roots())

    def test_explicit_root_honoured_without_sha(self):
        root = os.path.join(self.tmp.name, "explicit")
        make_tree(root, b"operator chosen liberty")
        self._only()
        self.assertEqual(pdk_resolve.resolve_pdk_root(root), root)
        lib, got = pdk_resolve.resolve_liberty(None, root)
        self.assertEqual((lib, got),
                         (os.path.join(root, pdk_resolve.SC_LIB_REL), root))

    def test_explicit_root_without_liberty_fails(self):
        with self.assertRaises(pdk_resolve.PdkNotFound):
            pdk_resolve.resolve_pdk_root(os.path.join(self.tmp.name, "nope"))

    def test_explicit_liberty_without_root(self):
        self._only()
        lib = os.path.join(self.tmp.name, "x.lib")
        open(lib, "w").close()
        self.assertEqual(pdk_resolve.resolve_liberty(lib, None), (lib, None))
        with self.assertRaises(pdk_resolve.PdkNotFound):
            pdk_resolve.resolve_liberty(lib + ".missing", None)

    def test_no_machine_specific_paths_in_code(self):
        # Acceptance: no /Users/joseph defaults left in code paths.
        import subprocess
        out = subprocess.run(
            ["git", "-C", REPO, "grep", "-l", "/Users/" + "joseph", "--",
             "tools", "tests", "src"],
            capture_output=True, text=True, check=False)
        if out.returncode not in (0, 1):
            self.skipTest("NOT_RUN: git grep unavailable")
        self.assertEqual(out.stdout.split(), [])


if __name__ == "__main__":
    unittest.main(verbosity=2)
