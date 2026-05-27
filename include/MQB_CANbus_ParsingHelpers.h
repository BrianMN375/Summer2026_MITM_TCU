//   #include <FlexCAN_T4.h>
//   #include <Arduino.h>
  
//   // 0x106 - ESP - Brake Pressure
//     float parse_ESP_Brake_Pressure(const CAN_message_t &msg) {
//         // ESP_Brake_Pressure: start bit 16, length 10 bits, Intel byte order
//         // Byte 2 = bit 16-23, Byte 3 = bit 24-31
//         uint16_t raw = ((uint16_t)msg.buf[2]) | (((uint16_t)msg.buf[3] & 0x03) << 8);

//         // Apply scale and offset from DBC
//         float pressure_bar = (raw * 0.3f) - 30.0f;
//         return pressure_bar;
//     }



    // -----------------------------

#include <Arduino.h>
#include <FlexCAN_T4.h>

// 0x106 - ESP - Brakes
    struct ESP_05_t {
        uint8_t  CHECKSUM;
        uint8_t  COUNTER;
        bool     ESP_QBit_Brake_Pressure;
        bool     ESP_QBit_Driver_Brakes;
        uint8_t  ESP_Threshold_Negative_Pressure;
        float    ESP_Brake_Pressure;
        bool     ESP_Driver_Brakes;
        bool     ESP_Directory_TSK_Active;
        bool     ESP_Steering_Intervention_ADS;
        bool     ESP_Consistent_TSK;
        bool     ESP_Brake_Pressure_AWV2;
        bool     ESP_Consistent_AWV2;
        bool     ECD_Error;
        bool     ECD_Not_Available;
        bool     ESP_Status_Brake_Temp;
        bool     ESP_Autohold_Standby;
        bool     ESP_HDC_Standby;
        bool     ESP_HBA_Active;
        bool     ESP_Prefill_Triggered;
        bool     ESP_Reversing_Detected;
        bool     ESP_Status_Starting_Aid;
        bool     ESP_HDC_aktiv;
        uint8_t  ESP_StartStopp_Info;
        bool     ESP_Eingr_HL;
        bool     ESP_Eingr_HR;
        bool     ESP_Eingr_VL;
        bool     ESP_Eingr_VR;
        uint16_t ESP_BKV_Negative_Pressure;
        bool     ESP_Autohold_Active;
        bool     ESP_FStatus_Starting_Aid;
        bool     ESP_Verz_EPB_aktiv;
        bool     ECD_Brake_Light;
        bool     ESP_Delay_EPB_Available;
        bool     ESP_Status_Brake_Pressure;
        uint8_t  ESP_Requirement_EPB;
    };
    ESP_05_t parse_ESP_05(const CAN_message_t &msg) {
        ESP_05_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bit 12
        s.ESP_QBit_Brake_Pressure = (msg.buf[1] >> 4) & 0x01;

        // Bit 13
        s.ESP_QBit_Driver_Brakes = (msg.buf[1] >> 5) & 0x01;

        
        // Bits 14–15
        s.ESP_Threshold_Negative_Pressure = (msg.buf[1] >> 6) & 0x03;

        // Bits 16–25 (10 bits)
        {
            uint16_t raw = ((uint16_t)msg.buf[2]) | (((uint16_t)msg.buf[3] & 0x03) << 8);
            s.ESP_Brake_Pressure = (raw * 0.3f) - 30.0f;

        }

        // Bit 26
        s.ESP_Driver_Brakes = (msg.buf[3] >> 2) & 0x01;

        // Bits 27–31
        s.ESP_Directory_TSK_Active   = (msg.buf[3] >> 3) & 0x01;
        s.ESP_Steering_Intervention_ADS = (msg.buf[3] >> 4) & 0x01;
        s.ESP_Consistent_TSK         = (msg.buf[3] >> 5) & 0x01;
        s.ESP_Brake_Pressure_AWV2    = (msg.buf[3] >> 6) & 0x01;
        s.ESP_Consistent_AWV2        = (msg.buf[3] >> 7) & 0x01;

        // Bits 32–39 (byte 4)
        s.ECD_Error                = (msg.buf[4] >> 0) & 0x01;
        s.ECD_Not_Available        = (msg.buf[4] >> 1) & 0x01;
        s.ESP_Status_Brake_Temp    = (msg.buf[4] >> 2) & 0x01;
        s.ESP_Autohold_Standby     = (msg.buf[4] >> 3) & 0x01;
        s.ESP_HDC_Standby          = (msg.buf[4] >> 4) & 0x01;
        s.ESP_HBA_Active           = (msg.buf[4] >> 5) & 0x01;
        s.ESP_Prefill_Triggered    = (msg.buf[4] >> 6) & 0x01;
        s.ESP_Reversing_Detected   = (msg.buf[4] >> 7) & 0x01;

        // Bits 40–47 (byte 5)
        s.ESP_Status_Starting_Aid  = (msg.buf[5] >> 0) & 0x01;
        s.ESP_HDC_aktiv            = (msg.buf[5] >> 1) & 0x01;
        s.ESP_StartStopp_Info      = (msg.buf[5] >> 2) & 0x03;
        s.ESP_Eingr_HL             = (msg.buf[5] >> 4) & 0x01;
        s.ESP_Eingr_HR             = (msg.buf[5] >> 5) & 0x01;
        s.ESP_Eingr_VL             = (msg.buf[5] >> 6) & 0x01;
        s.ESP_Eingr_VR             = (msg.buf[5] >> 7) & 0x01;

        // Bits 48–55 (byte 6)
        s.ESP_BKV_Negative_Pressure = msg.buf[6] * 4; // (4,0) scaling → raw * 4

        // Bits 56–63 (byte 7)
        s.ESP_Autohold_Active      = (msg.buf[7] >> 0) & 0x01;
        s.ESP_FStatus_Starting_Aid = (msg.buf[7] >> 1) & 0x01;
        s.ESP_Verz_EPB_aktiv       = (msg.buf[7] >> 2) & 0x01;
        s.ECD_Brake_Light          = (msg.buf[7] >> 3) & 0x01;
        s.ESP_Delay_EPB_Available  = (msg.buf[7] >> 4) & 0x01;
        s.ESP_Status_Brake_Pressure = (msg.buf[7] >> 5) & 0x01;
        s.ESP_Requirement_EPB      = (msg.buf[7] >> 6) & 0x03;

        return s;
    }

