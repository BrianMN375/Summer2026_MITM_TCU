#pragma once
#ifndef PROGMEM_LOOKUP_DISPATCH_H
#define PROGMEM_LOOKUP_DISPATCH_H


// Auto-generated dispatch table for progmem 3-byte CHECKSUM_MAP arrays
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <pgmspace.h>

#include "progmem_lookup_3byte_full.h"
#include "progmem_lookup_7byte_full.h"

#ifdef CHECKSUM_PROG_ENTRIES_COUNT
// resolve_X_from_progmem(...) usages
#endif



// #define CHECKSUM_PROG_ENTRIES_COUNT (sizeof(CHECKSUM_PROG_ENTRIES)/sizeof(CHECKSUM_PROG_ENTRIES[0]))

// const size_t CHECKSUM_PROG_ENTRIES_COUNT_var = CHECKSUM_PROG_ENTRIES_COUNT;





typedef struct {
    uint32_t id;
    const void *table;  // generic pointer for either [4] or [8]
    size_t entries;
} ChecksumProgEntry;


extern const uint8_t CHECKSUM_MAP_40[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_41[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_8A[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_B1[][4] PROGMEM;
// extern const uint8_t CHECKSUM_MAP_104[][4] PROGMEM;
// extern const uint8_t CHECKSUM_MAP_106[][4] PROGMEM;
// extern const uint8_t CHECKSUM_MAP_116[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_117[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_11D[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_120[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_126[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_12E[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_130[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_18C[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_2B7[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_32A[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_366[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_380[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_385[][4] PROGMEM;
// extern const uint8_t CHECKSUM_MAP_392[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_395[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3DB[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3E5[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3E8[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3EC[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3FE[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_497[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_520[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_54B[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_584[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_585[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_5BE[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_5F0[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_641[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_65D[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_668[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_670[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_6AE[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_6B4[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_6B7[][4] PROGMEM;
extern const uint8_t CHECKSUM_MAP_6B8[][4] PROGMEM;


extern const uint8_t CHECKSUM_MAP_86[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_9F[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_A7[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_A8[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_AD[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_B2[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_FD[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_101[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_104[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_106[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_116[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_118[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_11E[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_121[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_134[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_135[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_1A2[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_1AB[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_30B[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_30D[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_31B[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_365[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_381[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_382[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_391[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_392[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3BE[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3C0[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_3C8[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_640[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_64A[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_65F[][8] PROGMEM;
extern const uint8_t CHECKSUM_MAP_6B2[][8] PROGMEM;





const ChecksumProgEntry CHECKSUM_PROG_ENTRIES[] PROGMEM = {

  // IDs which only require 3byte checksum logic
{ 0x40, (const uint8_t (*)[4])CHECKSUM_MAP_40, sizeof(CHECKSUM_MAP_40)/4 },
{ 0x41, (const uint8_t (*)[4])CHECKSUM_MAP_41, sizeof(CHECKSUM_MAP_41)/4 },
{ 0x8A, (const uint8_t (*)[4])CHECKSUM_MAP_8A, sizeof(CHECKSUM_MAP_8A)/4 },
{ 0xB1, (const uint8_t (*)[4])CHECKSUM_MAP_B1, sizeof(CHECKSUM_MAP_B1)/4 },
// { 0x104, (const uint8_t (*)[4])CHECKSUM_MAP_104, sizeof(CHECKSUM_MAP_104)/4 },
// { 0x106, (const uint8_t (*)[4])CHECKSUM_MAP_106, sizeof(CHECKSUM_MAP_106)/4 },
// { 0x116, (const uint8_t (*)[4])CHECKSUM_MAP_116, sizeof(CHECKSUM_MAP_116)/4 },
{ 0x117, (const uint8_t (*)[4])CHECKSUM_MAP_117, sizeof(CHECKSUM_MAP_117)/4 },
{ 0x11D, (const uint8_t (*)[4])CHECKSUM_MAP_11D, sizeof(CHECKSUM_MAP_11D)/4 },
{ 0x120, (const uint8_t (*)[4])CHECKSUM_MAP_120, sizeof(CHECKSUM_MAP_120)/4 },
{ 0x126, (const uint8_t (*)[4])CHECKSUM_MAP_126, sizeof(CHECKSUM_MAP_126)/4 },
{ 0x12E, (const uint8_t (*)[4])CHECKSUM_MAP_12E, sizeof(CHECKSUM_MAP_12E)/4 },
{ 0x130, (const uint8_t (*)[4])CHECKSUM_MAP_130, sizeof(CHECKSUM_MAP_130)/4 },
{ 0x18C, (const uint8_t (*)[4])CHECKSUM_MAP_18C, sizeof(CHECKSUM_MAP_18C)/4 },
{ 0x2B7, (const uint8_t (*)[4])CHECKSUM_MAP_2B7, sizeof(CHECKSUM_MAP_2B7)/4 },
{ 0x32A, (const uint8_t (*)[4])CHECKSUM_MAP_32A, sizeof(CHECKSUM_MAP_32A)/4 },
{ 0x366, (const uint8_t (*)[4])CHECKSUM_MAP_366, sizeof(CHECKSUM_MAP_366)/4 },
{ 0x380, (const uint8_t (*)[4])CHECKSUM_MAP_380, sizeof(CHECKSUM_MAP_380)/4 },
{ 0x385, (const uint8_t (*)[4])CHECKSUM_MAP_385, sizeof(CHECKSUM_MAP_385)/4 },
// { 0x392, (const uint8_t (*)[4])CHECKSUM_MAP_392, sizeof(CHECKSUM_MAP_392)/4 },
{ 0x395, (const uint8_t (*)[4])CHECKSUM_MAP_395, sizeof(CHECKSUM_MAP_395)/4 },
{ 0x3DB, (const uint8_t (*)[4])CHECKSUM_MAP_3DB, sizeof(CHECKSUM_MAP_3DB)/4 },
{ 0x3E5, (const uint8_t (*)[4])CHECKSUM_MAP_3E5, sizeof(CHECKSUM_MAP_3E5)/4 },
{ 0x3E8, (const uint8_t (*)[4])CHECKSUM_MAP_3E8, sizeof(CHECKSUM_MAP_3E8)/4 },
{ 0x3EC, (const uint8_t (*)[4])CHECKSUM_MAP_3EC, sizeof(CHECKSUM_MAP_3EC)/4 },
{ 0x3FE, (const uint8_t (*)[4])CHECKSUM_MAP_3FE, sizeof(CHECKSUM_MAP_3FE)/4 },
{ 0x497, (const uint8_t (*)[4])CHECKSUM_MAP_497, sizeof(CHECKSUM_MAP_497)/4 },
{ 0x520, (const uint8_t (*)[4])CHECKSUM_MAP_520, sizeof(CHECKSUM_MAP_520)/4 },
{ 0x54B, (const uint8_t (*)[4])CHECKSUM_MAP_54B, sizeof(CHECKSUM_MAP_54B)/4 },
{ 0x584, (const uint8_t (*)[4])CHECKSUM_MAP_584, sizeof(CHECKSUM_MAP_584)/4 },
{ 0x585, (const uint8_t (*)[4])CHECKSUM_MAP_585, sizeof(CHECKSUM_MAP_585)/4 },
{ 0x5BE, (const uint8_t (*)[4])CHECKSUM_MAP_5BE, sizeof(CHECKSUM_MAP_5BE)/4 },
{ 0x5F0, (const uint8_t (*)[4])CHECKSUM_MAP_5F0, sizeof(CHECKSUM_MAP_5F0)/4 },
{ 0x641, (const uint8_t (*)[4])CHECKSUM_MAP_641, sizeof(CHECKSUM_MAP_641)/4 },
{ 0x65D, (const uint8_t (*)[4])CHECKSUM_MAP_65D, sizeof(CHECKSUM_MAP_65D)/4 },
{ 0x668, (const uint8_t (*)[4])CHECKSUM_MAP_668, sizeof(CHECKSUM_MAP_668)/4 },
{ 0x670, (const uint8_t (*)[4])CHECKSUM_MAP_670, sizeof(CHECKSUM_MAP_670)/4 },
{ 0x6AE, (const uint8_t (*)[4])CHECKSUM_MAP_6AE, sizeof(CHECKSUM_MAP_6AE)/4 },
{ 0x6B4, (const uint8_t (*)[4])CHECKSUM_MAP_6B4, sizeof(CHECKSUM_MAP_6B4)/4 },
{ 0x6B7, (const uint8_t (*)[4])CHECKSUM_MAP_6B7, sizeof(CHECKSUM_MAP_6B7)/4 },
{ 0x6B8, (const uint8_t (*)[4])CHECKSUM_MAP_6B8, sizeof(CHECKSUM_MAP_6B8)/4 }, 


{ 0x86, (const uint8_t (*)[8]) CHECKSUM_MAP_86, sizeof(CHECKSUM_MAP_86)/8 },
{ 0x9F, (const uint8_t (*)[8]) CHECKSUM_MAP_9F, sizeof(CHECKSUM_MAP_9F)/8 },
{ 0xA7, (const uint8_t (*)[8]) CHECKSUM_MAP_A7, sizeof(CHECKSUM_MAP_A7)/8 },
{ 0xA8, (const uint8_t (*)[8]) CHECKSUM_MAP_A8, sizeof(CHECKSUM_MAP_A8)/8 },
{ 0xAD, (const uint8_t (*)[8]) CHECKSUM_MAP_AD, sizeof(CHECKSUM_MAP_AD)/8 },
{ 0xB2, (const uint8_t (*)[8]) CHECKSUM_MAP_B2, sizeof(CHECKSUM_MAP_B2)/8 },
{ 0xFD, (const uint8_t (*)[8]) CHECKSUM_MAP_FD, sizeof(CHECKSUM_MAP_FD)/8 },
{ 0x101, (const uint8_t (*)[8])CHECKSUM_MAP_101, sizeof(CHECKSUM_MAP_101)/8 },
{ 0x104, (const uint8_t (*)[8])CHECKSUM_MAP_104, sizeof(CHECKSUM_MAP_104)/8 },
{ 0x106, (const uint8_t (*)[8])CHECKSUM_MAP_106, sizeof(CHECKSUM_MAP_106)/8 },
{ 0x116, (const uint8_t (*)[8])CHECKSUM_MAP_116, sizeof(CHECKSUM_MAP_116)/8 },
{ 0x118, (const uint8_t (*)[8])CHECKSUM_MAP_118, sizeof(CHECKSUM_MAP_118)/8 },
{ 0x11E, (const uint8_t (*)[8])CHECKSUM_MAP_11E, sizeof(CHECKSUM_MAP_11E)/8 },
{ 0x121, (const uint8_t (*)[8])CHECKSUM_MAP_121, sizeof(CHECKSUM_MAP_121)/8 },
{ 0x134, (const uint8_t (*)[8])CHECKSUM_MAP_134, sizeof(CHECKSUM_MAP_134)/8 },
{ 0x135, (const uint8_t (*)[8])CHECKSUM_MAP_135, sizeof(CHECKSUM_MAP_135)/8 },
{ 0x1A2, (const uint8_t (*)[8])CHECKSUM_MAP_1A2, sizeof(CHECKSUM_MAP_1A2)/8 },
{ 0x1AB, (const uint8_t (*)[8])CHECKSUM_MAP_1AB, sizeof(CHECKSUM_MAP_1AB)/8 },
{ 0x30B, (const uint8_t (*)[8])CHECKSUM_MAP_30B, sizeof(CHECKSUM_MAP_30B)/8 },
{ 0x30D, (const uint8_t (*)[8])CHECKSUM_MAP_30D, sizeof(CHECKSUM_MAP_30D)/8 },
{ 0x31B, (const uint8_t (*)[8])CHECKSUM_MAP_31B, sizeof(CHECKSUM_MAP_31B)/8 },
{ 0x365, (const uint8_t (*)[8])CHECKSUM_MAP_365, sizeof(CHECKSUM_MAP_365)/8 },
{ 0x381, (const uint8_t (*)[8])CHECKSUM_MAP_381, sizeof(CHECKSUM_MAP_381)/8 },
{ 0x382, (const uint8_t (*)[8])CHECKSUM_MAP_382, sizeof(CHECKSUM_MAP_382)/8 },
{ 0x391, (const uint8_t (*)[8])CHECKSUM_MAP_391, sizeof(CHECKSUM_MAP_391)/8 },
{ 0x392, (const uint8_t (*)[8])CHECKSUM_MAP_392, sizeof(CHECKSUM_MAP_392)/8 },
{ 0x3BE, (const uint8_t (*)[8])CHECKSUM_MAP_3BE, sizeof(CHECKSUM_MAP_3BE)/8 },
{ 0x3C0, (const uint8_t (*)[8])CHECKSUM_MAP_3C0, sizeof(CHECKSUM_MAP_3C0)/8 },
{ 0x3C8, (const uint8_t (*)[8])CHECKSUM_MAP_3C8, sizeof(CHECKSUM_MAP_3C8)/8 },
{ 0x640, (const uint8_t (*)[8])CHECKSUM_MAP_640, sizeof(CHECKSUM_MAP_640)/8 },
{ 0x64A, (const uint8_t (*)[8])CHECKSUM_MAP_64A, sizeof(CHECKSUM_MAP_64A)/8 },
{ 0x65F, (const uint8_t (*)[8])CHECKSUM_MAP_65F, sizeof(CHECKSUM_MAP_65F)/8 },
{ 0x6B2, (const uint8_t (*)[8])CHECKSUM_MAP_6B2, sizeof(CHECKSUM_MAP_6B2)/8 },





};



// #define CHECKSUM_PROG_ENTRIES_COUNT (sizeof(CHECKSUM_PROG_ENTRIES)/sizeof(CHECKSUM_PROG_ENTRIES[0]))

// const size_t CHECKSUM_PROG_ENTRIES_COUNT = sizeof(CHECKSUM_PROG_ENTRIES)/sizeof(CHECKSUM_PROG_ENTRIES[0]);




#endif // PROGMEM_LOOKUP_DISPATCH_H
