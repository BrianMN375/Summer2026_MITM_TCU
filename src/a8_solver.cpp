// =============================================================================
// a8_solver.cpp
// =============================================================================

#include "a8_solver.h"

// =============================================================================
// LEARNED TABLE
// =============================================================================

const A8ChecksumEntry A8_TABLE[] = {

    {0xD0,0x0,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x9F},
    {0xD0,0x1,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x15},
    {0xD0,0x2,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x61},
    {0xD0,0x3,{0xDF,0xD8,0x78,0x64,0x00,0x00},0xE0},
    {0xD0,0x4,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x8A},
    {0xD0,0x5,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x11},
    {0xD0,0x6,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x98},
    {0xD0,0x7,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x46},
    {0xD0,0x8,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x8F},
    {0xD0,0x9,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x07},
    {0xD0,0xA,{0xDF,0xD8,0x78,0x64,0x00,0x00},0xC0},
    {0xD0,0xB,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x36},
    {0xD0,0xC,{0xDF,0xD8,0x78,0x64,0x00,0x00},0xBD},
    {0xD0,0xD,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x78},
    {0xD0,0xE,{0xDF,0xD8,0x78,0x64,0x00,0x00},0xF7},
    {0xD0,0xF,{0xDF,0xD8,0x78,0x64,0x00,0x00},0x6B},
};

const size_t A8_TABLE_COUNT =
    sizeof(A8_TABLE) /
    sizeof(A8_TABLE[0]);

// =============================================================================
// LOOKUP
// =============================================================================

static bool lookup_checksum_0xA8(
    const uint8_t frame[8],
    uint8_t* outChecksum)
{
    uint8_t familyNibble =
        frame[1] & 0xF0;

    uint8_t counterNibble =
        frame[1] & 0x0F;

    const uint8_t* payloadCore =
        &frame[2];

    for (size_t i = 0;
         i < A8_TABLE_COUNT;
         i++)
    {
        const A8ChecksumEntry& e =
            A8_TABLE[i];

        if (e.familyNibble != familyNibble)
            continue;

        if (e.counterNibble != counterNibble)
            continue;

        bool match = true;

        for (int j = 0; j < 6; j++) {

            if (e.payloadCore[j] !=
                payloadCore[j])
            {
                match = false;
                break;
            }
        }

        if (match) {

            *outChecksum =
                e.checksum;

            return true;
        }
    }

    return false;
}

// =============================================================================
// BRUTEFORCE PLACEHOLDER
// =============================================================================

static bool bruteforce_checksum_0xA8(
    const uint8_t frame[8],
    uint8_t* outChecksum)
{
    (void)frame;
    (void)outChecksum;

    // Future:
    //  CRC(payload || hiddenByte)
    //  counter-normalized solving
    //  hidden transform discovery

    return false;
}

// =============================================================================
// MASTER SOLVER
// =============================================================================

bool solve_checksum_0xA8(
    const uint8_t frame[8],
    uint8_t* outChecksum,
    bool* usedLookup,
    bool* usedBruteforce)
{
    *usedLookup = false;
    *usedBruteforce = false;

    if (lookup_checksum_0xA8(
            frame,
            outChecksum))
    {
        *usedLookup = true;
        return true;
    }

    if (bruteforce_checksum_0xA8(
            frame,
            outChecksum))
    {
        *usedBruteforce = true;
        return true;
    }

    return false;
}