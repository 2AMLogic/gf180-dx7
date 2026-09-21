#!/usr/bin/env python3
"""H06 pitch/modulation conformance harness (issue #28).

The golden is the FROZEN PYTHON MODEL (src/gf180_dx7/model/pitch.py,
contract gf180-dx7-integer-v1, composed per the pinned integrated order of
src/gf180_dx7/model/integrated.py::compute_frame -- seam finding F-3,
DR-0006); the device under test is the RTL (rtl/pitch_mod.v). The bar is
BIT-EXACTNESS -- no tolerance: any mismatch means the RTL is wrong, never
a loosened check (AGENTS.md honesty rules).

The harness plays the HOST side of the H03 contract split
(docs/CONTRACT-CORE-v1.md sections 4.2/4.7; DEC-010/DEC-014/DEC-019/
DEC-022/DEC-023; NUM-008 "event_time_pitch_math"): it computes the
float-gated per-event integers host-side (per-op detune deltas and fine
terms from dx7note.cc:38-64, the scaled bend integer from the float-gated
bend spot) and streams only per-event frozen 32-bit register writes into
the RTL. Everything per-frame -- pitch-EG stepping, LFO sampling (six
waveforms, delay ramp, keysync), PM/AM depth composition, transpose
application, Freqlut lookups -- is computed inside the RTL and compared
against the model's per-frame outputs. No per-sample host input exists in
this exchange (guarded additionally by tests/test_h06.py).

ROM bytes: the RTL $readmemh-loads reference/tables/freqlut_table.hex --
the SAME bytes the model pins by SHA-256 in reference/tables/manifest.json
(freqlut 6d3c5970..., verified here via tables.load_pinned before any
vector is generated) -- and reference/tables/lfo_sin_table.hex, the
regenerated msfa Sin table (SIN_DELTA form, sin.cc:31-56 + sin.h) the
model rebuilds with build_sintab(); both are re-verified byte-for-byte on
every run.

Vector coverage (issue #28 / N05 directed cases, >= 4000 frame evals):
ratio vs fixed mode (keyboard tracking vs invariance), coarse 0/extremes
(+&31 wrap), detune extremes, fine 99, pitch EG up/down/slow/fast/
nonzero-final/retrigger/repeated-note, all six LFO waveforms, LFO speed/
delay/keysync, PM depth/sensitivity composition, controller PM path
(documented nonzero wheel range), bend scaling (frozen), master tune,
transpose (DOCUMENTED mode per DEC-023/H03 4.2; the pinned-wrapper
omission, registry finding perf-transpose-12, is a wrapper finding and is
NOT reproduced here), AM depth composition, reset-state frames, and burst
event timing (8 note-on/off pairs inside one inter-frame window).

NEGATIVE CONTROLS (each must demonstrably FAIL the exact compare):
  lsb       -DMUTATE_INCREMENT_LSB          op-0 increment +1 LSB
  fixedkb   -DMUTATE_FIXED_TRACKS_KEYBOARD  fixed mode tracks keyboard
  sync      -DMUTATE_LFO_SYNC_IGNORED       LFO keydown keeps phase
  persample -DH06_PER_SAMPLE_HOST           illegal per-sample host gate:
            increments commit only on a host stream the harness never
            provides, so they stall -- the split violation fails.

Exit codes: 0 pass (real build exact AND every mutant control MET),
1 check failure, 2 could-not-run (missing iverilog/vvp; the message names
what is missing). Stdlib only.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO_ROOT, "src")
if SRC not in sys.path:
    sys.path.insert(0, SRC)

from gf180_dx7.model import pitch as P  # noqa: E402
from gf180_dx7.model.tables import load_pinned  # noqa: E402

TABLES_DIR = os.path.join(REPO_ROOT, "reference", "tables")
SIN_HEX = os.path.join(TABLES_DIR, "lfo_sin_table.hex")
RTL_REL = "rtl/pitch_mod.v"
TB_REL = "rtl/tb_pitch_mod.v"
OUTDIR_REL = "evidence/h06-pitch-rtl"
MIN_FRAMES = 4000
MIN_ACTIVE = 4000
SEED = 0x00D06001

U32 = 0xFFFFFFFF

# Contracted register map (rtl/pitch_mod.v header; docs/H06-PITCH-RTL.md s2)
A_CMD = 0x00
A_OPCFG = 0x10
A_FTERM = 0x18
A_PEGR = 0x20
A_PEGL = 0x21
A_LFOP = 0x22
A_PMDEP = 0x23
A_PMSENS = 0x24
A_TRANS = 0x25
A_AMDEP = 0x26
A_BEND = 0x28
A_MTUNE = 0x29
A_CTRLPM = 0x2A
A_CTRLAM = 0x2B
A_EGMOD = 0x2C
A_DDELTA = 0x30

CMD_NOTE_ON, CMD_NOTE_OFF, CMD_COMMIT = 1, 2, 3


class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def i32(x):
    return P.i32(x)


# ---------------------------------------------------------------------------
# pinned bytes: freqlut LUT + regenerated LFO sine table
# ---------------------------------------------------------------------------

def verify_freqlut():
    """Load the pinned freqlut table (SHA-256 checked by load_pinned) and
    make it the model's LUT, so golden and RTL consume the SAME bytes."""
    tabs = load_pinned(TABLES_DIR)
    lut = list(tabs["freqlut_table"].values)
    if len(lut) != 1025:
        raise CheckFailure("pinned freqlut table has %d entries" % len(lut))
    P.Freqlut._lut = lut
    return tabs


def sin_table_values():
    return P.build_sintab()


def verify_sin_hex():
    """The committed lfo_sin_table.hex must be byte-identical to the model's
    regenerated table (same generator discipline as gen_tables --check)."""
    want = sin_table_values()
    if not os.path.isfile(SIN_HEX):
        raise CheckFailure(
            "missing lfo_sin_table.hex (regenerate with: python3 "
            "tools/h06_compare.py --write-sin-hex)")
    raw = open(SIN_HEX, "r", encoding="ascii").read().split()
    if len(raw) != 2048:
        raise CheckFailure("lfo_sin_table.hex: %d lines != 2048" % len(raw))
    got = []
    for lineno, tok in enumerate(raw, 1):
        if len(tok) != 7 or not re.fullmatch(r"[0-9a-f]{7}", tok):
            raise CheckFailure(
                "lfo_sin_table.hex:%d: malformed entry %r" % (lineno, tok))
        v = int(tok, 16)
        if v >= (1 << 25):
            v -= (1 << 26)
        got.append(v)
    if got != want:
        for i, (g, w) in enumerate(zip(got, want)):
            if g != w:
                raise CheckFailure(
                    "lfo_sin_table.hex drifted from build_sintab() at entry "
                    "%d (%d != %d); regenerate with --write-sin-hex or "
                    "investigate tampering" % (i, g, w))
        raise CheckFailure("lfo_sin_table.hex drifted from build_sintab()")


