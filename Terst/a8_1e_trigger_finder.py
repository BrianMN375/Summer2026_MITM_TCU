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
# Known Linear Vectors
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

def load_residual_frames(target):

    results = []

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

            if actual_residual(frame) == target:
                results.append(frame)

    return results

# ============================================================

def analyze_byte(name, index, frames):

    counter = Counter(
        f[index]
        for f in frames
    )

    print()
    print(f"{name}")

    for value,count in counter.most_common(20):

        pct = (
            100.0 * count
            / len(frames)
        )

        print(
            f" 0x{value:02X} "
            f"{count:6d} "
            f"({pct:6.2f}%)"
        )

# ============================================================

def compare(name, index, frames_1e, frames_c7):

    print()
    print("=" * 60)
    print(f"{name}")
    print("=" * 60)

    print()
    print("Residual 0x1E")

    analyze_byte(
        name,
        index,
        frames_1e
    )

    print()
    print("Residual 0xC7")

    analyze_byte(
        name,
        index,
        frames_c7
    )

# ============================================================

def main():

    print()
    print("Using CSV:")
    print(CSV_FILE)

    frames_1e = load_residual_frames(0x1E)
    frames_c7 = load_residual_frames(0xC7)

    print()
    print(
        f"0x1E Frames : {len(frames_1e):,}"
    )

    print(
        f"0xC7 Frames : {len(frames_c7):,}"
    )

    for idx in range(2,8):

        compare(
            f"Byte{idx}",
            idx,
            frames_1e,
            frames_c7
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()