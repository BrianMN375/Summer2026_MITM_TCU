#pragma once

#include <FlexCAN_T4.h>

uint8_t actualResidual(
    const CAN_message_t& msg);

uint8_t predictedResidual(
    const CAN_message_t& msg);

void verifyA8Residual(
    const CAN_message_t& msg);