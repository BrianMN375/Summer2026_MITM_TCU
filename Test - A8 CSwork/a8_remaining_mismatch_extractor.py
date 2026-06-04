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


# ============================================================
# V10 PREDICTOR
# ============================================================

def signed8(v):
    return v - 256 if v > 127 else v

def rpm(frame):
    return ((frame[3] << 8) | frame[2]) / 4.0

def stat(frame):
    return ((frame[5] & 0x0F) << 8) | frame[4]

def dyn(frame):
    return signed8(frame[7])

# ------------------------------------------------------------

def predicted_residual(frame):

    b4 = frame[4]
    b5 = frame[5]
    b7 = frame[7]

    d = dyn(frame)

    # --------------------------------------------------------
    # NEGATIVE TORQUE FAMILY
    # --------------------------------------------------------

    if b5 == 0x64 and d < 0:

        if b7 >= 0x46:
            return 0x38

        if b7 >= 0x40:
            return 0x62

        return 0xBC

    # --------------------------------------------------------
    # POSITIVE TORQUE FAMILY
    # --------------------------------------------------------

    if b5 == 0x64 and d >= 0:

        # Stage 2 positive overlay

        if b4 >= 0x99 and b7 >= 0x40:
            return 0xC0

        # Stage 1 positive overlay

        if b4 >= 0x90:
            return 0x1E

        return 0xC7

    return 0x00

# ============================================================
# Counter transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00,0x7A,0x96,0x79,
    0xD5,0xBF,0x8F,0x9A,
    0xB8,0xCA,0x95,0x58,
    0x46,0x55,0xE7,0x68
]

# ============================================================
# Known vectors
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
# Residual decoder
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

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ============================================================

targets = {
    0x8A: [],
    0xDE: [],
    0x91: [],
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

        actual = actual_residual(frame)

        predicted = predicted_residual(frame)

        mismatch = actual ^ predicted

        if mismatch not in targets:
            continue

        targets[mismatch].append({
            "frame": frame,
            "actual": actual,
            "pred": predicted,
            "rpm": rpm(frame),
            "stat": stat(frame),
            "dyn": dyn(frame),
        })

# ============================================================

print()
print("=" * 80)
print("A8 REMAINING MISMATCH EXTRACTOR")
print("=" * 80)

for mismatch in (0x8A, 0xDE, 0x91):

    rows = targets[mismatch]

    print()
    print("=" * 70)
    print(f"MISMATCH XOR 0x{mismatch:02X}")
    print("=" * 70)

    print()
    print(f"Frames : {len(rows)}")

    if not rows:
        continue

    rpms  = [r["rpm"] for r in rows]
    stats = [r["stat"] for r in rows]
    dyns  = [r["dyn"] for r in rows]

    print(
        f"RPM  : {min(rpms):.1f}..{max(rpms):.1f} "
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

    print()
    print("Frames")
    print("-" * 70)

    for r in rows:

        frame = r["frame"]

        data = " ".join(
            f"{b:02X}" for b in frame
        )

        print(
            f"ACT=0x{r['actual']:02X} "
            f"PRED=0x{r['pred']:02X} "
            f"RPM={r['rpm']:7.1f} "
            f"STAT={r['stat']:3d} "
            f"DYN={r['dyn']:4d} "
            f"B4=0x{frame[4]:02X} "
            f"B5=0x{frame[5]:02X} "
            f"B6=0x{frame[6]:02X} "
            f"B7=0x{frame[7]:02X} "
            f"{data}"
        )

print()
print("Done.")