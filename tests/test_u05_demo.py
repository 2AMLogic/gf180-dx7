#!/usr/bin/env python3
"""U05 demo-tools FAST tests (issue #38; token u05_demo in
tools/test_fast.sh).

Runs in-process on the COMPACT profile (seconds, no renders, no board,
no zip): the committed musical fixture (superset of U01, pinned counts,
coverage wants that refuse an under-delivering fixture), the clean bench
with the three-way cross-check, the full corruption portfolio under
--expect-fail with the NAMED gates (a control that fails the wrong gate
fails the suite), the stress battery including the required
throttled-link underrun negative with its resolution proof, and the
version-manifest placeholder policy (declared dry now accepted; record
manifests refuse placeholders by name; dry manifests may not carry
hardware passes or concrete hardware values).

Claim under test: host-side link-contract conformance on the mock core -
no audio, no RTL, no fidelity, no audibility. The WAVE gate
(test_u05_full_demo.py, make test only) adds the full profile and the
environment-gated dev32 variant.
"""

import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
for p in (str(REPO / "src"), str(REPO / "tools"),
          str(REPO / "tools" / "demo")):
    if p not in sys.path:
        sys.path.insert(0, p)

import check_manifest  # noqa: E402
import demo_fixture as B  # noqa: E402
import fixtures_dx7 as F  # noqa: E402
import stress as S  # noqa: E402
from gf180_dx7 import host as hostmod  # noqa: E402

FIXTURE = REPO / "spec" / "demo-audition-v1.json"
V1 = REPO / "spec" / "audition-phrases-v1.json"
V2 = REPO / "spec" / "audition-phrases-v2.json"

CORRUPTIONS = [m for m in B.CORRUPTION_MODES if m != "none"]


def _bench(profile: str, mode: str, expect_fail: bool, tmp: Path,
           expect_refuse: bool = False) -> tuple[int, dict]:
    rp = tmp / f"bench-{profile}-{mode}.json"
    rc = B.run_bench(FIXTURE, profile, mode, expect_fail,
                     expect_refuse=expect_refuse, report_path=rp)
    return rc, json.loads(rp.read_text())


