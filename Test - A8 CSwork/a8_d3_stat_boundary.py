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
    (2,5):0x9B,(2,6):0x19,(2,7):0x32,
    (3,0):0x54,(3,1):0xA8,(3,2):0x7F,(3,3):0xFE,
    (3,6):0x3D,(3,7):0x7A,
    (4,0):0xAD,(4,1):0x75,(4,2):0xEA,(4,3):0xFB,
    (6,1):0x1C,(6,2):0x38,(6,3):0x70,(6,4):0xE0,
    (6,5):0xEF,(6,6):0xF1,(6,7):0xCD,
    (7,0):0xE9,(7,1):0xFD,(7,2):0xD5,(7,3):0x85,
    (7,4):0x25,
}

# ----------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ----------------------------------------------------------

def decode_rpm(frame):

    raw = build_u64(frame)

    return ((raw >> 48) & 0xFFFF) * 0.25

# ----------------------------------------------------------

def decode_stat(frame):

    raw = build_u64(frame)

    return (raw >> 21) & 0x1FF

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
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ----------------------------------------------------------

stat_bins = defaultdict(
    lambda: {
        "25": 0,
        "F6": 0
    }
)

frames_used = 0

print()
print("Loading dataset...")

with open(CSV_FILE, newline="") as csvfile:

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

        #
        # Only LOW_NEG / MID_NEG
        #

        if residual not in (0x25, 0xF6):
            continue

        rpm = decode_rpm(frame)

        #
        # Same transition window discovered
        #

        if rpm < 1500 or rpm > 1700:
            continue

        stat = decode_stat(frame)

        if residual == 0x25:
            stat_bins[stat]["25"] += 1
        else:
            stat_bins[stat]["F6"] += 1

        frames_used += 1

# ----------------------------------------------------------

print()
print("=" * 92)
print("A8 D3 STAT BOUNDARY")
print("=" * 92)

print()
print(f"Relevant Frames: {frames_used:,}")

print()
print(
    f"{'STAT':<8}"
    f"{'0x25':>10}"
    f"{'0xF6':>10}"
    f"{'%25':>10}"
    f"{'%F6':>10}"
    f"{'Dominant':>12}"
)

print("-" * 92)

for stat in sorted(stat_bins.keys()):

    c25 = stat_bins[stat]["25"]
    cf6 = stat_bins[stat]["F6"]

    total = c25 + cf6

    if total < 10:
        continue

    p25 = 100.0 * c25 / total
    pf6 = 100.0 * cf6 / total

    dominant = "0x25"

    if cf6 > c25:
        dominant = "0xF6"

    print(
        f"{stat:<8}"
        f"{c25:10d}"
        f"{cf6:10d}"
        f"{p25:9.1f}%"
        f"{pf6:9.1f}%"
        f"{dominant:>12}"
    )

print()
print("Done.")