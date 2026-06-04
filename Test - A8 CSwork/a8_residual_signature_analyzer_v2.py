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
    "a8_full_0012.csv"
)

# ============================================================
# COUNTER TRANSFORM
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# KNOWN VECTORS
# ============================================================

KNOWN_VECTORS = {

    (2,5):0x9B,
    (2,6):0x19,
    (2,7):0x32,

    (3,0):0x54,
    (3,1):0xA8,
    (3,2):0x7F,
    (3,3):0xFE,
    (3,6):0x3D,
    (3,7):0x7A,

    (4,0):0xAD,
    (4,1):0x75,
    (4,2):0xEA,
    (4,3):0xFB,

    (6,1):0x1C,
    (6,2):0x38,
    (6,3):0x70,
    (6,4):0xE0,
    (6,5):0xEF,
    (6,6):0xF1,
    (6,7):0xCD,

    (7,0):0xE9,
    (7,1):0xFD,
    (7,2):0xD5,
    (7,3):0x85,
    (7,4):0x25,
}

# ============================================================
# HELPERS
# ============================================================

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value


def decode_signals(frame):

    raw = build_u64(frame)

    rpm = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn


def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    linear = 0

    for byte_idx in range(2, 8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear


def classify_signature(frame):

    b4 = frame[4]
    b5 = frame[5]
    b6 = frame[6]
    b7 = frame[7]

    if (
        b4 == 0xFF and
        b5 == 0x00 and
        b6 == 0x00 and
        b7 == 0x00
    ):
        return "STARTUP"

    if b5 == 0x64:
        return "TORQUE_MODEL"

    if b5 in (
        0x5C,
        0x5D,
        0x5E,
        0x5F
    ):
        return "OVERLAY"

    return "UNKNOWN"

# ============================================================
# LOAD
# ============================================================

print()
print("Loading dataset...")

residual_rows = defaultdict(list)
transition_counts = Counter()
signature_counts = Counter()

previous_residual = None

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for row in reader:

        try:

            frame = [
                int(x, 16)
                for x in row[1:9]
            ]

        except:
            continue

        residual = actual_residual(frame)

        rpm, stat, dyn = decode_signals(frame)

        sig = classify_signature(frame)

        signature_counts[(residual, sig)] += 1

        residual_rows[residual].append({

            "frame": frame,
            "rpm": rpm,
            "stat": stat,
            "dyn": dyn,
            "sig": sig

        })

        if previous_residual is not None:

            transition_counts[
                (previous_residual, residual)
            ] += 1

        previous_residual = residual

# ============================================================
# RESIDUAL DISTRIBUTION
# ============================================================

print()
print("=" * 80)
print("RESIDUAL DISTRIBUTION")
print("=" * 80)

total_frames = sum(
    len(v)
    for v in residual_rows.values()
)

for residual, rows in sorted(
        residual_rows.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    count = len(rows)

    pct = (
        count * 100.0 /
        total_frames
    )

    print(
        f"0x{residual:02X} "
        f"{count:8d} "
        f"({pct:6.2f}%)"
    )

# ============================================================
# PER RESIDUAL
# ============================================================

for residual, rows in sorted(
        residual_rows.items(),
        key=lambda x: len(x[1]),
        reverse=True):

    print()
    print("=" * 80)
    print(
        f"RESIDUAL 0x{residual:02X}"
    )
    print("=" * 80)

    rpms = [r["rpm"] for r in rows]
    stats = [r["stat"] for r in rows]
    dyns = [r["dyn"] for r in rows]

    print()
    print(
        f"Frames : {len(rows):,}"
    )

    print(
        f"RPM  : "
        f"{min(rpms):.1f}..{max(rpms):.1f} "
        f"avg={sum(rpms)/len(rpms):.1f}"
    )

    print(
        f"STAT : "
        f"{min(stats)}..{max(stats)} "
        f"avg={sum(stats)/len(stats):.1f}"
    )

    print(
        f"DYN  : "
        f"{min(dyns)}..{max(dyns)} "
        f"avg={sum(dyns)/len(dyns):.1f}"
    )

    for idx, label in [
        (4, "BYTE4"),
        (5, "BYTE5"),
        (6, "BYTE6"),
        (7, "BYTE7")
    ]:

        cnt = Counter(
            r["frame"][idx]
            for r in rows
        )

        print()
        print(label)
        print("-" * 20)

        for value, count in cnt.most_common(10):

            pct = (
                count * 100.0 /
                len(rows)
            )

            print(
                f"0x{value:02X} "
                f"{count:6d} "
                f"({pct:5.1f}%)"
            )

    sig_counter = Counter(
        r["sig"]
        for r in rows
    )

    print()
    print("SIGNATURE TYPES")
    print("-" * 20)

    for sig, count in sig_counter.most_common():

        pct = (
            count * 100.0 /
            len(rows)
        )

        print(
            f"{sig:15s} "
            f"{count:6d} "
            f"({pct:5.1f}%)"
        )

# ============================================================
# TRANSITIONS
# ============================================================

print()
print("=" * 80)
print("TOP RESIDUAL TRANSITIONS")
print("=" * 80)

for (
    (src, dst),
    count
) in transition_counts.most_common(50):

    print(
        f"0x{src:02X}"
        f" -> "
        f"0x{dst:02X}"
        f"   {count}"
    )

# ============================================================
# SIGNATURE SUMMARY
# ============================================================

print()
print("=" * 80)
print("RESIDUAL / SIGNATURE SUMMARY")
print("=" * 80)

for (
    residual,
    sig
), count in sorted(
        signature_counts.items(),
        key=lambda x: x[1],
        reverse=True):

    print(
        f"0x{residual:02X} "
        f"{sig:15s} "
        f"{count}"
    )

print()
print("Done.")