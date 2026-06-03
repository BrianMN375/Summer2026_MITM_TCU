#!/usr/bin/env python3

import csv
from collections import Counter, defaultdict
from pathlib import Path

# ----------------------------------------------------------
# INPUT FILE
# ----------------------------------------------------------

CSV_FILE = (
    "/Users/brianmay/Dropbox/"
    "Electronics Dev Library and Docs/"
    "A2 - PlatformIO/"
    "PlatformIO Repository/"
    "Github Repositories/"
    "Summer2026_MITM_TCU/"
    "Summer2026_MITM_TCU/"
    "logs/"
    # "a7_payload_unique.csv"
    # "a7_unique.csv"
    # "a7_balanced.csv"
    # "a8_full_0015_a7only.csv"
    "a8_full_0015_a8only.csv"
)


# Maximum payloads per counter for balanced dataset
BALANCED_LIMIT = 5000

# ----------------------------------------------------------

def load_frame(row):

    try:
        return tuple(
            int(row[f"b{i}"], 16)
            for i in range(8)
        )
    except:
        return None

# ----------------------------------------------------------

def main():

    input_path = Path(CSV_FILE)

    out_unique = input_path.with_name(
        "a8_unique.csv"
    )

    out_payload_unique = input_path.with_name(
        "a8_payload_unique.csv"
    )

    out_balanced = input_path.with_name(
        "a8_balanced.csv"
    )

    report_file = input_path.with_name(
        "a8_dataset_report.txt"
    )

    # ------------------------------------------------------
    # Containers
    # ------------------------------------------------------

    unique_frames = set()
    payload_unique = set()

    counter_counts = Counter()

    balanced_rows = []
    balanced_seen = set()

    per_counter_count = Counter()

    total_rows = 0

    # ------------------------------------------------------
    # Read CSV
    # ------------------------------------------------------

    with open(input_path, newline="") as f:

        reader = csv.DictReader(f)

        for row in reader:

            frame = load_frame(row)

            if frame is None:
                continue

            total_rows += 1

            counter = frame[1] & 0x0F

            counter_counts[counter] += 1

            # ----------------------------------------------
            # Full-frame unique
            # ----------------------------------------------

            unique_frames.add(frame)

            # ----------------------------------------------
            # Payload unique
            #
            # b1..b7 only
            # ----------------------------------------------

            payload_key = frame[1:]

            payload_unique.add(payload_key)

            # ----------------------------------------------
            # Balanced dataset
            # ----------------------------------------------

            if payload_key not in balanced_seen:

                if per_counter_count[counter] < BALANCED_LIMIT:

                    balanced_seen.add(payload_key)

                    per_counter_count[counter] += 1

                    balanced_rows.append(frame)

    # ------------------------------------------------------
    # Write a7_unique.csv
    # ------------------------------------------------------

    with open(out_unique, "w", newline="") as f:

        writer = csv.writer(f)

        writer.writerow(
            [f"b{i}" for i in range(8)]
        )

        for frame in sorted(unique_frames):

            writer.writerow(
                [f"{x:02X}" for x in frame]
            )

    # ------------------------------------------------------
    # Write a7_payload_unique.csv
    # ------------------------------------------------------

    with open(out_payload_unique, "w", newline="") as f:

        writer = csv.writer(f)

        writer.writerow(
            ["b0","b1","b2","b3","b4","b5","b6","b7"]
        )

        for payload in sorted(payload_unique):

            writer.writerow(
                [f"{x:02X}" for x in payload]
            )

    # ------------------------------------------------------
    # Write balanced dataset
    # ------------------------------------------------------

    with open(out_balanced, "w", newline="") as f:

        writer = csv.writer(f)

        writer.writerow(
            [f"b{i}" for i in range(8)]
        )

        for frame in balanced_rows:

            writer.writerow(
                [f"{x:02X}" for x in frame]
            )

    # ------------------------------------------------------
    # Report
    # ------------------------------------------------------

    with open(report_file, "w") as f:

        f.write("A8 DATASET REPORT\n")
        f.write("=================\n\n")

        f.write(
            f"Total Rows           : {total_rows:,}\n"
        )

        f.write(
            f"Unique Frames        : {len(unique_frames):,}\n"
        )

        f.write(
            f"Unique Payloads      : {len(payload_unique):,}\n"
        )

        f.write(
            f"Balanced Dataset     : {len(balanced_rows):,}\n"
        )

        f.write("\n")

        f.write("Counter Distribution\n")
        f.write("--------------------\n")

        for c in range(16):

            f.write(
                f"{c:X}: "
                f"{counter_counts[c]:,}\n"
            )

        f.write("\n")

        f.write("Balanced Counts\n")
        f.write("--------------------\n")

        for c in range(16):

            f.write(
                f"{c:X}: "
                f"{per_counter_count[c]:,}\n"
            )

    # ------------------------------------------------------

    print()
    print("Done.")
    print()

    print("Generated:")
    print("  a8_unique.csv")
    print("  a8_payload_unique.csv")
    print("  a8_balanced.csv")
    print("  a8_dataset_report.txt")
    print()

# ----------------------------------------------------------

if __name__ == "__main__":
    main()