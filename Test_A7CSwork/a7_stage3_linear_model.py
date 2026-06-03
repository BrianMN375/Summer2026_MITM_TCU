#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    # "a7_payload_unique.csv"
    "a7_unique.csv"
    # "a7_balanced.csv"
)

print()
print("Using CSV:")
print(CSV_FILE)
print()

# ------------------------------------------------------------
# Recovered vectors from Stage 2
#
# ONLY high-confidence non-zero vectors
# ------------------------------------------------------------

VECTORS = {

    (1,4): 0xA2,
    (1,5): 0x6B,
    (1,6): 0xD6,

    (2,5): 0x9B,

    (3,6): 0x3D,
    (3,7): 0x7A,

    (4,0): 0xAD,
    (4,1): 0x75,
    (4,2): 0xEA,
    (4,3): 0xFB,
    (4,4): 0xD9,
    (4,5): 0x9D,
    (4,6): 0x15,
    (4,7): 0x2A,

    (5,1): 0x45,
    (5,2): 0x8A,
    (5,3): 0x3B,
    (5,4): 0x76,
    (5,5): 0xEC,
    (5,6): 0xF7,
    (5,7): 0xC1,

    (6,0): 0x0E,
    (6,1): 0x1C,
    (6,2): 0x38,
    (6,3): 0x70,
    (6,4): 0xE0,
    (6,5): 0xEF,
    (6,6): 0xF1,
    (6,7): 0xCD,
}

# ------------------------------------------------------------

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            frames.append(frame)

        except:
            pass

print(f"Loaded {len(frames):,} frames")

# ------------------------------------------------------------
# Prediction
# ------------------------------------------------------------

residual_hist = Counter()

counter_hist = defaultdict(Counter)

upper_nibble_hist = defaultdict(Counter)

exact_matches = 0

for frame in frames:

    observed = frame[0]

    payload = frame[1:]

    predicted = 0

    # ----------------------------------------
    # Apply discovered vectors
    # ----------------------------------------

    for byte_idx in range(1, 8):

        value = payload[byte_idx - 1]

        for bit_idx in range(8):

            if not (value & (1 << bit_idx)):
                continue

            vector = VECTORS.get(
                (byte_idx, bit_idx)
            )

            if vector is not None:

                predicted ^= vector

    residual = observed ^ predicted

    residual_hist[residual] += 1

    counter = frame[1] & 0x0F

    upper_nibble = frame[1] >> 4

    counter_hist[counter][residual] += 1

    upper_nibble_hist[upper_nibble][residual] += 1

    if residual == 0:
        exact_matches += 1

# ------------------------------------------------------------

print()
print("=" * 70)
print("MODEL RESULTS")
print("=" * 70)

accuracy = (
    100.0 *
    exact_matches /
    len(frames)
)

print(f"Exact residual=0 : {exact_matches:,}")
print(f"Accuracy         : {accuracy:.4f}%")

# ------------------------------------------------------------

print()
print("=" * 70)
print("TOP RESIDUALS")
print("=" * 70)

for residual, count in residual_hist.most_common(64):

    pct = 100.0 * count / len(frames)

    print(
        f"0x{residual:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------

print()
print("=" * 70)
print("TOP RESIDUAL BY COUNTER")
print("=" * 70)

for counter in range(16):

    if not counter_hist[counter]:
        continue

    residual, count = (
        counter_hist[counter]
        .most_common(1)[0]
    )

    total = sum(
        counter_hist[counter].values()
    )

    pct = (
        100.0 *
        count /
        total
    )

    print(
        f"{counter:X}: "
        f"0x{residual:02X} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------

print()
print("=" * 70)
print("TOP RESIDUAL BY UPPER NIBBLE")
print("=" * 70)

for nibble in sorted(upper_nibble_hist):

    residual, count = (
        upper_nibble_hist[nibble]
        .most_common(1)[0]
    )

    total = sum(
        upper_nibble_hist[nibble].values()
    )

    pct = (
        100.0 *
        count /
        total
    )

    print(
        f"{nibble:X}: "
        f"0x{residual:02X} "
        f"({pct:6.2f}%)"
    )

print()
print("Done.")
print()