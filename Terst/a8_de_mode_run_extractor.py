#!/usr/bin/env python3

import pandas as pd
from collections import defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 DE MODE RUN EXTRACTOR")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# MODE CLASSIFIER
# ============================================================

def classify_mode(b7, b4):

    if b7 == 0x40 and b4 in (0x80,0x81,0x82):
        return "MODE_80"

    if b7 == 0x3F and b4 in (0x91,0x92,0x93,0x94):
        return "MODE_92"

    if b4 in (0xA0,0xA1,0xA2,0xA3):
        return "MODE_A"

    if b4 == 0x83:
        return "MODE_83"

    return "MODE_OTHER"

# ============================================================
# BUILD MODE STREAM
# ============================================================

stream = []

for idx,row in enumerate(df.itertuples()):

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

    residual = (
        actual_residual(frame)
        ^
        predicted_residual(frame)
    )

    if residual != 0xDE:
        continue

    b4 = frame[4]
    b7 = frame[7]

    mode = classify_mode(b7,b4)

    stream.append(
        (
            idx,
            mode,
            b7,
            b4
        )
    )

# ============================================================
# EXTRACT RUNS
# ============================================================

runs = defaultdict(list)

if stream:

    start_idx = stream[0][0]
    current_mode = stream[0][1]

    prev_idx = stream[0][0]

    for idx,mode,b7,b4 in stream[1:]:

        contiguous = (idx - prev_idx) <= 5

        if mode != current_mode or not contiguous:

            runs[current_mode].append(
                (
                    start_idx,
                    prev_idx,
                    prev_idx - start_idx + 1
                )
            )

            start_idx = idx
            current_mode = mode

        prev_idx = idx

    runs[current_mode].append(
        (
            start_idx,
            prev_idx,
            prev_idx - start_idx + 1
        )
    )

# ============================================================
# SUMMARY
# ============================================================

print()
print("=" * 80)
print("MODE RUN SUMMARY")
print("=" * 80)

for mode in sorted(runs.keys()):

    lengths = [r[2] for r in runs[mode]]

    print()
    print(mode)
    print("-" * 40)

    print(f"Runs      : {len(lengths)}")
    print(f"Total     : {sum(lengths)}")

    print(
        f"Min Len   : {min(lengths)}"
    )

    print(
        f"Max Len   : {max(lengths)}"
    )

    print(
        f"Avg Len   : "
        f"{sum(lengths)/len(lengths):.2f}"
    )

# ============================================================
# TOP RUNS
# ============================================================

print()
print("=" * 80)
print("TOP RUNS")
print("=" * 80)

all_runs = []

for mode in runs:

    for start,end,length in runs[mode]:

        all_runs.append(
            (
                length,
                mode,
                start,
                end
            )
        )

all_runs.sort(reverse=True)

for length,mode,start,end in all_runs[:25]:

    print(
        f"{mode:<12}"
        f"LEN={length:4d} "
        f"IDX={start:6d}->{end:6d}"
    )

# ============================================================
# FULL RUN LIST
# ============================================================

print()
print("=" * 80)
print("RUN DETAILS")
print("=" * 80)

for mode in sorted(runs.keys()):

    print()
    print(mode)

    for start,end,length in runs[mode]:

        print(
            f"  LEN={length:4d} "
            f"IDX={start:6d}->{end:6d}"
        )

print()
print("Done.")