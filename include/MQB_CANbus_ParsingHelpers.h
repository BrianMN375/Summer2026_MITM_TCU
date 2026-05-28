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

#include <FlexCAN_T4.h>
#include <Arduino.h>



// 0x40 / 64 - Airbag_01 - good for ignition status
    struct Airbag_01_t {
        uint8_t CHECKSUM;
        uint8_t COUNTER;
        uint8_t AB_RGS_Anst;
        bool    AB_Front_Crash;
        bool    AB_Heck_Crash;
        bool    AB_SF_Crash;
        bool    AB_SB_Crash;
        bool    AB_Rollover_Crash;
        uint8_t AB_Crash_Int;
        bool    AB_Lampe;
        bool    AB_Deaktiviert;
        bool    AB_VB_deaktiviert;
        bool    AB_Systemfehler;
        bool    AB_Diagnose;
        bool    AB_ActuatorTest;
        uint8_t AB_Erh_Auf_VB;
        bool    AB_Gurtwarn_VF;
        bool    AB_Gurtwarn_VB;
        uint8_t AB_Anzeige_Fussg;
        uint8_t AB_Texte_AKS;
        bool    AB_PAO_Leuchte_Anf;
        bool    AB_MKB_Valid;
        bool    AB_MKB_Requirement;
        bool    AB_Versorgungsspannung;
        uint8_t AB_Deaktivierung_HV;
        uint8_t AB_EDR_Trigger;
        bool    AB_Gurtwarn_HFS;
        bool    AB_Gurtwarn_HBFS;
        float   SC_Masterzeit_Offset;
        float   SC_Masterzeit;
    };
    Airbag_01_t parse_Airbag_01(const CAN_message_t &msg) {
        Airbag_01_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–15
        s.AB_RGS_Anst = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–23
        s.AB_Front_Crash    = (msg.buf[2] >> 0) & 0x01;
        s.AB_Heck_Crash     = (msg.buf[2] >> 1) & 0x01;
        s.AB_SF_Crash       = (msg.buf[2] >> 2) & 0x01;
        s.AB_SB_Crash       = (msg.buf[2] >> 3) & 0x01;
        s.AB_Rollover_Crash = (msg.buf[2] >> 4) & 0x01;
        s.AB_Crash_Int      = (msg.buf[2] >> 5) & 0x07;

        // Bits 24–31
        s.AB_Lampe          = (msg.buf[3] >> 0) & 0x01;
        s.AB_Deaktiviert    = (msg.buf[3] >> 1) & 0x01;
        s.AB_VB_deaktiviert = (msg.buf[3] >> 2) & 0x01;
        s.AB_Systemfehler   = (msg.buf[3] >> 3) & 0x01;
        s.AB_Diagnose       = (msg.buf[3] >> 4) & 0x01;
        s.AB_ActuatorTest = (msg.buf[3] >> 5) & 0x01;
        s.AB_Erh_Auf_VB     = (msg.buf[3] >> 6) & 0x03;

        // Bits 32–39
        s.AB_Gurtwarn_VF     = (msg.buf[4] >> 0) & 0x01;
        s.AB_Gurtwarn_VB     = (msg.buf[4] >> 1) & 0x01;
        s.AB_Anzeige_Fussg   = (msg.buf[4] >> 2) & 0x03;
        s.AB_Texte_AKS       = (msg.buf[4] >> 4) & 0x03;
        s.AB_PAO_Leuchte_Anf = (msg.buf[4] >> 6) & 0x01;
        s.AB_MKB_Valid     = (msg.buf[4] >> 7) & 0x01;

        // Bits 40–47
        s.AB_MKB_Requirement    = (msg.buf[5] >> 0) & 0x01;
        s.AB_Versorgungsspannung = (msg.buf[5] >> 1) & 0x01;
        s.AB_Deaktivierung_HV   = (msg.buf[5] >> 2) & 0x07;
        s.AB_EDR_Trigger        = (msg.buf[5] >> 5) & 0x03;
        s.AB_Gurtwarn_HFS       = (msg.buf[5] >> 7) & 0x01;

        // Bits 48–55
        s.AB_Gurtwarn_HBFS = (msg.buf[6] >> 0) & 0x01;

        // Bits 53–54 (factor 5.08)
        s.SC_Masterzeit_Offset = ((msg.buf[6] >> 5) & 0x03) * 5.08f;

        // Bits 57–63 (7 bits), factor 0.04
        s.SC_Masterzeit = ((msg.buf[7] >> 1) & 0x7F) * 0.04f;

        return s;
    }


// 0x86 / 134 - LWI_01 - steering wheel angle and info
    struct LWI_01_t {
        uint8_t  CHECKSUM;
        uint8_t  COUNTER;
        bool     LWI_Sensorstatus;
        bool     LWI_QBit_Sub_Daten;
        bool     LWI_QBit_SteeringWheelAngle;
        float    LWI_SteeringWheelAngle;
        bool     LWI_VZ_SteeringWheelAngle;
        bool     LWI_VZ_SteeringWheelSpeed;
        uint16_t LWI_SteeringWheelSpeed;
        uint16_t LWI_Sub_Daten;
    };
    LWI_01_t parse_LWI_01(const CAN_message_t &msg) {
        LWI_01_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bit 12
        s.LWI_Sensorstatus = (msg.buf[1] >> 4) & 0x01;

        // Bit 13
        s.LWI_QBit_Sub_Daten = (msg.buf[1] >> 5) & 0x01;

        // Bit 15
        s.LWI_QBit_SteeringWheelAngle = (msg.buf[1] >> 7) & 0x01;

        // Bits 16–28 (13 bits), factor 0.1
        {
            uint16_t raw = ((uint16_t)msg.buf[2]) |
                        ((((uint16_t)msg.buf[3]) & 0x1F) << 8);
            raw &= 0x1FFF;
            s.LWI_SteeringWheelAngle = raw * 0.1f;
        }

        // Bit 29
        s.LWI_VZ_SteeringWheelAngle = (msg.buf[3] >> 5) & 0x01;

        // Bit 30
        s.LWI_VZ_SteeringWheelSpeed = (msg.buf[3] >> 6) & 0x01;

        // Bits 31–39 (9 bits), factor 5
        {
            uint16_t raw = ((uint16_t)(msg.buf[3] >> 7)) |
                        (((uint16_t)msg.buf[4]) << 1);
            raw &= 0x01FF;
            s.LWI_SteeringWheelSpeed = raw * 5;
        }

        // Bits 40–55 (16 bits)
        s.LWI_Sub_Daten = ((uint16_t)msg.buf[5]) |
                        (((uint16_t)msg.buf[6]) << 8);

        return s;
    }

// 0xA7 - Motor_11 - Engine Torques
    struct Motor_11_t {
        uint8_t  Motor_11_CRC;
        uint8_t  Motor_11_BZ;
        int16_t  MO_EngineTqTargetRaw_0xA7;
        int16_t  MO_EngineTqActual_0xA7;
        int16_t  MO_EngineTotalMomentsInertia;
        int16_t  MO_EngineTqTargetFiltered_0xA7;
        int16_t  MO_EngineTqThrust;
        bool     MO_Status_Normalbetrieb_01;
        bool     MO_erste_Ungenauschwelle;
        bool     MO_QBit_Motormomente;
    };
    Motor_11_t parse_Motor_11(const CAN_message_t &msg) {
        Motor_11_t s = {};

        // Bits 0–7
        s.Motor_11_CRC = msg.buf[0];

        // Bits 8–11
        s.Motor_11_BZ = msg.buf[1] & 0x0F;

        // Bits 12–21 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) | (((uint16_t)msg.buf[2]) << 4);
            raw &= 0x03FF;
            s.MO_EngineTqTargetRaw_0xA7 = (int16_t)raw - 509;
        }

        // Bits 22–31 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[2] >> 6)) |
                        (((uint16_t)msg.buf[3]) << 2);
            raw &= 0x03FF;
            s.MO_EngineTqActual_0xA7 = (int16_t)raw - 509;
        }

        // Bits 32–41 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)msg.buf[4]) |
                        ((((uint16_t)msg.buf[5]) & 0x03) << 8);
            raw &= 0x03FF;
            s.MO_EngineTotalMomentsInertia = (int16_t)raw - 509;
        }

        // Bits 42–51 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[5] >> 2)) |
                        (((uint16_t)msg.buf[6]) << 6);
            raw &= 0x03FF;
            s.MO_EngineTqTargetFiltered_0xA7 = (int16_t)raw - 509;
        }

        // Bits 52–60 (9 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[6] >> 4)) |
                        ((((uint16_t)msg.buf[7]) & 0x1F) << 4);
            raw &= 0x01FF;
            s.MO_EngineTqThrust = (int16_t)raw - 509;
        }

        // Bit 61
        s.MO_Status_Normalbetrieb_01 = (msg.buf[7] >> 5) & 0x01;

        // Bit 62
        s.MO_erste_Ungenauschwelle = (msg.buf[7] >> 6) & 0x01;

        // Bit 63
        s.MO_QBit_Motormomente = (msg.buf[7] >> 7) & 0x01;

        return s;
    }

// 0xA8 - Motor_12 - Engine RPM and some Torques - not yet translated and formatted
    struct Motor_12_t {
    uint8_t  Motor_12_CRC;
    uint8_t  Motor_12_BZ;
    int16_t  MO_12_EngineTq_Neg_Available;
    uint16_t MO_12_EngineTqLimit_Stat;
    int16_t  MO_12_EngineTqLimit_Dyn;
    uint8_t  MO_12_EngineTqPercent;
    bool     MO_12_QBit_EngineRPM;
    float    MO_12_EngineRPM;

    };
    Motor_12_t parse_Motor_12(const CAN_message_t &msg) {
        Motor_12_t s = {};

        // Bits 0–7
        s.Motor_12_CRC = msg.buf[0];

        // Bits 8–11
        s.Motor_12_BZ = msg.buf[1] & 0x0F;

        // Bits 12–20 (9 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) |
                        ((((uint16_t)msg.buf[2]) & 0x1F) << 4);
            raw &= 0x01FF;
            s.MO_12_EngineTq_Neg_Available = (int16_t)raw - 509;
        }

        // Bits 21–29 (9 bits), factor 1, offset 0
        {
            uint16_t raw = ((uint16_t)(msg.buf[2] >> 5)) |
                        (((uint16_t)msg.buf[3]) << 3) |
                        ((((uint16_t)msg.buf[4]) & 0x03) << 11);
            raw &= 0x01FF;
            s.MO_12_EngineTqLimit_Stat = raw;
        }

        // Bits 30–39 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[3] >> 6)) |
                        (((uint16_t)msg.buf[4]) << 2);
            raw &= 0x03FF;
            s.MO_12_EngineTqLimit_Dyn = (int16_t)raw - 509;
        }

        // Bits 40–46 (7 bits), factor 1, offset 0
        {
            uint8_t raw = msg.buf[5] & 0x7F;
            s.MO_12_EngineTqPercent = raw;
        }

        // Bit 47
        s.MO_12_QBit_EngineRPM = (msg.buf[5] >> 7) & 0x01;

        // Bits 48–63 (16 bits), factor 0.25, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[6]) |
                        (((uint16_t)msg.buf[7]) << 8);
            s.MO_12_EngineRPM = raw * 0.25f;
        }

        return s;
    }

