#!/bin/sh
# Repo-check entry point (DR-0002, issue #39): run every tools/check_*.py.
# Exits nonzero on the first failing check. Stdlib-only, no network.
set -eu
cd "$(dirname "$0")/.."

found=0
for check in tools/check_*.py; do
    [ -e "$check" ] || continue
    found=1
    echo "== python3 $check"
    python3 "$check"
done

if [ "$found" -eq 0 ]; then
    echo "no tools/check_*.py found" >&2
    exit 2
fi

# R01 (issue #6): verify the pinned Dexed reference when the local clone is
# present. Where the clone is absent the check is NOT_RUN (guarded skip),
# never reported as a pass, so CI without the clone still passes.
DEXED_ROOT="${DEXED_ROOT:-/Users/joseph/dev/dexed}"
if [ -d "$DEXED_ROOT" ]; then
    echo "== python3 tools/verify_reference.py --dexed-root $DEXED_ROOT"
    python3 tools/verify_reference.py --dexed-root "$DEXED_ROOT"
else
    echo "== tools/verify_reference.py NOT_RUN (dexed clone unavailable at $DEXED_ROOT; guarded skip)"
fi

# R02 (issue #7): fast oracle self-check — smoke-fixture decode via the P01
# codec plus oracle-identity schema/hash check. Deliberately NOT the full
# render: CI has no oracle binary, and render_selfcheck.py guarded-skips
# (NOT_RUN) the binary hash check when the oracle is absent.
echo "== python3 tools/render_selfcheck.py"
python3 tools/render_selfcheck.py

# N01 (issue #15): numeric/scheduling profile — schedule + storage arithmetic
# recomputed, decision estimates present, event timing vs contract-v1, and
# citation hashes/line content against the pinned trees (guarded skip per
# tree when a pinned clone is absent).
echo "== python3 tools/check_numeric_profile.py"
python3 tools/check_numeric_profile.py

# H03 (issue #25): core interface/schedule contract — worst-case budget
# recomputed from the measured H01/H02/N08 anchors (hash-gated), verdicts
# enforced (12.288 MHz FAIL / 24.576 MHz PASS), DEC citations checked.
echo "== python3 tools/h03_budget_check.py"
python3 tools/h03_budget_check.py

echo "all repo checks passed"
