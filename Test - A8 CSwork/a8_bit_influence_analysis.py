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

            frames.append(
                (frame, family_value)
            )

    print()
    print("======================================")
    print("A8 BIT INFLUENCE ANALYSIS")
    print("======================================")
    print()

    print(
        f"Frames Loaded: {len(frames):,}"
    )

    print()

    # --------------------------------------------------------
    # Stats per payload bit
    # --------------------------------------------------------

    stats = {}

    for byte_idx in range(2, 8):

        for bit_idx in range(8):

            stats[(byte_idx, bit_idx)] = {

                "changes": 0,
                "hamming_sum": 0
            }

    # --------------------------------------------------------
    # Compare neighboring frames
    # --------------------------------------------------------

    for i in range(1, len(frames)):

        prev_frame, prev_fv = frames[i - 1]
        curr_frame, curr_fv = frames[i]

        fv_hamming = hamming_distance(
            prev_fv,
            curr_fv
        )

        for byte_idx in range(2, 8):

            delta = (
                prev_frame[byte_idx] ^
                curr_frame[byte_idx]
            )

            for bit_idx in range(8):

                if delta & (1 << bit_idx):

                    s = stats[
                        (byte_idx, bit_idx)
                    ]

                    s["changes"] += 1
                    s["hamming_sum"] += fv_hamming

    # ========================================================
    # REPORT
    # ========================================================

    results = []

    for key, s in stats.items():

        if s["changes"] == 0:
            continue

        avg_hamming = (
            s["hamming_sum"]
            /
            s["changes"]
        )

        results.append(

            (
                avg_hamming,
                s["changes"],
                key[0],
                key[1]
            )
        )

    results.sort(
        reverse=True
    )

    print(
        "Top Influential Bits"
    )

    print(
        "--------------------"
    )

    for avg_hamming, changes, byte_idx, bit_idx in results[:32]:

        print(

            f"Byte{byte_idx} "
            f"Bit{bit_idx}  "
            f"Changes={changes:6d}  "
            f"AvgFVHam={avg_hamming:.3f}"
        )

    print()

    print(
        "Least Influential Bits"
    )

    print(
        "----------------------"
    )

    for avg_hamming, changes, byte_idx, bit_idx in results[-32:]:

        print(

            f"Byte{byte_idx} "
            f"Bit{bit_idx}  "
            f"Changes={changes:6d}  "
            f"AvgFVHam={avg_hamming:.3f}"
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()