// 0xAD / 173 - Gearbox_11
    struct Getriebe_11_t {
        uint8_t  CHECKSUM;                    // bits 0–7
        uint8_t  COUNTER_DISABLED;            // bits 8–11
        int16_t  GE_EngineTqTarget_02;             // bits 12–21,  (1, -509)
        int16_t  GE_EngineTqLead_02;          // bits 22–31,  (1, -509)
        float    GE_Uefkt;                    // bits 32–41,  (0.1, 0)
        uint8_t  GE_DriveMode;                // bits 42–45
        bool     GE_ReserveDriveMode ;         // bit 46
        uint8_t  GE_SwitchingSequence;        // bits 47–48
        bool     GE_Effect_Implausible;       // bit 49
        uint8_t  GE_EngineTq_Status_02;           // bits 50–52
        uint8_t  GE_Status_TractionControl;   // bits 53–55
        uint8_t  GE_EngineTq_Status;              // bits 56–57
        bool     GE_ReleaseMomentLead ;       // bit 58
        bool     GE_ProhibitionOfHiding ;    // bit 59
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
        s.GE_EngineTqTarget_02           = ((bits >> 12) & 0x3FF) - 509;
        s.GE_EngineTqLead_02        = ((bits >> 22) & 0x3FF) - 509;
        s.GE_Uefkt                  = ((bits >> 32) & 0x3FF) * 0.1f;
        s.GE_DriveMode              = (bits >> 42) & 0x0F;
        s.GE_ReserveDriveMode        = (bits >> 46) & 0x01;
        s.GE_SwitchingSequence      = (bits >> 47) & 0x03;
        s.GE_Effect_Implausible     = (bits >> 49) & 0x01;
        s.GE_EngineTq_Status_02         = (bits >> 50) & 0x07;
        s.GE_Status_TractionControl = (bits >> 53) & 0x07;
        s.GE_EngineTq_Status            = (bits >> 56) & 0x03;
        s.GE_ReleaseMomentLead      = (bits >> 58) & 0x01;
        s.GE_ProhibitionOfHiding   = (bits >> 59) & 0x01;
        s.GE_TargetGear             = (bits >> 60) & 0x0F;

        return s;
    }


// 0xAE / 174 - Gearbox_12
    struct Getriebe_12_t {
        uint8_t  Getriebe_12_CRC;
        uint8_t  Getriebe_12_BZ;
        bool     GE_TachometerDamping;
        bool     GE_FuelOverrunCutoff;
        bool     GE_ReleaseSynchronization;
        uint16_t GE_SynchronousDesiredSpeed;
        uint16_t GE_SynchronizationTime;
        uint16_t GE_MomentBasedGradient;
        uint16_t GE_IdleSpeedSetpoint_Increase;
        int16_t  GE_RecordingMoment;
        uint8_t  GE_StartOfZylabsch;
        uint8_t  GE_HYB_DZ_Intervention;
    };
    Getriebe_12_t parse_Getriebe_12(const CAN_message_t &msg) {
        Getriebe_12_t s = {};

        // Bits 0–7
        s.Getriebe_12_CRC = msg.buf[0];

        // Bits 8–11
        s.Getriebe_12_BZ = msg.buf[1] & 0x0F;

        // Bit 12
        s.GE_TachometerDamping = (msg.buf[1] >> 4) & 0x01;

        // Bit 13
        s.GE_FuelOverrunCutoff = (msg.buf[1] >> 5) & 0x01;

        // Bit 14
        s.GE_ReleaseSynchronization = (msg.buf[1] >> 6) & 0x01;

        // Bits 15–23 (9 bits), factor 25, offset 0
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 7)) |
                        (((uint16_t)msg.buf[2]) << 1);
            raw &= 0x01FF;
            s.GE_SynchronousDesiredSpeed = raw * 25;
        }

        // Bits 24–31 (8 bits), factor 20, offset 0
        s.GE_SynchronizationTime = (uint16_t)msg.buf[3] * 20;

        // Bits 32–39 (8 bits), factor 10, offset 0
        s.GE_MomentBasedGradient = (uint16_t)msg.buf[4] * 10;

        // Bits 40–47 (8 bits), factor 10, offset 0
        s.GE_IdleSpeedSetpoint_Increase = (uint16_t)msg.buf[5] * 10;

        // Bits 48–57 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)msg.buf[6]) |
                        ((((uint16_t)msg.buf[7]) & 0x03) << 8);
            raw &= 0x03FF;
            s.GE_RecordingMoment = (int16_t)raw - 509;
        }

        // Bits 58–59
        s.GE_StartOfZylabsch = (msg.buf[7] >> 2) & 0x03;

        // Bits 62–63
        s.GE_HYB_DZ_Intervention = (msg.buf[7] >> 6) & 0x03;

        return s;
    }

// 0xB2 / 178 - chassis and system flags 
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


// 0xFD / 253 - ESP_21
    struct ESP_21_t {
        uint8_t  CHECKSUM;
        uint8_t  COUNTER;
        int16_t  BR_Engagement_Torque;
        bool     ESP21_PLA_Brake_Intervention;
        bool     ESP21_Diagnose;
        bool     ESC_Reku_Release;
        uint8_t  ESC_VehicleSpeed_Signal_Qualifier_High_Low;
        bool     ESP21_Feedforward_control;
        bool     ESP21_AWV3_Brakes_Active;
        bool     OBD_Bad_Road;
        bool     OBD_QBit_Bad_Road;
        float    ESP21_VehicleSpeed_Signal;
        bool     ASR_Keying_Passive;
        bool     ESP21_Keying_Passive;
        bool     ESP21_Systemstatus;
        uint8_t  ASR_Intervention_Switch;
        bool     ESP21_Hold_Confirmation;
        bool     ESP21_MKB_Abort_Speed;
        bool     ESP21_QBit_v_Signal;
        bool     ABS_Braking;
        bool     ASR_Initial;
        bool     MSR_Initial;
        bool     EBV_Intervention;
        bool     EDS_Intervention;
        bool     ESP21_Intervention;
        bool     ESP21_ASP;
        bool     ESP21_Stopping_Process_ACC_aktiv;
    };
    ESP_21_t parse_ESP_21(const CAN_message_t &msg) {
        ESP_21_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–21 (10 bits), factor 1, offset -509
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) |
                        (((uint16_t)msg.buf[2]) << 4);
            raw &= 0x03FF;
            s.BR_Engagement_Torque = (int16_t)raw - 509;
        }

        // Bit 22
        s.ESP21_PLA_Brake_Intervention = (msg.buf[2] >> 6) & 0x01;

        // Bit 23
        s.ESP21_Diagnose = (msg.buf[2] >> 7) & 0x01;

        // Bit 24
        s.ESC_Reku_Release = (msg.buf[3] >> 0) & 0x01;

        // Bits 25–27
        s.ESC_VehicleSpeed_Signal_Qualifier_High_Low = (msg.buf[3] >> 1) & 0x07;

        // Bit 28
        s.ESP21_Feedforward_control = (msg.buf[3] >> 4) & 0x01;

        // Bit 29
        s.ESP21_AWV3_Brakes_Active = (msg.buf[3] >> 5) & 0x01;

        // Bit 30
        s.OBD_Bad_Road = (msg.buf[3] >> 6) & 0x01;

        // Bit 31
        s.OBD_QBit_Bad_Road = (msg.buf[3] >> 7) & 0x01;

        // Bits 32–47 (16 bits), factor 0.01, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[4]) |
                        (((uint16_t)msg.buf[5]) << 8);
            s.ESP21_VehicleSpeed_Signal = raw * 0.01f;
        }

        // Bit 48
        s.ASR_Keying_Passive = (msg.buf[6] >> 0) & 0x01;

        // Bit 49
        s.ESP21_Keying_Passive = (msg.buf[6] >> 1) & 0x01;

        // Bit 50
        s.ESP21_Systemstatus = (msg.buf[6] >> 2) & 0x01;

        // Bits 51–52
        s.ASR_Intervention_Switch = (msg.buf[6] >> 3) & 0x03;

        // Bit 53
        s.ESP21_Hold_Confirmation = (msg.buf[6] >> 5) & 0x01;

        // Bit 54
        s.ESP21_MKB_Abort_Speed = (msg.buf[6] >> 6) & 0x01;

        // Bit 55
        s.ESP21_QBit_v_Signal = (msg.buf[6] >> 7) & 0x01;

        // Bits 56–63
        s.ABS_Braking = (msg.buf[7] >> 0) & 0x01;
        s.ASR_Initial = (msg.buf[7] >> 1) & 0x01;
        s.MSR_Initial = (msg.buf[7] >> 2) & 0x01;
        s.EBV_Intervention = (msg.buf[7] >> 3) & 0x01;
        s.EDS_Intervention = (msg.buf[7] >> 4) & 0x01;
        s.ESP21_Intervention = (msg.buf[7] >> 5) & 0x01;
        s.ESP21_ASP = (msg.buf[7] >> 6) & 0x01;
        s.ESP21_Stopping_Process_ACC_aktiv = (msg.buf[7] >> 7) & 0x01;

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

// 0x104 / 260 - EPB - Parking Brake
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
        bool     EPB_Consistency_ACC;                 // bit 55
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
        s.EPB_Consistency_ACC             = (msg.buf[6] >> 7) & 0x01;

        // Byte 7 — bits 56–63
        s.EPB_Clamping_Force = msg.buf[7] & 0x1F;      // bits 0–4 (5 bits)
        s.EPB_Status         = (msg.buf[7] >> 5) & 0x03; // bits 5–6

        return s;
    }

// 0x106 / 262 - ESP_05 - Brakes
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


// 0x107 / 263 - Motor_04 - Display values for Boost, Gear Actual, Gear Desired maybe, and Tachometer (loosely linked to reality)
    struct Motor_04_t {
        uint8_t  MO4_ActualGear;
        uint8_t  MO4_TargetGear;
        float    MO4_OilPressure;
        uint16_t MO4_DisplayedRPM;
        bool     MO4_GearShiftRecommendationAvailable;
        float    MO4_BoostPressure;
        uint16_t MO4_KVS;
        bool     MO4_KVS_Ueberlauf;
    };
    Motor_04_t parse_Motor_04(const CAN_message_t &msg) {
        Motor_04_t s = {};

        // Bits 8–11
        s.MO4_ActualGear = msg.buf[1] & 0x0F;

        // Bits 12–15
        s.MO4_TargetGear = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–23 (8 bits), factor 0.04
        s.MO4_OilPressure = msg.buf[2] * 0.04f;

        // Bits 24–35 (12 bits), factor 3
        {
            uint16_t raw = ((uint16_t)msg.buf[3]) |
                        ((((uint16_t)msg.buf[4]) & 0x0F) << 8);
            raw &= 0x0FFF;
            s.MO4_DisplayedRPM = raw * 3;
            s.MO4_DisplayedRPM = raw * 3;
        }

        // Bit 38
        s.MO4_GearShiftRecommendationAvailable = (msg.buf[4] >> 6) & 0x01;

        // Bits 39–47 (9 bits), factor 0.01
        {
            uint16_t raw = ((uint16_t)(msg.buf[4] >> 7)) |
                        (((uint16_t)msg.buf[5]) << 1);
            raw &= 0x01FF;
            s.MO4_BoostPressure = raw * 0.01f;
        }

        // Bits 48–62 (15 bits)
        {
            uint16_t raw = ((uint16_t)msg.buf[6]) |
                        (((uint16_t)(msg.buf[7] & 0x7F)) << 8);
            raw &= 0x7FFF;
            s.MO4_KVS = raw;
        }

        // Bit 63
        s.MO4_KVS_Ueberlauf = (msg.buf[7] >> 7) & 0x01;

        return s;
    }


// 0x116 / 278 - ESP_10
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


// 0x11E / 286 - ESP_08 - Drivetrain - Brake
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


