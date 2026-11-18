"""Tests for tools/check_signoff_hashes.py — the item-9/10 hash gate (#260).

Live controls (each must demonstrably fail the check it targets):
  (a) a drifted hygiene file (ci.yml moved, its envelope untouched) ->
      exit 1 naming the path, both hashes and the signoff/README.md
      "Regenerating" remedy (the #259 regression replayed);
  (b) a deliberately corrupted content_hash in a COPY of item10.json ->
      exit 1;
  (c) the same for a COPY of item9.json (a testbench path) -> exit 1;
  (d) a missing envelope -> exit 2, asserted NOT 0.
Plus: the clean real tree passes (the failure is not unconditional) and
states that it is not a signoff-fresh pass, an unparseable envelope is
exit 2, an unsupported hash_algorithm is exit 2 (the checker's premise:
plain sha256 over raw bytes), a missing hashed file is a stated exit 1,
a CRLF rewrite of a matching file drifts (hash is over raw bytes),
command/pin entries are skipped without a verdict, an envelope with
nothing to compare is exit 2, and a present entry without a content_hash
is a stated exit 1.

The committed envelopes are never modified: every mutation happens on
copies inside a temp tree.
"""

import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CHECKER = os.path.join(REPO_ROOT, "tools", "check_signoff_hashes.py")
ENVELOPES = ("item9", "item10")


def h(data):
    return "sha256:" + hashlib.sha256(data).hexdigest()


def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data if isinstance(data, bytes) else data.encode("utf-8"))


def envelope(checks, algo="sha256-raw-bytes"):
    return json.dumps({
        "schema_version": 1,
        "kind": "attestation",
        "status": "pass",
        "attestation": {
            "producer": "klt",
            "hash_algorithm": algo,
            "checks": checks,
        },
    }).encode("utf-8")


