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

# ============================================================
# COMPARISONS
# ============================================================

COMPARISONS = [

    ("A7_D", 0x0A7, 0xD,
     "A8_D", 0x0A8, 0xD),

    ("A7_E", 0x0A7, 0xE,
     "AD_E", 0x0AD, 0xE),

    ("A7_0", 0x0A7, 0x0,
     "AE_0", 0x0AE, 0x0),

    ("A7_1", 0x0A7, 0x1,
     "AF_1", 0x0AF, 0x1),

    ("A7_A", 0x0A7, 0xA,
     "AF_A", 0x0AF, 0xA),
]

# ============================================================
# LOAD
# ============================================================

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        try:

            can_id = int(row["id"], 16)

            data = tuple(
                int(row[f"b{i}"], 16)
                for i in range(8)
            )

            frames.append(
                (can_id, data)
            )

        except:
            pass

print()
print(f"Loaded {len(frames):,} frames")

# ============================================================
# FILTER
# ============================================================

def get_frames(can_id, state):

    result = []

    for fid, frame in frames:

        if fid != can_id:
            continue

        upper = frame[1] >> 4

        if upper != state:
            continue

        result.append(frame)

    return result

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

    result = {}

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
            result[bit] = delta

    return result

# ============================================================
# ANALYSIS
# ============================================================

for (
    name1, id1, st1,
    name2, id2, st2
) in COMPARISONS:

    f1 = get_frames(id1, st1)
    f2 = get_frames(id2, st2)

    print()
    print("=" * 80)
    print(f"{name1}  vs  {name2}")
    print("=" * 80)

    print()
    print(
        f"{name1}: "
        f"{len(f1):,} frames"
    )

    print(
        f"{name2}: "
        f"{len(f2):,} frames"
    )

    payloads1 = {
        x[1:]
        for x in f1
    }

    payloads2 = {
        x[1:]
        for x in f2
    }

    print()
    print(
        f"{name1} unique payloads: "
        f"{len(payloads1):,}"
    )

    print(
        f"{name2} unique payloads: "
        f"{len(payloads2):,}"
    )

    v1 = discover_vectors(f1)
    v2 = discover_vectors(f2)

    shared = (
        set(v1.keys())
        &
        set(v2.keys())
    )

    exact = 0

    for key in shared:

        if v1[key] == v2[key]:
            exact += 1

    similarity = (
        100.0 * exact / len(shared)
        if shared else 0
    )

    print()
    print(
        f"Vectors {name1}: "
        f"{len(v1)}"
    )

    print(
        f"Vectors {name2}: "
        f"{len(v2)}"
    )

    print(
        f"Shared: "
        f"{len(shared)}"
    )

    print(
        f"Exact: "
        f"{exact}"
    )

    print(
        f"Similarity: "
        f"{similarity:.2f}%"
    )

    print()
    print("Shared Vectors")

    for key in sorted(shared):

        mark = (
            "MATCH"
            if v1[key] == v2[key]
            else "DIFF "
        )

        print(
            f"{key} "
            f"{mark} "
            f"{v1[key]:02X} "
            f"{v2[key]:02X}"
        )

print()
print("Done.")
print()