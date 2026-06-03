#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

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
# States to EXCLUDE
# ------------------------------------------------------------

EXCLUDED_UPPER_NIBBLES = {
    0x1,
    0xA,
}

# ------------------------------------------------------------
# Load filtered frames
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

            upper = frame[1] >> 4

            if upper in EXCLUDED_UPPER_NIBBLES:
                continue

            frames.append(frame)

        except:
            pass

print(f"Filtered Frames: {len(frames):,}")
print()

# ------------------------------------------------------------
# Build payload map
# ------------------------------------------------------------

payload_to_checksum = {}

for frame in frames:

    payload = frame[1:]
    checksum = frame[0]

    payload_to_checksum[payload] = checksum

print(
    f"Unique payloads: "
    f"{len(payload_to_checksum):,}"
)

# ------------------------------------------------------------
# Stage 2 Re-run
# Bit Vector Discovery
# ------------------------------------------------------------

print()
print("=" * 70)
print("FILTERED BIT VECTOR DISCOVERY")
print("=" * 70)

bit_vectors = defaultdict(Counter)

neighbor_matches = 0

for payload, checksum in payload_to_checksum.items():

    payload_list = list(payload)

    for byte_idx in range(7):

        for bit_idx in range(8):

            neighbor = payload_list.copy()

            neighbor[byte_idx] ^= (
                1 << bit_idx
            )

            neighbor = tuple(neighbor)

            if neighbor not in payload_to_checksum:
                continue

            checksum2 = payload_to_checksum[
                neighbor
            ]

            delta = checksum ^ checksum2

            bit_vectors[
                (byte_idx + 1, bit_idx)
            ][delta] += 1

            neighbor_matches += 1

print(
    f"Neighbor matches: "
    f"{neighbor_matches:,}"
)

print()

results = []

for bit, hist in bit_vectors.items():

    total = sum(hist.values())

    delta, count = (
        hist.most_common(1)[0]
    )

    confidence = (
        100.0 *
        count /
        total
    )

    results.append(
        (
            confidence,
            total,
            bit[0],
            bit[1],
            delta
        )
    )

results.sort(reverse=True)

print("Top Bit Vectors")
print("----------------")

for (
    confidence,
    total,
    byte_idx,
    bit_idx,
    delta
) in results[:80]:

    print(
        f"b{byte_idx}[{bit_idx}] "
        f"-> 0x{delta:02X} "
        f"matches={total:6,d} "
        f"confidence={confidence:6.2f}%"
    )

# ------------------------------------------------------------
# Build candidate table
# ------------------------------------------------------------

candidate_vectors = {}

for (
    confidence,
    total,
    byte_idx,
    bit_idx,
    delta
) in results:

    if confidence < 85:
        continue

    if total < 20:
        continue

    if delta == 0:
        continue

    candidate_vectors[
        (byte_idx, bit_idx)
    ] = delta

# ------------------------------------------------------------
# Linear model
# ------------------------------------------------------------

print()
print("=" * 70)
print("FILTERED LINEAR MODEL")
print("=" * 70)

residual_hist = Counter()

exact = 0

for frame in frames:

    observed = frame[0]

    payload = frame[1:]

    predicted = 0

    for byte_idx in range(1, 8):

        value = payload[byte_idx - 1]

        for bit_idx in range(8):

            if not (
                value & (1 << bit_idx)
            ):
                continue

            vector = candidate_vectors.get(
                (byte_idx, bit_idx)
            )

            if vector is not None:

                predicted ^= vector

    residual = observed ^ predicted

    residual_hist[residual] += 1

    if residual == 0:
        exact += 1

accuracy = (
    100.0 *
    exact /
    len(frames)
)

print()
print(
    f"Exact residual=0 : "
    f"{exact:,}"
)

print(
    f"Accuracy         : "
    f"{accuracy:.4f}%"
)

print()

print("Top Residuals")
print("-------------")

for residual, count in residual_hist.most_common(40):

    pct = (
        100.0 *
        count /
        len(frames)
    )

    print(
        f"0x{residual:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------

print()
print("Candidate Vector Table")
print("----------------------")

for key in sorted(candidate_vectors):

    print(
        f"({key[0]},{key[1]}): "
        f"0x{candidate_vectors[key]:02X},"
    )

print()
print("Done.")
print()