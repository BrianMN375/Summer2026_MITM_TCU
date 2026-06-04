#!/usr/bin/env python3

import pandas as pd
from collections import Counter, defaultdict

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 B4 OVERLAY TRANSITION MAPPER")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

frames = []

# ============================================================
# LOAD
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

    frames.append({

        "idx": idx,
        "residual": residual,

        "b4": frame[4],
        "b6": frame[6],
        "b7": frame[7]

    })

# ============================================================
# FIND TRANSITIONS
# ============================================================

transition_counts = Counter()

entry_b4 = defaultdict(list)
exit_b4  = defaultdict(list)

transition_dump = []

for i in range(1, len(frames)):

    prev = frames[i-1]
    curr = frames[i]

    if prev["residual"] == curr["residual"]:
        continue

    key = (
        prev["residual"],
        curr["residual"]
    )

    transition_counts[key] += 1

    #
    # record B4 values
    #

    entry_b4[key].append(curr["b4"])
    exit_b4[key].append(prev["b4"])

    transition_dump.append({

        "idx": i,

        "from": prev["residual"],
        "to": curr["residual"],

        "prev_b4": prev["b4"],
        "curr_b4": curr["b4"],

        "prev_b6": prev["b6"],
        "curr_b6": curr["b6"],

        "prev_b7": prev["b7"],
        "curr_b7": curr["b7"]

    })

# ============================================================
# SUMMARY
# ============================================================

print()
print("=" * 80)
print("TRANSITION COUNTS")
print("=" * 80)

for (src,dst),count in transition_counts.most_common():

    print(
        f"0x{src:02X} -> 0x{dst:02X} : {count}"
    )

# ============================================================
# B4 ANALYSIS
# ============================================================

print()
print("=" * 80)
print("B4 TRANSITION ANALYSIS")
print("=" * 80)

for key in sorted(entry_b4.keys()):

    src,dst = key

    enters = entry_b4[key]
    exits  = exit_b4[key]

    print()
    print(
        f"0x{src:02X} -> 0x{dst:02X}"
    )

    print(
        f"Transitions : {len(enters)}"
    )

    print()

    print(
        f"Exit B4  "
        f"min=0x{min(exits):02X} "
        f"max=0x{max(exits):02X} "
        f"avg={sum(exits)/len(exits):.2f}"
    )

    print(
        f"Entry B4 "
        f"min=0x{min(enters):02X} "
        f"max=0x{max(enters):02X} "
        f"avg={sum(enters)/len(enters):.2f}"
    )

    #
    # histogram
    #

    hist = Counter(enters)

    print()
    print("Entry Histogram")

    for value,count in hist.most_common(10):

        print(
            f"0x{value:02X} : {count}"
        )

# ============================================================
# RAW DUMP
# ============================================================

print()
print("=" * 80)
print("RAW TRANSITIONS")
print("=" * 80)

for t in transition_dump:

    print(

        f"IDX={t['idx']:7d} "

        f"0x{t['from']:02X}"
        f"->"
        f"0x{t['to']:02X} "

        f"B4 "
        f"0x{t['prev_b4']:02X}"
        f"->"
        f"0x{t['curr_b4']:02X} "

        f"B6 "
        f"0x{t['prev_b6']:02X}"
        f"->"
        f"0x{t['curr_b6']:02X} "

        f"B7 "
        f"0x{t['prev_b7']:02X}"
        f"->"
        f"0x{t['curr_b7']:02X}"

    )

print()
print("Done.")