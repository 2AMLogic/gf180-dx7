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
nodes are unproven. Issue #206 adds: backward compatibility of the eight
committed records under the widened (required-plus-optional) control guard,
refusal of unknown control keys and of detected=true with skipped>0, the
NOT_RUN-with-exit-0 guarded-skip encoding (requires a recorded skip
observation), per-node skip_policy enforcement, and stamp-side capture of
bounded output plus unittest run/skip counts. Issue #232 adds: the
registry-declared skip_token guarded-skip contract (a check reports a
skip only for its declared token; a None declaration cannot fabricate
one), refusal of fabricated skip flags on token-less checks, and the
anchored unittest skipped= summary regex. Issue #274 adds: the
fixed-model registration (n08 verify+challenge composition, GUARDED_SKIP
token, one-sample-mutation control) and its absent-oracle and
zero-skip-summary paths. Issue #275 adds: the rtl-core registration (four
bench drivers composed by tools/rtl_core_check.py, GUARDED_SKIP token,
H05 operator-order control), its verdict composition (a measured mismatch
outranks a skip), and the missing-simulator path through the registered
command. Issue #276 adds: the rtl-exact registration (the existing
read-only comparison path in tools/functional_verification.py, GUARDED_SKIP
token, two H07 mutant controls that count only a measured sample
divergence), its verdict composition (a measured mismatch is FAIL and a
stale pin STALE even when cases stay ungraded; a selected case that did not
execute is NOT_RUN naming it), the per-case coverage record recovered from
the stamp's bounded output tail, and the missing-tool path through the
registered command.

State-machine tests never execute evidence commands. Only --stamp cases run
commands, and those run only `python3 -c ...` against a temp root with a
test-local check registry (the production CHECKS registry is patched, never
mutated). The rtl-core missing-simulator case and the rtl-exact missing-tool
case run the real registered commands, which guard before any simulation
starts.
"""

from __future__ import annotations

import contextlib
import hashlib
import importlib.util
import io
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "src"))

from gf180_dx7 import capabilities as cap  # noqa: E402
from gf180_dx7 import compare  # noqa: E402

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


def _n08_module(name: str):
    spec = importlib.util.spec_from_file_location(
        name, REPO / "tools" / f"{name}.py"
    )
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def _write(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def _node(node_id: str = "alpha", *, check="fixture-check", prereqs=(),
          controls=None, covered=("inputs/a.txt",), claim="fixture claim",
          skip_policy="pass", engine=None) -> dict:
    return {
        "id": node_id,
        "claim": claim,
        "check": check,
        "engine": list(engine)
        if engine is not None
        else (list(NOOP) if check else "planned: nothing registered"),
        "prereqs": list(prereqs),
        "covered_inputs": list(covered),
        "required_negative_controls": controls
        if controls is not None
        else [{"id": "ctrl", "detects": "any mutation", "command": list(NOOP)}],
        "exclusions": ["test fixture only"],
        "skip_policy": skip_policy,
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

    def install_registry(self, *nodes: dict, register: bool = True,
                         skip_tokens: dict | None = None) -> None:
        """Register the fixture nodes' checks and evaluator inputs locally.

        ``skip_tokens`` maps a check name to its declared guarded-skip
        token (issue #232); checks absent from the mapping declare no
        skip path (skip_token=None).
        """
        checks = {}
        if register:
            for node in nodes:
                if node["check"]:
                    checks[node["check"]] = cap.Check(
                        tuple(node["engine"]),
                        tuple(node["covered_inputs"]),
                        skip_token=(skip_tokens or {}).get(node["check"]),
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


class OptionalRecordFieldsTests(FixtureCase):
    """Issue #206: additive optional record fields (SCHEMA stays /1)."""

    def _control(self, **extra):
        control = {"command": list(NOOP), "detected": True, "exit_code": 0}
        control.update(extra)
        return control

    def _add_check_skip(self, record_path: Path) -> None:
        record = json.loads(record_path.read_bytes())
        record["check_skip_reported"] = True
        record["check_output"] = {"sha256": "c" * 64, "tail": "NOT_RUN: guarded"}
        _write(
            record_path,
            (json.dumps(record, indent=2, sort_keys=True) + "\n").encode(),
        )

    def test_committed_records_still_validate_unchanged(self):
        # Backward compatibility (the #200 guard): every committed evidence
        # record must keep validating against its real node, untouched.
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        indexed = {node["id"]: node for node in nodes}
        checked = 0
        for node_id in sorted(indexed):
            node = indexed[node_id]
            check = cap.CHECKS.get(node["check"]) if node["check"] else None
            if check is None:
                continue
            record = json.loads(
                (REPO / cap.node_evidence_path(node_id)).read_bytes()
            )
            cap._validate_record(record, node, check)
            checked += 1
        self.assertGreaterEqual(checked, 8)

    def test_control_with_optional_fields_validates(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node,
            controls=[
                self._control(
                    output={"sha256": "a" * 64, "tail": "OK (skipped=0)"},
                    tests_run=11,
                    skipped=0,
                )
            ],
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.PASS)

    def test_control_with_unknown_key_is_refused(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, controls=[self._control(stdout="surprise")])
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("refused", " ".join(results["alpha"].reasons))

    def test_detected_true_with_skipped_tests_is_refused(self):
        # Negative control: detected=true alongside skipped>0 is a
        # contradiction and must be refused, never passed through.
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, controls=[self._control(tests_run=2, skipped=2)])
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("skipped>0", " ".join(results["alpha"].reasons))

    def test_not_run_zero_exit_requires_skip_observation(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, result="NOT_RUN", exit_code=0)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("skip observation", " ".join(results["alpha"].reasons))

    def test_not_run_zero_exit_with_skip_observation_validates(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node,
            result="NOT_RUN",
            exit_code=0,
            controls=[
                self._control(
                    detected=False,
                    output={"sha256": "b" * 64, "tail": "OK (skipped=1)"},
                    tests_run=11,
                    skipped=1,
                )
            ],
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NOT_RUN)
        self.assertIn("guarded skip", " ".join(results["alpha"].reasons))

    def test_pass_with_check_skip_requires_pass_policy(self):
        # Negative control: a PASS carrying a check-level skip under a
        # not_run policy must be refused by record validation itself.
        node = _node(skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "NOT_RUN"})
        self.write_input("inputs/a.txt", b"payload\n")
        self._add_check_skip(self.write_record(node))
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn("skip_policy", " ".join(results["alpha"].reasons))

    def test_pass_with_check_skip_under_pass_policy_surfaces_reason(self):
        node = _node(skip_policy="pass")
        self.install_registry(node, skip_tokens={"fixture-check": "NOT_RUN"})
        self.write_input("inputs/a.txt", b"payload\n")
        self._add_check_skip(self.write_record(node))
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.PASS)
        self.assertIn("guarded skip", " ".join(results["alpha"].reasons))

    def test_output_blob_shape_is_enforced(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(
            node, controls=[self._control(output={"sha256": "nothex", "tail": "x"})]
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)

    def test_graph_refuses_unknown_skip_policy(self):
        node = _node(skip_policy="maybe")
        with self.assertRaises(cap.CapabilityError):
            cap.validate_graph(_graph(node))


class SkipPolicyTests(FixtureCase):
    """Issue #206: a reported control skip never reads PASS, in any policy."""

    def _skipped_control(self, command=None):
        return [
            {
                "command": list(command or NOOP),
                "detected": False,
                "exit_code": 0,
                "tests_run": 2,
                "skipped": 2,
            }
        ]

    def test_not_run_policy_with_skipped_control_does_not_pass(self):
        node = _node(skip_policy="not_run")
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, controls=self._skipped_control())
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NOT_RUN)
        self.assertIn("skipped 2 of 2 tests", " ".join(results["alpha"].reasons))

    def test_skipped_control_forces_not_run_under_pass_policy_too(self):
        node = _node(skip_policy="pass")
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.write_record(node, controls=self._skipped_control())
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NOT_RUN)

    def test_declared_token_never_turns_a_failing_exit_into_not_run(self):
        # Issue #276: a check that guard-skipped some cases AND measured a
        # mismatch exits nonzero with the token present; FAIL outranks the
        # skip. The stamp maps every failing, non-2 exit to FAIL.
        node = _node(skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "GUARDED_SKIP"})
        self.write_input("inputs/a.txt", b"payload\n")
        for exit_code in (1, 3):
            self.assertEqual(self.tool._result_for_exit(exit_code), cap.FAIL)
            path = self.write_record(node, result="FAIL", exit_code=exit_code)
            record = json.loads(path.read_text())
            record["check_skip_reported"] = True
            path.write_text(json.dumps(record, indent=2))
            result = self.evaluate(node)["alpha"]
            self.assertEqual((result.local_state, result.state), (cap.FAIL, cap.FAIL))

    def test_stale_pin_exit_is_never_recorded_as_not_run_or_pass(self):
        # The FV driver's STALE exit (3) is a failing exit: a record that
        # tried to file it as NOT_RUN (or PASS) is refused, never a verdict.
        node = _node(skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "GUARDED_SKIP"})
        self.write_input("inputs/a.txt", b"payload\n")
        for result, skip in (("NOT_RUN", True), ("NOT_RUN", False), ("PASS", False)):
            path = self.write_record(node, result=result, exit_code=3)
            record = json.loads(path.read_text())
            if skip:
                record["check_skip_reported"] = True
            path.write_text(json.dumps(record, indent=2))
            refused = self.evaluate(node)["alpha"]
            self.assertEqual((refused.local_state, refused.state),
                             (cap.NO_VERDICT, cap.NO_VERDICT))
            self.assertIn("refused: unusable evidence record",
                          " ".join(refused.reasons))


