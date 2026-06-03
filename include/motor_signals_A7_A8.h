#pragma once
// =============================================================================
//  motor_signals_A7_A8.h
//
//  DBC signal encode / decode for:
//    0x0A7  Motor_11  (Motor_Diesel_MQB)
//    0x0A8  Motor_12  (Motor_Diesel_MQB)
//
//  DBC layout (all signals @1+ = little-endian / Intel byte order, unsigned raw):
//
//  Motor_11 (0xA7):
//    Bit  0.. 7  Motor_11_CRC               → buf[0]  (checksum, managed separately)
//    Bit  8..11  Motor_11_BZ                → buf[1] bits 0-3  (4-bit rolling counter)
//    Bit 12..21  MO11_EngineTqTargetRaw     → factor=1, offset=-509  (-509..+509 Nm)
//    Bit 22..31  MO11_EngineTqActual        → factor=1, offset=-509  (-509..+509 Nm)
//    Bit 32..41  MO11_EngineTotalMomentsInertia → factor=1, offset=-509
//    Bit 42..51  MO11_EngineTqTargetFiltered→ factor=1, offset=-509
//    Bit 52..60  MO11_EngineTqThrust        → factor=1, offset=-509  (9-bit, -509..0)
//    Bit 61      MO11_Status_NormalOperation_01
//    Bit 62      MO11_First_ImprecisionThreshold
//    Bit 63      MO11_QBit_EngineTq
//
//  Motor_12 (0xA8):
//    Bit  0.. 7  Motor_12_CRC               → buf[0]
//    Bit  8..11  Motor_12_BZ                → 4-bit rolling counter
//    Bit 12..20  MO12_EngineTq_Neg_Available→ factor=1, offset=-509  (9-bit, -509..0)
//    Bit 21..29  MO12_EngineTqLimit_Stat    → factor=1, offset=0     (0..509 Nm)
//    Bit 30..39  MO12_EngineTqLimit_Dyn     → factor=1, offset=-509  (-509..+509 Nm)
//    Bit 40..46  MO12_EngineTqPercent       → factor=1, offset=0     (0..100 %)
//    Bit 47      MO12_QBit_EngineRPM
//    Bit 48..63  MO12_EngineRPM             → factor=0.25, offset=0  (0..16383 RPM)
//
//  API:
//    Motor11_Signals  motor11_decode(const uint8_t buf[8]);
//    void             motor11_encode(uint8_t buf[8], const Motor11_Signals &s);
//    Motor12_Signals  motor12_decode(const uint8_t buf[8]);
//    void             motor12_encode(uint8_t buf[8], const Motor12_Signals &s);
//
//  MITM pattern (after #include "checksum_0xA7_0xA8.h"):
//    // Receive
//    Motor11_Signals s = motor11_decode(msg.buf);
//    // Modify
//    s.EngineTqActual = new_torque_nm;
//    // Re-encode + checksum (BZ counter is preserved automatically)
//    motor11_encode(msg.buf, s);
//    build_frame_with_checksum_0xA7(msg.buf);
//    // Forward
//    CAN3.write(msg);
// =============================================================================

#include <stdint.h>

// ---------------------------------------------------------------------------
// Helpers: clamp to valid raw range
// ---------------------------------------------------------------------------
static inline uint16_t _clamp16(int32_t v, uint16_t lo, uint16_t hi) {
    if (v < (int32_t)lo) return lo;
    if (v > (int32_t)hi) return hi;
    return (uint16_t)v;
}

// ---------------------------------------------------------------------------
//  Motor_11  (CAN ID 0x0A7)
// ---------------------------------------------------------------------------

struct Motor11_Signals {
    // Motor_11_BZ: 4-bit rolling counter (0-15) — preserved as-is during MITM
    uint8_t  BZ;                         // bits  8-11

    // Torque signals — physical unit: Newton-metres
    // raw = physical + 509  (10-bit, 0-1023);  physical range -509..+509
    int16_t  EngineTqTargetRaw;          // bits 12-21  raw[0..1023], phys -509..+509 Nm
    int16_t  EngineTqActual;             // bits 22-31  raw[0..1023], phys -509..+509 Nm
    int16_t  EngineTotalMomentsInertia;  // bits 32-41  raw[0..1023], phys -509..+509 Nm
    int16_t  EngineTqTargetFiltered;     // bits 42-51  raw[0..1023], phys -509..+509 Nm
    int16_t  EngineTqThrust;             // bits 52-60  raw[0..511 ], phys -509..  0 Nm (9-bit)

    // Status bits
    uint8_t  Status_NormalOperation_01;  // bit 61
    uint8_t  First_ImprecisionThreshold; // bit 62
    uint8_t  QBit_EngineTq;             // bit 63
};

