"""Tests for the R01 pinned reference manifest and its drift verifier (issue #6).

Checks the manifest schema, re-hashes the pinned files against the local Dexed
clone (skipped when the clone is absent), and runs the negative controls:
a one-byte mutation of a pinned file must be detected and refused, and a
clone at any other commit must be refused.
"""

from __future__ import annotations

import importlib.util
import json
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
MANIFEST = REPO / "reference" / "manifest.json"
TOOL = REPO / "tools" / "verify_reference.py"
PROTOCOL = REPO / "reference" / "oracle-protocol.md"
DEXED_ROOT = Path("/Users/joseph/dev/dexed")

COMMIT_RE = re.compile(r"^[0-9a-f]{40}$")
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


def load_tool():
    spec = importlib.util.spec_from_file_location("verify_reference", TOOL)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def load_manifest():
    with open(MANIFEST, "r", encoding="utf-8") as fh:
        return json.load(fh)


def git_head(root: Path) -> str:
    out = subprocess.run(
        ["git", "-C", str(root), "rev-parse", "HEAD"],
        capture_output=True, text=True, check=True)
    return out.stdout.strip()


def git_init_commit(root: Path) -> str:
    """Make root a git repo with one commit; return its HEAD sha."""
    def run(*cmd):
        subprocess.run(["git", "-C", str(root)] + list(cmd),
                       capture_output=True, text=True, check=True)
    run("init", "-q")
    run("add", "-A")
    run("-c", "user.email=verifier@example.invalid",
        "-c", "user.name=verify_reference test",
        "commit", "-q", "-m", "negative-control tree")
    return git_head(root)


class TestManifestSchema(unittest.TestCase):
    """Schema/structure checks — independent of the Dexed clone."""

    def setUp(self):
        self.man = load_manifest()

    def test_required_top_level_keys(self):
        for key in ("manifest_version", "id", "upstream", "engine",
                    "settings", "files", "toolchain", "oracle_protocol",
                    "drift_policy"):
            self.assertIn(key, self.man)

    def test_upstream_pin(self):
        up = self.man["upstream"]
        self.assertEqual(up["repo_url"], "https://github.com/asb2m10/dexed")
        self.assertTrue(COMMIT_RE.match(up["commit"]),
                        "upstream.commit must be a 40-hex sha")
        self.assertTrue(up.get("commit_subject"))
        self.assertTrue(up.get("commit_date"))
        self.assertIn("commit_verification", up)

    def test_engine_pin_marki_48k(self):
        eng = self.man["engine"]
        self.assertEqual(eng["engine_type"], "marki")
        enum = eng["engine_type_enum"]
        self.assertEqual(enum["name"], "DEXED_ENGINE_MARKI")
        self.assertEqual(enum["value"], 1)
        self.assertEqual(eng["sample_rate"], 48000)

    def test_every_file_entry_complete_and_unique(self):
        paths = set()
        for entry in self.man["files"]:
            for key in ("path", "sha256", "license_class", "license_basis",
                        "role"):
                self.assertIn(key, entry, f"{entry.get('path')}: missing {key}")
            self.assertTrue(SHA256_RE.match(entry["sha256"]),
                            f"{entry['path']}: bad sha256 format")
            p = Path(entry["path"])
            self.assertFalse(p.is_absolute(), f"{entry['path']}: absolute")
            self.assertNotIn("..", p.parts, f"{entry['path']}: path traversal")
            self.assertNotIn(entry["path"], paths, "duplicate pinned path")
            paths.add(entry["path"])
        self.assertGreaterEqual(len(paths), 1)

    def test_engine_files_pinned(self):
        paths = {e["path"] for e in self.man["files"]}
        for required in ("Source/EngineMkI.cpp", "Source/EngineMkI.h",
                         "Source/msfa/controllers.h", "Source/msfa/dx7note.cc",
                         "Source/msfa/fm_core.h", "Source/msfa/sin.h",
                         "Source/msfa/tuning.cc"):
            self.assertIn(required, paths)
        msfa = [p for p in paths if p.startswith("Source/msfa/")]
        self.assertEqual(len(msfa), 24,
                         "every Source/msfa/ file must be pinned individually")

    def test_gpl_files_marked_external_oracle_only(self):
        """DEC-025: GPL material is never adoptable; it is oracle-only."""
        for entry in self.man["files"]:
            if entry["license_class"] == "GPL-3.0-or-later":
                self.assertIn("external-oracle-only", entry["role"],
                              f"{entry['path']}: GPL file must be "
                              f"external-oracle-only")
                self.assertIn("never copied", entry["role"],
                              f"{entry['path']}: GPL role must forbid copying")

    def test_drift_policy_and_verifier_exist(self):
        drift = self.man["drift_policy"]
        self.assertEqual(drift["verifier"], "tools/verify_reference.py")
        self.assertTrue((REPO / drift["verifier"]).is_file())
        self.assertEqual(
            sorted(drift["exit_codes"]),
            ["0", "1", "2"],
            "exit codes must be exactly 0 (PASS), 1 (drift), 2 (NOT_RUN)")

    def test_protocol_doc_declared_and_present(self):
        self.assertEqual(self.man["oracle_protocol"]["protocol_doc"],
                         "reference/oracle-protocol.md")
        self.assertTrue(PROTOCOL.is_file())


