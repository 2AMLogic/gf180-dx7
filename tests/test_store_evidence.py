"""Tests for tools/store_evidence.py (issue #178). A fake `klt` stands in for
the broker, so these prove the wrapper's fail-closed behavior and manifest
shape, not that the real store accepted the bytes."""

import json
import os
import stat
import subprocess
import sys
import tempfile
import unittest

TOOLS = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                     "tools")
STORE = os.path.join(TOOLS, "store_evidence.py")

FAKE_KLT = r'''#!/usr/bin/env python3
import hashlib, json, os, sys
a = sys.argv[1:]
if os.environ.get("FAKE_KLT_FAIL_ON") and a[-1].endswith(os.environ["FAKE_KLT_FAIL_ON"]):
    sys.stderr.write("broker_unreachable\n"); sys.exit(1)
m = a[a.index("--manifest") + 1]
name = a[a.index("--name") + 1]
data = open(a[-1], "rb").read()
sha = hashlib.sha256(data).hexdigest()
doc = json.load(open(m)) if os.path.exists(m) else {"schema_version": 1, "entries": []}
doc["entries"].append({"sha256": sha, "size": len(data), "kind": "evidence",
    "producer": {"tool": "klt", "version": "0", "loom.repo": "o/r", "commit": None},
    "uri": "artifact:evidence/o__r/%s/%s" % (sha[:2], sha), "name": name})
json.dump(doc, open(m, "w"))
'''


class Case(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        t = self.tmp.name
        self.root = os.path.join(t, "repo")
        os.makedirs(self.root)
        subprocess.run(["git", "init", "-q", self.root], check=True)
        self.bin = os.path.join(t, "bin")
        os.makedirs(self.bin)
        klt = os.path.join(self.bin, "klt")
        with open(klt, "w") as fh:
            fh.write(FAKE_KLT)
        os.chmod(klt, os.stat(klt).st_mode | stat.S_IXUSR)
        self.stage = os.path.join(t, "stage")
        os.makedirs(os.path.join(self.stage, "sub"))
        open(os.path.join(self.stage, "a.log"), "w").write("alpha")
        open(os.path.join(self.stage, "sub", "b.log"), "w").write("beta")
        self.summary = os.path.join(self.root, "evidence", "b", "r1.md")
        self.manifest = os.path.join(self.root, "manifests", "artifacts",
                                     "b-r1.manifest.json")

    def tearDown(self):
        self.tmp.cleanup()

    def run_store(self, broker=True, run_id="r1", fail_on=None, path=True):
        env = {k: v for k, v in os.environ.items()
               if k not in ("KLT_ARTIFACT_BROKER", "FAKE_KLT_FAIL_ON")}
        if broker:
            env["KLT_ARTIFACT_BROKER"] = "http://broker.invalid"
        if fail_on:
            env["FAKE_KLT_FAIL_ON"] = fail_on
        if path:
            env["PATH"] = self.bin + os.pathsep + env["PATH"]
        return subprocess.run(
            [sys.executable, STORE, "b", run_id, self.stage,
             "--summary", self.summary, "--root", self.root],
            capture_output=True, text=True, env=env)

    def test_stores_manifest_and_summary(self):
        proc = self.run_store()
        self.assertEqual(proc.returncode, 0, proc.stderr)
        doc = json.load(open(self.manifest))
        self.assertEqual([e["name"] for e in doc["entries"]],
                         ["a.log", "sub/b.log"])
        self.assertTrue(all(e["uri"].startswith("artifact:")
                            for e in doc["entries"]))
        text = open(self.summary).read()
        self.assertIn(doc["entries"][0]["sha256"], text)

    def test_control_no_broker_writes_nothing(self):
        proc = self.run_store(broker=False)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertFalse(os.path.exists(self.manifest))
        self.assertFalse(os.path.exists(self.summary))

    def test_control_put_failure_leaves_no_partial_manifest(self):
        proc = self.run_store(fail_on="b.log")
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertFalse(os.path.exists(self.manifest))
        self.assertFalse(os.path.exists(self.summary))

    def test_control_existing_manifest_is_not_overwritten(self):
        self.assertEqual(self.run_store().returncode, 0)
        before = open(self.manifest).read()
        os.remove(self.summary)
        proc = self.run_store()
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("append-only", proc.stderr)
        self.assertEqual(open(self.manifest).read(), before)

    def test_control_empty_staging_dir(self):
        for d, _, fs in os.walk(self.stage):
            for f in fs:
                os.remove(os.path.join(d, f))
        proc = self.run_store()
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertFalse(os.path.exists(self.manifest))

    def test_control_klt_missing(self):
        env = {k: v for k, v in os.environ.items()}
        env["KLT_ARTIFACT_BROKER"] = "http://broker.invalid"
        env["PATH"] = "/nonexistent"
        proc = subprocess.run(
            [sys.executable, STORE, "b", "r1", self.stage,
             "--summary", self.summary, "--root", self.root],
            capture_output=True, text=True, env=env)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertFalse(os.path.exists(self.manifest))


if __name__ == "__main__":
    unittest.main()
