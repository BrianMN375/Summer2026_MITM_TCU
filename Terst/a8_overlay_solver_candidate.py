#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13_gold import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# OVERLAY MODEL
# ============================================================

def overlay_from_signals(
    rpm,
    dyn,
    stat,
    b4,
    b7
):
    """
    First-pass model derived from
    a8_overlay_classifier.py
    """

    # --------------------------------------------------------
    # Low RPM negative torque
    # --------------------------------------------------------

    if rpm < 3800:

        if dyn < 0:
            return 0x14

        return 0x00

    # --------------------------------------------------------
    # Mid RPM DE band
    # --------------------------------------------------------

    if 3900 <= rpm <= 4900:

        if dyn > 0:
            return 0xDE

        return 0x00

    # --------------------------------------------------------
    # 5300-6100 region
    # --------------------------------------------------------

    if 5200 <= rpm <= 6150:

        if dyn > 258:
            return 0x91

        if dyn > 0:
            return 0x5A

        return 0x00

    # --------------------------------------------------------
    # High RPM region
    # --------------------------------------------------------

    if rpm > 6100:

        if dyn > 258:
            return 0xDB

        if dyn < 0:
            return 0xCE

        return 0x10

    return 0x00


# ============================================================
# MAIN
# ============================================================

print()
print("=" * 80)
print("A8 OVERLAY SOLVER CANDIDATE")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

total = 0
correct_old = 0
correct_new = 0

overlay_hits = 0

overlay_hist = {}

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

    rpm, stat, dyn = decode_signals(frame)

    actual = actual_residual(frame)

    base = predicted_residual(frame)

    overlay = overlay_from_signals(

        rpm,
        dyn,
        stat,

        frame[4],
        frame[7]

    )

    improved = base ^ overlay

    total += 1

    if actual == base:
        correct_old += 1

    if actual == improved:
        correct_new += 1

    if overlay != 0:

        overlay_hits += 1

        overlay_hist[overlay] = (
            overlay_hist.get(
                overlay,
                0
            ) + 1
        )

# ============================================================
# RESULTS
# ============================================================

print()
print("=" * 80)
print("RESULTS")
print("=" * 80)

print(
    f"Frames       : {total:,}"
)

print(
    f"Old Correct  : {correct_old:,}"
)

print(
    f"New Correct  : {correct_new:,}"
)

old_acc = (
    100.0 *
    correct_old /
    total
)

new_acc = (
    100.0 *
    correct_new /
    total
)

print(
    f"\nOld Accuracy : {old_acc:.4f}%"
)

print(
    f"New Accuracy : {new_acc:.4f}%"
)

print(
    f"Improvement  : "
    f"{new_acc-old_acc:.4f}%"
)

# ============================================================
# OVERLAY HISTOGRAM
# ============================================================

print()
print("=" * 80)
print("OVERLAY APPLICATIONS")
print("=" * 80)

for overlay,count in sorted(

    overlay_hist.items(),

    key=lambda x: x[1],

    reverse=True

):

    print(

        f"0x{overlay:02X}"
        f"   {count:,}"

    )

print()
print("Done.")