def write_sin_hex():
    values = sin_table_values()
    text = "".join("%07x\n" % (v & 0x3FFFFFF) for v in values)
    with open(SIN_HEX, "w", encoding="ascii") as f:
        f.write(text)
    print("wrote %s (%d entries, regenerated from build_sintab())"
          % (os.path.relpath(SIN_HEX, REPO_ROOT), len(values)))


# ---------------------------------------------------------------------------
# Host frontend: frozen-integer event-time computation (the DEC-010
# patch-to-core conversion + the NUM-008 float-gated event-time spots)
# ---------------------------------------------------------------------------

def host_fine_term(fine):
    """dx7note.cc:50-53: host-side float math, frozen integer result."""
    if fine == 0:
        return 0
    return int(math.floor(24204406.323123 * math.log(1 + 0.01 * fine) + 0.5))


def host_detune_delta(logfreq, detune):
    """dx7note.cc:46-47: host-side float64 math, frozen integer result.
    detuned = int(logfreq + ratio*logfreq*(detune-7)); delta = detuned -
    logfreq. The RTL adds delta to its own identically-derived logfreq."""
    ratio = 0.0209 * math.exp(-0.396 * (float(logfreq) / (1 << 24))) / 7
    detuned = i32(int(logfreq + ratio * logfreq * (detune - 7)))
    return detuned - logfreq


def host_bend_pb(bend_value, range_up, range_dn, range_step):
    """NUM-008 float-gated bend scaling (dx7note.cc:219-233), frozen
    integer result; master tune is added core-side."""
    return P.NotePitch._pitch_base(bend_value, range_up, range_dn,
                                   range_step, 0)[0]


# ---------------------------------------------------------------------------
# Golden per-frame state (mirrors the RTL's register/FSM semantics using
# the model's own primitives -- no pitch math is re-implemented here)
# ---------------------------------------------------------------------------

def _zero_patch():
    return {
        "opcfg": [(0, 1, 0, 7)] * 6,   # (mode, coarse, fine, detune)
        "peg_rates": [0, 0, 0, 0],
        "peg_levels": [0, 0, 0, 0],
        "lfo": {"rate": 0, "delay": 0, "sync": 0, "wave": 0},
        "pm_depth_src": 0, "am_depth_src": 0, "pm_sens_src": 0,
        "transpose": 24,
        "pm_depth": 0, "am_depth": 0, "pm_sens": 0,
    }


def _copy_patch(p):
    q = dict(p)
    q["opcfg"] = list(p["opcfg"])
    q["peg_rates"] = list(p["peg_rates"])
    q["peg_levels"] = list(p["peg_levels"])
    q["lfo"] = dict(p["lfo"])
    return q


