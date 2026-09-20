#!/usr/bin/env python3
"""R06 registry gate for tools/check_all.sh (issue #13).

Runs the coverage validator in --check mode (schema validity, P01 field
reflection coverage, 32/32 algorithms, feedback cases for algorithms 4 and
6, well-formed events, holdout seal). Exit codes pass through: 0 = no
holes, 1 = invalid registry or coverage holes, 2 = registry unreadable.

Negative-control resolution is proven by tools/registry_coverage.py
--selftest-negative (invoked by tests/test_registry.py). Stdlib-only.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
COVERAGE = REPO_ROOT / "tools" / "registry_coverage.py"

EXIT_LABELS = {0: "PASS (no coverage holes)",
               1: "FAIL (invalid registry or coverage holes)",
               2: "NOT_RUN (registry unreadable)"}


def main() -> int:
    proc = subprocess.run([sys.executable, str(COVERAGE), "--check"],
                          capture_output=True, text=True)
    sys.stdout.write(proc.stdout)
    sys.stderr.write(proc.stderr)
    label = EXIT_LABELS.get(proc.returncode, f"FAIL (exit {proc.returncode})")
    print(f"check_registry: {label}")
    return proc.returncode


if __name__ == "__main__":
    sys.exit(main())
