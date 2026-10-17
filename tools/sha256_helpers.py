"""Shared SHA-256 helpers (issue #141).

One owner for the chunked-read file digest that used to be copied into
every tool and test. Stdlib only, so it can be imported from a sibling
script or via a `sys.path` entry for `tools/`.
"""

import hashlib

CHUNK_SIZE = 1 << 20


def sha256_file(path) -> str:
    """Hex SHA-256 of the file at `path`, read in CHUNK_SIZE blocks."""
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(CHUNK_SIZE), b""):
            h.update(chunk)
    return h.hexdigest()


def sha256_bytes(data: bytes) -> str:
    """Hex SHA-256 of an in-memory byte string."""
    return hashlib.sha256(data).hexdigest()