class Golden:
    """Golden mirror of the RTL: consumes the same writes/ticks and
    produces the expected per-frame outputs from the model primitives."""

    def __init__(self):
        self.lfo = P.Lfo()            # zeroed init (model declared init)
        self.np = None                # NotePitch while a note lives
        self.idle_peg = P.PitchEnv()  # PitchEnv() init before any note
        self.shadow = _zero_patch()
        self.active = _zero_patch()
        self.perf = {"bend_value": 0x2000, "range_up": 3, "range_dn": 3,
                     "step": 0, "master_tune": 0, "ctrl_pm": 0,
                     "ctrl_am": 0, "eg_mod": 127}
        self.pend_delta = [0] * 6
        self.pend_commit = False       # CMD_COMMIT pending (applied first)
        self.pending = None            # ("noteon"|"noteoff", note) pending

    def patch_image(self, p):
        """155-byte unpacked voice body equivalent to the loaded params
        (what a core holding the patch would derive from)."""
        b = bytearray(155)
        for op in range(6):
            off = op * 21
            mode, coarse, fine, detune = p["opcfg"][op]
            b[off + 17] = mode
            b[off + 18] = coarse
            b[off + 19] = fine
            b[off + 20] = detune
        for i in range(4):
            b[126 + i] = p["peg_rates"][i]
            b[130 + i] = p["peg_levels"][i]
        b[139] = p["pm_depth_src"]
        b[140] = p["am_depth_src"]
        b[143] = p["pm_sens_src"]
        b[144] = p["transpose"]
        return bytes(b)

    def write(self, addr, data):
        """Contracted write decode -- mirrors the RTL register map so the
        golden consumes exactly what the RTL consumes."""
        if addr == A_CMD:
            op = data & 0xFF
            note = (data >> 8) & 0xFF
            if op == CMD_NOTE_ON:
                self.pending = ("noteon", note)
            elif op == CMD_NOTE_OFF:
                self.pending = ("noteoff", 0)
            elif op == CMD_COMMIT:
                self.pend_commit = True
            else:
                self.pending = None       # RTL decodes unknown -> none
                self.pend_commit = False
        elif A_OPCFG <= addr <= A_OPCFG + 5:
            i = addr - A_OPCFG
            self.shadow["opcfg"][i] = (data & 0xFF,            # mode
                                       (data >> 8) & 0xFF,     # coarse
                                       (data >> 16) & 0xFF,    # fine
                                       (data >> 24) & 0xFF)    # detune
        elif A_PEGR == addr:
            self.shadow["peg_rates"] = [(data >> (8 * i)) & 0xFF
                                        for i in range(4)]
        elif A_PEGL == addr:
            self.shadow["peg_levels"] = [(data >> (8 * i)) & 0xFF
                                         for i in range(4)]
        elif A_LFOP == addr:
            self.shadow["lfo"] = {"rate": data & 0xFF,
                                  "delay": (data >> 8) & 0xFF,
                                  "sync": (data >> 16) & 0xFF,
                                  "wave": (data >> 24) & 7}
        elif A_PMDEP == addr:
            self.shadow["pm_depth"] = i32(data)
        elif A_PMSENS == addr:
            self.shadow["pm_sens"] = i32(data)
        elif A_TRANS == addr:
            self.shadow["transpose"] = data & 0xFF
        elif A_AMDEP == addr:
            self.shadow["am_depth"] = i32(data)
        elif A_BEND == addr:
            pass  # frozen bend_pb: golden keeps the raw controller state
        elif A_MTUNE == addr:
            self.perf["master_tune"] = i32(data)
        elif A_CTRLPM == addr:
            self.perf["ctrl_pm"] = i32(data)
        elif A_CTRLAM == addr:
            self.perf["ctrl_am"] = i32(data)
        elif A_EGMOD == addr:
            self.perf["eg_mod"] = i32(data)
        # A_FTERM/A_DDELTA are RTL-only frozen inputs (the golden derives
        # the same values from the patch bytes via the model).

    def frame(self):
        # tick boundary: a pending commit applies FIRST, then the pending
        # note event (contract order: commit lands, then the note uses it)
        if self.pend_commit:
            self.pend_commit = False
            self.active = _copy_patch(self.shadow)
            self.lfo.reset([self.active["lfo"]["rate"],
                            self.active["lfo"]["delay"], 0, 0,
                            self.active["lfo"]["sync"],
                            self.active["lfo"]["wave"]])
        kind, note = self.pending if self.pending else (None, 0)
        self.pending = None
        if kind == "noteon":
            patch = self.patch_image(self.active)
            self.np = P.NotePitch(patch, note, transpose_mode="documented")
            self.lfo.keydown()
        elif kind == "noteoff":
            if self.np is not None:
                self.np.pitchenv_.keydown(False)
        lfo_val = self.lfo.getsample()
        lfo_delay = self.lfo.getdelay()
        pitch_base = P.NotePitch._pitch_base(
            self.perf["bend_value"], self.perf["range_up"],
            self.perf["range_dn"], self.perf["step"],
            self.perf["master_tune"])[0]
        if self.np is None:
            peg = self.idle_peg.getsample()
            pm_nobase = i32(peg)      # depth/sens regs are 0 -> pmax = 0
            f = P.Freqlut.lookup(i32(i32(pm_nobase + pitch_base)))
            freqs = [f] * 6
            peg_level = peg
            basepitch = [0] * 6
            opmodes = [0] * 6
            inputs = [i32(i32(pm_nobase + pitch_base))] * 6
        else:
            pm_nobase = self.np.pitch_mod_per_frame(
                lfo_val, lfo_delay, ctrl_pitch_mod=self.perf["ctrl_pm"])
            freqs = []
            inputs = []
            for op in range(6):
                if self.np.op_mode[op]:
                    inputs.append(i32(self.np.basepitch[op] + pitch_base))
                else:
                    inputs.append(i32(self.np.basepitch[op] +
                                      i32(pm_nobase + pitch_base)))
                freqs.append(P.Freqlut.lookup(inputs[-1]))
            peg_level = self.np.pitchenv_.level_
            basepitch = list(self.np.basepitch)
            opmodes = list(self.np.op_mode)
        amd = P.amp_mod_per_frame(
            self.active["am_depth"], lfo_val, lfo_delay,
            ctrl_amp_mod=self.perf["ctrl_am"], ctrl_eg_mod=self.perf["eg_mod"])
        return {
            "freqs": [f & U32 for f in freqs],
            "peg": peg_level & U32,
            "lfo_val": lfo_val & 0x1FFFFFF,
            "lfo_delay": lfo_delay & 0x1FFFFFF,
            "amd": amd & U32,
            "basepitch": basepitch, "opmodes": opmodes,
            "inputs": inputs,
        }


# ---------------------------------------------------------------------------
# Vector builder: drives the golden and records the vector stream
# ---------------------------------------------------------------------------

NOTES = (0, 21, 60, 69, 81, 103, 127)


