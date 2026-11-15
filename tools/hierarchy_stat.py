"""Shared hierarchy-total Yosys `stat -liberty` transcript parser (issue #124).

Used by h07_synth.py, h08_synth.py and h10_synth.py. Each caller keeps its
own public `parse_stat(log)` and its own `CheckFailure`/`RunFailure`, and
passes its issue tag (#82 for H07, #94 for H08, #32 for H10) for the
diagnostics; this module only owns the hierarchy-total parsing that used
to be copied twice (h10_synth.py's local copy was folded in by issue #211).

THE FLOP PREDICATE (decided, issue #211 -- not merged by accident):
`flop_total` counts cells whose name contains "dff": the D-flip-flop
family, including the sdff scan variants, by substring against gf180mcu
cell names. Latches ("latq") and integrated clock gates ("icgt") are NOT
flops; they are reported as separate labelled counts (`latch_total`,
`clockgate_total`), never folded into the flop total. The pre-#211
h10_synth.py copy instead counted r"__(dff|sdff|latq|icgt)", so the same
mapped design carried two flop definitions depending on which tool parsed
it -- every committed transcript happens to map to dffq_* cells only, so
the two rules agreed on all committed evidence and no committed number
changes (verified at consolidation; regression-pinned by
tests/test_hierarchy_stat.py).

`attribution=True` additionally returns H10's per-module fields
(`module_local_area_um2`, `top_submodule_instances`, `hierarchy_tree`);
the default keeps the dict to the whole-design totals only, which is all
H07/H08 consume.

Separate from tools/single_module_stat.py on purpose: that contract
REJECTS hierarchical transcripts, this one REQUIRES them (or a genuinely
empty design).
"""

import re

HIER_MARK = "=== design hierarchy ==="
CELL_LINE = r"^\s+(\d+)\s+(\S+)\s+(gf180mcu\S+)\s*$"
HIER_BASIS = "design hierarchy section (includes submodules)"
EMPTY_BASIS = "empty design (no mapped cells in the transcript)"
TREE_LINE = r"^\s+(\d+)\s+([\d.E+]+)\s+(\w+)\s*$"


def _parse_empty_stat(log, failure, issue):
    """The one legitimate hierarchy-total-free transcript: a design that
    mapped to NOTHING (the H07_STRIP_OBSERVABILITY control -- yosys
    deletes every cell, so no `Chip area` line is printed at all).
    Accepted only when the transcript really is cell-free; anything else
    raises rather than reporting a per-module number as the total."""
    if re.search(r"Chip area for module", log):
        raise failure(
            "stat transcript has per-module 'Chip area for module' blocks "
            "but no 'Chip area for top module' line: the hierarchical "
            "total is absent (truncated or non-hierarchical stat). "
            "Refusing to substitute a per-module area as the design total "
            f"(issue {issue}).")
    stray = re.findall(CELL_LINE, log, re.M)
    if stray:
        raise failure(
            f"stat transcript reports {len(stray)} mapped cell line(s) but "
            "no hierarchical total and no chip-area line -- refusing to "
            f"report an unanchored count (issue {issue}).")
    return {
        "cells_by_name": {},
        "cell_total": 0,
        "dff_cells": {},
        "flop_total": 0,
        "chip_area_um2": None,
        "seq_area_um2": None,
        "totals_basis": EMPTY_BASIS,
    }


