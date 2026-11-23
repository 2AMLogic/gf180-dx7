"""Tests for tools/check_artifact_manifests.py with live negative controls
(issue #178). Each control must make `check` exit 1 for its own reason."""

import copy
import json
import os
import stat
import subprocess
import sys
import tempfile
import unittest

TOOLS = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                     "tools")
CHECKER = os.path.join(TOOLS, "check_artifact_manifests.py")

SHA = "a" * 64
SHA2 = "b" * 64

# Verifier double (issue #307): logs each invocation, exit code via env.
FAKE_KLT = r'''#!/usr/bin/env python3
import os, sys
log = os.environ.get("FAKE_KLT_LOG")
if log:
    with open(log, "a") as fh:
        fh.write(" ".join(sys.argv[1:]) + "\n")
code = os.environ.get("FAKE_KLT_EXIT")
if code:
    sys.stderr.write("simulated klt artifact verify failure\n")
    sys.exit(int(code))
sys.stdout.write("verified ok\n")
'''


def entry(sha=SHA, name="sim/report.json", **over):
    e = {"sha256": sha, "size": 10, "kind": "evidence",
         "producer": {"tool": "klt", "version": "0.7.0",
                      "loom.repo": "o/r", "commit": None},
         "uri": f"artifact:evidence/o__r/{sha[:2]}/{sha}", "name": name}
    e.update(over)
    return e


def manifest(*entries):
    return {"schema_version": 1, "entries": list(entries)}


