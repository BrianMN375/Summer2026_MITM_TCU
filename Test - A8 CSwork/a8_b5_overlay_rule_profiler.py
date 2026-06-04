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

print()
print("=" * 80)
print("A8 B5 OVERLAY RULE PROFILER")
print("=" * 80)

# ------------------------------------------------------------------

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

        if residual == 0:
            continue

        rpm, stat, dyn = decode_signals(frame)

        b5 = frame[5]

        groups[b5].append({

            "idx": idx,

            "actual": actual,
            "predicted": predicted,
            "residual": residual,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,

            "frame": frame
        })

# ------------------------------------------------------------------

for b5 in sorted(groups.keys(), reverse=True):

    frames = groups[b5]

    print()
    print("=" * 80)
    print(f"B5 = 0x{b5:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {len(frames)}")

    # --------------------------------------------------------------
    # Residuals
    # --------------------------------------------------------------

    residual_hist = Counter()

    for f in frames:
        residual_hist[f["residual"]] += 1

    print()
    print("Residuals")
    print("-" * 40)

    for residual, count in residual_hist.most_common():

        pct = (
            100.0 *
            count /
            len(frames)
        )

        print(
            f"0x{residual:02X} "
            f"{count:4d} "
            f"({pct:6.2f}%)"
        )

    # --------------------------------------------------------------
    # Predicted values
    # --------------------------------------------------------------

    pred_hist = Counter()

    for f in frames:
        pred_hist[f["predicted"]] += 1

    print()
    print("Predicted Families")
    print("-" * 40)

    for pred, count in pred_hist.most_common():

        pct = (
            100.0 *
            count /
            len(frames)
        )

        print(
            f"0x{pred:02X} "
            f"{count:4d} "
            f"({pct:6.2f}%)"
        )

    # --------------------------------------------------------------
    # Actual values
    # --------------------------------------------------------------

    act_hist = Counter()

    for f in frames:
        act_hist[f["actual"]] += 1

    print()
    print("Actual Families")
    print("-" * 40)

    for act, count in act_hist.most_common():

        pct = (
            100.0 *
            count /
            len(frames)
        )

        print(
            f"0x{act:02X} "
            f"{count:4d} "
            f"({pct:6.2f}%)"
        )

    # --------------------------------------------------------------
    # Signal ranges
    # --------------------------------------------------------------

    rpms = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns = [f["dyn"] for f in frames]

    print()
    print("Signal Ranges")
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

    # --------------------------------------------------------------
    # B4 / B7
    # --------------------------------------------------------------

    for byte_index in [4,7]:

        hist = Counter()

        for f in frames:

            hist[
                f["frame"][byte_index]
            ] += 1

        print()
        print(f"B{byte_index}")
        print("-" * 40)

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
    # Overlay inference
    # --------------------------------------------------------------

    if len(residual_hist) == 1:

        residual = next(iter(residual_hist))

        print()
        print("Overlay Candidate")
        print("-" * 40)

        print(
            f"B5=0x{b5:02X} "
            f"appears to map to "
            f"XOR 0x{residual:02X}"
        )

print()
print("=" * 80)
print("Done.")