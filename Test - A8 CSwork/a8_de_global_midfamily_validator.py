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
print("A8 DE GLOBAL MID-FAMILY VALIDATOR")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

matrix = {

    0x00 : Counter(),
    0xDE : Counter()

}

primary_hits = 0
secondary_hits = 0
midfamily_hits = 0

total_frames = 0
total_de = 0

# ============================================================
# PROCESS ENTIRE DATASET
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

    state = 0xDE if residual == 0xDE else 0x00

    if state == 0xDE:
        total_de += 1

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]

    counter = frame[1] & 0x0F

    # ========================================================
    # MID FAMILY GATE
    # ========================================================

    mid_family = (

        b4 >= 0x80
        and
        b4 < 0xC0

    )

    if mid_family:
        midfamily_hits += 1

    # ========================================================
    # PRIMARY RULE
    # ========================================================

    rule_primary = (

        b4 <= 0x92
        and
        b6 <= 0x9C
        and
        rpm > 4043.5
        and
        stat <= 252.5

    )

    # ========================================================
    # SECONDARY RULE
    # ========================================================

    rule_secondary = (

        b4 <= 0x92
        and
        b6 > 0x9C
        and
        stat > 246.0
        and
        counter <= 12

    )

    pred = (

        0xDE

        if (

            mid_family

            and

            (
                rule_primary
                or
                rule_secondary
            )

        )

        else

        0x00

    )

    if mid_family and rule_primary:
        primary_hits += 1

    if mid_family and rule_secondary:
        secondary_hits += 1

    matrix[state][pred] += 1

    total_frames += 1

# ============================================================
# RESULTS
# ============================================================

correct = (

    matrix[0x00][0x00]
    +
    matrix[0xDE][0xDE]

)

accuracy = correct / total_frames

print()
print("=" * 80)
print("RESULTS")
print("=" * 80)

print(f"Frames        : {total_frames:,}")
print(f"DE Frames     : {total_de:,}")
print(f"MidFamilyRows : {midfamily_hits:,}")

print()
print(f"Correct       : {correct:,}")
print(f"Accuracy      : {accuracy*100:.4f}%")

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in (0x00, 0xDE):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x00, 0xDE):

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

print(f"Precision : {precision*100:.2f}%")
print(f"Recall    : {recall*100:.2f}%")

# ============================================================
# COVERAGE
# ============================================================

print()
print("=" * 80)
print("DE COVERAGE")
print("=" * 80)

print(f"Captured : {tp}/{total_de}")

if total_de:
    print(
        f"Coverage : "
        f"{100.0*tp/total_de:.2f}%"
    )

# ============================================================
# RULE CONTRIBUTION
# ============================================================

print()
print("=" * 80)
print("RULE CONTRIBUTION")
print("=" * 80)

print(f"Primary Hits   : {primary_hits}")
print(f"Secondary Hits : {secondary_hits}")

print()
print("Done.")