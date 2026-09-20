# Reuse records

One JSON record per adopted component, created BEFORE the import lands
(issue #39). Schema:

```json
{
  "component": "name",
  "upstream": {"repo": "2AMLogic/x", "commit": "sha", "path": ["a", "b"]},
  "sha256": {"a": "…"},
  "license": "Apache-2.0",
  "destination": "rtl/… or src/…",
  "adaptations": ["…"],
  "maintainer": "name",
  "requalification": {"check": "command", "result": "PASS"},
  "negative_control": "command that must fail when the component breaks"
}
```

`tools/check_reuse.py` fails when a file under a governed destination has no
matching record (negative control for A01).
