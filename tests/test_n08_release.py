"""N08 frozen-release tests (issue #24).

Sections:
1. Preregistration spec: thresholds frozen before challenge, binding hashes
   match the current tree (model modules, registry, numeric profile, dev32).
2. Committed bundle: manifest integrity, bundle hash recomputation, vector
   file integrity, model identity, threshold/spec binding.
3. Verification: `tools/n08_verify_release.py` PASSES on the committed
   bundle (integrity in full + a fast re-render subset); exit 2 when the
   bundle is absent.
4. Determinism: building the bundle twice produces byte-identical outputs
   (subset build; the committed full bundle was additionally rebuilt and
   diffed byte-for-byte at freeze time — recorded in docs/N08-RELEASE.md).
5. TAMPER negative control: one flipped byte in a copied vector file must
   break verification loudly, naming the case (exit 1).
6. STALE rule: a drifted model-module hash in the manifest must FAIL the
   model-identity check (bundle invalid for the changed tree).
7. Mutation battery: the committed results are bound to this bundle and
   every mutation is detected; the fast mutations are re-run LIVE and must
   be caught by the verifier naming the cases.
8. Challenge record: bound to the bundle, evaluated at the frozen
   thresholds (byte-identical), every preregistered challenge case present
   with AGREE_EXACT at this freeze.
9. Exclusions: the indeterminate/divergent/blocked cases are recorded with
   reasons and absent from the bundle.

No fidelity claim: software-reference agreement only (DEC-001, plan
section 7). Stdlib-only.
"""

from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from n08_build_release import (  # noqa: E402
    DEFAULT_OUT, MODEL_MODULES, SPEC_PATH, build_bundle, bundle_hash,
    sha256_file,
)
from n08_mutations import MUTATIONS, run_battery  # noqa: E402
from n08_verify_release import verify_release  # noqa: E402

RELEASE = DEFAULT_OUT
MANIFEST = RELEASE / "manifest.json"
CHALLENGE = RELEASE / "challenge-results.json"
MUTATION_RESULTS = RELEASE / "mutation-results.json"

# Cheap preregistered cases for subset builds / re-renders in tests.
FAST_CASES = ["dir-base", "ev-repeated-note", "alg4-fb3"]
# Mutations that avoid the slow ev-sum-15 render (the committed full
# battery covers it; the live control here keeps the fast seams live).
FAST_MUTATIONS = ["param_shuffle", "wrong_gain", "broken_feedback",
                  "silent_stub"]


