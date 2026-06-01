#!/usr/bin/env python3

import csv
from collections import defaultdict

CSV_FILE = "/Users/brianmay/Dropbox/Electronics Dev Library and Docs/A2 - PlatformIO/PlatformIO Repository/Github Repositories/Summer2026_MITM_TCU/Summer2026_MITM_TCU/logs/a8_full_0007.csv"

# ------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------

def signed8(v):
    return v - 256 if v > 127 else v

def rpm(frame):
    return ((frame[3] << 8) | frame[2]) / 4.0

def stat(frame):
    return ((frame[5] & 0x0F) << 8) | frame[4]

def dyn(frame):
    return signed8(frame[7])

# ------------------------------------------------------------
# Predictor (same logic as V10)
# ------------------------------------------------------------

def predicted_residual(frame):

    b4 = frame[4]
    b5 = frame[5]
    b7 = frame[7]

    d = dyn(frame)

    # HIGH NEG
    if b5 == 0x64 and d < 0:

        if b7 >= 0x46:
            return 0x38

        if b7 >= 0x40:
            return 0x62

        return 0xBC

    # POSITIVE TORQUE FAMILY
    if b5 == 0x64 and d >= 0:

        if b4 >= 0x90:
            return 0x1E

        return 0xC7

    return 0x00

# ------------------------------------------------------------
# Load CSV
# ------------------------------------------------------------

print("\nLoading dataset...")

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    for row in reader:

        try:
            frame = [int(x, 16) for x in row[-8:]]

            actual = frame[0] ^ predicted_residual(frame)

            if actual in (0xC7, 0x1E, 0xC0, 0x19):
                frames.append(
                    {
                        "frame": frame,
                        "actual": actual,
                        "rpm": rpm(frame),
                        "stat": stat(frame),
                        "dyn": dyn(frame),
                    }
                )

        except:
            continue

# ------------------------------------------------------------
# Transition search
# ------------------------------------------------------------

transitions = defaultdict(list)

for i in range(1, len(frames)):

    prev = frames[i - 1]["actual"]
    curr = frames[i]["actual"]

    if prev != curr:
        transitions[(prev, curr)].append(i)

# ------------------------------------------------------------
# Report
# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 POSITIVE TORQUE STATE MACHINE")
print("=" * 80)

for trans, indexes in sorted(transitions.items()):

    print()
    print(f"{trans[0]:02X} -> {trans[1]:02X}")
    print("-" * 60)
    print(f"Occurrences : {len(indexes)}")

# ------------------------------------------------------------
# Detailed transition dumps
# ------------------------------------------------------------

for trans, indexes in sorted(transitions.items()):

    print()
    print("=" * 80)
    print(f"TRANSITION {trans[0]:02X} -> {trans[1]:02X}")
    print("=" * 80)

    for idx in indexes[:10]:

        start = max(0, idx - 3)
        end   = min(len(frames), idx + 4)

        print()

        for j in range(start, end):

            rec = frames[j]

            marker = ">>>" if j == idx else "   "

            print(
                f"{marker} "
                f"RES=0x{rec['actual']:02X} "
                f"RPM={rec['rpm']:7.1f} "
                f"STAT={rec['stat']:4d} "
                f"DYN={rec['dyn']:4d} "
                f"B4=0x{rec['frame'][4]:02X} "
                f"B7=0x{rec['frame'][7]:02X}"
            )

print()
print("Done.")