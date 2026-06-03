#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "all_0xAx_combinedSet1.csv"
)

TARGET_ID = 0x0A7

# ============================================================
# COUNTER TABLE
# ============================================================

COUNTER_TABLE = [
    0x00, 0x88, 0xFC, 0x7D,
    0x47, 0x3C, 0xAF, 0x26,
    0x6B, 0xBB, 0xBD, 0xD3,
    0x6C, 0x9C, 0x17, 0xC9,
]

# ============================================================
# BASIS VECTORS
# ============================================================

VECTORS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,1):0xA8,
    (3,2):0x7F,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,
    (4,4):0xD9,
    (4,5):0x9D,
    (4,6):0x15,
    (4,7):0x2A,

    (5,0):0xB5,
    (5,1):0x45,
    (5,2):0x8A,
    (5,3):0x3B,
    (5,4):0x76,
    (5,5):0xEC,
    (5,6):0xF7,
    (5,7):0xC1,

    (6,0):0x0E,
    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
}

# ============================================================
# AFFINE PREDICTOR
# ============================================================

def affine_prediction(frame):

    counter = frame[1] & 0x0F

    value = COUNTER_TABLE[counter]

    for byte_idx in range(2, 8):

        b = frame[byte_idx]

        for bit in range(8):

            if b & (1 << bit):

                value ^= VECTORS.get(
                    (byte_idx, bit),
                    0
                )

    return value

# ============================================================
# LOAD
# ============================================================

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            can_id = int(row["id"], 16)

            if can_id != TARGET_ID:
                continue

            frame = [
                int(row[f"b{i}"], 16)
                for i in range(8)
            ]

            frames.append(frame)

        except:
            pass

print()
print(f"Loaded {len(frames):,} A7 frames")

# ============================================================
# BUILD RESIDUALS
# ============================================================

records = []

residual_hist = Counter()

for frame in frames:

    pred = affine_prediction(frame)

    residual = frame[0] ^ pred

    residual_hist[residual] += 1

    records.append(
        (
            residual,
            frame
        )
    )

# ============================================================
# TOP RESIDUALS
# ============================================================

print()
print("=" * 80)
print("TOP RESIDUALS")
print("=" * 80)

top_residuals = []

for resid, count in residual_hist.most_common(20):

    pct = (
        100.0 *
        count /
        len(frames)
    )

    top_residuals.append(resid)

    print(
        f"0x{resid:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

# ============================================================
# FAMILY SIGNATURE ANALYSIS
# ============================================================

for resid in top_residuals:

    rows = [
        frame
        for r, frame in records
        if r == resid
    ]

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{resid:02X}")
    print("=" * 80)

    print(f"Frames: {len(rows):,}")

    for byte_idx in [2,3,4,5,6,7]:

        hist = Counter(
            row[byte_idx]
            for row in rows
        )

        print()
        print(f"Byte {byte_idx}")

        for value, count in hist.most_common(10):

            pct = (
                100.0 *
                count /
                len(rows)
            )

            print(
                f"0x{value:02X} "
                f"{count:8,d} "
                f"({pct:6.2f}%)"
            )

# ============================================================
# SIGNATURE SEARCH
# ============================================================

print()
print("=" * 80)
print("SIGNATURE SEARCH")
print("=" * 80)

signature_map = defaultdict(Counter)

for residual, frame in records:

    signature = (
        frame[3],  # b3
        frame[5],  # b5
        frame[6],  # b6
    )

    signature_map[signature][residual] += 1

results = []

for sig, hist in signature_map.items():

    total = sum(hist.values())

    if total < 25:
        continue

    top_resid, top_count = hist.most_common(1)[0]

    pct = (
        100.0 *
        top_count /
        total
    )

    results.append(
        (
            pct,
            sig,
            top_resid,
            total
        )
    )

results.sort(reverse=True)

print()
print("Strongest Signatures")
print("-" * 60)

for pct, sig, resid, total in results[:100]:

    print(
        f"B3=0x{sig[0]:02X} "
        f"B5=0x{sig[1]:02X} "
        f"B6=0x{sig[2]:02X}"
        f" -> Residual 0x{resid:02X}"
        f"  {pct:6.2f}%"
        f"  (n={total})"
    )

# ============================================================
# RESIDUAL -> SIGNATURE VIEW
# ============================================================

print()
print("=" * 80)
print("RESIDUAL -> SIGNATURE")
print("=" * 80)

for resid in top_residuals[:10]:

    sig_hist = Counter()

    for r, frame in records:

        if r != resid:
            continue

        sig = (
            frame[3],
            frame[5],
            frame[6]
        )

        sig_hist[sig] += 1

    print()
    print(f"Residual 0x{resid:02X}")

    for sig, count in sig_hist.most_common(15):

        print(
            f"  B3=0x{sig[0]:02X} "
            f"B5=0x{sig[1]:02X} "
            f"B6=0x{sig[2]:02X} "
            f"count={count}"
        )

print()
print("Done.")