#!/usr/bin/env python3

import pandas as pd

from collections import Counter, defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 MIDFAMILY OVERLAY STATE BREAKDOWN")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

TARGETS = [0xDE, 0x84, 0x5A]

states = defaultdict(list)

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

    if residual not in TARGETS:
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]
    b5 = frame[5]
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    # ========================================================
    # MID FAMILY ONLY
    # ========================================================

    if not (0x80 <= b4 < 0xC0):
        continue

    # ========================================================
    # OVERLAY ACTIVATION REGION
    # ========================================================

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

    states[residual].append({

        "b4": b4,
        "b5": b5,
        "b6": b6,
        "b7": b7,

        "counter": counter,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

# ============================================================
# REPORT
# ============================================================

for residual in TARGETS:

    rows = states[residual]

    print()
    print("=" * 80)
    print(f"STATE 0x{residual:02X}")
    print("=" * 80)

    print(f"Frames : {len(rows)}")

    if not rows:
        continue

    for field in [

        "b4",
        "b5",
        "b6",
        "b7",

        "counter",

        "rpm",
        "stat",
        "dyn"

    ]:

        values = [r[field] for r in rows]

        print(
            f"{field:8s}"
            f" min={min(values):8.2f}"
            f" max={max(values):8.2f}"
            f" avg={sum(values)/len(values):8.2f}"
        )

# ============================================================
# HISTOGRAMS
# ============================================================

for residual in TARGETS:

    rows = states[residual]

    if not rows:
        continue

    print()
    print("=" * 80)
    print(f"STATE 0x{residual:02X} B7 HISTOGRAM")
    print("=" * 80)

    hist = Counter(
        r["b7"]
        for r in rows
    )

    for value,count in sorted(hist.items()):

        print(
            f"0x{value:02X} : {count}"
        )

print()
print("Done.")