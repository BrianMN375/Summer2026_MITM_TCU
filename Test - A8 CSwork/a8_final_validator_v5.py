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
# Linear checksum vectors discovered so far
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

    rpm = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ------------------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter = frame[1] & 0x0F

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
# Current best state model
# ------------------------------------------------------------------

def predicted_residual(frame):

    byte4 = frame[4]
    byte5 = frame[5]
    byte7 = frame[7]

    rpm, stat, dyn = decode_signals(frame)

    # ----------------------------------------------------------
    # OFF state
    # ----------------------------------------------------------

    if byte4 == 0xFF:
        predicted = 0x0D

    # ----------------------------------------------------------
    # High-DYN ladder
    # ----------------------------------------------------------

    elif 0xB0 <= byte4 <= 0xBF:
        predicted = 0x5D

    elif 0xA0 <= byte4 <= 0xAF:
        predicted = 0x84

    # ----------------------------------------------------------
    # C0 transition family
    # ----------------------------------------------------------

    elif (
        0x99 <= byte4 <= 0x9F
        and
        byte7 >= 0x40
    ):
        predicted = 0xC0

    # ----------------------------------------------------------
    # Stage1 family
    # ----------------------------------------------------------

    elif 0x90 <= byte4 <= 0x9F:
        predicted = 0x1E

    # ----------------------------------------------------------
    # Main state machine
    # ----------------------------------------------------------

    else:

        neg = dyn < 3
        high = rpm >= 2048
        mid = stat >= 128

        if not high:

            if not mid:
                predicted = 0x25 if neg else 0x5E

            else:
                predicted = 0xF6 if neg else 0x8D

        else:

            if neg:
                predicted = 0xBC

            else:
                predicted = 0xC7

    # ----------------------------------------------------------
    # ALT NEGATIVE FAMILY
    #
    # Normal:
    #   0x25
    #   0xF6
    #   0xBC
    #
    # Alternate:
    #   0x5F
    #   0x8C
    #   0xC6
    #
    # Activated by Byte5 == 0x63
    # ----------------------------------------------------------

    if byte5 == 0x63:

        if predicted in (0x25, 0xF6, 0xBC):

            predicted ^= 0x7A

    # ----------------------------------------------------------
    # HIGH NEGATIVE OVERLAY
    #
    # Normal:
    #   0xBC
    #
    # Overlay:
    #   0x38
    #
    # Discovered from:
    #   0xBC ^ 0x84 = 0x38
    #
    # Signature:
    #   Byte7 bit6 = 1
    #   Byte7 bit5 = 0
    #
    # Equivalent:
    #   Byte7 in 0x40-0x5F range
    # ----------------------------------------------------------

    if predicted == 0xBC:

        if (byte7 & 0x60) == 0x40:

            predicted ^= 0x84

    return predicted
# ------------------------------------------------------------------

total = 0
correct = 0

mismatches = Counter()

print()
print("Using CSV:")
print(CSV_FILE)

print()
print("=" * 60)
print("A8 FINAL VALIDATOR V5")
print("=" * 60)

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

        total += 1

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        if actual == predicted:

            correct += 1

        else:

            mismatches[
                actual ^ predicted
            ] += 1

# ------------------------------------------------------------------

accuracy = (
    100.0 * correct / total
)

print()
print(f"Frames   : {total:,}")
print(f"Correct  : {correct:,}")
print(f"Accuracy : {accuracy:.4f}%")

print()
print("Top Mismatch XORs")
print("-----------------")

for delta, count in mismatches.most_common(20):

    pct = (
        100.0 *
        count /
        total
    )

    print(
        f"0x{delta:02X} "
        f"{count:8,d} "
        f"({pct:6.2f}%)"
    )

print()
print("Done.")