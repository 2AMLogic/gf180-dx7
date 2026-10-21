#!/usr/bin/env bash
# Fetch the DX7_AllTheWeb.zip archive into .cache/ and verify its sha256.
#
# The catalog (corpus/archive-catalogs/alltheweb-catalog.json) records the
# archive's sha256 but not where it was downloaded from, so the source URL is
# supplied by the caller: ALLTHEWEB_URL=<url> tools/fetch_alltheweb.sh
# (or pass the URL as $1).  The archive is never committed.  Override the
# destination with DX7_ALLTHEWEB_ZIP (the variable the tools read).
set -euo pipefail

SHA256="c2c47bd1aa29ab07bdf5e6ec8a25faa1b10e06ea0bcf57f6a83d8363051ed6ef"
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="${DX7_ALLTHEWEB_ZIP:-$REPO_ROOT/.cache/DX7_AllTheWeb.zip}"
URL="${1:-${ALLTHEWEB_URL:-}}"

verify() {
    echo "$SHA256  $1" | sha256sum --check --status
}

if [ -f "$DEST" ] && verify "$DEST"; then
    echo "fetch_alltheweb: OK (already present): $DEST"
    exit 0
fi
if [ -z "$URL" ]; then
    echo "fetch_alltheweb: BLOCKED: no source URL (the catalog records the" >&2
    echo "  sha256 only). Set ALLTHEWEB_URL or pass the URL as \$1." >&2
    exit 2
fi
mkdir -p "$(dirname "$DEST")"
TMP="$DEST.part"
curl -fsSL -o "$TMP" "$URL"
if ! verify "$TMP"; then
    echo "fetch_alltheweb: FAIL: sha256 of download != $SHA256" >&2
    rm -f "$TMP"
    exit 1
fi
mv "$TMP" "$DEST"
echo "fetch_alltheweb: OK: $DEST"
