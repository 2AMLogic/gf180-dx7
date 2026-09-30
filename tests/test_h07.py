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

def build_small_case(note_block=2, frames=16, note=60, velocity=100,
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


def build_small_burst(frames=24):
    """A 167-write burst in ONE inter-frame window (16 note-ons x 7 writes
    + 55 inert CC1s): the tick-skew mutant's stale read pointer re-pops
    one event past the queue end on the final 1-cycle pop, striking a
    spurious velocity-0 note (the evq slot past the last write is
    zero in simulation) -- the clean build must not."""
    import h07_compare as H
    from gf180_dx7 import sysex
    from gf180_dx7.model.algorithm import voice_patch
    from gf180_dx7.model.poly import PolyManager
    cache = json.load(open(os.path.join(EVIDENCE, "cases.json")))
    vb = bytes.fromhex(cache["cases"]["dir-base"]["voice_bytes"])
    voice = sysex.decode_voice(vb)
    body = voice_patch(voice)
    mgr = PolyManager([voice])
    notes = [(36 + 2 * k, 100) for k in range(16)]
    events = "".join(f"128 note_on {n} {v}\n" for n, v in notes)
    clips = mgr.render_clips(events, (frames * 64) / 48000.0)
    lines = [f"P {a:02X} {d:08X}" for a, d in H.page_writes(body)]
    lines.append("C")
    for n, v in notes:
        for a, d in H.note_on_writes(body, n, v):
            lines.append(f"E 2 {a:02X} {d:08X}")
    for _ in range(55):                      # inert writes; the LAST is a
        a, d = H.event_write((1 << 8) | 1, H.EA_CC)   # 1-cycle pop
        lines.append(f"E 2 {a:02X} {d:08X}")
    lines.append(f"W {(2 + frames + 2 - 2) * 64}")
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
        # localization: the glitch injects during WIRE frame 12's eval
        # (compute 12 = golden block 12-(base+1)); its sample 17 streams
        # at dump index (base+2)*64 + (12-(base+1))*64 + 17
        want_index = (base + 2 + (12 - (base + 1))) * 64 + 17
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
        body_text = re.sub(r"//.*", "", m.group(1))
        ports = set(re.findall(r"(\w+)\s*(?:,|$)", body_text, re.M))
        expected = {"clk", "rst_n", "spi_sck", "spi_mosi", "spi_cs_n",
                    "spi_miso", "i2s_bclk", "i2s_lrclk", "i2s_d",
                    "tap_frame", "tap_live", "tap_mix", "tap_mix_valid",
                    "status_overrun", "status_overflow", "state_obs"}
        self.assertEqual(ports, expected,
                         "core port surface drifted from the contracted "
                         "per-event interface")


class TestSynthStatParser(unittest.TestCase):
    """tools/h07_synth.py parse_stat: the reported area/cell/flop numbers
    must be the WHOLE-CORE hierarchy totals, never a per-module block
    (issue #82). These cases replay the already-committed yosys
    transcripts -- no synthesis, no heavy host, so they run in the fast
    lane and a future parser regression fails here instead of silently
    shipping a wrong number.

    Hierarchy totals of the committed evidence/h07-core/yosys_full.log
    (`=== design hierarchy ===`, closed by `Chip area for top module
    '\\dx7_core'`): 24,082,966.252793 um^2, 1,063,163 cells, 90,427
    dffq_1, 5,756,655.161602 um^2 sequential."""

    FULL_LOG = os.path.join(EVIDENCE, "yosys_full.log")
    STRIP_LOG = os.path.join(EVIDENCE, "yosys_strip.log")
    HIER = {"chip_area_um2": 24082966.252793,
            "cell_total": 1063163,
            "flop_total": 90427,
            "seq_area_um2": 5756655.161602}
    # the pre-fix (wrong) numbers: alg_router's LOCAL block, which is the
    # first `Chip area for module` match in the same transcript, plus the
    # across-all-blocks flop double count
    ALG_ROUTER_LOCAL_AREA = 1158826.3232
    ALG_ROUTER_LOCAL_CELLS = 36919
    ALG_ROUTER_LOCAL_SEQ = 473572.6912
    DOUBLE_COUNTED_FLOPS = 147889
    # the non-final intermediate dx7_core block earlier in the same log
    INTERMEDIATE_DX7_CORE_AREA = 6643568.646402

    def _full(self):
        if not os.path.exists(self.FULL_LOG):
            self.skipTest(f"NOT_RUN: {self.FULL_LOG} absent")
        with open(self.FULL_LOG) as f:
            return f.read()

    def test_synthparse_committed_log_yields_hierarchy_totals(self):
        import h07_synth
        st = h07_synth.parse_stat(self._full())
        for key, want in self.HIER.items():
            self.assertEqual(st[key], want,
                             f"{key} must be the design-hierarchy total")
        self.assertEqual(st["top_module"], "dx7_core")
        self.assertEqual(
            st["cells_by_name"]["gf180mcu_fd_sc_mcu7t5v0__dffq_1"], 90427)
        self.assertIn("design hierarchy", st["totals_basis"])

    def test_synthparse_does_not_report_a_per_module_block(self):
        """The three specific wrong values #82 recorded must not come
        back: alg_router's local area/cells/sequential area, the
        all-blocks flop double count, and the non-final intermediate
        dx7_core block (the trap for a fix that matches on module name
        or takes the first match)."""
        import h07_synth
        st = h07_synth.parse_stat(self._full())
        self.assertNotEqual(st["chip_area_um2"],
                            self.ALG_ROUTER_LOCAL_AREA)
        self.assertNotEqual(st["chip_area_um2"],
                            self.INTERMEDIATE_DX7_CORE_AREA)
        self.assertNotEqual(st["cell_total"], self.ALG_ROUTER_LOCAL_CELLS)
        self.assertNotEqual(st["seq_area_um2"], self.ALG_ROUTER_LOCAL_SEQ)
        self.assertNotEqual(st["flop_total"], self.DOUBLE_COUNTED_FLOPS)

    def test_synthparse_refuses_a_transcript_without_hierarchy(self):
        """NEGATIVE CONTROL: per-module blocks present, hierarchy section
        absent -> the parser must FAIL, not return a module number."""
        import h07_synth
        transcript = (
            "=== alg_router ===\n\n"
            "    36919 1.16E+06 cells\n"
            "     7439 4.74E+05   gf180mcu_fd_sc_mcu7t5v0__dffq_1\n\n"
            "   Chip area for module '\\alg_router': 1158826.323200\n"
            "     of which used for sequential elements: 473572.691200\n\n"
            "=== dx7_core ===\n\n"
            "   234527 6.64E+06 cells\n"
            "    49676 3.16E+06   gf180mcu_fd_sc_mcu7t5v0__dffq_1\n\n"
            "   Chip area for module '\\dx7_core': 6643568.646402\n"
            "     of which used for sequential elements: 3162413.900802\n")
        with self.assertRaises(h07_synth.CheckFailure) as ctx:
            h07_synth.parse_stat(transcript)
        self.assertIn("top module", str(ctx.exception))

    def test_synthparse_refuses_cells_without_any_chip_area(self):
        """NEGATIVE CONTROL: a truncated transcript with mapped cells but
        no area anchor at all must FAIL rather than report an unanchored
        count as the design total."""
        import h07_synth
        transcript = ("=== dx7_core ===\n\n"
                      "   234527 6.64E+06 cells\n"
                      "    49676 3.16E+06   "
                      "gf180mcu_fd_sc_mcu7t5v0__dffq_1\n")
        with self.assertRaises(h07_synth.CheckFailure):
            h07_synth.parse_stat(transcript)

    def test_synthparse_refuses_hierarchy_total_without_section(self):
        """NEGATIVE CONTROL: a top-module area line with no preceding
        `=== design hierarchy ===` section (so no bounded region to count
        cells in) must FAIL."""
        import h07_synth
        transcript = ("   Chip area for module '\\alg_router': 1158826.32\n"
                      "   Chip area for top module '\\dx7_core': "
                      "24082966.252793\n")
        with self.assertRaises(h07_synth.CheckFailure):
            h07_synth.parse_stat(transcript)

    def test_synthparse_empty_design_is_zero_not_an_error(self):
        """The strip-obs control's transcript maps NOTHING: no hierarchy
        section exists because there is nothing to report. That is the one
        legitimate hierarchy-free case -- zero cells, zero flops, no
        area -- and it must stay parseable, or the negative control
        cannot be evaluated."""
        import h07_synth
        if not os.path.exists(self.STRIP_LOG):
            self.skipTest(f"NOT_RUN: {self.STRIP_LOG} absent")
        with open(self.STRIP_LOG) as f:
            st = h07_synth.parse_stat(f.read())
        self.assertEqual(st["cell_total"], 0)
        self.assertEqual(st["flop_total"], 0)
        self.assertIsNone(st["chip_area_um2"])
        self.assertIsNone(st["seq_area_um2"])
        self.assertEqual(st["cells_by_name"], {})
        self.assertIn("empty design", st["totals_basis"])


class TestStripObsControl(unittest.TestCase):
    """The synthesis control: committed evidence, freshness-checked."""

    def test_committed_synth_report_area_is_the_hierarchy_total(self):
        """The committed report's numeric fields must be the whole-core
        hierarchy totals of its own committed transcript (issue #82).

        Until the report is regenerated with the fixed parser on the
        heavy host it still carries the pre-fix per-module numbers: that
        is reported as STALE / NOT_RUN (guarded skip naming the host),
        never as a pass."""
        import h07_synth
        path = os.path.join(EVIDENCE, "synth_report.json")
        log_path = os.path.join(EVIDENCE, "yosys_full.log")
        if not (os.path.exists(path) and os.path.exists(log_path)):
            self.skipTest(
                "NOT_RUN (guarded skip): no committed synth report/log; "
                f"yosys + the ciel 7t liberty live on {REMOTE_ALIAS}")
        with open(path) as f:
            report = json.load(f)
        with open(log_path) as f:
            want = h07_synth.parse_stat(f.read())
        got = report["runs"]["full"]
        drift = {k: (got.get(k), want[k])
                 for k in ("chip_area_um2", "cell_total", "flop_total",
                           "seq_area_um2") if got.get(k) != want[k]}
        if drift:
            msg = ("STALE: evidence/h07-core/synth_report.json still "
                   "carries pre-#82 parser output (recorded, hierarchy "
                   f"total): {drift}. Re-run tools/h07_synth.py on "
                   f"{REMOTE_ALIAS} (yosys + ciel 7t liberty) and commit "
                   "the regenerated report.")
            if remote_reachable():
                self.fail(msg)
            self.skipTest(f"NOT_RUN (guarded skip): heavy host "
                          f"{REMOTE_ALIAS} unreachable. {msg}")
        for key in ("chip_area_um2", "cell_total", "seq_area_um2"):
            self.assertEqual(report["gates"]["full"][key], want[key],
                             f"gates.full.{key} must match the run")
        self.assertEqual(report["gates"]["full"]["mapped_flops"],
                         want["flop_total"])

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
        # Freshness: the report must be about the CURRENT RTL.  When it is
        # not, that is STALE / NOT_RUN -- never a pass, and never a bare
        # FAIL on a host that structurally cannot regenerate it: yosys and
        # the ciel 7t liberty live on REMOTE_ALIAS.  Same shape as
        # test_committed_synth_report_area_is_the_hierarchy_total (#82) and
        # as the H08 STALE gate (#94).  DR-0012 (#98) re-freezes the core,
        # so this report is STALE until it is re-run on the current pin.
        drift = {rel: (sha256_file(os.path.join(REPO, rel)), want)
                 for rel, want in report["rtl"].items()
                 if sha256_file(os.path.join(REPO, rel)) != want}
        if drift:
            msg = ("STALE: evidence/h07-core/synth_report.json was produced "
                   f"on a different core revision: {drift}. Re-run "
                   f"tools/h07_synth.py on {REMOTE_ALIAS} (yosys + ciel 7t "
                   "liberty) and commit the regenerated report.")
            if remote_reachable():
                self.fail(msg)
            self.skipTest(f"NOT_RUN (guarded skip): heavy host "
                          f"{REMOTE_ALIAS} unreachable. {msg}")


class TestConformanceEvidence(unittest.TestCase):
    """The committed conformance results: frozen vectors, both runs
    artifact-hash identical, current RTL.

    RESULTS is newest-pin-first: a core refreeze adds its own re-certified
    battery (DR-0012 / issue #98 added `results-dr0012-p2.json`) and the
    earlier files stay as the historical record of the pins they measured.
    The freshness assertion below is what makes the ordering safe -- the
    file that is read must carry the CURRENT three-file fingerprint, so a
    stale newest entry fails rather than silently standing in.
    """

    RESULTS = ("results-dr0012-p2.json", "results-verilog-accept.json")

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
        # freshness is checked against the PRODUCER's fingerprint domain:
        # tools/h07_compare.py hashes the three synthesized RTL files
        # (the tb is exercised live by the negative controls instead)
        import h07_compare
        self.assertEqual(results.get("rtl_sha256"),
                         h07_compare.rtl_fingerprint(),
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
            golden = H.stress_golden(spec)
            # a re-render must agree byte-for-byte with the frozen cache
            # (stress_golden returns per-segment int lists; the fresh
            # render wraps them in dicts with the event schedule)
            fresh = H.render_stress_golden(spec)
            self.assertEqual(len(golden["segments"]),
                             len(fresh["segments"]), spec["id"])
            for a, b in zip(golden["segments"], fresh["segments"]):
                self.assertEqual(
                    list(a), list(b["ints"]),
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
