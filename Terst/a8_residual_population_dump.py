#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0015_a8only.csv"
)


# ---------------------------------------------------------
# IMPORT YOUR VALIDATOR FUNCTIONS
# ---------------------------------------------------------

from a8_final_validator_v13 import (
    actual_residual,
    predicted_residual
)

# ---------------------------------------------------------

def describe(name, values):

    if not values:
        return

    print(f"{name:<6} "
          f"min={min(values):8.2f} "
          f"max={max(values):8.2f} "
          f"avg={sum(values)/len(values):8.2f}")

# ---------------------------------------------------------

families = defaultdict(list)

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    for row in reader:

        try:

            frame = [
                int(x, 16)
                for x in row[:8]
            ]

        except:
            continue

        predicted = predicted_residual(frame)
        actual    = actual_residual(frame)

        residual = predicted ^ actual

        b4 = frame[4]
        b5 = frame[5]
        b6 = frame[6]
        b7 = frame[7]

        rpm = (
            ((frame[2] << 8) | frame[3])
            / 4.0
        )

        stat = (
            (b4 << 1)
            |
            (b6 >> 7)
        )

        dyn = (
            b6 & 0x7F
        )

        counter = (
            frame[1] & 0x0F
        )

        families[residual].append({
            "b4": b4,
            "b5": b5,
            "b6": b6,
            "b7": b7,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "counter": counter
        })

# ---------------------------------------------------------

print()
print("=" * 80)
print("A8 RESIDUAL POPULATION DUMP")
print("=" * 80)

print()

print("RESIDUAL INVENTORY")
print("-" * 80)

for residual, rows in sorted(
        families.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    print(
        f"0x{residual:02X} "
        f"{len(rows):6d} "
        f"({100.0 * len(rows) / sum(len(v) for v in families.values()):6.2f}%)"
    )

# ---------------------------------------------------------

TOP_FAMILIES = 10

for residual, rows in sorted(
        families.items(),
        key=lambda x: len(x[1]),
        reverse=True)[:TOP_FAMILIES]:

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

    describe("rpm", rpms)
    describe("stat", stats)
    describe("dyn", dyns)

    describe("b4", b4s)
    describe("b5", b5s)
    describe("b6", b6s)
    describe("b7", b7s)

    print()

    print("B4 Histogram")
    for value, count in Counter(b4s).most_common(10):
        print(f"0x{value:02X} : {count}")

    print()

    print("B5 Histogram")
    for value, count in Counter(b5s).most_common(10):
        print(f"0x{value:02X} : {count}")

    print()

    print("B7 Histogram")
    for value, count in Counter(b7s).most_common(10):
        print(f"0x{value:02X} : {count}")

    print()

    print("Counter Histogram")
    for value, count in sorted(
            Counter(counters).items()):

        print(f"{value:2d} : {count}")

print()
print("Done.")