"""Shared clean-instance enumeration over the committed P02 catalog (#285).

One owner for the five-field voice-instance record and the rule that
expands a P02 archive catalog into the ordered stream of eligible voice
instances: standalone decoded anomaly-free voice entries plus bank slot
rows with anomaly 0 and a resolvable hash index, sorted by
(path, standalone-first then slot, catalog order). Previously duplicated
by tools/select_dev32.py and tools/u02_candidates.py; a catalog-format or
eligibility fix now lands once. The selectors keep their own validators
and decoders -- only the clean enumeration is shared.

Stdlib only, so it can be imported from a sibling script or via a
`sys.path` entry for `tools/` (same convention as sha256_helpers).
"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class Instance:
    """One voice instance from the catalog (standalone file or bank slot)."""

    path: str
    slot: int | None  # None for a standalone single-voice file
    canonical_hash: str
    name: str
    order: int

    @property
    def sort_key(self) -> tuple[str, int, int]:
        return (self.path, -1 if self.slot is None else self.slot, self.order)


def clean_instances(catalog: dict) -> list[Instance]:
    """All decoded, anomaly-free voice instances in catalog order.

    Standalone `voice` entries are eligible when decoded, anomaly-free
    (a == 0) and carrying a canonical hash; decoded `bank` slots are
    eligible per slot row when the row's anomaly count is 0 and its hash
    index resolves (>= 0). The result is sorted by each instance's
    sort_key: zip path, then bank slot with standalone voices first,
    then catalog order.
    """
    hashes: list[str] = catalog["hashes"]
    names: list[str] = catalog["names"]
    out: list[Instance] = []
    for order, e in enumerate(catalog["entries"]):
        if e["c"] == "voice" and e["v"] == "decoded" and e["a"] == 0 \
                and e["h"] is not None:
            out.append(Instance(e["p"], None, e["h"], e["n"], order))
        elif e["c"] == "bank" and e["v"] == "decoded":
            w = e["w"]
            for k in range(0, len(w), 4):
                slot, hidx, nidx, nanom = w[k:k + 4]
                if nanom == 0 and hidx >= 0:
                    out.append(Instance(e["p"], slot, hashes[hidx],
                                        names[nidx], order))
    out.sort(key=lambda inst: inst.sort_key)
    return out
