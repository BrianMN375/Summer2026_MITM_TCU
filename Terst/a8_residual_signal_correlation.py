#!/usr/bin/env python3

import csv
from collections import defaultdict

# ============================================================
# Counter Transform
# ============================================================

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

# ============================================================
# Recovered Columns
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

def get_bits(data_bytes):

    value = 0

    for i, b in enumerate(data_bytes):
        value |= (b << (8 * i))

    return value

# ============================================================
# DBC Signal Decoding
# ============================================================

def decode_signals(frame):

    # frame[0] = CRC
    # frame[1] = counter

    raw = get_bits(frame)

    neg_tq = ((raw >> 12) & 0x1FF) - 509

    tq_limit_stat = (
        (raw >> 21) & 0x1FF
    )

    tq_limit_dyn = (
        ((raw >> 30) & 0x3FF) - 509
    )

    tq_percent = (
        (raw >> 40) & 0x7F
    )

    rpm = (
        ((raw >> 48) & 0xFFFF)
        * 0.25
    )

    return (
        neg_tq,
        tq_limit_stat,
        tq_limit_dyn,
        tq_percent,
        rpm
    )

# ============================================================

def calc_residual(frame):

    checksum = frame[0]
    counter  = frame[1] & 0x0F

    family_value = (
        checksum ^
        COUNTER_TRANSFORM[counter]
    )

    predicted = 0

    for byte_idx in range(2,8):

        value = frame[byte_idx]

        for bit_idx in range(8):

            if value & (1 << bit_idx):

                vec = KNOWN_VECTORS.get(
                    (byte_idx, bit_idx)
                )

                if vec is not None:
                    predicted ^= vec

    return family_value ^ predicted

# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    groups = defaultdict(list)

    total_frames = 0

    with open(filename, newline='') as csvfile:

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

            residual = calc_residual(frame)

            signals = decode_signals(frame)

            groups[residual].append(
                signals
            )

            total_frames += 1

    print()
    print("======================================")
    print("A8 RESIDUAL SIGNAL CORRELATION")
    print("======================================")
    print()

    print(
        f"Frames: {total_frames:,}"
    )

    print()

    print(
        "Residual Summary"
    )

    print(
        "----------------"
    )

    print(
        f"{'Residual':>8} "
        f"{'Frames':>8} "
        f"{'RPM':>8} "
        f"{'TqPct':>8} "
        f"{'Stat':>8} "
        f"{'Dyn':>8} "
        f"{'Neg':>8}"
    )

    for residual, values in sorted(
        groups.items(),
        key=lambda x: len(x[1]),
        reverse=True
    ):

        count = len(values)

        avg_neg = sum(v[0] for v in values) / count
        avg_stat = sum(v[1] for v in values) / count
        avg_dyn = sum(v[2] for v in values) / count
        avg_pct = sum(v[3] for v in values) / count
        avg_rpm = sum(v[4] for v in values) / count

        print(

            f"0x{residual:02X} "
            f"{count:8d} "
            f"{avg_rpm:8.0f} "
            f"{avg_pct:8.1f} "
            f"{avg_stat:8.1f} "
            f"{avg_dyn:8.1f} "
            f"{avg_neg:8.1f}"
        )

    print()
    print("Done.")

# ============================================================

if __name__ == "__main__":
    main()