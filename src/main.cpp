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
#include "global_vars.h"
// #include "checksum.h"

#include "a8_solver.h"

#include <SD.h>

File logFile;
File fullLogFile;
File unknownLogFile;

static constexpr uint32_t LOG_BUFFER_SIZE = 16384;

static char fullLogBuffer[LOG_BUFFER_SIZE];
static uint32_t fullLogPos = 0;

static char unknownLogBuffer[4096];
static uint32_t unknownLogPos = 0;


static constexpr uint32_t SD_BUFFER_SIZE = 16384;

static uint8_t sdBuffer[SD_BUFFER_SIZE];

static uint32_t sdBufferPos = 0;

static uint32_t totalFlushes = 0;
static uint32_t totalBytesWritten = 0;

// #include <MQB_CANbus_ParsingHelpers.h>
// #include "Internal_Onboard_Functions.h"


//   #pragma region // 0xA7 - Motor_11 - Engine Torques
//     uint8_t  MQB_Motor_11_0xA7_CRC;
//     uint8_t  MQB_Motor_11_0xA7_BZ;
//     int16_t  MQB_Motor_11_0xA7_EngineTqTargetRaw_0xA7;
//     int16_t  MQB_Motor_11_0xA7_EngineTqActual_0xA7;
//     int16_t  MQB_Motor_11_0xA7_EngineTotalMomentsInertia;
//     int16_t  MQB_Motor_11_0xA7_EngineTqTargetFiltered_0xA7;
//     int16_t  MQB_Motor_11_0xA7_EngineTqThrust;
//     bool     MQB_Motor_11_0xA7_Status_Normalbetrieb_01;
//     bool     MQB_Motor_11_0xA7_erste_Ungenauschwelle;
//     bool     MQB_Motor_11_0xA7_QBit_Motormomente;


//   #pragma endregion



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
uint32_t a8PassCount = 0;
uint32_t a8FailCount = 0;
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


void flushFullLog()
{
    if (!fullLogFile)
        return;

    if (fullLogPos == 0)
        return;

    fullLogFile.write(
        (uint8_t*)fullLogBuffer,
        fullLogPos);

    fullLogFile.flush();

    fullLogPos = 0;
}


void flushUnknownLog()
{
    if (!unknownLogFile)
        return;

    if (unknownLogPos == 0)
        return;

    unknownLogFile.write(
        (uint8_t*)unknownLogBuffer,
        unknownLogPos);

    unknownLogFile.flush();

    unknownLogPos = 0;
}

void logFullFrame(
    const CAN_message_t &msg)
{
    char line[96];

    int len =
        snprintf(
            line,
            sizeof(line),
            "%lu,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X\n",
            millis(),
            msg.buf[0],
            msg.buf[1],
            msg.buf[2],
            msg.buf[3],
            msg.buf[4],
            msg.buf[5],
            msg.buf[6],
            msg.buf[7]);

    if (
        fullLogPos + len >=
        LOG_BUFFER_SIZE)
    {
        flushFullLog();
    }

    memcpy(
        &fullLogBuffer[fullLogPos],
        line,
        len);

    fullLogPos += len;
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

    memcpy(
        lastUnknown,
        msg.buf,
        8);

    haveLastUnknown = true;

    char line[96];

    int len =
        snprintf(
            line,
            sizeof(line),
            "%lu,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X\n",
            millis(),
            msg.buf[0],
            msg.buf[1],
            msg.buf[2],
            msg.buf[3],
            msg.buf[4],
            msg.buf[5],
            msg.buf[6],
            msg.buf[7]);

    if (
        unknownLogPos + len >=
        sizeof(unknownLogBuffer))
    {
        flushUnknownLog();
    }

    memcpy(
        &unknownLogBuffer[unknownLogPos],
        line,
        len);

    unknownLogPos += len;
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
    float accuracy = 0.0f;

    if ((a8PassCount + a8FailCount) > 0)
    {
        accuracy =
            100.0f *
            ((float)a8PassCount /
            (float)(a8PassCount + a8FailCount));
    }

    Serial.println();
    Serial.println("========================================");
    Serial.println("A8 VALIDATOR STATS");
    Serial.println("========================================");

    Serial.printf(
        "Frames     : %lu\n",
        totalFrames);

    Serial.printf(
        "Pass       : %lu\n",
        a8PassCount);

    Serial.printf(
        "Fail       : %lu\n",
        a8FailCount);

    Serial.printf(
        "Accuracy   : %.4f%%\n",
        accuracy);

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

    logFullFrame(msg);

    bool pass =
        verifyChecksum_0xA8(
            msg.buf);

    if (pass)
        a8PassCount++;
    else
        a8FailCount++;

    uint8_t actual =
        actualResidual_0xA8(
            msg.buf);

    uint8_t predicted =
        predictedResidual_0xA8(
            msg.buf);

    uint8_t counter =
        msg.buf[1] & 0x0F;

    uint8_t family =
        msg.buf[1] & 0xF0;

    printFrame(msg);

    Serial.printf(
        " | CTR=%X FAMILY=%02X ",
        counter,
        family);

    Serial.printf(
        "| RES_ACT=%02X RES_PRED=%02X ",
        actual,
        predicted);

    if (pass)
    {
        Serial.print("PASS ");
    }
    else
    {
        Serial.print("FAIL ");

        logUnknownFrame(msg);
    }

    Serial.print("| ");

    decodeA8Signals(msg.buf);

    Serial.println();
}




