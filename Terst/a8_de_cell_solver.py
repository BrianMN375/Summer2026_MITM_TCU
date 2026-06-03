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
print("A8 DE CELL SOLVER")
print("=" * 80)

# ============================================================
# LOAD DATA
# ============================================================

df = pd.read_csv(CSV_FILE)

rows = []

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

    rows.append({

        "state": state,
        "rpm": rpm,
        "b4": b4,
        "stat": stat,
        "dyn": dyn,
        "b6": frame[6]

    })

print()
print(f"Frames : {len(rows):,}")

# ============================================================
# CANDIDATE B4 CELLS
# ============================================================

candidate_cells = [

    [0x80],
    [0x80,0x92],
    [0x80,0x92,0x83],
    [0x80,0x92,0x83,0x91],
    [0x80,0x92,0x83,0x91,0xA0]

]

best_acc = 0
best_rpm = None
best_cells = None

print()
print("Searching...")

for rpm_thr in range(4025, 4301, 25):

    for cells in candidate_cells:

        correct = 0

        for row in rows:

            pred = (

                0xDE

                if (
                    row["rpm"] <= rpm_thr
                    and
                    row["b4"] in cells
                )

                else

                0x00

            )

            if pred == row["state"]:
                correct += 1

        acc = correct / len(rows)

        if acc > best_acc:

            best_acc = acc
            best_rpm = rpm_thr
            best_cells = cells.copy()

# ============================================================
# RESULTS
# ============================================================

print()
print("=" * 80)
print("BEST RULE")
print("=" * 80)

print(
    f"RPM <= {best_rpm}"
)

print(
    "B4 in { "
    +
    ", ".join(
        f"0x{x:02X}"
        for x in best_cells
    )
    +
    " }"
)

print(
    f"Accuracy = "
    f"{best_acc*100:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

matrix = {

    0x00 : Counter(),
    0xDE : Counter()

}

for row in rows:

    pred = (

        0xDE

        if (
            row["rpm"] <= best_rpm
            and
            row["b4"] in best_cells
        )

        else

        0x00

    )

    matrix[row["state"]][pred] += 1

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
# PRECISION / RECALL
# ============================================================

tp = matrix[0xDE][0xDE]
fn = matrix[0xDE][0x00]
fp = matrix[0x00][0xDE]

precision = tp / (tp + fp) if (tp+fp) else 0
recall = tp / (tp + fn) if (tp+fn) else 0

print()
print("=" * 80)
print("DE METRICS")
print("=" * 80)

print(
    f"Precision : {precision*100:.2f}%"
)

print(
    f"Recall    : {recall*100:.2f}%"
)

print()
print("Done.")