class Gen:
    """Generates the directed vector stream + golden expected rows."""

    def __init__(self):
        self.g = Golden()
        self.records = []      # ("W", addr, data) / ("F", 0, 0)
        self.expected = []     # per-F output row dicts
        self.snaps = []        # per-F localization snapshots
        self.stats = {
            "frames": 0, "active_frames": 0, "writes": 0,
            "wave_frames": [0] * 6, "fixed_op_frames": 0,
            "keysync_noteons": 0, "bend_events": 0,
            "shifted_noteons": 0, "coarse0_frames": 0, "coarse31_frames": 0,
            "detune0_frames": 0, "detune14_frames": 0,
            "no_traffic_frames": 0, "burst_frames": 0,
            "negative_lookup_inputs": 0,
            "distinct_freqlut_ix": 0, "distinct_hibits": 0,
            "scenario_frames": {},
        }
        self._tag = "reset"
        self._wrote = False
        self._ixs = set()
        self._hibits = set()

    # -- primitive record emitters ------------------------------------------

    def _w(self, addr, data):
        self.records.append(("W", addr, data & U32))
        self.g.write(addr, data & U32)
        self.stats["writes"] += 1
        self._wrote = True

    def _set_tag(self, tag):
        self._tag = tag

    def frame(self, n=1):
        for _ in range(n):
            self.records.append(("F", 0, 0))
            row = self.g.frame()
            self.expected.append(row)
            snap = {
                "tag": self._tag,
                "note": (self.g.pending or (None, None))[1],
                "transpose": self.g.active["transpose"],
                "lfo": dict(self.g.active["lfo"]),
                "perf": dict(self.g.perf),
                "basepitch": list(row["basepitch"]),
                "opmodes": list(row["opmodes"]),
                "peg_levels": list(self.g.active["peg_levels"]),
                "peg_rates": list(self.g.active["peg_rates"]),
            }
            self.snaps.append(snap)
            s = self.stats
            s["frames"] += 1
            if self.g.np is not None:
                s["active_frames"] += 1
            s["wave_frames"][self.g.active["lfo"]["wave"] & 7] += 1
            if any(m for m, _c, _f, _d in self.g.active["opcfg"]):
                s["fixed_op_frames"] += 1
            co = [c for _m, c, _f, _d in self.g.active["opcfg"]]
            de = [d for _m, _c, _f, d in self.g.active["opcfg"]]
            if 0 in co:
                s["coarse0_frames"] += 1
            if 31 in co:
                s["coarse31_frames"] += 1
            if 0 in de:
                s["detune0_frames"] += 1
            if 14 in de:
                s["detune14_frames"] += 1
            if not self._wrote:
                s["no_traffic_frames"] += 1
            s["scenario_frames"][self._tag] = \
                s["scenario_frames"].get(self._tag, 0) + 1
            for inp in row["inputs"]:
                self._ixs.add((inp & 0xFFFFFF) >> 14)
                hib = inp >> 24              # arithmetic (C int32 >>)
                self._hibits.add(hib)
                if inp < 0:
                    s["negative_lookup_inputs"] += 1
            self._wrote = False

    # -- contracted host writes ----------------------------------------------

    def set_patch(self, oplist=None, **kw):
        """Update the SHADOW patch image (sources; frozen conversions are
        computed at send time, mirroring the host conversion step)."""
        sh = self.g.shadow
        if oplist is not None:
            sh["opcfg"] = [(o["mode"], o["coarse"], o["fine"], o["detune"])
                           for o in oplist]
        if "peg_rates" in kw:
            sh["peg_rates"] = list(kw["peg_rates"])
        if "peg_levels" in kw:
            sh["peg_levels"] = list(kw["peg_levels"])
        if "lfo" in kw:
            sh["lfo"].update(kw["lfo"])
        if "pm_depth_src" in kw:
            sh["pm_depth_src"] = kw["pm_depth_src"]
        if "am_depth_src" in kw:
            sh["am_depth_src"] = kw["am_depth_src"]
        if "pm_sens_src" in kw:
            sh["pm_sens_src"] = kw["pm_sens_src"]
        if "transpose" in kw:
            sh["transpose"] = kw["transpose"]

    def send_patch(self):
        """Write the full shadow image + COMMIT (one write per contracted
        register; the commit is atomic at the next frame tick, DEC-019)."""
        sh = self.g.shadow
        for op in range(6):
            mode, coarse, fine, detune = sh["opcfg"][op]
            self._w(A_OPCFG + op,
                    (detune << 24) | (fine << 16) | (coarse << 8) | mode)
            self._w(A_FTERM + op, host_fine_term(fine) & U32)
        self._w(A_PEGR, (sh["peg_rates"][0] | (sh["peg_rates"][1] << 8) |
                         (sh["peg_rates"][2] << 16) |
                         (sh["peg_rates"][3] << 24)))
        self._w(A_PEGL, (sh["peg_levels"][0] | (sh["peg_levels"][1] << 8) |
                         (sh["peg_levels"][2] << 16) |
                         (sh["peg_levels"][3] << 24)))
        lf = sh["lfo"]
        self._w(A_LFOP, (lf["rate"] | (lf["delay"] << 8) |
                         (lf["sync"] << 16) | ((lf["wave"] & 7) << 24)))
        self._w(A_PMDEP, (sh["pm_depth_src"] * 165) >> 6)
        self._w(A_PMSENS, P.PITCHMODSENSTAB[sh["pm_sens_src"] & 7])
        self._w(A_TRANS, sh["transpose"])
        self._w(A_AMDEP, (sh["am_depth_src"] * 165) >> 6)
        self._w(A_CMD, CMD_COMMIT)
        sh["pm_depth"] = (sh["pm_depth_src"] * 165) >> 6
        sh["am_depth"] = (sh["am_depth_src"] * 165) >> 6
        sh["pm_sens"] = P.PITCHMODSENSTAB[sh["pm_sens_src"] & 7]

    def note_on(self, note):
        """NOTE_ON event: the host sends the frozen per-op detune deltas
        (computed for the DOCUMENTED transposed note -- the same shift the
        core applies) plus the command. Deltas are computed against the
        SHADOW image: the tick applies a pending commit before the note-on,
        so the patch that will be active at note-on is the shadow."""
        shift = self.g.shadow["transpose"] - 24
        l = P.midinote_to_logfreq(note + shift)
        for op in range(6):
            mode, _coarse, _fine, detune = self.g.shadow["opcfg"][op]
            delta = host_detune_delta(l, detune) if mode == 0 else 0
            self._w(A_DDELTA + op, delta & U32)
        self._w(A_CMD, (note << 8) | CMD_NOTE_ON)
        if self.g.shadow["lfo"]["sync"]:
            self.stats["keysync_noteons"] += 1
        if shift != 0:
            self.stats["shifted_noteons"] += 1

    def note_off(self):
        self._w(A_CMD, CMD_NOTE_OFF)

    def bend(self, value, up=3, dn=3, step=0):
        """Bend event: the frozen scaled integer crosses the boundary."""
        pb = host_bend_pb(value, up, dn, step)
        self._w(A_BEND, pb & U32)
        self.g.perf["bend_value"] = value
        self.g.perf["range_up"] = up
        self.g.perf["range_dn"] = dn
        self.g.perf["step"] = step
        self.stats["bend_events"] += 1

    def perf_write(self, addr, value, key):
        self._w(addr, value & U32)
        self.g.perf[key] = i32(value)


# ---------------------------------------------------------------------------
# Directed scenarios (deterministic; the golden judges every frame)
# ---------------------------------------------------------------------------

NEUTRAL_PEG = {"peg_rates": [99, 99, 99, 99], "peg_levels": [50, 50, 50, 50]}
NEUTRAL_LFO = {"lfo": {"rate": 0, "delay": 0, "sync": 1, "wave": 0}}


def ops(mode=0, coarse=1, fine=0, detune=7):
    return [{"mode": mode, "coarse": coarse, "fine": fine,
             "detune": detune} for _ in range(6)]


