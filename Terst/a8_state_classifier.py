#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_full_0012.csv"
)

from a8_final_validator_v13_gold import (
    actual_residual,
    predicted_residual,
    decode_signals
)

# ============================================================
# LOAD
# ============================================================

families = defaultdict(list)

print()
print("Loading dataset...")

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for row in reader:

        frame = [
            int(x, 16)
            for x in row[1:9]
        ]

        actual = actual_residual(frame)
        predicted = predicted_residual(frame)

        if actual == predicted:
            continue

        rpm, stat, dyn = decode_signals(frame)

        key = (predicted, actual)

        families[key].append(
            (
                frame,
                rpm,
                stat,
                dyn
            )
        )

# ============================================================
# REPORT
# ============================================================

print()
print("=" * 80)
print("A8 STATE CLASSIFIER")
print("=" * 80)

for (pred, act), rows in sorted(
        families.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    print()
    print("=" * 70)
    print(
        f"PRED=0x{pred:02X}  "
        f"ACT=0x{act:02X}"
    )
    print("=" * 70)

    print()
    print(
        f"Frames : {len(rows):,}"
    )

    rpms  = [r[1] for r in rows]
    stats = [r[2] for r in rows]
    dyns  = [r[3] for r in rows]

    print(
        f"RPM  : {min(rpms):.1f}"
        f"..{max(rpms):.1f}"
        f" avg={sum(rpms)/len(rpms):.1f}"
    )

    print(
        f"STAT : {min(stats)}"
        f"..{max(stats)}"
        f" avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN  : {min(dyns)}"
        f"..{max(dyns)}"
        f" avg={sum(dyns)/len(dyns):.1f}"
    )

    print()

    for idx, name in (
        (4, "Byte4"),
        (5, "Byte5"),
        (6, "Byte6"),
        (7, "Byte7"),
    ):

        c = Counter(
            r[0][idx]
            for r in rows
        )

        print(name)
        print("-" * 20)

        for val, count in c.most_common(15):

            pct = (
                count /
                len(rows)
            ) * 100.0

            print(
                f"0x{val:02X}"
                f" {count:5d}"
                f" ({pct:5.1f}%)"
            )

        print()

    print("Examples")
    print("-" * 20)

    for frame, rpm, stat, dyn in rows[:20]:

        print(
            f"RPM={rpm:7.1f} "
            f"STAT={stat:3d} "
            f"DYN={dyn:4d} "
            + " ".join(
                f"{b:02X}"
                for b in frame
            )
        )

print()
print("Done.")