// Decode: unpack raw buf[8] → Motor11_Signals
// buf[0] (CRC) and buf[1] upper nibble (BZ high bits, none here) are handled.
inline Motor11_Signals motor11_decode(const uint8_t buf[8])
{
    Motor11_Signals s;

    // BZ: bits 8-11 = lower nibble of byte 1
    s.BZ = buf[1] & 0x0F;

    // MO11_EngineTqTargetRaw: bits 12-21
    //   bits 12-15 = upper nibble of byte 1
    //   bits 16-21 = bits 0-5 of byte 2
    {
        uint16_t raw = (uint16_t)(buf[1] >> 4)
                     | ((uint16_t)(buf[2] & 0x3F) << 4);
        s.EngineTqTargetRaw = (int16_t)raw - 509;
    }

    // MO11_EngineTqActual: bits 22-31
    //   bits 22-23 = bits 6-7 of byte 2
    //   bits 24-31 = byte 3
    {
        uint16_t raw = (uint16_t)((buf[2] >> 6) & 0x03)
                     | ((uint16_t)buf[3] << 2);
        s.EngineTqActual = (int16_t)raw - 509;
    }

    // MO11_EngineTotalMomentsInertia: bits 32-41
    //   bits 32-39 = byte 4
    //   bits 40-41 = bits 0-1 of byte 5
    {
        uint16_t raw = (uint16_t)buf[4]
                     | ((uint16_t)(buf[5] & 0x03) << 8);
        s.EngineTotalMomentsInertia = (int16_t)raw - 509;
    }

    // MO11_EngineTqTargetFiltered: bits 42-51
    //   bits 42-47 = bits 2-7 of byte 5
    //   bits 48-51 = bits 0-3 of byte 6
    {
        uint16_t raw = (uint16_t)((buf[5] >> 2) & 0x3F)
                     | ((uint16_t)(buf[6] & 0x0F) << 6);
        s.EngineTqTargetFiltered = (int16_t)raw - 509;
    }

    // MO11_EngineTqThrust: bits 52-60  (9-bit)
    //   bits 52-55 = bits 4-7 of byte 6
    //   bits 56-60 = bits 0-4 of byte 7
    {
        uint16_t raw = (uint16_t)((buf[6] >> 4) & 0x0F)
                     | ((uint16_t)(buf[7] & 0x1F) << 4);
        s.EngineTqThrust = (int16_t)raw - 509;
    }

    // Status bits in byte 7
    s.Status_NormalOperation_01  = (buf[7] >> 5) & 0x01;
    s.First_ImprecisionThreshold = (buf[7] >> 6) & 0x01;
    s.QBit_EngineTq              = (buf[7] >> 7) & 0x01;

    return s;
}

// Encode: pack Motor11_Signals → raw buf[8]
// - Preserves buf[0] (CRC slot — caller must call build_frame_with_checksum_0xA7 after)
// - Preserves buf[1] upper nibble (none — BZ is in lower nibble only, upper nibble
//   is fixed per the frame's b1 high nibble which is part of the ID/session token;
//   we preserve the existing upper nibble from the original frame)
inline void motor11_encode(uint8_t buf[8], const Motor11_Signals &s)
{
    // --- Byte 1: upper nibble preserved, lower nibble = BZ ---
    // (buf[1] >> 4) is the high-nibble "session prefix" from the ECU — keep it.
    buf[1] = (buf[1] & 0xF0) | (s.BZ & 0x0F);

    // --- EngineTqTargetRaw: bits 12-21 ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqTargetRaw + 509, 0, 1023);
        // bits 12-15 → upper nibble of byte 1
        buf[1] = (buf[1] & 0x0F) | (uint8_t)((raw & 0x0F) << 4);
        // bits 16-21 → bits 0-5 of byte 2  (clears bits 6-7 of byte 2 first)
        buf[2] = (buf[2] & 0xC0) | (uint8_t)((raw >> 4) & 0x3F);
    }

    // --- EngineTqActual: bits 22-31 ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqActual + 509, 0, 1023);
        // bits 22-23 → bits 6-7 of byte 2
        buf[2] = (buf[2] & 0x3F) | (uint8_t)((raw & 0x03) << 6);
        // bits 24-31 → byte 3
        buf[3] = (uint8_t)(raw >> 2);
    }

    // --- EngineTotalMomentsInertia: bits 32-41 ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTotalMomentsInertia + 509, 0, 1023);
        // bits 32-39 → byte 4
        buf[4] = (uint8_t)(raw & 0xFF);
        // bits 40-41 → bits 0-1 of byte 5  (clears bits 0-1 first)
        buf[5] = (buf[5] & 0xFC) | (uint8_t)((raw >> 8) & 0x03);
    }

    // --- EngineTqTargetFiltered: bits 42-51 ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqTargetFiltered + 509, 0, 1023);
        // bits 42-47 → bits 2-7 of byte 5
        buf[5] = (buf[5] & 0x03) | (uint8_t)((raw & 0x3F) << 2);
        // bits 48-51 → bits 0-3 of byte 6  (clears bits 0-3 first)
        buf[6] = (buf[6] & 0xF0) | (uint8_t)((raw >> 6) & 0x0F);
    }

    // --- EngineTqThrust: bits 52-60  (9-bit, phys range -509..0) ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqThrust + 509, 0, 511);
        // bits 52-55 → bits 4-7 of byte 6
        buf[6] = (buf[6] & 0x0F) | (uint8_t)((raw & 0x0F) << 4);
        // bits 56-60 → bits 0-4 of byte 7  (clears bits 0-4 first)
        buf[7] = (buf[7] & 0xE0) | (uint8_t)((raw >> 4) & 0x1F);
    }

    // --- Status bits in byte 7 (bits 61-63 = bits 5-7 of byte 7) ---
    buf[7] = (buf[7] & 0x1F)
           | (uint8_t)(s.Status_NormalOperation_01  & 0x01) << 5
           | (uint8_t)(s.First_ImprecisionThreshold & 0x01) << 6
           | (uint8_t)(s.QBit_EngineTq              & 0x01) << 7;
}


