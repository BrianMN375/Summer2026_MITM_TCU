#!/usr/bin/env python3

import csv

import a8_final_validator_v14 as v

TARGET = 0x5A

print()
print("=" * 80)
print("TRACE REMAINING 0x5A FRAMES")
print("=" * 80)

# ------------------------------------------------------------------
# Small helper that mirrors predicted_residual()
# but records decisions
# ------------------------------------------------------------------

def trace_prediction(frame):

    byte4 = frame[4]
    byte5 = frame[5]
    byte6 = frame[6]
    byte7 = frame[7]

    rpm, stat, dyn = v.decode_signals(frame)

    trace = []

    predicted = None

    # ----------------------------------------------------------
    # Copy ONLY the family-selection section from
    # predicted_residual() here.
    #
    # IMPORTANT:
    # Whenever a family is selected:
    #
    # predicted = 0xXX
    # trace.append("family: XXXXX")
    #
    # Example:
    #
    # predicted = 0x5D
    # trace.append("family: high_load")
    #
    # ----------------------------------------------------------

    #
    # EASIEST METHOD:
    #
    # Copy your predicted_residual() body into this function.
    # After every assignment:
    #
    # predicted = 0xXX
    #
    # add:
    #
    # trace.append(...)
    #
    # ----------------------------------------------------------

    #
    # Example placeholders
    #

    predicted = v.predicted_residual(frame)

    trace.append(f"final predicted=0x{predicted:02X}")

    return predicted, trace


# ------------------------------------------------------------------

with open(v.CSV_FILE, newline="") as csvfile:

    reader = csv.DictReader(csvfile)

    idx = 0

    for row in reader:

        frame = v.load_frame_from_row(row)

        if frame is None:
            continue

        idx += 1

        actual = v.actual_residual(frame)

        predicted = v.predicted_residual(frame)

        residual = actual ^ predicted

        if residual != TARGET:
            continue

        rpm, stat, dyn = v.decode_signals(frame)

        traced_predicted, trace = trace_prediction(frame)

        print()
        print("-" * 80)

        print(
            f"IDX={idx}"
        )

        print(
            f"ACT=0x{actual:02X} "
            f"PRED=0x{predicted:02X} "
            f"RES=0x{residual:02X}"
        )

        print(
            f"RPM={rpm:.1f} "
            f"STAT={stat} "
            f"DYN={dyn}"
        )

        print(
            "FRAME: "
            + " ".join(
                f"{x:02X}"
                for x in frame
            )
        )

        print()

        for t in trace:
            print("  ", t)

print()
print("Done.")