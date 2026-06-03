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
    # "a7_payload_unique.csv"
    "a7_unique.csv"
    # "a7_balanced.csv"
)

print()
print("Using CSV:")
print(CSV_FILE)
print()

# ------------------------------------------------------------
# Load Frames
# ------------------------------------------------------------

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            frames.append(frame)

        except:
            pass

print(f"Loaded {len(frames):,} frames")

# ------------------------------------------------------------
# Collect Statistics
# ------------------------------------------------------------

by_upper = defaultdict(list)

for frame in frames:

    checksum = frame[0]

    upper = frame[1] >> 4

    by_upper[upper].append(checksum)

# ------------------------------------------------------------
# Summary Table
# ------------------------------------------------------------

print()
print("=" * 80)
print("UPPER NIBBLE SUMMARY")
print("=" * 80)

print(
    f"{'Nib':<6}"
    f"{'Frames':>12}"
    f"{'UniqueCK':>12}"
    f"{'MostCommon':>14}"
    f"{'Count':>12}"
    f"{'Pct':>10}"
)

for upper in range(16):

    checksums = by_upper.get(upper, [])

    if not checksums:
        continue

    hist = Counter(checksums)

    top_ck, top_count = hist.most_common(1)[0]

    pct = 100.0 * top_count / len(checksums)

    print(
        f"{upper:<6X}"
        f"{len(checksums):>12,}"
        f"{len(hist):>12,}"
        f"{f'0x{top_ck:02X}':>14}"
        f"{top_count:>12,}"
        f"{pct:>9.2f}%"
    )

# ------------------------------------------------------------
# Detailed Breakdown
# ------------------------------------------------------------

print()
print("=" * 80)
print("TOP CHECKSUMS PER UPPER NIBBLE")
print("=" * 80)

for upper in range(16):

    checksums = by_upper.get(upper, [])

    if not checksums:
        continue

    hist = Counter(checksums)

    print()
    print(
        f"Upper Nibble 0x{upper:X}"
    )
    print("-" * 40)

    for ck, count in hist.most_common(20):

        pct = 100.0 * count / len(checksums)

        print(
            f"0x{ck:02X} "
            f"{count:8,d} "
            f"({pct:6.2f}%)"
        )

# ------------------------------------------------------------
# Constant Checksum Search
# ------------------------------------------------------------

print()
print("=" * 80)
print("CONSTANT-CHECKSUM CANDIDATES")
print("=" * 80)

for upper in range(16):

    checksums = by_upper.get(upper, [])

    if not checksums:
        continue

    hist = Counter(checksums)

    top_ck, top_count = hist.most_common(1)[0]

    pct = 100.0 * top_count / len(checksums)

    if pct > 50:

        print(
            f"Upper 0x{upper:X} "
            f"dominant checksum "
            f"0x{top_ck:02X} "
            f"({pct:.2f}%)"
        )

print()
print("Done.")
print()