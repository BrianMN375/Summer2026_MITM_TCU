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
print("A8 HIGH84 STABLE POPULATION SEARCH")
print("=" * 80)

df = pd.read_csv(CSV_FILE)

matches = []

# ============================================================
# SEARCH
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

    b4 = frame[4]
    b6 = frame[6]
    b7 = frame[7]

    #
    # Stable HIGH84 signature
    #

    if not (329 <= stat <= 337):
        continue

    if not (0x87 <= b4 <= 0x8F):
        continue

    if not (0x4F <= b7 <= 0x50):
        continue

    matches.append({

        "idx": idx,
        "residual": residual,

        "rpm": rpm,
        "stat": stat,
        "dyn": dyn,

        "b4": b4,
        "b6": b6,
        "b7": b7

    })

print()
print(f"Matches : {len(matches)}")

# ============================================================
# RESIDUAL BREAKDOWN
# ============================================================

print()
print("=" * 80)
print("RESIDUAL BREAKDOWN")
print("=" * 80)

counts = Counter(m["residual"] for m in matches)

for residual, count in counts.most_common():

    print(
        f"0x{residual:02X} : {count}"
    )

# ============================================================
# DUMP ALL MATCHES
# ============================================================

print()
print("=" * 80)
print("MATCHES")
print("=" * 80)

for m in matches:

    print(
        f"IDX={m['idx']:7d} "
        f"RES=0x{m['residual']:02X} "
        f"B4=0x{m['b4']:02X} "
        f"B6=0x{m['b6']:02X} "
        f"B7=0x{m['b7']:02X} "
        f"RPM={m['rpm']:7.1f} "
        f"STAT={m['stat']:3d} "
        f"DYN={m['dyn']:3d}"
    )

# ============================================================
# SUMMARY
# ============================================================

if matches:

    print()
    print("=" * 80)
    print("SIGNATURE SUMMARY")
    print("=" * 80)

    for field in (
        "rpm",
        "stat",
        "dyn",
        "b4",
        "b6",
        "b7"
    ):

        vals = [m[field] for m in matches]

        print(
            f"{field:6s}"
            f" min={min(vals):8.2f}"
            f" max={max(vals):8.2f}"
            f" avg={sum(vals)/len(vals):8.2f}"
        )

print()
print("Done.")