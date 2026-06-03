#!/usr/bin/env python3

from itertools import product

# ------------------------------------------------------------
# Recovered vectors
# ------------------------------------------------------------

VECTORS = {

    "b4": [
        0xAD,
        0x75,
        0xEA,
        0xFB,
        0xD9,
        0x9D,
        0x15,
        0x2A,
    ],

    "b5": [
        0x00,   # unknown
        0x45,
        0x8A,
        0x3B,
        0x76,
        0xEC,
        0xF7,
        0xC1,
    ],

    "b6": [
        0x0E,
        0x1C,
        0x38,
        0x70,
        0xE0,
        0xEF,
        0xF1,
        0xCD,
    ],
}

# ------------------------------------------------------------
# Helpers
# ------------------------------------------------------------

def rol8(v):
    return ((v << 1) | (v >> 7)) & 0xFF

def ror8(v):
    return ((v >> 1) | ((v & 1) << 7)) & 0xFF

# ------------------------------------------------------------
# Analyze
# ------------------------------------------------------------

for name, vecs in VECTORS.items():

    print()
    print("=" * 70)
    print(name)
    print("=" * 70)

    print()

    for i in range(len(vecs)-1):

        a = vecs[i]
        b = vecs[i+1]

        print(
            f"{i}->{i+1}   "
            f"{a:02X} -> {b:02X}"
        )

        print(
            f"  XOR      : "
            f"{a ^ b:02X}"
        )

        print(
            f"  ADD      : "
            f"{(a + a) & 0xFF:02X}"
        )

        print(
            f"  ROL1     : "
            f"{rol8(a):02X}"
        )

        print(
            f"  ROR1     : "
            f"{ror8(a):02X}"
        )

        print()

# ------------------------------------------------------------
# Search simple LFSR-style rules
# ------------------------------------------------------------

print()
print("=" * 70)
print("LFSR SEARCH")
print("=" * 70)

for poly in range(256):

    ok = True

    current = 0x0E

    expected = VECTORS["b6"]

    for idx in range(1, len(expected)):

        msb = current & 0x80

        current = (current << 1) & 0xFF

        if msb:
            current ^= poly

        if current != expected[idx]:

            ok = False
            break

    if ok:

        print(
            f"Found LFSR polynomial "
            f"0x{poly:02X}"
        )

# ------------------------------------------------------------
# XOR Matrix
# ------------------------------------------------------------

for name, vecs in VECTORS.items():

    print()
    print("=" * 70)
    print(f"{name} XOR Matrix")
    print("=" * 70)

    print("     ", end="")

    for i in range(len(vecs)):
        print(f"{i:>4}", end="")

    print()

    for i, a in enumerate(vecs):

        print(f"{i:>4} ", end="")

        for b in vecs:

            print(
                f"{a ^ b:04X}",
                end=""
            )

        print()

print()
print("Done.")