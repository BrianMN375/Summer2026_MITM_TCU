#pragma once
#include <stdint.h>
#include <FlexCAN_T4.h>
#include "checksum_types.h"

// Forward declare CAN_message_t (avoids FlexCAN_T4 dependency)

// -----------------------------------------------------------------------------
// Seed mode classification / analysis
// -----------------------------------------------------------------------------


// Human-readable name for a SeedMode
const char* seedModeName(SeedMode m);

// Seed-mode analysis functions (implemented in checksum.cpp)
void seedmode_on_observation(uint32_t id, const uint8_t payload8[8]);
SeedMode seedmode_get(uint32_t id);
void seedmode_force(uint32_t id, SeedMode mode);
void seedmode_print_report();
uint8_t compute_checksum_using_mode(uint32_t id, const uint8_t payload8[8], bool allow_bruteforce);


struct CAN_message_t;


#pragma region // Old/Archive/Not currently used for bare-bones progmem lookups


// -----------------------------------------------------------------------------
// Debug printing
// -----------------------------------------------------------------------------
void debug_print_frame_CAN2_Rx_with_checksum(const CAN_message_t &frame);
void debug_print_frame_CAN2_Tx_with_checksum(const CAN_message_t &frame);
void debug_print_frame_CAN3_Rx_with_checksum(const CAN_message_t &frame);
void debug_print_frame_CAN3_Tx_with_checksum(const CAN_message_t &frame);



// -----------------------------------------------------------------------------
// Seed / analyzer helpers (added 2025-11-02)
// -----------------------------------------------------------------------------
void record_recent_frame(const CAN_message_t &frame);
void clear_recent_frames(void);
void analyze_id_modes(uint32_t can_id, size_t max_samples_to_scan = 500);

void seed_last_seed_dump(void);


// -----------------------------------------------------------------------------
// Core CRC / checksum API
// -----------------------------------------------------------------------------
uint8_t crc_compute_from_seed(uint8_t seed, const uint8_t payload7[7]);
uint8_t computeChecksum(uint32_t can_id, const uint8_t payload[8]);
uint8_t computeChecksum_3B(uint32_t can_id, const uint8_t payload[8]);
uint8_t computeChecksum_7B(uint32_t can_id, const uint8_t payload[8]);
bool    validateChecksum(uint32_t can_id, const uint8_t payload[8]);

bool recomputeChecksum_7B(uint32_t can_id, uint8_t buf[8]);  // Convenience helper: recompute & apply correct Byte0 for 7-byte CRC frames

void modify_and_fix_checksum(CAN_message_t &msg, int byte_index_1_to_7, int8_t delta);

void build_frame_with_checksum_3Byte(CAN_message_t &msg,
                               uint32_t can_id,
                               const uint8_t payload1_to_7[7]);


// void build_frame_with_checksum_0x101(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x104(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x106(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);





// void build_frame_with_checksum_0x116(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x11E(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0xFD(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x1AB(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x31B(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);


// void build_frame_with_checksum_0x3C8(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);


// void build_frame_with_checksum_0x391(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);

// void build_frame_with_checksum_0x392(CAN_message_t &msg,
//                                uint32_t can_id,
//                                const uint8_t payload1_to_7[7]);




// -----------------------------------------------------------------------------
// Resolution / correction
// -----------------------------------------------------------------------------
bool checksum_resolve_X(uint32_t can_id, const uint8_t payload7[7],
                        uint8_t checksum_target, uint8_t *resolved);
uint8_t resolve_X_from_progmem(uint32_t can_id, const uint8_t payload[8]);

// -----------------------------------------------------------------------------
// Config helpers
// -----------------------------------------------------------------------------
void apply_mask_order(uint8_t *payload7_out,
                      const uint8_t *payload7_in,
                      uint8_t mask_mode, uint8_t order_mode);

// -----------------------------------------------------------------------------
// Optional runtime seed tracking (currently no-ops)
// -----------------------------------------------------------------------------
void    update_last_seed(uint32_t id, uint8_t seed);
uint8_t get_last_seed(uint32_t id, uint8_t fallback);




// -----------------------------------------------------------------------------
// Checksum update helpers
// -----------------------------------------------------------------------------





// ✅ Full legacy signature (matches calls in can_mitm.cpp)
bool checksum_update_byte0(uint32_t can_id,
                           const uint8_t payload7[7],
                           uint8_t *byte0_out);

// ✅ Modern inline wrapper for simpler usage with full 8-byte payloads
inline uint8_t checksum_update_byte0(uint32_t can_id, uint8_t payload[8]) {
    uint8_t newb0 = computeChecksum(can_id, payload);
    payload[0] = newb0;
    return newb0;
}


