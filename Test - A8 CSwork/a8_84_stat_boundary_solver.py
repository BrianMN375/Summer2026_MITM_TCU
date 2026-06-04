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
print("A8 84 STAT BOUNDARY SOLVER")
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

    if residual != 0x84:
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
    # Label based on V3 success/failure
    #

    if b7 <= 0x4B:

        label = "LOW84"

    else:

        label = "HIGH84"

    rows.append({

        "label": label,
        "stat": stat,
        "rpm": rpm,
        "dyn": dyn,
        "b7": b7

    })

print()
print(f"Frames : {len(rows)}")

# ============================================================
# SEARCH STAT THRESHOLD
# ============================================================

best_acc = 0
best_thresh = None

for thresh in range(200, 340):

    correct = 0

    for r in rows:

        pred = (
            "LOW84"
            if r["stat"] <= thresh
            else
            "HIGH84"
        )

        if pred == r["label"]:
            correct += 1

    acc = correct / len(rows)

    if acc > best_acc:

        best_acc = acc
        best_thresh = thresh

# ============================================================
# RESULTS
# ============================================================

print()
print("=" * 80)
print("BEST STAT THRESHOLD")
print("=" * 80)

print(f"STAT <= {best_thresh}")
print(f"Accuracy = {100.0*best_acc:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

matrix = {

    "LOW84": Counter(),
    "HIGH84": Counter()

}

for r in rows:

    pred = (
        "LOW84"
        if r["stat"] <= best_thresh
        else
        "HIGH84"
    )

    matrix[r["label"]][pred] += 1

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in ("LOW84", "HIGH84"):

    print()
    print(actual)

    for pred in ("LOW84", "HIGH84"):

        print(
            f"  {pred:7s}"
            f" : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# POPULATION STATS
# ============================================================

for label in ("LOW84", "HIGH84"):

    subset = [

        r for r in rows
        if r["label"] == label
    ]

    if not subset:
        continue

    print()
    print("=" * 80)
    print(label)
    print("=" * 80)

    for field in (

        "stat",
        "rpm",
        "dyn",
        "b7"

    ):

        vals = [r[field] for r in subset]

        print(
            f"{field:6s}"
            f" min={min(vals):8.2f}"
            f" max={max(vals):8.2f}"
            f" avg={sum(vals)/len(vals):8.2f}"
        )

print()
print("Done.")