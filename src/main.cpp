#include <Arduino.h>
#include "checksum.h"

#include <FlexCAN_T4.h>
#include "global_vars.h"
#include "Internal_Onboard_Functions.h"


CAN_message_t msg;  // global definition (resolves extern)

// Forward declarations from can_mitm.cpp
void can_setup();
void loop_TFTCAN1_poll();
void loop_TFTCAN2_poll();
void loop_TFTCAN3_poll();
void do_TFT_MITM_LC_BumpIn_Statuses();
void reset_inverse_cache();  // from checksum.cpp
void loop_SerialPrinting_MITM();

// Config
static const uint32_t SERIAL_BAUD = 115200;
static const uint32_t HEARTBEAT_MS = 1000;

// Runtime state
static uint32_t last_heartbeat = 0;
static uint32_t last_serial_check = 0;

// Simple serial command handler
static void handle_serial_commands() {
  while (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
if (line.length() == 0) continue;

    if (line.equalsIgnoreCase("help")) {
      Serial.println(F("Commands: help | stats | reset_cache"));
    } else if (line.equalsIgnoreCase("stats")) {
      Serial.println(F("Stats: not implemented (add counters in can_mitm.cpp)"));
    } else if (line.equalsIgnoreCase("reset_cache")) {
      Serial.println(F("Resetting inverse cache..."));
      reset_inverse_cache();
      Serial.println(F("done."));
    } else {
      Serial.print(F("Unknown command: "));
      Serial.println(line);
    }
  }
}



void setup() {

  #pragma region // configure pinmodes

    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH);


    pinMode(36, OUTPUT);
    pinMode(37, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(4, OUTPUT);

    pinMode(48, OUTPUT);
    pinMode(49, OUTPUT);
    pinMode(50, OUTPUT);
    pinMode(51, OUTPUT);
    pinMode(52, OUTPUT);
    pinMode(53, OUTPUT);
    pinMode(54, OUTPUT);

  #pragma endregion

  #pragma region // Configure Serial and CAN communications

  Serial.begin(115200);
  uint32_t t0 = millis();
  while (!Serial && millis() - t0 < 1500) delay(10);

  Serial.println();
  Serial.println(F("TFT CAN MITM starting..."));

  can_setup();
  last_heartbeat = millis();
  last_serial_check = millis();

  // TFTCAN_PT_CAN_init();

  Serial.println(F("Initialization complete."));


  #pragma endregion


}

void loop() {
  // Main CAN MITM loop
  loop_TFTCAN1_poll();
  loop_TFTCAN2_poll();
  loop_TFTCAN3_poll();

  if(IgnitionStatusTimer_TimeSinceLastMessage < 10000) {
  do_TFT_MITM_LC_BumpIn_Statuses();
  do_MCU_CoreTempMonitoring();
  // do_TFTCAN_PT_CAN_Sniffing();
  }

  loop_SerialPrinting_MITM();

  uint32_t now = millis();

  // Heartbeat
  if (now - last_heartbeat >= HEARTBEAT_MS) {
    last_heartbeat = now;
    // Serial.print('.');
  }

  // Handle serial commands
  if (now - last_serial_check >= 200) {
    last_serial_check = now;
    // handle_serial_commands();
  }

  yield();
}