// 0x120 / 288 - TSK_06 - Operational Statuses and Cruise Control on/off
    struct TSK_06_t {
        uint8_t  CHECKSUM;
        uint8_t  COUNTER;
        uint16_t TSK_WheelBrakeTorque;
        uint8_t  TSK_Status;
        bool     TSK_VehicleSpeed_Limitation_aktiv;
        bool     TSK_StandbyRequest_ESP;
        bool     TSK_Approval_WU;
        bool     TSK_ReleaseDelayStart;
        bool     TSK_Limiter_Selected;
        float    TSK_TranslationRequest;
        uint8_t  TSK_MainSwitch_CruiseControl_ONOFF;
        uint8_t  TSK_SRBM_Requirement_ASIL;
        float    TSK_ax_Gearbox_02;
        bool     TSK_ForcedSurcharge_ESP;
        float    TSK_AllowableStdDeviation;
    };
    TSK_06_t parse_TSK_06(const CAN_message_t &msg) {
        TSK_06_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–23 (12 bits), factor 8, offset 0
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) |
                        (((uint16_t)msg.buf[2]) << 4);
            raw &= 0x0FFF;
            s.TSK_WheelBrakeTorque = raw * 8;
        }

        // Bits 24–31
        s.TSK_Status               = (msg.buf[3] >> 0) & 0x07;  // bits 24–26
        s.TSK_VehicleSpeed_Limitation_aktiv   = (msg.buf[3] >> 3) & 0x01;  // bit 27
        s.TSK_StandbyRequest_ESP      = (msg.buf[3] >> 4) & 0x01;  // bit 28
        s.TSK_Approval_WU             = (msg.buf[3] >> 5) & 0x01;  // bit 29
        s.TSK_ReleaseDelayStart    = (msg.buf[3] >> 6) & 0x01;  // bit 30
        s.TSK_Limiter_Selected  = (msg.buf[3] >> 7) & 0x01;  // bit 31

        // Bits 32–41 (10 bits), factor 0.0245, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[4]) |
                        ((((uint16_t)msg.buf[5]) & 0x03) << 8);
            raw &= 0x03FF;
            s.TSK_TranslationRequest = raw * 0.0245f;
        }

        // Bits 42–43
        s.TSK_MainSwitch_CruiseControl_ONOFF = (msg.buf[5] >> 2) & 0x03;

        // Bits 44–46
        s.TSK_SRBM_Requirement_ASIL = (msg.buf[5] >> 4) & 0x07;

        // Bits 48–56 (9 bits), factor 0.024, offset -2.016
        {
            uint16_t raw = ((uint16_t)msg.buf[6]) |
                        ((((uint16_t)msg.buf[7]) & 0x01) << 8);
            raw &= 0x01FF;
            s.TSK_ax_Gearbox_02 = (raw * 0.024f) - 2.016f;
        }

        // Bit 57
        s.TSK_ForcedSurcharge_ESP = (msg.buf[7] >> 1) & 0x01;

        // Bits 58–63 (6 bits), factor 0.024, offset 0
        {
            uint8_t raw = (msg.buf[7] >> 2) & 0x3F;
            s.TSK_AllowableStdDeviation = raw * 0.024f;
        }

        return s;
    }


// 0x121 / 289 - Motor_20 - general engine operations
    struct Motor_20_t {
        uint8_t CHECKSUM;
        uint8_t COUNTER;
        float   MO_AccelPedalRaw_01;
        bool    MO_QBit_AccelPedalValues_01;
        uint16_t MO_AccelPedalGradient;
        bool    MO_Sig_AccelPedalGradient;
        uint16_t MO_rel_IntakeManifoldPressure;
        bool    MO_rel_IntakeManifoldPressure_gem_err;
        bool    MO_Torque_in_Idle;
        bool    MO_Overrun_Cutoff;
        bool    MO_StartStopp_Stop_Preparation;
        uint16_t MO_TargetIdleSpeed;
        uint16_t MO_Decoupling_TargetSlip;
        bool    MO_Temporary_DriverAbsence;
        float   TSK_a_Soll_Gradient_Limited;
    };
    Motor_20_t parse_Motor_20(const CAN_message_t &msg) {
        Motor_20_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–19 (8 bits), factor 0.4
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) |
                        ((((uint16_t)msg.buf[2]) & 0x0F) << 4);
            raw &= 0x00FF;
            s.MO_AccelPedalRaw_01 = raw * 0.4f;
        }

        // Bit 20
        s.MO_QBit_AccelPedalValues_01 = (msg.buf[2] >> 4) & 0x01;

        // Bits 21–28 (8 bits), factor 25
        {
            uint16_t raw = ((uint16_t)(msg.buf[2] >> 5)) |
                        ((((uint16_t)msg.buf[3]) & 0x3F) << 3);
            raw &= 0x00FF;
            s.MO_AccelPedalGradient = raw * 25;
        }

        // Bit 29
        s.MO_Sig_AccelPedalGradient = (msg.buf[3] >> 5) & 0x01;

        // Bits 30–35 (6 bits), factor 18
        {
            uint8_t raw = ((msg.buf[3] >> 6) & 0x03) |
                        ((msg.buf[4] & 0x0F) << 2);
            raw &= 0x3F;
            s.MO_rel_IntakeManifoldPressure = raw * 18;
        }

        // Bits 36–39
        s.MO_rel_IntakeManifoldPressure_gem_err = (msg.buf[4] >> 4) & 0x01;
        s.MO_Torque_in_Idle                     = (msg.buf[4] >> 5) & 0x01;
        s.MO_Overrun_Cutoff                     = (msg.buf[4] >> 6) & 0x01;
        s.MO_StartStopp_Stop_Preparation        = (msg.buf[4] >> 7) & 0x01;

        // Bits 40–47 (8 bits), factor 10
        s.MO_TargetIdleSpeed = msg.buf[5] * 10;

        // Bits 48–54 (7 bits), factor 20
        s.MO_Decoupling_TargetSlip = (msg.buf[6] & 0x7F) * 20;

        // Bit 55
        s.MO_Temporary_DriverAbsence = (msg.buf[6] >> 7) & 0x01;

        // Bits 57–63 (7 bits), factor 0.1, offset -7.2
        {
            uint8_t raw = (msg.buf[7] >> 1) & 0x7F;
            s.TSK_a_Soll_Gradient_Limited = (raw * 0.1f) - 7.2f;
        }

        return s;
    }


// 0x12B / 299 - GRA_ACC_01 - CruiseControl Stalk/Button activations
    struct GRA_ACC_01_t {
        uint8_t CHECKSUM;
        uint8_t COUNTER;
        bool    GRA_CruiseControl_MainSwitch;
        bool    GRA_CruiseControl_Cancel;
        bool    GRA_CruiseControl_Typ_MainSwitch;
        bool    GRA_CruiseControl_Limiter;
        bool    GRA_CruiseControl_SET;
        bool    GRA_CruiseControl_UP;
        bool    GRA_CruiseControl_DOWN;
        bool    GRA_CruiseControl_RES;
        uint8_t GRA_CruiseControl_AdjustmentTimeGap;
        uint8_t GRA_Coding;
        bool    GRA_Error;
        uint8_t GRA_Typ468;
        bool    GRA_Tip_Level_2;
        uint8_t GRA_ButtonTypeInfo;
    };
    GRA_ACC_01_t parse_GRA_ACC_01(const CAN_message_t &msg) {
        GRA_ACC_01_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–15
        s.GRA_CruiseControl_MainSwitch     = (msg.buf[1] >> 4) & 0x01;
        s.GRA_CruiseControl_Cancel         = (msg.buf[1] >> 5) & 0x01;
        s.GRA_CruiseControl_Typ_MainSwitch = (msg.buf[1] >> 6) & 0x01;
        s.GRA_CruiseControl_Limiter           = (msg.buf[1] >> 7) & 0x01;

        // Bits 16–23
        s.GRA_CruiseControl_SET           = (msg.buf[2] >> 0) & 0x01;
        s.GRA_CruiseControl_UP             = (msg.buf[2] >> 1) & 0x01;
        s.GRA_CruiseControl_DOWN           = (msg.buf[2] >> 2) & 0x01;
        s.GRA_CruiseControl_RES   = (msg.buf[2] >> 3) & 0x01;
        s.GRA_CruiseControl_AdjustmentTimeGap = (msg.buf[2] >> 4) & 0x03;
        s.GRA_Coding            = (msg.buf[2] >> 6) & 0x03;

        // Bits 24–29
        s.GRA_Error         = (msg.buf[3] >> 0) & 0x01;
        s.GRA_Typ468         = (msg.buf[3] >> 1) & 0x03;
        s.GRA_Tip_Level_2    = (msg.buf[3] >> 3) & 0x01;
        s.GRA_ButtonTypeInfo = (msg.buf[3] >> 4) & 0x03;

        return s;
    }


// 0x30B / 779 - Kombi_01
    struct Kombi_01_t {
        // Byte 0
        bool KBI_ABS_Lamp;
        bool KBI_ESP_Lamp;
        bool KBI_BKL_Lamp;
        bool KBI_Airbag_Lamp;
        bool KBI_SILA_Valid;
        bool KBI_Steering_Lamp;
        bool KBI_GlowPlug_System_Lamp;
        bool KBI_NV_in_Display;

        // Byte 1
        uint8_t Kombi_01_BZ;              // bits 8–11
        bool KBI_DisplayStatus_ACC;       // bit 12
        uint8_t KBI_DisplayStatus_GRA;    // bits 13–14
        bool KBI_OilPressure_Switch;       // bit 15

        // Byte 2
        bool KBI_FuelTank_Warning;             // bit 16
        bool KBI_MFA_v_Einheit_01;        // bit 17
        bool KBI_im_ActuatorTest;       // bit 18
        uint8_t KBI_DisplayError_LDW;    // bits 19–20
        bool KBI_Variante_USA;            // bit 21
        bool KBI_OilPressure_Warning;         // bit 22
        bool KBI_HandBrake;               // bit 23

        // Bytes 3–4
        uint16_t KBI_VehicleV_Digital;           // bits 24–32 (9 bits)
        bool KBI_PLA_in_Display;          // bit 33
        bool KBI_DisplayError_NV;        // bit 34
        uint8_t KBI_DisplayStatus_LIM;    // bits 35–36

        // Bytes 6–7
        float KBI_IndicatedSpeed;           // bits 48–57 (10 bits, scale 0.32 km/h)
        bool KBI_Unit_Tacho;           // bit 58
        bool KBI_Consistency_ACC;          // bit 59
        bool KBI_Error_Display_ACC;      // bit 60
        uint8_t KBI_DisplayError_SWA;    // bits 61–62
    };
    Kombi_01_t parse_Kombi_01(const CAN_message_t &msg) {
        Kombi_01_t s = {};

        uint8_t b0 = msg.buf[0];
        s.KBI_ABS_Lamp             = (b0 >> 0) & 0x01;
        s.KBI_ESP_Lamp             = (b0 >> 1) & 0x01;
        s.KBI_BKL_Lamp             = (b0 >> 2) & 0x01;
        s.KBI_Airbag_Lamp          = (b0 >> 3) & 0x01;
        s.KBI_SILA_Valid          = (b0 >> 4) & 0x01;
        s.KBI_Steering_Lamp         = (b0 >> 5) & 0x01;
        s.KBI_GlowPlug_System_Lamp = (b0 >> 6) & 0x01;
        s.KBI_NV_in_Display         = (b0 >> 7) & 0x01;

        uint8_t b1 = msg.buf[1];
        s.Kombi_01_BZ               =  b1 & 0x0F;
        s.KBI_DisplayStatus_ACC     = (b1 >> 4) & 0x01;
        s.KBI_DisplayStatus_GRA     = (b1 >> 5) & 0x03;
        s.KBI_OilPressure_Switch     = (b1 >> 7) & 0x01;

        uint8_t b2 = msg.buf[2];
        s.KBI_FuelTank_Warning           = (b2 >> 0) & 0x01;
        s.KBI_MFA_v_Einheit_01      = (b2 >> 1) & 0x01;
        s.KBI_im_ActuatorTest     = (b2 >> 2) & 0x01;
        s.KBI_DisplayError_LDW     = (b2 >> 3) & 0x03;
        s.KBI_Variante_USA          = (b2 >> 5) & 0x01;
        s.KBI_OilPressure_Warning       = (b2 >> 6) & 0x01;
        s.KBI_HandBrake             = (b2 >> 7) & 0x01;

        // 9-bit digital speed (bytes 3–4)
        uint16_t raw_vdigital = ((uint16_t)msg.buf[3]) | (((uint16_t)msg.buf[4] & 0x01) << 8);
        s.KBI_VehicleV_Digital = raw_vdigital;
        s.KBI_PLA_in_Display       = (msg.buf[4] >> 1) & 0x01;
        s.KBI_DisplayError_NV     = (msg.buf[4] >> 2) & 0x01;
        s.KBI_DisplayStatus_LIM    = (msg.buf[4] >> 3) & 0x03;

        // bytes 6–7: analog speed + flags       
        uint16_t raw_speed = ((uint16_t)msg.buf[6]) | (((uint16_t)msg.buf[7] & 0x03) << 8);
        s.KBI_IndicatedSpeed = (float)raw_speed * 0.32f;
        s.KBI_Unit_Tacho        = (msg.buf[7] >> 2) & 0x01;
        s.KBI_Consistency_ACC       = (msg.buf[7] >> 3) & 0x01;
        s.KBI_Error_Display_ACC   = (msg.buf[7] >> 4) & 0x01;
        s.KBI_DisplayError_SWA    = (msg.buf[7] >> 5) & 0x03;

        return s;
    }


