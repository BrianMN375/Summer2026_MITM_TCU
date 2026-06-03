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
print("A8 DE PRIMARY RULE VALIDATOR")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

rows = []

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
    # MID RPM REGION
    #

    if not (4000 <= rpm < 6200):
        continue

    rows.append({

        "state": state,

        "b4": b4,
        "b6": b6,

        "rpm": rpm,
        "stat": stat

    })

print()
print(f"Frames    : {len(rows):,}")

de_total = sum(
    1 for r in rows
    if r["state"] == 0xDE
)

print(f"DE Frames : {de_total:,}")

# ============================================================
# PRIMARY RULE
# ============================================================

matrix = {

    0x00 : Counter(),
    0xDE : Counter()

}

for row in rows:

    pred = (

        0xDE

        if (

            row["b4"] <= 0x92
            and
            row["b6"] <= 0x9C
            and
            row["rpm"] > 4043.5
            and
            row["stat"] <= 252.5

        )

        else

        0x00

    )

    matrix[row["state"]][pred] += 1

# ============================================================
# RESULTS
# ============================================================

correct = (

    matrix[0x00][0x00]
    +
    matrix[0xDE][0xDE]

)

accuracy = correct / len(rows)

print()
print("=" * 80)
print("RESULTS")
print("=" * 80)

print(f"Correct  : {correct:,}")
print(f"Accuracy : {accuracy*100:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in (0x00,0xDE):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x00,0xDE):

        print(
            f"  Pred 0x{pred:02X}"
            f" : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# METRICS
# ============================================================

tp = matrix[0xDE][0xDE]
fn = matrix[0xDE][0x00]
fp = matrix[0x00][0xDE]

precision = tp / (tp + fp) if (tp + fp) else 0
recall = tp / (tp + fn) if (tp + fn) else 0

print()
print("=" * 80)
print("DE METRICS")
print("=" * 80)

print(
    f"Precision : "
    f"{precision*100:.2f}%"
)

print(
    f"Recall    : "
    f"{recall*100:.2f}%"
)

# ============================================================
# COVERAGE
# ============================================================

print()
print("=" * 80)
print("DE COVERAGE")
print("=" * 80)

print(
    f"Captured : "
    f"{tp}/{de_total}"
)

print(
    f"Coverage : "
    f"{100.0*tp/de_total:.2f}%"
)

print()
print("Done.")