#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual
)

# ----------------------------------------------------------
# Residuals we care about
# ----------------------------------------------------------

TARGET_RESIDUALS = {
    0x91,
    0xD3,
    0x7A,
    0xCF,
    0x5A,
    0x10
}

# ----------------------------------------------------------

families = defaultdict(list)

# ----------------------------------------------------------

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    for row in reader:

        frame = load_frame_from_row(row)

        if frame is None:
            continue

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        residual = actual ^ predicted

        if residual not in TARGET_RESIDUALS:
            continue

        b0,b1,b2,b3,b4,b5,b6,b7 = frame

        counter = b1 & 0x0F

        rpm = ((b2 << 8) | b3) / 4.0

        stat = (b4 << 1) | (b6 >> 7)

        dyn = b6 & 0x7F

        families[residual].append(
            {
                "counter": counter,
                "rpm": rpm,
                "stat": stat,
                "dyn": dyn,
                "b4": b4,
                "b5": b5,
                "b6": b6,
                "b7": b7
            }
        )

# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 RESIDUAL FAMILY PROFILER V2")
print("=" * 80)

# ----------------------------------------------------------
# Inventory
# ----------------------------------------------------------

print()
print("RESIDUAL INVENTORY")
print("-" * 80)

total = sum(len(v) for v in families.values())

for residual, rows in sorted(
        families.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    pct = 100.0 * len(rows) / total

    print(
        f"0x{residual:02X} "
        f"{len(rows):6d} "
        f"({pct:6.2f}%)"
    )

# ----------------------------------------------------------

for residual, rows in sorted(
        families.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{residual:02X}")
    print("=" * 80)

    rpms = [x["rpm"] for x in rows]
    stats = [x["stat"] for x in rows]
    dyns = [x["dyn"] for x in rows]

    b4s = [x["b4"] for x in rows]
    b5s = [x["b5"] for x in rows]
    b6s = [x["b6"] for x in rows]
    b7s = [x["b7"] for x in rows]

    counters = [x["counter"] for x in rows]

    print()
    print(f"Frames : {len(rows)}")
    print()

    print(
        f"rpm    min={min(rpms):8.2f} "
        f"max={max(rpms):8.2f} "
        f"avg={sum(rpms)/len(rpms):8.2f}"
    )

    print(
        f"stat   min={min(stats):8.2f} "
        f"max={max(stats):8.2f} "
        f"avg={sum(stats)/len(stats):8.2f}"
    )

    print(
        f"dyn    min={min(dyns):8.2f} "
        f"max={max(dyns):8.2f} "
        f"avg={sum(dyns)/len(dyns):8.2f}"
    )

    print()

    # ------------------------------------------------------
    # Histograms
    # ------------------------------------------------------

    for label, values in [

        ("B4", b4s),
        ("B5", b5s),
        ("B6", b6s),
        ("B7", b7s)

    ]:

        print()
        print(label)
        print("-" * 40)

        for value, count in Counter(values).most_common(20):

            print(
                f"0x{value:02X} : {count}"
            )

    print()
    print("COUNTER")
    print("-" * 40)

    for value, count in sorted(
            Counter(counters).items()):

        print(
            f"{value:2d} : {count}"
        )

    # ------------------------------------------------------
    # Top signatures
    # ------------------------------------------------------

    signatures = Counter()

    for r in rows:

        signatures[
            (
                r["b4"],
                r["b5"],
                r["b7"]
            )
        ] += 1

    print()
    print("TOP SIGNATURES")
    print("-" * 40)

    for (
            b4,
            b5,
            b7
        ), count in signatures.most_common(20):

        print(
            f"B4=0x{b4:02X} "
            f"B5=0x{b5:02X} "
            f"B7=0x{b7:02X} "
            f"-> {count}"
        )

print()
print("Done.")