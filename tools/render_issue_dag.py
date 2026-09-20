#!/usr/bin/env python3
"""render_issue_dag.py — generate the README issue-DAG block from spec/issue-dag-v1.json.

Issue states come from GitHub (via `gh`) when available; without network the
tool renders state "UNKNOWN" and says so. This is a progress view of ISSUE
states only — it is not a verification verdict and never implies any
capability claim (plan section 7; torchsynth README discipline).

Usage:
  python3 tools/render_issue_dag.py            # rewrite README block
  python3 tools/render_issue_dag.py --check    # exit 1 if README block stale
"""
import json, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC = os.path.join(ROOT, "spec", "issue-dag-v1.json")
README = os.path.join(ROOT, "README.md")
BEGIN = "<!-- ISSUEDAG:BEGIN -->"
END = "<!-- ISSUEDAG:END -->"


def fetch_states():
    """Return {issue_number: "OPEN"|"CLOSED"} or None when gh/network unavailable."""
    try:
        out = subprocess.run(
            ["gh", "issue", "list", "--repo", "2AMLogic/gf180-dx7",
             "--state", "all", "--limit", "200", "--json", "number,state"],
            capture_output=True, text=True, timeout=30)
        if out.returncode != 0:
            return None
        return {i["number"]: i["state"] for i in json.loads(out.stdout)}
    except Exception:
        return None


def render(spec, states):
    nodes = spec["nodes"]
    known = states is not None
    def st(issue):
        return states.get(issue, "UNKNOWN") if known else "UNKNOWN"

    counts = {"CLOSED": 0, "OPEN": 0}
    for n in nodes.values():
        s = st(n["issue"])
        if s in counts:
            counts[s] += 1

    lines = []
    lines.append("## Backlog progress (issue DAG)")
    lines.append("")
    lines.append("Generated from [spec/issue-dag-v1.json](spec/issue-dag-v1.json) by"
                 " `tools/render_issue_dag.py` — do not hand-edit.")
    lines.append("")
    closed, open_, unknown = counts["CLOSED"], counts["OPEN"], len(nodes) - counts["CLOSED"] - counts["OPEN"]
    lines.append("| CLOSED | OPEN | UNKNOWN | TOTAL |")
    lines.append("| ---: | ---: | ---: | ---: |")
    lines.append(f"| {closed} | {open_} | {unknown} | {len(nodes)} |")
    lines.append("")
    lines.append("These are issue states, not verification verdicts: a closed issue"
                 " is not a capability claim, and no numeric PASS/coverage status is"
                 " shown here. Evidence-derived capability status arrives with the"
                 " fixed model and comparator (plan section 7).")
    lines.append("")
    lines.append("```mermaid")
    lines.append("graph TD")
    for eid, e in spec["epics"].items():
        lines.append(f'  {eid}["{eid}: epic #{e["issue"]}"]:::epic')
    for nid in sorted(nodes):
        n = nodes[nid]
        cls = "closed" if st(n["issue"]) == "CLOSED" else "open"
        lines.append(f'  {nid}["{nid} #{n["issue"]}"]::{cls}')
    for nid in sorted(nodes):
        n = nodes[nid]
        if n["epic"]:
            lines.append(f"  {n['epic']} -.-> {nid}")
        for d in n["deps"]:
            lines.append(f"  {d} --> {nid}")
    lines.append("  classDef epic fill:#7C3AED,color:#fff;")
    lines.append("  classDef closed fill:#0E8A16,color:#fff;")
    lines.append("  classDef open fill:#9CA3AF,color:#000;")
    lines.append("```")
    lines.append("")
    return "\n".join(lines)


def main():
    spec = json.load(open(SPEC))
    states = fetch_states()
    block = render(spec, states)
    text = open(README).read()
    if BEGIN not in text or END not in text:
        text = text.rstrip("\n") + "\n\n" + BEGIN + "\n" + block + "\n" + END + "\n"
    else:
        pre = text.split(BEGIN)[0]
        post = text.split(END)[1]
        text = pre + BEGIN + "\n" + block + "\n" + END + post
    if "--check" in sys.argv:
        current = open(README).read()
        sys.exit(0 if current == text else 1)
    with open(README, "w") as f:
        f.write(text)
    print(f"README issue-DAG block updated (states {'live' if states else 'unavailable'}).")


if __name__ == "__main__":
    main()
