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
    # "a8_full_0014.csv"
    # "a8_full_0015.csv"
    "a8_full_0017.csv"
    # "all_0xAx_combinedSet1.csv"
)



# ============================================================
# LOAD DATA
# ============================================================

print()
print("Using CSV:")
print(CSV_FILE)
print()

id_frames = defaultdict(list)

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            can_id = int(row["id"], 16)

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            id_frames[can_id].append(frame)

        except Exception:
            pass

# ============================================================
# SUMMARY
# ============================================================

print("=" * 80)
print("MQB AX FAMILY CENSUS")
print("=" * 80)

for can_id in sorted(id_frames):

    frames = id_frames[can_id]

    upper_hist = Counter()
    checksum_hist = Counter()

    for frame in frames:

        checksum = frame[0]

        upper = frame[1] >> 4

        upper_hist[upper] += 1
        checksum_hist[checksum] += 1

    print()
    print("=" * 80)
    print(f"ID 0x{can_id:03X}")
    print("=" * 80)

    print(f"Frames            : {len(frames):,}")
    print(f"Unique Checksums  : {len(checksum_hist)}")
    print(f"Unique States     : {len(upper_hist)}")

    top_ck, top_count = checksum_hist.most_common(1)[0]

    print(
        f"Most Common CK    : "
        f"0x{top_ck:02X} "
        f"({100.0 * top_count / len(frames):.2f}%)"
    )

    print()
    print("Upper Nibble Distribution")
    print("-------------------------")

    for upper in sorted(upper_hist):

        count = upper_hist[upper]

        pct = (
            100.0 *
            count /
            len(frames)
        )

        print(
            f"0x{upper:X} : "
            f"{count:>8,d} "
            f"({pct:6.2f}%)"
        )

# ============================================================
# CROSS-ID STATE MATRIX
# ============================================================

print()
print("=" * 80)
print("CROSS-ID STATE MATRIX")
print("=" * 80)

all_states = set()

for frames in id_frames.values():

    for frame in frames:

        all_states.add(frame[1] >> 4)

all_states = sorted(all_states)

header = "ID     "

for state in all_states:

    header += f"{state:X:^10}"

print(header)

for can_id in sorted(id_frames):

    frames = id_frames[can_id]

    hist = Counter()

    for frame in frames:

        hist[frame[1] >> 4] += 1

    total = len(frames)

    line = f"0x{can_id:03X} "

    for state in all_states:

        pct = (
            100.0 *
            hist[state] /
            total
        ) if total else 0

        line += f"{pct:9.1f}% "

    print(line)

# ============================================================
# CHECKSUM DOMINANCE
# ============================================================

print()
print("=" * 80)
print("CHECKSUM DOMINANCE")
print("=" * 80)

for can_id in sorted(id_frames):

    frames = id_frames[can_id]

    checksum_hist = Counter()

    for frame in frames:

        checksum_hist[frame[0]] += 1

    top_ck, top_count = checksum_hist.most_common(1)[0]

    pct = (
        100.0 *
        top_count /
        len(frames)
    )

    print(
        f"0x{can_id:03X}   "
        f"Top CK=0x{top_ck:02X}   "
        f"{pct:6.2f}%"
    )

print()
print("Done.")
print()