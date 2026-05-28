// =============================================================================
// main.cpp
//
// Teensy 4.1
// MQB A8 Full Logger + Discovery + Solver
//
// COMBINED FEATURES
// -----------------
//
// 1. Live CAN RX
// 2. 0xA8 checksum verification
// 3. Full frame logger
// 4. Unknown-state logger
// 5. Session logfile generation
// 6. Lookup solver
// 7. Experimental brute-force architecture
// 8. DBC decoding
// 9. Live statistics
// 10. Replay/learning support hooks
//
// =============================================================================

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <FlexCAN_T4.h>

#include "a8_solver.h"

// =============================================================================
// CAN
// =============================================================================

static FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> CANbus;

// =============================================================================
// SD CONFIG
// =============================================================================

#define A8_SESSION_LOGGING 1

char activeLogFilename[32];
char unknownLogFilename[32];

// =============================================================================
// LIVE STATS
// =============================================================================

static uint32_t totalFrames      = 0;
static uint32_t lookupHits       = 0;
static uint32_t bruteHits        = 0;
static uint32_t unsolvedFrames   = 0;

elapsedMillis statsTimer;

// =============================================================================
// DUPLICATE SUPPRESSION
// =============================================================================

static uint8_t lastUnknown[8];
static bool haveLastUnknown = false;

// =============================================================================
// CREATE SESSION FILENAMES
// =============================================================================

void createSessionFilenames()
{
    for (int i = 0; i < 10000; i++) {

        sprintf(
            activeLogFilename,
            "/a8_full_%04d.csv",
            i);

        if (!SD.exists(activeLogFilename)) {

            sprintf(
                unknownLogFilename,
                "/a8_unknown_%04d.csv",
                i);

            return;
        }
    }

    strcpy(activeLogFilename,
           "/a8_overflow.csv");

    strcpy(unknownLogFilename,
           "/a8_unknown_overflow.csv");
}

// =============================================================================
// PRINT FRAME
// =============================================================================

void printFrame(
    const CAN_message_t &msg)
{
    Serial.printf("0x%03X ", msg.id);

    for (int i = 0; i < msg.len; i++) {

        if (msg.buf[i] < 0x10)
            Serial.print("0");

        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
    }
}

// =============================================================================
// FULL RAW LOGGER
// =============================================================================

void logFullFrame(
    const CAN_message_t &msg)
{
    File f =
        SD.open(activeLogFilename,
                FILE_WRITE);

    if (!f)
        return;

    f.print(millis());

    for (int i = 0; i < 8; i++) {

        f.print(",");

        if (msg.buf[i] < 0x10)
            f.print("0");

        f.print(msg.buf[i], HEX);
    }

    f.println();

    f.close();
}

// =============================================================================
// UNKNOWN FRAME LOGGER
// =============================================================================

void logUnknownFrame(
    const CAN_message_t &msg)
{
    if (haveLastUnknown &&
        memcmp(lastUnknown,
               msg.buf,
               8) == 0)
    {
        return;
    }

    memcpy(lastUnknown,
           msg.buf,
           8);

    haveLastUnknown = true;

    File f =
        SD.open(unknownLogFilename,
                FILE_WRITE);

    if (!f)
        return;

    f.print(millis());

    for (int i = 0; i < 8; i++) {

        f.print(",");

        if (msg.buf[i] < 0x10)
            f.print("0");

        f.print(msg.buf[i], HEX);
    }

    f.println();

    f.close();
}

// =============================================================================
// LITTLE-ENDIAN SIGNAL EXTRACTION
// =============================================================================

uint32_t extractSignalLE(
    const uint8_t *buf,
    uint16_t startBit,
    uint8_t bitLength)
{
    uint64_t raw = 0;

    for (int i = 0; i < 8; i++) {

        raw |= ((uint64_t)buf[i]) << (8 * i);
    }

    uint64_t mask =
        ((uint64_t)1 << bitLength) - 1;

    return (raw >> startBit) & mask;
}

// =============================================================================
// DBC DECODER
// =============================================================================

void decodeA8Signals(
    const uint8_t buf[8])
{
    uint8_t counter =
        extractSignalLE(buf, 8, 4);

    int16_t tqNegAvail =
        extractSignalLE(buf, 12, 9) - 509;

    uint16_t tqLimitStat =
        extractSignalLE(buf, 21, 9);

    int16_t tqLimitDyn =
        extractSignalLE(buf, 30, 10) - 509;

    uint8_t tqPercent =
        extractSignalLE(buf, 40, 7);

    bool qBitRPM =
        extractSignalLE(buf, 47, 1);

    uint16_t engineRPMRaw =
        extractSignalLE(buf, 48, 16);

    float engineRPM =
        engineRPMRaw * 0.25f;

    Serial.printf(
        "CTR=%X "
        "NegTQ=%dNm "
        "LimStat=%uNm "
        "LimDyn=%dNm "
        "TQ%%=%u "
        "RPM=%.0f "
        "Q=%u",
        counter,
        tqNegAvail,
        tqLimitStat,
        tqLimitDyn,
        tqPercent,
        engineRPM,
        qBitRPM
    );
}

