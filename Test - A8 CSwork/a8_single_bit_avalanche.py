#!/usr/bin/env python3

import csv
from collections import defaultdict

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

def hamming8(a, b):
    return popcount(a ^ b)

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
                    int(x.strip(), 16)
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

            payload = tuple(frame[2:8])

            families[payload] = family_value

    print()
    print("======================================")
    print("A8 SINGLE BIT AVALANCHE TEST")
    print("======================================")
    print()

    print(
        f"Unique Families: {len(families):,}"
    )

    print()

    payload_set = set(families.keys())

    comparisons = 0

    total_hamming = 0

    per_bit = defaultdict(list)

    # --------------------------------------------------------
    # Search all single-bit neighbors
    # --------------------------------------------------------

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

                ham = hamming8(
                    fv1,
                    fv2
                )

                comparisons += 1

                total_hamming += ham

                per_bit[
                    (byte_idx + 2, bit_idx)
                ].append(ham)

    print(
        f"Single-Bit Neighbors Found: "
        f"{comparisons:,}"
    )

    print()

    if comparisons == 0:

        print(
            "No single-bit neighbors found."
        )

        return

    avg_hamming = (
        total_hamming /
        comparisons
    )

    print(
        f"Average FV Hamming Distance: "
        f"{avg_hamming:.3f}"
    )

    print()

    # ========================================================
    # Per-bit report
    # ========================================================

    results = []

    for key, values in per_bit.items():

        avg = (
            sum(values) /
            len(values)
        )

        results.append(
            (
                avg,
                len(values),
                key[0],
                key[1]
            )
        )

    results.sort(reverse=True)

    print(
        "Most Influential Bits"
    )

    print(
        "---------------------"
    )

    for avg, count, byte_idx, bit_idx in results[:24]:

        print(
            f"Byte{byte_idx} "
            f"Bit{bit_idx} "
            f"Pairs={count:5d} "
            f"AvgHam={avg:.3f}"
        )

    print()

    print(
        "Least Influential Bits"
    )

    print(
        "----------------------"
    )

    for avg, count, byte_idx, bit_idx in results[-24:]:

        print(
            f"Byte{byte_idx} "
            f"Bit{bit_idx} "
            f"Pairs={count:5d} "
            f"AvgHam={avg:.3f}"
        )

    print()

    if avg_hamming > 3.0:

        print(
            "Strong avalanche behavior detected."
        )

    elif avg_hamming > 1.5:

        print(
            "Moderate diffusion behavior detected."
        )

    else:

        print(
            "Weak diffusion behavior detected."
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()