// 0x30F / 783 - SWA_01
    struct SWA_01_t {
        uint8_t CHECKSUM;
        uint8_t COUNTER;
        uint8_t SWA_Anzeigen;
        bool    SWA_Blindheit_erkannt;
        bool    SWA_rel_Nichtverf;
        bool    SWA_rel_Fehler;
        bool    SWA_Sta_aktiv;
        bool    SWA_Sta_passiv;
        bool    SWA_FT_RueckLED;
        uint8_t ASW_Status;
        bool    SWA_Standziele_li;
        bool    SWA_Kolonne_li;
        bool    SWA_Infostufe_SWA_li;
        bool    SWA_Warnung_SWA_li;
        bool    ASW_Warnung_FS;
        bool    ASW_Warnung_BFS;
        uint8_t ASW_Kombitexte;
        bool    SWA_Kolonne_mi;
        bool    SWA_Standziele_re;
        bool    SWA_Kolonne_re;
        bool    SWA_Infostufe_SWA_re;
        bool    SWA_Warnung_SWA_re;
        uint8_t HRE_Anzeigetexte;
        uint8_t SWA_Gischtzaehler;
        uint8_t Heckradar_Kombitexte;
        uint8_t RCTA_Kombitexte;
    };
    SWA_01_t parse_SWA_01(const CAN_message_t &msg) {
        SWA_01_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 12–15
        s.SWA_Anzeigen = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–23
        s.SWA_Blindheit_erkannt = (msg.buf[2] >> 0) & 0x01;
        s.SWA_rel_Nichtverf     = (msg.buf[2] >> 1) & 0x01;
        s.SWA_rel_Fehler        = (msg.buf[2] >> 2) & 0x01;
        s.SWA_Sta_aktiv         = (msg.buf[2] >> 3) & 0x01;
        s.SWA_Sta_passiv        = (msg.buf[2] >> 4) & 0x01;
        s.SWA_FT_RueckLED       = (msg.buf[2] >> 5) & 0x01;
        s.ASW_Status            = (msg.buf[2] >> 6) & 0x03;

        // Bits 24–31
        s.SWA_Standziele_li     = (msg.buf[3] >> 0) & 0x01;
        s.SWA_Kolonne_li        = (msg.buf[3] >> 1) & 0x01;
        s.SWA_Infostufe_SWA_li  = (msg.buf[3] >> 2) & 0x01;
        s.SWA_Warnung_SWA_li    = (msg.buf[3] >> 3) & 0x01;
        s.ASW_Warnung_FS        = (msg.buf[3] >> 4) & 0x01;
        s.ASW_Warnung_BFS       = (msg.buf[3] >> 5) & 0x01;
        s.ASW_Kombitexte        = (msg.buf[3] >> 6) & 0x03;  // provisional, corrected below
        s.ASW_Kombitexte        = (msg.buf[3] >> 6) & 0x03;

        // Bit 32 unused in DBC
        // Bit 33
        s.SWA_Kolonne_mi = (msg.buf[4] >> 1) & 0x01;

        // Bits 40–47
        s.SWA_Standziele_re    = (msg.buf[5] >> 0) & 0x01;
        s.SWA_Kolonne_re       = (msg.buf[5] >> 1) & 0x01;
        s.SWA_Infostufe_SWA_re = (msg.buf[5] >> 2) & 0x01;
        s.SWA_Warnung_SWA_re   = (msg.buf[5] >> 3) & 0x01;
        s.HRE_Anzeigetexte     = (msg.buf[5] >> 4) & 0x0F;

        // Bits 48–54
        s.SWA_Gischtzaehler = msg.buf[6] & 0x7F;

        // Bits 56–63
        s.Heckradar_Kombitexte = (msg.buf[7] >> 0) & 0x1F;
        s.RCTA_Kombitexte      = (msg.buf[7] >> 5) & 0x07;

        // Correct 3-bit field at bits 30–32 across byte boundary
        s.ASW_Kombitexte = ((msg.buf[3] >> 6) & 0x03) |
                        ((msg.buf[4] & 0x01) << 2);

        return s;
    }


// 0x31B / 795 - ESP_24 - Checked Vehicle Speed and Wheel Rotation Counter
    struct ESP_24_t {
        uint8_t  ESP24_CHECKSUM;                // bits 0–7
        uint8_t  ESP24_COUNTER;                 // bits 8–11
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

        s.ESP24_CHECKSUM              = (bits >> 0) & 0xFF;
        s.ESP24_COUNTER               = (bits >> 8) & 0x0F;
        s.ESP24_VehicleSpeed      = ((bits >> 16) & 0xFFFF) * 0.5f;
        s.ESP24_UnknownByte4 = (bits >> 32) & 0xFFFF;
        s.ESP24_WheelRotationCounter = (bits >> 40) & 0x7FF; // 11 bits

        return s;
    }


// 0x31E / 798 - TSK_07
    struct TSK_07_t {
        uint8_t TSK_07_CRC;
        uint8_t TSK_07_BZ;
        float   TSK_GearboxDesiredSpeed;
        uint8_t TSK_Texts;
        uint8_t TSK_Acoustic;
        uint8_t TSK_PrimaryTexts;
        uint8_t TSK_Limiter_DriverInfo;
        bool    TSK_Limiter_Display;
        bool    TSK_VehicleStatus_GRA;
        bool    TSK_VehicleStatus_Limiter;
        bool    MO_EngineOperationWarning ;
        uint8_t TSK_StatusDisplay;
    };
    TSK_07_t parse_TSK_07(const CAN_message_t &msg) {
        TSK_07_t s = {};

        // Bits 0–7
        s.TSK_07_CRC = msg.buf[0];

        // Bits 8–11
        s.TSK_07_BZ = msg.buf[1] & 0x0F;

        // Bits 12–21 (10 bits), factor 0.32
        {
            uint16_t raw = ((uint16_t)(msg.buf[1] >> 4)) |
                        (((uint16_t)msg.buf[2]) << 4);
            raw &= 0x03FF;
            s.TSK_GearboxDesiredSpeed = raw * 0.32f;
        }

        // Bits 40–44
        s.TSK_Texts = msg.buf[5] & 0x1F;

        // Bits 45–47
        s.TSK_Acoustic = (msg.buf[5] >> 5) & 0x07;

        // Bits 48–52
        s.TSK_PrimaryTexts = msg.buf[6] & 0x1F;

        // Bits 53–54
        s.TSK_Limiter_DriverInfo = (msg.buf[6] >> 5) & 0x03;

        // Bit 55
        s.TSK_Limiter_Display = (msg.buf[6] >> 7) & 0x01;

        // Bits 56–58
        s.TSK_VehicleStatus_GRA     = (msg.buf[7] >> 0) & 0x01;
        s.TSK_VehicleStatus_Limiter = (msg.buf[7] >> 1) & 0x01;
        s.MO_EngineOperationWarning        = (msg.buf[7] >> 2) & 0x01;

        // Bits 61–63
        s.TSK_StatusDisplay = (msg.buf[7] >> 5) & 0x07;

        return s;
    }


// 0x366 / 870 - Hazard Lights and Switch
    struct Blinkmodi_02_t {
        bool Hazard_Switch;
        bool Comfort_Signal_Left;
        bool Comfort_Signal_Right;
        bool Left_Turn_Exterior_Bulb_1;
        bool Right_Turn_Exterior_Bulb_1;
        bool Left_Turn_Exterior_Bulb_2;
        bool Right_Turn_Exterior_Bulb_2;
        bool Fast_Send_Rate_Active;
    };
    Blinkmodi_02_t parse_Blinkmodi_02(const CAN_message_t &msg) {
        Blinkmodi_02_t s = {};

        // Bit 20
        s.Hazard_Switch = (msg.buf[2] >> 4) & 0x01;

        // Bit 23
        s.Comfort_Signal_Left = (msg.buf[2] >> 7) & 0x01;

        // Bit 24
        s.Comfort_Signal_Right = (msg.buf[3] >> 0) & 0x01;

        // Bit 25
        s.Left_Turn_Exterior_Bulb_1 = (msg.buf[3] >> 1) & 0x01;

        // Bit 26
        s.Right_Turn_Exterior_Bulb_1 = (msg.buf[3] >> 2) & 0x01;

        // Bit 27
        s.Left_Turn_Exterior_Bulb_2 = (msg.buf[3] >> 3) & 0x01;

        // Bit 28
        s.Right_Turn_Exterior_Bulb_2 = (msg.buf[3] >> 4) & 0x01;

        // Bit 37
        s.Fast_Send_Rate_Active = (msg.buf[4] >> 5) & 0x01;

        return s;
    }


// 0x385 / 901 - Charisma_01 - Drive Select Modes
    struct Charisma_01_t {
        uint8_t CHA_Ziel_FahrPr_ALR;
        uint8_t CHA_Ziel_FahrPr_ESP;
        uint8_t CHA_Ziel_FahrPr_FL;
        bool    CHA_Fahrer_Umschaltung;
        uint8_t CHA_Ziel_FahrPr_MO;
        uint8_t CHA_Ziel_FahrPr_GE;
        uint8_t CHA_Ziel_FahrPr_ST;
        uint8_t CHA_Ziel_FahrPr_SCU;
        uint8_t CHA_Ziel_FahrPr_DR;
        uint8_t CHA_Ziel_FahrPr_QS;
        uint8_t CHA_Ziel_FahrPr_AFS;
        uint8_t CHA_Ziel_FahrPr_RGS;
        uint8_t CHA_Ziel_FahrPr_EPS;
        uint8_t CHA_Ziel_FahrPr_ACC;
        uint8_t CHA_Ziel_FahrPr_SAK;
        uint8_t CHA_Ziel_FahrPr_MStSt;
    };
    Charisma_01_t parse_Charisma_01(const CAN_message_t &msg) {
        Charisma_01_t s = {};

        // Bits 0–3
        s.CHA_Ziel_FahrPr_ALR = msg.buf[0] & 0x0F;

        // Bits 4–7
        s.CHA_Ziel_FahrPr_ESP = (msg.buf[0] >> 4) & 0x0F;

        // Bits 8–11
        s.CHA_Ziel_FahrPr_FL = msg.buf[1] & 0x0F;

        // Bit 14
        s.CHA_Fahrer_Umschaltung = (msg.buf[1] >> 6) & 0x01;

        // Bits 16–19
        s.CHA_Ziel_FahrPr_MO = msg.buf[2] & 0x0F;

        // Bits 20–23
        s.CHA_Ziel_FahrPr_GE = (msg.buf[2] >> 4) & 0x0F;

        // Bits 24–27
        s.CHA_Ziel_FahrPr_ST = msg.buf[3] & 0x0F;

        // Bits 28–31
        s.CHA_Ziel_FahrPr_SCU = (msg.buf[3] >> 4) & 0x0F;

        // Bits 32–35
        s.CHA_Ziel_FahrPr_DR = msg.buf[4] & 0x0F;

        // Bits 36–39
        s.CHA_Ziel_FahrPr_QS = (msg.buf[4] >> 4) & 0x0F;

        // Bits 40–43
        s.CHA_Ziel_FahrPr_AFS = msg.buf[5] & 0x0F;

        // Bits 44–47
        s.CHA_Ziel_FahrPr_RGS = (msg.buf[5] >> 4) & 0x0F;

        // Bits 48–51
        s.CHA_Ziel_FahrPr_EPS = msg.buf[6] & 0x0F;

        // Bits 52–55
        s.CHA_Ziel_FahrPr_ACC = (msg.buf[6] >> 4) & 0x0F;

        // Bits 56–59
        s.CHA_Ziel_FahrPr_SAK = msg.buf[7] & 0x0F;

        // Bits 60–63
        s.CHA_Ziel_FahrPr_MStSt = (msg.buf[7] >> 4) & 0x0F;

        return s;
    }


