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
# Vectors recovered in Stage 2
# ------------------------------------------------------------

VECTORS_TO_VERIFY = {

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
# Load Frames
# ------------------------------------------------------------

payload_map = {}

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            payload_map[
                frame[1:]
            ] = frame

        except:
            pass

print(
    f"Loaded {len(payload_map):,} payloads"
)

# ------------------------------------------------------------
# Verify each vector
# ------------------------------------------------------------

for (
    byte_idx,
    bit_idx
), expected_delta in VECTORS_TO_VERIFY.items():

    histogram = Counter()

    exception_examples = []

    matches = 0
    total = 0

    for payload, frameA in payload_map.items():

        neighbor = list(payload)

        neighbor[
            byte_idx - 1
        ] ^= (1 << bit_idx)

        neighbor = tuple(neighbor)

        frameB = payload_map.get(neighbor)

        if frameB is None:
            continue

        delta = (
            frameA[0] ^
            frameB[0]
        )

        histogram[delta] += 1

        total += 1

        if delta == expected_delta:
            matches += 1

        elif len(exception_examples) < 10:

            exception_examples.append(
                (
                    delta,
                    frameA,
                    frameB
                )
            )

    if total == 0:
        continue

    confidence = (
        100.0 *
        matches /
        total
    )

    print()
    print("=" * 70)
    print(
        f"b{byte_idx}[{bit_idx}]"
        f" expected=0x{expected_delta:02X}"
    )
    print("=" * 70)

    print(
        f"Matches: {matches:,}"
    )

    print(
        f"Total:   {total:,}"
    )

    print(
        f"Confidence: {confidence:.2f}%"
    )

    print()
    print("Top Deltas")

    for delta, count in histogram.most_common(10):

        pct = (
            100.0 *
            count /
            total
        )

        print(
            f"0x{delta:02X} "
            f"{count:6,d} "
            f"({pct:6.2f}%)"
        )

    if exception_examples:

        print()
        print("Example Exceptions")

        for (
            delta,
            frameA,
            frameB
        ) in exception_examples:

            print(
                f"delta=0x{delta:02X}"
            )

            print(
                "A:",
                " ".join(
                    f"{x:02X}"
                    for x in frameA
                )
            )

            print(
                "B:",
                " ".join(
                    f"{x:02X}"
                    for x in frameB
                )
            )

            print()

print()
print("Done.")
print()