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


def derive_status(dr_text: str) -> str:
    """Read the leading disposition of the Status bullet, including continuations."""
    fields = re.findall(r"^- \*\*Status:\*\*(.*(?:\n[ \t]+.*)*)", dr_text, re.MULTILINE)
    if len(fields) != 1 or not fields[0].split():
        raise ValueError("DR-0008 must have exactly one nonempty Status bullet")
    disposition = fields[0].split()[0].lower()
    return "ratified" if disposition in {"accepted", "ratified"} else disposition


def render_view(contract_bytes: bytes, dr_text: str) -> bytes:
    view = {
        "status": derive_status(dr_text),
        "contract_sha256": sha256_bytes(contract_bytes),
        "rows": json.loads(contract_bytes)["budget_worst_case"]["rows"],
    }
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
