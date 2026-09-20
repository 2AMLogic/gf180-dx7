"""Evidence-derived capability DAG (C01, issue #45). Stdlib-only, offline.

Graph declarations in spec/capabilities-v1.json are trusted, reviewable
policy; evidence records under evidence/capabilities/ are untrusted data.
The compiler stamps verdicts ONLY from checks registered in CHECKS (a code
review boundary: declaring a name, command, or an existing file can never
register an evaluator) and fixes the flaws found in the sibling audit
(docs/REUSE-RULINGS.md, negative finding 2):

  1. file existence never counts as evidence — a verdict requires a record
     produced by tools/compile_capabilities.py --stamp for a REGISTERED
     check, carrying content-hash coverage of every input it depends on;
  2. every stamp carries SHA-256 coverage over the current bytes of its
     covered inputs (dirty and untracked included); any byte change is
     STALE;
  3. a node cannot PASS while a required negative control has not been
     recorded as executed AND detected — missing or undetected controls
     yield NO_VERDICT, never PASS;
  4. an unregistered or hand-declared check is refused (NO_VERDICT), and a
     fabricated PASS is rejected because stamping requires the record to
     name the registered command exactly.

Status vocabulary (AGENTS.md): PASS, FAIL, NOT_RUN, BLOCKED, NO_VERDICT,
STALE. Coverage is reported separately from agreement. This module never
executes evidence commands; only the explicit --stamp producer path runs
anything. Records are integrity-checked attestations, not cryptographic
authentication of the producer.
"""

from __future__ import annotations

import hashlib
import json
import re
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path, PurePosixPath

SCHEMA = "gf180-dx7.capability-evidence/1"
VIEW_SCHEMA = "gf180-dx7.capabilities-view/1"
EVIDENCE_DIR = "evidence/capabilities"

PASS = "PASS"
FAIL = "FAIL"
NOT_RUN = "NOT_RUN"
BLOCKED = "BLOCKED"
NO_VERDICT = "NO_VERDICT"
STALE = "STALE"

RECORD_TIMESTAMP = "%Y-%m-%dT%H:%M:%SZ"
_PATH_RE = re.compile(r"[A-Za-z0-9_./-]+")
_SHA256_RE = re.compile(r"[0-9a-f]{64}")


class CapabilityError(ValueError):
    """Invalid graph policy or unusable (refused) evidence."""


class StaleCoverage(CapabilityError):
    """A covered input is missing, unreadable, or its bytes changed."""


@dataclass(frozen=True)
class Check:
    """A registered check command plus the implementation bytes it depends on.

    Registration is the code-review boundary: evidence may only stamp a
    verdict when its check id and exact argv appear here.
    """

    command: tuple[str, ...]
    inputs: tuple[str, ...]


# Registered checks (current-main commands only; registry changes are
# reviewed code changes, never data edits).
CHECKS = {
    "contract-tests-v1": Check(
        ("python3", "-m", "unittest", "tests.test_contract_v1", "-v"),
        ("tests/test_contract_v1.py",),
    ),
    "reference-pin-v1": Check(
        ("python3", "tools/verify_reference.py"),
        ("tools/verify_reference.py", "reference/manifest.json"),
    ),
    "render-selfcheck-v1": Check(
        ("python3", "tools/render_selfcheck.py"),
        ("tools/render_selfcheck.py", "src/gf180_dx7/sysex.py"),
    ),
    "compare-evidence-v1": Check(
        ("python3", "tools/compare_evidence.py"),
        (
            "tools/compare_evidence.py",
            "tools/render_reference.py",
            "src/gf180_dx7/compare.py",
        ),
    ),
    "trace-capture-v1": Check(
        (
            "python3",
            "tools/capture_trace.py",
            "--traces-dir",
            "build/capabilities/traces",
        ),
        ("tools/capture_trace.py", "reference/trace-registry.json"),
    ),
    "comparator-qualification-v1": Check(
        ("python3", "-m", "unittest", "tests.test_compare", "-v"),
        ("tests/test_compare.py",),
    ),
    "dev32-validation-v1": Check(
        (
            "python3",
            "tools/validate_dev32.py",
            "--out",
            "build/capabilities/dev32-renderproof.json",
        ),
        ("tools/validate_dev32.py",),
    ),
}

