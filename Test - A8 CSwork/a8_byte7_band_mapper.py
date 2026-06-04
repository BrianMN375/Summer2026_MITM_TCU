#!/usr/bin/env python3

import pandas as pd
from collections import Counter, defaultdict

from a8_final_validator_v13_gold import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
)

print()
print("=" * 80)
print("A8 BYTE7 BAND MAPPER")
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

    byte7 = frame[7]

    records.append({

        "byte7": byte7,
        "band": byte7 >> 4,

        "actual": actual,
        "predicted": predicted,

        "overlay": overlay

    })

data = pd.DataFrame(records)

print(
    f"\nFrames: {len(data):,}"
)

# ============================================================
# OVERLAY HISTOGRAM
# ============================================================

print()
print("=" * 80)
print("OVERLAY HISTOGRAM")
print("=" * 80)

overlay_counts = (
    data.overlay
    .value_counts()
    .sort_values(
        ascending=False
    )
)

for overlay,count in overlay_counts.head(25).items():

    pct = (
        100.0 *
        count /
        len(data)
    )

    print(
        f"0x{overlay:02X}"
        f"  {count:8,d}"
        f" ({pct:6.2f}%)"
    )

# ============================================================
# BAND ANALYSIS
# ============================================================

print()
print("=" * 80)
print("BYTE7 HIGH-NIBBLE BANDS")
print("=" * 80)

for band in sorted(

    data.band.unique()

):

    subset = data[
        data.band == band
    ]

    print()
    print(
        f"Band 0x{band:X}x"
    )

    print(
        f"Frames: {len(subset):,}"
    )

    hist = Counter(
        subset.overlay
    )

    for overlay,count in (

        hist
        .most_common(10)

    ):

        pct = (
            100.0 *
            count /
            len(subset)
        )

        print(

            f"  0x{overlay:02X}"
            f"  {count:6d}"
            f" ({pct:5.1f}%)"

        )

# ============================================================
# INDIVIDUAL BYTE7 VALUES
# ============================================================

print()
print("=" * 80)
print("BYTE7 VALUE ANALYSIS")
print("=" * 80)

for b7 in sorted(

    data.byte7.unique()

):

    subset = data[
        data.byte7 == b7
    ]

    if len(subset) < 5:
        continue

    hist = Counter(
        subset.overlay
    )

    dominant_overlay, count = (

        hist.most_common(1)[0]

    )

    pct = (
        100.0 *
        count /
        len(subset)
    )

    print(

        f"0x{b7:02X}"
        f"  count={len(subset):4d}"
        f"  dominant=0x{dominant_overlay:02X}"
        f"  ({pct:5.1f}%)"

    )

# ============================================================
# OVERLAY -> BYTE7 MAP
# ============================================================

print()
print("=" * 80)
print("OVERLAY TO BYTE7 MAP")
print("=" * 80)

interesting = [

    0xDE,
    0xDB,
    0x91,
    0xCE,
    0x5A,
    0x10,

    0x84,
    0x14

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
        f"Frames: {len(subset):,}"
    )

    print(
        f"Byte7 range:"
        f" 0x{subset.byte7.min():02X}"
        f" .."
        f" 0x{subset.byte7.max():02X}"
    )

    counts = (
        subset.byte7
        .value_counts()
        .sort_index()
    )

    for b7,count in counts.items():

        print(
            f" 0x{b7:02X}"
            f" : {count}"
        )

# ============================================================
# BAND TRANSITIONS
# ============================================================

print()
print("=" * 80)
print("BYTE7 BAND TRANSITIONS")
print("=" * 80)

transitions = Counter()

band_list = data.band.tolist()

for i in range(

    len(band_list)-1

):

    transitions[
        (
            band_list[i],
            band_list[i+1]
        )
    ] += 1

for (

    (src,dst),
    count

) in transitions.most_common():

    print(

        f"0x{src:X}x"
        f" -> "
        f"0x{dst:X}x"

        f"  {count}"

    )

print()
print("Done.")