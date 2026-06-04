#!/usr/bin/env python3

from collections import Counter
import csv

from a8_final_validator_v14 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals
)

TARGETS = [
    0x5A,
    0x10,
    0xD3,
    0x3F,
]

print()
print("=" * 80)
print("A8 FINAL FOUR RESIDUALS")
print("=" * 80)

# ------------------------------------------------------------------

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

                "frame": frame
            })

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{TARGET:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {len(frames)}")

    # ----------------------------------------------------------
    # Pair summary
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
    print("PAIR SUMMARY")
    print("-" * 40)

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
            f"{count:4d} "
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
    print("-" * 40)

    print(
        f"RPM  "
        f"{min(rpms):8.2f} .. "
        f"{max(rpms):8.2f} "
        f"avg={sum(rpms)/len(rpms):8.2f}"
    )

    print(
        f"STAT "
        f"{min(stats):8.2f} .. "
        f"{max(stats):8.2f} "
        f"avg={sum(stats)/len(stats):8.2f}"
    )

    print(
        f"DYN  "
        f"{min(dyns):8.2f} .. "
        f"{max(dyns):8.2f} "
        f"avg={sum(dyns)/len(dyns):8.2f}"
    )

    # ----------------------------------------------------------
    # Byte distributions
    # ----------------------------------------------------------

    for byte_idx in [4,5,6,7]:

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_idx]
            ] += 1

        print()
        print(f"B{byte_idx}")
        print("-" * 40)

        for value, count in hist.most_common(15):

            pct = (
                100.0 *
                count /
                len(frames)
            )

            print(
                f"0x{value:02X} "
                f"{count:4d} "
                f"({pct:6.2f}%)"
            )

    # ----------------------------------------------------------
    # First examples
    # ----------------------------------------------------------

    print()
    print("FIRST 20 FRAMES")
    print("-" * 40)

    for f in frames[:20]:

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

print()
print("Done.")