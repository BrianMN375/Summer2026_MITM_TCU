#!/usr/bin/env python3

import csv
from collections import defaultdict

# ============================================================
# Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# Bit Extractor
# ============================================================

def extract_le(data, start_bit, bit_length):

    raw = 0

    for i, b in enumerate(data):
        raw |= b << (8 * i)

    mask = (1 << bit_length) - 1

    return (raw >> start_bit) & mask

# ============================================================
# Decode A8
# ============================================================

def decode_a8(frame):

    neg_torque_available = (
        extract_le(frame, 12, 9) - 509
    )

    torque_limit_stat = (
        extract_le(frame, 21, 9)
    )

    torque_limit_dyn = (
        extract_le(frame, 30, 10) - 509
    )

    torque_percent = (
        extract_le(frame, 40, 7)
    )

    rpm_raw = (
        extract_le(frame, 48, 16)
    )

    rpm = rpm_raw * 0.25

    return {
        "neg_torque_available": neg_torque_available,
        "torque_limit_stat": torque_limit_stat,
        "torque_limit_dyn": torque_limit_dyn,
        "torque_percent": torque_percent,
        "rpm": rpm
    }

# ============================================================
# Main
# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    frame_count = 0

    rpm_to_seed = defaultdict(set)
    tqpct_to_seed = defaultdict(set)
    stat_to_seed = defaultdict(set)
    dyn_to_seed = defaultdict(set)
    neg_to_seed = defaultdict(set)

    with open(filename, newline='') as csvfile:

        reader = csv.reader(csvfile)

        for row in reader:

            if not row:
                continue

            if row[0].lower() == "time":
                continue

            try:

                frame = [
                    int(x.strip(), 16)
                    for x in row[1:9]
                ]

            except:
                continue

            frame_count += 1

            checksum = frame[0]

            counter = (
                frame[1] & 0x0F
            )

            seed = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            sig = decode_a8(frame)

            rpm_to_seed[
                sig["rpm"]
            ].add(seed)

            tqpct_to_seed[
                sig["torque_percent"]
            ].add(seed)

            stat_to_seed[
                sig["torque_limit_stat"]
            ].add(seed)

            dyn_to_seed[
                sig["torque_limit_dyn"]
            ].add(seed)

            neg_to_seed[
                sig["neg_torque_available"]
            ].add(seed)

    # ========================================================

    print()
    print("====================================")
    print("A8 SEED DEPENDENCY ANALYSIS")
    print("====================================")
    print()

    print(
        f"Frames: {frame_count:,}"
    )

    print()

    def report(name, mapping):

        unique_values = len(mapping)

        unique_seeds = len(
            set().union(*mapping.values())
        )

        avg = sum(
            len(v)
            for v in mapping.values()
        ) / unique_values

        print(
            f"{name}"
        )

        print(
            f"  Unique Values : {unique_values:,}"
        )

        print(
            f"  Unique Seeds  : {unique_seeds:,}"
        )

        print(
            f"  Avg Seeds/Value : {avg:.2f}"
        )

        print()

    report(
        "RPM",
        rpm_to_seed
    )

    report(
        "Torque %",
        tqpct_to_seed
    )

    report(
        "Torque Limit Stat",
        stat_to_seed
    )

    report(
        "Torque Limit Dyn",
        dyn_to_seed
    )

    report(
        "Negative Torque Available",
        neg_to_seed
    )

    print(
        "Analysis Complete."
    )

# ============================================================

if __name__ == "__main__":
    main()