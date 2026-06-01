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

# ------------------------------------------------------------
# Residual decoder
# ------------------------------------------------------------

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

# ------------------------------------------------------------

families = {
    0x19: [],
    0xC7: [],
    0xC0: [],
    0x1E: [],
}

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

        if residual not in families:
            continue

        rpm, stat, dyn = decode_signals(frame)

        families[residual].append(
            (frame,rpm,stat,dyn)
        )

# ------------------------------------------------------------

def dump_family(name, frames):

    print()
    print("=" * 70)
    print(name)
    print("=" * 70)

    print()
    print(f"Frames : {len(frames)}")

    if not frames:
        return

    rpms  = [x[1] for x in frames]
    stats = [x[2] for x in frames]
    dyns  = [x[3] for x in frames]

    print()
    print(
        f"RPM  : {min(rpms):.0f}..{max(rpms):.0f} "
        f"avg={sum(rpms)/len(rpms):.1f}"
    )

    print(
        f"STAT : {min(stats)}..{max(stats)} "
        f"avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN  : {min(dyns)}..{max(dyns)} "
        f"avg={sum(dyns)/len(dyns):.1f}"
    )

    for byte_idx in (4,5,6,7):

        ctr = Counter()

        for frame,_,_,_ in frames:
            ctr[frame[byte_idx]] += 1

        print()
        print(f"Byte{byte_idx}")
        print("-" * 12)

        for value,count in ctr.most_common(20):

            pct = 100.0 * count / len(frames)

            print(
                f"0x{value:02X} "
                f"{count:6d} "
                f"({pct:5.1f}%)"
            )

# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 DE PRECISE BOUNDARY")
print("=" * 80)

dump_family("ACTUAL 0x19", families[0x19])

dump_family("ACTUAL 0xC7", families[0xC7])

dump_family("ACTUAL 0xC0", families[0xC0])

dump_family("ACTUAL 0x1E", families[0x1E])

# ------------------------------------------------------------
# Example frames
# ------------------------------------------------------------

for residual in (0x19,0xC0):

    print()
    print("=" * 70)
    print(f"EXAMPLES 0x{residual:02X}")
    print("=" * 70)

    for frame,rpm,stat,dyn in families[residual][:30]:

        data = " ".join(
            f"{b:02X}" for b in frame
        )

        print(
            f"RPM={rpm:7.2f} "
            f"STAT={stat:3d} "
            f"DYN={dyn:4d} "
            f"{data}"
        )

print()
print("Done.")