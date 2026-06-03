#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "all_0xAx_combinedSet1.csv"
)

TARGET_ID = 0x0A7

# ============================================================
# LOAD
# ============================================================

payload_groups = defaultdict(list)

total = 0

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            can_id = int(row["id"], 16)

            if can_id != TARGET_ID:
                continue

            frame = [
                int(row[f"b{i}"], 16)
                for i in range(8)
            ]

            checksum = frame[0]

            counter = frame[1] & 0x0F

            payload_key = tuple(frame[2:8])

            payload_groups[payload_key].append(
                (counter, checksum)
            )

            total += 1

        except:
            pass

print()
print(f"Loaded {total:,} A7 frames")
print(f"Unique payloads: {len(payload_groups):,}")

# ============================================================
# COUNTER RELATIONSHIP HISTOGRAMS
# ============================================================

pair_hist = defaultdict(Counter)

for payload, entries in payload_groups.items():

    if len(entries) < 2:
        continue

    n = len(entries)

    for i in range(n):

        ctr_a, ck_a = entries[i]

        for j in range(i + 1, n):

            ctr_b, ck_b = entries[j]

            delta = ck_a ^ ck_b

            pair_hist[
                (ctr_a, ctr_b)
            ][delta] += 1

            pair_hist[
                (ctr_b, ctr_a)
            ][delta] += 1

print()
print("=" * 70)
print("PAIR RELATIONSHIPS")
print("=" * 70)

for a in range(16):

    for b in range(16):

        if a == b:
            continue

        hist = pair_hist.get((a, b))

        if not hist:
            continue

        total_pairs = sum(hist.values())

        delta, count = hist.most_common(1)[0]

        confidence = (
            100.0 *
            count /
            total_pairs
        )

        if total_pairs >= 50:

            print(
                f"{a:X}->{b:X}  "
                f"0x{delta:02X}  "
                f"{confidence:6.2f}%  "
                f"n={total_pairs}"
            )

# ============================================================
# BUILD TABLE
# ============================================================

print()
print("=" * 70)
print("RECOVERING TABLE")
print("=" * 70)

transform = {
    0: 0x00
}

changed = True

while changed:

    changed = False

    for (a, b), hist in pair_hist.items():

        if a not in transform:
            continue

        delta, count = hist.most_common(1)[0]

        candidate = (
            transform[a]
            ^ delta
        )

        if b not in transform:

            transform[b] = candidate
            changed = True

print()

for i in range(16):

    value = transform.get(i)

    if value is None:

        print(
            f"{i:X}: UNKNOWN"
        )

    else:

        print(
            f"{i:X}: 0x{value:02X}"
        )

# ============================================================
# VERIFY
# ============================================================

print()
print("=" * 70)
print("VERIFY")
print("=" * 70)

correct = 0
total_pairs = 0

for (a, b), hist in pair_hist.items():

    if a not in transform:
        continue

    if b not in transform:
        continue

    observed, count = hist.most_common(1)[0]

    predicted = (
        transform[a]
        ^ transform[b]
    )

    total_pairs += 1

    if predicted == observed:
        correct += 1

accuracy = (
    100.0 *
    correct /
    total_pairs
)

print(
    f"Pair Accuracy: "
    f"{accuracy:.2f}%"
)

print()
print("TABLE")

table = []

for i in range(16):

    table.append(
        transform.get(i, 0)
    )

for i, value in enumerate(table):

    print(
        f"0x{value:02X},",
        end=" "
    )

    if (i + 1) % 4 == 0:
        print()

print()
print()
print("Done.")