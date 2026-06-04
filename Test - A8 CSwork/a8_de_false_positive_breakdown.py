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
print("A8 DE FALSE POSITIVE BREAKDOWN")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

fp_residuals = Counter()
fp_b4 = Counter()
fp_b7 = Counter()

total_fp = 0

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

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    # --------------------------------------------------------
    # Primary Rule
    # --------------------------------------------------------

    rule_primary = (

        b4 <= 0x92
        and
        b6 <= 0x9C
        and
        rpm > 4043.5
        and
        stat <= 252.5

    )

    # --------------------------------------------------------
    # Secondary Rule
    # --------------------------------------------------------

    rule_secondary = (

        b4 <= 0x92
        and
        b6 > 0x9C
        and
        stat > 246.0
        and
        counter <= 12

    )

    pred_de = (
        rule_primary
        or
        rule_secondary
    )

    #
    # False Positive
    #

    if pred_de and residual != 0xDE:

        total_fp += 1

        fp_residuals[residual] += 1
        fp_b4[b4] += 1
        fp_b7[b7] += 1

print()
print(f"False Positives : {total_fp}")

# ============================================================
# RESIDUAL BREAKDOWN
# ============================================================

print()
print("=" * 80)
print("FALSE POSITIVE RESIDUALS")
print("=" * 80)

for residual,count in fp_residuals.most_common():

    pct = 100.0 * count / total_fp

    print(
        f"0x{residual:02X} : "
        f"{count:5d} "
        f"({pct:6.2f}%)"
    )

# ============================================================
# B4 BREAKDOWN
# ============================================================

print()
print("=" * 80)
print("TOP B4 VALUES")
print("=" * 80)

for value,count in fp_b4.most_common(25):

    print(
        f"0x{value:02X} : {count}"
    )

# ============================================================
# B7 BREAKDOWN
# ============================================================

print()
print("=" * 80)
print("TOP B7 VALUES")
print("=" * 80)

for value,count in fp_b7.most_common(25):

    print(
        f"0x{value:02X} : {count}"
    )

print()
print("Done.")