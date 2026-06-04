#!/usr/bin/env python3

from collections import Counter, defaultdict
import csv

from a8_final_validator_v14 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

TARGET = 0x5A

print()
print("=" * 80)
print("A8 REMAINING 0x5A SPLIT")
print("=" * 80)

groups = defaultdict(list)

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

        if residual != TARGET:
            continue

        rpm, stat, dyn = decode_signals(frame)

        groups[(actual, predicted)].append({
            "idx": idx,
            "frame": frame,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
        })

# ------------------------------------------------------------------

for (actual, predicted), frames in sorted(
    groups.items(),
    key=lambda x: len(x[1]),
    reverse=True
):

    print()
    print("=" * 80)
    print(
        f"ACT=0x{actual:02X} "
        f"PRED=0x{predicted:02X} "
        f"COUNT={len(frames)}"
    )
    print("=" * 80)

    rpms = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns = [f["dyn"] for f in frames]

    print()
    print(
        f"RPM  {min(rpms):.1f} .. {max(rpms):.1f} "
        f"avg={sum(rpms)/len(rpms):.1f}"
    )

    print(
        f"STAT {min(stats)} .. {max(stats)} "
        f"avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN  {min(dyns)} .. {max(dyns)} "
        f"avg={sum(dyns)/len(dyns):.1f}"
    )

    # ----------------------------------------------------------
    # Byte distributions
    # ----------------------------------------------------------

    for byte_idx in range(1, 8):

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_idx]
            ] += 1

        print()
        print(f"B{byte_idx}")
        print("-" * 40)

        for value, count in hist.most_common(15):

            print(
                f"0x{value:02X} "
                f"{count}"
            )

    # ----------------------------------------------------------
    # Frame dump
    # ----------------------------------------------------------

    print()
    print("FRAMES")
    print("-" * 40)

    for f in frames:

        b = f["frame"]

        print(
            f"IDX={f['idx']:6d} "
            f"RPM={f['rpm']:8.1f} "
            f"STAT={f['stat']:4d} "
            f"DYN={f['dyn']:4d}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in b
            )
        )

print()
print("Done.")