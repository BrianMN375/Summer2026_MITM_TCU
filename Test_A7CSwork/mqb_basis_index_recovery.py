#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a7_unique.csv"
)

POLY = 0x2F
SEED = 0x0E

EXCLUDED_STATES = {
    0x1,
    0xA,
}

# ============================================================
# LFSR
# ============================================================

def advance(value):

    msb = value & 0x80

    value = (value << 1) & 0xFF

    if msb:
        value ^= POLY

    return value

# ============================================================
# BUILD BASIS TABLE
# ============================================================

basis = []
seen = set()

current = SEED

while current not in seen:

    seen.add(current)

    basis.append(current)

    current = advance(current)

print()
print("=" * 70)
print("MQB BASIS")
print("=" * 70)

print(f"Length: {len(basis)}")

value_to_index = {
    value: idx
    for idx, value in enumerate(basis)
}

# ============================================================
# LOAD DATA
# ============================================================

payload_map = {}

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            upper = frame[1] >> 4

            if upper in EXCLUDED_STATES:
                continue

            payload_map[
                frame[1:]
            ] = frame[0]

        except:
            pass

print(
    f"Frames loaded: "
    f"{len(payload_map):,}"
)

# ============================================================
# DISCOVER DELTAS
# ============================================================

bit_hist = defaultdict(Counter)

for payload, checksum in payload_map.items():

    payload_list = list(payload)

    for byte_idx in range(7):

        for bit_idx in range(8):

            neighbor = payload_list.copy()

            neighbor[byte_idx] ^= (
                1 << bit_idx
            )

            neighbor = tuple(neighbor)

            if neighbor not in payload_map:
                continue

            delta = (
                checksum ^
                payload_map[neighbor]
            )

            bit_hist[
                (byte_idx + 1, bit_idx)
            ][delta] += 1

# ============================================================
# MAP TO BASIS INDICES
# ============================================================

print()
print("=" * 70)
print("BIT -> BASIS INDEX")
print("=" * 70)

known = []

for bit in sorted(bit_hist):

    hist = bit_hist[bit]

    total = sum(hist.values())

    delta, count = (
        hist.most_common(1)[0]
    )

    confidence = (
        100.0 *
        count /
        total
    )

    idx = value_to_index.get(delta)

    if idx is None:
        continue

    known.append(idx)

    print(
        f"b{bit[0]}[{bit[1]}] "
        f"delta=0x{delta:02X} "
        f"basis[{idx}] "
        f"conf={confidence:6.2f}%"
    )

# ============================================================
# FIND MISSING BASIS ENTRIES
# ============================================================

print()
print("=" * 70)
print("MISSING BASIS INDICES")
print("=" * 70)

known = sorted(set(known))

for idx in range(max(known)+1):

    if idx not in known:

        print(
            f"basis[{idx}] "
            f"0x{basis[idx]:02X}"
        )

# ============================================================
# SEARCH FOR MISSING VALUES
# ============================================================

print()
print("=" * 70)
print("POSSIBLE ASSIGNMENTS")
print("=" * 70)

for idx in range(max(known)+1):

    if idx in known:
        continue

    value = basis[idx]

    print()
    print(
        f"basis[{idx}] = 0x{value:02X}"
    )

    matches = []

    for bit in sorted(bit_hist):

        hist = bit_hist[bit]

        total = sum(hist.values())

        if total < 20:
            continue

        if value not in hist:
            continue

        count = hist[value]

        pct = (
            100.0 *
            count /
            total
        )

        matches.append(
            (
                pct,
                bit
            )
        )

    matches.sort(reverse=True)

    for pct, bit in matches[:10]:

        print(
            f"  b{bit[0]}[{bit[1]}] "
            f"{pct:.2f}%"
        )

print()
print("Done.")