#!/usr/bin/env python3
"""Generate the signoff item 6 view of the DR-0008 core contract."""

import argparse
import json
import re
import sys
from pathlib import Path

from sha256_helpers import sha256_bytes

ROOT = Path(__file__).resolve().parents[1]
CONTRACT = ROOT / "spec/core-contract-v1.json"
DECISION = ROOT / "docs/decision-records/0008-core-schedule-contract.md"
VIEW = ROOT / "spec/core-contract-v1.signoff.json"

STATUS_MARKER = "- **Status:**"
# DR-0008 owns the scope and verdicts paired with its ratified status.
ITEM6_DISPOSITION = re.compile(
    r"^### Item 6 disposition\n\n```json\n(.*?)^```$", re.MULTILINE | re.DOTALL
)
# These Markdown blocks can end a metadata paragraph without a blank line.
BLOCK_START = re.compile(
    r"(?:[-*+]|[0-9]{1,9}[.)]|#{1,6})(?:[ \t]|$)"  # list or heading
    r"|>|~{3,}|`{3,}[^`]*$"  # quote or fenced code
    r"|(?P<rule>[-*_])(?:[ \t]*(?P=rule)){2,}[ \t]*$"  # thematic break
)


def derive_status(dr_text: str) -> str:
    """Read the leading disposition of the Status bullet, including continuations."""
    lines = dr_text.splitlines()
    starts = [i for i, line in enumerate(lines) if line.startswith(STATUS_MARKER)]
    if len(starts) != 1:
        raise ValueError("DR-0008 must have exactly one nonempty Status bullet")
    start = starts[0]
    item = [lines[start]]
    for line in lines[start + 1:]:
        # The "- " marker sets the content indent to two columns. A
        # paragraph can continue lazily at column one until a block starts.
        if line.strip(" \t") and not line.expandtabs(4).startswith("  "):
            if not item[-1].strip(" \t") or BLOCK_START.match(line.lstrip(" ")):
                break
        item.append(line)
    bullet = "\n".join(item).removeprefix(STATUS_MARKER)
    # The disposition must sit on the marker line: prose on a continuation
    # line is qualification, never the disposition itself.
    words = item[0].removeprefix(STATUS_MARKER).split()
    if not words:
        raise ValueError("DR-0008 must have exactly one nonempty Status bullet")
    disposition = words[0].lower()
    if disposition not in {"accepted", "ratified"}:
        return disposition
    if re.search(r"by[\s-]+default", bullet, re.IGNORECASE):
        return "accepted-by-default"
    return "ratified"


def render_view(contract_bytes: bytes, dr_text: str) -> bytes:
    view = {"status": derive_status(dr_text)}
    if view["status"] == "ratified":
        blocks = ITEM6_DISPOSITION.findall(dr_text)
        if len(blocks) != 1:
            raise ValueError("ratified DR-0008 requires one Item 6 disposition block")
        disposition = json.loads(blocks[0])
        view["ratification_scope"] = disposition["ratification_scope"]
        view["schedule_assessment"] = disposition["schedule_assessment"]
    view["contract_sha256"] = sha256_bytes(contract_bytes)
    view["rows"] = json.loads(contract_bytes)["budget_worst_case"]["rows"]
    return (json.dumps(view, indent=2) + "\n").encode("utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check", action="store_true", help="check the committed bytes"
    )
    args = parser.parse_args()
    expected = render_view(CONTRACT.read_bytes(), DECISION.read_text(encoding="utf-8"))
    if args.check:
        if not VIEW.is_file() or VIEW.read_bytes() != expected:
            print(
                f"{VIEW.name} is missing or stale. Run tools/gen_item6_view.py.",
                file=sys.stderr,
            )
            return 1
    else:
        VIEW.write_bytes(expected)
    return 0


if __name__ == "__main__":
    sys.exit(main())
