// // In your CAN receive handler:
// if (msg.id == 0x0A7) {

//     // 1. Observe: msg.buf[0..7] contains the original frame

//     // 2. Modify payload bytes (b1 = counter, leave alone; b2..b7 = data)
//     msg.buf[3] = some_modified_value;
//     msg.buf[5] = another_value;

//     // 3. Recompute checksum into buf[0]
//     build_frame_with_checksum_0xA7(msg.buf);

//     // 4. Forward
//     CAN3.write(msg);




// } else if (msg.id == 0x0A8) {
//     // Same pattern
//     msg.buf[4] = modified_value;
//     build_frame_with_checksum_0xA8(msg.buf);
//     CAN3.write(msg);
// }





/////



#include "checksum_0xA7_0xA8.h"
#include "motor_signals_A7_A8.h"
#include "motor_debug_A7_A8.h"

// ... inside your CAN3 receive loop:

if (msg.id == 0x0A7) {
    motor11_update(msg.buf);          // decode → g_mo11, throttled print

    // ---- YOUR MODIFICATIONS (edit g_mo11 fields directly) ----
    // g_mo11.EngineTqActual        = 150;
    // g_mo11.EngineTqTargetRaw     = 150;
    // g_mo11.EngineTqTargetFiltered = 150;
    // ----------------------------------------------------------

    motor11_encode(msg.buf, g_mo11);
    build_frame_with_checksum_0xA7(msg.buf);
    TFTCAN3.write(msg);
}

if (msg.id == 0x0A8) {
    motor12_update(msg.buf);          // decode → g_mo12, throttled print

    // ---- YOUR MODIFICATIONS ----
    // g_mo12.EngineTqPercent = 80;
    // g_mo12.EngineRPM_raw   = motor12_rpm_to_raw(3000.0f);
    // ----------------------------

    motor12_encode(msg.buf, g_mo12);
    build_frame_with_checksum_0xA8(msg.buf);
    TFTCAN3.write(msg);
}