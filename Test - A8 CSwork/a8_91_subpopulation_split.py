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

print()
print("=" * 80)
print("A8 0x91 SUBPOPULATION SPLIT")
print("=" * 80)

# ----------------------------------------------------------

group_A = []
group_B = []

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

        if residual != 0x91:
            continue

        rpm, stat, dyn = decode_signals(frame)

        entry = {
            "idx": idx,
            "frame": frame,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "actual": actual,
            "predicted": predicted
        }

        if actual == 0x8F and predicted == 0x1E:
            group_A.append(entry)

        elif actual == 0xCF and predicted == 0x5E:
            group_B.append(entry)

# ----------------------------------------------------------

print()
print(f"Group A (8F/1E) : {len(group_A)}")
print(f"Group B (CF/5E) : {len(group_B)}")

# ----------------------------------------------------------

def summarize(name, frames):

    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    if not frames:
        print("No frames")
        return

    rpms  = [f["rpm"] for f in frames]
    stats = [f["stat"] for f in frames]
    dyns  = [f["dyn"] for f in frames]

    print()
    print(
        f"RPM  min={min(rpms):8.2f} "
        f"max={max(rpms):8.2f} "
        f"avg={sum(rpms)/len(rpms):8.2f}"
    )

    print(
        f"STAT min={min(stats):8.2f} "
        f"max={max(stats):8.2f} "
        f"avg={sum(stats)/len(stats):8.2f}"
    )

    print(
        f"DYN  min={min(dyns):8.2f} "
        f"max={max(dyns):8.2f} "
        f"avg={sum(dyns)/len(dyns):8.2f}"
    )

    # ------------------------------------------------------

    for byte_index in [4,5,6,7]:

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

summarize(
    "GROUP A (ACT=0x8F PRED=0x1E)",
    group_A
)

summarize(
    "GROUP B (ACT=0xCF PRED=0x5E)",
    group_B
)

# ----------------------------------------------------------
# Compare simple rules
# ----------------------------------------------------------

print()
print("=" * 80)
print("GROUP B RULE CHECKS")
print("=" * 80)

tests = [

    (
        "RPM < 2500",
        lambda f: f["rpm"] < 2500
    ),

    (
        "RPM < 2200",
        lambda f: f["rpm"] < 2200
    ),

    (
        "STAT < 128",
        lambda f: f["stat"] < 128
    ),

    (
        "STAT < 120",
        lambda f: f["stat"] < 120
    ),

    (
        "DYN < 40",
        lambda f: f["dyn"] < 40
    ),

    (
        "DYN < 20",
        lambda f: f["dyn"] < 20
    ),

    (
        "B4 == 0x80",
        lambda f: f["frame"][4] == 0x80
    ),

    (
        "B7 == 0x15",
        lambda f: f["frame"][7] == 0x15
    ),

    (
        "B5 == 0x00",
        lambda f: f["frame"][5] == 0x00
    )

]

for name, fn in tests:

    hits = sum(
        1
        for f in group_B
        if fn(f)
    )

    pct = (
        100.0 *
        hits /
        len(group_B)
    )

    print(
        f"{pct:7.2f}% "
        f"{hits:3d}/{len(group_B)} "
        f"{name}"
    )

# ----------------------------------------------------------

print()
print("=" * 80)
print("FIRST 30 GROUP B FRAMES")
print("=" * 80)

for f in group_B[:30]:

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