def load(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


class CheckSignoffHashesTestCase(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.mkdtemp(prefix="signoff-hashes-check-")
        self.addCleanup(shutil.rmtree, self.tmp, True)

    def run_check(self, root=None):
        cmd = [sys.executable, CHECKER]
        if root is not None:
            cmd += ["--repo-root", root]
        return subprocess.run(cmd, capture_output=True, text=True)

    def copy_tree(self):
        """Replicate the committed envelopes and every file they hash into
        the temp tree, so the copy starts as a clean, passing tree."""
        for name in ENVELOPES:
            src = os.path.join(REPO_ROOT, "signoff", "evidence",
                               f"{name}.json")
            env = load(src)
            write(os.path.join(self.tmp, "signoff", "evidence",
                               f"{name}.json"),
                  json.dumps(env).encode("utf-8"))
            for entry in env["attestation"]["checks"]:
                if entry.get("check") == "present" \
                        and "content_hash" in entry:
                    with open(os.path.join(REPO_ROOT, entry["path"]),
                              "rb") as f:
                        write(os.path.join(self.tmp, entry["path"]),
                              f.read())

    def corrupt(self, name, path):
        """Flip one content_hash in the temp copy of envelope `name`."""
        target = os.path.join(self.tmp, "signoff", "evidence",
                              f"{name}.json")
        env = load(target)
        for entry in env["attestation"]["checks"]:
            if entry.get("path") == path:
                entry["content_hash"] = "sha256:" + "0" * 64
        write(target, json.dumps(env).encode("utf-8"))

    def write_envelope(self, name, body):
        write(os.path.join(self.tmp, "signoff", "evidence", f"{name}.json"),
              body)

    # -- the real-tree case ----------------------------------------------

    def test_real_tree_agrees_and_states_scope(self):
        proc = self.run_check()
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("PASS", proc.stdout)
        # A pass here is a hash comparison only; it must never read as a
        # signoff-fresh pass, so the output says so itself.
        self.assertIn("NOT a signoff-fresh pass", proc.stdout)

    # -- control (a): the #259 regression ---------------------------------

    def test_drifted_ci_yml_names_path_hashes_and_remedy(self):
        self.copy_tree()
        # The hygiene file moves; signoff/evidence/item10.json untouched.
        write(os.path.join(self.tmp, ".github", "workflows", "ci.yml"),
              b"name: ci\non: push\njobs: {}\n")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn(".github/workflows/ci.yml", proc.stderr)
        self.assertIn("signoff/README.md", proc.stderr)
        self.assertIn("Regenerating", proc.stderr)
        self.assertRegex(proc.stderr, r"committed sha256:[0-9a-f]{64}")
        self.assertRegex(proc.stderr, r"current sha256:[0-9a-f]{64}")

    # -- controls (b) and (c): corrupted hashes on envelope copies ---------

    def test_corrupted_item10_hash_fails(self):
        self.copy_tree()
        self.corrupt("item10", ".github/workflows/ci.yml")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn(".github/workflows/ci.yml", proc.stderr)

    def test_corrupted_item9_hash_fails(self):
        self.copy_tree()
        self.corrupt("item9", "rtl/tb_alg_router.v")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("rtl/tb_alg_router.v", proc.stderr)

    # -- control (d): missing envelope -------------------------------------

    def test_missing_envelope_could_not_run(self):
        self.copy_tree()
        os.remove(os.path.join(self.tmp, "signoff", "evidence",
                               "item10.json"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("item10.json", proc.stderr)

    # -- envelope integrity -------------------------------------------------

    def test_unparseable_envelope_could_not_run(self):
        self.copy_tree()
        self.write_envelope("item10", b"{not json")
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)

    def test_unsupported_hash_algorithm_could_not_run(self):
        # The checker's premise is plain sha256 over raw bytes; an
        # envelope hashed some other way is unverifiable, not a pass.
        self.copy_tree()
        target = os.path.join(self.tmp, "signoff", "evidence",
                              "item10.json")
        env = load(target)
        env["attestation"]["hash_algorithm"] = "sha3-256-raw-bytes"
        write(target, json.dumps(env).encode("utf-8"))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("hash_algorithm", proc.stderr)

    # -- stated failures, never silent passes -------------------------------

    def test_missing_hashed_file_fails(self):
        self.copy_tree()
        os.remove(os.path.join(self.tmp, "LICENSE"))
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("LICENSE", proc.stderr)

    def test_present_entry_without_hash_is_a_stated_failure(self):
        self.write_envelope("item9", envelope([]))
        self.write_envelope("item10", envelope([
            {"check": "present", "role": "hygiene", "path": "README.md",
             "passed": True},
        ]))
        write(os.path.join(self.tmp, "README.md"), b"readme\n")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("README.md", proc.stderr)
        self.assertIn("content_hash", proc.stderr)

    # -- raw-bytes hashing ----------------------------------------------------

    def test_crlf_rewrite_of_matching_file_drifts(self):
        self.copy_tree()
        target = os.path.join(self.tmp, "LICENSE")
        with open(target, "rb") as f:
            data = f.read()
        self.assertIn(b"\n", data)
        write(target, data.replace(b"\n", b"\r\n"))
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 1, proc.stderr)
        self.assertIn("LICENSE", proc.stderr)

    # -- command/pin entries and the nothing-to-compare guard -----------------

    def test_command_and_pin_entries_are_skipped(self):
        self.write_envelope("item9", envelope([
            {"check": "command", "role": "cold_start", "exit_status": 0,
             "passed": True},
            {"check": "pin", "role": "pdk", "passed": True,
             "value": {"name": "gf180mcuD"}},
        ]))
        self.write_envelope("item10", envelope([
            {"check": "present", "role": "hygiene", "path": "LICENSE",
             "passed": True, "content_hash": h(b"license\n")},
        ]))
        write(os.path.join(self.tmp, "LICENSE"), b"license\n")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("PASS", proc.stdout)

    def test_no_hash_bearing_entries_could_not_run(self):
        self.write_envelope("item9", envelope([
            {"check": "pin", "role": "tools", "passed": True,
             "value": {"klt": "0.8.0"}},
        ]))
        self.write_envelope("item10", envelope([
            {"check": "command", "role": "audit", "exit_status": 0,
             "passed": True},
        ]))
        proc = self.run_check(self.tmp)
        self.assertNotEqual(proc.returncode, 0)
        self.assertEqual(proc.returncode, 2, proc.stderr)
        self.assertIn("nothing to compare", proc.stderr)

    def test_minimal_matching_envelope_passes(self):
        self.write_envelope("item9", envelope([]))
        self.write_envelope("item10", envelope([
            {"check": "present", "role": "hygiene", "path": "README.md",
             "passed": True, "content_hash": h(b"readme\n")},
        ]))
        write(os.path.join(self.tmp, "README.md"), b"readme\n")
        proc = self.run_check(self.tmp)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("PASS", proc.stdout)


if __name__ == "__main__":
    unittest.main()
