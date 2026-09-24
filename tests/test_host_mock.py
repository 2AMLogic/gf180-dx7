#!/usr/bin/env python3
"""U04 embedded-host-vs-mock-core tests (issue #37).

Covers, per the issue's acceptance:
  - event timing meets the H03 contract under loaded conditions: a
    16-note chord + an immediate patch commit inside one frame window at
    the 24.576 MHz design point (no queue overflow at the contracted
    depth-8 skid FIFO, no defect flags, every event applied before the
    first sample of frame N+1), and the same burst REFUSED explicitly at
    the 12.288 MHz fallback (DEC-014: silent deferral is forbidden);
  - reset/recall: boot protocol (reset -> fresh -> image) restores exact
    state, and recall after a mid-transfer pad reset re-delivers the
    committed image byte-identically (transaction log hashed);
  - partial transfers: a truncated patch load is detected, commits
    NOTHING, the stale committed image stays observable, and the
    negative control holds in both directions - a naive loader that
    skips commit verification CLAIMS success but the stale-state check
    FAILS it;
  - malformed SysEx through the full path: P01 error classes propagate
    as contract errors with nothing placed on the link;
  - write-cost accounting: DX7-scale traffic (16-note burst, full boot
    image) against the H03 link budget at both clocks, with the
    parasynth boot-image datapoint (182 writes ~= 4.43 ms at 2 MHz)
    recorded as the family anchor;
  - failure injection, all visibly handled, never silent: queue
    overflow (host retry per DEC-014), drain-stall backpressure,
    overrun (defect detector), reset mid-transfer - plus a silent-loss
    stub control that the no-loss check demonstrably FAILS;
  - the status word shape (first read 0xD711_0001; fresh/queue/
    overflow/overrun fields) and the OVERRUN-never-sets rule over all
    clean traffic in this suite.

Stdlib only; unittest. The mock core is the device under test's
counterpart, not an FM engine: claims are host-side transaction
conformance only (no RTL/board/fidelity claim, AGENTS.md).
"""

import hashlib
import os
import sys
import unittest

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, "src"))

from gf180_dx7 import mock_core as mc
from gf180_dx7 import sysex
from gf180_dx7.host import (BOOT_IMAGE_WRITES, DESIGN_POINT, FALLBACK,
                            LOAD_COST_BOUND, NOTE_OFF_WRITES, NOTE_ON_WRITES,
                            CONTROLLER_WRITES, WORST_BURST_WRITES,
                            BackpressureRefused, ContractDefect,
                            EmbeddedHost, HostError, MalformedPatch,
                            PatchLoadRefused, QueueOverflow, ResetObserved,
                            TruncatedTransfer, capacity, frame_of_sample,
                            link_seconds, parse_status)
from gf180_dx7.mock_core import (ADDR_CTRL, ADDR_OP_DDELTA_BASE, CMD_COMMIT,
                                 CMD_NOTE_ON, Frame48, MockCore, SEC_EVENTS,
                                 SKID_DEPTH, TXN_PERIOD_CLKS)

FIXTURES = os.path.join(REPO, "tests", "fixtures")


def load_fixture(name: str) -> bytes:
    with open(os.path.join(FIXTURES, name), "rb") as f:
        return f.read()


VOICE = sysex.decode_voice(load_fixture("valid_voice.syx"))
BANK_BYTES = load_fixture("valid_bank.syx")
BANK = sysex.decode_bank(BANK_BYTES)


def variant_voice(voice: sysex.Voice, offset: int, value: int) -> sysex.Voice:
    """A DIFFERENT valid patch: same voice with one parameter changed and
    the SysEx checksum recomputed. Partial-transfer tests attempt THIS
    patch so 'stale committed image' and 'intended patch' disagree."""
    raw = bytearray(voice.raw_bytes)
    raw[offset] = value
    body = bytes(raw)
    message = bytes((0xF0, 0x43, voice.channel_byte, 0x00, 0x01, 0x1B)) \
        + body + bytes((sysex.checksum(body), 0xF7))
    return sysex.decode_voice(message)


