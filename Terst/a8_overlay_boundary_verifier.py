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
    "a8_full_0012.csv"
)

from a8_final_validator_v13_gold import (
    actual_residual,
    predicted_residual,
    decode_signals
)

# ------------------------------------------------------------
# CONFIG
# ------------------------------------------------------------

STAT_MIN = 250
STAT_MAX = 262

DYN1_MIN = 175
DYN1_MAX = 188

DYN2_MIN = 252
DYN2_MAX = 266

RPM_MIN = 6000
RPM_MAX = 6300

BYTE4_MIN = 120
BYTE4_MAX = 135

# ------------------------------------------------------------

stat_frames = []
dyn181_frames = []
dyn258_frames = []
rpm6144_frames = []
byte4_frames = []

print()
print("Loading dataset...")

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for row in reader:

        frame = [
            int(x, 16)
            for x in row[1:9]
        ]

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        xor_val = actual ^ predicted

        if xor_val == 0:
            continue

        rpm, stat, dyn = decode_signals(frame)

        record = (
            frame,
            actual,
            predicted,
            xor_val,
            rpm,
            stat,
            dyn
        )

        if STAT_MIN <= stat <= STAT_MAX:
            stat_frames.append(record)

        if DYN1_MIN <= dyn <= DYN1_MAX:
            dyn181_frames.append(record)

        if DYN2_MIN <= dyn <= DYN2_MAX:
            dyn258_frames.append(record)

        if RPM_MIN <= rpm <= RPM_MAX:
            rpm6144_frames.append(record)

        if BYTE4_MIN <= frame[4] <= BYTE4_MAX:
            byte4_frames.append(record)

# ------------------------------------------------------------

def dump_group(title, rows):

    print()
    print("=" * 80)
    print(title)
    print("=" * 80)

    print()
    print(f"Frames : {len(rows):,}")
    print()

    rows = sorted(
        rows,
        key=lambda x: (
            x[5],   # stat
            x[6],   # dyn
            x[4]    # rpm
        )
    )

    for (
        frame,
        actual,
        predicted,
        xor_val,
        rpm,
        stat,
        dyn
    ) in rows[:200]:

        print(
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            f"XOR=0x{xor_val:02X} "
            f"RPM={rpm:7.1f} "
            f"STAT={stat:4d} "
            f"DYN={dyn:4d} "
            f"B4=0x{frame[4]:02X} "
            f"B5=0x{frame[5]:02X} "
            f"B7=0x{frame[7]:02X} "
            + " ".join(
                f"{b:02X}"
                for b in frame
            )
        )

# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 OVERLAY BOUNDARY VERIFIER")
print("=" * 80)

dump_group(
    "STAT ~255",
    stat_frames
)

dump_group(
    "DYN ~181",
    dyn181_frames
)

dump_group(
    "DYN ~258",
    dyn258_frames
)

dump_group(
    "RPM ~6144",
    rpm6144_frames
)

dump_group(
    "BYTE4 ~127",
    byte4_frames
)

print()
print("Done.")