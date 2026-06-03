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
print("A8 B7 LADDER SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

rows = []

# ============================================================
# BUILD POPULATIONS
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

    residual = (
        actual_residual(frame)
        ^
        predicted_residual(frame)
    )

    rpm, stat, dyn = decode_signals(frame)

    b7 = frame[7]

    if residual == 0xDE:

        state = "DE"

    elif residual == 0x84:

        if stat <= 284:
            state = "LOW84"
        else:
            state = "HIGH84"

    else:
        continue

    rows.append({

        "state": state,
        "b7": b7

    })

print()
print(f"Frames : {len(rows)}")

# ============================================================
# DISTRIBUTIONS
# ============================================================

print()
print("=" * 80)
print("B7 DISTRIBUTIONS")
print("=" * 80)

for state in ("DE","LOW84","HIGH84"):

    vals = [

        r["b7"]
        for r in rows
        if r["state"] == state

    ]

    print()
    print(state)

    print(
        f"Count={len(vals)} "
        f"Min=0x{min(vals):02X} "
        f"Max=0x{max(vals):02X} "
        f"Avg={sum(vals)/len(vals):.2f}"
    )

# ============================================================
# SEARCH BANDS
# ============================================================

best_acc = -1
best = None

for de_max in range(0x30,0x60):

    for low84_max in range(de_max+1,0x60):

        correct = 0

        for r in rows:

            b7 = r["b7"]

            if b7 <= de_max:
                pred = "DE"

            elif b7 <= low84_max:
                pred = "LOW84"

            else:
                pred = "HIGH84"

            if pred == r["state"]:
                correct += 1

        acc = correct / len(rows)

        if acc > best_acc:

            best_acc = acc
            best = (

                de_max,
                low84_max

            )

# ============================================================
# RESULTS
# ============================================================

de_max, low84_max = best

print()
print("=" * 80)
print("BEST B7 LADDER")
print("=" * 80)

print(
    f"DE      <= 0x{de_max:02X}"
)

print(
    f"LOW84   <= 0x{low84_max:02X}"
)

print(
    f"HIGH84  >  0x{low84_max:02X}"
)

print()
print(
    f"Accuracy = {best_acc*100:.4f}%"
)

# ============================================================
# CONFUSION MATRIX
# ============================================================

conf = {}

for actual in ("DE","LOW84","HIGH84"):

    conf[actual] = {

        "DE":0,
        "LOW84":0,
        "HIGH84":0

    }

for r in rows:

    b7 = r["b7"]

    if b7 <= de_max:
        pred = "DE"

    elif b7 <= low84_max:
        pred = "LOW84"

    else:
        pred = "HIGH84"

    conf[r["state"]][pred] += 1

print()
print("=" * 80)
print("CONFUSION MATRIX")
print("=" * 80)

for actual in ("DE","LOW84","HIGH84"):

    print()
    print(actual)

    for pred in ("DE","LOW84","HIGH84"):

        print(
            f"  {pred:7s} : {conf[actual][pred]}"
        )

print()
print("Done.")