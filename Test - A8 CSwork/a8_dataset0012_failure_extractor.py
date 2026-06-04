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
    "a8_full_0012.csv"
)

# ------------------------------------------------------------
# Import your V13 predictor
# ------------------------------------------------------------

from a8_final_validator_v13_gold import (
    actual_residual,
    predicted_residual,
    decode_signals
)

# ------------------------------------------------------------

TARGET_XORS = {
    0xDE,
    0x5A,
    0x91,
    0xDB,
    0xCE,
    0x10,
}

# ------------------------------------------------------------

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

        xor_val = actual ^ predicted

        if xor_val in TARGET_XORS:

            rpm, stat, dyn = decode_signals(frame)

            families[xor_val].append(
                (
                    frame,
                    actual,
                    predicted,
                    rpm,
                    stat,
                    dyn
                )
            )

# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 DATASET0012 FAILURE EXTRACTOR")
print("=" * 80)

for xor_val in sorted(families):

    rows = families[xor_val]

    print()
    print("=" * 60)
    print(f"XOR 0x{xor_val:02X}")
    print("=" * 60)
    print()

    print(
        f"Frames : {len(rows):,}"
    )

    rpms = [r[3] for r in rows]
    stats = [r[4] for r in rows]
    dyns = [r[5] for r in rows]

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

    for idx, label in (
        (4, "Byte4"),
        (5, "Byte5"),
        (6, "Byte6"),
        (7, "Byte7"),
    ):

        counter = Counter(
            r[0][idx]
            for r in rows
        )

        print(label)
        print("-" * 20)

        for value, count in counter.most_common(20):

            pct = (
                count /
                len(rows)
            ) * 100.0

            print(
                f"0x{value:02X}"
                f" {count:5d}"
                f" ({pct:5.1f}%)"
            )

        print()

    print("Example Frames")
    print("-" * 20)

    for (
        frame,
        actual,
        predicted,
        rpm,
        stat,
        dyn
    ) in rows[:30]:

        print(
            f"RPM={rpm:7.1f} "
            f"STAT={stat:3d} "
            f"DYN={dyn:4d} "
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            + " ".join(
                f"{b:02X}"
                for b in frame
            )
        )

print()
print("Done.")