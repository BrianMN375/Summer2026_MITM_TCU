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
print("A8 B7 VS RESIDUAL MATRIX")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# BUILD MATRIX
# ============================================================

matrix = defaultdict(lambda: defaultdict(int))

b7_totals = defaultdict(int)
residual_totals = defaultdict(int)

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

    b7 = frame[7]

    matrix[b7][residual] += 1

    b7_totals[b7] += 1
    residual_totals[residual] += 1

# ============================================================
# RESIDUAL LIST
# ============================================================

residuals = sorted(residual_totals.keys())

print()
print("=" * 80)
print("B7 x RESIDUAL MATRIX")
print("=" * 80)

header = "B7     TOTAL "

for r in residuals:

    header += f" 0x{r:02X}".rjust(8)

print(header)

print("-" * len(header))

for b7 in sorted(matrix.keys()):

    total = b7_totals[b7]

    line = f"0x{b7:02X} {total:7d}"

    for r in residuals:

        line += f"{matrix[b7][r]:8d}"

    print(line)

# ============================================================
# DOMINANT RESIDUAL
# ============================================================

print()
print("=" * 80)
print("DOMINANT RESIDUAL PER B7")
print("=" * 80)

for b7 in sorted(matrix.keys()):

    total = b7_totals[b7]

    dominant_residual = None
    dominant_count = 0

    for r,count in matrix[b7].items():

        if count > dominant_count:

            dominant_count = count
            dominant_residual = r

    pct = 100.0 * dominant_count / total

    print(

        f"B7=0x{b7:02X} "
        f"TOTAL={total:5d} "
        f"DOM=0x{dominant_residual:02X} "
        f"COUNT={dominant_count:5d} "
        f"PCT={pct:6.2f}%"

    )

# ============================================================
# RESIDUAL BY B7 RANGE
# ============================================================

ranges = [

    ("DE_ZONE",    0x3E, 0x45),
    ("LOW84_ZONE", 0x46, 0x4B),
    ("HIGH84_ZONE",0x4C, 0x50),
    ("5A_ZONE",    0x51, 0x5F)

]

print()
print("=" * 80)
print("ZONE BREAKDOWN")
print("=" * 80)

for name,start,end in ranges:

    counts = defaultdict(int)
    total = 0

    for b7 in range(start,end+1):

        if b7 not in matrix:
            continue

        for residual,count in matrix[b7].items():

            counts[residual] += count
            total += count

    print()
    print(name)
    print("-" * 40)

    for residual in sorted(counts.keys()):

        pct = 100.0 * counts[residual] / total

        print(

            f"0x{residual:02X} "
            f"{counts[residual]:6d} "
            f"({pct:6.2f}%)"

        )

print()
print("Done.")