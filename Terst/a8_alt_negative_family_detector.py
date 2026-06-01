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

NORMAL_NEG = {0x25,0xF6,0xBC}
ALT_NEG    = {0x5F,0x8C,0xC6}

groups = {
    "NORMAL": [],
    "ALT": []
}

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

        rpm,stat,dyn = decode_signals(frame)

        if residual in NORMAL_NEG:

            groups["NORMAL"].append(
                (frame,rpm,stat,dyn,residual)
            )

        elif residual in ALT_NEG:

            groups["ALT"].append(
                (frame,rpm,stat,dyn,residual)
            )

# ------------------------------------------------------------

print()
print("="*80)
print("A8 ALT NEGATIVE FAMILY DETECTOR")
print("="*80)

for name in ["NORMAL","ALT"]:

    rows = groups[name]

    print()
    print("="*60)
    print(name)
    print("="*60)

    print()
    print(f"Frames : {len(rows):,}")

    rpms  = [r[1] for r in rows]
    stats = [r[2] for r in rows]
    dyns  = [r[3] for r in rows]

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

    residuals = Counter(
        r[4] for r in rows
    )

    print()
    print("Residual Breakdown")
    print("------------------")

    for value,count in residuals.most_common():

        print(
            f"0x{value:02X} "
            f"{count:8,d}"
        )

    for idx in [4,6,7]:

        c = Counter(
            r[0][idx]
            for r in rows
        )

        print()
        print(f"Byte{idx}")
        print("-"*12)

        for value,count in c.most_common(20):

            pct = (
                100.0 *
                count /
                len(rows)
            )

            print(
                f"0x{value:02X} "
                f"{count:6d} "
                f"({pct:5.1f}%)"
            )

# ------------------------------------------------------------

print()
print("="*80)
print("ALT FAMILY EXAMPLES")
print("="*80)

shown = 0

for frame,rpm,stat,dyn,residual in groups["ALT"][:40]:

    print(
        f"RES=0x{residual:02X} "
        f"RPM={rpm:7.2f} "
        f"STAT={stat:3d} "
        f"DYN={dyn:4d} "
        + " ".join(f"{b:02X}" for b in frame)
    )

    shown += 1

print()
print("Done.")