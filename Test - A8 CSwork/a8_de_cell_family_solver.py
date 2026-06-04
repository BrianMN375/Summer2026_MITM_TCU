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
print("A8 DE CELL FAMILY SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# BUILD CELL TABLE
# ============================================================

de_cells = defaultdict(int)
normal_cells = defaultdict(int)

total_de = 0

for row in df.itertuples():

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

    b4 = frame[4]
    b7 = frame[7]

    key = (b7,b4)

    if residual == 0xDE:

        de_cells[key] += 1
        total_de += 1

    else:

        normal_cells[key] += 1

# ============================================================
# RANK CELLS
# ============================================================

ranking = []

for key,de_count in de_cells.items():

    normal_count = normal_cells[key]

    purity = de_count / (de_count + normal_count)

    ranking.append(

        (
            de_count,
            purity,
            key,
            normal_count
        )

    )

ranking.sort(reverse=True)

print()
print("=" * 80)
print("TOP DE CELLS")
print("=" * 80)

for de_count,purity,key,normal_count in ranking[:50]:

    b7,b4 = key

    print(

        f"B7=0x{b7:02X} "
        f"B4=0x{b4:02X} "
        f"DE={de_count:4d} "
        f"00={normal_count:4d} "
        f"Purity={purity*100:6.2f}%"

    )

# ============================================================
# COVERAGE TABLE
# ============================================================

print()
print("=" * 80)
print("COVERAGE BY TOP-N CELLS")
print("=" * 80)

running = 0

for N in [1,2,3,5,10,15,20,25,30]:

    running = sum(

        ranking[i][0]
        for i in range(
            min(N,len(ranking))
        )

    )

    coverage = 100.0 * running / total_de

    print(

        f"Top {N:2d} Cells : "
        f"{running:4d}/{total_de} "
        f"({coverage:6.2f}%)"

    )

# ============================================================
# PURE CELL COVERAGE

# ============================================================

pure_de = 0
pure_cells = 0

for de_count,purity,key,normal_count in ranking:

    if normal_count == 0:

        pure_cells += 1
        pure_de += de_count

print()
print("=" * 80)
print("PURE CELL COVERAGE")
print("=" * 80)

print(f"Pure Cells : {pure_cells}")
print(f"DE Frames  : {pure_de}/{total_de}")

print(
    f"Coverage   : "
    f"{100.0*pure_de/total_de:.2f}%"
)

# ============================================================
# FAMILY SOLVER
# ============================================================

print()
print("=" * 80)
print("BEST CELL FAMILY")
print("=" * 80)

family = []

running = 0

for de_count,purity,key,normal_count in ranking:

    if purity < 0.90:
        continue

    family.append(key)

    running += de_count

coverage = 100.0 * running / total_de

print()

for b7,b4 in family:

    print(

        f"(0x{b7:02X},0x{b4:02X})"

    )

print()
print(

    f"Coverage = "
    f"{running}/{total_de} "
    f"({coverage:.2f}%)"

)

print()
print("Done.")