class TestFixtureAsset(unittest.TestCase):
    """The committed musical fixture is the plan section 6 protocol:
    velocity layers + chords + releases + a sustained pedal passage plus
    controllers - a real audition, versioned, with expected event counts
    and coverage wants that refuse an under-delivering fixture."""

    def setUp(self):
        self._td = tempfile.TemporaryDirectory()
        self.tmp = Path(self._td.name)

    def tearDown(self):
        self._td.cleanup()

    def test_asset_loads_and_pins_match(self):
        fx, phrases = F.load_fixture(FIXTURE)
        self.assertGreaterEqual(len(phrases), 17)
        for profile in ("compact", "full"):
            play = F.assemble_playthrough(fx, phrases, profile)
            pinned = fx["expected_counts"][profile]
            for k, v in play["counts"].items():
                self.assertEqual(
                    pinned.get(k), v,
                    f"expected_counts[{profile}][{k}] pin drifted from the "
                    f"committed schedule ({v!r} != {pinned.get(k)!r}) - "
                    f"re-render with --update-pins and review")

    def test_u01_phrases_byte_identical_in_v2(self):
        """Extend, not fork: every U01 phrase survives v2 byte-identical
        (raw event lines + duration), and the validator enforces it."""
        v1 = json.loads(V1.read_text())
        v2 = json.loads(V2.read_text())
        by2 = {p["id"]: p for p in v2["phrases"]}
        for row in v1["phrases"]:
            pid = row["id"]
            self.assertIn(pid, by2, f"U01 phrase {pid!r} missing from v2")
            self.assertEqual(by2[pid]["events"], row["events"],
                             f"U01 phrase {pid!r} events changed in v2")
            self.assertEqual(by2[pid]["duration_s"], row["duration_s"],
                             f"U01 phrase {pid!r} duration changed in v2")
        fx, phrases = F.load_fixture(FIXTURE)
        F.check_superset_of_v1(V1, phrases)  # raises FixtureRefuse if not

    def test_underdelivering_fixture_refused(self):
        """Dropping the pedal + all-notes-off phrases under-delivers the
        declared coverage (plan section 6 requires the sustained pedal
        passage and CC120; no other phrase supplies CC64/CC120): the
        harness must refuse it (bench exit 2), never improvise around
        it."""
        fx, phrases = F.load_fixture(FIXTURE)
        order = fx["playthrough_order"]["full"]
        for pid in ("sustain-pedal-60-64", "all-notes-off-60-64-67"):
            self.assertIn(pid, order)
        fx2 = copy.deepcopy(fx)
        fx2["playthrough_order"]["full"] = [
            p for p in order
            if p not in ("sustain-pedal-60-64", "all-notes-off-60-64-67")]
        play = F.assemble_playthrough(fx2, phrases, "full")
        problems = F.check_want(F.coverage_from_schedule(play["events"]),
                                fx2, "full")
        self.assertTrue(problems, "a fixture without the pedal/ANO "
                                  "passages slipped through the coverage "
                                  "wants")
        self.assertTrue(any("pedal" in p for p in problems), problems)
        self.assertTrue(any("all-notes-off" in p for p in problems), problems)
        # and the bench refuses it before any write (exit 2)
        p = self.tmp / "underdelivers.json"
        p.write_text(json.dumps(fx2))
        rp = self.tmp / "refuse-report.json"
        rc = B.run_bench(p, "full", "none", False, expect_refuse=True,
                         report_path=rp)
        self.assertEqual(rc, 0)
        self.assertEqual(json.loads(rp.read_text())["status"], "REFUSE")

    def test_one_note_audition_refused(self):
        """A one-note audition is not a demonstration (plan section 6):
        min_distinct_notes must refuse a single-note schedule."""
        fx, phrases = F.load_fixture(FIXTURE)
        fx2 = copy.deepcopy(fx)
        fx2["playthrough_order"]["full"] = ["low-36-vel100"]
        play = F.assemble_playthrough(fx2, phrases, "full")
        problems = F.check_want(F.coverage_from_schedule(play["events"]),
                                fx2, "full")
        self.assertTrue(any("one-note" in p for p in problems), problems)

    def test_fixture_rejects_alias_patch_identity(self):
        """Refuse-identity: a fixture whose patch canonical hash is not
        the committed one (or whose bank bytes are missing) is refused,
        not quietly substituted."""
        fx, _ = F.load_fixture(FIXTURE)
        bad = copy.deepcopy(fx)
        bad["patch"]["canonical_hash"] = "0" * 64
        with self.assertRaises(F.FixtureRefuse):
            F.resolve_patch(bad)


class TestBench(unittest.TestCase):
    """Clean runs pass every gate with the three-way equality
    (host prediction = CS_N pin = drain frame at every control-path
    landing frame); each corruption mode must trip its NAMED gates and
    nothing else."""

    def setUp(self):
        self._td = tempfile.TemporaryDirectory()
        self.tmp = Path(self._td.name)

    def tearDown(self):
        self._td.cleanup()

    def test_clean_compact_passes(self):
        rc, rep = _bench("compact", "none", False, self.tmp)
        self.assertEqual(rc, 0)
        self.assertEqual(rep["status"], "PASS")
        for gate, val in rep["gates"].items():
            self.assertTrue(val["pass"], gate)
        self.assertNotIn("expect_fail_verification", rep)

    def test_expect_fail_on_clean_run_is_refused(self):
        """--expect-fail has no named gates on a clean run (mode none has
        no EXPECTED_GATES entry): the bench refuses it rather than
        verifying a control that cannot fail. Together with
        test_every_corruption_mode_verifies (every named gate must fire
        on corruption) and test_clean_compact_passes (every named gate
        passes when clean), a vacuous gate - always-pass or always-fail
        - cannot escape the suite."""
        rc, rep = _bench("compact", "none", True, self.tmp)
        self.assertEqual(rc, 2, rep)
        self.assertEqual(rep["status"], "REFUSE")
        self.assertIn("corruption mode", rep["reason"])

    def test_every_corruption_mode_verifies(self):
        for mode in CORRUPTIONS:
            with self.subTest(mode=mode):
                rc, rep = _bench("compact", mode, True, self.tmp)
                self.assertEqual(rc, 0, f"{mode}: rc={rc}")
                self.assertTrue(
                    rep["expect_fail_verification"]["passed"],
                    f"{mode}: the named gates did not fire as expected - "
                    f"the corruption was not caught the way the contract "
                    f"says it must be (no resolution): "
                    f"{rep['expect_fail_verification']['problems']}")

    def test_want_refuse_short_circuits_before_writes(self):
        """A fixture refused on coverage must not deliver a single write:
        the refuse happens before the link is touched (exit 2)."""
        fx, phrases = F.load_fixture(FIXTURE)
        bad = copy.deepcopy(fx)
        bad["coverage_wants"]["compact"] = dict(
            bad["coverage_wants"]["compact"])
        bad["coverage_wants"]["compact"]["min_distinct_notes"] = 99
        p = self.tmp / "underdelivers.json"
        p.write_text(json.dumps(bad))
        rp = self.tmp / "refuse-report.json"
        rc = B.run_bench(p, "compact", "none", False,
                         expect_refuse=True, report_path=rp)
        self.assertEqual(rc, 0)
        rep = json.loads(rp.read_text())
        self.assertEqual(rep["status"], "REFUSE")
        self.assertTrue(rep["coverage_problems"])
        # no link activity: the refuse happened pre-assembly
        self.assertNotIn("boot", rep)


