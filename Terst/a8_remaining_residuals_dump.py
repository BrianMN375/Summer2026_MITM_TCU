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

MIN_COUNT = 10

print()
print("=" * 80)
print("A8 REMAINING RESIDUALS DUMP")
print("=" * 80)

# ------------------------------------------------------------------

all_frames = []
residual_hist = Counter()

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

        entry = {

            "idx": idx,

            "frame": frame,

            "actual": actual,
            "predicted": predicted,

            "residual": residual,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn
        }

        all_frames.append(entry)

        residual_hist[residual] += 1

# ------------------------------------------------------------------

print()
print("=" * 80)
print("RESIDUAL SUMMARY")
print("=" * 80)

for residual, count in residual_hist.most_common():

    if count < MIN_COUNT:
        continue

    print(
        f"0x{residual:02X} "
        f"{count:5d}"
    )

# ------------------------------------------------------------------

interesting = [

    residual
    for residual, count
    in residual_hist.most_common()
    if count >= MIN_COUNT

]

# ------------------------------------------------------------------

for residual in interesting:

    frames = [

        f
        for f in all_frames
        if f["residual"] == residual

    ]

    print()
    print("=" * 80)
    print(
        f"RESIDUAL 0x{residual:02X}"
    )
    print("=" * 80)

    print()
    print(
        f"Frames : {len(frames)}"
    )

    # --------------------------------------------------------------
    # Pair summary
    # --------------------------------------------------------------

    pair_hist = Counter()

    for f in frames:

        pair_hist[
            (
                f["actual"],
                f["predicted"]
            )
        ] += 1

    print()
    print("-" * 80)
    print("PAIR SUMMARY")
    print("-" * 80)

    for (actual,predicted), count in pair_hist.most_common():

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

    # --------------------------------------------------------------
    # Signal stats
    # --------------------------------------------------------------

    rpms = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns = [f["dyn"] for f in frames]

    print()
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

    # --------------------------------------------------------------
    # B4 B5 B7
    # --------------------------------------------------------------

    for byte_index in [4,5,7]:

        print()
        print(f"B{byte_index}")

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_index]
            ] += 1

        for value, count in hist.most_common(12):

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

    # --------------------------------------------------------------
    # Example frames
    # --------------------------------------------------------------

    print()
    print("FIRST 10 EXAMPLES")

    for f in frames[:10]:

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