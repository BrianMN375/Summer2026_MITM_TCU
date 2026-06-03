#!/usr/bin/env python3

from collections import Counter

# Major residuals discovered so far
RESIDUALS = [
    0xC7,
    0x8D,
    0x5E,
    0xBC,
    0x25,
    0xF6,
    0x1E,
    0xC0,
    0x38,
    0x43,
    0x5D,
    0x84,
    0x56,
    0x1C,
    0x72,
    0x07,
    0x62,
    0x19,
    0x4D,
]

print()
print("=" * 80)
print("A8 RESIDUAL FACTORIZER")
print("=" * 80)

print()
print("Pairwise XOR Matrix")
print("-" * 80)

xor_counts = Counter()

for i, a in enumerate(RESIDUALS):

    for b in RESIDUALS[i + 1:]:

        x = a ^ b

        xor_counts[x] += 1

        print(
            f"0x{a:02X} ^ 0x{b:02X} = 0x{x:02X}"
        )

print()
print("=" * 80)
print("MOST COMMON XOR DELTAS")
print("=" * 80)

for delta, count in xor_counts.most_common(50):

    if count < 2:
        continue

    print(
        f"0x{delta:02X}   {count}"
    )

print()
print("=" * 80)
print("RPM BAND HYPOTHESIS")
print("=" * 80)

pairs = [
    ("LowPos→MidPos", 0x5E, 0x8D),
    ("MidPos→HighPos", 0x8D, 0xC7),
    ("LowNeg→MidNeg", 0x25, 0xF6),
    ("MidNeg→HighNeg", 0xF6, 0xBC),
]

for name, a, b in pairs:

    print(
        f"{name:20s} "
        f"0x{a:02X} ^ 0x{b:02X} "
        f"= 0x{a ^ b:02X}"
    )

print()
print("=" * 80)
print("TORQUE SIGN HYPOTHESIS")
print("=" * 80)

pairs = [
    ("Low RPM", 0x5E, 0x25),
    ("Mid RPM", 0x8D, 0xF6),
    ("High RPM", 0xC7, 0xBC),
]

for name, a, b in pairs:

    print(
        f"{name:12s} "
        f"0x{a:02X} ^ 0x{b:02X} "
        f"= 0x{a ^ b:02X}"
    )

print()
print("=" * 80)
print("COMPOSITION SEARCH")
print("=" * 80)

for a in RESIDUALS:

    for b in RESIDUALS:

        if a == b:
            continue

        for c in RESIDUALS:

            if c in (a, b):
                continue

            if (a ^ b) == c:

                print(
                    f"0x{a:02X} ^ 0x{b:02X} = 0x{c:02X}"
                )

print()
print("Done.")