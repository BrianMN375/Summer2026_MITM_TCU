#!/usr/bin/env python3

import csv

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals
)

TARGETS = {
    0x91,
    0xD3
}

print()
print("=" * 80)
print("A8 NEW FAMILY EXTRACTOR")
print("=" * 80)

counts = {
    0x91: 0,
    0xD3: 0
}

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    idx = 0

    for row in reader:

        frame = load_frame_from_row(row)

        if frame is None:
            continue

        idx += 1

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        residual = actual ^ predicted

        if residual not in TARGETS:
            continue

        counts[residual] += 1

        rpm, stat, dyn = decode_signals(frame)

        b0,b1,b2,b3,b4,b5,b6,b7 = frame

        counter = b1 & 0x0F

        print(
            f"IDX={idx:6d} "
            f"RES=0x{residual:02X} "
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X}"
        )

        print(
            f"CTR={counter:2d} "
            f"RPM={rpm:8.2f} "
            f"STAT={stat:4d} "
            f"DYN={dyn:4d}"
        )

        print(
            f"{b0:02X} "
            f"{b1:02X} "
            f"{b2:02X} "
            f"{b3:02X} "
            f"{b4:02X} "
            f"{b5:02X} "
            f"{b6:02X} "
            f"{b7:02X}"
        )

        print()

print()
print("=" * 80)
print("SUMMARY")
print("=" * 80)

print(f"0x91 : {counts[0x91]}")
print(f"0xD3 : {counts[0xD3]}")

print()
print("Done.")