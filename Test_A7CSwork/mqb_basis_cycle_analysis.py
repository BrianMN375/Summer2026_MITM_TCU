#!/usr/bin/env python3

# ============================================================
# MQB BASIS CYCLE ANALYSIS
# ============================================================

POLY = 0x2F
SEED = 0x0E

# ============================================================
# DISCOVERED VECTORS
# ============================================================

DISCOVERED = {

    # Byte2
    "b2[5]": 0x9B,

    # Byte3
    "b3[6]": 0x3D,
    "b3[7]": 0x7A,

    # Byte4
    "b4[0]": 0xAD,
    "b4[1]": 0x75,
    "b4[2]": 0xEA,
    "b4[3]": 0xFB,
    "b4[4]": 0xD9,
    "b4[5]": 0x9D,
    "b4[6]": 0x15,
    "b4[7]": 0x2A,

    # Byte5
    "b5[1]": 0x45,
    "b5[2]": 0x8A,
    "b5[3]": 0x3B,
    "b5[4]": 0x76,
    "b5[5]": 0xEC,
    "b5[6]": 0xF7,
    "b5[7]": 0xC1,

    # Byte6
    "b6[0]": 0x0E,
    "b6[1]": 0x1C,
    "b6[2]": 0x38,
    "b6[3]": 0x70,
    "b6[4]": 0xE0,
    "b6[5]": 0xEF,
    "b6[6]": 0xF1,
    "b6[7]": 0xCD,

    # Byte7
    "b7[0]": 0xE9,

    # Byte1
    "b1[4]": 0xA2,
    "b1[5]": 0x6B,
    "b1[6]": 0xD6,
    "b1[7]": 0x83,
}

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
# BUILD FULL CYCLE
# ============================================================

cycle = []
seen = {}

current = SEED
index = 0

while current not in seen:

    seen[current] = index

    cycle.append(current)

    current = advance(current, POLY)

    index += 1

print()
print("=" * 70)
print("MQB BASIS CYCLE")
print("=" * 70)

print(f"Polynomial : 0x{POLY:02X}")
print(f"Seed       : 0x{SEED:02X}")
print(f"Cycle Len  : {len(cycle)}")

# ============================================================
# PRINT CYCLE
# ============================================================

print()
print("=" * 70)
print("FULL CYCLE")
print("=" * 70)

for idx, value in enumerate(cycle):

    print(
        f"{idx:03d} : 0x{value:02X}"
    )

# ============================================================
# VALUE -> POSITION
# ============================================================

value_to_pos = {
    value: idx
    for idx, value in enumerate(cycle)
}

# ============================================================
# DISCOVERED VECTOR POSITIONS
# ============================================================

print()
print("=" * 70)
print("DISCOVERED VECTOR POSITIONS")
print("=" * 70)

positions = []

for name, value in sorted(
    DISCOVERED.items()
):

    pos = value_to_pos.get(value)

    if pos is None:

        print(
            f"{name:8s} "
            f"0x{value:02X} "
            f"NOT FOUND"
        )

        continue

    positions.append(
        (pos, name, value)
    )

    print(
        f"{name:8s} "
        f"0x{value:02X} "
        f"basis[{pos}]"
    )

# ============================================================
# SORT BY BASIS POSITION
# ============================================================

print()
print("=" * 70)
print("SORTED BY BASIS POSITION")
print("=" * 70)

positions.sort()

for pos, name, value in positions:

    print(
        f"basis[{pos:03d}] "
        f"0x{value:02X} "
        f"{name}"
    )

# ============================================================
# LOOK FOR CONTIGUOUS REGIONS
# ============================================================

print()
print("=" * 70)
print("CONTIGUOUS REGIONS")
print("=" * 70)

for i in range(len(positions)-1):

    pos1 = positions[i][0]
    pos2 = positions[i+1][0]

    if pos2 == pos1 + 1:

        print(
            f"basis[{pos1}] -> basis[{pos2}]"
        )

# ============================================================
# PREDICT MISSING STATES
# ============================================================

print()
print("=" * 70)
print("POTENTIAL FUTURE VECTORS")
print("=" * 70)

for pos, name, value in positions:

    next_pos = (pos + 1) % len(cycle)

    print(
        f"{name:8s} "
        f"0x{value:02X} "
        f"next -> "
        f"basis[{next_pos}] "
        f"0x{cycle[next_pos]:02X}"
    )

print()
print("Done.")
print()