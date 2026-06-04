#!/usr/bin/env python3

from collections import defaultdict

# ============================================================
# OBSERVED RESIDUALS
# ============================================================

RESIDUALS = sorted({

    0x5E,
    0x25,

    0x8D,
    0xF6,

    0xC7,
    0xBC,

    0x1E,
    0xC0,
    0x19,

    0x56,
    0x1C,

    0x38,
    0x62,

    0x43,
    0x5D,

    0x84,
    0x07,

    0x72,
    0x4D
})

# ============================================================
# KNOWN VECTORS
# ============================================================

BASE = 0x5E

RPM_MID  = 0xD3
RPM_HIGH = 0x99

NEG_TORQUE = 0x7B

OVERLAYS = [
    0xDE,
    0x91,
    0xDB,
    0x5A,
]

# ============================================================

print()
print("=" * 80)
print("A8 VECTOR DECOMPOSER")
print("=" * 80)

# ============================================================
# BUILD SYNTHETIC SPACE
# ============================================================

generated = {}

for rpm_name, rpm_vec in [

    ("LOW", 0x00),
    ("MID", RPM_MID),
    ("HIGH", RPM_HIGH),

]:

    for torque_name, torque_vec in [

        ("POS", 0x00),
        ("NEG", NEG_TORQUE),

    ]:

        base_residual = (
            BASE
            ^ rpm_vec
            ^ torque_vec
        )

        key = (
            rpm_name,
            torque_name,
            "NONE"
        )

        generated[base_residual] = key

        for overlay in OVERLAYS:

            candidate = (
                base_residual
                ^ overlay
            )

            key = (
                rpm_name,
                torque_name,
                f"0x{overlay:02X}"
            )

            generated[candidate] = key

# ============================================================
# REPORT
# ============================================================

print()
print("Generated Residual Space")
print("-" * 80)

for residual in sorted(generated):

    rpm, torque, overlay = generated[residual]

    print(
        f"0x{residual:02X}  "
        f"{rpm:4s}  "
        f"{torque:3s}  "
        f"{overlay}"
    )

# ============================================================
# MATCH OBSERVED
# ============================================================

print()
print("=" * 80)
print("OBSERVED RESIDUALS")
print("=" * 80)

matched = 0

for residual in RESIDUALS:

    if residual in generated:

        matched += 1

        rpm, torque, overlay = (
            generated[residual]
        )

        print(
            f"0x{residual:02X}  "
            f"MATCH  "
            f"{rpm:4s} "
            f"{torque:3s} "
            f"{overlay}"
        )

    else:

        print(
            f"0x{residual:02X}  "
            f"UNEXPLAINED"
        )

# ============================================================
# COVERAGE
# ============================================================

print()
print("=" * 80)
print("COVERAGE")
print("=" * 80)

coverage = (
    matched
    * 100.0
    / len(RESIDUALS)
)

print(
    f"Observed Residuals : "
    f"{len(RESIDUALS)}"
)

print(
    f"Explained Residuals : "
    f"{matched}"
)

print(
    f"Coverage : "
    f"{coverage:.2f}%"
)

# ============================================================
# DISCOVER NEW OVERLAYS
# ============================================================

print()
print("=" * 80)
print("UNEXPLAINED RESIDUAL ANALYSIS")
print("=" * 80)

for residual in RESIDUALS:

    if residual in generated:
        continue

    print()
    print(
        f"Residual 0x{residual:02X}"
    )

    for base in [

        0x5E,
        0x25,

        0x8D,
        0xF6,

        0xC7,
        0xBC

    ]:

        vec = (
            residual
            ^ base
        )

        print(
            f"  vs 0x{base:02X}"
            f" -> 0x{vec:02X}"
        )

print()
print("Done.")