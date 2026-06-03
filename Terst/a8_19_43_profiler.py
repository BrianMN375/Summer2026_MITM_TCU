#!/usr/bin/env python3

from collections import Counter
import csv

from a8_v15_candidate_overlays import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals,
)

print()
print("=" * 80)
print("A8 19->43 PROFILER")
print("=" * 80)

print(f"CSV_FILE = {CSV_FILE}")

frames = []

# ----------------------------------------------------------

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

        if not (
            actual == 0x43
            and predicted == 0x19
        ):
            continue

        rpm, stat, dyn = decode_signals(frame)

        frames.append({
            "idx": idx,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "frame": frame,
        })

# ----------------------------------------------------------

print()
print(f"Frames : {len(frames)}")

if not frames:
    print()
    print("No matching frames.")
    print()
    raise SystemExit

# ----------------------------------------------------------

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
# Histograms
# ----------------------------------------------------------

for byte_idx in [4, 7]:

    hist = Counter()

    for f in frames:
        hist[f["frame"][byte_idx]] += 1

    print()
    print(f"B{byte_idx}")
    print("-" * 60)

    for value, count in hist.most_common(20):

        pct = 100.0 * count / len(frames)

        print(
            f"0x{value:02X} "
            f"{count:5d} "
            f"({pct:6.2f}%)"
        )

# ----------------------------------------------------------
# DYN histogram
# ----------------------------------------------------------

dyn_hist = Counter()

for f in frames:
    dyn_hist[f["dyn"]] += 1

print()
print("DYN")
print("-" * 60)

for value, count in dyn_hist.most_common(20):

    pct = 100.0 * count / len(frames)

    print(
        f"{value:3d} "
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

    print(
        f"IDX={f['idx']:6d} "
        f"RPM={f['rpm']:8.1f} "
        f"STAT={f['stat']:4d} "
        f"DYN={f['dyn']:4d}"
    )

    print(
        " ".join(
            f"{b:02X}"
            for b in f["frame"]
        )
    )

print()
print("Done.")