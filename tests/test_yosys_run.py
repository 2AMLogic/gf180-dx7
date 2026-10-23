"""Tests for the shared yosys invocation helpers (issue #146).

Uses fake yosys executables only: no real yosys run happens here, so this
establishes the helper's contract and the per-tool exception wiring, not
that live yosys logs are unchanged.
"""

import os
import stat
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
if str(REPO / "tools") not in sys.path:
    sys.path.insert(0, str(REPO / "tools"))

import yosys_run  # noqa: E402

TOOLS = ("h01", "h04", "h05", "h06", "h07", "h08", "h10",
         "storage_probe")


class Boom(Exception):
    pass


class Fail(Exception):
    pass


def fake_yosys(td, body):
    p = os.path.join(td, "fake_yosys")
    with open(p, "w", encoding="utf-8") as f:
        f.write("#!/bin/sh\n" + body + "\n")
    os.chmod(p, os.stat(p).st_mode | stat.S_IXUSR)
    return p


class YosysVersionTest(unittest.TestCase):
    def test_returns_stripped_stdout(self):
        with tempfile.TemporaryDirectory() as td:
            y = fake_yosys(td, "echo 'Yosys 0.99 (git sha1 abc)'")
            self.assertEqual(yosys_run.yosys_version(y, Boom),
                             "Yosys 0.99 (git sha1 abc)")

    def test_missing_binary_raises_caller_class(self):
        with self.assertRaises(Boom) as cm:
            yosys_run.yosys_version("/nonexistent/yosys", Boom)
        self.assertIn("yosys binary not found", str(cm.exception))

    def test_nonzero_and_empty_raise_caller_class(self):
        with tempfile.TemporaryDirectory() as td:
            bad = fake_yosys(td, "echo oops >&2; exit 3")
            with self.assertRaises(Boom) as cm:
                yosys_run.yosys_version(bad, Boom)
            self.assertIn("yosys -V failed: oops", str(cm.exception))
            empty = fake_yosys(td, "exit 0")
            with self.assertRaises(Boom):
                yosys_run.yosys_version(empty, Boom)


class RunYosysScriptTest(unittest.TestCase):
    def test_success_writes_header_and_returns_log(self):
        with tempfile.TemporaryDirectory() as td:
            y = fake_yosys(td, 'echo "out:$2"; echo err >&2; pwd')
            logp = os.path.join(td, "run.log")
            log = yosys_run.run_yosys_script(y, ["a", "b"], logp, td, Fail)
            self.assertIn("out:a; b", log)
            self.assertIn("err", log)
            with open(logp, encoding="utf-8") as f:
                text = f.read()
            self.assertEqual(
                text.splitlines()[0],
                f"# yosys script (cwd={td}): a; b")
            self.assertEqual(text, f"# yosys script (cwd={td}): a; b\n"
                             + log)

    def test_failure_raises_caller_class_with_last_line(self):
        with tempfile.TemporaryDirectory() as td:
            y = fake_yosys(td, "echo first; echo ERROR: bad thing; exit 2")
            logp = os.path.join(td, "run.log")
            with self.assertRaises(Fail) as cm:
                yosys_run.run_yosys_script(y, ["x"], logp, td, Fail)
            msg = str(cm.exception)
            self.assertIn("rc=2", msg)
            self.assertIn(logp, msg)
            self.assertTrue(msg.endswith("ERROR: bad thing"))
            self.assertTrue(os.path.isfile(logp))  # log kept on failure

    def test_failure_with_empty_log_still_raises(self):
        with tempfile.TemporaryDirectory() as td:
            y = fake_yosys(td, "exit 1")
            with self.assertRaises(Fail):
                yosys_run.run_yosys_script(
                    y, ["x"], os.path.join(td, "l.log"), td, Fail)


