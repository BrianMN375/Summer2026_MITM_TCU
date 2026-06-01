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

def hamming_distance(a, b):

    x = a ^ b

    count = 0

    while x:
        count += x & 1
        x >>= 1

    return count

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    frames = []

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

            counter = (
                frame[1] & 0x0F
            )

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            frames.append(
                (
                    frame,
                    family_value
                )
            )

    print()
    print("======================================")
    print("A8 FAMILY VALUE NEIGHBOR ANALYSIS")
    print("======================================")
    print()

    print(
        f"Frames Loaded: {len(frames):,}"
    )

    print()

    xor_histogram = defaultdict(int)

    hamming_histogram = defaultdict(int)

    total_pairs = 0

    # --------------------------------------------------------
    # Compare neighboring frames
    # --------------------------------------------------------

    for i in range(1, len(frames)):

        prev_frame, prev_fv = frames[i - 1]
        curr_frame, curr_fv = frames[i]

        # Ignore exact duplicates

        if prev_frame[2:8] == curr_frame[2:8]:
            continue

        xor_delta = (
            prev_fv ^ curr_fv
        )

        xor_histogram[
            xor_delta
        ] += 1

        hamming_histogram[
            hamming_distance(
                prev_fv,
                curr_fv
            )
        ] += 1

        total_pairs += 1

    # ========================================================
    # REPORT
    # ========================================================

    print(
        f"Neighbor Comparisons: "
        f"{total_pairs:,}"
    )

    print()

    print(
        "Top XOR Deltas"
    )

    print(
        "--------------"
    )

    ranked = sorted(
        xor_histogram.items(),
        key=lambda x: x[1],
        reverse=True
    )

    for xor_delta, count in ranked[:32]:

        print(
            f"0x{xor_delta:02X} : "
            f"{count:,}"
        )

    print()
    print(
        "Hamming Distance Distribution"
    )

    print(
        "-----------------------------"
    )

    for distance in sorted(
        hamming_histogram.keys()
    ):

        print(
            f"{distance} bits : "
            f"{hamming_histogram[distance]:,}"
        )

    print()

    avg_hamming = (
        sum(
            d * c
            for d, c in hamming_histogram.items()
        )
        /
        total_pairs
    )

    print(
        f"Average Hamming Distance: "
        f"{avg_hamming:.3f}"
    )

    print()

    if avg_hamming > 3.5:

        print(
            "Behavior resembles CRC/hash diffusion."
        )

    else:

        print(
            "Behavior resembles state-table clustering."
        )

    print()

    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()