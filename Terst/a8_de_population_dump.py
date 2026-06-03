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
print("A8 DE POPULATION DUMP")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

de_rows = []
zero_rows = []

# ============================================================
# LOAD DATA
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

    state = actual ^ predicted

    if state not in (0x00, 0xDE):
        continue

    rpm, stat, dyn = decode_signals(frame)

    b4 = frame[4]

    #
    # MID FAMILY
    #

    if not (0x80 <= b4 < 0xC0):
        continue

    #
    # MID RPM
    #

    if not (4000 <= rpm < 6200):
        continue

    rec = {

        "b4": frame[4],
        "b6": frame[6],
        "b7": frame[7],

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn

    }

    if state == 0xDE:
        de_rows.append(rec)
    else:
        zero_rows.append(rec)

print()
print(f"00 Frames : {len(zero_rows):,}")
print(f"DE Frames : {len(de_rows):,}")

# ============================================================
# SUMMARY STATS
# ============================================================

def dump_summary(name, rows):

    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    for field in [

        "b4",
        "b6",
        "b7",

        "rpm",
        "stat",
        "dyn"

    ]:

        vals = [r[field] for r in rows]

        print(

            f"{field:8s}"

            f" min={min(vals):8.2f}"

            f" max={max(vals):8.2f}"

            f" avg={sum(vals)/len(vals):8.2f}"

        )

dump_summary("STATE 0x00", zero_rows)
dump_summary("STATE 0xDE", de_rows)

# ============================================================
# TOP B4 VALUES
# ============================================================

print()
print("=" * 80)
print("TOP B4 VALUES")
print("=" * 80)

for label,rows in [

    ("00", zero_rows),
    ("DE", de_rows)

]:

    print()
    print(label)

    counts = Counter(

        r["b4"]
        for r in rows

    )

    for value,count in (

        counts.most_common(25)

    ):

        print(

            f"0x{value:02X}"
            f" : {count}"

        )

# ============================================================
# TOP B6 VALUES
# ============================================================

print()
print("=" * 80)
print("TOP B6 VALUES")
print("=" * 80)

for label,rows in [

    ("00", zero_rows),
    ("DE", de_rows)

]:

    print()
    print(label)

    counts = Counter(

        r["b6"]
        for r in rows

    )

    for value,count in (

        counts.most_common(25)

    ):

        print(

            f"0x{value:02X}"
            f" : {count}"

        )

# ============================================================
# TOP STAT VALUES
# ============================================================

print()
print("=" * 80)
print("TOP STAT VALUES")
print("=" * 80)

for label,rows in [

    ("00", zero_rows),
    ("DE", de_rows)

]:

    print()
    print(label)

    counts = Counter(

        r["stat"]
        for r in rows

    )

    for value,count in (

        counts.most_common(25)

    ):

        print(

            f"{value:4d}"
            f" : {count}"

        )

# ============================================================
# TOP RPM VALUES
# ============================================================

print()
print("=" * 80)
print("TOP RPM VALUES")
print("=" * 80)

for label,rows in [

    ("00", zero_rows),
    ("DE", de_rows)

]:

    print()
    print(label)

    counts = Counter(

        int(r["rpm"] // 50) * 50
        for r in rows

    )

    for value,count in (

        counts.most_common(25)

    ):

        print(

            f"{value:4d}"
            f" : {count}"

        )

print()
print("Done.")