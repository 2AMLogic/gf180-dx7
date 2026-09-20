# Reference manifest area

- R01: pinned Dexed reference (`manifest.json` + `tools/verify_reference.py`),
  see issue #6.
- R02: external oracle pin (`oracle-identity.json`), smoke fixture
  (`fixtures/smoke/`), render evidence (`evidence/smoke/`), and the harness
  `tools/render_reference.py`, see issue #7. The oracle itself lives outside
  this repository (GPL-3.0-or-later); renders are comparison evidence only.
