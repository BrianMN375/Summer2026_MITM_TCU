#!/usr/bin/env python3

from collections import Counter, defaultdict
import csv

from a8_v15_candidate_overlays import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

TARGET = 0xDE

print()
print("=" * 80)
print("A8 0xDE PROFILER V2")
print("=" * 80)

print(f"CSV_FILE = {CSV_FILE}")

groups = defaultdict(list)

# ------------------------------------------------------------------

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
            "actual": actual,
            "predicted": predicted,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "frame": frame,
        })

# ------------------------------------------------------------------

total_frames = sum(len(v) for v in groups.values())

print()
print(f"Frames : {total_frames}")

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
        f"DYN  {min(dyns)} .. {max(dyns):.1f} "
        f"avg={sum(dyns)/len(dyns):.1f}"
    )

    # ----------------------------------------------------------
    # B4/B5/B6/B7
    # ----------------------------------------------------------

    for byte_idx in [4, 5, 6, 7]:

        hist = Counter()

        for f in frames:
            hist[f["frame"][byte_idx]] += 1

        print()
        print(f"B{byte_idx}")
        print("-" * 60)

        for value, count in hist.most_common(20):

            pct = (
                100.0 *
                count /
                len(frames)
            )

            print(
                f"0x{value:02X} "
                f"{count:5d} "
                f"({pct:6.2f}%)"
            )

    # ----------------------------------------------------------
    # First examples
    # ----------------------------------------------------------

    print()
    print("FIRST 25 FRAMES")
    print("-" * 60)

    for f in frames[:25]:

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