class TestStress(unittest.TestCase):
    """The U05 stress battery, in-process: fast play, sustain pedal,
    patch change while notes are held (the recall check), all-sound-off,
    and the REQUIRED deliberately-throttled link whose underrun handling
    must engage per contract - with the resolution proof (the same
    check on a clean link must fail, so the check cannot be vacuous)."""

    def setUp(self):
        self.fx, _ = F.load_fixture(FIXTURE)
        self.voice = F.resolve_patch(self.fx)

    def test_all_scenarios_pass(self):
        rep = S.run_all(self.voice)
        self.assertEqual(rep["status"], "PASS", rep)
        for name in S.SCENARIOS:
            with self.subTest(scenario=name):
                self.assertEqual(rep["scenarios"][name]["status"], "PASS",
                                 rep["scenarios"][name])

    def test_throttled_underrun_engages_and_is_not_vacuous(self):
        rep = S.run_all(self.voice)
        sc = rep["scenarios"]["throttled_link"]
        self.assertGreaterEqual(
            sc["rejections"], 1,
            "the deliberately throttled link logged no explicit rejection: "
            "the underrun handling was never engaged")
        # resolution: the same check on an UNTHROTTLED run must fail
        core = B.TappedCore()
        host = hostmod.EmbeddedHost(core, hostmod.DESIGN_POINT)
        host.boot(self.voice)
        S.drive(host, core, S.build_events("fast_play"))
        problems = S.check_core(host, core, set(), require_overflow=True)
        self.assertTrue(any("NO explicit queue-overflow" in p
                            for p in problems),
                        "the underrun check passed on a healthy link: it "
                        "cannot distinguish underrun from normal delivery")

    def test_stuck_note_is_detected(self):
        core = B.TappedCore()
        host = hostmod.EmbeddedHost(core, hostmod.DESIGN_POINT)
        host.boot(self.voice)
        evs = [F.LinkEvent("note_on", 0, "stress:unit", "unit",
                           note=60, velocity=100)]
        S.drive(host, core, evs)
        problems = S.check_core(host, core, set())
        self.assertTrue(any("stuck" in p or "held" in p for p in problems),
                        f"an unreleased note passed the scenario checks: "
                        f"{problems}")

    def test_patch_change_while_held_commits_mid_hold(self):
        """The recall path inside play: a same-patch commit lands while a
        note is held, and the scenario ends with no stuck notes."""
        core = B.TappedCore()
        host = hostmod.EmbeddedHost(core, hostmod.DESIGN_POINT)
        evs = S.build_events("patch_changes_while_held")
        marker = next(i for i, e in enumerate(evs)
                      if e.kind == "pause_before_commit")
        host.boot(self.voice)
        S.drive(host, core, evs[:marker])
        self.assertTrue(core.held, "no note was held at the recall point")
        held_before = sorted(core.held)
        gen_before = core.generation
        host.load_patch(self.voice)
        self.assertEqual(core.generation, gen_before + 1,
                         "the mid-hold commit did not advance the generation")
        S.drive(host, core, evs[marker + 1:])
        self.assertEqual(sorted(core.held), [],
                         "stuck notes after the scenario")
        self.assertTrue(held_before, held_before)


