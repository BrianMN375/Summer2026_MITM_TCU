#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <pgmspace.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <FlexCAN_T4.h>
#define PROGMEM_LOOKUP_DEFINE

// #define DEBUG_VERBOSE_CHECKSUM   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_FULL   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x101   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x104   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x106   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x116   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x11E   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x1AB   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x31B   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0x392   // comment this out to silence debug output
// #define DEBUG_VERBOSE_CHECKSUM_0xFD   // comment this out to silence debug output


#include "checksum.h"
#include "checksum_config_full.h"
#include "seven_byte_ids_full.h"
#include "per_id_checksum_cfg.h"
// #include "progmem_lookup_3byte_full.h"
// #include "progmem_lookup_7byte_full.h"
#include "progmem_lookup_dispatch.h"
#include "checksum_integration.h"

// PROGMEM lookup table and count definition
// -----------------------------------------------------------------------------
#if !defined(CHECKSUM_PROG_ENTRIES_COUNT)
#define CHECKSUM_PROG_ENTRIES_COUNT (sizeof(CHECKSUM_PROG_ENTRIES) / sizeof(CHECKSUM_PROG_ENTRIES[0]))
#endif

const size_t CHECKSUM_MAP_101_COUNT = sizeof(CHECKSUM_MAP_101) / sizeof(CHECKSUM_MAP_101[0]);
const size_t CHECKSUM_MAP_104_COUNT = sizeof(CHECKSUM_MAP_104) / sizeof(CHECKSUM_MAP_104[0]);
const size_t CHECKSUM_MAP_106_COUNT = sizeof(CHECKSUM_MAP_106) / sizeof(CHECKSUM_MAP_106[0]);
const size_t CHECKSUM_MAP_116_COUNT = sizeof(CHECKSUM_MAP_116) / sizeof(CHECKSUM_MAP_116[0]);
const size_t CHECKSUM_MAP_11E_COUNT = sizeof(CHECKSUM_MAP_11E) / sizeof(CHECKSUM_MAP_11E[0]);
const size_t CHECKSUM_MAP_1AB_COUNT = sizeof(CHECKSUM_MAP_1AB) / sizeof(CHECKSUM_MAP_1AB[0]);
const size_t CHECKSUM_MAP_31B_COUNT = sizeof(CHECKSUM_MAP_31B) / sizeof(CHECKSUM_MAP_31B[0]);
const size_t CHECKSUM_MAP_392_COUNT = sizeof(CHECKSUM_MAP_392) / sizeof(CHECKSUM_MAP_392[0]);
const size_t CHECKSUM_MAP_FD_COUNT = sizeof(CHECKSUM_MAP_FD) / sizeof(CHECKSUM_MAP_FD[0]);





#pragma region // Old/disregard for now...

// CRC TABLES + UTILITIES
// ============================================================================
extern CAN_message_t msg;

static uint8_t reflect_table[256];
static uint8_t crc_table[256];
static bool tables_init_done = false;

static void init_tables_if_needed(void) {
    if (tables_init_done) return;
    for (int i = 0; i < 256; ++i) {
        uint8_t b = (uint8_t)i, r = 0;
        for (int j = 0; j < 8; ++j)
            if (b & (1 << j)) r |= (uint8_t)(1 << (7 - j));
        reflect_table[i] = r;
    }

    const uint8_t POLY = 0x1D;
    for (int i = 0; i < 256; ++i) {
        uint8_t c = (uint8_t)i;
        for (int j = 0; j < 8; ++j)
            c = (c & 0x80) ? (uint8_t)((c << 1) ^ POLY) : (uint8_t)(c << 1);
        crc_table[i] = c;
    }
    tables_init_done = true;
}

static inline uint8_t upd_state(uint8_t s, uint8_t b) { return crc_table[(uint8_t)(s ^ reflect_table[b])]; }
static inline uint8_t finalize_crc(uint8_t s) { return (uint8_t)(reflect_table[s] ^ 0x55); }

// DYNAMIC SEED MEMORY
// ============================================================================
static uint8_t last_seed_map[2048];
static bool    seed_valid_map[2048] = {0};

void update_last_seed(uint32_t id, uint8_t seed) {
    uint16_t slot = (uint16_t)(id & 0x7FF);
    last_seed_map[slot] = seed;
    seed_valid_map[slot] = true;
}
uint8_t get_last_seed(uint32_t id, uint8_t fallback) {
    uint16_t slot = (uint16_t)(id & 0x7FF);
    return seed_valid_map[slot] ? last_seed_map[slot] : fallback;
}

bool has_last_seed(uint32_t id) {
    return seed_valid_map[(uint16_t)(id & 0x7FF)];
}
void learn_seed_from_observed(uint32_t id, const uint8_t payload8[8]) {
    // Given observed byte0 (target) and b1..b7, brute the seed so our state is aligned.
    uint8_t target = payload8[0];
    uint8_t p7[7];
    for (int i=0;i<7;++i) p7[i] = payload8[i+1];

    // Try all seeds; stop at first match.
    for (int s = 0; s < 256; ++s) {
        if (crc_compute_from_seed((uint8_t)s, p7) == target) {
            update_last_seed(id, (uint8_t)s);
#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
            Serial.printf("[seed-learn] 0x%03X  learned seed=0x%02X from observed byte0=0x%02X\n",
                          (uint32_t)(id & 0x7FF), (uint8_t)s, target);
#endif
            return;
        }
    }
#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[seed-learn] 0x%03X  FAILED to learn seed from observed frame\n",
                  (uint32_t)(id & 0x7FF));
#endif
}




// --- Per-ID mode tallies and decision ---------------------------------------
struct SeedTally {
    uint32_t seen = 0;
    uint32_t hit_carry = 0;
    uint32_t hit_nib   = 0;
    uint32_t hit_fix00 = 0;
    uint32_t hit_fixFF = 0;
    uint32_t hit_brute = 0;  // whenever brute found a seed equal to observed
    SeedMode locked = SEED_UNKNOWN; // when confident, lock
};

static SeedTally seed_tally_map[2048]; // 11-bit ID space

static inline uint16_t idslot(uint32_t id){ return (uint16_t)(id & 0x7FF); }

const char* seedModeName(SeedMode m){
    switch(m){
        case SEED_CARRY_FORWARD: return "CARRY_FORWARD";
        case SEED_MIRROR_NIBBLE_B1HI: return "MIRROR_NIBBLE_B1HI";
        case SEED_FIXED_00: return "FIXED_00";
        case SEED_FIXED_FF: return "FIXED_FF";
        case SEED_BRUTE_TRACKED: return "BRUTE_TRACKED";
        case SEED_UNKNOWN: default: return "UNKNOWN";
    }
}

SeedMode seedmode_get(uint32_t id){
    return seed_tally_map[idslot(id)].locked;
}
void seedmode_force(uint32_t id, SeedMode mode){
    seed_tally_map[idslot(id)].locked = mode;
}



