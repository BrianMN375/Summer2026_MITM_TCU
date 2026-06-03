#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

WINDOW = 50

print()
print("=" * 80)
print("A8 MODE TRANSITION CONTEXT DUMP")
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

mode_stream = []

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
    b6 = frame[6]
    b7 = frame[7]

    counter = frame[1] & 0x0F

    rpm = ((frame[2] << 8) | frame[3]) / 4.0
    stat = (frame[4] << 1) + (frame[5] >> 7)
    dyn = frame[6] >> 1

    mode_stream.append(
        {
            "idx": idx,
            "mode": classify_mode(b7,b4),
            "b4": b4,
            "b6": b6,
            "b7": b7,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "counter": counter
        }
    )

# ============================================================
# FIND MODE CHANGES
# ============================================================

transitions = []

for i in range(1, len(mode_stream)):

    prev = mode_stream[i-1]
    curr = mode_stream[i]

    if prev["mode"] != curr["mode"]:

        transitions.append(
            (
                i,
                prev["mode"],
                curr["mode"]
            )
        )

print()
print(f"Transitions Found : {len(transitions)}")

# ============================================================
# DUMP CONTEXT
# ============================================================

for trans_idx, from_mode, to_mode in transitions:

    print()
    print("=" * 80)
    print(f"{from_mode} -> {to_mode}")
    print("=" * 80)

    start = max(0, trans_idx - WINDOW)
    end   = min(len(mode_stream), trans_idx + WINDOW)

    for i in range(start, end):

        row = mode_stream[i]

        marker = ">>>"

        if i < trans_idx:
            marker = "   "

        print(

            f"{marker} "
            f"IDX={row['idx']:7d} "
            f"{row['mode']:<10} "
            f"B4=0x{row['b4']:02X} "
            f"B6=0x{row['b6']:02X} "
            f"B7=0x{row['b7']:02X} "
            f"RPM={row['rpm']:8.1f} "
            f"STAT={row['stat']:4d} "
            f"DYN={row['dyn']:4d} "
            f"CNT={row['counter']:2d}"

        )

print()
print("Done.")