#!/usr/bin/env python3

import csv
from collections import Counter

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
# Recovered Columns
# ============================================================

COLUMNS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,0):0x54,
    (3,1):0xA8,
    (3,2):0x7F,
    (3,3):0xFE,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,

    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ============================================================

def calc_family_value_partial(frame):

    fv = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = COLUMNS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    fv ^= vec

    return fv

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    residuals = Counter()

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
                    int(x.strip(),16)
                    for x in row[1:9]
                ]

            except:
                continue

            checksum = frame[0]
            counter  = frame[1] & 0x0F

            observed_fv = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            predicted_fv = (
                calc_family_value_partial(frame)
            )

            residual = (
                observed_fv ^
                predicted_fv
            )

            residuals[residual] += 1

            frames += 1

    print()
    print("======================================")
    print("A8 LINEAR VALIDATOR")
    print("======================================")
    print()

    print(
        f"Frames: {frames:,}"
    )

    print()

    print(
        f"Unique Residuals: "
        f"{len(residuals)}"
    )

    print()

    print(
        "Top Residuals"
    )

    print(
        "-------------"
    )

    for residual, count in residuals.most_common(32):

        pct = (
            100.0 * count / frames
        )

        print(
            f"0x{residual:02X} "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

    print()

    # --------------------------------------------------------
    # Residual XOR relationships
    # --------------------------------------------------------

    top = [
        x[0]
        for x in residuals.most_common(8)
    ]

    print(
        "Residual XOR Matrix"
    )

    print(
        "-------------------"
    )

    for i in range(len(top)):

        for j in range(i+1, len(top)):

            print(
                f"0x{top[i]:02X} ^ "
                f"0x{top[j]:02X} = "
                f"0x{top[i]^top[j]:02X}"
            )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()