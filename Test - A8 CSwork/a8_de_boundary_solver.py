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
print("A8 DE BOUNDARY SOLVER")
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

    state = actual ^ predicted

    #
    # Only 00 / DE
    #

    if state not in (0x00, 0xDE):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]

    #
    # MID FAMILY
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # MID RPM
    #

    if not (4000 <= rpm < 6200):
        continue

    records.append({

        "state": 1 if state == 0xDE else 0,

        "b4": frame[4],
        "b5": frame[5],
        "b6": frame[6],
        "b7": frame[7],

        "counter": frame[1] & 0x0F,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    })

data = pd.DataFrame(records)

print()
print(
    f"Frames : {len(data):,}"
)

print(
    f"DE Frames : "
    f"{data.state.sum():,}"
)

# ============================================================
# TRAIN TREE
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
print("Training classifier...")

tree = DecisionTreeClassifier(

    max_depth=4,
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

        max_depth=4

    )

)

# ============================================================
# STATE SIGNATURES
# ============================================================

print()
print("=" * 80)
print("STATE SIGNATURES")
print("=" * 80)

for state in (0,1):

    subset = data[
        data.state == state
    ]

    label = "DE" if state else "00"

    print()
    print(label)

    for field in [

        "b7",
        "stat",
        "dyn",
        "rpm"

    ]:

        print(

            f"{field:8s}"

            f" min={subset[field].min():8.2f}"

            f" max={subset[field].max():8.2f}"

            f" avg={subset[field].mean():8.2f}"

        )

print()
print("Done.")