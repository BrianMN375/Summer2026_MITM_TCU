#!/usr/bin/env python3

import csv
from collections import defaultdict, Counter

print()
print("=" * 80)
print("A8 0x91 / 0xD3 STATE CLASSIFIER")
print("=" * 80)

CSV_FILE = input(
    "\nCSV path: "
).strip().strip('"')

TARGETS = {0x91, 0xD3}

# ------------------------------------------------------------------
# Helpers
# ------------------------------------------------------------------

def extract_signal_le(buf, start_bit, length):
    raw = 0

    for i in range(8):
        raw |= buf[i] << (8 * i)

    mask = (1 << length) - 1

    return (raw >> start_bit) & mask


def decode(buf):

    stat = extract_signal_le(buf, 21, 9)

    dyn = (
        extract_signal_le(buf, 30, 10)
        - 509
    )

    rpm_raw = extract_signal_le(buf, 48, 16)

    rpm = rpm_raw * 0.25

    counter = buf[1] & 0x0F

    return rpm, stat, dyn, counter


# ------------------------------------------------------------------
# Load
# ------------------------------------------------------------------

frames = []

with open(CSV_FILE, newline="") as f:

    reader = csv.reader(f)

    header = next(reader)

    for idx, row in enumerate(reader):

        try:

            b = [
                int(x, 16)
                for x in row[-8:]
            ]

            residual = (
                b[0]
                ^ b[1]
                ^ b[2]
                ^ b[3]
                ^ b[4]
                ^ b[5]
                ^ b[6]
                ^ b[7]
            )

            rpm, stat, dyn, counter = decode(b)

            frames.append({
                "idx": idx,
                "buf": b,
                "res": residual,
                "rpm": rpm,
                "stat": stat,
                "dyn": dyn,
                "counter": counter
            })

        except:
            pass

print()
print(f"Frames loaded : {len(frames):,}")

# ------------------------------------------------------------------
# Find target families
# ------------------------------------------------------------------

target_frames = [
    f for f in frames
    if f["res"] in TARGETS
]

print(
    f"Target Frames : {len(target_frames):,}"
)

# ------------------------------------------------------------------
# State signatures
# ------------------------------------------------------------------

print()
print("=" * 80)
print("STATE SIGNATURES")
print("=" * 80)

signature_counts = Counter()

for f in target_frames:

    b = f["buf"]

    sig = (
        b[4],
        b[5],
        b[6],
        b[7]
    )

    signature_counts[sig] += 1

for sig, count in signature_counts.most_common(50):

    print(
        f"{count:6d}  "
        f"B4={sig[0]:02X} "
        f"B5={sig[1]:02X} "
        f"B6={sig[2]:02X} "
        f"B7={sig[3]:02X}"
    )

# ------------------------------------------------------------------
# Family stats
# ------------------------------------------------------------------

print()
print("=" * 80)
print("FAMILY STATISTICS")
print("=" * 80)

for family in sorted(TARGETS):

    fam = [
        f for f in target_frames
        if f["res"] == family
    ]

    if not fam:
        continue

    rpms = [x["rpm"] for x in fam]
    stats = [x["stat"] for x in fam]
    dyns = [x["dyn"] for x in fam]

    print()
    print(f"Residual 0x{family:02X}")

    print(
        f"Frames : {len(fam):,}"
    )

    print(
        f"RPM    : "
        f"{min(rpms):.1f} .. "
        f"{max(rpms):.1f}"
    )

    print(
        f"STAT   : "
        f"{min(stats)} .. "
        f"{max(stats)}"
    )

    print(
        f"DYN    : "
        f"{min(dyns)} .. "
        f"{max(dyns)}"
    )

# ------------------------------------------------------------------
# Transitions
# ------------------------------------------------------------------

print()
print("=" * 80)
print("TRANSITIONS")
print("=" * 80)

transition_counts = Counter()

for i in range(1, len(frames)):

    prev = frames[i - 1]["res"]
    curr = frames[i]["res"]

    if prev != curr:

        transition_counts[
            (prev, curr)
        ] += 1

for (src, dst), count in (
    transition_counts.most_common(50)
):

    if (
        src in TARGETS or
        dst in TARGETS
    ):
        print(
            f"0x{src:02X} -> "
            f"0x{dst:02X} "
            f"({count})"
        )

# ------------------------------------------------------------------
# Entry samples
# ------------------------------------------------------------------

print()
print("=" * 80)
print("ENTRY EVENTS")
print("=" * 80)

shown = 0

for i in range(1, len(frames)):

    prev = frames[i - 1]["res"]
    curr = frames[i]["res"]

    if (
        curr in TARGETS and
        prev != curr
    ):

        f = frames[i]

        b = f["buf"]

        print()

        print(
            f"IDX={f['idx']}"
        )

        print(
            f"0x{prev:02X} -> "
            f"0x{curr:02X}"
        )

        print(
            f"RPM={f['rpm']:.1f} "
            f"STAT={f['stat']} "
            f"DYN={f['dyn']}"
        )

        print(
            " ".join(
                f"{x:02X}"
                for x in b
            )
        )

        shown += 1

        if shown >= 25:
            break

print()
print("Done.")