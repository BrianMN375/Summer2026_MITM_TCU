#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 B4 STATE BAND SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

rows = []

# ============================================================
# BUILD POPULATIONS
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

    residual = actual_residual(frame) ^ predicted_residual(frame)

    b4 = frame[4]
    b7 = frame[7]

    rpm  = ((frame[2] << 8) | frame[1]) / 4.0
    stat = frame[6] + frame[7] * 2

    #
    # DE
    #

    if residual == 0xDE:

        state = "DE"

    #
    # LOW84
    #

    elif residual == 0x84 and stat <= 284:

        state = "LOW84"

    #
    # HIGH84
    #

    elif residual == 0x84 and stat > 284:

        state = "HIGH84"

    #
    # 5A
    #

    elif residual == 0x5A:

        state = "5A"

    else:
        continue

    rows.append({

        "state": state,
        "b4": b4

    })

print()
print(f"Frames : {len(rows):,}")

# ============================================================
# STATE STATS
# ============================================================

print()
print("=" * 80)
print("B4 DISTRIBUTIONS")
print("=" * 80)

states = ["DE","LOW84","HIGH84","5A"]

for state in states:

    vals = [r["b4"] for r in rows if r["state"] == state]

    print()
    print(state)

    print(
        f"Count={len(vals)} "
        f"Min=0x{min(vals):02X} "
        f"Max=0x{max(vals):02X} "
        f"Avg={sum(vals)/len(vals):.2f}"
    )

# ============================================================
# SEARCH BANDARIES
# ============================================================

best_acc = -1
best = None

for de_max in range(0x70,0xC0):

    for low84_max in range(de_max+1,0xC0):

        for high84_max in range(low84_max+1,0xC0):

            correct = 0

            for r in rows:

                b4 = r["b4"]

                if b4 <= de_max:
                    pred = "DE"

                elif b4 <= low84_max:
                    pred = "LOW84"

                elif b4 <= high84_max:
                    pred = "HIGH84"

                else:
                    pred = "5A"

                if pred == r["state"]:
                    correct += 1

            acc = correct / len(rows)

            if acc > best_acc:

                best_acc = acc
                best = (
                    de_max,
                    low84_max,
                    high84_max
                )

# ============================================================
# RESULTS
# ============================================================

de_max, low84_max, high84_max = best

print()
print("=" * 80)
print("BEST B4 STATE LADDER")
print("=" * 80)

print(
    f"DE      <= 0x{de_max:02X}"
)

print(
    f"LOW84   <= 0x{low84_max:02X}"
)

print(
    f"HIGH84  <= 0x{high84_max:02X}"
)

print(
    f"5A      >  0x{high84_max:02X}"
)

print()
print(
    f"Accuracy = {best_acc*100:.4f}%"
)

print()
print("Done.")