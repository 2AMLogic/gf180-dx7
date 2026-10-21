#!/usr/bin/env bash
# Fetch the pinned Dexed reference tree (R01) into .oracle/dexed.
#
# Dexed (GPL-3.0-or-later in part) is an external comparison oracle only; it
# is cloned into a git-ignored directory and never copied into the repo.
# Override the destination with DEXED_ROOT (the same variable the checks read).
set -euo pipefail

PIN="b6cf2e9190e216387ad96f977284ebbef765b447"
URL="https://github.com/asb2m10/dexed"
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="${DEXED_ROOT:-$REPO_ROOT/.oracle/dexed}"

if [ ! -d "$DEST/.git" ]; then
    mkdir -p "$(dirname "$DEST")"
    git clone --no-checkout "$URL" "$DEST"
fi
git -C "$DEST" fetch --quiet origin "$PIN" 2>/dev/null || git -C "$DEST" fetch --quiet origin
git -C "$DEST" checkout --quiet --detach "$PIN"

# verify_reference.py hashes files under libs/ (submodules at the pinned tree).
git -C "$DEST" submodule update --init --recursive --quiet

HEAD="$(git -C "$DEST" rev-parse HEAD)"
if [ "$HEAD" != "$PIN" ]; then
    echo "fetch_dexed: FAIL: HEAD $HEAD != pinned $PIN" >&2
    exit 1
fi
echo "fetch_dexed: OK: $DEST @ $HEAD"
