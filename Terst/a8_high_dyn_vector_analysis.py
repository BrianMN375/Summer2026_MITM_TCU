#!/usr/bin/env python3

RESIDUALS = [
    0x25,   # NEG LOW
    0x5E,   # POS LOW

    0xF6,   # NEG MID
    0x8D,   # POS MID

    0xBC,   # NEG HIGH
    0xC7,   # POS HIGH

    0x1E,   # HIGH-DYN STAGE 1
    0xC0,   # TRANSITION?
    0x84,   # HIGH-DYN STAGE 2
    0x5D,   # HIGH-DYN STAGE 3
]

KNOWN_VECTORS = {
    0x7B : "NEG/POS",
    0xD3 : "MID",
    0x4A : "HIGH",
}

# ------------------------------------------------------------

print()
print("=" * 80)
print("A8 HIGH DYN VECTOR ANALYSIS")
print("=" * 80)

print()
print("KNOWN VECTORS")
print("-------------")

for vec, name in KNOWN_VECTORS.items():

    print(
        f"0x{vec:02X}  {name}"
    )

# ------------------------------------------------------------

print()
print("=" * 80)
print("PAIRWISE XOR RELATIONSHIPS")
print("=" * 80)

for i in range(len(RESIDUALS)):

    for j in range(i + 1, len(RESIDUALS)):

        a = RESIDUALS[i]
        b = RESIDUALS[j]

        delta = a ^ b

        tag = ""

        if delta in KNOWN_VECTORS:

            tag = f" <-- {KNOWN_VECTORS[delta]}"

        print(
            f"0x{a:02X} ^ 0x{b:02X} "
            f"= 0x{delta:02X}"
            f"{tag}"
        )

# ------------------------------------------------------------

print()
print("=" * 80)
print("HIGH DYN RELATIONSHIPS")
print("=" * 80)

high_dyn = [
    0x1E,
    0xC0,
    0x84,
    0x5D,
]

for i in range(len(high_dyn)):

    for j in range(i + 1, len(high_dyn)):

        a = high_dyn[i]
        b = high_dyn[j]

        delta = a ^ b

        print(
            f"0x{a:02X} ^ 0x{b:02X} "
            f"= 0x{delta:02X}"
        )

# ------------------------------------------------------------

print()
print("=" * 80)
print("DISTANCE FROM POS HIGH (0xC7)")
print("=" * 80)

for r in high_dyn:

    delta = 0xC7 ^ r

    tag = ""

    if delta in KNOWN_VECTORS:

        tag = f" <-- {KNOWN_VECTORS[delta]}"

    print(
        f"0xC7 ^ 0x{r:02X} "
        f"= 0x{delta:02X}"
        f"{tag}"
    )

# ------------------------------------------------------------

print()
print("=" * 80)
print("DISTANCE FROM NEG HIGH (0xBC)")
print("=" * 80)

for r in high_dyn:

    delta = 0xBC ^ r

    tag = ""

    if delta in KNOWN_VECTORS:

        tag = f" <-- {KNOWN_VECTORS[delta]}"

    print(
        f"0xBC ^ 0x{r:02X} "
        f"= 0x{delta:02X}"
        f"{tag}"
    )

print()
print("Done.")