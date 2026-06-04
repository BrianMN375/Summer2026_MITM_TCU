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
# Known recovered columns
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

RPM_LOW  = 0x5E
RPM_MID  = 0x8D
RPM_HIGH = 0xC7

DYN_NEG = 0x7B

# ============================================================

def get_bits(frame):

    value = 0

    for i,b in enumerate(frame):
        value |= (b << (8*i))

    return value

# ============================================================

def decode(frame):

    raw = get_bits(frame)

    dyn = ((raw >> 30) & 0x3FF) - 509
    rpm = (((raw >> 48) & 0xFFFF) * 0.25)

    return rpm, dyn

# ============================================================

def rpm_bucket(rpm):

    if rpm < 1500:
        return RPM_LOW

    if rpm < 2500:
        return RPM_MID

    return RPM_HIGH

# ============================================================

def predicted_residual(frame):

    rpm, dyn = decode(frame)

    residual = rpm_bucket(rpm)

    if dyn < -5:
        residual ^= DYN_NEG

    return residual

# ============================================================

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    fv = checksum ^ COUNTER_TRANSFORM[counter]

    pred = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx,bit_idx)
                )

                if vec is not None:
                    pred ^= vec

    return fv ^ pred

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    total = 0
    correct = 0

    mismatch_counter = Counter()

    with open(filename,newline='') as csvfile:

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

            actual = actual_residual(frame)
            predicted = predicted_residual(frame)

            total += 1

            if actual == predicted:
                correct += 1
            else:
                mismatch_counter[
                    actual ^ predicted
                ] += 1

    print()
    print("======================================")
    print("A8 STATE MODEL VALIDATOR")
    print("======================================")
    print()

    print(f"Frames: {total:,}")
    print(f"Correct: {correct:,}")

    accuracy = (
        100.0 * correct / total
    )

    print(
        f"Accuracy: {accuracy:.2f}%"
    )

    print()

    print(
        "Top Mismatch XORs"
    )

    print(
        "-----------------"
    )

    for delta,count in mismatch_counter.most_common(20):

        pct = (
            100.0 * count / total
        )

        print(
            f"0x{delta:02X} "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()