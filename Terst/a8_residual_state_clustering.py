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
# Known vectors
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

def get_bits(frame):
    value = 0
    for i, b in enumerate(frame):
        value |= (b << (8*i))
    return value

# ============================================================

def decode(frame):

    raw = get_bits(frame)

    tq_limit_dyn = ((raw >> 30) & 0x3FF) - 509

    rpm = (((raw >> 48) & 0xFFFF) * 0.25)

    return rpm, tq_limit_dyn

# ============================================================

def residual(frame):

    checksum = frame[0]
    counter = frame[1] & 0x0F

    fv = checksum ^ COUNTER_TRANSFORM[counter]

    pred = 0

    for byte_idx in range(2,8):

        val = frame[byte_idx]

        for bit_idx in range(8):

            if val & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    pred ^= vec

    return fv ^ pred

# ============================================================

def rpm_bucket(rpm):

    if rpm < 1500:
        return "LOW"

    if rpm < 2500:
        return "MID"

    return "HIGH"

# ============================================================

def dyn_bucket(dyn):

    if dyn < -5:
        return "NEG"

    if dyn > 5:
        return "POS"

    return "ZERO"

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    groups = defaultdict(Counter)

    total = 0

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

            rpm, dyn = decode(frame)

            key = (
                rpm_bucket(rpm),
                dyn_bucket(dyn)
            )

            groups[key][
                residual(frame)
            ] += 1

            total += 1

    print()
    print("====================================")
    print("A8 RESIDUAL STATE CLUSTERING")
    print("====================================")
    print()

    for key in sorted(groups.keys()):

        print()
        print(key)

        counter = groups[key]

        bucket_total = sum(counter.values())

        for res, count in counter.most_common(10):

            pct = (
                100.0 * count /
                bucket_total
            )

            print(
                f"  0x{res:02X} "
                f"{count:7d} "
                f"({pct:6.2f}%)"
            )

    print()
    print(f"Frames: {total:,}")

# ============================================================

if __name__ == "__main__":
    main()