#!/usr/bin/env python3

import csv
from collections import Counter

# ============================================================
# Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# Discovered Bit Vectors
# ============================================================

BIT_VECTORS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,0):0x54,
    (3,1):0xA8,
    (3,2):0x7F,
    (3,3):0xFE,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,

    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    base_counter = Counter()

    frames = 0

    with open(filename, newline='') as csvfile:

        reader = csv.reader(csvfile)

        for row in reader:

            if not row:
                continue

            if row[0].lower() == "time":
                continue

            try:

                frame = [
                    int(x.strip(),16)
                    for x in row[1:9]
                ]

            except:
                continue

            checksum = frame[0]
            counter  = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            predicted = 0

            # ------------------------------------------------
            # XOR active vectors
            # ------------------------------------------------

            for byte_idx in range(2,8):

                value = frame[byte_idx]

                for bit_idx in range(8):

                    if not (
                        value & (1 << bit_idx)
                    ):
                        continue

                    vector = BIT_VECTORS.get(
                        (byte_idx, bit_idx)
                    )

                    if vector is not None:

                        predicted ^= vector

            candidate_base = (
                family_value ^
                predicted
            )

            base_counter[
                candidate_base
            ] += 1

            frames += 1

    # ========================================================

    print()
    print("======================================")
    print("A8 BASE DISCOVERY")
    print("======================================")
    print()

    print(
        f"Frames analyzed: {frames:,}"
    )

    print()

    print(
        "Top Candidate Bases"
    )

    print(
        "-------------------"
    )

    for base, count in (
        base_counter.most_common(20)
    ):

        pct = (
            100.0 * count / frames
        )

        print(
            f"0x{base:02X}  "
            f"{count:,} frames  "
            f"({pct:.2f}%)"
        )

    print()

    print(
        f"Unique Bases: "
        f"{len(base_counter)}"
    )

    print()

    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()