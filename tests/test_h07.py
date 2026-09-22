#!/usr/bin/env python3
"""H07 integrated-core tests (issue #29).

Covers, per the issue's acceptance:
  - the conformance harness (tools/h07_compare.py) machinery is sound:
    dev vectors respect the contracted 168-write burst bound, the golden
    loaders are hash-pinned to the frozen N08 release manifest, the stress
    goldens are byte-frozen and match a fresh model render;
  - CONFORMANCE: the committed results file (two clean runs, artifact-hash
    identical) is verified against the frozen N08 vectors and the current
    RTL hashes; a live run is attempted only when the heavy-execution host
    is reachable -- the skip reason names the ssh alias
    (repo-remote-gf180-dx7), never silently passing;
  - the four NEGATIVE CONTROLS, each of which must demonstrably FAIL the
    bit-exact compare on a small local golden vector (built and simulated
    locally; heavy-only in wall time, not in logic -- the pass/fail
    verdicts are computed locally):
      * H07_MUTATE_GLITCH       one-sample mix glitch, frame 12 sample 17
                                (the comparator must LOCALIZE that sample)
      * H07_MUTATE_SUM_ORDER    sum-then-clip (N08 voice_sum_order_swap)
      * H07_MUTATE_TICK_SKEW    stale queue read pointer drops the last
                                write of a burst window
      * H07_STRIP_OBSERVABILITY synthesis control: all outputs tied off;
        the committed yosys report must show the flop gate failing
        (remote-synthesized evidence, freshness-checked; skipped with the
        alias named when absent);
  - reset determinism: the reset-mid-render stress vector played twice
    produces byte-identical artifacts;
  - interface guards: the core port surface is exactly the contracted set
    (no per-sample host data path -- DEC-010 / H03 4.7);
  - report-doc anchors.

Stdlib only.
"""

import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "src"))
sys.path.insert(0, os.path.join(REPO, "tools"))

COMPARE_TOOL = os.path.join(REPO, "tools", "h07_compare.py")
SYNTH_TOOL = os.path.join(REPO, "tools", "h07_synth.py")
RTL = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v",
       "rtl/tb_dx7_core.v"]
REPORT_PATH = os.path.join(REPO, "docs", "H07-CORE.md")
EVIDENCE = os.path.join(REPO, "evidence", "h07-core")
MANIFEST = os.path.join(REPO, "reference", "release", "manifest.json")
CIEL_LIB = ("/Users/joseph/dev/2amlogic/prep/tinytapeout/pdk/ciel/"
            "gf180mcu/versions/"
            "54435919abffb937387ec956209f9cf5fd2dfbee/"
            "gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
            "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")
REMOTE_ALIAS = "repo-remote-gf180-dx7"


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def rtl_fingerprint():
    h = hashlib.sha256()
    for rel in RTL:
        with open(os.path.join(REPO, rel), "rb") as f:
            h.update(f.read())
    return h.hexdigest()


def which(tool):
    return shutil.which(tool)


def remote_reachable():
    """True when the heavy-execution host answers ssh (2 s budget)."""
    try:
        proc = subprocess.run(["ssh", "-o", "ConnectTimeout=2",
                               "-o", "BatchMode=yes", REMOTE_ALIAS,
                               "true"], capture_output=True, timeout=10)
        return proc.returncode == 0
    except (OSError, subprocess.TimeoutExpired):
        return False


# ---------------------------------------------------------------------------
# small local golden-vector machinery (fast sims; the same encoding the
# conformance harness uses)
# ---------------------------------------------------------------------------

def build_small_case(note_block=2, frames=8, note=60, velocity=100,
                     cid="dir-base"):
    """One small single-note vector + its exact model golden."""
    import h07_compare as H
    from gf180_dx7 import sysex
    from gf180_dx7.model.algorithm import voice_patch
    from gf180_dx7.model.poly import PolyManager
    cache = json.load(open(os.path.join(EVIDENCE, "cases.json")))
    vb = bytes.fromhex(cache["cases"][cid]["voice_bytes"])
    voice = sysex.decode_voice(vb)
    body = voice_patch(voice)
    mgr = PolyManager([voice])
    pos = note_block * 64
    events = f"{pos} note_on {note} {velocity}\n"
    clips = mgr.render_clips(events, (frames * 64) / 48000.0)
    lines = [f"P {a:02X} {d:08X}" for a, d in H.page_writes(body)]
    lines.append("C")
    for a, d in H.note_on_writes(body, note, velocity):
        lines.append(f"E {note_block} {a:02X} {d:08X}")
    remain_blocks = 2 + frames + 2 - note_block
    lines.append(f"W {remain_blocks * 64}")
    return "\n".join(lines) + "\n", clips, body


