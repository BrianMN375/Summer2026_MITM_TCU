#!/usr/bin/env python3

from collections import Counter

# ============================================================
# Paste residuals from validator here
# ============================================================

RESIDUALS = [

    0xC7,
    0x8D,
    0x5E,
    0xBC,
    0x25,
    0xF6,
    0x1E,
    0x0D,
    0x38,
    0x62,
    0x84,
    0x5D,
    0xC6,
    0x73,
    0xC0,
    0xBD,
    0x83,
    0x14,
    0x8C,
    0x5F,
    0xEA,
    0xB4,
    0x36,
    0x08,
    0x1A,
    0x19,
    0xAF,
    0xF8
]

# ============================================================

def highest_bit(x):

    if x == 0:
        return -1

    return x.bit_length() - 1

# ============================================================

def build_basis(values):

    basis = []

    for value in values:

        v = value

        for b in basis:

            v = min(v, v ^ b)

        if v:

            basis.append(v)

            basis.sort(
                reverse=True
            )

    return basis

# ============================================================

def express(value, basis):

    v = value

    used = []

    for b in basis:

        if (v ^ b) < v:

            v ^= b

            used.append(b)

    return v == 0, used

# ============================================================

def main():

    print()
    print("======================================")
    print("A8 RESIDUAL BASIS FINDER")
    print("======================================")
    print()

    basis = build_basis(
        RESIDUALS
    )

    print(
        f"Residual Count: "
        f"{len(RESIDUALS)}"
    )

    print()

    print(
        f"Basis Size: "
        f"{len(basis)}"
    )

    print()

    print(
        "Basis Vectors"
    )

    print(
        "-------------"
    )

    for b in basis:

        print(
            f"0x{b:02X}"
        )

    print()

    print(
        "Residual Decomposition"
    )

    print(
        "----------------------"
    )

    for r in sorted(RESIDUALS):

        success, used = express(
            r,
            basis
        )

        print(
            f"0x{r:02X} = "
            + " ^ ".join(
                f"0x{x:02X}"
                for x in used
            )
        )

    print()

    print(
        "Done."
    )

# ============================================================

if __name__ == "__main__":
    main()