// CONFIG + TRANSFORM HELPERS
// ============================================================================
bool id_uses_7byte(uint32_t can_id) {
    for (size_t i = 0; i < SEVEN_BYTE_IDS_FULL_COUNT; ++i)
        if (SEVEN_BYTE_IDS_FULL[i] == can_id) return true;
    return false;
}
static inline bool find_per_id_cfg(uint32_t id, PerIdCfg *out) {
    for (size_t i = 0; i < PER_ID_CFG_TABLE_COUNT; ++i)
        if (PER_ID_CFG_TABLE[i].id == id) {
            if (out) *out = PER_ID_CFG_TABLE[i];
            return true;
        }
    return false;
}
void apply_mask_order(uint8_t out7[7], const uint8_t in7[7], uint8_t mask_mode, uint8_t order_mode) {
    for (int i = 0; i < 7; ++i) out7[i] = in7[i];
    if (mask_mode == 1) out7[0] &= 0xF0;
    else if (mask_mode == 2) out7[0] &= 0x0F;
    if (order_mode == 1) {
        uint8_t tmp[7];
        for (int i = 0; i < 7; ++i) tmp[i] = out7[6 - i];
        memcpy(out7, tmp, 7);
    }
}

// CORE CRC FUNCTIONS
// ============================================================================
uint8_t crc_compute_from_seed(uint8_t seed, const uint8_t payload7[7]) {
    init_tables_if_needed();
    uint8_t st = seed;
    for (int i = 0; i < 7; ++i) st = upd_state(st, payload7[i]);
    return finalize_crc(st);
}
static inline uint8_t progmem_read_u8(const uint8_t *addr) { return pgm_read_byte(addr); }

// MAIN BYTE0 UPDATE + RESOLVE (patched for debug logging)
// ============================================================================
bool checksum_update_byte0(uint32_t can_id, const uint8_t payload7[7], uint8_t *byte0_out) {
    init_tables_if_needed();  // ensure CRC tables ready

    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(can_id, &cfg))
        apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else
        memcpy(transformed, payload7, 7);

    // --- 3B fast PROGMEM lookup path ---
    if (CHECKSUM_PROG_ENTRIES_COUNT > 0 && !id_uses_7byte(can_id)) {
        for (size_t ei = 0; ei < CHECKSUM_PROG_ENTRIES_COUNT; ++ei) {
            ChecksumProgEntry entry;
            memcpy(&entry, &CHECKSUM_PROG_ENTRIES[ei], sizeof(ChecksumProgEntry));
            if (entry.id != can_id) continue;
            const uint8_t (*tbl)[4] = (const uint8_t (*)[4]) entry.table;
            for (size_t r = 0; r < entry.entries; ++r) {
                uint8_t b1 = pgm_read_byte(&tbl[r][0]);
                uint8_t b2 = pgm_read_byte(&tbl[r][1]);
                uint8_t b3 = pgm_read_byte(&tbl[r][2]);
                if (b1 == transformed[0] && b2 == transformed[1] && b3 == transformed[2]) {
                    *byte0_out = pgm_read_byte(&tbl[r][3]);

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
                    Serial.printf("[checksum_update_byte0] 3B PROGMEM hit for 0x%03X → 0x%02X\n", can_id, *byte0_out);
#endif
                    return true;
                }
            }
        }
    }

// --- 7B recompute (final ABS-correct variant) ---



// learn seed if missing
// --- 7B recompute (final MQB ABS / 0x101 variant) ---
init_tables_if_needed();

// --- Derive seed dynamically based on rolling counter nibble for ABS IDs ---
uint8_t seed = 0x00;
if (can_id == 0x101) {
    // Rolling counter nibble is the upper nibble of Byte1 (payload7[0])
    uint8_t counter_nib = (payload7[0] >> 4) & 0x0F;
    // Empirically, ABS ECUs use this nibble mirrored as the seed
    seed = (counter_nib << 4) | counter_nib;
} else {
    // For all other 7B IDs, use stored seed or default 0x00
    seed = get_last_seed(can_id, 0x00);
}

// --- Compute CRC8 0x1D forward, no reflect ---

    // compute checksum using canonical function (reflect + finalize)
    uint8_t ck = crc_compute_from_seed(seed, transformed);

    // store computed byte0 and update seed mapping
    *byte0_out = ck;
    
// Update seed store for continuity
update_last_seed(can_id, seed);

*byte0_out = ck;

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
// Serial.printf("[checksum_update_byte0] 7B recompute for 0x%03X seed=0x%02X → Byte0=0x%02X\n",
            //   can_id, seed, ck);
#endif

return true;


#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[checksum_update_byte0] FAILED for 0x%03X (no non-zero seed result)\n", can_id);
#endif
    *byte0_out = 0;
    return false;
}

bool checksum_update_byte0_0x104(const uint8_t payload7[7], uint8_t *byte0_out) {
    // same pattern as above
    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(0x104, &cfg))
        apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else
        memcpy(transformed, payload7, 7);

    uint8_t seed = get_last_seed(0x104, 0x00);
    *byte0_out = crc_compute_from_seed(seed, transformed);
    update_last_seed(0x104, seed);
    return true;
}





// Carry-Forward Seeds 
bool checksum_update_byte0_0x101(const uint8_t payload7[7], uint8_t *byte0_out) {
    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(0x101, &cfg))
        apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else
        memcpy(transformed, payload7, 7);

    uint8_t prev_seed = get_last_seed(0x101, 0x00);
    uint8_t ck = crc_compute_from_seed(prev_seed, transformed);
    *byte0_out = ck;

    // carry-forward: make the computed checksum become the stored seed for the next frame
    update_last_seed(0x101, ck);
    return true;
}

// bool checksum_update_byte0_0x106(const uint8_t payload7[7], uint8_t *byte0_out) {
//         init_tables_if_needed();  // ensure CRC tables ready

//     PerIdCfg cfg;
//     uint8_t transformed[7];
//     if (find_per_id_cfg(0x106, &cfg))
//         apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
//     else
//         memcpy(transformed, payload7, 7);

//     uint8_t prev_seed = get_last_seed(0x106, 0x00);
//     uint8_t ck = crc_compute_from_seed(prev_seed, transformed);
//     *byte0_out = ck;

//     // carry-forward: make the computed checksum become the stored seed for the next frame
//     update_last_seed(0x106, ck);
//     return true;
// }

bool checksum_update_byte0_0x1AB(const uint8_t payload7[7], uint8_t *byte0_out) {
    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(0x1AB, &cfg)) apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else memcpy(transformed, payload7, 7);

    // read previous seed (do NOT assume payload7[0] is the seed)
    uint8_t prev_seed = get_last_seed(0x1AB, 0x00);

    // compute checksum that should be used for this frame
    uint8_t ck = crc_compute_from_seed(prev_seed, transformed);

    // write it out and update seed store to this computed value for next frame
    *byte0_out = ck;
    // Serial.printf("[0x1AB] Final: prev_seed=0x%02X → new_ck=0x%02X → next_seed=0x%02X\n", prev_seed, ck, ck);

    update_last_seed(0x1AB, ck);

#if defined(DEBUG_VERBOSE_CHECKSUM_1AB)
    Serial.printf("[0x1AB] Using previous seed = 0x%02X\n", prev_seed);
    Serial.printf("[0x1AB] Computed new checksum = 0x%02X (this becomes next seed)\n", ck);
