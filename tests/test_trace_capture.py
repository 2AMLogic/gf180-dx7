"""Tests for the R05 non-invasive trace capture (issue #12).

End-to-end tests need the external GPL oracle (pinned in
reference/oracle-identity.json) and skip with the missing path named when it
is absent. Registry-schema, trace-format, and negative-control gate tests run
everywhere (stdlib-only): the negative controls drive stub oracles crafted in
a temp directory — one that writes traces AND perturbs the PCM must be
REJECTED by the byte-identity gate with no artifacts written.
"""

from __future__ import annotations

import hashlib
import importlib.util
import json
import stat
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]

TOOL = REPO / "tools" / "capture_trace.py"
IDENTITY = REPO / "reference" / "oracle-identity.json"
REGISTRY = REPO / "reference" / "trace-registry.json"
COMMITTED_TRACE = REPO / "reference" / "traces" / "smoke.jsonl"
COMMITTED_META = REPO / "reference" / "traces" / "smoke-meta.json"
FIXTURE_DIR = REPO / "reference" / "fixtures" / "smoke"

with open(IDENTITY, "r", encoding="utf-8") as fh:
    _IDENTITY = json.load(fh)
ORACLE = Path(_IDENTITY["binary"]["path"])

ORACLE_ABSENT_MSG = (
    f"external oracle binary absent at {ORACLE} (pinned in "
    "reference/oracle-identity.json)")

BLOCKS = 192000 // 64
FIELDS = {"block", "lfo_value", "lfo_delay", "live_mask", "keydown_mask",
          "active_notes"}
AVAILABLE_TAPS = {"events_applied", "lfo_value", "lfo_delay", "live_mask",
                  "keydown_mask", "active_notes", "final_pcm"}
UNAVAILABLE_TAPS = {"op_phase", "op_env_stage", "op_env_level",
                    "feedback_history", "op_output"}


