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

    frames = 0

    # --------------------------------------------------------
    # Combination -> FamilyValue
    # --------------------------------------------------------

    combo_b3b4 = defaultdict(set)
    combo_b6b7 = defaultdict(set)

    combo_b3b4b6b7 = defaultdict(set)

    combo_full = defaultdict(set)

    # --------------------------------------------------------

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

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            b3 = frame[3]
            b4 = frame[4]

            b6 = frame[6]
            b7 = frame[7]

            # ------------------------------------------------

            combo_b3b4[
                (b3,b4)
            ].add(family_value)

            combo_b6b7[
                (b6,b7)
            ].add(family_value)

            combo_b3b4b6b7[
                (b3,b4,b6,b7)
            ].add(family_value)

            combo_full[
                (
                    frame[2],
                    frame[3],
                    frame[4],
                    frame[5],
                    frame[6],
                    frame[7]
                )
            ].add(family_value)

    # ========================================================
    # REPORT
    # ========================================================

    print()
    print("======================================")
    print("A8 FAMILY VALUE ANALYSIS")
    print("======================================")
    print()

    print(
        f"Frames: {frames:,}"
    )

    print()

    def report(name, mapping):

        unique_combos = len(mapping)

        avg_values = (
            sum(
                len(v)
                for v in mapping.values()
            )
            /
            unique_combos
        )

        perfect = sum(
            1
            for v in mapping.values()
            if len(v) == 1
        )

        print(name)

        print(
            f"  Unique Combos : "
            f"{unique_combos:,}"
        )

        print(
            f"  Avg FamilyValues : "
            f"{avg_values:.2f}"
        )

        print(
            f"  Perfect Combos : "
            f"{perfect:,}"
        )

        print(
            f"  Perfect Rate : "
            f"{100.0 * perfect / unique_combos:.2f}%"
        )

        print()

    report(
        "(b3,b4)",
        combo_b3b4
    )

    report(
        "(b6,b7)",
        combo_b6b7
    )

    report(
        "(b3,b4,b6,b7)",
        combo_b3b4b6b7
    )

    report(
        "(b2,b3,b4,b5,b6,b7)",
        combo_full
    )

    print("Analysis Complete.")

# ============================================================

if __name__ == "__main__":
    main()