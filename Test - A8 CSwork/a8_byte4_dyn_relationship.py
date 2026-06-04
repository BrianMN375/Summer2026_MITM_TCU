#!/usr/bin/env python3

import csv
import math
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

# ---------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ---------------------------------------------------------

def decode_dyn(frame):

    raw = build_u64(frame)

    return ((raw >> 30) & 0x3FF) - 509

# ---------------------------------------------------------

byte4_dyn = defaultdict(list)

all_b4 = []
all_dyn = []

frames = 0

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    for row in reader:

        if not row:
            continue

        if row[0].lower() == "time":
            continue

        try:

            frame = [
                int(x.strip(), 16)
                for x in row[1:9]
            ]

        except:
            continue

        b4 = frame[4]
        dyn = decode_dyn(frame)

        byte4_dyn[b4].append(dyn)

        all_b4.append(b4)
        all_dyn.append(dyn)

        frames += 1

# ---------------------------------------------------------
# Pearson correlation
# ---------------------------------------------------------

mean_b4 = sum(all_b4) / len(all_b4)
mean_dyn = sum(all_dyn) / len(all_dyn)

num = 0.0
den_b4 = 0.0
den_dyn = 0.0

for b4, dyn in zip(all_b4, all_dyn):

    db4 = b4 - mean_b4
    ddyn = dyn - mean_dyn

    num += db4 * ddyn
    den_b4 += db4 * db4
    den_dyn += ddyn * ddyn

pearson = num / math.sqrt(den_b4 * den_dyn)

# ---------------------------------------------------------

print()
print("=" * 60)
print("A8 BYTE4 vs DYN RELATIONSHIP")
print("=" * 60)

print()
print(f"Frames: {frames:,}")

print()
print(f"Pearson Correlation = {pearson:.6f}")

print()

if abs(pearson) > 0.95:
    print("Very strong linear relationship detected.")
elif abs(pearson) > 0.90:
    print("Strong linear relationship detected.")
elif abs(pearson) > 0.80:
    print("Moderate relationship detected.")
else:
    print("Weak relationship detected.")

# ---------------------------------------------------------

print()
print("Per-Byte4 Statistics")
print("-" * 60)

print(
    f"{'Byte4':<8}"
    f"{'Frames':>8} "
    f"{'AvgDYN':>10} "
    f"{'StdDev':>10} "
    f"{'Min':>8} "
    f"{'Max':>8}"
)

print("-" * 60)

for b4 in sorted(byte4_dyn.keys()):

    values = byte4_dyn[b4]

    if len(values) < 25:
        continue

    avg = sum(values) / len(values)

    variance = sum(
        (x - avg) ** 2
        for x in values
    ) / len(values)

    stddev = math.sqrt(variance)

    print(
        f"0x{b4:02X}    "
        f"{len(values):8d} "
        f"{avg:10.2f} "
        f"{stddev:10.2f} "
        f"{min(values):8d} "
        f"{max(values):8d}"
    )

print()
print("Done.")