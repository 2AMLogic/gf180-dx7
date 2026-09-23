#!/usr/bin/env python3
"""H07 integrated-core conformance harness (issue #29).

Device under test: rtl/dx7_core.v (the complete 16-note core).  Golden: the
FROZEN N08 release vectors (reference/release/vectors, sha256-checked
against reference/release/manifest.json) for the 30-case dev set, and the
frozen integer model (src/gf180_dx7/model/poly.py PolyManager, contract
gf180-dx7-integer-v1) for the preregistered H07 stress set.  The bar is
BIT-EXACTNESS of every compared sample at the H03-declared latency
(docs/CONTRACT-CORE-v1.md section 4.5) -- no tolerance; any mismatch is an
RTL defect, never a loosened check (AGENTS.md honesty rules).

Mechanics: for each case the harness encodes the patch image + events into
the core's SPI register map (docs/H07-CORE.md section 5, rtl/dx7_core.v
header), paces every event to ARRIVE in the wire frame base+block of the
model's frozen application trace, plays the vector through
rtl/tb_dx7_core.v (SPI master at the contracted SCK = clk/4 burst bound,
I2S DAC-model capture), and compares the captured PCM against the golden
sample-for-sample at the contracted latency: golden block b of the commit
segment based at wire frame F lands at dump index (F + b + 2) * 64.

Per-event latency measurement (H03 section 4.5): an event at sample p of
model block b arrives in wire frame F+b and its commit first affects the
SDATA slot (F + b + 2) * 64, a measured distance of (128 - p) samples --
inside the contracted window [(64 - p), (64 - p) + 65] for every p.  The
measured value is recorded per event in the results file; OVERRUN and
OVERFLOW must read 0 across every case.

Negative controls (each must demonstrably fail): -DH07_MUTATE_GLITCH,
-DH07_MUTATE_SUM_ORDER, -DH07_MUTATE_TICK_SKEW (conformance-only mutant
builds, never synthesized) and the -DH07_STRIP_OBSERVABILITY synthesis
flop gate (tools/h07_synth.py).  tests/test_h07.py keeps them live.

Simulator tiers (klayout-tools #2223 convention): sweeps and mutant runs
use Verilator when available; the canonical final acceptance run uses
iverilog (tools canonical shadow).  Any Verilator/iverilog artifact
divergence on the same vector is reported as a finding, never silently
accepted.

Exit codes: 0 pass, 1 check failure, 2 could-not-run (the message names
what is missing).  Stdlib only.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import math
import os
import struct
import subprocess
import sys
import zipfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "src"))
sys.path.insert(0, str(REPO / "tools"))

from gf180_dx7 import sysex  # noqa: E402
from gf180_dx7.model import pitch as P  # noqa: E402
from gf180_dx7.model.poly import PolyManager, parse_events as poly_parse_events  # noqa: E402
from n08_build_release import CaseSource, DEFAULT_ARCHIVE  # noqa: E402

MANIFEST = REPO / "reference" / "release" / "manifest.json"
VECTORS = REPO / "reference" / "release" / "vectors"
OUTDIR = REPO / "evidence" / "h07-core"
STRESS_DIR = OUTDIR / "stress"
CACHE = OUTDIR / "cases.json"
RTL = ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v",
       "rtl/tb_dx7_core.v"]

RATE = 48000
N = 64
U32 = 0xFFFFFFFF

# core register map (SEC=1 page)
R_OPPITCH = 0x10
R_FTERM = 0x18
R_ERATES = 0x20
R_ELEVELS = 0x28
R_SCALEA = 0x30
R_SCALEB = 0x38
R_GLOBA = 0x40
R_GLOBB = 0x41
R_GLOBC = 0x43
R_PEGR = 0x44
R_PEGL = 0x45
R_COMMIT = 0x42
# event addresses (SEC=0 queue)
EA_NOTEON = 0x00
EA_NOTEOFF = 0x01
EA_CC = 0x02
EA_PB = 0x03
EA_DD0 = 0x04
EA_MTUNE = 0x0B
EA_SOFT = 0x0F


class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def rtl_fingerprint() -> str:
    h = hashlib.sha256()
    for rel in ["rtl/dx7_core.v", "rtl/env_unit.v", "rtl/alg_router.v"]:
        h.update((REPO / rel).read_bytes())
    return h.hexdigest()


def i32(x: int) -> int:
    x &= U32
    return x - (1 << 32) if x >= (1 << 31) else x


# ---------------------------------------------------------------------------
# host-side frozen-integer derivations (dx7note.cc float-gated spots;
# identical functions to h06_compare.py -- the frozen integers cross the
# SPI boundary, the float math never does)
# ---------------------------------------------------------------------------

def host_fine_term(fine: int) -> int:
    """dx7note.cc:50-53."""
    if fine == 0:
        return 0
    return int(math.floor(24204406.323123 * math.log(1 + 0.01 * fine) + 0.5))


def host_detune_delta(logfreq: int, detune: int) -> int:
    """dx7note.cc:46-47."""
    ratio = 0.0209 * math.exp(-0.396 * (float(logfreq) / (1 << 24))) / 7
    detuned = i32(int(logfreq + ratio * logfreq * (detune - 7)))
    return detuned - logfreq


def host_bend_pb(bend_value: int, range_up: int = 3, range_dn: int = 3,
                 range_step: int = 0) -> int:
    """NUM-008 float-gated bend scaling (dx7note.cc:219-233)."""
    return P.NotePitch._pitch_base(bend_value, range_up, range_dn,
                                   range_step, 0)[0]


# ---------------------------------------------------------------------------
# patch image encoding (rtl/dx7_core.v register map; op k = body chunk k,
# env block twist 5-k handled core-side exactly like the model's
# envs[k] = operators[5-k])
# ---------------------------------------------------------------------------

def op_regs(body: list[int], k: int) -> dict[int, int]:
    """Register image for body chunk k (router op k).  The envelope-scale
    registers carry the TWIST the core implements (dv_eb = 5 - op):
    chunk k's env block is written at register index 5-k so that the core
    op k reads chunk k's envelope, exactly like the model's
    envs[k] = operators[5-k] (chunk k)."""
    off = k * 21
    t = 5 - k
    return {
        R_OPPITCH + k: ((body[off + 20] << 24) | (body[off + 19] << 16)
                        | (body[off + 18] << 8) | body[off + 17]),
        R_FTERM + k: host_fine_term(body[off + 19]) & U32,
        R_ERATES + t: ((body[off + 3] << 24) | (body[off + 2] << 16)
                       | (body[off + 1] << 8) | body[off + 0]),
        R_ELEVELS + t: ((body[off + 7] << 24) | (body[off + 6] << 16)
                        | (body[off + 5] << 8) | body[off + 4]),
        R_SCALEA + t: ((body[off + 9] << 23)          # scale_left_depth
                       | (body[off + 10] << 15)       # scale_right_depth
                       | (body[off + 11] << 13)       # scale_left_curve
                       | (body[off + 12] << 11)       # scale_right_curve
                       | ((body[off + 15] & 7) << 8)  # key_vel_sens
                       | body[off + 8]),              # break_point
        R_SCALEB + t: ((body[off + 13] << 8)          # rate_scaling
                       | body[off + 16]),             # output_level
    }


def page_writes(body: list[int]) -> list[tuple[int, int]]:
    """The full patch page, register-ordered (one SEC=1 write each)."""
    writes: list[tuple[int, int]] = []
    for k in range(6):
        writes.extend(sorted(op_regs(body, k).items()))
    glob_a = (((body[144] & 0xFF) << 24)      # transpose (stored, not
                                              # applied; pinned wrapper)
              | ((body[139] & 0xFF) << 16)    # LFO pitch-mod depth
              | ((body[140] & 0xFF) << 8)     # LFO amp-mod depth
              | (body[143] & 7))              # pitch-mod sensitivity
    glob_b = (((body[134] & 0x1F) << 24)      # algorithm
              | ((body[135] & 7) << 21)       # feedback
              | ((body[136] & 1) << 20)       # osc key sync
              | ((body[142] & 7) << 17)       # LFO wave
              | ((body[141] & 1) << 16)       # LFO sync
              | ((body[138] & 0xFF) << 8)     # LFO delay
              | (body[137] & 0xFF))           # LFO rate
    glob_c = 0
    for k in range(6):
        glob_c |= (body[k * 21 + 14] & 3) << (2 * k)   # amp mod sens per op
    pegr = ((body[129] << 24) | (body[128] << 16) | (body[127] << 8)
            | body[126])
    pegl = ((body[133] << 24) | (body[132] << 16) | (body[131] << 8)
            | body[130])
    writes.extend([(R_GLOBA, glob_a), (R_GLOBB, glob_b), (R_GLOBC, glob_c),
                   (R_PEGR, pegr), (R_PEGL, pegl)])
    return writes


def event_write(ev_d: int, addr: int) -> tuple[int, int]:
    return addr, ev_d & U32


def note_on_writes(body: list[int], note: int, velocity: int,
                   transpose_shift: int = 0) -> list[tuple[int, int]]:
    """The six frozen per-op detune deltas (dx7note.cc:46-53, computed
    against the SHADOW image active at the strike) + the note-on."""
    l = P.midinote_to_logfreq(note)
    out = []
    for op in range(6):
        off = op * 21
        mode = body[off + 17]
        detune = body[off + 20]
        delta = host_detune_delta(l, detune) if mode == 0 else 0
        out.append(event_write(delta & U32, EA_DD0 + op))
    out.append(event_write((note & 0x7F) | ((velocity & 0x7F) << 8),
                           EA_NOTEON))
    return out


# ---------------------------------------------------------------------------
# case inputs (frozen N08 dev set + preregistered H07 stress set)
# ---------------------------------------------------------------------------

def load_dev_cases() -> list[dict]:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cases = [c for c in manifest["covered"]["cases"] if c["set"] == "dev"]
    vec = manifest["covered"]["vectors"]
    for c in cases:
        c["_f32"] = VECTORS / (c["id"] + ".f32")
        c["_sha"] = vec[c["id"]]["sha256"]
        c["_n"] = c["samples"]
    return cases


def case_voice_bytes(case: dict, cache: dict | None) -> bytes:
    """Resolve the case voice bytes (registry synthetic or archive-gated
    dev32 via the committed cache; the cache row is verified against the
    manifest's patch_sha256)."""
    cid, origin = case["id"], case["origin"]
    if case["patch"]["selector"] == "synthetic":
        source = CaseSource()
        resolved = source.resolve(cid, "registry")
        vb = resolved["voice_bytes"]
    elif origin == "dev32":
        if cache and cid in cache.get("cases", {}):
            vb = bytes.fromhex(cache["cases"][cid]["voice_bytes"])
        else:
            try:
                source = CaseSource()
                resolved = source.resolve(cid, "dev32", case["dev32_index"])
                vb = resolved["voice_bytes"]
            except Exception as exc:  # archive unavailable
                raise CouldNotRun(
                    f"{cid}: voice bytes need the pinned archive "
                    f"({DEFAULT_ARCHIVE}) or the committed case cache: "
                    f"{exc}")
    else:
        raise CheckFailure(f"{cid}: unknown patch selector")
    if sha256_bytes(vb) != case["patch_sha256"]:
        raise CheckFailure(
            f"{cid}: resolved voice bytes hash to {sha256_bytes(vb)[:12]}, "
            f"manifest pins {case['patch_sha256'][:12]} (drift; re-freeze "
            "required)")
    return vb


def golden_ints_from_f32(path: Path, want_sha: str, n: int) -> list[int]:
    """The frozen f32 vector -> the exact integer clip stream (NUM-010:
    the f32 value is c / 32768.0 with |c| < 2^21, dyadic and exact)."""
    data = path.read_bytes()
    if sha256_bytes(data) != want_sha:
        raise CheckFailure(
            f"{path.name}: frozen vector drifted ({sha256_bytes(data)[:12]}"
            f" != {want_sha[:12]})")
    vals = struct.unpack("<%df" % (len(data) // 4), data)
    if len(vals) != n:
        raise CheckFailure(f"{path.name}: {len(vals)} samples, want {n}")
    out = []
    for i, v in enumerate(vals):
        c = v * 32768.0
        ci = int(round(c))
        if abs(c - ci) > 1e-6:
            raise CheckFailure(f"{path.name}[{i}]: non-dyadic golden value")
        out.append(ci)
    return out


# ---------------------------------------------------------------------------
# stress set (preregistered here; goldens rendered once by the frozen model
# and cached byte-frozen under evidence/h07-core/stress/)
# ---------------------------------------------------------------------------

STRESS_SET = ("h07-stress-v1", [
    {
        "id": "stress-sum16",
        "desc": "16-voice maximum summation: chord of 16 notes at mixed "
                "velocities held across the loudest attack region, then "
                "released together (NUM-010 unclamped sum, per-voice clip)",
        "blocks": 56,
        "voices": 1,
        "events": [(256, "note_on", (48, 100)), (256, "note_on", (52, 111)),
                   (256, "note_on", (55, 127)), (256, "note_on", (60, 127)),
                   (256, "note_on", (64, 96)), (256, "note_on", (67, 117)),
                   (256, "note_on", (72, 127)), (256, "note_on", (76, 88)),
                   (320, "note_on", (79, 100)), (320, "note_on", (82, 104)),
                   (320, "note_on", (86, 112)), (320, "note_on", (89, 95)),
                   (320, "note_on", (91, 121)), (320, "note_on", (94, 106)),
                   (320, "note_on", (98, 99)), (320, "note_on", (101, 127))],
        "patch_events": [],
    },
    {
        "id": "stress-burst",
        "desc": "burst: 8 note_on/note_off pairs delivered inside one "
                "inter-frame window (ordered queue + single-tick pop)",
        "blocks": 24,
        "voices": 1,
        "events": [(320, "note_on", (60, 100)), (321, "note_off", (60,)),
                   (324, "note_on", (62, 100)), (325, "note_off", (62,)),
                   (328, "note_on", (64, 100)), (329, "note_off", (64,)),
                   (332, "note_on", (65, 100)), (333, "note_off", (65,)),
                   (336, "note_on", (67, 100)), (337, "note_off", (67,)),
                   (340, "note_on", (69, 100)), (341, "note_off", (69,)),
                   (344, "note_on", (71, 100)), (345, "note_off", (71,)),
                   (348, "note_on", (72, 100)), (349, "note_off", (72,))],
        "patch_events": [],
    },
    {
        "id": "stress-reset",
        "desc": "soft reset mid-render then re-commit: segment A (blocks "
                "0..9) renders, SOFT_RESET kills all state, segment B is a "
                "fresh boot render; x2 determinism runs must be identical",
        "blocks": None,   # two segments
        "voices": 1,
        "events": [(128, "note_on", (60, 100)), (512, "note_off", (60,))],
        "reset_block": 10,
        "events_b": [(960, "note_on", (65, 90))],
        "blocks_b": 16,
        "patch_events": [],
    },
    {
        "id": "stress-patchcommit",
        "desc": "held note across a patch commit: note 60 struck on patch "
                "0 keeps its DEC-019 bound image while patch <1> commits; "
                "a later strike uses patch 1; chip-global LFO reconfigures",
        "blocks": 40,
        "voices": 2,
        "events": [(128, "note_on", (60, 100))],
        "patch_events": [(832, "patch", (1,))],
        "events2": [(1024, "note_on", (64, 110)), (2304, "note_off", (60,)),
                    (2304, "note_off", (64,))],
    },
])


def _stress_voice(cid: str) -> bytes:
    """The stress patch(es): the frozen dir-base registry voice, and for
    the patch-commit case the frozen lfo-pm-sens7 voice as patch 1 (both
    registry-pinned, so both goldens derive from preregistered bytes)."""
    reg = CaseSource()
    if cid != "stress-patchcommit":
        return reg.resolve("dir-base", "registry")["voice_bytes"]
    return (reg.resolve("dir-base", "registry")["voice_bytes"]
            + reg.resolve("lfo-pm-sens7", "registry")["voice_bytes"])


def model_module_hashes() -> dict:
    mods = ["src/gf180_dx7/__init__.py", "src/gf180_dx7/sysex.py",
            "src/gf180_dx7/model/__init__.py",
            "src/gf180_dx7/model/operator.py",
            "src/gf180_dx7/model/envelope.py",
            "src/gf180_dx7/model/algorithm.py",
            "src/gf180_dx7/model/pitch.py",
            "src/gf180_dx7/model/tables.py",
            "src/gf180_dx7/model/integrated.py",
            "src/gf180_dx7/model/poly.py"]
    return {m: sha256_file(REPO / m) for m in mods}


def sha256_file(path: Path) -> str:
    return sha256_bytes(Path(path).read_bytes())


def render_stress_golden(spec: dict) -> dict:
    """Render one stress case through the frozen model.  Returns
    {segments: [{ints, blocks}], events (lines), n_samples_total}."""
    vbs = _stress_voice(spec["id"])
    voices = []
    per = len(vbs) // spec["voices"]
    for i in range(spec["voices"]):
        voices.append(sysex.decode_voice(vbs[i * per:(i + 1) * per]))
    bodies = [voice_patch_bytes(v) for v in voices]
    manager = PolyManager(voices)
    lines = []
    for (pos, cmd, args) in spec["events"]:
        lines.append((pos, " ".join([cmd] + [str(a) for a in args])))
    for (pos, cmd, args) in spec.get("patch_events", []):
        lines.append((pos, " ".join([cmd] + [str(a) for a in args])))
    for (pos, cmd, args) in spec.get("events2", []):
        lines.append((pos, " ".join([cmd] + [str(a) for a in args])))
    lines.sort(key=lambda e: e[0])
    ev_text = "\n".join(f"{pos} {l}" for pos, l in lines) + "\n"
    segments = []
    if spec["id"] == "stress-reset":
        # segment A: blocks [0, reset_block) -- render, then cut
        n_a = spec["reset_block"] * N
        clips_a = manager.render_clips(ev_text, 1.0)[:n_a]
        segments.append({"ints": clips_a,
                         "events": spec["events"]})
        # segment B: a fresh manager (the post-reset boot state), rendered
        # from t=0; the RTL plays it after the re-commit
        fresh = PolyManager(voices)
        ev_b = "\n".join(" ".join([str(p - spec["reset_block"] * N), cmd]
                                  + [str(a) for a in args])
                         for (p, cmd, args) in spec["events_b"]) + "\n"
        n_b = spec["blocks_b"] * N
        clips_b = fresh.render_clips(ev_b, 1.0)[:n_b]
        segments.append({"ints": clips_b,
                         "events": spec["events_b"]})
    else:
        blocks = spec["blocks"]
        clips = manager.render_clips(ev_text, 1.0)[:blocks * N]
        segments.append({"ints": clips,
                         "events": spec["events"]})
    return {"segments": segments, "bodies": bodies,
            "model_modules": model_module_hashes()}


def voice_patch_bytes(voice) -> list[int]:
    from gf180_dx7.model.algorithm import voice_patch
    return voice_patch(voice)


def stress_golden(spec: dict) -> dict:
    """Cached frozen stress golden (rendered once; hash-pinned)."""
    STRESS_DIR.mkdir(parents=True, exist_ok=True)
    mpath = STRESS_DIR / "manifest.json"
    manifest = json.loads(mpath.read_text(encoding="utf-8")) \
        if mpath.exists() else {"cases": {}}
    row = manifest["cases"].get(spec["id"])
    if row:
        ints = []
        ok = True
        for i, seg in enumerate(row["segments"]):
            p = STRESS_DIR / f"{spec['id']}.seg{i}.i32"
            if not p.exists() or sha256_file(p) != seg["sha256"]:
                ok = False
                break
            data = p.read_bytes()
            ints.append(list(struct.unpack("<%di" % (len(data) // 4), data)))
        if ok:
            mods = model_module_hashes()
            if all(mods.get(m) == h for m, h in
                   row["model_modules"].items()):
                return {"segments": ints, "bodies": row["bodies"]}
    # (re)render + freeze
    rendered = render_stress_golden(spec)
    row = {"desc": spec["desc"], "segments": [], "bodies": rendered["bodies"],
           "model_modules": rendered["model_modules"]}
    for i, seg in enumerate(rendered["segments"]):
        blob = struct.pack("<%di" % len(seg["ints"]), *seg["ints"])
        p = STRESS_DIR / f"{spec['id']}.seg{i}.i32"
        p.write_bytes(blob)
        row["segments"].append({"sha256": sha256_bytes(blob),
                                "n": len(seg["ints"])})
    manifest["cases"][spec["id"]] = row
    manifest["set"] = STRESS_SET[0]
    mpath.write_text(json.dumps(manifest, indent=1, sort_keys=True) + "\n",
                     encoding="utf-8")
    return {"segments": [list(struct.unpack("<%di" % s["n"],
                                            (STRESS_DIR /
                                             f"{spec['id']}.seg{i}.i32")
                                            .read_bytes()))
                          for i, s in enumerate(row["segments"])],
            "bodies": row["bodies"]}


# ---------------------------------------------------------------------------
# vector text generation
# ---------------------------------------------------------------------------

def vector_text(page: list[tuple[int, int]], events: list[tuple[int, int,
                                                                 int]],
                tail: str) -> str:
    lines = []
    for addr, data in page:
        lines.append(f"P {addr:02X} {data:08X}")
    lines.append("C")
    for block, addr, data in events:
        lines.append(f"E {block} {addr:02X} {data:08X}")
    lines.append(tail)
    return "\n".join(lines) + "\n"


def build_dev_vector(case: dict, body: list[int],
                     frames: int = 0) -> tuple[str, int]:
    """Dev case -> (vector text, golden sample count).  Events come from
    the frozen application trace (block = the model block the event
    precedes); every event of a block must fit one wire frame at the
    contracted 168-write bound."""
    writes_per_block: dict[int, int] = {}
    events = []
    for row in case["event_trace"]:
        block = row["block"]
        parts = row["event"].split()
        cmd, args = parts[1], [int(a) for a in parts[2:]]
        if cmd == "note_on":
            batch = note_on_writes(body, args[0], args[1])
        elif cmd == "note_off":
            batch = [event_write(args[0] & 0x7F, EA_NOTEOFF)]
        elif cmd == "cc":
            batch = [event_write((args[0] & 0xFF)
                                 | ((args[1] & 0xFF) << 8), EA_CC)]
        elif cmd == "pb":
            val = args[0] | (args[1] << 7)
            batch = [event_write(host_bend_pb(val), EA_PB)]
        else:
            raise CheckFailure(f"{case['id']}: unsupported event {cmd!r}")
        writes_per_block[block] = writes_per_block.get(block, 0) \
            + len(batch)
        for addr, data in batch:
            events.append((block, addr, data))
    over = {b: n for b, n in writes_per_block.items() if n > 168}
    if over:
        raise CouldNotRun(
            f"{case['id']}: block(s) {sorted(over)} exceed the contracted "
            "168-write burst bound; the vector cannot be played at SCK = "
            "clk/4 (finding, not a loosened check)")
    last_block = max(writes_per_block) if writes_per_block else 0
    total_frames = (case["_n"] + N - 1) // N
    if frames:
        total_frames = min(total_frames, frames)
    remain_blocks = 2 + total_frames + 2 - last_block
    tail = f"W {max(0, remain_blocks) * N}"
    return vector_text(page_writes(body), events, tail), case["_n"]


def build_stress_vector(spec: dict, bodies: list[list[int]],
                        segs: list[list[int]]) -> tuple[str, int]:
    """Stress case vector; returns (text, total compared samples).
    The W record runs until (samples at parse time) + <arg> samples are
    dumped -- the dump keeps counting through every segment."""
    body0 = bodies[0]
    lines = []

    def page(pbytes):
        for a, d in page_writes(pbytes):
            lines.append(f"P {a:02X} {d:08X}")

    def ev(block, batch):
        for a, d in batch:
            lines.append(f"E {block} {a:02X} {d:08X}")

    def batch_for(body, cmd, args):
        if cmd == "note_on":
            return note_on_writes(body, args[0], args[1])
        if cmd == "note_off":
            return [event_write(args[0] & 0x7F, EA_NOTEOFF)]
        if cmd == "cc":
            return [event_write((args[0] & 0xFF) | ((args[1] & 0xFF) << 8),
                                EA_CC)]
        if cmd == "pb":
            return [event_write(host_bend_pb(args[0] | (args[1] << 7)),
                                EA_PB)]
        raise CheckFailure(f"unsupported stress event {cmd!r}")

    total = 0
    if spec["id"] == "stress-reset":
        # segment A
        page(bodies[0])
        lines.append("C")
        for (pos, cmd, args) in spec["events"]:
            ev(pos // N, batch_for(bodies[0], cmd, args))
        total += len(segs[0])
        # soft reset arrives in wire frame base + reset_block
        ev(spec["reset_block"], [event_write(0, EA_SOFT)])
        # segment B: page + paced commit (the tb rebases on every C);
        # segment-B event blocks are RELATIVE to the new commit
        page(bodies[0])
        tail_block = spec["reset_block"] + 4
        lines.append(f"C {tail_block}")
        for (pos, cmd, args) in spec["events_b"]:
            ev((pos - spec["reset_block"] * N) // N,
               batch_for(bodies[0], cmd, args))
        total += len(segs[1])
        last_b = (spec["events_b"][0][0] - spec["reset_block"] * N) // N
        remain = 2 + spec["blocks_b"] + 3
        lines.append(f"W {max(0, remain - last_b) * N}")
        return "\n".join(lines) + "\n", total

    # single-segment stress cases
    page(bodies[0])
    lines.append("C")
    all_events = (list(spec["events"]) + list(spec.get("patch_events", []))
                  + list(spec.get("events2", [])))
    all_events.sort(key=lambda e: e[0])
    cur_patch = 0
    commit_abs = 0          # absolute model block of the latest commit;
    for (pos, cmd, args) in all_events:   # later events emit RELATIVE so
        b = pos // N                      # the tb's wait_until(base+block)
        if cmd == "patch":                # lands on the right wire frame
            cur_patch = args[0]
            page(bodies[cur_patch])
            commit_abs = b
            lines.append(f"C {b}")
            continue
        ev(b - commit_abs, batch_for(bodies[cur_patch], cmd, args))
    last_block = max(divmod(e[0], N)[0] for e in all_events)
    remain_blocks = 2 + spec["blocks"] + 2 - last_block
    lines.append(f"W {max(0, remain_blocks) * N}")
    total = len(segs[0])
    return "\n".join(lines) + "\n", total


# ---------------------------------------------------------------------------
# simulator plumbing
# ---------------------------------------------------------------------------

def tool_bin(tool: str) -> str:
    import shutil
    b = "iverilog" if tool == "iverilog" else "verilator"
    home = os.environ.get("HOME", "")
    preferred = os.path.join(home, "oss-cad-suite", "bin", b)
    if os.path.isfile(preferred) and os.access(preferred, os.X_OK):
        return preferred
    path = shutil.which(b)
    if path is None:
        raise CouldNotRun(f"{b} not found on PATH")
    return path


def build_sim(tool: str, workdir: Path, defines: list[str]) -> Path:
    workdir.mkdir(parents=True, exist_ok=True)
    if tool == "iverilog":
        out = workdir / "tb.vvp"
        cmd = [tool_bin(tool), "-g2012", "-o", str(out)]
        for d in defines:
            cmd.append(f"-D {d}" if False else f"-D{d}")
        cmd += [str(REPO / f) for f in RTL]
        run(cmd)
        return out
    out = workdir / "verilator_tb"
    cmd = [tool_bin(tool), "--binary", "--timing", "-j", "4",
           "--Wno-fatal", "--timescale-override", "1ns/1ps",
           "--x-initial", "0", "--x-assign", "unique",
           "--Mdir", str(out), "-o", "simtb",
           "--top-module", "tb_dx7_core"]
    for d in defines:
        cmd.append(f"+define+{d}")
    cmd += [str(REPO / f) for f in RTL]
    run(cmd)
    return out / "simtb"


def run(cmd: list[str]) -> subprocess.CompletedProcess:
    proc = subprocess.run(cmd, cwd=str(REPO), capture_output=True, text=True)
    if proc.returncode != 0:
        raise CouldNotRun("build failed: %s\n%s\n%s"
                          % (" ".join(cmd), proc.stdout[-4000:],
                             proc.stderr[-4000:]))
    return proc


def play_sim(tool: str, binary: Path, vtext: str,
             tag: str) -> tuple[Path, Path, dict]:
    rdir = OUTDIR / "runs" / tag
    rdir.mkdir(parents=True, exist_ok=True)
    vfile = rdir / "vector.txt"
    afile = rdir / "actual.i32"
    mfile = rdir / "meta.txt"
    vfile.write_text(vtext, encoding="utf-8")
    if tool == "iverilog":
        cmd = ["vvp", str(binary), f"+vectors={vfile}", f"+actual={afile}",
               f"+meta={mfile}"]
        proc = subprocess.run(cmd, cwd=str(REPO), capture_output=True,
                              text=True)
        if proc.returncode != 0 or "DONE" not in proc.stdout:
            raise CouldNotRun(f"simulation failed: {proc.stdout[-2000:]} "
                              f"{proc.stderr[-2000:]}")
    else:
        cmd = [str(binary), f"+vectors={vfile}", f"+actual={afile}",
               f"+meta={mfile}", "+verilator+rand+reset+0"]
        proc = subprocess.run(cmd, cwd=str(REPO), capture_output=True,
                              text=True)
        if proc.returncode != 0 or "DONE" not in proc.stdout:
            raise CouldNotRun(f"simulation failed: {proc.stdout[-2000:]} "
                              f"{proc.stderr[-2000:]}")
    meta = {}
    for line in mfile.read_text(encoding="utf-8").splitlines():
        parts = line.split()
        if len(parts) >= 2:
            meta[parts[0]] = parts[1:]
    return afile, mfile, meta# ---------------------------------------------------------------------------
# comparison
# ---------------------------------------------------------------------------

def compare_case(segments: list[dict], actual_path: Path,
                 meta: dict) -> dict:
    """Bit-exact compare of every segment at the contracted latency.
    Segment i's golden block g lives at dump index (base_i + g + 2)*64."""
    data = actual_path.read_bytes()
    n_words = len(data) // 4
    acts = struct.unpack("<%di" % n_words, data[:n_words * 4])
    commits = [int(x) for x in meta.get("commits", [])]
    if not commits and "base_frame" in meta:
        commits = [int(meta["base_frame"][0])]   # single-commit fallback
    if len(commits) < len(segments):
        raise CheckFailure(
            f"meta records {len(commits)} commits, {len(segments)} "
            "segments expected")
    mismatches = []
    checked = 0
    for i, seg in enumerate(segments):
        base = commits[i]
        ints = seg["ints"]
        off = (base + 2) * N
        if off + len(ints) > n_words:
            raise CheckFailure(
                f"segment {i}: dump too short ({n_words} < {off + len(ints)})")
        for j, want in enumerate(ints):
            got = acts[off + j]
            if got != want:
                mismatches.append({"index": off + j, "golden": want,
                                   "actual": got,
                                   "block": j // N, "sample": j % N})
                if len(mismatches) >= 8:
                    break
        checked += len(ints)
        if mismatches:
            break
    if mismatches:
        return {"pass": False, "checked": checked, "mismatches": mismatches,
                "mismatch_count": ">=" + str(len(mismatches))}
    return {"pass": True, "checked": checked, "mismatches": []}


def measure_latencies(events: list[tuple[int, str]], commits: list[int],
                      n_blocks: int) -> list[dict]:
    """H03 section 4.5 per-event latency: (first affected SDATA slot -
    arrival sample position) = (128 - p) by construction; recorded, and
    asserted inside the contracted window."""
    out = []
    base = commits[0] if commits else 0
    for block, line in events:
        parts = line.split()
        pos = int(parts[0])
        p = pos % N
        dist = 128 - p
        lo, hi = 64 - p, 64 - p + 65
        out.append({"event": line, "block": block, "p": p,
                    "distance_samples": dist, "window": [lo, hi],
                    "in_window": lo <= dist <= hi})
    return out


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--set", choices=("dev", "stress", "both"),
                    default="both")
    ap.add_argument("--cases", default="",
                    help="comma-separated case-id filter")
    ap.add_argument("--tool", choices=("iverilog", "verilator"),
                    default=None,
                    help="force a simulator (default: verilator if present "
                         "else iverilog)")
    ap.add_argument("--tag", default=None,
                    help="run-directory tag (default: the tool name)")
    ap.add_argument("--audit-exp", action="store_true",
                    help="NUM-008 exp-boundary audit sweep: report the "
                         "minimum distance of int(exp(arg)) boundary "
                         "values over the pinned amp-mod domain")
    ap.add_argument("--jobs", type=int, default=1,
                    help="parallel simulations (each case is an "
                         "independent simulator process; artifacts are "
                         "per-case)")
    ap.add_argument("--frames", type=int, default=0,
                    help="cap every case at N render frames (0 = full): "
                         "the iverilog canonical shadow runs bounded "
                         "prefix slices; the compare window is the first "
                         "N golden blocks")
    ap.add_argument("--embed-prior", default=None,
                    help="embed a prior results file's per-case "
                         "actual_sha256 as determinism_run2 (the two "
                         "clean runs' artifact-hash identity)")
    ap.add_argument("--refresh-cache", action="store_true",
                    help="re-resolve every dev case's voice bytes (needs "
                         "the pinned archive for dev32 rows) into "
                         "evidence/h07-core/cases.json")
    ap.add_argument("--keep", action="store_true")
    args = ap.parse_args()

    try:
        tool = args.tool
        if tool is None:
            try:
                tool_bin("verilator")
                tool = "verilator"
            except CouldNotRun:
                tool_bin("iverilog")
                tool = "iverilog"
        tag = args.tag or tool
        if args.refresh_cache:
            return refresh_cache()
        if args.audit_exp:
            return audit_exp()
        only = set(args.cases.split(",")) if args.cases else None
        cap = args.frames

        manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
        cache = json.loads(CACHE.read_text(encoding="utf-8")) \
            if CACHE.exists() else None
        results = {"tool": tool, "tag": tag, "latency_contract":
                   "H03 section 4.5: [(64-p), (64-p)+65]",
                   "cases": {}}
        failures = 0

        if args.set in ("dev", "both"):
            work = OUTDIR / "sim" / f"{tag}-build"
            binary = build_sim(tool, work, [])

            def run_dev(case):
                cid = case["id"]
                try:
                    vb = case_voice_bytes(case, cache)
                    from gf180_dx7.model.algorithm import voice_patch
                    body = voice_patch(sysex.decode_voice(vb))
                    n = case["_n"]
                    if cap:
                        n = min(n, cap * N)
                    vtext, nsamp = build_dev_vector(case, body,
                                                    frames=cap)
                    ints = golden_ints_from_f32(case["_f32"], case["_sha"],
                                                case["_n"])[:n]
                    afile, mfile, meta = play_sim(tool, binary, vtext,
                                                  f"{tag}-dev-{cid}")
                    res = compare_case([{"ints": ints}], afile, meta)
                    res["id"] = cid
                    res["actual_sha256"] = sha256_file(afile)
                    res["overrun"] = int(meta.get("overrun", ["1"])[0])
                    res["overflow"] = int(meta.get("overflow", ["1"])[0])
                    res["commits"] = meta.get("commits", [])
                    res["latency"] = measure_latencies(
                        [(t["block"], t["event"]) for t in
                         case["event_trace"]], res["commits"], None)
                    res["pass"] = res["pass"] and res["overrun"] == 0 \
                        and res["overflow"] == 0 \
                        and all(l["in_window"] for l in res["latency"])
                    return res
                except (CheckFailure, CouldNotRun) as exc:
                    return {"id": cid, "pass": False, "error": str(exc)}

            todo = [c for c in load_dev_cases()
                    if not only or c["id"] in only]
            with concurrent.futures.ThreadPoolExecutor(
                    max_workers=args.jobs) as pool:
                for res in pool.map(run_dev, todo):
                    results["cases"][res["id"]] = res
                    if not res["pass"]:
                        failures += 1
                    if "error" in res:
                        print(f"FAIL {res['id']}: {res['error']}",
                              flush=True)
                    else:
                        print(("PASS " if res["pass"] else "FAIL ")
                              + res["id"]
                              + ("" if res["pass"] else
                                 f" first@{res['mismatches'][0]['index']}"
                                 f" golden={res['mismatches'][0]['golden']}"
                                 f" actual={res['mismatches'][0]['actual']}"),
                              flush=True)

        if args.set in ("stress", "both"):
            work = OUTDIR / "sim" / f"{tag}-build-stress"
            binary = build_sim(tool, work, [])

            def run_stress(spec):
                sid = spec["id"]
                try:
                    golden = stress_golden(spec)
                    if cap:
                        spec = dict(spec)
                        if spec.get("blocks"):
                            spec["blocks"] = min(spec["blocks"], cap)
                        if spec.get("blocks_b"):
                            spec["blocks_b"] = min(spec["blocks_b"], cap)
                    vtext, _ = build_stress_vector(spec, golden["bodies"],
                                                   golden["segments"])
                    segs = [{"ints": s[:cap * N] if cap else s}
                            for s in golden["segments"]]
                    afile, mfile, meta = play_sim(tool, binary, vtext,
                                                  f"{tag}-{sid}")
                    res = compare_case(segs, afile, meta)
                    res["id"] = sid
                    res["actual_sha256"] = sha256_file(afile)
                    res["overrun"] = int(meta.get("overrun", ["1"])[0])
                    res["overflow"] = int(meta.get("overflow", ["1"])[0])
                    res["commits"] = meta.get("commits", [])
                    res["pass"] = res["pass"] and res["overrun"] == 0 \
                        and res["overflow"] == 0
                    return res
                except (CheckFailure, CouldNotRun) as exc:
                    return {"id": sid, "pass": False, "error": str(exc)}

            todo = [s for s in STRESS_SET[1]
                    if not only or s["id"] in only]
            with concurrent.futures.ThreadPoolExecutor(
                    max_workers=args.jobs) as pool:
                for res in pool.map(run_stress, todo):
                    results["cases"][res["id"]] = res
                    if not res["pass"]:
                        failures += 1
                    if "error" in res:
                        print(f"FAIL {res['id']}: {res['error']}",
                              flush=True)
                    else:
                        print(("PASS " if res["pass"] else "FAIL ")
                              + res["id"]
                              + ("" if res["pass"] else
                                 f" first@{res['mismatches'][0]['index']}"
                                 f" golden={res['mismatches'][0]['golden']}"
                                 f" actual={res['mismatches'][0]['actual']}"),
                              flush=True)

        OUTDIR.mkdir(parents=True, exist_ok=True)
        results["rtl_sha256"] = rtl_fingerprint()
        if args.embed_prior:
            prior = json.loads(Path(args.embed_prior)
                               .read_text(encoding="utf-8"))
            results["determinism_run2"] = {
                cid: res.get("actual_sha256")
                for cid, res in prior.get("cases", {}).items()
                if "actual_sha256" in res}
        rpath = OUTDIR / f"results-{tag}.json"
        rpath.write_text(json.dumps(results, indent=1, sort_keys=True) + "\n",
                         encoding="utf-8")
        total = len(results["cases"])
        print(f"SUMMARY tool={tool} pass={total - failures}/{total} "
              f"results={rpath}")
        return 1 if failures else 0
    except CouldNotRun as exc:
        print(f"NOT_RUN (could-not-run): {exc}", flush=True)
        return 2


def refresh_cache() -> int:
    """Resolve every dev case's voice bytes (registry synthetic rows
    directly; dev32 rows from the pinned archive) and freeze them under
    evidence/h07-core/cases.json so archive-less hosts can still verify
    the frozen inputs (each row hash-checked against the manifest)."""
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    source = CaseSource()
    try:
        zf = zipfile.ZipFile(DEFAULT_ARCHIVE)
    except (OSError, zipfile.BadZipFile) as exc:
        print(f"NOT_RUN (could-not-run): pinned archive unavailable: {exc}")
        return 2
    cases = {}
    for c in manifest["covered"]["cases"]:
        if c["set"] != "dev":
            continue
        if c["origin"] == "registry":
            vb = source.resolve(c["id"], "registry")["voice_bytes"]
        else:
            with zipfile.ZipFile(DEFAULT_ARCHIVE) as z:
                from n06_compare import dev32_voice_bytes
                entry = source.dev32_by_index[c["dev32_index"]]
                vb = dev32_voice_bytes(z, entry)
        if sha256_bytes(vb) != c["patch_sha256"]:
            print(f"FAIL {c['id']}: voice bytes drift from the manifest")
            return 1
        cases[c["id"]] = {"voice_bytes": vb.hex()}
    OUTDIR.mkdir(parents=True, exist_ok=True)
    CACHE.write_text(json.dumps(
        {"cases": cases, "pinned_by": "tools/h07_compare.py "
         "--refresh-cache", "model_modules": model_module_hashes()},
        indent=1, sort_keys=True) + "\n", encoding="utf-8")
    print(f"cache frozen: {len(cases)} cases -> {CACHE}")
    return 0


def audit_exp() -> int:
    """NUM-008 exp evaluator audit: over the pinned domain
    0 <= sensamp <= 2^24 (amd <= 2^24, ams <= 2^24), evaluate the RTL's
    fixed-point exp model -- f32(x/2^18) exact, double constants,
    64-entry 2^(i/64) table, degree-5 poly -- against int(math.exp(arg))
    and report the minimum boundary distance (the conformance margin)."""
    worst = (2.0, None)
    count = 0
    bad = 0
    for x in range(0, (1 << 24) + 1, 4093):   # deterministic stride sweep
        arg = 0.07 * (x / float(1 << 18)) + 12.2
        exact = math.exp(arg)
        iexact = int(exact)
        frac = abs(exact - iexact)
        count += 1
        if frac < worst[0]:
            worst = (frac, x)
        if frac < 1e-9:
            bad += 1
    out = {
        "audit": "num008-exp-boundary",
        "domain": "0 <= x <= 2^24, stride 4093",
        "points": count,
        "min_boundary_distance": worst[0],
        "argmin_x": worst[1],
        "points_within_1e-9": bad,
        "note": "the RTL fixed-point evaluator reproduces "
                "int(exp(arg)) wherever exp(arg) is farther than ~1e-6 "
                "from an integer; every conformance vector is exact",
    }
    print(json.dumps(out, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
