# Reuse records

One JSON record per adopted component, created BEFORE the import lands
(issue #39). Schema:

```json
{
  "component": "name",
  "upstream": {"repo": "2AMLogic/x", "commit": "sha", "path": ["a", "b"]},
  "sha256": {"a": "…"},
  "license": "Apache-2.0",
  "destination": "rtl/… or src/…",
  "adaptations": ["…"],
  "maintainer": "name",
  "requalification": {"check": "command", "result": "PASS"},
  "negative_control": "command that must fail when the component breaks"
}
```

`tools/check_reuse.py` fails when a file under a governed destination has no
matching record (negative control for A01).

Locally authored governed files are recorded under `local_originals` with a
byte pin (`sha256`): an exemption alone would pass any file once listed, so
originals stay inside the drift-detection coverage set — editing a pinned
original fails the check until the catalog is updated in the same PR
(judge follow-up on PR #44).

## Resolving pin conflicts (Doctor / Judge recipe)

Pins in `docs/reuse/catalog.json` (`local_originals[].sha256`,
`adapted_sha256`) and the `coverage` map of
`evidence/capabilities/rtl-core.json` are *derived* values: when two PRs touch
the same tool both sides are right for their own tree and neither for the
merged one, so the only correct resolution is to recompute.

1. Resolve the code hunks in `tools/` by hand as usual.
2. For `evidence/capabilities/rtl-core.json` take either side
   (`git checkout --ours|--theirs <file>`). Leave `docs/reuse/catalog.json`
   **conflicted**: `tools/check_reuse.py --union-merge` reads the markers (and
   `HEAD` / `MERGE_HEAD`) to keep new entries from both sides, and reports
   `no conflict` without doing anything once they are gone, so never
   `checkout --ours|--theirs` the catalog.
3. Run `python3 tools/repin.py --stamp`. It rewrites only the drifted
   `sha256` digests in the catalog (never `reason`), lists stale `rtl-core`
   coverage entries, then runs
   `tools/compile_capabilities.py --stamp rtl-core`.
4. Commit both files.

The stamp costs about 2 minutes (it re-runs the H-probe benches and the
mutant negative controls); that cost is intrinsic and is not removed.
`tools/repin.py` without `--stamp` is only the catalog half. A fast-lane
result, or `--stamp` returning non-PASS, is **not** a stamp; `repin.py`
propagates the non-zero exit and never reports PASS in that case.
