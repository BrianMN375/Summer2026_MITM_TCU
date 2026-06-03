#!/usr/bin/env python3

from collections import Counter
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
print("A8 D3 OVERLAY SOLVER")
print("=" * 80)

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
# Basic statistics
# ----------------------------------------------------------

for field in [

    "b4",
    "b5",
    "b6",
    "b7"

]:

    print()
    print("=" * 80)
    print(field.upper())
    print("=" * 80)

    values = [
        f[field]
        for f in frames
    ]

    print(
        f"Min={min(values)} "
        f"Max={max(values)} "
        f"Avg={sum(values)/len(values):.2f}"
    )

    hist = Counter(values)

    for value, count in hist.most_common(25):

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
# Candidate rules
# ----------------------------------------------------------

tests = []

tests.append((
    "B5 == 0x64",
    lambda f:
        f["b5"] == 0x64
))

tests.append((
    "B5 in (64,63)",
    lambda f:
        f["b5"] in (
            0x64,
            0x63
        )
))

tests.append((
    "B5 in (64,63,62)",
    lambda f:
        f["b5"] in (
            0x64,
            0x63,
            0x62
        )
))

tests.append((
    "98 <= STAT <= 127",
    lambda f:
        98 <= f["stat"] <= 127
))

tests.append((
    "100 <= STAT <= 127",
    lambda f:
        100 <= f["stat"] <= 127
))

tests.append((
    "105 <= STAT <= 127",
    lambda f:
        105 <= f["stat"] <= 127
))

tests.append((
    "0x20 <= B7 <= 0x28",
    lambda f:
        0x20 <= f["b7"] <= 0x28
))

tests.append((
    "0x20 <= B7 <= 0x26",
    lambda f:
        0x20 <= f["b7"] <= 0x26
))

tests.append((
    "RPM >= 2048",
    lambda f:
        f["rpm"] >= 2048
))

tests.append((
    "RPM >= 2200",
    lambda f:
        f["rpm"] >= 2200
))

tests.append((
    "DYN <= 40",
    lambda f:
        f["dyn"] <= 40
))

tests.append((
    "DYN <= 20",
    lambda f:
        f["dyn"] <= 20
))

tests.append((
    "DYN <= 10",
    lambda f:
        f["dyn"] <= 10
))

tests.append((
    "B5=(64|63) && B7=20-28",
    lambda f:
        (
            f["b5"] in (0x64,0x63)
            and
            0x20 <= f["b7"] <= 0x28
        )
))

tests.append((
    "B5=(64|63) && STAT=98-127",
    lambda f:
        (
            f["b5"] in (0x64,0x63)
            and
            98 <= f["stat"] <= 127
        )
))

tests.append((
    "B5=(64|63) && DYN<=40",
    lambda f:
        (
            f["b5"] in (0x64,0x63)
            and
            f["dyn"] <= 40
        )
))

tests.append((
    "B5=(64|63) && B7=20-28 && STAT=98-127",
    lambda f:
        (
            f["b5"] in (0x64,0x63)
            and
            0x20 <= f["b7"] <= 0x28
            and
            98 <= f["stat"] <= 127
        )
))

tests.append((
    "B5=(64|63) && B7=20-28 && DYN<=40",
    lambda f:
        (
            f["b5"] in (0x64,0x63)
            and
            0x20 <= f["b7"] <= 0x28
            and
            f["dyn"] <= 40
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
        f"{pct:7.3f}% "
        f"{hits:4d}/{len(frames)} "
        f"{name}"
    )

# ----------------------------------------------------------
# Pair-specific coverage
# ----------------------------------------------------------

print()
print("=" * 80)
print("PAIR BREAKDOWN")
print("=" * 80)

pairs = Counter()

for f in frames:

    pairs[
        (
            f["actual"],
            f["predicted"]
        )
    ] += 1

for (a,p), count in pairs.most_common():

    print(
        f"ACT=0x{a:02X} "
        f"PRED=0x{p:02X} "
        f"{count}"
    )

print()
print("Done.")