# A second patch: different algorithm + feedback + name byte.
VOICE_B = variant_voice(VOICE, 134, (VOICE.algorithm + 7) % 32)
assert VOICE_B.raw_bytes != VOICE.raw_bytes
assert sysex.checksum(VOICE_B.raw_bytes) != sysex.checksum(VOICE.raw_bytes)


def chord_events(n: int = 16, base: int = 48, sample: int = 0):
    return [dict(kind="note_on", sample=sample + i, note=base + i,
                 velocity=(32, 80, 127)[i % 3]) for i in range(n)]


class NaiveLoader(EmbeddedHost):
    """NEGATIVE-CONTROL stub: a loader that swallows transport
    rejections and skips commit verification - exactly the silent/stale
    failure the issue's negative control forbids. The checks below must
    FAIL it."""

    def _send(self, frame, *, retries=2):
        try:
            super()._send(frame, retries=retries)
        except TruncatedTransfer:
            pass

    def _verify_commit(self, voice):
        self.committed_voice = voice
        return {"generation": self.core.generation, "digest": -1}


class DeafHost(EmbeddedHost):
    """NEGATIVE-CONTROL stub: ignores explicit overflow rejection (never
    reads status, never retries) - the silent-loss host the DEC-014
    backpressure rule exists to prevent."""

    def _send(self, frame, *, retries=2):
        self.core.transfer(frame.encode())
        self.log.append(frame)


class U04TestBase(unittest.TestCase):
    BOOT_IN_SETUP = True

    def setUp(self):
        self.core = MockCore()
        self.host = EmbeddedHost(self.core)
        if self.BOOT_IN_SETUP:
            self.host.boot(VOICE)

    def assert_clean(self, core, since=0):
        """No fault, no defect flag: normal-operation invariant. `since`
        bounds the pre-existing fault count when earlier injected faults
        are retained as evidence."""
        self.assertEqual(
            len(core.faults), since,
            f"unexpected faults: {core.faults[since:]}")
        word = core.read_status()
        status = parse_status(word)
        self.assertFalse(status["overflow"], "OVERFLOW sticky set")
        self.assertFalse(status["overrun"], "OVERRUN sticky set")


