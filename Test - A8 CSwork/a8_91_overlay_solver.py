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
print("A8 0x91 OVERLAY SOLVER")
print("=" * 80)

# ----------------------------------------------------------

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

        b0,b1,b2,b3,b4,b5,b6,b7 = frame

        frames.append({

            "idx": idx,

            "actual": actual,
            "predicted": predicted,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,

            "b4": b4,
            "b5": b5,
            "b6": b6,
            "b7": b7
        })

# ----------------------------------------------------------

print()
print(f"Frames : {len(frames):,}")

# ----------------------------------------------------------
# BANDS
# ----------------------------------------------------------

for field in [

    "b4",
    "b5",
    "b6",
    "b7"

]:

    values = [
        f[field]
        for f in frames
    ]

    print()
    print("=" * 80)
    print(field.upper())
    print("=" * 80)

    print(
        f"Min={min(values)} "
        f"Max={max(values)} "
        f"Avg={sum(values)/len(values):.2f}"
    )

    hist = Counter(values)

    for value, count in hist.most_common(20):

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
# SIGNAL BANDS
# ----------------------------------------------------------

print()
print("=" * 80)
print("RPM")
print("=" * 80)

rpms = [f["rpm"] for f in frames]

print(
    f"Min={min(rpms):.2f} "
    f"Max={max(rpms):.2f} "
    f"Avg={sum(rpms)/len(rpms):.2f}"
)

print()
print("=" * 80)
print("STAT")
print("=" * 80)

stats = [f["stat"] for f in frames]

print(
    f"Min={min(stats):.2f} "
    f"Max={max(stats):.2f} "
    f"Avg={sum(stats)/len(stats):.2f}"
)

print()
print("=" * 80)
print("DYN")
print("=" * 80)

dyns = [f["dyn"] for f in frames]

print(
    f"Min={min(dyns):.2f} "
    f"Max={max(dyns):.2f} "
    f"Avg={sum(dyns)/len(dyns):.2f}"
)

# ----------------------------------------------------------
# RULE TESTS
# ----------------------------------------------------------

tests = []

tests.append((
    "B5 == 0x00",
    lambda f:
        f["b5"] == 0x00
))

tests.append((
    "B4 >= 0x95",
    lambda f:
        f["b4"] >= 0x95
))

tests.append((
    "0x95 <= B4 <= 0x96",
    lambda f:
        0x95 <= f["b4"] <= 0x96
))

tests.append((
    "0x2D <= B7 <= 0x2F",
    lambda f:
        0x2D <= f["b7"] <= 0x2F
))

tests.append((
    "STAT >= 145",
    lambda f:
        f["stat"] >= 145
))

tests.append((
    "STAT >= 150",
    lambda f:
        f["stat"] >= 150
))

tests.append((
    "DYN >= 80",
    lambda f:
        f["dyn"] >= 80
))

tests.append((
    "RPM >= 2500",
    lambda f:
        f["rpm"] >= 2500
))

tests.append((
    "RPM >= 2700",
    lambda f:
        f["rpm"] >= 2700
))

tests.append((
    "B5==0 && B4>=0x95",
    lambda f:
        (
            f["b5"] == 0
            and
            f["b4"] >= 0x95
        )
))

tests.append((
    "B5==0 && B4>=0x95 && B7 2D-2F",
    lambda f:
        (
            f["b5"] == 0
            and
            f["b4"] >= 0x95
            and
            0x2D <= f["b7"] <= 0x2F
        )
))

tests.append((
    "B5==0 && STAT>=145",
    lambda f:
        (
            f["b5"] == 0
            and
            f["stat"] >= 145
        )
))

# ----------------------------------------------------------

print()
print("=" * 80)
print("RULE COVERAGE")
print("=" * 80)

for name, fn in tests:

    hits = sum(
        1
        for f in frames
        if fn(f)
    )

    pct = (
        100.0 *
        hits /
        len(frames)
    )

    print(
        f"{pct:7.3f}%  "
        f"{hits:4d}/{len(frames)}  "
        f"{name}"
    )

print()
print("Done.")