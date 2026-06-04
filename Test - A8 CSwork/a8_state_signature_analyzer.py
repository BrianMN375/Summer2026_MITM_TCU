#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = input(
    "Path to A8 CSV: "
).strip()

# ------------------------------------------------------------------
# Counter transform table
# ------------------------------------------------------------------

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ------------------------------------------------------------------
# Linear checksum vectors
# ------------------------------------------------------------------

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

# ------------------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ------------------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ------------------------------------------------------------------

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

# ------------------------------------------------------------------
# LOAD
# ------------------------------------------------------------------

states = defaultdict(list)

with open(CSV_FILE) as f:

    reader = csv.reader(f)

    for row in reader:

        if len(row) < 9:
            continue

        try:

            frame = [
                int(x,16)
                for x in row[1:9]
            ]

        except:
            continue

        residual = actual_residual(frame)

        rpm, stat, dyn = decode_signals(frame)

        states[residual].append({

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,

            "b4": frame[4],
            "b5": frame[5],
            "b6": frame[6],
            "b7": frame[7],
        })

# ------------------------------------------------------------------
# REPORT
# ------------------------------------------------------------------

for residual in sorted(states.keys()):

    rows = states[residual]

    print()
    print("=" * 70)
    print(
        f"ACT=0x{residual:02X}"
    )
    print("=" * 70)

    rpms = [r["rpm"] for r in rows]
    stats = [r["stat"] for r in rows]
    dyns = [r["dyn"] for r in rows]

    print()
    print(
        f"Frames : {len(rows)}"
    )

    print(
        f"RPM  : "
        f"{min(rpms):.1f}..{max(rpms):.1f} "
        f"avg={sum(rpms)/len(rpms):.1f}"
    )

    print(
        f"STAT : "
        f"{min(stats)}..{max(stats)} "
        f"avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN  : "
        f"{min(dyns)}..{max(dyns)} "
        f"avg={sum(dyns)/len(dyns):.1f}"
    )

    # ----------------------------------------------------------
    # Byte distributions
    # ----------------------------------------------------------

    for key in ["b4","b5","b6","b7"]:

        cnt = Counter(
            r[key]
            for r in rows
        )

        print()
        print(
            key.upper()
        )
        print(
            "-" * 20
        )

        total = len(rows)

        for value,count in (
            cnt.most_common(20)
        ):

            pct = (
                count *
                100.0 /
                total
            )

            print(
                f"0x{value:02X}"
                f" {count:5d}"
                f" ({pct:5.1f}%)"
            )

    # ----------------------------------------------------------
    # Signature tuple
    # ----------------------------------------------------------

    signature = Counter()

    for r in rows:

        signature[(
            r["b4"],
            r["b5"],
            r["b7"]
        )] += 1

    print()
    print("TOP SIGNATURES")
    print("-" * 20)

    for sig,count in (
        signature.most_common(10)
    ):

        b4,b5,b7 = sig

        print(
            f"B4=0x{b4:02X} "
            f"B5=0x{b5:02X} "
            f"B7=0x{b7:02X} "
            f" -> {count}"
        )