class TestBurstTiming(U04TestBase):
    def test_16_note_chord_plus_commit_at_design_point(self):
        """Issue acceptance: event timing under burst load - a patch-load
        commit and the 16-note chord share one frame window at the
        24.576 MHz design point; the skid FIFO never overflows and every
        event applies before frame N+1, with the new notes bound to the
        newly committed generation (commit applies first, H06 section 2)."""
        # frame A: deliver the patch image (165 shadow writes), no commit
        image = self.host.patch_image_frames(VOICE_B)[:-1]
        for frame in image:
            self.host._send(frame)
        self.host.poll()
        self.core.flush_frame()
        self.assertEqual(self.core.generation, 1)
        # frame B: the commit + the full chord in one window
        # 1 commit + 16 x 8 note-on writes + 1 poll <= 168
        reception_frame = self.core.frame_now
        self.host._send(Frame48(SEC_EVENTS, 0x00, CMD_COMMIT << 16))
        accounting = self.host.schedule_burst(
            chord_events(), start_frame=reception_frame)
        self.core.flush_frame()
        applied = [e for e in self.core.applied_events()
                   if e.kind == "note_on"]
        self.assertEqual(len(applied), 16)
        commit_records = [e for e in self.core.applied_events()
                          if e.kind == "commit"]
        self.assertEqual(len(commit_records), 2)  # boot + this commit
        self.assertEqual(self.core.generation, 2)
        self.assertEqual(self.core.committed_digest(2),
                         sysex.checksum(VOICE_B.raw_bytes))
        for record in applied:
            self.assertEqual(record.frame, reception_frame + 1)
            self.assertIn("gen 2", record.detail)
        used = 1 + 16 * NOTE_ON_WRITES + 1
        self.assertLessEqual(used, capacity(DESIGN_POINT))
        self.assertEqual(accounting[0]["writes"], 16 * NOTE_ON_WRITES)
        self.assert_clean(self.core, since=0)

    def test_bare_commit_with_no_image_is_refused(self):
        """A COMMIT with nothing loaded has nothing to commit: the digest
        gate refuses it, recorded (the committed image is untouched)."""
        generation = self.core.generation
        self.host._send(Frame48(SEC_EVENTS, 0x00, CMD_COMMIT << 16))
        self.core.flush_frame()
        self.assertEqual(self.core.generation, generation)
        self.assertIn("commit_refused", [f.kind for f in self.core.faults])

    def test_burst_refused_explicitly_at_fallback_clock(self):
        """DEC-014: a frame that cannot fit the 84-window fallback
        capacity is refused with the excess enumerated - never silently
        deferred to a later frame."""
        core = MockCore(FALLBACK)
        host = EmbeddedHost(core, FALLBACK)
        host.boot(VOICE)
        before = len(core.applied_events())
        with self.assertRaises(BackpressureRefused) as ctx:
            host.schedule_burst(chord_events() +
                                [dict(kind="note_off", sample=i, note=48 + i)
                                 for i in range(16)])
        message = str(ctx.exception)
        self.assertIn("would defer silently", message)
        self.assertIn("DEC-014", message)
        # nothing was sent: the refusal precedes any transfer
        self.assertEqual(len(core.applied_events()), before)
        self.assertEqual([f.kind for f in core.faults], [])

    def test_events_apply_at_frame_n_plus_1_boundary(self):
        """H03 4.2: an event received during frame N is applied before the
        first sample of frame N+1 - including mixed event kinds."""
        reception = self.core.frame_now
        events = chord_events(4) + [
            dict(kind="note_off", sample=1, note=48),
            dict(kind="controller", sample=2, cc=64, value=127),
            dict(kind="bend", sample=3, value=8192 + 512),
        ]
        self.host.schedule_burst(events, start_frame=reception)
        self.core.flush_frame()
        kinds = [(e.kind, e.frame) for e in self.core.applied_events()]
        for kind, frame in kinds[-6:]:
            self.assertEqual(frame, reception + 1)
        self.assertIn(("controller", reception + 1), kinds)

    def test_worst_admissible_burst_fits_design_point_window(self):
        """H03 4.2 worst burst: 16 note-ons + 16 note-offs + 16 controller
        events = 160 writes (U04 map) + 1 poll <= 168 at 24.576 MHz."""
        reception = self.core.frame_now
        events = (chord_events()
                  + [dict(kind="note_off", sample=i, note=48 + i)
                     for i in range(16)]
                  + [dict(kind="controller", sample=i, cc=(1, 7, 64)[i % 3],
                          value=64) for i in range(16)])
        self.host.schedule_burst(events, start_frame=reception)
        self.core.flush_frame()
        note_ons = [e for e in self.core.applied_events()
                    if e.kind == "note_on"]
        note_offs = [e for e in self.core.applied_events()
                     if e.kind == "note_off"]
        ctrls = [e for e in self.core.applied_events()
                 if e.kind == "controller"]
        self.assertEqual((len(note_ons), len(note_offs), len(ctrls)),
                         (16, 16, 16))
        self.assertLessEqual(WORST_BURST_WRITES + 1,
                             capacity(DESIGN_POINT))
        self.assert_clean(self.core)

    def test_patch_change_while_held_uses_generation_ring(self):
        """H03 4.1 (DEC-019/DEC-021): a held note keeps its starting
        generation across a commit; a new note-on binds the new one."""
        held_frame = self.core.frame_now
        self.host.schedule_burst(
            [dict(kind="note_on", sample=0, note=60, velocity=100)],
            start_frame=held_frame)
        self.core.flush_frame()
        self.host.load_patch(VOICE)  # commit -> generation 2
        new_frame = self.core.frame_now
        self.host.schedule_burst(
            [dict(kind="note_on", sample=0, note=72, velocity=90)],
            start_frame=new_frame)
        self.core.flush_frame()
        note_ons = [e for e in self.core.applied_events()
                    if e.kind == "note_on"]
        self.assertEqual(len(note_ons), 2)
        self.assertIn("gen 1", note_ons[0].detail)
        self.assertIn("gen 2", note_ons[1].detail)
        self.assertEqual(self.core.held[60].generation, 1)
        self.assertEqual(self.core.held[72].generation, 2)


