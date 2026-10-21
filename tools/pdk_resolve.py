"""Locate the gf180mcuD PDK tree and its 7t5v0 tt liberty. Stdlib only.

One resolver for every synthesis tool and PDK-dependent test, replacing the
per-file machine-specific defaults.  A "PDK root" here is the directory that
CONTAINS ``gf180mcuD/`` (so ``<root>/gf180mcuD/libs.ref/...`` exists); the
callers join SC_LIB_REL / SRAM_*_REL onto it as before.

Search order (first candidate whose liberty matches the pin wins):
  $PDK_ROOT, /usr/share/pdk, /usr/local/share/pdk, ~/share/pdk,
  ~/.ciel, ~/.volare

A candidate is accepted only when the sha256 of its
``gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib`` equals LIB_SHA256, the digest
the repo already pins (evidence/h06-pitch-rtl/synth_report.json,
tools/verify_gf180_7t_techmaps.py).  The ciel/volare version hash is
deliberately NOT checked: different version directories ship the same
liberty bytes, and the liberty is what the pinned evidence depends on.
There is no silent fallback: when nothing matches, PdkNotFound names every
path tried and the expected digest.

An explicit root (``--ciel-root`` / ``--pdk-root``) is honoured as given
(the liberty file must exist) and is not digest-checked: it is the
operator's deliberate choice, as before.
"""

import functools
import hashlib
import os

LIB_SHA256 = "330aa9439255e3870ad7adfb987cd4d7bcfa7b6f9fb0cd0a712400e9bd80e38a"
SC_LIB_REL = ("gf180mcuD/libs.ref/gf180mcu_fd_sc_mcu7t5v0/lib/"
              "gf180mcu_fd_sc_mcu7t5v0__tt_025C_5v00.lib")

# Shown in the non-existent placeholder path used by tests when no PDK is
# found, so a skip message reads sensibly.
NO_PDK_PLACEHOLDER = "<no gf180mcuD PDK found; see tools/pdk_resolve.py>"


class PdkNotFound(RuntimeError):
    pass


def candidate_roots():
    cands = []
    env = os.environ.get("PDK_ROOT")
    if env:
        cands.append(env)
    cands += ["/usr/share/pdk", "/usr/local/share/pdk",
              os.path.expanduser("~/share/pdk"),
              os.path.expanduser("~/.ciel"),
              os.path.expanduser("~/.volare")]
    return cands


@functools.lru_cache(maxsize=None)
def _sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _check(root):
    """Returns None when ``root`` holds the pinned liberty, else a reason."""
    lib = os.path.join(root, SC_LIB_REL)
    if not os.path.isfile(lib):
        return "no 7t5v0 tt liberty"
    got = _sha256(lib)
    if got != LIB_SHA256:
        return f"liberty sha256 {got[:8]}... != pinned {LIB_SHA256[:8]}..."
    return None


def find_pdk_root():
    """The first candidate root with the pinned liberty, or None."""
    for root in candidate_roots():
        if _check(root) is None:
            return root
    return None


def resolve_pdk_root(explicit=None):
    """PDK root path; PdkNotFound (never a silent fallback) if none."""
    if explicit:
        if not os.path.isfile(os.path.join(explicit, SC_LIB_REL)):
            raise PdkNotFound(
                f"7t liberty not found at {os.path.join(explicit, SC_LIB_REL)}"
                f" (explicit PDK root {explicit})")
        return explicit
    tried = []
    for root in candidate_roots():
        why = _check(root)
        if why is None:
            return root
        tried.append(f"{root} ({why})")
    raise PdkNotFound(
        "no gf180mcuD PDK with the pinned 7t5v0 tt liberty "
        f"(expected sha256 {LIB_SHA256}); tried: " + "; ".join(tried)
        + ". Set PDK_ROOT to a tree containing gf180mcuD/ or pass "
        "--ciel-root/--pdk-root.")


def resolve_liberty(liberty=None, root=None):
    """(liberty_path, pdk_root_or_None) for a tool's --liberty/--ciel-root.

    An explicit ``liberty`` is used as given (must exist); the root is then
    the explicit/auto root when one resolves, else None.
    """
    if liberty:
        if not os.path.isfile(liberty):
            raise PdkNotFound(f"7t liberty not found at {liberty}")
        try:
            return liberty, resolve_pdk_root(root)
        except PdkNotFound:
            return liberty, None
    root = resolve_pdk_root(root)
    return os.path.join(root, SC_LIB_REL), root


def missing_reason():
    """Skip-reason text for tests when no PDK resolves (None if one does)."""
    try:
        resolve_pdk_root()
        return None
    except PdkNotFound as e:
        return str(e)
