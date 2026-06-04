#!/usr/bin/env python3

import csv
from collections import defaultdict

CSV_FILE = input(
    "CSV file: "
).strip().strip('"')

# ----------------------------------------------------------
# Counter transform
# ----------------------------------------------------------

COUNTER_TRANSFORM = {
    0:0x00,
    1:0x77,
    2:0xEE,
    3:0x99,
    4:0x07,
    5:0x70,
    6:0xE9,
    7:0xFD,
    8:0xD5,
    9:0x85,
    10:0x25,
    11:0xB5,
    12:0x5A,
    13:0xC2,
    14:0x32,
    15:0x6A,
}

# ----------------------------------------------------------
# Known vectors
# ----------------------------------------------------------

KNOWN_VECTORS = {
    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ----------------------------------------------------------

def build_u64(frame):

    value = 0

    for i,b in enumerate(frame):
        value |= (b << (8*i))

    return value

# ----------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm = (
        ((raw >> 48) & 0xFFFF)
        * 0.25
    )

    stat = (
        (raw >> 21)
        & 0x1FF
    )

    dyn = (
        ((raw >> 30) & 0x3FF)
        - 509
    )

    return rpm, stat, dyn

# ----------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

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
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ----------------------------------------------------------

stats = defaultdict(list)

print()
print("=" * 80)
print("A8 POSITIVE TORQUE BOUNDARIES")
print("=" * 80)

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

        if residual not in (
            0xC7,
            0x1E,
            0xC0,
            0x19
        ):
            continue

        rpm, stat, dyn = decode_signals(frame)

        stats[residual].append({
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "b4": frame[4],
            "b5": frame[5],
            "b6": frame[6],
            "b7": frame[7],
        })

# ----------------------------------------------------------

for residual in sorted(stats):

    rows = stats[residual]

    print()
    print("=" * 70)
    print(f"RESIDUAL 0x{residual:02X}")
    print("=" * 70)

    print()
    print(f"Frames : {len(rows):,}")

    for key in (
        "rpm",
        "stat",
        "dyn",
        "b4",
        "b5",
        "b6",
        "b7"
    ):

        values = [
            r[key]
            for r in rows
        ]

        print(
            f"{key.upper():5s}"
            f"  min={min(values)}"
            f"  max={max(values)}"
            f"  avg={sum(values)/len(values):.2f}"
        )

# ----------------------------------------------------------

pairs = [
    (0xC7,0x1E),
    (0xC7,0xC0),
    (0xC7,0x19),
    (0x1E,0xC0),
    (0x1E,0x19),
    (0xC0,0x19),
]

for a,b in pairs:

    if a not in stats or b not in stats:
        continue

    print()
    print("=" * 70)
    print(f"0x{a:02X} vs 0x{b:02X}")
    print("=" * 70)

    for key in (
        "rpm",
        "stat",
        "dyn",
        "b4",
        "b5",
        "b6",
        "b7"
    ):

        av = [r[key] for r in stats[a]]
        bv = [r[key] for r in stats[b]]

        lo = max(
            min(av),
            min(bv)
        )

        hi = min(
            max(av),
            max(bv)
        )

        if lo > hi:

            print(
                f"{key.upper():5s}  NO OVERLAP"
            )

        else:

            print(
                f"{key.upper():5s}  "
                f"{lo} .. {hi}"
            )

print()
print("Done.")