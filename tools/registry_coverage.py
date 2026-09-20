#!/usr/bin/env python3
"""R06 compatibility-registry coverage validator (issue #13).

Validates spec/compatibility-registry-v1.json against the P01 canonical
supported-field list (reflection over the Voice/Operator dataclasses in
src/gf180_dx7/sysex.py):

  - every reflected parameter field has >= 1 directed or boundary case;
  - all 32 algorithms have >= 1 case; algorithms 4 and 6 carry
    feedback-specific cases at feedback 0, 3 (mid) and 7;
  - every case-group reference resolves, event lists parse in the oracle
    events.txt format, short-subset hash-comparison partners are present,
    the holdout block is well-formed (sealed until N08 freezes thresholds);
  - coverage holes are printed as machine-readable JSON (never prose).

Exit codes: 0 = no holes, 1 = validation failed or holes exist,
2 = environment problem (registry unreadable).

--selftest-negative proves the check has resolution: three in-memory
mutations (drop one algorithm's cases, drop one field's cases, empty the
short subset) must each make validation fail.

Stdlib-only.
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO_ROOT / "src"))

from gf180_dx7 import sysex  # noqa: E402

DEFAULT_REGISTRY = REPO_ROOT / "spec" / "compatibility-registry-v1.json"
SCHEMA = "gf180-dx7.compatibility-registry/1"

# Voice dataclass members that are codec bookkeeping, not synthesis
# parameters. They are owned by the P01 test suite (tests/test_sysex.py),
# not by render-directed cases. `name`/`name_bytes` ARE covered (the
# name-identity cases prove name changes never affect audio).
BOOKKEEPING_FIELDS = {"operators", "channel_byte", "anomalies", "raw_bytes"}

ALGORITHM_COUNT = 32
FEEDBACK_CASE_ALGORITHMS = (4, 6)
FEEDBACK_CASE_VALUES = (0, 3, 7)

ORACLE_EVENT_CMDS = {"note_on": 2, "note_off": 1, "cc": 2, "pb": 2}
ORACLE_CC_CONTROLLERS = (1, 2, 4, 64, 120, 123)

MANUAL_PROPERTY_PREFIX = "manual:"
REF_PROPERTIES = ("distinct_hash_vs:", "identical_hash_vs:")
CASE_REF_PROPERTIES = ("distinct_hash_vs:", "identical_hash_vs:",
                       "late_peak_gt_case:", "late_peak_lt_case:")


class RegistryError(Exception):
    """Exit 2: the registry cannot be read at all."""


class ValidationFailure(Exception):
    """Exit 1: the registry is invalid; failures name the holes."""


def reflected_fields() -> list[tuple[str, str]]:
    """The P01 canonical supported-field list, by reflection.

    Returns (field, kind) pairs: every Operator dataclass field with kind
    'op', and every Voice dataclass field except bookkeeping, with kind
    'common'.
    """
    pairs = [(f.name, "op") for f in dataclasses.fields(sysex.Operator)]
    pairs += [(f.name, "common") for f in dataclasses.fields(sysex.Voice)
              if f.name not in BOOKKEEPING_FIELDS]
    return pairs


def load_registry(path: Path) -> dict:
    if not path.is_file():
        raise RegistryError(f"registry not found: {path}")
    try:
        with open(path, "r", encoding="utf-8") as fh:
            data = json.load(fh)
    except json.JSONDecodeError as exc:
        raise RegistryError(f"registry {path} is not valid JSON: {exc}")
    return data


def validate_event_line(case_id: str, line: str) -> None:
    """One events.txt line in the pinned oracle's format."""
    parts = line.split()
    if len(parts) < 2:
        raise ValidationFailure(f"case {case_id}: malformed event {line!r}")
    cmd = parts[1]
    try:
        pos = int(parts[0])
        args = [int(v) for v in parts[2:]]
    except ValueError as exc:
        raise ValidationFailure(
            f"case {case_id}: non-integer in event {line!r}") from exc
    if pos < 0:
        raise ValidationFailure(f"case {case_id}: negative event position {line!r}")
    if cmd not in ORACLE_EVENT_CMDS:
        raise ValidationFailure(
            f"case {case_id}: unknown event cmd {cmd!r} (oracle supports "
            f"{sorted(ORACLE_EVENT_CMDS)})")
    if len(args) != ORACLE_EVENT_CMDS[cmd]:
        raise ValidationFailure(
            f"case {case_id}: {cmd} takes {ORACLE_EVENT_CMDS[cmd]} arg(s) "
            f"in {line!r}")
    if any(not 0 <= a <= 127 for a in args):
        raise ValidationFailure(f"case {case_id}: event arg out of 0-127 in {line!r}")
    if cmd == "note_on" and not 1 <= args[1] <= 127:
        raise ValidationFailure(
            f"case {case_id}: note_on velocity must be 1-127 in {line!r}")
    if cmd == "cc" and args[0] not in ORACLE_CC_CONTROLLERS:
        raise ValidationFailure(
            f"case {case_id}: cc controller {args[0]} not in the pinned oracle "
            f"subset {list(ORACLE_CC_CONTROLLERS)} in {line!r}")


