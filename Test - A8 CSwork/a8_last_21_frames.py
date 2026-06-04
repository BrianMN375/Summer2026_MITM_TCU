#!/usr/bin/env python3

from collections import Counter
import csv

from a8_final_validator_v14 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

TARGETS = (
    0x5A,
    0xDE,
    0xA9,
    0x99,
    0x84,
)

print()
print("=" * 80)
print("A8 LAST 21 FRAMES")
print("=" * 80)

for TARGET in TARGETS:

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

            if residual != TARGET:
                continue

            rpm, stat, dyn = decode_signals(frame)

            frames.append({
                "idx": idx,
                "actual": actual,
                "predicted": predicted,
                "rpm": rpm,
                "stat": stat,
                "dyn": dyn,
                "frame": frame,
            })

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{TARGET:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {len(frames)}")

    if not frames:
        continue

    # ----------------------------------------------------------
    # Pair summary
    # ----------------------------------------------------------

    pair_hist = Counter()

    for f in frames:
        pair_hist[(f["actual"], f["predicted"])] += 1

    print()
    print("PAIR SUMMARY")
    print("-" * 60)

    for (actual, predicted), count in pair_hist.most_common():

        print(
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            f"COUNT={count}"
        )

    # ----------------------------------------------------------
    # Signal summary
    # ----------------------------------------------------------

    rpms = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns = [f["dyn"] for f in frames]

    print()
    print("SIGNALS")
    print("-" * 60)

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

    for byte_idx in [4, 5, 6, 7]:

        hist = Counter()

        for f in frames:
            hist[f["frame"][byte_idx]] += 1

        print()
        print(f"B{byte_idx}")
        print("-" * 60)

        for value, count in hist.most_common():

            print(
                f"0x{value:02X} "
                f"{count}"
            )

    # ----------------------------------------------------------
    # Full frame dump
    # ----------------------------------------------------------

    print()
    print("FULL FRAME DUMP")
    print("-" * 60)

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