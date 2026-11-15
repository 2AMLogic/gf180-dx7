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
                     hashes, bounded captured output (sha256 + tail) and
                     unittest run/skip counts, and regenerate the views.
                     Refuses anything not registered in
                     src/gf180_dx7/capabilities.py CHECKS. A required
                     control that reported a skip, or a check-level skip
                     under the node's "not_run" skip policy, records
                     NOT_RUN even at exit 0 — never a skip-shaped PASS.
                     A check reports a skip only when its
                     registry-declared skip_token appears in its
                     captured output (issue #232).

Evaluation never executes evidence commands; --stamp is the only mode that
runs anything. Exit codes: 0 ok, 1 check/refusal failure, 2 usage error.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import capabilities as cap  # noqa: E402

RESULT_BY_EXIT = {0: cap.PASS, 2: cap.NOT_RUN}
OUTPUT_TAIL_CHARS = 2000

_RAN_RE = re.compile(r"^Ran (\d+) tests? in", re.MULTILINE)
# unittest reports skips only on its summary line ("OK (skipped=2)" /
# "FAILED (failures=1, skipped=2)"). Anchoring matches _RAN_RE's discipline:
# an unanchored search let a competing "skipped=<n>" anywhere in a control's
# output masquerade as the run's skip count and force NOT_RUN (issue #232).
_SKIPPED_RE = re.compile(r"^(?:OK|FAILED) \(.*?skipped=(\d+)", re.MULTILINE)


def _result_for_exit(exit_code: int) -> str:
    return RESULT_BY_EXIT.get(exit_code, cap.FAIL)


def _run(argv: list[str], cwd: Path) -> tuple[int, str]:
    """Run a registered command, echoing its captured output live.

    Returns (exit_code, combined stdout+stderr) so the stamp can record a
    bounded digest+tail and read the guarded-skip notes the command emitted
    (issue #206). Non-UTF-8 output is replaced, never fatal.
    """
    print(f"[stamp] $ {' '.join(argv)}", flush=True)
    completed = subprocess.run(
        argv, cwd=str(cwd), capture_output=True, text=True, errors="replace"
    )
    text = completed.stdout + completed.stderr
    sys.stdout.write(text)
    sys.stdout.flush()
    return completed.returncode, text


def _bounded_output(text: str) -> dict:
    """Full-output digest plus a bounded tail; diff-stable and reviewable."""
    return {
        "sha256": hashlib.sha256(text.encode("utf-8")).hexdigest(),
        "tail": text[-OUTPUT_TAIL_CHARS:],
    }


def _unittest_counts(text: str) -> tuple[int | None, int | None]:
    """(tests_run, skipped) from unittest's summary lines, else (None, None).

    A completed unittest run that reports no skip marker observed zero
    skipped tests; skipped stays null only when the command was not a
    unittest run at all (no "Ran N tests" line).
    """
    ran = _RAN_RE.search(text)
    skipped = _SKIPPED_RE.search(text)
    tests_run = int(ran.group(1)) if ran else None
    if skipped:
        return tests_run, int(skipped.group(1))
    return tests_run, (0 if tests_run is not None else None)


def _paths(root: Path) -> dict[str, Path]:
    return {
        "docs": root / "docs" / "CAPABILITIES.md",
        "json": root / "docs" / "capabilities.json",
        "readme": root / "README.md",
        "graph": root / "spec" / "capabilities-v1.json",
    }


def _write_evidence(node: dict, root: Path, recorded_by: str) -> cap.Result:
    """Run the registered check + required controls; write the record."""
    check = cap.CHECKS.get(node["check"]) if node["check"] else None
    if check is None:
        raise cap.CapabilityError(
            f"{node['id']}: check {node['check']!r} is not registered; stamping"
            " is refused (registry is a code-review boundary)"
        )
    controls = []
    control_skipped = 0
    for control in node["required_negative_controls"]:
        exit_code, text = _run(list(control["command"]), root)
        tests_run, skipped = _unittest_counts(text)
        if skipped is not None and skipped > 0:
            control_skipped += skipped
        controls.append(
            {
                "command": list(control["command"]),
                "detected": exit_code == 0 and not (skipped is not None and skipped > 0),
                "exit_code": exit_code if exit_code >= 0 else None,
                "output": _bounded_output(text),
                "tests_run": tests_run,
                "skipped": skipped,
            }
        )
        if exit_code != 0:
            print(
                f"[stamp] WARNING control {control['id']} exited"
                f" {exit_code}: recorded as not detected",
                file=sys.stderr,
            )
    exit_code, check_text = _run(list(check.command), root)
    # Guarded-skip detection consults the registry declaration, not a bare
    # substring over the output (issue #232): only a check that declares a
    # skip token can report a skip, and only when its captured output
    # carries that token. Truthiness guards an empty declared token (which
    # would match every output); registration already refuses one.
    check_skip_reported = bool(check.skip_token) and check.skip_token in check_text
    coverage = cap.coverage_hashes(node, root)
    # A guarded skip keeps the command's honest exit code but must not read
    # PASS: a skipped control never fully ran its detection, and a
    # check-level skip forces NOT_RUN when the node declares skip_policy
    # "not_run" (a "pass" node keeps its declared, exclusion-named gap).
    if exit_code == 0 and (
        control_skipped
        or (check_skip_reported and node["skip_policy"] == "not_run")
    ):
        result = cap.NOT_RUN
    else:
        result = _result_for_exit(exit_code)
    record = {
        "schema": cap.SCHEMA,
        "node_id": node["id"],
        "check": node["check"],
        "argv": list(check.command),
        "result": result,
        "exit_code": exit_code,
        "recorded_at": datetime.now(timezone.utc).strftime(cap.RECORD_TIMESTAMP),
        "recorded_by": recorded_by,
        "coverage": coverage,
        "check_output": _bounded_output(check_text),
        "check_skip_reported": check_skip_reported,
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
