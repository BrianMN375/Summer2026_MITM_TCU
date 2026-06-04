#include <Arduino.h>

// Modify outgoing payloads here.
// Direction: outbound=true when going CAN2→CAN3, false for CAN3→CAN2.
void user_modify_payload(uint32_t id, uint8_t data[8], bool outbound) {
  // Example: if (id == 0x120 && outbound) data[2] = 0x55;
  (void)id;
  (void)data;
  (void)outbound;
}
