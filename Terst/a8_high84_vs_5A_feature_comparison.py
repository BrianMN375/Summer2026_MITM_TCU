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
print("A8 HIGH84 VS 5A FEATURE COMPARISON")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

high84 = []
state5a = []

# ============================================================
# BUILD DATASET
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

    if residual not in (0x84, 0x5A):
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
    # Overlay active only
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

    if residual == 0x84:

        if stat > 284:
            high84.append(record)

    elif residual == 0x5A:

        state5a.append(record)

# ============================================================
# SUMMARY
# ============================================================

print()
print(f"HIGH84 Frames : {len(high84)}")
print(f"5A Frames     : {len(state5a)}")

# ============================================================
# STATS
# ============================================================

def dump_stats(name, rows):

    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    for field in (

        "b4",
        "b6",
        "b7",

        "rpm",
        "stat",
        "dyn"

    ):

        vals = [r[field] for r in rows]

        print(
            f"{field:6s}"
            f" min={min(vals):8.2f}"
            f" max={max(vals):8.2f}"
            f" avg={sum(vals)/len(vals):8.2f}"
        )

dump_stats("HIGH84", high84)
dump_stats("5A", state5a)

# ============================================================
# HISTOGRAMS
# ============================================================

for field in ("b4", "b6", "b7", "stat"):

    print()
    print("=" * 80)
    print(field.upper())
    print("=" * 80)

    print()
    print("HIGH84")

    for value,count in Counter(
        r[field] for r in high84
    ).most_common(20):

        print(
            f"{value:>4} : {count}"
        )

    print()
    print("5A")

    for value,count in Counter(
        r[field] for r in state5a
    ).most_common(20):

        print(
            f"{value:>4} : {count}"
        )

# ============================================================
# UNIQUE VALUES
# ============================================================

print()
print("=" * 80)
print("UNIQUE VALUE ANALYSIS")
print("=" * 80)

for field in ("b4", "b6", "b7", "stat"):

    h = set(r[field] for r in high84)
    s = set(r[field] for r in state5a)

    only_h = sorted(h - s)
    only_s = sorted(s - h)

    print()
    print(field)

    print(
        f"HIGH84-only values : {len(only_h)}"
    )

    if only_h:
        print(
            only_h[:20]
        )

    print(
        f"5A-only values     : {len(only_s)}"
    )

    if only_s:
        print(
            only_s[:20]
        )

print()
print("Done.")