def input_fingerprint(case_row: dict) -> str:
    """Stable fingerprint over a case's covered inputs (patch + events +
    render config + expectation). A challenge run that finds a changed
    fingerprint marks previously recorded evidence rows STALE."""
    payload = json.dumps({
        "patch": case_row.get("patch"),
        "events": case_row.get("events"),
        "render_seconds": case_row.get("render_seconds"),
        "expect": case_row.get("expect"),
    }, sort_keys=True, separators=(",", ":"))
    return hashlib.sha256(payload.encode("utf-8")).hexdigest()


def validate_registry(reg: dict) -> dict:
    """Validate the registry; return the coverage report dict.

    Raises ValidationFailure on structural errors or coverage holes.
    """
    if reg.get("schema") != SCHEMA:
        raise ValidationFailure(
            f"registry schema must be {SCHEMA!r}, found {reg.get('schema')!r}")
    if not isinstance(reg.get("synthetic_base"), dict):
        raise ValidationFailure("registry has no synthetic_base block")
    if not isinstance(reg.get("holdout"), dict):
        raise ValidationFailure("registry has no holdout block")

    cases = reg.get("cases")
    if not isinstance(cases, list) or not cases:
        raise ValidationFailure("registry has no cases")
    by_id: dict[str, dict] = {}
    for c in cases:
        if not isinstance(c, dict):
            raise ValidationFailure("case entry is not an object")
        cid = c.get("id")
        if not isinstance(cid, str) or not cid:
            raise ValidationFailure("case without an id")
        if cid in by_id:
            raise ValidationFailure(f"duplicate case id {cid!r}")
        by_id[cid] = c

    valid_field_names = {name for name, _ in reflected_fields()}
    valid_override_fields = valid_field_names  # name is a Voice field

    blocked_ids = set()
    corpus_ids = set()
    for cid, c in by_id.items():
        patch = c.get("patch")
        if not isinstance(patch, dict):
            raise ValidationFailure(f"case {cid}: patch missing")
        selector = patch.get("selector")
        if selector == "synthetic":
            overrides = patch.get("overrides", {})
            if not isinstance(overrides, dict):
                raise ValidationFailure(f"case {cid}: overrides must be an object")
            for key, val in overrides.items():
                field = key.partition(".")[2] or key
                if field not in valid_override_fields:
                    raise ValidationFailure(
                        f"case {cid}: override field {key!r} is not a P01 "
                        f"supported field")
                if not isinstance(val, int) or isinstance(val, bool):
                    raise ValidationFailure(
                        f"case {cid}: override {key!r} must be an integer")
                if not 0 <= val <= 127:
                    raise ValidationFailure(
                        f"case {cid}: override {key!r}={val} outside 0-127")
            nm = patch.get("name")
            if nm is not None and (not isinstance(nm, str) or not nm):
                raise ValidationFailure(f"case {cid}: patch name must be a "
                                        "non-empty string")
        elif selector == "corpus":
            digest = patch.get("canonical_hash", "")
            if len(digest) != 64 or any(ch not in "0123456789abcdef" for ch in digest):
                raise ValidationFailure(
                    f"case {cid}: corpus case needs a 64-hex canonical_hash")
            corpus_ids.add(cid)
        else:
            raise ValidationFailure(f"case {cid}: unknown selector {selector!r}")

        blocked = bool(c.get("blocked"))
        events = c.get("events")
        if blocked:
            if not c.get("blocked_reason"):
                raise ValidationFailure(
                    f"case {cid}: blocked cases must carry blocked_reason")
            if events:
                raise ValidationFailure(
                    f"case {cid}: blocked cases carry no events")
            blocked_ids.add(cid)
        else:
            if not isinstance(events, list) or not events:
                raise ValidationFailure(f"case {cid}: events missing or empty")
            last_pos = -1
            for line in events:
                if not isinstance(line, str):
                    raise ValidationFailure(f"case {cid}: event is not a string")
                validate_event_line(cid, line)
                pos = int(line.split()[0])
                if pos < last_pos:
                    raise ValidationFailure(
                        f"case {cid}: event positions must be non-decreasing")
                last_pos = pos
            seconds = c.get("render_seconds")
            if not isinstance(seconds, (int, float)) or not 0 < seconds <= 30:
                raise ValidationFailure(f"case {cid}: bad render_seconds {seconds!r}")
            expect = c.get("expect")
            if not isinstance(expect, dict) or not expect.get("property"):
                raise ValidationFailure(f"case {cid}: expect.property missing")

    # --- short subset -------------------------------------------------------
    short = reg.get("short_subset")
    if not isinstance(short, list) or not short:
        raise ValidationFailure("short_subset missing or empty")
    for cid in short:
        if cid not in by_id:
            raise ValidationFailure(f"short_subset references unknown case {cid!r}")
        if cid in blocked_ids or cid in corpus_ids:
            raise ValidationFailure(
                f"short_subset case {cid!r} is blocked/corpus and cannot execute "
                "in the smoke")
    short_set = set(short)
    for cid in short:
        for pred in by_id[cid].get("expect", {}).get("also", []):
            if isinstance(pred, str) and pred.startswith(CASE_REF_PROPERTIES):
                ref = pred.split(":", 1)[1]
                if ref not in short_set:
                    raise ValidationFailure(
                        f"short_subset case {cid!r} compares against {ref!r} "
                        "which is not itself in the short subset")

    # --- field coverage -----------------------------------------------------
    fields = reg.get("fields")
    if not isinstance(fields, list):
        raise ValidationFailure("registry has no fields list")
    fields_by_name = {}
    for row in fields:
        if not isinstance(row, dict) or not row.get("field"):
            raise ValidationFailure("field entry is malformed")
        fields_by_name[row["field"]] = row

    holes: list[dict] = []
    unknown = sorted(set(fields_by_name) - valid_field_names)
    if unknown:
        raise ValidationFailure(
            f"registry fields not in the P01 supported list: {unknown}")
    for name, kind in reflected_fields():
        row = fields_by_name.get(name)
        if row is None:
            holes.append({"kind": "field", "field": name,
                          "reason": f"no registry row for supported {kind} field"})
            continue
        if row.get("kind") != kind:
            raise ValidationFailure(
                f"field {name!r}: kind {row.get('kind')!r} != reflected {kind!r}")
        case_ids = list(row.get("directed_cases", [])) + \
            list(row.get("boundary_cases", []))
        usable = [cid for cid in case_ids
                  if cid in by_id and cid not in blocked_ids]
        for cid in case_ids:
            if cid not in by_id:
                raise ValidationFailure(
                    f"field {name!r}: unknown case {cid!r}")
        if not usable:
            holes.append({"kind": "field", "field": name,
                          "reason": "no executable case covers this field"})

    # --- algorithm coverage -------------------------------------------------
    algorithms = reg.get("algorithms")
    if not isinstance(algorithms, list):
        raise ValidationFailure("registry has no algorithms list")
    alg_rows = {}
    for row in algorithms:
        if not isinstance(row, dict) or "algorithm" not in row:
            raise ValidationFailure("algorithm entry is malformed")
        alg_rows[row["algorithm"]] = row
    seen = sorted(set(alg_rows))
    expected = list(range(ALGORITHM_COUNT))
    if seen != expected:
        raise ValidationFailure(
            f"algorithms must enumerate exactly 0..{ALGORITHM_COUNT - 1}, "
            f"found {seen}")
    for alg, row in alg_rows.items():
        case_ids = list(row.get("cases", []))
        usable = [cid for cid in case_ids
                  if cid in by_id and cid not in blocked_ids]
        for cid in case_ids:
            if cid not in by_id:
                raise ValidationFailure(
                    f"algorithm {alg}: unknown case {cid!r}")
        if not usable:
            holes.append({"kind": "algorithm", "algorithm": alg,
                          "reason": "no executable case covers this algorithm"})
    for alg in FEEDBACK_CASE_ALGORITHMS:
        values = set()
        for cid in alg_rows[alg].get("cases", []):
            c = by_id.get(cid, {})
            fb = c.get("patch", {}).get("overrides", {}).get("feedback")
            if fb is not None:
                values.add(fb)
        missing = [v for v in FEEDBACK_CASE_VALUES if v not in values]
        if missing:
            holes.append({"kind": "algorithm_feedback", "algorithm": alg,
                          "reason": f"missing feedback-specific cases for "
                                    f"values {missing}"})

    # --- interaction / boundary / control groups ----------------------------
    for group_key, label_key, hole_kind in (("event_interactions", "interaction",
                                             "interaction"),
                                            ("boundary", "boundary", "boundary")):
        rows = reg.get(group_key)
        if not isinstance(rows, list) or not rows:
            raise ValidationFailure(f"registry has no {group_key} list")
        for row in rows:
            if not isinstance(row, dict) or not row.get(label_key):
                raise ValidationFailure(f"{group_key} entry is malformed")
            label = row[label_key]
            case_ids = list(row.get("cases", []))
            for cid in case_ids:
                if cid not in by_id:
                    raise ValidationFailure(
                        f"{group_key} {label!r}: unknown case {cid!r}")
            if not case_ids:
                holes.append({"kind": hole_kind, "name": label,
                              "reason": "no case registered"})

    controls = reg.get("performance_controls")
    if not isinstance(controls, list) or not controls:
        raise ValidationFailure("registry has no performance_controls list")
    for row in controls:
        if not isinstance(row, dict) or not row.get("control"):
            raise ValidationFailure("performance_controls entry is malformed")
        status = row.get("status")
        if status not in ("covered", "covered-observed-inert", "blocked"):
            raise ValidationFailure(
                f"performance control {row['control']!r}: bad status {status!r}")
        if status == "blocked" and not row.get("cases"):
            raise ValidationFailure(
                f"performance control {row['control']!r}: blocked rows must still "
                "name their directed case")

    # --- holdout seal -------------------------------------------------------
    holdout = reg["holdout"]
    if not isinstance(holdout.get("sealed"), bool):
        raise ValidationFailure("holdout.sealed must be a boolean")
    challenge = holdout.get("challenge_subset")
    if not isinstance(challenge, list):
        raise ValidationFailure("holdout.challenge_subset must be a list")
    if holdout["sealed"] is False and challenge:
        raise ValidationFailure(
            "holdout is not sealed but challenge_subset is non-empty")
    for cid in challenge:
        if cid not in by_id:
            raise ValidationFailure(
                f"holdout.challenge_subset references unknown case {cid!r}")

    fingerprints = {cid: input_fingerprint(c) for cid, c in by_id.items()}
    report = {
        "schema": SCHEMA,
        "registry": str(Path(reg.get("_path", DEFAULT_REGISTRY)).name),
        "field_count": len(reflected_fields()),
        "fields_covered": len(reflected_fields()) - 
            sum(1 for h in holes if h["kind"] == "field"),
        "algorithms_total": ALGORITHM_COUNT,
        "algorithms_covered": ALGORITHM_COUNT -
            sum(1 for h in holes if h["kind"] == "algorithm"),
        "case_count": len(by_id),
        "executable_cases": len(by_id) - len(blocked_ids),
        "blocked_cases": sorted(blocked_ids),
        "corpus_cases": sorted(corpus_ids),
        "short_subset": len(short),
        "holdout_sealed": bool(holdout["sealed"]),
        "holes": holes,
        "case_fingerprints": fingerprints,
    }
    return report


