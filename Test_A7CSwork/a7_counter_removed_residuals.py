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
# DISCOVERED COUNTER TRANSFORM
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x88, 0xFC, 0x7D,
    0x47, 0x3C, 0xAF, 0x26,
    0x6B, 0xBB, 0xBD, 0xD3,
    0x6C, 0x9C, 0x17, 0xC9,
]

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

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            frames.append(frame)

        except:
            pass

print()
print(f"Loaded {len(frames):,} A7 frames")

# ============================================================
# ANALYSIS
# ============================================================

state_hist = defaultdict(Counter)
global_hist = Counter()

for frame in frames:

    checksum = frame[0]

    state = frame[1] >> 4
    counter = frame[1] & 0x0F

    family = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    state_hist[state][family] += 1
    global_hist[family] += 1

# ============================================================
# STATE SUMMARY
# ============================================================

print()
print("=" * 80)
print("FAMILY VALUES BY STATE")
print("=" * 80)

for state in range(16):

    hist = state_hist[state]

    if not hist:
        continue

    total = sum(hist.values())

    unique = len(hist)

    top_val, top_count = hist.most_common(1)[0]

    pct = (
        100.0 *
        top_count /
        total
    )

    print()
    print(
        f"State {state:X}"
    )

    print(
        f"Frames          : {total:,}"
    )

    print(
        f"Unique Families : {unique}"
    )

    print(
        f"Top Family      : "
        f"0x{top_val:02X}"
    )

    print(
        f"Top Percentage  : "
        f"{pct:.2f}%"
    )

    print()

    for value, count in hist.most_common(10):

        pct2 = (
            100.0 *
            count /
            total
        )

        print(
            f"  0x{value:02X} "
            f"{count:8,d} "
            f"({pct2:6.2f}%)"
        )

# ============================================================
# GLOBAL SUMMARY
# ============================================================

print()
print("=" * 80)
print("GLOBAL FAMILY DISTRIBUTION")
print("=" * 80)

for value, count in global_hist.most_common(64):

    pct = (
        100.0 *
        count /
        len(frames)
    )

    print(
        f"0x{value:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

# ============================================================
# FAMILY PRESENCE MATRIX
# ============================================================

top_families = [
    x[0]
    for x in global_hist.most_common(16)
]

print()
print("=" * 80)
print("TOP FAMILY PRESENCE BY STATE")
print("=" * 80)

header = "State "

for fam in top_families:

    header += f"{fam:02X} "

print(header)

for state in range(16):

    hist = state_hist[state]

    total = sum(hist.values())

    if not total:
        continue

    line = f"{state:X}     "

    for fam in top_families:

        pct = (
            100.0 *
            hist[fam] /
            total
        )

        line += f"{pct:4.1f} "

    print(line)

print()
print("Done.")