#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

# ============================================================
# Fixed A8 Dataset
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
# Recovered Vectors
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

MAJOR_RESIDUALS = {
    0x5E, 0x25,
    0x8D, 0xF6,
    0xC7, 0xBC
}

# ============================================================

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ============================================================

def decode_stat(frame):

    raw = build_u64(frame)

    stat = (
        (raw >> 21) & 0x1FF
    )

    return stat

# ============================================================

def calc_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family_value = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    known = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    known ^= vec

    return family_value ^ known

# ============================================================

def main():

    print()
    print(f"Using CSV:")
    print(CSV_FILE)
    print()

    stat_hist = defaultdict(Counter)

    total = 0

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

            residual = calc_residual(frame)

            if residual not in MAJOR_RESIDUALS:
                continue

            stat = decode_stat(frame)

            stat_hist[stat][residual] += 1

            total += 1

    print("====================================================")
    print("A8 STAT BOUNDARY FINDER")
    print("====================================================")
    print()

    print(
        f"Major-state frames analyzed: "
        f"{total:,}"
    )

    print()

    print(
        "STAT VALUE OWNERSHIP"
    )

    print(
        "--------------------"
    )

    current_owner = None
    start_stat = None

    for stat in sorted(stat_hist.keys()):

        owner = stat_hist[stat].most_common(1)[0][0]

        if current_owner is None:

            current_owner = owner
            start_stat = stat
            continue

        if owner != current_owner:

            print(
                f"{start_stat:3d}-{stat-1:3d}"
                f"  -> 0x{current_owner:02X}"
            )

            current_owner = owner
            start_stat = stat

    if current_owner is not None:

        print(
            f"{start_stat:3d}-{stat:3d}"
            f"  -> 0x{current_owner:02X}"
        )

    print()

    print(
        "LIKELY STATE BOUNDARIES"
    )

    print(
        "-----------------------"
    )

    previous_owner = None

    for stat in sorted(stat_hist.keys()):

        owner = stat_hist[stat].most_common(1)[0][0]

        if previous_owner is not None:

            if owner != previous_owner:

                print(
                    f"Boundary near Stat={stat}"
                )

        previous_owner = owner

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()