def selftest_negative(reg_path: Path) -> int:
    """Three in-memory mutations must each break validation (resolution
    proof for the coverage check)."""
    reg = load_registry(reg_path)
    import copy

    def fails(mutated: dict) -> str | None:
        """Validation-failure text, or a hole summary when the mutated
        registry validates but leaves coverage holes."""
        try:
            holes = validate_registry(mutated)["holes"]
        except ValidationFailure as exc:
            return str(exc)
        return json.dumps(holes) if holes else None

    results = []

    # 1. drop one algorithm's cases
    m = copy.deepcopy(reg)
    alg4 = next(r for r in m["algorithms"] if r["algorithm"] == 4)
    alg4["cases"] = []
    err = fails(m)
    ok1 = err is not None
    results.append(("drop algorithm 4 cases", ok1, err))

    # 2. drop every case reference of one field
    m = copy.deepcopy(reg)
    field_row = next(r for r in m["fields"] if r["field"] == "osc_detune")
    field_row["directed_cases"] = []
    field_row["boundary_cases"] = []
    try:
        report = validate_registry(m)
        holes = report["holes"]
        ok2 = any(h.get("field") == "osc_detune" for h in holes)
        err = f"{len(holes)} hole(s): " + json.dumps([h for h in holes
                                                      if h.get("field")])[:120]
    except ValidationFailure as exc:
        ok2, err = False, str(exc)
    results.append(("drop osc_detune case refs", ok2, err))

    # 3. empty the short subset
    m = copy.deepcopy(reg)
    m["short_subset"] = []
    err = fails(m)
    ok3 = err is not None and "short_subset" in err
    results.append(("empty short subset", ok3, err))

    print(json.dumps({
        "selftest_negative": "PASS" if all(ok for ok, _ in
                                           ((r[1], r[2]) for r in results))
        else "FAIL",
        "mutations": [{"mutation": name, "detected": ok, "detail": detail}
                      for name, ok, detail in results],
    }, indent=2))
    return 0 if all(ok for ok, _ in ((r[1], r[2]) for r in results)) else 1


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description="Validate R06 compatibility-registry coverage (issue #13)")
    parser.add_argument("--registry", type=Path, default=DEFAULT_REGISTRY)
    parser.add_argument("--check", action="store_true",
                        help="exit 1 when any coverage hole exists")
    parser.add_argument("--selftest-negative", action="store_true",
                        help="prove the check has resolution (mutations fail)")
    args = parser.parse_args(argv)

    try:
        reg = load_registry(args.registry)
    except RegistryError as exc:
        print(f"registry_coverage: NOT_RUN — {exc}", file=sys.stderr)
        return 2

    if args.selftest_negative:
        return selftest_negative(args.registry)

    try:
        report = validate_registry(reg)
    except ValidationFailure as exc:
        print(f"registry_coverage: FAIL — {exc}", file=sys.stderr)
        print(json.dumps({"holes": [{"kind": "validation", "reason": str(exc)}]},
                         indent=2))
        return 1

    print(json.dumps(report, indent=2))
    if args.check and report["holes"]:
        print(f"registry_coverage: FAIL — {len(report['holes'])} coverage "
              "hole(s)", file=sys.stderr)
        return 1
    print(f"registry_coverage: PASS — {report['field_count']} fields, "
          f"{report['algorithms_covered']}/{report['algorithms_total']} "
          f"algorithms, {report['case_count']} cases, holes: "
          f"{len(report['holes'])}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
