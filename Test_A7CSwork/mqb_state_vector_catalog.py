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

POLY = 0x2F
SEED = 0x0E

# ============================================================
# LFSR
# ============================================================

def advance(v):

    msb = v & 0x80

    v = (v << 1) & 0xFF

    if msb:
        v ^= POLY

    return v

# ============================================================
# BUILD BASIS TABLE
# ============================================================

basis = []
seen = set()

current = SEED

while current not in seen:

    seen.add(current)

    basis.append(current)

    current = advance(current)

value_to_index = {
    value: idx
    for idx, value in enumerate(basis)
}

print()
print("=" * 80)
print("MQB STATE VECTOR CATALOG")
print("=" * 80)

print(f"Basis Length: {len(basis)}")

# ============================================================
# LOAD A7
# ============================================================

states = defaultdict(list)

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

            states[state].append(frame)

        except:
            pass

print()
print(f"Loaded {sum(len(v) for v in states.values()):,} A7 frames")

# ============================================================
# VECTOR DISCOVERY
# ============================================================

def discover_vectors(frames):

    payload_map = {}

    for frame in frames:

        payload_map[
            frame[1:]
        ] = frame[0]

    bit_vectors = defaultdict(Counter)

    for payload, checksum in payload_map.items():

        p = list(payload)

        for byte_idx in range(7):

            for bit_idx in range(8):

                neighbor = p.copy()

                neighbor[byte_idx] ^= (
                    1 << bit_idx
                )

                neighbor = tuple(neighbor)

                if neighbor not in payload_map:
                    continue

                delta = (
                    checksum ^
                    payload_map[neighbor]
                )

                bit_vectors[
                    (byte_idx + 1, bit_idx)
                ][delta] += 1

    results = {}

    for bit, hist in bit_vectors.items():

        total = sum(hist.values())

        delta, count = (
            hist.most_common(1)[0]
        )

        conf = (
            100.0 *
            count /
            total
        )

        if (
            total >= 20
            and conf >= 80
            and delta != 0
        ):
            results[bit] = (
                delta,
                conf,
                total
            )

    return results

# ============================================================
# PROCESS STATES
# ============================================================

state_vectors = {}

for state in range(16):

    frames = states[state]

    if not frames:
        continue

    vectors = discover_vectors(frames)

    state_vectors[state] = vectors

    print()
    print("=" * 80)
    print(f"STATE {state:X}")
    print("=" * 80)

    print(f"Frames  : {len(frames):,}")
    print(f"Vectors : {len(vectors)}")

    print()

    positions = []

    for bit in sorted(vectors):

        delta = vectors[bit][0]

        idx = value_to_index.get(delta)

        if idx is None:
            idx_text = "??"
        else:
            idx_text = str(idx)
            positions.append(idx)

        print(
            f"b{bit[0]}[{bit[1]}] "
            f"= 0x{delta:02X} "
            f"basis[{idx_text}]"
        )

    positions.sort()

    print()

    print("Basis Positions:")

    for pos in positions:

        print(pos, end=" ")

    print()

# ============================================================
# STATE OVERLAP
# ============================================================

print()
print("=" * 80)
print("STATE OVERLAP MATRIX")
print("=" * 80)

header = "     "

for s in range(16):
    header += f"{s:>5X}"

print(header)

for s1 in range(16):

    line = f"{s1:>3X} "

    v1 = state_vectors.get(s1, {})

    for s2 in range(16):

        v2 = state_vectors.get(s2, {})

        shared = (
            set(v1.keys())
            &
            set(v2.keys())
        )

        exact = 0

        for key in shared:

            if v1[key][0] == v2[key][0]:
                exact += 1

        pct = (
            100.0 * exact / len(shared)
            if shared else 0
        )

        line += f"{pct:5.0f}"

    print(line)

print()
print("Done.")
print()