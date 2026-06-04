#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

try:
    import networkx as nx
    HAVE_NX = True
except:
    HAVE_NX = False

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

# ----------------------------------------------------------
# Helpers
# ----------------------------------------------------------

def node_name(node):
    b7, b4 = node
    return f"{b7:02X}{b4:02X}"

# ----------------------------------------------------------
# Load DE population
# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 DE STATE GRAPH BUILDER")
print("=" * 80)

de_rows = []

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    header = next(reader, None)

    for idx, row in enumerate(reader):

        if len(row) < 9:
            continue

        try:

            frame = [

                int(row[1],16),  # b0
                int(row[2],16),  # b1
                int(row[3],16),  # b2
                int(row[4],16),  # b3
                int(row[5],16),  # b4
                int(row[6],16),  # b5
                int(row[7],16),  # b6
                int(row[8],16)   # b7

            ]

        except:
            continue

        residual = (
            actual_residual(frame)
            ^
            predicted_residual(frame)
        )

        if residual != 0xDE:
            continue

        de_rows.append(
            {
                "idx": idx,
                "b4": frame[4],
                "b6": frame[6],
                "b7": frame[7],
                "counter": frame[1] & 0x0F
            }
        )

print()
print(f"DE Frames : {len(de_rows)}")

# ----------------------------------------------------------
# Build graph
# ----------------------------------------------------------

state_counts = Counter()

transitions = Counter()

graph = defaultdict(set)
reverse_graph = defaultdict(set)

states = []

for row in de_rows:

    node = (
        row["b7"],
        row["b4"]
    )

    states.append(node)

    state_counts[node] += 1

for src, dst in zip(states, states[1:]):

    transitions[(src,dst)] += 1

    graph[src].add(dst)
    reverse_graph[dst].add(src)

all_nodes = sorted(
    set(state_counts.keys()) |
    set(graph.keys()) |
    set(reverse_graph.keys())
)

# ----------------------------------------------------------
# State Occupancy
# ----------------------------------------------------------

print()
print("=" * 80)
print("STATE OCCUPANCY")
print("=" * 80)

for node,count in state_counts.most_common():

    print(
        f"{node_name(node):8s} "
        f"{count:4d}"
    )

# ----------------------------------------------------------
# Top Transitions
# ----------------------------------------------------------

print()
print("=" * 80)
print("TOP TRANSITIONS")
print("=" * 80)

for (src,dst),count in transitions.most_common(100):

    print(
        f"{node_name(src):8s} -> "
        f"{node_name(dst):8s} : "
        f"{count}"
    )

# ----------------------------------------------------------
# Connectivity
# ----------------------------------------------------------

print()
print("=" * 80)
print("CELL CONNECTIVITY")
print("=" * 80)

for node in all_nodes:

    indeg = len(reverse_graph[node])
    outdeg = len(graph[node])

    print(
        f"{node_name(node):8s} "
        f"IN={indeg:3d} "
        f"OUT={outdeg:3d}"
    )

# ----------------------------------------------------------
# Entry States
# ----------------------------------------------------------

print()
print("=" * 80)
print("ENTRY STATES")
print("=" * 80)

for node in all_nodes:

    if len(reverse_graph[node]) == 0:

        print(node_name(node))

# ----------------------------------------------------------
# Terminal States
# ----------------------------------------------------------

print()
print("=" * 80)
print("TERMINAL STATES")
print("=" * 80)

for node in all_nodes:

    if len(graph[node]) == 0:

        print(node_name(node))

# ----------------------------------------------------------
# Dominant Next State
# ----------------------------------------------------------

print()
print("=" * 80)
print("DOMINANT NEXT STATE")
print("=" * 80)

for node in all_nodes:

    outgoing = []

    for (src,dst),count in transitions.items():

        if src == node:

            outgoing.append(
                (count,dst)
            )

    if not outgoing:
        continue

    outgoing.sort(reverse=True)

    best_count,best_dst = outgoing[0]

    print(
        f"{node_name(node):8s} -> "
        f"{node_name(best_dst):8s} "
        f"({best_count})"
    )

# ----------------------------------------------------------
# Linear Chains
# ----------------------------------------------------------

print()
print("=" * 80)
print("LINEAR CHAINS")
print("=" * 80)

visited = set()

for node in all_nodes:

    indeg = len(reverse_graph[node])

    if indeg != 1:

        chain = [node]

        current = node

        while True:

            if len(graph[current]) != 1:
                break

            nxt = next(iter(graph[current]))

            if len(reverse_graph[nxt]) != 1:
                break

            if nxt in chain:
                break

            chain.append(nxt)

            current = nxt

        if len(chain) >= 3:

            print()

            for i,n in enumerate(chain):

                if i:
                    print("  ↓")

                print(node_name(n))

# ----------------------------------------------------------
# SCC Analysis
# ----------------------------------------------------------

if HAVE_NX:

    print()
    print("=" * 80)
    print("STRONGLY CONNECTED COMPONENTS")
    print("=" * 80)

    G = nx.DiGraph()

    for (src,dst),count in transitions.items():

        G.add_edge(
            src,
            dst,
            weight=count
        )

    sccs = list(
        nx.strongly_connected_components(G)
    )

    found = False

    for scc in sccs:

        if len(scc) <= 1:
            continue

        found = True

        print()

        print(
            ", ".join(
                sorted(
                    node_name(x)
                    for x in scc
                )
            )
        )

    if not found:

        print("No SCCs > 1 node")

# ----------------------------------------------------------
# GraphViz Export
# ----------------------------------------------------------

dot_file = "a8_de_state_graph.dot"

with open(dot_file,"w") as f:

    f.write("digraph DE {\n\n")

    for node,count in state_counts.items():

        f.write(
            f"\"{node_name(node)}\" "
            f"[label=\"{node_name(node)}\\n{count}\"];\n"
        )

    f.write("\n")

    for (src,dst),count in transitions.items():

        f.write(
            f"\"{node_name(src)}\" -> "
            f"\"{node_name(dst)}\" "
            f"[label=\"{count}\"];\n"
        )

    f.write("\n}\n")

print()
print("=" * 80)
print("GRAPHVIZ")
print("=" * 80)

print(f"Wrote {dot_file}")

print()
print("Render with:")
print(
    "dot -Tpng a8_de_state_graph.dot "
    "-o a8_de_state_graph.png"
)

print()
print("Done.")