def load_tool():
    spec = importlib.util.spec_from_file_location("capture_trace", TOOL)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def sha256_file(path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def write_stub(root: Path, name: str, perturbs: bool, breaks_trace: bool):
    """Craft a tiny fake oracle in the temp dir. The perturbing variant is
    the R05 negative control: it writes plausible traces AND emits different
    PCM when --trace is present — the byte-identity gate must reject it. The
    breaks_trace variant emits identical PCM but a malformed trace (missing
    required field), which the trace gate must reject."""
    extra = "            del rec['lfo_delay']\n" if breaks_trace else ""
    amp_line = ("amp = 0.25 + (0.001 if trace else 0.0)\n" if perturbs
                else "amp = 0.25\n")
    stub = root / name
    stub.write_text(
        "#!/usr/bin/env python3\n"
        "import json, math, struct, sys\n"
        "args = sys.argv[1:]\n"
        "def get(flag, default=None):\n"
        "    return args[args.index(flag) + 1] if flag in args "
        "else default\n"
        "rate = int(get('--rate', '48000'))\n"
        "seconds = float(get('--seconds', '4'))\n"
        "n = int(rate * seconds)\n"
        "trace = get('--trace')\n"
        + amp_line +
        "pcm = struct.pack('<%df' % n,\n"
        "                  *[amp * math.sin(i * 0.01) for i in range(n)])\n"
        "with open(get('--out'), 'wb') as fh:\n"
        "    fh.write(pcm)\n"
        "if trace:\n"
        "    blocks = n // 64\n"
        "    with open(trace, 'w') as tf:\n"
        "        for k in range(blocks):\n"
        "            rec = {'block': k, 'lfo_value': 1000 + k,\n"
        "                   'lfo_delay': 5,\n"
        "                   'live_mask': 1 if k >= 2 else 0,\n"
        "                   'keydown_mask': 0,\n"
        "                   'active_notes': 1 if k >= 2 else 0}\n"
        + extra
        + "            tf.write(json.dumps(rec) + '\\n')\n",
        encoding="utf-8")
    stub.chmod(stub.stat().st_mode | stat.S_IEXEC)
    return stub


class StubMixin:
    """Inputs for stub runs: run_capture requires existing fixture files."""

    def make_inputs(self, root: Path):
        voice = root / "voice.syx"
        events = root / "events.txt"
        voice.write_bytes(b"\xf0" + b"\x00" * 161 + b"\xf7")
        events.write_text("0 note_on 60 100\n", encoding="utf-8")
        return voice, events

    def run_stub(self, mod, root: Path, stub: Path, artifacts: Path):
        registry = mod.load_registry(REGISTRY)
        voice, events = self.make_inputs(root)
        artifacts.mkdir(parents=True, exist_ok=True)
        return mod.run_capture(
            stub, voice, events, registry, REGISTRY,
            artifacts / "trace.jsonl", artifacts / "trace-meta.json",
            expected_binary_sha=None, identity=None, fixture="stub")


class TestRegistrySchema(unittest.TestCase):
    """No oracle needed: the versioned tap registry is spec data."""

    def test_committed_registry_valid_and_complete(self):
        mod = load_tool()
        registry = mod.load_registry(REGISTRY)  # raises on schema errors
        self.assertGreaterEqual(registry["registry_version"], 1)
        names = {t["name"] for t in registry["taps"]}
        self.assertTrue(AVAILABLE_TAPS <= names, "missing available taps")
        self.assertTrue(UNAVAILABLE_TAPS <= names,
                        "missing documented-unavailable taps")
        self.assertEqual(AVAILABLE_TAPS | UNAVAILABLE_TAPS, names,
                         "unexpected extra taps (registry must stay honest)")
        by_name = {t["name"]: t for t in registry["taps"]}
        for name in AVAILABLE_TAPS:
            self.assertTrue(by_name[name]["available"], name)
            self.assertTrue(by_name[name]["semantics"])
            self.assertTrue(by_name[name]["stream"], name)
        for name in UNAVAILABLE_TAPS:
            tap = by_name[name]
            self.assertFalse(tap["available"], name)
            # Unavailable taps must be explicit, never silently omitted.
            self.assertTrue(
                any(k in tap["reason"]
                    for k in ("msfa", "EngineMkI", "engine-internal")),
                f"{name}: reason must name the engine-internal location")
            self.assertTrue(tap["requirements_to_make_available"], name)
            self.assertEqual(tap["status"], "planned", name)
        # Cross-process stability must be declared per tap (R05 finding:
        # upstream msfa leaves the LFO start state uninitialized).
        for name in ("lfo_value", "lfo_delay"):
            tap = by_name[name]
            self.assertEqual(tap["stability"],
                             "instance-dependent across oracle processes",
                             name)
            self.assertTrue(tap["stability_reason"], name)
        for name in ("live_mask", "keydown_mask", "active_notes",
                     "events_applied", "final_pcm"):
            self.assertEqual(
                by_name[name]["stability"],
                "deterministic across processes", name)
        self.assertIn("byte-identical", registry["non_invasiveness_rule"])

    def test_unavailable_tap_without_reason_rejected(self):
        mod = load_tool()
        registry = json.loads(REGISTRY.read_text(encoding="utf-8"))
        registry["taps"][7].pop("reason")
        with self.assertRaises(mod.CaptureFail):
            mod.validate_registry(registry)

    def test_duplicate_tap_name_rejected(self):
        mod = load_tool()
        registry = json.loads(REGISTRY.read_text(encoding="utf-8"))
        registry["taps"].append(dict(registry["taps"][0]))
        with self.assertRaises(mod.CaptureFail):
            mod.validate_registry(registry)

    def test_bad_granularity_and_missing_stream_rejected(self):
        mod = load_tool()
        registry = json.loads(REGISTRY.read_text(encoding="utf-8"))
        registry["taps"][1]["granularity"] = "sometimes"
        with self.assertRaises(mod.CaptureFail):
            mod.validate_registry(registry)
        registry = json.loads(REGISTRY.read_text(encoding="utf-8"))
        registry["taps"][1].pop("stream")
        with self.assertRaises(mod.CaptureFail):
            mod.validate_registry(registry)

    def test_missing_non_invasiveness_rule_rejected(self):
        mod = load_tool()
        registry = json.loads(REGISTRY.read_text(encoding="utf-8"))
        registry.pop("non_invasiveness_rule")
        with self.assertRaises(mod.CaptureFail):
            mod.validate_registry(registry)


class TestTraceFormat(unittest.TestCase):
    """No oracle needed: the JSONL gate validates structure and semantics."""

    def good_line(self, block: int) -> dict:
        return {"block": block, "lfo_value": 1000 + block, "lfo_delay": 5,
                "live_mask": 1 if block >= 2 else 0, "keydown_mask": 0,
                "active_notes": 1 if block >= 2 else 0}

    def write_trace(self, path: Path, lines) -> None:
        path.write_text(
            "".join(json.dumps(ln) + "\n" for ln in lines),
            encoding="utf-8")

    def test_valid_trace_parses(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.jsonl"
            self.write_trace(path, [self.good_line(k) for k in range(3)])
            records = mod.parse_trace(path, 3)
            self.assertEqual(len(records), 3)
            self.assertEqual(records[2]["active_notes"], 1)

    def test_wrong_block_count_rejected(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.jsonl"
            self.write_trace(path, [self.good_line(k) for k in range(3)])
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 4)

    def test_out_of_sequence_block_rejected(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.jsonl"
            lines = [self.good_line(0), self.good_line(2)]
            self.write_trace(path, lines)
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 2)

    def test_stale_active_notes_rejected(self):
        """Negative control: a tap stub returning stale/zero values must be
        detected (issue #12) — here a live_mask of 1 with active_notes 0."""
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.jsonl"
            bad = self.good_line(0)
            bad["live_mask"] = 1
            self.write_trace(path, [bad])
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 1)

    def test_out_of_range_mask_and_missing_field_rejected(self):
        mod = load_tool()
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.jsonl"
            bad = self.good_line(0)
            bad["live_mask"] = 0x10000
            self.write_trace(path, [bad])
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 1)
            bad = self.good_line(0)
            bad.pop("lfo_delay")
            self.write_trace(path, [bad])
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 1)
            path.write_text("not json\n", encoding="utf-8")
            with self.assertRaises(mod.CaptureFail):
                mod.parse_trace(path, 1)


