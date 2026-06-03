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
# DISCOVERED VECTOR TABLE
# ============================================================

VECTORS = {

    (2,0): 0xF4,
    (2,1): 0xC7,
    (2,6): 0x19,
    (2,7): 0x32,

    (4,0): 0xAD,

    (5,2): 0x8A,
    (5,3): 0x3B,

    (6,4): 0xE0,
    (6,5): 0xEF,
    (6,6): 0xF1,
    (6,7): 0xCD,

    (7,0): 0xE9,
}

# ============================================================
# PREDICT
# ============================================================

def predict_checksum(frame):

    ck = 0

    for (byte_idx, bit_idx), vec in VECTORS.items():

        byte_val = frame[byte_idx]

        if byte_val & (1 << bit_idx):

            ck ^= vec

    return ck

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
print(f"Loaded {len(frames):,} frames")

# ============================================================
# ANALYSIS
# ============================================================

state_residuals = defaultdict(Counter)

counter_residuals = defaultdict(Counter)

for frame in frames:

    observed = frame[0]

    predicted = predict_checksum(frame)

    residual = observed ^ predicted

    state = frame[1] >> 4

    counter = frame[1] & 0x0F

    state_residuals[state][residual] += 1

    counter_residuals[counter][residual] += 1

# ============================================================
# STATE VIEW
# ============================================================

print()
print("=" * 80)
print("RESIDUAL BY STATE")
print("=" * 80)

for state in range(16):

    hist = state_residuals[state]

    if not hist:
        continue

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
        f"Top Residual=0x{top_val:02X} "
        f"{pct:6.2f}%"
    )

# ============================================================
# COUNTER VIEW
# ============================================================

print()
print("=" * 80)
print("RESIDUAL BY COUNTER")
print("=" * 80)

for ctr in range(16):

    hist = counter_residuals[ctr]

    if not hist:
        continue

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
        f"Counter {ctr:X}: "
        f"Top Residual=0x{top_val:02X} "
        f"{pct:6.2f}%"
    )

# ============================================================
# GLOBAL
# ============================================================

global_hist = Counter()

for hist in state_residuals.values():

    global_hist.update(hist)

print()
print("=" * 80)
print("TOP GLOBAL RESIDUALS")
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

print()
print("Done.")
print()