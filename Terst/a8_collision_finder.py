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

    combos = defaultdict(list)

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

                timestamp = row[0]

            except:
                continue

            frames += 1

            checksum = frame[0]
            counter = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            key = (
                frame[2],  # b2
                frame[3],  # b3
                frame[4],  # b4
                frame[6],  # b6
                frame[7]   # b7
            )

            combos[key].append({
                "timestamp": timestamp,
                "family_value": family_value,
                "frame": frame
            })

    # ========================================================
    # Find collisions
    # ========================================================

    collision_count = 0

    print()
    print("======================================")
    print("A8 COLLISION FINDER")
    print("======================================")
    print()

    for key, entries in combos.items():

        values = {
            e["family_value"]
            for e in entries
        }

        if len(values) <= 1:
            continue

        collision_count += 1

        print()
        print(
            f"COLLISION #{collision_count}"
        )

        print(
            f"Key = "
            f"{' '.join(f'{x:02X}' for x in key)}"
        )

        print(
            f"Family Values = "
            f"{', '.join(f'0x{x:02X}' for x in sorted(values))}"
        )

        print()

        for e in entries[:20]:

            frame_str = " ".join(
                f"{x:02X}"
                for x in e["frame"]
            )

            print(
                f"{e['timestamp']:>10} "
                f"FV=0x{e['family_value']:02X} "
                f"{frame_str}"
            )

    print()
    print(
        f"Total Collisions: "
        f"{collision_count}"
    )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()