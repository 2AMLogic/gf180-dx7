"""cocotb driver for `klt functional-verification` (issue #150).

Thin wrapper, not a new bench: one cocotb test per H07 case (30 dev cases +
the stress set).  Each test plays the case through the existing
rtl/tb_dx7_core.v vector playback and compares bit-exactly with the
comparator in tools/h07_compare.py (run_dev_case / run_stress_case).  Nothing
is re-derived here: vectors, golden integers and the compare are the H07
harness's own.

The cocotb-attached simulation is only the harness anchor (hdl_toplevel
dx7_core, the DR-0012 pin).  The verdict comes from the vvp playback of
tb_dx7_core, which instantiates that same core from the same pinned files;
tb_dx7_core takes its stimulus through plusargs the cocotb Runner does not
pass, so the playback is a vvp subprocess of the same Icarus.

Graded set and frame prefix come from tools/functional_verification.py
(split_dev_cases / dev_frames): dev cases whose events end by block 75,
compared over last-event-block + 16 golden blocks, plus the stress set at
its own preregistered length.  The remaining dev cases are NOT_RUN here.

Environment (read by the tests):
  FV_DEFINES  comma-separated -D defines for the tb build, e.g.
              H07_MUTATE_GLITCH (negative control only)
  FV_JOBS     parallel playbacks, capped at 2 (shared host)
  FV_WORKDIR  scratch dir for the per-case artifacts (default: next to this
              file's build dir under build/fv-dx7-core)

Claim covered: (1) RTL == frozen integer model on the pinned trio under
Icarus, over the declared frame prefix.  Not model-vs-reference agreement,
synthesis, STA, or sound.
"""

from __future__ import annotations

import concurrent.futures
import os
import re
import sys
from pathlib import Path

import cocotb
from cocotb.triggers import Timer

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))

import functional_verification as FVT  # noqa: E402
import h07_compare as H  # noqa: E402

DEFINES = [d for d in os.environ.get("FV_DEFINES", "").split(",") if d]
JOBS = max(1, min(2, int(os.environ.get("FV_JOBS", "2"))))
WORK = Path(os.environ.get("FV_WORKDIR", str(REPO / "build" / "fv-dx7-core")))
TAG = "fv"
TOOL = "iverilog"


def test_name(case_id: str) -> str:
    return "case_" + re.sub(r"[^0-9A-Za-z]", "_", case_id)


def _load_specs() -> list[tuple[str, str, dict]]:
    graded, _ = FVT.split_dev_cases(H.load_dev_cases())
    specs = [("dev", c["id"], c) for c in graded]
    specs += [("stress", s["id"], s) for s in H.STRESS_SET[1]]
    return specs


SPECS = _load_specs()
_BINARY = None
_RESULTS: dict[str, dict] = {}


def _binary() -> Path:
    global _BINARY
    if _BINARY is None:
        _BINARY = H.build_sim(TOOL, WORK / "build", DEFINES)
    return _BINARY


def _play(kind: str, spec: dict) -> dict:
    H.OUTDIR = WORK  # per-case artifacts stay out of evidence/h07-core
    if kind == "dev":
        res = H.run_dev_case(spec, TOOL, _binary(), TAG,
                             FVT.dev_frames(spec), _cache())
        run = f"{TAG}-dev-{spec['id']}"
    else:
        res = H.run_stress_case(spec, TOOL, _binary(), TAG, 0)
        run = f"{TAG}-{spec['id']}"
    res["audible"] = _audible(res, WORK / "runs" / run / "actual.i32")
    return res


def _audible(res: dict, afile: Path):
    """Coverage, reported apart from agreement: non-zero samples in the
    dump from the first commit's compare window on (None if not run)."""
    import struct
    if "error" in res or not res.get("commits"):
        return None
    data = afile.read_bytes()
    acts = struct.unpack("<%di" % (len(data) // 4), data)
    start = (int(res["commits"][0]) + 2) * H.N
    return sum(1 for x in acts[start:] if x)


_CACHE = None


def _cache():
    global _CACHE
    if _CACHE is None:
        import json
        _CACHE = json.loads(H.CACHE.read_text(encoding="utf-8")) \
            if H.CACHE.exists() else None
    return _CACHE


def _prefetch() -> None:
    """Run the selected cases JOBS-wide once; tests then read the results."""
    flt = os.environ.get("COCOTB_TEST_FILTER")
    todo = [(k, cid, sp) for k, cid, sp in SPECS
            if not flt or re.search(flt, test_name(cid))]
    _binary()
    with concurrent.futures.ThreadPoolExecutor(max_workers=JOBS) as pool:
        futs = {cid: pool.submit(_play, k, sp) for k, cid, sp in todo}
        for cid, fut in futs.items():
            _RESULTS[cid] = fut.result()


def _sidecar(case_id: str, res: dict) -> None:
    """Per-case facts the klt envelope cannot carry; merged into the
    committed report's provenance by tools/functional_verification.py."""
    import json
    d = WORK / "cases"
    d.mkdir(parents=True, exist_ok=True)
    (d / (test_name(case_id) + ".json")).write_text(json.dumps({
        "case": case_id, "checked_samples": res["checked"],
        "audible_samples": res["audible"],
        "actual_sha256": res["actual_sha256"]}, sort_keys=True))


def _make(kind: str, case_id: str, spec: dict):
    async def body(dut):
        # anchor: the pinned core elaborates and the sim clock advances
        await Timer(1, unit="ns")
        if not _RESULTS:
            _prefetch()
        res = _RESULTS.get(case_id) or _play(kind, spec)
        if "error" in res:
            raise AssertionError(f"{case_id}: could not run: {res['error']}")
        assert res["pass"], (
            f"{case_id}: RTL != frozen model; first mismatches "
            f"{res.get('mismatches', [])[:3]} overrun={res['overrun']} "
            f"overflow={res['overflow']}")
        _sidecar(case_id, res)
        dut._log.info("%s bit-exact: %d samples, %d audible, "
                      "actual_sha256=%s defines=%s", case_id, res["checked"],
                      res["audible"], res["actual_sha256"], DEFINES)

    body.__name__ = test_name(case_id)
    body.__qualname__ = body.__name__
    return cocotb.test()(body)


for _kind, _cid, _spec in SPECS:
    globals()[test_name(_cid)] = _make(_kind, _cid, _spec)
