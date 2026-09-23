#!/bin/sh
# Fast lane (DR-0009): the unit suites that run in seconds on any machine.
# Evidence-heavy suites (oracle renders, yosys synthesis, archive reads) are
# WAVE GATES: run via `make test` before judge approval and at merge waves,
# not inside every builder/merge loop.
set -eu
cd "$(dirname "$0")/.."
python3 -m unittest discover -s tests \
  -p "test_*.py" \
  -k contract -k sysex -k check_reuse -k numeric_profile -k physical \
  -k compare -k registry -k capabilities -k issue_dag -k host_mock \
  -k audition -k reference_manifest -k h08 2>&1 | tail -3
