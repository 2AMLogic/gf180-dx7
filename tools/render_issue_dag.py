#!/usr/bin/env python3
"""render_issue_dag.py — generate the README issue-DAG block from spec/issue-dag-v1.json.

Issue states come from GitHub (via `gh`) when available; without network the
tool renders state "UNKNOWN" and says so. This is a progress view of ISSUE
states only — it is not a verification verdict and never implies any
capability claim (plan section 7; torchsynth README discipline).

The emitted mermaid is deliberately maximally conservative so GitHub renders
it: `graph TD`, quoted labels with no `#` (mermaid reserves `#...;` for HTML
entities) and no other punctuation beyond parentheses, plain `-->` for
dependency edges, `-.->` for epic membership, and `:::`-shorthand classes
with named (hex-free) colors. A lint over the final block must pass before
anything is written; the tool exits 1 on any lint violation.

Usage:
  python3 tools/render_issue_dag.py            # rewrite README block
  python3 tools/render_issue_dag.py --check    # exit 1 if README block stale
"""
import json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SPEC = os.path.join(ROOT, "spec", "issue-dag-v1.json")
README = os.path.join(ROOT, "README.md")
BEGIN = "<!-- ISSUEDAG:BEGIN -->"
END = "<!-- ISSUEDAG:END -->"

EDGE_RE = re.compile(r"^\s*([A-Za-z0-9_]+)\s*(-\.->|-->|--)\s*([A-Za-z0-9_]+)\s*$")
NODE_RE = re.compile(r"^\s*([A-Za-z0-9_]+)\[")


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
        lines.append(f'  {eid}["{eid} epic (issue {e["issue"]})"]:::epic')
    for nid in sorted(nodes):
        n = nodes[nid]
        s = st(n["issue"])
        cls = "closed" if s == "CLOSED" else ("unknown" if s == "UNKNOWN" else "open")
        lines.append(f'  {nid}["{nid} (issue {n["issue"]}) {s}"]:::{cls}')
    for nid in sorted(nodes):
        n = nodes[nid]
        if n["epic"]:
            lines.append(f"  {n['epic']} -.-> {nid}")
        for d in n["deps"]:
            lines.append(f"  {d} --> {nid}")
    lines.append("  classDef epic fill:purple,color:white;")
    lines.append("  classDef closed fill:green,color:white;")
    lines.append("  classDef open fill:darkgray,color:black;")
    lines.append("  classDef unknown fill:orange,color:black;")
    lines.append("```")
    lines.append("")
    return "\n".join(lines)


def lint_markdown(text):
    """Lint the assembled README. Returns a list of violation strings (empty = pass)."""
    errors = []
    if BEGIN not in text or END not in text:
        return [f"block markers missing (need {BEGIN!r} before {END!r})"]
    pre, rest = text.split(BEGIN, 1)
    block, post = rest.split(END, 1)
    if BEGIN in block or END in block:
        errors.append("nested or duplicated ISSUEDAG markers inside block")
    if text.count(BEGIN) != 1 or text.count(END) != 1:
        errors.append("ISSUEDAG markers must appear exactly once each")

    openers = len(re.findall(r"^```mermaid\s*$", block, re.M))
    closers = len(re.findall(r"^```\s*$", block, re.M))
    if openers != 1 or closers != 1:
        errors.append(
            f"mermaid fence integrity: expected exactly one ```mermaid open and one"
            f" ``` close, found {openers} open / {closers} close")

    m = re.search(r"```mermaid\n(.*?)\n```", block, re.S)
    if m is None:
        errors.append("cannot extract mermaid diagram from block")
        return errors
    diagram = m.group(1)
    if "#" in diagram:
        errors.append("'#' character inside mermaid fence (reserved for HTML entities;"
                      " GitHub renders may fail)")
    declared, edges = set(), []
    for line in diagram.splitlines():
        nm = NODE_RE.match(line)
        if nm:
            declared.add(nm.group(1))
            continue
        em = EDGE_RE.match(line)
        if em:
            edges.append((em.group(1), em.group(3)))
    for src, dst in edges:
        for endpoint, role in ((src, "source"), (dst, "target")):
            if endpoint not in declared:
                errors.append(f"edge {src} --> {dst}: {role} node {endpoint!r} is never declared")
    return errors


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

    errors = lint_markdown(text)
    if errors:
        for e in errors:
            print(f"ISSUEDAG LINT FAIL: {e}", file=sys.stderr)
        sys.exit(1)

    if "--check" in sys.argv:
        current = open(README).read()
        if current == text:
            print("README issue-DAG block is fresh.")
            sys.exit(0)
        print("README issue-DAG block is STALE (regenerate with"
              " tools/render_issue_dag.py).", file=sys.stderr)
        sys.exit(1)
    with open(README, "w") as f:
        f.write(text)
    print(f"README issue-DAG block updated (states {'live' if states else 'unavailable'}).")


if __name__ == "__main__":
    main()
