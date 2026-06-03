#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13_gold import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# LOAD DATA
# ============================================================

print()
print("Loading dataset...")
print(CSV_FILE)

df = pd.read_csv(CSV_FILE)

frames = []

for row in df.itertuples():

    frame = [

        int(row.b0,16),
        int(row.b1,16),
        int(row.b2,16),
        int(row.b3,16),
        int(row.b4,16),
        int(row.b5,16),
        int(row.b6,16),
        int(row.b7,16),

    ]

    actual = actual_residual(frame)

    if actual not in (0x1E,0xC0):
        continue

    predicted = predicted_residual(frame)

    rpm, stat, dyn = decode_signals(frame)

    frames.append({

        "time"      : row.time,

        "actual"    : actual,
        "predicted" : predicted,

        "match"     : (
            actual == predicted
        ),

        "counter"   : frame[1] & 0x0F,

        "b4"        : frame[4],
        "b5"        : frame[5],
        "b6"        : frame[6],
        "b7"        : frame[7],

        "rpm"       : rpm,
        "stat"      : stat,
        "dyn"       : dyn
    })

boundary = pd.DataFrame(frames)

print()
print(
    f"Boundary Frames: {len(boundary):,}"
)

# ============================================================
# SPLIT
# ============================================================

df1E = boundary[
    boundary.actual == 0x1E
]

dfC0 = boundary[
    boundary.actual == 0xC0
]

# ============================================================
# SUMMARY
# ============================================================

print()
print("="*80)
print("0x1E SUMMARY")
print("="*80)

print(
    f"Frames: {len(df1E):,}"
)

for field in [

    "rpm",
    "stat",
    "dyn",
    "b4",
    "b5",
    "b6",
    "b7"

]:

    print()

    print(field)

    print(
        f"  min={df1E[field].min()}"
    )

    print(
        f"  max={df1E[field].max()}"
    )

    print(
        f"  avg={df1E[field].mean():.2f}"
    )

# ============================================================

print()
print("="*80)
print("0xC0 SUMMARY")
print("="*80)

print(
    f"Frames: {len(dfC0):,}"
)

for field in [

    "rpm",
    "stat",
    "dyn",
    "b4",
    "b5",
    "b6",
    "b7"

]:

    print()

    print(field)

    print(
        f"  min={dfC0[field].min()}"
    )

    print(
        f"  max={dfC0[field].max()}"
    )

    print(
        f"  avg={dfC0[field].mean():.2f}"
    )

# ============================================================
# BYTE4 DISTRIBUTION
# ============================================================

print()
print("="*80)
print("BYTE4 DISTRIBUTION")
print("="*80)

dist1E = (
    df1E["b4"]
    .value_counts()
    .sort_index()
)

distC0 = (
    dfC0["b4"]
    .value_counts()
    .sort_index()
)

all_b4 = sorted(

    set(dist1E.index)
    |
    set(distC0.index)

)

for b4 in all_b4:

    c1 = dist1E.get(
        b4,
        0
    )

    c2 = distC0.get(
        b4,
        0
    )

    total = c1 + c2

    pctC0 = (
        100*c2/total
        if total else 0
    )

    print(
        f"0x{b4:02X}"
        f"  1E={c1:4d}"
        f"  C0={c2:4d}"
        f"  C0%={pctC0:6.1f}"
    )

# ============================================================
# BYTE7 DISTRIBUTION
# ============================================================

print()
print("="*80)
print("BYTE7 DISTRIBUTION")
print("="*80)

for b7 in sorted(

    set(df1E.b7)
    |
    set(dfC0.b7)

):

    c1 = len(

        df1E[
            df1E.b7 == b7
        ]

    )

    c2 = len(

        dfC0[
            dfC0.b7 == b7
        ]

    )

    total = c1 + c2

    if total < 10:
        continue

    pct = (
        100*c2/total
    )

    print(
        f"0x{b7:02X}"
        f"  1E={c1:4d}"
        f"  C0={c2:4d}"
        f"  C0%={pct:6.1f}"
    )

# ============================================================
# BOUNDARY FAILURES
# ============================================================

print()
print("="*80)
print("BOUNDARY FAILURES")
print("="*80)

fail = boundary[
    boundary.actual !=
    boundary.predicted
]

print(
    f"Count: {len(fail):,}"
)

fail.to_csv(

    "boundary_failures.csv",

    index=False

)

print(
    "Saved:"
    " boundary_failures.csv"
)

# ============================================================
# MOST INTERESTING FAILURES
# ============================================================

print()
print("="*80)
print("FIRST 50 FAILURES")
print("="*80)

for row in (

    fail
    .head(50)
    .itertuples()

):

    print(

        f"T={row.time} "

        f"ACT=0x{row.actual:02X} "

        f"PRED=0x{row.predicted:02X} "

        f"B4=0x{row.b4:02X} "

        f"B7=0x{row.b7:02X} "

        f"RPM={row.rpm:.1f} "

        f"STAT={row.stat} "

        f"DYN={row.dyn}"

    )

print()
print("Done.")