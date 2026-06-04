#!/usr/bin/env python3

import pandas as pd

from sklearn.tree import (
    DecisionTreeClassifier,
    export_text
)

from sklearn.metrics import accuracy_score

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 MID-MID STATE SOLVER")
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

    # ========================================================
    # ONLY MID FAMILY / MID RPM
    # ========================================================

    if not (0x80 <= b4 < 0xC0):
        continue

    if not (4000 <= rpm < 6200):
        continue

    records.append({

        "state": overlay,

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
print(f"Frames : {len(data):,}")

print()
print("=" * 80)
print("STATE DISTRIBUTION")
print("=" * 80)

dist = data.state.value_counts()

for state,count in dist.items():

    pct = (
        100.0 *
        count /
        len(data)
    )

    print(
        f"0x{state:02X}"
        f"   {count:6d}"
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
y = data["state"]

print()
print("Training decision tree...")

tree = DecisionTreeClassifier(

    max_depth=8,
    random_state=0

)

tree.fit(X,y)

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

    key=lambda x:x[1],
    reverse=True

)

for name,val in importance:

    print(
        f"{name:8s} "
        f"{val:.4f}"
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
# STATE SIGNATURES
# ============================================================

print()
print("=" * 80)
print("STATE SIGNATURES")
print("=" * 80)

for state in sorted(data.state.unique()):

    subset = data[
        data.state == state
    ]

    print()
    print(
        f"State 0x{state:02X}"
    )

    print(
        f"Count : {len(subset)}"
    )

    for field in [

        "b4",
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
# BYTE7 HISTOGRAMS
# ============================================================

print()
print("=" * 80)
print("BYTE7 HISTOGRAMS")
print("=" * 80)

for state in sorted(data.state.unique()):

    subset = data[
        data.state == state
    ]

    print()
    print(
        f"State 0x{state:02X}"
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