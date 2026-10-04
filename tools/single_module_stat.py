"""Shared single-module Yosys `stat -liberty` transcript parser (issue #119).

Used by h01/h04/h05/h06_synth.py. Each caller keeps its own public
`parse_stat(log)` and its own `CheckFailure`; this module only owns the
guard and the cell/area parsing that used to be copied four times.

Excluded on purpose: the hierarchical H07/H08 parsers and the storage-probe
parser (their returned fields differ).
"""

import re

HIER_MARK = "=== design hierarchy ==="
MODULE_AREA_RE = r"Chip area for module .*?: ([\d.]+)"
TOP_AREA_RE = r"Chip area for top module"


def parse_single_module_stat(log, module, failure):
    """Mapped cell/area facts from a single-module `stat -liberty` transcript.

    The transcript's ONE `Chip area for module` block is reported as the
    design total. That is correct only while `module` is a flat,
    single-module design (issue #95). If it ever acquires submodules, yosys
    emits one block PER module plus a hierarchy section whose `Chip area for
    top module` line is the whole-design total, and taking a per-module
    block would silently under-report -- exactly the bug #82 (h07) and #94
    (h08) had to fix. A hierarchical or multi-block transcript therefore
    raises `failure` instead of shipping a wrong number; the fix when it
    fires is #94's hierarchy-total parser, never a looser regex or a relaxed
    guard.

    An EMPTY area list is legitimate: the strip-observability control maps
    nothing, so yosys prints no area line (chip_area_um2 stays None).
    """
    if HIER_MARK in log or re.search(TOP_AREA_RE, log):
        raise failure(
            "stat transcript is HIERARCHICAL ('=== design hierarchy ===' "
            "and/or a 'Chip area for top module' line present): "
            f"{module} is no longer a single-module design, so a "
            "per-module 'Chip area for module' block is NOT the whole-design "
            "total. Refusing to report one as the design area "
            "(issues #82/#94/#95) -- parse the hierarchy total instead.")
    areas = re.findall(MODULE_AREA_RE, log)
    if len(areas) > 1:
        raise failure(
            f"stat transcript has {len(areas)} 'Chip area for module' blocks; "
            f"{module} is expected to be a single-module design "
            "(exactly one). Refusing to report the first block as the design "
            "area (issues #82/#94/#95).")
    cells = {}
    for m in re.finditer(r"^\s+(\d+)\s+([\d.]+E\+\d+|[\d.]+)\s+"
                         r"(gf180mcu\S+)\s*$", log, re.M):
        cells[m.group(3)] = cells.get(m.group(3), 0) + int(m.group(1))
    seq = re.search(r"of which used for sequential elements: ([\d.]+)", log)
    total = re.search(r"^\s+(\d+)\s+[\d.]+E\+\d+\s+cells\s*$", log, re.M)
    return {
        "cells_by_name": dict(sorted(cells.items())),
        "cell_total": int(total.group(1)) if total else sum(cells.values()),
        "dff_cells": {k: v for k, v in cells.items() if "dff" in k},
        "flop_total": sum(v for k, v in cells.items() if "dff" in k),
        "chip_area_um2": float(areas[0]) if areas else None,
        "seq_area_um2": float(seq.group(1)) if seq else None,
    }