#endif

    return true;
}

bool checksum_update_byte0_0x31B(const uint8_t payload7[7], uint8_t *byte0_out) {
    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(0x31B, &cfg)) apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else memcpy(transformed, payload7, 7);

    // get previous seed
    uint8_t prev_seed = get_last_seed(0x31B, 0x00);

    // compute CRC from previous seed
    uint8_t ck = crc_compute_from_seed(prev_seed, transformed);

    // update seed for next frame
    *byte0_out = ck;
    update_last_seed(0x31B, ck);

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[0x31B] Using previous seed = 0x%02X\n", prev_seed);
    Serial.printf("[0x31B] Computed new checksum = 0x%02X (this becomes next seed)\n", ck);
#endif

    return true;
}





// Fixed Seeds 
bool checksum_update_byte0_0x116(const uint8_t payload7[7], uint8_t *byte0_out) {
    uint8_t ck = crc_compute_from_seed(0xFF, payload7);
    *byte0_out = ck;
    update_last_seed(0x116, 0xFF);   // keep constant seed
    return true;
}



bool checksum_update_byte0_0x392(const uint8_t payload7[7], uint8_t *byte0_out) {
    uint8_t ck = crc_compute_from_seed(0xFF, payload7);
    *byte0_out = ck;
    update_last_seed(0x392, 0xFF);   // keep constant seed
    return true;
}

bool checksum_update_byte0_0xFD(const uint8_t payload7[7], uint8_t *byte0_out) {
    uint8_t ck = crc_compute_from_seed(0xFF, payload7);
    *byte0_out = ck;
    update_last_seed(0xFD, 0xFF);   // keep constant seed
    return true;
}



// Nibble-Mirror for Byte1
bool checksum_update_byte0_0x11E(const uint8_t payload7[7], uint8_t *byte0_out) {
    PerIdCfg cfg; uint8_t t7[7];
    if (find_per_id_cfg(0x11E, &cfg))
        apply_mask_order(t7, payload7, cfg.mask, cfg.order);
    else
        memcpy(t7, payload7, 7);

    uint8_t seed = ((payload7[1] >> 4) & 0x0F) * 0x11;  // nibble mirror from byte1
    uint8_t ck = crc_compute_from_seed(seed, t7);
    *byte0_out = ck;
    update_last_seed(0x11E, seed);
    return true;
}



bool checksum_update_byte0_0x31A(const uint8_t payload7[7], uint8_t *byte0_out) {
    // same pattern as above
    PerIdCfg cfg;
    uint8_t transformed[7];
    if (find_per_id_cfg(0x31B, &cfg))
        apply_mask_order(transformed, payload7, cfg.mask, cfg.order);
    else
        memcpy(transformed, payload7, 7);

    uint8_t seed = get_last_seed(0x31B, 0x00);
    *byte0_out = crc_compute_from_seed(seed, transformed);
    update_last_seed(0x31B, seed);
    return true;
}


// ============================================================================
// 3-BYTE CHECKSUM COMPUTE (ADDITIVE VARIANTS)
// ============================================================================
uint8_t computeChecksum_3B(uint32_t can_id, const uint8_t *buf) {
    uint8_t p7[7];
    for (int i = 0; i < 7; ++i) p7[i] = buf[i + 1];

    PerIdCfg cfg;
    uint8_t t7[7];
    if (find_per_id_cfg(can_id, &cfg)) apply_mask_order(t7, p7, cfg.mask, cfg.order);
    else memcpy(t7, p7, 7);

    // PROGMEM path
    if (CHECKSUM_PROG_ENTRIES_COUNT > 0 && !id_uses_7byte(can_id)) {
        for (size_t ei = 0; ei < CHECKSUM_PROG_ENTRIES_COUNT; ++ei) {
            ChecksumProgEntry entry;
            memcpy(&entry, &CHECKSUM_PROG_ENTRIES[ei], sizeof(ChecksumProgEntry));
            if (entry.id != can_id) continue;
            const uint8_t (*tbl)[4] = (const uint8_t (*)[4]) entry.table;
            for (size_t r = 0; r < entry.entries; ++r) {
                uint8_t b1 = progmem_read_u8(&tbl[r][0]);
                uint8_t b2 = progmem_read_u8(&tbl[r][1]);
                uint8_t b3 = progmem_read_u8(&tbl[r][2]);
                if (b1 == t7[0] && b2 == t7[1] && b3 == t7[2])
                    return progmem_read_u8(&tbl[r][3]);
            }
        }
    }

    // --- Verified 0x101 logic ---
    uint8_t b1=t7[0], b2=t7[1], b3=t7[2], b4=t7[3], b5=t7[4], b6=t7[5];
    uint16_t sum = b1 + b2 + b3 + b4 + b5 + b6;
    uint8_t ck = (uint8_t)(~sum) ^ b4;

    // MQB ABS variant: MSB flip when bit7 of b6 set
    if (b6 & 0x40) ck ^= 0x80;

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[computeChecksum_3B][Final] ID=0x%03X b1..b6=", can_id);
    for (int i=0;i<6;++i){ if(t7[i]<0x10) Serial.print('0'); Serial.print(t7[i],HEX); Serial.print(' ');}
    Serial.printf("→ Byte0=0x%02X%s\n", ck, (b6 & 0x40) ? " (MSB flip)" : "");
#endif

    return ck;
}

// ============================================================================
// DISPATCH + VALIDATION
// ============================================================================
uint8_t computeChecksum(uint32_t can_id, const uint8_t *buf) {
    return id_uses_7byte(can_id) ? computeChecksum_7B(can_id, buf)
                                 : computeChecksum_3B(can_id, buf);
}
uint8_t computeChecksum_7B(uint32_t, const uint8_t *buf) {
    uint8_t p7[7]; for(int i=0;i<7;++i)p7[i]=buf[i+1];
    return crc_compute_from_seed(buf[0], p7);
}

// VALIDATION (patched to print matching seed if found)
// ============================================================================
// bool validateChecksum(uint32_t id, const uint8_t payload[8]) {
//     init_tables_if_needed();
//     uint8_t p7[7]; for (int i = 0; i < 7; ++i) p7[i] = payload[i + 1];
//     uint8_t target = payload[0];

//     for (int seed = 0; seed < 256; ++seed) {
//         uint8_t st = seed;
//         for (int j = 0; j < 7; ++j)
//             st = crc_table[(uint8_t)(st ^ reflect_table[p7[j]])];
//         uint8_t ck = (uint8_t)(reflect_table[st] ^ 0x55);
//         if (ck == target) {
// #if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
//             Serial.printf("[validateChecksum] 7B CRC valid for 0x%03X with seed=0x%02X\n", id, seed);
// #endif
//             return true;
//         }
//     }

// #if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
//     Serial.printf("[validateChecksum] 7B CRC INVALID for 0x%03X target=0x%02X\n", id, target);
// #endif
//     return false;
// }



