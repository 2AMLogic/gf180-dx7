# Artifact manifests

Bulky evidence from new runs (sim outputs, synthesis transcripts) is stored in
the fleet artifact store, not git. Git keeps one manifest per run here:
`<bench>-<run-id>.manifest.json`, naming each object by sha256 and an
`artifact:<key>` URI (`klt artifact`, schema in klayout-tools
`docs/cli/artifact.md`).

- **Evidence bytes are private, manifests are public.** Never write a broker
  URL, bucket name, account id, storage URL or cloud resource identifier here.
- **Write** with `tools/store_evidence.py <bench> <run-id> <staging-dir>
  --summary <md>` on a fleet worker (stage files under the git-ignored
  `evidence-staging/`). It fails closed without `$KLT_ARTIFACT_BROKER`.
- **Check** with `tools/check_artifact_manifests.py check` (runs in
  `tools/check_all.sh` and CI): schema only, **not** hash-verified.
- **Verify** with `tools/check_artifact_manifests.py verify` (or `klt artifact
  verify <manifest>`) on a worker with broker access. Without a broker it is
  NOT_RUN, never a pass.
- The "bytes also committed" control in `check` is **name-only**: it compares
  manifest entry names to git-tracked paths, not content hashes, so a copy
  committed under a different path is not caught.
- Manifests are **append-only**: a new run gets a new run-id; do not edit or
  rewrite one. No history rewrite.
- Evidence already committed under `evidence/` stays where it is; existing
  producers and hash-pinned consumers are unchanged.
