"""Shared hierarchy-total Yosys `stat -liberty` transcript parser (issue #124).

Used by h07_synth.py and h08_synth.py. Each caller keeps its own public
`parse_stat(log)` and its own `CheckFailure`, and passes its issue tag
(#82 for H07, #94 for H08) for the diagnostics; this module only owns the
hierarchy-total parsing that used to be copied twice.

Separate from tools/single_module_stat.py on purpose: that contract
REJECTS hierarchical transcripts, this one REQUIRES them (or a genuinely
empty design).
"""

import re

HIER_MARK = "=== design hierarchy ==="
CELL_LINE = r"^\s+(\d+)\s+(\S+)\s+(gf180mcu\S+)\s*$"
HIER_BASIS = "design hierarchy section (includes submodules)"
EMPTY_BASIS = "empty design (no mapped cells in the transcript)"


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


def parse_hierarchy_stat(log, failure, issue):
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
    return {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)),
        "dff_cells": {k: v for k, v in sorted(cells.items()) if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "chip_area_um2": float(top.group(2)),
        "seq_area_um2": float(seq.group(1)) if seq else None,
        "top_module": top.group(1),
        "totals_basis": HIER_BASIS,
    }
