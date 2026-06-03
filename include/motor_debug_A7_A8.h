#pragma once
#include <Arduino.h>
#include "checksum_0xA7_0xA8.h"
#include "motor_signals_A7_A8.h"
// =============================================================================
//  motor_debug_A7_A8.h
//
//  Persistent decoded-signal variables for Motor_11 (0xA7) and Motor_12 (0xA8),
//  updated on every received frame. Includes throttled Serial.printf debug output.
//
//  Dependencies pulled in automatically above — no manual ordering needed.
//
//  Usage — in your CAN receive handler (e.g. can_mitm.cpp):
//
//    if (msg.id == 0x0A7) {
//        motor11_update(msg.buf);         // decode + optional debug print
//        // ---- YOUR MODIFICATIONS ----
//        g_mo11.EngineTqActual = 150;
//        // ----------------------------
//        motor11_encode(msg.buf, g_mo11);
//        build_frame_with_checksum_0xA7(msg.buf);
//        TFTCAN3.write(msg);
//    }
//    if (msg.id == 0x0A8) {
//        motor12_update(msg.buf);
//        // ---- YOUR MODIFICATIONS ----
//        // g_mo12.EngineTqPercent = 80;
//        // ----------------------------
//        motor12_encode(msg.buf, g_mo12);
//        build_frame_with_checksum_0xA8(msg.buf);
//        TFTCAN3.write(msg);
//    }
//
//  Debug output is throttled to once every MOTOR_DEBUG_INTERVAL_MS milliseconds
//  (default 500 ms). Set to 0 to print every frame. Comment out the
//  #define MOTOR_DEBUG_PRINT line to silence all debug output completely.
// =============================================================================

// ---------------------------------------------------------------------------
// Debug output control
// ---------------------------------------------------------------------------
#define MOTOR_DEBUG_PRINT                     // comment out to silence all prints
#define MOTOR_DEBUG_INTERVAL_MS   500u        // ms between Serial prints (0 = every frame)

// ---------------------------------------------------------------------------
// Persistent global signal structs — updated by motor11_update / motor12_update
// ---------------------------------------------------------------------------

// Motor_11 (0xA7) — last decoded frame
static Motor11_Signals g_mo11 = {};

// Motor_12 (0xA8) — last decoded frame
static Motor12_Signals g_mo12 = {};

// Frame counters (total received since boot)
static uint32_t g_mo11_frame_count = 0;
static uint32_t g_mo12_frame_count = 0;

// ---------------------------------------------------------------------------
// Debug print helpers
// ---------------------------------------------------------------------------
#ifdef MOTOR_DEBUG_PRINT

static elapsedMillis _mo11_print_timer;
static elapsedMillis _mo12_print_timer;

static inline void motor11_print_debug() {
    Serial.printf(
        "[0xA7 Motor_11] BZ=%u | TqTargetRaw=%d Nm | TqActual=%d Nm | "
        "TotalMI=%d Nm | TqFiltered=%d Nm | TqThrust=%d Nm | "
        "Stat_NormOp=%u | ImprecThresh=%u | QBit=%u | frames=%lu\n",
        g_mo11.BZ,
        g_mo11.EngineTqTargetRaw,
        g_mo11.EngineTqActual,
        g_mo11.EngineTotalMomentsInertia,
        g_mo11.EngineTqTargetFiltered,
        g_mo11.EngineTqThrust,
        g_mo11.Status_NormalOperation_01,
        g_mo11.First_ImprecisionThreshold,
        g_mo11.QBit_EngineTq,
        (unsigned long)g_mo11_frame_count
    );
}

static inline void motor12_print_debug() {
    Serial.printf(
        "[0xA8 Motor_12] BZ=%u | TqNegAvail=%d Nm | TqLimitStat=%u Nm | "
        "TqLimitDyn=%d Nm | TqPct=%u%% | QBitRPM=%u | "
        "RPM_raw=%u (%.2f RPM) | frames=%lu\n",
        g_mo12.BZ,
        g_mo12.EngineTq_Neg_Available,
        g_mo12.EngineTqLimit_Stat,
        g_mo12.EngineTqLimit_Dyn,
        g_mo12.EngineTqPercent,
        g_mo12.QBit_EngineRPM,
        g_mo12.EngineRPM_raw,
        motor12_rpm_physical(g_mo12.EngineRPM_raw),
        (unsigned long)g_mo12_frame_count
    );
}

#endif  // MOTOR_DEBUG_PRINT

// ---------------------------------------------------------------------------
// motor11_update()
//   Call once per received 0xA7 frame BEFORE making any modifications.
//   - Decodes buf into g_mo11
//   - Increments g_mo11_frame_count
//   - Optionally Serial.prints at MOTOR_DEBUG_INTERVAL_MS rate
// ---------------------------------------------------------------------------
static inline void motor11_update(const uint8_t buf[8]) {
    g_mo11 = motor11_decode(buf);
    g_mo11_frame_count++;

#ifdef MOTOR_DEBUG_PRINT
    if (_mo11_print_timer >= MOTOR_DEBUG_INTERVAL_MS) {
        _mo11_print_timer = 0;
        motor11_print_debug();
    }
#endif
}

// ---------------------------------------------------------------------------
// motor12_update()
//   Call once per received 0xA8 frame BEFORE making any modifications.
// ---------------------------------------------------------------------------
static inline void motor12_update(const uint8_t buf[8]) {
    g_mo12 = motor12_decode(buf);
    g_mo12_frame_count++;

#ifdef MOTOR_DEBUG_PRINT
    if (_mo12_print_timer >= MOTOR_DEBUG_INTERVAL_MS) {
        _mo12_print_timer = 0;
        motor12_print_debug();
    }
#endif
}

