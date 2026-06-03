#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

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

# ------------------------------------------------------------
# Counter table
# ------------------------------------------------------------

COUNTER_TRANSFORM = [
    0x00, 0x88, 0xFC, 0x7D,
    0x47, 0x3C, 0xAF, 0x26,
    0x6B, 0xBB, 0xBD, 0xD3,
    0x6C, 0x9C, 0x17, 0xC9,
]

# ------------------------------------------------------------
# State table
# ------------------------------------------------------------

STATE_FAMILY = [
    0x6A, 0xBB, 0x95, 0x37,
    0x7F, 0xDC, 0x34, 0xC3,
    0x99, 0xE3, 0x7D, 0xAC,
    0xF5, 0x24, 0x78, 0xA9,
]

# ------------------------------------------------------------
# Load
# ------------------------------------------------------------

frames_by_state = defaultdict(list)

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            can_id = int(row["id"], 16)

            if can_id != TARGET_ID:
                continue

            frame = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            state = frame[1] >> 4

            frames_by_state[state].append(frame)

        except:
            pass

total = sum(len(v) for v in frames_by_state.values())

print()
print(f"Loaded {total:,} A7 frames")

# ------------------------------------------------------------
# Discover vectors for each state
# ------------------------------------------------------------

state_vectors = {}

for state in range(16):

    frames = frames_by_state[state]

    if len(frames) < 1000:
        continue

    print()
    print("=" * 80)
    print(f"STATE {state:X}")
    print("=" * 80)

    payload_map = {}

    for frame in frames:

        counter = frame[1] & 0x0F

        residual = (
            frame[0]
            ^ COUNTER_TRANSFORM[counter]
            ^ STATE_FAMILY[state]
        )

        payload = tuple(frame[1:8])

        payload_map[payload] = residual

    vectors = {}

    payloads = list(payload_map.keys())

    checked = 0

    for payload in payloads:

        checked += 1

        if checked % 10000 == 0:
            print(
                f"State {state:X}: "
                f"{checked:,}/{len(payloads):,}"
            )

        r1 = payload_map[payload]

        for byte_idx in range(7):

            for bit_idx in range(8):

                modified = list(payload)

                modified[byte_idx] ^= (1 << bit_idx)

                modified = tuple(modified)

                if modified not in payload_map:
                    continue

                r2 = payload_map[modified]

                delta = r1 ^ r2

                key = (byte_idx + 1, bit_idx)

                if key not in vectors:
                    vectors[key] = Counter()

                vectors[key][delta] += 1

    final_vectors = {}

    for key, hist in vectors.items():

        best_val, best_count = hist.most_common(1)[0]

        confidence = (
            100.0 * best_count / sum(hist.values())
        )

        if confidence < 90:
            continue

        if best_count < 10:
            continue

        final_vectors[key] = (
            best_val,
            best_count,
            confidence
        )

    state_vectors[state] = final_vectors

    print()
    print(f"Vectors found: {len(final_vectors)}")
    print()

    for key in sorted(final_vectors):

        val, cnt, conf = final_vectors[key]

        print(
            f"{key}: "
            f"0x{val:02X} "
            f"matches={cnt:,} "
            f"conf={conf:.2f}%"
        )

# ------------------------------------------------------------
# Compare states
# ------------------------------------------------------------

print()
print("=" * 80)
print("STATE COMPARISON")
print("=" * 80)

for s1 in sorted(state_vectors):

    for s2 in sorted(state_vectors):

        if s2 <= s1:
            continue

        v1 = state_vectors[s1]
        v2 = state_vectors[s2]

        shared = set(v1.keys()) & set(v2.keys())

        if not shared:
            continue

        exact = 0

        for key in shared:

            if v1[key][0] == v2[key][0]:
                exact += 1

        similarity = (
            100.0 * exact / len(shared)
        )

        print(
            f"State {s1:X} vs {s2:X} : "
            f"{exact}/{len(shared)} "
            f"({similarity:.2f}%)"
        )

print()
print("Done.")