def build_vectors():
    gen = Gen()
    g = gen.g
    rng = random.Random(SEED)

    # 0. reset-state frames before any traffic (DEC-011 explicit reset,
    #    repeatability of the zeroized LFO/PEG state).
    gen._set_tag("reset-state")
    gen.frame(4)

    # A. static pitch matrix: mode x coarse x detune x fine (N05 cases
    #    bd-coarse0/31, bd-detune0/14, bd-fine99, osc-fixed-carrier), each
    #    at a cycling keyboard note; ratio ops must track the keyboard,
    #    fixed ops must not.
    gen._set_tag("static-matrix")
    note_i = 0
    for mode in (0, 1):
        for coarse in (0, 1, 2, 3, 4, 15, 20, 31):
            for detune in (0, 1, 7, 8, 13, 14):
                for fine in (0, 50, 99):
                    gen.set_patch(ops(mode, coarse, fine, detune),
                                  **NEUTRAL_PEG, **NEUTRAL_LFO,
                                  transpose=24, pm_depth_src=0,
                                  am_depth_src=0, pm_sens_src=0)
                    gen.send_patch()
                    gen.note_on(NOTES[note_i % len(NOTES)])
                    note_i += 1
                    gen.frame(2)
                    gen.note_off()
                    gen.frame(1)

    # A2. fixed-mode keyboard invariance, directed (issue #19 named case):
    #     same fixed patch at three notes -> identical fixed increments.
    gen._set_tag("fixed-keyboard-invariance")
    for note in (48, 60, 72):
        gen.set_patch(ops(mode=1, coarse=4, fine=50, detune=10),
                      **NEUTRAL_PEG, **NEUTRAL_LFO,
                      transpose=24, pm_depth_src=0, am_depth_src=0,
                      pm_sens_src=0)
        gen.send_patch()
        gen.note_on(note)
        gen.frame(2)

    # B. transpose (DOCUMENTED mode; the core applies patch transpose - 24
    #    exactly once, DEC-023 / contract 4.2). Same note under different
    #    transposes must move the ratio increments; the pinned-wrapper
    #    omission (perf-transpose-12) is out of RTL scope by contract.
    gen._set_tag("transpose-documented")
    for tr in (0, 12, 24, 36, 50, 99):
        for note in (60, 72):
            gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG, **NEUTRAL_LFO,
                          transpose=tr, pm_depth_src=0, am_depth_src=0,
                          pm_sens_src=0)
            gen.send_patch()
            gen.note_on(note)
            gen.frame(2)

    # C. bend scaling (frozen integers across the boundary) + master tune:
    #    ev-pitch-bend-up/down/center corners, asymmetric ranges, coarse
    #    steps, master tune offsets.
    gen._set_tag("bend-frozen")
    gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG, **NEUTRAL_LFO,
                  transpose=24, pm_depth_src=0, am_depth_src=0,
                  pm_sens_src=0)
    gen.send_patch()
    gen.note_on(60)
    for bv, up, dn, st in (
            (0x2000, 3, 3, 0), (0x2FFF, 3, 3, 0), (0x1001, 3, 3, 0),
            (0x3FFF, 12, 12, 0), (0x0000, 12, 12, 0), (0x2001, 3, 12, 0),
            (0x1FFF, 12, 3, 0), (0x3FFF, 3, 3, 1), (0x0000, 3, 3, 2),
            (0x2AAA, 0, 0, 0), (0x1555, 6, 6, 0), (0x2000, 3, 3, 0)):
        gen.bend(bv, up, dn, st)
        gen.frame(1)
    for tune in (0, 2345, -2345):
        gen.perf_write(A_MTUNE, tune & U32, "master_tune")
        gen.frame(1)
    gen.note_off()
    gen.frame(1)

    # D. pitch-EG shapes (peg-up/peg-down/slow rates/nonzero final/
    #    retrigger/repeated-note; env.cc segment rule).
    shapes = [
        ("peg-up-fast", [99, 99, 99, 99], [10, 50, 80, 40], 14, 18),
        ("peg-down-fast", [99, 99, 99, 99], [90, 60, 30, 70], 14, 18),
        ("peg-up-slow", [3, 3, 3, 3], [10, 50, 80, 40], 44, 14),
        ("peg-down-slow", [3, 3, 3, 3], [90, 60, 30, 70], 44, 14),
        ("peg-rate3-mixed", [3, 50, 3, 90], [20, 60, 100, 10], 40, 14),
        ("peg-nonzero-final", [90, 90, 90, 90], [10, 40, 70, 99], 14, 16),
        ("peg-hold", [99, 99, 99, 99], [50, 50, 50, 0], 14, 16),
        ("peg-retrigger", [70, 70, 70, 70], [10, 60, 99, 30], 30, 14),
    ]
    for name, rates, levels, hold, rel in shapes:
        gen._set_tag("peg-" + name)
        gen.set_patch(ops(0, 1, 0, 7), peg_rates=rates, peg_levels=levels,
                      **NEUTRAL_LFO, transpose=24, pm_depth_src=0,
                      am_depth_src=0, pm_sens_src=0)
        gen.send_patch()
        gen.note_on(60)
        gen.frame(hold)
        if name == "peg-retrigger":
            gen.note_on(64)          # retrigger mid-shape
            gen.frame(10)
        gen.note_off()
        gen.frame(rel)
        # repeated note (ev-repeated-note style): off/on inside one window
        gen.note_off()
        gen.note_on(67)
        gen.frame(6)

    # E. LFO sweep: all six waveforms x speeds x delay ramps x PM depth/
    #    sensitivity (lfo-wave0..5, lfo-speed, lfo-delay99, lfo-pm-sens7),
    #    keysync on (keydown restarts phase + delay ramp each note-on).
    for wave in range(6):
        for rate in (0, 99):
            for delay in (0, 99):
                for depth, sens in ((0, 0), (99, 7)):
                    gen._set_tag("lfo-w%d-r%d-d%d-pm%d" %
                                 (wave, rate, delay, depth))
                    gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG,
                                  lfo={"rate": rate, "delay": delay,
                                       "sync": 1, "wave": wave},
                                  transpose=24, pm_depth_src=depth,
                                  am_depth_src=0, pm_sens_src=sens)
                    gen.send_patch()
                    gen.note_on(60)
                    gen.frame(40)
    # mid speed + keysync-off spots (lfo-keysync0; DEC-020 free-running)
    for wave in range(6):
        gen._set_tag("lfo-w%d-r35-nosync" % wave)
        gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG,
                      lfo={"rate": 35, "delay": 0, "sync": 0, "wave": wave},
                      transpose=24, pm_depth_src=99, am_depth_src=0,
                      pm_sens_src=7)
        gen.send_patch()
        gen.note_on(60)
        gen.frame(20)
        gen.note_off()
        gen.frame(10)

    # F. controller PM path (documented nonzero wheel range; the pinned
    #    default range 0 leaves CC1 inert -- registry ev-modwheel-127).
    for cpm in (13, 125):
        for sens in (0, 7):
            gen._set_tag("ctrlpm-%d-sens%d" % (cpm, sens))
            gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG,
                          lfo={"rate": 99, "delay": 0, "sync": 1, "wave": 0},
                          transpose=24, pm_depth_src=0, am_depth_src=0,
                          pm_sens_src=sens)
            gen.send_patch()
            gen.note_on(60)
            gen.perf_write(A_CTRLPM, cpm, "ctrl_pm")
            gen.frame(20)
            gen.perf_write(A_CTRLPM, 0, "ctrl_pm")
            gen.frame(4)

    # G. AM depth composition (ampmod-style: amd_mod boundary values).
    for am_src, eg, cam in ((0, 127, 0), (99, 127, 0), (99, 0, 0),
                            (50, 127, 100), (0, 127, 100)):
        gen._set_tag("am-%d-eg%d-cam%d" % (am_src, eg, cam))
        gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG,
                      lfo={"rate": 99, "delay": 0, "sync": 1, "wave": 1},
                      transpose=24, pm_depth_src=0, am_depth_src=am_src,
                      pm_sens_src=0)
        gen.send_patch()
        gen.note_on(60)
        gen.perf_write(A_CTRLAM, cam, "ctrl_am")
        gen.perf_write(A_EGMOD, eg, "eg_mod")
        gen.frame(24)

    # H. seeded pseudo-random coverage: random patches/notes/controllers,
    #    re-commits and re-note-ons (deterministic, fixed seed). Notes and
    #    transposes are guarded to the frozen model's Freqlut domain
    #    (freqlut.cc:44-45: hibits <= 20; beyond it the model's own lookup
    #    raises, so the vector domain respects the declared model boundary).
    gen._set_tag("random")
    MAX_COARSEMUL = max(abs(c) for c in P.COARSEMUL)
    PEG_MAX = P.PitchEnv.PITCHENV_TAB[99] << 19
    PM_BEND_HEADROOM = (1 << 24) + (1 << 23)

    def in_domain(note):
        shift = gen.g.shadow["transpose"] - 24
        worst = (P.A4_BASE_LOGFREQ + P.SEMI_LOGFREQ * (note + shift)
                 + MAX_COARSEMUL + PEG_MAX + PM_BEND_HEADROOM)
        return (worst >> 24) <= 20

    for k in range(700):
        if k % 10 == 0:
            opcfg = []
            for _op in range(6):
                opcfg.append({"mode": rng.randrange(2),
                              "coarse": rng.choice((0, 1, 2, 3, 4, 15, 20,
                                                    31)),
                              "fine": rng.choice((0, 1, 50, 99)),
                              "detune": rng.choice((0, 1, 7, 8, 13, 14))})
            gen.set_patch(opcfg,
                          peg_rates=[rng.randrange(100) for _ in range(4)],
                          peg_levels=[rng.randrange(100) for _ in range(4)],
                          lfo={"rate": rng.choice((0, 17, 50, 99)),
                               "delay": rng.choice((0, 30, 70, 99)),
                               "sync": rng.randrange(2),
                               "wave": rng.randrange(6)},
                          transpose=rng.choice((0, 12, 24, 36, 50, 99)),
                          pm_depth_src=rng.choice((0, 40, 99)),
                          am_depth_src=rng.choice((0, 60, 99)),
                          pm_sens_src=rng.randrange(8))
            note = rng.choice((24, 48, 60, 69, 81, 96))
            if not in_domain(note):
                note = next((n for n in (48, 36, 24, 12) if in_domain(n)), 24)
            gen.send_patch()
            gen.note_on(note)
        elif k % 17 == 0:
            gen.perf_write(A_CTRLPM, rng.choice((0, 13, 125)), "ctrl_pm")
        elif k % 13 == 0:
            gen.bend(rng.choice((0x0000, 0x2000, 0x2AAA, 0x3FFF)))
        elif k % 11 == 0:
            gen.note_off()
        gen.frame(1)

    # I. burst event timing: 8 note-on/off pairs inside ONE inter-frame
    #    window (last command wins at the tick; latency <= 1 frame,
    #    contract 4.2), then normal frames.
    gen._set_tag("burst")
    gen.set_patch(ops(0, 1, 0, 7), **NEUTRAL_PEG, **NEUTRAL_LFO,
                  transpose=24, pm_depth_src=0, am_depth_src=0, pm_sens_src=0)
    gen.send_patch()
    for k in range(8):
        gen.note_on(60 + k)
        gen.note_off()
    gen.note_on(72)
    gen.frame(6)
    gen.stats["burst_frames"] = 6

    # coverage over the freqlut index/shift domain
    gen.stats["distinct_freqlut_ix"] = len(gen._ixs)
    gen.stats["distinct_hibits"] = len(gen._hibits)
    gen.stats["seed"] = SEED
    return gen


