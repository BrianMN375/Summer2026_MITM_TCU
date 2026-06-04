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
    "a8_full_0007.csv"
)

# ------------------------------------------------------------------
# Counter transform table
# ------------------------------------------------------------------

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ------------------------------------------------------------------
# Linear vectors
# ------------------------------------------------------------------

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

# ------------------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ------------------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ------------------------------------------------------------------

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

# ------------------------------------------------------------------

print()
print("Loading dataset...")

frames = []

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

        if (
            frame[5] == 0x00 and
            frame[6] == 0x00 and
            frame[7] == 0x00
        ):
            frames.append(frame)

# ------------------------------------------------------------------

print()
print("=" * 80)
print("A8 STARTUP FAMILY SEARCH")
print("=" * 80)

print()
print(f"Frames : {len(frames):,}")

# ------------------------------------------------------------------

residual_counts = Counter()
byte4_counts    = Counter()

rpm_values  = []
stat_values = []
dyn_values  = []

examples = defaultdict(list)

for frame in frames:

    residual = actual_residual(frame)

    residual_counts[residual] += 1
    byte4_counts[frame[4]] += 1

    rpm, stat, dyn = decode_signals(frame)

    rpm_values.append(rpm)
    stat_values.append(stat)
    dyn_values.append(dyn)

    if len(examples[residual]) < 20:
        examples[residual].append(
            (
                rpm,
                stat,
                dyn,
                frame
            )
        )

# ------------------------------------------------------------------

print()
print("Actual Residual Distribution")
print("----------------------------")

for value, count in residual_counts.most_common():

    pct = (
        100.0 *
        count /
        len(frames)
    )

    print(
        f"0x{value:02X} "
        f"{count:6d} "
        f"({pct:5.1f}%)"
    )

# ------------------------------------------------------------------

print()
print("Byte4 Distribution")
print("------------------")

for value, count in byte4_counts.most_common():

    pct = (
        100.0 *
        count /
        len(frames)
    )

    print(
        f"0x{value:02X} "
        f"{count:6d} "
        f"({pct:5.1f}%)"
    )

# ------------------------------------------------------------------

print()
print("Signal Statistics")
print("-----------------")

print(
    f"RPM  : {min(rpm_values):.1f}"
    f"..{max(rpm_values):.1f}"
    f" avg={sum(rpm_values)/len(rpm_values):.1f}"
)

print(
    f"STAT : {min(stat_values)}"
    f"..{max(stat_values)}"
    f" avg={sum(stat_values)/len(stat_values):.1f}"
)

print(
    f"DYN  : {min(dyn_values)}"
    f"..{max(dyn_values)}"
    f" avg={sum(dyn_values)/len(dyn_values):.1f}"
)

# ------------------------------------------------------------------

for residual in sorted(examples.keys()):

    print()
    print("=" * 60)
    print(f"RESIDUAL 0x{residual:02X}")
    print("=" * 60)

    for rpm, stat, dyn, frame in examples[residual]:

        frame_str = " ".join(
            f"{b:02X}"
            for b in frame
        )

        print(
            f"RPM={rpm:7.1f} "
            f"STAT={stat:4d} "
            f"DYN={dyn:4d} "
            f"B4=0x{frame[4]:02X} "
            f"{frame_str}"
        )

print()
print("Done.")