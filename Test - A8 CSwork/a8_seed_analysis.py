#!/usr/bin/env python3

import csv
from collections import defaultdict

# ============================================================
# Counter Transform
#
# Current working hypothesis.
# Can be updated later.
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# Little-endian bit extractor
# ============================================================

def extract_le(data, start_bit, bit_length):

    raw = 0

    for i, b in enumerate(data):
        raw |= b << (8 * i)

    mask = (1 << bit_length) - 1

    return (raw >> start_bit) & mask

# ============================================================
# Decode A8 DBC
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

    return (
        neg_torque_available,
        torque_limit_stat,
        torque_limit_dyn,
        torque_percent,
        rpm
    )

# ============================================================
# Main
# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    frame_count = 0

    unique_families = set()
    unique_states = set()
    unique_seeds = set()

    payload_to_seed = defaultdict(set)
    state_to_seed = defaultdict(set)

    seed_usage = defaultdict(int)

    with open(filename, newline='') as csvfile:

        reader = csv.reader(csvfile)

        for row in reader:

            if not row:
                continue

            # Skip header
            if row[0].lower() == "time":
                continue

            try:

                # time,b0,b1,b2,b3,b4,b5,b6,b7

                data = [
                    int(x.strip(), 16)
                    for x in row[1:9]
                ]

                if len(data) != 8:
                    continue

            except Exception:
                continue

            frame_count += 1

            checksum = data[0]

            counter = (
                data[1] & 0x0F
            )

            seed = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            unique_seeds.add(seed)

            # ----------------------------------
            # Payload family
            # ----------------------------------

            family = tuple(data[1:8])

            unique_families.add(family)

            payload_to_seed[family].add(seed)

            # ----------------------------------
            # Decoded state
            # ----------------------------------

            decoded_state = decode_a8(data)

            unique_states.add(decoded_state)

            state_to_seed[decoded_state].add(seed)

            seed_usage[seed] += 1

    # ========================================================
    # Statistics
    # ========================================================

    payload_collisions = sum(
        1
        for v in payload_to_seed.values()
        if len(v) > 1
    )

    state_collisions = sum(
        1
        for v in state_to_seed.values()
        if len(v) > 1
    )

    # ========================================================
    # Report
    # ========================================================

    print()
    print("==========================================")
    print("A8 SEED ANALYSIS REPORT")
    print("==========================================")
    print()

    print(
        f"Frames: {frame_count:,}"
    )

    print(
        f"Unique Payload Families: "
        f"{len(unique_families):,}"
    )

    print(
        f"Unique Decoded States: "
        f"{len(unique_states):,}"
    )

    print(
        f"Unique Seeds: "
        f"{len(unique_seeds):,}"
    )

    print()

    print(
        f"Payload->Seed Collisions: "
        f"{payload_collisions:,}"
    )

    print(
        f"DecodedState->Seed Collisions: "
        f"{state_collisions:,}"
    )

    print()

    print("Most Common Seeds")
    print("-----------------")

    ranked = sorted(
        seed_usage.items(),
        key=lambda x: x[1],
        reverse=True
    )

    for seed, count in ranked[:20]:

        print(
            f"0x{seed:02X} : {count:,}"
        )

    print()
    print("Analysis Complete.")

# ============================================================

if __name__ == "__main__":
    main()