# TRACE-PLAN — reference trace adapter (R05, issue #12)

Status: **implemented at wrapper level**; operator-level taps are planned and
explicitly documented-unavailable. Normative data: the versioned tap registry
`reference/trace-registry.json`; executable gate: `tools/capture_trace.py`;
evidence: `reference/traces/smoke.jsonl` + `reference/traces/smoke-meta.json`.

## 1. What exists

The executable reference is the **external GPL oracle**
(`/Users/joseph/dx7-oracle-marki`, pinned by `reference/oracle-identity.json`;
never copied into this Apache-2.0 repository). R05 extended it with a
`--trace <file.jsonl>` flag that emits one JSON line per 64-sample block:

```json
{"block":0,"lfo_value":14917958,"lfo_delay":16777215,"live_mask":0,"keydown_mask":0,"active_notes":0}
```

## 2. Non-invasiveness rule (normative)

**A capture is evidence only if the traced and untraced renders of the same
inputs are byte-identical.** Non-invasiveness is a gate, not a claim:

- `tools/capture_trace.py` runs the pinned oracle twice (fresh subprocess
  each: once without `--trace`, once with), byte-compares the PCM, and
  refuses to write any trace artifact on divergence or on a malformed trace
  (exit 1, nothing written).
- The oracle-side taps are read-only reads of the wrapper's own variables
  (`lfovalue`, `lfodelay`, `voices[n].live`, `voices[n].keydown`) at the
  block boundary; no engine-internal state is read or written.
- Negative control (kept live in `tests/test_trace_capture.py`): a stub
  oracle that writes traces AND perturbs the PCM is rejected by the gate
  with no artifacts. A gate that cannot fail proves nothing.

## 3. Tap inventory (registry v1)

### Available (7)

| tap | granularity | stream | semantics |
|---|---|---|---|
| `events_applied` | per_render | oracle `--meta` | ordered applied-events log: sample position, command, block applied before |
| `lfo_value` | per_block | `smoke.jsonl` | LFO waveform sample for the block (`Lfo::getsample`, int32), shared per timbre |
| `lfo_delay` | per_block | `smoke.jsonl` | LFO delay/phase counter for the block (`Lfo::getdelay`, int32) |
| `live_mask` | per_block | `smoke.jsonl` | 16-bit mask, bit n = wrapper voice n live (summed this block); read after that block's events are applied |
| `keydown_mask` | per_block | `smoke.jsonl` | 16-bit mask, bit n = wrapper voice n keydown (may differ from `live_mask` during releases) |
| `active_notes` | per_block | `smoke.jsonl` | popcount(`live_mask`); validated by the capture tool |
| `final_pcm` | per_render | render `.f32` | dry mono f32le PCM (voice mix after per-voice scaling/clip); byte-identical in both modes by the gate |

**Cross-process stability (R05 finding).** Upstream msfa `Lfo` leaves
`phase_`, `delaystate_` and `randstate_` uninitialized, and
`Lfo::reset()` does not set them — the same undefined start state exists in
upstream Dexed. Consequently `lfo_value`/`lfo_delay` are
**instance-dependent across oracle processes** and comparable within one
captured render only; this is a property of the pinned upstream engine, not
of the trace adapter, and is recorded verbatim in the registry
(`stability`/`stability_reason`). `block`, `live_mask`, `keydown_mask`,
`active_notes`, `events_applied` and the PCM are deterministic across
processes; every capture meta hashes the deterministic columns
(`deterministic_columns_sha256`), and cross-run trace comparison uses only
that hash plus the PCM hash. Repeated renders stay byte-identical because
the smoke fixture routes nothing through the LFO.

### Unavailable (5) — documented, never silently omitted

