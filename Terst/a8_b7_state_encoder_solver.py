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
print("A8 B7 STATE ENCODER SOLVER")
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

    if residual not in (0xDE, 0x84, 0x5A):
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
    # Overlay activation region
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

    #
    # Split 84 into LOW/HIGH
    #

    if residual == 0xDE:

        state = "DE"

    elif residual == 0x84:

        state = (
            "LOW84"
            if stat <= 284
            else
            "HIGH84"
        )

    else:

        state = "5A"

    rows.append({

        "state": state,
        "b7": b7

    })

print()
print(f"Frames : {len(rows)}")

# ============================================================
# SEARCH BOUNDARIES
# ============================================================

best_acc = 0

best_a = None
best_b = None
best_c = None

for a in range(0x3F, 0x60):

    for b in range(a + 1, 0x60):

        for c in range(b + 1, 0x60):

            correct = 0

            for r in rows:

                b7 = r["b7"]

                if b7 <= a:

                    pred = "DE"

                elif b7 <= b:

                    pred = "LOW84"

                elif b7 <= c:

                    pred = "HIGH84"

                else:

                    pred = "5A"

                if pred == r["state"]:

                    correct += 1

            acc = correct / len(rows)

            if acc > best_acc:

                best_acc = acc

                best_a = a
                best_b = b
                best_c = c

# ============================================================
# RESULTS
# ============================================================

print()
print("=" * 80)
print("BEST B7 ENCODER")
print("=" * 80)

print(f"DE     <= 0x{best_a:02X}")
print(f"LOW84  <= 0x{best_b:02X}")
print(f"HIGH84 <= 0x{best_c:02X}")

print()
print(f"Accuracy = {100.0 * best_acc:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

states = ["DE", "LOW84", "HIGH84", "5A"]

matrix = {
    s: Counter()
    for s in states
}

for r in rows:

    b7 = r["b7"]

    if b7 <= best_a:

        pred = "DE"

    elif b7 <= best_b:

        pred = "LOW84"

    elif b7 <= best_c:

        pred = "HIGH84"

    else:

        pred = "5A"

    matrix[r["state"]][pred] += 1

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in states:

    print()
    print(actual)

    for pred in states:

        print(
            f"  {pred:7s}"
            f" : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# PER-STATE ACCURACY
# ============================================================

print()
print("=" * 80)
print("PER-STATE ACCURACY")
print("=" * 80)

for state in states:

    total = sum(matrix[state].values())

    if total == 0:
        continue

    correct = matrix[state][state]

    pct = 100.0 * correct / total

    print(
        f"{state:7s}"
        f" {correct}/{total}"
        f" ({pct:.2f}%)"
    )

print()
print("Done.")