/*
 * compute_final_checksum_bruteforce:
 *  - For 3-byte IDs: returns computeChecksum_3B result (fast path).
 *  - For 7-byte IDs:
 *      1) If a stored seed exists -> use it and return crc_compute_from_seed(seed, transformed).
 *      2) If can_id==0x101, try the mirrored-nibble heuristic first.
 *      3) If store_found_seed == true -> brute-force seeds 0..255 searching for a seed
 *         that yields payload[0] (observed); if found, store it and return that checksum.
 *      4) Otherwise fallback to stored seed (or 0x00) and return computed checksum for that seed.
 *
 * Note: transformed[] applies any per-ID mask/order before CRC computation.
 */
// uint8_t compute_final_checksum_bruteforce(uint32_t can_id, const uint8_t payload[8], bool store_found_seed) {
//     init_tables_if_needed();

//     // Build p7 from payload[1..7]
//     uint8_t p7[7];
//     for (int i = 0; i < 7; ++i) p7[i] = payload[i + 1];

//     // Apply per-ID mask/order into transformed[]
//     PerIdCfg cfg;
//     uint8_t transformed[7];
//     if (find_per_id_cfg(can_id, &cfg)) {
//         apply_mask_order(transformed, p7, cfg.mask, cfg.order);
//     } else {
//         memcpy(transformed, p7, 7);
//     }

//     // Fast path for 3B IDs
//     if (!id_uses_7byte(can_id)) {
//         uint8_t tmpbuf[8];
//         tmpbuf[0] = 0;            // placeholder
//         memcpy(&tmpbuf[1], transformed, 7);
//         return computeChecksum_3B(can_id, tmpbuf);
//     }

//     // 7-byte path
//     const uint8_t observed = payload[0];

//     // 1) Try stored seed first
//     if (has_last_seed(can_id)) {
//         uint8_t seed = get_last_seed(can_id, 0x00);
//         return crc_compute_from_seed(seed, transformed);
//     }

//     // 2) Special-case heuristic for 0x101 (mirrored nibble) - try it first
//     if (can_id == 0x101) {
//         uint8_t counter_nib = (transformed[0] >> 4) & 0x0F;
//         uint8_t seed_guess = (counter_nib << 4) | counter_nib;
//         uint8_t ck_guess = crc_compute_from_seed(seed_guess, transformed);
//         if (ck_guess == observed) {
//             if (store_found_seed) update_last_seed(can_id, seed_guess);
//             return ck_guess;
//         }
//         // else continue to brute/ fallback
//     }

//     // 3) Brute force (only if explicitly requested)
//     if (store_found_seed) {
//         for (int s = 0; s < 256; ++s) {
//             uint8_t ck = crc_compute_from_seed((uint8_t)s, transformed);
//             if (ck == observed) {
//                 update_last_seed(can_id, (uint8_t)s);
//                 return ck; // equals observed
//             }
//         }
//         // No seed matched observed byte — fallthrough to fallback
//     }

//     // 4) Fallback: no seed found, use fallback seed (0x00) or last seed if available
//     uint8_t fallback_seed = get_last_seed(can_id, 0x00);
//     return crc_compute_from_seed(fallback_seed, transformed);
// }

// // Thin wrapper that does not brute-force by default; delegates to brute-force variant with store_found_seed=false
// uint8_t compute_final_checksum(uint32_t can_id, const uint8_t payload[8]) {
//     return compute_final_checksum_bruteforce(can_id, payload, false);
// }



// -----------------------------------------------------------------------------
// validateChecksum: returns true if observed payload[0] equals computed checksum
// -----------------------------------------------------------------------------
bool validateChecksum(uint32_t id, const uint8_t payload[8]) {
    uint8_t computed = compute_final_checksum(id, payload);

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[validateChecksum] ID=0x%03X observed=0x%02X computed=0x%02X -> %s\n",
                  id, payload[0], computed, (payload[0] == computed) ? "OK" : "MISMATCH");
#endif
    return (payload[0] == computed);
}


// //  3byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_3Byte(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (!id_uses_7byte(id))  {
//          uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); 
//         }
//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }


// void build_frame_with_checksum_0x104(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) checksum_update_byte0_0x104(payload7, &ck);
//     else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x104)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }










// void build_frame_with_checksum_0x116(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) checksum_update_byte0_0x116(payload7, &ck);
//     else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x116)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }

// void build_frame_with_checksum_0x392(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) checksum_update_byte0_0x392(payload7, &ck);
//     else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x392)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }



// //  0xFD - 7byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_0xFD(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) checksum_update_byte0_0xFD(payload7, &ck);
//     else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0xFD)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }

// //  0x101 - 7byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_0x101(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) checksum_update_byte0_0x101(payload7, &ck);
//     else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x101)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }

// //  0x11E - 7byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_0x11E(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;
//     if (id_uses_7byte(id)) {
//     checksum_update_byte0_0x11E(payload7, &ck);
//     } else { uint8_t tmp[8]; tmp[0]=0; memcpy(&tmp[1], payload7, 7); ck = computeChecksum_3B(id, tmp); }

//     frame.id = id; frame.len = 8;
//     frame.buf[0] = ck; memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x11E)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }

// //  0x1AB - 7byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_0x1AB(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;

//     if (id_uses_7byte(id)) {
//         // Serial.println(F("[DEBUG] Taking 7B path → calling checksum_update_byte0_0x1AB"));
//         checksum_update_byte0_0x1AB(payload7, &ck);
//     } else {
//         // Serial.println(F("[DEBUG] Taking 3B fallback path"));
//         uint8_t tmp[8];
//         tmp[0] = 0;
//         memcpy(&tmp[1], payload7, 7);
//         ck = computeChecksum_3B(id, tmp);
//     }

//     frame.id = id;
//     frame.len = 8;
//     frame.buf[0] = ck;
//     memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x1AB)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }


// //  0x31B - 7byte Checksum Calculation and build frame. ============================================================================
// void build_frame_with_checksum_0x31B(CAN_message_t &frame, uint32_t id, const uint8_t payload7[7]) {
//     uint8_t ck = 0;

//     if (id_uses_7byte(id)) {
//         // Serial.println(F("[DEBUG] Taking 7B path → calling checksum_update_byte0_0x31B"));
//         checksum_update_byte0_0x31B(payload7, &ck);
//     } else {
//         // Serial.println(F("[DEBUG] Taking 3B fallback path"));
//         uint8_t tmp[8];
//         tmp[0] = 0;
//         memcpy(&tmp[1], payload7, 7);
//         ck = computeChecksum_3B(id, tmp);
//     }

//     frame.id = id;
//     frame.len = 8;
//     frame.buf[0] = ck;
//     memcpy(&frame.buf[1], payload7, 7);

// #if defined(DEBUG_VERBOSE_CHECKSUM_0x31B)
//     Serial.printf("[Build Frame] ID=0x%03X → Byte0=0x%02X (%s)\n",
//         id, ck, id_uses_7byte(id) ? "7B" : "3B");
// #endif
// }






// reflected CRC8 (poly 0x1D reflected => 0xB8)
uint8_t crc8_reflected(const uint8_t *data, size_t len, uint8_t init, uint8_t xorout){
    uint8_t crc = init;
    const uint8_t poly_ref = 0xB8;
    for(size_t i=0;i<len;i++){
        crc ^= data[i];
        for(int j=0;j<8;j++){
            if (crc & 1) crc = (crc >> 1) ^ poly_ref;
            else crc >>= 1;
        }
    }
    return crc ^ xorout;
}

