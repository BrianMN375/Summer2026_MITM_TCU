#!/usr/bin/env python3

import pandas as pd

from a8_final_validator_v13 import (
    CSV_FILE,
    actual_residual,
    predicted_residual
)

print()
print("=" * 80)
print("A8 DE B7 PEAK ANALYSIS")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

# ============================================================
# COLLECT B7 = 0x40
# ============================================================

de_rows = []
normal_rows = []

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

    b7 = frame[7]

    if b7 != 0x40:
        continue

    rpm  = getattr(row,"rpm",0)
    stat = getattr(row,"stat",0)
    dyn  = getattr(row,"dyn",0)

    record = {

        "idx":idx,
        "b4":frame[4],
        "b6":frame[6],
        "rpm":rpm,
        "stat":stat,
        "dyn":dyn

    }

    if residual == 0xDE:
        de_rows.append(record)
    else:
        normal_rows.append(record)

print()
print("=" * 80)
print("POPULATION")
print("=" * 80)

print(f"B7=0x40 Total : {len(de_rows)+len(normal_rows)}")
print(f"DE            : {len(de_rows)}")
print(f"Normal        : {len(normal_rows)}")

# ============================================================
# RANGE DUMP
# ============================================================

def summarize(name,rows):

    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    if not rows:
        return

    for field in ["b4","b6","rpm","stat","dyn"]:

        vals = [r[field] for r in rows]

        print(

            f"{field:5s} "
            f"min={min(vals):8.2f} "
            f"max={max(vals):8.2f} "
            f"avg={sum(vals)/len(vals):8.2f}"

        )

summarize("DE @ B7=0x40",de_rows)
summarize("NORMAL @ B7=0x40",normal_rows)

# ============================================================
# SIMPLE THRESHOLD SEARCH
# ============================================================

print()
print("=" * 80)
print("BEST SINGLE FEATURE")
print("=" * 80)

best_acc = 0
best_rule = None

for field in ["b4","b6","rpm","stat","dyn"]:

    values = sorted(

        set(

            int(r[field])
            for r in de_rows + normal_rows

        )

    )

    for threshold in values:

        correct = 0
        total = len(de_rows)+len(normal_rows)

        for r in de_rows:

            if r[field] <= threshold:
                correct += 1

        for r in normal_rows:

            if r[field] > threshold:
                correct += 1

        acc = correct / total

        if acc > best_acc:

            best_acc = acc
            best_rule = (field,threshold)

print(

    f"{best_rule[0]} <= {best_rule[1]}"
)

print(

    f"Accuracy = {best_acc*100:.4f}%"

)

# ============================================================
# TOP DE FRAMES
# ============================================================

print()
print("=" * 80)
print("FIRST 25 DE FRAMES")
print("=" * 80)

for r in de_rows[:25]:

    print(

        f"IDX={r['idx']:7d} "
        f"B4=0x{r['b4']:02X} "
        f"B6=0x{r['b6']:02X} "
        f"RPM={r['rpm']:7.1f} "
        f"STAT={r['stat']:4d} "
        f"DYN={r['dyn']:4d}"

    )

print()
print("Done.")