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

def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

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

def predicted_residual(frame):

    rpm, stat, dyn = decode_signals(frame)

    #
    # OFF
    #

    if (
        frame[2] == 0xDF and
        frame[3] == 0xBF and
        frame[4] == 0xFF
    ):
        return 0x0D

    #
    # SPECIAL HIGH
    #

    if dyn >= 67:
        return 0x1E

    #
    # NEG / POS
    #

    if dyn < 3:
        residual = 0x25
    else:
        residual = 0x5E

    #
    # MID
    #

    if stat >= 128:
        residual ^= 0xD3

    #
    # HIGH
    #

    if rpm >= 2050:
        residual ^= 0x4A

    return residual

# ----------------------------------------------------------

mismatch_groups = {}

print()
print("Loading dataset...")

frames_total = 0
mismatch_total = 0

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

        frames_total += 1

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        if actual == predicted:
            continue

        mismatch_total += 1

        delta = actual ^ predicted

        rpm, stat, dyn = decode_signals(frame)

        if delta not in mismatch_groups:

            mismatch_groups[delta] = {
                "count": 0,
                "rpm": [],
                "stat": [],
                "dyn": [],
                "b4": [],
                "b6": [],
                "b7": [],
            }

        g = mismatch_groups[delta]

        g["count"] += 1

        g["rpm"].append(rpm)
        g["stat"].append(stat)
        g["dyn"].append(dyn)

        g["b4"].append(frame[4])
        g["b6"].append(frame[6])
        g["b7"].append(frame[7])

# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 FINAL MISMATCH CLASSIFIER")
print("=" * 80)

print()
print(f"Frames     : {frames_total:,}")
print(f"Mismatches : {mismatch_total:,}")

for delta, g in sorted(
    mismatch_groups.items(),
    key=lambda x: x[1]["count"],
    reverse=True
):

    print()
    print("=" * 60)
    print(f"DELTA 0x{delta:02X}")
    print("=" * 60)

    print()
    print(f"Frames : {g['count']:,}")

    print()
    print(
        f"RPM   : "
        f"{min(g['rpm']):.0f}..{max(g['rpm']):.0f} "
        f"avg={sum(g['rpm'])/len(g['rpm']):.1f}"
    )

    print(
        f"STAT  : "
        f"{min(g['stat'])}..{max(g['stat'])} "
        f"avg={sum(g['stat'])/len(g['stat']):.1f}"
    )

    print(
        f"DYN   : "
        f"{min(g['dyn'])}..{max(g['dyn'])} "
        f"avg={sum(g['dyn'])/len(g['dyn']):.1f}"
    )

    print(
        f"Byte4 : "
        f"0x{int(sum(g['b4'])/len(g['b4'])):02X}"
    )

    print(
        f"Byte6 : "
        f"0x{int(sum(g['b6'])/len(g['b6'])):02X}"
    )

    print(
        f"Byte7 : "
        f"0x{int(sum(g['b7'])/len(g['b7'])):02X}"
    )

print()
print("Done.")