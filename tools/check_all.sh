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
echo "all repo checks passed"
