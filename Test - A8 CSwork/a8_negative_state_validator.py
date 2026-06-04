#!/usr/bin/env python3

import csv
from collections import Counter

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0007.csv"
)

COUNTER_TRANSFORM = [
    0x00,0x7A,0x96,0x79,
    0xD5,0xBF,0x8F,0x9A,
    0xB8,0xCA,0x95,0x58,
    0x46,0x55,0xE7,0x68
]

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

LOW_POS  = 0x5E
MID_POS  = 0x8D
HIGH_POS = 0xC7

LOW_NEG  = 0x25
MID_NEG  = 0xF6
HIGH_NEG = 0xBC

NEG_VECTOR = 0x7B

# ----------------------------------------------------------

def build_u64(frame):

    value = 0

    for i,b in enumerate(frame):
        value |= (b << (8*i))

    return value

# ----------------------------------------------------------

def decode_rpm(frame):

    raw = build_u64(frame)

    return ((raw >> 48) & 0xFFFF) * 0.25

# ----------------------------------------------------------

def decode_dyn(frame):

    raw = build_u64(frame)

    return ((raw >> 30) & 0x3FF) - 509

# ----------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

    linear = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx,bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ----------------------------------------------------------

def predict(frame):

    #
    # 0x0D special state
    #

    if (
        frame[2] == 0xDF and
        frame[3] == 0xBF and
        frame[4] == 0xFF
    ):
        return 0x0D

    #
    # 0x1E special state
    #

    if (frame[4] & 0xF0) == 0x90:
        return 0x1E

    rpm   = decode_rpm(frame)
    dyn   = decode_dyn(frame)
    byte7 = frame[7]

    #
    # NEGATIVE STATES
    #

    if dyn <= 2:

        #
        # Byte7-driven LOW_NEG / MID_NEG split
        #

        if byte7 <= 0x17:

            return LOW_NEG

        elif byte7 >= 0x19:

            return MID_NEG

        #
        # Ambiguous transition zone
        #

        else:

            if rpm < 1800:
                return LOW_NEG
            else:
                return MID_NEG

    #
    # POSITIVE STATES
    #

    if rpm < 1600:

        return LOW_POS

    elif rpm < 2050:

        return MID_POS

    else:

        return HIGH_POS

# ----------------------------------------------------------

print()
print("Using CSV:")
print(CSV_FILE)

total = 0
correct = 0

mismatch_counter = Counter()

with open(CSV_FILE,newline="") as csvfile:

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

        predicted = predict(frame)

        total += 1

        if actual == predicted:

            correct += 1

        else:

            mismatch_counter[
                actual ^ predicted
            ] += 1

accuracy = (
    100.0 * correct / total
)

print()
print("=" * 60)
print("A8 NEGATIVE STATE VALIDATOR")
print("=" * 60)

print()
print(f"Frames   : {total:,}")
print(f"Correct  : {correct:,}")
print(f"Accuracy : {accuracy:.4f}%")

print()
print("Top Mismatch XORs")
print("-----------------")

for delta,count in mismatch_counter.most_common(20):

    pct = 100.0 * count / total

    print(
        f"0x{delta:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

print()
print("Done.")