void flushSDBuffer()
{
    if (sdBufferPos == 0)
        return;

    size_t written =
        logFile.write(
            sdBuffer,
            sdBufferPos);

    totalBytesWritten += written;

    logFile.flush();

    totalFlushes++;

    sdBufferPos = 0;
}

void bufferedWrite(
    const uint8_t* data,
    uint32_t len)
{
    if (len > SD_BUFFER_SIZE)
        return;

    if (
        sdBufferPos + len >
        SD_BUFFER_SIZE)
    {
        flushSDBuffer();
    }

    memcpy(
        &sdBuffer[sdBufferPos],
        data,
        len);

    sdBufferPos += len;
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

                fullLogFile =
            SD.open(
                activeLogFilename,
                FILE_WRITE);

        unknownLogFile =
            SD.open(
                unknownLogFilename,
                FILE_WRITE);
    }

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

        // if (msg.id == 0x0A7 &&
        //     msg.len == 8)
        // {
        //         Motor_11_t motor11 = parse_Motor_11(msg);
        //         MQB_Motor_11_0xA7_EngineTqTargetRaw_0xA7 = motor11.MO_EngineTqTargetRaw_0xA7;
        //         MQB_Motor_11_0xA7_EngineTqActual_0xA7 = motor11.MO_EngineTqActual_0xA7;
        //         MQB_Motor_11_0xA7_EngineTotalMomentsInertia = motor11.MO_EngineTotalMomentsInertia;
        //         MQB_Motor_11_0xA7_EngineTqTargetFiltered_0xA7 = motor11.MO_EngineTqTargetFiltered_0xA7;
        //         MQB_Motor_11_0xA7_EngineTqThrust = motor11.MO_EngineTqThrust;
        //         MQB_Motor_11_0xA7_Status_Normalbetrieb_01 = motor11.MO_Status_Normalbetrieb_01;
        //         MQB_Motor_11_0xA7_erste_Ungenauschwelle = motor11.MO_erste_Ungenauschwelle;
        //         MQB_Motor_11_0xA7_QBit_Motormomente = motor11.MO_QBit_Motormomente;



        //         //  PT_CAN_msg0xA7_buf0_Raw = PT_CAN_frame.buf[0];
        //         //  PT_CAN_msg0xA7_buf1_Raw = PT_CAN_frame.buf[1];
        //         //  PT_CAN_msg0xA7_buf2_Raw = PT_CAN_frame.buf[2];
        //         //  PT_CAN_msg0xA7_buf3_Raw = PT_CAN_frame.buf[3];
        //         //  PT_CAN_msg0xA7_buf4_Raw = PT_CAN_frame.buf[4];
        //         //  PT_CAN_msg0xA7_buf5_Raw = PT_CAN_frame.buf[5];
        //         //  PT_CAN_msg0xA7_buf6_Raw = PT_CAN_frame.buf[6];
        //         //  PT_CAN_msg0xA7_buf7_Raw = PT_CAN_frame.buf[7];

        //         //  PT_CAN_msg0xA7_buf0 = PT_CAN_msg0xA7_buf0_Raw;
        //         //  PT_CAN_msg0xA7_buf1 = PT_CAN_msg0xA7_buf1_Raw;
        //         //  PT_CAN_msg0xA7_buf2 = PT_CAN_msg0xA7_buf2_Raw;
        //         //  PT_CAN_msg0xA7_buf3 = PT_CAN_msg0xA7_buf3_Raw;
        //         //  PT_CAN_msg0xA7_buf4 = PT_CAN_msg0xA7_buf4_Raw;
        //         //  PT_CAN_msg0xA7_buf5 = PT_CAN_msg0xA7_buf5_Raw;
        //         //  PT_CAN_msg0xA7_buf6 = PT_CAN_msg0xA7_buf6_Raw;
        //         //  PT_CAN_msg0xA7_buf7 = PT_CAN_msg0xA7_buf7_Raw;


        //     Serial.print("MQB_Motor_11_0xA7_EngineTqTargetRaw_0xA7: "); Serial.print(MQB_Motor_11_0xA7_EngineTqTargetRaw_0xA7); Serial.print("\t");
        //     Serial.print("MQB_Motor_11_0xA7_Status_Normalbetrieb_01: "); Serial.print(MQB_Motor_11_0xA7_Status_Normalbetrieb_01); Serial.print("\t");
        //     Serial.println("");



        // }


    }

        static uint32_t lastFlush = 0;

        if (
            millis() - lastFlush >
            1000)
        {
            flushFullLog();
            flushUnknownLog();

            lastFlush = millis();
        }


    if (statsTimer > 5000) {

        printStats();

        statsTimer = 0;
    }
}