#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = "../logs/a8_full_0015_a8only.csv"

TARGET_RESIDUALS = {
    0x91,
    0xD3,
    0x7A,
    0xCF
}

# ============================================================
# YOUR CURRENT VALIDATOR FUNCTION
# ============================================================

def predicted_crc(b):
    """
    Paste the exact checksum model currently used by
    a8_final_validator_v13.py here.

    Return predicted CRC.
    """

    return 0


# ============================================================
# Helpers
# ============================================================

def rpm_from_b4(b4):
    return b4 * 31.5

def stat_from_b5_b6(b5, b6):
    return (b5 << 1) | (b6 >> 7)

def dyn_from_b6(b6):
    return ((b6 & 0x7F) - 64) * 2


# ============================================================
# Containers
# ============================================================

stats = {}

for r in TARGET_RESIDUALS:

    stats[r] = {
        "frames": 0,

        "rpm": [],
        "stat": [],
        "dyn": [],

        "b4": Counter(),
        "b5": Counter(),
        "b6": Counter(),
        "b7": Counter(),
        "cnt": Counter(),

        "sig": Counter()
    }

# ============================================================
# Parse CSV
# ============================================================

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    for row in reader:

        b = []

        for i in range(8):
            b.append(int(row[f"Byte{i}"], 16))

        observed = b[0]
        predicted = predicted_crc(b)

        residual = observed ^ predicted

        if residual not in TARGET_RESIDUALS:
            continue

        s = stats[residual]

        b4 = b[4]
        b5 = b[5]
        b6 = b[6]
        b7 = b[7]

        cnt = b[1] & 0x0F

        rpm = rpm_from_b4(b4)
        stat = stat_from_b5_b6(b5, b6)
        dyn = dyn_from_b6(b6)

        s["frames"] += 1

        s["rpm"].append(rpm)
        s["stat"].append(stat)
        s["dyn"].append(dyn)

        s["b4"][b4] += 1
        s["b5"][b5] += 1
        s["b6"][b6] += 1
        s["b7"][b7] += 1
        s["cnt"][cnt] += 1

        signature = (b4, b5, b7)
        s["sig"][signature] += 1

# ============================================================
# Report
# ============================================================

print()
print("=" * 80)
print("A8 TARGET RESIDUAL PROFILER")
print("=" * 80)

for residual in sorted(stats):

    s = stats[residual]

    if s["frames"] == 0:
        continue

    print()
    print("=" * 80)
    print(f"RESIDUAL 0x{residual:02X}")
    print("=" * 80)

    print()
    print(f"Frames : {s['frames']}")

    rpm_avg = sum(s["rpm"]) / len(s["rpm"])
    stat_avg = sum(s["stat"]) / len(s["stat"])
    dyn_avg = sum(s["dyn"]) / len(s["dyn"])

    print(
        f"RPM  : {min(s['rpm']):.1f}..{max(s['rpm']):.1f} "
        f"avg={rpm_avg:.1f}"
    )

    print(
        f"STAT : {min(s['stat'])}..{max(s['stat'])} "
        f"avg={stat_avg:.1f}"
    )

    print(
        f"DYN  : {min(s['dyn'])}..{max(s['dyn'])} "
        f"avg={dyn_avg:.1f}"
    )

    print()
    print("BYTE4")
    print("-" * 20)

    for val, cnt in s["b4"].most_common(20):
        print(f"0x{val:02X} {cnt:6}")

    print()
    print("BYTE5")
    print("-" * 20)

    for val, cnt in s["b5"].most_common(20):
        print(f"0x{val:02X} {cnt:6}")

    print()
    print("BYTE6")
    print("-" * 20)

    for val, cnt in s["b6"].most_common(20):
        print(f"0x{val:02X} {cnt:6}")

    print()
    print("BYTE7")
    print("-" * 20)

    for val, cnt in s["b7"].most_common(20):
        print(f"0x{val:02X} {cnt:6}")

    print()
    print("COUNTER")
    print("-" * 20)

    for val, cnt in sorted(s["cnt"].items()):
        print(f"{val:2d} {cnt:6}")

    print()
    print("TOP SIGNATURES")
    print("-" * 20)

    for (b4, b5, b7), cnt in s["sig"].most_common(20):

        print(
            f"B4=0x{b4:02X} "
            f"B5=0x{b5:02X} "
            f"B7=0x{b7:02X} "
            f" -> {cnt}"
        )