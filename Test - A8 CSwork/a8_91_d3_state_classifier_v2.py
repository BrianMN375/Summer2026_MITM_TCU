#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    load_frame_from_row,
    actual_residual,
    predicted_residual,
    decode_signals
)

TARGETS = {
    0x91,
    0xD3
}

# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 0x91 / 0xD3 STATE CLASSIFIER V2")
print("=" * 80)

# ----------------------------------------------------------

frames = []

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

        rpm, stat, dyn = decode_signals(frame)

        frames.append({

            "idx": idx,

            "frame": frame,

            "actual": actual,
            "predicted": predicted,
            "residual": residual,

            "rpm": rpm,
            "stat": stat,
            "dyn": dyn
        })

# ----------------------------------------------------------

target_frames = [

    f for f in frames

    if f["residual"] in TARGETS
]

print()
print(f"Frames Loaded : {len(frames):,}")
print(f"Target Frames : {len(target_frames):,}")

# ----------------------------------------------------------
# Residual populations
# ----------------------------------------------------------

print()
print("=" * 80)
print("RESIDUAL POPULATION")
print("=" * 80)

for residual in sorted(TARGETS):

    count = sum(
        1 for f in target_frames
        if f["residual"] == residual
    )

    print(
        f"0x{residual:02X} : {count}"
    )

# ----------------------------------------------------------
# State signatures
# ----------------------------------------------------------

print()
print("=" * 80)
print("TOP STATE SIGNATURES")
print("=" * 80)

signature_counts = Counter()

for f in target_frames:

    b = f["frame"]

    signature = (
        b[4],
        b[5],
        b[7]
    )

    signature_counts[signature] += 1

for (b4, b5, b7), count in signature_counts.most_common(50):

    print(
        f"{count:6d}  "
        f"B4=0x{b4:02X} "
        f"B5=0x{b5:02X} "
        f"B7=0x{b7:02X}"
    )

# ----------------------------------------------------------
# Family statistics
# ----------------------------------------------------------

print()
print("=" * 80)
print("FAMILY STATISTICS")
print("=" * 80)

for residual in sorted(TARGETS):

    fam = [

        f for f in target_frames

        if f["residual"] == residual
    ]

    if not fam:
        continue

    rpms = [x["rpm"] for x in fam]
    stats = [x["stat"] for x in fam]
    dyns = [x["dyn"] for x in fam]

    print()
    print(f"Residual 0x{residual:02X}")
    print("-" * 40)

    print(
        f"Frames : {len(fam):,}"
    )

    print(
        f"RPM    min={min(rpms):8.2f} "
        f"max={max(rpms):8.2f} "
        f"avg={sum(rpms)/len(rpms):8.2f}"
    )

    print(
        f"STAT   min={min(stats):8.2f} "
        f"max={max(stats):8.2f} "
        f"avg={sum(stats)/len(stats):8.2f}"
    )

    print(
        f"DYN    min={min(dyns):8.2f} "
        f"max={max(dyns):8.2f} "
        f"avg={sum(dyns)/len(dyns):8.2f}"
    )

# ----------------------------------------------------------
# Transitions
# ----------------------------------------------------------

print()
print("=" * 80)
print("TRANSITIONS INTO TARGET STATES")
print("=" * 80)

transition_counts = Counter()

for i in range(1, len(frames)):

    prev = frames[i - 1]["residual"]
    curr = frames[i]["residual"]

    if curr in TARGETS and prev != curr:

        transition_counts[
            (prev, curr)
        ] += 1

for (src, dst), count in (

    transition_counts.most_common()

):

    print(
        f"0x{src:02X} -> "
        f"0x{dst:02X} "
        f"({count})"
    )

# ----------------------------------------------------------
# Entry examples
# ----------------------------------------------------------

print()
print("=" * 80)
print("ENTRY EXAMPLES")
print("=" * 80)

shown = 0

for i in range(1, len(frames)):

    prev = frames[i - 1]
    curr = frames[i]

    if (

        curr["residual"] in TARGETS

        and

        prev["residual"] != curr["residual"]

    ):

        b = curr["frame"]

        print()

        print(
            f"IDX={curr['idx']}"
        )

        print(
            f"0x{prev['residual']:02X} -> "
            f"0x{curr['residual']:02X}"
        )

        print(
            f"ACT=0x{curr['actual']:02X} "
            f"PRED=0x{curr['predicted']:02X}"
        )

        print(
            f"RPM={curr['rpm']:.2f} "
            f"STAT={curr['stat']} "
            f"DYN={curr['dyn']}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in b
            )
        )

        shown += 1

        if shown >= 40:
            break

# ----------------------------------------------------------
# Dominant B5 values
# ----------------------------------------------------------

print()
print("=" * 80)
print("B5 DISTRIBUTION")
print("=" * 80)

for residual in sorted(TARGETS):

    fam = [

        f for f in target_frames

        if f["residual"] == residual
    ]

    hist = Counter()

    for f in fam:

        hist[
            f["frame"][5]
        ] += 1

    print()
    print(
        f"Residual 0x{residual:02X}"
    )

    for value, count in hist.most_common():

        print(
            f"0x{value:02X} : {count}"
        )

print()
print("Done.")