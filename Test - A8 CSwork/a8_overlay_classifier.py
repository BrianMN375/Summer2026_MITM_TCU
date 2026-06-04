#!/usr/bin/env python3

import pandas as pd

from sklearn.tree import (
    DecisionTreeClassifier,
    export_text
)

from sklearn.metrics import (
    accuracy_score
)

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
print("=" * 80)
print("A8 OVERLAY CLASSIFIER")
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

        "counter" : frame[1] & 0x0F,

        "b4" : frame[4],
        "b5" : frame[5],
        "b6" : frame[6],
        "b7" : frame[7],

        "band" : frame[7] >> 4,

        "rpm" : rpm,
        "stat" : stat,
        "dyn" : dyn

    })

data = pd.DataFrame(records)

# ============================================================
# OVERLAY DISTRIBUTION
# ============================================================

print()
print("=" * 80)
print("OVERLAY DISTRIBUTION")
print("=" * 80)

dist = (
    data.overlay
    .value_counts()
)

for overlay,count in dist.items():

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
# CLASSIFIER
# ============================================================

features = [

    "band",

    "b4",
    "b5",
    "b6",
    "b7",

    "rpm",
    "stat",
    "dyn"

]

X = data[features]

y = data["overlay"]

print()
print("Training decision tree...")

tree = DecisionTreeClassifier(

    max_depth=6,

    random_state=0

)

tree.fit(
    X,
    y
)

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
print("OVERLAY CLASSIFIER")
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
        f"{name:8s} "
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

        max_depth=6

    )

)

# ============================================================
# PER OVERLAY STATS
# ============================================================

print()
print("=" * 80)
print("OVERLAY SIGNATURES")
print("=" * 80)

interesting = [

    0xDE,
    0xDB,
    0x91,
    0xCE,
    0x10,
    0x5A,

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
        f"Frames : {len(subset)}"
    )

    print(
        f"Band   : "
        f"{subset.band.min()}"
        f" .. "
        f"{subset.band.max()}"
    )

    print(
        f"B4     : "
        f"0x{subset.b4.min():02X}"
        f" .. "
        f"0x{subset.b4.max():02X}"
    )

    print(
        f"B7     : "
        f"0x{subset.b7.min():02X}"
        f" .. "
        f"0x{subset.b7.max():02X}"
    )

    print(
        f"RPM    : "
        f"{subset.rpm.min():.1f}"
        f" .. "
        f"{subset.rpm.max():.1f}"
    )

    print(
        f"STAT   : "
        f"{subset.stat.min()}"
        f" .. "
        f"{subset.stat.max()}"
    )

    print(
        f"DYN    : "
        f"{subset.dyn.min()}"
        f" .. "
        f"{subset.dyn.max()}"
    )

print()
print("Done.")