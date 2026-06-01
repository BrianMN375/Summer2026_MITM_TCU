#!/usr/bin/env python3

import csv
from collections import defaultdict

COUNTER_TRANSFORM = [
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
]

POLYS = [0x1D,0x2F,0x07,0x31,0x9B]
INITS = [0x00,0xFF,0xA5,0xC3,0x55]
XOROUTS = [0x00,0x55,0xFF]
REFLECTS = [False,True]

def reflect8(x):
    r = 0
    for _ in range(8):
        r = (r << 1) | (x & 1)
        x >>= 1
    return r

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

def main():

    filename = input("A8 CSV file: ").strip()

    samples = []

    with open(filename,newline='') as f:

        reader = csv.reader(f)

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

            checksum = frame[0]
            counter = frame[1] & 0x0F

            family_value = (
                checksum ^
                COUNTER_TRANSFORM[counter]
            )

            payload = frame[2:8]

            samples.append(
                (payload,family_value)
            )

    print()
    print(f"Loaded {len(samples):,} samples")
    print()

    best = []

    for poly in POLYS:
        for init in INITS:
            for xorout in XOROUTS:
                for reflect in REFLECTS:

                    matches = 0

                    # Use first 5000 samples
                    for payload, fv in samples[:5000]:

                        if crc8(
                            payload,
                            poly,
                            init,
                            xorout,
                            reflect
                        ) == fv:

                            matches += 1

                    best.append(
                        (
                            matches,
                            poly,
                            init,
                            xorout,
                            reflect
                        )
                    )

    best.sort(reverse=True)

    print("Top CRC Candidates")
    print("------------------")

    for item in best[:20]:

        matches, poly, init, xorout, reflect = item

        print(
            f"Matches={matches:4d} "
            f"Poly=0x{poly:02X} "
            f"Init=0x{init:02X} "
            f"Xor=0x{xorout:02X} "
            f"Reflect={reflect}"
        )

if __name__ == "__main__":
    main()