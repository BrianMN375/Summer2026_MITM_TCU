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
print("A8 MIDMID DYN THRESHOLD TESTER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

de_rows = []
s84_rows = []

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
    # DE SEARCH DATASET
    #

    if state in (0x00, 0xDE):

        de_rows.append({

            "state": state,
            "dyn": dyn,
            "stat": stat,
            "b7": frame[7]

        })

    #
    # 84 SEARCH DATASET
    #

    if state in (0x84, 0x5A):

        s84_rows.append({

            "state": state,
            "dyn": dyn,
            "stat": stat,
            "b4": b4,
            "b7": frame[7]

        })

print()
print(f"DE Dataset  : {len(de_rows):,}")
print(f"84 Dataset  : {len(s84_rows):,}")

# ============================================================
# DE SEARCH
# ============================================================

print()
print("=" * 80)
print("DE THRESHOLD SEARCH")
print("=" * 80)

best_x = None
best_acc = 0

for x in range(-100, 301):

    correct = 0

    for row in de_rows:

        pred = 0xDE if row["dyn"] <= x else 0x00

        if pred == row["state"]:
            correct += 1

    acc = correct / len(de_rows)

    if acc > best_acc:

        best_acc = acc
        best_x = x

print()
print(f"Best Threshold : {best_x}")
print(f"Accuracy       : {best_acc*100:.4f}%")

# ============================================================
# DE CONFUSION MATRIX
# ============================================================

print()
print("DE CONFUSION MATRIX")
print("-" * 40)

matrix = {

    0x00 : Counter(),
    0xDE : Counter()

}

for row in de_rows:

    pred = 0xDE if row["dyn"] <= best_x else 0x00

    matrix[row["state"]][pred] += 1

for actual in (0x00, 0xDE):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x00, 0xDE):

        print(
            f"  Pred 0x{pred:02X} : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# 84 SEARCH
# ============================================================

print()
print("=" * 80)
print("84 THRESHOLD SEARCH")
print("=" * 80)

best_y = None
best_acc = 0

for y in range(-100, 301):

    correct = 0

    for row in s84_rows:

        pred = 0x84 if row["dyn"] <= y else 0x5A

        if pred == row["state"]:
            correct += 1

    acc = correct / len(s84_rows)

    if acc > best_acc:

        best_acc = acc
        best_y = y

print()
print(f"Best Threshold : {best_y}")
print(f"Accuracy       : {best_acc*100:.4f}%")

# ============================================================
# 84 CONFUSION MATRIX
# ============================================================

print()
print("84 CONFUSION MATRIX")
print("-" * 40)

matrix = {

    0x84 : Counter(),
    0x5A : Counter()

}

for row in s84_rows:

    pred = 0x84 if row["dyn"] <= best_y else 0x5A

    matrix[row["state"]][pred] += 1

for actual in (0x84, 0x5A):

    print()
    print(f"Actual 0x{actual:02X}")

    for pred in (0x84, 0x5A):

        print(
            f"  Pred 0x{pred:02X} : "
            f"{matrix[actual][pred]}"
        )

# ============================================================
# SUMMARY
# ============================================================

print()
print("=" * 80)
print("SUMMARY")
print("=" * 80)

print()
print(f"DE Best DYN Threshold : {best_x}")
print(f"84 Best DYN Threshold : {best_y}")

print()
print("Done.")