# The judge's own bytes participate in every node's coverage: changing the
# compiler, the module, or the graph stales every recorded verdict until the
# affected checks are re-stamped. Generated views are deliberately NOT
# covered (they are derived from this coverage; covering them would cycle).
EVALUATOR_INPUTS = (
    "src/gf180_dx7/capabilities.py",
    "tools/compile_capabilities.py",
    "spec/capabilities-v1.json",
)

_NODE_KEYS = {
    "id",
    "claim",
    "check",
    "engine",
    "prereqs",
    "covered_inputs",
    "required_negative_controls",
    "exclusions",
}


def _require(condition: bool, message: str) -> None:
    if not condition:
        raise CapabilityError(message)


def _digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _check_path(relative: str) -> None:
    parts = relative.split("/")
    _require(
        bool(relative)
        and not PurePosixPath(relative).is_absolute()
        and all(part not in ("", ".", "..") for part in parts)
        and _PATH_RE.fullmatch(relative) is not None,
        f"unsafe relative path: {relative!r}",
    )


def _path(root: Path, relative: str) -> Path:
    _check_path(relative)
    root = root.resolve()
    target = root / relative
    _require(
        not any(part.is_symlink() for part in (target, *target.parents[:-1])),
        f"symlink reference refused: {relative}",
    )
    _require(
        target.resolve().is_relative_to(root),
        f"reference escapes repository root: {relative}",
    )
    return target


def node_evidence_path(node_id: str) -> str:
    return f"{EVIDENCE_DIR}/{node_id}.json"


def _ordered(nodes: list[dict]) -> list[dict]:
    """Topological order (sorted, deterministic); cycles are policy errors."""
    indexed = {node["id"]: node for node in nodes}
    done: set[str] = set()
    active: set[str] = set()
    ordered: list[dict] = []

    def visit(node_id: str) -> None:
        _require(node_id not in active, f"dependency cycle at {node_id}")
        if node_id in done:
            return
        active.add(node_id)
        for dep in sorted(indexed[node_id]["prereqs"]):
            visit(dep)
        active.remove(node_id)
        done.add(node_id)
        ordered.append(indexed[node_id])

    for node_id in sorted(indexed):
        visit(node_id)
    return ordered


