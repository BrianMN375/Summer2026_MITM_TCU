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
# DISCOVER FAMILY TABLE
# ============================================================

state_hist = defaultdict(Counter)

for frame in frames:

    checksum = frame[0]

    state = frame[1] >> 4
    counter = frame[1] & 0x0F

    family = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    state_hist[state][family] += 1

state_family = [0] * 16

print()
print("=" * 80)
print("DISCOVERED STATE FAMILY TABLE")
print("=" * 80)

for state in range(16):

    hist = state_hist[state]

    top_family, top_count = (
        hist.most_common(1)[0]
    )

    state_family[state] = top_family

    total = sum(hist.values())

    pct = (
        100.0 *
        top_count /
        total
    )

    print(
        f"State {state:X} -> "
        f"0x{top_family:02X} "
        f"({pct:.2f}%)"
    )

print()
print("C TABLE")
print("-------------------------")

for i, value in enumerate(state_family):

    print(
        f"0x{value:02X},",
        end=" "
    )

    if (i + 1) % 4 == 0:
        print()

# ============================================================
# REMOVE BOTH LAYERS
# ============================================================

residual_hist = Counter()
state_residual_hist = defaultdict(Counter)

for frame in frames:

    checksum = frame[0]

    state = frame[1] >> 4
    counter = frame[1] & 0x0F

    residual = (
        checksum
        ^ COUNTER_TRANSFORM[counter]
        ^ state_family[state]
    )

    residual_hist[residual] += 1
    state_residual_hist[state][residual] += 1

# ============================================================
# GLOBAL RESIDUALS
# ============================================================

print()
print("=" * 80)
print("GLOBAL RESIDUALS")
print("=" * 80)

for value, count in residual_hist.most_common(64):

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
# PER STATE RESIDUALS
# ============================================================

print()
print("=" * 80)
print("PER STATE RESIDUALS")
print("=" * 80)

for state in range(16):

    hist = state_residual_hist[state]

    total = sum(hist.values())

    top_val, top_count = (
        hist.most_common(1)[0]
    )

    pct = (
        100.0 *
        top_count /
        total
    )

    print(
        f"State {state:X}: "
        f"TopResidual=0x{top_val:02X} "
        f"{pct:6.2f}%"
    )

# ============================================================
# UNIQUE COUNTS
# ============================================================

print()
print("=" * 80)
print("STATE RESIDUAL ENTROPY")
print("=" * 80)

for state in range(16):

    unique = len(
        state_residual_hist[state]
    )

    print(
        f"State {state:X}: "
        f"{unique} unique residuals"
    )

print()
print("Done.")