// 0x101 - ESP - Chassis Dynamics
    // =============================================================
    // MQB ESP_02 — CAN ID 0x101 (257)
    // Length: 8 bytes
    // =============================================================
    struct ESP_02_t {
            uint8_t  CHECKSUM;                             // bits 0–7
            uint8_t  COUNTER;                              // bits 8–11
            bool     ESP_QBit_Yaw_Rate;                    // bit 12
            bool     ESP_QBit_LongitudinalMovement;        // bit 13
            bool     ESP_QBit_LateralMovement;             // bit 14
            bool     ESP_Standstill_Flag;                  // bit 15
            float    ESP_LateralAcceleration;              // bits 16–23 (0.01, -1.27)
            float    ESP_LongitudinalAcceleration;         // bits 24–33 (10 bits, 0.03125, -16)
            float    ESP_Distribution_Wankmom;             // bits 34–38 (5 bits, 0.1, -1)
            bool     ESP_QBit_Initial_Value_Wank;          // bit 39
            float    ESP_Yaw_Rate;                         // bits 40–53 (14 bits, 0.01, 0)
            bool     ESP_VZ_YawRate;                       // bit 54
            bool     ESP_EmergencyBrakeIndicator;          // bit 55
            bool     ESP_Voltage_Initial;                  // bit 56
            uint8_t  ESP_PLA_Abort;                        // bits 57–59
            uint8_t  ESP_Status_ESP_PLA;                   // bits 60–63
        };
    ESP_02_t parse_ESP_02(const CAN_message_t &msg) {
            ESP_02_t s = {};

            // Byte 0
            s.CHECKSUM = msg.buf[0];

            // Byte 1
            s.COUNTER = msg.buf[1] & 0x0F;
            s.ESP_QBit_Yaw_Rate             = (msg.buf[1] >> 4) & 0x01;
            s.ESP_QBit_LongitudinalMovement = (msg.buf[1] >> 5) & 0x01;
            s.ESP_QBit_LateralMovement      = (msg.buf[1] >> 6) & 0x01;
            s.ESP_Standstill_Flag           = (msg.buf[1] >> 7) & 0x01;

            // Byte 2 (bits 16–23)
            s.ESP_LateralAcceleration = (msg.buf[2] * 0.01f) - 1.27f;

            // Bits 24–33 (10 bits, spanning bytes 3–4)
            {
                uint16_t raw = ((uint16_t)msg.buf[3]) | (((uint16_t)msg.buf[4] & 0x03) << 8);
                s.ESP_LongitudinalAcceleration = (raw * 0.03125f) - 16.0f;
            }

            // Bits 34–38 (5 bits → bits 2–6 of byte 4)
            s.ESP_Distribution_Wankmom = (((msg.buf[4] >> 2) & 0x1F) * 0.1f) - 1.0f;

            // Bit 39
            s.ESP_QBit_Initial_Value_Wank = (msg.buf[4] >> 7) & 0x01;

            // Bits 40–53 (14 bits → bytes 5–6)
            {
                uint16_t raw = ((uint16_t)msg.buf[5]) | (((uint16_t)msg.buf[6] & 0x3F) << 8);
                s.ESP_Yaw_Rate = raw * 0.01f;  // (0.01, 0)
            }

            // Bit 54
            s.ESP_VZ_YawRate = (msg.buf[6] >> 6) & 0x01;

            // Bit 55
            s.ESP_EmergencyBrakeIndicator = (msg.buf[6] >> 7) & 0x01;

            // Bits 56–63 (byte 7)
            s.ESP_Voltage_Initial = (msg.buf[7] >> 0) & 0x01;
            s.ESP_PLA_Abort       = (msg.buf[7] >> 1) & 0x07;
            s.ESP_Status_ESP_PLA  = (msg.buf[7] >> 4) & 0x0F;

            return s;
        }