def validate_graph(graph: dict) -> list[dict]:
    _require(isinstance(graph, dict), "graph must be a JSON object")
    _require(graph.get("schema_version") == 1, "unsupported graph schema")
    nodes = graph.get("nodes")
    _require(isinstance(nodes, list) and bool(nodes), "graph needs nodes")
    indexed: dict[str, dict] = {}
    for node in nodes:
        _require(isinstance(node, dict), "node must be an object")
        _require(set(node) == _NODE_KEYS, f"node fields wrong: {sorted(node)}")
        node_id = node["id"]
        _require(
            isinstance(node_id, str) and re.fullmatch(r"[a-z0-9-]+", node_id),
            f"bad node id: {node_id!r}",
        )
        _require(node_id not in indexed, f"duplicate node id: {node_id}")
        indexed[node_id] = node
        _require(
            isinstance(node["claim"], str) and node["claim"].strip(),
            f"{node_id}: claim required",
        )
        check_id = node["check"]
        engine = node["engine"]
        _require(
            isinstance(node["exclusions"], list)
            and all(isinstance(item, str) and item for item in node["exclusions"]),
            f"{node_id}: exclusions must be non-empty strings",
        )
        _require(
            isinstance(node["covered_inputs"], list)
            and all(
                isinstance(item, str) and item for item in node["covered_inputs"]
            ),
            f"{node_id}: covered_inputs must be path strings",
        )
        for relative in node["covered_inputs"]:
            _check_path(relative)
        if check_id is None:
            _require(
                isinstance(engine, str) and engine.startswith("planned:"),
                f"{node_id}: unregistered nodes declare engine 'planned: ...'",
            )
            _require(
                node["required_negative_controls"] == [],
                f"{node_id}: unregistered nodes declare no controls yet",
            )
        else:
            _require(
                isinstance(check_id, str) and check_id in CHECKS,
                f"{node_id}: check {check_id!r} is not registered",
            )
            registered = CHECKS[check_id]
            _require(
                engine == list(registered.command),
                f"{node_id}: engine must equal the registered command exactly",
            )
        controls = node["required_negative_controls"]
        _require(isinstance(controls, list), f"{node_id}: controls must be a list")
        seen_controls = set()
        for control in controls:
            _require(
                isinstance(control, dict)
                and set(control) == {"id", "detects", "command"},
                f"{node_id}: control fields must be id/detects/command",
            )
            _require(
                isinstance(control["id"], str)
                and control["id"]
                and control["id"] not in seen_controls,
                f"{node_id}: control ids must be unique non-empty strings",
            )
            seen_controls.add(control["id"])
            _require(
                isinstance(control["detects"], str) and control["detects"],
                f"{node_id}: control {control['id']} needs a detects clause",
            )
            _require(
                isinstance(control["command"], list)
                and control["command"]
                and all(isinstance(part, str) for part in control["command"]),
                f"{node_id}: control {control['id']} command must be argv",
            )
    for node in nodes:
        for dep in node["prereqs"]:
            _require(
                dep in indexed and dep != node["id"],
                f"{node['id']}: unknown or self prerequisite {dep!r}",
            )
    return _ordered(nodes)


def load_graph(path: Path) -> list[dict]:
    try:
        graph = json.loads(path.read_bytes())
    except (OSError, ValueError) as error:
        raise CapabilityError(f"unreadable graph {path}: {error}") from error
    return validate_graph(graph)


def coverage_paths(node: dict) -> list[str]:
    """Every byte this node's verdict depends on (node inputs + judge bytes)."""
    check = CHECKS.get(node["check"]) if node["check"] else None
    paths = set(node["covered_inputs"]) | set(EVALUATOR_INPUTS)
    if check is not None:
        paths.update(check.inputs)
    return sorted(paths)


def coverage_hashes(node: dict, root: Path) -> dict[str, str]:
    """Hash current stored bytes (dirty and untracked included)."""
    result: dict[str, str] = {}
    for relative in coverage_paths(node):
        path = _path(root, relative)
        if not path.is_file():
            raise StaleCoverage(f"covered input missing: {relative}")
        try:
            result[relative] = _digest(path.read_bytes())
        except OSError as error:
            raise StaleCoverage(
                f"covered input unreadable: {relative}: {error}"
            ) from error
    return result


