"""C01 capability-DAG compiler tests (issue #45).

Negative controls required by the issue acceptance:

  (a) fabricated pass-evidence without recorded controls -> NO_VERDICT, never PASS
  (b) a doctored covered input byte -> STALE
  (c) a non-whitelisted check is refused (graph validation, stamping, evidence)
  (d) a required control recorded detected=false -> NO_VERDICT, never PASS
  (e) view regeneration is deterministic and the mermaid lint passes
  (f) the committed README block equals the regenerated block, placed right
      after the ISSUEDAG block

Plus the positive path (registered check + current coverage + detected
controls -> PASS), BLOCKED propagation, argv mismatch refusal, the --stamp
producer path, --check staleness detection, and --strict being red while
nodes are unproven.

State-machine tests never execute evidence commands. Only --stamp cases run
commands, and those run only `python3 -c ...` against a temp root with a
test-local check registry (the production CHECKS registry is patched, never
mutated).
"""

from __future__ import annotations

import contextlib
import importlib.util
import io
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import capabilities as cap  # noqa: E402

TOOL = REPO / "tools" / "compile_capabilities.py"
NOOP = ["python3", "-c", "pass"]
FAILING = ["python3", "-c", "raise SystemExit(1)"]

GRAPH_PATH = "spec/capabilities-v1.json"
ISSUEDAG_END = "<!-- ISSUEDAG:END -->"
MIN_README = "# fixture\n\n<!-- ISSUEDAG:BEGIN -->\nblock\n" + ISSUEDAG_END + "\n"


