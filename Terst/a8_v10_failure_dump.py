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
# Linear checksum vectors
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
# EXACT V10 PREDICTOR
# ------------------------------------------------------------------

def predicted_residual(frame):

    byte4 = frame[4]
    byte5 = frame[5]
    byte7 = frame[7]

    rpm, stat, dyn = decode_signals(frame)

    if byte4 == 0xFF:

        predicted = 0x0D

    elif 0xB0 <= byte4 <= 0xBF:

        predicted = 0x5D

    elif 0xA0 <= byte4 <= 0xAF:

        predicted = 0x84

    elif (
        0x99 <= byte4 <= 0x9F
        and
        byte7 >= 0x40
    ):

        predicted = 0xC0

    elif 0x90 <= byte4 <= 0x9F:

        predicted = 0x1E

    else:

        neg  = dyn < 3
        high = rpm >= 2048
        mid  = stat >= 128

        if not high:

            if not mid:
                predicted = 0x25 if neg else 0x5E
            else:
                predicted = 0xF6 if neg else 0x8D

        else:

            if neg:

                predicted = 0xBC

            else:

                if (
                    byte5 == 0x64 and
                    121 <= stat <= 127
                ):
                    predicted = 0x14

                else:
                    predicted = 0xC7

    # ALT NEGATIVE FAMILY

    if byte5 == 0x63:

        if predicted in (
            0x25,
            0xF6,
            0xBC,
            0xC7
        ):
            predicted ^= 0x7A

    # ALT2 FAMILY

    if byte5 == 0x62:

        if predicted in (
            0x25,
            0xBC,
            0xC7
        ):
            predicted ^= 0xCF

    # ALT3 FAMILY

    if byte5 == 0x61:

        if predicted in (
            0x25,
            0xBC,
            0xC7
        ):
            predicted ^= 0x3F

    # HIGH NEGATIVE OVERLAY

    if predicted == 0xBC:

        if (byte7 & 0x60) == 0x40:

            if stat < 256:
                predicted = 0x62
            else:
                predicted = 0x38

    return predicted

# ------------------------------------------------------------------

print()
print("Loading dataset...")

failures = []

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
        predicted = predicted_residual(frame)

        if actual != predicted:

            rpm, stat, dyn = decode_signals(frame)

            failures.append(
                (
                    actual ^ predicted,
                    actual,
                    predicted,
                    rpm,
                    stat,
                    dyn,
                    frame
                )
            )

# ------------------------------------------------------------------

failures.sort(key=lambda x: x[0])

print()
print("=" * 90)
print("A8 V10 FAILURE DUMP")
print("=" * 90)

print()
print(f"Total Failures : {len(failures)}")

current_xor = None

for xor_val, actual, pred, rpm, stat, dyn, frame in failures:

    if xor_val != current_xor:

        current_xor = xor_val

        print()
        print("=" * 70)
        print(f"XOR 0x{xor_val:02X}")
        print("=" * 70)

    frame_str = " ".join(
        f"{b:02X}"
        for b in frame
    )

    print(
        f"ACT=0x{actual:02X} "
        f"PRED=0x{pred:02X} "
        f"RPM={rpm:7.1f} "
        f"STAT={stat:4d} "
        f"DYN={dyn:4d} "
        f"B4=0x{frame[4]:02X} "
        f"B5=0x{frame[5]:02X} "
        f"B6=0x{frame[6]:02X} "
        f"B7=0x{frame[7]:02X} "
        f"{frame_str}"
    )

print()
print("Done.")