def _validate_record(record: object, node: dict, check: Check) -> None:
    _require(isinstance(record, dict), "evidence record must be an object")
    _require(
        record.get("schema") == SCHEMA, "evidence record schema unsupported"
    )
    _require(
        record.get("node_id") == node["id"], "evidence node id mismatch"
    )
    _require(record.get("check") == node["check"], "evidence check id mismatch")
    _require(
        record.get("argv") == list(check.command),
        "recorded argv is not the registered check command",
    )
    result = record.get("result")
    exit_code = record.get("exit_code")
    _require(result in (PASS, FAIL, NOT_RUN), f"unknown recorded result {result!r}")
    _require(
        isinstance(exit_code, int) and not isinstance(exit_code, bool),
        "exit_code must be an integer",
    )
    if result == PASS:
        _require(exit_code == 0, "PASS with nonzero exit_code")
    elif result == NOT_RUN:
        _require(exit_code == 2, "NOT_RUN requires exit code 2")
    else:
        _require(
            exit_code not in (0, 2), "FAIL requires a failing, non-NOT_RUN exit"
        )
    recorded_at = record.get("recorded_at")
    _require(isinstance(recorded_at, str), "recorded_at required")
    try:
        datetime.strptime(recorded_at, RECORD_TIMESTAMP)
    except ValueError as error:
        raise CapabilityError(f"bad recorded_at: {recorded_at!r}") from error
    _require(
        isinstance(record.get("recorded_by"), str) and record["recorded_by"],
        "recorded_by required",
    )
    coverage = record.get("coverage")
    _require(isinstance(coverage, dict) and bool(coverage), "coverage required")
    for relative, digest in coverage.items():
        _check_path(relative)
        _require(
            isinstance(digest, str) and _SHA256_RE.fullmatch(digest) is not None,
            f"bad coverage digest for {relative}",
        )
    controls = record.get("controls")
    _require(isinstance(controls, list), "controls list required")
    for control in controls:
        _require(
            isinstance(control, dict)
            and set(control) == {"command", "detected", "exit_code"},
            "control records need command/detected/exit_code",
        )
        _require(
            isinstance(control["command"], list)
            and all(isinstance(part, str) for part in control["command"]),
            "control command must be argv",
        )
        _require(
            isinstance(control["detected"], bool),
            "control detected must be a boolean",
        )
        _require(
            control["exit_code"] is None
            or (isinstance(control["exit_code"], int)),
            "control exit_code must be an integer or null",
        )


@dataclass(frozen=True)
class Result:
    state: str
    local_state: str
    reasons: tuple[str, ...]
    evidence: str | None
    healthy: bool


def _refuse(node_id: str, message: str) -> Result:
    return Result(NO_VERDICT, NO_VERDICT, (message,), node_evidence_path(node_id), False)


def _local(node: dict, root: Path) -> Result:
    check_id = node["check"]
    evidence_relative = node_evidence_path(node["id"])
    check = CHECKS.get(check_id) if check_id else None
    if check is None:
        if _path(root, evidence_relative).exists():
            return _refuse(
                node["id"],
                "refused: unregistered check cannot stamp evidence "
                "(evidence file present for a planned node)",
            )
        return Result(
            NO_VERDICT,
            NO_VERDICT,
            (
                "declared check is not registered; the node cannot pass "
                "until a check is registered, run, and stamped "
                f"(engine: {node['engine']})",
            ),
            evidence_relative,
            True,
        )
    evidence_path = _path(root, evidence_relative)
    if not evidence_path.is_file():
        return Result(
            NO_VERDICT,
            NO_VERDICT,
            ("no evidence recorded for the registered check",),
            evidence_relative,
            True,
        )
    try:
        record = json.loads(evidence_path.read_bytes())
        _validate_record(record, node, check)
    except (OSError, ValueError) as error:
        return _refuse(node["id"], f"refused: unusable evidence record: {error}")
    try:
        current = coverage_hashes(node, root)
    except StaleCoverage as error:
        return Result(STALE, STALE, (str(error),), evidence_relative, False)
    if set(current) != set(record["coverage"]):
        changed = sorted(set(current) ^ set(record["coverage"]))
        return Result(
            STALE,
            STALE,
            ("covered input set changed: " + ", ".join(changed),),
            evidence_relative,
            False,
        )
    changed = sorted(
        relative
        for relative, digest in current.items()
        if digest != record["coverage"][relative]
    )
    if changed:
        return Result(
            STALE,
            STALE,
            ("covered bytes changed: " + ", ".join(changed),),
            evidence_relative,
            False,
        )
    if record["result"] == FAIL:
        return Result(
            FAIL,
            FAIL,
            (f"recorded check failure (exit_code={record['exit_code']})",),
            evidence_relative,
            False,
        )
    if record["result"] == NOT_RUN:
        return Result(
            NOT_RUN,
            NOT_RUN,
            (f"recorded check did not run (exit_code=2)",),
            evidence_relative,
            False,
        )
    recorded_controls = {
        tuple(control["command"]): control for control in record["controls"]
    }
    required = {
        tuple(control["command"]): control
        for control in node["required_negative_controls"]
    }
    for key in sorted(set(recorded_controls) - set(required)):
        return _refuse(
            node["id"],
            "refused: unexpected recorded control: " + " ".join(key),
        )
    for key, control in sorted(required.items()):
        record_control = recorded_controls.get(key)
        if record_control is None:
            return _refuse(
                node["id"],
                "required negative control not recorded: " + control["id"],
            )
        if not record_control["detected"]:
            return _refuse(
                node["id"],
                "required negative control did not detect its fault: "
                + control["id"],
            )
    return Result(
        PASS,
        PASS,
        (
            "registered check ran clean, coverage is current, and every "
            "required negative control was recorded as detected",
        ),
        evidence_relative,
        True,
    )