// Find modifier X such that crc(payload7 + X) == target_checksum. Returns 0xFF if not found.
uint8_t find_modifier_bruteforce_target(const uint8_t payload7[7], uint8_t target_checksum){
    uint8_t buf[8];
    for(int i=0;i<7;i++) buf[i]=payload7[i];
    for(uint16_t x=0;x<256;x++){
        buf[7] = (uint8_t)x;
        uint8_t c = crc8_reflected(buf,8);
        if (c == target_checksum) return (uint8_t)x;
    }
    return 0xFF;
}

// PROGMEM table format: ID_hi, ID_lo, b1, b2, b3, X, ... terminator FF FF FF FF FF FF
extern const uint8_t checksum_3b_table[];
uint8_t lookup_modifier_3b(uint16_t can_id, const uint8_t key3[3], bool &found){
    found = false;
    const uint8_t *p = checksum_3b_table;
    size_t off = 0;
    while(true){
        uint8_t idhi = pgm_read_byte_near(p+off);
        uint8_t idlo = pgm_read_byte_near(p+off+1);
        // terminator check (0xFF,0xFF)
        if (idhi==0xFF && idlo==0xFF) break;
        uint16_t id = ((uint16_t)idhi<<8) | idlo;
        if (id == can_id){
            if (pgm_read_byte_near(p+off+2) == key3[0] &&
                pgm_read_byte_near(p+off+3) == key3[1] &&
                pgm_read_byte_near(p+off+4) == key3[2]){
                found = true;
                return pgm_read_byte_near(p+off+5);
            }
        }
        off += 6;
    }
    return 0x00;
}

// MITM wrapper implementation
void mitm_forward_with_checksum(CAN_message_t &msg_in, FlexCAN_T4_Base &TFTCAN3, bool preserve_checksum){
    // copy payload bytes 1..7 into payload7[0..6]
    uint8_t payload7[7];
    for(int i=0;i<7;i++) payload7[i] = msg_in.buf[i+1];
    uint8_t original_checksum = msg_in.buf[0];

    // example modification: if byte7 == 30 then add 15 to byte2 (index 1)
    if (payload7[6] == 30){
        payload7[1] = (uint8_t)(payload7[1] + 15);
    }

    // preserve counter nibble in msg_in.buf[1] if necessary (no change here)

    uint8_t X = 0x00;
    bool found = false;
    uint8_t key3[3] = { payload7[0], payload7[1], payload7[2] };

    if (preserve_checksum){
        // attempt to find X so that crc(payload7 + X) == original_checksum
        // first try lookup: some tables map key3->X for given recorded checksum; but lookup may be based on observed mapping
        // Here we try lookup assuming the key maps to the same X as recorded previously.
        X = lookup_modifier_3b(msg_in.id, key3, found);
        if (!found){
            uint8_t bf = find_modifier_bruteforce_target(payload7, original_checksum);
            if (bf != 0xFF){
                X = bf;
                found = true;
            }
        }
        // if still not found, default X=0x00 and we'll set checksum to computed crc(payload7 + X) -- this will change checksum value.
    } else {
        // Recompute for modified payload: try to lookup X for the new key3, otherwise default to X=0x00
        X = lookup_modifier_3b(msg_in.id, key3, found);
        if (!found){
            // If no lookup entry, pick X=0x00 (alternatively could brute force to match some target)
            X = 0x00;
        }
    }

    // Build outgoing message
    CAN_message_t outmsg;
    outmsg.id = msg_in.id;
    outmsg.len = msg_in.len;
    // put payload bytes into buf[1..7]
    for(int i=0;i<7;i++) outmsg.buf[i+1] = payload7[i];
    // compute checksum over payload7 + X and place into buf[0]
    uint8_t tmp[8];
    for(int i=0;i<7;i++) tmp[i] = payload7[i];
    tmp[7] = X;
    uint8_t new_crc = crc8_reflected(tmp,8);
    outmsg.buf[0] = new_crc;

    // preserve other fields like extended frame? copy from input
    outmsg.flags = msg_in.flags;
    // outmsg.priority = msg_in.priority;

    // send
    TFTCAN3.write(outmsg);
}


// -----------------------------------------------------------------------------
// Compute and return the final checksum value (Byte0) for a given frame.
// This works regardless of whether the frame uses 3B or 7B CRC variant.
// -----------------------------------------------------------------------------
uint8_t compute_final_checksum(uint32_t can_id, const uint8_t buf[8]) {
    uint8_t payload7[7];
    memcpy(payload7, &buf[1], 7);

    uint8_t seed = get_last_seed(can_id, 0x00);
    uint8_t computed = crc_compute_from_seed(seed, payload7);
    return computed;
}


// -----------------------------------------------------------------------------
// Brute-force variant: find the seed that reproduces the observed Byte0,
// update the stored seed (learn mode), and return the calculated Byte0.
// -----------------------------------------------------------------------------
uint8_t compute_final_checksum_bruteforce(uint32_t can_id, const uint8_t buf[8], bool learn) {
    uint8_t payload7[7];
    memcpy(payload7, &buf[1], 7);
    uint8_t observed = buf[0];
    uint8_t found = 0;
    bool matched = false;

    for (int s = 0; s < 256; ++s) {
        uint8_t candidate = crc_compute_from_seed((uint8_t)s, payload7);
        if (candidate == observed) {
            found = (uint8_t)s;
            matched = true;
            break;
        }
    }

    uint8_t result = crc_compute_from_seed(found, payload7);

    if (learn && matched)
        update_last_seed(can_id, found);

#if defined(DEBUG_VERBOSE_CHECKSUM_FULL)
    Serial.printf("[BRUTE] ID=0x%03X  seed=0x%02X  → computed=0x%02X  observed=0x%02X%s\n",
                  can_id, found, result, observed, matched ? " (learned)" : " (no match)");
#endif
    return result;
}



static inline uint8_t compute_ck_for_seed(uint8_t seed, const uint8_t t7[7]){
    return crc_compute_from_seed(seed, t7);
}

static bool try_mode_hit(SeedMode mode, uint8_t observed, const uint8_t t7[7],
                         const uint8_t *prev_seed_opt, uint8_t *ck_out)
{
    uint8_t seed = 0;
    switch(mode){
        case SEED_CARRY_FORWARD:
            if (!prev_seed_opt) return false;
            seed = *prev_seed_opt;
            break;
        case SEED_MIRROR_NIBBLE_B1HI:{
            uint8_t n = (t7[0] >> 4) & 0x0F;
            seed = (uint8_t)((n<<4)|n);
            break;
        }
        case SEED_FIXED_00: seed = 0x00; break;
        case SEED_FIXED_FF: seed = 0xFF; break;
        default: return false;
    }
    uint8_t ck = compute_ck_for_seed(seed, t7);
    if (ck_out) *ck_out = ck;
    return (ck == observed);
}


