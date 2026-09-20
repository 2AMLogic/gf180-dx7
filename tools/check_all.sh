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

echo "all repo checks passed"