# ---------------------------------------------------------------------------
# Vector / expected file writers (deterministic bytes)
# ---------------------------------------------------------------------------

def vector_text(records):
    out = [
        "# h06 pitch/mod directed vectors (issue #28)",
        "# generated by tools/h06_compare.py from the FROZEN PYTHON MODEL",
        "# golden: src/gf180_dx7/model/pitch.py composed per the pinned",
        "# integrated order (src/gf180_dx7/model/integrated.py F-3, DR-0006)",
        "# ROM bytes: reference/tables/freqlut_table.hex pinned by "
        "reference/tables/manifest.json",
        "#   freqlut sha256 6d3c597008e85c67cfa1165ef4038a3c1c9c56a60a4b441b"
        "7486296a897dc64d",
        "# ROM bytes: reference/tables/lfo_sin_table.hex regenerated from "
        "build_sintab(),",
        "#   re-verified byte-for-byte on every run",
        "# W <addr hex2> <data hex8>  one contracted cfg write (per-event)",
        "# F                          one frame tick (64-sample frame)",
    ]
    for kind, a, d in records:
        if kind == "W":
            out.append("W %02x %08x" % (a, d))
        else:
            out.append("F")
    return "\n".join(out) + "\n"


def expected_text(expected):
    out = [
        "# h06 expected outputs (golden model; BIT-EXACT bar, no tolerance)",
        "# per frame: freq0..5 hex8, peg hex8, lfo_val hex6, lfo_delay hex6,"
        " amd hex8",
    ]
    for row in expected:
        out.append("%s %s %s %s %s %s %08x %06x %06x %08x" %
                   (tuple("%08x" % f for f in row["freqs"]) +
                    (row["peg"], row["lfo_val"], row["lfo_delay"],
                     row["amd"])))
    return "\n".join(out) + "\n"


def parse_result_text(path):
    rows = []
    with open(path, "r", encoding="ascii") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 10 or any(
                    not re.fullmatch(r"[0-9a-fA-F]+", p) for p in parts):
                raise CheckFailure(
                    "%s:%d: malformed result line %r" % (path, lineno, line))
            rows.append(tuple(int(p, 16) for p in parts))
    return rows


