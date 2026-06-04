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
print("A8 84 B4/B7 THRESHOLD TESTER")
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

    if state not in (0x84, 0x5A):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b7 = frame[7]

    #
    # Keep only MID FAMILY / MID RPM
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    if not (4000 <= rpm < 6200):
        continue

    rows.append({

        "state": state,
        "b4": b4,
        "b7": b7

    })

print()
print(f"Frames : {len(rows):,}")

# ============================================================
# SEARCH
# ============================================================

print()
print("Searching thresholds...")

best_correct = 0
best_b4 = None
best_b7 = None

for b4_thr in range(0x80, 0xC0):

    for b7_thr in range(0x40, 0x61):

        correct = 0

        for row in rows:

            pred = (

                0x84

                if (
                    row["b4"] <= b4_thr
                    and
                    row["b7"] <= b7_thr
                )

                else

                0x5A

            )

            if pred == row["state"]:
                correct += 1

        if correct > best_correct:

            best_correct = correct
            best_b4 = b4_thr
            best_b7 = b7_thr

# ============================================================
# RESULTS
# ============================================================

accuracy = (

    100.0 *
    best_correct /
    len(rows)

)

print()
print("=" * 80)
print("BEST THRESHOLDS")
print("=" * 80)

print(
    f"B4 Threshold = 0x{best_b4:02X}"
)

print(
    f"B7 Threshold = 0x{best_b7:02X}"
)

print(
    f"Correct      = {best_correct}"
)

print(
    f"Accuracy     = {accuracy:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

matrix = {

    0x84 : Counter(),
    0x5A : Counter()

}

for row in rows:

    pred = (

        0x84

        if (
            row["b4"] <= best_b4
            and
            row["b7"] <= best_b7
        )

        else

        0x5A

    )

    matrix[row["state"]][pred] += 1

for actual in (0x84, 0x5A):

    print()
    print(
        f"Actual 0x{actual:02X}"
    )

    for pred in (0x84, 0x5A):

        print(
            f"  Pred 0x{pred:02X}"
            f" : {matrix[actual][pred]}"
        )

# ============================================================
# STATE STATS
# ============================================================

print()
print("=" * 80)
print("STATE RANGES")
print("=" * 80)

for state in (0x84, 0x5A):

    subset = [

        r for r in rows
        if r["state"] == state

    ]

    b4s = [r["b4"] for r in subset]
    b7s = [r["b7"] for r in subset]

    print()
    print(f"State 0x{state:02X}")

    print(
        f"B4 : "
        f"0x{min(b4s):02X}"
        f" .. "
        f"0x{max(b4s):02X}"
    )

    print(
        f"B7 : "
        f"0x{min(b7s):02X}"
        f" .. "
        f"0x{max(b7s):02X}"
    )

print()
print("Done.")