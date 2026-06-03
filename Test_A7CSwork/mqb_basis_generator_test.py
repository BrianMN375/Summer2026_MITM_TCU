#!/usr/bin/env python3

# ============================================================
# MQB BASIS GENERATOR TEST
# ============================================================

A7_BYTE4 = [
    0xAD,
    0x75,
    0xEA,
    0xFB,
    0xD9,
    0x9D,
    0x15,
    0x2A,
]

A7_BYTE6 = [
    0x0E,
    0x1C,
    0x38,
    0x70,
    0xE0,
    0xEF,
    0xF1,
    0xCD,
]

A8_BYTE6_PARTIAL = [
    0x1C,
    0x38,
    0x70,
    0xE0,
    0xEF,
    0xF1,
    0xCD,
]

POLY = 0x2F

# ============================================================
# LFSR ADVANCE
# ============================================================

def advance(value, poly):

    msb = value & 0x80

    value = (value << 1) & 0xFF

    if msb:
        value ^= poly

    return value

# ============================================================
# VERIFY SEQUENCE
# ============================================================

def verify_sequence(name, seq, poly):

    print()
    print("=" * 70)
    print(name)
    print("=" * 70)

    current = seq[0]

    success = True

    for idx in range(1, len(seq)):

        predicted = advance(current, poly)

        expected = seq[idx]

        match = predicted == expected

        print(
            f"{idx-1}->{idx}   "
            f"current=0x{current:02X}   "
            f"pred=0x{predicted:02X}   "
            f"expected=0x{expected:02X}   "
            f"{'OK' if match else 'FAIL'}"
        )

        if not match:
            success = False

        current = expected

    print()

    if success:
        print("PASS")
    else:
        print("FAIL")

# ============================================================
# FIND SEED
# ============================================================

def generate_from_seed(seed, poly, count):

    result = [seed]

    current = seed

    for _ in range(count - 1):

        current = advance(current, poly)

        result.append(current)

    return result

# ============================================================
# SEARCH FOR BYTE4 SEED
# ============================================================

def search_seed(target, poly):

    for seed in range(256):

        generated = generate_from_seed(
            seed,
            poly,
            len(target)
        )

        if generated == target:

            return seed

    return None

# ============================================================
# TEST BYTE4
# ============================================================

verify_sequence(
    "A7 BYTE4",
    A7_BYTE4,
    POLY
)

# ============================================================
# TEST BYTE6
# ============================================================

verify_sequence(
    "A7 BYTE6",
    A7_BYTE6,
    POLY
)

# ============================================================
# SEED SEARCH
# ============================================================

print()
print("=" * 70)
print("SEED SEARCH")
print("=" * 70)

seed4 = search_seed(
    A7_BYTE4,
    POLY
)

seed6 = search_seed(
    A7_BYTE6,
    POLY
)

print()

if seed4 is None:
    print("Byte4 seed not found")
else:
    print(f"Byte4 seed = 0x{seed4:02X}")

if seed6 is None:
    print("Byte6 seed not found")
else:
    print(f"Byte6 seed = 0x{seed6:02X}")

# ============================================================
# BASIS TABLE
# ============================================================

print()
print("=" * 70)
print("FULL BASIS TABLE FROM BYTE6 SEED")
print("=" * 70)

if seed6 is not None:

    basis = generate_from_seed(
        seed6,
        POLY,
        32
    )

    for idx, value in enumerate(basis):

        print(
            f"{idx:02d} : 0x{value:02X}"
        )

# ============================================================
# CROSS-CHECK SHARED VECTORS
# ============================================================

print()
print("=" * 70)
print("A8 SHARED VECTOR CHECK")
print("=" * 70)

current = A8_BYTE6_PARTIAL[0]

for expected in A8_BYTE6_PARTIAL[1:]:

    predicted = advance(
        current,
        POLY
    )

    print(
        f"{current:02X} -> "
        f"{predicted:02X} "
        f"(expected {expected:02X})"
    )

    current = expected

print()
print("Done.")