// -----------------------------------------------------------------------------
// Per-ID 7-byte checksum update helpers
// -----------------------------------------------------------------------------
bool checksum_update_byte0_0xFD(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x101(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x104(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x106(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x116(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x11E(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x1AB(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x31B(const uint8_t payload7[7], uint8_t *byte0_out);
bool checksum_update_byte0_0x392(const uint8_t payload7[7], uint8_t *byte0_out);



// -----------------------------------------------------------------------------
// ID helper utilities
// -----------------------------------------------------------------------------
bool id_uses_7byte(uint32_t can_id);



// Learn dynamic seed from an observed 8B frame (brute-verifies too)
void learn_seed_from_observed(uint32_t id, const uint8_t payload8[8]);

// Optional: direct set when you trust carry-forward = previous byte0
void update_last_seed(uint32_t id, uint8_t seed);
bool has_last_seed(uint32_t id);


// CRC, lookup and brute-force modifier routines
uint8_t crc8_reflected(const uint8_t *data, size_t len, uint8_t init=0x00, uint8_t xorout=0x55);
uint8_t find_modifier_bruteforce_target(const uint8_t payload7[7], uint8_t target_checksum);
uint8_t lookup_modifier_3b(uint16_t can_id, const uint8_t key3[3], bool &found);

// MITM helper: process message, optionally preserve original checksum or recompute.
// If preserve_checksum == true, will attempt to find modifier X so that crc(payload7 + X) == original checksum.
// If preserve_checksum == false, it will attempt to lookup modifier for new payload and compute checksum accordingly.
// Writes resulting frame to outbus.
void mitm_forward_with_checksum(CAN_message_t&msg_in, FlexCAN_T4_Base &outbus, bool preserve_checksum=false);



///////////////////////////////////////////////////////////////////

/* Validate a received 8-byte frame:
 * - payload7 is bytes[1..7]
 * - recvChk is byte0 (observed checksum)
 * - foundX (out) contains the discovered X when function returns true
 * - usedBruteforce (out) is set true when a BF search was used
 */
bool vw_validate_checksum(const uint8_t payload7[7],
                          uint8_t recvChk,
                          uint8_t *foundX,
                          bool *usedBruteforce);


 // -----------------------------------------------------------------------------
// Compute final checksum helpers
// -----------------------------------------------------------------------------
// uint8_t compute_final_checksum(uint32_t can_id, const uint8_t payload[8]);
// // Brute-force variant: if store_found_seed==true, tries seeds 0..255 to match observed payload[0]
// // and stores the discovered seed with update_last_seed(can_id, seed).
// // Returns the computed checksum (what would be placed in payload[0]) in all cases.
// uint8_t compute_final_checksum_bruteforce(uint32_t can_id, const uint8_t payload[8], bool store_found_seed=false);

                         
uint8_t compute_final_checksum(uint32_t can_id, const uint8_t buf[8]);
uint8_t compute_final_checksum_bruteforce(uint32_t can_id, const uint8_t buf[8], bool learn);

// -----------------------------------------------------------------------------
// Preview helper: compute checksum using current stored seed, but do NOT
// update or learn any seed state.  Useful for verifying incoming frames.
// -----------------------------------------------------------------------------
// Preview helper: compute checksum using the CURRENT stored seed but DO NOT
// update or learn any seed state. Useful to verify an incoming frame.
uint8_t computeChecksum_preview(uint32_t can_id, const uint8_t payload8[8]);




#pragma endregion








#pragma region // 0x101
bool build_frame_with_checksum_0x101(uint8_t buf[8]);
bool lookup_checksum_0x101(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x104

bool build_frame_with_checksum_0x104(uint8_t buf[8]);
bool lookup_checksum_0x104(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x106
bool build_frame_with_checksum_0x106(uint8_t buf[8]);
bool lookup_checksum_0x106(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x116
bool build_frame_with_checksum_0x116(uint8_t buf[8]);
bool lookup_checksum_0x116(const uint8_t payload7[7], uint8_t *out_checksum);

// -----------------------------------------------------------------------------
//  Checksum function for CAN ID 0x116  (Audi RS3 comfort/suspension bus)
//  Verified 100% — CRC-8 poly=0x1D, reflected, init = 0x55 ^ Byte1, xorout = 0xFF
//  Byte layout: [0]=checksum, [1]=counter nibble, [2..7]=payload
// -----------------------------------------------------------------------------

static inline uint8_t crc8_reflected_0x1D(uint8_t seed, const uint8_t *data, size_t len)
{
    uint8_t crc = seed;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j)
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x1D) : (crc << 1);
    }
    return crc ^ 0xFF;
}

// Compute checksum for CAN ID 0x116
static inline uint8_t compute_checksum_0x116(const uint8_t payload8[8])
{
    // payload8[1] is Byte1 (rolling counter)
    uint8_t seed = 0x55 ^ payload8[1];
    // Run CRC on Bytes2..7  (6 bytes of data)
    return crc8_reflected_0x1D(seed, &payload8[2], 6);
}


// // example usage of CRC8 Calculation usage:
// if (msg.id == 0x116) {
//     msg.buf[0] = compute_checksum_0x116(msg.buf);
// }


#pragma endregion

#pragma region // 0x11E
bool build_frame_with_checksum_0x11E(uint8_t buf[8]);
bool lookup_checksum_0x11E(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x1AB
bool build_frame_with_checksum_0x1AB(uint8_t buf[8]);
bool lookup_checksum_0x1AB(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x392
bool build_frame_with_checksum_0x392(uint8_t buf[8]);
bool lookup_checksum_0x392(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0x31B
bool build_frame_with_checksum_0x31B(uint8_t buf[8]);
bool lookup_checksum_0x31B(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

#pragma region // 0xFD
bool build_frame_with_checksum_0xFD(uint8_t buf[8]);
bool lookup_checksum_0xFD(const uint8_t payload7[7], uint8_t *out_checksum);
#pragma endregion