void seedmode_on_observation(uint32_t id, const uint8_t payload8[8]){
    if (!id_uses_7byte(id)) return;

    uint16_t slot = idslot(id);
    SeedTally &T = seed_tally_map[slot];
    T.seen++;

    // Build transformed payload7
    uint8_t p7[7]; for (int i=0;i<7;++i) p7[i]=payload8[i+1];
    PerIdCfg cfg;
    uint8_t t7[7];
    if (find_per_id_cfg(id, &cfg)) apply_mask_order(t7, p7, cfg.mask, cfg.order); else memcpy(t7, p7, 7);

    uint8_t observed = payload8[0];

    uint8_t prev_seed = 0;
    const uint8_t *prev_seed_ptr = has_last_seed(id) ? &prev_seed : nullptr;
    if (prev_seed_ptr) prev_seed = get_last_seed(id, 0x00);

    bool hit_any = false;
    uint8_t dummy;

    if (try_mode_hit(SEED_CARRY_FORWARD, observed, t7, prev_seed_ptr, &dummy)){ T.hit_carry++; hit_any = true; }
    if (try_mode_hit(SEED_MIRROR_NIBBLE_B1HI, observed, t7, nullptr, &dummy)){ T.hit_nib++;   hit_any = true; }
    if (try_mode_hit(SEED_FIXED_00, observed, t7, nullptr, &dummy)){ T.hit_fix00++; hit_any = true; }
    if (try_mode_hit(SEED_FIXED_FF, observed, t7, nullptr, &dummy)){ T.hit_fixFF++; hit_any = true; }

    // If none hit, see if *some* seed exists (truthy “CRC family” confirmation + training)
    if (!hit_any){
        for (int s=0; s<256; ++s){
            uint8_t ck = compute_ck_for_seed((uint8_t)s, t7);
            if (ck == observed){
                T.hit_brute++;
                hit_any = true;
                // Useful for continuity: store the seed used THIS time,
                // and remember that next time CARRY_FORWARD will likely match.
                update_last_seed(id, (uint8_t)s);
                break;
            }
        }
    } else {
        // If a rule matched, DO NOT overwrite last_seed with the current observed
        // byte0. Doing so makes the "previous seed" equal to the current frame's
        // checksum and causes the one-frame offset bug. We only update the last
        // seed when we successfully brute-learn a concrete seed above.
    }

    // Lock decision when confident:
    if (T.locked == SEED_UNKNOWN && T.seen >= 8) {
        // Pick the mode with the highest hits:
        uint32_t best = 0; SeedMode winner = SEED_UNKNOWN;
        struct { uint32_t hits; SeedMode mode; } cand[] = {
            {T.hit_carry, SEED_CARRY_FORWARD},
            {T.hit_nib,   SEED_MIRROR_NIBBLE_B1HI},
            {T.hit_fix00, SEED_FIXED_00},
            {T.hit_fixFF, SEED_FIXED_FF},
            {T.hit_brute, SEED_BRUTE_TRACKED}
        };
        for (auto &c : cand){ if (c.hits > best){ best = c.hits; winner = c.mode; } }
        if (best >= 5) T.locked = winner; // simple threshold
    }
}

void seedmode_print_report(){
    Serial.println(F("\n=== Seed Mode Report (7B IDs) ==="));
    for (uint16_t s=0; s<2048; ++s){
        const SeedTally &T = seed_tally_map[s];
        if (!T.seen) continue;
        Serial.printf("ID=0x%03X seen=%lu  carry=%lu nib=%lu fix00=%lu fixFF=%lu brute=%lu  -> locked=%s\n",
                      (uint32_t)s, (unsigned long)T.seen,
                      (unsigned long)T.hit_carry, (unsigned long)T.hit_nib,
                      (unsigned long)T.hit_fix00, (unsigned long)T.hit_fixFF,
                      (unsigned long)T.hit_brute,
                      seedModeName(T.locked));
    }
    Serial.println(F("=== End Report ===\n"));
}

uint8_t compute_checksum_using_mode(uint32_t id, const uint8_t payload8[8], bool allow_bruteforce){
    // 3B IDs: delegate
    if (!id_uses_7byte(id)) return computeChecksum_3B(id, payload8);

    // build transformed
    uint8_t p7[7]; for(int i=0;i<7;++i) p7[i]=payload8[i+1];
    PerIdCfg cfg; uint8_t t7[7];
    if (find_per_id_cfg(id, &cfg)) apply_mask_order(t7, p7, cfg.mask, cfg.order); else memcpy(t7, p7, 7);

    SeedMode m = seedmode_get(id);
    uint8_t seed = 0x00;

    switch(m){
        case SEED_CARRY_FORWARD:
            seed = get_last_seed(id, 0x00);
            return crc_compute_from_seed(seed, t7);
        case SEED_MIRROR_NIBBLE_B1HI:{
            uint8_t n = (t7[0] >> 4) & 0x0F;
            seed = (uint8_t)((n<<4)|n);
            return crc_compute_from_seed(seed, t7);
        }
        case SEED_FIXED_00:
            return crc_compute_from_seed(0x00, t7);
        case SEED_FIXED_FF:
            return crc_compute_from_seed(0xFF, t7);
        case SEED_BRUTE_TRACKED:
        case SEED_UNKNOWN:
        default:
            // try stored, else optional brute, else 0x00
            if (has_last_seed(id)) {
                seed = get_last_seed(id, 0x00);
                return crc_compute_from_seed(seed, t7);
            }
            if (allow_bruteforce){
                uint8_t observed = payload8[0];
                for (int s=0; s<256; ++s){
                    uint8_t ck = crc_compute_from_seed((uint8_t)s, t7);
                    if (ck == observed){ update_last_seed(id, (uint8_t)s); return ck; }
                }
            }
            return crc_compute_from_seed(0x00, t7);
    }
}











// ---------- Recent frame buffer & per-ID analyzer helpers --------------
#define RECENT_FRAMES_CAP 8192

struct RecentFrame {
    uint32_t id;
    uint8_t buf[8];
    uint32_t ts_ms;
};

static RecentFrame recent_frames[RECENT_FRAMES_CAP];
static size_t recent_head = 0;
static size_t recent_count = 0;

// call this from your CAN read path for every received frame
void record_recent_frame(const CAN_message_t &frame) {
    RecentFrame &r = recent_frames[recent_head];
    r.id = frame.id;
    memcpy(r.buf, frame.buf, 8);
    r.ts_ms = millis();
    recent_head = (recent_head + 1) % RECENT_FRAMES_CAP;
    if (recent_count < RECENT_FRAMES_CAP) ++recent_count;
}

// Clear buffer (useful before controlled recording / replay)
void clear_recent_frames() {
    recent_head = 0;
    recent_count = 0;
}

// Helper: read the n-th most recent (0 = newest)
static bool get_recent_n(size_t n, RecentFrame &out) {
    if (n >= recent_count) return false;
    size_t idx = (recent_head + RECENT_FRAMES_CAP - 1 - n) % RECENT_FRAMES_CAP;
    out = recent_frames[idx];
    return true;
}

// Compute CRC for a single frame using a seed
static uint8_t compute_ck_seed(uint8_t seed, const uint8_t payload7[7]){
    return crc_compute_from_seed(seed, payload7);
}

