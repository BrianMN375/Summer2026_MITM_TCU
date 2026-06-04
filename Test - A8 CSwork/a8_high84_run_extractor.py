#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 HIGH84 RUN EXTRACTOR")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

frames = []

# ============================================================
# PRE-DECODE
# ============================================================

for row in df.itertuples():

    frame = [

        int(row.b0,16),
        int(row.b1,16),
        int(row.b2,16),
        int(row.b3,16),
        int(row.b4,16),
        int(row.b5,16),
        int(row.b6,16),
        int(row.b7,16)

    ]

    actual = actual_residual(frame)
    predicted = predicted_residual(frame)

    residual = actual ^ predicted

    rpm, stat, dyn = decode_signals(frame)

    frames.append({

        "residual": residual,
        "rpm": rpm,
        "stat": stat,
        "dyn": dyn,

        "b4": frame[4],
        "b6": frame[6],
        "b7": frame[7]

    })

# ============================================================
# HIGH84 FLAG
# ============================================================

flags = []

for f in frames:

    high84 = (

        f["residual"] == 0x84
        and
        f["stat"] > 284

    )

    flags.append(high84)

# ============================================================
# EXTRACT RUNS
# ============================================================

runs = []

i = 0

while i < len(flags):

    if not flags[i]:
        i += 1
        continue

    start = i

    while i < len(flags) and flags[i]:
        i += 1

    end = i - 1

    runs.append((start,end))

# ============================================================
# REPORT
# ============================================================

print()
print(f"HIGH84 Runs : {len(runs)}")

for run_num,(start,end) in enumerate(runs):

    length = end - start + 1

    first = frames[start]
    last  = frames[end]

    print()
    print("=" * 80)
    print(f"RUN #{run_num+1}")
    print("=" * 80)

    print(f"Start IDX : {start}")
    print(f"End IDX   : {end}")
    print(f"Length    : {length}")

    print()

    print(
        f"RPM  : {first['rpm']:7.1f}"
        f" -> {last['rpm']:7.1f}"
    )

    print(
        f"STAT : {first['stat']:3d}"
        f" -> {last['stat']:3d}"
    )

    print(
        f"B4   : 0x{first['b4']:02X}"
        f" -> 0x{last['b4']:02X}"
    )

    print(
        f"B6   : 0x{first['b6']:02X}"
        f" -> 0x{last['b6']:02X}"
    )

    print(
        f"B7   : 0x{first['b7']:02X}"
        f" -> 0x{last['b7']:02X}"
    )

    print(
        f"DYN  : {first['dyn']:3d}"
        f" -> {last['dyn']:3d}"
    )

    #
    # preceding context
    #

    before = max(0,start-2)
    after  = min(len(frames)-1,end+2)

    print()
    print("Context")

    for idx in range(before, after+1):

        f = frames[idx]

        marker = "###" if start <= idx <= end else "   "

        print(
            f"{marker} "
            f"{idx:7d} "
            f"RES=0x{f['residual']:02X} "
            f"B4=0x{f['b4']:02X} "
            f"B7=0x{f['b7']:02X} "
            f"RPM={f['rpm']:7.1f} "
            f"STAT={f['stat']:3d} "
            f"DYN={f['dyn']:3d}"
        )

print()
print("Done.")