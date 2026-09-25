#!/bin/sh
# Fast lane (DR-0009): the unit suites that run in seconds on any machine.
# Evidence-heavy suites (oracle renders, yosys synthesis, archive reads) are
# WAVE GATES: run via `make test` before judge approval and at merge waves,
# not inside every builder/merge loop.
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
  -k audition -k reference_manifest -k h08 -k u05_demo -k bank128 \
  >"$log" 2>&1
rc=$?
set -e
tail -3 "$log"
rm -f "$log"
test "$rc" -eq 0
