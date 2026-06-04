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
# Decode RPM
# ============================================================

def decode_rpm(frame):

    rpm_raw = extract_le(frame, 48, 16)

    return rpm_raw * 0.25

# ============================================================
# Main
# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    rpm_to_seeds = defaultdict(set)
    rpm_counts = defaultdict(int)

    bucket_to_seeds = defaultdict(set)
    bucket_counts = defaultdict(int)

    frames = 0

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

            frames += 1

            checksum = frame[0]

            counter = frame[1] & 0x0F

            seed = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            rpm = int(
                decode_rpm(frame)
            )

            rpm_to_seeds[rpm].add(seed)
            rpm_counts[rpm] += 1

            # 250 RPM buckets
            bucket = (
                rpm // 250
            ) * 250

            bucket_to_seeds[bucket].add(seed)
            bucket_counts[bucket] += 1

    print()
    print("======================================")
    print("A8 RPM SEED ANALYSIS")
    print("======================================")
    print()

    print(
        f"Frames: {frames:,}"
    )

    print(
        f"Unique RPM values: "
        f"{len(rpm_to_seeds):,}"
    )

    print(
        f"Unique RPM buckets: "
        f"{len(bucket_to_seeds):,}"
    )

    print()

    avg_exact = (
        sum(len(v)
            for v in rpm_to_seeds.values())
        /
        len(rpm_to_seeds)
    )

    avg_bucket = (
        sum(len(v)
            for v in bucket_to_seeds.values())
        /
        len(bucket_to_seeds)
    )

    print(
        f"Avg Seeds per Exact RPM : "
        f"{avg_exact:.2f}"
    )

    print(
        f"Avg Seeds per 250RPM Bucket : "
        f"{avg_bucket:.2f}"
    )

    print()
    print("Top RPM Buckets")
    print("----------------")

    ranked = sorted(
        bucket_counts.items(),
        key=lambda x: x[1],
        reverse=True
    )

    for bucket, count in ranked[:20]:

        seeds = len(
            bucket_to_seeds[bucket]
        )

        print(
            f"{bucket:5d}-{bucket+249:5d} RPM "
            f"Frames={count:6d} "
            f"Seeds={seeds:3d}"
        )

    print()
    print("Analysis Complete.")

if __name__ == "__main__":
    main()