@unittest.skipUnless(DEXED_ROOT.is_dir(), "dexed clone unavailable")
class TestPinnedTree(unittest.TestCase):
    """Hash/commit checks against the real clone; negative controls."""

    @classmethod
    def setUpClass(cls):
        cls.man = load_manifest()
        cls.vr = load_tool()
        cls.pinned = cls.man["files"]
        cls.commit = cls.man["upstream"]["commit"]

    def test_clone_head_is_pinned_commit(self):
        self.assertEqual(git_head(DEXED_ROOT), self.commit,
                         "local clone is not at the pinned commit")

    def test_all_pinned_files_exist_and_hash_match(self):
        self.assertEqual(self.vr.verify_hashes(DEXED_ROOT, self.pinned), [])

    def test_cli_passes_on_pinned_clone(self):
        out = subprocess.run([sys.executable, str(TOOL),
                              "--dexed-root", str(DEXED_ROOT)],
                             capture_output=True, text=True)
        self.assertEqual(out.returncode, 0, out.stdout + out.stderr)
        self.assertIn("PASS", out.stdout)

    def test_negative_control_one_mutated_byte_is_refused(self):
        """Copy the pinned tree, flip one byte, verifier must refuse (exit 1)
        naming that file — even with the commit check satisfied."""
        victim = "Source/msfa/sin.h"
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "dexed"
            root.mkdir()
            for entry in self.pinned:
                dest = root / entry["path"]
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(DEXED_ROOT / entry["path"], dest)
            victim_path = root / victim
            data = bytearray(victim_path.read_bytes())
            data[-1] ^= 0x01
            victim_path.write_bytes(bytes(data))
            head = git_init_commit(root)

            # Manifest copy re-pinned to the temp commit: only the mutated
            # byte can cause drift, isolating the hash check end-to-end.
            man = json.loads(json.dumps(self.man))
            man["upstream"]["commit"] = head
            man_path = Path(tmp) / "manifest.json"
            man_path.write_text(json.dumps(man), encoding="utf-8")

            out = subprocess.run(
                [sys.executable, str(TOOL), "--manifest", str(man_path),
                 "--dexed-root", str(root)],
                capture_output=True, text=True)
            self.assertEqual(out.returncode, 1,
                             "mutated byte must be rejected as drift:\n"
                             + out.stdout + out.stderr)
            self.assertIn(victim, out.stdout)
            self.assertIn("FAIL", out.stdout)

    def test_negative_control_wrong_commit_is_refused(self):
        """A usable tree at any other commit must be refused (exit 1)."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "dexed"
            root.mkdir()
            (root / "marker.txt").write_text("some other tree\n",
                                             encoding="utf-8")
            git_init_commit(root)
            self.assertNotEqual(git_head(root), self.commit)
            out = subprocess.run(
                [sys.executable, str(TOOL), "--dexed-root", str(root)],
                capture_output=True, text=True)
            self.assertEqual(out.returncode, 1)
            self.assertIn("commit drift", out.stdout)


class TestVerifierEnvironment(unittest.TestCase):
    """Exit code 2 (NOT_RUN) paths — independent of the Dexed clone."""

    def test_missing_root_is_not_run(self):
        out = subprocess.run(
            [sys.executable, str(TOOL), "--dexed-root",
             "/nonexistent/dexed/definitely-absent"],
            capture_output=True, text=True)
        self.assertEqual(out.returncode, 2)
        self.assertIn("NOT_RUN", out.stderr)


if __name__ == "__main__":
    unittest.main()
