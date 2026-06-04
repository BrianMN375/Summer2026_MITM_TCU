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
print("A8 84 VS 5A BOUNDARY SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

rows = []

# ============================================================
# BUILD DATASET
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

    residual = actual ^ predicted

    if residual not in (0x84, 0x5A):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    #
    # Mid-family only
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # Overlay-active region
    #

    primary = (

        b4 <= 0x92
        and
        b6 <= 0x9C
        and
        rpm > 4043.5
        and
        stat <= 252.5

    )

    secondary = (

        b4 <= 0x92
        and
        b6 > 0x9C
        and
        stat > 246.0
        and
        counter <= 12

    )

    if not (primary or secondary):
        continue

    rows.append({

        "state": residual,
        "b6": b6,
        "b7": b7

    })

print()
print(f"Frames : {len(rows)}")

# ============================================================
# B7 SEARCH
# ============================================================

best_acc = 0
best_b7 = None

for thresh in range(0x40, 0x60):

    correct = 0

    for r in rows:

        pred = 0x84 if r["b7"] <= thresh else 0x5A

        if pred == r["state"]:
            correct += 1

    acc = correct / len(rows)

    if acc > best_acc:

        best_acc = acc
        best_b7 = thresh

print()
print("=" * 80)
print("BEST B7 THRESHOLD")
print("=" * 80)

print(f"B7 <= 0x{best_b7:02X}")
print(f"Accuracy = {100.0*best_acc:.4f}%")

# ============================================================
# B7+B6 SEARCH
# ============================================================

best_acc2 = 0
best_b7_2 = None
best_b6_2 = None

for b7_thresh in range(0x40, 0x60):

    for b6_thresh in range(100,256):

        correct = 0

        for r in rows:

            if (
                r["b7"] <= b7_thresh
                and
                r["b6"] <= b6_thresh
            ):
                pred = 0x84
            else:
                pred = 0x5A

            if pred == r["state"]:
                correct += 1

        acc = correct / len(rows)

        if acc > best_acc2:

            best_acc2 = acc

            best_b7_2 = b7_thresh
            best_b6_2 = b6_thresh

print()
print("=" * 80)
print("BEST B7+B6 THRESHOLD")
print("=" * 80)

print(f"B7 <= 0x{best_b7_2:02X}")
print(f"B6 <= {best_b6_2}")

print(f"Accuracy = {100.0*best_acc2:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

matrix = {

    0x84 : Counter(),
    0x5A : Counter()

}

for r in rows:

    if (
        r["b7"] <= best_b7_2
        and
        r["b6"] <= best_b6_2
    ):
        pred = 0x84
    else:
        pred = 0x5A

    matrix[r["state"]][pred] += 1

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in (0x84,0x5A):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x84,0x5A):

        print(
            f"  Pred 0x{pred:02X}"
            f" : "
            f"{matrix[actual][pred]}"
        )

print()
print("Done.")