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
print("A8 DE B7/B4 HEATMAP")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# BUILD COUNTS
# ============================================================

de_cells = defaultdict(int)
normal_cells = defaultdict(int)

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
    else:
        normal_cells[key] += 1

# ============================================================
# TOP DE CELLS
# ============================================================

print()
print("=" * 80)
print("TOP DE CELLS")
print("=" * 80)

ranking = []

for key,count in de_cells.items():

    b7,b4 = key

    normal = normal_cells[key]

    purity = count / (count + normal)

    ranking.append(

        (

            count,
            purity,
            b7,
            b4,
            normal

        )

    )

ranking.sort(reverse=True)

for count,purity,b7,b4,normal in ranking[:50]:

    print(

        f"B7=0x{b7:02X} "
        f"B4=0x{b4:02X} "
        f"DE={count:4d} "
        f"00={normal:4d} "
        f"Purity={purity*100:6.2f}%"

    )

# ============================================================
# PURE DE CELLS
# ============================================================

print()
print("=" * 80)
print("PURE DE CELLS")
print("=" * 80)

pure = []

for key,count in de_cells.items():

    b7,b4 = key

    if normal_cells[key] == 0:

        pure.append(

            (

                count,
                b7,
                b4

            )

        )

pure.sort(reverse=True)

for count,b7,b4 in pure[:50]:

    print(

        f"B7=0x{b7:02X} "
        f"B4=0x{b4:02X} "
        f"DE={count}"

    )

# ============================================================
# COMPACT HEATMAP
# ============================================================

print()
print("=" * 80)
print("DE HEATMAP")
print("=" * 80)

b7_values = sorted(

    {

        key[0]
        for key in de_cells.keys()

    }

)

b4_values = sorted(

    {

        key[1]
        for key in de_cells.keys()

    }

)

# Focus on populated area

b7_values = [

    x for x in b7_values
    if 0x3E <= x <= 0x45

]

b4_values = [

    x for x in b4_values
    if 0x80 <= x <= 0xA5

]

header = "B7\\B4 "

for b4 in b4_values:

    header += f"{b4:02X} "

print(header)

for b7 in b7_values:

    line = f"{b7:02X}    "

    for b4 in b4_values:

        count = de_cells[(b7,b4)]

        if count == 0:
            line += ".. "

        else:
            line += f"{count:02d} "

    print(line)

# ============================================================
# DENSITY SUMMARY
# ============================================================

print()
print("=" * 80)
print("B4 DISTRIBUTION INSIDE DE")
print("=" * 80)

b4_hist = defaultdict(int)

for (b7,b4),count in de_cells.items():

    b4_hist[b4] += count

for b4,count in sorted(

    b4_hist.items(),
    key=lambda x:x[1],
    reverse=True

)[:25]:

    print(

        f"0x{b4:02X} : {count}"

    )

print()
print("Done.")