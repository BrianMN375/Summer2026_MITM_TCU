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
print("A8 84 MISCLASSIFICATION DUMP")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

misses = []
correct = []

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

    if residual != 0x84:
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b5 = frame[5]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    #
    # Mid-family only
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # Overlay-active region
    #

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

    #
    # V3 classifier
    #

    if b7 <= 0x44:

        pred = 0xDE

    elif b7 <= 0x4B:

        pred = 0x84

    else:

        pred = 0x5A

    record = {

        "b4": b4,
        "b5": b5,
        "b6": b6,
        "b7": b7,

        "counter": counter,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    }

    if pred == 0x84:

        correct.append(record)

    else:

        misses.append(record)

# ============================================================
# SUMMARY
# ============================================================

print()
print(f"Correct 84 : {len(correct)}")
print(f"Missed 84  : {len(misses)}")

# ============================================================
# MISSED FRAMES
# ============================================================

print()
print("=" * 80)
print("MISSED 84 FRAMES")
print("=" * 80)

for i,r in enumerate(misses):

    print(
        f"{i+1:02d}  "
        f"B4=0x{r['b4']:02X} "
        f"B6=0x{r['b6']:02X} "
        f"B7=0x{r['b7']:02X} "
        f"RPM={r['rpm']:.1f} "
        f"STAT={r['stat']} "
        f"DYN={r['dyn']} "
        f"CNT={r['counter']}"
    )

# ============================================================
# AGGREGATES
# ============================================================

def summarize(name, rows):

    if not rows:
        return

    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    for field in [

        "b4",
        "b6",
        "b7",

        "rpm",
        "stat",
        "dyn"

    ]:

        values = [r[field] for r in rows]

        print(
            f"{field:6s}"
            f" min={min(values):8.2f}"
            f" max={max(values):8.2f}"
            f" avg={sum(values)/len(values):8.2f}"
        )

summarize("CORRECT 84", correct)
summarize("MISSED 84", misses)

print()
print("Done.")