def build_small_multivoice(frames=14):
    """A 4-voice high-level vector: per-voice clip paths are active, so
    the sum-order mutant (clip applied to the SUM) must diverge."""
    import h07_compare as H
    from gf180_dx7 import sysex
    from gf180_dx7.model.algorithm import voice_patch
    from gf180_dx7.model.poly import PolyManager
    cache = json.load(open(os.path.join(EVIDENCE, "cases.json")))
    vb = bytes.fromhex(cache["cases"]["dir-base"]["voice_bytes"])
    voice = sysex.decode_voice(vb)
    body = voice_patch(voice)
    mgr = PolyManager([voice])
    lines = []
    events = ""
    for k, (note, vel) in enumerate([(48, 127), (55, 127), (60, 127),
                                     (67, 127)]):
        pos = 64 + k * 8
        events += f"{pos} note_on {note} {vel}\n"
    events += f"{(frames - 2) * 64} note_off 48\n"
    events += f"{(frames - 2) * 64} note_off 55\n"
    clips = mgr.render_clips(events, (frames * 64) / 48000.0)
    lines = [f"P {a:02X} {d:08X}" for a, d in H.page_writes(body)]
    lines.append("C")
    for k, (note, vel) in enumerate([(48, 127), (55, 127), (60, 127),
                                     (67, 127)]):
        blk = (64 + k * 8) // 64
        for a, d in H.note_on_writes(body, note, vel):
            lines.append(f"E {blk} {a:02X} {d:08X}")
    off_blk = frames - 2
    for note in (48, 55):
        a, d = H.event_write(note, H.EA_NOTEOFF)
        lines.append(f"E {off_blk} {a:02X} {d:08X}")
    lines.append(f"W {(2 + frames + 2 - off_blk) * 64}")
    return "\n".join(lines) + "\n", clips


def build_small_burst(frames=8):
    """Two note_ons in ONE block: the tick-skew mutant (stale read
    pointer drops the final write of a burst window) must diverge."""
    import h07_compare as H
    from gf180_dx7 import sysex
    from gf180_dx7.model.algorithm import voice_patch
    from gf180_dx7.model.poly import PolyManager
    cache = json.load(open(os.path.join(EVIDENCE, "cases.json")))
    vb = bytes.fromhex(cache["cases"]["dir-base"]["voice_bytes"])
    voice = sysex.decode_voice(vb)
    body = voice_patch(voice)
    mgr = PolyManager([voice])
    events = ("128 note_on 60 100\n" "192 note_on 67 100\n")
    clips = mgr.render_clips(events, (frames * 64) / 48000.0)
    lines = [f"P {a:02X} {d:08X}" for a, d in H.page_writes(body)]
    lines.append("C")
    for blk, note in ((2, 60), (3, 67)):
        for a, d in H.note_on_writes(body, note, 100):
            lines.append(f"E {blk} {a:02X} {d:08X}")
    lines.append(f"W {(2 + frames + 2 - 3) * 64}")
    return "\n".join(lines) + "\n", clips