# ---------------------------------------------------------------------------
# simulation
# ---------------------------------------------------------------------------

MUTANTS = {
    "lsb": ("-DMUTATE_INCREMENT_LSB",
            "op-0 phase increment +1 LSB (boundary-value mutation)"),
    "fixedkb": ("-DMUTATE_FIXED_TRACKS_KEYBOARD",
                "fixed mode tracks the keyboard (N05 named control)"),
    "sync": ("-DMUTATE_LFO_SYNC_IGNORED",
             "LFO keydown keeps phase (N05 named control)"),
    "persample": ("-DH06_PER_SAMPLE_HOST",
                  "illegal per-sample host stream gates increments "
                  "(split violation)"),
}


def tool_identity(iverilog):
    try:
        out = subprocess.run([iverilog, "-V"], capture_output=True,
                             text=True)
    except OSError:
        raise CouldNotRun(f"iverilog binary not found: {iverilog}")
    first = (out.stdout or out.stderr).strip().splitlines()
    if not first:
        raise CouldNotRun("iverilog -V produced no output")
    return first[0].strip()


def run_sim(iverilog, vvp, mutant, vectors, outdir):
    """Compile + run the bench (real build when mutant is None); return
    (actual_rows, sim_meta)."""
    if shutil.which(iverilog) is None:
        raise CouldNotRun(f"iverilog not found on PATH: {iverilog}")
    if shutil.which(vvp) is None:
        raise CouldNotRun(f"vvp not found on PATH: {vvp}")
    name = "tb.vvp" if mutant is None else ("tb_%s.vvp" % mutant)
    vvp_out = os.path.join(tempfile.mkdtemp(prefix="h06_sim_"), name)
    stem = "actual.txt" if mutant is None else ("actual_%s.txt" % mutant)
    actual_path = os.path.join(outdir, stem)
    cmd = [iverilog, "-g2012", "-o", vvp_out]
    if mutant is not None:
        cmd.append(MUTANTS[mutant][0])
    cmd += [os.path.join(REPO_ROOT, RTL_REL), os.path.join(REPO_ROOT, TB_REL)]
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=REPO_ROOT)
    if proc.returncode != 0:
        raise CheckFailure("iverilog compile failed (%s): " % (mutant,) +
                           (proc.stderr or proc.stdout)[-500:])
    try:
        run = subprocess.run([vvp, vvp_out, f"+vectors={vectors}",
                              f"+actual={actual_path}"],
                             capture_output=True, text=True, cwd=REPO_ROOT,
                             timeout=1800)
        out = run.stdout + run.stderr
        if run.returncode != 0 or "FAIL" in out:
            raise CheckFailure(
                "bench failed (%s): %s" % (mutant, out.strip()[-500:]))
        m = re.search(r"PASS tb_pitch_mod \(frames=(\d+) writes=(\d+) "
                      r"captured=(\d+) maxcycles=(\d+)\)", out)
        if not m:
            raise CheckFailure(
                "bench produced no PASS summary (%s): %s"
                % (mutant, out[-300:]))
        rows = parse_result_text(actual_path)
    finally:
        try:
            os.remove(vvp_out)
        except OSError:
            pass
    return rows, {
        "bench_frames": int(m.group(1)),
        "bench_writes": int(m.group(2)),
        "bench_captured": int(m.group(3)),
        "bench_max_cycles_per_frame": int(m.group(4)),
        "actual_file": stem,
        "vvp_stdout_tail": out.strip()[-200:],
    }


