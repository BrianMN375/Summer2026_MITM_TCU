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
# RECOVERED A7 COUNTER TABLE
# ============================================================

COUNTER_TABLE = [
    0x00, 0x88, 0xFC, 0x7D,
    0x47, 0x3C, 0xAF, 0x26,
    0x6B, 0xBB, 0xBD, 0xD3,
    0x6C, 0x9C, 0x17, 0xC9,
]

# ============================================================
# RECOVERED BASIS VECTORS
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
# BUILD RESIDUALS
# ============================================================

residual_frames = defaultdict(list)
residual_hist = Counter()

for frame in frames:

    predicted = affine_prediction(frame)

    residual = frame[0] ^ predicted

    residual_frames[residual].append(frame)

    residual_hist[residual] += 1

# ============================================================
# GLOBAL RESIDUALS
# ============================================================

print()
print("=" * 80)
print("TOP RESIDUALS")
print("=" * 80)

for resid, count in residual_hist.most_common(32):

    pct = 100.0 * count / len(frames)

    print(
        f"0x{resid:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

# ============================================================
# RESIDUAL FAMILY ANALYSIS
# ============================================================

for resid, rows in residual_hist.most_common(20):

    rows = residual_frames[resid]

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{resid:02X}")
    print("=" * 80)

    print(f"Frames: {len(rows):,}")

    state_hist = Counter(
        (row[1] >> 4)
        for row in rows
    )

    counter_hist = Counter(
        (row[1] & 0x0F)
        for row in rows
    )

    print()
    print("States")

    for value, count in state_hist.most_common(10):

        pct = 100.0 * count / len(rows)

        print(
            f"0x{value:X} "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

    print()
    print("Counters")

    for value, count in counter_hist.most_common(10):

        pct = 100.0 * count / len(rows)

        print(
            f"0x{value:X} "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

    # --------------------------------------------------------
    # Byte Analysis
    # --------------------------------------------------------

    for byte_idx in range(2, 8):

        hist = Counter(
            row[byte_idx]
            for row in rows
        )

        print()
        print(f"Byte {byte_idx}")

        for value, count in hist.most_common(12):

            pct = 100.0 * count / len(rows)

            print(
                f"0x{value:02X} "
                f"{count:8,d} "
                f"({pct:6.2f}%)"
            )

# ============================================================
# FIELD PREDICTOR SEARCH
# ============================================================

print()
print("=" * 80)
print("FIELD -> RESIDUAL RELATIONSHIPS")
print("=" * 80)

for byte_idx in range(2, 8):

    print()
    print(f"BYTE {byte_idx}")
    print("-" * 60)

    value_map = defaultdict(Counter)

    for frame in frames:

        predicted = affine_prediction(frame)

        residual = frame[0] ^ predicted

        value_map[
            frame[byte_idx]
        ][residual] += 1

    results = []

    for value, hist in value_map.items():

        total = sum(hist.values())

        if total < 50:
            continue

        top_resid, top_count = hist.most_common(1)[0]

        pct = 100.0 * top_count / total

        results.append(
            (
                pct,
                value,
                top_resid,
                total
            )
        )

    results.sort(reverse=True)

    for pct, value, resid, total in results[:20]:

        print(
            f"0x{value:02X} -> "
            f"Residual 0x{resid:02X} "
            f"{pct:6.2f}% "
            f"(n={total})"
        )

print()
print("Done.")