| tap | granularity | why unavailable | what instrumenting requires |
|---|---|---|---|
| `op_phase` | per_operator_per_block | phase accumulators live inside Dexed's msfa engine (`Dx7Note`/operator state), invisible at wrapper level | copy Apache-2.0 `msfa/` into the external oracle tree + patch that copy (see §5) |
| `op_env_stage` | per_operator_per_block | envelope stage lives in msfa `Env` per `Dx7Note` operator | same as `op_phase` |
| `op_env_level` | per_operator_per_block | envelope level lives in msfa `Env` per `Dx7Note` operator | same as `op_phase` |
| `feedback_history` | per_voice_per_block | DX7 self-feedback shift register is engine-internal msfa state | same as `op_phase`; until available, N0x model bring-up must not assume feedback-history traces exist (issue #12 stop/escalate clause) |
| `op_output` | per_operator_per_block | operator outputs are computed inside `EngineMkI::compute`; only the summed, scaled voice mix is visible | same as `op_phase` |

Related availability notes (registry `notes`):

- **Decoded parameters / routing** need no oracle instrumentation: the
  fixture voice decodes via the P01 codec (`src/gf180_dx7/sysex.py`), which
  yields the patch parameters including the algorithm (routing).
- **Per-voice pre-sum mix** would be obtainable at wrapper level (the
  wrapper accumulates each live voice into `sumbuf`) without msfa surgery;
  candidate future wrapper-level tap, not implemented in registry v1.

## 4. Sample trace (smoke fixture)

`reference/traces/smoke.jsonl` (3000 lines = 192000 samples / 64) +
`reference/traces/smoke-meta.json` (oracle commit/binary hash, registry
hash, input hashes, both PCM sha256, byte-identity verdict). Facts recorded
there: both PCM sha256 equal `1bebaf668e82ae5885750566af22ca47cbb4ed9403e6fc121b349c5fc0bd8e9a`
— also the committed R02 render evidence hash, so the trace-enabled binary
reproduces the R02 baseline exactly; first live block 37 matches the
delayed note-on at sample 2400 (2400/64 = 37).

## 5. Roadmap: operator-level taps

The five unavailable taps all need the same move, in the **external oracle
repository** (GPL-3.0-or-later); nothing is patched inside this repository
and the pinned `~/dev/dexed` tree must stay byte-clean throughout:

1. Copy `msfa/` from the pinned Dexed commit into `dx7-oracle-marki`
   (Apache-2.0, license-compatible there; record provenance: upstream
   commit, paths, hashes, destination, adaptations).
2. Build the oracle against the **copied** msfa; verify the untraced PCM is
   still byte-identical to the pinned baseline (the copy must not change
   behavior — hash-verified sources, same compiler flags).
3. Patch the copy with read-only taps (phase, env stage/level, feedback
   history, operator output) exported per operator/voice per block through
   the same `--trace` JSONL (new fields; registry version bump).
4. Re-prove non-invasiveness end to end (`tools/capture_trace.py` must stay
   green with the new fields validated).
5. Re-pin `reference/oracle-identity.json` (new oracle commit + binary
   hash), update `reference/trace-registry.json`, re-capture the smoke
   trace, and re-run `tests/test_trace_capture.py`.

A smaller roadmap item from the same move: once a **copied** msfa exists in
the oracle tree (step 1–2), the wrapper can define the previously-undefined
LFO start state (`phase_ = delaystate_ = randstate_ = 0` in the copy), which
would make `lfo_value`/`lfo_delay` cross-process reproducible. Until then
the LFO columns remain instance-dependent and are excluded from cross-run
comparison (see §3).

Until that lands, the five taps above are **documented-unavailable**;
model bring-up (N02–N05) works from the available taps plus the in-repo
decoded parameters and must not pretend the missing taps exist.

## 6. Fidelity scope

Trace agreement is software-reference agreement only: it establishes that
the pinned oracle's wrapper state is observable without perturbing it. It is
not original-DX7 fidelity and says nothing about musical quality
(`reference/oracle-protocol.md` section 4, DEC-001).
