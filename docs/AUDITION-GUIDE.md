# Audition & recall guide (U01, issue #34)

`tools/audition.py` is the software audition loop: select an archived sound
by stable identity, render a fixed phrase set through the pinned external
oracle, record a favorite, and later prove the favorite still reproduces
byte-identically. It exists so musical judgment enters the loop early,
while the numeric engine work proceeds — without ever claiming that
agreement or audition establishes original-DX7 fidelity or preset quality.

## The fixed phrase set

`spec/audition-phrases-v1.json` is committed and never improvised per
candidate. It implements the plan's audition protocol (plan section 6):
low/mid/high notes (36/60/84), three velocities (32/80/127), a short pluck,
a long hold, a release tail, a three-note chord, and the applicable
controllers (mod-wheel CC1 sweep, pitch bend). The tool refuses to run
against a phrase file missing any of these. Rationale:

- A quiet sound at middle C or at one velocity is not automatically broken;
  velocity and register phrases expose sensitivity curves and key scaling.
- Short pluck / long hold / release tail separate attack, sustain and
  release-envelope behavior.
- The chord exercises voice allocation; controllers expose LFO routing and
  bend handling.
- Events are oracle `events.txt` lines (`<pos> note_on <pitch> <vel>`,
  `note_off`, `cc <controller> <value>`, `pb <lsb> <msb>`), positions in
  absolute samples at 48 kHz, applied before the 64-sample block containing
  them (see `reference/oracle-identity.json`).

## Usage

```sh
# discovery (names are not identities)
python3 tools/audition.py select --name ELECTNIC            # by name fragment
python3 tools/audition.py select --hash <64-hex>            # by identity

# render every phrase for a sound (writes <out>/<phrase_id>/render.{f32,wav,json})
python3 tools/audition.py render --hash <64-hex> --out /tmp/listen

# render a single phrase
python3 tools/audition.py render --hash <64-hex> --phrase long-hold-60 --out /tmp/listen

# record a favorite (renders all phrases, stores per-phrase sha256)
python3 tools/audition.py favorite --hash <64-hex> --note "bright tine, thin bass"

# later: prove the favorite still reproduces byte-identically
python3 tools/audition.py recall --hash <64-hex>
```

Global flags: `--catalog` (default
`corpus/archive-catalogs/alltheweb-catalog.json`), `--phrases` (default
`spec/audition-phrases-v1.json`), `--store` (default
`corpus/favorites.json`), `--identity` (default
`reference/oracle-identity.json`), `--zip` (path to the pinned
`DX7_AllTheWeb.zip`). Exit codes: `0` PASS, `1` FAIL (mismatch, corruption,
identity failure), `2` NOT_RUN (missing oracle/zip/catalog).

`select` output rows are JSON: `{"path", "bank", "slot", "message", "name",
"canonical_hash"}` — a standalone voice has `bank`/`slot` null, a bank
voice names its containing file and slot, a stream voice adds the message
index.

## Dry rule

Comparison renders are dry. The tool records peak levels; it never changes
gain, never normalizes per candidate, never time-warps, and never swaps
reference engines per patch (AGENTS.md). A render that is silent while its
phrase strikes a note is rejected (silent-output guard, same policy as
R02), because a broken audition must fail, not play quiet.

## Integrity rules

1. **Identity, not names.** Selection for rendering/recall is by canonical
   hash (P02: sha256 over decoded parameters, display name excluded). Names
   are search sugar only; two aliases of one parameter set are one sound.
2. **Pinned oracle.** Every render re-hashes the external GPL oracle binary
   against `reference/oracle-identity.json` and refuses to run on mismatch.
   A pin is never relaxed. The oracle is software-reference agreement
   machinery only; it is never copied into this repository.
3. **Pinned archive.** The zip is hash-checked against the catalog's
   archive pin; the extracted voice is re-decoded and re-hashed against the
   selected identity before rendering. Catalog/archive drift fails loudly
   instead of rendering the wrong sound.
4. **Schema-validated store.** `corpus/favorites.json`
   (`gf180-dx7.favorites/1`) stores, per canonical hash: `note`, `added`
   (UTC timestamp), `phrase_hashes` (phrase id → output sha256). Any bad
   JSON, short/non-hex key, missing field, unknown phrase id, or malformed
   hash is STORE CORRUPTION: recall exits 1 loudly rather than playing the
   wrong sound.
5. **Recall = re-render + hash equality.** A favorite passes recall only if
   every phrase re-renders byte-identically to the recorded hash. Any
   difference fails with the per-phrase expected/actual hashes. This is the
   issue's negative control: corrupt the store and recall must fail, never
   play.

## Claim hygiene

Audition renders are oracle renders. They establish that a sound can be
selected, reproduced and heard repeatably. They do not establish RTL-vs-
model exactness, model-vs-reference budgets, hardware playback, or that a
favorite is musically good; listening records (human judgments) are the
only evidence for the last claim.