def compare_bit_exact(expected, actual, snaps):
    """BIT-EXACT comparison; returns (mismatches, first_mismatch|None).
    Each row is (freq0..5, peg, lfo_val, lfo_delay, amd)."""
    mismatches = []
    first = None
    count = len(expected)
    if len(actual) != count:
        mismatches.append({
            "frame_index": min(count, len(actual)),
            "error": "row-count mismatch: expected %d rows, RTL dumped %d"
                     % (count, len(actual)),
        })
    names = ("freq0", "freq1", "freq2", "freq3", "freq4", "freq5",
             "peg", "lfo_val", "lfo_delay", "amd")
    for n in range(min(count, len(actual))):
        exp = expected[n]
        exp_row = (exp["freqs"][0], exp["freqs"][1], exp["freqs"][2],
                   exp["freqs"][3], exp["freqs"][4], exp["freqs"][5],
                   exp["peg"], exp["lfo_val"], exp["lfo_delay"], exp["amd"])
        act = actual[n]
        if exp_row != act:
            diff = {}
            for name, e, a in zip(names, exp_row, act):
                if e != a:
                    diff[name] = ["%08x" % e, "%08x" % a]
            snap = snaps[n]
            mm = {
                "frame_index": n,
                "scenario_tag": snap["tag"],
                "note": snap["note"],
                "transpose": snap["transpose"],
                "lfo": snap["lfo"],
                "bend": {"value": snap["perf"]["bend_value"],
                         "range_up": snap["perf"]["range_up"],
                         "range_dn": snap["perf"]["range_dn"],
                         "step": snap["perf"]["step"]},
                "ctrl_pm": snap["perf"]["ctrl_pm"],
                "peg_rates": snap["peg_rates"],
                "peg_levels": snap["peg_levels"],
                "basepitch": ["%08x" % (b & U32)
                              for b in snap["basepitch"]],
                "diff": diff,
            }
            mismatches.append(mm)
            if first is None:
                first = mm
    return mismatches, first


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--outdir", default=os.path.join(REPO_ROOT, OUTDIR_REL))
    ap.add_argument("--gen-only", action="store_true",
                    help="write vectors + expected + gen report, no sim")
    ap.add_argument("--skip-mutants", action="store_true",
                    help="run only the real build")
    ap.add_argument("--mutant", choices=sorted(MUTANTS), default=None,
                    help="run only one negative-control mutant build")
    ap.add_argument("--write-sin-hex", action="store_true",
                    help="(re)generate reference/tables/lfo_sin_table.hex")
    ap.add_argument("--iverilog", default="iverilog")
    ap.add_argument("--vvp", default="vvp")
    args = ap.parse_args(argv)

    if args.write_sin_hex:
        write_sin_hex()

    try:
        tabs = verify_freqlut()
        verify_sin_hex()
        gen = build_vectors()
    except CheckFailure as exc:
        print("FAIL: %s" % exc, file=sys.stderr)
        return 1
    except Exception as exc:  # TableDrift or model error: could not run
        print(f"COULD-NOT-RUN: golden generation failed: {exc}",
              file=sys.stderr)
        return 2

    stats = gen.stats
    if stats["frames"] < MIN_FRAMES or stats["active_frames"] < MIN_ACTIVE:
        print("FAIL: vector set has %d frames (%d active) < required "
              "%d/%d" % (stats["frames"], stats["active_frames"],
                         MIN_FRAMES, MIN_ACTIVE), file=sys.stderr)
        return 1

    os.makedirs(args.outdir, exist_ok=True)
    vectors_path = os.path.join(args.outdir, "vectors.txt")
    expected_path = os.path.join(args.outdir, "expected.txt")
    with open(vectors_path, "w", encoding="ascii") as f:
        f.write(vector_text(gen.records))
    with open(expected_path, "w", encoding="ascii") as f:
        f.write(expected_text(gen.expected))

    if args.gen_only:
        gen_report = {
            "schema": "h06-gen-report/1",
            "issue": 28,
            "planning_id": "H06",
            "golden": "src/gf180_dx7/model/pitch.py via the pinned "
                      "integrated order (integrated.py F-3, DR-0006)",
            "tables_manifest": "reference/tables/manifest.json",
            "vector_stats": stats,
            "vectors_sha256": sha256_file(vectors_path),
            "expected_sha256": sha256_file(expected_path),
        }
        with open(os.path.join(args.outdir, "gen_report.json"), "w",
                  encoding="utf-8") as f:
            json.dump(gen_report, f, indent=1, sort_keys=True)
            f.write("\n")
        print(f"h06_compare: generated {stats['frames']} frames "
              f"({stats['writes']} writes) -> {vectors_path}")
        return 0

    report = {
        "schema": "h06-compare-report/1",
        "issue": 28,
        "planning_id": "H06",
        "claim_boundary": "RTL-vs-frozen-model bit-exactness only; no "
                          "synthesis, timing, fit, or audibility claim; "
                          "model-vs-reference agreement is N05's result",
        "golden": "src/gf180_dx7/model/pitch.py via the pinned integrated "
                  "order (src/gf180_dx7/model/integrated.py compute_frame, "
                  "seam finding F-3, DR-0006)",
        "host_core_split": "docs/CONTRACT-CORE-v1.md sections 4.2/4.7/5; "
                           "DEC-010/DEC-014/DEC-019/DEC-022/DEC-023; "
                           "NUM-008 event_time_pitch_math",
        "transpose_ruling": "documented mode (transpose - 24) applied once "
                            "in-core per DEC-023 + contract 4.2; the "
                            "pinned-wrapper omission (registry "
                            "perf-transpose-12) is a wrapper finding, not "
                            "RTL scope",
        "tables": {
            "manifest": "reference/tables/manifest.json",
            "freqlut_sha256": tabs["freqlut_table"].sha256,
            "lfo_sin_hex": "reference/tables/lfo_sin_table.hex",
            "lfo_sin_sha256_packed_s32": hashlib.sha256(
                b"".join(v.to_bytes(4, "little", signed=True)
                         for v in sin_table_values())).hexdigest(),
        },
        "tool_identity": {
            "iverilog": tool_identity(args.iverilog),
            "python": sys.version.split()[0],
            "rtl": {p: sha256_file(os.path.join(REPO_ROOT, p))
                    for p in (RTL_REL, TB_REL)},
        },
        "vector_stats": stats,
        "vectors_sha256": sha256_file(vectors_path),
        "expected_sha256": sha256_file(expected_path),
    }

    status = "FAIL"
    try:
        if args.mutant is None:
            actual, meta = run_sim(args.iverilog, args.vvp, None,
                                   vectors_path, args.outdir)
            mismatches, first = compare_bit_exact(gen.expected, actual,
                                                  gen.snaps)
            report["equivalence"] = dict(
                meta,
                expected_rows=len(gen.expected),
                actual_rows=len(actual),
                mismatches=len(mismatches),
                first_mismatch=first,
                status="PASS" if not mismatches else "FAIL",
            )
        if not args.skip_mutants:
            selected = [args.mutant] if args.mutant else sorted(MUTANTS)
            controls = {}
            for mut in selected:
                m_actual, m_meta = run_sim(args.iverilog, args.vvp, mut,
                                           vectors_path, args.outdir)
                m_mm, m_first = compare_bit_exact(gen.expected, m_actual,
                                                  gen.snaps)
                met = len(m_mm) > 0
                controls[mut] = dict(
                    m_meta,
                    define=MUTANTS[mut][0],
                    description=MUTANTS[mut][1],
                    mismatches=len(m_mm),
                    first_mismatch=m_first,
                    status="MET (mutant fails exact compare as required)"
                           if met else
                           "NOT MET (mutant passed -- no resolution)",
                )
            report["negative_controls"] = controls

        eq_ok = args.mutant is not None or \
            report["equivalence"]["status"] == "PASS"
        nc_ok = True
        if not args.skip_mutants:
            nc_ok = all(c["status"].startswith("MET") for c in
                        report["negative_controls"].values())
        status = "PASS" if (eq_ok and nc_ok) else "FAIL"
        report["status"] = status
    except CouldNotRun as exc:
        print(f"COULD-NOT-RUN: {exc}", file=sys.stderr)
        return 2
    except CheckFailure as exc:
        report["status"] = "FAIL"
        report["error"] = str(exc)
        status = "FAIL"

    with open(os.path.join(args.outdir, "compare_report.json"), "w",
              encoding="utf-8") as f:
        json.dump(report, f, indent=1, sort_keys=True)
        f.write("\n")

    if "equivalence" in report:
        eq = report["equivalence"]
        print(f"equivalence: {eq['status']} "
              f"({eq['expected_rows']} frames, {eq['mismatches']} "
              f"mismatches, worst-tail {eq['bench_max_cycles_per_frame']} "
              f"clk/frame)")
        if eq["first_mismatch"]:
            fm = eq["first_mismatch"]
            print("  first mismatch at frame %d [%s]: diff=%s"
                  % (fm["frame_index"], fm.get("scenario_tag"),
                     json.dumps(fm.get("diff"), sort_keys=True)))
    for name, c in report.get("negative_controls", {}).items():
        print("negative control %s: %s (%d mismatches)"
              % (name, c["status"], c["mismatches"]))
    print(f"report: {os.path.join(args.outdir, 'compare_report.json')}")
    return 0 if status == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main())