class TestBootRecall(U04TestBase):
    BOOT_IN_SETUP = False
    def test_boot_protocol_reset_fresh_image(self):
        """H03 4.4 boot: first status read after reset is the
        0xD711_0001-shaped word; FRESH clears on the first accepted
        write; the image commits atomically."""
        core = MockCore()
        self.assertEqual(core.read_status(), 0xD7110001)
        host = EmbeddedHost(core)
        report = host.boot(VOICE)
        self.assertEqual(report["writes"], BOOT_IMAGE_WRITES)
        self.assertLessEqual(report["writes"], LOAD_COST_BOUND)
        self.assertEqual(report["frames"], 2)  # absorbs across <= 2 frames
        self.assertFalse(core.fresh)
        self.assertEqual(core.generation, 1)
        self.assertEqual(core.committed_digest(1),
                         sysex.checksum(VOICE.raw_bytes))
        image = core.committed_image(1)
        self.assertEqual(bytes(image[i] for i in range(sysex.VOICE_DATA_LEN)),
                         VOICE.raw_bytes)
        self.assert_clean(core)

    def test_recall_after_reset_redelivers_identical_transactions(self):
        """Issue acceptance: recall after reset re-delivers identical
        transactions - hashed over the transaction log."""
        first = self.host.boot(VOICE)
        image_bytes = b"".join(f.encode()
                               for f in self.host.patch_image_frames(VOICE))
        self.assertEqual(hashlib.sha256(image_bytes).hexdigest(),
                         first["image_sha256"])
        image_before = self.core.committed_image(1)
        self.core.pad_reset()
        self.assertTrue(self.core.fresh)
        second = self.host.recall()
        self.assertEqual(second["image_sha256"], first["image_sha256"])
        self.assertEqual(self.core.generation, 1)
        self.assertEqual(self.core.committed_image(1), image_before)
        self.assertEqual(self.core.committed_digest(1),
                         sysex.checksum(VOICE.raw_bytes))
        self.assert_clean(self.core)

    def test_reset_mid_transfer_detected_and_recalled(self):
        """Failure injection: pad reset mid-load. The host detects it
        (FRAME counter regression / FRESH), commits nothing of the
        interrupted image, and recall re-delivers the last good image."""
        good = self.host.boot(VOICE)
        pre_reset_log = [f.encode() for f in self.host.log[
            -BOOT_IMAGE_WRITES:]]
        self.core.injection.reset_after_writes = 200
        with self.assertRaises(ResetObserved):
            self.host.load_patch(VOICE)
        self.assertIn("reset_mid_transfer",
                      [f.kind for f in self.core.faults])
        self.assertEqual(self.core.generation, 0)  # reset-default image
        recovered = self.host.recall()
        self.assertEqual(recovered["image_sha256"], good["image_sha256"])
        self.assertEqual(self.core.generation, 1)
        self.assertEqual(self.core.committed_digest(1),
                         sysex.checksum(VOICE.raw_bytes))
        recalled_log = [f.encode() for f in self.host.log[
            -BOOT_IMAGE_WRITES:]]
        self.assertEqual(recalled_log, pre_reset_log)
        # the retained reset_mid_transfer record is the evidence the
        # detection assertion above used; nothing new since recovery
        self.assert_clean(self.core, since=1)


