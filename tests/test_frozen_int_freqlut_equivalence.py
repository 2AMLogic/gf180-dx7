"""Issue #208 (operator ruling 2026-11-23, option C): pin the two live
Freqlut implementations and the two LUT generators to each other.

The frozen integer model carries two independent implementations of msfa
freqlut.cc:46-55 (Q24 logfreq -> 32-bit phase increment):

- ``operator.Freqlut`` — instance-based, LUT injected via the public
  constructor / ``from_artifact`` (the pinned, SHA-256-checked bytes),
  explicit out-of-domain ``hibits > 20`` guard. Reachable only from
  ``tests/test_operator.py`` (the N02 independent-oracle probes) and this
  module.
- ``pitch.Freqlut`` — class-global ``_lut`` regenerated in float64 at
  runtime, unguarded shift mirroring the C. This is the LIVE copy:
  ``integrated.py``, ``pitch.py`` and the H06/H07 RTL harnesses consume it.

The two agreed when measured (issue body: 200k in-domain pairs, 0
mismatches; Hermit follow-up: 20k negative logfreq plus -2**31, 0
divergences) but nothing asserted the agreement, so a drift in either
would silently break claim (1) — "the RTL matches the frozen integer
model exactly". The operator ruled option C: keep both (the divergence
in out-of-domain behaviour — declared guard vs. mirrored unguarded shift
— is deliberate; the comments at both class definitions are deferred to
#303, so this docstring is the interim record) and pin them with this
live equivalence test. No ``src/`` logic may depend on
this module.

Seeded sample (recorded per the acceptance criteria; smaller than the
issue's 200k measurement but covering the same input classes, so the
fast lane stays within the DR-0009 seconds budget):

- 4,000 random low-24-bit values at every ``hibits`` 0..20 -> 84,000
- 20,000 random negative ``logfreq`` (``-2**28`` domain)
- edges: 0, ``hibits=20`` plain, ``hibits=20 | 0xFFFFFF``, ``-(2**31)``

Total: 104,004 pairs per run, seed 208.

Stdlib only.
"""

from __future__ import annotations

import random
import sys
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
_SRC = REPO / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

from gf180_dx7.model import operator as op  # noqa: E402
from gf180_dx7.model import pitch as P  # noqa: E402
from gf180_dx7.model import tables as tmod  # noqa: E402

TABLES_DIR = REPO / "reference" / "tables"
SAMPLE_RATE = 48000

# reference/tables/manifest.json "freqlut_table" pin, also quoted at
# tools/h06_compare.py:25 and :799.
PINNED_FREQLUT_SHA256 = (
    "6d3c597008e85c67cfa1165ef4038a3c1c9c56a60a4b441b7486296a897dc64d")

SEED = 208
LOWBITS_PER_HIBITS = 4000
NEGATIVE_SAMPLE = 20000
EDGES = (0, 20 << 24, (20 << 24) | 0xFFFFFF, -(1 << 31))


def _fmt(logfreq: int) -> str:
    """Distinctive message so a control failure provably names its case."""
    return "logfreq=%d (0x%08x)" % (logfreq, logfreq & 0xFFFFFFFF)


