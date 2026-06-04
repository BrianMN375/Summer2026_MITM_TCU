#!/usr/bin/env python3

from collections import defaultdict

# ============================================================
# RESIDUALS DISCOVERED SO FAR
# ============================================================

BASE_POS = {
    "LOW":  0x5E,
    "MID":  0x8D,
    "HIGH": 0xC7,
}

BASE_NEG = {
    "LOW":  0x25,
    "MID":  0xF6,
    "HIGH": 0xBC,
}

OVERLAYS = [
    0x1E,
    0xC0,
    0x19,
    0x56,
    0x1C,
    0x62,
    0x72,
    0x38,
    0x43,
    0x5D,
    0x84,
    0x07,
    0x4D,
]

# ============================================================

print()
print("=" * 80)
print("A8 STATE VECTOR SOLVER")
print("=" * 80)

# ============================================================
# TORQUE VECTOR
# ============================================================

print()
print("TORQUE VECTOR")
print("-" * 80)

torque_vectors = []

for band in ("LOW", "MID", "HIGH"):

    vec = (
        BASE_POS[band]
        ^
        BASE_NEG[band]
    )

    torque_vectors.append(vec)

    print(
        f"{band:5s}: "
        f"0x{BASE_POS[band]:02X} "
        f"^ "
        f"0x{BASE_NEG[band]:02X} "
        f"= "
        f"0x{vec:02X}"
    )

if len(set(torque_vectors)) == 1:

    NEG_TORQUE = torque_vectors[0]

    print()
    print(
        f"NEGATIVE_TORQUE_VECTOR = "
        f"0x{NEG_TORQUE:02X}"
    )

else:

    print()
    print("Torque vector NOT constant")

# ============================================================
# RPM BAND VECTORS
# ============================================================

print()
print("=" * 80)
print("RPM BAND VECTORS")
print("=" * 80)

rpm1 = (
    BASE_POS["LOW"]
    ^
    BASE_POS["MID"]
)

rpm2 = (
    BASE_POS["MID"]
    ^
    BASE_POS["HIGH"]
)

print(
    f"LOW→MID  : 0x{rpm1:02X}"
)

print(
    f"MID→HIGH : 0x{rpm2:02X}"
)

if rpm1 == rpm2:

    print()
    print(
        f"Single RPM vector = "
        f"0x{rpm1:02X}"
    )

else:

    print()
    print(
        "Multiple RPM vectors detected"
    )

# ============================================================
# OVERLAY VECTOR DISCOVERY
# ============================================================

print()
print("=" * 80)
print("OVERLAY VECTOR DISCOVERY")
print("=" * 80)

overlay_hits = defaultdict(int)

for overlay in OVERLAYS:

    for base_name, base in BASE_POS.items():

        vec = overlay ^ base

        overlay_hits[vec] += 1

for vec, count in sorted(
        overlay_hits.items(),
        key=lambda x: x[1],
        reverse=True):

    if count >= 2:

        print(
            f"VECTOR 0x{vec:02X} "
            f"appears {count} times"
        )

# ============================================================
# EXPLICIT OVERLAY RELATIONSHIPS
# ============================================================

print()
print("=" * 80)
print("KNOWN OVERLAY RELATIONSHIPS")
print("=" * 80)

pairs = [

    ("0x1E vs 0xC0",
     0x1E,
     0xC0),

    ("0xC7 vs 0x19",
     0xC7,
     0x19),

    ("0xC7 vs 0x56",
     0xC7,
     0x56),

    ("0x8D vs 0x1C",
     0x8D,
     0x1C),

    ("0xC7 vs 0x1C",
     0xC7,
     0x1C),

    ("0x38 vs 0x62",
     0x38,
     0x62),

    ("0x5D vs 0x07",
     0x5D,
     0x07),

]

for name, a, b in pairs:

    print(
        f"{name:18s} "
        f"0x{a ^ b:02X}"
    )

# ============================================================
# CAN WE BUILD STATES FROM VECTORS?
# ============================================================

print()
print("=" * 80)
print("VECTOR COMPOSITION SEARCH")
print("=" * 80)

vectors = [

    0x7B,  # torque

    0xD3,  # rpm low-mid
    0x4A,  # rpm mid-high

    0xDE,
    0x91,
    0xDB,
    0x5A,
    0xCE,
    0x10,
]

known = set(
    list(BASE_POS.values())
    +
    list(BASE_NEG.values())
    +
    OVERLAYS
)

for base_name, base in BASE_POS.items():

    print()
    print(
        f"BASE {base_name} "
        f"(0x{base:02X})"
    )

    for vec in vectors:

        candidate = base ^ vec

        if candidate in known:

            print(
                f"  ^0x{vec:02X}"
                f" -> 0x{candidate:02X}"
            )

print()
print("Done.")