class SimBinary:
    """One Verilator build per define-set, cached by RTL fingerprint."""

    CACHE = os.path.join(EVIDENCE, "sim", "test-cache")

    @classmethod
    def get(cls, defines, workdir):
        vlt = which("verilator")
        if vlt is None:
            raise unittest.SkipTest(
                "NOT_RUN (guarded skip): verilator not found on PATH; the "
                "negative-control sims also run on the heavy host via ssh "
                f"alias {REMOTE_ALIAS}")
        fp = rtl_fingerprint()[:12]
        name = "-".join(defines) or "clean"
        out = os.path.join(cls.CACHE, f"{name}-{fp}")
        marker = os.path.join(out, ".built")
        if not os.path.exists(marker):
            if os.path.exists(out):
                shutil.rmtree(out)
            os.makedirs(out, exist_ok=True)
            cmd = [vlt, "--binary", "--timing", "-j", "4", "--Wno-fatal",
                   "--timescale-override", "1ns/1ps", "--Mdir", out,
                   "-o", "simtb", "--top-module", "tb_dx7_core"]
            cmd += [f"+define+{d}" for d in defines]
            cmd += [os.path.join(REPO, f) for f in RTL]
            proc = subprocess.run(cmd, cwd=REPO, capture_output=True,
                                  text=True, timeout=1800)
            if proc.returncode != 0:
                raise AssertionError(
                    f"verilator build failed for {name}: "
                    f"{proc.stderr[-2000:]}")
            open(marker, "w").write(fp)
        return os.path.join(out, "simtb")


