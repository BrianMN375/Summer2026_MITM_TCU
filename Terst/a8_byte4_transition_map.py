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
    "a8_full_0007.csv"
)

COUNTER_TRANSFORM = [
    0x00,0x7A,0x96,0x79,
    0xD5,0xBF,0x8F,0x9A,
    0xB8,0xCA,0x95,0x58,
    0x46,0x55,0xE7,0x68
]

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

# ---------------------------------------------------------

def build_u64(frame):

    value = 0

    for i, b in enumerate(frame):
        value |= (b << (8 * i))

    return value

# ---------------------------------------------------------

def decode(frame):

    raw = build_u64(frame)

    rpm  = ((raw >> 48) & 0xFFFF) * 0.25
    stat = ((raw >> 21) & 0x1FF)
    dyn  = ((raw >> 30) & 0x3FF) - 509

    return rpm, stat, dyn

# ---------------------------------------------------------

def actual_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family = checksum ^ COUNTER_TRANSFORM[counter]

    linear = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    linear ^= vec

    return family ^ linear

# ---------------------------------------------------------

byte4_stats = defaultdict(lambda: {
    "count": 0,
    "rpm": 0,
    "stat": 0,
    "dyn": 0,
    "residuals": Counter()
})

frames_loaded = 0

with open(CSV_FILE, newline="") as csvfile:

    reader = csv.reader(csvfile)

    for row in reader:

        if not row:
            continue

        if row[0].lower() == "time":
            continue

        try:

            frame = [
                int(x.strip(),16)
                for x in row[1:9]
            ]

        except:
            continue

        residual = actual_residual(frame)

        rpm, stat, dyn = decode(frame)

        b4 = frame[4]

        s = byte4_stats[b4]

        s["count"] += 1
        s["rpm"] += rpm
        s["stat"] += stat
        s["dyn"] += dyn
        s["residuals"][residual] += 1

        frames_loaded += 1

# ---------------------------------------------------------

print()
print("=" * 90)
print("A8 BYTE4 TRANSITION MAP")
print("=" * 90)

print()
print(f"Frames Loaded: {frames_loaded:,}")

print()
print(
    f"{'Byte4':<8}"
    f"{'Frames':>8}  "
    f"{'DomRes':>8}  "
    f"{'Dom%':>7}  "
    f"{'RPM':>8}  "
    f"{'STAT':>8}  "
    f"{'DYN':>8}"
)

print("-" * 90)

for b4 in sorted(byte4_stats.keys()):

    s = byte4_stats[b4]

    count = s["count"]

    if count < 25:
        continue

    dom_res, dom_count = s["residuals"].most_common(1)[0]

    dom_pct = 100.0 * dom_count / count

    avg_rpm = s["rpm"] / count
    avg_stat = s["stat"] / count
    avg_dyn = s["dyn"] / count

    print(
        f"0x{b4:02X}    "
        f"{count:8d}  "
        f"0x{dom_res:02X}    "
        f"{dom_pct:6.1f}%  "
        f"{avg_rpm:8.0f}  "
        f"{avg_stat:8.1f}  "
        f"{avg_dyn:8.1f}"
    )

print()
print("Done.")