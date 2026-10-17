#!/usr/bin/env bash
# Cold-start entry point for the T1 item 9 attestation (inventory-item9.json).
#
# Runs every shipped Verilog bench (rtl/tb_*.v) from a clean checkout and
# exits 0 only if all of them ran AND passed. A missing simulator is a
# failure here (exit 2, BLOCKED), never a guarded skip: the unit tests skip
# when iverilog/verilator are absent, and an attestation must not treat a
# skipped bench as a pass.
#
#   tb_operator_probe -> tools/h01_compare.py  (iverilog)
#   tb_env_unit       -> tools/h04_compare.py  (iverilog)
#   tb_alg_router     -> tools/h05_compare.py  (iverilog)
#   tb_pitch_mod      -> tools/h06_compare.py  (iverilog)
#   tb_dx7_core       -> tests/test_h07.py TestNegativeControls (Verilator)
#   tb_synth_top      -> tests/test_h08.py TestH08LiveControls  (Verilator)
#
# Each leg runs its conformance (or live-control) pass only; the mutant
# sweeps stay in `make test`. Passing says the benches run and agree with
# their checkers, nothing about musical quality or original-DX7 fidelity.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

missing=()
for tool in iverilog vvp verilator; do
  command -v "$tool" >/dev/null 2>&1 || missing+=("$tool")
done
if [ "${#missing[@]}" -ne 0 ]; then
  echo "BLOCKED: required simulator(s) not on PATH: ${missing[*]}" >&2
  exit 2
fi
iverilog -V 2>/dev/null | head -1 >&2 || true
verilator --version >&2

out="$(mktemp -d)"
trap 'rm -rf "$out"' EXIT

python3 tools/h01_compare.py --skip-mutate --outdir "$out/h01" >/dev/null
python3 tools/h04_compare.py --skip-mutants --outdir "$out/h04" >/dev/null
python3 tools/h05_compare.py --skip-mutants --outdir "$out/h05" >/dev/null
python3 tools/h06_compare.py --skip-mutants --outdir "$out/h06" >/dev/null

# The Verilator legs are unittest classes that skip when the tool is absent
# or a build fails; fail if anything was skipped.
log="$out/verilator.log"
python3 -m unittest -v tests.test_h07.TestNegativeControls \
  tests.test_h08.TestH08LiveControls >"$log" 2>&1 || { tail -40 "$log" >&2; exit 1; }
if grep -qiE 'skipped' "$log"; then
  echo "FAIL: a Verilator bench leg was skipped (NOT_RUN is not a pass)" >&2
  grep -iE 'skip' "$log" >&2
  exit 1
fi
echo "all 6 benches ran and passed"