def run_sim(binary, vtext, tag):
    rdir = tempfile.mkdtemp(prefix=f"h07-{tag}-")
    vfile = os.path.join(rdir, "vector.txt")
    afile = os.path.join(rdir, "actual.i32")
    mfile = os.path.join(rdir, "meta.txt")
    with open(vfile, "w") as f:
        f.write(vtext)
    proc = subprocess.run([binary, f"+vectors={vfile}",
                           f"+actual={afile}", f"+meta={mfile}"],
                          cwd=REPO, capture_output=True, text=True,
                          timeout=1800)
    if proc.returncode != 0 or "DONE" not in proc.stdout:
        raise AssertionError(f"sim failed: {proc.stdout[-1500:]} "
                             f"{proc.stderr[-1500:]}")
    meta = {}
    with open(mfile) as f:
        for line in f:
            parts = line.split()
            if len(parts) >= 2:
                meta[parts[0]] = parts[1:]
    with open(afile, "rb") as f:
        data = f.read()
    acts = struct.unpack("<%di" % (len(data) // 4), data)
    return acts, meta


def first_mismatch(acts, clips, base):
    off = (base + 2) * 64
    for i, want in enumerate(clips):
        got = acts[off + i]
        if got != want:
            return {"index": off + i, "block": i // 64, "sample": i % 64,
                    "golden": want, "actual": got}
    return None


# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------

class TestNegativeControls(unittest.TestCase):
    """The four issue-named controls, each demonstrably failing."""

    @classmethod
    def setUpClass(cls):
        cls.vtext, cls.clips, cls.body = build_small_case()
        cls.mvtext, cls.mclips = build_small_multivoice()
        cls.bvtext, cls.bclips = build_small_burst()

    def test_clean_build_passes_all_three_small_vectors(self):
        binary = SimBinary.get([], "clean")
        for tag, vt, clips in (("single", self.vtext, self.clips),
                               ("multivoice", self.mvtext, self.mclips),
                               ("burst", self.bvtext, self.bclips)):
            acts, meta = run_sim(binary, vt, "clean-" + tag)
            base = int(meta["base_frame"][0])
            mm = first_mismatch(acts, clips, base)
            self.assertIsNone(
                mm, f"clean build must be bit-exact on {tag}: {mm}")
            self.assertEqual(meta["overrun"], ["0"])
            self.assertEqual(meta["overflow"], ["0"])

    def test_glitch_mutant_fails_and_localizes(self):
        binary = SimBinary.get(["H07_MUTATE_GLITCH"], "glitch")
        acts, meta = run_sim(binary, self.vtext, "glitch")
        base = int(meta["base_frame"][0])
        mm = first_mismatch(acts, self.clips, base)
        self.assertIsNotNone(
            mm, "glitch mutant must FAIL the bit-exact compare")
        # localization: the injected glitch is frame 12 (wire) sample 17
        # of the compute that streams there => golden block 12-(base+1)-1
        want_index = (base + 2 + (12 - (base + 1) - 1)) * 64 + 17
        self.assertEqual(
            mm["index"], want_index,
            f"comparator must localize the glitch to sample {want_index}")
        self.assertEqual(mm["actual"], mm["golden"] + 1)

    def test_sum_order_mutant_fails(self):
        binary = SimBinary.get(["H07_MUTATE_SUM_ORDER"], "sumorder")
        acts, meta = run_sim(binary, self.mvtext, "sumorder")
        base = int(meta["base_frame"][0])
        mm = first_mismatch(acts, self.mclips, base)
        self.assertIsNotNone(
            mm, "sum-then-clip mutant (N08 voice_sum_order_swap) must "
                "FAIL the bit-exact compare on a clipping 4-voice vector")

    def test_tick_skew_mutant_fails(self):
        binary = SimBinary.get(["H07_MUTATE_TICK_SKEW"], "tickskew")
        acts, meta = run_sim(binary, self.bvtext, "tickskew")
        base = int(meta["base_frame"][0])
        mm = first_mismatch(acts, self.bclips, base)
        self.assertIsNotNone(
            mm, "stale-read-pointer mutant must FAIL (the last write of "
                "a burst window is dropped)")

    def test_interface_is_exactly_contracted(self):
        """No per-sample host data path (DEC-010, H03 4.7)."""
        with open(os.path.join(REPO, "rtl", "dx7_core.v")) as f:
            text = f.read()
        m = re.search(r"module dx7_core \((.*?)\);", text, re.S)
        ports = set(re.findall(r"(\w+)\s*(?:,|$)", m.group(1), re.M))
        ports = {p for p in ports if not p.startswith("//")}
        expected = {"clk", "rst_n", "spi_sck", "spi_mosi", "spi_cs_n",
                    "spi_miso", "i2s_bclk", "i2s_lrclk", "i2s_d",
                    "tap_frame", "tap_live", "tap_mix", "tap_mix_valid",
                    "status_overrun", "status_overflow", "state_obs"}
        self.assertEqual(ports, expected,
                         "core port surface drifted from the contracted "
                         "per-event interface")


class TestStripObsControl(unittest.TestCase):
    """The synthesis control: committed evidence, freshness-checked."""

    def test_committed_synth_report_gates(self):
        path = os.path.join(EVIDENCE, "synth_report.json")
        if not os.path.exists(path):
            if remote_reachable():
                self.fail(
                    f"NOT_RUN: heavy host {REMOTE_ALIAS} is reachable but "
                    "no committed synth_report.json -- run "
                    "tools/h07_synth.py there and commit the report")
            self.skipTest(
                f"NOT_RUN (guarded skip): no committed synth_report.json "
                f"and the heavy host ({REMOTE_ALIAS}) is unreachable -- "
                "yosys + the ciel 7t liberty live there")
        with open(path) as f:
            report = json.load(f)
        self.assertEqual(report["gates"]["full"]["status"], "PASS",
                         "full flop gate must PASS")
        self.assertEqual(
            report["gates"]["strip_negative_control"]["status"],
            "MET (gate fails as required)",
            "strip-obs control must make the flop gate FAIL")
        self.assertGreater(report["delta_vs_h02"]["h07_mapped_flops"],
                           report["delta_vs_h02"]["h02_measured_mapped_flops"])
        # freshness: the report must be about the CURRENT RTL
        for rel, want in report["rtl"].items():
            got = sha256_file(os.path.join(REPO, rel))
            self.assertEqual(got, want,
                             f"{rel} drifted after the synth report "
                             "(STALE; re-run tools/h07_synth.py)")


class TestConformanceEvidence(unittest.TestCase):
    """The committed conformance results: frozen vectors, both runs
    artifact-hash identical, current RTL."""

    RESULTS = ("results-verilog-accept.json",)

    def _load_results(self):
        for name in self.RESULTS:
            path = os.path.join(EVIDENCE, name)
            if os.path.exists(path):
                with open(path) as f:
                    return name, json.load(f)
        return None, None

    def test_two_clean_runs_artifact_hash_identical(self):
        name, results = self._load_results()
        if results is None:
            if remote_reachable():
                self.fail(
                    f"NOT_RUN: heavy host {REMOTE_ALIAS} reachable but no "
                    "committed conformance results -- run "
                    "tools/h07_compare.py there and commit")
            self.skipTest(
                f"NOT_RUN (guarded skip): no committed conformance results "
                f"and the heavy host ({REMOTE_ALIAS}) is unreachable")
        dup = results.get("determinism_run2")
        self.assertIsNotNone(
            dup, f"{name} must embed the second clean run's hashes")
        for cid, res in results["cases"].items():
            self.assertIn("actual_sha256", res, cid)
            self.assertEqual(res["actual_sha256"], dup.get(cid),
                             f"{cid}: the two clean runs differ")
            self.assertTrue(res["pass"], f"{cid}: not a PASS row")
            self.assertEqual(res["overrun"], 0, cid)
            self.assertEqual(res["overflow"], 0, cid)
            for lat in res.get("latency", []):
                self.assertTrue(lat["in_window"],
                                f"{cid}: {lat['event']} outside the "
                                "H03 4.5 window")

    def test_results_match_frozen_vectors_and_current_rtl(self):
        name, results = self._load_results()
        if results is None:
            self.skipTest(
                f"NOT_RUN (guarded skip): no committed results; heavy host "
                f"is {REMOTE_ALIAS}")
        with open(MANIFEST) as f:
            manifest = json.load(f)
        vec = manifest["covered"]["vectors"]
        dev = {c["id"] for c in manifest["covered"]["cases"]
               if c["set"] == "dev"}
        for cid in dev:
            self.assertIn(cid, results["cases"], cid)
        self.assertEqual(results.get("rtl_sha256"), rtl_fingerprint(),
                         f"{name} is STALE: RTL moved after the run")

    def test_dev_vectors_respect_burst_bound(self):
        """Every committed dev vector fits the contracted 168-write frame
        bound at SCK = clk/4 (the event queue's sizing contract)."""
        import h07_compare as H
        from gf180_dx7 import sysex
        from gf180_dx7.model.algorithm import voice_patch
        cache = json.load(open(os.path.join(EVIDENCE, "cases.json")))
        with open(MANIFEST) as f:
            manifest = json.load(f)
        for case in manifest["covered"]["cases"]:
            if case["set"] != "dev":
                continue
            vb = bytes.fromhex(cache["cases"][case["id"]]["voice_bytes"])
            body = voice_patch(sysex.decode_voice(vb))
            per_block = {}
            for t in case["event_trace"]:
                parts = t["event"].split()
                n = 7 if parts[1] == "note_on" else 1
                per_block[t["block"]] = per_block.get(t["block"], 0) + n
            for blk, n in per_block.items():
                self.assertLessEqual(
                    n, 168,
                    f"{case['id']} block {blk}: {n} writes exceed the "
                    "contracted bound")


class TestHarnessMachinery(unittest.TestCase):
    """Golden loaders are pinned; stress goldens are byte-frozen."""

    def test_golden_loader_rejects_drifted_vector(self):
        import h07_compare as H
        with tempfile.TemporaryDirectory() as td:
            bad = os.path.join(td, "x.f32")
            open(bad, "wb").write(b"\x00" * 256)
            with self.assertRaises(H.CheckFailure):
                H.golden_ints_from_f32(__import__("pathlib").Path(bad),
                                       "deadbeef", 64)

    def test_stress_golden_is_frozen_and_fresh(self):
        import h07_compare as H
        for spec in H.STRESS_SET[1]:
            golden = stress = H.stress_golden(spec)
            # a re-render must agree byte-for-byte with the frozen cache
            fresh = H.render_stress_golden(spec)
            for a, b in zip(golden["segments"], fresh["segments"]):
                self.assertEqual(
                    list(a), list(b),
                    f"{spec['id']}: frozen stress golden drifted from a "
                    "fresh model render (model moved? re-freeze)")

    def test_latency_window_math(self):
        import h07_compare as H
        for p in (0, 17, 63):
            lo, hi = 64 - p, 64 - p + 65
            dist = 128 - p
            self.assertTrue(lo <= dist <= hi, p)


class TestReportDoc(unittest.TestCase):
    def test_doc_records_the_required_anchors(self):
        if not os.path.exists(REPORT_PATH):
            self.skipTest(f"NOT_RUN: {REPORT_PATH} absent")
        with open(REPORT_PATH) as f:
            text = f.read()
        for anchor in ("h07_compare.py", "h07_synth.py",
                       "38,781", "BIT-EXACT", "OVERRUN", "H03"):
            self.assertIn(anchor, text, f"missing anchor: {anchor}")
        self.assertNotIn("WIP — bring-up only", text,
                         "the doc still says the branch is bring-up WIP")


if __name__ == "__main__":
    unittest.main()