def evaluate(nodes: list[dict], root: Path) -> dict[str, Result]:
    root = root.resolve()
    results: dict[str, Result] = {}
    for node in _ordered(nodes):
        local = _local(node, root)
        blocked = sorted(
            f"{dep}={results[dep].state}"
            for dep in node["prereqs"]
            if results[dep].state != PASS
        )
        reasons = local.reasons
        if blocked:
            reasons = reasons + (
                "prerequisite not demonstrated: " + ", ".join(blocked),
            )
        state = BLOCKED if blocked else local.state
        healthy = local.healthy and not (blocked and local.state == PASS)
        results[node["id"]] = Result(
            state, local.state, reasons, local.evidence, healthy
        )
    return results


def _cell(value: str) -> str:
    return (
        value.replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("|", "&#124;")
        .replace("\n", " ")
    )


def render_mermaid(nodes: list[dict], results: dict[str, Result]) -> str:
    """Conservative mermaid: quoted labels, no '#' (GitHub reserved), plain edges."""
    indexed = {node["id"]: node for node in nodes}
    alias = {node_id: f"n{i}" for i, node_id in enumerate(sorted(indexed))}
    lines = ["```mermaid", "graph TD"]
    for node_id in sorted(indexed):
        lines.append(f'  {alias[node_id]}["{node_id}: {results[node_id].state}"]')
    for node_id in sorted(indexed):
        for dep in sorted(indexed[node_id]["prereqs"]):
            lines.append(f"  {alias[dep]} --> {alias[node_id]}")
    lines.append("```")
    return "\n".join(lines)


def _summary(results: dict[str, Result]) -> dict[str, int]:
    summary = {state: 0 for state in (PASS, FAIL, NOT_RUN, BLOCKED, NO_VERDICT, STALE)}
    for result in results.values():
        summary[result.state] += 1
    return summary


def render_json(nodes: list[dict], results: dict[str, Result]) -> str:
    payload = {
        "schema": VIEW_SCHEMA,
        "generated_by": "tools/compile_capabilities.py",
        "source": "spec/capabilities-v1.json",
        "note": (
            "Generated view. States are evidence-derived; issue closure, file "
            "existence, and prose never establish a capability. Coverage is "
            "reported separately from agreement."
        ),
        "summary": _summary(results),
        "nodes": [
            {
                "id": node["id"],
                "claim": node["claim"],
                "state": results[node["id"]].state,
                "local_state": results[node["id"]].local_state,
                "reasons": list(results[node["id"]].reasons),
                "prereqs": sorted(node["prereqs"]),
                "check": node["check"],
                "engine": node["engine"],
                "evidence": results[node["id"]].evidence,
                "covered_inputs": list(node["covered_inputs"]),
                "required_negative_controls": [
                    {"id": control["id"], "detects": control["detects"]}
                    for control in node["required_negative_controls"]
                ],
                "exclusions": list(node["exclusions"]),
            }
            for node in _ordered(nodes)
        ],
    }
    return json.dumps(payload, indent=2, sort_keys=True) + "\n"


