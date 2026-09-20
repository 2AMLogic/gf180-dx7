# Reference manifest area

- R01: pinned Dexed reference (`manifest.json` + `tools/verify_reference.py`),
  see issue #6.
- R02: external oracle pin (`oracle-identity.json`), smoke fixture
  (`fixtures/smoke/`), render evidence (`evidence/smoke/`), and the harness
  `tools/render_reference.py`, see issue #7. The oracle itself lives outside
  this repository (GPL-3.0-or-later); renders are comparison evidence only.
- R05: non-invasive trace adapter — versioned tap registry
  (`trace-registry.json`), capture gate (`tools/capture_trace.py`), and the
  smoke sample trace (`traces/smoke.jsonl` + `traces/smoke-meta.json`), see
  issue #12 and `docs/TRACE-PLAN.md`. Traced and untraced renders must be
  byte-identical before any trace artifact is written; the unavailable
  operator-level taps are documented in the registry, never omitted.
