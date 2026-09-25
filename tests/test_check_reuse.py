"""Tests for tools/check_reuse.py — the A01 negative control (issue #39).

Cases:
  (a) a temp tree with an unrecorded source file -> check fails (exit 1);
  (b) a recorded file whose hash changed -> check fails (exit 1);
  (c) a compliant tree -> passes (exit 0);
  plus: missing catalog -> could-not-run (exit 2); an adapt destination that
  exists without a recorded adapted hash -> fails (provenance-before-import).
"""

import hashlib
import json
import os
import subprocess
import sys
import tempfile
import unittest

CHECKER = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "tools", "check_reuse.py")

UPSTREAM_BYTES = b"def adopted():\n    return 0x5eed\n"


def sha(data):
    return hashlib.sha256(data).hexdigest()


def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)


def make_catalog(component_extra=None, exempt=None, originals=None):
    return {
        "schema_version": 1,
        "rulings_enum": ["adopt", "adapt", "reject", "reference-only",
                         "external-oracle-only"],
        "local_originals": originals or [],
        "components": [
            {
                "component": "fixture-adopted",
                "ruling": "adopt",
                "upstream": {"repo": "2AMLogic/gf180-example",
                             "commit": "0" * 40,
                             "path": ["lib/comp.py"]},
                "sha256": {"lib/comp.py": sha(UPSTREAM_BYTES)},
                "license": "Apache-2.0",
                "destination": ["src/comp.py"],
                "maintainer": "test",
                "requalification": {"check": "none", "result": "NOT_RUN"},
                "negative_control": "n/a",
                "notes": "fixture",
            },
            {"component": "fixture-adapted",
             "ruling": "adapt",
             "upstream": {"repo": "2AMLogic/gf180-example", "commit": "0" * 40,
                          "path": ["lib/prose.md"]},
             "sha256": {"lib/prose.md": sha(b"upstream prose\n")},
             "license": "Apache-2.0",
             "destination": ["docs/dec.md"],
             "maintainer": "test",
             "requalification": {"check": "none", "result": "NOT_RUN"},
             "negative_control": "n/a",
             "notes": "fixture",
             "adaptations": ["re-expressed"],
             "adapted_sha256": {"docs/dec.md": sha(b"local prose\n")},
             },
        ] + ([component_extra] if component_extra else []),
    }


class CheckReuseTestCase(unittest.TestCase):
    def run_check(self, root):
        return subprocess.run(
            [sys.executable, CHECKER, "--repo-root", root],
            capture_output=True, text=True)

    def test_compliant_tree_passes(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)

    def test_unrecorded_source_file_fails(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        write(os.path.join(tmp, "src", "unrecorded.py"), b"import os\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("unrecorded.py", proc.stdout)

    def test_drifted_hash_fails(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        # adopted file present but bytes edited after the "import"
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        write(os.path.join(tmp, "src", "comp.py"),
              UPSTREAM_BYTES + b"# local tweak\n")
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("drifted", proc.stdout)

    def test_adapt_destination_without_adapted_hash_fails(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        del catalog["components"][1]["adapted_sha256"]
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("without a recorded adapted sha256", proc.stdout)

    def test_missing_catalog_could_not_run(self):
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 2, proc.stdout + proc.stderr)
        self.assertIn("COULD-NOT-RUN", proc.stdout)

    def test_reject_entry_may_not_own_destination(self):
        bad = {"component": "rejected-dsp", "ruling": "reject",
               "upstream": {"repo": "2AMLogic/gf180-example",
                            "commit": "0" * 40, "path": []},
               "sha256": {}, "license": "Apache-2.0",
               "destination": ["src/dsp.v"], "maintainer": "test",
               "requalification": {"check": "none", "result": "NOT_RUN"},
               "negative_control": "n/a", "notes": ""}
        catalog = make_catalog(component_extra=bad, originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("must not declare destinations", proc.stdout)

    def test_original_drift_fails(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"),
              b"# edited after pinning\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("original", proc.stdout)
        self.assertIn("drifted", proc.stdout)

    def test_missing_original_fails(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("missing from the tree", proc.stdout)

    def _compliant_tree(self):
        catalog = make_catalog(originals=[
            {"path": "src/gf180_dx7/__init__.py", "sha256": sha(b""),
             "reason": "local package marker"}])
        tmp = tempfile.mkdtemp(prefix="reuse-check-")
        write(os.path.join(tmp, "docs", "reuse", "catalog.json"),
              json.dumps(catalog).encode())
        write(os.path.join(tmp, "src", "comp.py"), UPSTREAM_BYTES)
        write(os.path.join(tmp, "docs", "dec.md"), b"local prose\n")
        write(os.path.join(tmp, "src", "gf180_dx7", "__init__.py"), b"")
        return tmp

    def test_unrecorded_asic_file_fails(self):
        # PR #84 judge finding: sibling-derived ORFS files under asic/ shipped
        # with no provenance record because asic/ was not governed.
        tmp = self._compliant_tree()
        write(os.path.join(tmp, "asic", "orfs", "sta-synth.tcl"),
              b"define_corners tt ss ff\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("asic/orfs/sta-synth.tcl", proc.stdout)

    def test_git_ignored_build_tree_not_scanned(self):
        tmp = self._compliant_tree()
        try:
            subprocess.run(["git", "init", "-q", tmp], check=True,
                           capture_output=True)
        except (OSError, subprocess.CalledProcessError):
            self.skipTest("git unavailable")
        write(os.path.join(tmp, ".gitignore"), b"work/\n")
        write(os.path.join(tmp, "asic", "orfs", "work", "results", "x.v"),
              b"module x; endmodule\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
        # the same file outside an ignored directory is still governed
        write(os.path.join(tmp, "asic", "orfs", "results", "x.v"),
              b"module x; endmodule\n")
        proc = self.run_check(tmp)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("asic/orfs/results/x.v", proc.stdout)


if __name__ == "__main__":
    unittest.main()
