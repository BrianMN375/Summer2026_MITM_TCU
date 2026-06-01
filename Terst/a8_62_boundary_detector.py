#!/usr/bin/env python3

import csv
from collections import defaultdict

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

# ------------------------------------------------------------

def build_u64(frame):

    value = 0

    for i,b in enumerate(frame):
        value |= (b << (8*i))

    return value

# ------------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ------------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

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

# ------------------------------------------------------------

stat_bins = defaultdict(lambda: [0,0])

frames = 0

print()
print("Loading dataset...")

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

        residual = actual_residual(frame)

        if residual not in (0x62, 0x38):
            continue

        rpm, stat, dyn = decode_signals(frame)

        if residual == 0x62:
            stat_bins[stat][0] += 1
        else:
            stat_bins[stat][1] += 1

        frames += 1

# ------------------------------------------------------------

print()
print("=" * 92)
print("A8 0x62 BOUNDARY DETECTOR")
print("=" * 92)

print()
print(f"Relevant Frames: {frames:,}")

print()
print(
    f"{'STAT':<10}"
    f"{'0x62':>10}"
    f"{'0x38':>10}"
    f"{'%62':>10}"
    f"{'%38':>10}"
    f"{'Dominant':>12}"
)

print("-" * 92)

for stat in sorted(stat_bins):

    a62 = stat_bins[stat][0]
    a38 = stat_bins[stat][1]

    total = a62 + a38

    pct62 = 100.0 * a62 / total
    pct38 = 100.0 * a38 / total

    dominant = "0x62" if a62 > a38 else "0x38"

    print(
        f"{stat:<10d}"
        f"{a62:10d}"
        f"{a38:10d}"
        f"{pct62:9.1f}%"
        f"{pct38:9.1f}%"
        f"{dominant:>12}"
    )

print()
print("Done.")