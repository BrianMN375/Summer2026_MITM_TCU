#!/usr/bin/env python3

import csv
from collections import defaultdict

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

def report(name, mapping):

    unique_combos = len(mapping)

    avg_values = (
        sum(len(v) for v in mapping.values())
        / unique_combos
    )

    perfect = sum(
        1 for v in mapping.values()
        if len(v) == 1
    )

    print(name)
    print(f"  Unique Combos      : {unique_combos:,}")
    print(f"  Avg FamilyValues   : {avg_values:.4f}")
    print(f"  Perfect Combos     : {perfect:,}")
    print(f"  Perfect Rate       : {100.0 * perfect / unique_combos:.2f}%")
    print()

def main():

    filename = input("A8 CSV file: ").strip()

    combos = {
        "b3,b4,b6,b7"       : defaultdict(set),
        "b3,b4,b5,b6,b7"    : defaultdict(set),
        "b2,b3,b4,b6,b7"    : defaultdict(set),
        "b2,b3,b4,b5,b6,b7" : defaultdict(set)
    }

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

            b2 = frame[2]
            b3 = frame[3]
            b4 = frame[4]
            b5 = frame[5]
            b6 = frame[6]
            b7 = frame[7]

            combos["b3,b4,b6,b7"][
                (b3,b4,b6,b7)
            ].add(family_value)

            combos["b3,b4,b5,b6,b7"][
                (b3,b4,b5,b6,b7)
            ].add(family_value)

            combos["b2,b3,b4,b6,b7"][
                (b2,b3,b4,b6,b7)
            ].add(family_value)

            combos["b2,b3,b4,b5,b6,b7"][
                (b2,b3,b4,b5,b6,b7)
            ].add(family_value)

    print()
    print("======================================")
    print("A8 FAMILY VALUE REFINEMENT")
    print("======================================")
    print()
    print(f"Frames: {frames:,}")
    print()

    for name, mapping in combos.items():
        report(name, mapping)

if __name__ == "__main__":
    main()