// Try all seed modes for a single transformed t7 and observed target
struct ModeResult {
    // counters
    uint32_t carry = 0;
    uint32_t nibble = 0;
    uint32_t fixed00 = 0;
    uint32_t fixedFF = 0;
    uint32_t brute = 0;
    // add total tested frames
    uint32_t total = 0;
};

static bool try_seed_modes_for_frame(uint32_t id, const uint8_t t7[7], uint8_t observed, ModeResult &out, uint8_t *brute_found_seed /*optional, first found*/){
    out.total++;
    bool any = false;
    uint8_t prev_seed = 0;
    bool has_prev = has_last_seed(id);
    if (has_prev) prev_seed = get_last_seed(id, 0x00);

    // carry-forward
    if (has_prev) {
        uint8_t ck = compute_ck_seed(prev_seed, t7);
        if (ck == observed) { out.carry++; any = true; }
    }
    // mirrored nibble (upper nibble of byte1 mirrored into both nibbles)
    {
        uint8_t n = (t7[0] >> 4) & 0x0F;
        uint8_t seed = (uint8_t)((n<<4)|n);
        uint8_t ck = compute_ck_seed(seed, t7);
        if (ck == observed) { out.nibble++; any = true; }
    }
    // fixed 00
    {
        uint8_t ck = compute_ck_seed(0x00, t7);
        if (ck == observed) { out.fixed00++; any = true; }
    }
    // fixed FF
    {
        uint8_t ck = compute_ck_seed(0xFF, t7);
        if (ck == observed) { out.fixedFF++; any = true; }
    }
    // brute force
    for (int s=0; s<256; ++s){
        uint8_t ck = compute_ck_seed((uint8_t)s, t7);
        if (ck == observed) {
            out.brute++;
            any = true;
            if (brute_found_seed && *brute_found_seed == 0x00) *brute_found_seed = (uint8_t)s; // capture first found
            // we don't break - count all brute matches occurrences (but we could)
            break;
        }
    }
    return any;
}

// Try mask/order combos and count successes across N observed frames for the ID
struct ComboTally {
    uint8_t mask_mode;
    uint8_t order_mode;
    ModeResult mr;
    // track a few example brute seeds discovered
    uint8_t sample_brute_seed;
};

void analyze_id_modes(uint32_t can_id, size_t max_samples_to_scan) {
    // Gather up to max_samples_to_scan most-recent frames for that ID
    size_t scanned = 0;
    size_t found = 0;
    // we'll test 3 mask modes (0=none,1=clear low nibble,2=clear high nibble) and 2 orders (0 normal,1 reversed)
    ComboTally combos[6];
    int idx = 0;
    for (uint8_t mask=0; mask<=2; ++mask) for (uint8_t order=0; order<=1; ++order) {
        combos[idx].mask_mode = mask;
        combos[idx].order_mode = order;
        combos[idx].mr = ModeResult();
        combos[idx].sample_brute_seed = 0x00;
        idx++;
    }

    for (size_t n=0; n<recent_count && scanned < RECENT_FRAMES_CAP && found < max_samples_to_scan; ++n) {
        // n is offset from newest (0 newest). iterate newest -> older
        RecentFrame rf;
        if (!get_recent_n(n, rf)) break;
        scanned++;
        if (rf.id != can_id) continue;
        found++;

        // payload bytes [1..7] -> p7
        uint8_t p7[7]; for (int i=0;i<7;++i) p7[i]=rf.buf[i+1];
        uint8_t observed = rf.buf[0];

        // for each combo, apply mask & order then try seed modes
        for (int c=0;c<6;++c){
            uint8_t t7[7];
            // copy base
            for (int i=0;i<7;++i) t7[i]=p7[i];
            // apply mask per your apply_mask_order function semantics:
            // mask==1 -> out7[0] &= 0xF0  (clear low nibble)
            // mask==2 -> out7[0] &= 0x0F  (clear high nibble)
            if (combos[c].mask_mode == 1) t7[0] &= 0xF0;
            else if (combos[c].mask_mode == 2) t7[0] &= 0x0F;
            // order_mode 1 => reverse bytes
            if (combos[c].order_mode == 1) {
                uint8_t tmp[7];
                for (int i=0;i<7;++i) tmp[i] = t7[6-i];
                memcpy(t7, tmp, 7);
            }
            uint8_t bf_seed = 0x00;
            try_seed_modes_for_frame(can_id, t7, observed, combos[c].mr, &bf_seed);
            if (bf_seed != 0x00 && combos[c].sample_brute_seed == 0x00) combos[c].sample_brute_seed = bf_seed;
        }
    }

    // Print results summary
    Serial.printf("\n=== Analyzer for ID=0x%03X (scanned=%u found=%u) ===\n", (unsigned)can_id, (unsigned)scanned, (unsigned)found);
    Serial.println("mask | order | total | carry | nibble | fix00 | fixFF | brute | brute_example");
    for (int c=0;c<6;++c) {
        const ComboTally &ct = combos[c];
        const ModeResult &m = ct.mr;
        const char *ms = (ct.mask_mode==0) ? "NONE" : (ct.mask_mode==1) ? "CLR_LO" : "CLR_HI";
        const char *os = (ct.order_mode==0) ? "NOR" : "REV";
        Serial.printf("%4s | %4s | %5u | %5u | %6u | %6u | %6u | %5u | 0x%02X\n",
                      ms, os, (unsigned)m.total, (unsigned)m.carry, (unsigned)m.nibble,
                      (unsigned)m.fixed00, (unsigned)m.fixedFF, (unsigned)m.brute, ct.sample_brute_seed);
    }

    // pick best combo by highest (carry + nibble + fixed00 + fixedFF) hits, else brute
    uint32_t best_total_hits = 0;
    int best_idx = -1;
    for (int c=0;c<6;++c){
        const ModeResult &m = combos[c].mr;
        uint32_t non_brute_hits = m.carry + m.nibble + m.fixed00 + m.fixedFF;
        if (non_brute_hits > best_total_hits) { best_total_hits = non_brute_hits; best_idx = c; }
    }
    if (best_idx >= 0) {
        const ComboTally &ct = combos[best_idx];
        const ModeResult &m = ct.mr;
        float conf = (m.total == 0) ? 0.0f : (100.0f * (float)best_total_hits / (float)m.total);
        Serial.printf("\nRecommended (non-brute) combo: mask=%s order=%s  hits=%u/%u  conf=%.1f%%\n",
                      (ct.mask_mode==0) ? "NONE" : (ct.mask_mode==1) ? "CLR_LO" : "CLR_HI",
                      (ct.order_mode==0) ? "NOR" : "REV",
                      (unsigned)best_total_hits, (unsigned)combos[best_idx].mr.total, conf);
        if (conf >= 90.0f) Serial.println("High confidence -> consider locking this combo (set in checksum_config_full.h).");
        else if (conf >= 60.0f) Serial.println("Moderate confidence -> consider using this combo but keep learning.");
        else Serial.println("Low confidence -> keep BRUTE_TRACKED / learning for now.");
    } else {
        Serial.println("\nNo non-brute winner. Most frames required brute. Keep learning.");
    }

    // show some learned seeds (scan recent again and print up to 8 unique brute-found seeds)
    uint8_t seeds_seen[16]; int seeds_n = 0;
    for (size_t n=0; n<recent_count && seeds_n < 8; ++n){
        RecentFrame rf;
        if (!get_recent_n(n, rf)) break;
        if (rf.id != can_id) continue;
        uint8_t p7[7]; for (int i=0;i<7;++i) p7[i]=rf.buf[i+1];
        uint8_t obs = rf.buf[0];
        // brute find first seed that matches
        for (int s=0;s<256;++s){
            if (crc_compute_from_seed((uint8_t)s, p7) == obs){
                bool seen=false;
                for (int k=0;k<seeds_n;k++) if (seeds_seen[k]==(uint8_t)s){seen=true;break;}
                if (!seen) { seeds_seen[seeds_n++] = (uint8_t)s; Serial.printf("example brute seed: 0x%02X\n",(unsigned)s); }
                break;
            }
        }
    }
    Serial.println("=== End analyzer ===\n");
}