// 0x11E - Drivetrain - Brake
    struct ESP_08_t {
        uint8_t  ESP_08_CRC;                          // bits 0–7
        uint8_t  ESP_08_BZ;                           // bits 8–11
        bool     ESP_ANB_CM_Reonversion;              // bit 12
        bool     ESP_Consistency_ACC_Message;         // bit 13
        bool     ESP_StandStillPhase_Exhausted;       // bit 14
        bool     ESP_ZT_ReConversion;                 // bit 15
        bool     ESP_DoorContact_DriversDoor;         // bit 16
        bool     ESP_Slipping_Stillstand;             // bit 18
        bool     ESP_Fahrer_tritt_ZBR_Schw;           // bit 19
        bool     ESP_QBit_v_ref;                      // bit 41
        uint8_t  ESP_v_ref_Direction_Of_Travel;       // bits 42–43
        uint16_t ESC_Brake_Pressure_Gradient;         // bits 44–51 (scaled)
        float    ESP_v_ref;                           // bits 52–63 (12 bits scaled)
    };
    ESP_08_t parse_ESP_08(const CAN_message_t &msg) {
        ESP_08_t s = {};

        // Byte 0
        s.ESP_08_CRC = msg.buf[0];

        // Byte 1
        s.ESP_08_BZ = msg.buf[1] & 0x0F;
        s.ESP_ANB_CM_Reonversion         = (msg.buf[1] >> 4) & 0x01;
        s.ESP_Consistency_ACC_Message    = (msg.buf[1] >> 5) & 0x01;
        s.ESP_StandStillPhase_Exhausted  = (msg.buf[1] >> 6) & 0x01;
        s.ESP_ZT_ReConversion            = (msg.buf[1] >> 7) & 0x01;

        // Byte 2
        s.ESP_DoorContact_DriversDoor = (msg.buf[2] >> 0) & 0x01;
        s.ESP_Slipping_Stillstand     = (msg.buf[2] >> 2) & 0x01;
        s.ESP_Fahrer_tritt_ZBR_Schw   = (msg.buf[2] >> 3) & 0x01;

        // Byte 5 (bit 41) to Byte 7
        s.ESP_QBit_v_ref = (msg.buf[5] >> 1) & 0x01;

        // Bits 42–43 (bits 2–3 of byte 5)
        s.ESP_v_ref_Direction_Of_Travel = (msg.buf[5] >> 2) & 0x03;

        // Bits 44–51 (byte 5 bits 4–7 + byte 6 bits 0–3)
        {
            uint16_t raw = ((uint16_t)(msg.buf[5] >> 4)) | (((uint16_t)(msg.buf[6] & 0x0F)) << 4);
            s.ESC_Brake_Pressure_Gradient = raw * 10; // (10,0)
        }

        // Bits 52–63 (12 bits)
        {
            uint16_t raw = (((uint16_t)(msg.buf[6] >> 4)) & 0x0F) | (((uint16_t)msg.buf[7]) << 4);
            s.ESP_v_ref = raw * 0.125f;  // (0.125, 0)
        }

        return s;
    }

// 0x116 - ESP
    struct ESP_10_t {
        uint8_t  CHECKSUM;                           // bits 0–7
        uint8_t  COUNTER;                            // bits 8–11
        bool     ESP_QBit_Path_Impulse_VL;           // bit 12
        bool     ESP_QBit_Path_Impulse_VR;           // bit 13
        bool     ESP_QBit_Path_Impulse_HL;           // bit 14
        bool     ESP_QBit_Path_Impulse_HR;           // bit 15

        uint16_t ESP_Path_Impulse_VL;                // bits 16–25, scale (1,0)
        uint16_t ESP_Path_Impulse_VR;                // bits 26–35
        uint16_t ESP_Path_Impulse_HL;                // bits 36–45
        uint16_t ESP_Path_Impulse_HR;                // bits 46–55

        uint8_t  ESP_VL_Direction_of_Travel;         // bits 56–57
        uint8_t  ESP_VR_Direction_of_Travel;         // bits 58–59
        uint8_t  ESP_HL_Direction_of_Travel;         // bits 60–61
        uint8_t  ESP_HR_Direction_of_Travel;         // bits 62–63
    };
    ESP_10_t parse_ESP_10(const CAN_message_t &msg) {
        ESP_10_t s = {};

        // Byte 0
        s.CHECKSUM = msg.buf[0];

        // Byte 1
        s.COUNTER = msg.buf[1] & 0x0F;
        s.ESP_QBit_Path_Impulse_VL = (msg.buf[1] >> 4) & 0x01;
        s.ESP_QBit_Path_Impulse_VR = (msg.buf[1] >> 5) & 0x01;
        s.ESP_QBit_Path_Impulse_HL = (msg.buf[1] >> 6) & 0x01;
        s.ESP_QBit_Path_Impulse_HR = (msg.buf[1] >> 7) & 0x01;

        // 10-bit values (2 bytes each, little-endian bit packing)
        // Each has scale (1,0)
        // Bit offset 16 → byte 2, bit 0
        {
            uint16_t raw = ((uint16_t)msg.buf[2]) | (((uint16_t)msg.buf[3] & 0x03) << 8);
            s.ESP_Path_Impulse_VL = raw;
        }
        {
            uint16_t raw = ((uint16_t)(msg.buf[3] >> 2)) | (((uint16_t)(msg.buf[4] & 0x0F)) << 6);
            s.ESP_Path_Impulse_VR = raw;
        }
        {
            uint16_t raw = ((uint16_t)(msg.buf[4] >> 4)) | (((uint16_t)(msg.buf[5] & 0x3F)) << 4);
            s.ESP_Path_Impulse_HL = raw;
        }
        {
            uint16_t raw = ((uint16_t)(msg.buf[5] >> 6)) | (((uint16_t)(msg.buf[6])) << 2);
            s.ESP_Path_Impulse_HR = raw & 0x03FF;
        }

        // Final two bits of each wheel direction (bits 56–63 → byte 7)
        uint8_t dir = msg.buf[7];
        s.ESP_VL_Direction_of_Travel = (dir >> 0) & 0x03;
        s.ESP_VR_Direction_of_Travel = (dir >> 2) & 0x03;
        s.ESP_HL_Direction_of_Travel = (dir >> 4) & 0x03;
        s.ESP_HR_Direction_of_Travel = (dir >> 6) & 0x03;

        return s;
    }

