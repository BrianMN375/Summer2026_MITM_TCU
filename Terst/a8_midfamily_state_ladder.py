#!/usr/bin/env python3

import pandas as pd

from collections import defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 MIDFAMILY STATE LADDER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

groups = defaultdict(list)

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

    if residual not in (0xDE, 0x84, 0x5A):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    #
    # Mid-family only
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # Overlay activation region
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
    # Split 84
    #

    if residual == 0x84:

        state = (
            "LOW84"
            if stat <= 284
            else
            "HIGH84"
        )

    elif residual == 0xDE:

        state = "DE"

    else:

        state = "5A"

    groups[state].append({

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn,
        "b4": b4,
        "b6": b6,
        "b7": b7

    })

# ============================================================
# SUMMARY TABLE
# ============================================================

print()
print("=" * 80)
print("STATE SUMMARY")
print("=" * 80)

for state in ("DE","LOW84","HIGH84","5A"):

    rows = groups[state]

    if not rows:
        continue

    print()
    print(state)
    print("-" * 40)

    for field in ("rpm","stat","dyn","b4","b6","b7"):

        vals = [r[field] for r in rows]

        print(
            f"{field:6s}"
            f" min={min(vals):8.2f}"
            f" max={max(vals):8.2f}"
            f" avg={sum(vals)/len(vals):8.2f}"
        )

# ============================================================
# LADDER BY RPM
# ============================================================

print()
print("=" * 80)
print("RPM LADDER")
print("=" * 80)

for state in ("DE","LOW84","HIGH84","5A"):

    rows = groups[state]

    if not rows:
        continue

    rpms = [r["rpm"] for r in rows]

    print(
        f"{state:8s}"
        f"  avg={sum(rpms)/len(rpms):8.2f}"
    )

# ============================================================
# LADDER BY STAT
# ============================================================

print()
print("=" * 80)
print("STAT LADDER")
print("=" * 80)

for state in ("DE","LOW84","HIGH84","5A"):

    rows = groups[state]

    if not rows:
        continue

    vals = [r["stat"] for r in rows]

    print(
        f"{state:8s}"
        f"  avg={sum(vals)/len(vals):8.2f}"
    )

# ============================================================
# LADDER BY B7
# ============================================================

print()
print("=" * 80)
print("B7 LADDER")
print("=" * 80)

for state in ("DE","LOW84","HIGH84","5A"):

    rows = groups[state]

    if not rows:
        continue

    vals = [r["b7"] for r in rows]

    print(
        f"{state:8s}"
        f"  avg={sum(vals)/len(vals):8.2f}"
    )

print()
print("Done.")