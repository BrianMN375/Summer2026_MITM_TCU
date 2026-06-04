#!/usr/bin/env python3

import csv
from collections import Counter

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

# ----------------------------------------------------------
# State decoder
# ----------------------------------------------------------

def decode_state(b7, b4):

    cell = (b7, b4)

    # ------------------------------------------------------
    # Countdown family
    # ------------------------------------------------------

    if b4 == 0x83 and 0x40 <= b7 <= 0x4A:
        return "COUNTDOWN"

    # ------------------------------------------------------
    # Primary steady state
    # ------------------------------------------------------

    if cell == (0x40,0x80):
        return "STEADY_4080"

    # ------------------------------------------------------
    # A family
    # ------------------------------------------------------

    if (
        (b7 in (0x3E,0x3F))
        and
        (0xA0 <= b4 <= 0xA3)
    ):
        return "A_FAMILY"

    # ------------------------------------------------------
    # Bridge family
    # ------------------------------------------------------

    if cell in (
        (0x40,0x8D),
        (0x40,0x8F),
        (0x41,0x8F)
    ):
        return "BRIDGE"

    # ------------------------------------------------------
    # 9 family
    # ------------------------------------------------------

    if (
        b7 == 0x3F
        and
        0x91 <= b4 <= 0x94
    ):
        return "NINE_FAMILY"

    # ------------------------------------------------------
    # Exit path
    # ------------------------------------------------------

    if cell in (
        (0x45,0x80),
        (0x42,0x89),
        (0x41,0x86)
    ):
        return "EXIT"

    # ------------------------------------------------------

    return "OTHER"

# ----------------------------------------------------------
# Main
# ----------------------------------------------------------

print()
print("=" * 80)
print("A8 DE STATE MACHINE DECODER")
print("=" * 80)

decoded = []

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

        b4 = frame[4]
        b5 = frame[5]
        b6 = frame[6]
        b7 = frame[7]

        state = decode_state(b7,b4)

        decoded.append({

            "idx": idx,
            "state": state,
            "b4": b4,
            "b5": b5,
            "b6": b6,
            "b7": b7

        })

# ----------------------------------------------------------
# Population
# ----------------------------------------------------------

print()
print("=" * 80)
print("STATE POPULATION")
print("=" * 80)

counts = Counter()

for row in decoded:
    counts[row["state"]] += 1

for state,count in counts.most_common():

    print(
        f"{state:15s} {count:4d}"
    )

# ----------------------------------------------------------
# Runs
# ----------------------------------------------------------

print()
print("=" * 80)
print("STATE RUNS")
print("=" * 80)

runs = []

if decoded:

    start = 0

    current = decoded[0]["state"]

    for i in range(1,len(decoded)):

        if decoded[i]["state"] != current:

            runs.append(

                (
                    current,
                    start,
                    i - 1
                )

            )

            start = i
            current = decoded[i]["state"]

    runs.append(

        (
            current,
            start,
            len(decoded)-1
        )

    )

for state,start,end in runs:

    length = end - start + 1

    first = decoded[start]
    last  = decoded[end]

    print()

    print(
        f"{state:15s}"
        f" LEN={length:3d}"
        f" IDX={first['idx']:6d}"
        f"->{last['idx']:6d}"
    )

# ----------------------------------------------------------
# Transition matrix
# ----------------------------------------------------------

print()
print("=" * 80)
print("STATE TRANSITIONS")
print("=" * 80)

transitions = Counter()

for a,b in zip(decoded, decoded[1:]):

    sa = a["state"]
    sb = b["state"]

    if sa != sb:

        transitions[(sa,sb)] += 1

for (src,dst),count in transitions.most_common():

    print(
        f"{src:15s}"
        f" -> "
        f"{dst:15s}"
        f" : {count}"
    )

print()
print("Done.")