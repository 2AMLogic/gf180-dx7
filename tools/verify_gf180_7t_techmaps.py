#!/usr/bin/env python3
"""Verify the gf180mcu 7t techmaps committed at asic/orfs/gf180_7t/.

Two independent checks, both recorded by DR-0010 and required by the reuse
governance ("hash equality alone never qualifies"):

1. LIB CHECK -- every `gf180mcu_fd_sc_mcu7t5v0__*` cell referenced by the
   techmaps exists in the 7t tt liberty (the PDK reference library). A
   techmap that names a cell the library does not have would produce
   unmapped/undriven nets in synthesis; this check makes that impossible at
   commit time rather than as a confusing yosys failure.

2. TRANSFORM CHECK (needs --image-dir) -- each committed file is exactly a
   leading block of `//` comment lines followed, byte-for-byte, by the
   pinned ORFS image platform file with the single documented substitution
   `mcu9t5v0 -> mcu7t5v0`. This pins the derivation to the image (the
   attribution the prior `reject` ruling required) and catches any drift
   between the image source and the committed bytes.

Exit codes: 0 all checks pass; 1 a check failed (violations printed);
2 the liberty file could not be located (lib check skipped); 3 usage error.
Stdlib only. The negative control is exercised hermetically by
tests/test_h10_techmaps.py (fake image + fake liberty).
"""

import argparse
import os
import re
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TECHMAP_DIR = os.path.join(REPO_ROOT, "pnr", "orfs", "gf180_7t")
TECHMAPS = ("cells_adders.v", "cells_latch.v")
IMAGE_FILES = ("cells_adders.v", "cells_latch.v")
SUBSTITUTION = ("mcu9t5v0", "mcu7t5v0")
CELL_RE = re.compile(r"gf180mcu_fd_sc_mcu7t5v0__[A-Za-z0-9_]+")
LIB_CELLS_RE = re.compile(r"cell\s*\(\s*([A-Za-z0-9_]+)\s*\)")

DEFAULT_LIB_LOCATIONS = (
    os.environ.get("GF180MCU_7T_TT_LIB", ""),
    "/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/gf180mcu/versions/"
    "54435919abffb937387ec956209f9cf5fd2dfbee/gf180mcuD/libs.ref/"
    "gf180mcu_fd_sc_mcu7t5v0/lib/gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib",
)


def split_header(path):
    """Returns (header_lines, body_text) for a committed techmap."""
    data = open(path).read().splitlines(keepends=True)
    i = 0
    while i < len(data) and data[i].startswith("//"):
        i += 1
    if i == 0:
        raise SystemExit(f"3: {path} has no leading provenance header")
    return i, "".join(data[i:])


def lib_cells(lib_path):
    return {m.group(1) for m in LIB_CELLS_RE.finditer(open(lib_path).read())}


def check_lib(techmap_dir, lib_path, violations):
    cells = lib_cells(lib_path)
    if not cells:
        violations.append(
            "liberty parsed to ZERO cells -- the parse failed, every "
            "reference would be reported missing, and a vacuous pass is "
            "refused")
        return
    for name in TECHMAPS:
        path = os.path.join(techmap_dir, name)
        for ref in sorted(set(CELL_RE.findall(open(path).read()))):
            if ref not in cells:
                violations.append(
                    f"{name}: referenced cell {ref} is NOT in the 7t tt liberty")


def check_transform(techmap_dir, image_dir, violations):
    for name in TECHMAPS:
        committed = os.path.join(techmap_dir, name)
        image = os.path.join(image_dir, name)
        if not os.path.isfile(image):
            violations.append(f"transform: image file missing: {image}")
            continue
        _, body = split_header(committed)
        expected = open(image).read().replace(*SUBSTITUTION)
        if body.rstrip("\n") != expected.rstrip("\n"):
            violations.append(
                f"transform: {name} body is not the image file under "
                f"'{SUBSTITUTION[0]} -> {SUBSTITUTION[1]}' (byte drift or "
                "unrecorded edit)")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--techmap-dir", default=TECHMAP_DIR)
    ap.add_argument("--lib", default=None,
                    help="7t tt liberty; default: $GF180MCU_7T_TT_LIB then the "
                         "known local PDK path")
    ap.add_argument("--image-dir", default=None,
                    help="dir containing the pinned image's "
                         "platforms/gf180/cells_*.v (enables the transform "
                         "check)")
    args = ap.parse_args(argv)

    ok = True
    for name in TECHMAPS:
        if not os.path.isfile(os.path.join(args.techmap_dir, name)):
            print(f"FAIL: techmap missing: {args.techmap_dir}/{name}")
            return 3

    lib_path = args.lib or next((p for p in DEFAULT_LIB_LOCATIONS if p
                                 and os.path.isfile(p)), None)
    violations = []
    if lib_path:
        check_lib(args.techmap_dir, lib_path, violations)
        print(f"lib check: {lib_path}")
    else:
        print("lib check: SKIPPED (no 7t tt liberty found)")
    if args.image_dir:
        check_transform(args.techmap_dir, args.image_dir, violations)
        print(f"transform check: {args.image_dir}")
    else:
        print("transform check: SKIPPED (no --image-dir)")

    if violations:
        for v in violations:
            print("VIOLATION:", v)
        ok = False
    print("RESULT:", "PASS" if ok else "FAIL")
    return 1 if violations else (2 if not lib_path and not args.image_dir
                                 else 0)


if __name__ == "__main__":
    sys.exit(main())