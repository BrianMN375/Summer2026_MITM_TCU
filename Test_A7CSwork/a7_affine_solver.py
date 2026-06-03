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
    "all_0xAx_combinedSet1.csv"
)

TARGET_ID = 0x0A7

# ------------------------------------------------------------
# Previously discovered layers
# ------------------------------------------------------------

COUNTER_TRANSFORM = [
    0x00,0x88,0xFC,0x7D,
    0x47,0x3C,0xAF,0x26,
    0x6B,0xBB,0xBD,0xD3,
    0x6C,0x9C,0x17,0xC9,
]

STATE_FAMILY = [
    0x6A,0xBB,0x95,0x37,
    0x7F,0xDC,0x34,0xC3,
    0x99,0xE3,0x7D,0xAC,
    0xF5,0x24,0x78,0xA9,
]

# ------------------------------------------------------------
# Load
# ------------------------------------------------------------

payload_map = {}

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

            state = frame[1] >> 4
            counter = frame[1] & 0x0F

            residual = (
                frame[0]
                ^ COUNTER_TRANSFORM[counter]
                ^ STATE_FAMILY[state]
            )

            payload = tuple(frame[1:8])

            payload_map[payload] = residual

        except:
            pass

print()
print(f"Unique payloads: {len(payload_map):,}")

payloads = list(payload_map.keys())

# ------------------------------------------------------------
# Discover affine basis vectors
# ------------------------------------------------------------

basis = {}

print()
print("=" * 80)
print("AFFINE BASIS DISCOVERY")
print("=" * 80)

processed = 0

for payload in payloads:

    processed += 1

    if processed % 10000 == 0:

        print(
            f"Processed {processed:,} / {len(payloads):,}"
        )

    r1 = payload_map[payload]

    for byte_idx in range(7):

        for bit_idx in range(8):

            modified = list(payload)

            modified[byte_idx] ^= (1 << bit_idx)

            modified = tuple(modified)

            if modified not in payload_map:
                continue

            r2 = payload_map[modified]

            delta = r1 ^ r2

            key = (byte_idx, bit_idx)

            if key not in basis:
                basis[key] = delta

            elif basis[key] != delta:

                print()
                print(
                    f"NON-LINEAR:"
                    f" byte={byte_idx+1}"
                    f" bit={bit_idx}"
                    f" old=0x{basis[key]:02X}"
                    f" new=0x{delta:02X}"
                )

# ------------------------------------------------------------
# Basis report
# ------------------------------------------------------------

print()
print("=" * 80)
print("DISCOVERED BASIS")
print("=" * 80)

for key in sorted(basis):

    print(
        f"b{key[0]+1}[{key[1]}]"
        f" -> 0x{basis[key]:02X}"
    )

# ------------------------------------------------------------
# Verify affine model
# ------------------------------------------------------------

print()
print("=" * 80)
print("VERIFYING MODEL")
print("=" * 80)

correct = 0
total = 0

for payload, observed in payload_map.items():

    calc = 0

    for byte_idx in range(7):

        value = payload[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                key = (byte_idx, bit_idx)

                if key in basis:

                    calc ^= basis[key]

    total += 1

    if calc == observed:
        correct += 1

accuracy = 100.0 * correct / total

print()
print(f"Correct : {correct:,}")
print(f"Total   : {total:,}")
print(f"Accuracy: {accuracy:.6f}%")

# ------------------------------------------------------------
# Residual histogram
# ------------------------------------------------------------

hist = defaultdict(int)

for payload, observed in payload_map.items():

    calc = 0

    for byte_idx in range(7):

        value = payload[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                key = (byte_idx, bit_idx)

                if key in basis:
                    calc ^= basis[key]

    hist[observed ^ calc] += 1

print()
print("=" * 80)
print("TOP RESIDUALS")
print("=" * 80)

for value, count in sorted(
        hist.items(),
        key=lambda x: x[1],
        reverse=True
    )[:64]:

    pct = 100.0 * count / total

    print(
        f"0x{value:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

print()
print("Done.")