def _load(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


@unittest.skipUnless(MANIFEST.is_file(),
                     "reference/release/manifest.json not committed")
class TestPreregistration(unittest.TestCase):
    def setUp(self):
        self.spec = _load(SPEC_PATH)
        self.manifest = _load(MANIFEST)

    def test_thresholds_frozen_before_challenge(self):
        self.assertTrue(self.spec["thresholds_frozen_before_challenge"])
        self.assertTrue(
            self.manifest["covered"]["thresholds_frozen_before_challenge"])

    def test_spec_thresholds_copied_verbatim_into_manifest(self):
        self.assertEqual(self.spec["thresholds"],
                         self.manifest["covered"]["thresholds"])

    def test_frozen_threshold_is_exact(self):
        thresholds = self.spec["thresholds"]
        self.assertEqual(thresholds["vector_conformance"]["kind"],
                         "sha256_exact")
        oracle = thresholds["model_vs_oracle"]
        self.assertEqual(oracle["kind"], "byte_identical_f32le")
        self.assertEqual(oracle["max_abs_error"], 0.0)

    def test_binding_hashes_match_current_tree(self):
        binding = self.spec["binding"]
        self.assertEqual(sha256_file(REPO / binding["registry_path"]),
                         binding["registry_sha256"])
        self.assertEqual(
            sha256_file(REPO / binding["numeric_profile_path"]),
            binding["numeric_profile_sha256"])
        self.assertEqual(sha256_file(REPO / binding["dev32_path"]),
                         binding["dev32_sha256"])
        for mod, want in binding["model_modules"].items():
            self.assertEqual(sha256_file(REPO / mod), want,
                             f"{mod} drifted from the frozen preregistration")

    def test_manifest_model_identity_matches_tree_and_spec(self):
        covered = self.manifest["covered"]
        for mod in MODEL_MODULES:
            self.assertIn(mod, covered["model_identity"]["modules"])
        self.assertEqual(covered["model_identity"]["modules"],
                         self.spec["binding"]["model_modules"])


@unittest.skipUnless(MANIFEST.is_file(),
                     "reference/release/manifest.json not committed")
class TestCommittedBundle(unittest.TestCase):
    def setUp(self):
        self.manifest = _load(MANIFEST)

    def test_bundle_hash_recomputes(self):
        self.assertEqual(self.manifest["bundle_sha256"],
                         bundle_hash(self.manifest["covered"]))

    def test_vector_files_match_recorded_shas(self):
        vectors = self.manifest["covered"]["vectors"]
        self.assertTrue(vectors)
        for case_id, entry in vectors.items():
            path = RELEASE / entry["file"]
            self.assertTrue(path.is_file(), f"missing vector {entry['file']}")
            self.assertEqual(sha256_file(path), entry["sha256"], case_id)

    def test_every_case_has_a_vector_and_expected_sha(self):
        covered = self.manifest["covered"]
        for case in covered["cases"]:
            entry = covered["vectors"].get(case["id"])
            self.assertIsNotNone(entry, case["id"])
            self.assertEqual(entry["sha256"], case["expected_pcm_sha256"])

    def test_event_trace_present_and_block_aligned(self):
        for case in self.manifest["covered"]["cases"]:
            trace = case["event_trace"]
            self.assertEqual(len(trace), case["events_applied"], case["id"])
            for entry in trace:
                self.assertGreaterEqual(entry["block"], 0)
            blocks = [e["block"] for e in trace]
            self.assertEqual(blocks, sorted(blocks), case["id"])

    def test_dev_single_note_cases_record_layer_equality(self):
        spec = _load(SPEC_PATH)
        by_id = {c["id"]: c for c in self.manifest["covered"]["cases"]}
        for case_id in spec["dev_set"]["registry_single_note"]:
            self.assertTrue(by_id[case_id].get("integrated_equal"), case_id)

    def test_selection_matches_preregistration(self):
        spec = _load(SPEC_PATH)
        covered = self.manifest["covered"]
        bundled = {c["id"] for c in covered["cases"]}
        dev_ids = set(spec["dev_set"]["registry_single_note"]) \
            | set(spec["dev_set"]["registry_poly"]) \
            | {f"dev32-{i:02d}" for i in spec["dev_set"]["dev32_indices"]}
        challenge_ids = {e["id"]
                         for e in spec["challenge_set"]["registry_cases"]}
        self.assertEqual(bundled, dev_ids | challenge_ids)
        for case in covered["cases"]:
            want = "challenge" if case["id"] in challenge_ids else "dev"
            self.assertEqual(case["set"], want, case["id"])


@unittest.skipUnless(MANIFEST.is_file(),
                     "reference/release/manifest.json not committed")
class TestVerification(unittest.TestCase):
    def test_verify_passes_on_committed_bundle(self):
        code, rows = verify_release(RELEASE, only=FAST_CASES, rerender=True)
        self.assertEqual(code, 0, [r for r in rows if r["status"] == "FAIL"])
        checks = {(r["check"], r.get("case")) for r in rows}
        for case_id in FAST_CASES:
            self.assertIn(("rerender", case_id), checks)

    def test_verify_names_missing_bundle_not_run(self):
        with tempfile.TemporaryDirectory() as td:
            code, _ = verify_release(Path(td) / "absent", rerender=False)
        self.assertEqual(code, 2)

    def test_tampered_vector_breaks_verification_naming_the_case(self):
        """Negative control: flip one byte in a copied vector; verification
        must fail loudly naming the case (issue #24 negative control)."""
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / "bundle"
            build_bundle(out, only=["dir-base", "alg4-fb3"])
            tampered = Path(td) / "tampered"
            shutil.copytree(out, tampered)
            vec = tampered / "vectors" / "dir-base.f32"
            data = bytearray(vec.read_bytes())
            data[len(data) // 2] ^= 0x01
            vec.write_bytes(bytes(data))
            code, rows = verify_release(tampered, rerender=False)
            self.assertEqual(code, 1)
            fails = [r for r in rows if r["status"] == "FAIL"]
            self.assertTrue(any(r.get("case") == "dir-base"
                                and r["check"] == "vector_file"
                                for r in fails),
                            f"tamper not named: {fails}")

    def test_drifted_model_module_fails_stale(self):
        """STALE rule: any model-module change invalidates the bundle."""
        with tempfile.TemporaryDirectory() as td:
            out = Path(td) / "bundle"
            build_bundle(out, only=["dir-base"])
            manifest = _load(out / "manifest.json")
            covered = manifest["covered"]
            mod = "src/gf180_dx7/model/poly.py"
            covered["model_identity"]["modules"][mod] = "0" * 64
            manifest["bundle_sha256"] = bundle_hash(covered)
            drift = Path(td) / "drift"
            drift.mkdir()
            (drift / "manifest.json").write_text(json.dumps(manifest))
            shutil.copytree(out / "vectors", drift / "vectors")
            code, rows = verify_release(drift, rerender=False)
            self.assertEqual(code, 1)
            stale = [r for r in rows if r["status"] == "FAIL"
                     and r["check"] == "model_identity"]
            self.assertTrue(stale and "STALE" in stale[0]["detail"])


class TestDeterminism(unittest.TestCase):
    def test_build_twice_byte_identical(self):
        with tempfile.TemporaryDirectory() as td:
            outs = []
            for i in range(2):
                out = Path(td) / f"run{i}"
                build_bundle(out, only=FAST_CASES)
                outs.append(out)
            files = sorted(p.relative_to(outs[0])
                           for p in outs[0].rglob("*") if p.is_file())
            for rel in files:
                self.assertEqual(
                    (outs[0] / rel).read_bytes(), (outs[1] / rel).read_bytes(),
                    f"non-deterministic output: {rel}")


@unittest.skipUnless(MANIFEST.is_file(),
                     "reference/release/manifest.json not committed")
class TestMutationBattery(unittest.TestCase):
    def test_committed_results_bound_and_all_detected(self):
        self.assertTrue(MUTATION_RESULTS.is_file(),
                        "mutation-results.json not committed")
        record = _load(MUTATION_RESULTS)
        manifest = _load(MANIFEST)
        self.assertEqual(record["bundle_sha256"],
                         manifest["bundle_sha256"])
        self.assertTrue(record["all_detected"])
        committed = {m["id"] for m in MUTATIONS}
        for row in record["results"]:
            self.assertIn(row["id"], committed)
            self.assertTrue(row["detected"], row["id"])
            self.assertEqual(row["verify_exit"], 1, row["id"])
            self.assertTrue(row["mismatching_cases"], row["id"])

    def test_every_battery_mutation_is_committed(self):
        record = _load(MUTATION_RESULTS)
        self.assertEqual({m["id"] for m in MUTATIONS},
                         {r["id"] for r in record["results"]})

    def test_fast_mutations_detected_live(self):
        """Live negative controls: the fast seams must demonstrably fail
        the release verification right now, naming the cases."""
        results, problems = run_battery(RELEASE, FAST_MUTATIONS)
        self.assertEqual(problems, [])
        self.assertEqual(len(results), len(FAST_MUTATIONS))
        for row in results:
            self.assertTrue(row["detected"], row["id"])
            self.assertEqual(sorted(row["mismatching_cases"]),
                             sorted(row["cases_rendered"]), row["id"])


@unittest.skipUnless(CHALLENGE.is_file(),
                     "challenge-results.json not committed")
class TestChallengeRecord(unittest.TestCase):
    def setUp(self):
        self.results = _load(CHALLENGE)
        self.manifest = _load(MANIFEST)
        self.spec = _load(SPEC_PATH)

    def test_bound_to_bundle_and_frozen_thresholds(self):
        self.assertEqual(self.results["bundle_sha256"],
                         self.manifest["bundle_sha256"])
        self.assertTrue(self.results["thresholds_frozen_before_challenge"])
        self.assertEqual(self.results["thresholds_source"],
                         self.manifest["covered"]["spec"])

    def test_protocol_ordering_documented(self):
        note = self.results["protocol_note"]
        self.assertIn("BEFORE any challenge render was inspected", note)
        self.assertIn("frozen", note)
        self.assertIn("never", note)

    def test_every_preregistered_case_present_and_exact(self):
        challenge_ids = [e["id"] for e in
                         self.spec["challenge_set"]["registry_cases"]]
        by_id = {c["id"]: c for c in self.results["cases"]}
        self.assertEqual(sorted(by_id), sorted(challenge_ids))
        for case_id in challenge_ids:
            row = by_id[case_id]
            self.assertEqual(row["verdict"], "AGREE_EXACT",
                             f"{case_id}: {row.get('detail', '')}")
            self.assertEqual(row["model_pcm_sha256"],
                             row["frozen_vector_sha256"], case_id)
            self.assertTrue(row["oracle_pcm_sha256"], case_id)

    def test_summary_matches_rows(self):
        rows = self.results["cases"]
        summary = self.results["summary"]
        self.assertEqual(summary["challenge_cases"], len(rows))
        self.assertEqual(
            summary["agree_exact"],
            sum(1 for r in rows if r["verdict"] == "AGREE_EXACT"))
        self.assertEqual(summary["model_mismatch"], 0)


@unittest.skipUnless(MANIFEST.is_file(),
                     "reference/release/manifest.json not committed")
class TestExclusions(unittest.TestCase):
    def test_known_exclusions_recorded_with_reasons(self):
        spec = _load(SPEC_PATH)
        manifest = _load(MANIFEST)
        excluded = {e["id"]: e["reason"]
                    for e in manifest["covered"]["excluded_cases"]}
        self.assertEqual(excluded, {e["id"]: e["reason"]
                                    for e in spec["excluded_cases"]})
        for case_id in ("lfo-keysync0", "ev-steal-17", "ev-aftertouch",
                        "ev-volume-cc7", "ev-patch-change-held"):
            self.assertIn(case_id, excluded)
            self.assertTrue(excluded[case_id].strip(), case_id)

    def test_excluded_cases_absent_from_bundle(self):
        manifest = _load(MANIFEST)
        bundled = {c["id"] for c in manifest["covered"]["cases"]}
        for case_id in ("lfo-keysync0", "ev-steal-17", "ev-aftertouch",
                        "ev-volume-cc7", "ev-patch-change-held"):
            self.assertNotIn(case_id, bundled)


if __name__ == "__main__":
    unittest.main()