// ---------------------------------------------------------------------------
//  Motor_12  (CAN ID 0x0A8)
// ---------------------------------------------------------------------------

struct Motor12_Signals {
    // Motor_12_BZ: 4-bit rolling counter (preserved during MITM)
    uint8_t  BZ;                         // bits  8-11

    // Torque / limit signals
    int16_t  EngineTq_Neg_Available;     // bits 12-20  9-bit, phys -509..0 Nm
    uint16_t EngineTqLimit_Stat;         // bits 21-29  9-bit, phys    0..509 Nm
    int16_t  EngineTqLimit_Dyn;          // bits 30-39  10-bit, phys -509..+509 Nm
    uint8_t  EngineTqPercent;            // bits 40-46  7-bit, 0..100 %
    uint8_t  QBit_EngineRPM;            // bit  47

    // Engine speed: physical = raw * 0.25   (0..16383 RPM, 0.25 RPM resolution)
    // Stored here as 16-bit raw to preserve full precision.
    // Use motor12_rpm_physical() / motor12_rpm_to_raw() for conversion.
    uint16_t EngineRPM_raw;             // bits 48-63  raw value
};

// Convenience: convert raw RPM to physical RPM (float)
static inline float motor12_rpm_physical(uint16_t raw) {
    return (float)raw * 0.25f;
}

// Convenience: convert physical RPM (float) to raw (clamped 0..65535)
static inline uint16_t motor12_rpm_to_raw(float rpm_physical) {
    int32_t r = (int32_t)(rpm_physical * 4.0f + 0.5f);  // round
    if (r < 0)     r = 0;
    if (r > 65535) r = 65535;
    return (uint16_t)r;
}

// Decode: unpack buf[8] → Motor12_Signals
inline Motor12_Signals motor12_decode(const uint8_t buf[8])
{
    Motor12_Signals s;

    // BZ: bits 8-11 = lower nibble of byte 1
    s.BZ = buf[1] & 0x0F;

    // MO12_EngineTq_Neg_Available: bits 12-20  (9-bit)
    //   bits 12-15 = upper nibble of byte 1
    //   bits 16-20 = bits 0-4 of byte 2
    {
        uint16_t raw = (uint16_t)(buf[1] >> 4)
                     | ((uint16_t)(buf[2] & 0x1F) << 4);
        s.EngineTq_Neg_Available = (int16_t)raw - 509;
    }

    // MO12_EngineTqLimit_Stat: bits 21-29  (9-bit, offset=0)
    //   bits 21-23 = bits 5-7 of byte 2
    //   bits 24-29 = bits 0-5 of byte 3
    {
        uint16_t raw = (uint16_t)((buf[2] >> 5) & 0x07)
                     | ((uint16_t)(buf[3] & 0x3F) << 3);
        s.EngineTqLimit_Stat = raw;  // offset=0
    }

    // MO12_EngineTqLimit_Dyn: bits 30-39  (10-bit)
    //   bits 30-31 = bits 6-7 of byte 3
    //   bits 32-39 = byte 4
    {
        uint16_t raw = (uint16_t)((buf[3] >> 6) & 0x03)
                     | ((uint16_t)buf[4] << 2);
        s.EngineTqLimit_Dyn = (int16_t)raw - 509;
    }

    // MO12_EngineTqPercent: bits 40-46  (7-bit, offset=0)
    //   bits 40-46 = bits 0-6 of byte 5
    s.EngineTqPercent = buf[5] & 0x7F;

    // MO12_QBit_EngineRPM: bit 47 = bit 7 of byte 5
    s.QBit_EngineRPM = (buf[5] >> 7) & 0x01;

    // MO12_EngineRPM: bits 48-63 = bytes 6-7 (little-endian 16-bit)
    s.EngineRPM_raw = (uint16_t)buf[6] | ((uint16_t)buf[7] << 8);

    return s;
}

