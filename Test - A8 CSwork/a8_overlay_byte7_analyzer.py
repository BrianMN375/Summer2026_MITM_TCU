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

records = []

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

    rpm, stat, dyn = decode_signals(frame)

    records.append({

        "actual"    : actual,
        "predicted" : predicted,

        "byte4"     : frame[4],
        "byte5"     : frame[5],
        "byte6"     : frame[6],
        "byte7"     : frame[7],

        "rpm"       : rpm,
        "stat"      : stat,
        "dyn"       : dyn

    })

data = pd.DataFrame(records)

# ============================================================
# RESIDUAL SUMMARY
# ============================================================

interesting = [

    0x1E,
    0xC0,

    0x5A,
    0x91,
    0xDB,
    0xDE,
    0xCE,

    0x84,
    0x14,
    0x51,
    0x4A

]

print()
print("=" * 80)
print("OVERLAY BYTE7 ANALYZER")
print("=" * 80)

for residual in interesting:

    subset = data[
        data.actual == residual
    ]

    if len(subset) == 0:
        continue

    print()
    print("=" * 70)
    print(
        f"RESIDUAL 0x{residual:02X}"
    )
    print("=" * 70)

    print(
        f"Frames : {len(subset):,}"
    )

    print(
        f"Byte7 : "
        f"0x{subset.byte7.min():02X}"
        f" .. "
        f"0x{subset.byte7.max():02X}"
    )

    print(
        f"Byte4 : "
        f"0x{subset.byte4.min():02X}"
        f" .. "
        f"0x{subset.byte4.max():02X}"
    )

    print(
        f"RPM   : "
        f"{subset.rpm.min():.1f}"
        f" .. "
        f"{subset.rpm.max():.1f}"
    )

    print(
        f"STAT  : "
        f"{subset.stat.min()}"
        f" .. "
        f"{subset.stat.max()}"
    )

    print(
        f"DYN   : "
        f"{subset.dyn.min()}"
        f" .. "
        f"{subset.dyn.max()}"
    )

# ============================================================
# BYTE7 DISTRIBUTION
# ============================================================

print()
print("=" * 80)
print("BYTE7 DISTRIBUTION")
print("=" * 80)

for residual in interesting:

    subset = data[
        data.actual == residual
    ]

    if len(subset) < 5:
        continue

    counts = (
        subset.byte7
        .value_counts()
        .sort_index()
    )

    print()
    print(
        f"Residual 0x{residual:02X}"
    )

    for b7,count in counts.items():

        print(
            f" 0x{b7:02X}"
            f" : {count}"
        )

# ============================================================
# MISPREDICTIONS
# ============================================================

print()
print("=" * 80)
print("TOP MISPREDICTION GROUPS")
print("=" * 80)

fails = data[
    data.actual !=
    data.predicted
]

groups = (

    fails
    .groupby(
        [
            "actual",
            "predicted"
        ]
    )
    .size()
    .sort_values(
        ascending=False
    )

)

for (actual,pred),count in groups.head(25).items():

    print(

        f"ACT=0x{actual:02X} "
        f"PRED=0x{pred:02X} "
        f"COUNT={count}"

    )

# ============================================================
# BYTE7 VS MISPREDICTION
# ============================================================

print()
print("=" * 80)
print("BYTE7 MISPREDICTIONS")
print("=" * 80)

for b7 in sorted(

    fails.byte7.unique()

):

    subset = fails[
        fails.byte7 == b7
    ]

    if len(subset) < 5:
        continue

    print()

    print(
        f"Byte7=0x{b7:02X}"
    )

    print(
        f"Count={len(subset)}"
    )

    top = (

        subset
        .groupby(
            [
                "actual",
                "predicted"
            ]
        )
        .size()
        .sort_values(
            ascending=False
        )

    )

    for (

        (actual,pred),
        count

    ) in top.head(5).items():

        print(

            f"   "
            f"ACT=0x{actual:02X} "
            f"PRED=0x{pred:02X} "
            f"{count}"

        )

print()
print("Done.")