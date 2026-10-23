"""Shared verdict exceptions for the tool scripts (issue #176).

One owner for the exit-code contract that used to be copied into every
tool: `CheckFailure` is a check that ran and failed (exit 1) and
`CouldNotRun` is a check that could not be evaluated (exit 2). Callers
catch these by class, so every tool must see the same class objects.
Stdlib only, so it can be imported from a sibling script or via a
`sys.path` entry for `tools/`.
"""

class CheckFailure(Exception):
    pass


class CouldNotRun(Exception):
    pass
