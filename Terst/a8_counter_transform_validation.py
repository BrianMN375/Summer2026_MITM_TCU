#!/usr/bin/env python3

import csv
from collections import defaultdict

# ============================================================
# Candidate Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    families = defaultdict(set)

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
                    int(x.strip(), 16)
                    for x in row[1:9]
                ]

            except:
                continue

            frames += 1

            checksum = frame[0]

            counter = (
                frame[1] & 0x0F
            )

            candidate_seed = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            # Ignore counter nibble
            family = (
                frame[1] & 0xF0,
                frame[2],
                frame[3],
                frame[4],
                frame[5],
                frame[6],
                frame[7]
            )

            families[family].add(
                candidate_seed
            )

    # ========================================================

    tested = 0
    perfect = 0

    worst_family = None
    worst_count = 0

    for family, seeds in families.items():

        if len(seeds) < 1:
            continue

        tested += 1

        if len(seeds) == 1:

            perfect += 1

        if len(seeds) > worst_count:

            worst_count = len(seeds)
            worst_family = family

    print()
    print("====================================")
    print("COUNTER TRANSFORM VALIDATION")
    print("====================================")
    print()

    print(
        f"Frames: {frames:,}"
    )

    print(
        f"Families: {tested:,}"
    )

    print(
        f"Perfect Families: {perfect:,}"
    )

    print(
        f"Success Rate: "
        f"{100.0 * perfect / tested:.2f}%"
    )

    print()

    print(
        f"Worst Family Seed Count: "
        f"{worst_count}"
    )

    if worst_family:

        print(
            "Worst Family:"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in worst_family
            )
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()