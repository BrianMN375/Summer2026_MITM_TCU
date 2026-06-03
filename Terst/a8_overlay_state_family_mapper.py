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
print("A8 OVERLAY STATE FAMILY MAPPER")
print("=" * 80)

# ============================================================
# LOAD DATA
# ============================================================

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

    overlay = actual ^ predicted

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]

    # --------------------------------------------------------
    # B4 FAMILY
    # --------------------------------------------------------

    if b4 < 0x80:
        family = "LOW"

    elif b4 < 0xC0:
        family = "MID"

    else:
        family = "HIGH"

    # --------------------------------------------------------
    # RPM BAND
    # --------------------------------------------------------

    if rpm < 4000:
        rpm_band = "LOW"

    elif rpm < 6200:
        rpm_band = "MID"

    else:
        rpm_band = "HIGH"

    records.append({

        "overlay"  : overlay,

        "family"   : family,
        "rpm_band" : rpm_band,

        "b4"   : b4,
        "b7"   : frame[7],

        "rpm"  : rpm,
        "dyn"  : dyn,
        "stat" : stat

    })

data = pd.DataFrame(records)

# ============================================================
# FAMILY MAP
# ============================================================

print()
print("=" * 80)
print("FAMILY MAP")
print("=" * 80)

groups = defaultdict(Counter)

for row in data.itertuples():

    groups[
        (row.family,
         row.rpm_band)
    ][row.overlay] += 1

for key in sorted(groups.keys()):

    family, rpm_band = key

    print()
    print(
        f"{family:5s} "
        f"{rpm_band:5s}"
    )

    for overlay,count in (

        groups[key]
        .most_common(10)

    ):

        pct = (

            100.0 *
            count /
            sum(groups[key].values())

        )

        print(

            f"   0x{overlay:02X}"
            f"  {count:8,d}"
            f" ({pct:6.2f}%)"

        )

# ============================================================
# DOMINANT OVERLAYS
# ============================================================

print()
print("=" * 80)
print("DOMINANT FAMILY STATES")
print("=" * 80)

for key in sorted(groups.keys()):

    family, rpm_band = key

    overlay, count = (

        groups[key]
        .most_common(1)[0]

    )

    total = sum(

        groups[key].values()

    )

    pct = (

        100.0 *
        count /
        total

    )

    print(

        f"{family:5s} "
        f"{rpm_band:5s} "
        f"-> "
        f"0x{overlay:02X}"
        f" ({pct:.2f}%)"

    )

# ============================================================
# OVERLAY BREAKDOWN
# ============================================================

print()
print("=" * 80)
print("OVERLAY BREAKDOWN")
print("=" * 80)

interesting = [

    0x14,
    0x5A,
    0x91,

    0x10,
    0xDB,
    0xCE,

    0xDE,
    0x84,
    0xE4,

    0x51,
    0x4A,
    0xA1

]

for overlay in interesting:

    subset = data[
        data.overlay == overlay
    ]

    if len(subset) == 0:
        continue

    print()
    print(
        f"Overlay 0x{overlay:02X}"
    )

    print(
        f"Count: {len(subset)}"
    )

    fam_counts = (
        subset.family
        .value_counts()
    )

    rpm_counts = (
        subset.rpm_band
        .value_counts()
    )

    print(
        "Families:"
    )

    for fam,count in fam_counts.items():

        print(
            f"   {fam:5s}"
            f" {count}"
        )

    print(
        "RPM Bands:"
    )

    for band,count in rpm_counts.items():

        print(
            f"   {band:5s}"
            f" {count}"
        )

    print(
        f"B4 range:"
        f" 0x{subset.b4.min():02X}"
        f" .."
        f" 0x{subset.b4.max():02X}"
    )

    print(
        f"B7 range:"
        f" 0x{subset.b7.min():02X}"
        f" .."
        f" 0x{subset.b7.max():02X}"
    )

# ============================================================
# B4 FAMILY PURITY
# ============================================================

print()
print("=" * 80)
print("B4 FAMILY PURITY")
print("=" * 80)

for family in [

    "LOW",
    "MID",
    "HIGH"

]:

    subset = data[
        data.family == family
    ]

    counts = (
        subset.overlay
        .value_counts()
    )

    print()
    print(f"{family} FAMILY")

    for overlay,count in counts.head(10).items():

        pct = (
            100.0 *
            count /
            len(subset)
        )

        print(

            f"0x{overlay:02X}"
            f"  {count:8,d}"
            f" ({pct:6.2f}%)"

        )

print()
print("Done.")