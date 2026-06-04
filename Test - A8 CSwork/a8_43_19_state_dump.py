#!/usr/bin/env python3

import csv

from a8_v15_candidate_overlays import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

print()
print("=" * 80)
print("A8 43->19 STATE DUMP")
print("=" * 80)

print(f"CSV_FILE = {CSV_FILE}")

matches = []

# ----------------------------------------------------------

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

        if not (
            actual == 0x19
            and predicted == 0x43
        ):
            continue

        rpm, stat, dyn = decode_signals(frame)

        matches.append(
            (
                idx,
                rpm,
                stat,
                dyn,
                frame
            )
        )

# ----------------------------------------------------------

print()
print(f"Frames : {len(matches)}")

print()
print("=" * 80)
print("FULL DUMP")
print("=" * 80)

for idx, rpm, stat, dyn, frame in matches:

    print(
        f"IDX={idx:6d} "
        f"RPM={rpm:8.1f} "
        f"STAT={stat:4d} "
        f"DYN={dyn:4d}"
    )

    print(
        " ".join(
            f"{b:02X}"
            for b in frame
        )
    )

print()
print("Done.")