def render_markdown(nodes: list[dict], results: dict[str, Result]) -> str:
    lines = [
        "# Capability status (evidence-derived)",
        "",
        "<!-- Generated by tools/compile_capabilities.py; edit"
        " spec/capabilities-v1.json and evidence/capabilities/, never this view. -->",
        "",
        "Claim nodes for the DX7-compatible engine (plan section 7), stamped only"
        " by checks registered in `src/gf180_dx7/capabilities.py` (C01, issue"
        " #45). File existence, issue closure, tests counts, and prose never"
        " turn a node green: PASS requires a registered check that ran clean,"
        " content-hash coverage matching the current bytes of every input"
        " (including dirty and untracked files), and every required negative"
        " control recorded as executed AND detected. Any covered byte change is"
        " STALE; missing or undetected controls are NO_VERDICT, never PASS;"
        " unmet prerequisites are BLOCKED.",
        "",
        "Regenerate with `python3 tools/compile_capabilities.py`; `--check`"
        " verifies these views without executing anything (CI staleness guard);"
        " `--strict` additionally requires every node PASS and is the future"
        " release gate (honestly red while nodes are NO_VERDICT). Evidence is"
        " produced only via `--stamp <node>`, which runs the registered command"
        " and the node's required controls.",
        "",
        "| Node | Claim | State | Reason |",
        "| --- | --- | --- | --- |",
    ]
    ordered = _ordered(nodes)
    for node in ordered:
        result = results[node["id"]]
        lines.append(
            "| "
            + " | ".join(
                _cell(field)
                for field in (node["id"], node["claim"], result.state, "; ".join(result.reasons))
            )
            + " |"
        )
    lines += ["", render_mermaid(nodes, results), ""]
    for node in ordered:
        result = results[node["id"]]
        engine = (
            "`" + " ".join(node["engine"]) + "`"
            if isinstance(node["engine"], list)
            else node["engine"]
        )
        lines += [
            f"## {node['id']} — {result.state}",
            "",
            _cell(node["claim"]),
            "",
            "- Prerequisites: " + (", ".join(sorted(node["prereqs"])) or "none") + ".",
            "- Check: "
            + (
                f"`{node['check']}` — {engine}."
                if node["check"]
                else f"not registered ({node['engine']})."
            ),
            "- Covered inputs (plus the judge's own bytes): "
            + (
                ", ".join(f"`{path}`" for path in node["covered_inputs"])
                or "none declared"
            )
            + ".",
            "- Required negative controls: "
            + (
                "; ".join(
                    f"`{control['id']}` detects {control['detects']}"
                    f" (`{' '.join(control['command'])}`)"
                    for control in node["required_negative_controls"]
                )
                or "none declared"
            )
            + ".",
            "- Exclusions: " + "; ".join(_cell(item) for item in node["exclusions"]) + ".",
            "- Evidence: "
            + (
                "none; a planned node must not carry an evidence record"
                if node["check"] is None
                else f"`{result.evidence}`."
            ),
            "",
        ]
    return "\n".join(lines)


README_BEGIN = "<!-- CAPABILITIES:BEGIN -->"
README_END = "<!-- CAPABILITIES:END -->"
ISSUEDAG_END = "<!-- ISSUEDAG:END -->"


