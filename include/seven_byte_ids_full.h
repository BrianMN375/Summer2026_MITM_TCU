#pragma once
#include <stdint.h>
#include <stddef.h>

// -----------------------------------------------------------------------------
// 7-byte IDs (require extended checksum calculation)
// -----------------------------------------------------------------------------
static const uint32_t SEVEN_BYTE_IDS_FULL[] = {
    0x86,
    0x9F,
    0xA7,
    0xA8,
    0xAD,
    0xB2,
    0xFD,
    0x101,
    0x104,
    0x106,
    0x116,
    0x118,
    0x11E,
    0x121,
    0x134,
    0x135,
    0x1A2,
    0x1AB,
    0x30B,
    0x30D,
    0x31B,
    0x365,
    0x381,
    0x382,
    0x391,
    0x392,
    0x3BE,
    0x3C0,
    0x3C8,
    0x640,
    0x64A,
    0x65F,
    0x6B2
};

// Automatically updated count macro
static const size_t SEVEN_BYTE_IDS_FULL_COUNT =
    sizeof(SEVEN_BYTE_IDS_FULL) / sizeof(SEVEN_BYTE_IDS_FULL[0]);
