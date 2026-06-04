#!/usr/bin/env python3

import csv
import numpy as np

from sklearn.tree import DecisionTreeClassifier
from sklearn.tree import export_text

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0012.csv"
)

from a8_final_validator_v13_gold import (
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# LOAD OVERLAY FRAMES ONLY
# ============================================================

print()
print("Loading dataset...")

X = []
Y = []

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for row in reader:

        frame = [
            int(x, 16)
            for x in row[1:9]
        ]

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        state_xor = actual ^ predicted

        #
        # ONLY OVERLAY STATES
        #
        if state_xor == 0:
            continue

        rpm, stat, dyn = decode_signals(frame)

        b4 = frame[4]
        b5 = frame[5]
        b6 = frame[6]
        b7 = frame[7]

        X.append([
            rpm,
            stat,
            dyn,
            b4,
            b5,
            b6,
            b7
        ])

        Y.append(state_xor)

X = np.array(X)
Y = np.array(Y)

print(f"Overlay Frames : {len(X):,}")

# ============================================================
# TRAIN
# ============================================================

print()
print("Training overlay classifier...")

tree = DecisionTreeClassifier(
    max_depth=8,
    min_samples_leaf=10,
    random_state=1
)

tree.fit(X, Y)

accuracy = tree.score(X, Y)

# ============================================================
# REPORT
# ============================================================

print()
print("=" * 80)
print("DISCOVERED A8 OVERLAY STATE MACHINE")
print("=" * 80)

print()
print(
    f"Training Accuracy : "
    f"{accuracy * 100.0:.4f}%"
)

print()

feature_names = [
    "RPM",
    "STAT",
    "DYN",
    "BYTE4",
    "BYTE5",
    "BYTE6",
    "BYTE7"
]

print("Feature Importance")
print("------------------")

for name, importance in sorted(
        zip(
            feature_names,
            tree.feature_importances_
        ),
        key=lambda x: x[1],
        reverse=True):

    print(
        f"{name:8s} "
        f"{importance:.4f}"
    )

print()

print("Decision Tree")
print("-------------")

print(
    export_text(
        tree,
        feature_names=feature_names
    )
)

# ============================================================
# CLASS DISTRIBUTION
# ============================================================

print()
print("Overlay XOR Distribution")
print("------------------------")

unique, counts = np.unique(
    Y,
    return_counts=True
)

for value, count in sorted(
        zip(unique, counts),
        key=lambda x: x[1],
        reverse=True):

    pct = (
        count /
        len(Y)
    ) * 100.0

    print(
        f"0x{value:02X} "
        f"{count:8d} "
        f"({pct:6.2f}%)"
    )

print()
print("Class Mapping")
print("-------------")

for value in sorted(unique):

    print(
        f"class {value:3d} "
        f"= 0x{value:02X}"
    )

print()
print("Done.")