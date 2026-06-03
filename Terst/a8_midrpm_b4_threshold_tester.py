#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 MIDRPM B4 THRESHOLD TESTER")
print("=" * 80)

# ============================================================
# LOAD DATA
# ============================================================

df = pd.read_csv(CSV_FILE)

records = []

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

    overlay = actual ^ predicted

    rpm, stat, dyn = decode_signals(frame)

    if overlay not in (0x5A, 0x91):
        continue

    records.append({

        "overlay": overlay,

        "b4": frame[4],
        "b5": frame[5],
        "b6": frame[6],
        "b7": frame[7],

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

data = pd.DataFrame(records)

print()
print(
    f"Mid RPM Overlay Frames: {len(data):,}"
)

# ============================================================
# BRUTE FORCE SINGLE B4 THRESHOLD
# ============================================================

best_correct = 0
best_threshold = None

total = len(data)

print()
print("Searching B4 threshold...")

for threshold in range(0x00, 0x100):

    correct = 0

    for row in data.itertuples():

        if row.b4 >= threshold:
            pred = 0x91
        else:
            pred = 0x5A

        if pred == row.overlay:
            correct += 1

    if correct > best_correct:

        best_correct = correct
        best_threshold = threshold

# ============================================================
# RESULTS
# ============================================================

accuracy = (
    100.0 *
    best_correct /
    total
)

print()
print("=" * 80)
print("BEST THRESHOLD")
print("=" * 80)

print(
    f"Threshold = 0x{best_threshold:02X}"
)

print(
    f"Correct   = {best_correct:,}"
)

print(
    f"Accuracy  = {accuracy:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

matrix = {

    0x5A: {0x5A:0, 0x91:0},
    0x91: {0x5A:0, 0x91:0}

}

for row in data.itertuples():

    if row.b4 >= best_threshold:
        pred = 0x91
    else:
        pred = 0x5A

    matrix[row.overlay][pred] += 1

for actual in (0x5A, 0x91):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x5A, 0x91):

        print(
            f"  Pred 0x{pred:02X} : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# STATS
# ============================================================

print()
print("=" * 80)
print("OVERLAY STATS")
print("=" * 80)

for overlay in (0x5A, 0x91):

    subset = data[
        data.overlay == overlay
    ]

    print()
    print(f"Overlay 0x{overlay:02X}")

    print(
        f"Count : {len(subset)}"
    )

    print(
        f"B4    : "
        f"0x{subset.b4.min():02X}"
        f" .. "
        f"0x{subset.b4.max():02X}"
        f" (avg={subset.b4.mean():.2f})"
    )

    print(
        f"RPM   : "
        f"{subset.rpm.min():.1f}"
        f" .. "
        f"{subset.rpm.max():.1f}"
    )

    print(
        f"DYN   : "
        f"{subset.dyn.min()}"
        f" .. "
        f"{subset.dyn.max()}"
    )

    print(
        f"B7    : "
        f"0x{subset.b7.min():02X}"
        f" .. "
        f"0x{subset.b7.max():02X}"
    )

print()
print("Done.")