class TestManifestValidator(unittest.TestCase):
    """The dry-run/record-time version-manifest policy, from the brief:
    a declared dry run NOW is accepted (hardware rows NOT_RUN/BLOCKED,
    each naming its operator issue, concrete software values); at record
    time ANY placeholder anywhere is refused by name; a dry manifest
    may never carry a concrete hardware value or a PASS hardware row
    (an unsupported hardware claim)."""

    def _base(self) -> dict:
        return {
            "schema": check_manifest.SCHEMA,
            "run_id": "u05-demo-20260923T000000Z",
            "mode": "dry",
            "date": "2026-09-23T00:00:00Z",
            "claim": check_manifest.DRY_CLAIM,
            "software": {
                "repo_sha": "a" * 40,
                "branch": "loom/u05-gf180",
                "python": "3.13.7",
                "fixture": {"path": "spec/demo-audition-v1.json",
                            "sha256": "ab" * 32},
                "phrase_set": {"path": "spec/audition-phrases-v2.json",
                               "sha256": "cd" * 32},
                "patch": {"source": "tests/fixtures/valid_bank.syx slot 7",
                          "canonical_hash": "ef" * 32},
                "tools": {"demo_fixture.py": {"sha256": "11" * 32},
                          "stress.py": {"sha256": "22" * 32}}
            },
            "results": {
                "demo_clean_compact": "PASS",
                "demo_clean_full": "PASS",
                "stress": {"fast_play": {"status": "PASS"}},
                "negatives": {"drop-deltas": True}
            },
            "hardware": {
                "rtl_tag": "<RTL_TAG>",
                "bitstream_sha256": "<BITSTREAM_SHA256>",
                "bank_version": "<BANK_VERSION>",
                "capture": {"status": "NOT_RUN",
                            "blocked_by": "issue #31"},
                "listening_bank": {"status": "NOT_RUN",
                                   "blocked_by": "issue #35"},
                "board_latency": {"status": "BLOCKED",
                                  "blocked_by": "issue #31"},
                "board_audit": {"status": "BLOCKED",
                                "blocked_by": "issue #31"}
            }
        }

    def test_declared_dry_accepted(self):
        self.assertEqual(check_manifest.check(copy.deepcopy(self._base())),
                         [])

    def test_record_refuses_placeholders_by_name(self):
        m = self._base()
        m["mode"] = "record"
        m["claim"] = "recorded from the board capture under reference/"
        m["hardware"]["rtl_tag"] = "gf180-dx7-rtl-v1"
        m["hardware"]["bitstream_sha256"] = "ab" * 32
        m["hardware"]["bank_version"] = "u02-final"
        for row in ("capture", "listening_bank", "board_latency",
                    "board_audit"):
            m["hardware"][row]["status"] = "PASS"
        self.assertEqual(check_manifest.check(m), [])
        m["hardware"]["capture"]["blocked_by"] = "<FILL_LATER>"
        problems = check_manifest.check(m)
        self.assertTrue(any("placeholder" in p and "blocked_by" in p
                            for p in problems), problems)

    def test_dry_refuses_concrete_hardware_values(self):
        m = self._base()
        m["hardware"]["bitstream_sha256"] = "ab" * 32
        problems = check_manifest.check(m)
        self.assertTrue(any("bitstream_sha256" in p for p in problems),
                        problems)

    def test_dry_refuses_placeholder_software_rows(self):
        m = self._base()
        m["software"]["python"] = "<TBD>"
        problems = check_manifest.check(m)
        self.assertTrue(any("python" in p for p in problems), problems)

    def test_dry_hardware_rows_may_not_pass(self):
        m = self._base()
        m["hardware"]["capture"]["status"] = "PASS"
        problems = check_manifest.check(m)
        self.assertTrue(any("capture" in p for p in problems), problems)

    def test_not_run_rows_must_name_their_blocker(self):
        m = self._base()
        m["hardware"]["listening_bank"]["blocked_by"] = ""
        problems = check_manifest.check(m)
        self.assertTrue(any("blocked_by" in p for p in problems), problems)


if __name__ == "__main__":
    unittest.main()