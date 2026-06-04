#!/usr/bin/env python3

import csv
from itertools import product

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
# CRC Parameters
# ============================================================

POLYS = [0x1D, 0x2F, 0x07, 0x31, 0x9B]

INITS = [0x00, 0xFF, 0xA5, 0xC3, 0x55]

XOROUTS = [0x00, 0x55, 0xFF]

REFLECTS = [False, True]

# ============================================================
# Reflect
# ============================================================

def reflect8(x):

    r = 0

    for _ in range(8):

        r = (r << 1) | (x & 1)

        x >>= 1

    return r

# ============================================================
# CRC8
# ============================================================

def crc8(data, poly, init, xorout, reflect):

    crc = init

    for b in data:

        if reflect:
            b = reflect8(b)

        crc ^= b

        for _ in range(8):

            if crc & 0x80:
                crc = ((crc << 1) ^ poly) & 0xFF
            else:
                crc = (crc << 1) & 0xFF

    if reflect:
        crc = reflect8(crc)

    return crc ^ xorout

# ============================================================
# Main
# ============================================================

def main():

    filename = input(
        "A8 CSV file: "
    ).strip()

    unique_families = {}

    # --------------------------------------------------------

    with open(filename, newline='') as csvfile:

        reader = csv.reader(csvfile)

        for row in reader:

            if not row:
                continue

            if row[0].lower() == "time":
                continue

            try:

                frame = [
                    int(x.strip(), 16)
                    for x in row[1:9]
                ]

            except:
                continue

            checksum = frame[0]

            counter = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            # Deduplicate on payload

            family_key = tuple(frame[2:8])

            if family_key not in unique_families:

                unique_families[family_key] = (
                    frame,
                    family_value
                )

    # --------------------------------------------------------

    samples = list(
        unique_families.values()
    )

    print()
    print(
        f"Unique families analyzed: "
        f"{len(samples):,}"
    )
    print()

    # --------------------------------------------------------
    # Payload extraction modes
    # --------------------------------------------------------

    payload_modes = {

        "b2-b7":
            lambda f: f[2:8],

        "b2-b4-b6-b7":
            lambda f: [
                f[2], f[3], f[4],
                f[6], f[7]
            ],

        "b3-b7":
            lambda f: f[3:8],

        "b3-b4-b6-b7":
            lambda f: [
                f[3], f[4],
                f[6], f[7]
            ],

        "reverse_b2-b7":
            lambda f: list(
                reversed(f[2:8])
            ),

        "reverse_b2-b4-b6-b7":
            lambda f: list(
                reversed([
                    f[2], f[3], f[4],
                    f[6], f[7]
                ])
            )
    }

    # --------------------------------------------------------

    best = []

    total_tests = (
        len(payload_modes)
        * len(POLYS)
        * len(INITS)
        * len(XOROUTS)
        * len(REFLECTS)
    )

    print(
        f"Running {total_tests} CRC candidates..."
    )

    # --------------------------------------------------------

    for mode_name, mode_func in payload_modes.items():

        for poly, init, xorout, reflect in product(
            POLYS,
            INITS,
            XOROUTS,
            REFLECTS
        ):

            matches = 0
            xor_sum = 0

            # Use first 10k unique families
            # for speed

            for frame, family_value in samples[:10000]:

                payload = mode_func(frame)

                crc = crc8(
                    payload,
                    poly,
                    init,
                    xorout,
                    reflect
                )

                if crc == family_value:
                    matches += 1

                xor_sum += (
                    crc ^ family_value
                )

            avg_xor_error = (
                xor_sum / 10000.0
            )

            best.append({

                "matches":
                    matches,

                "avg_xor":
                    avg_xor_error,

                "mode":
                    mode_name,

                "poly":
                    poly,

                "init":
                    init,

                "xorout":
                    xorout,

                "reflect":
                    reflect
            })

    # ========================================================
    # REPORT
    # ========================================================

    print()
    print(
        "===================================="
    )
    print(
        "TOP EXACT MATCH CANDIDATES"
    )
    print(
        "===================================="
    )
    print()

    by_matches = sorted(
        best,
        key=lambda x: x["matches"],
        reverse=True
    )

    for r in by_matches[:20]:

        print(
            f"Matches={r['matches']:4d} "
            f"Mode={r['mode']:20s} "
            f"Poly=0x{r['poly']:02X} "
            f"Init=0x{r['init']:02X} "
            f"Xor=0x{r['xorout']:02X} "
            f"Ref={r['reflect']}"
        )

    print()
    print(
        "===================================="
    )
    print(
        "LOWEST XOR ERROR"
    )
    print(
        "===================================="
    )
    print()

    by_xor = sorted(
        best,
        key=lambda x: x["avg_xor"]
    )

    for r in by_xor[:20]:

        print(
            f"AvgXOR={r['avg_xor']:7.2f} "
            f"Mode={r['mode']:20s} "
            f"Poly=0x{r['poly']:02X} "
            f"Init=0x{r['init']:02X} "
            f"Xor=0x{r['xorout']:02X} "
            f"Ref={r['reflect']}"
        )

# ============================================================

if __name__ == "__main__":
    main()