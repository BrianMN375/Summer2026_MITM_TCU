#!/usr/bin/env python3

import csv

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
    0x00,0x7A,0x96,0x79,
    0xD5,0xBF,0x8F,0x9A,
    0xB8,0xCA,0x95,0x58,
    0x46,0x55,0xE7,0x68
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

# ----------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ----------------------------------------------------------

def decode_signals(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ----------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

    linear = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ----------------------------------------------------------

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
                int(x.strip(),16)
                for x in row[1:9]
            ]

        except:
            continue

        actual = actual_residual(frame)

        rpm, stat, dyn = decode_signals(frame)

        frames.append(
            (
                frame,
                actual,
                rpm,
                stat,
                dyn
            )
        )

print()
print(f"Loaded {len(frames):,} frames")

# ----------------------------------------------------------

best_acc = 0.0
best_thresh = None

for thresh in range(-20, 21):

    correct = 0

    for frame, actual, rpm, stat, dyn in frames:

        #
        # OFF state
        #

        if (
            frame[2] == 0xDF and
            frame[3] == 0xBF and
            frame[4] == 0xFF
        ):
            predicted = 0x0D

        #
        # SPECIAL HIGH
        #

        elif dyn >= 67:
            predicted = 0x1E

        else:

            #
            # NEG/POS
            #

            if dyn < thresh:
                predicted = 0x25
            else:
                predicted = 0x5E

            #
            # MID
            #

            if stat >= 128:
                predicted ^= 0xD3

            #
            # HIGH
            #

            if rpm >= 2050:
                predicted ^= 0x4A

        if predicted == actual:
            correct += 1

    accuracy = (
        100.0 * correct / len(frames)
    )

    if accuracy > best_acc:

        best_acc = accuracy
        best_thresh = thresh

        print(
            f"NEW BEST "
            f"dyn={thresh:3d} "
            f"acc={accuracy:.4f}%"
        )

print()
print("=" * 60)
print("BEST RESULT")
print("=" * 60)

print()
print(f"DYN Threshold : {best_thresh}")
print(f"Accuracy      : {best_acc:.4f}%")

print()