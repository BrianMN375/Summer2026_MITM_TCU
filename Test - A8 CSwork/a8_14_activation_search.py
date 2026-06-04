#!/usr/bin/env python3

import csv
from collections import Counter

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

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

KNOWN_VECTORS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,0):0x54,
    (3,1):0xA8,
    (3,2):0x7F,
    (3,3):0xFE,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,

    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ------------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ------------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ------------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

    linear = 0

    for byte_idx in range(2, 8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ------------------------------------------------------------

frames = []

print()
print("Loading dataset...")

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

        actual = actual_residual(frame)

        if actual != 0x14:
            continue

        frames.append(frame)

# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 0x14 ACTIVATION SEARCH")
print("=" * 80)

print()
print(f"Frames : {len(frames)}")

if not frames:

    print("\nNo frames found.")
    raise SystemExit

byte4s = Counter()
byte5s = Counter()
byte6s = Counter()
byte7s = Counter()

rpms  = []
stats = []
dyns  = []

for frame in frames:

    byte4s[frame[4]] += 1
    byte5s[frame[5]] += 1
    byte6s[frame[6]] += 1
    byte7s[frame[7]] += 1

    rpm, stat, dyn = decode_signals(frame)

    rpms.append(rpm)
    stats.append(stat)
    dyns.append(dyn)

# ------------------------------------------------------------

print()
print("Signal Statistics")
print("-----------------")

print(
    f"RPM  : {min(rpms):.0f}..{max(rpms):.0f} "
    f"avg={sum(rpms)/len(rpms):.1f}"
)

print(
    f"STAT : {min(stats)}..{max(stats)} "
    f"avg={sum(stats)/len(stats):.1f}"
)

print(
    f"DYN  : {min(dyns)}..{max(dyns)} "
    f"avg={sum(dyns)/len(dyns):.1f}"
)

# ------------------------------------------------------------

def dump_counter(name, counter):

    print()
    print(name)
    print("-" * 12)

    for value, count in counter.most_common():

        pct = 100.0 * count / len(frames)

        print(
            f"0x{value:02X} "
            f"{count:5d} "
            f"({pct:5.1f}%)"
        )

dump_counter("Byte4", byte4s)
dump_counter("Byte5", byte5s)
dump_counter("Byte6", byte6s)
dump_counter("Byte7", byte7s)

# ------------------------------------------------------------

print()
print("Example Frames")
print("--------------")

for frame in frames[:50]:

    rpm, stat, dyn = decode_signals(frame)

    data = " ".join(
        f"{b:02X}"
        for b in frame
    )

    print(
        f"RPM={rpm:7.2f} "
        f"STAT={stat:3d} "
        f"DYN={dyn:4d} "
        f"{data}"
    )

print()
print("Done.")