// =============================================================================
// LIVE STATS
// =============================================================================

void printStats()
{
    float hitRate = 0.0f;

    if (totalFrames > 0) {

        hitRate =
            100.0f *
            ((float)(lookupHits + bruteHits) /
             (float)totalFrames);
    }

    Serial.println();
    Serial.println("========================================");
    Serial.println("A8 SOLVER STATS");
    Serial.println("========================================");

    Serial.printf(
        "Frames       : %lu\n",
        totalFrames);

    Serial.printf(
        "Lookup Hits  : %lu\n",
        lookupHits);

    Serial.printf(
        "Bruteforce   : %lu\n",
        bruteHits);

    Serial.printf(
        "Unsolved     : %lu\n",
        unsolvedFrames);

    Serial.printf(
        "Hit Rate     : %.2f%%\n",
        hitRate);

    Serial.printf(
        "Table Entries: %u\n",
        A8_TABLE_COUNT);

    Serial.println("========================================");
    Serial.println();
}

// =============================================================================
// PROCESS A8
// =============================================================================

void processA8(
    const CAN_message_t &msg)
{
    totalFrames++;

    // Full logging
    logFullFrame(msg);

    uint8_t calculated = 0;

    bool usedLookup = false;
    bool usedBruteforce = false;

    bool solved =
        solve_checksum_0xA8(
            msg.buf,
            &calculated,
            &usedLookup,
            &usedBruteforce);

    if (usedLookup)
        lookupHits++;

    if (usedBruteforce)
        bruteHits++;

    if (!solved)
        unsolvedFrames++;

    uint8_t observed =
        msg.buf[0];

    uint8_t counter =
        msg.buf[1] & 0x0F;

    uint8_t family =
        msg.buf[1] & 0xF0;

    printFrame(msg);

    Serial.printf(
        " | CTR=%X FAMILY=%02X ",
        counter,
        family);

    if (!solved) {

        Serial.println("UNSOLVED");

        logUnknownFrame(msg);

        return;
    }

    Serial.printf(
        "| OBS=%02X CALC=%02X ",
        observed,
        calculated);

    if (usedLookup)
        Serial.print("LOOKUP ");

    if (usedBruteforce)
        Serial.print("BRUTE ");

    if (observed == calculated)
        Serial.print("MATCH ");
    else
        Serial.print("FAIL ");

    Serial.print("| ");

    decodeA8Signals(msg.buf);

    Serial.println();
}

// =============================================================================
// SETUP
// =============================================================================

void setup()
{
    Serial.begin(115200);

    delay(2000);

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH);

    Serial.println();
    Serial.println("========================================");
    Serial.println("A8 FULL LOGGER + SOLVER");
    Serial.println("========================================");

    // CAN
    CANbus.begin();
    CANbus.setBaudRate(500000);

    Serial.println("CAN initialized");

    // SD
    delay(500);

    if (!SD.begin(BUILTIN_SDCARD)) {

        Serial.println("SD INIT FAILED");

    } else {

        Serial.println("SD INIT SUCCESS");

#if A8_SESSION_LOGGING

        createSessionFilenames();

#else

        strcpy(activeLogFilename,
               "/a8_full.csv");

        strcpy(unknownLogFilename,
               "/a8_unknown.csv");

#endif

        Serial.print("Full Log: ");
        Serial.println(activeLogFilename);

        Serial.print("Unknown Log: ");
        Serial.println(unknownLogFilename);

        // Create full log header
        {
            File f =
                SD.open(activeLogFilename,
                        FILE_WRITE);

            if (f) {

                f.println(
                    "time,b0,b1,b2,b3,b4,b5,b6,b7");

                f.close();
            }
        }

        // Create unknown log header
        {
            File f =
                SD.open(unknownLogFilename,
                        FILE_WRITE);

            if (f) {

                f.println(
                    "time,b0,b1,b2,b3,b4,b5,b6,b7");

                f.close();
            }
        }
    }

    Serial.printf(
        "A8_TABLE_COUNT = %u\n",
        A8_TABLE_COUNT);

    Serial.println();
}

// =============================================================================
// LOOP
// =============================================================================

void loop()
{
    CAN_message_t msg;

    while (CANbus.read(msg)) {

        if (msg.id == 0x0A8 &&
            msg.len == 8)
        {
            processA8(msg);
        }
    }

    if (statsTimer > 5000) {

        printStats();

        statsTimer = 0;
    }
}