class TestPartialTransfers(U04TestBase):
    def setUp(self):
        super().setUp()
        self.gen_before = self.core.generation
        self.image_before = self.core.committed_image(1)
        self.digest_before = self.core.committed_digest(1)

    def assert_stale_state(self, core, claimed_voice):
        """The issue's negative-control check: after a partial transfer
        the committed image must be UNCHANGED and differ from the
        intended patch - stale state never silently replaces the
        committed patch, and the failure left evidence."""
        self.assertEqual(core.generation, self.gen_before,
                         "commit landed despite a partial transfer")
        self.assertEqual(core.committed_image(1), self.image_before)
        self.assertEqual(core.committed_digest(1), self.digest_before)
        self.assertNotEqual(core.committed_digest(1),
                            sysex.checksum(claimed_voice.raw_bytes))
        self.assertTrue(core.faults,
                        "partial transfer left no trace: silent failure")

    def assert_host_claim_matches_core(self, host):
        """A host that claims patch P committed at generation G must find
        checksum(P) == core.committed_digest(G). This is the check the
        silent/stale stubs must demonstrably FAIL."""
        voice = host.committed_voice
        self.assertIsNotNone(voice)
        self.assertEqual(
            sysex.checksum(voice.raw_bytes),
            self.core.committed_digest(host.committed_generation),
            "host claim does not match the committed core image")

    def test_truncated_patch_load_detected_no_commit(self):
        """Failure injection: truncated transfer mid-load. The host
        aborts, sends no commit, and the stale committed image is
        observable and reported."""
        self.core.injection.truncate_next_bits = 24
        with self.assertRaises(TruncatedTransfer):
            self.host.load_patch(VOICE_B)
        self.assertIn("truncated_transfer",
                      [f.kind for f in self.core.faults])
        self.assert_stale_state(self.core, VOICE_B)
        # the honest host still claims the OLD patch at generation 1,
        # and that claim verifies against the core
        self.assert_host_claim_matches_core(self.host)
        # recovery: a clean retry commits the intended patch
        faults_before = len(self.core.faults)
        report = self.host.load_patch(VOICE_B)
        self.assertEqual(report["generation"], self.gen_before + 1)
        self.assert_host_claim_matches_core(self.host)
        self.assert_clean(self.core, since=faults_before)

    def test_bad_checksum_commit_refused(self):
        """Failure injection: a corrupted byte in flight (bad checksum at
        the load digest). The core refuses the commit; the host reports
        PatchLoadRefused; retry recovers."""
        self.core.injection.corrupt_byte = (10, 0x7F)
        with self.assertRaises(PatchLoadRefused):
            self.host.load_patch(VOICE_B)
        self.assertIn("corrupt_byte", [f.kind for f in self.core.faults])
        self.assertIn("commit_refused", [f.kind for f in self.core.faults])
        self.assert_stale_state(self.core, VOICE_B)
        faults_before = len(self.core.faults)
        report = self.host.load_patch(VOICE_B)
        self.assertEqual(report["generation"], self.gen_before + 1)
        self.assert_host_claim_matches_core(self.host)
        self.assert_clean(self.core, since=faults_before)

    def test_directed_bad_checksum_injection_refused(self):
        self.core.injection.corrupt_checksum = True
        with self.assertRaises(PatchLoadRefused):
            self.host.load_patch(VOICE_B)
        self.assertIn("commit_refused", [f.kind for f in self.core.faults])
        self.assert_stale_state(self.core, VOICE_B)

    def test_negative_control_naive_loader_fails_stale_check(self):
        """NEGATIVE CONTROL (issue #37): a partial transfer leaving stale
        patch state FAILS. The NaiveLoader stub swallows the truncation
        and CLAIMS success; the core's digest gate keeps the image stale
        (the gate holds), and the host-claim verification check
        demonstrably FAILS the stub's claim."""
        naive = NaiveLoader(self.core)
        self.core.injection.truncate_next_bits = 24
        report = naive.load_patch(VOICE_B)  # blind success
        self.assertEqual(report["digest"], -1)
        self.assertIn("commit_refused", [f.kind for f in self.core.faults])
        self.assert_stale_state(self.core, VOICE_B)
        with self.assertRaises(AssertionError):
            self.assert_host_claim_matches_core(naive)
        # the naive path left the stale image the check demanded
        self.assertEqual(self.core.generation, self.gen_before)