// Encode: pack Motor12_Signals → raw buf[8]
inline void motor12_encode(uint8_t buf[8], const Motor12_Signals &s)
{
    // --- Byte 1: preserve high nibble, write BZ to low nibble ---
    buf[1] = (buf[1] & 0xF0) | (s.BZ & 0x0F);

    // --- EngineTq_Neg_Available: bits 12-20  (9-bit) ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTq_Neg_Available + 509, 0, 511);
        // bits 12-15 → upper nibble of byte 1
        buf[1] = (buf[1] & 0x0F) | (uint8_t)((raw & 0x0F) << 4);
        // bits 16-20 → bits 0-4 of byte 2  (clears bits 0-4)
        buf[2] = (buf[2] & 0xE0) | (uint8_t)((raw >> 4) & 0x1F);
    }

    // --- EngineTqLimit_Stat: bits 21-29  (9-bit, offset=0) ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqLimit_Stat, 0, 511);
        // bits 21-23 → bits 5-7 of byte 2
        buf[2] = (buf[2] & 0x1F) | (uint8_t)((raw & 0x07) << 5);
        // bits 24-29 → bits 0-5 of byte 3
        buf[3] = (buf[3] & 0xC0) | (uint8_t)((raw >> 3) & 0x3F);
    }

    // --- EngineTqLimit_Dyn: bits 30-39  (10-bit) ---
    {
        uint16_t raw = _clamp16((int32_t)s.EngineTqLimit_Dyn + 509, 0, 1023);
        // bits 30-31 → bits 6-7 of byte 3
        buf[3] = (buf[3] & 0x3F) | (uint8_t)((raw & 0x03) << 6);
        // bits 32-39 → byte 4
        buf[4] = (uint8_t)(raw >> 2);
    }

    // --- EngineTqPercent: bits 40-46  (7-bit, offset=0) ---
    {
        uint8_t raw = (uint8_t)_clamp16((int32_t)s.EngineTqPercent, 0, 100);
        // bits 40-46 → bits 0-6 of byte 5  (clears bits 0-6)
        buf[5] = (buf[5] & 0x80) | (raw & 0x7F);
    }

    // --- QBit_EngineRPM: bit 47 = bit 7 of byte 5 ---
    buf[5] = (buf[5] & 0x7F) | (uint8_t)(s.QBit_EngineRPM & 0x01) << 7;

    // --- EngineRPM: bits 48-63 = bytes 6-7 ---
    buf[6] = (uint8_t)(s.EngineRPM_raw & 0xFF);
    buf[7] = (uint8_t)(s.EngineRPM_raw >> 8);
}


// ---------------------------------------------------------------------------
//  Example MITM callback skeletons
//  (requires: #include "checksum_0xA7_0xA8.h" before this file)
//
//  Call these from your CAN receive interrupt / polling loop wherever you
//  currently decide to forward or modify frames.
// ---------------------------------------------------------------------------

// Motor_11 (0xA7): observe → optionally modify → recompute checksum → forward
//
// Example: cap EngineTqActual to 200 Nm and forward with corrected CRC.
//
//   void on_motor11_rx(CAN_message_t &msg, FlexCAN_T4_Base &output_bus) {
//       Motor11_Signals s = motor11_decode(msg.buf);
//
//       // ----- YOUR MODIFICATIONS HERE -----
//       if (s.EngineTqActual > 200)
//           s.EngineTqActual = 200;
//       // ------------------------------------
//
//       motor11_encode(msg.buf, s);           // pack signals back into bytes
//       build_frame_with_checksum_0xA7(msg.buf); // recompute b0
//       output_bus.write(msg);                // forward modified frame
//   }
//
// Motor_12 (0xA8): same pattern
//
//   void on_motor12_rx(CAN_message_t &msg, FlexCAN_T4_Base &output_bus) {
//       Motor12_Signals s = motor12_decode(msg.buf);
//
//       // ----- YOUR MODIFICATIONS HERE -----
//       float rpm = motor12_rpm_physical(s.EngineRPM_raw);
//       if (rpm > 4000.0f)
//           s.EngineRPM_raw = motor12_rpm_to_raw(4000.0f);
//       // ------------------------------------
//
//       motor12_encode(msg.buf, s);
//       build_frame_with_checksum_0xA8(msg.buf);
//       output_bus.write(msg);
//   }
