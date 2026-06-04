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

// #include "a8_solver.h"
// #include "motor_signals_A7_A8.h"   // signal structs + pack/unpack
#include "motor_debug_A7_A8.h"

// #include <SD.h>

void can_setup();

void loop_TFTCAN1_poll_MITM_ABS();
void loop_TFTCAN2_poll_MITM_ABS();
void loop_TFTCAN3_poll_MITM_ABS();
void do_TFT_MITM_LC_BumpIn_Statuses();
void reset_inverse_cache();  // from checksum.cpp
void loop_SerialPrinting_MITM_ABS();



void loop_TFTCAN1_poll_MITM_TCU();
void loop_TFTCAN2_poll_MITM_TCU();
void loop_TFTCAN3_poll_MITM_TCU();
void do_TFT_MITM_TCU_EngTQmod();
void reset_inverse_cache_MITM_TCU();  // from checksum.cpp
void loop_SerialPrinting_MITM_TCU();



elapsedMillis loopDelaySerialPrinting;





void setup() {
    Serial.begin(115200);

    delay(2000);

    // pinMode(32, OUTPUT);
    // digitalWrite(32, HIGH);


        // CAN
    can_setup();

        Serial.println("CAN initialized");

        // SD
        delay(500);

        if (!SD.begin(BUILTIN_SDCARD)) {

            Serial.println("SD INIT FAILED");

        } else {

            Serial.println("SD INIT SUCCESS");
        }
    }



void loop() {

    //   loop_TFTCAN1_poll_MITM_TCU();
    loop_TFTCAN2_poll_MITM_TCU();
    loop_TFTCAN3_poll_MITM_TCU();

    do_TFT_MITM_TCU_EngTQmod();
    loop_SerialPrinting_MITM_TCU();
    
        if (loopDelaySerialPrinting > 5000) {

            // printStats();
            // motor_validate_print_summary();
            loopDelaySerialPrinting = 0;
            
        }
}