class TestMalformedSysex(U04TestBase):
    def test_p01_error_classes_propagate_as_contract_errors(self):
        cases = [
            ("bad_checksum_voice.syx", sysex.BadChecksum),
            ("bad_framing_voice.syx", sysex.BadFraming),
            ("truncated_voice.syx", sysex.BadLength),
            ("unsupported_dx7ii_performance.syx", sysex.UnsupportedFormat),
        ]
        for name, codec_error in cases:
            with self.subTest(fixture=name):
                core = MockCore()
                host = EmbeddedHost(core)
                txn_before = core.txn_index
                with self.assertRaises(MalformedPatch) as ctx:
                    host.load_patch_bytes(load_fixture(name))
                self.assertIsInstance(ctx.exception.__cause__, codec_error)
                # nothing reached the link: refusal is host-side (P01)
                self.assertEqual(core.txn_index, txn_before)

    def test_bank_dump_requires_slot_and_loads_selected_voice(self):
        core = MockCore()
        host = EmbeddedHost(core)
        host.boot(VOICE)
        with self.assertRaises(MalformedPatch):
            host.load_patch_bytes(BANK_BYTES)
        report = host.load_patch_bytes(BANK_BYTES, bank_slot=7)
        expected = sysex.encode_voice(BANK.voices[7])[6:6 + 155]
        self.assertEqual(core.committed_digest(2), sysex.checksum(expected))
        self.assertEqual(report["generation"], 2)
        self.assert_clean(core)

    def test_bank_slot_out_of_range(self):
        core = MockCore()
        host = EmbeddedHost(core)
        host.boot(VOICE)
        with self.assertRaises(MalformedPatch):
            host.load_patch_bytes(BANK_BYTES, bank_slot=32)


