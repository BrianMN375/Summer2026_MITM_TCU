#pragma once
#include <stdint.h>
#include <stdbool.h>

uint8_t crc8_reflected(const uint8_t *data, size_t len, uint8_t init=0x00, uint8_t xorout=0x55);
uint8_t find_modifier_bruteforce(const uint8_t payload7[7], uint8_t target_checksum);
uint8_t lookup_modifier_3b(uint16_t can_id, const uint8_t key3[3], bool &found);
