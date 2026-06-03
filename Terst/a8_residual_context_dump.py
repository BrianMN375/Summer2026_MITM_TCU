#!/usr/bin/env python3

import csv

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0012.csv"
)

# ============================================================
# COUNTER TRANSFORM
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# LINEAR MODEL
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

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value


def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn


def residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    linear = 0

    for byte_idx in range(2, 8):

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

print()
print("Loading dataset...")

rows = []

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for row in reader:

        try:

            timestamp = int(row[0])

            frame = [
                int(x, 16)
                for x in row[1:9]
            ]

        except:
            continue

        rows.append({

            "time": timestamp,
            "frame": frame,
            "residual": residual(frame)

        })

# ============================================================

print()
print("=" * 80)
print("A8 RESIDUAL CONTEXT DUMP")
print("=" * 80)

for target in (0x1E, 0xC0):

    print()
    print("=" * 70)
    print(f"RESIDUAL 0x{target:02X}")
    print("=" * 70)

    count = 0

    for idx in range(1, len(rows) - 1):

        current = rows[idx]

        if current["residual"] != target:
            continue

        count += 1

        prev_row = rows[idx - 1]
        next_row = rows[idx + 1]

        frame = current["frame"]

        rpm, stat, dyn = decode_signals(frame)

        print()
        print(
            f"Time : {current['time']}"
        )

        print(
            f"Prev : 0x{prev_row['residual']:02X}"
        )

        print(
            f"Curr : 0x{current['residual']:02X}"
        )

        print(
            f"Next : 0x{next_row['residual']:02X}"
        )

        print()

        print(
            f"RPM  : {rpm:.1f}"
        )

        print(
            f"STAT : {stat}"
        )

        print(
            f"DYN  : {dyn}"
        )

        print()

        print(
            "DATA : "
            + " ".join(
                f"{b:02X}"
                for b in frame
            )
        )

    print()
    print(
        f"Total Frames : {count}"
    )

print()
print("Done.")