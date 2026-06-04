#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# LOAD DATA
# ============================================================

print()
print("=" * 80)
print("A8 OVERLAY EXCEPTION FINDER")
print("=" * 80)

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

    records.append({

        "overlay" : overlay,

        "b4" : frame[4],
        "b5" : frame[5],
        "b6" : frame[6],
        "b7" : frame[7],

        "counter" : frame[1] & 0x0F,

        "rpm" : rpm,
        "stat" : stat,
        "dyn" : dyn

    })

data = pd.DataFrame(records)

# ============================================================
# OVERLAY DEFINITIONS
# ============================================================

overlay_regions = {

    0x14 : lambda r:
        r.rpm < 3800 and r.dyn < 0,

    0xDE : lambda r:
        3900 <= r.rpm <= 4900 and r.dyn > 0,

    0x5A : lambda r:
        4300 <= r.rpm <= 6100 and
        0 <= r.dyn <= 258,

    0x91 : lambda r:
        5300 <= r.rpm <= 6100 and
        r.dyn > 258,

    0xCE : lambda r:
        r.rpm > 6100 and
        r.dyn < 0,

    0x10 : lambda r:
        r.rpm > 6100 and
        0 <= r.dyn <= 258,

    0xDB : lambda r:
        r.rpm > 6100 and
        r.dyn > 258
}

# ============================================================
# ANALYZE
# ============================================================

for overlay, condition in overlay_regions.items():

    print()
    print("=" * 80)
    print(f"OVERLAY 0x{overlay:02X}")
    print("=" * 80)

    region = data[
        data.apply(
            condition,
            axis=1
        )
    ]

    actual = region[
        region.overlay == overlay
    ]

    others = region[
        region.overlay != overlay
    ]

    print(
        f"Region Frames     : {len(region):,}"
    )

    print(
        f"Overlay Frames    : {len(actual):,}"
    )

    if len(region):

        pct = (
            100.0 *
            len(actual) /
            len(region)
        )

        print(
            f"Hit Rate          : "
            f"{pct:.4f}%"
        )

    if len(actual) < 5:
        continue

    print()
    print("ACTUAL OVERLAY")

    for field in [

        "b4",
        "b5",
        "b6",
        "b7",
        "counter",
        "stat"

    ]:

        print(
            f"{field:8s}"
            f" avg={actual[field].mean():8.2f}"
            f" min={actual[field].min():5.0f}"
            f" max={actual[field].max():5.0f}"
        )

    print()
    print("OTHER FRAMES")

    for field in [

        "b4",
        "b5",
        "b6",
        "b7",
        "counter",
        "stat"

    ]:

        print(
            f"{field:8s}"
            f" avg={others[field].mean():8.2f}"
            f" min={others[field].min():5.0f}"
            f" max={others[field].max():5.0f}"
        )

    print()
    print("DELTA")

    for field in [

        "b4",
        "b5",
        "b6",
        "b7",
        "counter",
        "stat"

    ]:

        delta = (
            actual[field].mean()
            -
            others[field].mean()
        )

        print(
            f"{field:8s}"
            f" {delta:+8.2f}"
        )

# ============================================================
# BYTE VALUE HISTOGRAMS
# ============================================================

interesting = [

    0xDE,
    0x91,
    0xDB,
    0xCE,
    0x10

]

for overlay in interesting:

    subset = data[
        data.overlay == overlay
    ]

    if len(subset) == 0:
        continue

    print()
    print("=" * 80)
    print(
        f"BYTE7 HISTOGRAM 0x{overlay:02X}"
    )
    print("=" * 80)

    counts = (
        subset.b7
        .value_counts()
        .sort_index()
    )

    for value,count in counts.items():

        print(
            f"0x{value:02X}"
            f" : {count}"
        )

print()
print("Done.")