class TestFreqlutEquivalence(unittest.TestCase):
    """The option-C pin: generators, constants, lookups, out-of-domain."""

    def setUp(self):
        # pitch.Freqlut._lut is class-global; restore whatever a previous
        # suite left behind so this module cannot affect other tests.
        self._saved_lut = P.Freqlut._lut
        P.Freqlut.init(SAMPLE_RATE)

    def tearDown(self):
        P.Freqlut._lut = self._saved_lut

    def _sample(self) -> list[int]:
        r = random.Random(SEED)
        lfs = []
        for hibits in range(21):
            for _ in range(LOWBITS_PER_HIBITS):
                lfs.append((hibits << 24) | r.randrange(1 << 24))
        lfs.extend(-r.getrandbits(28) for _ in range(NEGATIVE_SAMPLE))
        lfs.extend(EDGES)
        return lfs

    def _assert_equivalent(self, of, logfreqs) -> None:
        """The live check: the shared assertion the control must defeat.
        ``of`` is an ``operator.Freqlut`` instance, so the caller decides
        how it was built (artifact factory vs. perturbed constructor)."""
        for lf in logfreqs:
            self.assertEqual(of.lookup(lf), P.Freqlut.lookup(lf), _fmt(lf))

    def test_generators_agree_and_match_pinned(self):
        """tables.freqlut_table(48000) == pitch.Freqlut.init(48000) LUT
        (all 1025 entries), and both equal the pinned, manifest-hashed
        artifact bytes (load_pinned re-verifies every SHA-256)."""
        art = tmod.generate_all(SAMPLE_RATE)["freqlut_table"]
        self.assertEqual(art.sha256, PINNED_FREQLUT_SHA256)
        self.assertEqual(list(tmod.freqlut_table(SAMPLE_RATE)),
                         P.Freqlut.lut())
        self.assertEqual(list(art.values), P.Freqlut.lut())
        pinned = tmod.load_pinned(TABLES_DIR)["freqlut_table"]
        self.assertEqual(list(pinned.values), P.Freqlut.lut())

    def test_constants_agree(self):
        """The four FREQLUT_* constants in tables.py are restated as
        pitch.Freqlut class attributes; they must not drift apart."""
        self.assertEqual(tmod.FREQLUT_LG_N_SAMPLES, P.Freqlut.LG_N_SAMPLES)
        self.assertEqual(tmod.FREQLUT_N_SAMPLES, P.Freqlut.N_SAMPLES)
        self.assertEqual(tmod.FREQLUT_SAMPLE_SHIFT, P.Freqlut.SAMPLE_SHIFT)
        self.assertEqual(tmod.FREQLUT_MAX_LOGFREQ_INT,
                         P.Freqlut.MAX_LOGFREQ_INT)

    def test_lookup_equivalence_seeded(self):
        """operator.Freqlut (from the generated artifact) and the live
        pitch.Freqlut agree over the seeded sample: every hibits 0..20,
        negatives (exercising the arithmetic-shift path and the _i32
        wrap on the operator side), and both top-of-domain edges."""
        art = tmod.generate_all(SAMPLE_RATE)["freqlut_table"]
        self.assertEqual(art.sha256, PINNED_FREQLUT_SHA256)
        self._assert_equivalent(op.Freqlut.from_artifact(art),
                                self._sample())

    def test_out_of_domain_hibits_21_both_raise(self):
        """Above the pinned contract the two diverge ON PURPOSE:
        operator raises its declared guard, pitch mirrors the
        reference's unguarded shift (interpreter "negative shift
        count"). Same exception class; messages and intent differ by
        design (do not assert equal messages)."""
        art = tmod.generate_all(SAMPLE_RATE)["freqlut_table"]
        of = op.Freqlut.from_artifact(art)
        lf = 21 << 24
        with self.assertRaises(ValueError):
            of.lookup(lf)
        with self.assertRaises(ValueError):
            P.Freqlut.lookup(lf)

    def test_control_one_entry_perturbation_fails_and_names_case(self):
        """Live negative control: a one-entry LUT perturbation on the
        operator side must make the equivalence assertion FAIL, and the
        failure must name the failing logfreq. The same probes pass on
        the unperturbed LUT, proving the perturbation is what fails."""
        art = tmod.generate_all(SAMPLE_RATE)["freqlut_table"]
        # hibits=20 (no final shift), ix=100, lowbits=0 -> lookup is
        # exactly lut[100], so a +1 perturbation must be visible.
        probes = [20 << 24 | (100 << 14), 20 << 24 | (100 << 14) | 1]
        # control baseline: the unperturbed artifact factory passes
        self._assert_equivalent(op.Freqlut.from_artifact(art), probes)
        bad = list(art.values)
        bad[100] += 1
        with self.assertRaises(AssertionError) as cm:
            self._assert_equivalent(op.Freqlut(bad), probes)
        self.assertIn(_fmt(probes[0]), str(cm.exception),
                      "equivalence failure did not name the failing case")


if __name__ == "__main__":
    unittest.main()
