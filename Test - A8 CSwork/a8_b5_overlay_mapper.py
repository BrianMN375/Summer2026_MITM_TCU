#!/usr/bin/env python3

from collections import Counter, defaultdict
import csv

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
)

print()
print("=" * 80)
print("A8 B5 OVERLAY MAPPER")
print("=" * 80)

# ------------------------------------------------------------------

overlay_map = defaultdict(list)

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    for idx, row in enumerate(reader):

        frame = load_frame_from_row(row)

        if frame is None:
            continue

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        residual = actual ^ predicted

        if residual == 0:
            continue

        b5 = frame[5]

        overlay_map[b5].append(
            (
                residual,
                actual,
                predicted
            )
        )

# ------------------------------------------------------------------

for b5 in sorted(overlay_map.keys(), reverse=True):

    entries = overlay_map[b5]

    print()
    print("=" * 80)
    print(f"B5 = 0x{b5:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {len(entries)}")

    # --------------------------------------------------------------

    residual_hist = Counter()

    for residual, actual, predicted in entries:
        residual_hist[residual] += 1

    print()
    print("Residuals")
    print("-" * 40)

    for residual, count in residual_hist.most_common():

        pct = 100.0 * count / len(entries)

        print(
            f"0x{residual:02X} "
            f"{count:4d} "
            f"({pct:6.2f}%)"
        )

    # --------------------------------------------------------------

    pair_hist = Counter()

    for residual, actual, predicted in entries:

        pair_hist[
            (
                residual,
                actual,
                predicted
            )
        ] += 1

    print()
    print("Top ACT/PRED Pairs")
    print("-" * 40)

    for (
        residual,
        actual,
        predicted
    ), count in pair_hist.most_common(12):

        pct = 100.0 * count / len(entries)

        print(
            f"RES=0x{residual:02X} "
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            f"{count:4d} "
            f"({pct:6.2f}%)"
        )

print()
print("=" * 80)
print("GLOBAL B5 SUMMARY")
print("=" * 80)

for b5 in sorted(overlay_map.keys(), reverse=True):

    residual_hist = Counter()

    for residual, _, _ in overlay_map[b5]:
        residual_hist[residual] += 1

    total = len(overlay_map[b5])

    residual_str = ", ".join(
        f"0x{r:02X}:{c}"
        for r, c in residual_hist.most_common(6)
    )

    print(
        f"B5=0x{b5:02X} "
        f"Frames={total:4d} "
        f"{residual_str}"
    )

print()
print("Done.")