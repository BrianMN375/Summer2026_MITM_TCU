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
print("A8 MIDMID COMPOSITE SOLVER")
print("=" * 80)

# ============================================================
# CANDIDATE STATE MACHINE
# ============================================================

def predict_state(frame):

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b7 = frame[7]

    #
    # DE branch
    #

    if stat <= 255:

        if dyn <= 76:
            return 0xDE

        return 0x00

    #
    # Positive-stat branch
    #

    if (
        b4 <= 0x8F and
        b7 <= 0x4B
    ):
        return 0x84

    return 0x5A


# ============================================================
# LOAD DATA
# ============================================================

df = pd.read_csv(CSV_FILE)

total = 0
correct = 0

states = [

    0x00,
    0xDE,
    0x84,
    0x5A

]

matrix = {

    state: Counter()
    for state in states

}

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

    state = actual ^ predicted

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

    #
    # Ignore high-rpm boundary overlay
    #

    if state == 0x10:
        continue

    pred_state = predict_state(frame)

    total += 1

    if pred_state == state:
        correct += 1

    matrix[state][pred_state] += 1

# ============================================================
# RESULTS
# ============================================================

accuracy = (

    100.0 *
    correct /
    total

)

print()
print("=" * 80)
print("RESULTS")
print("=" * 80)

print(
    f"Frames   : {total:,}"
)

print(
    f"Correct  : {correct:,}"
)

print(
    f"Accuracy : {accuracy:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in states:

    print()
    print(
        f"Actual 0x{actual:02X}"
    )

    for pred in states:

        print(

            f"  Pred 0x{pred:02X}"
            f" : "
            f"{matrix[actual][pred]}"

        )

# ============================================================
# ERROR BREAKDOWN
# ============================================================

errors = Counter()

for actual in states:

    for pred,count in matrix[actual].items():

        if actual != pred:

            errors[
                (actual,pred)
            ] += count

print()
print("=" * 80)
print("ERROR BREAKDOWN")
print("=" * 80)

for (actual,pred),count in errors.most_common():

    print(

        f"0x{actual:02X}"
        f" -> "
        f"0x{pred:02X}"

        f" : {count}"

    )

# ============================================================
# PER-STATE ACCURACY
# ============================================================

print()
print("=" * 80)
print("PER-STATE ACCURACY")
print("=" * 80)

for state in states:

    total_state = sum(
        matrix[state].values()
    )

    if total_state == 0:
        continue

    correct_state = matrix[state][state]

    pct = (

        100.0 *
        correct_state /
        total_state

    )

    print(

        f"0x{state:02X}"
        f"  "
        f"{correct_state}/{total_state}"
        f"  "
        f"{pct:.2f}%"

    )

print()
print("Done.")