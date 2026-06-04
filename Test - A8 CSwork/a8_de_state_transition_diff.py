#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

# ----------------------------------------------------------
# Helpers
# ----------------------------------------------------------

def decode_rpm(frame):
    return ((frame[2] << 8) | frame[3]) / 4.0

def decode_stat(frame):
    return ((frame[4] << 1) | (frame[6] >> 7))

def decode_dyn(frame):
    return frame[6] & 0x7F

# ----------------------------------------------------------
# State decoder
# ----------------------------------------------------------

def decode_state(b7, b4):

    cell = (b7, b4)

    if b4 == 0x83 and 0x40 <= b7 <= 0x4A:
        return "COUNTDOWN"

    if cell == (0x40,0x80):
        return "STEADY_4080"

    if (
        b7 in (0x3E,0x3F)
        and
        0xA0 <= b4 <= 0xA3
    ):
        return "A_FAMILY"

    if cell in (
        (0x40,0x8D),
        (0x40,0x8F),
        (0x41,0x8F)
    ):
        return "BRIDGE"

    if (
        b7 == 0x3F
        and
        0x91 <= b4 <= 0x94
    ):
        return "NINE_FAMILY"

    if cell in (
        (0x45,0x80),
        (0x42,0x89),
        (0x41,0x86)
    ):
        return "EXIT"

    return "OTHER"

# ----------------------------------------------------------
# Build DE dataset
# ----------------------------------------------------------

rows = []

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    next(reader)

    for idx,row in enumerate(reader):

        if len(row) < 9:
            continue

        try:

            frame = [

                int(row[1],16),
                int(row[2],16),
                int(row[3],16),
                int(row[4],16),
                int(row[5],16),
                int(row[6],16),
                int(row[7],16),
                int(row[8],16)

            ]

        except:
            continue

        residual = (
            actual_residual(frame)
            ^
            predicted_residual(frame)
        )

        if residual != 0xDE:
            continue

        rows.append({

            "idx": idx,

            "frame": frame,

            "b4": frame[4],
            "b6": frame[6],
            "b7": frame[7],

            "rpm": decode_rpm(frame),
            "stat": decode_stat(frame),
            "dyn": decode_dyn(frame),

            "state": decode_state(
                frame[7],
                frame[4]
            )

        })

# ----------------------------------------------------------
# Find transitions
# ----------------------------------------------------------

transitions = []

for prev,cur in zip(rows, rows[1:]):

    if prev["state"] == cur["state"]:
        continue

    transitions.append(
        (prev,cur)
    )

# ----------------------------------------------------------
# Output
# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 DE STATE TRANSITION DIFF")
print("=" * 80)

print()
print("Transitions :", len(transitions))

summary = Counter()

for prev,cur in transitions:

    summary[
        (
            prev["state"],
            cur["state"]
        )
    ] += 1

print()
print("=" * 80)
print("TRANSITION SUMMARY")
print("=" * 80)

for (src,dst),count in summary.items():

    print(
        f"{src:15s}"
        f" -> "
        f"{dst:15s}"
        f" : {count}"
    )

print()
print("=" * 80)
print("TRANSITION DETAILS")
print("=" * 80)

for i,(prev,cur) in enumerate(transitions,1):

    print()
    print("-" * 80)

    print(
        f"TRANSITION #{i}"
    )

    print(
        f"{prev['state']} "
        f"-> "
        f"{cur['state']}"
    )

    print()

    print(
        f"IDX {prev['idx']} -> {cur['idx']}"
    )

    print()

    print(
        f"B4   "
        f"0x{prev['b4']:02X}"
        f" -> "
        f"0x{cur['b4']:02X}"
        f"   Δ={cur['b4']-prev['b4']:+d}"
    )

    print(
        f"B6   "
        f"0x{prev['b6']:02X}"
        f" -> "
        f"0x{cur['b6']:02X}"
        f"   Δ={cur['b6']-prev['b6']:+d}"
    )

    print(
        f"B7   "
        f"0x{prev['b7']:02X}"
        f" -> "
        f"0x{cur['b7']:02X}"
        f"   Δ={cur['b7']-prev['b7']:+d}"
    )

    print()

    print(
        f"RPM  "
        f"{prev['rpm']:.1f}"
        f" -> "
        f"{cur['rpm']:.1f}"
        f"   Δ={cur['rpm']-prev['rpm']:+.1f}"
    )

    print(
        f"STAT "
        f"{prev['stat']}"
        f" -> "
        f"{cur['stat']}"
        f"   Δ={cur['stat']-prev['stat']:+d}"
    )

    print(
        f"DYN  "
        f"{prev['dyn']}"
        f" -> "
        f"{cur['dyn']}"
        f"   Δ={cur['dyn']-prev['dyn']:+d}"
    )

print()
print("Done.")