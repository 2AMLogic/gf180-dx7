"""Tests for tools/repin.py (issue #326): positive path + live negative control.

Each case builds a temp tree with a catalog pinning tools/a.py (original) and
an adapted file, so the real catalog is never touched.
"""

import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPIN = os.path.join(ROOT, "tools", "repin.py")
CHECK = os.path.join(ROOT, "tools", "check_reuse.py")


def sha(b):
    return hashlib.sha256(b).hexdigest()


class RepinTest(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.d)
        os.makedirs(os.path.join(self.d, "tools"))
        os.makedirs(os.path.join(self.d, "docs", "reuse"))
        # repin/check_reuse import siblings by path, so copy them in.
        for name in ("repin.py", "check_reuse.py", "sha256_helpers.py",
                     "verdict_errors.py"):
            shutil.copy(os.path.join(ROOT, "tools", name),
                        os.path.join(self.d, "tools", name))
        self.write("tools/a.py", b"print(1)\n")
        self.originals = {}
        for rel in ("tools/a.py", "tools/repin.py", "tools/check_reuse.py",
                    "tools/sha256_helpers.py", "tools/verdict_errors.py"):
            self.originals[rel] = sha(self.read(rel))
        cat = {
            "schema_version": 1, "components": [],
            "local_originals": [
                {"path": r, "sha256": h, "reason": "keep; me untouched " + r}
                for r, h in self.originals.items()
            ],
        }
        self.cat_path = os.path.join(self.d, "docs", "reuse", "catalog.json")
        with open(self.cat_path, "w") as f:
            json.dump(cat, f, indent=1)
            f.write("\n")

    def write(self, rel, data):
        with open(os.path.join(self.d, rel), "wb") as f:
            f.write(data)

    def read(self, rel):
        with open(os.path.join(self.d, rel), "rb") as f:
            return f.read()

    def run_tool(self, script, *args):
        return subprocess.run(
            [sys.executable, os.path.join(self.d, "tools", script),
             "--repo-root" if script == "check_reuse.py" else "--root",
             self.d, *args], capture_output=True, text=True)

    def test_clean_tree_is_noop(self):
        before = open(self.cat_path, "rb").read()
        r = self.run_tool("repin.py")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(before, open(self.cat_path, "rb").read())

    def test_negative_control_then_repin_passes(self):
        self.assertEqual(self.run_tool("check_reuse.py").returncode, 0)
        self.write("tools/a.py", b"print(2)\n")
        # control: without repin the existing pin check must FAIL
        self.assertEqual(self.run_tool("check_reuse.py").returncode, 1)
        before = open(self.cat_path).read()
        r = self.run_tool("repin.py")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(self.run_tool("check_reuse.py").returncode, 0)
        after = open(self.cat_path).read()
        # exactly one digest changed; everything else byte-identical
        old, new = self.originals["tools/a.py"], sha(b"print(2)\n")
        self.assertEqual(before.replace(old, new), after)

    def test_absent_stamp_record_reports_not_run(self):
        # no evidence/capabilities/rtl-core.json in the tree: the coverage
        # check did not run and must not read as "0 stale entries" (a pass)
        r = self.run_tool("repin.py")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("NOT_RUN", r.stdout)
        self.assertIn("rtl-core.json", r.stdout)
        self.assertNotIn("0 stale entries", r.stdout)

    def test_stale_coverage_reported_not_edited(self):
        rec = os.path.join(self.d, "evidence", "capabilities", "rtl-core.json")
        os.makedirs(os.path.dirname(rec))
        with open(rec, "w") as f:
            json.dump({"coverage": {"tools/a.py": "f" * 64}}, f)
        before = open(rec, "rb").read()
        r = self.run_tool("repin.py")
        self.assertIn("stale: tools/a.py", r.stdout)
        self.assertEqual(before, open(rec, "rb").read())

    def test_stamp_non_pass_is_not_reported_pass(self):
        # stub delegate that fails: repin must propagate and never say PASS
        self.write("tools/compile_capabilities.py",
                   b"import sys; sys.exit(1)\n")
        r = self.run_tool("repin.py", "--stamp")
        self.assertEqual(r.returncode, 1)
        self.assertNotIn("stamp rtl-core: PASS", r.stdout)

    def test_stamp_delegates_on_pass(self):
        self.write("tools/compile_capabilities.py", (
            b"import sys, os\n"
            b"root = sys.argv[sys.argv.index('--root') + 1]\n"
            b"open(os.path.join(root, 'stamped'), 'w').write(' '.join(sys.argv[3:]))\n"))
        r = self.run_tool("repin.py", "--stamp")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(self.read("stamped"), b"--stamp rtl-core")

    def test_adapted_pin_drift_repin_passes_check_reuse(self):
        # components[].adapted_sha256 path: a drifted adapted file is re-pinned
        # through the adapted regex, every other byte stays, check_reuse passes
        os.makedirs(os.path.join(self.d, "rtl"), exist_ok=True)
        self.write("rtl/mod.v", b"module mod;\nendmodule\n")
        drifted = sha(b"original adapted bytes\n")
        cat = json.load(open(self.cat_path))
        cat["components"].append({
            "component": "mod-ip", "ruling": "adapt",
            "upstream": {"repo": "example/ip", "commit": "0" * 40,
                         "path": ["src/mod.v"]},
            "sha256": {"src/mod.v": "a" * 64},
            "destination": ["rtl/mod.v"],
            "adapted_sha256": {"rtl/mod.v": drifted},
            "reason": "adapted; every other byte must stay",
        })
        with open(self.cat_path, "w") as f:
            json.dump(cat, f, indent=1)
        # control: the drifted adapted pin fails check_reuse before repin
        self.assertEqual(self.run_tool("check_reuse.py").returncode, 1)
        before = open(self.cat_path).read()
        r = self.run_tool("repin.py")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("repinned rtl/mod.v", r.stdout)
        self.assertEqual(self.run_tool("check_reuse.py").returncode, 0)
        after = open(self.cat_path).read()
        # exactly the drifted digest changed; all other bytes identical
        new = sha(b"module mod;\nendmodule\n")
        self.assertIn(drifted, before)
        self.assertEqual(before.replace(drifted, new), after)

    def test_missing_file_entry_left_alone(self):
        cat = json.load(open(self.cat_path))
        cat["local_originals"].append(
            {"path": "tools/gone.py", "sha256": "0" * 64, "reason": "deleted"})
        with open(self.cat_path, "w") as f:
            json.dump(cat, f, indent=1)
        before = open(self.cat_path, "rb").read()
        self.assertEqual(self.run_tool("repin.py").returncode, 0)
        self.assertEqual(before, open(self.cat_path, "rb").read())

    def test_conflicted_catalog_could_not_run(self):
        with open(self.cat_path, "w") as f:
            f.write("<<<<<<< HEAD\n{}\n=======\n{}\n>>>>>>> x\n")
        self.assertEqual(self.run_tool("repin.py").returncode, 2)

    def test_conflicted_stamp_record_could_not_run_catalog_untouched(self):
        self.write("tools/a.py", b"print(2)\n")  # drift a pin: catalog would be rewritten
        rec = os.path.join(self.d, "evidence", "capabilities", "rtl-core.json")
        os.makedirs(os.path.dirname(rec), exist_ok=True)
        with open(rec, "w") as f:
            f.write("<<<<<<< HEAD\n{}\n=======\n{}\n>>>>>>> x\n")
        before = open(self.cat_path, "rb").read()
        proc = self.run_tool("repin.py")
        self.assertEqual(proc.returncode, 2)
        self.assertIn("COULD-NOT-RUN", proc.stderr)
        self.assertEqual(before, open(self.cat_path, "rb").read())


if __name__ == "__main__":
    unittest.main()
