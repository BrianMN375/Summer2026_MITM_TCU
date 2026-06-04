#!/usr/bin/env python3

from collections import defaultdict, deque

# ============================================================
# OBSERVED RESIDUALS
# ============================================================

RESIDUALS = sorted({

    0x5E,
    0x25,

    0x8D,
    0xF6,

    0xC7,
    0xBC,

    0x1E,
    0xC0,
    0x19,

    0x56,
    0x1C,

    0x38,
    0x62,

    0x43,
    0x5D,

    0x84,
    0x07,

    0x72,
    0x4D
})

# ============================================================
# DISCOVERED VECTORS
# ============================================================

VECTORS = {

    0x7B : "NEG_TORQUE",

    0xD3 : "RPM_LOW_MID",
    0x4A : "RPM_MID_HIGH",

    0xDE : "OVL_DE",
    0x91 : "OVL_91",
    0xDB : "OVL_DB",
    0x5A : "OVL_5A",

    0x84 : "MODE_84",
    0xC0 : "MODE_C0",
    0x43 : "MODE_43",
}

# ============================================================
# BUILD GRAPH
# ============================================================

graph = defaultdict(list)

for a in RESIDUALS:

    for b in RESIDUALS:

        if a >= b:
            continue

        delta = a ^ b

        if delta in VECTORS:

            graph[a].append(
                (b, delta)
            )

            graph[b].append(
                (a, delta)
            )

# ============================================================
# EDGE LIST
# ============================================================

print()
print("=" * 80)
print("A8 VECTOR GRAPH")
print("=" * 80)

print()
print("Edges")
print("-" * 80)

printed = set()

for node in sorted(graph):

    for nbr, delta in graph[node]:

        key = tuple(
            sorted([node, nbr])
        )

        if key in printed:
            continue

        printed.add(key)

        print(
            f"0x{node:02X}"
            f" --[{VECTORS[delta]} / 0x{delta:02X}]--> "
            f"0x{nbr:02X}"
        )

# ============================================================
# CONNECTED COMPONENTS
# ============================================================

print()
print("=" * 80)
print("CONNECTED COMPONENTS")
print("=" * 80)

visited = set()

component_id = 0

for start in sorted(RESIDUALS):

    if start in visited:
        continue

    component_id += 1

    queue = deque([start])

    component = []

    while queue:

        node = queue.popleft()

        if node in visited:
            continue

        visited.add(node)

        component.append(node)

        for nbr, _ in graph[node]:

            if nbr not in visited:
                queue.append(nbr)

    component.sort()

    print()
    print(
        f"Component {component_id}"
    )

    print(
        "  "
        + ", ".join(
            f"0x{x:02X}"
            for x in component
        )
    )

# ============================================================
# PATH SEARCH
# ============================================================

def bfs_path(start, goal):

    queue = deque()

    queue.append(
        (start, [])
    )

    visited = {start}

    while queue:

        node, path = queue.popleft()

        if node == goal:
            return path

        for nbr, delta in graph[node]:

            if nbr in visited:
                continue

            visited.add(nbr)

            queue.append(

                (
                    nbr,
                    path + [
                        (
                            nbr,
                            delta
                        )
                    ]
                )
            )

    return None

# ============================================================
# KNOWN FAMILIES
# ============================================================

print()
print("=" * 80)
print("FAMILY PATHS")
print("=" * 80)

families = [

    (0x5E, 0xC7),
    (0x25, 0xBC),

    (0xC7, 0x19),
    (0xC7, 0x56),
    (0xC7, 0x1C),

    (0x1E, 0xC0),
]

for start, goal in families:

    path = bfs_path(
        start,
        goal
    )

    print()

    print(
        f"0x{start:02X}"
        f" -> "
        f"0x{goal:02X}"
    )

    if path is None:

        print(
            "  NO PATH"
        )

        continue

    current = start

    for nxt, delta in path:

        print(
            f"  0x{current:02X}"
            f" --{VECTORS[delta]}--> "
            f"0x{nxt:02X}"
        )

        current = nxt

print()
print("Done.")