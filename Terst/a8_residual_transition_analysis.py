#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

# ============================================================
# A8 Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# Recovered A8 Vectors
# ============================================================

KNOWN_VECTORS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,0):0x54,
    (3,1):0xA8,
    (3,2):0x7F,
    (3,3):0xFE,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,

    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ============================================================
# Major Residual States
# ============================================================

TARGET_RESIDUALS = [
    0x5E,
    0x25,
    0x8D,
    0xF6,
    0xC7,
    0xBC
]

# ============================================================

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ============================================================

def decode_signals(frame):

    raw = build_u64(frame)

    neg_tq = ((raw >> 12) & 0x1FF) - 509

    stat_limit = ((raw >> 21) & 0x1FF)

    dyn_limit = ((raw >> 30) & 0x3FF) - 509

    tq_pct = ((raw >> 40) & 0x7F)

    rpm = (((raw >> 48) & 0xFFFF) * 0.25)

    return {
        "rpm": rpm,
        "tq_pct": tq_pct,
        "stat": stat_limit,
        "dyn": dyn_limit,
        "neg": neg_tq
    }

# ============================================================

def calc_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family_value = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    known = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    known ^= vec

    return family_value ^ known

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    stats = defaultdict(list)

    transitions = Counter()

    previous_residual = None

    frame_count = 0

    with open(filename, newline="") as csvfile:

        reader = csv.reader(csvfile)

        for row in reader:

            if not row:
                continue

            if row[0].lower() == "time":
                continue

            try:

                frame = [
                    int(x.strip(),16)
                    for x in row[1:9]
                ]

            except:
                continue

            residual = calc_residual(frame)

            sigs = decode_signals(frame)

            if residual in TARGET_RESIDUALS:

                stats[residual].append(sigs)

            if (
                previous_residual is not None
                and previous_residual in TARGET_RESIDUALS
                and residual in TARGET_RESIDUALS
                and previous_residual != residual
            ):

                transitions[
                    (previous_residual, residual)
                ] += 1

            previous_residual = residual

            frame_count += 1

    print()
    print("=" * 60)
    print("A8 RESIDUAL TRANSITION ANALYSIS")
    print("=" * 60)
    print()

    print(f"Frames Processed: {frame_count:,}")
    print()

    # ========================================================
    # Residual Statistics
    # ========================================================

    for residual in TARGET_RESIDUALS:

        if residual not in stats:
            continue

        rows = stats[residual]

        rpm_vals = [x["rpm"] for x in rows]
        dyn_vals = [x["dyn"] for x in rows]
        stat_vals = [x["stat"] for x in rows]

        print(f"Residual 0x{residual:02X}")
        print("-" * 40)

        print(f"Frames : {len(rows):,}")

        print(
            f"RPM    : "
            f"{min(rpm_vals):.0f} .. "
            f"{max(rpm_vals):.0f} "
            f"(avg {sum(rpm_vals)/len(rpm_vals):.0f})"
        )

        print(
            f"STAT   : "
            f"{min(stat_vals)} .. "
            f"{max(stat_vals)} "
            f"(avg {sum(stat_vals)/len(stat_vals):.1f})"
        )

        print(
            f"DYN    : "
            f"{min(dyn_vals)} .. "
            f"{max(dyn_vals)} "
            f"(avg {sum(dyn_vals)/len(dyn_vals):.1f})"
        )

        print()

    # ========================================================
    # Transition Analysis
    # ========================================================

    print()
    print("=" * 60)
    print("TOP RESIDUAL TRANSITIONS")
    print("=" * 60)
    print()

    for (src,dst), count in transitions.most_common(50):

        print(
            f"0x{src:02X} -> 0x{dst:02X}   {count:,}"
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()