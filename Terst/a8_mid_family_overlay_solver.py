#!/usr/bin/env python3

import pandas as pd

from sklearn.tree import (
    DecisionTreeClassifier,
    export_text
)

from sklearn.metrics import (
    accuracy_score
)

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 MID FAMILY OVERLAY SOLVER")
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

    # --------------------------------------------------------
    # ONLY MID FAMILY OVERLAYS
    # --------------------------------------------------------

    if overlay not in (
        0x5A,
        0xDE,
        0x84
    ):
        continue

    rpm, stat, dyn = decode_signals(frame)

    records.append({

        "overlay" : overlay,

        "counter" : frame[1] & 0x0F,

        "b4" : frame[4],
        "b5" : frame[5],
        "b6" : frame[6],
        "b7" : frame[7],

        "rpm" : rpm,
        "stat" : stat,
        "dyn" : dyn

    })

data = pd.DataFrame(records)

print()
print(
    f"Frames : {len(data):,}"
)

# ============================================================
# DISTRIBUTION
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
        f"  {count:5d}"
        f" ({pct:6.2f}%)"

    )

# ============================================================
# DECISION TREE
# ============================================================

features = [

    "b4",
    "b5",
    "b6",
    "b7",

    "counter",

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
print("CLASSIFIER RESULTS")
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
        f"{name:8s}"
        f" {value:.4f}"
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
# OVERLAY SIGNATURES
# ============================================================

print()
print("=" * 80)
print("OVERLAY SIGNATURES")
print("=" * 80)

for overlay in (

    0x5A,
    0xDE,
    0x84

):

    subset = data[
        data.overlay == overlay
    ]

    print()
    print(
        f"Overlay 0x{overlay:02X}"
    )

    print(
        f"Count : {len(subset)}"
    )

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

            f" min={subset[field].min():8.2f}"

            f" max={subset[field].max():8.2f}"

            f" avg={subset[field].mean():8.2f}"

        )

# ============================================================
# B7 HISTOGRAMS
# ============================================================

print()
print("=" * 80)
print("BYTE7 HISTOGRAMS")
print("=" * 80)

for overlay in (

    0x5A,
    0xDE,
    0x84

):

    subset = data[
        data.overlay == overlay
    ]

    print()
    print(
        f"Overlay 0x{overlay:02X}"
    )

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