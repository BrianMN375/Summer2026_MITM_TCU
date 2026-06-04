#!/usr/bin/env python3

from collections import Counter

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals
)

TARGET = 0x91

print()
print("=" * 80)
print("A8 0x91 FAMILY SOLVER")
print("=" * 80)

# ------------------------------------------------------------------

frames = []

import csv

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
            "frame": frame,
            "actual": actual,
            "predicted": predicted,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn
        })

print()
print(f"Frames : {len(frames):,}")

# ------------------------------------------------------------------
# ACTUAL VALUES
# ------------------------------------------------------------------

print()
print("=" * 80)
print("ACTUAL VALUE DISTRIBUTION")
print("=" * 80)

hist = Counter()

for f in frames:
    hist[f["actual"]] += 1

for value, count in hist.most_common():

    pct = 100.0 * count / len(frames)

    print(
        f"0x{value:02X} "
        f"{count:6d} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------------
# PREDICTED VALUES
# ------------------------------------------------------------------

print()
print("=" * 80)
print("PREDICTED VALUE DISTRIBUTION")
print("=" * 80)

hist = Counter()

for f in frames:
    hist[f["predicted"]] += 1

for value, count in hist.most_common():

    pct = 100.0 * count / len(frames)

    print(
        f"0x{value:02X} "
        f"{count:6d} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------------
# ACT/PRED PAIRS
# ------------------------------------------------------------------

print()
print("=" * 80)
print("ACTUAL / PREDICTED PAIRS")
print("=" * 80)

pair_hist = Counter()

for f in frames:

    pair_hist[
        (
            f["actual"],
            f["predicted"]
        )
    ] += 1

for (a,p), count in pair_hist.most_common():

    pct = 100.0 * count / len(frames)

    print(
        f"ACT=0x{a:02X} "
        f"PRED=0x{p:02X} "
        f"{count:6d} "
        f"({pct:6.2f}%)"
    )

# ------------------------------------------------------------------
# B4
# ------------------------------------------------------------------

print()
print("=" * 80)
print("B4 DISTRIBUTION")
print("=" * 80)

hist = Counter()

for f in frames:

    hist[
        f["frame"][4]
    ] += 1

for value, count in hist.most_common():

    print(
        f"0x{value:02X} "
        f"{count}"
    )

# ------------------------------------------------------------------
# B7
# ------------------------------------------------------------------

print()
print("=" * 80)
print("B7 DISTRIBUTION")
print("=" * 80)

hist = Counter()

for f in frames:

    hist[
        f["frame"][7]
    ] += 1

for value, count in hist.most_common():

    print(
        f"0x{value:02X} "
        f"{count}"
    )

# ------------------------------------------------------------------
# RPM / STAT / DYN
# ------------------------------------------------------------------

print()
print("=" * 80)
print("SIGNAL STATISTICS")
print("=" * 80)

rpms = [f["rpm"] for f in frames]
stats = [f["stat"] for f in frames]
dyns = [f["dyn"] for f in frames]

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

# ------------------------------------------------------------------
# TOP EXAMPLES
# ------------------------------------------------------------------

print()
print("=" * 80)
print("FIRST 50 EXAMPLES")
print("=" * 80)

for f in frames[:50]:

    b = f["frame"]

    print(
        f"IDX={f['idx']:6d} "
        f"ACT=0x{f['actual']:02X} "
        f"PRED=0x{f['predicted']:02X} "
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