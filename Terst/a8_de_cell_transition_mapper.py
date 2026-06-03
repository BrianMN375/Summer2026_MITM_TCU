#!/usr/bin/env python3

import pandas as pd
from collections import defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 DE CELL TRANSITION MAPPER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# BUILD DE CELL STREAM
# ============================================================

de_stream = []

for idx,row in enumerate(df.itertuples()):

    frame = [

        int(row.b0,16),
        int(row.b1,16),
        int(row.b2,16),
        int(row.b3,16),
        int(row.b4,16),
        int(row.b5,16),
        int(row.b6,16),
        int(row.b7,16)

    ]

    residual = (
        actual_residual(frame)
        ^
        predicted_residual(frame)
    )

    if residual != 0xDE:
        continue

    b4 = frame[4]
    b7 = frame[7]

    de_stream.append(
        (
            idx,
            b7,
            b4
        )
    )

print()
print(f"DE Frames : {len(de_stream)}")

# ============================================================
# TRANSITIONS
# ============================================================

transitions = defaultdict(int)

for i in range(len(de_stream)-1):

    idx1,b71,b41 = de_stream[i]
    idx2,b72,b42 = de_stream[i+1]

    # Only connect nearby frames
    if idx2 - idx1 > 5:
        continue

    src = (b71,b41)
    dst = (b72,b42)

    transitions[(src,dst)] += 1

# ============================================================
# TOP TRANSITIONS
# ============================================================

print()
print("=" * 80)
print("TOP TRANSITIONS")
print("=" * 80)

ranking = []

for pair,count in transitions.items():

    src,dst = pair

    ranking.append(

        (
            count,
            src,
            dst
        )

    )

ranking.sort(reverse=True)

for count,src,dst in ranking[:50]:

    print(

        f"(0x{src[0]:02X},0x{src[1]:02X}) "
        f"-> "
        f"(0x{dst[0]:02X},0x{dst[1]:02X}) "
        f": {count}"

    )

# ============================================================
# CELL CONNECTIVITY
# ============================================================

print()
print("=" * 80)
print("CELL CONNECTIVITY")
print("=" * 80)

outgoing = defaultdict(int)
incoming = defaultdict(int)

for (src,dst),count in transitions.items():

    outgoing[src] += count
    incoming[dst] += count

cells = set()

for src,dst in transitions.keys():

    cells.add(src)
    cells.add(dst)

for cell in sorted(cells):

    print(

        f"(0x{cell[0]:02X},0x{cell[1]:02X}) "
        f"OUT={outgoing[cell]:3d} "
        f"IN={incoming[cell]:3d}"

    )

# ============================================================
# DOMINANT NEXT CELL
# ============================================================

print()
print("=" * 80)
print("DOMINANT NEXT CELL")
print("=" * 80)

next_map = defaultdict(lambda: defaultdict(int))

for (src,dst),count in transitions.items():

    next_map[src][dst] += count

for src in sorted(next_map.keys()):

    best_dst = None
    best_count = 0

    for dst,count in next_map[src].items():

        if count > best_count:

            best_count = count
            best_dst = dst

    print(

        f"(0x{src[0]:02X},0x{src[1]:02X}) "
        f"-> "
        f"(0x{best_dst[0]:02X},0x{best_dst[1]:02X}) "
        f"({best_count})"

    )

print()
print("Done.")