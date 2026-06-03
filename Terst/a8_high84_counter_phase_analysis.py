#!/usr/bin/env python3

import pandas as pd
from collections import Counter

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual,
    decode_signals
)

print()
print("=" * 80)
print("A8 HIGH84 COUNTER PHASE ANALYSIS")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

frames = []

# ============================================================
# DECODE
# ============================================================

for idx, row in enumerate(df.itertuples()):

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

    counter = frame[1] & 0x0F

    frames.append({

        "idx": idx,

        "residual": residual,

        "counter": counter,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn,

        "b4": frame[4],
        "b6": frame[6],
        "b7": frame[7]

    })

# ============================================================
# HIGH84 STABLE
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
print(f"Stable Runs : {len(runs)}")

# ============================================================
# COUNTER ANALYSIS
# ============================================================

entry_counter = Counter()
exit_counter = Counter()

print()
print("=" * 80)
print("RUN DETAILS")
print("=" * 80)

for run_num, (start,end) in enumerate(runs):

    print()
    print(f"RUN #{run_num+1}")

    start_counter = frames[start]["counter"]
    end_counter   = frames[end]["counter"]

    entry_counter[start_counter] += 1
    exit_counter[end_counter] += 1

    print(
        f"Start IDX      : {start}"
    )

    print(
        f"End IDX        : {end}"
    )

    print(
        f"Length         : {end-start+1}"
    )

    print(
        f"Entry Counter  : {start_counter}"
    )

    print(
        f"Exit Counter   : {end_counter}"
    )

    print()
    print("Counter Sequence")

    seq = []

    for idx in range(start,end+1):

        seq.append(
            frames[idx]["counter"]
        )

    print(seq)

# ============================================================
# ENTRY HISTOGRAM
# ============================================================

print()
print("=" * 80)
print("ENTRY COUNTER HISTOGRAM")
print("=" * 80)

for value,count in sorted(entry_counter.items()):

    print(
        f"{value:2d} : {count}"
    )

# ============================================================
# EXIT HISTOGRAM
# ============================================================

print()
print("=" * 80)
print("EXIT COUNTER HISTOGRAM")
print("=" * 80)

for value,count in sorted(exit_counter.items()):

    print(
        f"{value:2d} : {count}"
    )

# ============================================================
# GLOBAL HIGH84 COUNTERS
# ============================================================

counter_hist = Counter()

for f in frames:

    if is_high84_stable(f):

        counter_hist[f["counter"]] += 1

print()
print("=" * 80)
print("HIGH84 COUNTER DISTRIBUTION")
print("=" * 80)

for value,count in sorted(counter_hist.items()):

    print(
        f"{value:2d} : {count}"
    )

print()
print("Done.")