class TestStubGate(unittest.TestCase, StubMixin):
    """Byte-identity gate proven with stub oracles (no oracle needed)."""

    def test_clean_stub_passes_and_writes_artifacts(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            stub = write_stub(root, "clean_oracle.py", perturbs=False,
                              breaks_trace=False)
            artifacts = root / "artifacts"
            mod = load_tool()
            meta = self.run_stub(mod, root, stub, artifacts)
            self.assertTrue(meta["byte_identity"]["identical"])
            self.assertEqual(meta["trace"]["blocks"], BLOCKS)
            self.assertTrue((artifacts / "trace.jsonl").is_file())
            self.assertTrue((artifacts / "trace-meta.json").is_file())
            lines = (artifacts / "trace.jsonl").read_text(
                encoding="utf-8").splitlines()
            self.assertEqual(len(lines), BLOCKS)
            rec = json.loads(lines[2])
            self.assertEqual(set(rec), FIELDS)
            self.assertEqual(rec["active_notes"],
                             bin(rec["live_mask"]).count("1"))

    def test_perturbing_stub_rejected_with_no_artifacts(self):
        """NEGATIVE CONTROL: a stub that writes traces AND perturbs the PCM
        must be rejected by the non-invasiveness gate (evidence refused)."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            stub = write_stub(root, "perturbing_oracle.py", perturbs=True,
                              breaks_trace=False)
            artifacts = root / "artifacts"
            mod = load_tool()
            with self.assertRaises(mod.CaptureFail) as ctx:
                self.run_stub(mod, root, stub, artifacts)
            self.assertIn("non-invasiveness", str(ctx.exception))
            self.assertFalse((artifacts / "trace.jsonl").exists(),
                             "trace written despite perturbed PCM")
            self.assertFalse((artifacts / "trace-meta.json").exists())

    def test_malformed_trace_stub_rejected_with_no_artifacts(self):
        """A gate that only checked PCM would accept a broken instrument:
        identical PCM but a trace missing a required field must also be
        rejected, with nothing written."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            stub = write_stub(root, "broken_trace_oracle.py",
                              perturbs=False, breaks_trace=True)
            artifacts = root / "artifacts"
            mod = load_tool()
            with self.assertRaises(mod.CaptureFail):
                self.run_stub(mod, root, stub, artifacts)
            self.assertFalse((artifacts / "trace.jsonl").exists())
            self.assertFalse((artifacts / "trace-meta.json").exists())

    def test_cli_rejects_unpinned_oracle(self):
        """The CLI always pins: a hash-mismatched binary must FAIL (exit 1)
        before any capture, with no artifacts written."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            stub = write_stub(root, "clean_oracle.py", perturbs=False,
                              breaks_trace=False)
            artifacts = root / "traces"
            proc = subprocess.run(
                [sys.executable, str(TOOL), "--fixture", "smoke",
                 "--oracle", str(stub), "--traces-dir", str(artifacts)],
                capture_output=True, text=True, cwd=REPO)
            self.assertEqual(proc.returncode, 1, proc.stdout + proc.stderr)
            self.assertIn("identity mismatch", proc.stderr)
            self.assertFalse(artifacts.exists())


@unittest.skipUnless(ORACLE.is_file(), ORACLE_ABSENT_MSG)
class TestRealOracleCapture(unittest.TestCase):
    """Full CLI capture against the pinned oracle + committed-evidence
    cross-check."""

    def test_cli_capture_matches_committed_evidence(self):
        with tempfile.TemporaryDirectory() as tmp:
            traces = Path(tmp) / "traces"
            proc = subprocess.run(
                [sys.executable, str(TOOL), "--fixture", "smoke",
                 "--traces-dir", str(traces)],
                capture_output=True, text=True, cwd=REPO)
            self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
            fresh_trace = traces / "smoke.jsonl"
            fresh_meta = traces / "smoke-meta.json"
            self.assertTrue(fresh_trace.is_file())
            self.assertTrue(fresh_meta.is_file())
            lines = fresh_trace.read_text(encoding="utf-8").splitlines()
            self.assertEqual(len(lines), BLOCKS)
            first = json.loads(lines[0])
            self.assertEqual(set(first), FIELDS)
            meta = json.loads(fresh_meta.read_text(encoding="utf-8"))
            self.assertTrue(meta["byte_identity"]["identical"])
            self.assertTrue(meta["non_invasiveness"].startswith("PASS"))
            self.assertTrue(meta["identity_verified"])
            self.assertEqual(
                meta["oracle_binary"]["sha256"],
                _IDENTITY["binary"]["sha256"])
            # Fresh capture must reproduce the committed evidence on every
            # cross-process-comparable axis: the deterministic trace columns
            # (lfo_value/lfo_delay are instance-dependent — upstream msfa
            # leaves the LFO start state uninitialized; see the registry)
            # and the PCM.
            self.assertTrue(COMMITTED_TRACE.is_file())
            committed_meta = json.loads(
                COMMITTED_META.read_text(encoding="utf-8"))
            self.assertEqual(
                meta["trace"]["stability"]["deterministic_columns_sha256"],
                committed_meta["trace"]["stability"]
                ["deterministic_columns_sha256"],
                "deterministic trace columns disagree with committed "
                "smoke evidence (oracle drift?)")
            self.assertEqual(
                meta["pcm"]["untraced"]["sha256"],
                committed_meta["pcm"]["untraced"]["sha256"])
            self.assertEqual(
                meta["trace"]["first_live_block"],
                committed_meta["trace"]["first_live_block"])
            # The traced PCM is the committed R02 render evidence of record.
            self.assertEqual(
                meta["pcm"]["untraced"]["sha256"],
                "1bebaf668e82ae5885750566af22ca47cbb4ed9403e6fc121b349c5fc0bd8e9a")

    def test_two_captures_agree_on_deterministic_columns(self):
        """Reference-vs-reference repeatability, stated precisely: the
        deterministic columns and PCM repeat exactly across fresh capture
        runs; the whole trace file may differ because the LFO columns are
        instance-dependent (documented, see registry)."""
        mod = load_tool()
        results = []
        with tempfile.TemporaryDirectory() as tmp:
            for run in ("a", "b"):
                traces = Path(tmp) / run
                proc = subprocess.run(
                    [sys.executable, str(TOOL), "--fixture", "smoke",
                     "--traces-dir", str(traces)],
                    capture_output=True, text=True, cwd=REPO)
                self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
                meta = json.loads((traces / "smoke-meta.json")
                                  .read_text(encoding="utf-8"))
                results.append(meta)
        a, b = results
        self.assertEqual(
            a["trace"]["stability"]["deterministic_columns_sha256"],
            b["trace"]["stability"]["deterministic_columns_sha256"])
        self.assertEqual(a["pcm"]["untraced"]["sha256"],
                         b["pcm"]["untraced"]["sha256"])
        self.assertEqual(a["trace"]["live_blocks"], b["trace"]["live_blocks"])


if __name__ == "__main__":
    unittest.main()
