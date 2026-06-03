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
# DISCOVERED A7 COUNTER TABLE
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x88, 0xFC, 0x7D,
    0x47, 0x3C, 0xAF, 0x26,
    0x6B, 0xBB, 0xBD, 0xD3,
    0x6C, 0x9C, 0x17, 0xC9,
]

# ============================================================
# DISCOVERED STATE FAMILIES
# ============================================================

STATE_FAMILY = [
    0x6A, 0xBB, 0x95, 0x37,
    0x7F, 0xDC, 0x34, 0xC3,
    0x99, 0xE3, 0x7D, 0xAC,
    0xF5, 0x24, 0x78, 0xA9,
]

# ============================================================
# KNOWN BASIS VECTORS
# ============================================================

KNOWN_BASIS = {

    0x19 : "basis[38]",
    0x32 : "basis[39]",

    0xAD : "basis[16]",
    0x75 : "basis[17]",
    0xEA : "basis[18]",
    0xFB : "basis[19]",
    0xD9 : "basis[20]",
    0x9D : "basis[21]",

    0x8A : "basis[10]",
    0x3B : "basis[11]",

    0xE0 : "basis[4]",
    0xEF : "basis[5]",
    0xF1 : "basis[6]",
    0xCD : "basis[7]",

    0xE9 : "basis[119]"
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
# RESIDUALS
# ============================================================

basis_hits = defaultdict(Counter)

for frame in frames:

    checksum = frame[0]

    state = frame[1] >> 4
    counter = frame[1] & 0x0F

    residual = (
        checksum
        ^ COUNTER_TRANSFORM[counter]
        ^ STATE_FAMILY[state]
    )

    if residual not in KNOWN_BASIS:
        continue

    for byte_idx in range(1, 8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                basis_hits[residual][
                    (byte_idx, bit_idx)
                ] += 1

# ============================================================
# REPORT
# ============================================================

print()
print("=" * 80)
print("BASIS ACTIVATION REPORT")
print("=" * 80)

for basis_value in sorted(basis_hits):

    print()
    print("-" * 80)

    print(
        f"0x{basis_value:02X}  "
        f"{KNOWN_BASIS[basis_value]}"
    )

    print("-" * 80)

    total = sum(
        basis_hits[basis_value].values()
    )

    for (
        bit,
        count
    ) in basis_hits[basis_value].most_common(20):

        pct = (
            100.0 *
            count /
            total
        )

        print(
            f"b{bit[0]}[{bit[1]}] "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

# ============================================================
# INVERSE VIEW
# ============================================================

bit_to_basis = defaultdict(Counter)

for basis_value, hist in basis_hits.items():

    for bit, count in hist.items():

        bit_to_basis[bit][
            basis_value
        ] += count

print()
print("=" * 80)
print("BIT -> BASIS VIEW")
print("=" * 80)

for bit in sorted(bit_to_basis):

    total = sum(
        bit_to_basis[bit].values()
    )

    if total < 500:
        continue

    print()
    print(
        f"b{bit[0]}[{bit[1]}]"
    )

    for val, count in (
        bit_to_basis[bit]
        .most_common(10)
    ):

        pct = (
            100.0 *
            count /
            total
        )

        print(
            f"   0x{val:02X} "
            f"{pct:6.2f}% "
            f"{KNOWN_BASIS.get(val,'')}"
        )

print()
print("Done.")