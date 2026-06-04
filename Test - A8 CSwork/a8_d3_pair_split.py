#!/usr/bin/env python3

from collections import Counter, defaultdict
import csv

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals
)

TARGET = 0xD3

print()
print("=" * 80)
print("A8 D3 PAIR SPLIT")
print("=" * 80)

# ------------------------------------------------------------------

pair_frames = defaultdict(list)

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

        pair_frames[
            (actual, predicted)
        ].append({

            "idx": idx,
            "frame": frame,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn
        })

# ------------------------------------------------------------------

print()
print("=" * 80)
print("PAIR SUMMARY")
print("=" * 80)

for pair, frames in sorted(
    pair_frames.items(),
    key=lambda x: len(x[1]),
    reverse=True
):

    actual, predicted = pair

    print(
        f"ACT=0x{actual:02X} "
        f"PRED=0x{predicted:02X} "
        f"COUNT={len(frames)}"
    )

# ------------------------------------------------------------------

for pair, frames in sorted(
    pair_frames.items(),
    key=lambda x: len(x[1]),
    reverse=True
):

    actual, predicted = pair

    print()
    print("=" * 80)
    print(
        f"ACT=0x{actual:02X} "
        f"PRED=0x{predicted:02X}"
    )
    print("=" * 80)

    rpms  = [f["rpm"]  for f in frames]
    stats = [f["stat"] for f in frames]
    dyns  = [f["dyn"]  for f in frames]

    print()

    print(
        f"RPM  "
        f"min={min(rpms):8.2f} "
        f"max={max(rpms):8.2f} "
        f"avg={sum(rpms)/len(rpms):8.2f}"
    )

    print(
        f"STAT "
        f"min={min(stats):8.2f} "
        f"max={max(stats):8.2f} "
        f"avg={sum(stats)/len(stats):8.2f}"
    )

    print(
        f"DYN  "
        f"min={min(dyns):8.2f} "
        f"max={max(dyns):8.2f} "
        f"avg={sum(dyns)/len(dyns):8.2f}"
    )

    # ----------------------------------------------------------

    for byte_index in [4,5,7]:

        print()
        print(f"B{byte_index}")

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_index]
            ] += 1

        for value, count in hist.most_common(15):

            pct = (
                100.0 *
                count /
                len(frames)
            )

            print(
                f"0x{value:02X} "
                f"{count:6d} "
                f"({pct:6.2f}%)"
            )

    # ----------------------------------------------------------

    print()
    print("FIRST 15 FRAMES")

    for f in frames[:15]:

        b = f["frame"]

        print(
            f"IDX={f['idx']:6d} "
            f"RPM={f['rpm']:8.2f} "
            f"STAT={f['stat']:4d} "
            f"DYN={f['dyn']:4d}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in b
            )
        )

# ------------------------------------------------------------------

print()
print("Done.")
