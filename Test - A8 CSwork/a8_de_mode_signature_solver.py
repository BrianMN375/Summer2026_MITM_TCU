#!/usr/bin/env python3

import pandas as pd
from collections import defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 DE MODE SIGNATURE SOLVER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

modes = defaultdict(list)

# ============================================================
# MODE CLASSIFIER
# ============================================================

def classify_mode(b7, b4):

    # --------------------------------------------------------
    # MODE_80
    # --------------------------------------------------------

    if b7 == 0x40 and b4 in (0x80,0x81,0x82):
        return "MODE_80"

    # --------------------------------------------------------
    # MODE_92
    # --------------------------------------------------------

    if b7 == 0x3F and b4 in (0x91,0x92,0x93,0x94):
        return "MODE_92"

    # --------------------------------------------------------
    # MODE_A
    # --------------------------------------------------------

    if b4 in (0xA0,0xA1,0xA2,0xA3):
        return "MODE_A"

    # --------------------------------------------------------
    # MODE_83
    # --------------------------------------------------------

    if b4 == 0x83:
        return "MODE_83"

    return "MODE_OTHER"

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

    if residual != 0xDE:
        continue

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    rpm = ((frame[2] << 8) | frame[3]) / 4.0
    stat = (frame[4] << 1) + (frame[5] >> 7)
    dyn = frame[6] >> 1

    mode = classify_mode(b7,b4)

    modes[mode].append(
        {
            "b4": b4,
            "b6": b6,
            "b7": b7,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "counter": counter
        }
    )

# ============================================================
# REPORT
# ============================================================

for mode in sorted(modes.keys()):

    rows = modes[mode]

    print()
    print("=" * 80)
    print(mode)
    print("=" * 80)

    print(f"Frames : {len(rows)}")

    for field in [

        "rpm",
        "stat",
        "dyn",
        "b4",
        "b6",
        "b7",
        "counter"

    ]:

        values = [r[field] for r in rows]

        print(
            f"{field:<8}"
            f"min={min(values):8.2f} "
            f"max={max(values):8.2f} "
            f"avg={sum(values)/len(values):8.2f}"
        )

# ============================================================
# MODE COVERAGE
# ============================================================

print()
print("=" * 80)
print("MODE COVERAGE")
print("=" * 80)

total = sum(len(v) for v in modes.values())

for mode in sorted(modes.keys()):

    count = len(modes[mode])

    print(
        f"{mode:<12}"
        f"{count:4d} "
        f"({100.0*count/total:6.2f}%)"
    )

print()
print("Done.")