class SkipTokenDeclarationTests(FixtureCase):
    """Issue #232: guarded-skip detection is declared in the registry.

    The two negative controls here demonstrably bite against unmodified
    main logic (origin/main @ 222c0ec, run via the PR harness): there, a
    skip wording other than NOT_RUN stamped a skip-shaped PASS (false
    negative) and an incidental NOT_RUN token fabricated a
    check_skip_reported=true record (false positive).
    """

    def test_no_skip_path_ignores_incidental_not_run_token(self):
        # FP control: a check that declares no skip path and incidentally
        # prints "NOT_RUN" (the tests/test_compare.py method-docstring
        # scenario) must not record a skip it never performed.
        loud = ["python3", "-c", "print('docstring mentions NOT_RUN; no skip')"]
        node = _node(engine=loud, skip_policy="pass")
        self.install_registry(node)  # skip_token=None declared
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertFalse(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.PASS)

    def test_declared_skip_wording_other_than_not_run_is_recorded(self):
        # FN control: a guard-skip whose wording is not the bare "NOT_RUN"
        # token is recorded because the registry declares the wording the
        # check actually emits. Under the replaced bare-substring test the
        # same output stamped a skip-shaped PASS (shown failing against
        # origin/main in the PR).
        skipping = [
            "python3", "-c",
            "print('oracle absent SKIPPED (guarded skip): deferred')",
        ]
        node = _node(engine=skipping, skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "SKIPPED"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.NOT_RUN)
        self.assertEqual(record["exit_code"], 0)

    def test_declared_token_emitted_on_stderr_only_still_records(self):
        # The compiler captures combined stdout+stderr; verify_reference,
        # capture_trace and validate_dev32 print their skip notes to stderr.
        skipping = [
            "python3", "-c",
            "import sys; print('oracle absent SKIPPED (guarded skip)',"
            " file=sys.stderr)",
        ]
        node = _node(engine=skipping, skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "SKIPPED"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.NOT_RUN)

    def test_non_utf8_output_still_matches_declared_token(self):
        skipping = [
            "python3", "-c",
            "import sys;"
            " sys.stdout.buffer.write(b'\\xff\\xfe SKIPPED (guarded skip)');",
        ]
        node = _node(engine=skipping, skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "SKIPPED"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.NOT_RUN)

    def test_pass_policy_declared_skip_stays_pass(self):
        skipping = ["python3", "-c", "print('partial: SKIPPED (guarded)')"]
        node = _node(engine=skipping, skip_policy="pass")
        self.install_registry(node, skip_tokens={"fixture-check": "SKIPPED"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.PASS)

    def test_record_with_fabricated_skip_flag_and_no_declared_token_refused(self):
        # Validation-side guard: a record claiming a skip for a check that
        # declares no skip path is refused, not silently believed.
        node = _node()
        self.install_registry(node)  # skip_token=None declared
        self.write_input("inputs/a.txt", b"payload\n")
        record_path = self.write_record(node)
        record = json.loads(record_path.read_bytes())
        record["check_skip_reported"] = True
        record["check_output"] = {"sha256": "c" * 64, "tail": "NOT_RUN: no skip"}
        _write(
            record_path,
            (json.dumps(record, indent=2, sort_keys=True) + "\n").encode(),
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NO_VERDICT)
        self.assertIn(
            "declares no skip token", " ".join(results["alpha"].reasons)
        )

    def test_unittest_counts_ignores_competing_skipped_outside_summary(self):
        # A control that prints "skipped=3" somewhere other than
        # unittest's summary line must not have that mistaken for the
        # run's skip count. Fails against the replaced unanchored regex.
        text = (
            "listing note: skipped=3 stale fixtures\n"
            "Ran 5 tests in 0.001s\n"
            "OK\n"
        )
        self.assertEqual(self.tool._unittest_counts(text), (5, 0))

    def test_unittest_counts_reads_summary_lines_only(self):
        self.assertEqual(
            self.tool._unittest_counts("Ran 9 tests in 0.1s\nOK (skipped=2)\n"),
            (9, 2),
        )
        self.assertEqual(
            self.tool._unittest_counts(
                "Ran 9 tests in 0.1s\nFAILED (failures=1, skipped=3)\n"
            ),
            (9, 3),
        )
        self.assertEqual(
            self.tool._unittest_counts("Ran 4 tests in 0.1s\nOK\n"), (4, 0)
        )

    def test_unittest_counts_not_a_unittest_run_stays_null(self):
        self.assertEqual(self.tool._unittest_counts("hello world\n"), (None, None))

    def test_control_competing_skipped_text_does_not_force_not_run(self):
        # Stamp-path form of the anchoring control: the control stays
        # detected and the node PASSes even though its output contains a
        # competing "skipped=3". Fails against the unanchored regex, which
        # forced NOT_RUN and detected=false from the identical output.
        noisy_control = [
            "python3", "-c",
            "print('Ran 4 tests in 0.001s'); print('OK');"
            " print('listing note: skipped=3 stale fixtures')",
        ]
        node = _node(controls=[{
            "id": "noisy", "detects": "any mutation",
            "command": noisy_control,
        }])
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertEqual(record["controls"][0]["detected"], True)
        self.assertEqual(record["controls"][0]["skipped"], 0)
        self.assertEqual(record["result"], cap.PASS)


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


class StampCaptureTests(FixtureCase):
    """Issue #206: --stamp captures bounded output and unittest skip counts."""

    def test_stamp_records_output_counts_and_skip_result(self):
        control = [
            "python3",
            "-c",
            "import sys; sys.stderr.write"
            "('Ran 2 tests in 0.001s\\n\\nOK (skipped=2)\\n')",
        ]
        node = _node(
            controls=[{"id": "ctrl", "detects": "any mutation", "command": control}]
        )
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        code, _, _ = self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertEqual(record["result"], cap.NOT_RUN)
        self.assertEqual(record["exit_code"], 0)
        self.assertFalse(record["controls"][0]["detected"])
        self.assertEqual(record["controls"][0]["tests_run"], 2)
        self.assertEqual(record["controls"][0]["skipped"], 2)
        captured = "Ran 2 tests in 0.001s\n\nOK (skipped=2)\n"
        self.assertEqual(
            record["controls"][0]["output"]["sha256"],
            hashlib.sha256(captured.encode()).hexdigest(),
        )
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.NOT_RUN)

    def test_stamp_non_unittest_control_records_null_counts(self):
        node = _node()
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertIsNone(record["controls"][0]["tests_run"])
        self.assertIsNone(record["controls"][0]["skipped"])
        self.assertTrue(record["controls"][0]["detected"])
        self.assertEqual(record["result"], cap.PASS)

    def test_stamp_check_output_digest_covers_full_text(self):
        loud = ["python3", "-c", "print('z' * 5000)"]
        node = _node(engine=loud)
        self.install_registry(node)
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        full_text = "z" * 5000 + "\n"
        self.assertEqual(
            record["check_output"]["sha256"],
            hashlib.sha256(full_text.encode()).hexdigest(),
        )
        self.assertLessEqual(len(record["check_output"]["tail"]), 2000)
        self.assertTrue(
            record["check_output"]["tail"].endswith(full_text[-2000:])
        )
        self.assertFalse(record["check_skip_reported"])

    def test_stamp_not_run_policy_check_skip_records_not_run(self):
        skipping = ["python3", "-c", "print('citation checks NOT_RUN for 71')"]
        node = _node(engine=skipping, skip_policy="not_run")
        self.install_registry(node, skip_tokens={"fixture-check": "NOT_RUN"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.NOT_RUN)
        self.assertEqual(record["exit_code"], 0)

    def test_stamp_pass_policy_check_skip_stays_pass(self):
        skipping = ["python3", "-c", "print('citation checks NOT_RUN for 3')"]
        node = _node(engine=skipping, skip_policy="pass")
        self.install_registry(node, skip_tokens={"fixture-check": "NOT_RUN"})
        self.write_input("inputs/a.txt", b"payload\n")
        self.prime_readme()
        self.run_tool("--stamp", "alpha")
        record = json.loads(
            (self.root / cap.node_evidence_path("alpha")).read_bytes()
        )
        self.assertTrue(record["check_skip_reported"])
        self.assertEqual(record["result"], cap.PASS)
        results = self.evaluate(node)
        self.assertEqual(results["alpha"].state, cap.PASS)
        self.assertIn("guarded skip", " ".join(results["alpha"].reasons))


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

    def test_registered_checks_declare_skip_token_explicitly(self):
        # Issue #232: skip_token is a required Check field (omitting it is
        # a TypeError at registration — there is no silent default), and
        # every registered check pins its guarded-skip contract: the
        # exact token it emits when it guard-skips, or None when it has
        # no guarded-skip path. Changing a declaration is a reviewed
        # registry edit that must update this pin deliberately.
        import dataclasses

        fields = {f.name: f for f in dataclasses.fields(cap.Check)}
        self.assertEqual(fields["skip_token"].default, dataclasses.MISSING)
        with self.assertRaises(TypeError):
            cap.Check(("python3", "-c", "pass"), ("inputs/a.txt",))
        with self.assertRaises(cap.CapabilityError):
            cap.Check(
                ("python3", "-c", "pass"), ("inputs/a.txt",), skip_token=""
            )
        self.assertEqual(
            {name: check.skip_token for name, check in cap.CHECKS.items()},
            {
                "contract-tests-v1": None,
                "reference-pin-v1": "NOT_RUN",
                "render-selfcheck-v1": "NOT_RUN",
                "compare-evidence-v1": None,
                "trace-capture-v1": "NOT_RUN",
                "comparator-qualification-v1": None,
                "dev32-validation-v1": "NOT_RUN",
                "numeric-profile-v1": "NOT_RUN",
                "fixed-model-challenge-v1": "GUARDED_SKIP",
                "rtl-core-v1": "GUARDED_SKIP",
                "rtl-exact-v1": "GUARDED_SKIP",
            },
        )


class TestFixedModelControl(unittest.TestCase):
    """Required negative control for the fixed-model node (issue #274).

    A temporary copy of a committed challenge vector with exactly one
    sample byte flipped must FAIL the exact comparison path the challenge
    itself uses (gf180_dx7.compare.compare_streams at the frozen
    byte-identical f32le threshold), with the divergent sample named; the
    unmutated pair must pass it. Runs without the external oracle: it
    exercises the comparison apparatus over committed vectors, not a
    reference render.
    """

    VECTOR = "reference/release/vectors/alg-05.f32"
    SAMPLE = 1000

    def test_one_flipped_sample_fails_exact_comparison(self):
        pcm = (REPO / self.VECTOR).read_bytes()
        clean = compare.compare_streams(pcm, pcm)
        self.assertTrue(clean.passed)
        self.assertTrue(clean.byte_identical)
        mutated = bytearray(pcm)
        mutated[self.SAMPLE * 4] ^= 0x01
        report = compare.compare_streams(pcm, bytes(mutated))
        self.assertFalse(report.passed)
        self.assertFalse(report.byte_identical)
        primary = report.primary or {}
        self.assertFalse(primary["exact_equal"])
        self.assertGreaterEqual(primary["mismatch_count"], 1)
        self.assertEqual(primary["first_divergent_sample"], self.SAMPLE)
        self.assertIsNotNone(primary["max_abs_error"])


class FixedModelCheckTests(unittest.TestCase):
    """Issue #274: the fixed-model registration and its skip-token contract.

    The check composes the frozen-release verification with the dry
    oracle challenge; its guarded-skip token GUARDED_SKIP must occur in
    the composed output exactly when a row actually did not run, so the
    absent-oracle path (exit 2) and the zero-NOT_RUN summary path can
    never fabricate or hide a skip.
    """

    def test_registration_matches_registry(self):
        nodes = {n["id"]: n for n in cap.load_graph(REPO / GRAPH_PATH)}
        node = nodes["fixed-model"]
        self.assertEqual(node["check"], "fixed-model-challenge-v1")
        check = cap.CHECKS[node["check"]]
        self.assertEqual(node["engine"], list(check.command))
        self.assertEqual(node["skip_policy"], "not_run")
        self.assertEqual(check.skip_token, "GUARDED_SKIP")
        for tool in ("tools/n08_challenge.py", "tools/n08_verify_release.py"):
            self.assertIn(tool, check.inputs)
        covered = set(node["covered_inputs"])
        for pin in (
            "reference/manifest.json",
            "reference/oracle-identity.json",
            "spec/release-vectors-v1.json",
            "reference/release/manifest.json",
        ):
            self.assertIn(pin, covered)
        self.assertIn("src/gf180_dx7/compare.py", covered)
        self.assertIn("src/gf180_dx7/sysex.py", covered)
        self.assertTrue(
            any(p.startswith("src/gf180_dx7/model/") for p in covered)
        )
        vectors = [p for p in covered
                   if p.startswith("reference/release/vectors/")]
        self.assertEqual(len(vectors), 51)
        control = node["required_negative_controls"][0]
        self.assertEqual(
            control["command"],
            ["python3", "-m", "unittest",
             "tests.test_capabilities.TestFixedModelControl"],
        )

    def test_absent_oracle_exits_two_without_skip_token(self):
        with tempfile.TemporaryDirectory() as td:
            absent = Path(td) / "absent-oracle"
            results = Path(td) / "challenge.json"
            proc = subprocess.run(
                [sys.executable, "tools/n08_challenge.py",
                 "--oracle", str(absent), "--results", str(results)],
                cwd=REPO, capture_output=True, text=True,
            )
            combined = proc.stdout + proc.stderr
            self.assertEqual(proc.returncode, 2, combined)
            self.assertIn("NOT_RUN", combined)
            self.assertNotIn("GUARDED_SKIP", combined)
            self.assertFalse(results.exists())

    def test_zero_not_run_summary_emits_no_skip_token(self):
        proc = subprocess.run(
            [sys.executable, "tools/n08_verify_release.py", "--no-rerender"],
            cwd=REPO, capture_output=True, text=True,
        )
        combined = proc.stdout + proc.stderr
        self.assertEqual(proc.returncode, 0, combined)
        self.assertIn("0 NOT_RUN", proc.stdout)
        self.assertNotIn("GUARDED_SKIP", combined)

    def test_verify_report_emits_token_only_for_actual_skip(self):
        verify = _n08_module("n08_verify_release")
        rows = [
            {"check": "vector_file", "case": "alg-05",
             "status": "PASS", "detail": "vectors/alg-05.f32"},
        ]
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            verify.print_report(Path("reference/release"), 0, rows)
        self.assertNotIn("GUARDED_SKIP", out.getvalue())
        rows.append({"check": "rerender", "case": "dev32-03",
                     "status": "NOT_RUN", "detail": "archive unavailable"})
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            verify.print_report(Path("reference/release"), 0, rows)
        text = out.getvalue()
        self.assertIn("GUARDED_SKIP", text)
        self.assertIn("1 NOT_RUN", text)

    def test_challenge_skip_token_emitted_only_for_actual_skip(self):
        challenge = _n08_module("n08_challenge")
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            challenge.emit_guarded_skip(0)
        self.assertNotIn("GUARDED_SKIP", out.getvalue())
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            challenge.emit_guarded_skip(2)
        self.assertIn("GUARDED_SKIP", out.getvalue())


class RtlCoreCheckTests(unittest.TestCase):
    """Issue #275: the rtl-core registration and its verdict composition.

    The check composes the four per-module bench drivers; a measured
    mismatch (FAIL) outranks a skip, a missing simulator or skipped
    required bench is NOT_RUN naming the missing work, and the combined
    output carries GUARDED_SKIP exactly when a bench did not run.
    """

    def _rtl_core_module(self):
        spec = importlib.util.spec_from_file_location(
            "rtl_core_check", REPO / "tools" / "rtl_core_check.py"
        )
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module

    def _bench(self, bench_id):
        rtl = self._rtl_core_module()
        return next(b for b in rtl.BENCHES if b["id"] == bench_id)

    def test_registration_matches_registry(self):
        nodes = {n["id"]: n for n in cap.load_graph(REPO / GRAPH_PATH)}
        node = nodes["rtl-core"]
        self.assertEqual(node["check"], "rtl-core-v1")
        check = cap.CHECKS[node["check"]]
        self.assertEqual(node["engine"], list(check.command))
        self.assertEqual(node["skip_policy"], "not_run")
        self.assertEqual(check.skip_token, "GUARDED_SKIP")
        for tool in ("tools/rtl_core_check.py", "tools/h01_compare.py",
                     "tools/h04_compare.py", "tools/h05_compare.py",
                     "tools/h06_compare.py", "tools/iverilog_run.py"):
            self.assertIn(tool, check.inputs)
        covered = set(node["covered_inputs"])
        for rtl_input in (
            "rtl/operator_probe.v", "rtl/tb_operator_probe.v",
            "rtl/env_unit.v", "rtl/tb_env_unit.v",
            "rtl/alg_router.v", "rtl/tb_alg_router.v",
            "rtl/pitch_mod.v", "rtl/tb_pitch_mod.v",
        ):
            self.assertIn(rtl_input, covered)
        self.assertTrue(
            any(p.startswith("src/gf180_dx7/model/") for p in covered)
        )
        for table in ("reference/tables/manifest.json",
                      "reference/tables/alg_table.hex",
                      "reference/tables/alg_manifest.json",
                      "reference/tables/freqlut_table.hex",
                      "reference/tables/lfo_sin_table.hex"):
            self.assertIn(table, covered)
        control = node["required_negative_controls"][0]
        self.assertEqual(control["id"], "h05-oporder-mutant-detected")
        self.assertEqual(
            control["command"],
            ["python3", "-m", "unittest",
             "tests.test_h05.TestConformance."
             "test_negative_control_oporder_fails_widely", "-v"],
        )
        for path in cap.coverage_paths(node):
            self.assertTrue(
                (REPO / path).is_file(), f"covered input missing: {path}"
            )

    def test_exact_bench_summary_carries_counts_and_controls(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h01")
        report = {
            "status": "PASS",
            "equivalence": {
                "expected_rows": 4794, "actual_rows": 4794,
                "mismatches": 0, "first_mismatch": None, "status": "PASS",
            },
            "negative_control": {"status": "MET (mutant fails)"},
        }
        summary = rtl.summarize(bench, 0, "", report)
        self.assertEqual(summary["verdict"], "PASS")
        self.assertEqual(summary["expected"], 4794)
        self.assertEqual(summary["mismatches"], 0)
        self.assertIsNone(summary["first_divergence"])
        line = rtl.summary_line(summary, report)
        self.assertIn("expected=4794", line)
        self.assertIn("actual=4794", line)
        self.assertIn("mismatches=0", line)
        self.assertIn("control=MET", line)

    def test_h05_conformance_counts_are_normalized(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h05")
        report = {
            "status": "PASS",
            "conformance": {
                "expected_records": 159548, "actual_records": 159548,
                "mismatch_records": 0, "first_mismatch": None,
                "status": "PASS",
            },
            "mutants": {"fbdelay": {"status": "MET (mutant fails)"}},
        }
        summary = rtl.summarize(bench, 0, "", report)
        self.assertEqual(summary["verdict"], "PASS")
        self.assertEqual(summary["expected"], 159548)
        self.assertEqual(summary["actual"], 159548)
        self.assertEqual(summary["mismatches"], 0)
        self.assertIn("mutants=fbdelay:MET", rtl.summary_line(summary, report))

    def test_measured_mismatch_is_fail_with_first_divergence(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h04")
        first = {
            "eval_index": 12, "phase_before": "0000abcd",
            "expected_out": "00000001", "actual_out": "00000002",
        }
        report = {
            "status": "FAIL",
            "equivalence": {
                "expected_rows": 27931, "actual_rows": 27931,
                "mismatches": 3, "first_mismatch": first, "status": "FAIL",
            },
        }
        summary = rtl.summarize(bench, 1, "", report)
        self.assertEqual(summary["verdict"], "FAIL")
        self.assertIn('"eval_index":12', summary["first_divergence"])
        line = rtl.summary_line(summary, report)
        self.assertIn("first_divergence=", line)
        self.assertIn("mismatches=3", line)

    def test_missing_simulator_is_not_run_naming_missing_work(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h05")
        text = "...\nCOULD-NOT-RUN: iverilog binary not found: /nope\n"
        summary = rtl.summarize(bench, 2, text, None)
        self.assertEqual(summary["verdict"], "NOT_RUN")
        self.assertEqual(
            summary["reason"],
            "COULD-NOT-RUN: iverilog binary not found: /nope",
        )
        line = rtl.summary_line(summary, None)
        self.assertIn("h05 routing/feedback: NOT_RUN", line)
        self.assertIn("reason=COULD-NOT-RUN: iverilog binary not found", line)

    def test_report_without_required_section_is_skipped_bench(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h06")
        summary = rtl.summarize(bench, 0, "", {"status": "PASS"})
        self.assertEqual(summary["verdict"], "NOT_RUN")
        self.assertIn("skipped required bench", summary["reason"])
        self.assertIn("equivalence", summary["reason"])
        # Exit 0 without any report at all is equally a skipped bench.
        summary = rtl.summarize(bench, 0, "", None)
        self.assertEqual(summary["verdict"], "NOT_RUN")
        self.assertIn("skipped required bench", summary["reason"])

    def test_crash_without_report_is_fail_not_not_run(self):
        rtl = self._rtl_core_module()
        bench = self._bench("h06")
        summary = rtl.summarize(bench, 1, "Traceback ...", None)
        self.assertEqual(summary["verdict"], "FAIL")
        self.assertIn("without writing compare_report.json",
                      summary["reason"])

    def test_compose_fail_outranks_skip_and_skip_outranks_pass(self):
        rtl = self._rtl_core_module()

        def verdict_of(statuses):
            return rtl.compose([{"verdict": s} for s in statuses])

        self.assertEqual(verdict_of(["PASS", "PASS", "PASS", "PASS"]), "PASS")
        self.assertEqual(
            verdict_of(["FAIL", "NOT_RUN", "PASS", "PASS"]), "FAIL")
        self.assertEqual(
            verdict_of(["NOT_RUN", "PASS", "PASS", "PASS"]), "NOT_RUN")

    def test_guarded_skip_token_emitted_only_for_actual_skip(self):
        rtl = self._rtl_core_module()
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rtl.emit_guarded_skip(0, [])
        self.assertNotIn("GUARDED_SKIP", out.getvalue())
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rtl.emit_guarded_skip(2, ["h01", "h05"])
        text = out.getvalue()
        self.assertIn("GUARDED_SKIP", text)
        self.assertIn("h01", text)
        self.assertIn("h05", text)

    def test_registered_command_missing_simulator_is_not_run(self):
        """The real registered command, every bench guarded to exit 2.

        Each driver probes iverilog before any simulation, so a
        nonexistent executable composes to exit 2 with the declared
        GUARDED_SKIP token and every bench named as missing work --
        never a FAIL and never a silent pass.
        """
        check = cap.CHECKS["rtl-core-v1"]
        argv = list(check.command) + [
            "--scratch", tempfile.mkdtemp(prefix="rtl_core_skip_"),
            "--iverilog", "/nonexistent/iverilog",
        ]
        proc = subprocess.run(
            argv, cwd=REPO, capture_output=True, text=True, timeout=120
        )
        combined = proc.stdout + proc.stderr
        self.assertEqual(proc.returncode, 2, combined)
        self.assertIn("GUARDED_SKIP: 4 bench(es) did not run", combined)
        self.assertIn("COULD-NOT-RUN: iverilog binary not found", combined)
        for bench_id in ("h01", "h04", "h05", "h06"):
            self.assertIn(f"rtl-core {bench_id} ", combined)
            self.assertIn("NOT_RUN reason=COULD-NOT-RUN", combined)
        self.assertIn("rtl-core verdict: NOT_RUN", combined)


def _fv_module():
    spec = importlib.util.spec_from_file_location(
        "functional_verification", REPO / "tools" / "functional_verification.py"
    )
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class RtlExactCheckTests(unittest.TestCase):
    """Issue #276: the rtl-exact registration over the existing read-only
    comparison path (tools/functional_verification.py check mode).

    A partial run -- 23 windows exact, 11 dev cases ungraded -- is a
    guarded skip: the declared token is emitted, the check exits 0, and
    the stamp records NOT_RUN under skip_policy not_run.  A measured
    mismatch is FAIL and a stale pin STALE whatever else did not run; a
    selected case that did not execute is NOT_RUN naming it.  The controls
    count only a measured sample divergence, never a cocotb `failed` by
    itself.  The per-case coverage record must stay recoverable from the
    stamp's bounded 2,000-character output tail.
    """

    # evidence/t1-functional/report.json as committed at 69f0464 and named
    # by the issue; the registration was stamped against exactly these
    # bytes and the read-only check must never rewrite them.
    BASELINE_REPORT_SHA256 = (
        "724f1dbeff16d182bbf772dde3c6c7a7f6d13d8cc9d2281d03a7db1e1a379851"
    )
    STAMP = REPO / "evidence" / "capabilities" / "rtl-exact.json"
    GLITCH = (
        "stress-sum16: RTL != frozen model; first mismatches "
        "[{'index': 849, 'golden': 23300, 'actual': 23316, "
        "'block': 11, 'sample': 17}] overrun=0 overflow=0\nassert False"
    )

    @classmethod
    def setUpClass(cls):
        cls.fv = _fv_module()
        cls.tool = _tool_module()
        cls.committed = json.loads(
            (REPO / "evidence" / "t1-functional" / "report.json").read_text()
        )
        cls.selected, cls.ungraded = cls.fv.lane_cases()
        cls.hashes = cls.fv.rtl_hashes()

    def _fresh(self, **changes):
        """A deep copy of the committed report standing in for a fresh
        run: `drop` removes tests by name, `fail` maps a test name to the
        error_message of a failed test, `skip` maps a test name to a
        status that is neither passed nor failed."""
        fresh = json.loads(json.dumps(self.committed))
        dropped = set(changes.get("drop", ()))
        fresh["tests"] = [t for t in fresh["tests"] if t["name"] not in dropped]
        for name, message in changes.get("fail", {}).items():
            test = next(t for t in fresh["tests"] if t["name"] == name)
            test["status"] = "failed"
            test["error_message"] = message
        for name, status in changes.get("skip", {}).items():
            test = next(t for t in fresh["tests"] if t["name"] == name)
            test["status"] = status
        bad = [t["name"] for t in fresh["tests"] if t["status"] != "passed"]
        fresh["status"] = "pass" if not bad else "fail"
        fresh["failed_count"] = len(bad)
        fresh["passed_count"] = len(fresh["tests"]) - len(bad)
        fresh["test_count"] = len(fresh["tests"])
        return fresh

    def _verdict(self, fresh, hashes=None):
        return self.fv.check_verdict(
            self.committed, fresh, hashes or self.hashes, self.selected
        )

    def _committed_counts(self):
        return {
            cid: row["checked_samples"]
            for cid, row in self.committed["provenance"]["cases"].items()
        }

    def _record_text(self, status, rows, note):
        lines = self.fv.coverage_lines(
            status, rows, self.ungraded,
            self.committed["provenance"]["not_graded"]["reason"],
        )
        return "\n".join(lines + [f"GUARDED_SKIP: {note}", status])

    def test_registration_matches_registry(self):
        nodes = {n["id"]: n for n in cap.load_graph(REPO / GRAPH_PATH)}
        node = nodes["rtl-exact"]
        self.assertEqual(node["check"], "rtl-exact-v1")
        check = cap.CHECKS[node["check"]]
        # the existing read-only comparison path, exactly as registered
        self.assertEqual(
            node["engine"], ["python3", "tools/functional_verification.py"]
        )
        self.assertEqual(list(check.command), node["engine"])
        self.assertNotIn("--write", check.command)
        self.assertEqual(node["prereqs"], ["rtl-core"])
        self.assertEqual(node["skip_policy"], "not_run")
        self.assertEqual(check.skip_token, self.fv.SKIP_TOKEN)
        for tool in (
            "tools/functional_verification.py",
            "tools/fv_cocotb_dx7_core.py",
            "tools/h07_compare.py",
            "tools/host_frozen_int.py",
            "tools/n08_build_release.py",
            "tools/registry_smoke.py",
            "tools/sha256_helpers.py",
            "tools/verdict_errors.py",
        ):
            self.assertIn(tool, check.inputs)
        covered = set(node["covered_inputs"])
        for pin in (
            "docs/decision-records/0012-exp-term-width-core-refreeze.md",
            "evidence/t1-functional/request.json",
            "evidence/t1-functional/report.json",
            "rtl/dx7_core.v",
            "rtl/env_unit.v",
            "rtl/alg_router.v",
            "rtl/tb_dx7_core.v",
            "reference/release/manifest.json",
            "reference/tables/alg_table.hex",
            "reference/tables/freqlut_table.hex",
            "reference/tables/sinexp_table.hex",
            "reference/tables/sinlog_table.hex",
            "spec/compatibility-registry-v1.json",
            "src/gf180_dx7/sysex.py",
        ):
            self.assertIn(pin, covered)
        # every model module the frozen stress golden is pinned against
        for module in sorted((REPO / "src" / "gf180_dx7" / "model").glob("*.py")):
            self.assertIn(module.relative_to(REPO).as_posix(), covered)
        # exactly the graded dev vectors, and every frozen stress segment
        dev = [c for c in self.selected if not c.startswith("stress-")]
        self.assertEqual(
            {p for p in covered if p.startswith("reference/release/vectors/")},
            {f"reference/release/vectors/{c}.f32" for c in dev},
        )
        stress = REPO / "evidence" / "h07-core" / "stress"
        self.assertEqual(
            {p for p in covered if p.startswith("evidence/h07-core/stress/")},
            {p.relative_to(REPO).as_posix() for p in sorted(stress.iterdir())},
        )
        controls = {
            c["id"]: c["command"] for c in node["required_negative_controls"]
        }
        self.assertEqual(
            controls,
            {
                "h07-glitch-mutant-detected": [
                    "python3",
                    "tools/functional_verification.py",
                    "--control",
                    "H07_MUTATE_GLITCH",
                ],
                "h07-sum-order-mutant-detected": [
                    "python3",
                    "tools/functional_verification.py",
                    "--control",
                    "H07_MUTATE_SUM_ORDER",
                ],
            },
        )
        self.assertEqual(
            sorted(self.fv.CONTROLS), ["H07_MUTATE_GLITCH", "H07_MUTATE_SUM_ORDER"]
        )
        for path in cap.coverage_paths(node):
            self.assertTrue(
                (REPO / path).is_file(), f"covered input missing: {path}"
            )

    def test_baseline_report_matches_lane_selection_and_pin(self):
        report = REPO / "evidence" / "t1-functional" / "report.json"
        self.assertEqual(
            hashlib.sha256(report.read_bytes()).hexdigest(),
            self.BASELINE_REPORT_SHA256,
        )
        provenance = self.committed["provenance"]
        self.assertEqual((len(self.selected), len(self.ungraded)), (23, 11))
        self.assertEqual(
            (
                sum(not c.startswith("stress-") for c in self.selected),
                sum(c.startswith("stress-") for c in self.selected),
            ),
            (19, 4),
        )
        self.assertEqual(set(provenance["cases"]), set(self.selected))
        self.assertEqual(provenance["not_graded"]["cases"], self.ungraded)
        self.assertEqual(provenance["not_graded"]["status"], "NOT_RUN")
        self.assertEqual(self.committed["skipped_count"], 0)
        self.assertTrue(all(t["status"] == "passed" for t in self.committed["tests"]))
        # the tree still is the DR-0012 pin the baseline names
        self.assertEqual(provenance["rtl_sha256"], self.hashes)
        self.assertTrue(provenance["dr0012_pin_matches"])
        self.assertEqual(
            self.fv.dr12_pin(self.fv.DR12.read_text(encoding="utf-8")),
            {k: self.hashes[k] for k in self.fv.RTL_PIN},
        )

    def test_partial_run_is_guarded_skip_not_pass(self):
        status, reasons, rows = self._verdict(self._fresh())
        self.assertEqual((status, reasons), ("PASS", []))
        self.assertTrue(all(r["verdict"] == "PASS" for r in rows))
        self.assertEqual(
            {r["case"]: r["checked"] for r in rows}, self._committed_counts()
        )
        # every selected window agreed, yet 11 dev cases stayed ungraded
        note = self.fv.skip_note(rows, self.ungraded)
        self.assertIsNotNone(note)
        self.assertIn("11 case(s) NOT_RUN", note)
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            self.fv.emit_guarded_skip(note)
        for cid in self.ungraded:
            self.assertIn(cid, out.getvalue())
        self.assertIn("GUARDED_SKIP", out.getvalue())
        # ... while a run that graded everything emits no token at all
        self.assertIsNone(self.fv.skip_note(rows, []))
        self.assertEqual(self.tool._result_for_exit(0), cap.PASS)

    def test_measured_mismatch_is_fail_with_divergence_despite_ungraded(self):
        fresh = self._fresh(fail={"case_stress_sum16": self.GLITCH})
        status, reasons, rows = self._verdict(fresh)
        self.assertEqual(status, "FAIL")
        self.assertIn(
            "stress-sum16: first divergence index=849 golden=23300 actual=23316"
            " block=11 sample=17",
            reasons,
        )
        row = next(r for r in rows if r["case"] == "stress-sum16")
        self.assertEqual(row["verdict"], "FAIL")
        self.assertEqual(row["divergence"]["index"], 849)
        self.assertEqual(
            (row["divergence"]["golden"], row["divergence"]["actual"]),
            (23300, 23316),
        )
        # the other cases stayed ungraded and the token still names them,
        # but the failing exit is FAIL at the stamp whatever the token says
        self.assertIn("11 case(s) NOT_RUN", self.fv.skip_note(rows, self.ungraded))
        self.assertEqual(self.fv.EXIT[status], 1)
        self.assertEqual(self.tool._result_for_exit(self.fv.EXIT[status]), cap.FAIL)

    def test_comparator_failure_without_sample_mismatch_is_still_fail(self):
        # overrun/overflow fail the comparator with an empty mismatch list
        message = (
            "stress-burst: RTL != frozen model; first mismatches [] "
            "overrun=1 overflow=0\nassert False"
        )
        status, reasons, rows = self._verdict(
            self._fresh(fail={"case_stress_burst": message})
        )
        self.assertEqual(status, "FAIL")
        row = next(r for r in rows if r["case"] == "stress-burst")
        self.assertEqual(
            (row["verdict"], row["divergence"], row["detail"]),
            ("FAIL", None, "overrun=1 overflow=0"),
        )

    def test_stale_pin_keeps_driver_diagnosis_over_any_rerun(self):
        drifted = dict(self.hashes)
        drifted["rtl/dx7_core.v"] = "0" * 64
        status, reasons, _ = self._verdict(
            self._fresh(fail={"case_stress_sum16": self.GLITCH}), drifted
        )
        self.assertEqual(status, "STALE")
        self.assertEqual(
            reasons,
            ["committed report names different RTL: ['rtl/dx7_core.v']"],
        )
        # exit 3 is a failing exit at the stamp: never PASS, never NOT_RUN
        self.assertEqual(self.fv.EXIT["STALE"], 3)
        self.assertEqual(self.tool._result_for_exit(self.fv.EXIT["STALE"]), cap.FAIL)
        # the diagnosis holds for a clean re-run too
        status, reasons, _ = self._verdict(self._fresh(), drifted)
        self.assertEqual(status, "STALE")

    def test_unexecuted_selected_case_is_not_run_naming_it(self):
        # zero simulator skips cannot establish coverage: an absent case
        status, reasons, rows = self._verdict(self._fresh(drop=("case_dir_base",)))
        self.assertEqual(status, "NOT_RUN")
        self.assertEqual(
            reasons,
            ["selected case(s) did not execute: dir-base (absent from the run)"],
        )
        self.assertEqual(
            [(r["case"], r["verdict"], r["detail"])
             for r in rows if r["verdict"] != "PASS"],
            [("dir-base", "NOT_RUN", "absent from the run")],
        )
        # the driver's "could not run" (a cocotb failed that never
        # reached the comparator) is unexecuted work, not a mismatch
        status, reasons, _ = self._verdict(
            self._fresh(fail={"case_dir_base":
                              "dir-base: could not run: simulation failed: x"})
        )
        self.assertEqual(status, "NOT_RUN")
        self.assertIn("dir-base (could not run)", reasons[0])
        status, reasons, _ = self._verdict(
            self._fresh(skip={"case_dir_base": "skipped"})
        )
        self.assertEqual(status, "NOT_RUN")
        self.assertIn("dir-base (status skipped)", reasons[0])
        # ... unless something else measured a mismatch: FAIL outranks it
        status, _, _ = self._verdict(self._fresh(
            drop=("case_dir_base",), fail={"case_stress_sum16": self.GLITCH}))
        self.assertEqual(status, "FAIL")

    def test_control_counts_only_a_measured_sample_mismatch(self):
        control = json.loads(
            (REPO / "evidence" / "t1-functional"
             / "control-H07_MUTATE_GLITCH.json").read_text()
        )
        want = ["stress-sum16"]
        ok, msg = self.fv.control_verdict(control, want)
        self.assertTrue(ok, msg)
        self.assertIn(
            "case_stress_sum16 index=849 golden=23300 actual=23316 block=11"
            " sample=17",
            msg,
        )

        def variant(**change):
            report = json.loads(json.dumps(control))
            report["tests"][0].update(change)
            return report

        # surviving mutant: the selected case passed under -DH07_MUTATE_GLITCH
        ok, msg = self.fv.control_verdict(variant(status="passed"), want)
        self.assertEqual(
            (ok, msg),
            (False, "mutant did NOT fail: {'case_stress_sum16': 'passed'}"),
        )
        # setup failure: cocotb says failed, but nothing reached the comparator
        ok, msg = self.fv.control_verdict(
            variant(error_message="stress-sum16: could not run: build failed"),
            want,
        )
        self.assertFalse(ok)
        self.assertIn(
            "no sample mismatch, the mutant did not reach the comparator:"
            " case_stress_sum16",
            msg,
        )
        # a comparator failure without a sample divergence is no detection
        ok, msg = self.fv.control_verdict(
            variant(error_message="stress-sum16: RTL != frozen model; first"
                                  " mismatches [] overrun=1 overflow=0"),
            want,
        )
        self.assertFalse(ok)
        self.assertIn("no sample mismatch", msg)
        # missing case: nothing ran, or another case ran instead
        for tests in ([], [dict(control["tests"][0], name="case_stress_burst")]):
            report = json.loads(json.dumps(control))
            report["tests"] = tests
            ok, msg = self.fv.control_verdict(report, want)
            self.assertFalse(ok)
            self.assertIn("but expected ['case_stress_sum16']", msg)

    def test_both_committed_mutant_controls_stay_live_under_the_rule(self):
        for name, note in (
            ("H07_MUTATE_GLITCH", "index=849 golden=23300 actual=23316"),
            ("H07_MUTATE_SUM_ORDER", "index=397 golden=2 actual=1024"),
        ):
            with self.subTest(control=name):
                report = json.loads(
                    (REPO / "evidence" / "t1-functional"
                     / f"control-{name}.json").read_text()
                )
                self.assertEqual(report["provenance"]["defines"], [name])
                ok, msg = self.fv.control_verdict(report, self.fv.CONTROLS[name])
                self.assertTrue(ok, msg)
                self.assertIn(note, msg)
                self.assertNotIn(self.fv.SKIP_TOKEN, msg)

    def test_coverage_record_round_trips_through_stamp_tail(self):
        _, _, rows = self._verdict(self._fresh())
        text = self._record_text(
            "PASS", rows, self.fv.skip_note(rows, self.ungraded)
        )
        tail = text[-2000:]
        record = self.fv.parse_coverage(tail)
        self.assertIsNotNone(record, "coverage record lost to the tail")
        self.assertEqual(record["verdict"], "PASS")
        self.assertEqual(
            (record["graded_count"], record["ungraded_count"]), (23, 11)
        )
        self.assertEqual([r["case"] for r in record["rows"]], self.selected)
        self.assertEqual(
            {r["case"]: r["checked"] for r in record["rows"]},
            self._committed_counts(),
        )
        self.assertEqual([r["verdict"] for r in record["rows"]], ["PASS"] * 23)
        self.assertEqual(record["ungraded"], self.ungraded)
        self.assertEqual(
            record["ungraded_reason"],
            self.committed["provenance"]["not_graded"]["reason"],
        )
        self.assertEqual(
            record["checked_samples"], sum(self._committed_counts().values())
        )
        # a tail that lost the header reports nothing rather than a
        # silently partial record
        self.assertIsNone(self.fv.parse_coverage(text[text.index("\n") + 1:]))

    def test_coverage_record_carries_divergence_and_unexecuted_cases(self):
        fresh = self._fresh(
            fail={
                "case_stress_sum16": self.GLITCH,
                "case_dir_base": "dir-base: could not run: playback failed",
            },
            drop=("case_stress_burst",),
            skip={"case_stress_reset": "skipped"},
        )
        status, _, rows = self._verdict(fresh)
        text = self._record_text(
            status, rows, self.fv.skip_note(rows, self.ungraded)
        )
        record = self.fv.parse_coverage(text[-2000:])
        self.assertEqual(record["verdict"], "FAIL")
        recovered = {r["case"]: r for r in record["rows"]}
        self.assertEqual(list(recovered), self.selected)
        self.assertEqual(
            recovered["stress-sum16"],
            {
                "case": "stress-sum16",
                "verdict": "FAIL",
                "checked": None,
                "detail": "index=849 golden=23300 actual=23316 block=11 sample=17",
            },
        )
        self.assertEqual(
            (recovered["dir-base"]["verdict"], recovered["dir-base"]["detail"]),
            ("NOT_RUN", "could not run"),
        )
        self.assertEqual(
            (recovered["stress-burst"]["verdict"],
             recovered["stress-burst"]["detail"]),
            ("NOT_RUN", "absent from the run"),
        )
        self.assertEqual(
            (recovered["stress-reset"]["verdict"],
             recovered["stress-reset"]["detail"]),
            ("NOT_RUN", "status skipped"),
        )
        self.assertEqual(
            (recovered["dir-base"]["checked"], recovered["alg4-fb0"]["checked"]),
            (None, self._committed_counts()["alg4-fb0"]),
        )
        note = self.fv.skip_note(rows, self.ungraded)
        self.assertIn(
            "selected but not executed: dir-base, stress-burst, stress-reset",
            note,
        )
        self.assertIn("14 case(s) NOT_RUN", note)

    def test_registered_command_missing_tool_is_not_run(self):
        """The real registered command with no klt venv: exit 2, the
        declared token, the committed report untouched."""
        report = REPO / "evidence" / "t1-functional" / "report.json"
        before = report.read_bytes()
        check = cap.CHECKS["rtl-exact-v1"]
        proc = subprocess.run(
            list(check.command) + ["--venv", "/nonexistent/venv-fv"],
            cwd=REPO,
            capture_output=True,
            text=True,
            timeout=120,
        )
        combined = proc.stdout + proc.stderr
        self.assertEqual(proc.returncode, 2, combined)
        self.assertIn("GUARDED_SKIP: check did not run:", combined)
        self.assertIn("klt missing", combined)
        self.assertNotIn(self.fv.COVERAGE_MARK, combined)
        self.assertEqual(report.read_bytes(), before)
        # the control path has no token to emit: not detected, never PASS
        for name in ("H07_MUTATE_GLITCH", "H07_MUTATE_SUM_ORDER"):
            proc = subprocess.run(
                list(check.command)
                + ["--control", name, "--venv", "/nonexistent/venv-fv"],
                cwd=REPO,
                capture_output=True,
                text=True,
                timeout=120,
            )
            combined = proc.stdout + proc.stderr
            self.assertEqual(proc.returncode, 2, combined)
            self.assertIn("NOT_RUN:", combined)
            self.assertNotIn(self.fv.SKIP_TOKEN, combined)
            self.assertNotIn("CONTROL_OK", combined)
        self.assertEqual(report.read_bytes(), before)

    def test_committed_stamp_reads_not_run_and_retains_coverage(self):
        record = json.loads(self.STAMP.read_text())
        self.assertEqual(record["node_id"], "rtl-exact")
        self.assertEqual(record["check"], "rtl-exact-v1")
        self.assertEqual(record["argv"], list(cap.CHECKS["rtl-exact-v1"].command))
        # the 23 windows agreed (exit 0) and the other 11 stayed ungraded:
        # a guarded skip under skip_policy not_run, never a PASS
        self.assertEqual(record["result"], cap.NOT_RUN)
        self.assertEqual(record["exit_code"], 0)
        self.assertIs(record["check_skip_reported"], True)
        tail = record["check_output"]["tail"]
        self.assertIn("GUARDED_SKIP: 11 case(s) NOT_RUN", tail)
        # the complete coverage record is recoverable from the bounded tail
        coverage = self.fv.parse_coverage(tail)
        self.assertIsNotNone(coverage, "coverage record lost to the tail")
        self.assertEqual(coverage["verdict"], "PASS")
        self.assertEqual(
            (coverage["graded_count"], coverage["ungraded_count"]), (23, 11)
        )
        self.assertEqual([r["case"] for r in coverage["rows"]], self.selected)
        self.assertEqual(
            {r["case"]: r["checked"] for r in coverage["rows"]},
            self._committed_counts(),
        )
        self.assertEqual([r["verdict"] for r in coverage["rows"]], ["PASS"] * 23)
        self.assertEqual(
            coverage["ungraded"],
            self.committed["provenance"]["not_graded"]["cases"],
        )
        self.assertEqual(
            coverage["ungraded_reason"],
            self.committed["provenance"]["not_graded"]["reason"],
        )
        self.assertEqual(
            coverage["checked_samples"], sum(self._committed_counts().values())
        )
        # both mutant controls ran, reached the comparator, and diverged
        controls = {tuple(c["command"]): c for c in record["controls"]}
        self.assertEqual(len(controls), 2)
        for name, note in (
            ("H07_MUTATE_GLITCH", "index=849 golden=23300 actual=23316"),
            ("H07_MUTATE_SUM_ORDER", "index=397 golden=2 actual=1024"),
        ):
            with self.subTest(control=name):
                control = controls[
                    (
                        "python3",
                        "tools/functional_verification.py",
                        "--control",
                        name,
                    )
                ]
                self.assertTrue(control["detected"])
                self.assertEqual(control["exit_code"], 0)
                self.assertIn("CONTROL_OK", control["output"]["tail"])
                self.assertIn(note, control["output"]["tail"])
                self.assertIsNone(control["skipped"])
        # inputs bound by content: the baseline report is byte-identical
        # before and after stamping and identical to the registered pin
        self.assertEqual(
            record["coverage"]["evidence/t1-functional/report.json"],
            self.BASELINE_REPORT_SHA256,
        )
        self.assertEqual(
            record["coverage"]["evidence/t1-functional/request.json"],
            hashlib.sha256(
                (REPO / "evidence" / "t1-functional" / "request.json").read_bytes()
            ).hexdigest(),
        )
        self.assertEqual(
            record["coverage"]["rtl/dx7_core.v"], self.hashes["rtl/dx7_core.v"]
        )
        # local verdict and effective DAG verdict
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        results = cap.evaluate(nodes, REPO)
        self.assertEqual(results["rtl-exact"].local_state, cap.NOT_RUN)
        self.assertEqual(results["rtl-exact"].state, cap.BLOCKED)
        self.assertIn(
            "prerequisite not demonstrated: rtl-core=BLOCKED",
            results["rtl-exact"].reasons,
        )
        self.assertIn(
            "recorded check reported a guarded skip and did not do all of its"
            " work (exit_code=0)",
            results["rtl-exact"].reasons,
        )

    def test_changed_covered_input_invalidates_the_stamp(self):
        """A byte change in any covered input (RTL, the baseline report,
        the driver, a frozen vector) after stamping -> STALE locally and
        BLOCKED (never PASS/NOT_RUN) in the DAG."""
        nodes = cap.load_graph(REPO / GRAPH_PATH)
        node = next(n for n in nodes if n["id"] == "rtl-exact")
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            for relative in cap.coverage_paths(node):
                dest = root / relative
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_bytes((REPO / relative).read_bytes())
            (root / cap.EVIDENCE_DIR).mkdir(parents=True, exist_ok=True)
            for path in sorted((REPO / cap.EVIDENCE_DIR).glob("*.json")):
                shutil.copy(path, root / cap.EVIDENCE_DIR / path.name)
            results = cap.evaluate(nodes, root)
            self.assertEqual(
                (results["rtl-exact"].local_state, results["rtl-exact"].state),
                (cap.NOT_RUN, cap.BLOCKED),
            )
            for relative in (
                "rtl/dx7_core.v",
                "evidence/t1-functional/report.json",
                "tools/fv_cocotb_dx7_core.py",
                "reference/release/vectors/alg4-fb0.f32",
                "evidence/h07-core/stress/stress-sum16.seg0.i32",
                "docs/decision-records/0012-exp-term-width-core-refreeze.md",
            ):
                with self.subTest(changed=relative):
                    path = root / relative
                    pristine = path.read_bytes()
                    path.write_bytes(pristine + b"\n")
                    result = cap.evaluate(nodes, root)["rtl-exact"]
                    self.assertEqual(result.local_state, cap.STALE)
                    self.assertEqual(result.state, cap.BLOCKED)
                    self.assertEqual(
                        result.reasons[0], "covered bytes changed: " + relative
                    )
                    path.write_bytes(pristine)

    def test_three_registrations_visible_and_prerequisites_propagate(self):
        """fixed-model (#274), rtl-core (#275) and rtl-exact (#276) each
        carry a registered check and a stamped record in the committed
        views, and each stays BLOCKED behind an undemonstrated
        prerequisite, rooted at oracle-identity."""
        nodes = {n["id"]: n for n in cap.load_graph(REPO / GRAPH_PATH)}
        results = cap.evaluate(list(nodes.values()), REPO)
        view = json.loads((REPO / "docs" / "capabilities.json").read_text())
        view_nodes = {n["id"]: n for n in view["nodes"]}
        markdown = (REPO / "docs" / "CAPABILITIES.md").read_text()
        chain = ("fixed-model", "rtl-core", "rtl-exact")
        for node_id in chain:
            with self.subTest(node=node_id):
                node, result = nodes[node_id], results[node_id]
                self.assertIn(node["check"], cap.CHECKS)
                self.assertTrue(
                    (REPO / cap.node_evidence_path(node_id)).is_file()
                )
                self.assertIn(result.local_state, (cap.PASS, cap.NOT_RUN))
                self.assertEqual(result.state, cap.BLOCKED)
                undemonstrated = [
                    p for p in node["prereqs"] if results[p].state != cap.PASS
                ]
                self.assertTrue(undemonstrated)
                # evaluate() joins every blocked prerequisite into ONE
                # reason string, so each undemonstrated prerequisite is
                # asserted as a "prereq=STATE" fragment of it
                blocked_reasons = " ".join(
                    r for r in result.reasons
                    if r.startswith("prerequisite not demonstrated")
                )
                self.assertTrue(blocked_reasons)
                for prereq in undemonstrated:
                    self.assertIn(
                        f"{prereq}={results[prereq].state}", blocked_reasons
                    )
                shown = view_nodes[node_id]
                self.assertEqual(shown["check"], node["check"])
                self.assertEqual(
                    (shown["local_state"], shown["state"]),
                    (result.local_state, result.state),
                )
                section = markdown.split(f"## {node_id} ", 1)[1].split("\n## ", 1)[0]
                self.assertIn(f"- Check: `{node['check']}`", section)
                self.assertIn(f"- Evidence: `{cap.node_evidence_path(node_id)}`.", section)
                self.assertNotIn("not registered", section)
        # the chain in order, rooted at the oracle-dependent nodes: every
        # registration stays behind oracle-identity until it is demonstrated
        self.assertEqual(nodes["rtl-core"]["prereqs"], ["fixed-model"])
        self.assertEqual(nodes["rtl-exact"]["prereqs"], ["rtl-core"])
        ancestors = set()
        frontier = ["rtl-exact"]
        while frontier:
            for prereq in nodes[frontier.pop()]["prereqs"]:
                if prereq not in ancestors:
                    ancestors.add(prereq)
                    frontier.append(prereq)
        for root_id in ("oracle-identity", "comparator", "fixed-model", "rtl-core"):
            self.assertIn(root_id, ancestors)
            self.assertNotEqual(results[root_id].state, cap.PASS)
        # the partial-run registration itself reads NOT_RUN, not PASS
        self.assertEqual(view_nodes["rtl-exact"]["local_state"], cap.NOT_RUN)


if __name__ == "__main__":
    unittest.main()
