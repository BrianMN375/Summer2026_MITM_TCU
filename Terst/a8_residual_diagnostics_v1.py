#!/usr/bin/env python3

import os
from collections import Counter, defaultdict

import csv
import pandas as pd

from sklearn.tree import (
    DecisionTreeClassifier,
    export_text
)

from a8_final_validator_v13_gold import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# OUTPUT DIRECTORY
# ============================================================

OUTPUT_DIR = "outputs"

os.makedirs(
    OUTPUT_DIR,
    exist_ok=True
)

# ============================================================
# LOAD DATASET
# ============================================================

print()
print("Loading dataset...")
print(CSV_FILE)

rows = []

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    for row in reader:

        if not row:
            continue

        if row[0].lower() == "time":
            continue

        try:

            timestamp = int(row[0])

            frame = [
                int(x.strip(), 16)
                for x in row[1:9]
            ]

        except:
            continue

        rpm, stat, dyn = decode_signals(frame)

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        rows.append({

            "time": timestamp,

            "actual": actual,
            "predicted": predicted,

            "delta": actual ^ predicted,

            "counter": frame[1] & 0x0F,

            "b0": frame[0],
            "b1": frame[1],
            "b2": frame[2],
            "b3": frame[3],
            "b4": frame[4],
            "b5": frame[5],
            "b6": frame[6],
            "b7": frame[7],

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn
        })

df = pd.DataFrame(rows)

print(
    f"\nFrames loaded: {len(df):,}"
)

# ============================================================
# RESIDUAL DISTRIBUTION
# ============================================================

print()
print("=" * 80)
print("RESIDUAL DISTRIBUTION")
print("=" * 80)

dist = (
    df["actual"]
    .value_counts()
    .sort_values(ascending=False)
)

for residual, count in dist.items():

    pct = (
        100.0 *
        count /
        len(df)
    )

    print(
        f"0x{residual:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

dist.to_csv(
    f"{OUTPUT_DIR}/residual_distribution.csv"
)

# ============================================================
# TRANSITION MATRIX
# ============================================================

print()
print("=" * 80)
print("TOP TRANSITIONS")
print("=" * 80)

transitions = Counter()

actuals = df["actual"].tolist()

for i in range(len(actuals)-1):

    transitions[
        (
            actuals[i],
            actuals[i+1]
        )
    ] += 1

transition_rows = []

for (a,b),count in transitions.items():

    transition_rows.append({

        "dst": f"0x{a:02X}",
        "dst": f"0x{b:02X}",
        "count": count

    })

transition_df = (
    pd.DataFrame(transition_rows)
    .sort_values(
        "count",
        ascending=False
    )
)

transition_df.to_csv(
    f"{OUTPUT_DIR}/transition_matrix.csv",
    index=False
)

for row in transition_df.head(50).itertuples():

    print(
        f"{row.dst} -> {row.dst}"
        f"   {row.count}"
    )

# ============================================================
# RESIDUAL VS COUNTER
# ============================================================

counter_table = pd.crosstab(
    df["counter"],
    df["actual"]
)

counter_table.to_csv(
    f"{OUTPUT_DIR}/residual_vs_counter.csv"
)

# ============================================================
# BYTE ANALYSIS
# ============================================================

for byte_name in [

    "b4",
    "b5",
    "b6",
    "b7"

]:

    print()
    print("=" * 80)
    print(
        f"{byte_name.upper()} ANALYSIS"
    )
    print("=" * 80)

    table = pd.crosstab(

        df[byte_name],
        df["actual"]

    )

    table.to_csv(
        f"{OUTPUT_DIR}/residual_vs_{byte_name}.csv"
    )

    totals = (
        df.groupby(byte_name)
        .size()
        .sort_index()
    )

    for value,count in totals.items():

        if count < 20:
            continue

        print(
            f"0x{value:02X}"
            f"  count={count}"
        )

# ============================================================
# MISPREDICTIONS
# ============================================================

mispred = df[
    df["actual"] != df["predicted"]
]

mispred.to_csv(
    f"{OUTPUT_DIR}/mispredictions.csv",
    index=False
)

print()
print("=" * 80)
print("MISPREDICTIONS")
print("=" * 80)

print(
    f"Count: {len(mispred):,}"
)

# ============================================================
# 0x1E VS 0xC0
# ============================================================

df_1e = df[
    df["actual"] == 0x1E
]

df_c0 = df[
    df["actual"] == 0xC0
]

df_1e.to_csv(
    f"{OUTPUT_DIR}/residual_1E.csv",
    index=False
)

df_c0.to_csv(
    f"{OUTPUT_DIR}/residual_C0.csv",
    index=False
)

print()
print("=" * 80)
print("0x1E VS 0xC0")
print("=" * 80)

print(
    f"0x1E frames: {len(df_1e):,}"
)

print(
    f"0xC0 frames: {len(df_c0):,}"
)

# ============================================================
# DECISION TREE
# ============================================================

print()
print("=" * 80)
print("DECISION TREE")
print("=" * 80)

features = [

    "counter",

    "b4",
    "b5",
    "b6",
    "b7",

    "rpm",
    "stat",
    "dyn"

]

X = df[features]

y = df["actual"]

tree = DecisionTreeClassifier(

    max_depth=5,

    random_state=0

)

tree.fit(X,y)

print(
    export_text(
        tree,
        feature_names=features
    )
)

# ============================================================
# FEATURE IMPORTANCE
# ============================================================

importance = pd.DataFrame({

    "feature": features,

    "importance":
        tree.feature_importances_

})

importance = importance.sort_values(

    "importance",

    ascending=False

)

importance.to_csv(

    f"{OUTPUT_DIR}/feature_importance.csv",

    index=False

)

print()
print("=" * 80)
print("FEATURE IMPORTANCE")
print("=" * 80)

for row in importance.itertuples():

    print(
        f"{row.feature:10s}"
        f"{row.importance:.4f}"
    )

print()
print(
    f"CSV outputs written to: {OUTPUT_DIR}"
)

print()
print("Done.")