// 0x3C8 / 968 - Chassis and Gearbox Status
    struct Getriebe_14_t {
        uint8_t  GE_OBD_ShutoffValve;             // bits 12–15
        float    GE_amax_Possible;                // bits 16–24, (0.024, -2.016)
        uint8_t  GE_Charisma_FahrPr;              // bits 25–28
        uint8_t  GE_Charisma_Status;              // bits 29–30
        uint8_t  GE_TorqueLoss;                // bits 32–39
        bool     GE_Freigabe_Verfallsinfo_WFS;    // bit 49
        bool     GE_Codierung_MSG;                // bit 50
        bool     GE_LaunchControl;                // bit 51
        uint8_t  GE_HeatingRequest;               // bits 52–53
        bool     GE_OBD_Status;                   // bit 54
        bool     GE_LFR_Adaption;                 // bit 55
        int16_t  GE_Sumpftemperatur;              // bits 56–63, (1,-58)
    };
    Getriebe_14_t parse_Getriebe_14(const CAN_message_t &msg) {
        Getriebe_14_t s = {};

        // Byte 1 → bits 8–15 contain GE_OBD_ShutoffValve (bits 12–15)
        s.GE_OBD_ShutoffValve = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–24 → spans byte2 + 1 bit from byte3 (9 bits)
        {
            uint16_t raw = ((uint16_t)msg.buf[2]) | (((uint16_t)(msg.buf[3] & 0x01)) << 8);
            s.GE_amax_Possible = (raw * 0.024f) - 2.016f;
        }

        // Bits 25–28 (byte3 bits 1–4)
        s.GE_Charisma_FahrPr = (msg.buf[3] >> 1) & 0x0F;

        // Bits 29–30 (byte3 bits 5–6)
        s.GE_Charisma_Status = (msg.buf[3] >> 5) & 0x03;

        // Bits 32–39 → byte4
        s.GE_TorqueLoss = msg.buf[4];

        // Bits 49–55 → byte6 bits 1–7
        s.GE_Freigabe_Verfallsinfo_WFS = (msg.buf[6] >> 1) & 0x01;
        s.GE_Codierung_MSG             = (msg.buf[6] >> 2) & 0x01;
        s.GE_LaunchControl             = (msg.buf[6] >> 3) & 0x01;
        s.GE_HeatingRequest            = (msg.buf[6] >> 4) & 0x03;
        s.GE_OBD_Status                = (msg.buf[6] >> 6) & 0x01;
        s.GE_LFR_Adaption              = (msg.buf[6] >> 7) & 0x01;

        // Bits 56–63 → byte7 (signed, scale 1, offset -58)
        s.GE_Sumpftemperatur = ((int16_t)msg.buf[7]) - 58;

        return s;
    }

// 0x104 / 260 - EPB
    struct EPB_01_t {
        uint8_t  EPB_01_CRC;                         // bits 0–7
        uint8_t  EPB_01_BZ;                          // bits 8–11
        bool     EPB_QBit_Longitudinal_Acceleration; // bit 12
        bool     EPB_QBit_Clutch_Pedal_Travel;       // bit 13
        bool     EPB_BCM2_Motor_Wakeup;              // bit 14
        bool     EPB_Release_Delay_Request;          // bit 15

        float    EPB_Delay_requested;                // bits 16–23, (0.048, -7.968)
        int8_t   EPB_Longitudinal_acceleration;      // bits 24–31, (1, -128)
        float    EPB_Clutch_Pedal_Travel;            // bits 32–39, (0.4, 0)

        bool     EPB_Release_Deceleration_Start;     // bit 48
        bool     EPB_DAA_Randbed_erf;                // bit 49
        uint8_t  EPB_ErrorStatus;                    // bits 50–51
        uint8_t  EPB_Switch_Position;                // bits 52–53
        bool     EPB_QBit_Switch_Position;           // bit 54
        bool     EPB_Konsistenz_ACC;                 // bit 55
        uint8_t  EPB_Clamping_Force;                 // bits 56–60 (scaled 1,0)
        uint8_t  EPB_Status;                         // bits 61–62
    };
    EPB_01_t parse_EPB_01(const CAN_message_t &msg) {
        EPB_01_t s = {};

        // Byte 0
        s.EPB_01_CRC = msg.buf[0];

        // Byte 1
        s.EPB_01_BZ = msg.buf[1] & 0x0F;
        s.EPB_QBit_Longitudinal_Acceleration = (msg.buf[1] >> 4) & 0x01;
        s.EPB_QBit_Clutch_Pedal_Travel       = (msg.buf[1] >> 5) & 0x01;
        s.EPB_BCM2_Motor_Wakeup              = (msg.buf[1] >> 6) & 0x01;
        s.EPB_Release_Delay_Request          = (msg.buf[1] >> 7) & 0x01;

        // Byte 2 — EPB_Delay_requested (0.048, -7.968)
        s.EPB_Delay_requested = (float)msg.buf[2] * 0.048f - 7.968f;

        // Byte 3 — EPB_Longitudinal_acceleration (1, -128)
        s.EPB_Longitudinal_acceleration = ((int8_t)msg.buf[3]) - 128;

        // Byte 4 — EPB_Clutch_Pedal_Travel (0.4, 0)
        s.EPB_Clutch_Pedal_Travel = (float)msg.buf[4] * 0.4f;

        // Byte 6 — bits 48–55
        s.EPB_Release_Deceleration_Start = (msg.buf[6] >> 0) & 0x01;
        s.EPB_DAA_Randbed_erf            = (msg.buf[6] >> 1) & 0x01;
        s.EPB_ErrorStatus                = (msg.buf[6] >> 2) & 0x03;
        s.EPB_Switch_Position            = (msg.buf[6] >> 4) & 0x03;
        s.EPB_QBit_Switch_Position       = (msg.buf[6] >> 6) & 0x01;
        s.EPB_Konsistenz_ACC             = (msg.buf[6] >> 7) & 0x01;

        // Byte 7 — bits 56–63
        s.EPB_Clamping_Force = msg.buf[7] & 0x1F;      // bits 0–4 (5 bits)
        s.EPB_Status         = (msg.buf[7] >> 5) & 0x03; // bits 5–6

        return s;
    }

