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

NORMAL_NEG = {0x25,0xF6,0xBC}
ALT_NEG    = {0x5F,0x8C,0xC6}

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

normal_frames = []
alt_frames = []

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

        if residual in NORMAL_NEG:
            normal_frames.append(frame)

        elif residual in ALT_NEG:
            alt_frames.append(frame)

# ------------------------------------------------------------

print()
print("=" * 90)
print("A8 ALT NEG BITFIELD SEARCH")
print("=" * 90)

print()
print(f"Normal Frames : {len(normal_frames):,}")
print(f"ALT Frames    : {len(alt_frames):,}")

results = []

for byte_idx in range(2,8):

    for bit_idx in range(8):

        mask = 1 << bit_idx

        normal_set = sum(
            1 for f in normal_frames
            if f[byte_idx] & mask
        )

        alt_set = sum(
            1 for f in alt_frames
            if f[byte_idx] & mask
        )

        normal_pct = (
            100.0 *
            normal_set /
            len(normal_frames)
        )

        alt_pct = (
            100.0 *
            alt_set /
            len(alt_frames)
        )

        delta = abs(
            alt_pct - normal_pct
        )

        results.append(
            (
                delta,
                byte_idx,
                bit_idx,
                normal_pct,
                alt_pct
            )
        )

# ------------------------------------------------------------

results.sort(reverse=True)

print()
print("Top Bit Differences")
print("-" * 90)

print(
    f"{'Byte':<8}"
    f"{'Bit':<8}"
    f"{'Normal%':>12}"
    f"{'ALT%':>12}"
    f"{'Delta':>12}"
)

print("-" * 90)

for (
    delta,
    byte_idx,
    bit_idx,
    normal_pct,
    alt_pct
) in results[:50]:

    print(
        f"{byte_idx:<8}"
        f"{bit_idx:<8}"
        f"{normal_pct:11.1f}%"
        f"{alt_pct:11.1f}%"
        f"{delta:11.1f}%"
    )

print()
print("Done.")