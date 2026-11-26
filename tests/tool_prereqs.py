"""Shared Icarus and Yosys prerequisite checks for the RTL test suites.

The H01, H04, H05 and H06 suites gate the same live cases behind the same
two tool checks, so each check lives here once (issue #354): a change to a
prerequisite diagnostic reaches every suite that reports it. A check
resolves what it finds and returns the reason the suites give when
something is missing -- a live case that cannot run is a guarded skip
naming every missing prerequisite (NOT_RUN), never a silent pass. The
liberty is located through tools/pdk_resolve.py, the one PDK resolver.
Stdlib only.
"""

import os
import shutil
import sys

_TOOLS_DIR = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "tools")
if _TOOLS_DIR not in sys.path:
    sys.path.insert(0, _TOOLS_DIR)
import pdk_resolve  # noqa: E402


def _skip_reason(missing):
    """The suites' NOT_RUN text naming ``missing``; '' when nothing is."""
    return ("NOT_RUN (guarded skip): " + "; ".join(missing)) if missing else ""


def icarus():
    """(iverilog, vvp, reason): the resolved Icarus binaries, None where
    one is missing, and the guarded-skip reason naming each that is."""
    iverilog = shutil.which("iverilog")
    vvp = shutil.which("vvp")
    missing = []
    if iverilog is None:
        missing.append("iverilog not on PATH")
    if vvp is None:
        missing.append("vvp not on PATH")
    return iverilog, vvp, _skip_reason(missing)


def yosys():
    """(yosys, liberty, reason): the resolved Yosys binary (None when
    missing), the pinned gf180mcuD 7t5v0 tt liberty it needs, and the
    guarded-skip reason naming each missing prerequisite."""
    found = shutil.which("yosys")
    root = pdk_resolve.find_pdk_root() or pdk_resolve.NO_PDK_PLACEHOLDER
    liberty = os.path.join(root, pdk_resolve.SC_LIB_REL)
    missing = []
    if found is None:
        missing.append("yosys binary not on PATH")
    if not os.path.isfile(liberty):
        missing.append(f"ciel 7t liberty not found: "
                       f"{pdk_resolve.missing_reason() or liberty}")
    return found, liberty, _skip_reason(missing)