// 0x392 / 914 0- ESP per-wheel torque adaption and sensor flags
    struct ESP_07_t {
        uint8_t  ESP_07_CRC;                    // bits 0–7
        uint8_t  ESP_07_BZ;                     // bits 8–11
        bool     ESP_ACC_LDE;                   // bit 12
        bool     ESP_Quattro_Drie;              // bit 13
        uint8_t  ESP_Coding_ADS;                // bits 14–15

        float    ESP_RTA_HL;                    // bits 16–23,  (0.048828125, -6.20117)
        float    ESP_RTA_HR;                    // bits 24–31,  (0.048828125, -6.20117)
        float    ESP_RTA_VR;                    // bits 32–39,  (0.048828125, -6.20117)

        uint8_t  OBD_Wheel_Sensor_Error_HL;     // bits 40–43
        uint8_t  OBD_Wheel_Sensor_Error_HR;     // bits 44–47
        uint8_t  OBD_Wheel_Sensor_Error_VL;     // bits 48–51
        uint8_t  OBD_Wheel_Sensor_Error_VR;     // bits 52–55

        bool     ESP_Qualification_Drive_Type;  // bit 56
        bool     ESP_Offroad_Mode;              // bit 57
        bool     ESP_MKB_Triggerable;           // bit 58
        bool     ESP_MKB_Status;                // bit 59
        bool     ESP_CM_Variant;                // bit 60
        bool     ESP_OBD_Status;                // bit 61
    };
    ESP_07_t parse_ESP_07(const CAN_message_t &msg) {
        ESP_07_t s = {};

        // Byte 0
        s.ESP_07_CRC = msg.buf[0];

        // Byte 1
        s.ESP_07_BZ       = msg.buf[1] & 0x0F;
        s.ESP_ACC_LDE     = (msg.buf[1] >> 4) & 0x01;
        s.ESP_Quattro_Drie= (msg.buf[1] >> 5) & 0x01;
        s.ESP_Coding_ADS  = (msg.buf[1] >> 6) & 0x03;

        // Bytes 2–4 — three torque-adaptation channels
        s.ESP_RTA_HL = (float)msg.buf[2] * 0.048828125f - 6.20117f;
        s.ESP_RTA_HR = (float)msg.buf[3] * 0.048828125f - 6.20117f;
        s.ESP_RTA_VR = (float)msg.buf[4] * 0.048828125f - 6.20117f;

        // Byte 5 — sensor-error nibbles HL/HR
        s.OBD_Wheel_Sensor_Error_HL =  msg.buf[5] & 0x0F;
        s.OBD_Wheel_Sensor_Error_HR = (msg.buf[5] >> 4) & 0x0F;

        // Byte 6 — sensor-error nibbles VL/VR
        s.OBD_Wheel_Sensor_Error_VL =  msg.buf[6] & 0x0F;
        s.OBD_Wheel_Sensor_Error_VR = (msg.buf[6] >> 4) & 0x0F;

        // Byte 7 — bit flags
        uint8_t b7 = msg.buf[7];
        s.ESP_Qualification_Drive_Type = (b7 >> 0) & 0x01;
        s.ESP_Offroad_Mode              = (b7 >> 1) & 0x01;
        s.ESP_MKB_Triggerable           = (b7 >> 2) & 0x01;
        s.ESP_MKB_Status                = (b7 >> 3) & 0x01;
        s.ESP_CM_Variant                = (b7 >> 4) & 0x01;
        s.ESP_OBD_Status                = (b7 >> 5) & 0x01;

        return s;
    }

// 0xFD / 253 - chassis and system flags
    struct ESP_21_t {
        uint8_t  CHECKSUM;                        // bits 0–7
        uint8_t  COUNTER;                         // bits 8–11
        int16_t  BR_Engagement_Torque;            // bits 12–21, (1, -509)
        bool     ESP_PLA_Brake_Intervention;      // bit 22
        bool     ESP_Diagnose;                    // bit 23
        bool     ESC_Reku_Release;                // bit 24
        uint8_t  ESC_v_Signal_Qualifier_High_Low; // bits 25–27
        bool     ESP_Feedforward_control;         // bit 28
        bool     ESP_AWV3_Brakes_Active;          // bit 29
        bool     OBD_Bad_Road;                    // bit 30
        bool     OBD_QBit_Bad_Road;               // bit 31
        float    ESP_v_Signal;                    // bits 32–47, (0.01, 0)
        bool     ASR_Keying_Passive;              // bit 48
        bool     ESP_Keying_Passive;              // bit 49
        bool     ESP_Systemstatus;                // bit 50
        uint8_t  ASR_Intervention_Switch;         // bits 51–52
        bool     ESP_Hold_Confirmation;           // bit 53
        bool     ESP_MKB_Abort_Speed;             // bit 54
        bool     ESP_QBit_v_Signal;               // bit 55
        bool     ABS_Braking;                     // bit 56
        bool     ASR_Initial;                     // bit 57
        bool     MSR_Initial;                     // bit 58
        bool     EBV_Intervention;                // bit 59
        bool     EDS_Intervention;                // bit 60
        bool     ESP_Intervention;                // bit 61
        bool     ESP_ASP;                         // bit 62
        bool     ESP_Stopping_Process_ACC_aktiv;  // bit 63
    };
    ESP_21_t parse_ESP_21(const CAN_message_t &msg) {
        ESP_21_t s = {};

        // Byte 0
        s.CHECKSUM = msg.buf[0];

        // Byte 1
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–21 (10 bits)
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) | (((uint16_t)(msg.buf[2] & 0x3F)) << 4);
            s.BR_Engagement_Torque = (int16_t)raw - 509;
        }

        // Byte 2 bits 6–7 through Byte 3 bits 0–7
        uint32_t bits = ((uint32_t)msg.buf[2] >> 6) | ((uint32_t)msg.buf[3] << 2) | ((uint32_t)msg.buf[4] << 10);

        s.ESP_PLA_Brake_Intervention      = (bits >> 0) & 0x01;
        s.ESP_Diagnose                    = (bits >> 1) & 0x01;
        s.ESC_Reku_Release                = (bits >> 2) & 0x01;
        s.ESC_v_Signal_Qualifier_High_Low = (bits >> 3) & 0x07;
        s.ESP_Feedforward_control         = (bits >> 6) & 0x01;
        s.ESP_AWV3_Brakes_Active          = (bits >> 7) & 0x01;
        s.OBD_Bad_Road                    = (bits >> 8) & 0x01;
        s.OBD_QBit_Bad_Road               = (bits >> 9) & 0x01;

        // Bytes 4–5 → ESP_v_Signal (0.01, 0)
        s.ESP_v_Signal = (float)(((uint16_t)msg.buf[4]) | ((uint16_t)msg.buf[5] << 8)) * 0.01f;

        // Byte 6 bits 0–7
        s.ASR_Keying_Passive             = (msg.buf[6] >> 0) & 0x01;
        s.ESP_Keying_Passive             = (msg.buf[6] >> 1) & 0x01;
        s.ESP_Systemstatus               = (msg.buf[6] >> 2) & 0x01;
        s.ASR_Intervention_Switch        = (msg.buf[6] >> 3) & 0x03;
        s.ESP_Hold_Confirmation          = (msg.buf[6] >> 5) & 0x01;
        s.ESP_MKB_Abort_Speed            = (msg.buf[6] >> 6) & 0x01;
        s.ESP_QBit_v_Signal              = (msg.buf[6] >> 7) & 0x01;

        // Byte 7 bits 0–7
        uint8_t b7 = msg.buf[7];
        s.ABS_Braking                    = (b7 >> 0) & 0x01;
        s.ASR_Initial                    = (b7 >> 1) & 0x01;
        s.MSR_Initial                    = (b7 >> 2) & 0x01;
        s.EBV_Intervention               = (b7 >> 3) & 0x01;
        s.EDS_Intervention               = (b7 >> 4) & 0x01;
        s.ESP_Intervention               = (b7 >> 5) & 0x01;
        s.ESP_ASP                        = (b7 >> 6) & 0x01;
        s.ESP_Stopping_Process_ACC_aktiv = (b7 >> 7) & 0x01;

        return s;
    }

