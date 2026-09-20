"""Command-line interface for gf180-dx7.

codec-check runs the repository self-test suite (unittest discovery over
tests/) and reports a verification status plus a process exit code:
  exit 0  -> PASS  (all tests passed)
  exit 1  -> FAIL  (suite ran; at least one failure or error)
  exit 2  -> NOT_RUN (suite could not be discovered/executed)
"""

from __future__ import annotations

import argparse
import sys
import unittest
from pathlib import Path


def _find_tests_dir() -> Path | None:
    candidates = [
        Path(__file__).resolve().parents[2] / "tests",
        Path.cwd() / "tests",
    ]
    for c in candidates:
        if (c / "test_sysex.py").exists():
            return c
    return None


def _find_src_dir() -> Path:
    return Path(__file__).resolve().parents[1]


def codec_check(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        prog="gf180-dx7 codec-check",
        description="Run the self-test suite and summarize (exit 0/1/2).")
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="per-test output")
    args = parser.parse_args(argv)

    tests_dir = _find_tests_dir()
    if tests_dir is None:
        print("codec-check: NOT_RUN (tests/ directory not found)",
              file=sys.stderr)
        return 2

    loader = unittest.TestLoader()
    sys.path.insert(0, str(_find_src_dir()))
    suite = loader.discover(str(tests_dir))
    if loader.errors:
        print(f"codec-check: NOT_RUN ({len(loader.errors)} collection error(s))",
              file=sys.stderr)
        for err in loader.errors:
            print(err, file=sys.stderr)
        return 2

    result = unittest.TextTestRunner(
        verbosity=2 if args.verbose else 1).run(suite)
    total = result.testsRun
    bad = len(result.failures) + len(result.errors)
    if bad:
        print(f"codec-check: FAIL ({bad} of {total} tests failed/errored)")
        return 1
    print(f"codec-check: PASS ({total} tests)")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="gf180-dx7")
    sub = parser.add_subparsers(dest="command", required=True)
    check = sub.add_parser("codec-check", help="run self-test suite (exit 0/1/2)")
    check.add_argument("-v", "--verbose", action="store_true",
                       help="per-test output")
    args, extra = parser.parse_known_args(argv)
    if args.command == "codec-check":
        return codec_check(extra + (["-v"] if args.verbose else []))
    return 2


if __name__ == "__main__":
    sys.exit(main())
