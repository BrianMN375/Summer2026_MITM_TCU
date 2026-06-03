#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter
from itertools import combinations

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    # "a7_payload_unique.csv"
    "a7_unique.csv"
    # "a7_balanced.csv"
)

# ------------------------------------------------------------
# CSV Loader
# ------------------------------------------------------------

def load_frame_from_row(row):

    try:

        return tuple(
            int(row[f"b{i}"], 16)
            for i in range(8)
        )

    except:
        return None

# ------------------------------------------------------------

print()
print("Using CSV:")
print(CSV_FILE)
print()

frames = []

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    for row in reader:

        frame = load_frame_from_row(row)

        if frame is not None:
            frames.append(frame)

print(f"Loaded {len(frames):,} frames")

# ------------------------------------------------------------
# Group by payload EXCLUDING counter
#
# b2..b7 only
#
# We are trying to find situations where:
#
# same payload
# different counter
#
# ------------------------------------------------------------

groups = defaultdict(list)

for frame in frames:

    checksum = frame[0]

    counter = frame[1] & 0x0F

    payload = frame[2:]

    groups[payload].append(
        (counter, checksum)
    )

# ------------------------------------------------------------

print()
print("Searching repeated payloads...")
print()

pair_counter = Counter()

delta_counter = Counter()

counter_pair_best = defaultdict(Counter)

repeated_payloads = 0

for payload, entries in groups.items():

    if len(entries) < 2:
        continue

    repeated_payloads += 1

    for a, b in combinations(entries, 2):

        ctrA, ckA = a
        ctrB, ckB = b

        delta = ckA ^ ckB

        pair_counter[
            (ctrA, ctrB)
        ] += 1

        delta_counter[
            delta
        ] += 1

        counter_pair_best[
            (ctrA, ctrB)
        ][delta] += 1

# ------------------------------------------------------------

print(f"Repeated payload groups : {repeated_payloads:,}")

print()
print("=" * 70)
print("MOST COMMON DELTAS")
print("=" * 70)

for delta, count in delta_counter.most_common(40):

    print(
        f"0x{delta:02X} "
        f"{count:8,d}"
    )

# ------------------------------------------------------------

print()
print("=" * 70)
print("COUNTER PAIR ANALYSIS")
print("=" * 70)

for ctrA in range(16):

    for ctrB in range(16):

        if ctrA >= ctrB:
            continue

        results = counter_pair_best.get(
            (ctrA, ctrB)
        )

        if not results:
            continue

        best_delta, best_count = (
            results.most_common(1)[0]
        )

        total = sum(results.values())

        confidence = (
            100.0 *
            best_count /
            total
        )

        print(
            f"{ctrA:X}->{ctrB:X}  "
            f"Best=0x{best_delta:02X}  "
            f"Count={best_count:6,d}  "
            f"Total={total:6,d}  "
            f"Confidence={confidence:6.2f}%"
        )

# ------------------------------------------------------------
# Attempt LUT Recovery
# ------------------------------------------------------------

print()
print("=" * 70)
print("PROVISIONAL LUT")
print("=" * 70)

#
# Assume counter 0 = reference
#

lut = {0: 0}

for ctr in range(1, 16):

    results = counter_pair_best.get(
        (0, ctr)
    )

    if not results:
        continue

    delta, count = results.most_common(1)[0]

    lut[ctr] = delta

for ctr in range(16):

    value = lut.get(ctr)

    if value is None:

        print(
            f"{ctr:X}: UNKNOWN"
        )

    else:

        print(
            f"{ctr:X}: 0x{value:02X}"
        )

print()
print("Done.")
print()