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
print("A8 OVERLAY SOLVER V1")
print("=" * 80)

# ============================================================
# DISCOVERED OVERLAY MODEL
# ============================================================

def predict_overlay(frame):

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b7 = frame[7]

    # --------------------------------------------------------
    # HIGH FAMILY
    # --------------------------------------------------------

    if b4 >= 0xC0:

        if rpm >= 6200:
            return 0xDB

        if rpm >= 5000:
            return 0x91

        return 0x00

    # --------------------------------------------------------
    # MID FAMILY
    # --------------------------------------------------------

    if 0x80 <= b4 < 0xC0:

        #
        # DE state
        #
        if stat <= 255:
            return 0xDE

        #
        # 84 state
        #
        if (
            b4 <= 0x8F and
            b7 <= 0x4B
        ):
            return 0x84

        #
        # High RPM MID family
        #
        if rpm >= 6200:
            return 0x10

        #
        # Normal MID family
        #
        return 0x5A

    # --------------------------------------------------------
    # LOW FAMILY
    # --------------------------------------------------------

    if b4 < 0x80:

        #
        # High RPM LOW family
        #
        if rpm >= 6200:
            return 0xCE

        #
        # Low-RPM states remain unsolved
        #
        return 0x00

    return 0x00

# ============================================================
# LOAD DATA
# ============================================================

df = pd.read_csv(CSV_FILE)

total = 0

correct_base = 0
correct_v1 = 0

remaining_xors = Counter()

overlay_used = Counter()

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

    base = predicted_residual(frame)

    overlay = predict_overlay(frame)

    improved = base ^ overlay

    total += 1

    if actual == base:
        correct_base += 1

    if actual == improved:
        correct_v1 += 1
    else:
        remaining_xors[
            actual ^ improved
        ] += 1

    if overlay:
        overlay_used[overlay] += 1

# ============================================================
# RESULTS
# ============================================================

base_acc = (
    100.0 *
    correct_base /
    total
)

v1_acc = (
    100.0 *
    correct_v1 /
    total
)

print()
print("=" * 80)
print("ACCURACY")
print("=" * 80)

print(
    f"Frames        : {total:,}"
)

print(
    f"Base Correct  : {correct_base:,}"
)

print(
    f"V1 Correct    : {correct_v1:,}"
)

print()

print(
    f"Base Accuracy : "
    f"{base_acc:.4f}%"
)

print(
    f"V1 Accuracy   : "
    f"{v1_acc:.4f}%"
)

print(
    f"Improvement   : "
    f"{v1_acc-base_acc:.4f}%"
)

# ============================================================
# OVERLAYS USED
# ============================================================

print()
print("=" * 80)
print("OVERLAY APPLICATIONS")
print("=" * 80)

for overlay,count in (

    overlay_used.most_common()

):

    print(
        f"0x{overlay:02X}"
        f"   {count:,}"
    )

# ============================================================
# REMAINING XORS
# ============================================================

print()
print("=" * 80)
print("REMAINING XORS")
print("=" * 80)

for xor,count in (

    remaining_xors.most_common(25)

):

    pct = (
        100.0 *
        count /
        total
    )

    print(

        f"0x{xor:02X}"
        f"   {count:6,d}"
        f" ({pct:.4f}%)"

    )

print()
print("Done.")