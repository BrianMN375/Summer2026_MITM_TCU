#!/usr/bin/env python3

import csv

# ============================================================
# Fixed Dataset
# ============================================================

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

# ============================================================
# Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00,0x7A,0x96,0x79,
    0xD5,0xBF,0x8F,0x9A,
    0xB8,0xCA,0x95,0x58,
    0x46,0x55,0xE7,0x68
]

# ============================================================
# Linear Vectors
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

LOW_VECTOR  = 0x5E
MID_VECTOR  = 0x8D
HIGH_VECTOR = 0xC7

DYN_NEG_VECTOR = 0x7B

# ============================================================

def build_u64(frame):

    v = 0

    for i,b in enumerate(frame):
        v |= (b << (8*i))

    return v

# ============================================================

def decode(frame):

    raw = build_u64(frame)

    rpm = (
        ((raw >> 48) & 0xFFFF)
        * 0.25
    )

    dyn = (
        ((raw >> 30) & 0x3FF)
        - 509
    )

    return rpm, dyn

# ============================================================

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

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

# ============================================================

print()
print("Loading dataset...")
print()

dataset = []

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

        rpm,dyn = decode(frame)

        dataset.append(
            (
                rpm,
                dyn,
                actual_residual(frame)
            )
        )

print(
    f"Loaded {len(dataset):,} frames"
)

print()

# ============================================================
# Search Space
# ============================================================

best_accuracy = 0
best_low_mid = 1500
best_mid_high = 2048
best_dyn = 0

tests = 0

for low_mid in range(1200,1901,25):

    for mid_high in range(1900,2501,25):

        if mid_high <= low_mid:
            continue

        for dyn_threshold in range(-5,11):

            correct = 0

            for rpm,dyn,actual in dataset:

                if rpm < low_mid:

                    pred = LOW_VECTOR

                elif rpm < mid_high:

                    pred = MID_VECTOR

                else:

                    pred = HIGH_VECTOR

                if dyn <= dyn_threshold:

                    pred ^= DYN_NEG_VECTOR

                if pred == actual:

                    correct += 1

            accuracy = (
                correct * 100.0
                / len(dataset)
            )

            tests += 1

            if accuracy > best_accuracy:

                best_accuracy = accuracy

                best_low_mid = low_mid
                best_mid_high = mid_high
                best_dyn = dyn_threshold

                print(
                    f"NEW BEST: "
                    f"{accuracy:.4f}%"
                    f"  low_mid={low_mid}"
                    f"  mid_high={mid_high}"
                    f"  dyn={dyn_threshold}"
                )

print()
print("=" * 60)
print("BEST RESULT")
print("=" * 60)
print()

print(
    f"Accuracy   : {best_accuracy:.4f}%"
)

print(
    f"LOW/MID    : {best_low_mid}"
)

print(
    f"MID/HIGH   : {best_mid_high}"
)

print(
    f"DYN THRESH : {best_dyn}"
)

print()
print(
    f"Tests Run: {tests:,}"
)