class Case(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        subprocess.run(["git", "init", "-q", self.root], check=True)
        os.makedirs(os.path.join(self.root, "manifests", "artifacts"))
        self.bin = os.path.join(self.root, "bin")
        os.makedirs(self.bin)
        klt = os.path.join(self.bin, "klt")
        with open(klt, "w") as fh:
            fh.write(FAKE_KLT)
        os.chmod(klt, os.stat(klt).st_mode | stat.S_IXUSR)
        self.klt_log = os.path.join(self.root, "klt-calls.log")

    def tearDown(self):
        self.tmp.cleanup()

    def run_check(self, data, raw=None, track=()):
        for t in track:
            p = os.path.join(self.root, t)
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "w").write("x")
            subprocess.run(["git", "-C", self.root, "add", t], check=True)
        path = os.path.join(self.root, "manifests", "artifacts",
                            "b-r1.manifest.json")
        with open(path, "w") as fh:
            fh.write(raw if raw is not None else json.dumps(data))
        return subprocess.run(
            [sys.executable, CHECKER, "check", "--root", self.root],
            capture_output=True, text=True)

    def assertFails(self, proc, needle):
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn(needle, proc.stderr)

    def run_verify(self, klt_exit=None):
        """verify with broker and verifier double (issue #307)."""
        env = {k: v for k, v in os.environ.items()
               if k not in ("KLT_ARTIFACT_BROKER", "FAKE_KLT_EXIT",
                            "FAKE_KLT_LOG")}
        env["KLT_ARTIFACT_BROKER"] = "http://broker.invalid"
        env["PATH"] = self.bin + os.pathsep + env["PATH"]
        env["FAKE_KLT_LOG"] = self.klt_log
        if klt_exit is not None:
            env["FAKE_KLT_EXIT"] = str(klt_exit)
        return subprocess.run(
            [sys.executable, CHECKER, "verify", "--root", self.root],
            capture_output=True, text=True, env=env)

    def klt_calls(self):
        if not os.path.exists(self.klt_log):
            return []
        with open(self.klt_log) as fh:
            return [l for l in fh.read().splitlines() if l]

    def test_good_manifest_passes_and_says_schema_only(self):
        proc = self.run_check(manifest(entry(), entry(SHA2, "sim/b.json")))
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("NOT hash-verified", proc.stdout)

    def test_no_manifests_passes(self):
        proc = subprocess.run(
            [sys.executable, CHECKER, "--root", self.root],
            capture_output=True, text=True)
        self.assertEqual(proc.returncode, 0, proc.stderr)

    def test_control_non_artifact_uri(self):
        self.assertFails(self.run_check(manifest(entry(uri="https://x/" + SHA))),
                         "artifact:<key>")

    def test_control_s3_uri(self):
        self.assertFails(self.run_check(manifest(entry(uri="s3://bkt/" + SHA))),
                         "s3://")

    def test_control_arn(self):
        e = entry(name="arn:aws:s3:::bkt/x")
        self.assertFails(self.run_check(manifest(e)), "arn:aws:")

    def test_control_key_sha_mismatch(self):
        e = entry(uri=f"artifact:evidence/o__r/aa/{SHA2}")
        self.assertFails(self.run_check(manifest(e)), "does not end in")

    def test_control_duplicate_names(self):
        proc = self.run_check(manifest(entry(), entry(SHA2, "sim/report.json")))
        self.assertFails(proc, "duplicate name")

    def test_control_bytes_also_tracked(self):
        proc = self.run_check(manifest(entry()), track=["sim/report.json"])
        self.assertFails(proc, "git-tracked")

    def test_control_unknown_field_and_bad_sha(self):
        self.assertFails(self.run_check(manifest(entry(extra=1))), "schema")
        bad = entry()
        bad["sha256"] = "A" * 64
        self.assertFails(self.run_check(manifest(bad)), "sha256")

    def test_control_invalid_json(self):
        self.assertFails(self.run_check(None, raw="{"), "not valid JSON")

    def test_verify_without_broker_is_not_run(self):
        self.run_check(manifest(entry()))
        env = {k: v for k, v in os.environ.items()
               if k != "KLT_ARTIFACT_BROKER"}
        proc = subprocess.run(
            [sys.executable, CHECKER, "verify", "--root", self.root],
            capture_output=True, text=True, env=env)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("NOT_RUN", proc.stderr)
        self.assertNotIn("PASS (hash", proc.stdout)

    def test_verify_empty_dir_with_broker_and_klt_is_not_run(self):
        proc = self.run_verify()
        self.assertEqual(proc.returncode, 2, proc.stdout + proc.stderr)
        self.assertIn("NOT_RUN", proc.stderr)
        self.assertNotIn("PASS (hash", proc.stdout)
        self.assertEqual(self.klt_calls(), [])
        self.assertIn("PASS (schema-only", proc.stdout)

    def test_verify_empty_entries_with_broker_and_klt_is_not_run(self):
        self.run_check(manifest())
        proc = self.run_verify()
        self.assertEqual(proc.returncode, 2, proc.stdout + proc.stderr)
        self.assertIn("NOT_RUN", proc.stderr)
        self.assertNotIn("PASS (hash", proc.stdout)
        self.assertEqual(self.klt_calls(), [])

    def test_verify_populated_manifest_passes_via_broker(self):
        self.run_check(manifest(entry()))
        proc = self.run_verify()
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
        self.assertIn("verify PASS (hash-verified via broker)", proc.stdout)
        calls = self.klt_calls()
        self.assertEqual(len(calls), 1, calls)
        self.assertTrue(calls[0].endswith("b-r1.manifest.json"), calls)

    def test_control_verify_mismatch_fails(self):
        self.run_check(manifest(entry()))
        proc = self.run_verify(klt_exit=3)
        self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
        self.assertIn("missing or mismatched", proc.stderr)
        self.assertNotIn("PASS (hash", proc.stdout)
        self.assertEqual(len(self.klt_calls()), 1)

    def test_committed_manifests_are_clean(self):
        repo = os.path.dirname(TOOLS)
        proc = subprocess.run([sys.executable, CHECKER],
                              capture_output=True, text=True, cwd=repo)
        self.assertEqual(proc.returncode, 0, proc.stderr)


if __name__ == "__main__":
    unittest.main()
