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

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    family_counts = defaultdict(int)

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
            counter  = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            family_counts[family_value] += 1

    # ========================================================

    print()
    print("======================================")
    print("A8 FAMILY VALUE DISTRIBUTION")
    print("======================================")
    print()

    print(f"Frames: {frames:,}")
    print(f"Unique Family Values: {len(family_counts)}")
    print()

    counts = list(family_counts.values())

    print(f"Min Count : {min(counts)}")
    print(f"Max Count : {max(counts)}")
    print(f"Avg Count : {sum(counts)/len(counts):.2f}")
    print()

    print("Top 32 Most Common")
    print("------------------")

    ranked = sorted(
        family_counts.items(),
        key=lambda x: x[1],
        reverse=True
    )

    for fv, count in ranked[:32]:

        print(
            f"0x{fv:02X} : {count:,}"
        )

    print()
    print("Bottom 32 Least Common")
    print("----------------------")

    ranked = sorted(
        family_counts.items(),
        key=lambda x: x[1]
    )

    for fv, count in ranked[:32]:

        print(
            f"0x{fv:02X} : {count:,}"
        )

    # --------------------------------------------------------
    # Uniformity Metric
    # --------------------------------------------------------

    avg = sum(counts) / len(counts)

    deviation = (
        sum(abs(c - avg) for c in counts)
        / len(counts)
    )

    print()
    print(
        f"Average Absolute Deviation: "
        f"{deviation:.2f}"
    )

    print()

    if deviation < avg * 0.20:

        print(
            "Distribution appears relatively uniform."
        )

    elif deviation < avg * 0.50:

        print(
            "Distribution shows moderate clustering."
        )

    else:

        print(
            "Distribution strongly clustered."
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()