// 0x392 / 914 - ESP_07 - Per-wheel torque adaption and sensor flags
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

// 0391 / 913 - OBD_01 - General Engine Operational stuff
    struct OBD_01_t {
        float OBD_Calc_Load_Val;
        int16_t OBD_Eng_Cool_Temp;
        float OBD_Abs_Throttle_Pos;
        float OBD_Abs_Load_Val;
        float OBD_Abs_Pedal_Pos;
        bool  OBD_ColdStart_Denominator;
        bool  OBD_Minimum_Trip;
        bool  OBD_Driving_Cycle;
        bool  OBD_Warm_Up_Cycle;
        bool  OBD_Normed_Trip;
    };
    OBD_01_t parse_OBD_01(const CAN_message_t &msg) {
        OBD_01_t s = {};

        // Bits 0–7 (8 bits), factor 0.39215686275, offset 0
        s.OBD_Calc_Load_Val = msg.buf[0] * 0.39215686275f;

        // Bits 8–15 (8 bits), factor 1, offset -40
        s.OBD_Eng_Cool_Temp = (int16_t)msg.buf[1] - 40;

        // Bits 16–23 (8 bits), factor 0.39215686275, offset 0
        s.OBD_Abs_Throttle_Pos = msg.buf[2] * 0.39215686275f;

        // Bits 24–39 (16 bits), factor 0.39215686275, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[3]) |
                        (((uint16_t)msg.buf[4]) << 8);
            s.OBD_Abs_Load_Val = raw * 0.39215686275f;
        }

        // Bits 40–47 (8 bits), factor 0.39215686275, offset 0
        s.OBD_Abs_Pedal_Pos = msg.buf[5] * 0.39215686275f;

        // Bits 59–63
        s.OBD_ColdStart_Denominator = (msg.buf[7] >> 3) & 0x01;
        s.OBD_Minimum_Trip          = (msg.buf[7] >> 4) & 0x01;
        s.OBD_Driving_Cycle         = (msg.buf[7] >> 5) & 0x01;
        s.OBD_Warm_Up_Cycle         = (msg.buf[7] >> 6) & 0x01;
        s.OBD_Normed_Trip           = (msg.buf[7] >> 7) & 0x01;

        return s;
    }


// 0x394 / 916 - WBA_03 - Gearbox Data
    struct WBA_03_t {
        uint8_t WBA_03_CRC;
        uint8_t WBA_03_BZ;
        uint8_t WBA_Gear_02;
        uint8_t WBA_TargetGear;
        uint8_t WBA_GE_Warning_02;
        uint8_t WBA_eing_Gang_02;
        uint8_t WBA_GE_Texte;
        bool    WBA_Segeln_aktiv;
        uint8_t WBA_Schaltschema;
    };
    WBA_03_t parse_WBA_03(const CAN_message_t &msg) {
        WBA_03_t s = {};

        // Bits 0–7
        s.WBA_03_CRC = msg.buf[0];

        // Bits 8–11
        s.WBA_03_BZ = msg.buf[1] & 0x0F;

        // Bits 12–15
        s.WBA_Gear_02 = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–19
        s.WBA_TargetGear = msg.buf[2] & 0x0F;

        // Bits 20–23
        s.WBA_GE_Warning_02 = (msg.buf[2] >> 4) & 0x0F;

        // Bits 24–27
        s.WBA_eing_Gang_02 = msg.buf[3] & 0x0F;

        // Bits 28–30
        s.WBA_GE_Texte = (msg.buf[3] >> 4) & 0x07;

        // Bit 31
        s.WBA_Segeln_aktiv = (msg.buf[3] >> 7) & 0x01;

        // Bits 32–36
        s.WBA_Schaltschema = msg.buf[4] & 0x1F;

        return s;
    }

// 0x397 / 919 - LDW_02
    struct LDW_02_t {
        uint8_t LDW_Gong;
        bool    LDW_SW_Warnung_links;
        bool    LDW_SW_Warnung_rechts;
        uint8_t LDW_Texte;
        bool    LDW_Seite_DLCTLC;
        uint8_t LDW_Lernmodus;
        uint8_t LDW_Anlaufsp_VLR;
        uint8_t LDW_Vib_Amp_VLR;
        uint8_t LDW_Anlaufzeit_VLR;
        uint8_t LDW_Lernmodus_rechts;
        uint8_t LDW_Lernmodus_links;
        float   LDW_DLC;
        float   LDW_TLC;
        bool    LDW_Warnung_links;
        bool    LDW_Warnung_rechts;
        uint8_t LDW_Codierinfo_fuer_VLR;
        bool    LDW_Frontscheibenheizung_aktiv;
        bool    LDW_Status_LED_gelb;
        bool    LDW_Status_LED_gruen;
        bool    LDW_KD_Fehler;
    };
    LDW_02_t parse_LDW_02(const CAN_message_t &msg) {
        LDW_02_t s = {};

        // Bits 12–15
        s.LDW_Gong               = (msg.buf[1] >> 4) & 0x03;  // bits 12–13
        s.LDW_SW_Warnung_links   = (msg.buf[1] >> 6) & 0x01;  // bit 14
        s.LDW_SW_Warnung_rechts  = (msg.buf[1] >> 7) & 0x01;  // bit 15

        // Bits 16–23
        s.LDW_Texte         = (msg.buf[2] >> 0) & 0x0F;  // bits 16–19
        s.LDW_Seite_DLCTLC  = (msg.buf[2] >> 4) & 0x01;  // bit 20
        s.LDW_Lernmodus     = (msg.buf[2] >> 5) & 0x07;  // bits 21–23

        // Bits 24–31
        s.LDW_Anlaufsp_VLR = (msg.buf[3] >> 0) & 0x0F;   // bits 24–27
        s.LDW_Vib_Amp_VLR  = (msg.buf[3] >> 4) & 0x0F;   // bits 28–31

        // Bits 32–39
        s.LDW_Anlaufzeit_VLR   = (msg.buf[4] >> 0) & 0x0F;  // bits 32–35
        s.LDW_Lernmodus_rechts = (msg.buf[4] >> 4) & 0x03;  // bits 36–37
        s.LDW_Lernmodus_links  = (msg.buf[4] >> 6) & 0x03;  // bits 38–39

        // Bits 40–47 (8 bits), factor 0.01, offset -1.25
        s.LDW_DLC = (msg.buf[5] * 0.01f) - 1.25f;

        // Bits 48–52 (5 bits), factor 0.1
        s.LDW_TLC = (msg.buf[6] & 0x1F) * 0.1f;

        // Bits 56–63
        s.LDW_Warnung_links               = (msg.buf[7] >> 0) & 0x01;
        s.LDW_Warnung_rechts              = (msg.buf[7] >> 1) & 0x01;
        s.LDW_Codierinfo_fuer_VLR         = (msg.buf[7] >> 2) & 0x03;
        s.LDW_Frontscheibenheizung_aktiv  = (msg.buf[7] >> 4) & 0x01;
        s.LDW_Status_LED_gelb             = (msg.buf[7] >> 5) & 0x01;
        s.LDW_Status_LED_gruen            = (msg.buf[7] >> 6) & 0x01;
        s.LDW_KD_Fehler                   = (msg.buf[7] >> 7) & 0x01;

        return s;
    }

// 0x3BE / 958 - Motor_14
    struct Motor_14_t {
        uint8_t MO_StartStopp_Status;
        bool    MO_StartStopp_Wiederstart;
        bool    MO_StartStopp_Motorstopp;
        uint8_t MO_Freig_Reku;
        bool    MO_Kl_75;
        bool    MO_Kl_50;
        uint8_t MO_Gangposition;
        uint8_t MO_StartStopp_Fahrerwunsch;
        bool    MO_HYB_DriverReadiness;
        bool    MO_Ext_E_Drive_Active;
        bool    MO_Driver_Brakes;
        bool    MO_QBit_Driver_Brakes;
        bool    MO_BLS;
        bool    MO_Consistency_BrakePad;
        bool    MO_Timeout_ESP;
        uint8_t MO_Climate_Entry;
        bool    MO_Special_Occasion;
        bool    MO_Release_Occasion;
        bool    MO_Clutch_Switch;
        bool    MO_Interlock;
        bool    MO_Motor_Running;
        bool    MO_Kickdown;
        bool    MO_Status_CylinderSwitching_01;
        uint8_t MO_EKlKomLeiRed;
        bool    MO_Handshake_STH;
        bool    MO_BKV_LowPressureWarning;
        bool    MO_Release_Sailing;
        uint8_t MO_PTC_Status;
        bool    MO_QBit_GearPosition;
        bool    MO_SignalSource_GearPosition;
        bool    MO_Remotestart_Operation;
    };  
    Motor_14_t parse_Motor_14(const CAN_message_t &msg) {
            Motor_14_t s = {};

            // Bits 12–15
            s.MO_StartStopp_Status       = (msg.buf[1] >> 4) & 0x03;  // bits 12–13
            s.MO_StartStopp_Wiederstart  = (msg.buf[1] >> 6) & 0x01;  // bit 14
            s.MO_StartStopp_Motorstopp   = (msg.buf[1] >> 7) & 0x01;  // bit 15

            // Bits 16–23
            s.MO_Freig_Reku              = (msg.buf[2] >> 0) & 0x03;  // bits 16–17
            s.MO_Kl_75                   = (msg.buf[2] >> 2) & 0x01;  // bit 18
            s.MO_Kl_50                   = (msg.buf[2] >> 3) & 0x01;  // bit 19
            s.MO_Gangposition            = (msg.buf[2] >> 4) & 0x0F;  // bits 20–23

            // Bits 24–31
            s.MO_StartStopp_Fahrerwunsch = (msg.buf[3] >> 0) & 0x03;  // bits 24–25
            s.MO_HYB_DriverReadiness     = (msg.buf[3] >> 2) & 0x01;  // bit 26
            s.MO_Ext_E_Drive_Active      = (msg.buf[3] >> 3) & 0x01;  // bit 27
            s.MO_Driver_Brakes           = (msg.buf[3] >> 4) & 0x01;  // bit 28
            s.MO_QBit_Driver_Brakes      = (msg.buf[3] >> 5) & 0x01;  // bit 29
            s.MO_BLS                     = (msg.buf[3] >> 6) & 0x01;  // bit 30
            s.MO_Consistency_BrakePad    = (msg.buf[3] >> 7) & 0x01;  // bit 31

            // Bits 32–39
            s.MO_Timeout_ESP             = (msg.buf[4] >> 0) & 0x01;  // bit 32
            s.MO_Climate_Entry           = (msg.buf[4] >> 1) & 0x03;  // bits 33–34
            s.MO_Special_Occasion        = (msg.buf[4] >> 3) & 0x01;  // bit 35
            s.MO_Release_Occasion        = (msg.buf[4] >> 4) & 0x01;  // bit 36
            s.MO_Clutch_Switch           = (msg.buf[4] >> 5) & 0x01;  // bit 37
            s.MO_Interlock               = (msg.buf[4] >> 6) & 0x01;  // bit 38
            s.MO_Motor_Running           = (msg.buf[4] >> 7) & 0x01;  // bit 39

            // Bits 40–47
            s.MO_Kickdown                    = (msg.buf[5] >> 0) & 0x01;  // bit 40
            s.MO_Status_CylinderSwitching_01 = (msg.buf[5] >> 1) & 0x01;  // bit 41
            s.MO_EKlKomLeiRed                = (msg.buf[5] >> 2) & 0x03;  // bits 42–43
            s.MO_Handshake_STH               = (msg.buf[5] >> 4) & 0x01;  // bit 44
            s.MO_BKV_LowPressureWarning      = (msg.buf[5] >> 5) & 0x01;  // bit 45
            s.MO_Release_Sailing             = (msg.buf[5] >> 6) & 0x01;  // bit 46
            s.MO_PTC_Status                  = (msg.buf[5] >> 7) & 0x01;  // start of bits 47–49

            // Bits 47–49 (3 bits) crosses byte boundary
            s.MO_PTC_Status = ((msg.buf[5] >> 7) & 0x01) |
                            ((msg.buf[6] & 0x03) << 1);

            // Bits 50–52
            s.MO_QBit_GearPosition          = (msg.buf[6] >> 2) & 0x01;  // bit 50
            s.MO_SignalSource_GearPosition  = (msg.buf[6] >> 3) & 0x01;  // bit 51
            s.MO_Remotestart_Operation      = (msg.buf[6] >> 4) & 0x01;  // bit 52

            return s;
        }

