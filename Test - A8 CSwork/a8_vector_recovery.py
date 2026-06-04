#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

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
# Known vectors recovered so far
# ============================================================

KNOWN_VECTORS = {

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

def compute_residual(frame):

    checksum = frame[0]
    counter = frame[1] & 0x0F

    family_value = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    predicted = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    predicted ^= vec

    return family_value ^ predicted

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

            residual = compute_residual(frame)

            frames.append(
                (frame, residual)
            )

    print()
    print("======================================")
    print("A8 VECTOR RECOVERY")
    print("======================================")
    print()

    # --------------------------------------------------------
    # Determine unknown bits
    # --------------------------------------------------------

    unknown_bits = []

    for byte_idx in range(2,8):

        for bit_idx in range(8):

            if (byte_idx, bit_idx) not in KNOWN_VECTORS:

                unknown_bits.append(
                    (byte_idx, bit_idx)
                )

    print(
        f"Unknown bits: {len(unknown_bits)}"
    )

    print()

    # --------------------------------------------------------
    # Analyze each unknown bit
    # --------------------------------------------------------

    for byte_idx, bit_idx in unknown_bits:

        bit0 = Counter()
        bit1 = Counter()

        for frame, residual in frames:

            value = frame[byte_idx]

            if value & (1 << bit_idx):

                bit1[residual] += 1

            else:

                bit0[residual] += 1

        if not bit0 or not bit1:
            continue

        res0, cnt0 = bit0.most_common(1)[0]
        res1, cnt1 = bit1.most_common(1)[0]

        total0 = sum(bit0.values())
        total1 = sum(bit1.values())

        conf0 = 100.0 * cnt0 / total0
        conf1 = 100.0 * cnt1 / total1

        # Only print strong candidates

        if conf0 < 70 and conf1 < 70:
            continue

        vector = res0 ^ res1

        print(
            f"Byte{byte_idx} Bit{bit_idx}"
        )

        print(
            f"  bit=0 -> 0x{res0:02X} "
            f"({conf0:.1f}%)"
        )

        print(
            f"  bit=1 -> 0x{res1:02X} "
            f"({conf1:.1f}%)"
        )

        print(
            f"  Candidate Vector = "
            f"0x{vector:02X}"
        )

        print()

    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()