#!/usr/bin/env python3

import csv
from collections import defaultdict

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

TARGET_ID = 0x0A8

POLYS = [
    0x07,
    0x1D,
    0x2F,
    0x31,
    0x9B,
    0xA6,
]

# ------------------------------------------------------------

def reflect8(x):

    y = 0

    for i in range(8):

        if x & (1 << i):
            y |= (1 << (7 - i))

    return y

# ------------------------------------------------------------

def crc8(data,
         poly,
         init,
         xorout,
         refin,
         refout):

    crc = init

    for b in data:

        if refin:
            b = reflect8(b)

        crc ^= b

        for _ in range(8):

            if crc & 0x80:
                crc = ((crc << 1) ^ poly) & 0xFF
            else:
                crc = (crc << 1) & 0xFF

    if refout:
        crc = reflect8(crc)

    crc ^= xorout

    return crc

# ------------------------------------------------------------

frames = []

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

            frames.append(frame)

        except:
            pass

print()
print(f"Loaded {len(frames):,} A8 frames")

# use unique payloads

unique_frames = {}

for frame in frames:

    unique_frames[
        frame[1:]
    ] = frame

frames = list(unique_frames.values())

print(
    f"Unique payloads: "
    f"{len(frames):,}"
)

# ------------------------------------------------------------

layouts = []

def raw(frame):
    return list(frame[1:])

def reverse(frame):
    return list(frame[1:])[::-1]

def counter_removed(frame):
    x = list(frame[1:])
    x[0] &= 0xF0
    return x

def state_removed(frame):
    x = list(frame[1:])
    x[0] &= 0x0F
    return x

def remove_b1(frame):
    return list(frame[2:])

layouts.append(("raw", raw))
layouts.append(("reverse", reverse))
layouts.append(("counter_removed", counter_removed))
layouts.append(("state_removed", state_removed))
layouts.append(("remove_b1", remove_b1))

# ------------------------------------------------------------

results = []

for poly in POLYS:

    for init in (0x00, 0xFF):

        for xorout in (0x00, 0xFF, 0x55):

            for refin in (False, True):

                for refout in (False, True):

                    for name, func in layouts:

                        matches = 0

                        for frame in frames:

                            crc = crc8(
                                func(frame),
                                poly,
                                init,
                                xorout,
                                refin,
                                refout
                            )

                            if crc == frame[0]:
                                matches += 1

                        acc = (
                            100.0 *
                            matches /
                            len(frames)
                        )

                        results.append(
                            (
                                acc,
                                poly,
                                init,
                                xorout,
                                refin,
                                refout,
                                name
                            )
                        )

results.sort(reverse=True)

print()
print("=" * 80)
print("TOP RESULTS")
print("=" * 80)

for r in results[:50]:

    (
        acc,
        poly,
        init,
        xorout,
        refin,
        refout,
        name
    ) = r

    print(
        f"{acc:8.4f}%  "
        f"poly=0x{poly:02X} "
        f"init=0x{init:02X} "
        f"xor=0x{xorout:02X} "
        f"rin={refin} "
        f"rout={refout} "
        f"{name}"
    )

print()
print("Done.")