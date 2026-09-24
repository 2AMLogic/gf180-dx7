# U05 — Demo Playback Preparation (issue #38)

Software-only demo readiness for the plan §6 audition protocol. The claim
is exactly: **the software control path is ready and verified on the mock
core; the hardware rows are NOT_RUN, pending #31 (H09 FPGA capture) and
#35 (listening bank)**. This directory and doc never claim board
playback, board latency, original-hardware fidelity, or preset quality.
The mock core is a link-contract conformance counterpart: host-side
transaction conformance only, no audio.

## Single entry point

```sh
python3 tools/demo/run_demo.py        # exit 0 = all software rows pass
```

One command regenerates the committed evidence in `reference/evidence/u05-demo/`:
clean compact + full bench reports, the eight negative-control reports,
the stress battery report, the dry version manifest (self-validated), and
the hardware NOT_RUN record. `make test-fast` carries the fast subset
(token `u05_demo` in `tools/test_fast.sh`); `make test` (wave gate) adds
the full-profile and environment-gated dev32 runs.

## The musical fixture (plan §6 protocol)

Committed and versioned, never improvised per run:

| asset | role |
|---|---|
| `spec/audition-phrases-v2.json` | 17 phrases: U01's twelve, byte-identical superset (extend, not fork), + 5 U05: velocity-layers 32/64/96/127, 4-note chord, sustain-pedal passage, all-notes-off (CC120), 16th-notes fast run |
| `spec/demo-audition-v1.json` | fixture: patch identity (slot 7 of `tests/fixtures/valid_bank.syx`, canonical `d3f96a97…`), phrase-set ref, playthrough order per profile, `expected_counts` pinned, `coverage_wants` per profile |
| `tools/demo/demo_fixture.py` | the bench: assemble → refuse-under-delivery (exit 2) → boot → drive on the production host path (one link frame per song frame) → gates |
| `tools/demo/fixtures_dx7.py` | phrase→schedule mapping + session layer + coverage-from-schedule counting |

Coverage (computed from the schedule, never from a claim) must meet
`coverage_wants` before a single write reaches the link: velocity layers,
a width-≥4 chord, release tails, pedal down+up with a sustained passage,
CC120 all-notes-off, pitch-bend steps, CC1 mod wheel, ≥4 distinct notes
(a one-note audition is refused by name), and no note still held at the
end. A fixture that would under-deliver is REFUSED (exit 2, report
retained) — the ported upstream negative, intact.

### Demo session layer (documented policy, NOT an H03 contract claim)

The H03 link carries generic CTRL writes; it has no pedal state or
all-notes-off semantics. The session layer (what host keyboard firmware
would do) maps CC64<64 pedal-up to parked note-offs in key-up order, and
CC120 to explicit note-offs for all held notes, ascending. Derived
note-offs are part of the link schedule and covered by the same
cross-check.

## The bench gates (every control-path landing frame)

Per event batch and per boot/commit: **host prediction = CS_N pin =
drain frame** for every landing frame (the three-way cross-check),
H03§4.2 applied timing (event visible before the first sample of frame
N+1), commit audit (exactly the expected commits/generations), no
contract defects (OVERRUN may never be set; pad-reset semantics), no
stuck notes, and expected event counts.

## Negative controls (each with NAMED gates, each verified to fire)

`--wrong <mode> --expect-fail` runs the corruption and asserts the NAMED
gates go the expected way (a control that trips the wrong gate fails):

drop-deltas, drop-restore (pad reset mid-write; the no-recall host
delivers nothing), late-window, drop-bend, drop-bend-step, burst
(collapse two bursts into one link frame), throttle (drain stall; DEC-014
explicit rejections observed, zero silent loss — the mock host retries
immediately, backoff would be out of contract), drop-note-off. Clean
run + `--expect-fail` is refused (there are no named gates to verify):
together with the clean-pass and corruption-fire tests, a vacuous gate
cannot pass the suite.

## Stress battery (`tools/demo/stress.py`)

fast_play (16th-note run), sustain_pedal, patch_changes_while_held (a
same-patch commit lands *while* a note is held — the recall path — and
the scenario ends with no stuck notes), all_sound_off (CC120), and the
required **throttled_link**: a deliberately throttled link whose
underrun handling engages per contract — explicit `queue_overflow`
rejections logged, zero silent loss, and the resolution proof: the same
`require_overflow` check on an unthrottled run fails, so the check
cannot be vacuous.

## Version manifest

`spec/demo-manifest-v1.schema.json` (documented) +
`spec/demo-manifest-template.json` + `tools/demo/check_manifest.py`
(executable). Policies: a **declared dry run now** is accepted with
concrete software values (real repo sha, tool hashes, pass/fail results)
and hardware rows NOT_RUN/BLOCKED each naming its operator issue; at
**record time any placeholder anywhere is refused by name**; a dry
manifest may never carry a concrete hardware value or a PASS hardware
row (an unsupported claim).

## Environment-gated dev32 variant

`--patch dev32:<canonical_hash>` runs the same fixture through a real
patch from the pinned AllTheWeb archive (`DX7_ALLTHEWEB_ZIP`), extracted
with hash verification. Absent/unreadable archive → report `NOT_RUN`,
named, never a pass and never a silent skip. Wave-gate tests assert the
gate reports itself honestly.

## Hardware rows (this file's boundary)

`reference/evidence/u05-demo/hardware-not-run.json` records, per row:
NOT_RUN or BLOCKED with the operator issue it is blocked on (#31 H09
board capture/latency/audit; #35 listening bank). A `record`-mode
manifest — concrete RTL tag, bitstream hash, bank version, capture
evidence — is what closes these rows; until then nothing here claims
hardware results.