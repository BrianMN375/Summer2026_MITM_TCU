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
# Load Frames
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
# Build Payload Map
# ------------------------------------------------------------

payload_to_checksum = {}

for frame in frames:

    payload = frame[1:]     # b1..b7
    checksum = frame[0]

    payload_to_checksum[payload] = checksum

print(
    f"Unique payloads: "
    f"{len(payload_to_checksum):,}"
)

# ------------------------------------------------------------
# Search Single-Bit Neighbors
# ------------------------------------------------------------

print()
print("Searching one-bit neighbors...")
print()

bit_vectors = defaultdict(Counter)

processed = 0

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

            processed += 1

            if processed % 100000 == 0:

                print(
                    f"Processed "
                    f"{processed:,}"
                )

print()
print(
    f"Neighbor matches found: "
    f"{processed:,}"
)

# ------------------------------------------------------------
# Generate Ranked Results
# ------------------------------------------------------------

print()
print("=" * 70)
print("DISCOVERED BIT VECTORS")
print("=" * 70)

results = []

for bit, counter in bit_vectors.items():

    total = sum(counter.values())

    delta, count = (
        counter.most_common(1)[0]
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

for (
    confidence,
    total,
    byte_idx,
    bit_idx,
    delta
) in results[:200]:

    print(
        f"b{byte_idx}[{bit_idx}] "
        f"-> 0x{delta:02X} "
        f"matches={total:6,d} "
        f"confidence={confidence:6.2f}%"
    )

# ------------------------------------------------------------
# Export Candidate Vector Table
# ------------------------------------------------------------

print()
print("=" * 70)
print("CANDIDATE VECTOR TABLE")
print("=" * 70)

candidate_vectors = {}

for (
    confidence,
    total,
    byte_idx,
    bit_idx,
    delta
) in results:

    if confidence < 70:
        continue

    if total < 20:
        continue

    candidate_vectors[
        (byte_idx, bit_idx)
    ] = delta

for key in sorted(candidate_vectors):

    print(
        f"({key[0]},{key[1]}): "
        f"0x{candidate_vectors[key]:02X},"
    )

print()
print("Done.")
print()