#!/usr/bin/env python3

import csv
from collections import Counter

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

LOW_VECTOR  = 0x5E
MID_VECTOR  = 0x8D
HIGH_VECTOR = 0xC7

DYN_NEG_VECTOR = 0x7B

LOW_MID_RPM  = 1600
MID_HIGH_RPM = 2050
DYN_THRESH   = 2

# ============================================================

def build_u64(frame):

    v = 0

    for i,b in enumerate(frame):
        v |= (b << (8*i))

    return v

# ============================================================

def decode(frame):

    raw = build_u64(frame)

    rpm = ((raw >> 48) & 0xFFFF) * 0.25
    dyn = ((raw >> 30) & 0x3FF) - 509

    return rpm,dyn

# ============================================================

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

    linear = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get((byte_idx,bit_idx))

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ============================================================

def predicted_residual(frame):

    rpm,dyn = decode(frame)

    # 0x0D

    if (
        frame[2] == 0xDF and
        frame[3] == 0xBF and
        frame[4] == 0xFF
    ):
        return 0x0D

    # 0x1E

    if (frame[4] & 0xF0) == 0x90:
        return 0x1E

    if rpm < LOW_MID_RPM:
        residual = LOW_VECTOR

    elif rpm < MID_HIGH_RPM:
        residual = MID_VECTOR

    else:
        residual = HIGH_VECTOR

    if dyn <= DYN_THRESH:
        residual ^= DYN_NEG_VECTOR

    return residual

# ============================================================

def print_histogram(name, values):

    print()
    print(name)
    print("-" * len(name))

    counter = Counter(values)

    total = len(values)

    for value,count in counter.most_common(20):

        pct = 100.0 * count / total

        print(
            f"0x{value:02X} "
            f"{count:6d} "
            f"({pct:6.2f}%)"
        )

# ============================================================

low_b4 = []
low_b6 = []
low_b7 = []

mid_b4 = []
mid_b6 = []
mid_b7 = []

d3_b4 = []
d3_b6 = []
d3_b7 = []

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
        pred   = predicted_residual(frame)

        if actual == pred:

            if actual == 0x5E:

                low_b4.append(frame[4])
                low_b6.append(frame[6])
                low_b7.append(frame[7])

            elif actual == 0x8D:

                mid_b4.append(frame[4])
                mid_b6.append(frame[6])
                mid_b7.append(frame[7])

        else:

            if (actual ^ pred) == 0xD3:

                d3_b4.append(frame[4])
                d3_b6.append(frame[6])
                d3_b7.append(frame[7])

# ============================================================

print()
print("=" * 60)
print("LOW (0x5E)")
print("=" * 60)

print_histogram("Byte4", low_b4)
print_histogram("Byte6", low_b6)
print_histogram("Byte7", low_b7)

print()
print("=" * 60)
print("MID (0x8D)")
print("=" * 60)

print_histogram("Byte4", mid_b4)
print_histogram("Byte6", mid_b6)
print_histogram("Byte7", mid_b7)

print()
print("=" * 60)
print("D3 MISMATCHES")
print("=" * 60)

print_histogram("Byte4", d3_b4)
print_histogram("Byte6", d3_b6)
print_histogram("Byte7", d3_b7)

print()
print("Done.")