// 0x3C0 / 1700 - Klemmen_Status_01
    struct Klemmen_Status_01_t {
        uint8_t CHECKSUM;
        uint8_t COUNTER;
        bool    ZAS_Kl_S;
        bool    ZAS_Kl_15;
        bool    ZAS_Kl_X;
        bool    ZAS_Kl_50;
    };
    Klemmen_Status_01_t parse_Klemmen_Status_01(const CAN_message_t &msg) {
        Klemmen_Status_01_t s = {};

        // Bits 0–7
        s.CHECKSUM = msg.buf[0];

        // Bits 8–11
        s.COUNTER = msg.buf[1] & 0x0F;

        // Bits 16–19
        s.ZAS_Kl_S  = (msg.buf[2] >> 0) & 0x01;
        s.ZAS_Kl_15 = (msg.buf[2] >> 1) & 0x01;
        s.ZAS_Kl_X  = (msg.buf[2] >> 2) & 0x01;
        s.ZAS_Kl_50 = (msg.buf[2] >> 3) & 0x01;

        return s;
    }

// 0x3C7 / 967 - Motor_26
    struct Motor_26_t {
        uint8_t  MO_HYB_Status_HV_Ladung;
        bool     WIV_Anzeige_aktiv;
        bool     WIV_OilMin_Warn;
        bool     WIV_SensorError;
        bool     WIV_PrecariousSituation;
        float    WIV_OilLevel;
        uint8_t  MO_Zustand_HWP;
        bool     WIV_OilSystem_aktiv;
        bool     WIV_NotAtOperatingTemp;
        bool     WIV_Overfill_Warning;
        bool     WIV_Ongoing_Motor;
        bool     MO_HYB_Text_1;
        bool     MO_HYB_Text_2;
        bool     MO_HYB_Text_3;
        bool     MO_HYB_Text_4;
        uint8_t  MO_Text_Motorstart;
        bool     MO_HYB_Text_5;
        bool     MO_HYB_Text_6;
        bool     MO_HYB_Text_7;
        bool     MO_Text_Partikelfil_Reg;
        uint16_t WIV_OilQuanity;
        bool     MO_SystemLamp;
        bool     MO_OBD2_Lamp;
        bool     MO_HighBeams;
        bool     MO_Partikel_Lampe;
        bool     MO_WinterDrivingProgram;
        bool     WIV_OilLevel_not_Detected;
        bool     WIV_Refill_Indicator_On;
        bool     WIV_OverfillProtection_Deactivated;
        bool     WIV_Underfill_Warning;
        bool     MO_FuelCap_Lampe;
        bool     MO_Text_FuelCapWarning;
        bool     WIV_OilDrain_Warn_Motor;
    };
    Motor_26_t parse_Motor_26(const CAN_message_t &msg) {
        Motor_26_t s = {};

        // Bits 8–10
        s.MO_HYB_Status_HV_Ladung = msg.buf[1] & 0x07;

        // Bits 12–15
        s.WIV_Anzeige_aktiv      = (msg.buf[1] >> 4) & 0x01;
        s.WIV_OilMin_Warn        = (msg.buf[1] >> 5) & 0x01;
        s.WIV_SensorError        = (msg.buf[1] >> 6) & 0x01;
        s.WIV_PrecariousSituation = (msg.buf[1] >> 7) & 0x01;

        // Bits 16–19 (4 bits), factor 12.5, offset 0
        s.WIV_OilLevel = (msg.buf[2] & 0x0F) * 12.5f;

        // Bits 20–21
        s.MO_Zustand_HWP = (msg.buf[2] >> 4) & 0x03;

        // Bits 24–31
        s.WIV_OilSystem_aktiv   = (msg.buf[3] >> 0) & 0x01;
        s.WIV_NotAtOperatingTemp = (msg.buf[3] >> 1) & 0x01;
        s.WIV_Overfill_Warning  = (msg.buf[3] >> 2) & 0x01;
        s.WIV_Ongoing_Motor     = (msg.buf[3] >> 3) & 0x01;
        s.MO_HYB_Text_1         = (msg.buf[3] >> 4) & 0x01;
        s.MO_HYB_Text_2         = (msg.buf[3] >> 5) & 0x01;
        s.MO_HYB_Text_3         = (msg.buf[3] >> 6) & 0x01;
        s.MO_HYB_Text_4         = (msg.buf[3] >> 7) & 0x01;

        // Bits 32–39
        s.MO_Text_Motorstart = msg.buf[4] & 0x0F;
        s.MO_HYB_Text_5      = (msg.buf[4] >> 4) & 0x01;
        s.MO_HYB_Text_6      = (msg.buf[4] >> 5) & 0x01;
        s.MO_HYB_Text_7      = (msg.buf[4] >> 6) & 0x01;

        // Bit 41
        s.MO_Text_Partikelfil_Reg = (msg.buf[5] >> 1) & 0x01;

        // Bits 43–47 (5 bits), factor 125, offset 0
        s.WIV_OilQuanity = ((msg.buf[5] >> 3) & 0x1F) * 125;

        // Bits 48–55
        s.MO_SystemLamp                    = (msg.buf[6] >> 0) & 0x01;
        s.MO_OBD2_Lamp                     = (msg.buf[6] >> 1) & 0x01;
        s.MO_HighBeams                     = (msg.buf[6] >> 2) & 0x01;
        s.MO_Partikel_Lampe                = (msg.buf[6] >> 3) & 0x01;
        s.MO_WinterDrivingProgram          = (msg.buf[6] >> 4) & 0x01;
        s.WIV_OilLevel_not_Detected        = (msg.buf[6] >> 5) & 0x01;
        s.WIV_Refill_Indicator_On          = (msg.buf[6] >> 6) & 0x01;
        s.WIV_OverfillProtection_Deactivated = (msg.buf[6] >> 7) & 0x01;

        // Bits 56–63
        s.WIV_Underfill_Warning = (msg.buf[7] >> 0) & 0x01;
        s.MO_FuelCap_Lampe      = (msg.buf[7] >> 1) & 0x01;
        s.MO_Text_FuelCapWarning = (msg.buf[7] >> 2) & 0x01;
        s.WIV_OilDrain_Warn_Motor = (msg.buf[7] >> 4) & 0x01;

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


// 0x3D8 / 984 - RGS_VL_01 - Charisma Statuses
    struct RGS_VL_01_t {
        uint8_t RGS_VL_Texte;
        uint8_t RGS_VL_Charisma_FahrPr;
        uint8_t RGS_VL_Charisma_Status;
        bool    RGS_VL_aktiv;
        bool    RGS_VL_PC_SeatActuator;
        bool    RGS_VL_PC_SunroofActuator;
        bool    RGS_VL_PC_WindowActuator;
        bool    RGS_VL_PC_HazardLightsActuator;
        uint8_t RGS_VL_Precrash_Basis;
        uint8_t RGS_VL_Precrash_Front;
        uint8_t RGS_VL_Precrash_Rear;
    };
    RGS_VL_01_t parse_RGS_VL_01(const CAN_message_t &msg) {
        RGS_VL_01_t s = {};

        // Bits 12–13
        s.RGS_VL_Texte = (msg.buf[1] >> 4) & 0x03;

        // Bits 14–17 (crosses from buf[1] into buf[2])
        s.RGS_VL_Charisma_FahrPr = ((msg.buf[1] >> 6) & 0x03) |
                                ((msg.buf[2] & 0x03) << 2);

        // Bits 18–19
        s.RGS_VL_Charisma_Status = (msg.buf[2] >> 2) & 0x03;

        // Bit 21
        s.RGS_VL_aktiv = (msg.buf[2] >> 5) & 0x01;

        // Bits 25–28
        s.RGS_VL_PC_SeatActuator          = (msg.buf[3] >> 1) & 0x01;
        s.RGS_VL_PC_SunroofActuator   = (msg.buf[3] >> 2) & 0x01;
        s.RGS_VL_PC_WindowActuator       = (msg.buf[3] >> 3) & 0x01;
        s.RGS_VL_PC_HazardLightsActuator    = (msg.buf[3] >> 4) & 0x01;

        // Bits 32–39
        s.RGS_VL_Precrash_Basis = msg.buf[4];

        // Bits 40–47
        s.RGS_VL_Precrash_Front = msg.buf[5];

        // Bits 48–55
        s.RGS_VL_Precrash_Rear = msg.buf[6];

        return s;
    }


// 0x3DD / 989 - Gateway_74 - Access Control and TipTronic Status/ Paddles
    struct Gateway_74_t {
        bool    LH_EPS_01_alt;
        bool    Kessy_04_alt;
        bool    LIN_2_alt;
        bool    MFG_01_alt;
        bool    GW_74_va_14;
        bool    Klima_02_alt;
        bool    Parkhilfe_01_alt;
        bool    ELV_01_alt;
        uint8_t KY_StartStopp_Info;
        uint8_t PH_StartStopp_Info;
        uint8_t EPS_Lenkerposition;
        bool    ELV_Anf_Klemme_50;
        uint8_t MF_StartStopp_Info;
        float   KL_FanVoltage_Target;
        uint8_t KL_RecirculationFlap_Status;
        bool    MFL_Tip_PaddleDOWN;
        bool    MFL_Tip_PaddleUP;
        bool    LS_Tiptronic_Fehler;
    };
    Gateway_74_t parse_Gateway_74(const CAN_message_t &msg) {
        Gateway_74_t s = {};

        // Bits 0–7
        s.LH_EPS_01_alt    = (msg.buf[0] >> 0) & 0x01;
        s.Kessy_04_alt     = (msg.buf[0] >> 1) & 0x01;
        s.LIN_2_alt        = (msg.buf[0] >> 2) & 0x01;
        s.MFG_01_alt       = (msg.buf[0] >> 3) & 0x01;
        s.GW_74_va_14      = (msg.buf[0] >> 4) & 0x01;
        s.Klima_02_alt     = (msg.buf[0] >> 5) & 0x01;
        s.Parkhilfe_01_alt = (msg.buf[0] >> 6) & 0x01;
        s.ELV_01_alt       = (msg.buf[0] >> 7) & 0x01;

        // Bits 16–23
        s.KY_StartStopp_Info = (msg.buf[2] >> 0) & 0x03;  // bits 16–17
        s.PH_StartStopp_Info = (msg.buf[2] >> 2) & 0x03;  // bits 18–19
        s.EPS_Lenkerposition = (msg.buf[2] >> 4) & 0x03;  // bits 20–21
        s.ELV_Anf_Klemme_50  = (msg.buf[2] >> 6) & 0x01;  // bit 22

        // Bits 25–26
        s.MF_StartStopp_Info = (msg.buf[3] >> 1) & 0x03;

        // Bits 40–47 (8 bits), factor 0.05, offset 0.5
        {
            uint8_t raw = msg.buf[5];
            s.KL_FanVoltage_Target = (raw * 0.05f) + 0.5f;
        }

        // Bits 48–51
        s.KL_RecirculationFlap_Status = msg.buf[6] & 0x0F;

        // Bits 56–58
        s.MFL_Tip_PaddleDOWN        = (msg.buf[7] >> 0) & 0x01;
        s.MFL_Tip_PaddleUP          = (msg.buf[7] >> 1) & 0x01;
        s.LS_Tiptronic_Fehler = (msg.buf[7] >> 2) & 0x01;

        return s;
    }


// 0x488 / 1160 - Motor_06 - Might not be used but this has intersting tcu implications
    struct Motor_6_t {
        float   Istmoment_f_r_Getriebe;
        float   ElevationInfo__Motor_6_;
        float   GRA_TargetAcceleration;
        float   Ruckmeldung_Momenten;
        uint8_t Free_Motor_6_3;
        bool    GRA_BrakeIntervention_Release;
        bool    OilTemp_Protection;
        uint8_t Free_Motor_6_4;
    };
    Motor_6_t parse_Motor_6(const CAN_message_t &msg) {
        Motor_6_t s = {};

        // Bits 16–23 (8 bits), factor 0.39
        s.Istmoment_f_r_Getriebe = msg.buf[2] * 0.39f;

        // Bits 24–31 (8 bits), factor 0.00787
        s.ElevationInfo__Motor_6_ = msg.buf[3] * 0.00787f;

        // Bits 32–39 (8 bits), factor 0.024, offset -3.984
        s.GRA_TargetAcceleration = (msg.buf[4] * 0.024f) - 3.984f;

        // Bits 40–47 (8 bits), factor 0.39
        s.Ruckmeldung_Momenten = msg.buf[5] * 0.39f;

        // Bits 48–55 (8 bits)
        s.Free_Motor_6_3 = msg.buf[6];

        // Bit 56
        s.GRA_BrakeIntervention_Release = (msg.buf[7] >> 0) & 0x01;

        // Bit 57
        s.OilTemp_Protection = (msg.buf[7] >> 1) & 0x01;

        // Bits 58–59
        s.Free_Motor_6_4 = (msg.buf[7] >> 2) & 0x03;

        return s;
    }


// 0x640 / 1600 - Motor_07
    struct Motor_07_t {
    bool     MO7_QBit_IntakeAir_Temp;
    bool     MO7_QBit_Oil_Temp;
    bool     MO7_QBit_Coolant_Temp;
    bool     MO7_ActuatorTest_SoundActuator;
    bool     MO7_HYB_Fehler_HV_Netz;
    bool     MO7_Active_GearboxHeating;
    uint8_t  MO7_ShutoffValve_Open;
    float    MO7_IntakeAir_Temp;
    int16_t  MO7_Oil_Temp;
    float    MO7_Coolant_Temp;
    float    MO7_AltitudeInfo;
    bool     MO7_MapBased_Control;
    uint8_t  MO7_Versionsinfo;
    bool     MO7_Gearbox_Cooling;
    float    MO7_Moment_Intertia_02;
    uint8_t  MO7_HeatingPumpControl;
    bool     MO7_StartingVoltage;
    uint16_t MO7_Overrun_HeatPumpTime;
};
    Motor_07_t parse_Motor_07(const CAN_message_t &msg) {
    Motor_07_t s = {};

    // Bits 0–7
    s.MO7_QBit_IntakeAir_Temp        = (msg.buf[0] >> 0) & 0x01;
    s.MO7_QBit_Oil_Temp              = (msg.buf[0] >> 1) & 0x01;
    s.MO7_QBit_Coolant_Temp          = (msg.buf[0] >> 2) & 0x01;
    s.MO7_ActuatorTest_SoundActuator = (msg.buf[0] >> 3) & 0x01;
    s.MO7_HYB_Fehler_HV_Netz         = (msg.buf[0] >> 4) & 0x01;
    s.MO7_Active_GearboxHeating      = (msg.buf[0] >> 5) & 0x01;
    s.MO7_ShutoffValve_Open          = (msg.buf[0] >> 6) & 0x03;

    // Bits 8–15 (8 bits), factor 0.75, offset -48
    s.MO7_IntakeAir_Temp = (msg.buf[1] * 0.75f) - 48.0f;

    // Bits 16–23 (8 bits), factor 1, offset -60
    s.MO7_Oil_Temp = (int16_t)msg.buf[2] - 60;

    // Bits 24–31 (8 bits), factor 0.75, offset -48
    s.MO7_Coolant_Temp = (msg.buf[3] * 0.75f) - 48.0f;

    // Bits 32–39 (8 bits), factor 0.00781, offset 0
    s.MO7_AltitudeInfo = msg.buf[4] * 0.00781f;

    // Bit 40
    s.MO7_MapBased_Control = (msg.buf[5] >> 0) & 0x01;

    // Bits 41–46
    s.MO7_Versionsinfo = (msg.buf[5] >> 1) & 0x3F;

    // Bit 47
    s.MO7_Gearbox_Cooling = (msg.buf[5] >> 7) & 0x01;

    // Bits 48–52 (5 bits), factor 0.01
    {
        uint8_t raw = msg.buf[6] & 0x1F;
        s.MO7_Moment_Intertia_02 = raw * 0.01f;
    }

    // Bits 53–56 (4 bits), crosses byte boundary
    {
        uint8_t raw = ((msg.buf[6] >> 5) & 0x07) |
                      ((msg.buf[7] & 0x01) << 3);
        s.MO7_HeatingPumpControl = raw * 10;
    }

    // Bit 57
    s.MO7_StartingVoltage = (msg.buf[7] >> 1) & 0x01;

    // Bits 58–63 (6 bits), factor 15
    {
        uint8_t raw = (msg.buf[7] >> 2) & 0x3F;
        s.MO7_Overrun_HeatPumpTime = raw * 15;
    }

    return s;
}


// 0x647 / 1607 - Motor_09
    struct Motor_09_t {
        float    MO9_ITM_Coolant_Temp;
        uint8_t  MO9_E85_Sensor;
        uint8_t  SCR_NumberOfEngStarts;
        uint16_t SCR_Reach;
        bool     SCR_WarningLevel_1;
        bool     SCR_WarningLevel_2;
        uint8_t  SCR_Text;
        uint8_t  SCR_Acoustic;
        bool     MO9_FuelFilter_Water;
        bool     SCR_SystemError;
        uint8_t  SCR_Inducement_Strategy;
        uint16_t MO9_CO2_Factor;
    };
    Motor_09_t parse_Motor_09(const CAN_message_t &msg) {
        Motor_09_t s = {};

        // Bits 0–7 (8 bits), factor 0.75, offset -48
        s.MO9_ITM_Coolant_Temp = (msg.buf[0] * 0.75f) - 48.0f;

        // Bits 8–11 (4 bits), factor 10, offset 0
        s.MO9_E85_Sensor = (msg.buf[1] & 0x0F) * 10;

        // Bits 12–15
        s.SCR_NumberOfEngStarts = (msg.buf[1] >> 4) & 0x0F;

        // Bits 16–30 (15 bits)
        {
            uint16_t raw = ((uint16_t)msg.buf[2]) |
                        (((uint16_t)(msg.buf[3] & 0x7F)) << 8);
            raw &= 0x7FFF;
            s.SCR_Reach = raw;
        }

        // Bit 32
        s.SCR_WarningLevel_1 = (msg.buf[4] >> 0) & 0x01;

        // Bit 33
        s.SCR_WarningLevel_2 = (msg.buf[4] >> 1) & 0x01;

        // Bits 34–36
        s.SCR_Text = (msg.buf[4] >> 2) & 0x07;

        // Bits 37–38
        s.SCR_Acoustic = (msg.buf[4] >> 5) & 0x03;

        // Bit 40
        s.MO9_FuelFilter_Water = (msg.buf[5] >> 0) & 0x01;

        // Bit 41
        s.SCR_SystemError = (msg.buf[5] >> 1) & 0x01;

        // Bits 42–43
        s.SCR_Inducement_Strategy = (msg.buf[5] >> 2) & 0x03;

        // Bits 44–55 (12 bits)
        {
            uint16_t raw = ((uint16_t)(msg.buf[5] >> 4)) |
                        (((uint16_t)msg.buf[6]) << 4);
            raw &= 0x0FFF;
            s.MO9_CO2_Factor = raw;
        }

        return s;
    }


// 0x65A / 1626 - BCM_01
    struct BCM_01_t {
        bool    BCM_Brake_Pad_Sensor;
        bool    BCM_Brake_Fluid_Sensor;
        bool    BCM1_Light_Warning;
        bool    BCM_WasherFluid_Sensor;
        bool    BCM_Coolant_Sensor;
        bool    BCM1_Kl_15_HW_erkannt;
        bool    BCM_IceOffRoad_Button;
        bool    ZZH_UpperLimit_Position;
        bool    ZZH_LowerLimit_Position;
        bool    ZZH_Implausible_Position;
        bool    BCM2_EZS_Pressed;
        bool    BCM2_SST_Pressed;
        bool    BCM_Hybrid_StartStopp_Button;
        bool    BCM1_HazardLightsSwitch;
        bool    BCM1_Valet_Parking_Button;
        bool    BCM_Remotestart_Operation;
        bool    BCM1_HSK_Button;
        bool    BCM1_RearSunShade_Button;
        bool    BCM1_ReverseLight_Switch;
        bool    BCM1_MH_Switch;
        bool    BCM1_MH_WIV_Switch;
        bool    BCM_Eco_Charisma_Button;
        uint8_t BCM_Thermomanagement;
        bool    BCM_Thermomanagement_Error;
        bool    BCM_Thermomanagement_Valid;
        uint8_t BCM1_LightWarning_Text;
    };
    BCM_01_t parse_BCM_01(const CAN_message_t &msg) {
        BCM_01_t s = {};

        // Bits 12–15
        s.BCM_Brake_Pad_Sensor   = (msg.buf[1] >> 4) & 0x01;
        s.BCM_Brake_Fluid_Sensor = (msg.buf[1] >> 5) & 0x01;
        s.BCM1_Light_Warning     = (msg.buf[1] >> 6) & 0x01;
        s.BCM_WasherFluid_Sensor = (msg.buf[1] >> 7) & 0x01;

        // Bits 16–23
        s.BCM_Coolant_Sensor        = (msg.buf[2] >> 0) & 0x01;
        s.BCM1_Kl_15_HW_erkannt     = (msg.buf[2] >> 1) & 0x01;
        s.BCM_IceOffRoad_Button     = (msg.buf[2] >> 2) & 0x01;
        s.ZZH_UpperLimit_Position   = (msg.buf[2] >> 3) & 0x01;
        s.ZZH_LowerLimit_Position   = (msg.buf[2] >> 4) & 0x01;
        s.ZZH_Implausible_Position  = (msg.buf[2] >> 5) & 0x01;
        s.BCM2_EZS_Pressed          = (msg.buf[2] >> 6) & 0x01;
        s.BCM2_SST_Pressed          = (msg.buf[2] >> 7) & 0x01;

        // Bits 24–31
        s.BCM_Hybrid_StartStopp_Button = (msg.buf[3] >> 0) & 0x01;
        s.BCM1_HazardLightsSwitch      = (msg.buf[3] >> 1) & 0x01;
        s.BCM1_Valet_Parking_Button    = (msg.buf[3] >> 2) & 0x01;
        s.BCM_Remotestart_Operation    = (msg.buf[3] >> 3) & 0x01;
        s.BCM1_HSK_Button              = (msg.buf[3] >> 4) & 0x01;
        s.BCM1_RearSunShade_Button     = (msg.buf[3] >> 5) & 0x01;
        s.BCM1_ReverseLight_Switch     = (msg.buf[3] >> 6) & 0x01;
        s.BCM1_MH_Switch               = (msg.buf[3] >> 7) & 0x01;

        // Bits 32–39
        s.BCM1_MH_WIV_Switch          = (msg.buf[4] >> 0) & 0x01;
        s.BCM_Eco_Charisma_Button     = (msg.buf[4] >> 1) & 0x01;
        s.BCM_Thermomanagement        = (msg.buf[4] >> 2) & 0x03;
        s.BCM_Thermomanagement_Error  = (msg.buf[4] >> 4) & 0x01;
        s.BCM_Thermomanagement_Valid  = (msg.buf[4] >> 5) & 0x01;
        s.BCM1_LightWarning_Text      = (msg.buf[4] >> 6) & 0x03;

        return s;
    }

// 0x65D / 1629 - ESP_20
    struct ESP_20_t {
        uint8_t  CHECKSUM;
        uint8_t  COUNTER;
        uint8_t  BR_Systemart;
        uint8_t  ESP_Voltage_Req_02;
        uint8_t  ESP_Number_of_Teeth;
        uint8_t  ESP_Charisma_FahrPr;
        uint8_t  ESP_Charisma_Status;
        bool     ESP_Wiederstart_Anz_01;
        bool     ESP_Wiederstart_Anz_02;
        bool     ESP_Wiederstart_Anz_03;
        bool     ESP_Wiederstart_Anz_04;
        bool     ESP_Stoppverbot_Anz_01;
        bool     ESP_Stoppverbot_Anz_02;
        bool     ESP_Stoppverbot_Anz_03;
        bool     ESP_Stoppverbot_Anz_04;
        bool     ESP_Stoppverbot_Anz_05;
        bool     ESP_Stoppverbot_Anz_06;
        bool     ESP_Stoppverbot_Anz_07;
        bool     ESP_Stoppverbot_Anz_Std;
        uint8_t  ESP_RoofRail_Sensor;
        bool     ESP_No_Stopping_Sign_08;
        uint8_t  HDC_Charisma_FahrPr;
        uint8_t  HDC_Charisma_Status;
        bool     BR_QBit_Tire_Circumference;
        uint16_t BR_Tire_Circumference;
    };
    ESP_20_t parse_ESP_20(const CAN_message_t &msg) {
    ESP_20_t s = {};

    // Bits 0–7
    s.CHECKSUM = msg.buf[0];

    // Bits 8–11
    s.COUNTER = msg.buf[1] & 0x0F;

    // Bits 12–13
    s.BR_Systemart = (msg.buf[1] >> 4) & 0x03;

    // Bits 14–15
    s.ESP_Voltage_Req_02 = (msg.buf[1] >> 6) & 0x03;

    // Bits 16–23
    s.ESP_Number_of_Teeth = msg.buf[2];

    // Bits 24–27
    s.ESP_Charisma_FahrPr = msg.buf[3] & 0x0F;

    // Bits 28–29
    s.ESP_Charisma_Status = (msg.buf[3] >> 4) & 0x03;

    // Bit 30
    s.ESP_Wiederstart_Anz_01 = (msg.buf[3] >> 6) & 0x01;

    // Bit 31
    s.ESP_Wiederstart_Anz_02 = (msg.buf[3] >> 7) & 0x01;

    // Bits 32–39
    s.ESP_Wiederstart_Anz_03 = (msg.buf[4] >> 0) & 0x01;
    s.ESP_Wiederstart_Anz_04 = (msg.buf[4] >> 1) & 0x01;
    s.ESP_Stoppverbot_Anz_01 = (msg.buf[4] >> 2) & 0x01;
    s.ESP_Stoppverbot_Anz_02 = (msg.buf[4] >> 3) & 0x01;
    s.ESP_Stoppverbot_Anz_03 = (msg.buf[4] >> 4) & 0x01;
    s.ESP_Stoppverbot_Anz_04 = (msg.buf[4] >> 5) & 0x01;
    s.ESP_Stoppverbot_Anz_05 = (msg.buf[4] >> 6) & 0x01;
    s.ESP_Stoppverbot_Anz_06 = (msg.buf[4] >> 7) & 0x01;

    // Bits 40–47
    s.ESP_Stoppverbot_Anz_07 = (msg.buf[5] >> 0) & 0x01;
    s.ESP_Stoppverbot_Anz_Std = (msg.buf[5] >> 1) & 0x01;
    s.ESP_RoofRail_Sensor = (msg.buf[5] >> 2) & 0x03;
    s.ESP_No_Stopping_Sign_08 = (msg.buf[5] >> 4) & 0x01;
    s.HDC_Charisma_FahrPr = (msg.buf[5] >> 5) & 0x07;

    // HDC_Charisma_FahrPr is 4 bits starting at bit 45, so it crosses into buf[6]
    s.HDC_Charisma_FahrPr = ((msg.buf[5] >> 5) & 0x07) |
                            ((msg.buf[6] & 0x01) << 3);

    // Bits 49–50
    s.HDC_Charisma_Status = (msg.buf[6] >> 1) & 0x03;

    // Bit 51
    s.BR_QBit_Tire_Circumference = (msg.buf[6] >> 3) & 0x01;

    // Bits 52–63
    {
        uint16_t raw = ((uint16_t)(msg.buf[6] >> 4)) |
                       (((uint16_t)msg.buf[7]) << 4);
        raw &= 0x0FFF;
        s.BR_Tire_Circumference = raw;
    }

    return s;
}

// 0x670 / 1648 - Motor_18  Instrument Cluster - RPM and Shift Warning Lights 
    struct Motor_18_t {
        uint8_t MO18_Hybrid_StartStopp_LED;
        uint8_t MO18_Ice_Offroad_LED;
        uint8_t MO18_NumberOfCylinders;
        uint8_t MO18_CylinderShutOff_Texte;
        uint8_t MO18_E85_BS_Texte;
        bool    MO18_RPM_ShiftWarningLight;
        uint16_t MO18_UpperSpeedLimit;
    };
    Motor_18_t parse_Motor_18(const CAN_message_t &msg) {
        Motor_18_t s = {};

        // Bits 43–44
        s.MO18_Hybrid_StartStopp_LED = (msg.buf[5] >> 3) & 0x03;

        // Bits 45–46
        s.MO18_Ice_Offroad_LED = (msg.buf[5] >> 5) & 0x03;

        // Bits 47–49 (crosses byte boundary)
        s.MO18_NumberOfCylinders = ((msg.buf[5] >> 7) & 0x01) |
                                ((msg.buf[6] & 0x03) << 1);

        // Bits 50–51
        s.MO18_CylinderShutOff_Texte = (msg.buf[6] >> 2) & 0x03;

        // Bits 52–54
        s.MO18_E85_BS_Texte = (msg.buf[6] >> 4) & 0x07;

        // Bit 55
        s.MO18_RPM_ShiftWarningLight = (msg.buf[6] >> 7) & 0x01;

        // Bits 56–63 (8 bits), factor 50
        s.MO18_UpperSpeedLimit = msg.buf[7] * 50;

        return s;
    }


// 0x6B4 / 1716 - VIN_01
    struct VIN_01_t {
        uint8_t VIN_01_MUX;

        // m0
        uint8_t KS_Geheimnis_1;
        uint8_t KS_Geheimnis_2;
        uint8_t KS_Geheimnis_3;
        uint8_t KS_Geheimnis_4;
        uint8_t VIN_1;
        uint8_t VIN_2;
        uint8_t VIN_3;

        // m1
        uint8_t VIN_4;
        uint8_t VIN_5;
        uint8_t VIN_6;
        uint8_t VIN_7;
        uint8_t VIN_8;
        uint8_t VIN_9;
        uint8_t VIN_10;

        // m2
        uint8_t VIN_11;
        uint8_t VIN_12;
        uint8_t VIN_13;
        uint8_t VIN_14;
        uint8_t VIN_15;
        uint8_t VIN_16;
        uint8_t VIN_17;
    };
    VIN_01_t parse_VIN_01(const CAN_message_t &msg) {
        VIN_01_t s = {};

        // Bits 0–1
        s.VIN_01_MUX = msg.buf[0] & 0x03;

        switch (s.VIN_01_MUX) {
            case 0:
                // m0
                s.KS_Geheimnis_1 = msg.buf[1];
                s.KS_Geheimnis_2 = msg.buf[2];
                s.KS_Geheimnis_3 = msg.buf[3];
                s.KS_Geheimnis_4 = msg.buf[4];
                s.VIN_1          = msg.buf[5];
                s.VIN_2          = msg.buf[6];
                s.VIN_3          = msg.buf[7];
                break;

            case 1:
                // m1
                s.VIN_4  = msg.buf[1];
                s.VIN_5  = msg.buf[2];
                s.VIN_6  = msg.buf[3];
                s.VIN_7  = msg.buf[4];
                s.VIN_8  = msg.buf[5];
                s.VIN_9  = msg.buf[6];
                s.VIN_10 = msg.buf[7];
                break;

            case 2:
                // m2
                s.VIN_11 = msg.buf[1];
                s.VIN_12 = msg.buf[2];
                s.VIN_13 = msg.buf[3];
                s.VIN_14 = msg.buf[4];
                s.VIN_15 = msg.buf[5];
                s.VIN_16 = msg.buf[6];
                s.VIN_17 = msg.buf[7];
                break;

            default:
                // mux value 3 is not defined in the DBC you pasted
                break;
        }

        return s;
    }


// 0x6B8 / 1720 - Kombi_03
    struct Kombi_03_t {
        uint16_t KBI_Reifenumfang;
        bool     KBI_Variante_USA;
        bool     KBI_Variante;
        bool     KBI_BCmE_aktiv;
        bool     KBI_Sparhinweis_quittiert;
        uint8_t  KBI_Tankfuellstand_Prozen_TankLevelPercent;
        bool     KBI_Nachtanken_erkannt;
        float    KBI_Tankinhalt_hochaufl;
        float    KBI_Max_Tankinhalt;
        uint16_t KBI_Reifenumfang_Sekundaer;
    };
    Kombi_03_t parse_Kombi_03(const CAN_message_t &msg) {
        Kombi_03_t s = {};

        // Bits 0–11 (12 bits), factor 1, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[0]) |
                        ((((uint16_t)msg.buf[1]) & 0x0F) << 8);
            s.KBI_Reifenumfang = raw;
        }

        // Bit 12
        s.KBI_Variante_USA = (msg.buf[1] >> 4) & 0x01;

        // Bit 13
        s.KBI_Variante = (msg.buf[1] >> 5) & 0x01;

        // Bit 16
        s.KBI_BCmE_aktiv = (msg.buf[2] >> 0) & 0x01;

        // Bit 17
        s.KBI_Sparhinweis_quittiert = (msg.buf[2] >> 1) & 0x01;

        // Bits 18–24 (7 bits), factor 1, offset 0
        {
            uint8_t raw = (msg.buf[2] >> 2) & 0x7F;
            s.KBI_Tankfuellstand_Prozen_TankLevelPercent = raw;
        }

        // Bit 25
        s.KBI_Nachtanken_erkannt = (msg.buf[3] >> 1) & 0x01;

        // Bits 26–39 (14 bits), factor 0.01, offset 0
        {
            uint16_t raw = ((uint16_t)(msg.buf[3] >> 2)) |
                        (((uint16_t)msg.buf[4]) << 6) |
                        ((((uint16_t)msg.buf[5]) & 0x0F) << 14);
            raw &= 0x3FFF;
            s.KBI_Tankinhalt_hochaufl = raw * 0.01f;
        }

        // Bits 40–47 (8 bits), factor 0.5, offset 0
        {
            uint8_t raw = msg.buf[5];
            s.KBI_Max_Tankinhalt = raw * 0.5f;
        }

        // Bits 48–59 (12 bits), factor 1, offset 0
        {
            uint16_t raw = ((uint16_t)msg.buf[6]) |
                        ((((uint16_t)msg.buf[7]) & 0x0F) << 8);
            s.KBI_Reifenumfang_Sekundaer = raw;
        }

        return s;
    }













// 0x65D / 1629 - ESP_20

