#!/usr/bin/env python3
"""compile_capabilities.py — evidence-derived capability views (C01, issue #45).

Stdlib-only, offline. Evaluates the claim DAG (spec/capabilities-v1.json)
against evidence records (evidence/capabilities/<node>.json) and regenerates:

  - docs/CAPABILITIES.md   (table + mermaid + per-node details)
  - docs/capabilities.json (machine-readable view)
  - README.md block        (<!-- CAPABILITIES:BEGIN/END -->, placed right
                            after the ISSUEDAG block)

Modes:

  (default)          regenerate stale views
  --check            verify all three views agree with a fresh evaluation;
                     executes nothing (CI staleness guard)
  --strict           --check plus: every node must be PASS. The future
                     release gate; honestly red while nodes are NO_VERDICT
  --stamp NODE       producer path: run NODE's REGISTERED check command and
                     its required negative controls for real, then write
                     evidence/capabilities/NODE.json with fresh coverage
                     hashes and regenerate the views. Refuses anything not
                     registered in src/gf180_dx7/capabilities.py CHECKS.

Evaluation never executes evidence commands; --stamp is the only mode that
runs anything. Exit codes: 0 ok, 1 check/refusal failure, 2 usage error.
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import capabilities as cap  # noqa: E402

RESULT_BY_EXIT = {0: cap.PASS, 2: cap.NOT_RUN}


def _result_for_exit(exit_code: int) -> str:
    return RESULT_BY_EXIT.get(exit_code, cap.FAIL)


def _paths(root: Path) -> dict[str, Path]:
    return {
        "docs": root / "docs" / "CAPABILITIES.md",
        "json": root / "docs" / "capabilities.json",
        "readme": root / "README.md",
        "graph": root / "spec" / "capabilities-v1.json",
    }


def _run(argv: list[str], cwd: Path) -> int:
    print(f"[stamp] $ {' '.join(argv)}", flush=True)
    completed = subprocess.run(argv, cwd=str(cwd))
    return completed.returncode


def _write_evidence(node: dict, root: Path, recorded_by: str) -> cap.Result:
    """Run the registered check + required controls; write the record."""
    check = cap.CHECKS.get(node["check"]) if node["check"] else None
    if check is None:
        raise cap.CapabilityError(
            f"{node['id']}: check {node['check']!r} is not registered; stamping"
            " is refused (registry is a code-review boundary)"
        )
    controls = []
    for control in node["required_negative_controls"]:
        exit_code = _run(list(control["command"]), root)
        controls.append(
            {
                "command": list(control["command"]),
                "detected": exit_code == 0,
                "exit_code": exit_code if exit_code >= 0 else None,
            }
        )
        if exit_code != 0:
            print(
                f"[stamp] WARNING control {control['id']} exited"
                f" {exit_code}: recorded as not detected",
                file=sys.stderr,
            )
    exit_code = _run(list(check.command), root)
    coverage = cap.coverage_hashes(node, root)
    record = {
        "schema": cap.SCHEMA,
        "node_id": node["id"],
        "check": node["check"],
        "argv": list(check.command),
        "result": _result_for_exit(exit_code),
        "exit_code": exit_code,
        "recorded_at": datetime.now(timezone.utc).strftime(cap.RECORD_TIMESTAMP),
        "recorded_by": recorded_by,
        "coverage": coverage,
        "controls": controls,
    }
    target = cap._path(root, cap.node_evidence_path(node["id"]))
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes((json.dumps(record, indent=2, sort_keys=True) + "\n").encode("utf-8"))
    print(f"[stamp] wrote {target.relative_to(root)}: result={record['result']}")
    return cap._local(node, root)


def build_views(nodes: list[dict], results: dict, paths: dict[str, Path]) -> dict[Path, bytes]:
    markdown = cap.render_markdown(nodes, results)
    block = cap.render_readme_block(nodes, results)
    views = {
        paths["docs"]: markdown.encode("utf-8"),
        paths["json"]: cap.render_json(nodes, results).encode("utf-8"),
        paths["readme"]: cap.replace_readme_block(
            paths["readme"].read_text(encoding="utf-8"), block
        ).encode("utf-8"),
    }
    for label, payload in (("docs/CAPABILITIES.md", markdown), ("README block", block)):
        errors = cap.lint_mermaid(payload)
        if errors:
            raise cap.CapabilityError(
                f"mermaid lint failed for {label}: " + "; ".join(errors)
            )
    return views


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", type=Path, default=REPO_ROOT,
                        help="repository root (default: this checkout)")
    parser.add_argument("--check", action="store_true",
                        help="verify views agree; write and execute nothing")
    parser.add_argument("--strict", action="store_true",
                        help="--check plus every node must be PASS (future release gate)")
    parser.add_argument("--stamp", metavar="NODE", default=None,
                        help="run the registered check + controls for NODE and write its evidence record")
    parser.add_argument("--recorded-by", default="tools/compile_capabilities.py --stamp",
                        help="producer note recorded in the evidence record")
    args = parser.parse_args()
    if args.stamp and (args.check or args.strict):
        parser.error("--stamp cannot be combined with --check/--strict")

    root = args.root.resolve()
    paths = _paths(root)
    try:
        nodes = cap.load_graph(paths["graph"])
        stamped_state = None
        if args.stamp:
            matching = [n for n in nodes if n["id"] == args.stamp]
            if not matching:
                print(f"ERROR: unknown node: {args.stamp}", file=sys.stderr)
                return 2
            result = _write_evidence(matching[0], root, args.recorded_by)
            stamped_state = result.state
            print(f"[stamp] {args.stamp} evaluates: {result.state}")
            if result.state != cap.PASS:
                for reason in result.reasons:
                    print(f"  - {reason}", file=sys.stderr)

        results = cap.evaluate(nodes, root)
        expected = build_views(nodes, results, paths)
        stale = [
            path for path, payload in expected.items()
            if not path.is_file() or path.read_bytes() != payload
        ]
        if args.check or args.strict:
            for path in stale:
                print(
                    f"ERROR: generated view differs: {path.relative_to(root)};"
                    " regenerate with tools/compile_capabilities.py",
                    file=sys.stderr,
                )
            if stale:
                return 1
        else:
            for path, payload in expected.items():
                path.parent.mkdir(parents=True, exist_ok=True)
                if not path.is_file() or path.read_bytes() != payload:
                    path.write_bytes(payload)

        counts = ", ".join(
            f"{state}={count}"
            for state, count in sorted(
                Counter(r.state for r in results.values()).items()
            )
        )
        print(f"Capability states: {counts}")
        if args.strict:
            unpassed = sorted(
                node_id for node_id, result in results.items()
                if result.state != cap.PASS
            )
            if unpassed:
                print(
                    "ERROR: --strict requires every node PASS; not PASS: "
                    + ", ".join(unpassed),
                    file=sys.stderr,
                )
                return 1
            print("--strict: every node PASS; views fresh.")
        else:
            print(
                "Generated capability views."
                if not (args.check or args.strict)
                else "Generated-view agreement checked."
            )
        if stamped_state is not None and stamped_state != cap.PASS:
            print(
                f"ERROR: stamped node did not reach PASS: {stamped_state}",
                file=sys.stderr,
            )
            return 1
        return 0
    except (cap.CapabilityError, OSError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
