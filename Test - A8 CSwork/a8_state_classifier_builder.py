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

# ============================================================
# Recovered Linear Vectors
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

STATE_MAP = {

    0x25 : "LOW",
    0x5E : "LOW",

    0xF6 : "MID",
    0x8D : "MID",

    0xBC : "HIGH",
    0xC7 : "HIGH",
}

# ============================================================

def build_u64(frame):

    value = 0

    for i,b in enumerate(frame):
        value |= (b << (8*i))

    return value

# ============================================================

def decode(frame):

    raw = build_u64(frame)

    rpm = (
        ((raw >> 48) & 0xFFFF)
        * 0.25
    )

    stat = (
        (raw >> 21) & 0x1FF
    )

    dyn = (
        ((raw >> 30) & 0x3FF)
        - 509
    )

    return rpm, stat, dyn

# ============================================================

def calc_residual(frame):

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
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ============================================================

def state_from_residual(residual):

    return STATE_MAP.get(residual)

# ============================================================

def main():

    print()
    print("Using CSV:")
    print(CSV_FILE)
    print()

    rows = []

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

            residual = calc_residual(frame)

            state = state_from_residual(
                residual
            )

            if state is None:
                continue

            rpm, stat, dyn = decode(frame)

            rows.append(
                (state, rpm, stat, dyn)
            )

    print("=" * 60)
    print("A8 STATE CLASSIFIER BUILDER")
    print("=" * 60)
    print()

    print(f"Frames: {len(rows):,}")
    print()

    for state in ["LOW","MID","HIGH"]:

        subset = [
            x for x in rows
            if x[0] == state
        ]

        rpms = [x[1] for x in subset]
        stats = [x[2] for x in subset]
        dyns = [x[3] for x in subset]

        print(state)
        print("-" * 40)

        print(
            f"RPM  : "
            f"{min(rpms):.0f} .. "
            f"{max(rpms):.0f} "
            f"(avg {sum(rpms)/len(rpms):.0f})"
        )

        print(
            f"STAT : "
            f"{min(stats)} .. "
            f"{max(stats)} "
            f"(avg {sum(stats)/len(stats):.1f})"
        )

        print(
            f"DYN  : "
            f"{min(dyns)} .. "
            f"{max(dyns)} "
            f"(avg {sum(dyns)/len(dyns):.1f})"
        )

        print()

    # --------------------------------------------------------

    print("=" * 60)
    print("SIMPLE THRESHOLD TEST")
    print("=" * 60)
    print()

    correct = 0

    for state, rpm, stat, dyn in rows:

        if rpm < 1500:
            pred = "LOW"
        elif rpm < 2048:
            pred = "MID"
        else:
            pred = "HIGH"

        if pred == state:
            correct += 1

    acc = (
        100.0 * correct / len(rows)
    )

    print(
        f"RPM-only Accuracy: "
        f"{acc:.2f}%"
    )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()