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

TARGETS = (
    0x5A,
    0xDE,
    0x91,
    0xDB,
    0xCE,
    0x84,
)

print()
print("=" * 80)
print("A8 CROSS DATASET PROFILER")
print("=" * 80)

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

        if residual not in TARGETS:
            continue

        rpm, stat, dyn = decode_signals(frame)

        groups[residual].append({

            "idx": idx,

            "actual": actual,
            "predicted": predicted,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,

            "frame": frame
        })

# ------------------------------------------------------------------

for residual in TARGETS:

    frames = groups[residual]

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{residual:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {len(frames)}")

    if not frames:
        continue

    # ----------------------------------------------------------
    # Pair breakdown
    # ----------------------------------------------------------

    pair_hist = Counter()

    for f in frames:

        pair_hist[
            (
                f["actual"],
                f["predicted"]
            )
        ] += 1

    print()
    print("PAIR BREAKDOWN")
    print("-" * 60)

    for (
        actual,
        predicted
    ), count in pair_hist.most_common():

        pct = (
            100.0 *
            count /
            len(frames)
        )

        print(
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            f"{count:5d} "
            f"({pct:6.2f}%)"
        )

    # ----------------------------------------------------------
    # Signal ranges
    # ----------------------------------------------------------

    rpms = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns = [f["dyn"] for f in frames]

    print()
    print("SIGNALS")
    print("-" * 60)

    print(
        f"RPM  "
        f"{min(rpms):8.1f} .. "
        f"{max(rpms):8.1f} "
        f"avg={sum(rpms)/len(rpms):8.1f}"
    )

    print(
        f"STAT "
        f"{min(stats):8.1f} .. "
        f"{max(stats):8.1f} "
        f"avg={sum(stats)/len(stats):8.1f}"
    )

    print(
        f"DYN  "
        f"{min(dyns):8.1f} .. "
        f"{max(dyns):8.1f} "
        f"avg={sum(dyns)/len(dyns):8.1f}"
    )

    # ----------------------------------------------------------
    # B4/B5/B6/B7
    # ----------------------------------------------------------

    for byte_idx in [4,5,6,7]:

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_idx]
            ] += 1

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