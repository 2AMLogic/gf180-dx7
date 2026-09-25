# H10 run-attempts digest (DR-0011, PR-A scope)

Digest-only record of the synthesis-path runs behind the single-driver core
refreeze. Full logs are hash-referenced; the heavy artifacts (full 1_2 logs,
`1_synth.odb`, netlists, floorplan) are NOT committed — they land with the H10
report bundle (PR-B, `evidence/h10-gf180/`). Digest rows: run, tool identity,
artifact (sha256), outcome. All runs on machine `repo-remote-gf180-dx7`
(AWS EC2, m5.2xlarge) except record B (operator host, macOS).

Tool identities:
- Pinned ORFS flow image: `openroad/orfs:26Q3-296-gda37dce1c`, yosys 0.67
  (git `2d1509d1b`), platform gf180mcu 7t, `FLOW_VARIANT=natdie`.
- Host yosys (record B only): Yosys 0.69+post (git `143eb14f9cc55d6f8927e68523b0c9d2166ed02c`, AppleClang 21).

| # | Run | Core revision (rtl fingerprint, first 16) | Artifact (sha256) | Outcome |
|---|-----|-------------------------------------------|-------------------|---------|
| A | `do-yosys-canonicalize` (1_1) + `check -assert` on canonicalized netlist, pinned image | frozen `335599ea…` (H07 pin) | `evidence/h10-legality/witness-frozen-image.log` (netlist `2fd2fa57a041…`, log hash `5e2293ee4d`) | **0 problems** — elaboration-level legality clean |
| B | raw host-yosys elaboration + `check -assert` (3 files, top `dx7_core`) | frozen `335599ea…` | `evidence/h10-legality/witness-frozen-raw-check.log` (log hash `4c17d43339`) | **0 problems** — supersedes the 2026-09-23 "9 fatals" working note; see DR-0011 supersession note. Carries the silent out-of-range-select finding (dx7_core.v:1028-1030) |
| C | `do-yosys-canonicalize` (1_1) + `check -assert`, pinned image, same flow/machine as A | refrozen `34f93d2d…` (DR-0011 candidate) | `evidence/h10-legality/witness-merged-image.log` (log hash `9939688ca1`) | **0 problems** |
| D | flow `do-yosys` (1_2, full synthesis incl. gf180 mapping), pinned image | frozen `335599ea…` | `evidence/h10-legality/witness-frozen-1_2-check.log` (full log `c646d1a6ef64085c60d52227fc2f74b840e2c7b3ad5d1403f098feabef071473`, 5,080,150 B) | **FAIL** — pass 22 `check -assert`: "ERROR: Found 9 problems" (9 `evq_cnt[0..8]` cells with multiple conflicting drivers; "multiple conflicting drivers" occurs 12,938× across passes). No netlist stats produced |
| E | flow `do-yosys` (1_2) on the refrozen core, same flow/machine as D | refrozen `34f93d2d…` | `pnr/orfs/work/logs/gf180/dx7core/natdie/1_2_yosys.log` = `e5e706cd86eafdb413b87d3497ec76508a86696a8a353c9f6df1bed4048a76bb` (2026-09-24 02:45:46Z) | **PASS** — clean `check -assert`, mapped netlist `1_2_yosys.v` (217,270,769 B) |
| F | flow `1_synth` (synth database), same run as E | refrozen `34f93d2d…` | `pnr/orfs/work/results/gf180/dx7core/natdie/1_synth.odb` = `5b8118c440a96a1c61d192578d00347601c6f7d4a76d912953e3399bba7576ee` (491,339,608 B, 2026-09-24 02:46:37Z) | **PASS** — merged core is synthesized and database-written end to end |
| G | flow `2_1_floorplan`, same work dir, after config fix iterations | refrozen `34f93d2d…` | `2_1_floorplan.odb` = `e6026ac7a5fe7e2ca6faf2b8624cc8811308b096833321c9ae9ae666629df481` (2026-09-24 03:39:28Z) | **PASS** — floorplan exists |
| H | flow `do-3_5_place_dp`, same work dir | refrozen `34f93d2d…` | `~/h10-floor7.log` (box; config-level failure, 0.87 s — instant fail, not a long run): `[ERROR ORD-0007] …/3_4_place_resized.odb does not exist` | **BLOCKED (out of PR-A scope)** — place/PnR iteration and the full physical result are PR-B (H10 report) content |
| I | H07 battery, first attempt (verilator), box | refrozen `34f93d2d…` | `~/battery-p3.log` (box, 2026-09-24 01:49Z, 70 B — attempt record only) | superseded by J (fresh clean-tree run per the parked-notice rule: no mixing of partial artifacts into final evidence) |
| J | H07 battery, final (verilator, clean tree), box | refrozen (fingerprint `7bab93f5…`) | `evidence/h07-core/results-h10-merged-p3.json` = `813f3a9035db62f174ba5fe1ecf83ebed45d399dbfb98502c40f2c4a22a7c1e6`, `tag=h10-merged-p3`, latency contract H03 §4.5 [(64-p), (64-p)+65] | **PASS 34/34** — behavioral bit-exactness of the refrozen core vs the frozen model, including the patchcommit/stress cases that exercise the merged control path |
| K | H07 canonical accept battery re-cert on the refrozen core: two clean verilator runs (build+run per case), box | refrozen (fingerprint `7bab93f5…`) | `evidence/h07-core/results-verilog-accept-r1.json` (run 1) and `evidence/h07-core/results-verilog-accept.json` (tag `verilog-accept`, run-1 hashes embedded as `determinism_run2`); toolchain: oss-cad-suite Verilator 5.041 devel | **PASS 34/34 both runs**, zero overrun/overflow, every latency in window, per-case artifact sha256 identical across the two clean runs — this is the file `test_h07.TestConformanceEvidence` freshness-checks |
| L | iverilog cross-shadow re-run (24-frame window), box | refrozen (fingerprint `7bab93f5…`) | `evidence/h07-core/results-iverilog-shadow.json` (tag `iverilog-shadow`) | **33/34** — `dev32-30` deadlocked in the box's iverilog 13.0 devel build (vvp 99% CPU >2 h, zero output growth; killed by PID per the box hard-rules notice) — recorded as a **tool failure, not an RTL divergence**: the same case passes cleanly twice under Verilator (rows J/K). Full iverilog cross-shadow on a stable build is deferred to the H10 report (PR-B) |
| M | H07 synthesis-gate re-cert on the refrozen core (`tools/h07_synth.py`): host yosys 0.57+72 (`fa02d71f6`) with dfflibmap/abc against the gf180mcu 7t liberty (sha-pinned in the report), box | refrozen (all three `rtl` shas verified FRESH against the report) | `evidence/h07-core/synth_report.json` + `yosys_full.log`, `yosys_strip.log` | **gates: full = PASS** (mapped-flop floor > 38,781 floor held, area/cells > 0) and **strip_negative_control = MET (gate fails as required)** — the flop gate has resolution; the committed report is the one `test_h07.TestStripObsControl` freshness-checks |

Rule of this digest: a claim of synthegizability/fits may only cite rows that
actually ran (rows E/F/G/H); estimation-derived fit claims fail review (issue
#32 negative control). Row H is recorded as BLOCKED, not omitted.
Addendum (H10 report, PR-B, issue #32): row H is diagnosed in
`docs/H10-GF180-FEASIBILITY.md` section 6. In the pinned image's
`flow/Makefile` the phony `do-3_5_place_dp` target depends only on
`copyright.txt`, so invoking it without stages 3_1-3_4 aborts with ORD-0007.
That is an invocation-order error, not a flow or driver defect. Row H stays
BLOCKED as recorded; placement cannot run on either fixed die in any case,
because the mapped cells exceed both cores.
