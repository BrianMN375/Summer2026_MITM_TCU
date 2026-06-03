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
print("A8 DE LOW84 HIGH84 COMPARISON")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

states = {
    "DE": [],
    "LOW84": [],
    "HIGH84": []
}

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

    b4 = frame[4]
    b5 = frame[5]
    b6 = frame[6]
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

    states[state].append({

        "b4": b4,
        "b5": b5,
        "b6": b6,
        "b7": b7,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

# ============================================================
# SUMMARY
# ============================================================

for state in ("DE","LOW84","HIGH84"):

    rows = states[state]

    print()
    print("=" * 80)
    print(state)
    print("=" * 80)

    print(f"Frames : {len(rows)}")

    for field in (
        "b4",
        "b5",
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

# ============================================================
# DIFFERENCE TABLE
# ============================================================

print()
print("=" * 80)
print("AVERAGE COMPARISON")
print("=" * 80)

fields = (
    "b4",
    "b6",
    "b7",
    "rpm",
    "stat",
    "dyn"
)

for field in fields:

    de_avg = sum(r[field] for r in states["DE"]) / len(states["DE"])

    low_avg = (
        sum(r[field] for r in states["LOW84"])
        / len(states["LOW84"])
    )

    high_avg = (
        sum(r[field] for r in states["HIGH84"])
        / len(states["HIGH84"])
    )

    print()

    print(field)

    print(
        f"DE      {de_avg:10.2f}"
    )

    print(
        f"LOW84   {low_avg:10.2f}"
    )

    print(
        f"HIGH84  {high_avg:10.2f}"
    )

# ============================================================
# B7 HISTOGRAMS
# ============================================================

for state in ("DE","LOW84","HIGH84"):

    print()
    print("=" * 80)
    print(f"{state} B7 HISTOGRAM")
    print("=" * 80)

    hist = Counter(
        r["b7"] for r in states[state]
    )

    for value,count in hist.most_common(20):

        print(
            f"0x{value:02X} : {count}"
        )

print()
print("Done.")