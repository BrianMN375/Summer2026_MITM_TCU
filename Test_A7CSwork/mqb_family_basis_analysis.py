#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict

# ============================================================
# INPUT FILES
# ============================================================

A7_CSV = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a7_unique.csv"
)

A8_CSV = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    "a8_unique.csv"
)

EXCLUDED_STATES = {
    0x1,
    0xA,
}

# ============================================================
# HELPERS
# ============================================================

def load_frames(csv_file):

    frames = []

    with open(csv_file, newline="") as f:

        reader = csv.DictReader(f)

        for row in reader:

            try:

                frame = tuple(
                    int(row[f"b{i}"], 16)
                    for i in range(8)
                )

                frames.append(frame)

            except Exception:
                pass

    return frames


def state_analysis(frames):

    by_upper = defaultdict(list)

    for frame in frames:

        upper = frame[1] >> 4
        checksum = frame[0]

        by_upper[upper].append(checksum)

    results = {}

    for upper, checksums in by_upper.items():

        hist = Counter(checksums)

        top_ck, top_count = hist.most_common(1)[0]

        pct = (
            100.0 *
            top_count /
            len(checksums)
        )

        results[upper] = {
            "frames": len(checksums),
            "unique_checksums": len(hist),
            "top_checksum": top_ck,
            "top_pct": pct,
        }

    return results


def discover_vectors(frames):

    filtered = []

    for frame in frames:

        upper = frame[1] >> 4

        if upper in EXCLUDED_STATES:
            continue

        filtered.append(frame)

    payload_map = {}

    for frame in filtered:

        payload_map[
            frame[1:]
        ] = frame[0]

    bit_vectors = defaultdict(Counter)

    for payload, checksum in payload_map.items():

        payload_list = list(payload)

        for byte_idx in range(7):

            for bit_idx in range(8):

                neighbor = payload_list.copy()

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

    vectors = {}

    for bit, hist in bit_vectors.items():

        total = sum(hist.values())

        delta, count = (
            hist.most_common(1)[0]
        )

        confidence = (
            100.0 *
            count /
            total
        )

        if (
            confidence >= 85.0
            and total >= 20
            and delta != 0
        ):
            vectors[bit] = (
                delta,
                confidence,
                total
            )

    return vectors


def lfsr_search(sequence):

    if len(sequence) < 2:
        return None

    start = sequence[0]

    for poly in range(256):

        current = start

        ok = True

        for expected in sequence[1:]:

            msb = current & 0x80

            current = (
                current << 1
            ) & 0xFF

            if msb:
                current ^= poly

            if current != expected:

                ok = False
                break

        if ok:
            return poly

    return None


def extract_byte_vectors(vectors, byte_idx):

    result = []

    for bit in range(8):

        entry = vectors.get(
            (byte_idx, bit)
        )

        if entry is None:
            return None

        result.append(
            entry[0]
        )

    return result


# ============================================================
# MAIN
# ============================================================

print()
print("=" * 80)
print("MQB FAMILY BASIS ANALYSIS")
print("=" * 80)

# ------------------------------------------------------------
# Load
# ------------------------------------------------------------

a7_frames = load_frames(A7_CSV)
a8_frames = load_frames(A8_CSV)

print()
print(f"A7 Frames: {len(a7_frames):,}")
print(f"A8 Frames: {len(a8_frames):,}")

# ------------------------------------------------------------
# State Analysis
# ------------------------------------------------------------

print()
print("=" * 80)
print("STATE ANALYSIS")
print("=" * 80)

a7_states = state_analysis(a7_frames)
a8_states = state_analysis(a8_frames)

for name, states in [

    ("A7", a7_states),
    ("A8", a8_states),

]:

    print()
    print(name)
    print("-" * 40)

    for upper in sorted(states):

        s = states[upper]

        print(
            f"0x{upper:X}  "
            f"Frames={s['frames']:>7,d}  "
            f"UniqueCK={s['unique_checksums']:>4,d}  "
            f"Top=0x{s['top_checksum']:02X}  "
            f"{s['top_pct']:6.2f}%"
        )

# ------------------------------------------------------------
# Vector Discovery
# ------------------------------------------------------------

print()
print("=" * 80)
print("VECTOR DISCOVERY")
print("=" * 80)

a7_vectors = discover_vectors(a7_frames)
a8_vectors = discover_vectors(a8_frames)

print()
print(f"A7 vectors: {len(a7_vectors)}")
print(f"A8 vectors: {len(a8_vectors)}")

# ------------------------------------------------------------
# Shared Locations
# ------------------------------------------------------------

shared = set(
    a7_vectors.keys()
).intersection(
    a8_vectors.keys()
)

print()
print("=" * 80)
print("SHARED VECTOR LOCATIONS")
print("=" * 80)

for key in sorted(shared):

    a7_val = a7_vectors[key][0]
    a8_val = a8_vectors[key][0]

    print(
        f"{key}  "
        f"A7=0x{a7_val:02X}  "
        f"A8=0x{a8_val:02X}"
    )

# ------------------------------------------------------------
# Similarity
# ------------------------------------------------------------

exact_matches = 0

for key in shared:

    if (
        a7_vectors[key][0]
        ==
        a8_vectors[key][0]
    ):
        exact_matches += 1

print()
print("=" * 80)
print("SIMILARITY")
print("=" * 80)

print(f"Shared locations : {len(shared)}")
print(f"Exact matches    : {exact_matches}")

if shared:

    similarity = (
        100.0 *
        exact_matches /
        len(shared)
    )

    print(
        f"Similarity       : "
        f"{similarity:.2f}%"
    )

# ------------------------------------------------------------
# LFSR Search
# ------------------------------------------------------------

print()
print("=" * 80)
print("LFSR SEARCH")
print("=" * 80)

for name, vectors in [

    ("A7", a7_vectors),
    ("A8", a8_vectors),

]:

    print()
    print(name)
    print("-" * 40)

    for byte_idx in [4, 5, 6]:

        seq = extract_byte_vectors(
            vectors,
            byte_idx
        )

        if seq is None:

            print(
                f"Byte {byte_idx}: incomplete"
            )

            continue

        poly = lfsr_search(seq)

        if poly is None:

            print(
                f"Byte {byte_idx}: no recurrence"
            )

        else:

            print(
                f"Byte {byte_idx}: poly=0x{poly:02X}"
            )

print()
print("Done.")
print()