#!/usr/bin/env python3

import csv

from a8_final_validator_v14 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

print()
print("=" * 80)
print("A8 LAST 6 FRAMES")
print("=" * 80)

# ------------------------------------------------------------------

frames = []

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

        if residual == 0:
            continue

        rpm, stat, dyn = decode_signals(frame)

        frames.append({
            "idx": idx,
            "frame": frame,
            "actual": actual,
            "predicted": predicted,
            "residual": residual,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
        })

print()
print(f"Remaining Frames: {len(frames)}")

# ------------------------------------------------------------------

for i, f in enumerate(frames):

    print()
    print("=" * 80)
    print(
        f"MISMATCH {i+1}/{len(frames)}"
    )
    print("=" * 80)

    print()

    print(
        f"IDX      : {f['idx']}"
    )

    print(
        f"ACT      : 0x{f['actual']:02X}"
    )

    print(
        f"PRED     : 0x{f['predicted']:02X}"
    )

    print(
        f"RESIDUAL : 0x{f['residual']:02X}"
    )

    print()

    print(
        f"RPM  = {f['rpm']:.1f}"
    )

    print(
        f"STAT = {f['stat']}"
    )

    print(
        f"DYN  = {f['dyn']}"
    )

    print()

    print(
        "FRAME : "
        + " ".join(
            f"{x:02X}"
            for x in f["frame"]
        )
    )

    # ----------------------------------------------------------
    # Previous mismatch
    # ----------------------------------------------------------

    if i > 0:

        prev = frames[i - 1]

        print()
        print("PREVIOUS REMAINING MISMATCH")

        print(
            f"IDX={prev['idx']} "
            f"RES=0x{prev['residual']:02X}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in prev["frame"]
            )
        )

    # ----------------------------------------------------------
    # Next mismatch
    # ----------------------------------------------------------

    if i < len(frames) - 1:

        nxt = frames[i + 1]

        print()
        print("NEXT REMAINING MISMATCH")

        print(
            f"IDX={nxt['idx']} "
            f"RES=0x{nxt['residual']:02X}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in nxt["frame"]
            )
        )

print()
print("Done.")