// 0xB2 / 253 - chassis and system flags
    struct ESP_19_t {
        float ESP_HL_Wheel_Speed_02;  // bits 0–15   (0.0075, 0)
        float ESP_HR_Wheel_Speed_02;  // bits 16–31  (0.0075, 0)
        float ESP_VL_Wheel_Speed_02;  // bits 32–47  (0.0075, 0)
        float ESP_VR_Wheel_Speed_02;  // bits 48–63  (0.0075, 0)
    };
    ESP_19_t parse_ESP_19(const CAN_message_t &msg) {
        ESP_19_t s = {};

        // Each wheel speed is a 16-bit unsigned value (little-endian)
        s.ESP_HL_Wheel_Speed_02 = (float)((uint16_t)msg.buf[0] | ((uint16_t)msg.buf[1] << 8)) * 0.0075f;
        s.ESP_HR_Wheel_Speed_02 = (float)((uint16_t)msg.buf[2] | ((uint16_t)msg.buf[3] << 8)) * 0.0075f;
        s.ESP_VL_Wheel_Speed_02 = (float)((uint16_t)msg.buf[4] | ((uint16_t)msg.buf[5] << 8)) * 0.0075f;
        s.ESP_VR_Wheel_Speed_02 = (float)((uint16_t)msg.buf[6] | ((uint16_t)msg.buf[7] << 8)) * 0.0075f;

        return s;
    }

