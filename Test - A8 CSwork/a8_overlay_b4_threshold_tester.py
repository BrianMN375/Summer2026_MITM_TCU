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
print("A8 OVERLAY B4 THRESHOLD TESTER")
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

    if overlay not in (0x10, 0xDB, 0xCE):
        continue

    records.append({

        "overlay": overlay,

        "b4": frame[4],
        "b7": frame[7],

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

data = pd.DataFrame(records)

print()
print(
    f"High RPM Overlay Frames: {len(data):,}"
)

# ============================================================
# BRUTE FORCE B4 THRESHOLDS
# ============================================================

best_correct = 0
best_low = None
best_high = None

total = len(data)

print()
print("Searching B4 thresholds...")

for low in range(0x70, 0xC8):

    for high in range(low + 1, 0xC9):

        correct = 0

        for row in data.itertuples():

            b4 = row.b4

            if b4 >= high:

                pred = 0xDB

            elif b4 >= low:

                pred = 0x10

            else:

                pred = 0xCE

            if pred == row.overlay:

                correct += 1

        if correct > best_correct:

            best_correct = correct
            best_low = low
            best_high = high

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
print("BEST THRESHOLDS")
print("=" * 80)

print(
    f"LOW  = 0x{best_low:02X}"
)

print(
    f"HIGH = 0x{best_high:02X}"
)

print(
    f"Correct = {best_correct:,}"
)

print(
    f"Accuracy = {accuracy:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

matrix = {

    0x10: {0x10:0, 0xDB:0, 0xCE:0},
    0xDB: {0x10:0, 0xDB:0, 0xCE:0},
    0xCE: {0x10:0, 0xDB:0, 0xCE:0}

}

for row in data.itertuples():

    b4 = row.b4

    if b4 >= best_high:

        pred = 0xDB

    elif b4 >= best_low:

        pred = 0x10

    else:

        pred = 0xCE

    matrix[row.overlay][pred] += 1

for actual in (0x10, 0xDB, 0xCE):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x10, 0xDB, 0xCE):

        print(
            f"  Pred 0x{pred:02X} : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# OVERLAY STATS
# ============================================================

print()
print("=" * 80)
print("OVERLAY STATS")
print("=" * 80)

for overlay in (0x10, 0xDB, 0xCE):

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
        f"STAT  : "
        f"{subset.stat.min()}"
        f" .. "
        f"{subset.stat.max()}"
        f" (avg={subset.stat.mean():.2f})"
    )

    print(
        f"B7    : "
        f"0x{subset.b7.min():02X}"
        f" .. "
        f"0x{subset.b7.max():02X}"
    )

print()
print("Done.")