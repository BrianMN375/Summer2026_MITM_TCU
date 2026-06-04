#!/usr/bin/env python3

# ============================================================
# Dominant residuals discovered from clustering
# ============================================================

LOW_POS   = 0x5E
LOW_NEG   = 0x25

MID_POS   = 0x8D
MID_NEG   = 0xF6

HIGH_POS  = 0xC7
HIGH_NEG  = 0xBC

# ============================================================

def main():

    print()
    print("======================================")
    print("A8 RESIDUAL FACTORIZATION")
    print("======================================")
    print()

    dyn1 = LOW_POS ^ LOW_NEG
    dyn2 = MID_POS ^ MID_NEG
    dyn3 = HIGH_POS ^ HIGH_NEG

    print("Dyn Sign Candidate")
    print("------------------")

    print(f"LOW  : 0x{dyn1:02X}")
    print(f"MID  : 0x{dyn2:02X}")
    print(f"HIGH : 0x{dyn3:02X}")

    print()

    if dyn1 == dyn2 == dyn3:

        dyn = dyn1

        print(
            f"COMMON DYN VECTOR = 0x{dyn:02X}"
        )

        print()

        rpm_low  = LOW_POS
        rpm_mid  = MID_POS
        rpm_high = HIGH_POS

        print("RPM State Vectors")
        print("-----------------")

        print(
            f"LOW  = 0x{rpm_low:02X}"
        )

        print(
            f"MID  = 0x{rpm_mid:02X}"
        )

        print(
            f"HIGH = 0x{rpm_high:02X}"
        )

        print()

        print("Validation")
        print("----------")

        print(
            f"LOW_NEG  = "
            f"0x{rpm_low ^ dyn:02X}"
        )

        print(
            f"MID_NEG  = "
            f"0x{rpm_mid ^ dyn:02X}"
        )

        print(
            f"HIGH_NEG = "
            f"0x{rpm_high ^ dyn:02X}"
        )

        print()

        print(
            "Perfect two-factor model."
        )

    else:

        print(
            "No common dyn vector found."
        )

    print()

# ============================================================

if __name__ == "__main__":
    main()