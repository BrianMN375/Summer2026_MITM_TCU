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
# Main
# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    frame_count = 0

    byte_maps = [
        defaultdict(set),  # b2
        defaultdict(set),  # b3
        defaultdict(set),  # b4
        defaultdict(set),  # b5
        defaultdict(set),  # b6
        defaultdict(set),  # b7
    ]

    pair_maps = {
        "b2_b3": defaultdict(set),
        "b3_b4": defaultdict(set),
        "b4_b5": defaultdict(set),
        "b5_b6": defaultdict(set),
        "b6_b7": defaultdict(set),
    }

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

            frame_count += 1

            checksum = frame[0]

            counter = (
                frame[1] & 0x0F
            )

            seed = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            # --------------------------------------------------
            # Individual bytes
            # --------------------------------------------------

            b2 = frame[2]
            b3 = frame[3]
            b4 = frame[4]
            b5 = frame[5]
            b6 = frame[6]
            b7 = frame[7]

            byte_maps[0][b2].add(seed)
            byte_maps[1][b3].add(seed)
            byte_maps[2][b4].add(seed)
            byte_maps[3][b5].add(seed)
            byte_maps[4][b6].add(seed)
            byte_maps[5][b7].add(seed)

            # --------------------------------------------------
            # Adjacent pairs
            # --------------------------------------------------

            pair_maps["b2_b3"][(b2,b3)].add(seed)
            pair_maps["b3_b4"][(b3,b4)].add(seed)
            pair_maps["b4_b5"][(b4,b5)].add(seed)
            pair_maps["b5_b6"][(b5,b6)].add(seed)
            pair_maps["b6_b7"][(b6,b7)].add(seed)

    # ========================================================
    # REPORT
    # ========================================================

    print()
    print("======================================")
    print("A8 BYTE INFLUENCE ANALYSIS")
    print("======================================")
    print()

    print(
        f"Frames: {frame_count:,}"
    )
    print()

    # --------------------------------------------------------

    for idx, mapping in enumerate(byte_maps):

        unique_values = len(mapping)

        avg_seeds = (
            sum(len(v)
                for v in mapping.values())
            /
            unique_values
        )

        print(
            f"Byte{idx+2}"
        )

        print(
            f"  Unique Values : "
            f"{unique_values:,}"
        )

        print(
            f"  Avg Seeds/Value : "
            f"{avg_seeds:.2f}"
        )

        print()

    # --------------------------------------------------------

    print("PAIR ANALYSIS")
    print("-------------")
    print()

    for name, mapping in pair_maps.items():

        unique_pairs = len(mapping)

        avg_seeds = (
            sum(len(v)
                for v in mapping.values())
            /
            unique_pairs
        )

        print(
            f"{name}"
        )

        print(
            f"  Unique Pairs : "
            f"{unique_pairs:,}"
        )

        print(
            f"  Avg Seeds/Pair : "
            f"{avg_seeds:.2f}"
        )

        print()

    print(
        "Analysis Complete."
    )

# ============================================================

if __name__ == "__main__":
    main()