def _tool_module():
    spec = importlib.util.spec_from_file_location("compile_capabilities", TOOL)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def _write(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def _node(node_id: str = "alpha", *, check="fixture-check", prereqs=(),
          controls=None, covered=("inputs/a.txt",), claim="fixture claim") -> dict:
    return {
        "id": node_id,
        "claim": claim,
        "check": check,
        "engine": list(NOOP) if check else "planned: nothing registered",
        "prereqs": list(prereqs),
        "covered_inputs": list(covered),
        "required_negative_controls": controls
        if controls is not None
        else [{"id": "ctrl", "detects": "any mutation", "command": list(NOOP)}],
        "exclusions": ["test fixture only"],
    }


def _graph(*nodes: dict) -> dict:
    return {"schema_version": 1, "note": "fixture", "nodes": list(nodes)}


class FixtureCase(unittest.TestCase):
    """A temp repository with a patched, self-contained check registry."""

    def setUp(self) -> None:
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root = Path(tmp.name).resolve()
        self.tool = _tool_module()

    def patch(self, target, name, value):
        original = getattr(target, name)
        self.addCleanup(setattr, target, name, original)
        setattr(target, name, value)

    def install_registry(self, *nodes: dict, register: bool = True) -> None:
        """Register the fixture nodes' checks and evaluator inputs locally."""
        checks = {}
        if register:
            for node in nodes:
                if node["check"]:
                    checks[node["check"]] = cap.Check(
                        tuple(node["engine"]), tuple(node["covered_inputs"])
                    )
        self.patch(cap, "CHECKS", checks)
        _write(self.root / "src/gf180_dx7/capabilities.py", b"fixture judge bytes\n")
        self.patch(cap, "EVALUATOR_INPUTS", ("src/gf180_dx7/capabilities.py",))
        self.write_graph(*nodes)

    def write_graph(self, *nodes: dict) -> None:
        _write(self.root / GRAPH_PATH, json.dumps(_graph(*nodes), indent=2).encode())

    def write_input(self, relative: str, data: bytes) -> None:
        _write(self.root / relative, data)

    def prime_readme(self) -> None:
        _write(self.root / "README.md", MIN_README.encode())

    def write_record(self, node: dict, *, result="PASS", exit_code=0,
                     controls=None, coverage=None, argv=None) -> Path:
        if coverage is None:
            coverage = cap.coverage_hashes(node, self.root)
        record = {
            "schema": cap.SCHEMA,
            "node_id": node["id"],
            "check": node["check"],
            "argv": list(argv if argv is not None else node["engine"]),
            "result": result,
            "exit_code": exit_code,
            "recorded_at": "2026-09-20T00:00:00Z",
            "recorded_by": "tests/test_capabilities.py",
            "coverage": coverage,
            "controls": controls
            if controls is not None
            else [
                {
                    "command": list(control["command"]),
                    "detected": True,
                    "exit_code": 0,
                }
                for control in node["required_negative_controls"]
            ],
        }
        path = self.root / cap.node_evidence_path(node["id"])
        _write(path, (json.dumps(record, indent=2, sort_keys=True) + "\n").encode())
        return path

    def run_tool(self, *args: str) -> tuple[int, str, str]:
        """Run tools/compile_capabilities.py in-process against the fixture."""
        out, err = io.StringIO(), io.StringIO()
        argv = [str(TOOL), "--root", str(self.root), *args]
        self.patch(sys, "argv", argv)
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = self.tool.main()
        return code, out.getvalue(), err.getvalue()

    def evaluate(self, *nodes: dict) -> dict:
        return cap.evaluate(list(nodes), self.root)


class StateMachineTests(FixtureCase):
    def test_registered_check_with_valid_evidence_passes(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.PASS)

    def test_fabricated_pass_without_controls_is_never_pass(self):
        # (a): a hand-written PASS whose controls never ran.
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, controls=[])
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("not recorded", " ".join(results["alpha"].reasons))

    def test_doctored_covered_byte_is_stale(self):
        # (b): flipping one covered byte (dirty or untracked) stales the node.
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        self.write_input("inputs/a.txt", b"payload doctoring\n")
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.STALE)
        self.assertIn("covered bytes changed", " ".join(results["alpha"].reasons))

    def test_doctored_judge_byte_is_stale(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        _write(self.root / "src/gf180_dx7/capabilities.py", b"tampered judge\n")
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.STALE)

    def test_missing_covered_input_is_stale(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        (self.root / "inputs/a.txt").unlink()
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.STALE)

    def test_unregistered_check_is_refused(self):
        # (c): nothing outside the CHECKS whitelist can stamp a verdict.
        node = _node(check="sneaky-check")
        self.install_registry(node, register=False)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn(
            "unregistered check cannot stamp", " ".join(results["alpha"].reasons)
        )

    def test_planned_node_with_fabricated_evidence_is_refused(self):
        # (c, cont.): an evidence file next to a planned (unregistered) check.
        node = _node(check=None)
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        record = {
            "schema": cap.SCHEMA,
            "node_id": node["id"],
            "check": node["check"],
            "argv": [],
            "result": "PASS",
            "exit_code": 0,
            "recorded_at": "2026-09-20T00:00:00Z",
            "recorded_by": "tests/test_capabilities.py",
            "coverage": {},
            "controls": [],
        }
        _write(
            self.root / cap.node_evidence_path(node["id"]),
            (json.dumps(record, indent=2, sort_keys=True) + "\n").encode(),
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("refused", " ".join(results["alpha"].reasons))

    def test_argv_mismatch_is_refused(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, argv=["python3", "-c", "some other command"])
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn(
            "registered check command", " ".join(results["alpha"].reasons)
        )

    def test_undetected_control_is_never_pass(self):
        # (d): the control ran but did not detect its fault.
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node,
            controls=[{"command": list(NOOP), "detected": False, "exit_code": 1}],
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("did not detect", " ".join(results["alpha"].reasons))

    def test_missing_control_record_is_never_pass(self):
        node = _node()
        other = ["python3", "-c", "other"]
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node, controls=[{"command": list(other), "detected": True, "exit_code": 0}]
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)

    def test_unexpected_recorded_control_is_refused(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node,
            controls=[
                {"command": list(NOOP), "detected": True, "exit_code": 0},
                {
                    "command": ["python3", "-c", "extra"],
                    "detected": True,
                    "exit_code": 0,
                },
            ],
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn(
            "unexpected recorded control", " ".join(results["alpha"].reasons)
        )

    def test_recorded_check_failure_is_fail(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, result="FAIL", exit_code=1)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.FAIL)

    def test_recorded_not_run_is_not_run(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, result="NOT_RUN", exit_code=2)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NOT_RUN)

    def test_pass_with_nonzero_exit_is_refused(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, result="PASS", exit_code=3)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)

    def test_missing_evidence_is_no_verdict(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertTrue(results["alpha"].healthy)

    def test_planned_node_is_no_verdict_and_healthy(self):
        node = _node(check=None)
        self.install_registry(node)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertTrue(results["alpha"].healthy)

    def test_prerequisite_not_pass_blocks_the_dependent(self):
        parent = _node("parent")
        child = _node("child", prereqs=("parent",))
        self.install_registry(parent, child)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(child)
        results = self.evaluate(parent, child)
        self.assertEqual(results["parent"].state, cap.NO_VERDICT)
        self.assertEqual(results["child"].state, cap.BLOCKED)
        self.assertEqual(results["child"].local_state, cap.PASS)
        self.assertFalse(results["child"].healthy)

    def test_stale_prerequisite_blocks_a_current_dependent(self):
        parent = _node("parent")
        child = _node("child", prereqs=("parent",))
        self.install_registry(parent, child)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(parent)
        self.write_record(child)
        self.write_input("inputs/a.txt", b"changed\n")
        results = self.evaluate(parent, child)
        self.assertEqual(results["parent"].state, cap.STALE)
        self.assertEqual(results["child"].state, cap.BLOCKED)

    def test_dependency_cycle_is_policy_error(self):
        a = _node("a", prereqs=("b",))
        b = _node("b", prereqs=("a",))
        self.install_registry(a, b)
        with self.assertRaises(cap.CapabilityError):
            self.evaluate(a, b)

    def test_path_escape_is_refused(self):
        node = _node(covered=("../outside.txt",))
        self.install_registry(node, register=False)
        with self.assertRaises(cap.CapabilityError):
            cap.validate_graph(_graph(node))


class StampTests(FixtureCase):
    def test_stamp_runs_registered_check_and_controls_then_passes(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        code, out, _ = self.run_tool("--stamp", "alpha")
        self.assertEqual(code, 0, out)
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertEqual(record["result"], cap.PASS)
        self.assertEqual(record["exit_code"], 0)
        self.assertTrue(record["controls"][0]["detected"])
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.PASS)

    def test_stamp_records_not_detected_for_failing_control(self):
        node = _node(controls=[
            {"id": "ctrl", "detects": "any mutation", "command": list(FAILING)}
        ])
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        code, _, err = self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertEqual(record["result"], cap.PASS)  # the check itself ran clean
        self.assertFalse(record["controls"][0]["detected"])
        self.assertEqual(record["controls"][0]["exit_code"], 1)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("did not detect", err)
        self.assertNotEqual(code, 0)  # the producer is told the node is not PASS

    def test_stamp_refuses_unregistered_check(self):
        # (c): the producer path refuses anything outside the whitelist.
        node = _node(check="not-in-registry")
        self.install_registry(node, register=False)
        self.prime_readme()
        code, _, err = self.run_tool("--stamp", "alpha")
        self.assertEqual(code, 1)
        self.assertIn("not registered", err)
        self.assertFalse(
            (self.root / cap.node_evidence_path("alpha")).exists(),
            "stamping must not write evidence for an unregistered check",
        )

    def test_check_mode_flags_stale_views_and_regenerates(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node)
        self.prime_readme()
        self.assertEqual(self.run_tool()[0], 0)
        self.assertEqual(self.run_tool("--check")[0], 0)
        # A covered byte change must surface in the views, and --check must
        # refuse to bless the now-stale committed views.
        self.write_input("inputs/a.txt", b"changed\n")
        self.assertNotEqual(self.run_tool("--check")[0], 0)
        self.assertEqual(self.run_tool()[0], 0)
        self.assertIn(cap.STALE, (self.root / "docs" / "CAPABILITIES.md").read_text())

    def test_strict_is_red_while_nodes_are_not_pass(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.assertEqual(self.run_tool()[0], 0)
        code, _, err = self.run_tool("--strict")
        self.assertEqual(code, 1)
        self.assertIn("requires every node PASS", err)


class RealRepositoryTests(unittest.TestCase):
    """Tests (e) and (f) run against this checkout's committed views."""

    def test_view_regeneration_is_deterministic_and_lints(self):
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        results_a = cap.evaluate(nodes, REPO)
        results_b = cap.evaluate(nodes, REPO)
        self.assertEqual(
            cap.render_markdown(nodes, results_a),
            cap.render_markdown(nodes, results_b),
        )
        self.assertEqual(
            cap.render_json(nodes, results_a),
            cap.render_json(nodes, results_b),
        )
        self.assertEqual(
            cap.render_readme_block(nodes, results_a),
            cap.render_readme_block(nodes, results_b),
        )
        markdown = (REPO / "docs" / "CAPABILITIES.md").read_text()
        readme = (REPO / "README.md").read_text()
        self.assertEqual(cap.lint_mermaid(markdown), [])
        block = cap.extract_readme_block(readme)
        self.assertIsNotNone(block)
        self.assertEqual(cap.lint_mermaid(block), [])

    def test_readme_block_matches_regenerated_output_and_position(self):
        # (f): the committed block is the regenerated block, right after ISSUEDAG.
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        results = cap.evaluate(nodes, REPO)
        readme = (REPO / "README.md").read_text()
        block = cap.extract_readme_block(readme)
        expected = cap.render_readme_block(nodes, results).strip("\n")
        self.assertIsNotNone(block)
        self.assertEqual(block, expected)
        self.assertEqual(readme.count(cap.README_BEGIN), 1)
        self.assertEqual(readme.count(cap.README_END), 1)
        issuedag_end = readme.index(ISSUEDAG_END) + len(ISSUEDAG_END)
        block_begin = readme.index(cap.README_BEGIN)
        self.assertGreater(
            block_begin, issuedag_end, "CAPABILITIES block must follow ISSUEDAG"
        )
        self.assertEqual(readme[issuedag_end:block_begin].strip(), "")

    def test_compiler_check_mode_agrees_with_committed_views(self):
        completed = subprocess.run(
            [sys.executable, str(TOOL), "--check"], capture_output=True, text=True
        )
        self.assertEqual(completed.returncode, 0, completed.stderr)


class RegistryBoundaryTests(unittest.TestCase):
    def test_real_graph_only_names_registered_checks(self):
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        for node in nodes:
            if node["check"] is not None:
                self.assertIn(node["check"], cap.CHECKS)

    def test_registered_check_commands_exist_on_current_main(self):
        # Only commands on current main may be registered; e.g. the registry
        # coverage tool from another branch must never appear here.
        self.assertNotIn("registry-coverage-v1", cap.CHECKS)
        for check in cap.CHECKS.values():
            program = (
                check.command[1] if check.command[0] == "python3" else check.command[0]
            )
            if program == "-m":
                continue
            self.assertTrue(
                (REPO / program).is_file(),
                f"registered command references missing file: {program}",
            )


if __name__ == "__main__":
    unittest.main()
