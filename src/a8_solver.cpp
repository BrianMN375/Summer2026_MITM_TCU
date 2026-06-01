#include "a8_solver.h"

// ------------------------------------------------------------------
// Counter transform table
// ------------------------------------------------------------------

static const uint8_t COUNTER_TRANSFORM[16] =
{
    0x00, 0x7A, 0x96, 0x79,
    0xD5, 0xBF, 0x8F, 0x9A,
    0xB8, 0xCA, 0x95, 0x58,
    0x46, 0x55, 0xE7, 0x68
};

// ------------------------------------------------------------------
// Linear vectors
// ------------------------------------------------------------------

static uint8_t getVector(
    uint8_t byteIdx,
    uint8_t bitIdx)
{
    switch ((byteIdx << 8) | bitIdx)
    {
        case 0x0205: return 0x9B;
        case 0x0206: return 0x19;
        case 0x0207: return 0x32;

        case 0x0300: return 0x54;
        case 0x0301: return 0xA8;
        case 0x0302: return 0x7F;
        case 0x0303: return 0xFE;
        case 0x0306: return 0x3D;
        case 0x0307: return 0x7A;

        case 0x0400: return 0xAD;
        case 0x0401: return 0x75;
        case 0x0402: return 0xEA;
        case 0x0403: return 0xFB;

        case 0x0601: return 0x1C;
        case 0x0602: return 0x38;
        case 0x0603: return 0x70;
        case 0x0604: return 0xE0;
        case 0x0605: return 0xEF;
        case 0x0606: return 0xF1;
        case 0x0607: return 0xCD;

        case 0x0700: return 0xE9;
        case 0x0701: return 0xFD;
        case 0x0702: return 0xD5;
        case 0x0703: return 0x85;
        case 0x0704: return 0x25;

        default:
            return 0;
    }
}

// ------------------------------------------------------------------

void decodeSignals_0xA8(
    const uint8_t frame[8],
    float& rpm,
    uint16_t& stat,
    int16_t& dyn)
{
    uint64_t raw = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        raw |=
            ((uint64_t)frame[i])
            << (8 * i);
    }

    rpm =
        ((raw >> 48) & 0xFFFF) * 0.25f;

    stat =
        (raw >> 21) & 0x1FF;

    dyn =
        ((raw >> 30) & 0x3FF) - 509;
}

// ------------------------------------------------------------------

uint8_t actualResidual_0xA8(
    const uint8_t frame[8])
{
    uint8_t checksum =
        frame[0];

    uint8_t counter =
        frame[1] & 0x0F;

    uint8_t family =
        checksum ^
        COUNTER_TRANSFORM[counter];

    uint8_t linear = 0;

    for (uint8_t byteIdx = 2;
         byteIdx < 8;
         byteIdx++)
    {
        uint8_t value =
            frame[byteIdx];

        for (uint8_t bitIdx = 0;
             bitIdx < 8;
             bitIdx++)
        {
            if (value &
                (1 << bitIdx))
            {
                linear ^=
                    getVector(
                        byteIdx,
                        bitIdx);
            }
        }
    }

    return family ^ linear;
}

// ------------------------------------------------------------------

uint8_t predictedResidual_0xA8(
    const uint8_t frame[8])
{
    uint8_t byte4 = frame[4];
    uint8_t byte5 = frame[5];
    uint8_t byte6 = frame[6];
    uint8_t byte7 = frame[7];

    float rpm;
    uint16_t stat;
    int16_t dyn;

    decodeSignals_0xA8(
        frame,
        rpm,
        stat,
        dyn);

    // ----------------------------------------------------------
    // Startup family
    // ----------------------------------------------------------

    if (
        byte5 == 0x00 &&
        byte6 == 0x00 &&
        byte7 == 0x00)
    {
        if (byte4 == 0xFF)
            return 0x0D;

        if (
            byte4 >= 0x76 &&
            byte4 <= 0x7F)
            return 0xB4;
    }

    uint8_t predicted;

    // ----------------------------------------------------------

    if (byte4 == 0xFF)
    {
        predicted = 0x0D;
    }

    else if (
        byte4 >= 0xB0 &&
        byte4 <= 0xBF)
    {
        predicted = 0x5D;
    }

    else if (
        byte4 >= 0xA0 &&
        byte4 <= 0xAF)
    {
        predicted = 0x84;
    }

    else if (
        byte4 >= 0x91 &&
        byte4 <= 0x98 &&
        byte5 == 0x64 &&
        stat >= 247 &&
        dyn >= 72)
    {
        predicted = 0xC0;
    }

    else if (
        byte4 >= 0x99 &&
        byte4 <= 0x9F &&
        byte7 >= 0x40)
    {
        predicted = 0xC0;
    }

    else if (
        byte4 >= 0x90 &&
        byte4 <= 0x9F)
    {
        predicted = 0x1E;
    }

    else
    {
        bool neg =
            dyn < 3;

        bool high =
            rpm >= 2048;

        bool mid =
            stat >= 128;

        if (!high)
        {
            if (!mid)
                predicted =
                    neg ? 0x25 : 0x5E;
            else
                predicted =
                    neg ? 0xF6 : 0x8D;
        }
        else
        {
            if (neg)
            {
                predicted = 0xBC;
            }
            else
            {
                if (
                    byte5 == 0x64 &&
                    stat >= 121 &&
                    stat <= 127)
                {
                    predicted = 0x14;
                }
                else if (
                    byte5 == 0x64 &&
                    byte4 >= 0x80 &&
                    byte4 <= 0x86 &&
                    byte7 >= 0x4C)
                {
                    predicted = 0x19;
                }
                else
                {
                    predicted = 0xC7;
                }
            }
        }
    }

    // ----------------------------------------------------------
    // Alternate families
    // ----------------------------------------------------------

    if (byte5 == 0x63)
    {
        if (
            predicted == 0x25 ||
            predicted == 0xF6 ||
            predicted == 0xBC ||
            predicted == 0xC7)
        {
            predicted ^= 0x7A;
        }
    }

    if (byte5 == 0x62)
    {
        if (
            predicted == 0x25 ||
            predicted == 0xBC ||
            predicted == 0xC7)
        {
            predicted ^= 0xCF;
        }
    }

    if (byte5 == 0x61)
    {
        if (
            predicted == 0x25 ||
            predicted == 0xBC ||
            predicted == 0xC7)
        {
            predicted ^= 0x3F;
        }
    }

    if (byte5 == 0x60)
    {
        if (
            predicted == 0x25 ||
            predicted == 0xBC ||
            predicted == 0xC7)
        {
            predicted ^= 0x8A;
        }
    }

    // ----------------------------------------------------------
    // High negative overlay
    // ----------------------------------------------------------

    if (predicted == 0xBC)
    {
        if (
            (byte7 & 0x60) == 0x40)
        {
            if (stat < 256)
                predicted = 0x62;
            else
                predicted = 0x38;
        }
    }

    return predicted;
}

// ------------------------------------------------------------------

bool verifyChecksum_0xA8(
    const uint8_t frame[8])
{
    return
        actualResidual_0xA8(frame)
        ==
        predictedResidual_0xA8(frame);
}