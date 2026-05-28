// checksum_types.h — canonical shared enum definitions
#pragma once
#include <stdint.h>
#include <Arduino.h>

// -----------------------------------------------------------------------------
// Seed selection strategies
// -----------------------------------------------------------------------------
typedef enum SeedMode {
    SEED_FIXED = 0,             // use default fixed seed (legacy)
    SEED_FIXED_00,              // explicit fixed 0x00
    SEED_FIXED_FF,              // explicit fixed 0xFF
    SEED_CARRY_FORWARD,         // use last observed checksum as next seed
    SEED_MIRROR_NIBBLE_B1HI,    // mirror upper nibble of Byte1 (n<<4)|n
    SEED_BRUTE_TRACKED,         // brute-force discovered seed, stored dynamically
    SEED_UNKNOWN                // fallback (untuned)
} SeedMode;

// -----------------------------------------------------------------------------
// Byte masking strategies for CRC input
// -----------------------------------------------------------------------------
typedef enum MaskMode {
    MASK_NONE = 0,  // use bytes as-is
    MASK_LOW,       // zero out lower nibble
    MASK_HIGH       // zero out upper nibble
} MaskMode;

// -----------------------------------------------------------------------------
// Byte order options
// -----------------------------------------------------------------------------
typedef enum OrderMode {
    ORDER_NORMAL = 0,
    ORDER_REVERSED
} OrderMode;

// -----------------------------------------------------------------------------
// Optional "extra" modes for special CRC cases
// -----------------------------------------------------------------------------
typedef enum ExtraMode {
    EXTRA_NONE = 0,
    EXTRA_0xAA,
    EXTRA_ID_LOW
} ExtraMode;


struct ChecksumEntry7B {
    uint8_t payload7[7];
    uint8_t checksum;
};
