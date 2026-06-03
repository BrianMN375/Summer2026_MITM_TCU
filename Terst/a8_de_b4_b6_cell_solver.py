#!/usr/bin/env python3

import pandas as pd

from collections import Counter

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 DE B4/B6 CELL SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

de_cells = Counter()
zero_cells = Counter()

de_rows = []
zero_rows = []

# ============================================================
# LOAD DATA
# ============================================================

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

    actual = actual_residual(frame)
    predicted = predicted_residual(frame)

    state = actual ^ predicted

    if state not in (0x00, 0xDE):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]

    #
    # MID FAMILY
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # MID RPM
    #

    if not (4000 <= rpm < 6200):
        continue

    cell = (b4, b6)

    if state == 0xDE:

        de_cells[cell] += 1

        de_rows.append({

            "cell": cell,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn

        })

    else:

        zero_cells[cell] += 1

        zero_rows.append({

            "cell": cell,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn

        })

print()
print(f"00 Frames : {len(zero_rows):,}")
print(f"DE Frames : {len(de_rows):,}")

# ============================================================
# TOP DE CELLS
# ============================================================

print()
print("=" * 80)
print("TOP DE CELLS")
print("=" * 80)

de_total = len(de_rows)

top_cells = []

coverage = 0

for (b4,b6),count in de_cells.most_common(30):

    pct = 100.0 * count / de_total

    coverage += count

    print(

        f"B4=0x{b4:02X} "
        f"B6=0x{b6:02X} "
        f"Count={count:3d} "
        f"({pct:5.2f}%)"

    )

    top_cells.append((b4,b6))

print()
print(
    f"Top30 Coverage: "
    f"{coverage}/{de_total} "
    f"({100.0*coverage/de_total:.2f}%)"
)

# ============================================================
# CELL PURITY
# ============================================================

print()
print("=" * 80)
print("TOP CELL PURITY")
print("=" * 80)

for cell in top_cells[:20]:

    de_count = de_cells[cell]
    zero_count = zero_cells[cell]

    total = de_count + zero_count

    purity = (

        100.0 *
        de_count /
        total

    ) if total else 0

    b4,b6 = cell

    print(

        f"B4=0x{b4:02X} "
        f"B6=0x{b6:02X} "

        f"DE={de_count:3d} "
        f"00={zero_count:3d} "

        f"Purity={purity:6.2f}%"

    )

# ============================================================
# TOP B4 GROUPS
# ============================================================

print()
print("=" * 80)
print("DE BY B4")
print("=" * 80)

b4_counter = Counter()

for (b4,b6),count in de_cells.items():

    b4_counter[b4] += count

for b4,count in b4_counter.most_common(20):

    print(

        f"0x{b4:02X} "
        f"{count}"

    )

# ============================================================
# TOP B6 GROUPS
# ============================================================

print()
print("=" * 80)
print("DE BY B6")
print("=" * 80)

b6_counter = Counter()

for (b4,b6),count in de_cells.items():

    b6_counter[b6] += count

for b6,count in b6_counter.most_common(20):

    print(

        f"0x{b6:02X} "
        f"{count}"

    )

# ============================================================
# SIMPLE CELL SOLVER
# ============================================================

print()
print("=" * 80)
print("TOP-N CELL SOLVER")
print("=" * 80)

best_acc = 0
best_n = 0

rows = []

for row in de_rows:

    rows.append((1,row["cell"]))

for row in zero_rows:

    rows.append((0,row["cell"]))

for n in range(1,31):

    allowed = set(top_cells[:n])

    correct = 0

    for state,cell in rows:

        pred = 1 if cell in allowed else 0

        if pred == state:
            correct += 1

    acc = correct / len(rows)

    if acc > best_acc:

        best_acc = acc
        best_n = n

print()

print(
    f"Best N = {best_n}"
)

print(
    f"Accuracy = "
    f"{best_acc*100:.4f}%"
)

print()
print("Done.")