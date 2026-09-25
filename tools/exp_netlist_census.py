#!/usr/bin/env python3
"""Issue #86: census of the exp() unit's registers in a mapped dx7_core netlist.

The pinned ORFS flow names every mapped flip-flop after its RTL register
(`renames -wire`, image flow/scripts/synth.tcl): e.g. `\\exp_st$_DFF_P_`.
This counts, per exp() register, the distinct mapped flop instances whose
name starts with that register, and reports the synth_stat chip area.  If
the exp datapath registers (exp_y .. exp_pt_q) are all absent while the FSM
(exp_st) survives, synthesis has constant-folded the exp() result.

Usage: exp_netlist_census.py NETLIST.v [--stat synth_stat.txt] [--label L]
Stdlib only.  Exit 0 always when the files could be read (this is a census,
the verdict is drawn in docs/EXP-RANGE-PROOF-86.md); 2 if unreadable.
"""

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

REGS = ("exp_st", "exp_start", "exp_amd", "exp_ams", "exp_y", "exp_t1",
        "exp_arg", "exp_zq", "exp_k", "exp_T", "exp_xq", "exp_p2", "exp_p3",
        "exp_p4", "exp_p5", "exp_res", "exp_pt_q", "w_li")
DATAPATH = ("exp_y", "exp_t1", "exp_arg", "exp_zq", "exp_k", "exp_T", "exp_xq",
            "exp_p2", "exp_p3", "exp_p4", "exp_p5", "exp_res", "exp_pt_q")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("netlist", type=Path)
    ap.add_argument("--stat", type=Path, default=None)
    ap.add_argument("--label", default="")
    a = ap.parse_args()
    try:
        data = a.netlist.read_bytes()
    except OSError as e:
        print(json.dumps({"status": "NOT_RUN", "reason": str(e)}))
        return 2
    text = data.decode(errors="replace")
    # Instance names: `<cell> \<reg>[<i>]$_<TYPE>_ (` or `\<reg>$_<TYPE>_ (`.
    inst = re.findall(r"\\([A-Za-z_]\w*)(?:\[\d+\])?\$_[A-Z0-9_]+_\s*\(", text)
    counts = {r: 0 for r in REGS}
    for name in inst:
        if name in counts:
            counts[name] += 1
    out = {"label": a.label, "netlist_sha256": hashlib.sha256(data).hexdigest(),
           "netlist_bytes": len(data), "mapped_flops_named_after": counts,
           "exp_datapath_flops_total": sum(counts[r] for r in DATAPATH),
           "all_flop_instances": len(inst)}
    if a.stat:
        m = re.search(r"Chip area for module '\\\\?dx7_core': ([0-9.]+)", a.stat.read_text())
        out["chip_area_um2"] = float(m.group(1)) if m else None
    print(json.dumps(out, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
