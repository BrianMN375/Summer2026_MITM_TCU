#!/usr/bin/env python3

# ============================================================
# OBSERVED RESIDUALS
# ============================================================

RESIDUALS = sorted({

    0x07,
    0x19,
    0x1C,
    0x1E,
    0x25,
    0x38,
    0x43,
    0x4D,
    0x56,
    0x5D,
    0x5E,
    0x62,
    0x72,
    0x84,
    0x8D,
    0xBC,
    0xC0,
    0xC7,
    0xF6,

})

# ============================================================
# BASE MODEL
# ============================================================

BASE = 0x5E

RPM_VECTORS = {

    "LOW"  : 0x00,
    "MID"  : 0xD3,
    "HIGH" : 0x99,

}

TORQUE_VECTORS = {

    "POS" : 0x00,
    "NEG" : 0x7B,

}

MODE_VECTORS = {

    "NONE" : 0x00,

    "84"   : 0x84,
    "C0"   : 0xC0,
    "43"   : 0x43,

}

OVERLAY_VECTORS = {

    "NONE" : 0x00,

    "DE"   : 0xDE,
    "91"   : 0x91,
    "DB"   : 0xDB,
    "5A"   : 0x5A,

}

# ============================================================

print()
print("=" * 80)
print("A8 RESIDUAL RECONSTRUCTOR")
print("=" * 80)

# ============================================================
# GENERATE ALL COMBINATIONS
# ============================================================

generated = {}

for rpm_name, rpm_vec in RPM_VECTORS.items():

    for torque_name, torque_vec in TORQUE_VECTORS.items():

        for mode_name, mode_vec in MODE_VECTORS.items():

            for overlay_name, overlay_vec in OVERLAY_VECTORS.items():

                residual = (
                    BASE
                    ^ rpm_vec
                    ^ torque_vec
                    ^ mode_vec
                    ^ overlay_vec
                )

                generated.setdefault(
                    residual,
                    []
                ).append({

                    "rpm"     : rpm_name,
                    "torque"  : torque_name,
                    "mode"    : mode_name,
                    "overlay" : overlay_name

                })

# ============================================================
# REPORT
# ============================================================

matched = 0

for residual in RESIDUALS:

    print()
    print("-" * 80)

    print(
        f"Residual 0x{residual:02X}"
    )

    if residual not in generated:

        print(
            "  UNEXPLAINED"
        )

        continue

    matched += 1

    solutions = generated[residual]

    print(
        f"  Solutions: {len(solutions)}"
    )

    for s in solutions:

        print(
            f"    RPM={s['rpm']:4s} "
            f"TORQUE={s['torque']:3s} "
            f"MODE={s['mode']:4s} "
            f"OVL={s['overlay']}"
        )

# ============================================================
# COVERAGE
# ============================================================

print()
print("=" * 80)
print("COVERAGE")
print("=" * 80)

coverage = (
    matched * 100.0 /
    len(RESIDUALS)
)

print(
    f"Observed : {len(RESIDUALS)}"
)

print(
    f"Explained: {matched}"
)

print(
    f"Coverage : {coverage:.2f}%"
)

# ============================================================
# GENERATED SPACE
# ============================================================

print()
print("=" * 80)
print("GENERATED RESIDUAL SPACE")
print("=" * 80)

for residual in sorted(generated):

    print(
        f"0x{residual:02X}"
    )

print()
print("Done.")