def _parse_attribution(log, hier_start, sect, failure, issue):
    """H10's attribution fields (opt-in, issue #211): per-module local
    areas, the top's submodule multiplicities, and the hierarchy tree
    at the top of the section ("count area module" rows between the
    first and second `+----------Count` block headers). Refuses rather
    than silently returning an empty tree when the headers are absent."""
    if sect.count("+----------Count") < 2:
        raise failure(
            "attribution requested but the hierarchy section lacks the "
            "two '+----------Count' block headers that bound the "
            f"hierarchy tree (issue {issue}).")
    tree = {}
    body = (sect.split("+----------Count including submodules.")[1]
            .split("+----------Count")[0])
    for m in re.finditer(TREE_LINE, body, re.M):
        tree[m.group(3)] = {"count_incl_sub": int(m.group(1)),
                            "area_incl_sub_um2_3sf": float(m.group(2))}
    # submodule multiplicities of the top (the "submodules" block)
    subs = {}
    sm = re.search(r"^\s+\d+\s+\S+\s+submodules\s*$(.*?)(?:^\s*$)", sect,
                   re.M | re.S)
    if sm:
        for m in re.finditer(r"^\s+(\d+)\s+\S+\s+(\w+)\s*$", sm.group(1),
                             re.M):
            subs[m.group(2)] = int(m.group(1))
    # per-module local areas (from the per-module blocks preceding it)
    local = {}
    for m in re.finditer(r"Chip area for module '\\?([\w$]+)': ([\d.]+)",
                         log[:hier_start]):
        local[m.group(1)] = float(m.group(2))
    return {
        "module_local_area_um2": dict(sorted(local.items())),
        "top_submodule_instances": dict(sorted(subs.items())),
        "hierarchy_tree": tree,
    }


def parse_hierarchy_stat(log, failure, issue, attribution=False):
    """Whole-design mapped cell/area facts from a hierarchical
    `stat -liberty` transcript.

    yosys prints one LOCAL block per module (`=== <module> ===`, "Chip
    area for module '\\<module>'") and then, for a multi-module design, a
    final `=== design hierarchy ===` section whose counts INCLUDE
    submodules, closed by `Chip area for top module '\\<top>'`. Only that
    last section describes the design as a whole. This parser:

      - anchors on `Chip area for top module` and takes the LAST match
        (a non-final intermediate per-module block of the top can exist
        earlier in the same transcript and is NOT the total);
      - bounds cell/flop counting to the hierarchy section preceding it,
        so the per-module blocks cannot be added into the totals;
      - raises `failure` (tagged `issue`) rather than falling back to any
        per-module number when the hierarchy totals are absent.

    Flops are counted with the one decided predicate (see module
    docstring); latches and clock gates come back as separate labelled
    counts. With `attribution=True` the dict additionally carries H10's
    per-module fields.
    """
    top_m = list(re.finditer(
        r"Chip area for top module '\\?([\w$]+)': ([\d.]+)", log))
    if not top_m:
        return _parse_empty_stat(log, failure, issue)
    top = top_m[-1]
    start = log.rfind(HIER_MARK, 0, top.start())
    if start < 0:
        raise failure(
            f"'Chip area for top module' found but no '{HIER_MARK}' "
            "section precedes it -- refusing to report per-module numbers "
            f"as the design total (issue {issue}).")
    sect = log[start:top.end()]
    total = re.search(r"^\s+(\d+)\s+\S+\s+cells\s*$", sect, re.M)
    if not total:
        raise failure(
            "no hierarchical 'cells' total line inside the "
            f"'{HIER_MARK}' section (issue {issue}).")
    cells = {}
    for m in re.finditer(CELL_LINE, sect, re.M):
        # assignment, not accumulation: within the bounded hierarchy
        # section each cell type appears exactly once, already summed
        # over submodules by yosys.
        cells[m.group(3)] = int(m.group(1))
    seq = re.search(r"of which used for sequential elements: ([\d.]+)",
                    log[top.end():top.end() + 400])
    latch_cells = {k: v for k, v in sorted(cells.items()) if "latq" in k}
    icg_cells = {k: v for k, v in sorted(cells.items()) if "icgt" in k}
    out = {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)),
        "dff_cells": {k: v for k, v in sorted(cells.items()) if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "latch_cells": latch_cells,
        "latch_total": sum(latch_cells.values()),
        "clockgate_cells": icg_cells,
        "clockgate_total": sum(icg_cells.values()),
        "chip_area_um2": float(top.group(2)),
        "seq_area_um2": float(seq.group(1)) if seq else None,
        "top_module": top.group(1),
        "totals_basis": HIER_BASIS,
    }
    if attribution:
        out.update(_parse_attribution(log, start, sect, failure, issue))
    return out