def render_readme_block(nodes: list[dict], results: dict[str, Result]) -> str:
    summary = _summary(results)
    states = (PASS, FAIL, NOT_RUN, BLOCKED, NO_VERDICT, STALE)
    lines = [
        "## Capability status (evidence-derived)",
        "",
        "Generated from [spec/capabilities-v1.json](spec/capabilities-v1.json) by"
        " `tools/compile_capabilities.py` — do not hand-edit. Evidence-derived"
        " node states; issue closure, file existence, and prose never establish"
        " a capability claim. Per-node claims, coverage, and controls:"
        " [docs/CAPABILITIES.md](docs/CAPABILITIES.md).",
        "",
        "| " + " | ".join(states) + " |",
        "| " + " | ".join("---:" for _ in states) + " |",
        "| " + " | ".join(str(summary[state]) for state in states) + " |",
        "",
        render_mermaid(nodes, results),
        "",
    ]
    return "\n".join(lines)


def lint_mermaid(block_text: str) -> list[str]:
    """Lint a markdown fragment containing exactly one mermaid block.

    Same discipline as tools/render_issue_dag.py: one fence pair, no '#'
    inside the diagram (mermaid reserves '#...;' for HTML entities), and
    every edge endpoint must be a declared node.
    """
    errors: list[str] = []
    openers = len(re.findall(r"^```mermaid\s*$", block_text, re.M))
    closers = len(re.findall(r"^```\s*$", block_text, re.M))
    if openers != 1 or closers != 1:
        return [
            f"mermaid fence integrity: expected one ```mermaid open and one"
            f" ``` close, found {openers} open / {closers} close"
        ]
    match = re.search(r"```mermaid\n(.*?)\n```", block_text, re.S)
    if match is None:
        return ["cannot extract mermaid diagram"]
    diagram = match.group(1)
    if "#" in diagram:
        errors.append(
            "'#' inside mermaid fence (reserved for HTML entities; GitHub"
            " rendering may fail)"
        )
    declared: set[str] = set()
    edges: list[tuple[str, str]] = []
    node_re = re.compile(r"^\s*([A-Za-z0-9_]+)\[")
    edge_re = re.compile(r"^\s*([A-Za-z0-9_]+)\s*(-\.->|-->|--)\s*([A-Za-z0-9_]+)\s*$")
    for line in diagram.splitlines():
        node_match = node_re.match(line)
        if node_match:
            declared.add(node_match.group(1))
            continue
        edge_match = edge_re.match(line)
        if edge_match:
            edges.append((edge_match.group(1), edge_match.group(3)))
    for source, target in edges:
        for endpoint, role in ((source, "source"), (target, "target")):
            if endpoint not in declared:
                errors.append(
                    f"edge {source} --> {target}: {role} node {endpoint!r}"
                    " is never declared"
                )
    return errors


def replace_readme_block(readme_text: str, block: str) -> str:
    """Place/replace the CAPABILITIES block right after the ISSUEDAG block."""
    wrapped = f"{README_BEGIN}\n{block}\n{README_END}"
    if README_BEGIN in readme_text or README_END in readme_text:
        _require(
            README_BEGIN in readme_text and README_END in readme_text,
            "README CAPABILITIES markers must appear as a pair",
        )
        pre = readme_text.split(README_BEGIN)[0]
        post = readme_text.split(README_END, 1)[1]
    elif ISSUEDAG_END in readme_text:
        pre = readme_text.split(ISSUEDAG_END)[0] + ISSUEDAG_END
        post = readme_text.split(ISSUEDAG_END, 1)[1]
    else:
        pre, post = readme_text, ""
    _require(
        README_BEGIN not in pre
        and README_END not in pre
        and README_BEGIN not in post
        and README_END not in post,
        "nested or duplicated CAPABILITIES block",
    )
    prefix = pre.rstrip("\n") + "\n\n"
    suffix = "\n\n" + post.lstrip("\n") if post.strip() else "\n"
    return prefix + wrapped + suffix


def extract_readme_block(readme_text: str) -> str | None:
    if README_BEGIN not in readme_text or README_END not in readme_text:
        return None
    block = readme_text.split(README_BEGIN, 1)[1].split(README_END, 1)[0]
    return block.strip("\n")
