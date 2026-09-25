#!/bin/sh
# Fast lane (DR-0009): the unit suites that run in seconds on any machine.
# Evidence-heavy suites (oracle renders, yosys synthesis, archive reads) are
# WAVE GATES: run via `make test` before judge approval and at merge waves,
# not inside every builder/merge loop.
#
# `synthparse` selects the tools/h07_synth.py stat-parser cases (issue #82)
# out of the otherwise heavy tests/test_h07.py: they replay the already
# committed yosys transcripts, so they need no yosys, no verilator and no
# ssh, and run in ~0.2 s.
set -eu
cd "$(dirname "$0")/.."
#
# NOTE: capture discover's exit code explicitly. A bare "discover | tail" in
# POSIX sh exits with tail's status (always 0) and would mask test failures,
# turning a red gate green. (Footgun found while re-pinning this script for
# H08; the audition-suite TCC error it surfaced is environmental — see the
# H08 PR description.)
log=$(mktemp)
set +e
python3 -m unittest discover -s tests \
  -p "test_*.py" \
  -k contract -k sysex -k check_reuse -k numeric_profile -k physical \
  -k compare -k registry -k capabilities -k issue_dag -k host_mock \
  -k audition -k reference_manifest -k h08 -k u05_demo -k bank128 -k h10 \
  -k synthparse -k exp_range \
  >"$log" 2>&1
rc=$?
set -e
# Print the unittest summary ("Ran N tests" + "OK"/"FAILED (...)"), not a
# blind tail: tests may print their own output last (PR #84 review). On
# failure show the log tail so the failing case is visible.
if [ "$rc" -eq 0 ]; then
  grep -E '^(Ran [0-9]+ tests? in |OK$|OK \(|FAILED)' "$log" | tail -2
else
  tail -60 "$log"
fi
rm -f "$log"
test "$rc" -eq 0
