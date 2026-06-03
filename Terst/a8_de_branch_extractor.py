#!/usr/bin/env python3

import pandas as pd

from sklearn.tree import (
    DecisionTreeClassifier,
    _tree
)

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 DE BRANCH EXTRACTOR")
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
print(f"Frames : {len(data):,}")
print(f"DE Frames : {data.state.sum():,}")

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

tree = DecisionTreeClassifier(
    max_depth=4,
    random_state=0
)

tree.fit(X, y)

print()
print("=" * 80)
print("FEATURE IMPORTANCE")
print("=" * 80)

importance = sorted(
    zip(features, tree.feature_importances_),
    key=lambda x: x[1],
    reverse=True
)

for name, value in importance:

    print(
        f"{name:8s} {value:.4f}"
    )

# ============================================================
# EXTRACT DE LEAVES
# ============================================================

print()
print("=" * 80)
print("DE BRANCHES")
print("=" * 80)

t = tree.tree_

branch_id = 0

def walk(node, rules):

    global branch_id

    #
    # leaf
    #

    if t.feature[node] == _tree.TREE_UNDEFINED:

        value = t.value[node][0]

        predicted_class = int(value.argmax())

        samples = t.n_node_samples[node]

        de_count = int(round(
            samples * value[1] / value.sum()
        ))
        #
        # only show DE leaves
        #

        if predicted_class == 1:

            branch_id += 1

            purity = (
                100.0 * de_count / samples
            )

            print()
            print("-" * 80)
            print(
                f"DE BRANCH #{branch_id}"
            )
            print("-" * 80)

            for r in rules:
                print(r)

            print()

            print(
                f"Samples : {samples}"
            )

            print(
                f"DE Count : {de_count}"
            )

            print(
                f"Purity   : {purity:.2f}%"
            )

        return

    feature = features[
        t.feature[node]
    ]

    threshold = t.threshold[node]

    #
    # LEFT
    #

    walk(

        t.children_left[node],

        rules + [

            f"{feature} <= {threshold:.2f}"

        ]

    )

    #
    # RIGHT
    #

    walk(

        t.children_right[node],

        rules + [

            f"{feature} > {threshold:.2f}"

        ]

    )

walk(0, [])

print()
print("Done.")