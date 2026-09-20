# Archive catalog policy (P02, issue #9)

Scope: how `tools/catalog_archive.py` classifies archive entries, defines the
canonical hash, handles duplicate aliases, and what may be redistributed.

## Input pinning

An archive is hashed (SHA-256), sized, and entry-counted *before* it is
treated as an input; these identity fields are recorded in the catalog
(`archive` block) together with the run command, tool hash, and codec hash.
If the pinned archive is unavailable or unhashable the run fails loudly; a
different archive is never substituted silently. No archive bytes are
committed to this repository.

## Classification rules

Every zip entry is classified exactly once, in archive order; nothing is
dropped. Entry classes (`c`) and verdicts (`v`):

| class | verdict | meaning |
|---|---|---|
| `voice` | `decoded` | one DX7 single-voice bulk dump (163 bytes) |
| `bank` | `decoded` | one DX7 32-voice bank bulk dump (4104 bytes) |
| `stream` | `decoded` | 2+ framed messages, every message decoded |
| `unsupported` | `rejected` | framed, but not a supported DX7 form (non-Yamaha ID, non-bulk sub-status, unknown format byte) |
| `malformed` | `rejected` | framing, length, checksum, zip-read, or mixed-stream failure |
| `other` | `skipped` | non-`.syx` file (never decoded) or directory entry |

Reason codes (`r`): `non_syx`, `directory`, `bad_framing`, `bad_length`,
`bad_checksum`, `unsupported_format`, `zip_read_error`; `rd` carries the
classified codec message (truncated). A multi-message file in which any
message fails is classified `malformed` with the failing reason and the count
of messages decoded before failure; its decoded voices are not expanded.

Classification is content-based via the P01 codec (`src/gf180_dx7/sysex.py`),
never from filenames; the `.syx` extension only decides whether a file is a
decoding candidate.

## Canonical hash

For every successfully decoded voice, `canonical_hash` = SHA-256 over the
155-byte unpacked (VCED) voice body with the 10 display-name bytes
(unpacked offsets 145–154, the last 10 bytes of the body) zeroed. Voices
decoded from the packed bank form (128-byte VMEM chunks) are converted to
the unpacked body by re-encoding the decoded parameters with the P01 codec
before hashing, so a patch hashes identically whether it came from a
single-voice dump or from inside a bank. Excluded: SysEx framing, byte-count
header, channel nibble, checksum, and the display name. Out-of-range values
are hashed as decoded — never clamped or repaired. The hash is an identity
for alias detection only; it is not a "sound hash" and establishes nothing
about how a patch sounds.

Aliases are compared by the decoded name (raw 10 bytes mapped to printable
ASCII, trailing spaces significant — codec name policy, issue #8).

## Alias policy

Voices are grouped by canonical hash (across standalone voices, bank slots,
and streams). A group is an *alias group* when its members carry 2+ distinct
names; per group, `aliases = distinct_names - 1` and the recorded canonical
id is the hash. Aliases are **listed, never deleted or merged**; every entry
and every bank slot keeps its source path and slot (`w` rows:
`[slot, hash_idx, name_idx, anomaly_count]` for banks, `[msg, sub, hash_idx,
name_idx, anomaly_count]` for streams, `sub = -1` for a plain voice message;
`hash_idx = -1` marks a hash that could not be computed, counted as
`canonical_failures`).

## Reconciliation contract

Before the catalog is written, every counter is recomputed independently
from the emitted entry rows and must match: `total_entries ==
len(entries) == sum(by_class) == files + directories`, per-class and
per-reason sums, and the voice-instance totals (`standalone == by_class.voice`,
`in_banks == 32 * by_class.bank`). Any mismatch aborts the run with a
nonzero exit and no output; a catalog that dropped one entry cannot pass
(negative control, tested in `tests/test_catalog_archive.py`).

## Rights policy

Catalogs contain metadata only: paths, sizes, classes, names, hashes,
counts. The committed catalog carries the rights statement:
"redistribution rights not established; catalog metadata only, no audio or
bank bytes redistributed". No audio, bank, or voice bytes from any archive
are committed to this repository.
