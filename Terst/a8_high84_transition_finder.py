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
print("A8 HIGH84 TRANSITION FINDER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

frames = []

# ============================================================
# PRE-DECODE EVERYTHING
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

        "frame": frame,

        "residual": residual,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn,

        "b4": frame[4],
        "b6": frame[6],
        "b7": frame[7]

    })

# ============================================================
# FIND HIGH84 EVENTS
# ============================================================

events = []

for i, f in enumerate(frames):

    if f["residual"] != 0x84:
        continue

    #
    # HIGH84 definition
    #

    if f["stat"] <= 284:
        continue

    events.append(i)

print()
print(f"HIGH84 Events : {len(events)}")

# ============================================================
# DUMP CONTEXT
# ============================================================

for event_num, idx in enumerate(events):

    print()
    print("=" * 80)
    print(f"HIGH84 EVENT #{event_num+1}")
    print("=" * 80)

    start = max(0, idx - 3)
    end   = min(len(frames), idx + 4)

    for j in range(start, end):

        f = frames[j]

        marker = ">>> " if j == idx else "    "

        print(
            marker
            + f"IDX={j:6d} "
            + f"RES=0x{f['residual']:02X} "
            + f"B4=0x{f['b4']:02X} "
            + f"B7=0x{f['b7']:02X} "
            + f"RPM={f['rpm']:7.1f} "
            + f"STAT={f['stat']:3d} "
            + f"DYN={f['dyn']:3d}"
        )

print()
print("Done.")