// -----------------------------------------------------------------------------
// Compute checksum for the current frame using the CURRENT stored seed
// but DO NOT update the stored seed. Useful to verify incoming frames.
// -----------------------------------------------------------------------------
// Preview helper implementation
uint8_t computeChecksum_preview(uint32_t can_id, const uint8_t payload8[8]) {
    // 3B IDs: just delegate
    if (!id_uses_7byte(can_id)) {
        return computeChecksum_3B(can_id, payload8);
    }

    // build transformed payload7 (apply per-id mask/order)
    uint8_t p7[7];
    for (int i=0;i<7;++i) p7[i] = payload8[i+1];

    PerIdCfg cfg;
    uint8_t t7[7];
    if (find_per_id_cfg(can_id, &cfg)) apply_mask_order(t7, p7, cfg.mask, cfg.order);
    else memcpy(t7, p7, 7);

    // Use stored "previous" seed (do NOT change/update it)
    uint8_t prev_seed = get_last_seed(can_id, 0x00);
    return crc_compute_from_seed(prev_seed, t7);
}


#pragma endregion















// ---------------------------------------   Bare bones progmem lookups


#pragma region // 0x101 PROGMEM lookup
 
bool lookup_checksum_0x101(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x101 ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_101_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_101[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_101[i][7]));
            Serial.printf("  ✅ 0x101 MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x101 No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x101(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x101(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion


#pragma region // 0x104 PROGMEM lookup
 
bool lookup_checksum_0x104(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x104 ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_104_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_104[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_104[i][7]));
            Serial.printf("  ✅ 0x104 MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x104 No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x104(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x104(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0x116 PROGMEM lookup
 
// 0x116 PROGMEM Lookup
bool lookup_checksum_0x116(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x116 ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_116_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_116[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_116[i][7]));
            Serial.printf("  ✅ 0x116 MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x116 No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x116(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x116(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion

#pragma region // 0x116 CRC8 Calculation

uint8_t crc8_reflected_0x116(uint8_t seed, const uint8_t* data, size_t len) {
  uint8_t crc = seed;
  for (size_t i = 0; i < len; ++i) {
    crc ^= data[i];
    for (int j = 0; j < 8; ++j)
      crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x1D) : (crc << 1);
  }
  return crc ^ 0xFF;
}

// Example for CAN ID 0x116:
uint8_t compute_checksum_0x116(uint8_t b1, const uint8_t* payload) {
  uint8_t seed = 0x55 ^ b1;      // rolling seed from counter nibble
  return crc8_reflected_0x116(seed, payload + 1, 6);  // bytes2–7
}


#pragma endregion

#pragma region // 0x106 PROGMEM lookup
 
bool lookup_checksum_0x106(const uint8_t payload7[7], uint8_t *out_checksum) {
    // Serial.println(F("[lookup_checksum_0x106 ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_106_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_106[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_106[i][7]));
            // Serial.printf("  ✅ 0x106 MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x106 No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x106(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x106(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0x1AB PROGMEM lookup
 
bool lookup_checksum_0x1AB(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x1AB ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_1AB_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_1AB[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_1AB[i][7]));
            Serial.printf("  ✅ 0x1AB MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x1AB No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x1AB(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x1AB(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0x11E PROGMEM lookup
 
bool lookup_checksum_0x11E(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x11E ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_11E_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_11E[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_11E[i][7]));
            // Serial.printf("  ✅ 0x11E MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x11E No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x11E(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x11E(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0x31B PROGMEM lookup
 
bool lookup_checksum_0x31B(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x31B ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_31B_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_31B[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_31B[i][7]));
            Serial.printf("  ✅ 0x31B MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x31B No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x31B(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x31B(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0x392 PROGMEM lookup
 
bool lookup_checksum_0x392(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0x392 ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_392_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_392[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_392[i][7]));
            Serial.printf("  ✅ 0x392 MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ 0x392 No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0x392(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0x392(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion



#pragma region // 0xFD PROGMEM lookup
 
bool lookup_checksum_0xFD(const uint8_t payload7[7], uint8_t *out_checksum) {
    Serial.println(F("[lookup_checksum_0xFD ENTERED]"));

    // // Show what we’re searching for
    // Serial.print(F("  Search payload7 = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", payload7[i]);
    // }
    // Serial.println();

    // // Optionally show the first table row for context
    // Serial.print(F("  Table[0] = "));
    // for (int i = 0; i < 7; ++i) {
    //     Serial.printf("%02X ", pgm_read_byte(&(CHECKSUM_MAP_106[0][i])));
    // }
    // Serial.printf("| chk %02X\n", pgm_read_byte(&(CHECKSUM_MAP_106[0][7])));

    // Scan table
    for (size_t i = 0; i < CHECKSUM_MAP_FD_COUNT; ++i) {
        bool match = true;

        for (uint8_t b = 0; b < 7; ++b) {
            uint8_t tableVal = pgm_read_byte(&(CHECKSUM_MAP_FD[i][b]));
            if (tableVal != payload7[b]) {
                match = false;
                break;
            }
        }

        if (match) {
            *out_checksum = pgm_read_byte(&(CHECKSUM_MAP_FD[i][7]));
            Serial.printf("  ✅ MATCH at row %u → checksum 0x%02X\n", (unsigned)i, *out_checksum);
            return true;
        }
    }

    Serial.println(F("  ❌ No match found in PROGMEM table"));
    return false;
}

bool build_frame_with_checksum_0xFD(uint8_t buf[8]) {
    uint8_t payload7[7];
    for (int i = 0; i < 7; ++i)
        payload7[i] = buf[i + 1];   // extract Byte1..Byte7 from the frame


        // Serial.print("Pre-lookup payload7 = ");
        // for (int i = 0; i < 7; ++i) Serial.printf("%02X ", payload7[i]);
        // Serial.println();

        
    uint8_t checksum = 0;
    bool found = lookup_checksum_0xFD(payload7, &checksum);

    if (found) {
        buf[0] = checksum;           // write checksum into Byte0
        return true;
    }

    // fallback path (optional)
    buf[0] = 0x00;                   // or compute fallback CRC if desired
    return false;
}

#pragma endregion