class TestWriteCostAccounting(unittest.TestCase):
    """Issue acceptance: write-cost accounting for DX7-scale traffic
    against the H03 link budget at both clocks."""

    def test_budget_report_matches_contract_arithmetic(self):
        report = EmbeddedHost(MockCore()).budget_report()
        # per-event costs (U04 map; H06 section 2 conventions)
        self.assertEqual(report["note_on_writes"], 8)
        self.assertEqual(report["note_off_writes"], 1)
        self.assertEqual(report["controller_writes"], 1)
        # worst admissible burst: 16+16+16 events
        self.assertEqual(report["worst_burst_writes"], 16 * 8 + 16 + 16)
        # capacities: floor(clks_per_frame / 194), H03 section 2 row 15
        self.assertEqual(report["capacity_24m576"], 32768 // 194)
        self.assertEqual(report["capacity_12m288"], 16384 // 194)
        self.assertEqual(report["capacity_24m576"], 168)
        self.assertEqual(report["capacity_12m288"], 84)
        self.assertTrue(report["burst_fits_24m576"])
        self.assertFalse(report["burst_fits_12m288"])
        # boot image inside the declared load cost (H03 section 4.1)
        self.assertEqual(report["boot_image_writes"], 171)
        self.assertLessEqual(report["boot_image_writes"],
                             report["load_cost_bound"])
        self.assertEqual(report["load_cost_bound"], 216)
        # frame absorption: 2 frames at the design point (declared),
        # 3 at the fallback (recorded; the fallback already FAILS H03)
        self.assertEqual(report["boot_frames_24m576"], 2)
        self.assertEqual(report["boot_frames_12m288"], 3)
        # link times recomputed independently
        self.assertAlmostEqual(report["boot_seconds_24m576"],
                               171 * 194 / 24_576_000, places=9)
        self.assertAlmostEqual(report["burst_seconds_24m576"],
                               160 * 194 / 24_576_000, places=9)
        self.assertAlmostEqual(report["boot_seconds_12m288"],
                               171 * 194 / 12_288_000, places=9)
        # parasynth family anchor present and not treated as our budget
        anchor = report["parasynth_anchor"]
        self.assertEqual(anchor["writes"], 182)
        self.assertAlmostEqual(anchor["seconds_at_2mhz"], 4.43e-3, places=6)
        self.assertIn("family anchor", anchor["role"])

    def test_family_anchor_comparison_recorded(self):
        """The parasynth boot datapoint (182 writes ~= 4.43 ms at 2 MHz
        SPI = 24.3 us/write) against our contracted link (194 core clks
        = 7.89 us/write at SCK = f_core/4). Family anchor only: the
        sibling's numbers are never transferred as a budget (H03)."""
        our_per_write = link_seconds(1, DESIGN_POINT)
        self.assertAlmostEqual(our_per_write, 194 / 24_576_000, places=9)
        anchor_per_write = 4.43e-3 / 182
        # our design-point boot image is the same order as the sibling's,
        # and faster per write (6.144 MHz SCK vs the sibling's 2 MHz)
        self.assertAlmostEqual(
            EmbeddedHost(MockCore()).budget_report()["boot_seconds_24m576"],
            171 * our_per_write, places=9)
        self.assertLess(our_per_write, anchor_per_write)

    def test_measured_traffic_matches_the_budget_model(self):
        """The accounting is measured, not just declared: the mock's
        transaction counter reflects exactly the modeled windows."""
        core = MockCore()
        host = EmbeddedHost(core)
        host.boot(VOICE)
        boot_txns = core.txn_index
        # 171 writes + 1 fresh-wait read + 2 per-frame polls
        self.assertEqual(boot_txns, BOOT_IMAGE_WRITES + 3)
        host.schedule_burst(chord_events(), start_frame=core.frame_now)
        burst_txns = core.txn_index - boot_txns
        self.assertEqual(burst_txns, 16 * NOTE_ON_WRITES + 1)


class TestFailureInjection(U04TestBase):
    def test_queue_overflow_explicit_and_retried(self):
        """NEGATIVE CONTROL (DEC-014): forced overflow must engage the
        host's backpressure handling visibly - rejection read, cleared,
        write retried - never silent loss."""
        target = Frame48(SEC_EVENTS, ADDR_CTRL, 42)
        self.core.injection.freeze_queue = True
        with self.assertRaises(QueueOverflow):
            self.host._send(target)
        self.assertIn("queue_overflow", [f.kind for f in self.core.faults])
        # the drop was observable in the sticky status flag (now cleared)
        self.assertFalse(self.core.sticky_overflow)
        # recovery: injection off, the write is re-sent and applies
        self.core.injection.freeze_queue = False
        self.host._send(target)
        self.core.flush_frame()
        applied = [e for e in self.core.applied_events()
                   if e.kind == "controller"]
        self.assertEqual([e.detail for e in applied], ["cc 0 = 42"])

    def test_negative_control_silent_loss_stub_detected(self):
        """NEGATIVE CONTROL: a deaf host that ignores overflow rejection
        loses writes silently; the no-loss check must FAIL it."""
        deaf = DeafHost(self.core)
        deaf.committed_voice = self.host.committed_voice
        self.core.injection.freeze_queue = True
        deaf.schedule_burst(chord_events(8),
                            start_frame=self.core.frame_now)
        self.core.injection.freeze_queue = False
        self.core.flush_frame()
        applied = [e for e in self.core.applied_events()
                   if e.kind == "note_on"]
        self.assertLess(len(applied), 8,
                        "the stub must demonstrably lose writes")
        self.assertIn("queue_overflow", [f.kind for f in self.core.faults])

    def test_drain_stall_backpressure(self):
        """Failure injection: backpressure stall. The FIFO fills to its
        contracted depth, the next write is explicitly rejected, and
        after the stall clears everything the host intended applies -
        no silent deferral, no loss."""
        self.core.flush_frame()  # align: the burst must fit one window
        self.core.injection.stall_drain_clks = 4000
        sent = 0
        with self.assertRaises(QueueOverflow):
            for i in range(12):
                self.host._send(Frame48(SEC_EVENTS, ADDR_CTRL, i))
                sent += 1
        self.assertEqual(self.core.queue_depth_now(), SKID_DEPTH)
        self.assertIn("drain_stall", [f.kind for f in self.core.faults])
        self.core.tick(6000)  # stall expires; the drain catches up
        self.assertEqual(self.core.queue_depth_now(), 0)
        for i in range(sent, 12):
            self.host._send(Frame48(SEC_EVENTS, ADDR_CTRL, i))
        self.core.flush_frame()
        applied = [e for e in self.core.applied_events()
                   if e.kind == "controller"]
        self.assertEqual(len(applied), 12)
        self.assertEqual([e.detail.split("= ")[1] for e in applied],
                         [str(i) for i in range(12)])

    def test_queue_depth_enforced_at_eight(self):
        """H03 4.2: the skid FIFO is depth 8; the 9th concurrent write is
        rejected explicitly (impossible at the declared SCK bound, so a
        defect detector - here directed)."""
        core = MockCore()
        core.injection.stall_drain_clks = 4000
        accepted = 0
        for i in range(9):
            result = core.transfer(
                Frame48(SEC_EVENTS, ADDR_CTRL, i).encode())
            accepted += result.accepted
        self.assertEqual(accepted, 8)
        self.assertEqual(core.queue_depth_now(), SKID_DEPTH)
        self.assertIn("queue_overflow", [f.kind for f in core.faults])

    def test_overrun_injection_reported_then_cleared(self):
        """H03 4.3: OVERRUN is a sticky defect detector (must never set
        in real operation); when directed it surfaces once and clears on
        read."""
        self.core.injection.overrun_once = True
        with self.assertRaises(ContractDefect):
            self.host.poll()
        self.assertIn("overrun_injected",
                      [f.kind for f in self.core.faults])
        status = self.host.poll()  # sticky cleared by the raising read
        self.assertFalse(status["overrun"])

    def test_overrun_never_sets_across_clean_traffic(self):
        """H03 4.3/H07 rule on the mock: across every clean scenario in
        this suite's traffic patterns, OVERRUN never sets."""
        core = MockCore()
        host = EmbeddedHost(core)
        host.boot(VOICE)
        host.schedule_burst(chord_events(), start_frame=core.frame_now)
        host.load_patch(VOICE)
        host.schedule_burst([dict(kind="note_off", sample=0, note=60)],
                            start_frame=core.frame_now)
        self.assertFalse(core.sticky_overrun)
        self.assertEqual([f.kind for f in core.faults], [])


class TestStatusWord(U04TestBase):
    def test_status_word_shape(self):
        word = (0xD7 << 24) | (0x1 << 20) | (0 << 19) | (1 << 18) \
            | (1 << 17) | (0 << 16) | 1234
        status = parse_status(word)
        self.assertEqual(status["version"], 1)
        self.assertFalse(status["overrun"])
        self.assertTrue(status["queue_nonempty"])
        self.assertTrue(status["overflow"])
        self.assertFalse(status["fresh"])
        self.assertEqual(status["frame"], 1234)

    def test_fresh_clears_on_first_accepted_write(self):
        core = MockCore()
        self.assertTrue(parse_status(core.read_status())["fresh"])
        core.transfer(Frame48(SEC_EVENTS, ADDR_CTRL, 1).encode())
        self.assertFalse(parse_status(core.read_status())["fresh"])

    def test_frame_counter_advances_and_is_free_running(self):
        start = parse_status(self.core.read_status())["frame"]
        self.core.flush_frame()
        after = parse_status(self.core.read_status())["frame"]
        self.assertEqual(after, start + 1)

    def test_frame_of_sample_mapping(self):
        """48 kHz in 64-sample frames (H03 section 1)."""
        self.assertEqual(frame_of_sample(0), 0)
        self.assertEqual(frame_of_sample(63), 0)
        self.assertEqual(frame_of_sample(64), 1)
        self.assertEqual(frame_of_sample(47_999), 749)
        self.assertEqual(frame_of_sample(48_000), 750)


class TestTransportShape(unittest.TestCase):
    def test_frame48_round_trip(self):
        frame = Frame48(SEC_EVENTS, ADDR_OP_DDELTA_BASE + 3, 0xDEADBEEF)
        raw = frame.encode()
        self.assertEqual(len(raw), 6)  # 48 bits, frame-serializable
        decoded = Frame48.decode(raw)
        self.assertEqual(decoded, frame)

    def test_frame48_field_ranges_enforced(self):
        with self.assertRaises(mc.MockCoreError):
            Frame48(SEC_EVENTS, 0x100, 0)
        with self.assertRaises(mc.MockCoreError):
            Frame48(SEC_EVENTS, 0, 1 << 32)

    def test_transaction_period_is_194_clocks(self):
        core = MockCore()
        clk0 = core.clk
        core.transfer(Frame48(SEC_EVENTS, ADDR_CTRL, 0).encode())
        self.assertEqual(core.clk - clk0, TXN_PERIOD_CLKS)


if __name__ == "__main__":
    unittest.main()
