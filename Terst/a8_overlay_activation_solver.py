#!/usr/bin/env python3

import pandas as pd
from sklearn.tree import DecisionTreeClassifier, export_text
from sklearn.metrics import accuracy_score

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 OVERLAY ACTIVATION SOLVER")
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
    # FAMILY
    # --------------------------------------------------------

    if b4 < 0x80:
        family = 0

    elif b4 < 0xC0:
        family = 1

    else:
        family = 2

    # --------------------------------------------------------
    # RPM BAND
    # --------------------------------------------------------

    if rpm < 4000:
        rpm_band = 0

    elif rpm < 6200:
        rpm_band = 1

    else:
        rpm_band = 2

    overlay_active = 0

    if overlay != 0:
        overlay_active = 1

    records.append({

        "active": overlay_active,

        "family": family,
        "rpm_band": rpm_band,

        "counter": frame[1] & 0x0F,

        "b4": frame[4],
        "b5": frame[5],
        "b6": frame[6],
        "b7": frame[7],

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

data = pd.DataFrame(records)

print()
print(
    f"Frames: {len(data):,}"
)

print()
print(
    f"Overlay Active: "
    f"{data.active.sum():,}"
)

# ============================================================
# TRAIN TREE
# ============================================================

features = [

    "family",
    "rpm_band",

    "counter",

    "b4",
    "b5",
    "b6",
    "b7",

    "rpm",
    "stat",
    "dyn"

]

X = data[features]
y = data["active"]

print()
print("Training classifier...")

tree = DecisionTreeClassifier(
    max_depth=8,
    random_state=0
)

tree.fit(X, y)

pred = tree.predict(X)

acc = accuracy_score(
    y,
    pred
)

# ============================================================
# RESULTS
# ============================================================

print()
print("=" * 80)
print("ACTIVATION CLASSIFIER")
print("=" * 80)

print(
    f"Training Accuracy : "
    f"{acc*100:.4f}%"
)

print()
print("Feature Importance")
print("------------------")

importance = sorted(

    zip(
        features,
        tree.feature_importances_
    ),

    key=lambda x: x[1],
    reverse=True

)

for name,value in importance:

    print(
        f"{name:10s} "
        f"{value:.4f}"
    )

# ============================================================
# TREE
# ============================================================

print()
print("=" * 80)
print("DECISION TREE")
print("=" * 80)

print(

    export_text(

        tree,

        feature_names=features,

        max_depth=8

    )

)

# ============================================================
# OVERLAY REGIONS
# ============================================================

print()
print("=" * 80)
print("OVERLAY ACTIVE REGIONS")
print("=" * 80)

active = data[
    data.active == 1
]

for field in [

    "b4",
    "b5",
    "b6",
    "b7",

    "rpm",
    "stat",
    "dyn"

]:

    print(
        f"{field:8s}"
        f" min={active[field].min():8.2f}"
        f" max={active[field].max():8.2f}"
        f" avg={active[field].mean():8.2f}"
    )

# ============================================================
# BY FAMILY / RPM
# ============================================================

print()
print("=" * 80)
print("ACTIVATION RATE BY REGION")
print("=" * 80)

for fam in range(3):

    for band in range(3):

        region = data[
            (data.family == fam) &
            (data.rpm_band == band)
        ]

        if len(region) == 0:
            continue

        active_count = (
            region.active.sum()
        )

        pct = (

            100.0 *
            active_count /
            len(region)

        )

        print(

            f"Family={fam} "
            f"RPM={band} "

            f"Active={active_count:6d} "

            f"Rate={pct:7.3f}%"

        )

print()
print("Done.")