// 0x30B / 779 - Kombi
    struct Kombi_01_t {
        // Byte 0
        bool KBI_ABS_Lampe;
        bool KBI_ESP_Lampe;
        bool KBI_BKL_Lampe;
        bool KBI_Airbag_Lampe;
        bool KBI_SILA_gueltig;
        bool KBI_Lenkung_Lampe;
        bool KBI_Vorglueh_System_Lampe;
        bool KBI_NV_in_Anzeige;

        // Byte 1
        uint8_t Kombi_01_BZ;              // bits 8–11
        bool KBI_Anzeigestatus_ACC;       // bit 12
        uint8_t KBI_Anzeigestatus_GRA;    // bits 13–14
        bool KBI_Oeldruck_Schalter;       // bit 15

        // Byte 2
        bool KBI_Tankwarnung;             // bit 16
        bool KBI_MFA_v_Einheit_01;        // bit 17
        bool KBI_im_Stellgliedtest;       // bit 18
        uint8_t KBI_Anzeigefehler_LDW;    // bits 19–20
        bool KBI_Variante_USA;            // bit 21
        bool KBI_Oeldruckwarnung;         // bit 22
        bool KBI_HandBrake;               // bit 23

        // Bytes 3–4
        uint16_t KBI_V_Digital;           // bits 24–32 (9 bits)
        bool KBI_PLA_in_Anzeige;          // bit 33
        bool KBI_Anzeigefehler_NV;        // bit 34
        uint8_t KBI_Anzeigestatus_LIM;    // bits 35–36

        // Bytes 6–7
        float KBI_angez_Geschw;           // bits 48–57 (10 bits, scale 0.32 km/h)
        bool KBI_Einheit_Tacho;           // bit 58
        bool KBI_Konsistenz_ACC;          // bit 59
        bool KBI_Fehler_Anzeige_ACC;      // bit 60
        uint8_t KBI_Anzeigefehler_SWA;    // bits 61–62
    };
    Kombi_01_t parse_Kombi_01(const CAN_message_t &msg) {
        Kombi_01_t s = {};

        uint8_t b0 = msg.buf[0];
        s.KBI_ABS_Lampe             = (b0 >> 0) & 0x01;
        s.KBI_ESP_Lampe             = (b0 >> 1) & 0x01;
        s.KBI_BKL_Lampe             = (b0 >> 2) & 0x01;
        s.KBI_Airbag_Lampe          = (b0 >> 3) & 0x01;
        s.KBI_SILA_gueltig          = (b0 >> 4) & 0x01;
        s.KBI_Lenkung_Lampe         = (b0 >> 5) & 0x01;
        s.KBI_Vorglueh_System_Lampe = (b0 >> 6) & 0x01;
        s.KBI_NV_in_Anzeige         = (b0 >> 7) & 0x01;

        uint8_t b1 = msg.buf[1];
        s.Kombi_01_BZ               =  b1 & 0x0F;
        s.KBI_Anzeigestatus_ACC     = (b1 >> 4) & 0x01;
        s.KBI_Anzeigestatus_GRA     = (b1 >> 5) & 0x03;
        s.KBI_Oeldruck_Schalter     = (b1 >> 7) & 0x01;

        uint8_t b2 = msg.buf[2];
        s.KBI_Tankwarnung           = (b2 >> 0) & 0x01;
        s.KBI_MFA_v_Einheit_01      = (b2 >> 1) & 0x01;
        s.KBI_im_Stellgliedtest     = (b2 >> 2) & 0x01;
        s.KBI_Anzeigefehler_LDW     = (b2 >> 3) & 0x03;
        s.KBI_Variante_USA          = (b2 >> 5) & 0x01;
        s.KBI_Oeldruckwarnung       = (b2 >> 6) & 0x01;
        s.KBI_HandBrake             = (b2 >> 7) & 0x01;

        // 9-bit digital speed (bytes 3–4)
        uint16_t raw_vdigital = ((uint16_t)msg.buf[3]) | (((uint16_t)msg.buf[4] & 0x01) << 8);
        s.KBI_V_Digital = raw_vdigital;
        s.KBI_PLA_in_Anzeige       = (msg.buf[4] >> 1) & 0x01;
        s.KBI_Anzeigefehler_NV     = (msg.buf[4] >> 2) & 0x01;
        s.KBI_Anzeigestatus_LIM    = (msg.buf[4] >> 3) & 0x03;

        // bytes 6–7: analog speed + flags
        uint16_t raw_speed = ((uint16_t)msg.buf[6]) | (((uint16_t)msg.buf[7] & 0x03) << 8);
        s.KBI_angez_Geschw = (float)raw_speed * 0.32f;
        s.KBI_Einheit_Tacho        = (msg.buf[7] >> 2) & 0x01;
        s.KBI_Konsistenz_ACC       = (msg.buf[7] >> 3) & 0x01;
        s.KBI_Fehler_Anzeige_ACC   = (msg.buf[7] >> 4) & 0x01;
        s.KBI_Anzeigefehler_SWA    = (msg.buf[7] >> 5) & 0x03;

        return s;
    }

// 0x3BE / 958 - Motor_14
    struct Motor_14_t {
        uint8_t MO_StartStopp_Status;           // bits 12–13
        bool    MO_StartStopp_Wiederstart;      // bit 14
        bool    MO_StartStopp_Motorstopp;       // bit 15
        uint8_t MO_Freig_Reku;                  // bits 16–17
        bool    MO_Kl_75;                       // bit 18
        bool    MO_Kl_50;                       // bit 19
        uint8_t MO_Gangposition;                // bits 20–23
        uint8_t MO_StartStopp_Fahrerwunsch;     // bits 24–25
        bool    MO_HYB_DriverReadiness;         // bit 26
        bool    MO_Ext_E_Drive_Active;          // bit 27
        bool    MO_Driver_Brakes;               // bit 28
        bool    MO_QBit_Driver_Brakes;          // bit 29
        bool    MO_BLS;                         // bit 30
        bool    MO_Consistency_BrakePad;        // bit 31
        bool    MO_Timeout_ESP;                 // bit 32
        uint8_t MO_Climate_Entry;               // bits 33–34
        bool    MO_Special_Occasion;            // bit 35
        bool    MO_Release_Occasion;            // bit 36
        bool    MO_Clutch_Switch;               // bit 37
        bool    MO_Interlock;                   // bit 38
        bool    MO_Motor_Running;               // bit 39
        bool    MO_Kickdown;                    // bit 40
        bool    MO_Status_CylinderSwitching_01; // bit 41
        uint8_t MO_EKlKomLeiRed;                // bits 42–43
        bool    MO_Handshake_STH;               // bit 44
        bool    MO_BKV_LowPressureWarning;      // bit 45
        bool    MO_Release_Sailing;             // bit 46
        uint8_t MO_PTC_Status;                  // bits 47–49
        bool    MO_QBit_GearPosition;           // bit 50
        bool    MO_SignalSource_GearPosition;   // bit 51
        bool    MO_Remotestart_Operation;       // bit 52
    };
    Motor_14_t parse_Motor_14(const CAN_message_t &msg) {
        Motor_14_t s = {};
        const uint8_t *d = msg.buf;

        // --- Byte 1 (low nibble from Byte 1 and onward) ---
        uint32_t bits = 0;
        for (int i = 0; i < 8; ++i)
            bits |= ((uint32_t)d[i]) << (8 * i);

        s.MO_StartStopp_Status           = (bits >> 12) & 0x03;
        s.MO_StartStopp_Wiederstart      = (bits >> 14) & 0x01;
        s.MO_StartStopp_Motorstopp       = (bits >> 15) & 0x01;
        s.MO_Freig_Reku                  = (bits >> 16) & 0x03;
        s.MO_Kl_75                       = (bits >> 18) & 0x01;
        s.MO_Kl_50                       = (bits >> 19) & 0x01;
        s.MO_Gangposition                = (bits >> 20) & 0x0F;
        s.MO_StartStopp_Fahrerwunsch     = (bits >> 24) & 0x03;
        s.MO_HYB_DriverReadiness         = (bits >> 26) & 0x01;
        s.MO_Ext_E_Drive_Active          = (bits >> 27) & 0x01;
        s.MO_Driver_Brakes               = (bits >> 28) & 0x01;
        s.MO_QBit_Driver_Brakes          = (bits >> 29) & 0x01;
        s.MO_BLS                         = (bits >> 30) & 0x01;
        s.MO_Consistency_BrakePad        = (bits >> 31) & 0x01;
        s.MO_Timeout_ESP                 = (bits >> 32) & 0x01;
        s.MO_Climate_Entry               = (bits >> 33) & 0x03;
        s.MO_Special_Occasion            = (bits >> 35) & 0x01;
        s.MO_Release_Occasion            = (bits >> 36) & 0x01;
        s.MO_Clutch_Switch               = (bits >> 37) & 0x01;
        s.MO_Interlock                   = (bits >> 38) & 0x01;
        s.MO_Motor_Running               = (bits >> 39) & 0x01;
        s.MO_Kickdown                    = (bits >> 40) & 0x01;
        s.MO_Status_CylinderSwitching_01 = (bits >> 41) & 0x01;
        s.MO_EKlKomLeiRed                = (bits >> 42) & 0x03;
        s.MO_Handshake_STH               = (bits >> 44) & 0x01;
        s.MO_BKV_LowPressureWarning      = (bits >> 45) & 0x01;
        s.MO_Release_Sailing             = (bits >> 46) & 0x01;
        s.MO_PTC_Status                  = (bits >> 47) & 0x07;
        s.MO_QBit_GearPosition           = (bits >> 50) & 0x01;
        s.MO_SignalSource_GearPosition   = (bits >> 51) & 0x01;
        s.MO_Remotestart_Operation       = (bits >> 52) & 0x01;

        return s;
    }

