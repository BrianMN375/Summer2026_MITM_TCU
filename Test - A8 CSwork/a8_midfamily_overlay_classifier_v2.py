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
print("A8 MIDFAMILY OVERLAY CLASSIFIER V2")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

TARGETS = [0xDE, 0x84, 0x5A]

matrix = {
    0xDE: Counter(),
    0x84: Counter(),
    0x5A: Counter()
}

frames = 0

# ============================================================
# PROCESS
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

    if residual not in TARGETS:
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    # ========================================================
    # MID FAMILY ONLY
    # ========================================================

    if not (0x80 <= b4 < 0xC0):
        continue

    # ========================================================
    # OVERLAY ACTIVATION REGION
    # ========================================================

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

    # ========================================================
    # CLASSIFIER V2
    # ========================================================

    if b6 < 150:

        pred_state = 0xDE

    elif b7 < 0x4D:

        pred_state = 0x84

    else:

        pred_state = 0x5A

    matrix[residual][pred_state] += 1

    frames += 1

# ============================================================
# RESULTS
# ============================================================

correct = (

    matrix[0xDE][0xDE]
    +
    matrix[0x84][0x84]
    +
    matrix[0x5A][0x5A]

)

accuracy = correct / frames if frames else 0

print()
print("=" * 80)
print("RESULTS")
print("=" * 80)

print(f"Frames   : {frames}")
print(f"Correct  : {correct}")
print(f"Accuracy : {accuracy*100:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in TARGETS:

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in TARGETS:

        print(
            f"  Pred 0x{pred:02X}"
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

for state in TARGETS:

    total = sum(matrix[state].values())

    if total == 0:
        continue

    correct_state = matrix[state][state]

    pct = 100.0 * correct_state / total

    print(
        f"0x{state:02X}  "
        f"{correct_state}/{total}  "
        f"{pct:.2f}%"
    )

print()
print("Done.")