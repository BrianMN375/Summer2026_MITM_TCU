#pragma once

#include <Arduino.h>

// ------------------------------------------------------------------
// A8 residual model
// ------------------------------------------------------------------

uint8_t actualResidual_0xA8(
    const uint8_t frame[8]);

uint8_t predictedResidual_0xA8(
    const uint8_t frame[8]);

bool verifyChecksum_0xA8(
    const uint8_t frame[8]);

// ------------------------------------------------------------------
// Signal decoder
// ------------------------------------------------------------------

void decodeSignals_0xA8(
    const uint8_t frame[8],
    float& rpm,
    uint16_t& stat,
    int16_t& dyn);