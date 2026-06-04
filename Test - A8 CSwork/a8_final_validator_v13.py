#!/usr/bin/env python3

import csv
from collections import Counter


# ------------------------------------------------------------------
# CSV Loader
# Supports:
#
# 1)
# time,b0,b1,b2,b3,b4,b5,b6,b7
#
# 2)
# frame,time,id,len,b0,b1,b2,b3,b4,b5,b6,b7
#
# ------------------------------------------------------------------

def load_frame_from_row(row):

    # ----------------------------------------------------------
    # DictReader format
    # ----------------------------------------------------------

    if isinstance(row, dict):

        # New logger
        if "b0" in row:

            frame = []

            for i in range(8):

                value = row[f"b{i}"].strip()

                if value == "":
                    value = "0"

                frame.append(int(value, 16))

            return frame

        return None

    # ----------------------------------------------------------
    # Legacy reader format
    # ----------------------------------------------------------

    try:

        # Header row
        if row[0].lower() in (
            "time",
            "frame"
        ):
            return None

    except:
        return None

    # time,b0,b1,b2,b3,b4,b5,b6,b7

    if len(row) >= 9:

        try:

            return [
                int(row[1], 16),
                int(row[2], 16),
                int(row[3], 16),
                int(row[4], 16),
                int(row[5], 16),
                int(row[6], 16),
                int(row[7], 16),
                int(row[8], 16),
            ]

        except:
            return None

    return None


CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0015_a8only.csv"
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
# Current best state model (V7)
# ------------------------------------------------------------------

def predicted_residual(frame):

    byte4 = frame[4]
    byte5 = frame[5]
    byte6 = frame[6]
    byte7 = frame[7]

    rpm, stat, dyn = decode_signals(frame)

    # ----------------------------------------------------------
    # Startup family
    # ----------------------------------------------------------

    if (
        byte5 == 0x00
        and byte6 == 0x00
        and byte7 == 0x00
    ):

        if byte4 == 0xFF:
            return 0x0D

        if 0x76 <= byte4 <= 0x7F:
            return 0xB4


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
        0x91 <= byte4 <= 0x98
        and byte5 == 0x64
        and stat >= 247
        and dyn >= 72
    ):
        predicted = 0xC0


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

        if byte7 >= 0x40:
            predicted = 0xC0
        else:
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

                if (
                    byte5 == 0x64 and
                    121 <= stat <= 127
                ):
                    predicted = 0x14

                else:

                    if (
                        byte5 == 0x64
                        and
                        0x80 <= byte4 <= 0x86
                        and
                        byte7 >= 0x4C
                    ):
                        predicted = 0x19

                    else:
                        predicted = 0xC7

    # ----------------------------------------------------------
    # ALT NEGATIVE FAMILY
    #
    # Byte5 = 0x63
    #
    # 0x25 -> 0x5F
    # 0xF6 -> 0x8C
    # 0xBC -> 0xC6
    # ----------------------------------------------------------

    if byte5 == 0x63:

        if predicted in (
            0x25,
            0xF6,
            0xBC,
            0xC7,

            # Newly discovered
            0x5E,
            0x8D
        ):

            predicted ^= 0x7A

    # ----------------------------------------------------------
    # ALT2 FAMILY
    #
    # Byte5 = 0x62
    #
    # 0x25 -> 0xEA
    # 0xBC -> 0x73
    # 0xC7 -> 0x08
    #
    # XOR = 0xCF
    # ----------------------------------------------------------

    if byte5 == 0x62:

        if predicted in (
            0x25,
            0xBC,
            0xC7,

            # Newly discovered
            0x1E,
            0x5E,
            0xF6
        ):

            predicted ^= 0xCF
    if byte5 == 0x62:

        if predicted in (
            0x25,
            0xBC,
            0xC7,

            # Newly discovered
            0x1E,
            0x5E,
            0xF6
        ):

            predicted ^= 0xCF


    # ----------------------------------------------------------
    # ALT3 FAMILY
    #
    # Byte5 = 0x61
    #
    # 0x25 -> 0x1A
    # 0xBC -> 0x83
    # 0xC7 -> 0xF8
    #
    # XOR = 0x3F
    # ----------------------------------------------------------

    if byte5 == 0x61:

        if predicted in (0x25, 0xBC, 0xC7):

            predicted ^= 0x3F


    # ALT4 FAMILY

    if byte5 == 0x60:

        if predicted in (
            0x25,
            0xBC,
            0xC7
        ):
            predicted ^= 0x8A


    # ----------------------------------------------------------
    # HIGH NEGATIVE OVERLAY
    #
    # Normal:
    #   0xBC
    #
    # Overlay:
    #   0x62
    #   0x38
    #
    # Byte7[6]=1
    # Byte7[5]=0
    # ----------------------------------------------------------

    if predicted == 0xBC:

        if (byte7 & 0x60) == 0x40:

            if stat < 256:
                predicted = 0x62
            else:
                predicted = 0x38


    # ----------------------------------------------------------
    # B5 Overlay Ladder
    # ----------------------------------------------------------

    B5_OVERLAY_TABLE = {
        0x60: 0x8A,
        0x5F: 0x51,
        0x5E: 0xE4,
        0x5D: 0x14,
        0x5C: 0xA1,
        0x5B: 0xDB,
        0x5A: 0x6E,
        0x59: 0x9E,
        0x58: 0x2B,
        0x57: 0x6A,
    }


    # ----------------------------------------------------------
    # 0x91 Overlay Candidate
    # ----------------------------------------------------------

    if byte5 == 0x00:

        if (
            predicted == 0x1E
            and
            byte4 >= 0x94
            and
            byte7 >= 0x2D
        ):
            predicted ^= 0x91

        elif (
            predicted == 0x5E
            and
            byte7 == 0x15
        ):
            predicted ^= 0x91

    # ----------------------------------------------------------
    # D3 Overlay Candidate V2
    # ----------------------------------------------------------

    if (
        98 <= stat <= 127
        and
        predicted in (
            0xBC,
            0xBD,
            0xC7,
            0xC6
        )
    ):
        predicted ^= 0xD3



        
    return predicted

# ------------------------------------------------------------------

# ------------------------------------------------------------------

def run_validator():

    total = 0
    correct = 0

    mismatches = Counter()

    print()
    print("Using CSV:")
    print(CSV_FILE)

    print()
    print("=" * 60)
    print("A8 FINAL VALIDATOR V13")
    print("=" * 60)

    with open(CSV_FILE, newline="") as csvfile:

        reader = csv.DictReader(csvfile)

        for row in reader:

            frame = load_frame_from_row(row)

            if frame is None:
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

    if total == 0:

        print()
        print("ERROR: No frames parsed.")
        print("Check CSV format.")
        print()

        return

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


# ------------------------------------------------------------------

if __name__ == "__main__":

    run_validator()