class ToolWiringTest(unittest.TestCase):
    """Each tool's wrapper must raise ITS OWN CouldNotRun class."""

    def test_wrappers_raise_tool_local_could_not_run(self):
        for name in TOOLS:
            with self.subTest(tool=name):
                mod = __import__(f"{name}_synth")
                with self.assertRaises(mod.CouldNotRun):
                    mod.yosys_version("/nonexistent/yosys")

    def test_run_yosys_wrappers_raise_tool_local_check_failure(self):
        # Control: the failure class is the tool's, not a shared one.
        with tempfile.TemporaryDirectory() as td:
            y = fake_yosys(td, "exit 1")
            for name, args in (("h05", (y, td, "lib", False)),
                               ("h06", (y, td, "lib", False)),
                               ("h07", (y, "lib", False)),
                               ("h08", (y, "lib", False))):
                with self.subTest(tool=name):
                    mod = __import__(f"{name}_synth")
                    with self.assertRaises(mod.CheckFailure):
                        mod.run_yosys(*args, os.path.join(td, "l.log"))


class RomRepresentationTest(unittest.TestCase):
    MAP = "MEMORY_MAP pass (converting memories to logic\n"
    MEM = "     2   $mem_v2\n"

    def test_logic_when_mapped_and_no_mem_cells(self):
        r = yosys_run.rom_representation(self.MAP, "my detail")
        self.assertEqual(r, {
            "representation": "logic", "detail": "my detail",
            "evidence": {"memory_map_pass_ran": True,
                         "memory_cells_in_final_stat": 0}})

    def test_mem_cells_never_classify_as_logic(self):
        # Control: a surviving $mem cell must defeat the logic verdict.
        r = yosys_run.rom_representation(self.MAP + self.MEM, "my detail")
        self.assertEqual(r["representation"], "memory cells")
        self.assertEqual(r["detail"], "inspect the committed yosys log")
        self.assertIs(r["evidence"]["memory_cells_in_final_stat"], True)

    def test_unknown_when_nothing_ran(self):
        r = yosys_run.rom_representation("nothing\n", "d")
        self.assertEqual(r["representation"], "unknown")
        self.assertEqual(r["evidence"], {"memory_map_pass_ran": False,
                                         "memory_cells_in_final_stat": False})

    def test_inferred_logic_branch_is_unreachable_by_design(self):
        # memory_map and not mem_cells is always "logic"; "logic (inferred)"
        # is kept verbatim from the old copies and cannot be produced.
        self.assertNotEqual(
            yosys_run.rom_representation(self.MAP, "d")["representation"],
            "logic (inferred)")

    def test_readmem_opt_in_only_in_fallback_evidence(self):
        log = "$readmemh x\n"
        off = yosys_run.rom_representation(log, "d")
        on = yosys_run.rom_representation(log, "d", readmem=True)
        self.assertNotIn("readmem_seen", off["evidence"])
        self.assertIs(on["evidence"]["readmem_seen"], True)
        self.assertEqual(list(on["evidence"]), [
            "memory_map_pass_ran", "readmem_seen",
            "memory_cells_in_final_stat"])
        # the logic branch never carries it, matching the old copies
        self.assertNotIn("readmem_seen", yosys_run.rom_representation(
            self.MAP, "d", readmem=True)["evidence"])

    def test_tool_blocks_match_committed_evidence(self):
        # The refactor must reproduce the committed rom_representation
        # dicts exactly (no yosys re-run; this replays the logic branch).
        import json
        for name, ev in (("h01", "h01-operator-probe"),
                         ("h05", "h05-routing-rtl"),
                         ("h06", "h06-pitch-rtl"),
                         ("h07", "h07-core")):
            with self.subTest(tool=name):
                mod = __import__(f"{name}_synth")
                path = REPO / "evidence" / ev / "synth_report.json"
                want = json.loads(path.read_text(encoding="utf-8"))[
                    "runs"]["full"]["rom_representation"]
                self.assertEqual(mod.rom_representation(self.MAP), want)


if __name__ == "__main__":
    unittest.main()
