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

    tq_pct = (
        (raw >> 40) & 0x7F
    )

    stat = (
        (raw >> 21) & 0x1FF
    )

    dyn = (
        ((raw >> 30) & 0x3FF)
        - 509
    )

    neg = (
        ((raw >> 12) & 0x1FF)
        - 509
    )

    return rpm, tq_pct, stat, dyn, neg

# ============================================================

def actual_residual(frame):

    checksum = frame[0]
    counter = frame[1] & 0x0F

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

def analyze_residual(target):

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

            residual = actual_residual(frame)

            if residual != target:
                continue

            rpm,tq,stat,dyn,neg = decode(frame)

            rows.append(
                (
                    rpm,
                    tq,
                    stat,
                    dyn,
                    neg,
                    frame
                )
            )

    print()
    print("=" * 60)
    print(f"RESIDUAL 0x{target:02X}")
    print("=" * 60)

    if not rows:
        print("No frames found.")
        return

    rpms  = [x[0] for x in rows]
    tqs   = [x[1] for x in rows]
    stats = [x[2] for x in rows]
    dyns  = [x[3] for x in rows]
    negs  = [x[4] for x in rows]

    print()
    print(f"Frames : {len(rows):,}")

    print()
    print("Signal Summary")
    print("--------------")

    print(
        f"RPM      : {min(rpms):.0f}..{max(rpms):.0f}"
        f"  avg={sum(rpms)/len(rpms):.0f}"
    )

    print(
        f"TQ%      : {min(tqs)}..{max(tqs)}"
        f"  avg={sum(tqs)/len(tqs):.1f}"
    )

    print(
        f"STAT     : {min(stats)}..{max(stats)}"
        f"  avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN      : {min(dyns)}..{max(dyns)}"
        f"  avg={sum(dyns)/len(dyns):.1f}"
    )

    print(
        f"NEG      : {min(negs)}..{max(negs)}"
        f"  avg={sum(negs)/len(negs):.1f}"
    )

    print()
    print("Unique Counts")
    print("-------------")

    print(f"RPM Values  : {len(set(rpms))}")
    print(f"STAT Values : {len(set(stats))}")
    print(f"DYN Values  : {len(set(dyns))}")
    print(f"TQ Values   : {len(set(tqs))}")

    print()
    print("Most Common STAT Values")
    print("-----------------------")

    stat_counter = Counter(stats)

    for value,count in stat_counter.most_common(20):

        print(
            f"{value:4d} : {count:,}"
        )

    print()
    print("Example Frames")
    print("--------------")

    for row in rows[:20]:

        frame = row[5]

        print(
            " ".join(
                f"{b:02X}"
                for b in frame
            )
        )

# ============================================================

def main():

    print()
    print("Using CSV:")
    print(CSV_FILE)

    analyze_residual(0x0D)
    analyze_residual(0x1E)

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()