// ---------------------------------------------------------------------------
// motor_debug_print_all()
//   Force an immediate Serial.printf of both structs regardless of timer.
//   Handy to call from a serial command / button press for a one-shot dump.
// ---------------------------------------------------------------------------
static inline void motor_debug_print_all() {
#ifdef MOTOR_DEBUG_PRINT
    motor11_print_debug();
    motor12_print_debug();
#endif
}


// =============================================================================
//  CHECKSUM VALIDATOR
//
//  Call motor11_validate() / motor12_validate() on the raw received buf[8]
//  BEFORE any modification. Prints observed vs calculated checksum and
//  PASS/FAIL. Maintains running totals since boot.
//
//  Controls (independent from MOTOR_DEBUG_PRINT):
//    #define MOTOR_VALIDATE              — enable validator (comment to disable)
//    #define MOTOR_VALIDATE_PRINT_FAILS  — always print on a FAIL (ignores timer)
//    #define MOTOR_VALIDATE_INTERVAL_MS  — throttle rate for PASS prints
//
//  Usage:
//    if (msg.id == 0x0A7) {
//        motor11_validate(msg.buf);    // <-- add this line
//        motor11_update(msg.buf);
//        ...
//    }
//    if (msg.id == 0x0A8) {
//        motor12_validate(msg.buf);
//        motor12_update(msg.buf);
//        ...
//    }
//
//  One-shot summary (e.g. on serial command):
//    motor_validate_print_summary();
// =============================================================================

#define MOTOR_VALIDATE                        // comment out to disable entirely
#define MOTOR_VALIDATE_PRINT_FAILS            // always print on a FAIL (recommended)
#define MOTOR_VALIDATE_INTERVAL_MS   10000u    // throttle rate for PASS prints (ms)

#ifdef MOTOR_VALIDATE

// --- Running counters ---
static uint32_t g_mo11_pass  = 0;
static uint32_t g_mo11_fail  = 0;
static uint32_t g_mo12_pass  = 0;
static uint32_t g_mo12_fail  = 0;

static elapsedMillis _mo11_val_timer;
static elapsedMillis _mo12_val_timer;

// ---- 0xA7 validator --------------------------------------------------------
static inline void motor11_validate(const uint8_t buf[8]) {
    uint8_t observed   = buf[0];
    uint8_t calculated = checksum_Ax_compute(
        buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
        CHECKSUM_0xA7_G_LO, CHECKSUM_0xA7_OFFSET);

    bool pass = (observed == calculated);
    if (pass) g_mo11_pass++; else g_mo11_fail++;

    // Print on every FAIL, or on throttled timer for PASS
    bool do_print = false;
#ifdef MOTOR_VALIDATE_PRINT_FAILS
    if (!pass) do_print = true;
#endif
    if (_mo11_val_timer >= MOTOR_VALIDATE_INTERVAL_MS) {
        _mo11_val_timer = 0;
        do_print = true;
    }

    if (do_print) {
        Serial.printf(
            "[0xA7 CRC] observed=0x%02X  calculated=0x%02X  %s"
            "  (pass=%lu  fail=%lu)\n",
            observed, calculated,
            pass ? "PASS" : "*** FAIL ***",
            (unsigned long)g_mo11_pass,
            (unsigned long)g_mo11_fail);
    }
}

// ---- 0xA8 validator --------------------------------------------------------
static inline void motor12_validate(const uint8_t buf[8]) {
    uint8_t observed   = buf[0];
    uint8_t calculated = checksum_Ax_compute(
        buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7],
        CHECKSUM_0xA8_G_LO, CHECKSUM_0xA8_OFFSET);

    bool pass = (observed == calculated);
    if (pass) g_mo12_pass++; else g_mo12_fail++;

    bool do_print = false;
#ifdef MOTOR_VALIDATE_PRINT_FAILS
    if (!pass) do_print = true;
#endif
    if (_mo12_val_timer >= MOTOR_VALIDATE_INTERVAL_MS) {
        _mo12_val_timer = 0;
        do_print = true;
    }

    if (do_print) {
        Serial.printf(
            "[0xA8 CRC] observed=0x%02X  calculated=0x%02X  %s"
            "  (pass=%lu  fail=%lu)\n",
            observed, calculated,
            pass ? "PASS" : "*** FAIL ***",
            (unsigned long)g_mo12_pass,
            (unsigned long)g_mo12_fail);
    }
}

// ---- Summary dump (call on demand, e.g. serial command or button) ----------
static inline void motor_validate_print_summary() {
    uint32_t tot11 = g_mo11_pass + g_mo11_fail;
    uint32_t tot12 = g_mo12_pass + g_mo12_fail;
    Serial.printf(
        "\n--- Checksum Validator Summary ---\n"
        "  0xA7 Motor_11: %lu / %lu PASS  (%lu FAIL)  %.2f%%\n"
        "  0xA8 Motor_12: %lu / %lu PASS  (%lu FAIL)  %.2f%%\n"
        "----------------------------------\n",
        (unsigned long)g_mo11_pass, (unsigned long)tot11,
        (unsigned long)g_mo11_fail,
        tot11 ? (g_mo11_pass * 100.0f / tot11) : 0.0f,
        (unsigned long)g_mo12_pass, (unsigned long)tot12,
        (unsigned long)g_mo12_fail,
        tot12 ? (g_mo12_pass * 100.0f / tot12) : 0.0f);
}

#else  // MOTOR_VALIDATE not defined — compile out to nothing
static inline void motor11_validate(const uint8_t[8]) {}
static inline void motor12_validate(const uint8_t[8]) {}
static inline void motor_validate_print_summary() {}
#endif  // MOTOR_VALIDATE
