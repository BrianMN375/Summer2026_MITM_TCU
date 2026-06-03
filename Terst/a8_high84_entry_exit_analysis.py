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
print("A8 HIGH84 ENTRY/EXIT ANALYSIS")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

frames = []

# ============================================================
# DECODE
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
# HIGH84_STABLE FLAG
# ============================================================

def is_high84_stable(f):

    return (

        f["residual"] == 0x84

        and

        329 <= f["stat"] <= 337

        and

        0x87 <= f["b4"] <= 0x8F

        and

        0x4F <= f["b7"] <= 0x50

    )

# ============================================================
# FIND RUNS
# ============================================================

runs = []

i = 0

while i < len(frames):

    if not is_high84_stable(frames[i]):
        i += 1
        continue

    start = i

    while i < len(frames) and is_high84_stable(frames[i]):
        i += 1

    end = i - 1

    runs.append((start,end))

print()
print(f"Stable HIGH84 Runs : {len(runs)}")

# ============================================================
# ANALYZE EACH RUN
# ============================================================

for run_num,(start,end) in enumerate(runs):

    print()
    print("=" * 80)
    print(f"RUN #{run_num+1}")
    print("=" * 80)

    print(f"Start IDX : {start}")
    print(f"End IDX   : {end}")
    print(f"Length    : {end-start+1}")

    #
    # entry frame
    #

    entry_idx = start - 1
    exit_idx  = end + 1

    print()
    print("ENTRY")

    for idx in range(max(0,start-3), start+1):

        f = frames[idx]

        marker = ">>> " if idx == start else "    "

        print(

            marker
            + f"IDX={idx:7d} "
            + f"RES=0x{f['residual']:02X} "
            + f"B4=0x{f['b4']:02X} "
            + f"B7=0x{f['b7']:02X} "
            + f"RPM={f['rpm']:7.1f} "
            + f"STAT={f['stat']:3d} "
            + f"DYN={f['dyn']:3d}"

        )

    print()
    print("EXIT")

    for idx in range(end, min(len(frames), end+4)):

        f = frames[idx]

        marker = "<<< " if idx == end else "    "

        print(

            marker
            + f"IDX={idx:7d} "
            + f"RES=0x{f['residual']:02X} "
            + f"B4=0x{f['b4']:02X} "
            + f"B7=0x{f['b7']:02X} "
            + f"RPM={f['rpm']:7.1f} "
            + f"STAT={f['stat']:3d} "
            + f"DYN={f['dyn']:3d}"

        )

# ============================================================
# TRANSITION SUMMARY
# ============================================================

print()
print("=" * 80)
print("TRANSITION SUMMARY")
print("=" * 80)

for start,end in runs:

    before = frames[start-1]["residual"] if start > 0 else None
    after  = frames[end+1]["residual"] if end+1 < len(frames) else None

    print(
        f"0x{before:02X} -> HIGH84 -> 0x{after:02X}"
    )

print()
print("Done.")