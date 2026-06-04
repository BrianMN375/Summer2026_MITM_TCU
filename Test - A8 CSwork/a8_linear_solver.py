#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

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

def popcount(x):
    return bin(x).count("1")

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    families = {}

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
            counter = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            payload = tuple(frame[2:8])

            families[payload] = family_value

    print()
    print("======================================")
    print("A8 LINEAR SOLVER")
    print("======================================")
    print()

    print(
        f"Unique Families: {len(families):,}"
    )

    print()

    payload_set = set(
        families.keys()
    )

    # --------------------------------------------------------
    # Collect FV deltas per payload bit
    # --------------------------------------------------------

    bit_deltas = defaultdict(Counter)

    comparisons = 0

    for payload, fv1 in families.items():

        payload_bytes = list(payload)

        for byte_idx in range(6):

            for bit_idx in range(8):

                neighbor = payload_bytes.copy()

                neighbor[byte_idx] ^= (
                    1 << bit_idx
                )

                neighbor = tuple(neighbor)

                if neighbor not in payload_set:
                    continue

                fv2 = families[neighbor]

                delta_fv = (
                    fv1 ^ fv2
                )

                bit_deltas[
                    (byte_idx + 2, bit_idx)
                ][delta_fv] += 1

                comparisons += 1

    print(
        f"Single-bit comparisons: "
        f"{comparisons:,}"
    )

    print()

    print(
        "Per-Bit Delta Analysis"
    )

    print(
        "----------------------"
    )

    for key in sorted(bit_deltas.keys()):

        counter = bit_deltas[key]

        total = sum(
            counter.values()
        )

        most_common_delta, count = (
            counter.most_common(1)[0]
        )

        confidence = (
            100.0 *
            count /
            total
        )

        print(

            f"Byte{key[0]} "
            f"Bit{key[1]}  "

            f"Delta=0x{most_common_delta:02X}  "

            f"Hits={count}/{total}  "

            f"({confidence:.1f}%)"
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()