// 0xAD / 173 - Gearbox_11
    struct Getriebe_11_t {
        uint8_t  CHECKSUM;                    // bits 0–7
        uint8_t  COUNTER_DISABLED;            // bits 8–11
        int16_t  GE_MMom_Soll_02;             // bits 12–21,  (1, -509)
        int16_t  GE_MMom_Vorhalt_02;          // bits 22–31,  (1, -509)
        float    GE_Uefkt;                    // bits 32–41,  (0.1, 0)
        uint8_t  GE_Fahrstufe;                // bits 42–45
        bool     GE_reserv_Fahrstufe;         // bit 46
        uint8_t  GE_SwitchingSequence;        // bits 47–48
        bool     GE_Effect_Implausible;       // bit 49
        uint8_t  GE_MMom_Status_02;           // bits 50–52
        uint8_t  GE_Status_TractionControl;   // bits 53–55
        uint8_t  GE_MMom_Status;              // bits 56–57
        bool     GE_Freig_MMom_Vorhalt;       // bit 58
        bool     GE_Prohibition_of_Hiding;    // bit 59
        uint8_t  GE_TargetGear;               // bits 60–63
    };
    Getriebe_11_t parse_Getriebe_11(const CAN_message_t &msg) {
        Getriebe_11_t s = {};
        const uint8_t *d = msg.buf;

        // Flatten to a 64-bit integer for convenient bit-field extraction
        uint64_t bits = 0;
        for (int i = 0; i < 8; ++i)
            bits |= ((uint64_t)d[i]) << (8 * i);

        s.CHECKSUM                  = (bits >> 0)  & 0xFF;
        s.COUNTER_DISABLED          = (bits >> 8)  & 0x0F;
        s.GE_MMom_Soll_02           = ((bits >> 12) & 0x3FF) - 509;
        s.GE_MMom_Vorhalt_02        = ((bits >> 22) & 0x3FF) - 509;
        s.GE_Uefkt                  = ((bits >> 32) & 0x3FF) * 0.1f;
        s.GE_Fahrstufe              = (bits >> 42) & 0x0F;
        s.GE_reserv_Fahrstufe       = (bits >> 46) & 0x01;
        s.GE_SwitchingSequence      = (bits >> 47) & 0x03;
        s.GE_Effect_Implausible     = (bits >> 49) & 0x01;
        s.GE_MMom_Status_02         = (bits >> 50) & 0x07;
        s.GE_Status_TractionControl = (bits >> 53) & 0x07;
        s.GE_MMom_Status            = (bits >> 56) & 0x03;
        s.GE_Freig_MMom_Vorhalt     = (bits >> 58) & 0x01;
        s.GE_Prohibition_of_Hiding  = (bits >> 59) & 0x01;
        s.GE_TargetGear             = (bits >> 60) & 0x0F;

        return s;
    }

// 0x31B / 795 - ESP_24
    struct ESP_24_t {
        uint8_t  CHECKSUM;                // bits 0–7
        uint8_t  COUNTER;                 // bits 8–11
        float    ESP24_VehicleSpeed;        // bits 16–31, (0.5, 0)
        uint16_t ESP24_UnknownByte4;   // bits 32–47, raw 16-bit value
        uint16_t ESP24_WheelRotationCounter;// bits 40–50, 11-bit field (rolling counter / diagnostic)
    };
    ESP_24_t parse_ESP_24(const CAN_message_t &msg) {
        ESP_24_t s = {};
        const uint8_t *d = msg.buf;

        // Flatten buffer to 64-bit for easier bit access
        uint64_t bits = 0;
        for (int i = 0; i < 8; ++i)
            bits |= ((uint64_t)d[i]) << (8 * i);

        s.CHECKSUM              = (bits >> 0) & 0xFF;
        s.COUNTER               = (bits >> 8) & 0x0F;
        s.ESP24_VehicleSpeed      = ((bits >> 16) & 0xFFFF) * 0.5f;
        s.ESP24_UnknownByte4 = (bits >> 32) & 0xFFFF;
        s.ESP24_WheelRotationCounter = (bits >> 40) & 0x7FF; // 11 bits

        return s;
    }

// 0x3BE / 958 - Motor_14

