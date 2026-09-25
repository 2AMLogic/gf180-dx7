# Curated 128-patch bank manifest policy (U03, issue #36)

Scope: the schema of the U03 curated bank manifest (`gf180-dx7.bank128/1`)
and the executable check `tools/validate_bank128.py`. This document defines
what a valid manifest must contain. It does not contain or choose any
patches.

## Status

- **Validator and schema: present** (this document, `tools/validate_bank128.py`,
  `tests/test_bank128.py`).
- **Bank content: not present.** `corpus/bank128.json` does not exist. It must
  be built from U02 listening records (issue #35, operator-only). Until then,
  `python3 tools/validate_bank128.py` exits 2 with `NOT_RUN`. That is not a
  pass.
- The tests use only synthetic fixtures generated in code (`FIXTURE-000`…,
  hashes of fixed strings, backed by a synthetic four-bank catalog). They
  cannot validate against the committed P02 catalog, and a test checks that
  they don't. They show nothing about any real patch or about sound quality.

## Manifest shape

```jsonc
{
  "schema": "gf180-dx7.bank128/1",
  "revision": 1,                        // positive int; bumped per bank revision,
                                        // independent of RTL milestones
  "catalog": {
    "snapshot_sha256": "<sha256 of corpus/archive-catalogs/alltheweb-catalog.json>",
    "archive": { /* copy of the catalog's `archive` identity block */ }
  },
  "coverage": {                         // must equal the validator's recount
    "by_category": {"ep-tine": n, ...},
    "by_distribution_status": {"redistributable": n, "restricted": n}
  },
  "exclusions": [                       // wanted patches left out, each with a reason
    {"name": "...", "canonical_hash": "<hex or null>", "reason": "..."}
  ],
  "entries": [                          // exactly 128, index 0..127 in order
    {
      "index": 0,
      "name": "display name",
      "canonical_hash": "<64 hex, P02 canonical hash>",
      "category": "ep-tine",            // U02 family key
      "source": {
        "kind": "archive",              // or "original"
        "authorship": "author, or 'unknown' stated explicitly",
        "catalog_path": "DX7_AllTheWeb/...",   // archive: resolves in the catalog
        "catalog_slot": 12                      // bank slot, or null for a single voice
        // original: "record": "<path to committed provenance record>"
      },
      "audition": {
        "notes": "listening notes from the U01 phrase set",
        "listening_record": "<reference to the U02/U03 listening record>",
        "verdict": "selected"
      },
      "performance": {
        "poly_mono": "poly",            // poly | mono
        "pitch_bend_range": 2,          // 0..12
        "pitch_bend_step": 0,           // 0..12
        "portamento_mode": "retain",    // retain | follow | fingered | full-time
        "portamento_glissando": false,
        "portamento_time": 0,           // 0..99
        "mod_wheel":      {"range": 50, "assign": ["pitch"]},
        "foot_control":   {"range": 0,  "assign": []},
        "breath_control": {"range": 0,  "assign": []},
        "aftertouch":     {"range": 0,  "assign": []}
        // assign is a subset of pitch | amplitude | eg_bias
      },
      "distribution": {
        "status": "redistributable",    // redistributable | restricted
        "basis": "why this status is established (license, permission, ...)"
      },
      // only when this entry has the same canonical hash as an earlier one:
      "duplicate_of": 3,
      "alias_note": "why this parameter-identical entry is kept"
    }
  ]
}
```

The performance block uses the DX7 function-parameter set. It records how a
patch is meant to be played. It does not claim the chip supports every
setting (for example mono mode). That is decided by the product contract.

## Validation rules

| rule | failure |
|---|---|
| schema id, positive `revision` | FAIL |
| exactly 128 entries, indices 0..127 | FAIL |
| well-formed canonical hash per entry | FAIL (unhashable) |
| `source` present with kind and authorship; archive sources resolve at their own (path, slot) in the committed catalog with the same hash; original sources name a provenance record | FAIL (unprovenanced) |
| audition notes, listening-record reference, verdict `selected` | FAIL |
| complete, in-range performance settings | FAIL |
| distribution status is `redistributable` or `restricted`, and a basis is given | FAIL. Unknown or unclear status must go in `exclusions` with a reason (issue stop/escalate rule) |
| entries with the same canonical hash: each later one declares `duplicate_of` (index of the first) and an `alias_note` | FAIL (issue negative control) |
| `duplicate_of` points at an entry with the same hash | FAIL (false alias) |
| every U02 family has at least one entry; recorded coverage equals the recount | FAIL |
| every exclusion has a name and a reason | FAIL |
| catalog snapshot hash and archive identity match | STALE |

Exit codes: `0` PASS, `1` FAIL or STALE, `2` NOT_RUN (manifest or catalog
missing). `--report PATH` writes a JSON validation log with tool, manifest
and catalog hashes, problems, and the duplicate groups. Each duplicate group
lists every name the catalog records for that hash (catalog aliases), so
reviewers can check them.

## What this does not establish

- **Near-duplicates.** The canonical hash (P02) catches exact parameter
  duplicates and renamed aliases. It cannot see two patches that differ by
  one parameter. Catching those is a listening-record judgment. The
  validator does not score "filler".
- **Variety beyond family coverage.** The validator checks that every family
  is present and that coverage is counted honestly. It sets no per-family
  quota.
- **Sound quality or fidelity.** A passing manifest is complete and
  provenanced. It does not show that any patch sounds good (only listening
  records do), and Dexed renders are not original-DX7 fidelity.
- **Distribution rights.** The validator checks that a status and a basis
  are recorded. It cannot check that the basis is true. The P02 archive
  catalog's own rights statement is "redistribution rights not established",
  so an archive patch needs its own basis before it can be marked
  `redistributable`.
