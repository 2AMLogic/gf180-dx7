"""Pinned external-oracle discovery and dry PCM rendering for model tests."""

import array
import os
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from render_reference import (
    DEFAULT_IDENTITY,
    RENDER_RATE,
    HarnessFail,
    HarnessNotRun,
    check_oracle_binary,
    load_identity,
    sha256_bytes,
)


def probe_oracle(
    identity_path: Path = DEFAULT_IDENTITY, oracle_root_env: str = "DX7_ORACLE_ROOT"
) -> tuple[Path | None, str]:
    """Locate and verify the pinned binary, or give the unittest skip reason."""
    try:
        identity = load_identity(identity_path)
        root = os.environ.get(oracle_root_env)
        path = (
            Path(root) / "dx7-oracle"
            if root is not None
            else Path(identity["binary"]["path"])
        )
        check_oracle_binary(identity, path)
    except (HarnessFail, HarnessNotRun) as exc:
        return None, f"NOT_RUN: {exc}"
    return path, ""


class OracleRenderer:
    """Render each distinct (voice, events, duration) once per instance."""

    def __init__(
        self, oracle_path: Path, prefix: str = "dx7-oracle-", rate: int = RENDER_RATE
    ):
        self._oracle = oracle_path.resolve()
        self._rate = rate
        self._tmp = tempfile.TemporaryDirectory(prefix=prefix)
        self._cache = {}
        self.runs = []

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()

    def close(self):
        self._tmp.cleanup()

    def render_bytes(
        self, voice_bytes: bytes, events_text: str, seconds: float
    ) -> bytes:
        events_bytes = events_text.encode("utf-8")
        voice_hash, events_hash = sha256_bytes(voice_bytes), sha256_bytes(events_bytes)
        key = (voice_hash, events_hash, seconds)
        if key in self._cache:
            return self._cache[key]

        work = Path(self._tmp.name)
        voice, events, out = work / "voice.syx", work / "events.txt", work / "out.f32"
        voice.write_bytes(voice_bytes)
        events.write_bytes(events_bytes)
        out.unlink(missing_ok=True)  # A successful exit must produce fresh PCM.
        proc = subprocess.run(
            [
                self._oracle,
                "--voice",
                voice,
                "--events",
                events,
                "--out",
                out,
                "--rate",
                str(self._rate),
                "--seconds",
                str(seconds),
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        if proc.returncode:
            raise AssertionError(
                f"oracle failed (exit {proc.returncode}): {proc.stderr.strip()}"
            )
        if not out.is_file():
            raise AssertionError("oracle produced no output")
        pcm = out.read_bytes()
        expected = 4 * int(self._rate * seconds)
        if len(pcm) != expected:
            raise AssertionError(
                f"oracle produced {len(pcm)} bytes, expected {expected}"
            )

        self.runs.append(
            {
                "voice_sha256": voice_hash,
                "events_sha256": events_hash,
                "seconds": seconds,
                "pcm_sha256": sha256_bytes(pcm),
            }
        )
        self._cache[key] = pcm
        return pcm

    def render_f32(
        self, voice_bytes: bytes, events_text: str, seconds: float
    ) -> array.array:
        samples = array.array("f", self.render_bytes(voice_bytes, events_text, seconds))
        if sys.byteorder == "big":
            samples.byteswap()
        return samples
